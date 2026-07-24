"""
resolver.py — map between HMS named actions (`Screen.Alias`) and field geometry,
using the curated alias table (tools/fields/aliases.fwv) over the static catalog.

Two directions:
  name_for(file, line)  -> "Screen.Alias"   (a recorded click's call site -> a name)
  resolve("Screen.Alias") -> (cx, cy)        (a name -> a static click point)

Join robustness: line numbers drift when source moves, so resolution joins on
(file, symbol) — the symbol is stable. src_line in aliases.fwv is kept for humans
and the RECORD.log path; audit() reports rows whose line has drifted or whose
symbol no longer exists (the staleness tripwire).

The resolver operates on catalog row dicts (from build_catalog()), not the .fwv
file, so it is decoupled from serialization and easy to unit-test.
"""
import os

from . import fwv

ALIASES_FWV = os.path.join(os.path.dirname(__file__), "..", "fields", "aliases.fwv")
ALIAS_COLUMNS = ["src_file", "src_line", "symbol", "screen", "alias"]
_ALIAS_NUMERIC = {"src_line"}


def load_aliases(path=None):
    return fwv.read(path or ALIASES_FWV)


def save_aliases(rows, path=None):
    fwv.write(rows, ALIAS_COLUMNS, _ALIAS_NUMERIC, path or ALIASES_FWV)


class Resolver:
    def __init__(self, catalog_rows, alias_rows, runtime_rects=None):
        self.by_fileline = {(r["file"], int(r["line"])): r for r in catalog_rows}
        self.by_filesym = {(r["file"], r["symbol"]): r for r in catalog_rows if r["symbol"]}
        self.aliases = alias_rows
        self.alias_by_filesym = {(a["src_file"], a["symbol"]): a for a in alias_rows}
        self.alias_by_qualified = {f"{a['screen']}.{a['alias']}": a for a in alias_rows}
        # Runtime geometry overlay, keyed by "basename:line" (from RECORD.logs).
        self.runtime_rects = runtime_rects or {}

    @classmethod
    def from_repo(cls, alias_path=None):
        from .build import build_catalog
        from .runtime import load_runtime
        return cls(build_catalog(), load_aliases(alias_path), load_runtime())

    def _runtime_point(self, row):
        """Center from the runtime geometry overlay for a row, or None."""
        origin = f"{os.path.basename(row['file'])}:{row['line']}"
        rect = self.runtime_rects.get(origin)
        if rect is None:
            return None
        x1, y1, x2, y2 = rect
        return ((x1 + x2) // 2, (y1 + y2) // 2)

    def name_for(self, file, line):
        """A recorded click's call site (file, current line) -> 'Screen.Alias' or None.
        Joins the call site to the catalog by line, then to an alias by the row's
        stable symbol — so it survives alias-table line drift."""
        row = self.by_fileline.get((file, int(line)))
        if not row or not row["symbol"]:
            return None
        a = self.alias_by_filesym.get((file, row["symbol"]))
        return f"{a['screen']}.{a['alias']}" if a else None

    def resolve(self, qualified):
        """'Screen.Alias' -> (cx, cy). Returns (None, reason) when unresolved:
        'unknown' (no such alias), 'missing' (symbol not in catalog),
        'runtime' (no static geometry AND no runtime-log rect for it yet).
        Static geometry wins; otherwise the runtime-log overlay supplies the center."""
        a = self.alias_by_qualified.get(qualified)
        if a is None:
            return None, "unknown"
        row = self.by_filesym.get((a["src_file"], a["symbol"]))
        if row is None:
            return None, "missing"
        if row["click_cx"] != "" and row["click_cy"] != "":
            return (int(row["click_cx"]), int(row["click_cy"])), "ok"
        pt = self._runtime_point(row)
        if pt is not None:
            return pt, "runtime-log"
        return None, "runtime"

    def export_lookup_rows(self):
        """Rows for the baked rmr2hms lookup: 'basename:line' -> 'Screen.Alias'.
        Uses each alias's CURRENT catalog line (by symbol) so the origin matches
        what the RECORD.log stamps (both come from current source). Aliases whose
        symbol is missing from the catalog are skipped."""
        rows = []
        for a in self.aliases:
            row = self.by_filesym.get((a["src_file"], a["symbol"]))
            if row is None:
                continue
            origin = f"{os.path.basename(a['src_file'])}:{row['line']}"
            rows.append({"origin": origin, "name": f"{a['screen']}.{a['alias']}"})
        return rows

    def export_points_rows(self):
        """Rows for the parser's name->click-point table: 'Screen.Alias' -> (cx, cy).
        Only aliases with static geometry appear; runtime=1 fields (sprites, popup
        origins) are skipped until runtime geometry is merged."""
        rows = []
        for a in self.aliases:
            pt, _ = self.resolve(f"{a['screen']}.{a['alias']}")
            if pt is None:
                continue
            rows.append({"name": f"{a['screen']}.{a['alias']}", "cx": str(pt[0]), "cy": str(pt[1])})
        return rows

    def audit(self):
        """Staleness report: one (qualified, status, detail) per alias row.
        status in {'ok', 'drift', 'missing'}."""
        out = []
        for a in self.aliases:
            qual = f"{a['screen']}.{a['alias']}"
            key_line = (a["src_file"], int(a["src_line"]))
            row = self.by_fileline.get(key_line)
            if row is not None and row["symbol"] == a["symbol"]:
                out.append((qual, "ok", f"{a['src_file']}:{a['src_line']} {a['symbol']}"))
                continue
            cur = self.by_filesym.get((a["src_file"], a["symbol"]))
            if cur is not None:
                out.append((qual, "drift", f"line {a['src_line']} -> {cur['line']} for {a['symbol']}"))
            else:
                out.append((qual, "missing", f"symbol {a['symbol']} not found in {a['src_file']}"))
        return out


LOOKUP_FWV = os.path.join(os.path.dirname(__file__), "..", "fields", "alias_lookup.fwv")
POINTS_FWV = os.path.join(os.path.dirname(__file__), "..", "fields", "alias_points.fwv")


def _main():
    import sys
    r = Resolver.from_repo()

    if len(sys.argv) > 1 and sys.argv[1] == "export":
        lookup_out = os.path.abspath(LOOKUP_FWV)
        points_out = os.path.abspath(POINTS_FWV)
        lrows = r.export_lookup_rows()
        prows = r.export_points_rows()
        fwv.write(lrows, ["origin", "name"], set(), lookup_out)
        fwv.write(prows, ["name", "cx", "cy"], {"cx", "cy"}, points_out)
        print(f"wrote {len(lrows)} lookups -> {lookup_out}")
        print(f"wrote {len(prows)} points  -> {points_out}")
        return 0

    print("=== alias audit ===")
    stale = 0
    for qual, status, detail in r.audit():
        if status != "ok":
            stale += 1
        print(f"  [{status:7}] {qual:32} {detail}")
    print("\n=== resolution ===")
    for qual in sorted(r.alias_by_qualified):
        pt, reason = r.resolve(qual)
        print(f"  {qual:32} -> {pt if pt else '(' + reason + ')'}")
    return 1 if stale else 0


if __name__ == "__main__":
    raise SystemExit(_main())
