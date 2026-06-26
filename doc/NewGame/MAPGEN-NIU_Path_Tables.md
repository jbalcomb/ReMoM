MAPGEN-NIU_Path_Tables.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Build_Connectivity_Graphs.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Build_Connectivity_Graphs.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr055\Build_Connectivity_Graph.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr055\Build_Connectivity_Graph.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr054\Find_Shortest_Path.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr054\Find_Shortest_Path.c

Build_Connectivity_Graphs()
    |-> Build_Connectivity_Graph()
        |-> Find_Shortest_Path()

Init_New_Game()
    |-> Build_Connectivity_Graphs(ARCANUS_PLANE);
        |-> Build_Connectivity_Graph(&movement_mode_cost_maps[wp].walking.moves2[0], &connectivity_grid_land[wp]);
        |-> Build_Connectivity_Graph(&movement_mode_cost_maps[wp].sailing.moves2[0], &connectivity_grid_sea[wp]);
    |-> Build_Connectivity_Graphs(MYRROR_PLANE);
        |-> Build_Connectivity_Graph(&movement_mode_cost_maps[wp].walking.moves2[0], &connectivity_grid_land[wp]);
        |-> Build_Connectivity_Graph(&movement_mode_cost_maps[wp].sailing.moves2[0], &connectivity_grid_sea[wp]);

Elsewhere, for Gemini-Online, box up:
    ovr051
        Build_Connectivity_Graphs.asm
    ovr055
        Build_Connectivity_Graph.asm
    ovr054
        Find_Shortest_Path.asm
        UU_SPATH_Segmented.asm
        UU_SPATH_15by15.asm
        UU_SPATH_Segments.asm
        UU_SPATH_GetBorderTiles.asm

Role                        Overland (Move_Path_Find)   Combat (Combat_Move_Path_Find)  NIU (Find_Shortest_Path) — current
cost-so-far (init INF)      Reach_Costs                 _cmbt_mvpth_c                   shortest_path_cost_to_reach
predecessor (self-init)     Reach_From                  _cmbt_path_data                 shortest_path_came_from
reversed back-trace buffer  Reverse_Path                (reuses cost map)               shortest_path_backtrace
---

# NIU Path Tables (`connectivity_grid_land` / `connectivity_grid_sea`) — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Build_Connectivity_Graphs` | [MAPGEN.c:6099-6105](../../MoM/src/MAPGEN.c#L6099-L6105) | Per-plane wrapper. Refreshes the EMM map window, then builds two section-connectivity grids for plane `wp`: walking → `connectivity_grid_land[wp]`, sailing → `connectivity_grid_sea[wp]`. |
| `Build_Connectivity_Graph` | [MAPGEN.c:8074-8388](../../MoM/src/MAPGEN.c#L8074-L8388) | Slices the 60×40 movement-cost map into an 8×12 grid of 5×5 sections and writes one byte per section — a bitflag of which of the 4 cardinal directions you can path *out of* that section — then a second pass adds the reciprocal "into" bits on neighbors. |
| `Find_Shortest_Path` | [MAPGEN.c:7686-8031](../../MoM/src/MAPGEN.c#L7686-L8031) | Brute-force shortest path on a small (≤225-tile) sub-map. Returns the path length in tiles (0 if none), and fills caller-supplied X/Y coordinate arrays with the route. Called once per section edge by the `__STUB`. |

**Headline:** these three functions build the per-plane "path grid" tables `connectivity_grid_land` (land) and `connectivity_grid_sea` (sea). The tables are computed at new-game time, persisted in the save file — and **never read back for gameplay**. They are **Not In Use (NIU)**. They are also **RNG-neutral: zero `Random()` calls** across all three functions, so they cannot perturb the new-game `Random()` call sequence we are aligning against OG-MoM.

## Why this doc exists

This cluster surfaces in two ways during the new-game review:

1. **RNG alignment** — `Build_Connectivity_Graphs` runs inside `Init_New_Game`, so it sits in the new-game call chain. Verified: it consumes **no** `Random()` calls (neither does either callee). It is invisible to the PRNG sequence.
2. **Save-file fidelity / repurposing** — `connectivity_grid_land`/`connectivity_grid_sea` occupy real bytes in the save file but are dead. drake178's header comment on `Find_Shortest_Path` notes the arrays "are never used by the game, and can be repurposed even though they are part of the save game file."

## RNG-neutrality (verified)

No `Random()` call appears in any of the three function bodies:

- `Find_Shortest_Path` — [MAPGEN.c:7686-8031](../../MoM/src/MAPGEN.c#L7686-L8031): grep for `Random(` in this range returns nothing.
- `Build_Connectivity_Graph` — [MAPGEN.c:8074-8388](../../MoM/src/MAPGEN.c#L8074-L8388): grep for `Random(` returns nothing.
- `Build_Connectivity_Graphs` — [MAPGEN.c:6099-6105](../../MoM/src/MAPGEN.c#L6099-L6105): only an `EMMDATAH_Map()` call and the two `__STUB` calls.

These are pure deterministic transforms of the movement-cost maps. They can be reordered, stubbed, or skipped without affecting the new-game `Random()` sequence.

## NIU evidence — `connectivity_grid_land` / `connectivity_grid_sea`

| Lifecycle stage | Site | Notes |
|---|---|---|
| Declared | [MOM_DAT.h:4092-4093](../../MoX/src/MOM_DAT.h#L4092-L4093) | `extern SAMB_ptr connectivity_grid_land/2;` Record size `2*96` ([MOM_DAT.h:304-305](../../MoX/src/MOM_DAT.h#L304-L305)) = `NUM_PLANES`(2) × 96. |
| Allocated | [ALLOC.c:98-99](../../MoM/src/ALLOC.c#L98-L99) | Carved out of the `World_Data` save block. |
| **Written** | [MAPGEN.c:6103-6104](../../MoM/src/MAPGEN.c#L6103-L6104) | **Only** here (via the `__STUB`). Nothing else writes them. |
| Saved | [LOADSAVE.c:100-101](../../MoX/src/LOADSAVE.c#L100-L101) | `stu_fwrite(connectivity_grid_land, NUM_PLANES, 96, ...)` — 96 bytes/plane = the 8×12 grid. |
| Loaded | [LOADSAVE.c:294-295](../../MoX/src/LOADSAVE.c#L294-L295) | `stu_fread(...)` — round-trips the dead bytes. |
| **Read for gameplay** | — | **None.** Only other reference is the debug dumper ([MOM_DBG.c:96-97](../../MoM/src/MOM_DBG.c#L96-L97)). |

The data is produced, serialized, and deserialized, but no movement / AI / pathing code ever consults it. Confirmed NIU.

## `Build_Connectivity_Graphs` — code walk

[MAPGEN.c:6099-6105](../../MoM/src/MAPGEN.c#L6099-L6105). **Verified 1:1 with `ovr051` asm.**

```
EMMDATAH_Map();                                                                            // map the EMM data window
Build_Connectivity_Graph(&movement_mode_cost_maps[wp].walking.moves2[0], &connectivity_grid_land[wp]);  // land grid
Build_Connectivity_Graph(&movement_mode_cost_maps[wp].sailing.moves2[0], &connectivity_grid_sea[wp]);  // sea grid
```

The asm calls `EMM_Map4_EMMDATAH` then the section-grid builder twice. Argument order matches: arg1 = the movement-cost map (`walking`/`sailing` `moves2`), arg2 = the result table (`connectivity_grid_land`/`connectivity_grid_sea`). Called once per plane from `Init_New_Game` ([MAPGEN.c:484-485](../../MoM/src/MAPGEN.c#L484-L485)). **Faithful.**

## `Build_Connectivity_Graph` — code walk

[MAPGEN.c:8074-8388](../../MoM/src/MAPGEN.c#L8074-L8388). Signature `(void * move_map, void * result_map)` — `move_map` = the 60×40 movement-cost map (stride 60), `result_map` = the 8×12 byte grid (= one plane of a connectivity grid, `connectivity_grid_land`/`connectivity_grid_sea`). Both names match the asm stack args `MoveMap@` / `Result@`.

The function's own comment: *"creates a road map type structure slicing the passed map into 5 by 5 segments, and representing the available paths out of each one as a single byte bitflag."*

### Direction bitflags

Each section's byte packs four "can path out" bits and (after the second pass) four reciprocal "can path in" bits:

| Bit | Dir | Reciprocal set on neighbor |
|---|---|---|
| `0x01` | North | `0x04` (South) on section above |
| `0x04` | South | `0x01` (North) on section below |
| `0x08` | West  | `0x02` (East) on section to the left |
| `0x02` | East  | `0x08` (West) on section to the right |

### Grid build ([8099-8362](../../MoM/src/MAPGEN.c#L8099-L8362))

Two nested loops, `Y_GridIndex` 0..7 (rows) and `X_GridIndex` 0..11 (cols) — the 8×12 = 96-entry grid. For each section it copies the relevant edge sub-maps (`TopBottom_Section_Map` 5×7, `LeftRight_Section_Map` 7×5) out of `move_map` and runs `Find_Shortest_Path` across each of the four edges; a found path (return ≠ 0) sets the corresponding direction bit in `Passable_Direction_Flags`. The byte is stored at `result_map[Y_GridIndex * 12 + X_GridIndex]` ([8324](../../MoM/src/MAPGEN.c#L8324)).

### Reciprocal-bit pass ([8330-8351](../../MoM/src/MAPGEN.c#L8330-L8351))

A second `Y_GridIndex_2`/`X_GridIndex_2` double-loop ORs the mirror bit onto each in-bounds neighbor so connectivity is symmetric. This is what drake178's WARNING describes: *"matching the connections in the end means that they may be present even if the original criteria are not actually satisfied from a direction"* — the symmetrization can mark a connection that the per-edge pathfind did not independently confirm from that side.

## OG quirks preserved

### B1 — East-direction bound tests the wrong loop variable ([8346](../../MoM/src/MAPGEN.c#L8346)) — OGBUG, faithful

The four reciprocal-bit guards should each bound the axis they move along:

- North ([8334](../../MoM/src/MAPGEN.c#L8334)): `Y_GridIndex_2 != 0` — correct (don't go above row 0).
- South ([8338](../../MoM/src/MAPGEN.c#L8338)): `Y_GridIndex_2 < 7` — correct (don't go below row 7).
- West ([8342](../../MoM/src/MAPGEN.c#L8342)): `X_GridIndex_2 != 0` — correct (don't go left of col 0).
- East ([8346](../../MoM/src/MAPGEN.c#L8346)): `Y_GridIndex_2 < 11` — **should be `X_GridIndex_2 < 11`** (don't go right of col 11).

Since `Y_GridIndex_2 ∈ [0,7]`, the test `< 11` is *always true*, so the East→West reciprocal bit is written even for the rightmost column (`X_GridIndex_2 == 11`). The write `result_map[Y_GridIndex_2 * 12 + X_GridIndex_2 + 1]` then targets index `Y*12 + 12` = `(Y+1)*12 + 0` — i.e. it leaks a West bit into column 0 of the next row down.

This is **faithful to the OG**, not a reconstruction error. The `ovr055` asm `loc_5227A` block tests the row variable, not the column variable:

```
loc_5227A:
    ... test [byte ptr es:bx], 2        ; East bit
    jz   short loc_522C2
    cmp  [bp+Y_GridIndex_2], 0Bh        ; <-- compares Y_GridIndex_2 to 11, NOT X_GridIndex_2
    jge  short loc_522C2
    ... or al, 8 ; write West bit on [es:bx+1]
```

(The other three blocks — `loc_521A2`, `loc_521EA`, `loc_52232` — correctly test `Y`,`Y`,`X` respectively, matching production.) Harmless in practice: the table is NIU. **OGBUG — preserve; do not "fix."**

## `Find_Shortest_Path` — code walk

[MAPGEN.c:7686-8031](../../MoM/src/MAPGEN.c#L7686-L8031). `int16_t Find_Shortest_Path(int SrcX, int SrcY, int TgtX, int TgtY, int Wdt, int Hgt, unsigned char * Map, unsigned char * Xs, unsigned char * Ys)`.

drake178's header: *"a brute force shortest path algorithm that fills the passed arrays with the result, returning the length of the path in tiles, or 0 if none found; WARNING: uses global arrays limited to a maximum map size of 225 tiles and path length of 100, with no wrapping."*

A flood/expansion shortest-path on the small (`Wdt`×`Hgt` ≤ 225) section sub-map `Map`, from `(SrcX,SrcY)` to `(TgtX,TgtY)`. On success it back-traces the route into the caller's `Xs`/`Ys` arrays and returns the tile count; on failure it returns 0. The `__STUB` only uses the **return value** (≠ 0 → the edge is passable) — the filled `Xs`/`Ys` route arrays are themselves discarded, which is part of why drake178 flags the whole output as repurposable. No map wrapping (fine — sections are interior 5×7 / 7×5 windows). **RNG-neutral.**

## Review depth / honesty

- `Build_Connectivity_Graphs` — reviewed operand-by-operand against `ovr051`; **1:1**.
- `Build_Connectivity_Graph` — reviewed structurally (purpose, the 8×12/5×5 tiling, the result store, and the full reciprocal-bit pass byte-checked against `ovr055`, including the B1 East-bound OGBUG). Not every edge-submap copy was diffed operand-by-operand.
- `Find_Shortest_Path` — reviewed at the purpose/contract level (brute-force shortest path, return-value semantics, RNG-neutral) per drake178's notes; the long search loop was **not** audited operand-by-operand.

The lighter audit on the two large callees is justified: both are **RNG-neutral** (cannot affect new-game alignment) and **NIU** (output never read for gameplay). If `connectivity_grid_land`/`connectivity_grid_sea` are ever repurposed for real pathing, `Find_Shortest_Path` would need a full operand-level pass first.

## Sub-functions / external calls

- **`EMMDATAH_Map`** — maps the EMM data window before the grid build.
- **`Find_Shortest_Path`** ([MAPGEN.c:7686](../../MoM/src/MAPGEN.c#L7686)) — per-edge shortest-path test.
- **`movement_mode_cost_maps[wp].walking/sailing.moves2`** — per-tile movement-cost source maps (input).
- **`connectivity_grid_land` / `connectivity_grid_sea`** ([MOM_DAT.h:4092-4093](../../MoX/src/MOM_DAT.h#L4092-L4093)) — the NIU output tables.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Build_Connectivity_Graphs.asm` — wrapper (the authority).
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr055\Build_Connectivity_Graph.asm` — grid builder; `loc_5227A` = the B1 East-bound block.
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr054\Find_Shortest_Path.asm` — brute-force pathfinder.
- [LOADSAVE.c:100-101](../../MoX/src/LOADSAVE.c#L100-L101) / [294-295](../../MoX/src/LOADSAVE.c#L294-L295) — save/load of the dead tables.
- [MAPGEN.c:484-485](../../MoM/src/MAPGEN.c#L484-L485) — `Init_New_Game` call sites.
