"""Run reccmp via the Python API; reuse JSON when binary mtimes are unchanged."""

from __future__ import annotations

import json
import logging
from pathlib import Path

from reccmp.compare import Compare
from reccmp.formats import PEImage
from reccmp.formats.exceptions import InvalidVirtualAddressError
from reccmp.project.detect import DetectWhat, RecCmpProject, detect_project
from reccmp.compare.report import ReccmpStatusReport, serialize_reccmp_report
from reccmp.tools.roadmap import ModuleMap, RoadmapRow, export_to_csv, match_type_abbreviation
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
    file_id,
)


def _stamp() -> dict:
    return {
        "version": 2,
        "original": file_id(ORIGINAL_EXE),
        "recomp": file_id(RECOMP_EXE),
        "pdb": file_id(RECOMP_PDB),
    }


def load_engine() -> tuple[object, Compare]:
    project = RecCmpProject.from_directory(BUILD)
    target = project.get("LEMBALL")
    logging.getLogger("reccmp").setLevel(logging.WARNING)
    return target, Compare.from_target(target)


_load_engine = load_engine


def _write_roadmap(target, engine: Compare, csv_path: Path) -> None:
    orig_bin = engine.orig_bin
    recomp_bin = engine.recomp_bin
    if not isinstance(orig_bin, PEImage) or not isinstance(recomp_bin, PEImage):
        raise ValueError("roadmap requires 32-bit PE images")

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
    reuse = False
    if canonical and RECCMP_JSON.exists() and RECCMP_STAMP.exists():
        try:
            reuse = json.loads(RECCMP_STAMP.read_text(encoding="utf-8")) == _stamp()
        except (OSError, ValueError):
            pass

    need_roadmap = roadmap and (not ROADMAP_CSV.exists() or not reuse)
    if reuse and not need_roadmap:
        return out

    target, engine = _load_engine()

    if not reuse:
        report = ReccmpStatusReport(filename=target.original_path.name)
        for match in engine.compare_all():
            match_type = getattr(match, "type", getattr(match, "match_type", None))
            if (
                match_type == EntityType.FUNCTION
                and match.name in target.report_config.ignore_functions
            ):
                continue
            report.add_match(match)
        out.write_text(serialize_reccmp_report(report, diff_included=True), encoding="utf-8")
        if canonical:
            RECCMP_STAMP.write_text(json.dumps(_stamp()) + "\n", encoding="utf-8")

    if roadmap:
        _write_roadmap(target, engine, ROADMAP_CSV)

    return out


def check_decomplint(
    paths: list[Path | str] | None = None,
    target: str = "LEMBALL",
    warnfail: bool = True,
    encoding: str = "utf-8",
) -> int:
    """Run reccmp decomplint checks via the Python API."""
    import argparse
    from pathlib import PurePath
    import sys
    import colorama
    from reccmp.parser.error import ParserAlert
    from reccmp.project.error import RecCmpProjectException
    from reccmp.tools.decomplint import (
        check_aliases,
        decomplint_parse_args,
        display_errors,
        lint_all_targets,
    )

    search_paths = (
        [Path(p) if not isinstance(p, Path) else p for p in paths]
        if paths
        else [ROOT / "src"]
    )
    args = argparse.Namespace(
        paths=search_paths,
        target=target,
        warnfail=warnfail,
        encoding=encoding,
    )
    try:
        lint_targets = decomplint_parse_args(args)
    except RecCmpProjectException as e:
        sys.stderr.write(f"decomplint: {e.args[0]}\n")
        return 1

    all_alerts = lint_all_targets(lint_targets)
    all_alerts.extend(check_aliases(lint_targets))

    error_count = 0
    warning_count = 0
    filtered_alerts_by_path: dict[PurePath, list[ParserAlert]] = {}

    for alert in all_alerts:
        if args.target is None or args.target == alert.target or alert.target is None:
            filtered_alerts_by_path.setdefault(alert.path, []).append(alert)

    sorted_paths = sorted(filtered_alerts_by_path.keys(), key=lambda p: str(p).lower())
    for error_path in sorted_paths:
        alerts = filtered_alerts_by_path[error_path]
        if alerts:
            error_count += sum(1 for alert in alerts if alert.is_error())
            warning_count += sum(1 for alert in alerts if alert.is_warning())

            sorted_alerts = sorted(alerts, key=lambda a: a.line_number)
            display_errors(sorted_alerts, error_path)
            print()

    print(colorama.Style.RESET_ALL, end="")

    if error_count > 0 or (warning_count > 0 and warnfail):
        return 1
    return 0
