#!/usr/bin/env python3
"""Verify a user-supplied original LEMBALL executable."""

import argparse
import hashlib
from pathlib import Path


EXPECTED_SHA256 = "d6337b58ccaf98df728b1490812cad0f927802d2e2c5fc932d00961f97027f63"


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    args = parser.parse_args()
    actual = sha256(args.reference)
    if actual != EXPECTED_SHA256:
        raise SystemExit(
            f"SHA-256 mismatch for {args.reference}: expected {EXPECTED_SHA256}, got {actual}"
        )
    print(f"verified {args.reference}: {actual}")


if __name__ == "__main__":
    main()