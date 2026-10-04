#!/usr/bin/env python3
"""Preserve MSVC 4.00 compiler file identities after source relocation."""

import argparse
import json
import posixpath
import re
from pathlib import Path

INCLUDE = re.compile(r'(^\s*#include\s*")([^"\r\n]+)(")', re.M)


def write_changed(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.exists() or path.read_bytes() != data:
        path.write_bytes(data)


def stage(root, build):
    source = root / "src"
    destination = build / "compiler-src"
    manifest = json.loads((root / "cmake/source-layout.json").read_text())
    identities = {entry["source"]: identity for identity, entry in manifest.items()}
    if len(identities) != len(manifest):
        raise ValueError("Duplicate maintained source path")
    actual = {
        p.relative_to(source).as_posix()
        for p in source.rglob("*")
        if p.suffix.lower() in (".cpp", ".h", ".inl")
    }
    if actual != identities.keys():
        raise ValueError(f"Source manifest differs: {sorted(actual ^ identities.keys())}")
    inputs, units = [], []
    for identity, entry in manifest.items():
        path = source / entry["source"]
        output = destination / identity
        if not path.resolve().is_relative_to(source.resolve()):
            raise ValueError(f"Source outside src: {path}")
        if not output.resolve().is_relative_to(destination.resolve()):
            raise ValueError(f"Compiler path outside staging tree: {output}")
        occurrences = {}

        def translate(match):
            target = (path.parent / match[2]).resolve()
            if not target.exists():
                target = (source / match[2]).resolve()
            if not target.exists():
                return match[0]
            if not target.is_relative_to(source.resolve()):
                return match[0]
            target_path = target.relative_to(source.resolve()).as_posix()
            if target_path not in identities:
                raise ValueError(f"Unmapped include in {path}: {match[2]}")
            target_identity = identities[target_path]
            spelling = entry.get("includes", {}).get(
                target_identity,
                posixpath.relpath(target_identity, posixpath.dirname(identity) or "."),
            )
            if isinstance(spelling, list):
                index = occurrences.get(target_identity, 0)
                spelling = spelling[min(index, len(spelling) - 1)]
                occurrences[target_identity] = index + 1
            return match[1] + spelling + match[3]

        text = INCLUDE.sub(translate, path.read_bytes().decode("utf-8"))
        write_changed(output, text.encode("utf-8"))
        inputs.append(path)
        if path.suffix == ".cpp":
            units.append(output)
    content = ""
    for name, paths in (("LEMBALL_SOURCE_INPUTS", inputs), ("LEMBALL_SOURCES", units)):
        content += f"set({name}\n"
        content += "".join(f'    "{path.as_posix()}"\n' for path in paths)
        content += ")\n"
    write_changed(build / "compiler-sources.cmake", content.encode("utf-8"))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, required=True)
    parser.add_argument("--build", type=Path, required=True)
    args = parser.parse_args()
    stage(args.root.resolve(), args.build.resolve())


if __name__ == "__main__":
    main()
