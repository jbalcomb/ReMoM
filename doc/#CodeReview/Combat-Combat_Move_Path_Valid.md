Combat-Combat_Move_Path_Valid.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr155\Combat_Move_Path_Valid.asm (ground truth - disassembly of original Borland C 3.0 code)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr155\Combat_Move_Path_Valid.c   (second opinion - Gemini Pro AI translation)

OON XREF:  Assign_Combat_Grids() |-> Combat_Move_Path_Valid()

Assign_Combat_Grids()
    |-> j_Assign_Combat_Grids()
        |-> Combat_Screen()
        |-> Turn_Off_Auto_Combat()
        |-> Auto_Do_Combat_Turn()
        |-> AI_EvaluateCmbtSpell()

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-01).** `Combat_Move_Path_Valid` is 1:1 with the disassembly — control flow, loop bounds, statement order, local widths, and frame layout all match. `CMBTMVPT.c` compiles clean, no warnings.

Scope = `Combat_Move_Path_Valid` ([CMBTMVPT.c:246-345](../../MoM/src/CMBTMVPT.c#L246-L345)), plus the two shared macros it expands: `PREP` ([70-78](../../MoM/src/CMBTMVPT.c#L70-L78)) and `RELAX_ADJACENT_CELLS` ([86-102](../../MoM/src/CMBTMVPT.c#L86-L102)). `Combat_Move_Path_Find` is cited only where the comparison is load-bearing; it needs its own review and still carries defects of its own (see Related references).

Ground truth = `ovr155/Combat_Move_Path_Valid.asm`. `asm:N` refers to line N of that listing. Where a comparison against `Combat_Move_Path_Find` matters, the bytes came from `ovr155/Combat_Move_Path_Find.asm`.

## Review status

| Concern | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| Signature / frame | [CMBTMVPT.c:246](../../MoM/src/CMBTMVPT.c#L246) | asm:2, 16-18 | faithful — 3 word params, no return |
| Local set | [248-263](../../MoM/src/CMBTMVPT.c#L248-L263) | asm:3-15, 19-21 | faithful — 16 locals, one per frame slot, no extras |
| Local declaration order | [248-263](../../MoM/src/CMBTMVPT.c#L248-L263) | asm:3-15 | faithful — matches frame layout `-18h` -> `-2` exactly |
| Local widths | [248-252](../../MoM/src/CMBTMVPT.c#L248-L252) | asm:3-7 | faithful — byte, byte, word, word, word |
| `PREP` init | [265-268](../../MoM/src/CMBTMVPT.c#L265-L268) | asm:27-57 | faithful |
| Sweep loop shape | [271-272](../../MoM/src/CMBTMVPT.c#L271-L272) | asm:58-59, 281-284 | faithful — test-at-bottom `while` |
| Sweep bounds | [276-277](../../MoM/src/CMBTMVPT.c#L276-L277) | asm:62-63 | faithful — 19 then 20, in that order |
| Three-phase raster | [282-310](../../MoM/src/CMBTMVPT.c#L282-L310) | asm:67-280 | faithful |
| Adjacency loop / cost model | [288](../../MoM/src/CMBTMVPT.c#L288), [297](../../MoM/src/CMBTMVPT.c#L297), [306](../../MoM/src/CMBTMVPT.c#L306) | asm:82-130 | faithful — single `0..8`, no diagonal surcharge |
| Relaxation body | [86-102](../../MoM/src/CMBTMVPT.c#L86-L102) | asm:84-130 | faithful |
| Overlay loop | [314-318](../../MoM/src/CMBTMVPT.c#L314-L318) | asm:286-334 | faithful — bounds and stride |
| Overlay body | [321-341](../../MoM/src/CMBTMVPT.c#L321-L341) | asm:291-325 | faithful |

## Frame map

The asm frame carries 13 stack locals plus 3 register locals. Production declares all 16 in the order the frame lays them out, one for one with nothing left over:

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-18h` (byte) | `move_cost` | `move_cost` (`uint8_t`) |
| `bp-17h` (byte) | `potential_path_cost` | `new_cost_to_reach` (`uint8_t`) |
| `bp-16h` | `Tile_GetTo_Cost` | `get_to_cost` |
| `bp-14h` | `max_i` | `max_y` |
| `bp-12h` | `adjacent_path_cost` | `adjacent_path_cost` |
| `bp-10h` | `max_j` | `max_x` |
| `bp-0Eh` | `itr_j` | `itr_x` |
| `bp-0Ch` | `itr_i` | `itr_y` |
| `bp-0Ah` | `current_origin` | `current_origin` |
| `bp-8` | `tense` | `a_cost_was_updated` |
| `bp-6` | `itr` | `itr` |
| `bp-4` | `itr_y` | `itr2_y` |
| `bp-2` | `itr_x` | `itr2_x` |
| `cx` | — | `ctr` |
| `di` | — | `itr_adjacent` |
| `si` | — | `adjacent_idx` |

Note the shear: the asm's `itr_i`/`itr_j` are the *sweep* row/column and its `itr_y`/`itr_x` are the *overlay* row/column. Production reverses which pair carries the `2` suffix. That is a naming choice, not a structural divergence — the slots line up one-for-one.

Because the mapping is exhaustive, any local added here without a frame slot is phantom by construction. The frame map is the check for that.

## Faithful — verified against the asm, leave alone

- **`movement_path_grid_cell_count = 0` precedes `PREP`** — asm:27, before the two init loops. Production [265](../../MoM/src/CMBTMVPT.c#L265) / [268](../../MoM/src/CMBTMVPT.c#L268).
- **`PREP` init**, both loops and the source seed — asm:28-40 (`_cmbt_path_data[itr] = itr`, word store), asm:41-50 (`_cmbt_mvpth_c[itr] = INF`, byte store), asm:51-57 (`_cmbt_mvpth_c[source_cgy * COMBAT_GRID_WIDTH + source_cgx] = 0`). The bound is `e_COMBAT_GRID_CELL_COUNT` on the first loop (asm:39) and the literal `462` on the second (asm:49) — same value, the listing just symbolized one of the two.
- **Test-at-bottom `while`** — asm:58-59 seeds `tense = ST_TRUE` and jumps to the test at asm:281-284. Production's `a_cost_was_updated = ST_TRUE; while(a_cost_was_updated == ST_TRUE)` is the same shape.
- **`max_*` and `ctr` reset inside the loop body**, not hoisted — asm:61-64 sit at `loc_E2EE5`, the top of the repeat body.
- **Sweep bounds and their order** — `asm:62` stores `max_j` = 19, then `asm:63` stores `max_i` = 20. Both are bare literals in the listing, so the values are not in doubt. `max_j` is the column bound (production `max_x` = `COMBAT_GRID_WIDTH - 2`) and `max_i` the row bound (production `max_y` = `COMBAT_GRID_HEIGHT - 2`); the named forms evaluate to the asm literals exactly. Each raster row therefore advances `ctr` by `1 + max_x + 1` = 21, which is what keeps the three-phase edge substitution aligned to real columns 0 and 20.
- **Three-phase raster with three offset tables** — `CMB_AdjctOfs_NoWest` (asm:88), `CMB_AdjacentOffsets` (asm:156), `CMB_AdjctOfs_NoEast` (asm:230). The point of the split is that a diagonal cannot wrap across a row boundary at the grid edges.
- **One `0..8` adjacency loop per phase, no diagonal/orthogonal split** — asm:82 (`xor di,di`) / asm:129 (`cmp di, 8`), and likewise asm:150/198 and asm:224/271. Contrast `Combat_Move_Path_Find`, which splits `0..4` then `4..8` (`Combat_Move_Path_Find.asm:142-144`).
- **No diagonal surcharge** — the accumulate is `mov al` / `add al` / `mov [potential_path_cost], al` at asm:101-103, asm:169-171 and asm:243-245, with no `inc al` in any of the three. `Combat_Move_Path_Find.asm:113-116` *does* carry `inc al` in its `0..4` diagonal block and omits it in the `4..8` block, which is what that function's `EXTRA_COST` 1/0 split reproduces. `RELAX_ADJACENT_CELLS(..., 0, 8, 0)` here is correct.
- **Byte-width accumulation** — `potential_path_cost` is a byte slot (asm:4) fed by `add al` (asm:102), so the sum wraps modulo 256 before the unsigned byte `cmp` / `jbe` at asm:106-108. Production's `uint8_t new_cost_to_reach` ([249](../../MoM/src/CMBTMVPT.c#L249)) truncates on assignment at [92](../../MoM/src/CMBTMVPT.c#L92) before the compare at [93](../../MoM/src/CMBTMVPT.c#L93), reproducing that. `adjacent_path_cost` is correctly a word on both sides (asm:7, loaded `mov al,[bx]` / `mov ah,0`).
- **`ctr++` still runs on the `move_cost == INF` skip path** in all three phases — asm:74 -> asm:132, asm:142 -> asm:201, asm:216 -> asm:274. Production [290](../../MoM/src/CMBTMVPT.c#L290), [299](../../MoM/src/CMBTMVPT.c#L299), [308](../../MoM/src/CMBTMVPT.c#L308).
- **Bounds check** `adjacent_idx >= 0 && adjacent_idx < COMBAT_GRID_CELL_COUNT` — asm:90-93 (`or si,si` / `jl`, then `cmp si, 462` / `jge`); the third phase spells it `e_COMBAT_GRID_CELL_COUNT` at asm:234.
- **Update-block write order** — `_cmbt_path_data[ctr]` first (asm:109-113), then `_cmbt_mvpth_c[ctr]` (asm:114-117).
- **The redundant re-read** of `_cmbt_path_data[ctr]` before comparing against `current_origin` (asm:118-123) is preserved in the macro rather than reusing `adjacent_idx`. Correct — the OG re-loads.
- **Overlay loop** — `itr_x < COMBAT_GRID_WIDTH` (asm:329), `itr_y < COMBAT_GRID_HEIGHT` (asm:333), and `ctr = itr_y * COMBAT_GRID_WIDTH + itr_x` (asm:292-296). The stride has to match what `Assign_Combat_Grids` reads back at [Combat.c:3676](../../MoM/src/Combat.c#L3676), and it does.
- **Overlay body** — unconditional `ST_FALSE` default (asm:301), `INF` skip (asm:304), `get_to_cost = _cmbt_mvpth_c[ctr] - move_cost` with both operands zero-extended before a signed subtract (asm:302-317), and a **signed** `moves2 > get_to_cost` (`jle` at asm:320). Production's `int16_t get_to_cost` and the integer promotion of the two `uint8_t` operands match.

## OG-faithful behaviors (not bugs — do not "fix")

- **`Combat_Move_Path_Valid` and `Combat_Move_Path_Find` run different cost models.** Valid charges nothing extra for diagonals; Find charges `+1`. Verified in bytes on both sides (asm:101-103 vs `Combat_Move_Path_Find.asm:113-116`). The reachability overlay is therefore more permissive than the path solver can pay for: a cell can be highlighted, accepted by `Battle_Unit_Action`, and then cost more to actually reach. `Move_Battle_Unit__WIP` absorbs the difference by clamping at [Combat.c:3323](../../MoM/src/Combat.c#L3323) rather than re-checking. Original behavior; stays.
- **`_cmbt_path_data` is repurposed mid-function** from the predecessor array `PREP` built into a `{ST_FALSE, ST_TRUE}` boolean map, destroying the predecessor data (asm:301, asm:325 write words). Callers depend on this: `Assign_Combat_Grids` reads it as booleans immediately after ([Combat.c:3676](../../MoM/src/Combat.c#L3676)).
- **The unit's own cell always comes back reachable.** `Assign_Combat_Grids` marks every active unit's square `INF` in the cost map before calling, including the active unit's own. `PREP` then seeds `_cmbt_mvpth_c[source] = 0`, so the overlay computes `get_to_cost = 0 - 255 = -255` and `moves2 > -255` is true. `Assign_Combat_Grids` overwrites that cell with the unit index one line later ([Combat.c:3690](../../MoM/src/Combat.c#L3690)), which is why it never surfaces.

## Build state

`cmake --build --preset MSVC-debug` (2026-08-01): [CMBTMVPT.c](../../MoM/src/CMBTMVPT.c) compiles clean, no warnings, and `momlib.lib` builds.

The tree does not link, for a reason unrelated to this review. Every executable target fails on the same 8 unresolved externals — `Combat_Cast_Spell`, `Spell_Resistance_Modifier`, `Combat_Casting_Cost_Multiplier`, `Combat_Spellbook_Screen_Draw`, `AITP_EarthToMud`, `AITP_Disrupt`, `AITP_CracksCall`, `AITP_RecallHero`. Those symbols moved into `MoM/src/CMBMAGIC.c`, which is registered in [Makefile.am:204](../../Makefile.am#L204) but not in [MoM/src/CMakeLists.txt](../../MoM/src/CMakeLists.txt#L16-L17). Adding it there is what turns the tree green; nothing in `CMBTMVPT.c` is implicated.

No `stub_wip_todo.md` box to check — `Combat_Move_Path_Valid` is not a `__WIP`/`__STUB` function and has no tracker entry.

## Related references

- [CMBTMVPT.c](../../MoM/src/CMBTMVPT.c) — `Combat_Move_Path_Find` shares `PREP` and `RELAX_ADJACENT_CELLS` and needs its own review. Known open against `Combat_Move_Path_Find.asm`: pixel constants at [166-167](../../MoM/src/CMBTMVPT.c#L166-L167) where the asm holds literals 19/20 at `Combat_Move_Path_Find.asm:74-75`; `int16_t new_cost_to_reach` at [123](../../MoM/src/CMBTMVPT.c#L123) against the byte slot at `Combat_Move_Path_Find.asm:4`; phantom locals `existing_path_cost` / `new_next_cell_index` at [135-136](../../MoM/src/CMBTMVPT.c#L135-L136).
- [Combat.c](../../MoM/src/Combat.c) — `Assign_Combat_Grids` (sole caller), `Move_Battle_Unit__WIP`, `Battle_Unit_Action`.
- [Combat.h:186-206](../../MoM/src/Combat.h#L186-L206) — `COMBAT_GRID_CELL_WIDTH`/`_HEIGHT` are **pixels** (32x16); `COMBAT_GRID_WIDTH`/`_HEIGHT` are **cells** (21x22). Separately, `COMBAT_STRUCTURE_CGX`/`CGY` are 11/6, giving `COMBAT_STRUCTURE_IDX` = 137, while the arithmetic comment at [Combat.h:203](../../MoM/src/Combat.h#L203) and the city-centre test at [Combat.c:19234](../../MoM/src/Combat.c#L19234) both put the central structure at cgx 6 / cgy 11 = index 237.
- [doc/PathFinding/MoM-MovePath-Compare.md](../PathFinding/MoM-MovePath-Compare.md) — shared five-step skeleton across the three solvers.
- C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr155\Combat_Move_Path_Find.asm
