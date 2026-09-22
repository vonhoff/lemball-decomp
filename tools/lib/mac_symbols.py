"""Decode the CodeWarrior function encodings present in the MacsBug catalog.

No return type or parameter name is inferred. Unknown encodings fail explicitly.
Grammar cross-check: https://github.com/encounter/cwdemangle (CC0).
"""

from dataclasses import dataclass
import re


OPERATORS = {
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
        if token and (token.isdigit() or token == "Q"):
            return self.name()
        token = self.take()
        if token in SCALARS:
            return SCALARS[token]
        if token in "PR":
            suffix = "*" if token == "P" else "&"
            if self.peek() == "F":
                self.take()
                args = self.parameters(stop="_")
                if self.take() != "_":
                    raise ValueError("missing function-pointer return type")
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
        while self.peek() and (not stop or self.peek() != stop):
            parameters.append(self.type())
        if parameters == ["void"]:
            return ()
        if "void" in parameters or "..." in parameters[:-1]:
            raise ValueError("invalid parameter sequence")
        if not parameters:
            raise ValueError("missing parameter encoding")
        return tuple(parameters)


def decode_signature(symbol):
    # A source identifier can itself contain '__' (e.g. CSurface__2DMemSet).
    # Accept only a separator whose complete suffix parses.
    errors = []
    for split in re.finditer(r"__(?=\d|Q\d|F)", symbol):
        method = symbol[:split.start()]
        decoder = Decoder(symbol[split.end():])
        try:
            owner = "" if decoder.peek() == "F" else decoder.name()
            const = False
            while decoder.peek() and decoder.peek() in "CS":
                const |= decoder.take() == "C"
            if decoder.take() != "F":
                raise ValueError("missing function encoding")
            parameters = decoder.parameters()
            if method == "__ct":
                method = "<constructor>"
            elif method == "__dt":
                method = "<destructor>"
            elif method.startswith("__op"):
                conversion = Decoder(method[4:])
                method = "operator " + conversion.type()
                if conversion.peek():
                    raise ValueError("trailing conversion encoding")
            elif method.startswith("__"):
                if method not in OPERATORS:
                    raise ValueError("unsupported operator " + method)
                method = OPERATORS[method]
            if method in ("<constructor>", "<destructor>") and not owner:
                raise ValueError("constructor or destructor without owner")
            return Signature(owner, method, parameters, const)
        except ValueError as error:
            errors.append(str(error))
    if errors:
        raise ValueError(errors[-1])
    if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
        return Signature("", symbol, None)
    raise ValueError("unsupported symbol form")
