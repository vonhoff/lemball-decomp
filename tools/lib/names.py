"""Check compiled identities at catalog-mapped Windows addresses."""

import csv
import re
from collections import defaultdict

from reccmp.cvdump.demangler import msvc_demangle
from reccmp.types import EntityType

from link_binary import read_jump_target
from . import ROOT, thunk_symbol
from .codewarrior import decode_identity

WINDOWS_NAMES = {
    (
        0x00471AF0,
        "SysCloseSocket__14CTCPIPRWSocketFv",
        "CTCPIPCommonSocket",
        "SysCloseSocket",
    ),
    (0x0043A500, "OnZoomBox__4CWndFUc", "CWnd", "OnDriverChange"),
    (0x0045EDA0, "GetCDDir__FPCc", "CPlatformServices", "GetCDDir"),
}

FUNCTION = re.compile(
    r"(?:(?P<owner>[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*::\s*)?"
    r"(?P<method>operator\s*(?:new\b|delete\b|[A-Za-z_]\w*|[^\w\s(]+)|~?[A-Za-z_]\w*)\s*\("
)


def windows_identity(symbol):
    declaration = msvc_demangle(symbol)
    declaration = re.sub(r"`(?:adjustor|vtordisp)\{[^}]*\}'", "", declaration)
    destructor = re.search(
        r"(?P<owner>[\w:]+)::`(?:scalar|vector) deleting (?:dtor|destructor)'",
        declaration,
    )
    if destructor:
        return destructor["owner"], "<destructor>"
    match = FUNCTION.search(declaration)
    if match is None:
        raise ValueError("no function identity: " + symbol)
    owner = match["owner"] or ""
    method = re.sub(r"\s+", "", match["method"])
    leaf = owner.split("::")[-1]
    if owner and method == leaf:
        method = "<constructor>"
    elif owner and method == "~" + leaf:
        method = "<destructor>"
    return owner, method


def read_catalog(path):
    mappings = defaultdict(list)
    with path.open(newline="", encoding="utf-8-sig") as stream:
        rows = csv.reader(stream)
        next(rows)
        for _, symbol, win in rows:
            if win:
                mappings[int(win, 16)].append(symbol)
    return mappings


def compiled_identity(engine, entities, address):
    entity = entities.get(address)
    if entity is None or not entity.matched:
        raise ValueError("missing compiled implementation")
    if entity.get("stub"):
        raise ValueError("stub implementation")
    if entity.entity_type not in (
        EntityType.FUNCTION,
        EntityType.VTORDISP,
        EntityType.THUNK,
        EntityType.IMPORT_THUNK,
    ):
        raise ValueError("mapped address is not a function")
    symbol = entity.get("symbol") or ""
    if symbol == thunk_symbol(address):
        destination = read_jump_target(engine.orig_bin, address)
        body = entities.get(destination)
        if (
            body is None
            or not body.matched
            or read_jump_target(engine.recomp_bin, entity.recomp_addr)
            != body.recomp_addr
        ):
            raise ValueError("jump entry destination mismatch")
        return compiled_identity(engine, entities, destination)
    return windows_identity(symbol)


def check_names(engine, entities):
    """Require every mapping to resolve to a non-stub function with its catalog identity."""
    mappings = read_catalog(ROOT / "tools/data/mac-symbol-catalog.csv")
    errors = 0
    for address, symbols in mappings.items():
        try:
            actual = compiled_identity(engine, entities, address)
            if not any(
                decode_identity(symbol) == actual
                or (address, symbol, *actual) in WINDOWS_NAMES
                for symbol in symbols
            ):
                raise ValueError(
                    "catalog identity mismatch: " + "::".join(filter(None, actual))
                )
        except ValueError as error:
            print(f"catalog: 0x{address:08x}: {error}; expected {'; '.join(symbols)}")
            errors += 1
    print(
        f"catalog: {len(mappings) - errors}/{len(mappings)} mapped implementations; {errors} errors"
    )
    return errors
