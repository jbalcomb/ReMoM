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
    def __init__(self, catalog_rows, alias_rows):
        self.by_fileline = {(r["file"], int(r["line"])): r for r in catalog_rows}
        self.by_filesym = {(r["file"], r["symbol"]): r for r in catalog_rows if r["symbol"]}
        self.aliases = alias_rows
        self.alias_by_filesym = {(a["src_file"], a["symbol"]): a for a in alias_rows}
        self.alias_by_qualified = {f"{a['screen']}.{a['alias']}": a for a in alias_rows}

    @classmethod
    def from_repo(cls, alias_path=None):
        from .build import build_catalog
        return cls(build_catalog(), load_aliases(alias_path))

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
        'runtime' (field has no static geometry — needs the runtime log)."""
        a = self.alias_by_qualified.get(qualified)
        if a is None:
            return None, "unknown"
        row = self.by_filesym.get((a["src_file"], a["symbol"]))
        if row is None:
            return None, "missing"
        if row["click_cx"] == "" or row["click_cy"] == "":
            return None, "runtime"
        return (int(row["click_cx"]), int(row["click_cy"])), "ok"

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


def _main():
    r = Resolver.from_repo()
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
