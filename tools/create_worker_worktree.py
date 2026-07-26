#!/usr/bin/env python3
"""Create one isolated worker branch and claim from a clean coordinator checkout."""

import argparse
import re
import subprocess
from pathlib import Path

import claims


OWNER = re.compile(r"^[a-z0-9][a-z0-9-]*$")


def run(root, *command):
    subprocess.run(command, cwd=root, check=True)


def output(root, *command):
    return subprocess.check_output(command, cwd=root, text=True).strip()


def main():
    parser = argparse.ArgumentParser(description="Create an isolated claimed worker worktree")
    parser.add_argument("owner", help="lowercase worker name, for example codex-01")
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--path", type=Path)
    args = parser.parse_args()

    if not OWNER.fullmatch(args.owner):
        raise SystemExit("owner must use lowercase letters, digits, and hyphens")
    root = args.root.resolve()
    if output(root, "git", "status", "--porcelain"):
        raise SystemExit("coordinator checkout must be clean before creating a worker")
    branch = f"codex/{args.owner}"
    if output(root, "git", "branch", "--list", branch):
        raise SystemExit(f"branch already exists: {branch}")
    destination = (args.path or root.parent / f"{root.name}-{args.owner}").resolve()
    if destination.exists():
        raise SystemExit(f"worktree path already exists: {destination}")

    claims.start(root, args.owner)
    run(root, "git", "add", "CLAIMS.md")
    run(root, "git", "commit", "-m", f"Claim range for {args.owner}")
    try:
        run(root, "git", "worktree", "add", "-b", branch, str(destination), "HEAD")
    except subprocess.CalledProcessError:
        raise SystemExit("claim committed but worktree creation failed; remove the claim before retrying")
    print(f"worker worktree: {destination}")
    print(f"worker branch: {branch}")


if __name__ == "__main__":
    main()
