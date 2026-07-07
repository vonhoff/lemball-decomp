#!/usr/bin/env python3

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

from project_manifest import (
    find_original_binary,
    list_cpp_sources,
    load_lines,
    load_source_inventory,
    load_target_slice_units,
    unit_name_from_source,
)

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


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a realistic pipeline status report for LEMBALL."
    )
    parser.add_argument("--version", required=True, help="Version identifier, e.g. LEMBALL")
    parser.add_argument("--output", required=True, help="Path to write status.json")
    return parser.parse_args()


def digest_of(path: Path, algorithm: str) -> str:
    digest = hashlib.new(algorithm)
    digest.update(path.read_bytes())
    return digest.hexdigest()


def list_objects(root: Path, pattern: str = "*.obj") -> list[str]:
    if not root.exists():
        return []
    return sorted(path.name for path in root.glob(pattern) if path.is_file())


def build_status(version: str) -> dict[str, object]:
    source_inventory_path = Path("manifests") / "source_files" / f"{version}.txt"
    slice_manifest_path = Path("manifests") / "target_slices" / f"{version}.txt"
    base_dir = Path("build") / "base" / version
    target_dir = Path("build") / "target" / version
    original_binary = find_original_binary(version)

    source_inventory = load_source_inventory(source_inventory_path)
    slices = load_lines(slice_manifest_path)
    base_objects = list_objects(base_dir)
    target_objects = list_objects(target_dir)
    current_sources = list_cpp_sources(Path("LEMBALL"))
    current_source_units = [unit_name_from_source(path) for path in current_sources]
    target_slice_units = load_target_slice_units(slice_manifest_path)
    report_units = sorted(set(current_source_units) | set(target_slice_units))

    return {
        "version": version,
        "original_binary": {
            "found": original_binary is not None,
            "path": str(original_binary) if original_binary is not None else None,
            "sha1": digest_of(original_binary, "sha1") if original_binary is not None else None,
            "sha256": digest_of(original_binary, "sha256") if original_binary is not None else None,
        },
        "source_layout": {
            "source_root": ".",
            "include_root": ".",
            "module_roots": ["LEMBALL", "LEMBALL/VISOS"],
            "style": "single game package with game modules under LEMBALL and framework modules under LEMBALL/VISOS",
            "source_inventory": [
                {
                    "file": row.file,
                    "anchor_address": row.anchor_address,
                    "anchor_function": row.anchor_function,
                    "status": row.status,
                }
                for row in source_inventory
            ],
            "current_sources": [str(path.relative_to(Path.cwd())) for path in current_sources],
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
            "target_slice_manifest": slices,
            "report_units": report_units,
            "base_objects": base_objects,
            "target_objects": target_objects,
            "base_object_disasm": list_objects(base_dir / "disasm", "*.lst"),
        },
        "compiler_strategy": {
            "primary_family": "Microsoft Visual C++ (inferred)",
            "probe_family": "Open Watcom",
            "evidence": [
                "Microsoft Visual C++ Runtime Library",
                "Visual C++ assertion text",
                "statically linked CRT-style startup region",
            ],
            "probe_profile": "toolchains/openwatcom/wpp386-byteprobe.flags",
        },
        "comparison": {
            "decomp_dev_report_ready": False,
            "blocking_reason": (
                "The repository can emit exploratory Watcom objects and target-side byte "
                "wrappers, but real function-level verification still needs a supported "
                "MSVC-era COFF/PDB or equivalent comparison backend."
            ),
            "next_requirement": "Recover a matching Microsoft compiler path before claiming function-level matches."
        },
    }


def main() -> int:
    args = parse_args()
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    status = build_status(args.version)
    output_path.write_text(json.dumps(status, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
