"""
test_fwv.py — round-trip test for the fixed-width reader/writer.

Covers the cases that motivated left-justified headers: empty numeric cells
(must not merge into the gutter) and a free-text tail containing spaces.
"""
import os
import tempfile

from ..fwv import read, write

COLUMNS = ["name", "num", "opt", "tail"]
NUMERIC = {"num"}
ROWS = [
    {"name": "alpha", "num": "5", "opt": "", "tail": "hello world"},        # empty text cell
    {"name": "b", "num": "", "opt": "zz", "tail": "a, b, c"},               # empty numeric cell + comma tail
    {"name": "gamma", "num": "123", "opt": "q", "tail": ""},                # empty tail
]


def _roundtrip(rows):
    fd, path = tempfile.mkstemp(suffix=".fwv")
    os.close(fd)
    try:
        write(rows, COLUMNS, NUMERIC, path)
        return read(path)
    finally:
        os.remove(path)


def test_roundtrip_preserves_all_cells():
    got = _roundtrip(ROWS)
    assert got == ROWS, f"round-trip mismatch:\n{got}"


def test_empty_numeric_cell_not_lost():
    got = _roundtrip(ROWS)
    assert got[1]["num"] == "" and got[1]["opt"] == "zz"


def test_tail_with_spaces_intact():
    got = _roundtrip(ROWS)
    assert got[0]["tail"] == "hello world"
    assert got[1]["tail"] == "a, b, c"


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
