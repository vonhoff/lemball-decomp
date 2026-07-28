# reccmp data sources

These CSV files contain symbol metadata that cannot be represented by a normal source implementation annotation:

- `reccmp-library-symbols.csv`: CRT, runtime, and external library functions.
- `reccmp-compiler-generated.csv`: compiler-generated functions without standalone source implementations.
- `reccmp-linker-ilts.csv`: original LINK 3.00 incremental-link entries, imported as non-progress stubs.
- `reccmp-source-symbols.csv`: by-name correlations for reconstructed or intentionally stubbed project functions whose placement cannot carry a direct annotation.

Real source implementations should use direct `FUNCTION`, `STUB`, or `LINKERILT` annotations whenever source placement and reccmp ordering permit it. Do not move difficult programmer-written functions into these inventories merely to exclude them from reconstruction progress.

Every CSV is configured under `data-sources` in `reccmp-project.yml`. The optional `source` column documents ownership and is ignored by reccmp 0.1.6.
