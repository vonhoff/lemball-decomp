#!/usr/bin/env python3
"""Check consistency of reccmp's optional source recommendations."""

import re
from pathlib import Path

import tree_sitter_cpp
from tree_sitter import Language, Parser


SIZE = re.compile(r"\s*// SIZE (0x[0-9a-fA-F]+)\s*")
DECL = re.compile(r"\s*(?:class|struct)\s+(\w+)")
ASSERT = re.compile(r"DECOMP_SIZE_ASSERT\((\w+),\s*(0x[0-9a-fA-F]+)\)")
VTABLE = re.compile(r"\s*// VTABLE:")
VTABLE_OFFSET = re.compile(r"// vtable\+0x[0-9a-fA-F]+\s*$")
MEMBER_OFFSET = re.compile(r"// 0x[0-9a-fA-F]+\s*$")
CPP = Language(tree_sitter_cpp.language())


def walk(root):
    nodes = [root]
    while nodes:
        node = nodes.pop()
        yield node
        nodes.extend(reversed(node.children))


def is_method(field):
    for node in walk(field):
        if node.type == "function_declarator":
            declarator = node.child_by_field_name("declarator")
            if declarator and declarator.type not in {"parenthesized_declarator", "pointer_declarator"}:
                return True
    return False


def lint(root=Path("src")):
    annotations = {}
    assertions = {}
    vtables = set()
    errors = []
    member_count = vtable_count = 0
    files = sorted(path for path in root.rglob("*") if path.suffix.upper() in {".H", ".CPP"})

    for path in files:
        text = path.read_text(encoding="utf-8")
        pending = None
        pending_vtable = False
        for number, line in enumerate(text.splitlines(), 1):
            size_match = SIZE.fullmatch(line)
            if "// SIZE" in line and not size_match:
                errors.append(f"{path}:{number}: malformed SIZE annotation")
            if size_match:
                pending = (int(size_match.group(1), 16), path, number)
                continue
            if VTABLE.match(line):
                pending_vtable = True
                continue
            if match := DECL.match(line):
                name = match.group(1)
                if pending:
                    if name in annotations:
                        errors.append(f"{path}:{number}: duplicate SIZE for {name}")
                    annotations[name] = pending
                if pending_vtable:
                    vtables.add(name)
                pending = None
                pending_vtable = False
            elif line.strip() and not line.lstrip().startswith("//"):
                if pending:
                    errors.append(f"{pending[1]}:{pending[2]}: SIZE is not followed by a type")
                pending = None
                pending_vtable = False
            for match in ASSERT.finditer(line):
                name, size = match.group(1), int(match.group(2), 16)
                if name in assertions:
                    errors.append(f"{path}:{number}: duplicate size assertion for {name}")
                assertions[name] = (size, path, number)

    for name in sorted(annotations.keys() | assertions.keys()):
        annotation = annotations.get(name)
        assertion = assertions.get(name)
        if annotation is None:
            errors.append(f"{assertion[1]}:{assertion[2]}: {name} has no SIZE annotation")
        elif assertion is None:
            errors.append(f"{annotation[1]}:{annotation[2]}: {name} has no DECOMP_SIZE_ASSERT")
        elif annotation[0] != assertion[0]:
            errors.append(f"{assertion[1]}:{assertion[2]}: {name} size does not match annotation")

    for path in files:
        source = path.read_bytes()
        lines = source.decode(errors="replace").splitlines()
        tree = Parser(CPP).parse(source)
        for node in walk(tree.root_node):
            if node.type not in {"class_specifier", "struct_specifier"}:
                continue
            name_node = node.child_by_field_name("name")
            body = node.child_by_field_name("body")
            if not name_node or not body:
                continue
            name = source[name_node.start_byte : name_node.end_byte].decode(errors="replace")
            for field in (child for child in body.named_children if child.type == "field_declaration"):
                line = lines[field.end_point.row] if field.end_point.row < len(lines) else ""
                kinds = {child.type for child in field.children}
                if name in annotations and "storage_class_specifier" not in kinds and not is_method(field):
                    member_count += 1
                    if not MEMBER_OFFSET.search(line):
                        errors.append(f"{path}:{field.start_point.row + 1}: member has no offset")
                if name in vtables and ("virtual" in kinds or "virtual_specifier" in kinds):
                    if not any(child.type == "destructor_name" for child in walk(field)):
                        vtable_count += 1
                        if not VTABLE_OFFSET.search(line):
                            errors.append(f"{path}:{field.start_point.row + 1}: virtual method has no vtable offset")

    for error in errors:
        print(error)
    print(
        f"{len(annotations)} sized types, {member_count} members, "
        f"{len(vtables)} vtables, {vtable_count} virtual methods, {len(errors)} errors"
    )
    return bool(errors)


if __name__ == "__main__":
    raise SystemExit(lint())
