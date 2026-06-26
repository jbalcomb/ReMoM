MAPGEN-Build_Connectivity_Graphs.md

SEEALSO: C:\STU\devel\ReMoM\doc\NewGame\MAPGEN-NIU_Path_Tables.md
SEEALSO: C:\STU\devel\ReMoM\doc\PathFinding\MoM-MovePath-Compare.md
SEEALSO: C:\STU\devel\ReMoM\doc\PathFinding\MoM-MovePath.md
SEEALSO: C:\STU\devel\ReMoM\doc\PathFinding\MoX-Combat-PathFindAlgo.md

OG disassembly files retain drake178's original labels (the C functions have since been renamed):
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\CRP_NEWG_CreatePathGrids__WIP.asm   → Build_Connectivity_Graphs
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr055\CRP_NEWG_CreatePathGrid__STUB.asm    → Build_Connectivity_Graph
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr054\CRP_SPATH_Arbitrary.asm              → Find_Shortest_Path

Init_New_Game()
    |-> Build_Connectivity_Graphs(ARCANUS_PLANE);
    |-> Build_Connectivity_Graphs(MYRROR_PLANE);
            |-> Build_Connectivity_Graph(walking.moves2, connectivity_grid_land + plane record);
            |-> Build_Connectivity_Graph(sailing.moves2, connectivity_grid_sea + plane record);
                    |-> Find_Shortest_Path(...)   // brute-force shortest path per 5×5 / 7×5 section

---

# `Build_Connectivity_Graphs` / `Build_Connectivity_Graph` / `Find_Shortest_Path` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Build_Connectivity_Graphs` | [MAPGEN.c:6192-6199](../../MoM/src/MAPGEN.c#L6192-L6199) | Thin per-plane wrapper: maps the EMM cost-map bank, then builds two path grids for the plane — **walking → `connectivity_grid_land`**, **sailing → `connectivity_grid_sea`**. |
| `Build_Connectivity_Graph` | [MAPGEN.c:8104-8418](../../MoM/src/MAPGEN.c#L8104-L8418) | Slices one plane's 60×40 cost map into a 12×8 grid of 5×5 sections and writes a 1-byte adjacency bitflag per section (96 bytes) into the target connectivity-grid record (`connectivity_grid_land` or `connectivity_grid_sea`). |
| `Find_Shortest_Path` | [MAPGEN.c:7692-8037](../../MoM/src/MAPGEN.c#L7692-L8037) | Brute-force shortest-path solver over an arbitrary `Wdt × Hgt` section map; returns path length (0 = no path). `Build_Connectivity_Graph` uses it only to test whether a path *exists* between section edges. |

## Purpose

This is the final new-game world step. For each plane it produces two **section-connectivity grids** — one for walking, one for sailing — describing, for every 5×5 block of the map, which of its four cardinal neighbours are reachable. The results live in `connectivity_grid_land` (walking) and `connectivity_grid_sea` (sailing).

**These two arrays are Not In Use (NIU).** drake178's header on `Find_Shortest_Path` states it outright: *"the arrays created by this function are never used by the game, and can be repurposed even though they are part of the save game file"* ([MAPGEN.c:7664-7677](../../MoM/src/MAPGEN.c#L7664-L7677)); the in-code `OGBUG` comment restates it (*"connectivity_grid_land/connectivity_grid_sea are NIU (never read for gameplay)"*, [MAPGEN.c:8410](../../MoM/src/MAPGEN.c#L8410)). Nothing reads `connectivity_grid_land`/`connectivity_grid_sea` for gameplay; they are computed, **saved** (and re-loaded), and otherwise dead.

**RNG-neutral: zero `Random()` calls** across all three functions. Does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:615-616](../../MoM/src/MAPGEN.c#L615-L616) | Called once per plane, after `Movement_Mode_Cost_Maps` (which fills the `walking`/`sailing` cost maps this reads) and `Draw_Building_The_Worlds(90)`. Followed by the `[connectivity_grids] event=calc` lifecycle trace ([625-635](../../MoM/src/MAPGEN.c#L625-L635)). |

## The `connectivity_grid_land` / `connectivity_grid_sea` arrays

| Property | Value | Source |
|---|---|---|
| Declared type | `int8_t *` (flat pointer) | [MOM_DAT.h:4092-4093](../../MoX/src/MOM_DAT.h#L4092-L4093) |
| Allocation | `Allocate_Next_Block(World_Data, 14)` = 14 paragraphs = **224 B** each | [ALLOC.c:112-113](../../MoM/src/ALLOC.c#L112-L113) |
| Bytes used | `NUM_PLANES * 96` = **192 B** (one **96-byte record per plane**; 32 B slack) | [ALLOC.c:129](../../MoM/src/ALLOC.c#L129) |
| Record-size macro | `CONNECTIVITY_GRID_{LAND,SEA}_RECORD_SIZE = 2*96` = 192 | [MOM_DAT.h:304-305](../../MoX/src/MOM_DAT.h#L304-L305) |
| Layout | `12 cols × 8 rows = 96` section bytes per plane; byte = `result_map[row*12 + col]` | `Build_Connectivity_Graph` [8388](../../MoM/src/MAPGEN.c#L8388) |
| Saved? | Yes — `stu_fwrite(connectivity_grid_land, NUM_PLANES, 96, …)` / `stu_fread(…)` | [LOADSAVE.c:100-101](../../MoX/src/LOADSAVE.c#L100-L101), [293-294](../../MoX/src/LOADSAVE.c#L293-L294) |
| Read for gameplay? | **No (NIU)** | drake178 header |
| Per-plane stride | `&connectivity_grid_land[wp * 96]` — matches asm `imul 96` ([MAPGEN.c:6196-6197](../../MoM/src/MAPGEN.c#L6196-L6197)) | `Build_Connectivity_Graphs` |

Each section byte packs an 8-bit adjacency mask: bits `1/2/4/8` = N/E/S/W "neighbour reachable", and the high nibble `0x10/0x20/0x40/0x80` mirrors those as "edge passable in that direction" (set in `Build_Connectivity_Graph`'s per-direction blocks).

## Findings

### B1 — reciprocal-pass east-column bound tests the row index (OGBUG, preserved) ([MAPGEN.c:8410-8414](../../MoM/src/MAPGEN.c#L8410-L8414))

`Build_Connectivity_Graph`'s final reciprocal pass propagates each section's connectivity bit to its neighbour. The East→West reciprocal guards against the rightmost column with the **wrong loop variable**:

```c
if((result_map[Y_GridIndex_2 * 12 + X_GridIndex_2] & 2) && Y_GridIndex_2 < 11)   /* should be X_GridIndex_2 < 11 */
{
    result_map[Y_GridIndex_2 * 12 + X_GridIndex_2 + 1] |= 8;
}
```

The asm confirms it: `cmp [bp+Y_GridIndex_2], 0Bh / jge` (`CRP_NEWG_CreatePathGrid__STUB.asm`, `loc_5227A`, line 1106) compares the **row** index `Y_GridIndex_2` to 11, not the **column** index. Since `Y_GridIndex_2 ∈ [0,7]`, `< 11` is always true, so even at `X_GridIndex_2 == 11` it writes `result_map[Y*12 + 12]` = `result_map[(Y+1)*12 + 0]`, leaking a West bit into column 0 of the next row down. Faithful to the OG; harmless (NIU). Already marked `/* OGBUG … Do not "fix". */` in-code. Preserve.

The other three reciprocal directions guard correctly — N (`Y_GridIndex_2 != 0`), S (`Y_GridIndex_2 < 7`), W (`X_GridIndex_2 != 0`) — matching asm `loc_521EA` / `loc_52232` and the bit-1 block.

## Verification against the asm

| Item | OG asm | Production |
|---|---|---|
| EMM bank map | `call EMM_Map4_EMMDATAH` | `EMMDATAH_Map()` ✓ |
| Walking → `connectivity_grid_land` | `…+ s_MOVE_MODE_COST_MAPS.Walking` → `connectivity_grid_land + wp*96` | `&…walking.moves2[0]` → `&connectivity_grid_land[wp * 96]` ✓ |
| Sailing → `connectivity_grid_sea` | `…+ s_MOVE_MODE_COST_MAPS.Sailing` → `connectivity_grid_sea + wp*96` | `&…sailing.moves2[0]` → `&connectivity_grid_sea[wp * 96]` ✓ |
| Grid loops | `Y_GridIndex 0..7`, `X_GridIndex 0..11`; `X_GridBase += 5`, `itr1 += 5` | same ✓ |
| Section byte store | `result_map[Y*12 + X]` (`imul 12`) | `result_map[Y_GridIndex*12 + X_GridIndex]` ✓ |
| Impassable sentinel | `cmp …, -1` / `0FFh` | `!= (char)-1` ✓ |
| Reciprocal N / S / W | `Y!=0` / `Y<7` / `X!=0` | same ✓ |
| Reciprocal E bound | `cmp Y_GridIndex_2, 0Bh` (asm 1106 — wrong var) | `Y_GridIndex_2 < 11` (**B1**, faithful) ✓ |
| `Find_Shortest_Path` skeleton | 5-step solver (prelude offsets → bail → init → relax-to-fixed-point → backtrace+convert) | same; see [MoM-MovePath-Compare.md](../PathFinding/MoM-MovePath-Compare.md) ✓ |
| RNG | no `call Random` | none ✓ |

The three functions are faithful to the asm; the only differences are name aliases (`.Walking`/`walking`, `EMM_Map4_EMMDATAH`/`EMMDATAH_Map`) and the preserved **B1** OGBUG.

## Tests

Unit-tested in `MoM/tests/` (gtest, in `MOM_tests`). All three functions are deterministic (zero `Random()`), so the cases assert exact known values:

| Test file | Cases | Covers |
|---|---|---|
| [test_Find_Shortest_Path.cpp](../../MoM/tests/test_Find_Shortest_Path.cpp) | 8 | the solver on mocked grids: exact Chebyshev length on an open grid; length-1 orthogonal & diagonal neighbours (8-connectivity); a single-row corridor asserting the unique route hop-by-hop; source/target impassable and a full wall → `0`; a wall-forced detour. A `CheckPath` helper validates every returned path is a contiguous, in-bounds, passable 8-neighbour chain ending at the target. |
| [test_Build_Connectivity_Graph.cpp](../../MoM/tests/test_Build_Connectivity_Graph.cpp) | 3 | the section grid: all-passable (`0xEE` top row / `0xFF` interior / `0xBB` bottom row), all-impassable (all `0x00`), and a 2-row barrier on a section boundary that severs exactly section-row 3's south and row 4's north — the discriminating mixed-geometry case. |
| [test_Build_Connectivity_Graphs.cpp](../../MoM/tests/test_Build_Connectivity_Graphs.cpp) | 1 | the wrapper: walking → `connectivity_grid_land`, sailing → `connectivity_grid_sea`, and the `wp*96` per-plane stride (plane-0 and plane-1 records stay independent). |

The fixtures back `move_map` with leading/trailing padding for the OG out-of-bounds reads (and `result_map` slack for the **B1** east-column write); the wrapper test nulls `_EMMDATAH_seg` so `EMMDATAH_Map()` is a no-op and `movement_mode_cost_maps` can be a plain allocation. That the barrier case matched the geometry predicted from the disassembly is independent confirmation of the reconstruction.

## Sub-functions / external calls

- **`EMMDATAH_Map`** — maps the EMM page holding `movement_mode_cost_maps` (asm `EMM_Map4_EMMDATAH`).
- **`Find_Shortest_Path`** — brute-force shortest-path existence test; uses globals `shortest_path_came_from` / `shortest_path_cost_to_reach` / `shortest_path_backtrace` ([MAPGEN.c:217-222](../../MoM/src/MAPGEN.c#L217-L222); max 225 tiles, path ≤ 100, no wrap). Shares the 5-step skeleton with the overland/combat solvers — see [MoM-MovePath-Compare.md](../PathFinding/MoM-MovePath-Compare.md).
- **`movement_mode_cost_maps[wp].{walking,sailing}.moves2[]`** — the per-square cost maps built by `Movement_Mode_Cost_Maps`; the `move_map` input here.

## Related references

OG disassembly (authority) — files keep drake178's labels:
- `…\ovr051\CRP_NEWG_CreatePathGrids__WIP.asm` (= `Build_Connectivity_Graphs`) — `connectivity_grid_land + wp*96` (asm 11-17), `connectivity_grid_sea + wp*96` (asm 28-34).
- `…\ovr055\CRP_NEWG_CreatePathGrid__STUB.asm` (= `Build_Connectivity_Graph`) — result store `imul 12` (asm 1041), reciprocal E-bound `cmp Y_GridIndex_2, 0Bh` (asm 1106).
- `…\ovr054\CRP_SPATH_Arbitrary.asm` (= `Find_Shortest_Path`) — path solver.

Source / docs:
- [MAPGEN.c:615-616](../../MoM/src/MAPGEN.c#L615-L616) — call sites + `[connectivity_grids] event=calc` trace.
- [ALLOC.c:112-142](../../MoM/src/ALLOC.c#L112-L142) — allocation + `[connectivity_grids] event=allocate` trace; [LOADSAVE.c:100-101](../../MoX/src/LOADSAVE.c#L100-L101) / [293-296](../../MoX/src/LOADSAVE.c#L293-L296) — save/load + post-`fread` trace.
- [MAPGEN-Movement_Mode_Cost_Maps.md](MAPGEN-Movement_Mode_Cost_Maps.md) — builds the `walking`/`sailing` cost maps this consumes.
- [MAPGEN-NIU_Path_Tables.md](MAPGEN-NIU_Path_Tables.md) — companion overview of the NIU path tables.
- [MoM-MovePath-Compare.md](../PathFinding/MoM-MovePath-Compare.md) — the shared shortest-path skeleton (`Find_Shortest_Path` variant).
