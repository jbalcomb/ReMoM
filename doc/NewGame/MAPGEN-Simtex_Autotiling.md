MAPGEN-Simtex_Autotiling.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Simtex_Autotiling.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Simtex_Autotiling.c

Init_New_Game()
    |-> Simtex_Autotiling();

---

# `Simtex_Autotiling` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Simtex_Autotiling` | [MAPGEN.c:3904-4568](../../MoM/src/MAPGEN.c#L3904-L4568) | Lays the base coastline/edge tiling: four independent 8-neighbor bitmask-autotile passes (Ocean shores, Mountain ranges, Tundra edges, Hills ranges), each mapping a neighbor mask through `TERRTYPE.LBX` to an edge-variant tile. |

## Purpose

After climate terrain is placed, this turns the blocky base terrain into properly
edged tiles. Each pass scans both planes; for every square of the pass's base type
it builds an 8-bit mask of which neighbors "match" (per a pass-specific predicate),
looks the mask up in the loaded `terrtype` table, and writes the resulting
edge-variant tile.

**RNG-neutral: zero `Random()` calls** ([3904-4568](../../MoM/src/MAPGEN.c#L3904-L4568)). Does not affect new-game `Random()` alignment. (The following pass, `Shuffle_Terrains`, is the RNG-consuming one.)

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:584](../../MoM/src/MAPGEN.c#L584) | Followed by `gd_dump_world_map("20_Simtex_Autotiling_W")` ([585](../../MoM/src/MAPGEN.c#L585)). |

## The shared shape

The table is loaded once (`Near_Allocate_First(5*512)` + `LBX_Load_Data_Static(…,5,512)`, [3921-3922](../../MoM/src/MAPGEN.c#L3921-L3922)), then four passes run. Each pass, per matching square, accumulates an 8-bit `mask`:

| bit | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 |
|---|---|---|---|---|---|---|---|---|
| dir | NW | N | NE | E | SE | S | SW | W |

(asm: `inc [mask]`, then `add [mask], 2/4/8/16/32/64/128` — verified identical in all four passes.) The passes differ only in base type, neighbor predicate, X-wrap, and the `terrtype` write:

| pass | base type | neighbor matches when… | X-wrap | write | guard / write |
|---|---|---|---|---|---|
| **Ocean** | `tte_Ocean` | land: `>= tte_Grasslands && <= tte_Hills` | yes (`adj_wx`) | `terrtype[mask]` | [3933](../../MoM/src/MAPGEN.c#L3933) / [4034](../../MoM/src/MAPGEN.c#L4034) |
| **Mountain** | `tte_Mountain` | `== tte_Mountain` or `_Mount0010.._Mount1001` | no | `terrtype[256 + mask]` | [4054](../../MoM/src/MAPGEN.c#L4054) / [4209](../../MoM/src/MAPGEN.c#L4209) |
| **Tundra** | `tte_Tundra` | `!= tte_Tundra && (< _Tundra00001000 \|\| > _TerType_Count)` | yes | `600 + terrtype[mask]` | [4227](../../MoM/src/MAPGEN.c#L4227) / [4385](../../MoM/src/MAPGEN.c#L4385) |
| **Hills** | `tte_Hills` | `== tte_Hills` or `_Hills_0010.._1Hills2` | no | `16 + terrtype[256 + mask]` | [4403](../../MoM/src/MAPGEN.c#L4403) / [4556](../../MoM/src/MAPGEN.c#L4556) |

`256+mask` is the asm's `[si+200h]` displacement (0x200 = 512 bytes = 256 words). Each pass writes only when `mask > 0`.

## OG bugs preserved (do **not** "fix" in baseline ReMoM)

- **B1 — south-edge bound is `wy < WORLD_HEIGHT` instead of `(wy+1) < WORLD_HEIGHT`** (all four passes — the S / SE / SW neighbor checks). At `wy == WORLD_HEIGHT-1` these read `p_world_map[wp][40][x]`, one row past the map. Faithful to the asm (which has the same always-true guard); the OOB read lands in the static pool that backs `_world_maps` — addressable, sentinel-initialized, deterministic — so it neither faults nor reads heap garbage (same mechanism as Animate_Oceans / Generate_Terrain_Specials).
- **B2 — Hills "no cardinal neighbor" → Grasslands4** ([4553-4556](../../MoM/src/MAPGEN.c#L4553-L4556)). Mountain and Hills share the `terrtype[256+mask]` table, which holds **Mountain** tile indices. Hills adds `+16` to remap a mountain *variant* (`_Mount0010..1001` = 0x103..0x112) to the aligned hills variant (`_Hills_0010..` = 0x113..) — the two blocks are exactly 16 apart (`_Hills_0010` = `_Mount0010` + 16). But when a tile has **no cardinal same-type neighbor**, the table returns the *base* `tte_Mountain` (0xA4) rather than a variant, and:

  ```
  16 + tte_Mountain  =  0x10 + 0xA4  =  0xB4  =  tt_Grasslands4
  ```

  So an isolated / diagonal-only hill is overwritten as `tt_Grasslands4` (grassland). The Mountain pass is immune — it writes `terrtype[256+mask]` with **no** `+16`, so the base stays `tte_Mountain` (a lone mountain remains a mountain). This is drake178's "turns certain single map square hills into grasslands." *(The `terrtype[256 + <no-cardinal mask>] == tte_Mountain` link is inferred from the arithmetic landing exactly on `tt_Grasslands4`; the literal table byte is in `TERRTYPE.LBX` record 1.)*

## OOB backing note (not in the OG)

The B1 south-edge OOB read is backed by the **static pool** (see [PLAN-Static-Pool-Allocator](../%23AI_Plans/PLAN-Static-Pool-Allocator.md)): the read lands in addressable, `0xCC`-sentinel pool memory, so it neither faults under a sanitizer nor reads process-heap garbage, and is deterministic run-to-run. Earlier ReMoM used a `WORLD_OVERFLOW` over-allocation padding plus a `gd_ci_inject_world_overrun` CI hook to stamp OG's exact bytes into that padding; **both were retired in Phase 5a** once the pool made the read safe on its own. (Neither the OOB read's backing nor the pool is in the Dasm — the pool is a ReMoM platform accommodation.)

## Verification against the asm

- **Four passes, in order** Ocean → Mountain → Tundra → Hills: guards `cmp tte_Ocean` (asm 57), `cmp tte_Mountain` (438), `cmp _Tundra00001000` (983), `cmp tte_Hills` (1456). ✓
- **Mask bits**: `+1,+2,+4,+8,+16,+32,+64,+128` ×4 (NW…W). ✓
- **Neighbor predicates**: Ocean `tte_Grasslands1..tte_Hills` (asm `tte_Grasslands1` = production `tte_Grasslands` = 0xA2); Mountain `tte_Mountain`/`_Mount0010`/`_Mount1001`; Tundra `_Tundra00001000`/`_TerType_Count`; Hills `tte_Hills`/`_Hills_0010`/`_1Hills2`. ✓
- **Writes**: Ocean `terrtype[mask]`; Mountain `[si+200h]` = `terrtype[256+mask]`; Tundra `add ax, 600`; Hills `[si+200h]` + `add ax, 16`. ✓
- **X-wrap**: Ocean + Tundra use `adj_wx` (`±WORLD_WIDTH`); Mountain + Hills use raw `wx±1` with bounds checks. ✓
- **RNG**: no `call Random`. ✓

No reconstruction errors; the only divergences are name aliases (`tte_Grasslands`/`tte_Grasslands1`).

## Sub-functions / external calls

- **`Near_Allocate_First`** / **`LBX_Load_Data_Static`** — load the 5×512 `TERRTYPE.LBX` mask→variant table.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Simtex_Autotiling.asm` — IDA Pro 5.5 disassembly (authority); pass guards (`tte_Ocean` 57 / `tte_Mountain` 438 / `_Tundra00001000` 983 / `tte_Hills` 1456), writes (`add ax,600` 1394 / `[si+200h]` 894 / `add ax,16` 1901).
- [MAPGEN.c:584-585](../../MoM/src/MAPGEN.c#L584-L585) — call site + dump.
- [MAPGEN-Desert_Autotile.md](MAPGEN-Desert_Autotile.md) / [MAPGEN-River_Autotile.md](MAPGEN-River_Autotile.md) — sibling bitmask autotilers; [MAPGEN-Generate_Terrain_Specials.md](MAPGEN-Generate_Terrain_Specials.md) — same pool-backed OOB pattern.
- [TerrType.h](../../MoM/src/TerrType.h) — `tte_Mountain`=0xA4, `tt_Grasslands4`=0xB4 (the B2 landing); `_Mount0010..1001`=0x103..0x112, `_Hills_0010..1Hills2`=0x113..0x123.
