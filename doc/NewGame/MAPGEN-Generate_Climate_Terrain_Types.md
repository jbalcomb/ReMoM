MAPGEN-Generate_Climate_Terrain_Types.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Climate_Terrain_Types.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Climate_Terrain_Types.c

Init_New_Game()
    |-> Generate_Climate_Terrain_Types(ARCANUS_PLANE);
    |-> Generate_Climate_Terrain_Types(MYRROR_PLANE);

---

# `Generate_Climate_Terrain_Types` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Generate_Climate_Terrain_Types` | [MAPGEN.c:2057-2198](../../MoM/src/MAPGEN.c#L2057-L2198) | Overlays climate features on the base terrain of one plane: polar **Tundra** bands, then 8 random-walked **Desert** patches, then 8 random-walked **Swamp** patches. |

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

After `Translate_Heightmap_To_Base_Terrain_Types` lays grass/forest/hills/etc.,
this paints climate: tundra near the poles, deserts and swamps as randomly-walked
blobs seeded on forest squares.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:500-504](../../MoM/src/MAPGEN.c#L500-L504) | Called once per plane; followed by `gd_dump_world_map("05_ClimateTerrain_W")`. |

## RNG footprint (alignment-critical)

This function is RNG-heavy; draw sites and gating match the asm exactly:

- **Tundra** ([2080-2107](../../MoM/src/MAPGEN.c#L2080-L2107)) — for `itr_wy` 2..7 × `itr_wx` 0..59: **one `Random(8)`** per *north*-band square that is grassland/forest, and **one** per *south*-band square (`WORLD_YMAX - itr_wy`) that is grassland/forest. (The `Random(8)` is computed inside the grass/forest guard, so the count is map-dependent — same gating as the asm.)
- **Desert** ([2116-2148](../../MoM/src/MAPGEN.c#L2116-L2148)) — 8 patches; per patch: `Random(54)` (base_wx), `Random(24)` (base_wy), then 5 directions × [ `Random(6)` (step count) + `Steps` × `Random(4)` (walk direction) ].
- **Swamp** ([2157-2191](../../MoM/src/MAPGEN.c#L2157-L2191)) — 8 patches; per patch: `Random(56)` (base_wx), `Random(20)` (base_wy), then 5 directions × [ `Random(3)` (step count) + `Steps` × `Random(4)` ].

## How it works

1. **Tundra polar bands** — for the top six interior rows (2..7) and the mirrored bottom rows (`39 - itr_wy` = 37..32), any grassland/forest square becomes `tt_Tundra1` when `(2 + Random(8)) >= itr_wy` — i.e. probability tapers toward the equator.
2. **Desert patches** — 8 times: pick `base = (2+Random(54), 8+Random(24))`; if it's `tt_Forest1`, make it `tt_Desert1`. Then for each of the 5 `dir_chg_tbl` offsets (S/W/N/E/center, [MAPGEN.c:117-118](../../MoM/src/MAPGEN.c#L117-L118)) seed `curr` and random-walk `4+Random(6)` steps, each step moving by a random cardinal (`Random(4)-1`) with world-wrap; any non-`tt_Ocean` square walked becomes `tt_Desert1`.
3. **Swamp patches** — 8 times: `do { base = (1+Random(56), 10+Random(20)); } while(base_wy in 35..45)` (the re-roll band is dead — never true, see B1); if `tt_Forest1`, make `tt_Swamp1`. Then 5 directions × `2+Random(3)` steps; any `tt_Forest1` square walked becomes `tt_Swamp1`.

## Findings

### B1 — dead Swamp equatorial-band re-roll (OGBUG, preserved) ([MAPGEN.c:2159-2163](../../MoM/src/MAPGEN.c#L2159-L2163))

```c
do
{
    base_wx = ( 1 + Random((WORLD_WIDTH  - ( 2 * 2))));  // 1 + Random(56)  →  2..57
    base_wy = (10 + Random((WORLD_HEIGHT - (10 * 2))));  // 10 + Random(20) → 11..30
} while((base_wy >= 35) && (base_wy <= 45));  // OGBUG out of bounds; meant as an equatorial band?
```

`base_wy` is always in **11..30**, so the `35..45` re-roll condition is **never true** — a dead OG quirk (probably meant as an equatorial exclusion band, but the bounds are wrong / out of the 0..39 map). Preserve.

This `do/while` is the byte-exact reconstruction of the asm. The band check `jmp short @@Loop_Swamp` (asm 328) jumps **directly to the loop body top** (`@@Loop_Swamp`, asm 311) — re-rolling `base_wx`/`base_wy` *without* re-testing `itr`. That shared body-top target (the outer `for` test at asm 470-473 also jumps to `@@Loop_Swamp`) is exactly what a `do { base draw } while(band)` nested first in the `for` body compiles to. A `continue` (which routes through the `itr < 8` test) or an `itr--; continue;` would **not** be 1:1 — both re-run the loop test that the asm bypasses.

### Preserved quirk — `new_direction = ST_UNDEFINED` is set but never used

Both the Desert and Swamp direction loops assign `new_direction = ST_UNDEFINED` ([2129](../../MoM/src/MAPGEN.c#L2129), [2172](../../MoM/src/MAPGEN.c#L2172)) and never read it — a c&p artifact present in the asm too (`mov [bp+new_direction], e_ST_UNDEFINED`). Faithful.

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| Tundra loops | `next_wy 2..7` (`cmp ,8`), `next_wx 0..WORLD_WIDTH` | `itr_wy 2..8`, `itr_wx < WORLD_WIDTH` ✓ |
| Tundra north/south | grass/forest guard → `Random(8)+2 >= wy` → `tt_Tundra1`; south row `39 - next_wy` | same, `WORLD_YMAX - itr_wy` ✓ |
| Desert seeds | `2+Random(54)`, `8+Random(24)`; forest→`tt_Desert1` | same ✓ |
| Desert walk | 5 dirs; `4+Random(6)` steps; `Random(4)-1` move; 4-way wrap; non-`tt_Ocean1`→`tt_Desert1` | same ✓ |
| Swamp seeds | `1+Random(56)`, `10+Random(20)`; `35..45` band check | same (band dead — **B1**) ✓ |
| Swamp band re-roll | `jmp @@Loop_Swamp` → body top (asm 328→311), re-roll same slot | `do {…} while(base_wy 35..45)` — same body-top retry (band dead — **B1**) ✓ |
| Swamp walk | 5 dirs; `2+Random(3)` steps; `Random(4)-1`; wrap; `tt_Forest1`→`tt_Swamp1` | same ✓ |
| `dir_chg_tbl` | `dir_chg_tbl_wx__2`/`wy__2` (word arrays) | `dir_chg_tbl_wx/wy[5]` = `{0,-1,0,1,0}`/`{1,0,-1,0,0}` (S/W/N/E/center) ✓ |
| `new_direction` | `mov [bp+new_direction], e_ST_UNDEFINED` (unused) | set, unused ✓ |

Constants are name aliases (`tt_Ocean`/`tt_Ocean1` = 0; `tt_Grasslands1`, `tt_Forest1`, `tt_Tundra1`, `tt_Desert1`, `tt_Swamp1`). No reconstruction errors.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — `Random(8)` (tundra), `Random(54)/(24)/(6)/(4)` (desert), `Random(56)/(20)/(3)/(4)` (swamp).
- **`dir_chg_tbl_wx` / `dir_chg_tbl_wy`** ([MAPGEN.c:117-118](../../MoM/src/MAPGEN.c#L117-L118)) — 5-entry direction offsets (S, W, N, E, center).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Climate_Terrain_Types.asm` — IDA Pro 5.5 disassembly (authority); tundra `Random(8)` (asm 59/111), desert seeds `Random(54)/(24)` (151/157), swamp band `jmp @@Loop_Swamp` (328).
- [MAPGEN.c:500-504](../../MoM/src/MAPGEN.c#L500-L504) — call sites.
- [MAPGEN.c:117-118](../../MoM/src/MAPGEN.c#L117-L118) — `dir_chg_tbl_wx/wy`.
- [MAPGEN-Simtex_Autotiling.md](MAPGEN-Simtex_Autotiling.md) — the next pass; edges the `tt_*1` base tiles this function places.
