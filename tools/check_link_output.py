#!/usr/bin/env python3

from pathlib import Path
import subprocess
import sys


args = sys.argv[2:]

for arg in args:
    if arg.startswith("@"):
        rsp_path = Path(arg[1:])
        if rsp_path.exists():
            content = rsp_path.read_text(encoding="utf-8", errors="ignore")
            tokens = content.split()
            rsp_path.write_text("\n".join(tokens) + "\n", encoding="utf-8")

output = next((Path(arg[5:]) for arg in args if arg.upper().startswith("/OUT:")), None)
result = subprocess.run([sys.argv[1], *args])
if not result.returncode and (output is None or not output.exists()):
    sys.exit(f"linker produced no output: {output}")
sys.exit(result.returncode)