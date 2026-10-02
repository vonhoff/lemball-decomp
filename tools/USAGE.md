# Repository tools

Run these commands from the repository root with the Python environment containing
`requirements.txt`. No additional dependencies are needed for the tools.

| Command | Purpose |
| --- | --- |
| `python tools/build.py` | Incremental MSVC 4.00 build; full output in `build-msvc400/last_build.log`. |
| `python tools/build.py --clean-first` | Clean rebuild, including removal of stale executable/PDB artifacts. |
| `python tools/match.py 0x00401000` | Build and compare one original address using upstream reccmp. Accepts multiple addresses. |
| `python tools/match.py 0x00401000 --no-build` | Compare using the current executable and PDB. |
| `python tools/report.py` | Regenerate canonical `reccmp.json` and `report.json` in `build-msvc400`. |
| `python tools/badges.py` | Export Exact, Fuzzy, and Effective code badges from the current reports to `build-msvc400/badges/`. |
| `python tools/next.py --kind near` | Rank unfinished functions by raw score, then size and address. |
| `python tools/next.py --kind gain` | Rank by size times raw score. `--limit 0` shows every row. |
| `python tools/gate.py` | Check comments, reconstruction smells, class/file layout, upstream annotations, names, and tool tests. |
| `python tools/gate.py --path src/Visos/Graphics/CWnd.cpp` | Restrict source checks; omit tool tests. Repeat `--path` for more paths. |
| `python tools/gate.py --names --verbose` | Inspect catalog name and signature comparisons, including pending Windows reviews. |
| `python tools/gate.py --names-strict` | Also fail on case differences and pending signature reviews. |
| `python tools/gate.py --vtable` | Compare vtables using the current executable/PDB; `--verbose` shows diffs. |
| `python tools/gate.py --all` | Run the default gate and then compare vtables. |

Standalone gate selectors run only their selected check. With `--all`, source checks
run first and stop at the first failure. A normal names pass leaves signature reviews
open; it does not resolve Windows ABI questions.

The build wrapper retries a stale link once and requires both the executable and
PDB. Its `--link` entry point belongs to the CMake toolchain; keep the legacy linker
path and response-file handling there.
Clean builds belong to `build.py --clean-first`; `match.py` performs incremental
builds. The build wrapper uses the project's fixed `msvc400` preset.

To run only the Python tests, use unittest directly from `tools`:

```sh
cd tools
python -m unittest discover -s tests
```

## Where changes belong

The top-level scripts handle commands. `lib/__init__.py` owns repository paths,
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
matches remain fuzzy. Regenerate full reports at batch boundaries, not for each source trial.

Annotation checks call reccmp's linter directly. Reporting uses one comparison
engine and upstream PDB module lookup; it needs no subprocess or temporary CSV.
In `gate.py`, argument parsing, source checks, annotation checks, and command routing
are separate functions. In `report.py`, inventory selection, module grouping, record
conversion, totals, and report assembly are separate functions. Its `main()` handles
generation and file output.
In `match.py` and `next.py`, argument parsing and output are separate from command
execution and ranking. Ranking tests live in `tests/test_next.py`, independently of
report generation tests.

Badge generation reads the canonical inventory and sizes. Exact and fuzzy values
come from `report.json`; effective adds code in upstream equivalent functions from
`reccmp.json` to exact code, counting each function once. It never modifies either
report. CI publishes only the three badge JSON files to the `badges` branch on
successful `main` builds; Shields.io reads them from GitHub's raw file host.

## Tool checks

`ruff` and `pylint` can be installed with pip. Configuration for both tools is read
from `pyproject.toml`. The checks are run with:

```sh
python -m ruff check tools
python -m pylint --recursive=y --reports=n tools
python tools/gate.py
```
