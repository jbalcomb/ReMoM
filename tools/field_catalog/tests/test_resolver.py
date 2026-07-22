"""
test_resolver.py — hermetic unit test for resolver.py.

Uses hand-built catalog rows + alias rows (no repo scan, no aliases.fwv), so the
assertions are stable. Covers both directions, all resolve outcomes, and the
staleness audit (ok / drift / missing).
"""
from ..resolver import Resolver

CATALOG = [
    {"file": "a.c", "line": 416, "symbol": "_new_button", "click_cx": 159, "click_cy": 167},
    {"file": "a.c", "line": 408, "symbol": "_continue_button", "click_cx": "", "click_cy": ""},  # runtime
    {"file": "a.c", "line": 50, "symbol": "_moved_button", "click_cx": 10, "click_cy": 20},      # drifted
]
ALIASES = [
    {"src_file": "a.c", "src_line": "416", "symbol": "_new_button", "screen": "S", "alias": "New"},
    {"src_file": "a.c", "src_line": "408", "symbol": "_continue_button", "screen": "S", "alias": "Cont"},
    {"src_file": "a.c", "src_line": "99", "symbol": "_moved_button", "screen": "S", "alias": "Moved"},  # line stale
    {"src_file": "a.c", "src_line": "1", "symbol": "_gone_button", "screen": "S", "alias": "Gone"},     # symbol gone
]


def _r():
    return Resolver(CATALOG, ALIASES)


def test_name_for_hit():
    assert _r().name_for("a.c", 416) == "S.New"
    assert _r().name_for("a.c", 408) == "S.Cont"


def test_name_for_miss():
    assert _r().name_for("a.c", 999) is None      # no catalog row at that line


def test_name_for_survives_line_drift():
    # alias says line 99 but the symbol currently registers at line 50;
    # a click recorded at the current line 50 still names correctly.
    assert _r().name_for("a.c", 50) == "S.Moved"


def test_resolve_static():
    assert _r().resolve("S.New") == ((159, 167), "ok")


def test_resolve_runtime():
    assert _r().resolve("S.Cont") == (None, "runtime")


def test_resolve_unknown():
    assert _r().resolve("S.Nope") == (None, "unknown")


def test_resolve_missing_symbol():
    assert _r().resolve("S.Gone") == (None, "missing")


def test_audit_statuses():
    status = {q: s for q, s, _ in _r().audit()}
    assert status["S.New"] == "ok"
    assert status["S.Cont"] == "ok"
    assert status["S.Moved"] == "drift"       # line 99 -> 50
    assert status["S.Gone"] == "missing"      # symbol not in catalog


def _main():
    tests = [v for k, v in sorted(globals().items()) if k.startswith("test_") and callable(v)]
    failed = 0
    for t in tests:
        try:
            t()
            print(f"PASS {t.__name__}")
        except AssertionError as e:
            failed += 1
            print(f"FAIL {t.__name__}: {e}")
    print(f"\n{len(tests) - failed}/{len(tests)} passed")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(_main())
