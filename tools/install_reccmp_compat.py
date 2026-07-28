#!/usr/bin/env python3
"""Install repository-local reccmp compatibility hooks into current Python."""

import sysconfig
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PURELIB = Path(sysconfig.get_path("purelib"))
MODULE = PURELIB / "_lemball_reccmp_ilt_compat.py"
PTH = PURELIB / "lemball_reccmp_compat.pth"
CONTENT = "import _lemball_reccmp_ilt_compat\n"


def main():
    module_content = (ROOT / "tools" / "reccmp_ilt_compat.py").read_text(encoding="utf-8")
    if not MODULE.exists() or MODULE.read_text(encoding="utf-8") != module_content:
        MODULE.write_text(module_content, encoding="utf-8")
    if not PTH.exists() or PTH.read_text(encoding="utf-8") != CONTENT:
        PTH.write_text(CONTENT, encoding="utf-8")
    print(f"reccmp compatibility: {PTH}")


if __name__ == "__main__":
    main()
