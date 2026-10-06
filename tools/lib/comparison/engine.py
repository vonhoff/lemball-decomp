"""Initialize the configured reccmp engine and install comparison lookups."""

from reccmp.compare import Compare
from reccmp.project.detect import RecCmpProject

from .vtables import install_vtable_lookups
from .references import install_unique_lookups
from .guards import repair_guard_symbols
from ..project import BUILD, TARGET_ID


def load_engine():
    target = RecCmpProject.from_directory(BUILD).get(TARGET_ID)
    engine = Compare.from_target(target)
    repair_guard_symbols(engine.function_comparator)
    install_vtable_lookups(engine.function_comparator)
    install_unique_lookups(engine.function_comparator)
    return target, engine
