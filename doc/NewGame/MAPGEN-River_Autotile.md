MAPGEN-River_Autotile.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\River_Terrain.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\River_Terrain.c

Init_New_Game()
    |-> River_Autotile(ARCANUS_PLANE);
    |-> River_Autotile(MYRROR_PLANE);

---

# `River_Autotile` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `River_Autotile` | [MAPGEN.c:4985-5472](../../MoM/src/MAPGEN.c#L4985-L5472) | Second river pass: converts the abstract river *placeholders* (`1000`) that `Generate_River` stamped, plus pre-existing lake/shore tiles, into concrete **autotiled** river / shore-outlet terrain based on each tile's river/ocean neighbor connectivity. One `Random(4)` per placeholder picks among 4 visual variants. |

> Name note: `River_Autotile` is the ReMoM name. The OG overlay function (`MAGIC` `ovr051`) and its disassembly authority are named **`River_Terrain`** (`River_Terrain.asm`); this doc cites that file throughout. `Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

`Generate_River` lays rivers down as the placeholder value `TT_RIVER_PLACEHOLDER` (`1000`, [TerrType.h:4](../../MoM/src/TerrType.h#L4)). `River_Autotile` is the **autotiler** that turns those placeholders — and the lake/shore tiles they touch — into the correct terrain graphic for how water connects at each square (which cardinal neighbors are river/ocean). It runs once per plane.

### What makes it an "autotiler" (and how it relates to `Simtex_Autotiling`)

Autotiling = selecting a tile's visual variant automatically from a **bitmask of its neighbors' states**. `River_Autotile` does exactly that for water: it builds a 4-bit cardinal connectivity `river_mask` and selects the matching pre-authored variant (`TILE_River_Types[mask]`, `_1LakeRiv_{N,E,S,W}`, the `_Shore…R…` outlets).

It is the **specialized, second** autotiling pass. The general one is `Simtex_Autotiling` ([MAPGEN.c:3925](../../MoM/src/MAPGEN.c#L3925)), run earlier ([584](../../MoM/src/MAPGEN.c#L584)):

| | `Simtex_Autotiling` | `River_Autotile` |
|---|---|---|
| Runs | before rivers ([584](../../MoM/src/MAPGEN.c#L584)) | after `Generate_River` ([596-597](../../MoM/src/MAPGEN.c#L596)) |
| Keys on | **land** adjacency (8-neighbor ocean shores; 4-neighbor mountains) | **river/ocean** connectivity (4 cardinal) |
| Mask | up to 8-bit (0..255) | 4-bit (0..15) |
| Lookup | data-driven `terrtype[mask]` (LBX table) | hard-coded `switch`/case + `TILE_River_Types` |
| RNG | none (deterministic) | 1 `Random(4)` per placeholder |

`Simtex_Autotiling` lays the base coastline (assigning the `_Shore…` tiles). `River_Autotile` then **refines that output**: blocks 2–9 key on the `_Shore…` value Simtex already assigned and, where a river now meets it, splice in the river-outlet variant — plus they resolve lakes and convert the `1000` placeholders.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:596-597](../../MoM/src/MAPGEN.c#L596-L597) | Per plane, immediately after the `Generate_River` retry loops ([591-592](../../MoM/src/MAPGEN.c#L591-L592)) and before `Shuffle_Terrains` ([606](../../MoM/src/MAPGEN.c#L606)). Followed by `gd_dump_world_map("22_River_Autotile_W")` ([598](../../MoM/src/MAPGEN.c#L598)). |

## RNG footprint (the alignment-relevant part)

**Exactly one `Random(4)` per river-placeholder tile** — [MAPGEN.c:5018](../../MoM/src/MAPGEN.c#L5018):

```
DBG_river_type = TILE_River_Types[river_mask][(Random(4) - 1)];
p_world_map[wp][wy][wx] = DBG_river_type;
```

`River_Autotile`'s total `Random()` consumption equals the number of river tiles `Generate_River` placed (summed over the two per-plane calls). **1:1 with the asm** (`River_Terrain.asm` `loc_49221` → `call Random`):

```
mov ax, 4 ; push ; call Random
shl ax, 1                 ; (rand result) * 2  -> word stride
mov bx, [bp+river_mask]
mov cl, 3 ; shl bx, cl    ; river_mask * 8    = river_mask * 4 words
add bx, ax
mov ax, TILE_River_Types[bx-2]   ; == TILE_River_Types[river_mask*4 + (rand-1)]
```

`TILE_River_Types` is `[16][4]` ([MAPGEN.c:129](../../MoM/src/MAPGEN.c#L129)) — 16 connectivity masks × 4 interchangeable variants. The asm's `river_mask<<3 + rand*2 - 2` is exactly `[river_mask][rand-1]`. **This is the only RNG in the function**; everything else is deterministic table-driven remapping.

> Production splits the OG one-liner into a `DBG_river_type` temp ([5017](../../MoM/src/MAPGEN.c#L5017) keeps the original inline form commented out) — a debug aid only; RNG- and value-identical.

## Per-tile flow

Nested loop over every square (`wy` 0..39, `wx` 0..59). Per square:

1. **Classify** ([5003-5006](../../MoM/src/MAPGEN.c#L5003-L5006)) — read `terrain_type = p_world_map[wp][wy][wx]` **raw** (so the `1000` placeholder survives); if `Square_Is_River_NewGame`, force `terrain_type = TT_RIVER_PLACEHOLDER`.
2. **Placeholder → concrete river** ([5010-5019](../../MoM/src/MAPGEN.c#L5010-L5019)) — for a placeholder, build `river_mask` from **ocean-OR-river** neighbors (`N+=1` wy-1, `E+=2` wx+1, `S+=4` wy+1, `W+=8` wx-1) and place `TILE_River_Types[river_mask][Random(4)-1]`.
3. **Rebuild mask, river-only** ([5022-5026](../../MoM/src/MAPGEN.c#L5022-L5026)) — recompute `river_mask` from **river neighbors only** (same `1/2/4/8` weights). This mask drives all nine blocks below.

## The nine remap blocks

An `if / else-if` chain on `terrain_type` (mutually exclusive — the asm ends each block with `jmp loc_4A85A`, the loop-continue). Each tile hits at most one. All tile writes use the word-strided `p_world_map[wp][wy][wx]`, matching the asm's `shl ax,1` / `mov [word ptr es:bx], …`.

| # | Guard ([line](../../MoM/src/MAPGEN.c)) | Switches on | Cases (river_mask unless noted) | asm dispatch | Role |
|---|---|---|---|---|---|
| 1 | `== _1Lake` ([5029](../../MoM/src/MAPGEN.c#L5029)) | `river_mask` | 0..15 | `off_4A922` | Lake: `0`→`_1Desert`; single inflow→`_1LakeRiv_{N,E,S,W}`; multi-inflow→keep one outlet, grassland the rest, backtrack. |
| 2 | `< _Shore10000000` ([5123](../../MoM/src/MAPGEN.c#L5123)) | `terrain_type` | tt 3..30 | `off_4A8EA` | Single-side shore → river-outlet shore (per-case, one valid direction). |
| 3 | `>= _Shore11000001 && <= _Shore11101110` ([5180](../../MoM/src/MAPGEN.c#L5180)) | `terrain_type` | tt 34..49 | `off_4A8CA` | Two-valid-side shore → single/double outlet (per case: rm 1/8/9, 2/4/6, 1/2/3, 4/8/12). |
| 4 | `>= _Shore11101110 && <= _Shore01000100` ([5268](../../MoM/src/MAPGEN.c#L5268)) | `river_mask` (if-chain) | 1, 4, 5 | `loc_4A148` (base 0x32) | Offset-arithmetic outlet `tt + (RESULT − _Shore11101110)`. |
| 5 | `>= _Shore10010011 && <= _Shore00111001` ([5286](../../MoM/src/MAPGEN.c#L5286)) | `river_mask` (if-chain) | 2, 8, 10 | base 0x42 | Offset-arithmetic outlet `tt + (RESULT − _Shore10010011)`. |
| 6 | `== _Shore00011111 \|\| …` ([5304](../../MoM/src/MAPGEN.c#L5304)) | `river_mask` | 2,4,8,6,10,12,14 | `off_4A8B0` (base 0x52) | Offset-arithmetic outlet `tt + (RESULT − _Shore00011111)`. |
| 7 | `== _Shore11000111 \|\| …` ([5344](../../MoM/src/MAPGEN.c#L5344)) | `river_mask` | 1,4,8,5,9,12,13 | `off_4A896` (base 0x53) | `tt + (RESULT − _Shore11000111)`. |
| 8 | `== _Shore11110001 \|\| …` ([5385](../../MoM/src/MAPGEN.c#L5385)) | `river_mask` | 1,2,8,3,9,10,11 | `off_4A880` (base 0x54) | `tt + (RESULT − _Shore11110001)`. |
| 9 | `== _Shore01111100 \|\| …` ([5425](../../MoM/src/MAPGEN.c#L5425)) | `river_mask` | 1,2,4,3,5,6,7 | `off_4A872` (base 0x55) | `tt + (RESULT − _Shore01111100)`. |

Block-boundary constants are the asm's named symbols ([TerrType.h](../../MoM/src/TerrType.h)): `_Shore10000000`=0x1F(31), `_Shore11000001`=0x22(34), `_Shore11101110`=0x32(50), `_Shore01000100`=0x41(65), `_Shore10010011`=0x42(66), `_Shore00111001`=0x51(81). Block 2's `< _Shore10000000` is the asm guard (`River_Terrain.asm:656`); its `[3,30]` range lives in the switch dispatch gate (`sub bx,3 / cmp bx,27`, lines 662-663), expressed by the case set. Block 3's upper is `_Shore11101110` (50), not 49 — the switch handles 34..49 and `tt==50` falls to the no-op default (asm `loc_4A133` → continue).

## OG behavior worth knowing

- **Mid-iteration backtracking.** Multi-inflow cases (lake `9..15`; the shore offset blocks) convert the *extra* inflow neighbors to `tte_Grasslands` and then rewind the loop vars (`wx -= 1/2; wy -= 1`) so those just-changed neighbors get re-tiled. This is intentional OG behavior (per drake178's note, [5029](../../MoM/src/MAPGEN.c#L5029)); the asm does it via `sub wx,2 / dec wy` before `loc_4A85A`. Preserved verbatim.

## Verification against the asm

Verified **operand-level, 1:1** against `River_Terrain.asm`:
- `Random()` call + `TILE_River_Types` indexing; both river-mask passes (bit weights, N/E/S/W order); loop bounds.
- **Block 1** — all 16 lake cases (result constants, grassland directions, `wx/wy` backtracks via the shared `loc_4967F/4968B/4968E` epilogues).
- **Block 2** — all populated watershed cases + the empty cases routing to the default.
- **Block 3** — 16-case structure (spot-checked the rm 1/8/9 / 2/4/6 / 1/2/3 / 4/8/12 bodies).
- **Blocks 4–9** — every `add ax,(_Shore… − base)` offset expression matched to its case; the six bases confirmed (`0x32/0x42/0x52/0x53/0x54/0x55`).
- Tile writes are word-strided `p_world_map[wp][wy][wx]` (`int16_t (*)[40][60]`, [MOM_DAT.h:4095](../../MoX/src/MOM_DAT.h#L4095)) — matching the asm's `shl ax,1` / `mov [word ptr es:bx]`, **not** a byte-strided view of the raw `_world_maps` heap (`uint8_t *`, which would halve the offset and truncate values > 255 such as `_1Desert`=0x134; reaching a tile through `_world_maps` needs the `*2` stride per [MOX_DEF.h:429](../../MoX/src/MOX_DEF.h#L429)).

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — `1..n`; one `Random(4)` per placeholder tile.
- **`Square_Is_River_NewGame` / `Square_Is_Ocean_NewGame`** — neighbor connectivity tests feeding the masks.
- **`TILE_River_Types[16][4]`** ([MAPGEN.c:129](../../MoM/src/MAPGEN.c#L129)) — mask → 4 interchangeable river-tile variants.
- **`p_world_map`** ([MOM_DAT.h:4095](../../MoX/src/MOM_DAT.h#L4095)) — the `int16_t (*)[40][60]` tile view; read/written. Terrain constants `_1Lake`, `_1Desert`, `_1LakeRiv_*`, `_Shore*`, `tte_Grasslands`.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\River_Terrain.asm` — IDA Pro 5.5 disassembly (authority; OG name `River_Terrain`); `loc_49221` = `Random(4)`/`TILE_River_Types`; jump tables `off_4A922` (block 1), `off_4A8EA` (2), `off_4A8CA` (3), `off_4A8B0/896/880/872` (blocks 6-9).
- [MAPGEN-Generate_River.md](MAPGEN-Generate_River.md) — the first river pass that stamps the `1000` placeholders this function consumes.
- `Simtex_Autotiling` ([MAPGEN.c:3925](../../MoM/src/MAPGEN.c#L3925)) — the general, first-pass coastline/terrain autotiler this function refines.
- [MAPGEN.c:596-597](../../MoM/src/MAPGEN.c#L596-L597) — call sites; [3759](../../MoM/src/MAPGEN.c#L3759) — downstream reader of the `_1Desert` tiles created here.
