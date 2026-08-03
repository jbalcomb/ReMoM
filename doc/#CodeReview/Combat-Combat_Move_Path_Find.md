Combat-Combat_Move_Path_Find.md

SEEALSO:  C:\STU\devel\ReMoM\doc\PathFinding\MoM-MovePath-Compare.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr155\Combat_Move_Path_Find.asm (ground truth - disassembly of original Borland C 3.0 code)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr155\Combat_Move_Path_Find.c   (second opinion - Gemini Pro AI translation)

Move_Battle_Unit()
AI_GetCombatRallyPt()
Do_Auto_Unit_Turn()
Do_Auto_Unit_Turn()
Auto_Move_Unit()
    |-> j_Combat_Move_Path_Find()
        |-> Combat_Move_Path_Find()

/*
    Structure map — see doc/PathFinding/MoM-MovePath-Compare.md ("Combat_Move_Path_Find — combat grid").

    Combat solver (live). Like Find_Shortest_Path (MAPGEN.c) — and unlike the overland Move_Path_Find, which
    splits the work with its caller Make_Move_Path — this runs all five shared-skeleton steps in one body.
    NOTE: the local "1..5" section numbers below are NOT the skeleton step numbers; the mapping is:
        local 1 (bail)              = [Skeleton step 2]
        local 2 (PREP init)         = [Skeleton step 1]
        local 3 (relaxation sweep)  = [Skeleton step 3]
        local 4 (trace)             = [Skeleton step 4]
        local 5 (reverse + convert) = [Skeleton step 5]
    Combat-specific: single-direction raster (left-edge / middle / right-edge phases per row), and diagonal
    moves cost +1 vs orthogonal +0 (the RELAX_ADJACENT_CELLS EXTRA_COST argument).
*/

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-03).** `Combat_Move_Path_Find` is 1:1 with the disassembly — control flow, loop bounds, statement order, local widths and frame layout all match.

`CMBTMVPT.c`, `CMBTAI.c` and `Combat.c` all compile clean, no warnings.

Line numbers are a snapshot taken while `CMBTMVPT.c` was under active restructuring — re-verify before relying on any specific citation.

Scope = `Combat_Move_Path_Find` ([CMBTMVPT.c:101-206](../../MoM/src/CMBTMVPT.c#L101-L206)), plus the two shared macros it expands: `PREP` ([67-75](../../MoM/src/CMBTMVPT.c#L67-L75)) and `RELAX_ADJACENT_CELLS` ([82-98](../../MoM/src/CMBTMVPT.c#L82-L98)). Its sibling `Combat_Move_Path_Valid` is reviewed in [Combat-Combat_Move_Path_Valid.md](Combat-Combat_Move_Path_Valid.md).

Ground truth = `ovr155/Combat_Move_Path_Find.asm`. `asm:N` refers to line N of that listing. Data-segment layout comes from `dseg/_misc.asm`.

## Review status

| Concern | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| Signature | [CMBTMVPT.c:101](../../MoM/src/CMBTMVPT.c#L101) | asm:2, 13-16 | 4 word params faithful; return type — see Observation |
| Local set | [103-117](../../MoM/src/CMBTMVPT.c#L103-L117) | asm:3-12, 17-19 | 13 mapped slots + `path_cgx`/`path_cgy` |
| Local declaration order | [103-115](../../MoM/src/CMBTMVPT.c#L103-L115) | asm:3-12 | faithful — matches frame layout `-12h` -> `-2` exactly |
| Local widths | [103-106](../../MoM/src/CMBTMVPT.c#L103-L106) | asm:3-6 | faithful — byte, byte, word, word |
| Early bail | [119-126](../../MoM/src/CMBTMVPT.c#L119-L126) | asm:27-37 | faithful — count reset first, index inline in the condition |
| `PREP` init | [129](../../MoM/src/CMBTMVPT.c#L129) | asm:39-69 | faithful |
| Sweep loop shape | [132-133](../../MoM/src/CMBTMVPT.c#L132-L133) | asm:70-71, 442-445 | faithful — test-at-bottom `while` |
| Sweep bounds | [135-136](../../MoM/src/CMBTMVPT.c#L135-L136) | asm:74-75 | faithful — 19 then 20, in that order |
| Three-phase raster | [139-175](../../MoM/src/CMBTMVPT.c#L139-L175) | asm:79-441 | faithful |
| Offset-table selection | [147-148](../../MoM/src/CMBTMVPT.c#L147-L148), [159-160](../../MoM/src/CMBTMVPT.c#L159-L160), [170-171](../../MoM/src/CMBTMVPT.c#L170-L171) | asm:100/150, 219/269, 341/391 | faithful — see Offset tables |
| Diagonal / orthogonal split | same lines | asm:142-144, 261-263, 383-385 | faithful — `0..4` +1, `4..8` +0 |
| Buffer hijack | [181-183](../../MoM/src/CMBTMVPT.c#L181-L183) | asm:447-449 | faithful |
| Trace loop | [187-194](../../MoM/src/CMBTMVPT.c#L187-L194) | asm:450-475 | faithful |
| Convert loop | [198-204](../../MoM/src/CMBTMVPT.c#L198-L204) | asm:478-513 | faithful — single loop, double lookup, double `idiv` |
| Path array element type | [201](../../MoM/src/CMBTMVPT.c#L201), [203](../../MoM/src/CMBTMVPT.c#L203) | asm:489-493, 504-508 | faithful — `int16_t`, word stores |

## Frame map

The asm frame carries 10 stack locals plus 3 register locals, and `sub sp, 12h` (asm:22) = 18 bytes accounts for exactly those: two byte slots plus eight words. There is no slack in the frame, which is what makes a slot-less production local phantom by construction.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-12h` (byte) | `move_cost` | `move_cost` (`uint8_t`) |
| `bp-11h` (byte) | `potential_path_cost` | `new_cost_to_reach` (`uint8_t`) |
| `bp-10h` | `max_i` | `max_y` |
| `bp-0Eh` (word) | `adjacent_path_cost` | `adjacent_path_cost` (`int16_t`) |
| `bp-0Ch` | `max_j` | `max_x` |
| `bp-0Ah` | `itr_j` | `itr_x` |
| `bp-8` | `itr_i` | `itr_y` |
| `bp-6` | `current_origin` | `current_origin` |
| `bp-4` | `tense` | `a_cost_was_updated` |
| `bp-2` | `itr` | `itr` |
| `cx` | — | `ctr` |
| `si` | — | `adjacent_idx` |
| `di` | — | `itr_adjacent` |

`Combat_Move_Path_Valid` has three extra slots (`Tile_GetTo_Cost`, plus its own `itr_y`/`itr_x` for the overlay pass) that this function does not; otherwise the two frames are the same shape.

## Offset tables

`dseg/_misc.asm:2949-2951` holds three contiguous 8-word tables, in this order:

```
CMB_AdjacentOffsets dw    -20,    20,    22,   -22,   -21,    21,    -1,     1
CMB_AdjctOfs_NoWest dw    -20, -1000,    22, -1000,   -21,    21, -1000,     1
CMB_AdjctOfs_NoEast dw  -1000,    20, -1000,   -22,   -21,    21,    -1, -1000
```

Production folds them into `adjacent_offsets[3][8]` ([CMBTMVPT.c:38-44](../../MoM/src/CMBTMVPT.c#L38-L44)), and with `COMBAT_GRID_WIDTH` = 21 the three rows evaluate to those exact values in that exact order — row 0 unrestricted, row 1 no-west, row 2 no-east. Same 48 bytes, same layout.

Row selection matches the asm at all three phases: left edge takes row 1 (asm:100, asm:150 reference `CMB_AdjctOfs_NoWest`), middle takes row 0 (asm:219, asm:269 -> `CMB_AdjacentOffsets`), right edge takes row 2 (asm:341, asm:391 -> `CMB_AdjctOfs_NoEast`). The addressing is `add ax, [<table>+bx]` with `bx` = `itr_adjacent * 2`; a 2-D access `adjacent_offsets[n][itr_adjacent]` constant-folds its base to the same effective address, so the emitted form is indistinguishable.

**Caveat:** contiguity and identical values prove the *layout* is the same, not that the OG C declared a 2-D array. Three separately-declared `int16_t X[8]` globals in one translation unit also land contiguously in declaration order, and IDA labels each referenced address either way. The 2-D reading is a defensible deduction, not a provable one.

## Observation — the asm sets a return value on the bail path

```
asm:34   cmp     [byte ptr bx], e_INF
asm:35   jnz     short loc_E29F9
asm:36   xor     ax, ax
asm:37   jmp     @@Done
```

`ax` is zeroed only on the early-bail path; the normal exit at asm:434-441 falls into the shared epilogue without setting it. That is the signature of an `int`-returning function with `return 0;` on one branch and no return statement at the end. Production declares `void` ([101](../../MoM/src/CMBTMVPT.c#L101)).

No caller reads a result — all five call sites use it as a statement and read `movement_path_grid_cell_count` afterwards instead ([Combat.c:3164](../../MoM/src/Combat.c#L3164), [CMBTAI.c:715](../../MoM/src/CMBTAI.c#L715), [1533](../../MoM/src/CMBTAI.c#L1533), [1567](../../MoM/src/CMBTAI.c#L1567), [1744](../../MoM/src/CMBTAI.c#L1744)). Recorded rather than raised as a finding: the return type is a judgement about what the OG C declared, and the bytes are consistent with either an `int` function nobody uses or a `void` function where `ax` happens to be cleared.

## Faithful — verified against the asm, leave alone

- **Bail test** — `movement_path_grid_cell_count = 0` first (asm:27), then the destination index computed inline as part of evaluating the condition (asm:28-34), with nothing preserved afterwards. Production [119-126](../../MoM/src/CMBTMVPT.c#L119-L126) matches, including having no temp for the index.
- **`PREP` init**, both loops and the source seed — asm:39-52 (`_cmbt_path_data[itr] = itr`, word store with `shl ax, 1`), asm:53-62 (`_cmbt_mvpth_c[itr] = INF`, byte store), asm:63-69 (`_cmbt_mvpth_c[source_cgy * COMBAT_GRID_WIDTH + source_cgx] = 0`, the full expression, no temp). Both loop bounds are `e_COMBAT_GRID_CELL_COUNT` here (asm:51, asm:61).
- **Test-at-bottom `while`** — asm:70-71 seeds `tense = ST_TRUE` and jumps to the test at asm:442-445.
- **`max_*` and `ctr` reset inside the loop body**, not hoisted — asm:73-77 sit at `loc_E2A51`, the top of the repeat body. Store order `max_j` then `max_i` (asm:74-75) matches production's `max_x` then `max_y`, and both are bare literals in the listing (19, 20) so the named `COMBAT_GRID_WIDTH - 2` / `COMBAT_GRID_HEIGHT - 2` forms evaluate to them exactly. The resulting row stride of `1 + max_x + 1` = 21 is what keeps the edge substitutions on real columns 0 and 20 so diagonals cannot wrap across a row boundary.
- **Two adjacency blocks per phase**, `0..4` then `4..8`, with the explicit reset between them — `cmp di, 4` / `mov di, 4` at asm:142-144, asm:261-263, asm:383-385. Production's paired `RELAX_ADJACENT_CELLS(..., 0, 4, 1)` / `(..., 4, 8, 0)` reproduces both the split and the start index.
- **Diagonal surcharge is `inc al` in the `0..4` block only** — present at asm:115, asm:234, asm:356; absent from the `4..8` blocks at asm:165, asm:284, asm:406. That is exactly the `EXTRA_COST` 1/0 argument. Contrast `Combat_Move_Path_Valid`, which has a single `0..8` loop and no surcharge anywhere.
- **Byte-width accumulation** — `potential_path_cost` is a byte slot (asm:4) fed by `add al` + `inc al` (asm:114-115), so the sum wraps modulo 256 before the unsigned byte `cmp` / `jbe` at asm:119-121. Production's `uint8_t new_cost_to_reach` ([104](../../MoM/src/CMBTMVPT.c#L104)) truncates on assignment at [88](../../MoM/src/CMBTMVPT.c#L88) before the compare at [89](../../MoM/src/CMBTMVPT.c#L89), reproducing that.
- **`ctr++` still runs on the `move_cost == INF` skip path** in all three phases — asm:86 -> asm:194, asm:205 -> asm:313, asm:327 -> asm:435. Production [150](../../MoM/src/CMBTMVPT.c#L150), [162](../../MoM/src/CMBTMVPT.c#L162), [173](../../MoM/src/CMBTMVPT.c#L173).
- **Bounds check** `adjacent_idx >= 0 && adjacent_idx < COMBAT_GRID_CELL_COUNT` — `or si,si` / `jl` then `cmp` / `jge` at asm:102-105 and its five repeats. The listing spells the bound `e_COMBAT_GRID_CELL_COUNT` at asm:104, 154, 223, 395 and the literal `462` at asm:273, 345 — same value.
- **Update-block write order** — `_cmbt_path_data[ctr]` first (asm:122-126), then `_cmbt_mvpth_c[ctr]` (asm:127-130).
- **The redundant re-read** of `_cmbt_path_data[ctr]` before comparing against `current_origin` (asm:131-136) is preserved in the macro rather than reusing `adjacent_idx`. Correct — the OG re-loads.
- **Buffer hijack then count reset, in that order** — asm:447-448 aims the path-index pointer at `_cmbt_movepath_cost_map`, asm:449 zeroes the count. Production [181](../../MoM/src/CMBTMVPT.c#L181) / [183](../../MoM/src/CMBTMVPT.c#L183).
- **Trace loop** — test-at-bottom `while (_cmbt_path_data[ctr] != ctr)` (asm:469-475), body order `store ctr` -> `ctr = _cmbt_path_data[ctr]` -> `count++` (asm:458-468). Production [189-194](../../MoM/src/CMBTMVPT.c#L189-L194).
- **One convert loop, with the index expression recomputed for X and again for Y** — asm:479-488 computes `(count-1)-itr`, scales, loads and runs `idiv`; asm:494-503 recomputes that entire sequence and runs a **second** `idiv`. No common-subexpression elimination, no cached index. Production [198-204](../../MoM/src/CMBTMVPT.c#L198-L204) matches, X stored before Y. (Two `idiv`s rather than one is itself evidence the source was C: hand-written assembler would have taken the quotient and remainder off a single `idiv`, since they come back in `ax` and `dx` together.)
- **`_cmbt_mvpth_x` / `_cmbt_mvpth_y` are word arrays** — asm:489-493 and asm:504-508 scale the index by 2 and write 16 bits. Contrast `_cmbt_mvpth_c` in the same listing, a byte array with no `shl` (asm:58). Production declares them `int16_t *` ([Combat.h:1316](../../MoM/src/Combat.h#L1316), [1319](../../MoM/src/Combat.h#L1319); [Combat.c:1273](../../MoM/src/Combat.c#L1273), [1275](../../MoM/src/Combat.c#L1275)) and stores whole values. The 60-byte allocation at [Combat.c:8991-8992](../../MoM/src/Combat.c#L8991-L8992) is therefore 30 path steps — a snug fit above the ~22-step maximum a shortest path can take on a 21x22 grid.

## OG-faithful behaviors (not bugs — do not "fix")

- **The path-index buffer aliases the cost map.** The path-index pointer is aimed at `_cmbt_movepath_cost_map` (asm:447-448) and then written with word-sized path indices, overwriting the terrain costs the sweep just consumed. Deliberate memory reuse in a 640K program; production reproduces it at [181](../../MoM/src/CMBTMVPT.c#L181).
- **No movement-point cap.** The solver paths to the destination regardless of what the unit can afford; the caller gates on `g_combat_grid_action_mapaction_map` and absorbs any overshoot by clamping at [Combat.c:3323](../../MoM/src/Combat.c#L3323). Combined with `Combat_Move_Path_Valid` charging nothing for diagonals while this function charges `+1`, the reachable set the player is shown is wider than what this solver will actually price. Both halves verified in bytes; original behavior.
- **`_cmbt_mvpth_c` is `uint8_t` with `INF` = 255 as its sentinel**, so a legitimate accumulated cost of 255 is indistinguishable from unreachable (asm:111, asm:161 and repeats compare against 255 / `e_INF`).

## Build state

`cmake --build --preset MSVC-debug` (2026-08-01): [CMBTMVPT.c](../../MoM/src/CMBTMVPT.c), [CMBTAI.c](../../MoM/src/CMBTAI.c) and [Combat.c](../../MoM/src/Combat.c) all compile clean, no warnings, and `momlib.lib` builds. That pass is what confirms the `int16_t` path-array element type carries through every reader in `Move_Battle_Unit__WIP` and the four AI call sites without a type complaint.

The tree does not link: every executable target fails on 8 unresolved externals from `MoM/src/CMBMAGIC.c`, which is registered in [Makefile.am:204](../../Makefile.am#L204) but not in [MoM/src/CMakeLists.txt](../../MoM/src/CMakeLists.txt#L16-L17). Unrelated to this file.

No `stub_wip_todo.md` box to check — `Combat_Move_Path_Find` is not a `__WIP`/`__STUB` function and has no tracker entry.

## Related references

- [Combat-Combat_Move_Path_Valid.md](Combat-Combat_Move_Path_Valid.md) — the sibling solver, DONE-DONE. Shares `PREP`, `RELAX_ADJACENT_CELLS` and `adjacent_offsets`; its frame map documents the three extra slots that function has.
- [CMBTMVPT.c](../../MoM/src/CMBTMVPT.c) — `PREP` [67-75](../../MoM/src/CMBTMVPT.c#L67-L75) and `RELAX_ADJACENT_CELLS` [82-98](../../MoM/src/CMBTMVPT.c#L82-L98) are shared by both functions, so a local-width change in either one changes the macro's operand types there.
- [Combat.c](../../MoM/src/Combat.c) — `Move_Battle_Unit__WIP` (caller, and the main consumer of `_cmbt_mvpth_x`/`_y`), `Assign_Combat_Grids`, `CMB_SetNearAllocs__WIP`.
- [CMBTAI.c](../../MoM/src/CMBTAI.c) — four AI call sites, all reading `movement_path_grid_cell_count` after the call rather than a return value.
- [doc/PathFinding/MoM-MovePath-Compare.md](../PathFinding/MoM-MovePath-Compare.md) — shared five-step skeleton across the three solvers.
- C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr155\Combat_Move_Path_Valid.asm
- C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\dseg\_misc.asm (offset-table data, lines 2949-2951)
