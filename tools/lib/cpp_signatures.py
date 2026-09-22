"""Read ordinary C++ declarations for naming audits, without guessing typedefs."""

import re

from .mac_symbols import Signature
from .source import brace_ends


FUNCTION = re.compile(
    r"(?:(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*::\s*)?"
    r"(?P<method>operator\s*(?:new\b|delete\b|[A-Za-z_]\w*|[^\w\s(]+)|~?[A-Za-z_]\w*)\s*\("
)
WORDS = set("void bool char short int long float double signed unsigned const volatile wchar_t".split())


def split_parameters(text, separator=","):
    parts, stack, start = [], [], 0
    closing = {"(": ")", "[": "]", "<": ">", "{": "}"}
    for index, char in enumerate(text):
        if char in closing:
            stack.append(closing[char])
        elif stack and char == stack[-1]:
            stack.pop()
        elif char == separator and not stack:
            parts.append(text[start:index].strip())
            start = index + 1
    if stack:
        raise ValueError("unbalanced parameter declaration")
    return parts + [text[start:].strip()]


def canonical_type(text):
    """Normalize spelling, preserving pointee constness and integer distinctions."""
    text = re.sub(r"\b(?:class|struct|enum|register)\s+", "", text).strip()
    # Complex declarators require a real type resolver. Never claim equivalence.
    if any(char in text for char in "()[]<>"):
        raise ValueError("complex parameter declarator needs review")
    if text == "...":
        return text
    if not re.fullmatch(r"[A-Za-z_][\w\s:*&]*", text):
        raise ValueError("unsupported parameter type")
    parts = re.split(r"([*&])", text)
    for index in range(0, len(parts), 2):
        words = parts[index].split()
        cv = [word for word in ("const", "volatile") if word in words]
        words = [word for word in words if word not in cv]
        # Top-level parameter cv does not participate in a C++ function type.
        if index == len(parts) - 1:
            cv = []
        if index == 0:
            if words == ["unsigned"]:
                words = ["unsigned", "int"]
            if words == ["signed"] or words == ["signed", "int"]:
                words = ["int"]
            if "int" in words and ("short" in words or "long" in words):
                words.remove("int")
            if "signed" in words and ("short" in words or "long" in words):
                words.remove("signed")
            if not words:
                raise ValueError("missing parameter type")
        parts[index] = " ".join(cv + words)
    return "".join(parts)


def parameter_type(text):
    # Defaults and parameter identifiers are not signature evidence.
    text = split_parameters(text, "=")[0]
    array = re.search(r"\s*\[(?:\d+)?\]\s*$", text)
    if array:
        text = text[:array.start()]
    tail = re.search(r"\b([A-Za-z_]\w*)\s*$", text)
    if tail and tail[1] not in WORDS:
        prefix = text[:tail.start()].rstrip()
        if prefix and not prefix.endswith("::") and any(
            word not in {"const", "volatile", "struct", "class", "enum", "register"}
            for word in re.findall(r"[A-Za-z_]\w*|[*&]", prefix)
        ):
            text = prefix
    if array:
        text += "*"  # A one-dimensional array parameter decays to a pointer.
    return canonical_type(text)


def class_ranges(code):
    ends = brace_ends(code)
    result = []
    for match in re.finditer(r"\b(?:class|struct)\s+(\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{", code):
        opening = match.end() - 1
        if opening in ends:
            result.append((opening, ends[opening], match[1]))
    return result


def adjacent_signature(code, offset, ranges):
    start = offset
    while start < len(code) and code[start].isspace():
        start += 1
    end = re.search(r"[;{}#]", code[start:])
    declaration = code[start:start + end.start()] if end else code[start:]
    match = FUNCTION.search(declaration)
    if not match:
        raise ValueError("no adjacent function declaration")
    owner = match["owner"] or "::".join(r[2] for r in ranges if r[0] < start < r[1])
    method = re.sub(r"\s+", "", match["method"])
    leaf = owner.split("::")[-1] if owner else ""
    if owner and method == leaf:
        method = "<constructor>"
    elif owner and method == "~" + leaf:
        method = "<destructor>"
    depth, closing = 1, match.end()
    while closing < len(declaration) and depth:
        depth += (declaration[closing] == "(") - (declaration[closing] == ")")
        closing += 1
    if depth:
        raise ValueError("unclosed function parameters")
    raw = declaration[match.end():closing - 1].strip()
    const = bool(re.match(r"\s*const\b", declaration[closing:]))
    try:
        parameters = () if raw in ("", "void") else tuple(parameter_type(p) for p in split_parameters(raw))
        reason = None
    except ValueError as error:
        parameters, reason = None, str(error)
    return Signature(owner, method, parameters, const), reason
