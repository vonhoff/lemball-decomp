"""Decode CodeWarrior function symbols into C++ signatures.

Grammar: https://github.com/encounter/cwdemangle (CC0).
"""

import re

from .signatures import Signature

METHOD_NAMES = {
    "__ct": "<constructor>",
    "__dt": "<destructor>",
    "__as": "operator=",
    "__ls": "operator<<",
    "__nw": "operatornew",
    "__dl": "operatordelete",
    "__apl": "operator+=",
    "__pl": "operator+",
    "__eq": "operator==",
    "__gt": "operator>",
    "__ml": "operator*",
    "__mi": "operator-",
    "__dv": "operator/",
}
SCALARS = {
    "b": "bool",
    "c": "char",
    "s": "short",
    "i": "int",
    "l": "long",
    "x": "long long",
    "f": "float",
    "d": "double",
    "w": "wchar_t",
    "v": "void",
    "e": "...",
}


class SymbolDecoder:
    def __init__(self, text):
        self.text = text
        self.pos = 0

    def peek(self):
        return self.text[self.pos : self.pos + 1]

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
            match = re.match(r"[1-9][0-9]*", self.text[self.pos :])
            if not match:
                raise ValueError("missing name length")
            self.pos += len(match[0])
            size = int(match[0])
            name = self.text[self.pos : self.pos + size]
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
            qualifier = {"C": "const", "V": "volatile", "U": "unsigned", "S": "signed"}[
                token
            ]
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
        if method.startswith("__op"):
            conversion = SymbolDecoder(method[4:])
            method = "operator " + conversion.type()
            if conversion.peek():
                raise ValueError("trailing conversion encoding")
        elif method.startswith("__"):
            if method not in METHOD_NAMES:
                raise ValueError("unsupported operator " + method)
            method = METHOD_NAMES[method]
        if method in ("<constructor>", "<destructor>") and not owner:
            raise ValueError("constructor or destructor without owner")
        return Signature(owner, method, parameters, const)


def decode_signature(symbol):
    """Try separators until a complete encoding parses; identifiers may contain '__'."""
    failure = None
    for split in re.finditer(r"__(?=\d|Q\d|F)", symbol):
        try:
            return SymbolDecoder(symbol[split.end() :]).signature(
                symbol[: split.start()]
            )
        except ValueError as error:
            failure = str(error)
    if failure is not None:
        raise ValueError(failure)
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
        return Signature("", symbol, None)
    raise ValueError("unsupported symbol form")
