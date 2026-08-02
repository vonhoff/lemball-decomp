# Macintosh naming tracker

`data/macintosh-structure.json` is the durable contract for recovered class ownership and physical Windows files. It records:

- class/module state (`planned`, `partial`, `mapped`, or `blocked`);
- current or proposed source files;
- behavior each class or file must contain;
- behavior it must not absorb;
- retained mixed Windows TUs that must not be renamed after one class.

`data/macintosh-68k-symbols.csv` remains the raw Macintosh name inventory. `data/macintosh-x86-correlations.csv` remains the accepted function-level mapping. Do not copy those rows into the structure manifest.

Use:

```text
.decomp-venv/Scripts/python.exe tools/macintosh_naming.py check
.decomp-venv/Scripts/python.exe tools/macintosh_naming.py summary
.decomp-venv/Scripts/python.exe tools/macintosh_naming.py todo
.decomp-venv/Scripts/python.exe tools/macintosh_naming.py class CMine
.decomp-venv/Scripts/python.exe tools/macintosh_naming.py file src/ENGINE/CORE/LINKSCF.CPP
.decomp-venv/Scripts/python.exe tools/macintosh_naming.py names --class CMine --status unmapped
```

`tools/lint_reccmp_metadata.py` runs the tracker check automatically. A new accepted class must therefore receive a class contract, and stale physical owners fail canonical metadata validation.

Only set a class to `mapped` when its accepted Windows subset and exclusions are explicit. A class may remain `partial` even when every currently correlated method is named: the raw Macintosh family can contain methods with no proven Windows address. Use `blocked` to preserve a known rejected approach and its reason.
