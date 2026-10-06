"""Additional reccmp matches after normalizing direct jump-thunk calls."""

from dataclasses import replace

from reccmp.compare.asm.parse import ParseAsm
from reccmp.types import EntityType, ImageId

from .thunks import resolve_jump_thunk


class ThunkParseAsm(ParseAsm):
    def __init__(self, image, targets, upstream):
        super().__init__(addr_test=upstream.addr_test, name_lookup=upstream.name_lookup)
        self.image, self.targets = image, targets
        self.used_thunk = False

    def sanitize(self, inst):
        _, _, mnemonic, operands = inst
        if mnemonic in ("call", "jmp") and operands.startswith("0x"):
            target = resolve_jump_thunk(self.image, self.targets, int(operands, 16))
            name = self.lookup(target, exact=True) if target is not None else None
            if name is not None:
                self.used_thunk = True
                return mnemonic, name
        return super().sanitize(inst)

    def parse_asm(self, data, start_addr):
        self.used_thunk = False
        return super().parse_asm(data, start_addr)


def additional_effective_matches(engine, comparisons: dict) -> set[int]:
    """Compare remaining functions with one-hop E9 call/jump normalization."""
    candidates = [
        match
        for match in engine.get_functions()
        if (comparison := comparisons.get(match.orig_addr)) is not None
        and comparison.is_function()
        and comparison.is_matched()
        and not comparison.is_stub
        and comparison.accuracy != 1
        and not comparison.is_effective_match
    ]
    if not candidates:
        return set()
    upstream = engine.function_comparator
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
    comparator = replace(upstream)
    comparator.orig_sanitize, comparator.recomp_sanitize = original, rebuilt
    accepted = set()
    for match in candidates:
        result = comparator.compare_function(match)
        if (original.used_thunk or rebuilt.used_thunk) and (
            result.match_ratio == 1 or result.is_effective_match
        ):
            accepted.add(match.orig_addr)
    return accepted
