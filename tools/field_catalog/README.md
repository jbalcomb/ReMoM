# field_catalog

Clean-room static catalog of every input field (`Add_*Field`) the game registers,
built by parsing the C source directly — **no game run required**. It gives every
call site an identity and decoded hotkey, plus concrete 320×200 geometry wherever
the coordinate expression folds to constants. Sites whose geometry depends on
runtime state (loop indices, popup origins, sprite extents) are flagged
`runtime=1` for the runtime `FIELDADD` log to fill in.

This is the Phase 0 catalog builder for the HMS named-actions work — see
[doc/#AI_Plans/PLAN-HMS-Named-Actions-And-Waits.md](../../doc/%23AI_Plans/PLAN-HMS-Named-Actions-And-Waits.md)
and [doc/#Devel/HMS-Scripting.md](../../doc/%23Devel/HMS-Scripting.md).

## Run

```sh
python -m tools.field_catalog                    # writes tools/fields/catalog.fwv
python -m tools.field_catalog --out other.fwv    # custom path
python -m tools.field_catalog.tests.test_catalog # unit test (hermetic; no repo scan)
```

The output is a **fixed-width values file** (`catalog.fwv`): read it by column
offset, not by whitespace-splitting — empty numeric cells are space-padded, and
`raw_args` (the free-text tail) is the only column that may contain spaces.

## Modules

| module | role |
|---|---|
| `constants.py` | loads `#define`s from `MoX/src/MOM_DEF.h` + `MOX_DEF.h`, pins the 320×200 video-mode branch, folds integer coord expressions via a whitelisted `ast` evaluator |
| `signatures.py` | positional arg schema for all ten `Add_*Field` functions, transcribed from `MoX/src/Fields.h` |
| `scan.py` | finds call sites (comment/string-aware paren matching), enclosing function, LHS symbol, and enclosing `if`/`for`/`while`/`switch` guards; filters out the function definitions themselves |
| `build.py` | maps each call's args through its signature, folds geometry, decodes hotkeys, classifies `runtime`, writes the `.fwv` |
| `resolver.py` | maps between HMS `Screen.Alias` names and geometry via the curated `tools/fields/aliases.fwv`; `name_for(file,line)` and `resolve("Screen.Alias")`, joined on `(file, symbol)` so it survives line drift, with a staleness `audit()` |
| `fwv.py` | fixed-width values read/write shared by the catalog and the alias table |
| `tests/` | `fixture_screen.c` + `test_catalog.py` (parser) + `test_resolver.py` + `test_fwv.py` — hermetic assertions |

## Named actions (`Screen.Alias`)

`tools/fields/aliases.fwv` (`src_file, src_line, symbol, screen, alias`) is the hand-curated map from a
field's call site to a `Screen.Alias` HMS name. `resolver.py` turns a recorded click's call site into a
name and a name into a static click point:

```sh
python -m tools.field_catalog.resolver     # audit the alias table + dump resolutions
```

Resolution joins alias↔catalog on `(file, symbol)` — the symbol is stable, so an alias whose `src_line`
has drifted still resolves, and `audit()` flags the drift. Rows that are `runtime=1` in the catalog
resolve to `(runtime)` until the runtime `FIELDADD` log supplies their geometry.

## Columns

`file, line, function, symbol, add_func, geom_kind, xmin, ymin, xmax, ymax,
box_w, box_h, cols, rows, click_cx, click_cy, hotkey_value, hotkey_name,
hotkey_unresolved, runtime, guards, raw_args`

- **geom_kind** — how the clickable extent is derived:
  `rect` (explicit xmax/ymax), `wh` (xmin,ymin + width,height), `w_only`
  (width only; height not in the call), `grid` (box_w/box_h × cols/rows,
  **inclusive** last pixel), `sprite` (top-left only; extent from the sprite),
  `none` (bare hotkey — key-addressable, no geometry).
- **click_cx/cy** — a safe click point, present only when geometry fully folded.
- **hotkey_value/name** — decoded key; `hotkey_unresolved=1` when the hotkey is a
  string's first char (`str_hotkey_ESC__ovr076[0]`) or an out-of-scope constant.
- **runtime** — `1` when the pixel geometry could not be resolved statically and
  the runtime `FIELDADD` log is the authority. `0` for fully-folded and for
  key-addressable rows.
- **guards** — enclosing control-flow condition(s), outermost first, `&&`-joined.

## Coverage & validation (current)

- 374 call sites. **187 (50%) statically addressable with zero runtime data** —
  102 resolve a click point, 85 are key-addressable. 187 flagged `runtime=1`.
- 230 rows carry a `guards` precondition.
- **Static↔runtime cross-check: 56/56 folded coordinates agree** with the
  runtime-captured `tools/fields/*.csv` (joined on `(file, symbol)`). This check
  caught the grid off-by-one now fixed.

## Known limitations

- **Hotkey values**: only ~24/85 key rows resolve to a concrete key — the rest
  pass a string's first char or a `cnst_HOTKEY_*` constant outside the DEF
  headers. The **symbol name** stays legible (`hotkey_escape`, `cnst_HOTKEY_D_4`).
- **Braceless guards**: only braced `{...}` blocks are scanned. No braceless
  `if(...) Add_*(...)` one-liners exist before an `Add_*` call today.
- **Screen attribution**: the `function` column is the enclosing C function, not
  a normalized screen name. Helpers (`Add_Unit_Action_Fields` → `Main_Screen`)
  still need a function→screen map — deferred to the alias/resolver phase.
