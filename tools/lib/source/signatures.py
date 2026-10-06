"""Parse C++ declarations into normalized function signatures."""

import re
from dataclasses import dataclass


@dataclass(frozen=True)
class Signature:
    owner: str
    method: str
    parameters: tuple[str, ...] | None  # None: an unmangled name has no type evidence.
    const: bool = False

    def display(self):
        method = self.method
        if method in ("<constructor>", "<destructor>"):
            method = ("~" if method == "<destructor>" else "") + self.owner.split("::")[
                -1
            ]
        name = f"{self.owner}::{method}" if self.owner else method
        args = "?" if self.parameters is None else ", ".join(self.parameters)
        return f"{name}({args})" + (" const" if self.const else "")


TYPE_DEF = re.compile(
    r"\b(?:class|struct)\s+(?P<name>\w+)\s*(?:final\s*)?(?::[^;{}]*)?\{"
)


def delimiter_ends(code: str, opening: str, closing: str) -> dict[int, int]:
    """Map each opening delimiter to its matching closing delimiter."""
    ends, stack = {}, []
    for pos, char in enumerate(code):
        if char == opening:
            stack.append(pos)
        elif char == closing and stack:
            ends[stack.pop()] = pos
    return ends


FUNCTION = re.compile(
    r"(?:(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*::\s*)?"
    r"(?P<method>operator\s*(?:new\b|delete\b|[A-Za-z_]\w*|[^\w\s(]+)|~?[A-Za-z_]\w*)\s*\("
)
WORDS = {
    "void",
    "bool",
    "char",
    "short",
    "int",
    "long",
    "float",
    "double",
    "signed",
    "unsigned",
    "const",
    "volatile",
    "wchar_t",
}
FUNCTION_PARAMETER = re.compile(
    r"(?P<result>.+?)\s*\(\s*(?P<indirection>[*&])\s*(?:[A-Za-z_]\w*)?\s*\)\s*"
    r"\((?P<parameters>.*)\)",
    re.DOTALL,
)


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


def normalize_integer_words(words):
    """Normalize optional 'int' and 'signed' without changing integer width."""
    if words == ["unsigned"]:
        return ["unsigned", "int"]
    if words in (["signed"], ["signed", "int"]):
        return ["int"]
    if "short" in words or "long" in words:
        for optional in ("int", "signed"):
            if optional in words:
                words.remove(optional)
    if not words:
        raise ValueError("missing parameter type")
    return words


def canonical_type(text):
    """Normalize spelling, preserving pointee constness and integer distinctions."""
    text = re.sub(r"\b(?:class|struct|enum|register)\s+", "", text).strip()
    if match := FUNCTION_PARAMETER.fullmatch(text):
        if re.search(
            r"\b__(?:cdecl|stdcall|fastcall|thiscall|vectorcall)\b", match["result"]
        ):
            raise ValueError("callback calling convention needs review")
        result = canonical_type(match["result"])
        raw = match["parameters"].strip()
        parameters = (
            ()
            if raw in ("", "void")
            else tuple(parameter_type(p) for p in split_parameters(raw))
        )
        if "void" in parameters or "..." in parameters[:-1]:
            raise ValueError("invalid callback parameter sequence")
        return f"{result} ({match['indirection']})({', '.join(parameters)})"
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
            words = normalize_integer_words(words)
        parts[index] = " ".join(cv + words)
    return "".join(parts)


def parameter_type(text):
    # Defaults and parameter identifiers are not signature evidence.
    text = split_parameters(text, "=")[0]
    if FUNCTION_PARAMETER.fullmatch(text):
        return canonical_type(text)
    array = re.search(r"\s*\[(?:\d+)?]\s*$", text)
    if array:
        text = text[: array.start()]
    tail = re.search(r"\b([A-Za-z_]\w*)\s*$", text)
    if tail and tail[1] not in WORDS:
        prefix = text[: tail.start()].rstrip()
        if (
            prefix
            and not prefix.endswith("::")
            and any(
                word not in {"const", "volatile", "struct", "class", "enum", "register"}
                for word in re.findall(r"[A-Za-z_]\w*|[*&]", prefix)
            )
        ):
            text = prefix
    if array:
        text += "*"  # A one-dimensional array parameter decays to a pointer.
    return canonical_type(text)


def class_ranges(code):
    ends = delimiter_ends(code, "{", "}")
    return [
        (opening, ends[opening], match["name"])
        for match in TYPE_DEF.finditer(code)
        if (opening := match.end() - 1) in ends
    ]


def adjacent_signature(code, offset, ranges):
    """Parse the declaration after an annotation; unsupported parameters stay unresolved."""
    declaration = code[offset:].lstrip()
    start = len(code) - len(declaration)
    end = re.search(r"[;{}#]", declaration)
    if end:
        declaration = declaration[: end.start()]
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
    closing = delimiter_ends(declaration, "(", ")").get(match.end() - 1)
    if closing is None:
        raise ValueError("unclosed function parameters")
    raw = declaration[match.end() : closing].strip()
    const = bool(re.match(r"\s*const\b", declaration[closing + 1 :]))
    try:
        parameters = (
            ()
            if raw in ("", "void")
            else tuple(parameter_type(p) for p in split_parameters(raw))
        )
    except ValueError:
        parameters = None
    return Signature(owner, method, parameters, const)
