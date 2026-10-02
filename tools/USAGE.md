# Repository tools

Run these commands from the repository root with the Python environment containing
`requirements.txt`. No additional dependencies are needed for the tools.

| Command | Purpose |
| --- | --- |
| `python tools/build.py` | Incremental MSVC 4.00 build; full output in `build-msvc400/last_build.log`. |
| `python tools/build.py --clean-first` | Clean rebuild, including removal of stale executable/PDB artifacts. |
| `python tools/match.py 0x00401000` | Build and compare one original address using upstream reccmp. Accepts multiple addresses. |
| `python tools/match.py 0x00401000 --no-build` | Compare using the current executable and PDB. |
| `python tools/report.py` | Regenerate canonical `reccmp.json`, `roadmap.csv`, and `report.json` in `build-msvc400`. |
| `python tools/next.py --kind near` | Rank unfinished functions by raw score, then size and address. |
| `python tools/next.py --kind gain` | Rank by size times raw score. `--limit 0` shows every row. |
| `python tools/gate.py` | Check comments, reconstruction smells, class/file layout, upstream annotations, names, and tool tests. |
| `python tools/gate.py --path src/Visos/Graphics/CWnd.cpp` | Restrict source checks; omit tool tests. Repeat `--path` for more paths. |
| `python tools/gate.py --tools` | Run the Python tests without building the game. |
| `python tools/gate.py --names --verbose` | Inspect catalog name and signature comparisons, including pending Windows reviews. |
| `python tools/gate.py --names-strict` | Also fail on case differences and pending signature reviews. |
| `python tools/gate.py --names-json` | Emit the catalog comparison data as JSON. |
| `python tools/gate.py --vtable` | Compare vtables using the current executable/PDB; `--verbose` shows diffs. |
| `python tools/gate.py --all` | Run the default gate and then compare vtables. |

Standalone gate selectors run only their selected check. With `--all`, source checks
run first and stop at the first failure. A normal names pass leaves signature reviews
open; it does not resolve Windows ABI questions.

The build wrapper retries a stale link once and requires both the executable and
PDB. Its `--link` entry point belongs to the CMake toolchain; keep the legacy linker
path and response-file handling there.

## Where changes belong

The five top-level scripts handle commands. `lib/__init__.py` owns repository paths,
source collection, comment/string masking, shared delimiter/type recognition, and
upstream engine setup. `lib/comments.py`, `lib/layout.py`, and `lib/smell.py` own their
source checks. `lib/names.py` validates and compares independent catalog evidence;
`lib/signatures.py` decodes catalog signatures and reads C++ declarations. Keep
Windows reviews tied to their exact address, symbol, and source signature.

Source recognizers are deliberately limited, not a C++ compiler. Mask comments and
strings before structural matching, preserve offsets and line numbers, and report
unsupported signatures as unresolved. Add regression cases to `tests/` when changing
a recognizer. Run `python tools/gate.py` and `python -m ruff check tools` afterward.

Reports retain raw upstream scores: stubs score zero, exact means 100%, and effective
matches remain fuzzy. Keep native comparison reports separate from this canonical
report. Regenerate full reports at batch boundaries, not for each source trial.
