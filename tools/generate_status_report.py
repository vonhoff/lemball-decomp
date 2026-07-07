#!/usr/bin/env python3

from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path

ORIGINAL_BINARY_PATH = Path("data/LEMBALL.EXE")
FUNCTION_MANIFEST_PATH = Path("data/manifest.json")
ENTRYPOINT_VA = "0x0047FE20"
ENTRY_FUNCTION = "entry"
MAIN_CONTEXT_FUNCTION = "initialize_main_game_context"
SUBSYSTEM_INIT_FUNCTION = "initialize_core_subsystems"
SUBSYSTEM_SHUTDOWN_FUNCTION = "shutdown_core_subsystems"
COMMAND_LINE_FUNCTION = "parse_command_line_option_token"
KNOWN_SWITCHES = ["paranoid", "nowait", "/SNDDEBUG", "/STATDEBUG", "/MEMDEBUG"]
KNOWN_LOG_LINES = [
    "ViSOS v",
    "_MEM_Init",
    "_STRM_Init",
    "_DBG_Init",
    "_INP_Init",
    "_TIME_Init",
    "_GDI_Init",
    "_STAT_Init",
    "_RES_Init",
    "debug.out",
]
FUNCTION_MARKER_RE = re.compile(r"^\s*//\s*FUNCTION:\s*(\w+)\s+(0x[0-9A-Fa-f]+)\s*$")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a local pipeline snapshot for LEMBALL."
    )
    parser.add_argument("--output", required=True, help="Path to write status.json")
    return parser.parse_args()


def digest_of(path: Path, algorithm: str) -> str:
    digest = hashlib.new(algorithm)
    digest.update(path.read_bytes())
    return digest.hexdigest()


def gather_source_markers(source_root: Path, target_name: str) -> list[int]:
    markers: list[int] = []
    for path in sorted(source_root.rglob("*.CPP")):
        for line in path.read_text().splitlines():
            match = FUNCTION_MARKER_RE.match(line)
            if match is None:
                continue
            module, address = match.groups()
            if module != target_name:
                continue
            markers.append(int(address, 16))
    return markers


def build_status() -> dict[str, object]:
    original_binary = ORIGINAL_BINARY_PATH if ORIGINAL_BINARY_PATH.exists() else None
    function_manifest = (
        json.loads(FUNCTION_MANIFEST_PATH.read_text()) if FUNCTION_MANIFEST_PATH.exists() else None
    )
    source_markers = gather_source_markers(Path("src"), "LEMBALL")
    marked_code_bytes = 0
    total_reportable_code_bytes = 0
    if function_manifest is not None:
        functions_by_address = {
            int(function["address"], 16): function for function in function_manifest["functions"]
        }
        marked_code_bytes = sum(
            int(functions_by_address[address]["size"])
            for address in source_markers
            if address in functions_by_address
        )
        total_reportable_code_bytes = sum(
            int(function["size"])
            for function in function_manifest["functions"]
            if function["category"] in {"internal", "thunk"}
        )

    return {
        "version": "LEMBALL",
        "original_binary": {
            "found": original_binary is not None,
            "path": str(original_binary) if original_binary is not None else None,
            "sha1": digest_of(original_binary, "sha1") if original_binary is not None else None,
            "sha256": digest_of(original_binary, "sha256") if original_binary is not None else None,
        },
        "source_layout": {
            "source_root": ".",
            "include_root": ".",
            "module_roots": ["src", "src/visos"],
            "style": "single game package with game modules under src and framework modules under src/visos",
        },
        "startup_path": {
            "entrypoint_va": ENTRYPOINT_VA,
            "entry_function": ENTRY_FUNCTION,
            "main_context_function": MAIN_CONTEXT_FUNCTION,
            "subsystem_init_function": SUBSYSTEM_INIT_FUNCTION,
            "subsystem_shutdown_function": SUBSYSTEM_SHUTDOWN_FUNCTION,
            "command_line_option_function": COMMAND_LINE_FUNCTION,
            "known_switches": KNOWN_SWITCHES,
            "known_log_lines": KNOWN_LOG_LINES,
        },
        "artifacts": {
            "function_inventory_path": (
                str(FUNCTION_MANIFEST_PATH) if function_manifest is not None else None
            ),
        },
        "function_inventory": (
            None
            if function_manifest is None
            else {
                "program": str(function_manifest["program"]),
                "function_count": int(function_manifest["function_count"]),
                "summary": {
                    key: int(value) for key, value in dict(function_manifest["summary"]).items()
                },
            }
        ),
        "annotation_progress": {
            "marked_functions": len(source_markers),
            "marked_code_bytes": marked_code_bytes,
            "total_reportable_code_bytes": total_reportable_code_bytes,
            "marked_code_percent": (
                marked_code_bytes / total_reportable_code_bytes * 100.0
                if total_reportable_code_bytes
                else 0.0
            ),
        },
        "compiler_strategy": {
            "primary_family": "Microsoft Visual C++ (inferred)",
            "evidence": [
                "Microsoft Visual C++ Runtime Library",
                "Visual C++ assertion text",
                "statically linked CRT-style startup region",
            ],
        },
        "comparison": {
            "function_report_publishable": function_manifest is not None,
            "object_report_publishable": False,
            "blocking_reason": (
                "The repository reports progress from the Ghidra-backed function inventory "
                "and the reccmp lane."
            ),
            "next_requirement": (
                "Keep adding verified source markers, rebuild with the MSVC lane, and "
                "publish reccmp-backed report.json artifacts."
            ),
        },
    }


def main() -> int:
    args = parse_args()
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    status = build_status()
    output_path.write_text(json.dumps(status, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
