# Repository tools

Run from the repository root. Python environment: dependencies from `requirements.txt`.

| Command | Purpose |
| --- | --- |
| `python tools/build.py` | Incremental MSVC 4.00 build. `--clean-first`: clean rebuild. |
| `python tools/match.py 0x00401000` | Build, compare, show reccmp diff. Multiple addresses accepted; `--no-build`: use current artifacts. |
| `python tools/report.py` | Regenerate canonical `reccmp.json` and `report.json` in `build-msvc400`. |
| `python tools/badges.py` | Export Exact, Fuzzy, and Effective code badges from the current reports to `build-msvc400/badges/`. |
| `python tools/next.py` | Show 40 unfinished functions: highest raw score first, then smallest size and address. `--limit N`: row count; `0`: all. |
| `python tools/gate.py` | Check comments, reconstruction smells, class/file layout, upstream annotations, names, and tool tests. |
| `python tools/gate.py --path src/Visos/Graphics/CWnd.cpp` | Restrict source checks; omit tool tests. Repeat `--path` for more paths. |
| `python tools/gate.py --names` | Catalog name/signature review details only. Supports `--path`. |
| `python tools/gate.py --vtable` | Vtable exact-match summary only; current executable/PDB. |

Default gate: stop at first failure. `--names` and `--vtable`: separate checks;
mutually exclusive. Pending signature reviews require Windows evidence; a gate
pass leaves them open. Vtable failures remain informational in CI.

Build: fixed `msvc400` preset; one stale-link retry; executable and PDB required.
Full log: `build-msvc400/last_build.log`. `--link`: internal CMake entry point.
`--disable-enforcements`: playable build without CD/install startup checks.

Python tests only:

```sh
cd tools
python -m unittest discover -s tests
```

## Where changes belong

Top-level scripts: commands. `lib/__init__.py`: paths, source collection, masking,
shared recognizers, reccmp setup. `lib/comments.py`, `lib/layout.py`, `lib/smell.py`:
source checks. `lib/names.py`: catalog comparison; `lib/signatures.py`: signature
decoding/parsing. Windows reviews: exact address, symbol, source signature.

Recognizers: limited C++ syntax. Mask comments/strings; preserve offsets and line
numbers. Unsupported signatures: unresolved. Parser changes: regression cases in `tests/`.

Reports: raw upstream scores; stubs zero; exact 100%; equivalent matches stay fuzzy.
Regenerate at batch boundaries. Annotation checks and comparisons: direct reccmp APIs.

Badges: exact/fuzzy from `report.json`; effective adds equivalent code from
`reccmp.json`, each function counted once. Input reports unchanged. Successful
`main` builds publish three JSON files to the `badges` branch; Shields.io reads them.

## Tool checks

Install `ruff`/`pylint` with pip. Configuration: `pyproject.toml`.

```sh
python -m ruff check tools
python -m pylint --recursive=y --reports=n tools
python tools/gate.py
```
