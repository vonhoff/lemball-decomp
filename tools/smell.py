#!/usr/bin/env python3
"""Fail on decomp smells agents keep reintroducing.

  python tools/smell.py
  python tools/smell.py --annot
  python tools/smell.py src/Visos/Graphics

Default errors:
  vbptr-walk     *(int*)(*(int*)(obj + 0x40) + 4) virtual-base poke
  offset-poke    (char*)obj +/- field offset
  this-adjust    (char*)this - N mixin poke
  no-annotation  non-empty .cpp function with neither reccmp nor 68K marker

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
OFFSET_POKE = re.compile(
	r"\(\s*char\s*\*\s*\)\s*(?P<expr>this|[A-Za-z_][\w]*|\(\s*[A-Za-z_][\w]*\s*\))"
	r"\s*[+-]\s*(?:0x[0-9A-Fa-f]+|\d+)\b"
)
VAR_OFFSET = re.compile(
	r"\(\s*char\s*\*\s*\)\s*(?P<expr>this|[A-Za-z_][\w]*)\s*[+-]\s*(?!0x)(?P<off>[A-Za-z_][\w]*)"
)
METHOD_DEF = re.compile(r"^[A-Za-z_][\w:]*::~?[A-Za-z_][\w]*\s*\(")
FREE_DEF = re.compile(r"^(?:static\s+)?(?:[A-Za-z_][\w:*&]*\s+)+\w+\s*\(")
BUFFER_OK = re.compile(r"Buffer|Bits\b|sz[A-Z]", re.I)
SKIP_LEAD = {"if", "while", "for", "switch", "return", "else", "case", "catch", "extern"}


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
			for match in OFFSET_POKE.finditer(code):
				if allowed_expr(match.group("expr")):
					continue
				hits.append("%s:%d: offset-poke %s" % (rel, lineno, match.group(0).strip()))
			for match in VAR_OFFSET.finditer(code):
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
	parser.add_argument("paths", nargs="*", help="files or dirs (default src)")
	args = parser.parse_args(argv[1:])
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
