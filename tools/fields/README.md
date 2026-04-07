# ReMoM Input Field Catalog

This directory contains the catalog of every **input field** (button, hidden hotkey
region, grid, scrollbar, etc.) registered by the game, broken out per screen.
It is consumed by:

- the **Record & Replay** infrastructure (to validate that a recorded click
  landed on the intended field, and to produce stable replay files)
- the **AI / artificial-human-player input simulator** (to pick a field to
  interact with and compute a click coordinate or hotkey press)
- scenario-authoring agents that need to know what the user can do on a given
  screen

## Pipeline

```
  MoM/src/*.c + MoX/src/*.c
         |
         |  tools/fields_extract.py   (static scan)
         v
  tools/fields/_static.csv             <-- every Add_*Field call site
         |
         |  tools/fields_merge.py <TRACE.LOG>
         |         ^
         |         |  runtime capture
         |         |
         |  game built with STU_DEBUG, with g_dbg_fields_trace = 1,
         |  emits FIELDADD + FIELDSNAPSHOT lines to TRACE.LOG via trc_prn()
         v
  tools/fields/<Screen>.csv            <-- merged catalog per screen
```

### Regenerate

```sh
# 1. static side (fast, no game run needed)
python tools/fields_extract.py

# 2. runtime side: run the game with STU_DEBUG build and g_dbg_fields_trace=1
#    (currently set from a debugger, or add a temporary assignment in the
#    screen you are capturing). Exit cleanly so TRACE.LOG is flushed.

# 3. merge
python tools/fields_merge.py path/to/TRACE.LOG
```

## CSV schema (`<Screen>.csv`)

| column          | source    | meaning                                                                                 |
|-----------------|-----------|-----------------------------------------------------------------------------------------|
| `screen`        | merger    | logical screen name (helpers rolled up via `helper_to_screen` in fields_merge.py)       |
| `src_file`      | static    | source file of the `Add_*Field` call                                                    |
| `src_line`      | static    | 1-based line number in `src_file`                                                       |
| `function`      | static    | enclosing C function where the call lives                                               |
| `add_func`      | static    | which Add_* function was called (`Add_Button_Field`, `Add_Hidden_Field`, ...)           |
| `symbol`        | static    | LHS assignment target if any, e.g. `_info_button`, `g_unit_window_fields[itr_stack]`    |
| `args`          | static    | the literal argument-list text (unresolved macros, expressions, etc.)                   |
| `guards`        | static    | enclosing `if` / `for` / `while` / `switch` conditions — the field is conditional if non-empty |
| `rt_seen`       | merger    | 1 if this call site was exercised at runtime, 0 otherwise                               |
| `rt_hit_count`  | merger    | how many FIELDADD records matched this site (loops register many instances)            |
| `rt_type`       | runtime   | `e_Field_Type` enum value (`ft_Button=0`, `ft_HotKey=7`, `ft_Grid=12`, ...)             |
| `rt_x1..rt_y2`  | runtime   | bounding box in **original 320x200 game coordinates**                                   |
| `rt_click_cx`   | merger    | `(x1+x2)/2` — a safe click point inside the bbox                                        |
| `rt_click_cy`   | merger    | `(y1+y2)/2`                                                                             |
| `rt_hotkey`     | runtime   | raw hotkey value as stored in `p_fields[].hotkey`                                       |
| `rt_hotkey_name`| merger    | human-decoded hotkey: `F1`, `ESCAPE`, `N`, `ALT_P`, or `0xNN` if unknown                |
| `rt_help`       | runtime   | help-entry index (`ST_UNDEFINED` / -1 if none)                                          |
| `rt_origin`     | runtime   | which Add_* function the runtime entry came from (for sanity-checking the join)        |

## How to use this from a scenario agent

1. **Pick a field by name, not by geometry.** Use `symbol` first (`_next_turn_button`),
   then `function`/`add_func`/`guards` to disambiguate. Treat `rt_x1..rt_y2` as
   confirmation, not as the primary key.
2. **Click the center:** use `(rt_click_cx, rt_click_cy)` unless you have a
   specific reason to click an edge.
3. **Prefer hotkeys when `rt_hotkey_name` is non-empty** — they bypass mouse
   cursor state, animation, and hover timing issues.
4. **Respect `guards`.** If `rt_seen==0` and `guards` is non-empty, the field
   only exists when the guard holds (e.g. `_special_button` only when a settler
   is selected). Your scenario must set up the guard state before attempting to
   click it.

## Coordinate space

All `rt_*` coordinates are in the original game's **320x200 logical framebuffer**
space. The SDL3 platform layer scales mouse input down to this space before
dispatching, so as long as your injected clicks go through the same platform
entry points that real mouse events use (`Platform_Push_Mouse_*` / the HeMoM
input injection path), you should pass these coordinates verbatim. Do **not**
multiply by window scale yourself.

## Special cases

### Grid fields (`add_func = Add_Grid_Field`, `rt_type = 12`)

A single Add_Grid_Field call represents a whole grid of click targets. The
runtime stores:

- `rt_x1, rt_y1` — top-left of the grid
- `Param1 = box_width`, `Param2 = box_height` (visible in FIELDSNAPSHOT rows,
  not currently propagated into the merged CSV — see open issues below)
- `rt_x2, rt_y2` — outer right/bottom

To click cell `(col, row)`:

```
cx = rt_x1 + col * box_width  + box_width  / 2
cy = rt_y1 + row * box_height + box_height / 2
```

The Main Screen has two grid fields:

- the main movement map  (`MAP_WIDTH` x `MAP_HEIGHT` cells)
- the reduced (mini-)map (`REDUCED_MAP_WIDTH` x `REDUCED_MAP_HEIGHT` cells)

### Loop-registered fields (`for` loops)

Some fields are registered inside a `for` loop, so the static CSV has one row
but the runtime will produce up to N FIELDADD records with the same
`src_file:src_line` key. The merger reports this as `rt_hit_count > 1` and
currently shows the **first** runtime hit's coordinates. To get all instances,
grep `TRACE.LOG` directly for the `<basename>:<line>` of that row.

Example: `g_unit_window_fields[itr_stack]` in `Add_Unit_Window_Fields` —
one static row, up to 9 runtime instances (one per unit in the stack).

### Overlay fields

Fields are registered globally into `p_fields[]`. When a message box, advisor,
or any modal overlay is active, its fields are **on top** of the underlying
screen's fields, and the underlying ones are temporarily ignored via
`Clear_Fields_Above`. Captured coordinates for a given screen are only valid
**while that screen is the topmost with no dialog open.** If your scenario
opens a popup, capture the popup's own field set separately.

## Open issues / future work

- Grid-field `Param1`/`Param2` are in FIELDSNAPSHOT rows but not yet joined
  into `<Screen>.csv`. Add them if the scenario agent needs them directly.
- `helper_to_screen` in `fields_merge.py` is seeded for Main Screen only.
  Extend it as other screens are captured.
- No `ReMoM.ini` toggle for `g_dbg_fields_trace` yet — flip it from a debugger
  or temporarily in source.
