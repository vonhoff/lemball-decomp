#!/usr/bin/env python3
"""Fail on decomp smells agents keep reintroducing.

  python tools/smell.py
  python tools/smell.py --annot
  python tools/smell.py --selftest
  python tools/smell.py src/Visos/Graphics

Default errors:
  vbptr-walk     *(int*)(*(int*)(obj + 0x40) + 4) virtual-base poke
  offset-poke    pointer-cast +/- field offset, including Ghidra
                 ((int*)((short*)p + 0x16))[0] and *(T*)(obj + N)
  expr-char-offset  (char*)expr +/- 0xN including -> chains and
                 (Ai*) ((char*) p->m_process - 0x10)
  mi-dtor-poke   ((T*) ((char*) this + 0x40))->~T() manual MI teardown
  this-adjust    (char*)this - N mixin poke

Does not flag every `+ 0x` (resource ids, timers, LCG). Those are not field pokes.

--annot also fails 68K-only functions (missing FUNCTION/STUB/GLOBAL/...).
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"

RECCMP_MARK = re.compile(r"^\s*//\s*(FUNCTION|STUB|GLOBAL|LIBRARY|TEMPLATE|SYNTHETIC)\s*:")
K68_MARK = re.compile(r"^\s*//\s*68K\s+")
VBPTR_WALK = re.compile(
	r"\*\(\s*int\s*\*\s*\)\s*\(\s*\*\(\s*int\s*\*\s*\)\s*\(\s*[^;]{1,60}?\+\s*0x40\s*\)\s*\+\s*4\s*\)"
)
THIS_ADJUST = re.compile(r"\(\s*char\s*\*\s*\)\s*this\s*-\s*(?:0x[0-9A-Fa-f]+|\d+)\b")
EXPR_CHAR_OFFSET = re.compile(
	r"\(\s*char\s*\*\s*\)"
	r"(?:\(\s*)?"
	r"\s*"
	r"(?P<expr>this|[A-Za-z_][\w]*(?:\s*(?:->|\.)\s*[A-Za-z_][\w]*|\[[^\]]+\])*)"
	r"\s+[+-]\s+(?:0x[0-9A-Fa-f]+|\d+)\b"
)
MI_DTOR_POKE = re.compile(
	r"\(\s*[A-Za-z_][\w]*\s*\*\s*\)\s*"
	r"\(\s*\(\s*char\s*\*\s*\)[^;]{1,120}?~\s*[A-Za-z_][\w]*\s*\("
)
PTR_CAST = re.compile(
	r"\(\s*(?:unsigned\s+|signed\s+)?(?:char|short|int|long|void|__int16|__int32)\s*\*\s*\)"
)
CAST_THEN_ARITH = re.compile(
	PTR_CAST.pattern
	+ r"\s*(?:\([^;]{1,80}?\)|[A-Za-z_][\w.]*(?:\[[^\]]{0,40}\])?)"
	+ r"(?:\s*[+-]\s*[A-Za-z_][\w]*)*"
	+ r"\s*[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
)
CAST_PAREN_ARITH = re.compile(
	PTR_CAST.pattern + r"\s*\(\s*[^;]{1,80}?\s*[+-]\s*(?:0x[0-9A-Fa-f]+|(?!1\b)\d+)\s*\)"
)
NAKED_DATA_OFFSET = re.compile(
	r"(?:->|\.)(?:m_data|m_buffer)\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
	r"|(?<![\w.])(?:m_data|m_buffer)\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
)
CHAR_VAR_OFFSET = re.compile(
	r"\(\s*char\s*\*\s*\)\s*(?P<expr>this|[A-Za-z_][\w]*)\s*[+-]\s*(?!0x)(?P<off>[A-Za-z_][\w]*)"
)
METHOD_DEF = re.compile(r"^[A-Za-z_][\w:]*::~?[A-Za-z_][\w]*\s*\(")
FREE_DEF = re.compile(r"^(?:static\s+)?(?:[A-Za-z_][\w:*&]*\s+)+\w+\s*\(")
BUFFER_OK = re.compile(r"m_numberBuffer|Bits\b|sz[A-Z]|\bp_bits\b")
SKIP_LEAD = {"if", "while", "for", "switch", "return", "else", "case", "catch", "extern"}
OFFSET_POKE_SAMPLES = (
	("m_projectileEndY = ((int*) ((short*) offsets + 0x16))[0] + m_junctions[i].m_y;", True),
	("rawLength = *(unsigned int*) (p_patch + 0x38);", True),
	("colorCount = *(int*) ((unsigned char*) p_palette + 0x48) - 10;", True),
	("x = *(short*) ((unsigned char*) p_point + 4);", True),
	("position.m_y = (short) (*(int*) (offsets + 2) + m_gunY);", True),
	("m_projectileX = offsets[6] + m_selectionStartX;", False),
	("m_writeCursor = write + sizeof(Message);", False),
	("musicId = m_randomMusicIndex + 0x239;", False),
	("*(int*) &ownerList[2] = *(int*) &ownerList[2] - 1;", False),
	("if (handle == (void*) -1) {", False),
	("m_name = (char*) &entry[2];", False),
	("p_headerCursor = (unsigned char*) &entry[2] + m_headerSkip;", False),
	("message->Set((unsigned char*) (header + 1));", False),
	("id = header->m_messageId;", False),
	("((UserActionMessage*) m_userActionMessage)->Set(packet->m_data + 0x10);", True),
	("*(unsigned short*) (m_buffer + 0x0a) = 0;", True),
	("storage = (unsigned char*) operator new(m_payloadCapacity + sizeof(BasePacketHeader));", False),
	("((PvBackBuffSurface*) ((char*) this + 0x94))->~PvBackBuffSurface();", True),
	("((PvSurface*) ((char*) slot->m_surface + 0x55c))->~PvSurface();", True),
	("ai = (Ai*) ((char*) m_game->m_process - 0x10);", True),
	("m_activeProcess = (char*) m_game->m_process - 0x10;", True),
	("ai = (Ai*) m_game->m_process;", False),
	("dst = (char*) m_numberBuffer + 0x21 + signLen;", False),
	("((GunButtons*) this)->DrawBackBuffer();", False),
)


def strip_line_comment(line: str) -> str:
	in_str = False
	out = []
	i = 0
	while i < len(line):
		ch = line[i]
		if ch == '"' and (i == 0 or line[i - 1] != "\\"):
			in_str = not in_str
			out.append(ch)
			i += 1
			continue
		if not in_str and ch == "/" and i + 1 < len(line) and line[i + 1] == "/":
			break
		out.append(ch)
		i += 1
	return "".join(out)


def is_func_def(stripped: str) -> bool:
	if not stripped or stripped.startswith(("#", "//", ":", "*", "}")):
		return False
	if stripped.endswith(";") or stripped.endswith(","):
		return False
	lead = stripped.split(None, 1)
	if lead and lead[0] in SKIP_LEAD:
		return False
	if METHOD_DEF.match(stripped):
		return True
	if "::" in stripped.split("(", 1)[0]:
		return False
	return bool(FREE_DEF.match(stripped))


def body_is_empty(lines: list[str], index: int) -> bool:
	i = index
	saw_open = False
	while i < len(lines) and i < index + 6:
		text = lines[i].strip()
		if "{" in text:
			saw_open = True
			after = text.split("{", 1)[1].strip()
			if after.startswith("}"):
				return True
			if after:
				return False
		elif saw_open:
			if text.startswith("}"):
				return True
			if text:
				return False
		i += 1
	return False


def preceding_block(lines: list[str], index: int) -> list[str]:
	block = []
	i = index - 1
	while i >= 0:
		raw = lines[i].strip()
		if raw == "":
			if block:
				break
			i -= 1
			continue
		if raw.startswith("//"):
			block.append(raw)
			i -= 1
			continue
		break
	block.reverse()
	return block


def allowed_expr(expr: str) -> bool:
	return bool(BUFFER_OK.search(expr))


def expr_char_offset_poke(code: str, match: re.Match[str]) -> bool:
	if allowed_expr(match.group("expr")):
		return False
	return True


def is_offset_poke(code: str) -> bool:
	if allowed_expr(code):
		return False
	if MI_DTOR_POKE.search(code):
		return True
	if CAST_THEN_ARITH.search(code) or CAST_PAREN_ARITH.search(code) or NAKED_DATA_OFFSET.search(code):
		return True
	for match in EXPR_CHAR_OFFSET.finditer(code):
		if expr_char_offset_poke(code, match):
			return True
	return False


def scan_file(path: Path, strict_annot: bool) -> list[str]:
	lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
	hits: list[str] = []
	rel = path.as_posix()
	if path.suffix.lower() in {".cpp", ".h", ".c"}:
		for lineno, raw in enumerate(lines, 1):
			code = strip_line_comment(raw)
			if VBPTR_WALK.search(code):
				hits.append("%s:%d: vbptr-walk" % (rel, lineno))
			if THIS_ADJUST.search(code):
				hits.append("%s:%d: this-adjust-poke" % (rel, lineno))
			poked = False
			for match in EXPR_CHAR_OFFSET.finditer(code):
				if expr_char_offset_poke(code, match):
					hits.append("%s:%d: expr-char-offset %s" % (rel, lineno, match.group(0).strip()[:100]))
					poked = True
					break
			if MI_DTOR_POKE.search(code):
				hits.append("%s:%d: mi-dtor-poke %s" % (rel, lineno, code.strip()[:100]))
				poked = True
			if not poked and is_offset_poke(code):
				hits.append("%s:%d: offset-poke %s" % (rel, lineno, code.strip()[:100]))
			for match in CHAR_VAR_OFFSET.finditer(code):
				if allowed_expr(match.group("expr")):
					continue
				if match.group("off") == "sizeof":
					continue
				hits.append("%s:%d: offset-poke %s" % (rel, lineno, match.group(0).strip()))
	if path.suffix.lower() != ".cpp":
		return hits
	for i, raw in enumerate(lines):
		stripped = raw.strip()
		if not is_func_def(stripped):
			continue
		prev = preceding_block(lines, i)
		has_reccmp = any(RECCMP_MARK.match(line) for line in prev)
		has_68k = any(K68_MARK.match(line) for line in prev)
		if has_reccmp:
			continue
		if has_68k and not strict_annot:
			continue
		if not has_68k and body_is_empty(lines, i) and not strict_annot:
			continue
		kind = "incomplete-annotation" if has_68k else "no-annotation"
		hits.append("%s:%d: %s %s" % (rel, i + 1, kind, stripped[:90]))
	return hits


def iter_sources(root: Path):
	for path in sorted(root.rglob("*")):
		if path.suffix.lower() in {".cpp", ".h", ".c"}:
			yield path


def main(argv: list[str]) -> int:
	parser = argparse.ArgumentParser(description="Decomp smell gate")
	parser.add_argument("--annot", action="store_true", help="fail 68K-only and empty unmarked functions")
	parser.add_argument("--selftest", action="store_true", help="check offset-poke samples and exit")
	parser.add_argument("paths", nargs="*", help="files or dirs (default src)")
	args = parser.parse_args(argv[1:])
	if args.selftest:
		failed = 0
		for sample, expect in OFFSET_POKE_SAMPLES:
			got = is_offset_poke(sample)
			if got != expect:
				sys.stderr.write("selftest fail: expect %s got %s: %s\n" % (expect, got, sample))
				failed += 1
		if failed:
			return 1
		sys.stdout.write("smell selftest: ok\n")
		return 0
	targets = [Path(a) for a in args.paths] if args.paths else [SRC]
	files: list[Path] = []
	for target in targets:
		path = target if target.is_absolute() else ROOT / target
		if path.is_dir():
			files.extend(iter_sources(path))
		else:
			files.append(path)
	hits: list[str] = []
	for path in files:
		hits.extend(scan_file(path, args.annot))
	if hits:
		sys.stderr.write("smell: %d hit(s)\n" % len(hits))
		for hit in hits:
			sys.stderr.write(hit + "\n")
		return 1
	sys.stdout.write("smell: ok\n")
	return 0


if __name__ == "__main__":
	sys.exit(main(sys.argv))
