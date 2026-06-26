MAPGEN-Desert_Autotile.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Desert_Autotile.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Desert_Autotile.c

Init_New_Game()
    |-> Desert_Autotile();

---

# `Desert_Autotile` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Desert_Autotile` | [MAPGEN.c:3716-3853](../../MoM/src/MAPGEN.c#L3716-L3853) | Gives dried-lake desert squares a real landmass, then 8-neighbor bitmask-autotiles every desert square to its correct desert-edge subtype from the `TERRTYPE.LBX` table. |

## Purpose

Two passes over both planes. Pass 1 repairs the landmass of `_1Desert` squares
(dry lakes — see below), copying a real landmass from a neighbor. Pass 2 reshapes
every desert square (`tt_Desert1..4`, `_1Desert`) to the desert-edge variant
chosen by an 8-neighbor "is-not-desert" bitmask indexed into the loaded `terrtype`
table — the same bitmask-autotiling concept as `Simtex_Autotiling`, desert-only.

**RNG-neutral: zero `Random()` calls** ([3716-3853](../../MoM/src/MAPGEN.c#L3716-L3853)). Does not affect new-game `Random()` alignment.

## Why pass 1 exists (dry-lake landmass repair)

`_1Desert` squares here are created by `River_Autotile`: a `_1Lake` with no river
inflow becomes `_1Desert` ([MAPGEN.c:5049](../../MoM/src/MAPGEN.c#L5049)). A lake is
*water* — `_1Lake` (0x12 = 18) `< tt_Grasslands1` (0xA2 = 162), so
`Square_Is_Ocean_NewGame` treats it as ocean, and water squares carry
**landmass 0** (NO_LANDMASS; `0` terrain ↔ `0` landmass, [MAPGEN.c:2239](../../MoM/src/MAPGEN.c#L2239)).
When the dry lake flips to desert (land) the terrain changes but `_landmasses`
stays `0`, leaving a land square with no continent. Pass 1 adopts it into a
neighbor's landmass. (`tt_Desert1` deserts from `Generate_Climate_Terrain_Types`
were already on real land, so they aren't touched by pass 1.)

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:600](../../MoM/src/MAPGEN.c#L600) | Followed by `gd_dump_landmasses("23_…")` and `gd_dump_world_map("24_…")` ([601-602](../../MoM/src/MAPGEN.c#L601-L602)). |

## How it works

1. **Load the terrtype table** ([3729-3730](../../MoM/src/MAPGEN.c#L3729-L3730)) — `terrtype = Near_Allocate_First(5*512)` (2560 B), `LBX_Load_Data_Static(terrtype_lbx, 0, terrtype, 0, 5, 512)`.
2. **Pass 1 — dry-lake landmass repair** ([3733-3756](../../MoM/src/MAPGEN.c#L3733-L3756)) — for each `_1Desert` square ([3740](../../MoM/src/MAPGEN.c#L3740)), scan the full 3×3 (`wy_offset`/`wx_offset ∈ {-1,0,1}`, [3742-3744](../../MoM/src/MAPGEN.c#L3742-L3744)); `neighbor_idx` is the neighbor's flat index, and for any neighbor with a non-zero landmass the square copies it ([3746-3749](../../MoM/src/MAPGEN.c#L3746-L3749)) — last qualifying neighbor wins.
3. **Pass 2 — bitmask autotile** ([3759-3849](../../MoM/src/MAPGEN.c#L3759-L3849)) — for each desert square (`tt_Desert1..4`, `_1Desert`): build an 8-bit `mask` from the eight neighbors, each bit set when the neighbor is **not** desert (`Square_Is_Desert_NewGame == ST_FALSE`) **and** in bounds:

   | bit | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
   |---|---|---|---|---|---|---|---|---|
   | dir | NW | N | NE | E | SE | S | SW | W |

   - `mask > 0` → `p_world_map = tt_Desert_Fst + (terrtype[mask] - 2)` ([3839](../../MoM/src/MAPGEN.c#L3839)).
   - `mask == 0` (fully desert-surrounded) → `p_world_map = tt_Desert1` (= asm `_AllDesert1`, both `0xA5`) ([3843](../../MoM/src/MAPGEN.c#L3843)).

   The `terrtype[]` values are **2-based**: the minimum (2, at mask 8) maps to the
   first subtype `_Desert00001000` (`tt_Desert_Fst` = 0x124 = 292), running up to
   161 at `tt_Desert_Lst` (0x1C3 = 451). The asm encodes this as the literal
   `add ax, 290` (= `tt_Desert_Fst - 2`); production's `tt_Desert_Fst + (terrtype[mask] - 2)`
   is value-identical (`292 + t - 2 = 290 + t`).

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| Table load | `Near_Allocate_First(2560)`; `LBX_Load_Data_Static(…,0,terrtype,0,5,512)` | `Near_Allocate_First(5*512)`; same args ✓ |
| Pass-1 guard | `cmp [p_world_map], _1Desert / jz` | `== _1Desert` ✓ |
| Pass-1 neighbor loop | `wy_offset/wx_offset = -1 .. 1` (`cmp ,2 / jl`) | `< 2` ✓ |
| Pass-1 copy | `if _landmasses[nbr_off] != 0 → _landmasses[self_off] = _landmasses[nbr_off]` (single indirection) | `neighbor_idx = nbr_off; if(_landmasses[neighbor_idx] != 0) _landmasses[self] = _landmasses[neighbor_idx]` ✓ |
| Pass-2 guard | `tt_Desert1/2/3/4` or `_1Desert` | same 5 types ✓ |
| 8-neighbor mask | NW=1,N=2,NE=4,E=8,SE=16,S=32,SW=64,W=128; bit set iff `!Square_Is_Desert` AND in-bounds | same bits, order, bounds ✓ |
| `mask > 0` write | `terrtype[mask]` (word) `+ 290` | `tt_Desert_Fst + (terrtype[mask] - 2)` = `290 + terrtype[mask]` ✓ |
| `mask == 0` write | `_AllDesert1` (`0xA5`) | `tt_Desert1` (`0xA5`, = `_AllDesert1`) ✓ |
| Loop bounds | `wp<2`, `wy<WORLD_HEIGHT`, `wx<WORLD_WIDTH` | same ✓ |

## Sub-functions / external calls

- **`Near_Allocate_First`** / **`LBX_Load_Data_Static`** — load the 5×512 `TERRTYPE.LBX` mask→subtype table.
- **`Square_Is_Desert_NewGame`** — neighbor classifier driving the bitmask.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Desert_Autotile.asm` — IDA Pro 5.5 disassembly (authority); pass-1 neighbor loop (`loc_473A8`/`loc_473B1`, `cmp ,2`), 8-neighbor mask (`loc_474A1`…), `add ax, 290` write, `_AllDesert1` else-branch (`loc_475F9`).
- [MAPGEN.c:5049](../../MoM/src/MAPGEN.c#L5049) — `River_Autotile` creates the `_1Desert` (dry-lake) squares pass 1 repairs.
- [MAPGEN.c:600-602](../../MoM/src/MAPGEN.c#L600-L602) — call site + dumps.
- [TerrType.h](../../MoM/src/TerrType.h) — `tt_Desert1`=`_AllDesert1`=`0xA5`, `_1Desert`=`0x134`, `tt_Desert_Fst/Lst`=`0x124`/`0x1C3`, `_Desert00001000`=`tt_Desert_Fst`.
