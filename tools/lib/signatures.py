"""Read catalog encodings and C++ declarations for signature comparisons.

CodeWarrior grammar: https://github.com/encounter/cwdemangle (CC0).
"""

import re
from dataclasses import dataclass

from . import TYPE_DEF, delimiter_ends

METHOD_NAMES = {
    "__ct": "<constructor>", "__dt": "<destructor>",
    "__as": "operator=", "__ls": "operator<<", "__nw": "operatornew",
    "__dl": "operatordelete", "__apl": "operator+=", "__pl": "operator+",
    "__eq": "operator==", "__gt": "operator>", "__ml": "operator*",
    "__mi": "operator-", "__dv": "operator/",
}
SCALARS = {
    "b": "bool", "c": "char", "s": "short", "i": "int", "l": "long",
    "x": "long long", "f": "float", "d": "double", "w": "wchar_t",
    "v": "void", "e": "...",
}


@dataclass(frozen=True)
class Signature:
    owner: str
    method: str
    parameters: tuple[str, ...] | None  # None: an unmangled name has no type evidence.
    const: bool = False

    def display(self):
        method = self.method
        if method in ("<constructor>", "<destructor>"):
            method = ("~" if method == "<destructor>" else "") + self.owner.split("::")[-1]
        name = f"{self.owner}::{method}" if self.owner else method
        args = "?" if self.parameters is None else ", ".join(self.parameters)
        return f"{name}({args})" + (" const" if self.const else "")


class Decoder:
    def __init__(self, text):
        self.text = text
        self.pos = 0

    def peek(self):
        return self.text[self.pos:self.pos + 1]

    def take(self):
        token = self.peek()
        if not token:
            raise ValueError("truncated type encoding")
        self.pos += 1
        return token

    def name(self):
        count = 1
        if self.peek() == "Q":
            self.take()
            digit = self.take()
            if digit not in "123456789":
                raise ValueError("invalid qualified-name count")
            count = int(digit)
        names = []
        for _ in range(count):
            match = re.match(r"[1-9][0-9]*", self.text[self.pos:])
            if not match:
                raise ValueError("missing name length")
            self.pos += len(match[0])
            size = int(match[0])
            name = self.text[self.pos:self.pos + size]
            if len(name) != size:
                raise ValueError("invalid name length")
            self.pos += size
            names.append(name)
        return "::".join(names)

    def type(self):
        token = self.peek()
        if token.isdigit() or token == "Q":
            return self.name()
        token = self.take()
        if token in SCALARS:
            return SCALARS[token]
        if token in "PR":
            suffix = "*" if token == "P" else "&"
            if self.peek() == "F":
                self.take()
                args = self.parameters(stop="_")
                self.take()  # parameters() stops at '_'; take() rejects end of input.
                result = self.type()
                return f"{result} ({suffix})({', '.join(args)})"
            return self.type() + suffix
        if token in "CVUS":
            qualifier = {"C": "const", "V": "volatile", "U": "unsigned", "S": "signed"}[token]
            nested = self.type()
            if token in "CV" and nested.endswith(("*", "&")):
                return nested + " " + qualifier
            return qualifier + " " + nested
        raise ValueError(f"unsupported type encoding {token!r}")

    def parameters(self, stop=""):
        parameters = []
        while self.peek() and self.peek() != stop:
            parameters.append(self.type())
        if parameters == ["void"]:
            return ()
        if "void" in parameters or "..." in parameters[:-1]:
            raise ValueError("invalid parameter sequence")
        if not parameters:
            raise ValueError("missing parameter encoding")
        return tuple(parameters)

    def signature(self, method):
        """Decode the owner, constness, and parameters after a symbol separator."""
        owner = "" if self.peek() == "F" else self.name()
        const = False
        while self.peek() and self.peek() in "CS":
            const |= self.take() == "C"
        if self.take() != "F":
            raise ValueError("missing function encoding")
        parameters = self.parameters()
        method = decode_method_name(method)
        if method in ("<constructor>", "<destructor>") and not owner:
            raise ValueError("constructor or destructor without owner")
        return Signature(owner, method, parameters, const)


def decode_method_name(method):
    """Translate CodeWarrior constructors, operators, and conversion names."""
    if method.startswith("__op"):
        conversion = Decoder(method[4:])
        name = "operator " + conversion.type()
        if conversion.peek():
            raise ValueError("trailing conversion encoding")
        return name
    if not method.startswith("__"):
        return method
    if method not in METHOD_NAMES:
        raise ValueError("unsupported operator " + method)
    return METHOD_NAMES[method]


def decode_signature(symbol):
    """Try separators until a complete encoding parses; identifiers may contain '__'."""
    failure = None
    for split in re.finditer(r"__(?=\d|Q\d|F)", symbol):
        try:
            return Decoder(symbol[split.end():]).signature(symbol[:split.start()])
        except ValueError as error:
            failure = str(error)
    if failure is not None:
        raise ValueError(failure)
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
        return Signature("", symbol, None)
    raise ValueError("unsupported symbol form")


FUNCTION = re.compile(
    r"(?:(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*::\s*)?"
    r"(?P<method>operator\s*(?:new\b|delete\b|[A-Za-z_]\w*|[^\w\s(]+)|~?[A-Za-z_]\w*)\s*\("
)
WORDS = {"void", "bool", "char", "short", "int", "long", "float", "double",
         "signed", "unsigned", "const", "volatile", "wchar_t"}
FUNCTION_PARAMETER = re.compile(
    r"(?P<result>.+?)\s*\(\s*(?P<indirection>[*&])\s*(?:[A-Za-z_]\w*)?\s*\)\s*"
    r"\((?P<parameters>.*)\)", re.DOTALL
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
        if re.search(r"\b__(?:cdecl|stdcall|fastcall|thiscall|vectorcall)\b", match["result"]):
            raise ValueError("callback calling convention needs review")
        result = canonical_type(match["result"])
        raw = match["parameters"].strip()
        parameters = () if raw in ("", "void") else tuple(parameter_type(p) for p in split_parameters(raw))
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
    ends = delimiter_ends(code, "{", "}")
    return [(opening, ends[opening], match["name"]) for match in TYPE_DEF.finditer(code)
            if (opening := match.end() - 1) in ends]


def adjacent_signature(code, offset, ranges):
    """Parse the declaration after an annotation; unsupported parameters stay unresolved."""
    declaration = code[offset:].lstrip()
    start = len(code) - len(declaration)
    end = re.search(r"[;{}#]", declaration)
    if end:
        declaration = declaration[:end.start()]
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
    raw = declaration[match.end():closing].strip()
    const = bool(re.match(r"\s*const\b", declaration[closing + 1:]))
    try:
        parameters = () if raw in ("", "void") else tuple(parameter_type(p) for p in split_parameters(raw))
    except ValueError:
        parameters = None
    return Signature(owner, method, parameters, const)
