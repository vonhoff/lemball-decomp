#!/usr/bin/env python3
"""Compatibility wrapper for tools\\coordinator.py start."""

import sys

import coordinator


if __name__ == "__main__":
    coordinator.main(["start", *sys.argv[1:]])
