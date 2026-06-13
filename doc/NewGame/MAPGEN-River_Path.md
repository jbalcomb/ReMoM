MAPGEN-River_Path.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\River_Path.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\River_Path.c

Init_New_Game()
    |-> for(tries<2000 && River_Path(ARCANUS) != 0) { }   [MAPGEN.c:382]
    |-> for(tries<2000 && River_Path(MYRROR)  != 0) { }   [MAPGEN.c:383]

---

# `River_Path` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `River_Path` | [MAPGEN.c:4154-4283](../../MoM/src/MAPGEN.c#L4154-L4283) | Tries to grow one river: pick a valid inland start, random-walk "downstream" toward an outflow (ocean or an existing river), and on success stamp the tiles as river. Returns `ST_TRUE` if it placed one (caller retries), `ST_FALSE` if this attempt failed. |
| `River_Path__GEMINI` | [MAPGEN.c:4286-4430](../../MoM/src/MAPGEN.c#L4286-L4430) (inside `#if 0`) | Reference IDA→C translation (= Piethawn `*.c`). Matches the asm; kept for cross-reference. |

Verified faithful to the disassembly `River_Path.asm` (structure and `Random()` sequence 1:1), carrying preserved OG dead code (the `niu_*` arrays/vars) — see below.

## Purpose

Called in a retry loop by `Init_New_Game` — `for(tries=0; tries < 2000 && River_Path(plane) != 0; tries++)` per plane. Each call is one placement attempt:

1. Roll an inland start point; reject if it has a terrain special, touches Ocean (3×3), or is Mountain/Hills/Node/River.
2. Random-walk: each step usually continues `downstream` (50%) or, after the first step, picks a non-reverse direction; reject-and-retry steps onto special/Mountain/Hills/Node/Desert tiles; stop (`Have_Outflow`) on reaching an existing river or Ocean.
3. Bail if the walk takes >30 attempts or exceeds 28 tiles, or the finished path is <4 tiles, or the outflow tile is invalid; otherwise stamp every path tile as river (value `1000`).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:382-383](../../MoM/src/MAPGEN.c#L382-L383) | Up to 2000 attempts per plane; loop stops when `River_Path` returns `ST_FALSE`. |

## Code walk

Line refs are production [MAPGEN.c](../../MoM/src/MAPGEN.c); cross-checked against `River_Path.asm` (the authority). `Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

### Start selection ([4176-4201](../../MoM/src/MAPGEN.c#L4176-L4201))

`base_wx = 4 + Random(WORLD_WIDTH-8)` (= `4 + Random(52)`), `base_wy = 4 + Random(WORLD_HEIGHT-8)` (= `4 + Random(32)`) — matches the asm's `Random(52)/Random(32)`. Reject if the start has a terrain special, if any of the 3×3 neighbors is Ocean, or if it's Mountain/Hills/Node/River. Seed `wx_array[0]/wy_array[0]`, `length=1`, `Have_Outflow=FALSE`, `downstream = Random(4)-1`, `same_dir = ST_UNDEFINED`. **Faithful.**

### Walk loop ([4203-4261](../../MoM/src/MAPGEN.c#L4203-L4261))

`while(Have_Outflow == ST_FALSE)`:
- **Direction** ([4206-4220](../../MoM/src/MAPGEN.c#L4206-L4220)): if `Random(2) > 1` (50%) **or** `same_dir == ST_UNDEFINED` (first step) → `direction = downstream`; else `do { direction = Random(4)-1; } while(upstream[downstream] == direction)` (any direction except the reverse of downstream). Matches the asm gate exactly (`cmp ax,1; jg`, then the `same_dir==UNDEFINED` test, then the `upstream[]` do-while).
- Step to `next = curr + dir_chg_tbl_wx/wy[direction]` ([4226-4227](../../MoM/src/MAPGEN.c#L4226-L4227)); `attemps++`, bail if `> 30` ([4231-4236](../../MoM/src/MAPGEN.c#L4231-L4236)).
- Reject-and-`continue` (re-roll, no append) on special/Mountain/Hills/Node/Desert ([4238-4242](../../MoM/src/MAPGEN.c#L4238-L4242)); set `Have_Outflow` on hitting a river ([4244](../../MoM/src/MAPGEN.c#L4244)); append the tile; set `Have_Outflow` if any of the 5 N/W/C/E/S neighbors is Ocean ([4251-4255](../../MoM/src/MAPGEN.c#L4251-L4255)); bail if `length > 28` ([4257](../../MoM/src/MAPGEN.c#L4257)). **Faithful.**

### Finish ([4263-4282](../../MoM/src/MAPGEN.c#L4263-L4282))

`if(length < 4) return ST_FALSE`; reject if any of the 5 outflow-tile neighbors is an invalid outflow (`Square_Is_Bad_River_Mouth`; drake178's "some shore types are wrongly excluded" query is OG behavior, preserved); else stamp `p_world_map[wp][wy][wx] = 1000` for every path tile and `return ST_TRUE`. **Faithful.**

## OG quirks preserved

- **`niu_directions_array[30]`** ([4156](../../MoM/src/MAPGEN.c#L4156)) — written at `[0]` and each `[length]` but **never read**. Dead, but the asm writes it too (`loc_48E66`), so the writes are faithful. (`BUGBUG  as coded, completely useless`.)
- **`niu_prev_dir`** ([4161](../../MoM/src/MAPGEN.c#L4161)) — assigned `direction` each step, never read. Faithful dead write.
- **`same_dir`** ([4168](../../MoM/src/MAPGEN.c#L4168)) — only its `== ST_UNDEFINED` state matters (it's effectively a "first step" flag); it's assigned `direction` **twice** per iteration ([4224](../../MoM/src/MAPGEN.c#L4224) and [4259](../../MoM/src/MAPGEN.c#L4259)), a copy-paste artifact from `Generate_Landmasses`'s `new_direction`. The asm has both assignments (`loc_48E66`, `loc_49007`) — faithful.

These are RNG-neutral (plain memory writes); preserve them.

## Notes vs `__GEMINI`

GEMINI matches the asm and production behaviorally; the only difference is cosmetic — GEMINI names the step table `dir_chg_tbl_wx__1` where production uses the merged `dir_chg_tbl_wx` (identical values). GEMINI's header comment `WORLD_WIDTH 80 / WORLD_SIZE 4800` is wrong (it's 60/2400); production uses the correct constants, and the `Random(52)/Random(32)` args match the asm regardless.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — returns `1..n`.
- **`Square_Is_Ocean_NewGame` / `_Mountain_` / `_Hills_` / `_Node_` / `_River_` / `_Desert_NewGame`** — start/step tile-type tests.
- **`Square_Is_Bad_River_Mouth(wx, wy, wp)`** — rejects unsuitable outflow shore tiles.
- **`dir_chg_tbl_wx/wy`** (`{0,-1,0,1,0}`/`{1,0,-1,0,0}`), **`upstream[4] = {2,3,0,1}`** ([MAPGEN.c:109](../../MoM/src/MAPGEN.c#L109)) — direction step + reverse-direction tables.
- **`_map_square_terrain_specials`**, **`p_world_map`** — globals read/written.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\River_Path.asm` — IDA Pro 5.5 disassembly (the authority; `loc_4900D` = loop cond, `loc_49016` = the `length<4` check).
- [MAPGEN.c:4286-4430](../../MoM/src/MAPGEN.c#L4286-L4430) — `__GEMINI` reference translation (`#if 0`).
- [MAPGEN.c:382-383](../../MoM/src/MAPGEN.c#L382-L383) — the retry-loop call sites.
- `MOM_DEF.h` — `WORLD_WIDTH`/`WORLD_HEIGHT` (60/40), `WORLD_SIZE` (2400).
