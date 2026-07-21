# PLAN — HMS named actions + state-aware waits

Implementation plan for the two `.hms` improvements specified in
[HMS-Scripting.md](../%23Devel/HMS-Scripting.md): **named field actions**
(`click Screen.Field`) and **state-aware waits** (`wait_screen` / `wait_turn` / `wait_field`).

Tracer-bullet ordering: each phase is a vertical slice that leaves the tree building and testable. Do
not start a phase before the previous one is green. No code is written under this plan until the spec
doc is signed off.

---

## Architecture — three sources, one join key

The catalog is built from three sources, joined on **`file:line`** (stable across coordinate changes,
and the one key all three can produce):

| Source | Owns | Coverage |
|---|---|---|
| **Static signature parser** (`fields_extract.py` extended) | field **identity** + **hotkey** for every call site; **geometry** for literal/const-foldable args | all 315 call sites, no game run |
| **Constant folder** (new, feeds the static parser) | resolves `#define`/enum names in coord args so more geometry becomes static | the ~65 macro/const-only rows |
| **Runtime log** (`FIELDADD` via re-enabled `trc_prn`) | **authority** for the pixel rect of variable/sprite-sized fields; cross-check for everything else | only screens actually visited |

Measured split of the 315 call sites (rough — the arg classifier is heuristic):

- **39** — purely literal coords → static resolves geometry outright.
- **~65** — args are macro/const names only (`SCREEN_XMIN`, `WORLD_WIDTH`) → static resolves **iff** the
  constant folder is in place.
- **~148** — carry a lowercase runtime variable (loop `itr`, state `menu_shift`) → static gives the
  *formula*, not a concrete rect; runtime log is the geometry authority.
- **76** — `Add_Button_Field`/`Add_Picture_Field`: the signature has **no xmax/ymax**
  ([Fields.h:679](../../MoX/src/Fields.h#L679), [:667](../../MoX/src/Fields.h#L667)); extent comes from
  the sprite. Static gives top-left + hotkey only; runtime (or sprite-dimension lookup) owns the box.

**Consequence:** identity + hotkey are 100% static and complete — which is all the HMS naming and
rmr2hms translation actually need (a recorded click's geometry is already in the log that recorded it).
Runtime coverage only matters for *synthesizing* a click for a field never visited, and even there
static handles the ~100 literal/const-foldable rows.

---

## Phase 0 — Static catalog builder: per-type signature parsers + constant folder

Primary catalog builder. Pure tooling; no engine change; no game run; testable in isolation.

1. Extend `fields_extract.py` (or a sibling) with a **parser per add-field function**, each keyed to
   its signature so args become named/typed columns instead of one opaque `args` blob. Signatures are
   in [Fields.h](../../MoX/src/Fields.h): `Add_Hidden_Field(xmin,ymin,xmax,ymax,hotkey,help)`
   ([:686](../../MoX/src/Fields.h#L686)), `Add_Button_Field(xmin,ymin,string,pict_seg,hotkey,help)`
   ([:679](../../MoX/src/Fields.h#L679), no extent), `Add_Scroll_Field(...,width,height,...)`
   ([:702](../../MoX/src/Fields.h#L702)), `Add_Picture_Field` ([:667](../../MoX/src/Fields.h#L667)),
   `Add_Hot_Key`, `Add_Grid_Field`, etc. — the ten intercepted at
   [Fields.h:854-862](../../MoX/src/Fields.h#L854-L862).
2. **Constant folder:** the coordinate macros live in
   [MoX/src/MOM_DEF.h](../../MoX/src/MOM_DEF.h) and [MoX/src/MOX_DEF.h](../../MoX/src/MOX_DEF.h)
   (`SCREEN_XMIN/XMAX/YMIN/YMAX`, `WORLD_WIDTH`, `MAP_HEIGHT`, `SQUARE_WIDTH`, …). Build a
   `name -> value` table and constant-fold coord expressions. **Gotcha:** several are defined more than
   once with conflicting values behind resolution `#ifdef`s — e.g. `SCREEN_XMAX` is `319`
   ([MOX_DEF.h:767](../../MoX/src/MOX_DEF.h#L767)) **and** `639`
   ([:777](../../MoX/src/MOX_DEF.h#L777), [:787](../../MoX/src/MOX_DEF.h#L787)); `WORLD_WIDTH` and
   `MAP_HEIGHT` appear in both headers. The folder must pin the **320×200 branch** (the HMS coordinate
   space), not take first-match. Fold what folds; leave a symbolic form + `rt_needed=1` flag on rows
   that don't (loop/state vars, sprite extents).
3. Emit per row: `file, line, function, add_func, symbol, hotkey (decoded), xmin, ymin, xmax, ymax (or
   blank), click_cx, click_cy (when resolvable), guards, rt_needed`.

**Green when:** the static catalog carries identity + hotkey for all 315 rows and concrete geometry for
the ~100 literal/const-foldable rows; every unresolved row is flagged `rt_needed=1` with its formula
preserved.

**Verify:** `python` smoke over a fixture C file with one call of each add-field type (literal, macro,
loop-var, button) → asserts correct column extraction and the right `rt_needed` flagging. Diff the
resolved rows against the current runtime-captured `tools/fields/*.csv` for the 38 known fields — they
must agree (this is the static↔runtime cross-check).

---

## Phase 1 — Alias table + resolver; runtime log as residue-authority + cross-check

Builds naming on the static catalog; wires the runtime log in as authority for `rt_needed` geometry.

1. Create `tools/fields/aliases.csv` — `src_file, src_line, symbol, screen, alias`. Seed it with the
   fields existing scenes click (Main_Menu new/continue, Main_Screen patrol / next-turn). `screen` comes
   from the static parser's function→screen mapping; `file:line` is the join key.
2. Resolver (Python, alongside `fields_extract.py`) answering both directions off the static catalog:
   - `alias(file, line) -> Screen.Alias`
   - `resolve(Screen.Alias) -> (cx, cy)` — from static geometry when present, else from the runtime
     catalog for `rt_needed` rows.
3. **Staleness tripwire:** if the `symbol` at `file:line` no longer matches the alias row, error out —
   never silently mis-map.
4. **Re-enable the runtime instrumentation** (was the old Phase 0), now scoped to its real job —
   geometry for `rt_needed` rows and cross-checking the static parse:
   - Un-comment [Fields.c:899](../../MoX/src/Fields.c#L899) (`FIELDADD`) and
     [:905-911](../../MoX/src/Fields.c#L905-L911) (`FIELDSNAPSHOT`); both are already `#ifdef STU_DEBUG`
     with a `g_dbg_fields_trace == 0` early-return, so Release and flag-off Debug are unaffected.
   - Add a `ReMoM.ini` toggle for `g_dbg_fields_trace`.
   - Make `fields_merge.py` **accumulate** (keep existing `rt_seen=1` rows; only fill blanks) instead
     of single-run overwrite ([fields_merge.py:152](../../tools/fields_merge.py#L152)), so a traversal
     of many screens composes instead of clobbering.

**Green when:** resolver round-trips the seed rows from static alone; `rt_needed` rows resolve once a
runtime log is merged; a stale-line fixture fails loudly; a Release build still links.

**Verify:** `python` smoke over a fixture static-catalog + alias pair; `cmake --build --preset
MSVC-debug` and `--preset MSVC-release` both link (catches `trc_prn` leakage into Release); re-run
`ctest -R "HeMoM_"`.

---

## Phase 2 — Record log carries screen + call site

One-line-per-hit change to the replay field-hit callback, in both targets.

1. Extend the debug-only shadow state: alongside the `FIELDADD` emit, record each field index's
   originating `g_dbg_fields_screen_tag` + `g_dbg_fields_call_file:g_dbg_fields_call_line` into a
   parallel array keyed by field index. **Do not touch `s_Field`** — it is byte-offset-faithful to the
   disassembly ([Fields.h:90-154](../../MoX/src/Fields.h#L90-L154)).
2. Change `HeMoM_Replay_Log_Field_Hit` ([HeMoM.c:795](../../src/HeMoM.c#L795)) and the ReMoMber
   equivalent ([ReMoM.c:200](../../src/ReMoM.c#L200)) to print
   `field[N]=<screen>@<basename>:<line>` when the shadow entry exists, else the current
   index+rect form.

**Green when:** a RECORD.log from a menu click shows `field[..]=Main_Menu_Screen@MainMenu.c:410`.

**Verify:** record a short session in a Debug build; inspect the sidecar log.

---

## Phase 3 — `rmr2hms` emits named clicks

1. Teach `rmr2hms` to open the `.RMR`'s sibling `*-RECORD.log` (same stem) if present, index it by
   frame, and for each emitted click look up the frame's `screen@file:line` → alias.
2. Emit `click Screen.Alias` with the raw coords in a trailing comment; fall back to bare `click X Y`
   when no field/alias resolves.
3. `rmr2hms` is C; the resolver is Python. Either (a) shell out, or (b) have the Python merge step
   pre-bake a compact `file:line,alias,cx,cy` table `rmr2hms` reads. **(b) keeps `rmr2hms`
   dependency-free — preferred.**

**Green when:** converting a recording with a sidecar log produces named clicks; converting one
without a log produces today's output unchanged (no regression).

**Verify:** run `rmr2hms` on `assets/menu_baseline_seed12345.RMR` (has a RECORD.log) and on one
without; diff against expected.

---

## Phase 4 — Parser resolves `click Screen.Field`

Engine change, smallest that closes the loop.

1. In `Artificial_Human_Player.c`, when a `click`/`rclick` argument is `Screen.Alias` (non-numeric),
   resolve it to `(cx, cy)` at **parse time** against the baked table from Phase 3(b), so the runtime
   path is unchanged — it still stores `act->x/act->y`.
2. Unresolved name → log and skip the action (consistent with existing bad-coord handling at
   [:546](../../src/Artificial_Human_Player.c#L546)), never click `(0,0)`.

**Green when:** `click Main_Menu_Screen.New_Game_Button` clicks the button center; an unknown name is
logged and skipped.

**Verify:** a HeMoM `--scenario` smoke that clicks a named field and asserts the resulting screen
transition in the log.

---

## Phase 5 — State-aware waits

Matches the filed proposal at [__TODO-Test.md:695](../__TODO-Test.md#L695) (`act_WAIT_SCREEN`,
`act_WAIT_TURN`, `act_WAIT_FIELD`).

1. Add action types and verbs:
   - `wait_screen NAME [timeout]` — block until `current_screen == scr_NAME`
     ([MOX_T4.h:61](../../MoX/src/MOX_T4.h#L61)).
   - `wait_turn N [timeout]` — block until `_turn >= N`
     ([MOM_DAT.h:4235](../../MoX/src/MOM_DAT.h#L4235)).
   - `wait_field Screen.Alias [timeout]` — block until that field is registered and hittable.
2. `NAME` → `e_SCREENS` via a lookup table mirroring [MOM_SCR.h](../../MoM/src/MOM_SCR.h).
3. **Mandatory timeout** (reuse the `wait` `N{f|ms|s|m}` grammar; default a generous cap): on
   expiry, log an error and either abort or continue-and-fail so CI never hangs.
4. Convert `08b_som_complete.hms` (once `demo/scenes/` is restored) and one HeMoM regression scenario
   from blind `wait Nms` to state-aware waits as the proof.

**Green when:** a scenario using `wait_screen`/`wait_turn` completes; a deliberately-unreachable state
times out with a logged error instead of hanging.

**Verify:** HeMoM `--scenario` runs of both the happy path and the timeout path; full `ctest -R
"HeMoM_"` since scenarios share golden state.

---

## Phase 6 — `move` (cursor position without a click)

Small, independent parser addition. There is no verb today that warps the cursor without dispatching a
button — `Set_Pointer_Position` is only reached inside the `act_CLICK`/`act_RCLICK` case
([Artificial_Human_Player.c:742](../../src/Artificial_Human_Player.c#L742)). Useful for demo-reel
polish (a cursor that glides to a control reads better on video than a teleporting click) and for any
UI that responds to hover.

1. Add `act_MOVE` and a `move X Y` verb, mirroring the `click` coordinate parse
   ([:541-549](../../src/Artificial_Human_Player.c#L541-L549)) — same 320×200 space, same
   `sscanf("%hd %hd")`, same bad-coord log-and-skip.
2. Execution: `Set_Pointer_Position(act->x, act->y)` and update `pointer_x/pointer_y` for the scaled
   window, **without** touching `platform_frame_mouse_buttons` or calling `User_Mouse_Handler` — i.e.
   the click case minus the button dispatch.
3. Once Phase 4 lands, accept a named target too: `move Screen.Field` resolves to the field center via
   the same parse-time lookup as `click Screen.Field`.
4. Optional smoothing (`glide X Y over 500ms` interpolating across frames) is out of scope here — a
   single-frame warp is enough to unblock hover UI and can be revisited if the reel needs eased motion.

**Green when:** `move 160 120` repositions the cursor with no click registered; a following `click`
still works from the moved position.

**Verify:** HeMoM `--scenario` smoke that issues `move` then reads back the cursor / a hover-triggered
log line; confirm no spurious field-hit is logged from the `move` alone.

---

## Cross-cutting

- **Dual build systems:** any new `.c`/tool must be added to both `<dir>/CMakeLists.txt` and the
  project-root `Makefile.am`.
- **Fidelity:** no edits to `s_Field` or any disassembly-faithful structure; all name/shadow state is
  additive, `STU_DEBUG`-gated where it touches `Fields.c`.
- **Determinism:** scenario tests pin the seed with `--seed1 N`.
- **Keep in sync:** `rmr2hms`'s duplicated key constants
  ([rmr2hms.c:44-56](../../tools/rmr2hms.c#L44-L56)) vs `Platform_Keys.h`.
