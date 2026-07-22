"""CLI: python -m tools.field_catalog [--out PATH]

Builds the static field catalog and writes it as CSV. Prints a coverage summary
(how many call sites resolve a clickable point statically vs need the runtime log).
"""
import argparse
import os
from collections import Counter

from .build import build_catalog, write_fwv

DEFAULT_OUT = os.path.join(os.path.dirname(__file__), "..", "fields", "catalog.fwv")


def main():
    ap = argparse.ArgumentParser(prog="field_catalog")
    ap.add_argument("--out", default=DEFAULT_OUT, help="output CSV path")
    args = ap.parse_args()

    rows = build_catalog()
    out = os.path.abspath(args.out)
    write_fwv(rows, out)

    total = len(rows)
    by_kind = Counter(r["geom_kind"] for r in rows)
    # A site is statically usable when it yields a click point or is key-addressable.
    click = sum(1 for r in rows if r["click_cx"] != "")
    keyed = sum(1 for r in rows if r["geom_kind"] == "none")
    rt = sum(1 for r in rows if r["runtime"] == 1)
    hk_unres = sum(1 for r in rows if r["hotkey_unresolved"] == 1)

    print(f"wrote {total} call sites -> {out}")
    print(f"  geom kinds: {dict(by_kind)}")
    print(f"  static click point: {click}")
    print(f"  key-addressable (Hot_Key/Multi): {keyed}")
    print(f"  need runtime geometry (runtime=1): {rt}")
    print(f"  statically addressable (click OR key): {click + keyed}  of {total}")
    print(f"  hotkeys unresolved (string-index/unknown): {hk_unres}")


if __name__ == "__main__":
    main()
