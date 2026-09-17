"""Run reccmp via the Python API; cache binary and source-mapping inputs."""

from __future__ import annotations

import hashlib
import json
import logging
import subprocess
import sys
from pathlib import Path

from reccmp.compare import Compare
from reccmp.compare.report import ReccmpStatusReport, serialize_reccmp_report
from reccmp.formats import PEImage
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.project.detect import DetectWhat, RecCmpProject, detect_project
from reccmp.tools.roadmap import (
    ModuleMap,
    RoadmapRow,
    export_to_csv,
    match_type_abbreviation,
)
from reccmp.types import EntityType

from .paths import (
    BUILD,
    ORIGINAL_EXE,
    RECCMP_JSON,
    RECCMP_STAMP,
    RECOMP_EXE,
    RECOMP_PDB,
    ROADMAP_CSV,
    ROOT,
    SRC,
    file_id,
)
from .reccmp_compat import (
    configure_original_extents,
    configure_pointer_comparisons,
    install_parser_fix,
)


def _stamp() -> dict:
    return {
        "original": file_id(ORIGINAL_EXE),
        "recomp": file_id(RECOMP_EXE),
        "pdb": file_id(RECOMP_PDB),
        "sources": {
            path.relative_to(SRC).as_posix(): file_id(path)
            for path in sorted(SRC.rglob("*")) if path.is_file()
        },
        "configuration": {
            str(path): hashlib.sha256(path.read_bytes()).hexdigest() if path.exists() else None
            for path in (ROOT / "reccmp-project.yml", ROOT / "reccmp-user.yml",
                         ROOT / "reccmp-build.yml",
                         BUILD / "reccmp-build.yml", Path(__file__),
                         Path(__file__).with_name("reccmp_compat.py"))
        },
    }


def load_engine() -> tuple[object, Compare]:
    install_parser_fix()
    project = RecCmpProject.from_directory(BUILD)
    target = project.get("LEMBALL")
    logging.getLogger("reccmp").setLevel(logging.WARNING)
    engine = Compare.from_target(target)
    configure_pointer_comparisons(engine)
    configure_original_extents(engine)
    return target, engine


def _write_roadmap(target, engine: Compare, csv_path: Path) -> None:
    orig_bin = engine.orig_bin
    recomp_bin = engine.recomp_bin
    if not isinstance(orig_bin, PEImage) or not isinstance(recomp_bin, PEImage):
        raise TypeError("roadmap requires 32-bit PE images")

    module_map = ModuleMap(target.recompiled_pdb, recomp_bin)

    def same_section(orig: int, recomp: int) -> bool:
        try:
            return orig_bin.sections[orig - 1].name == recomp_bin.sections[recomp - 1].name
        except IndexError:
            return False

    rows: list[RoadmapRow] = []
    for match in engine.get_all():
        try:
            module_name = None
            if match.recomp_addr is not None and recomp_bin.is_valid_vaddr(match.recomp_addr):
                ref = module_map.get_module(match.recomp_addr)
                if ref is not None:
                    _, module_name = ref

            orig_sect_ofs = recomp_sect_ofs = None
            orig_sect = orig_ofs = recomp_sect = recomp_ofs = None
            displacement = None
            if match.orig_addr is not None:
                orig_sect, orig_ofs = orig_bin.get_relative_addr(match.orig_addr)
                orig_sect_ofs = f"{orig_sect:04}:{orig_ofs:08x}"
            if match.recomp_addr is not None:
                recomp_sect, recomp_ofs = recomp_bin.get_relative_addr(match.recomp_addr)
                recomp_sect_ofs = f"{recomp_sect:04}:{recomp_ofs:08x}"
            if (
                orig_sect is not None
                and recomp_sect is not None
                and same_section(orig_sect, recomp_sect)
                and orig_ofs is not None
                and recomp_ofs is not None
            ):
                displacement = recomp_ofs - orig_ofs

            rows.append(
                RoadmapRow(
                    orig_sect_ofs,
                    recomp_sect_ofs,
                    match.orig_addr,
                    match.recomp_addr,
                    displacement,
                    match_type_abbreviation(match.entity_type),
                    match.any_size() or 0,
                    match.name,
                    module_name,
                )
            )
        except InvalidVirtualAddressError:
            continue

    export_to_csv(str(csv_path), rows)


def run_reccmp(
    json_path: Path | None = None,
    *,
    detect: bool = False,
    roadmap: bool = False,
) -> Path:
    out = (json_path or RECCMP_JSON).resolve()
    BUILD.mkdir(parents=True, exist_ok=True)

    if detect:
        detect_project(
            project_directory=ROOT,
            search_path=[ROOT / "data"],
            detect_what=DetectWhat.ORIGINAL,
            build_directory=BUILD,
        )

    canonical = out == RECCMP_JSON.resolve()
    input_stamp = _stamp()
    reuse = False
    if canonical and RECCMP_JSON.exists() and RECCMP_STAMP.exists():
        try:
            reuse = json.loads(RECCMP_STAMP.read_text(encoding="utf-8")) == input_stamp
        except (OSError, ValueError):
            pass

    need_roadmap = roadmap and (not ROADMAP_CSV.exists() or not reuse)
    if reuse and not need_roadmap:
        return out

    # A failed or interrupted refresh must not leave either output cache-valid.
    if canonical:
        RECCMP_STAMP.unlink(missing_ok=True)
        if not reuse:
            ROADMAP_CSV.unlink(missing_ok=True)
    target, engine = load_engine()

    if not reuse:
        report = ReccmpStatusReport(filename=target.original_path.name)
        for match in engine.compare_all():
            match_type = getattr(match, "type", None)
            if (
                match_type == EntityType.FUNCTION
                and match.name in target.report_config.ignore_functions
            ):
                continue
            report.add_match(match)
        out.write_text(serialize_reccmp_report(report, diff_included=True), encoding="utf-8")

    if roadmap:
        _write_roadmap(target, engine, ROADMAP_CSV)

    if _stamp() != input_stamp:
        raise RuntimeError("Comparison inputs changed during the run. Wait for the build or edits to finish, then rerun.")
    if canonical:
        RECCMP_STAMP.write_text(json.dumps(input_stamp) + "\n", encoding="utf-8")

    return out

    return out


def check_decomplint(
    paths: list[Path | str] | None = None,
    target: str = "LEMBALL",
    warnfail: bool = True,
    encoding: str = "utf-8",
) -> int:
    command = [sys.executable, "-m", "reccmp.tools.decomplint", "--encoding", encoding]
    if target is not None:
        command.extend(["--target", target])
    if warnfail:
        command.append("--warnfail")
    command.extend(str(path) for path in (paths or [SRC]))
    return subprocess.run(command, check=False).returncode
