Combat-Miscellaneous.md

SEEALSO:  Combat-Homeless.md
SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen.md
SEEALSO:  Combat-End_Of_Combat.md

NX_EmptyFxn_o153p22()         ==>  o153p22_empty_function()
NX_IDK_CombatInit_Tactical()  ==>  NIU_Allocate_And_Build_Battlefield()
AI_RestrictToCity__WIP()      ==>  Update_Move_Map_City_Perimeter_Restrictions()
UU_IDK_Main_Screen_Draw()     ==>  NIU_Combat_Redraw_Main_Screen()
UU_IDK_CMB_s7DE08()           ==>  NIU_Enemy_Stack_Combat()

OVL_Action_OriginX  ==>  _combat_attacker_wx
OVL_Action_OriginY  ==>  _combat_attacker_wy

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\UU_IDK_CMB_s7DE08.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\UU_IDK_Main_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr110\Get_Effective_Hits.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\AI_RestrictToCity__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\NX_IDK_CombatInit_Tactical.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\NX_EmptyFxn_o153p22.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Combat_Grid_Screen_Coordinates.asm

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-25).** No findings open. Seven functions across six overlays, 353 asm lines, every one walked against its listing and every one faithful. Two findings were raised, both declaration-order defects in the `ovr096` pair, and both are applied. Every one of these was on the homeless list in [Combat-Homeless.md](Combat-Homeless.md); this review is the work of clearing it. Every one of these is on the homeless list in [Combat-Homeless.md](Combat-Homeless.md); this review is the work of clearing it.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Update_Move_Map_City_Perimeter_Restrictions` | [Combat.c:15740](../../MoM/src/Combat.c#L15740) | `ovr124/AI_RestrictToCity__WIP.asm` | 77 | **full body walk — faithful** |
| `Get_Effective_Hits` | [Combat.c:8481](../../MoM/src/Combat.c#L8481) | `ovr110/Get_Effective_Hits.asm` | 74 | **full body walk — faithful** |
| `NIU_Enemy_Stack_Combat` | [Combat.c:3507](../../MoM/src/Combat.c#L3507) | `ovr096/NIU_Enemy_Stack_Combat.asm` | 72 | **full body walk — faithful** |
| `NIU_Combat_Redraw_Main_Screen` | [Combat.c:3565](../../MoM/src/Combat.c#L3565) | `ovr096/UU_IDK_Main_Screen_Draw.asm` | 60 | **full body walk — faithful** |
| `Combat_Grid_Screen_Coordinates` | [Combat.c:22138](../../MoM/src/Combat.c#L22138) | `ovr154/Combat_Grid_Screen_Coordinates.asm` | 44 | **full body walk — faithful** |
| `NIU_Allocate_And_Build_Battlefield` | [Combat.c:18445](../../MoM/src/Combat.c#L18445) | `ovr153/NX_IDK_CombatInit_Tactical.asm` | 19 | **full body walk — faithful** |
| `o153p22_empty_function` | [Combat.c:19882](../../MoM/src/Combat.c#L19882) | `ovr153/NX_EmptyFxn_o153p22.asm` | 7 | **full body walk — faithful** |

## What belongs here, and why

**This is not a cohort.** Every other review in this tree is organised around a call graph - a screen, a dispatcher and its leaves, an overlay that does one job. These seven share nothing except the property that made them homeless: no finished review owns their callers, so no existing session could claim them.

That is a real category, not a filing accident. [tools/review_coverage.py](../../tools/review_coverage.py) computes it - attribution runs first, and only what it cannot place lands on the homeless list. A function gets here two ways: its callers are themselves unreviewed, or its callers span three or more reviews (`HOMELESS_DOC_SPREAD`) so no single one can reasonably own it. `Combat_Grid_Screen_Coordinates` is the second kind - twelve call sites across four reviews.

The value of doing them together is that they are the tail. Clearing this list takes combat-overlay coverage to complete, and each one closes a slot that has been sitting unclaimed through every cohort review so far.

### What does *not* belong here

- **`Raze_City_Prompt_Draw`** (`ovr124`, 126 asm) - homeless only because its sole caller, `Raze_City_Prompt`, is itself unadjudicated: [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md) names that caller in its header block and call tree but never gave it a scope row or a verdict. Both go to that review together; walking the callee here would have left the caller unwalked and the callee still unattributable.
- **`Calc_Battlefield_Bonuses`** (`o122p10`, ~450 asm) - homeless by the same rule, but going to [Combat-Combat.md](Combat-Combat.md) instead. It is large enough to deserve placement rather than lumping.
- **[Combat-Homeless.md](Combat-Homeless.md)** is the generated inventory, not a review. It renders no verdicts and [tools/review_coverage.py](../../tools/review_coverage.py) skips it via `SKIP_DOCS` for exactly that reason. This document is where the verdicts go.

## Findings

None outstanding. Two were raised against the `ovr096` pair and both are fixed: `NIU_Enemy_Stack_Combat` declared three locals in reverse frame order and was missing a fourth (the `Plane` slot at `bp-6`, a copy of the plane parameter), and `NIU_Combat_Redraw_Main_Screen` declared all four in reverse frame order.

## Verified faithful

### `Get_Effective_Hits` ([Combat.c:8481](../../MoM/src/Combat.c#L8481), asm 74)

Faithful, all 74 lines. It converts a unit's per-figure hit points into an arbitrary "effective health" score scaled by defense, for the strategic-combat estimator - [Strategic_Combat](../../MoM/src/Combat.c#L7783) sums it over the attacker's units at [Combat.c:7941](../../MoM/src/Combat.c#L7941) and the defender's at [Combat.c:7964](../../MoM/src/Combat.c#L7964).

**Frame.** Far call, so the parameters sit at `bp+6` and `bp+8`: `hits` first, `defense` second, matching the production signature. No `sub sp` and no stack locals - `effective_hits` is SI and the `defense` parameter is cached into DI, both register locals. Production declares exactly one local, `int16_t effective_hits`, and leaves it uninitialised; the listing's first write to SI is the `hits * 3` at asm:15, so that is correct.

**The `defense > 3` arm.** asm:16 tests `cmp di, 3 / jle` - signed, and the surviving `jle` sends control to the switch, so the fall-through path is `defense > 3` un-inverted. asm:18-21 build `(defense - 2)` twice from `add ax, -2` / `add dx, -2`, which is where production's `(defense + -2)` spelling comes from. asm:22 `imul dx`; asm:23-25 is the signed-halve idiom `cwd / sub ax,dx / sar ax,1`, whose `cwd` also truncates the product to 16 bits - immaterial here, since a 16-bit C `int` already truncates it. asm:27-29 then multiplies SI by that result in that operand order. Production's single expression at [Combat.c:8487](../../MoM/src/Combat.c#L8487) reproduces it term for term.

**The switch is a bounded jump table**, `mov bx, di / cmp bx, 3 / ja loc_89F41 / shl bx,1 / jmp [cs:off_89F4A+bx]` (asm:33-37). The table is in `ovr110/_misc.asm:6-9` and reads `loc_89F13, loc_89F1E, loc_89F27, loc_89F33` - defense 0, 1, 2, 3 in order, exactly production's four cases:

| defense | listing | production | net |
| --- | --- | --- | --- |
| 0 | asm:39-42, signed-halve idiom | `effective_hits / 2` | 50% |
| 1 | asm:47-49 `imul 3`, then `bx = 5` | `(effective_hits * 3) / 5` | 60% |
| 2 | asm:52-55 `imul 7`, `bx = 10` | `(effective_hits * 7) / 10` | 70% |
| 3 | asm:58-60 `shl ax, 2`, then `bx = 5` | `(effective_hits * 4) / 5` | 80% |

Case 3's `shl ax, cl` with `cl = 2` is the compiler's strength reduction of `* 4`; writing the multiply in C is the faithful form, not a deviation.

**Cross-jump tails.** `loc_89F39` (`bx = 5`) is shared by cases 1 and 3, `loc_89F3C` (`cwd / idiv bx`) by cases 1, 2 and 3, and `loc_89F1A` (the store back to SI) by case 0 and the merged divide tail. This is Borland tail-merging identical code; production writing each case out in full is correct.

**Negative `defense` falls through to the return.** The outer `jle` is signed so a negative reaches the switch, but the table's bounds check is the *unsigned* `ja`, which sends it straight to `loc_89F41` with SI still holding `hits * 3`. Production's `switch` has no `default`, so it exits with `effective_hits` unchanged - identical behaviour.


### `Update_Move_Map_City_Perimeter_Restrictions` ([Combat.c:15740](../../MoM/src/Combat.c#L15740), asm 77)

Faithful, all 77 lines. It seals the one-square band immediately outside the city block by writing `INF` (`0xFF`, [MOX_TYPE.h:36](../../MoX/src/MOX_TYPE.h#L36)) into `_cmbt_movepath_cost_map`, so the combat path solver cannot route across it and a defender inside has no legal exit. Called twice, from [CMBTAI.c:1426](../../MoM/src/CMBTAI.c#L1426) and [CMBTAI.c:1632](../../MoM/src/CMBTAI.c#L1632), in both cases immediately after [Update_Move_Map_City_Area_Restrictions](../../MoM/src/Combat.c#L15563) and under a guard requiring the defender, `_ai_stay_in_city`, a Wall of Fire or Wall of Darkness, and the unit already being inside the city.

**No frame.** No `sub sp`; both loop counters are register locals - SI and CX - and only SI is preserved. Both loops are bounded at 4 (asm:64, asm:70).

**Four independent `if`s, not a chain.** Each block falls through into the next test - asm:32, 37, 56 and 61 are all reached by fall-through, with no jump past the remaining tests. Production writes four separate `if` statements at [Combat.c:15748](../../MoM/src/Combat.c#L15748), [15760](../../MoM/src/Combat.c#L15760), [15764](../../MoM/src/Combat.c#L15764) and [15776](../../MoM/src/Combat.c#L15776), in the listing's order. The `or reg, reg / jnz` at asm:14, 23, 33 and 47 is the `== 0` test.

**All six hard-coded offsets are exact.** The listing writes raw byte offsets where production writes row/column arithmetic. Every one reconciles at `COMBAT_GRID_WIDTH` = 21 ([Combat.h:143](../../MoM/src/Combat.h#L143)) with the city bounds from [CMBTDEF.h:83-86](../../MoM/src/CMBTDEF.h#L83-L86) - `MIN_CGX_CITY` 5, `MAX_CGX_CITY` 8, `MIN_CGY_CITY` 10, `MAX_CGY_CITY` 13:

| listing | production | value |
| --- | --- | --- |
| `[bx+0C1h]` (asm:26) | `(MIN_CGY_CITY - 1) * 21 + (MIN_CGX_CITY - 1)` | 193 |
| `[bx+12Ah]` (asm:31) | `(MIN_CGY_CITY + 4) * 21 + (MIN_CGX_CITY - 1)` | 298 |
| `[bx+si+0C2h]` (asm:36) | `(MIN_CGY_CITY - 1) * 21 + MIN_CGX_CITY + cgx` | 194 |
| `[bx+0C6h]` (asm:50) | `(MIN_CGY_CITY - 1) * 21 + (MIN_CGX_CITY + 4)` | 198 |
| `[bx+12Fh]` (asm:55) | `(MIN_CGY_CITY + 4) * 21 + (MIN_CGX_CITY + 4)` | 303 |
| `[bx+si+12Bh]` (asm:60) | `(MIN_CGY_CITY + 4) * 21 + MIN_CGX_CITY + cgx` | 299 |

The two `si`-indexed forms are the top and bottom edges walked by `cgx`; the two `(MIN_CGY_CITY + cgy) * 21` writes at asm:22 and asm:46 are the left and right edges at columns 4 and 9, walked by `cgy`. Together they trace the complete perimeter of rows 9-14 by columns 4-9 around the 4x4 city interior, with all four corners covered by the nested `cgy == 0` / `cgy == 3` tests.

**Two notes on the constant spelling.** `MIN_CGY_CITY + 4` is 14 and `MIN_CGX_CITY + 4` is 9, which are `MAX_CGY_CITY + 1` and `MAX_CGX_CITY + 1`; the `+ 4` form ties the expression to the loop's 0..3 span instead of to the far edge, which reads oddly next to the `- 1` on the near edge. Separately, `cgx` and `cgy` name the loop counters here, but everywhere else in this file those identifiers mean absolute combat-grid coordinates (`battle_units[].cgx`); here they are 0..3 offsets within the ring. Neither affects fidelity - the listing has no names to be faithful to, since both counters are registers.

### `NIU_Allocate_And_Build_Battlefield` ([Combat.c:18445](../../MoM/src/Combat.c#L18445), asm 19)

Faithful, all 19 lines. No locals and no `sub sp` - just a prologue, two calls, and an epilogue.

`j_CMB_BaseAllocs__WIP` at asm:9 is production's `Allocate_Combat_Base_Blocks()`, and `j_CMB_Terrain_Init__WIP` at asm:15 is `Build_Battlefield()`; both renames are recorded in [Combat-Combat_Screen.md](Combat-Combat_Screen.md)'s header block, where those two functions are adjudicated. Push order at asm:11-14 is `wp`, `wy`, `wx`, so the call is `Build_Battlefield(wx, wy, wp)` - [Combat.c:18445](../../MoM/src/Combat.c#L18445).

The commented block directly above the function at [Combat.c:18433-18443](../../MoM/src/Combat.c#L18433-L18443) is reference material, not this listing - it shows a fuller initialisation that sets `_combat_wp`, the two player indices and calls `Cache_Graphics_Combat()`. None of that is in `ovr153/NX_IDK_CombatInit_Tactical.asm`, which does exactly two things.

### `o153p22_empty_function` ([Combat.c:19882](../../MoM/src/Combat.c#L19882), asm 7)

Faithful. The listing is a bare frame - `push bp` / `mov bp, sp` / `pop bp` / `retf` - with no body at all, and [Combat.c:19882-19885](../../MoM/src/Combat.c#L19882-L19885) is an empty function with those four instructions quoted in comments so the reader can see there is nothing missing.

Nothing calls it. The only other reference in the tree is its prototype at [Combat.h:2243](../../MoM/src/Combat.h#L2243). Its sibling `o153p24_empty_function` at [Combat.c:19981](../../MoM/src/Combat.c#L19981) is identical in form and *is* called, from [Combat.c:3423](../../MoM/src/Combat.c#L3423) - so the empty-function shape is real in this overlay, not an artifact of a missing reconstruction.

### `NIU_Enemy_Stack_Combat` ([Combat.c:3507](../../MoM/src/Combat.c#L3507), asm 72)

Body faithful; see R1 for the frame. A thin entry point that opens combat against an enemy stack standing on the map.

**Which parameter feeds which read is easy to get backwards, and production has it right.** asm:19-26 reads `_UNITS[si].wx` where SI is `unit_idx` at `bp+8`, the **second** parameter; asm:37-44 reads `_UNITS[arg_0].owner_idx` from `bp+6`, the **first**. [Combat.c:3515-3517](../../MoM/src/Combat.c#L3515-L3517) matches - position from the attacker, owner from the defender.

**Both call argument orders confirmed from push order.** `Player_Army_At_Square(wx, wy, wp, _human_player_idx, &troop_count, troops)` at asm:49-58, and `Combat(_human_player_idx, combat_defender_player_idx, troop_count, troops)` at asm:59-66. `Combat__WIP` in the listing is production's `Combat` at [Combat.c:3103](../../MoM/src/Combat.c#L3103).

**`_combat_environ = 0` is `cnv_Enemy_Stack`** ([Combat.h:578](../../MoM/src/Combat.h#L578)) - the named form at [Combat.c:3521](../../MoM/src/Combat.c#L3521) is the same value.

### `NIU_Combat_Redraw_Main_Screen` ([Combat.c:3565](../../MoM/src/Combat.c#L3565), asm 60)

Body faithful; see R2 for the frame. It saves the map scroll position, brings the combat square into view, redraws the main screen, holds briefly, and restores the scroll position.

**The save/restore bracket is exact** - `_map_x`/`_map_y` into the two `tmp_curr_world_*` locals at asm:10-13 and back at asm:53-56, with everything else between them. [Combat.c:3571-3572](../../MoM/src/Combat.c#L3571-L3572) and [Combat.c:3588-3589](../../MoM/src/Combat.c#L3588-L3589).

**Two six-argument calls, both confirmed from push order.** `OVL_BringIntoView(&wx, &wy, _combat_wx, _combat_wy, _combat_wp)` at asm:14-22, and `Main_Screen_Draw_Do_Draw(&wx, &wy, _combat_wp, wx, wy, _human_player_idx)` at asm:35-44 - note that one passes `wx`/`wy` **both** by address and by value, which is faithful.

**`Release_Time(6)`** at asm:49-52 is the only literal in the function, and the `_combat_wx__som_started_anim_ctr` at asm:16 is IDA's merged alias for plain `_combat_wx`, which production uses.

### `Combat_Grid_Screen_Coordinates` ([Combat.c:22138](../../MoM/src/Combat.c#L22138), asm 44)

Faithful, all 44 lines. The combat grid's forward isometric transform: cell coordinates plus a sub-cell offset in, screen pixels out through two pointers.

**No stack frame.** `push bp` / `mov bp, sp` with no `sub sp` - production's `sx` and `sy` are register locals (DX and SI), and the six parameters occupy `bp+6` through `bp+10h`.

**The arithmetic is exact, term for term:**

| production | listing |
| --- | --- |
| `sx = ((cgx - cgy) * 16) + 158` | `sub ax, [bp+cgc1]` / `mov cl, 4` / `shl ax, cl` / `add ax, 158` (asm:18-21) |
| `sy = ((cgx + cgy) * 8) - 80` | `add ax, [bp+cgc1]` / `mov cl, 3` / `shl ax, cl` / `add ax, -80` (asm:24-27) |
| `sx += ((cgx_subcell_offset - cgy_subcell_offset) * 2)` | `sub` / `shl ax, 1` / `add dx, ax` (asm:29-32) |
| `sy += (cgx_subcell_offset + cgy_subcell_offset)` | `add` / `add si, ax` (asm:33-35) |

Both shifts are the multiplications written as shifts, and `add ax, -80` is the subtraction. [Combat.c:22142-22145](../../MoM/src/Combat.c#L22142-L22145).

**The IDA parameter names are inverted and should not be copied.** `cgc2` sits at `bp+6` and is therefore the **first** parameter - the **x** cell - while `cgc1` at `bp+8` is the second, the **y** cell. Production's `(cgx, cgy, ...)` order is correct; the numbering in the listing suggests the opposite and is simply wrong.

**Both results are written through out-pointers** at asm:36-39, matching [Combat.c:22146-22147](../../MoM/src/Combat.c#L22146-L22147). Nothing is returned.

## Deviations - structurally different, behaviour unchanged

These do not change what the code does. They are listed because the target is 1:1 with the disassembly, not functional equivalence.

### D1 - zero-initialised locals

`Get_Effective_Hits` declares `int16_t effective_hits = 0` ([Combat.c:8483](../../MoM/src/Combat.c#L8483)) and `Update_Move_Map_City_Perimeter_Restrictions` declares `int16_t cgx = 0` / `int16_t cgy = 0` ([Combat.c:15742-15743](../../MoM/src/Combat.c#L15742-L15743)). Neither listing emits those stores - in both functions the counters and the accumulator live in registers, and the first write in each is the real one (`mov si, ax` after the `imul` at asm:15 in `Get_Effective_Hits`; the loop initialisers in the other). The initialisers are dead in every path and cost nothing observable. This is the same house-style deviation recorded as D1 in [Combat-Spell_Cast_AI_Select.md](Combat-Spell_Cast_AI_Select.md).

## A note on this function's prior coverage

`Combat_Grid_Screen_Coordinates` was named in [Combat-Combat_Screen.md](Combat-Combat_Screen.md)'s header block but never given a scope-table row, which is why it has shown as `CLAIMED - never adjudicated` in every reconciliation run. That doc carries **two** prose sections on it that disagree: one calls it faithful, the other says "**One finding — R22**" - and **R22 is defined nowhere in that document or any other**. It is a dangling reference with no content behind it.

This walk was done from the listing rather than from either section. The result is faithful, so the "R22" mention describes nothing that exists. Both sections should come out of that doc now that the verdict lives here.
