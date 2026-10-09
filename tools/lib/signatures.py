"""Parse C++ function identities and display decoded catalog signatures."""

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


def adjacent_signature(code, offset, ranges):
    """Read the class and method from the declaration after an annotation."""
    declaration = code[offset:].lstrip()
    start = len(code) - len(declaration)
    end = re.search(r"[;{}#]", declaration)
    if end:
        declaration = declaration[: end.start()]
    match = next(
        (
            match
            for match in FUNCTION.finditer(declaration)
            if match["method"] not in WORDS
        ),
        None,
    )
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
    return Signature(owner, method, None)
