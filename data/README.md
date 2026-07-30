# reccmp data sources

These CSV files contain symbol metadata that cannot be represented by a normal source implementation annotation:

- `objdiff-functions.csv`: source-owned functions synchronized from Ghidra `objdiff-unit:*` tags.
- `reccmp-library-symbols.csv`: reviewed CRT, runtime, and external library functions.
- `reccmp-compiler-generated.csv`: compiler-generated functions without standalone source implementations.
- `reccmp-linker-ilts.csv`: explicitly reviewed LINK 3.00 incremental-link entries. Manifest generation names every remaining ILT from its decoded direct-jump target.
- `reccmp-source-symbols.csv`: by-name correlations for reconstructed or intentionally stubbed project functions whose placement cannot carry a direct annotation.

Ghidra bootstrap metadata:

- `ghidra-evidence-function-names.csv`: the reviewed subset of source, compiler-generated, and library metadata that represents standalone functions outside the objdiff inventory.

`tools/build_ghidra_symbol_manifest.py` produces the clean-project manifest consumed by `tools/ImportLemballSymbols.java`. Non-library names must match `[a-z_][a-z0-9_]*` and are limited to 64 characters; conventional library spellings are exempt. CSV/Ghidra body-size differences remain diagnostics rather than forced body edits.

Real source implementations should use direct `FUNCTION`, `STUB`, or `LINKERILT` annotations whenever source placement and reccmp ordering permit it. Do not move difficult programmer-written functions into these inventories merely to exclude them from reconstruction progress.

Every `reccmp-*.csv` inventory is configured under `data-sources` in `reccmp-project.yml`. The optional `source` column documents ownership and is ignored by reccmp 0.1.6. The `ghidra-*.csv` files are bootstrap inputs and are not reccmp data sources.

Run `.decomp-venv\Scripts\python.exe tools\lint_reccmp_metadata.py` before building. It validates inventory type policies, configured files, owner paths, duplicate CSV addresses, duplicate source annotations, and CSV/source collisions.
