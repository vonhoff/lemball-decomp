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


def decode_name(text):
    """Read a length-prefixed qualified name and return the remaining encoding."""
    count = 1
    if text.startswith("Q"):
        count, text = int(text[1:2]), text[2:]
    names = []
    for _ in range(count):
        match = re.match(r"[1-9][0-9]*", text)
        if not match:
            raise ValueError("missing name length")
        text = text[match.end() :]
        size = int(match[0])
        if len(text) < size:
            raise ValueError("invalid name length")
        names.append(text[:size])
        text = text[size:]
    return "::".join(names), text


def function_identity(method, signature):
    owner = ""
    if not signature.startswith("F"):
        owner, signature = decode_name(signature)
    if not signature.lstrip("CS").startswith("F"):
        raise ValueError("missing function encoding")
    if method.startswith("__op"):
        method = "operator " + decode_name(method[4:])[0]
    elif method.startswith("__"):
        method = METHOD_NAMES[method]
    return owner, method


def decode_identity(symbol):
    """Decode the owner and method; parameter types are platform-specific."""
    for split in re.finditer(r"__(?=\d|Q\d|F)", symbol):
        try:
            return function_identity(symbol[: split.start()], symbol[split.end() :])
        except (ValueError, KeyError):
            continue
    if re.fullmatch(r"[A-Za-z_]\w*", symbol):
        return "", symbol
    raise ValueError("unsupported symbol form: " + symbol)
