#!/usr/bin/env python3
"""MSVC 4.00 linker adapter for CMake."""

import ctypes
import os
import re
import subprocess
import sys
from pathlib import Path

MSVC_WARNING = re.compile(r"\bwarning\s+[A-Z]*\d+\s*:", re.IGNORECASE)


def win_short_path(path: str) -> str:
    absp = os.path.abspath(path)
    get_short = ctypes.WinDLL("kernel32")["GetShortPathNameW"]
    get_short.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_uint]
    get_short.restype = ctypes.c_uint
    buf = ctypes.create_unicode_buffer(32768)
    if get_short(absp, buf, 32768) and buf.value:
        return buf.value
    return absp


def main(args: list[str]) -> int:
    linker, link_args = win_short_path(args[0]), args[1:]
    # LINK 4.00 requires short working-directory and toolchain paths.
    os.chdir(win_short_path(os.getcwd()))
    for env_var in ("LIB", "INCLUDE", "PATH"):
        val = os.environ.get(env_var, "")
        if val:
            os.environ[env_var] = ";".join(
                win_short_path(p) for p in val.split(";") if p
            )

    for arg in link_args:
        if arg.startswith("@"):
            rsp_path = Path(arg[1:])
            content = rsp_path.read_text(encoding="utf-8")
            # LINK 4.00 limits lines to 16383 characters; multiple response files crash it.
            rsp_path.write_text("\n".join(content.split()) + "\n", encoding="utf-8")

    res = subprocess.run(
        [linker, *link_args],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
        check=False,
    )
    output = res.stdout
    sys.stdout.write(output)
    if MSVC_WARNING.search(output):
        sys.stderr.write("linker emitted warnings\n")
        return res.returncode or 1
    return res.returncode


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
