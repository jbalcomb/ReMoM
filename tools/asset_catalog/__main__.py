"""CLI: python -m tools.asset_catalog [--out PATH]

Scans assets/*.LBX into a fixed-width asset catalog and prints a summary.
"""
import argparse
import os
from collections import Counter

from tools.field_catalog import fwv  # shared fixed-width read/write

from .build import COLUMNS, NUMERIC, build_catalog

DEFAULT_OUT = os.path.join(os.path.dirname(__file__), "..", "fields", "assets.fwv")


def main():
    ap = argparse.ArgumentParser(prog="asset_catalog")
    ap.add_argument("--out", default=DEFAULT_OUT, help="output .fwv path")
    ap.add_argument("--assets", default=None, help="assets dir (default: repo assets/)")
    args = ap.parse_args()

    rows, skipped = build_catalog(args.assets)
    out = os.path.abspath(args.out)
    fwv.write(rows, COLUMNS, NUMERIC, out)

    by_kind = Counter(r["kind"] for r in rows)
    archives = len({r["lbx"] for r in rows})
    print(f"wrote {len(rows)} entries from {archives} archives -> {out}")
    print(f"  kinds: {dict(by_kind)}")
    if skipped:
        print(f"  skipped {len(skipped)} non-LBX files: {', '.join(skipped[:6])}{' ...' if len(skipped) > 6 else ''}")


if __name__ == "__main__":
    main()
