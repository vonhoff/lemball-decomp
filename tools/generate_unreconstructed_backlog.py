#!/usr/bin/env python3
"""Regenerate the accepted Macintosh correlations explicitly marked unreconstructed."""
import csv

COV = "data/macintosh-symbol-coverage.csv"
OUT = "data/unreconstructed-correlations.csv"

with open(COV, encoding="utf-8-sig", newline="") as f:
    rows = list(csv.DictReader(f))


def unreconstructed(r):
    p = (r["source_analogue_paths"] or "").strip().lower()
    return r["coverage_category"] == "accepted_x86_correlation" and "unreconstructed" in p


items = [r for r in rows if unreconstructed(r)]
items.sort(key=lambda r: (r["mac_code_file"], r["x86_address"]))

with open(OUT, "w", encoding="utf-8", newline="") as f:
    w = csv.writer(f)
    w.writerow(["mac_code_file", "x86_address", "mac_mangled_name", "portable_scope", "evidence"])
    for r in items:
        w.writerow([r["mac_code_file"], r["x86_address"], r["mac_mangled_name"], r["portable_scope"], r["evidence"]])

print(f"wrote {len(items)} unreconstructed correlations to {OUT}")
