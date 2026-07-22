"""
test_catalog.py — hermetic unit test for the field_catalog parser.

Runs the scanner over tests/fixture_screen.c and the geometry/hotkey resolvers
over a fixed constant table (not the real DEF headers), so the assertions do not
drift when game source changes. Runnable two ways:

    python -m tools.field_catalog.tests.test_catalog     # plain runner, prints PASS/FAIL
    pytest tools/field_catalog/tests/test_catalog.py      # if pytest is installed

Each check is a `test_*` function so pytest collects them; the __main__ block
runs them without pytest.
"""
import os

from ..build import decode_hotkey, resolve_geometry
from ..constants import Folder
from ..scan import scan_file

FIXTURE = os.path.join(os.path.dirname(__file__), "fixture_screen.c")

# Hermetic constant table — mirrors the #defines in the fixture, nothing else.
FOLDER = Folder(defines={"BTN_X": "100", "BTN_Y": "50", "ST_UNDEFINED": "-1"})


def _by_symbol():
    return {r["symbol"]: r for r in scan_file(FIXTURE)}


def _geom(row):
    return resolve_geometry(row["add_func"], row["args"], FOLDER)


def test_call_sites_found_and_comment_skipped():
    rows = scan_file(FIXTURE)
    syms = {r["symbol"] for r in rows}
    assert len(rows) == 6, f"expected 6 real calls, got {len(rows)}: {sorted(syms)}"
    assert "dead_field" not in syms, "commented-out call was not skipped"
    assert syms == {"lit_field", "macro_field", "ok_btn", "esc_key", "grid_fld", "loop_field"}


def test_literal_rect_folds():
    g = _geom(_by_symbol()["lit_field"])
    assert (g["xmin"], g["ymin"], g["xmax"], g["ymax"]) == (10, 20, 30, 40)
    assert (g["click_cx"], g["click_cy"]) == (20, 30)
    assert g["runtime"] == 0


def test_macro_rect_folds_via_constants():
    g = _geom(_by_symbol()["macro_field"])
    assert (g["xmin"], g["ymin"], g["xmax"], g["ymax"]) == (100, 50, 119, 59)
    assert (g["click_cx"], g["click_cy"]) == (109, 54)
    assert g["runtime"] == 0


def test_button_is_sprite_topleft_only():
    g = _geom(_by_symbol()["ok_btn"])
    assert g["geom_kind"] == "sprite"
    assert (g["xmin"], g["ymin"]) == (200, 176)
    assert g["xmax"] == "" and g["ymax"] == ""      # extent comes from the sprite
    assert g["runtime"] == 1


def test_grid_extent_is_inclusive():
    g = _geom(_by_symbol()["grid_fld"])
    assert g["geom_kind"] == "grid"
    # 0 + 20*12 - 1 = 239 ; 0 + 18*10 - 1 = 179  (inclusive last pixel)
    assert (g["xmax"], g["ymax"]) == (239, 179)
    assert g["runtime"] == 0


def test_guarded_loopvar_partial_and_guards():
    row = _by_symbol()["loop_field"]
    g = _geom(row)
    assert g["xmin"] == 50 and g["xmax"] == 90          # x literals fold
    assert g["ymin"] == "" and g["ymax"] == ""          # y uses itr -> unfolded
    assert g["runtime"] == 1
    assert row["guards"] == "if(some_state > 0) && for(itr = 0; itr < 5; itr++)"


def test_hotkeys_decode():
    assert decode_hotkey("'Q'", FOLDER) == (81, "Q", False)
    assert decode_hotkey("'O'", FOLDER) == (79, "O", False)
    assert decode_hotkey("27", FOLDER) == (27, "ESCAPE", False)
    assert decode_hotkey("ST_UNDEFINED", FOLDER) == (None, "", False)      # sentinel -> no key
    assert decode_hotkey("hotkey_GameButton[0]", FOLDER)[2] is True        # string index -> unresolved


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
