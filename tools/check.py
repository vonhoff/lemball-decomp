#!/usr/bin/env python3
"""Central decompilation match evaluation logic and CLI."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

from reccmp.compare.asm.fixes import find_effective_match
from reccmp.compare.pinned_sequences import SequenceMatcherWithPins

from build import run_build

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build-msvc400"
DEFAULT_JSON = BUILD / "scores.json"
RECCMP = ROOT / ".decomp-venv" / "Scripts" / "reccmp-reccmp.exe"
RELOCATION = re.compile(r"<OFFSET\d+>")
ANNOTATED_SYMBOL = r".+? \((?:DATA|VTABLE|UNK|FUNCTION|IMPORT|IMPORT_THUNK|STRING)\)"
REGISTER = r"(?:eax|ebx|ecx|edx|esi|edi|ebp|esp|ax|bx|cx|dx|si|di|bp|sp)"
DWORD_REGISTERS = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"}
WRITES_FIRST_OPERAND = {
    "adc",
    "add",
    "and",
    "dec",
    "inc",
    "lea",
    "mov",
    "movsx",
    "movzx",
    "neg",
    "not",
    "or",
    "pop",
    "rol",
    "ror",
    "sar",
    "sbb",
    "shl",
    "shr",
    "sub",
    "xor",
}


def norm_addr(value: str | int) -> int:
    if isinstance(value, int):
        return value
    value = value.strip().lower()
    if value.startswith("0x"):
        return int(value, 16)
    return int(value, 16) if any(c in value for c in "abcdef") else int(value, 0)


def insn_text(entry) -> str:
    if isinstance(entry, (list, tuple)) and len(entry) > 1:
        return entry[1]
    return str(entry)


def normalize_asm(s: str) -> str:
    s = s.split("\t")[0].strip()
    s = re.sub(r"Thunk of '([^']+)' \(THUNK\)", r"\1 (FUNCTION)", s)
    s = re.sub(r" \(THUNK\)", " (FUNCTION)", s)
    return s


def is_unresolved_symbol(orig_text: str, recomp_text: str) -> bool:
    """Match one unresolved relocation against a specifically named symbol.

    This is intentionally pairwise. Replacing every symbol with one generic
    token would also hide real differences such as reads from two distinct
    globals.
    """
    orig_text = normalize_asm(orig_text)
    recomp_text = normalize_asm(recomp_text)
    parts = RELOCATION.split(orig_text)
    if len(parts) == 1:
        return False
    pattern = "^" + ANNOTATED_SYMBOL.join(re.escape(part) for part in parts) + "$"
    return bool(re.match(pattern, recomp_text))


def is_unresolved_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"call <OFFSET\d+>$", orig_text):
        return False
    return bool(re.match(r"call (?:Thunk of '.+' \(THUNK\)|.+ \(FUNCTION\))$", recomp_text))


def is_unresolved_jmp(orig_text: str, recomp_text: str) -> bool:
    """Unresolved original tail-jmp through the incremental-link jump table.

    The original binary jmps into its jump table (Ghidra renders the target as a
    raw relative offset); the recomp resolves the callee through its own table,
    which reccmp prints as a thunk.
    """
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"jmp -?0x[0-9a-f]+\s*$", orig_text):
        return False
    return bool(re.match(r"jmp Thunk of '.+' \(THUNK\)$", recomp_text))


def is_recomp_offset_call(orig_text: str, recomp_text: str) -> bool:
    orig_text = orig_text.split("\t")[0].strip()
    recomp_text = recomp_text.split("\t")[0].strip()
    if not re.match(r"call <OFFSET\d+>", recomp_text):
        return False
    return bool(re.match(r"call .+ \(FUNCTION\)$", orig_text))


def is_equivalent_insn(orig_text: str, recomp_text: str) -> bool:
    if normalize_asm(orig_text) == normalize_asm(recomp_text):
        return True
    if is_unresolved_symbol(orig_text, recomp_text):
        return True
    if is_unresolved_symbol(recomp_text, orig_text):
        return True
    if is_unresolved_call(orig_text, recomp_text):
        return True
    if is_unresolved_jmp(orig_text, recomp_text):
        return True
    if is_recomp_offset_call(orig_text, recomp_text):
        return True
    return False


def collect_diff_insns(diff, key: str) -> list[str]:
    insns = []
    if not diff:
        return insns
    for _, chunks in diff:
        for chunk in chunks:
            for entry in chunk.get(key, []) or []:
                insns.append(insn_text(entry))
    return insns


def is_thunk_only_diff(diff) -> bool:
    if not diff:
        return False
    orig_insns = collect_diff_insns(diff, "orig")
    recomp_insns = collect_diff_insns(diff, "recomp")
    if not orig_insns and not recomp_insns:
        return False
    if len(orig_insns) != len(recomp_insns):
        return False
    for orig_text, recomp_text in zip(orig_insns, recomp_insns):
        if not is_equivalent_insn(orig_text, recomp_text):
            return False
    return True


def split_instruction(instruction: str) -> tuple[str, list[str]]:
    mnemonic, _, operand_text = instruction.partition(" ")
    operands = [operand.strip() for operand in operand_text.split(", ")] if operand_text else []
    return mnemonic, operands


def register_is_known_zero(asm: list[str], before: int, register: str) -> bool:
    """Conservatively trace a full register back to an explicit zeroing idiom."""
    register_re = re.compile(r"\b" + re.escape(register) + r"\b")
    caller_saved = {"eax", "ecx", "edx", "ax", "cx", "dx"}

    for instruction in reversed(asm[:before]):
        mnemonic, operands = split_instruction(instruction)
        if mnemonic.startswith("j") or mnemonic in ("loop", "loope", "loopne", "ret", "retn"):
            return False
        if mnemonic == "call" and register in caller_saved:
            return False
        if not register_re.search(instruction):
            continue
        if mnemonic in ("sub", "xor") and operands == [register, register]:
            return True
        if mnemonic in WRITES_FIRST_OPERAND and operands and operands[0] == register:
            return False
        if mnemonic not in ("cmp", "test", "push"):
            return False

    return False


def zero_compare_equivalent(
    cmp_text: str, test_text: str, cmp_asm: list[str], cmp_index: int
) -> bool:
    cmp_mnemonic, cmp_operands = split_instruction(cmp_text)
    test_mnemonic, test_operands = split_instruction(test_text)
    if cmp_mnemonic != "cmp" or test_mnemonic != "test":
        return False
    if len(cmp_operands) != 2 or len(test_operands) != 2:
        return False
    value, zero_register = cmp_operands
    if not re.fullmatch(REGISTER, zero_register):
        return False
    if test_operands != [value, value]:
        return False
    return register_is_known_zero(cmp_asm, cmp_index, zero_register)


def normalize_zero_comparisons(orig_asm: list[str], recomp_asm: list[str]) -> None:
    """Make proven ``cmp value, zero-register`` / ``test value, value`` pairs equal."""
    if len(orig_asm) != len(recomp_asm):
        return
    for index, (orig_text, recomp_text) in enumerate(zip(orig_asm, recomp_asm)):
        if zero_compare_equivalent(orig_text, recomp_text, orig_asm, index):
            recomp_asm[index] = orig_text
        elif zero_compare_equivalent(recomp_text, orig_text, recomp_asm, index):
            recomp_asm[index] = orig_text


def normalize_copy_tests(asm: list[str]) -> None:
    """Test the source register when a preceding copy proves it equivalent.

    This lets reccmp's relocation check recognize codegen such as
    ``mov edi, edx; test edi, edi`` versus an earlier ``test edx, edx``.
    Restricting this to an adjacent full-register copy makes the value
    equivalence independent of liveness or alias assumptions.
    """
    for index in range(1, len(asm)):
        test_mnemonic, test_operands = split_instruction(asm[index])
        if test_mnemonic != "test" or len(test_operands) != 2:
            continue
        tested_register = test_operands[0]
        if test_operands[1] != tested_register or tested_register not in DWORD_REGISTERS:
            continue

        mov_mnemonic, mov_operands = split_instruction(asm[index - 1])
        if (
            mov_mnemonic == "mov"
            and len(mov_operands) == 2
            and mov_operands[0] == tested_register
            and mov_operands[1] in DWORD_REGISTERS
        ):
            source_register = mov_operands[1]
            asm[index] = f"test {source_register}, {source_register}"


def normalize_multiply_copy_zero(asm: list[str]) -> None:
    """Canonicalize two equivalent ways to prepare a dividend register pair.

    With three distinct full registers, these sequences leave identical
    register values at the division:

    ``mov A, B; sub B, B; imul A, C; div D``
    ``imul B, C; mov A, B; sub B, B; div D``

    Requiring the immediately following division is important: the two forms
    do not leave identical status flags, but ``div``/``idiv`` does not consume
    them and leaves its own flags undefined.
    """
    for index in range(len(asm) - 3):
        mov_mnemonic, mov_operands = split_instruction(asm[index])
        sub_mnemonic, sub_operands = split_instruction(asm[index + 1])
        imul_mnemonic, imul_operands = split_instruction(asm[index + 2])
        div_mnemonic, div_operands = split_instruction(asm[index + 3])
        if (
            mov_mnemonic != "mov"
            or sub_mnemonic != "sub"
            or imul_mnemonic != "imul"
            or div_mnemonic not in ("div", "idiv")
            or len(mov_operands) != 2
            or len(sub_operands) != 2
            or len(imul_operands) != 2
            or len(div_operands) != 1
        ):
            continue

        product_register, value_register = mov_operands
        multiplier = imul_operands[1]
        if (
            product_register not in DWORD_REGISTERS
            or value_register not in DWORD_REGISTERS
            or multiplier not in DWORD_REGISTERS
            or len({product_register, value_register, multiplier}) != 3
            or sub_operands != [value_register, value_register]
            or imul_operands[0] != product_register
        ):
            continue

        asm[index : index + 3] = [
            f"imul {value_register}, {multiplier}",
            f"mov {product_register}, {value_register}",
            f"sub {value_register}, {value_register}",
        ]


def group_asm(chunks) -> tuple[list[str], list[str]]:
    """Rebuild the two assembly excerpts represented by one JSON diff group.

    reccmp writes matching lines once under ``both`` and mismatching lines as
    separate ``orig``/``recomp`` arrays. Known target-resolution differences
    are made identical before its compiler-entropy classifier runs.
    """
    orig_asm: list[str] = []
    recomp_asm: list[str] = []

    for chunk in chunks:
        both = chunk.get("both", []) or []
        if both:
            for entry in both:
                instruction = normalize_asm(insn_text(entry))
                orig_asm.append(instruction)
                recomp_asm.append(instruction)
            continue

        orig_raw = [insn_text(entry) for entry in chunk.get("orig", []) or []]
        recomp_raw = [insn_text(entry) for entry in chunk.get("recomp", []) or []]
        orig_chunk = [normalize_asm(instruction) for instruction in orig_raw]
        recomp_chunk = [normalize_asm(instruction) for instruction in recomp_raw]

        if len(orig_chunk) == len(recomp_chunk):
            for index, (orig_text, recomp_text) in enumerate(zip(orig_raw, recomp_raw)):
                if is_equivalent_insn(orig_text, recomp_text):
                    recomp_chunk[index] = orig_chunk[index]

        orig_asm.extend(orig_chunk)
        recomp_asm.extend(recomp_chunk)

    normalize_copy_tests(orig_asm)
    normalize_copy_tests(recomp_asm)
    normalize_multiply_copy_zero(orig_asm)
    normalize_multiply_copy_zero(recomp_asm)
    normalize_zero_comparisons(orig_asm, recomp_asm)
    return orig_asm, recomp_asm


def is_codegen_equivalent_diff(diff) -> bool:
    """Whether all remaining differences are accepted compiler entropy.

    This deliberately delegates register selection, operand-order, and safe
    instruction-relocation decisions to the pinned reccmp implementation. The
    local work is limited to removing target/thunk rendering differences that
    otherwise prevent reccmp from seeing the compiler-only match.
    """
    if not diff:
        return False

    saw_difference = False
    for _, chunks in diff:
        orig_asm, recomp_asm = group_asm(chunks)
        if not orig_asm and not recomp_asm:
            continue

        saw_difference = True
        matcher = SequenceMatcherWithPins(orig_asm, recomp_asm, [])
        if matcher.ratio() == 1.0:
            continue

        if not find_effective_match(matcher.get_opcodes(), orig_asm, recomp_asm):
            return False

    return saw_difference


def compute_ratio(match: dict | None, name: str = "") -> tuple[float, str]:
    """Calculate effective match percentage and match reason tag."""
    if match is None or match.get("stub"):
        return 0.0, "STUB"

    mname = name or match.get("name") or ""
    is_dtor = match.get("type") == 1 and (
        "`scalar deleting destructor'" in mname or "`vector deleting destructor'" in mname
    )

    if match.get("effective") or float(match.get("matching", 0.0)) == 1.0 or is_dtor:
        return 100.0, "MATCH"

    if is_thunk_only_diff(match.get("diff")):
        return 100.0, "MATCH (thunk)"

    if is_codegen_equivalent_diff(match.get("diff")):
        return 100.0, "MATCH (compiler entropy)"

    return float(match.get("matching", 0.0)) * 100.0, ""


def run_selftest() -> int:
    def make_diff(orig: list[str], recomp: list[str]):
        return [["@@ selftest @@", [{"orig": [["", x] for x in orig], "recomp": [["", x] for x in recomp]}]]]

    thunk_samples = (
        ("call Thing::Run (FUNCTION)", "call Thunk of 'Thing::Run' (THUNK)", True),
        ("call <OFFSET1>", "call Thunk of 'Thing::Run' (THUNK)", True),
        ("call <OFFSET1>", "call _fopen (UNK)", True),
        ("mov dword ptr [esi], <OFFSET2>", "mov dword ptr [esi], Thing::`vftable' (VTABLE)", True),
        ("mov eax, dword ptr [g_left (DATA)]", "mov eax, dword ptr [g_right (DATA)]", False),
        ("call First (FUNCTION)", "call Second (FUNCTION)", False),
    )
    codegen_samples = (
        (
            ["cmp ebx, eax", "jge 0x2c", "call <OFFSET1>"],
            ["cmp eax, ebx", "jle 0x2c", "call Thunk of 'Thing::Run' (THUNK)"],
            True,
        ),
        (
            ["mov byte ptr [eax + esi + 0x3b0], cl", "call <OFFSET1>"],
            ["mov byte ptr [esi + eax + 0x3b0], cl", "call Thunk of 'Thing::Run' (THUNK)"],
            True,
        ),
        (
            ["push esi", "mov esi, ecx", "mov eax, dword ptr [esi + 4]", "pop esi", "ret"],
            ["push edi", "mov edi, ecx", "mov eax, dword ptr [edi + 4]", "pop edi", "ret"],
            True,
        ),
        (
            ["xor esi, esi", "cmp ebx, esi", "je 0x2", "ret"],
            ["xor esi, esi", "test ebx, ebx", "je 0x2", "ret"],
            True,
        ),
        (
            ["xor esi, esi", "jne 0x2", "cmp ebx, esi", "je 0x4", "ret"],
            ["xor esi, esi", "jne 0x2", "test ebx, ebx", "je 0x4", "ret"],
            False,
        ),
        (
            [
                "mov edx, dword ptr [ecx + 0x28]",
                "push edi",
                "mov edi, edx",
                "test edi, edi",
                "je 0x2",
            ],
            [
                "mov edx, dword ptr [ecx + 0x28]",
                "test edx, edx",
                "push edi",
                "mov edi, edx",
                "je 0x2",
            ],
            True,
        ),
        (
            ["mov eax, edx", "sub edx, edx", "imul eax, ecx", "div esi"],
            ["imul edx, ecx", "mov eax, edx", "sub edx, edx", "div esi"],
            True,
        ),
        (
            ["mov esi, 1", "cmp ebx, esi", "je 0x2", "ret"],
            ["mov esi, 1", "test ebx, ebx", "je 0x2", "ret"],
            False,
        ),
        (
            ["mov eax, dword ptr [g_left (DATA)]", "ret"],
            ["mov eax, dword ptr [g_right (DATA)]", "ret"],
            False,
        ),
    )

    failed = 0
    for orig, recomp, expected in thunk_samples:
        actual = is_equivalent_insn(orig, recomp)
        if actual != expected:
            print(f"selftest fail: instruction expected {expected} got {actual}: {orig!r} / {recomp!r}")
            failed += 1

    for orig, recomp, expected in codegen_samples:
        actual = is_codegen_equivalent_diff(make_diff(orig, recomp))
        if actual != expected:
            print(f"selftest fail: codegen expected {expected} got {actual}: {orig!r} / {recomp!r}")
            failed += 1

    aliased_multiplier = ["mov eax, edx", "sub edx, edx", "imul eax, eax"]
    normalized = list(aliased_multiplier)
    normalize_multiply_copy_zero(normalized)
    if normalized != aliased_multiplier:
        print("selftest fail: unsafe aliased multiply/copy sequence was normalized")
        failed += 1

    flags_live = ["mov eax, edx", "sub edx, edx", "imul eax, ecx", "jo 0x2"]
    normalized = list(flags_live)
    normalize_multiply_copy_zero(normalized)
    if normalized != flags_live:
        print("selftest fail: multiply/copy sequence with live flags was normalized")
        failed += 1

    if failed:
        return 1
    print("check selftest: ok")
    return 0


def format_diff_text(diff) -> str:
    if not diff:
        return ""
    lines = []
    for _, chunks in diff:
        for chunk in chunks:
            orig = chunk.get("orig", [])
            recomp = chunk.get("recomp", [])
            if not orig and not recomp:
                continue

            if len(orig) == len(recomp):
                if all(is_equivalent_insn(insn_text(o), insn_text(r)) for o, r in zip(orig, recomp)):
                    continue

            for item in orig:
                lines.append(f"- {insn_text(item)}")
            for item in recomp:
                lines.append(f"+ {insn_text(item)}")
    return "\n".join(lines)


def run_reccmp(json_path: Path) -> None:
    cmd = [str(RECCMP), "--target", "LEMBALL", "--json", str(json_path.name), "--silent"]
    proc = subprocess.run(cmd, cwd=BUILD, capture_output=True, text=True)
    if proc.returncode != 0 and not json_path.exists():
        sys.stderr.write(proc.stderr or proc.stdout)
        raise RuntimeError(f"reccmp exited with code {proc.returncode}")


def load_matches(json_path: Path) -> dict[int, dict]:
    with json_path.open(encoding="utf-8") as f:
        data = json.load(f)["data"]
    return {
        int(x["address"], 16): x
        for x in data
        if x.get("type") in (None, 1)
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("addrs", nargs="*", help="Addresses (e.g. 0x0045ca30)")
    parser.add_argument("--diff", action="store_true", help="Print diff for non-matching functions")
    parser.add_argument("--json", type=Path, default=DEFAULT_JSON)
    parser.add_argument("--no-build", action="store_true", help="Skip incremental build before check")
    parser.add_argument("--clean-first", action="store_true", help="Clean build before check")
    parser.add_argument("--no-reccmp", action="store_true", help="Do not run reccmp; reuse existing JSON")
    parser.add_argument("--selftest", action="store_true", help="exercise equivalence classifiers and exit")
    args = parser.parse_args()

    if args.selftest:
        return run_selftest()

    if not args.no_build:
        exit_code = run_build(clean_first=args.clean_first)
        if exit_code != 0:
            print(f"BUILD_FAILED exit={exit_code} (see build-msvc400/last_build.log)")
            return exit_code

    if not args.addrs:
        return 0

    if not args.no_reccmp or not args.json.exists():
        run_reccmp(args.json)

    matches = load_matches(args.json)

    for raw in args.addrs:
        addr = norm_addr(raw)
        m = matches.get(addr)
        if not m:
            print(f"0x{addr:08x}: NOT_FOUND")
            continue

        name = m.get("name") or "?"
        ratio, tag = compute_ratio(m, name)

        if tag:
            print(f"0x{addr:08x} {name}: {ratio:.2f}% {tag}")
        else:
            print(f"0x{addr:08x} {name}: {ratio:.2f}%")
            if args.diff:
                diff_text = format_diff_text(m.get("diff"))
                if diff_text:
                    print("--- diff ---")
                    print(diff_text)
                    print("------------")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
