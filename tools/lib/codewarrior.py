"""Decode class and method identities from CodeWarrior function symbols.

Grammar: https://github.com/encounter/cwdemangle (CC0).
"""

import re

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


class SymbolDecoder:
    def __init__(self, text):
        self.text = text
        self.pos = 0

    def peek(self):
        return self.text[self.pos : self.pos + 1]

    def name(self):
        count = 1
        if self.peek() == "Q":
            count = int(self.text[self.pos + 1 : self.pos + 2])
            self.pos += 2
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

    def identity(self, method):
        owner = "" if self.peek() == "F" else self.name()
        while self.peek() and self.peek() in "CS":
            self.pos += 1
        if self.peek() != "F":
            raise ValueError("missing function encoding")
        if method.startswith("__op"):
            method = "operator " + SymbolDecoder(method[4:]).name()
        elif method.startswith("__"):
            method = METHOD_NAMES[method]
        return owner, method


def decode_identity(symbol):
    """Decode the owner and method; parameter types are platform-specific."""
    for split in re.finditer(r"__(?=\d|Q\d|F)", symbol):
        try:
            return SymbolDecoder(symbol[split.end() :]).identity(
                symbol[: split.start()]
            )
        except (ValueError, KeyError):
            continue
    if re.fullmatch(r"[A-Za-z_]\w*", symbol):
        return "", symbol
    raise ValueError("unsupported symbol form: " + symbol)
