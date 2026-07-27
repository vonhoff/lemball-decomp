#!/usr/bin/env python3
"""Install repository-local reccmp compatibility hooks into current Python."""

import sysconfig
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PTH = Path(sysconfig.get_path("purelib")) / "lemball_reccmp_compat.pth"
CONTENT = f"{ROOT}\nimport tools.reccmp_ilt_compat\n"


def main():
    if not PTH.exists() or PTH.read_text(encoding="utf-8") != CONTENT:
        PTH.write_text(CONTENT, encoding="utf-8")
    print(f"reccmp compatibility: {PTH}")


if __name__ == "__main__":
    main()
