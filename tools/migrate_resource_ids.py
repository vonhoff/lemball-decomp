#!/usr/bin/env python3
"""Replace MOG resource hex literals with RES_* defines from Manifest.h."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_JSON = ROOT / "build-msvc400" / "resource_manifest.json"

# Files/dirs in migration scope (relative to src/)
DEFAULT_PATHS = [
    "Views/Display/Main2DDisplay.cpp",
    "Control/Game/Game.cpp",
    "Control/Level/LevelLoader.cpp",
    "Frontend/Resources/CdLoadAnim.cpp",
    "Frontend/Resources/FrontendResourceLoader.cpp",
    "Frontend/Windows/IntroAnimAnimWindow.cpp",
    "Frontend/Windows/SuccFailAnimWindow.cpp",
    "Frontend/Support/TargetAboutScreen.cpp",
    "Frontend/Base/BaseFrontendDrawer.cpp",
    "Frontend/Drawers/PreviewDrawer.cpp",
    "Frontend/Drawers/PasswordDrawer.cpp",
    "Frontend/Drawers/SuccFailDrawer.cpp",
    "Frontend/Drawers/MainOptions1Drawer.cpp",
    "Frontend/Drawers/MainOptions2Drawer.cpp",
    "Frontend/Drawers/NetworkOptionsDrawer.cpp",
    "Frontend/Controls/HiliteController.cpp",
    "Frontend/Controls/GunController.cpp",
    "Views/Sound/SoundView.cpp",
    "Visos/Graphics/Cursor.cpp",
]

HEX_RE = re.compile(r"\b0x([0-9a-fA-F]+)\b")

# Lines containing these substrings are never modified.
SKIP_LINE_MARKERS = (
    "unsigned char g_",
    "operator new(",
    "TextManager(0x2b6",
    "PrepareMusic(0x2220",
    "GetSystemMetrics",
    "WinHelpA",
    "GetDlgItem",
    "LoadCursorA",
    "m_color = 0x10000",
    "memcpy(",
    "GetModuleFileNameA",
    "GetDlgItemTextA",
    "SetDlgItemTextA",
    "fireTime = fireTime - 0x177",
    "// 0x",
    "SIZE 0x",
    "m_pad",
    "undefined m_",
    "vtable+0x",
)

# Globals whose initializer arrays should be migrated.
RESOURCE_GLOBAL_PREFIXES = (
    "g_dwFrontend",
    "g_dwPasswordButtonAnimIds",
    "g_dwMainOptions",
    "g_dwPreview",
    "g_dwSuccFail",
    "g_anNetworkOptionsAnimIds",
    "g_pEffectSpecs",
    "g_cursorResourceIds",
    "g_dwGunAnim",
    "g_dwHiliteAnimationId",
)

SAFE_CALL_PATTERNS = (
    re.compile(r"Res\w+::Load\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"AttachPalette\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"SetAnim\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"LoadFont\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"SetResId\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"RegisterRemap\s*\(\s*(0x[0-9a-fA-F]+)\s*,"),
    re.compile(r"LoadMovie\s*\(\s*i\s*\+\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"return\s+p_level\s*\+\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"musicId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"musicId\s*=\s*m_randomMusicIndex\s*\+\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_primaryAnimBase\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_secondaryAnimBase\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_desiredPalette\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_animationId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_backgroundId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_secondaryBitmapId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_primaryBitmapId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_tileBitmap\s*=\s*ResBitmap::Load\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"m_backgroundBitmap\s*=\s*ResBitmap::Load\s*\(\s*(0x[0-9a-fA-F]+)\s*\)"),
    re.compile(r"m_sideFrameAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_unknown384\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_topFrameAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_bottomFrameAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_lemmingAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_teamAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_ambientAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_opponentAnimId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_currentResourceId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"m_firstResourceId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"g_dwGunAnim\w+\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"g_dwHiliteAnimationId\s*=\s*(0x[0-9a-fA-F]+)\s*;"),
    re.compile(r"\{\s*\d+\s*,\s*(0x[0-9a-fA-F]+)\s*,"),  # SoundView effect spec
)


def load_manifest(path: Path) -> dict[int, str]:
    data = json.loads(path.read_text(encoding="utf-8"))
    mapping: dict[int, str] = {}
    for entry in data["entries"]:
        mapping[int(entry["id"])] = entry["define"]
    return mapping


def hex_to_int(token: str) -> int:
    return int(token, 16)


def should_skip_line(line: str) -> bool:
    return any(marker in line for marker in SKIP_LINE_MARKERS)


def in_resource_global_context(line: str) -> bool:
    return any(prefix in line for prefix in RESOURCE_GLOBAL_PREFIXES)


def manifest_include_for(path: Path) -> str:
    rel = path.relative_to(ROOT / "src")
    parts = rel.parts
    if parts[0] == "Visos" and parts[1] == "Resources":
        return '#include "Manifest.h"'
    if parts[0] == "Visos" and parts[1] == "Graphics":
        return '#include "../Resources/Manifest.h"'
    return '#include "../../Visos/Resources/Manifest.h"'


def replace_hex_in_line(line: str, mapping: dict[int, str], force: bool) -> tuple[str, list[str]]:
    if should_skip_line(line):
        return line, []

    changes: list[str] = []

    def sub_hex(match: re.Match[str]) -> str:
        value = hex_to_int(match.group(1))
        if value == 0:
            return match.group(0)
        define = mapping.get(value)
        if define is None:
            return match.group(0)
        changes.append(f"0x{match.group(1).lower()} -> {define}")
        return define

    if force:
        new_line = HEX_RE.sub(sub_hex, line)
        return new_line, changes

    safe = False
    for pattern in SAFE_CALL_PATTERNS:
        if pattern.search(line):
            safe = True
            break
    if in_resource_global_context(line):
        safe = True

    if not safe:
        return line, []

    new_line = line
    for pattern in SAFE_CALL_PATTERNS:
        def repl(m: re.Match[str]) -> str:
            token = m.group(1)
            value = hex_to_int(token)
            define = mapping.get(value)
            if define is None:
                return m.group(0)
            changes.append(f"{token} -> {define}")
            return m.group(0).replace(token, define)

        new_line = pattern.sub(repl, new_line)

    if in_resource_global_context(line):
        new_line = HEX_RE.sub(sub_hex, new_line)

    return new_line, changes


def ensure_include(text: str, include_line: str) -> str:
    if "Manifest.h" in text:
        return text
    marker = '#include "../../Visos/Resources/Res'
    alt_marker = '#include "../Resources/Res'
    local_marker = '#include "Res'
    for needle in (marker, alt_marker, local_marker):
        idx = text.find(needle)
        if idx != -1:
            line_end = text.find("\n", idx)
            return text[: line_end + 1] + include_line + "\n" + text[line_end + 1 :]
    # fallback: after last #include
    includes = list(re.finditer(r'^#include .+\n', text, re.MULTILINE))
    if includes:
        pos = includes[-1].end()
        return text[:pos] + include_line + "\n" + text[pos:]
    return include_line + "\n" + text


def in_resource_global_array(text: str, line_index: int) -> bool:
    """True if line_index is inside a known resource-id global array initializer."""
    chunk = text[:line_index]
    starts = list(
        re.finditer(
            r"^(unsigned (?:int|long) )?"
            r"(g_dwFrontend\w+|g_dwPasswordButtonAnimIds\w+|g_dwMainOptions\w+|"
            r"g_dwPreview\w+|g_dwSuccFail\w+|g_anNetworkOptionsAnimIds|"
            r"g_pEffectSpecs|g_cursorResourceIds)\b[^\n]*=\s*\{",
            chunk,
            re.MULTILINE,
        )
    )
    if not starts:
        return False
    start = starts[-1].end() - 1  # points at opening '{'
    depth = 0
    for ch in text[start:line_index]:
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                return False
    return depth > 0


def migrate_file(path: Path, mapping: dict[int, str], apply: bool) -> list[str]:
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines(keepends=True)
    out_lines: list[str] = []
    file_changes: list[str] = []
    offset = 0

    for i, line in enumerate(lines, 1):
        force = in_resource_global_context(line) or in_resource_global_array(
            text, offset
        )
        new_line, changes = replace_hex_in_line(line.rstrip("\n"), mapping, force)
        new_line += "\n" if line.endswith("\n") else ""
        if changes:
            for change in changes:
                file_changes.append(f"{path}:{i}: {change}")
        out_lines.append(new_line)
        offset += len(line)

    new_text = "".join(out_lines)
    if file_changes:
        new_text = ensure_include(new_text, manifest_include_for(path))
    if apply and new_text != text:
        path.write_text(new_text, encoding="utf-8")
    return file_changes


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_JSON)
    parser.add_argument("--apply", action="store_true", help="Write changes (default: dry-run)")
    parser.add_argument("paths", nargs="*", help="Source files relative to src/ (default: migration set)")
    args = parser.parse_args()

    if not args.manifest.is_file():
        print(f"Manifest JSON not found: {args.manifest}", file=sys.stderr)
        return 1

    mapping = load_manifest(args.manifest)
    targets = args.paths or DEFAULT_PATHS
    all_changes: list[str] = []

    for rel in targets:
        path = ROOT / "src" / rel
        if not path.is_file():
            print(f"Skip missing: {path}", file=sys.stderr)
            continue
        all_changes.extend(migrate_file(path, mapping, args.apply))

    mode = "APPLY" if args.apply else "DRY-RUN"
    print(f"=== {mode}: {len(all_changes)} replacement(s) ===")
    for change in all_changes:
        print(change)

    return 0


if __name__ == "__main__":
    sys.exit(main())
