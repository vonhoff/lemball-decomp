"""Additional Effective matches without changing raw reccmp comparisons."""

from copy import copy

from reccmp.compare.asm.parse import ParseAsm
from reccmp.compare.report import ReccmpComparedEntity
from reccmp.types import EntityType, ImageId

from .thunks import resolve_jump_thunk
from .normalize import normalize


class ThunkParseAsm(ParseAsm):
    def __init__(self, image, targets, upstream):
        super().__init__(addr_test=upstream.addr_test, name_lookup=upstream.name_lookup)
        self.image, self.targets = image, targets
        self.symbols = set()

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic in ("call", "jmp") and operands.startswith("0x"):
            target = resolve_jump_thunk(self.image, self.targets, int(operands, 16))
            name = self.lookup(target, exact=True) if target is not None else None
            if name is not None:
                return mnemonic, name
        return super().sanitize(inst)

    def lookup(self, addr, exact=False, indirect=False):
        name = super().lookup(addr, exact=exact, indirect=indirect)
        if name is not None:
            self.symbols.add(name)
        return name

    def parse_asm(self, data, start_addr):
        self.symbols = set()
        self.data, self.start_addr = data, start_addr
        self.assembly = super().parse_asm(data, start_addr)
        return self.assembly


def additional_effective_matches(
    engine, comparisons: dict[int, ReccmpComparedEntity]
) -> set[int]:
    """Try thunk equivalence, then equality under conservative normalization."""
    candidates = [
        match
        for match in engine.get_functions()
        if (comparison := comparisons.get(match.orig_addr)) is not None
        and comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and comparison.effective_accuracy != 1
    ]
    if not candidates:
        return set()
    upstream = copy(engine.function_comparator)
    functions = list(upstream.db.get_matches_by_type(EntityType.FUNCTION))
    original = ThunkParseAsm(
        upstream.orig_bin,
        {function.addr(ImageId.ORIG) for function in functions},
        upstream.orig_sanitize,
    )
    rebuilt = ThunkParseAsm(
        upstream.recomp_bin,
        {function.addr(ImageId.RECOMP) for function in functions},
        upstream.recomp_sanitize,
    )
    accepted = set()
    upstream.orig_sanitize, upstream.recomp_sanitize = original, rebuilt
    for match in candidates:
        result = upstream.compare_function(match)
        if result.match_ratio == 1 or result.is_effective_match:
            accepted.add(match.orig_addr)
            continue
        orig_normalized = normalize(
            original.assembly, original.data, original.start_addr, original.symbols
        )
        recomp_normalized = normalize(
            rebuilt.assembly, rebuilt.data, rebuilt.start_addr, rebuilt.symbols
        )
        if orig_normalized is not None and orig_normalized == recomp_normalized:
            accepted.add(match.orig_addr)
    return accepted
