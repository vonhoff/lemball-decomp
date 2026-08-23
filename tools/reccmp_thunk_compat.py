"""Automatic ILT thunk symbol name normalization for reccmp."""

from reccmp.compare.db import EntityTypeLookup, ReccmpEntity


def _thunk_aware_best_name(self):
    for key in ("computed_name", "name"):
        if (value := self._kvstore.get(key)) is not None:
            val = str(value)
            if val.startswith("Thunk of '") and val.endswith("'"):
                return val[10:-1]
            return val
    return None


def _thunk_aware_match_name(self, suffix: str = ""):
    best_name = self.best_name()
    if best_name is None:
        return None
    if suffix:
        return f"{best_name}{suffix} (OFFSET)"
    ctype = EntityTypeLookup.get(self.entity_type or -1, "UNK")
    if ctype == "THUNK":
        ctype = "FUNCTION"
    return f"{best_name} ({ctype})"


ReccmpEntity.best_name = _thunk_aware_best_name
ReccmpEntity.match_name = _thunk_aware_match_name
