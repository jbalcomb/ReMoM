"""
test_runtime.py — RECORD.log geometry harvest + resolver runtime overlay.

Hermetic: crafts a RECORD.log and a catalog/alias pair in memory / temp files;
no repo scan, no committed tables.
"""
import os
import tempfile

from ..resolver import Resolver
from ..runtime import load_runtime, merge_logs, parse_record_log

RECORD_LOG = (
    "idx=6   mouse=(290,180) btn=1  field[13]=(280,176)-(312,184)@MainScr.c:2039\n"
    "idx=7   mouse=(158,100) btn=0  field[3]=(0,20)-(239,199)@MainScr.c:2019\n"
    "idx=8   mouse=(0,0)     btn=0  no field here\n"
)


def _write(text, suffix):
    fd, path = tempfile.mkstemp(suffix=suffix)
    os.close(fd)
    with open(path, "w", encoding="utf-8") as f:
        f.write(text)
    return path


def test_parse_record_log():
    p = _write(RECORD_LOG, "-RECORD.log")
    try:
        rects = parse_record_log(p)
    finally:
        os.remove(p)
    assert rects["MainScr.c:2039"] == (280, 176, 312, 184)
    assert rects["MainScr.c:2019"] == (0, 20, 239, 199)
    assert len(rects) == 2  # the "no field" line is ignored


def test_merge_and_load_roundtrip():
    log = _write(RECORD_LOG, "-RECORD.log")
    fd, out = tempfile.mkstemp(suffix=".fwv")
    os.close(fd)
    try:
        added, total = merge_logs([log], out=out)
        assert (added, total) == (2, 2)
        loaded = load_runtime(out)
        assert loaded["MainScr.c:2039"] == (280, 176, 312, 184)
        # Re-merging the same log adds nothing (fill-only accumulate).
        added2, total2 = merge_logs([log], out=out)
        assert (added2, total2) == (0, 2)
    finally:
        os.remove(log)
        os.remove(out)


def test_resolver_runtime_overlay():
    # A runtime=1 catalog row (no static click point) resolves via the overlay.
    catalog = [{"file": "MoM/src/MainScr.c", "line": 2039, "symbol": "_patrol_button",
                "click_cx": "", "click_cy": ""}]
    aliases = [{"src_file": "MoM/src/MainScr.c", "src_line": "2039", "symbol": "_patrol_button",
                "screen": "Main_Screen", "alias": "Patrol_Button"}]
    overlay = {"MainScr.c:2039": (280, 176, 312, 184)}

    # Without the overlay: unresolved (runtime).
    r0 = Resolver(catalog, aliases)
    assert r0.resolve("Main_Screen.Patrol_Button") == (None, "runtime")

    # With the overlay: center of the runtime rect.
    r1 = Resolver(catalog, aliases, overlay)
    assert r1.resolve("Main_Screen.Patrol_Button") == ((296, 180), "runtime-log")


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
