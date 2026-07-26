#!/usr/bin/env python3
"""Coordinate isolated decompilation worker worktrees and claims."""

import argparse
import os
import re
import shutil
import subprocess
from pathlib import Path

import claims


OWNER = re.compile(r"^[a-z0-9][a-z0-9-]*$")
SHARED_DIRECTORIES = ("msvc420", ".decomp-venv")
TOOLCHAIN_CONFIG = ".worker-toolchain"


def run(root, *command):
    subprocess.run(command, cwd=root, check=True)


def output(root, *command):
    return subprocess.check_output(command, cwd=root, text=True).strip()


def primary_worktree(root):
    for line in output(root, "git", "worktree", "list", "--porcelain").splitlines():
        if line.startswith("worktree "):
            return Path(line.removeprefix("worktree ")).resolve()
    raise SystemExit("primary Git worktree not found")


def require_primary_worktree(root):
    if root != primary_worktree(root):
        raise SystemExit("run coordinator start and release from primary integration checkout")


def make_junction(target, source):
    subprocess.run(
        ["cmd.exe", "/d", "/c", "mklink", "/J", str(target), str(source)],
        check=True,
    )


def toolchain_root(primary):
    configured = os.environ.get("LEMBALL_TOOLCHAIN_ROOT")
    if configured:
        return Path(configured).resolve()
    path = primary / TOOLCHAIN_CONFIG
    if path.is_file():
        return Path(path.read_text(encoding="utf-8").strip()).resolve()
    raise SystemExit(
        "toolchain is not configured; run python tools\\worker.py configure from primary checkout"
    )


def write_reccmp_user(root, tools):
    (root / "reccmp-user.yml").write_text(
        "targets:\n  LEMBALL:\n    path: " + str(tools / "LEMBALL.EXE") + "\n",
        encoding="utf-8",
    )


def setup(root, primary=None, tools=None):
    primary = primary or primary_worktree(root)
    tools = tools or toolchain_root(primary)
    for name in SHARED_DIRECTORIES:
        source = tools / name
        target = root / name
        if not source.is_dir():
            raise SystemExit(f"missing shared dependency: {source}")
        if not target.exists():
            make_junction(target, source)
    source = tools / "LEMBALL.EXE"
    target = root / "data" / "LEMBALL.EXE"
    if not source.is_file():
        raise SystemExit(f"missing shared dependency: {source}")
    if not target.exists():
        try:
            os.link(source, target)
        except OSError:
            shutil.copy2(source, target)
    write_reccmp_user(root, tools)
    print(f"worker dependencies ready: {root}")


def configure(root, tools):
    require_primary_worktree(root)
    tools = tools.resolve()
    if tools.exists():
        raise SystemExit(f"toolchain path already exists: {tools}")
    sources = {
        "msvc420": root / "msvc420",
        ".decomp-venv": root / ".decomp-venv",
        "LEMBALL.EXE": root / "data" / "LEMBALL.EXE",
    }
    for source in sources.values():
        if not source.exists():
            raise SystemExit(f"missing local dependency: {source}")
    tools.mkdir(parents=True)
    moved = []
    try:
        for name, source in sources.items():
            destination = tools / name
            shutil.move(str(source), str(destination))
            moved.append((source, destination))
        (root / TOOLCHAIN_CONFIG).write_text(str(tools) + "\n", encoding="utf-8")
        setup(root, root, tools)
    except BaseException as error:
        for source, destination in reversed(moved):
            if destination.exists() and not source.exists():
                shutil.move(str(destination), str(source))
        if tools.exists() and not any(tools.iterdir()):
            tools.rmdir()
        if (root / TOOLCHAIN_CONFIG).exists():
            (root / TOOLCHAIN_CONFIG).unlink()
        raise SystemExit(f"toolchain migration stopped: {error}")
    print(f"shared toolchain configured: {tools}")


def remove_local_dependencies(root):
    for name in SHARED_DIRECTORIES:
        path = root / name
        if os.path.lexists(path):
            os.rmdir(path)
    path = root / "data" / "LEMBALL.EXE"
    if os.path.lexists(path):
        path.unlink()


def relocate(root, tools):
    require_primary_worktree(root)
    current = toolchain_root(root)
    tools = tools.resolve()
    if current == tools:
        print(f"shared toolchain already configured: {tools}")
        return
    if tools.exists():
        raise SystemExit(f"toolchain path already exists: {tools}")
    for name in (*SHARED_DIRECTORIES, "LEMBALL.EXE"):
        if not (current / name).exists():
            raise SystemExit(f"missing shared dependency: {current / name}")
    remove_local_dependencies(root)
    shutil.move(str(current), str(tools))
    (root / TOOLCHAIN_CONFIG).write_text(str(tools) + "\n", encoding="utf-8")
    setup(root, root, tools)
    print(f"shared toolchain moved: {tools}")


def start(root, owner, path=None):
    require_primary_worktree(root)
    tools = toolchain_root(root)
    if not OWNER.fullmatch(owner):
        raise SystemExit("owner must use lowercase letters, digits, and hyphens")
    if output(root, "git", "status", "--porcelain"):
        raise SystemExit("coordinator checkout must be clean before creating a worker")
    branch = f"codex/{owner}"
    if output(root, "git", "branch", "--list", branch):
        raise SystemExit(f"branch already exists: {branch}")
    destination = (path or root.parent / f"{root.name}-{owner}").resolve()
    if destination.exists():
        raise SystemExit(f"worktree path already exists: {destination}")

    claims.start(root, owner)
    run(root, "git", "add", "CLAIMS.md")
    run(root, "git", "commit", "-m", f"Claim range for {owner}")
    try:
        run(root, "git", "worktree", "add", "-b", branch, str(destination), "HEAD")
    except subprocess.CalledProcessError:
        raise SystemExit("claim committed but worktree creation failed; run coordinator release before retrying")
    setup(destination, root, tools)
    print(f"worker worktree: {destination}")
    print(f"worker branch: {branch}")


def release(root, owner):
    require_primary_worktree(root)
    if output(root, "git", "status", "--porcelain"):
        raise SystemExit("coordinator checkout must be clean before releasing a worker")
    claims.release(root, owner)
    run(root, "git", "add", "CLAIMS.md")
    run(root, "git", "commit", "-m", f"Release {owner} claim")


def main(argv=None):
    parser = argparse.ArgumentParser(description="Coordinate decompilation workers")
    commands = parser.add_subparsers(dest="command", required=True)
    for command in (commands.add_parser("start"), commands.add_parser("release")):
        command.add_argument("owner", help="lowercase worker name, for example codex-01")
        command.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    commands.choices["start"].add_argument("--path", type=Path)
    check = commands.add_parser("check")
    check.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    setup_command = commands.add_parser("setup")
    setup_command.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    setup_command.add_argument("--refresh", action="store_true")
    configure_command = commands.add_parser("configure")
    configure_command.add_argument(
        "--toolchain-root",
        type=Path,
        required=True,
        help="new shared directory for msvc420, decomp-venv, and LEMBALL.EXE",
    )
    configure_command.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    relocate_command = commands.add_parser("relocate")
    relocate_command.add_argument("--toolchain-root", type=Path, required=True)
    relocate_command.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args(argv)
    root = args.root.resolve()
    if args.command == "start":
        start(root, args.owner, args.path)
    elif args.command == "release":
        release(root, args.owner)
    elif args.command == "setup":
        if args.refresh:
            remove_local_dependencies(root)
        setup(root)
    elif args.command == "configure":
        configure(root, args.toolchain_root)
    elif args.command == "relocate":
        relocate(root, args.toolchain_root)
    else:
        claims.check(root)


if __name__ == "__main__":
    main()
