# reccmp data sources

These CSV files contain symbol metadata that cannot be represented by a normal source implementation annotation:

- `objdiff-functions.csv`: source-owned functions synchronized from Ghidra `objdiff-unit:*` tags.
- `reccmp-library-symbols.csv`: reviewed CRT, runtime, and external library functions.
- `reccmp-compiler-generated.csv`: compiler-generated functions without standalone source implementations.
- `reccmp-linker-ilts.csv`: explicitly reviewed LINK 3.00 incremental-link entries.
- `reccmp-source-symbols.csv`: by-name correlations for reconstructed or intentionally stubbed project functions whose placement cannot carry a direct annotation.

Real source implementations should use direct `FUNCTION`, `STUB`, or `LINKERILT` annotations whenever source placement and reccmp ordering permit it. Do not move difficult programmer-written functions into these inventories merely to exclude them from reconstruction progress.

Every `reccmp-*.csv` inventory is configured under `data-sources` in `reccmp-project.yml`. The optional `source` column documents repository policy ownership and is ignored by reccmp 0.1.6.

`macintosh-68k-symbols.csv` is a sibling-build evidence inventory, not a reccmp data source. It contains the original CodeWarrior MacsBug spellings recovered from the Japanese 68K application. `macintosh-x86-correlations.csv` records reviewed mappings into `/LEMBALL.EXE`, with confidence and evidence. Only reviewed correlations should change Windows names or metadata. See `docs/macintosh-sibling-evidence.md`.

Run `.decomp-venv\Scripts\python.exe tools\lint_reccmp_metadata.py` before building. It validates inventory type policies, configured files, owner paths, duplicate CSV addresses, duplicate source annotations, and CSV/source collisions.
