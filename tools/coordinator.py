#!/usr/bin/env python3
"""Coordinate isolated decompilation worker worktrees and claims."""

import argparse
import re
import shutil
import subprocess
from pathlib import Path

import claims


OWNER = re.compile(r"^[a-z0-9][a-z0-9-]*$")
SHARED_DIRECTORIES = ("msvc420", ".decomp-venv")
COPIED_FILES = ("reccmp-user.yml", "data/LEMBALL.EXE")


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


def setup(root, primary=None):
    primary = primary or primary_worktree(root)
    for name in SHARED_DIRECTORIES:
        source = primary / name
        target = root / name
        if not source.is_dir():
            raise SystemExit(f"missing shared dependency: {source}")
        if not target.exists():
            make_junction(target, source)
    for name in COPIED_FILES:
        source = primary / name
        target = root / name
        if not source.is_file():
            raise SystemExit(f"missing shared dependency: {source}")
        if not target.exists():
            shutil.copy2(source, target)
    print(f"worker dependencies ready: {root}")


def start(root, owner, path=None):
    require_primary_worktree(root)
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
    setup(destination, root)
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
    args = parser.parse_args(argv)
    root = args.root.resolve()
    if args.command == "start":
        start(root, args.owner, args.path)
    elif args.command == "release":
        release(root, args.owner)
    elif args.command == "setup":
        setup(root)
    else:
        claims.check(root)


if __name__ == "__main__":
    main()
