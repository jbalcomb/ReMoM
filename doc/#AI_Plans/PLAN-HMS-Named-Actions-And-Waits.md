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

## Phase 0 — Static catalog builder: per-type signature parsers + constant folder — **CORE DONE**

Primary catalog builder. Pure tooling; no engine change; no game run; testable in isolation.
Built clean-room as the package [tools/field_catalog/](../../tools/field_catalog/) (not derived from
the old `fields_extract.py`): `constants.py`, `signatures.py`, `scan.py`, `build.py`, `__main__.py`.
Run with `python -m tools.field_catalog`; output is [tools/fields/catalog.fwv](../../tools/fields/catalog.fwv)
(fixed-width, a **new** file that does not touch the old per-screen CSVs). The `runtime` column is what
the plan text below calls `rt_needed`.

1. **Per-add-field signature parser** — `signatures.py` transcribes all ten from
   [Fields.h](../../MoX/src/Fields.h): `Add_Hidden_Field(xmin,ymin,xmax,ymax,hotkey,help)`
   ([:686](../../MoX/src/Fields.h#L686)), `Add_Button_Field(xmin,ymin,string,pict_seg,hotkey,help)`
   ([:679](../../MoX/src/Fields.h#L679), no extent), `Add_Scroll_Field(...,width,height,...)`
   ([:702](../../MoX/src/Fields.h#L702)), `Add_Picture_Field` ([:667](../../MoX/src/Fields.h#L667)),
   `Add_Hot_Key`, `Add_Grid_Field`, etc. `scan.py` finds call sites with comment/string-aware paren
   matching, filters out the function definitions themselves, and reports correct current-source line
   numbers.
2. **Constant folder** (`constants.py`) — the coordinate macros live in
   [MoX/src/MOM_DEF.h](../../MoX/src/MOM_DEF.h) and [MoX/src/MOX_DEF.h](../../MoX/src/MOX_DEF.h). It
   loads 593 defines and pins the **320×200 branch**: several macros are defined more than once behind
   resolution `#ifdef`s — `SCREEN_XMAX` is `319` ([MOX_DEF.h:767](../../MoX/src/MOX_DEF.h#L767)) **and**
   `639` ([:777](../../MoX/src/MOX_DEF.h#L777)) — so it accepts `VIDEO_MODE_MODE_Y` and rejects the
   `_2x` branches. Coord expressions fold via a whitelisted `ast` evaluator (arithmetic + name
   resolution + cast stripping); unresolvable names (loop/state vars, popup origins) leave the row
   `runtime=1`.
3. Emits per row: `file, line, function, symbol, add_func, geom_kind, xmin, ymin, xmax, ymax, box_w,
   box_h, cols, rows, click_cx, click_cy, hotkey_value, hotkey_name, hotkey_unresolved, runtime,
   guards, raw_args`. The `guards` column captures the enclosing `if`/`for`/`while`/`switch`
   condition(s) via a brace-block scanner — 230 / 374 rows carry one, e.g. `_next_turn_button` →
   `if(all_units_moved == ST_TRUE)`, `_patrol_button` → `if(_unit_stack_count > 0)`. Braceless
   single-statement guards are the known limitation (none exist before an `Add_*` call today).

**Measured result (374 call sites — more than the old 315 because current source has more, incl. WIP
screens):**

- **187 / 374 (50%) statically addressable with zero runtime data** — 102 resolve a concrete click
  point, 85 are key-addressable (`Add_Hot_Key`/`Multi`). Matches the pre-build estimate (~100 click
  points) almost exactly.
- 187 flagged `runtime=1` (need the log for pixel geometry); 87 of those are sprite-extent buttons
  where static still gives the top-left corner.
- **Static↔runtime cross-check PASSED: 56 / 56 folded coordinates agree** with the runtime-captured
  `tools/fields/*.csv` (joined on `(file, symbol)` since old line numbers drifted), zero disagreements.
  The check caught and fixed one real bug — grid extent was the exclusive edge (`xmin+box_w*cols`); the
  game stores the inclusive last pixel (`-1`).

Documented in [tools/field_catalog/README.md](../../tools/field_catalog/README.md); locked down by a
hermetic 7-case unit test ([tests/test_catalog.py](../../tools/field_catalog/tests/test_catalog.py),
run `python -m tools.field_catalog.tests.test_catalog`) that asserts each resolution path over a
synthetic `fixture_screen.c` without scanning the repo.

**Still open in Phase 0 (deferred, non-blocking):**

- **Hotkey-value resolution** — only 24 / 85 key-addressable rows resolve to a concrete key; 180 pass
  the hotkey as a string's first char (`str_hotkey_ESC__ovr076[0]`) and 29 use `cnst_HOTKEY_*` outside
  the DEF headers. Flagged `hotkey_unresolved=1`; the symbol name stays legible for a hand-curated
  alias table.
- **Screen attribution** — the `function` column is the enclosing C function, not a normalized screen
  name; helpers (`Add_Unit_Action_Fields` → `Main_Screen`) need a function→screen map. Belongs to the
  Phase 1 alias/resolver work.

---

## Phase 1 — Alias table + resolver; runtime log as residue-authority + cross-check — **RESOLVER SLICE DONE**

Builds naming on the static catalog; wires the runtime log in as authority for `runtime=1` geometry.
The pure-Python resolver slice (steps 1–3) is done; the runtime-instrumentation half (step 4) is
deferred — it feeds `runtime=1` geometry and the RECORD.log screen stamp, which Phases 2–3 consume.

1. **DONE** — [tools/fields/aliases.fwv](../../tools/fields/aliases.fwv) seeded with the fields the
   existing scenes click: `Main_Menu_Screen.New_Game_Button` / `.Continue_Button`,
   `Main_Screen.Next_Turn_Button` / `.Patrol_Button`. `screen` is **authored** (the curated-alias
   decision), not auto-derived; columns `src_file, src_line, symbol, screen, alias`.
2. **DONE** — [resolver.py](../../tools/field_catalog/resolver.py) answers both directions off the
   in-memory catalog (decoupled from the `.fwv` serialization):
   - `name_for(file, line) -> "Screen.Alias"` — a recorded click's call site to a name.
   - `resolve("Screen.Alias") -> (cx, cy)` — static geometry when present, else `(None, "runtime")`
     for `runtime=1` rows, `"unknown"`/`"missing"` otherwise. Verified on the seed: `New_Game_Button`
     → (159,167), `Next_Turn_Button` → (280,187); `Patrol_Button`/`Continue_Button` → `(runtime)`.
   - **Join by `(file, symbol)`, not `file:line`** — symbol is stable; `name_for` survives alias-table
     line drift. 8-case hermetic test in [tests/test_resolver.py](../../tools/field_catalog/tests/test_resolver.py).
3. **DONE — staleness tripwire:** `audit()` reports each alias row `ok` / `drift` (symbol moved lines)
   / `missing` (symbol gone), instead of silently mis-mapping. `python -m tools.field_catalog.resolver`
   runs the audit + a resolution dump.
4. **DEFERRED — re-enable the runtime instrumentation** (was the old Phase 0), scoped to its real job —
   geometry for `runtime=1` rows and cross-checking the static parse:
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

## Phase 2 — Record log carries call site (`@basename:line`) — **DONE**

One-line-per-hit change to the replay field-hit callback, in both targets.

1. **DONE** — debug-only per-field-index origin shadow `g_dbg_field_origin_file/_line[]`
   ([Fields.c](../../MoX/src/Fields.c), populated in `Dbg_Trace_Field_Added` at the single Add_* choke
   point, cleared in `Clear_Fields`, read via `Dbg_Field_Origin_File/_Line`). Captured **independent of
   `g_dbg_fields_trace`** so recording gets origins without the FIELDADD trace on. **`s_Field`
   untouched** (byte-offset-faithful, [Fields.h:90-154](../../MoX/src/Fields.h#L90-L154)) — the origin
   lives in a parallel array, all under `#ifdef STU_DEBUG`.
2. **DONE** — `HeMoM_Replay_Log_Field_Hit` ([HeMoM.c:795](../../src/HeMoM.c#L795)) and the ReMoMber
   equivalent ([ReMoM.c:191](../../src/ReMoM.c#L191)) append `@<basename>:<line>` to the hit line under
   `#ifdef STU_DEBUG`, else the current rect-only form. **Screen name deliberately omitted** — the
   resolver derives the screen from the alias table, so `file:line` alone is the sufficient join key;
   `current_screen`→name mapping is an optional readability add, not required.

**Green (verified live):** a fresh RECORD.log from `HeMoM --newgame --scenario test_ai_5turns.hms
--record` shows `field[3]=(0,20)-(239,199)@MainScr.c:2019` and `field[5]=...@ItemScrn.c:1145`. Both
join to the catalog: `MainScr.c:2019` = `_main_map_grid_field` with **identical geometry**
(0,20,239,199, static `runtime=0`); `ItemScrn.c:1145` = a `runtime=1` sprite whose pixel rect the log
supplies (residue-authority working). Debug **and** Release both build; no debug-symbol leakage.

**Bridge note for Phase 3:** the log stamps the **basename** (`MainScr.c`); the catalog/resolver key on
the repo-relative path (`MoM/src/MainScr.c`). `rmr2hms` joins on basename — the same basename join the
static↔runtime cross-check already validated.

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
