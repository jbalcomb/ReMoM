MAPGEN-Generate_Roads.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Roads.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Roads.c

Init_New_Game()
    |-> Generate_Roads(ARCANUS_PLANE);
    |-> Generate_Roads(MYRROR_PLANE);

---

# `Generate_Roads` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Generate_Roads` | [MAPGEN.c:6017-6172](../../MoM/src/MAPGEN.c#L6017-L6172) | Builds the starting road network: clears map-square flags for the plane, then for each near, same-landmass neutral-city pair lays a road along `Path_Wrap` (rejecting ocean/shore paths), and finally flags every city square as road. |

## Purpose

Connects nearby neutral cities with roads (Enchanted Roads on Myrror). For one
plane it clears `_map_square_flags`, walks every unordered city pair, and — when
both are neutral, on the target plane, on the same landmass, and within range —
paths between them and, if the path avoids water, sets `MSF_ROAD` on each square
(`MSF_EROAD` too on Myrror). A final pass marks each city's own square as road.

**RNG-neutral: zero `Random()` calls** ([6017-6172](../../MoM/src/MAPGEN.c#L6017-L6172)); `Path_Wrap` and `Range` are deterministic. Does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:578-579](../../MoM/src/MAPGEN.c#L578-L579) | Called once per plane; followed by `gd_dump_map_square_flags("19_Generate_Roads_F")`. |

## How it works

1. **Clear flags** ([6036-6042](../../MoM/src/MAPGEN.c#L6036-L6042)) — `wy/wx` over the full map, `SET_MAP_SQUARE_FLAG(...,0)`.
2. **City-pair loops** ([6043-6122](../../MoM/src/MAPGEN.c#L6043-L6122)) — `src_city_idx` × `dst_city_idx > src`. A pair is skipped (B1) unless: both on plane `wp`, both `NEUTRAL_PLAYER_IDX`, same landmass, and `Range < 11`.
   - `path_length = Path_Wrap(src, dst, &path_wx_array, &path_wy_array, WORLD_WIDTH)` ([6067](../../MoM/src/MAPGEN.c#L6067)).
   - **Validity scan** ([6072-6106](../../MoM/src/MAPGEN.c#L6072-L6106)) — walk the path (indices `0 .. path_length-2`); mark `reject` if any square is shore (`tt_Shore1_Fst..tt_Shore1_Lst`), ocean (`tt_Ocean`), or in the two extra ranges (B2).
   - **Lay road** ([6107-6120](../../MoM/src/MAPGEN.c#L6107-L6120)) — if valid, `|= MSF_ROAD` per square; `|= MSF_EROAD` on Myrror.
3. **Per-city road flag** ([6125-6168](../../MoM/src/MAPGEN.c#L6125-L6168)) — for **every** city (B3), `|= MSF_ROAD` at the city's own square; on Myrror, `|= MSF_EROAD` at the **stale** `(wx,wy)` (B4).

## Verification against the asm

Production is 1:1 with the disassembly. Name differences are aliases with
identical values (verified against `MAGIC.inc`):

| Item | OG asm | Production |
| --- | --- | --- |
| Clear loop | `wy/wx < WORLD_HEIGHT/WIDTH` (`jl`), zero flags | same ✓ |
| Pair loops | `src 0..n`, `dst src+1..n` | same ✓ |
| Skip gate | both `wp`, both `NEUTRAL`, same `_landmasses`, `Range < 11` | same ✓ |
| `Path_Wrap` args | `src_wx,src_wy,dst_wx,dst_wy,&path_wx_array,&path_wy_array,WORLD_WIDTH` (push order) | same ✓ |
| Validity scan | `Shore1_Fst(2)..Shore1_Lst(161)`, `Ocean1(0)`, `_Shore00001R10(0xD9)..Shore2F_end(232)`, `Shore2_1st(452)..Ocean2(601)` | `tt_Ocean`=0, `tt_OceanAnim`=601 (= asm `tt_Ocean1`/`tt_Ocean2`); shore bounds match ✓ |
| Scan loop bound | `(path_length-1) > path_idx && reject==FALSE` | same ✓ |
| Lay loop | `|= TF_Road`; `wp==1 → |= TF_Enc_Road` | `MSF_ROAD`(0x08)/`MSF_EROAD`(0x10) = `TF_Road`/`TF_Enc_Road` ✓ |
| Final per-city loop | every city `|= TF_Road` at city coords; `wp==1 → |= TF_Enc_Road` at stale `[bp+wy]/[bp+wx]` | same ✓ (B3/B4) |

`path_wx_array`/`path_wy_array` are `int8_t[70]` (asm frame `Road_Xs`/`Road_Ys`: `Road_Ys` at `-0A4h`, `Road_Xs` at `-5Eh`, 0x46 = 70 bytes apart) — match.

## OG bugs preserved (do **not** "fix" in baseline ReMoM)

- **B1 — `Range < 11` instead of `Delta_XY_With_Wrap`** ([6062](../../MoM/src/MAPGEN.c#L6062)). The proximity test uses `Range` (no world-wrap); faithful.
- **B2 — validity-scan range should start at `$C5`** ([6088](../../MoM/src/MAPGEN.c#L6088)). The third exclusion batch begins at `_Shore00001R10` rather than `$C5`; harmless (none of those tiles exist at this stage), faithful.
- **B3 — final loop ignores `wp` and flags every city** ([6123](../../MoM/src/MAPGEN.c#L6123)). The per-city road pass has no plane filter, so both planes' cities get `MSF_ROAD` on each call. Faithful.
- **B4 — Myrror Enchanted-Road write uses stale `(wx,wy)`, not city coords** ([6147-6166](../../MoM/src/MAPGEN.c#L6147-L6166)). The `wp==MYRROR` branch `|= MSF_EROAD` at the leftover `(wx,wy)` (and param `wp`), not the city's square — so town roads on Myrror never get the enchant, and the write lands on whatever square `(wx,wy)` last held. Faithful.

### B4 OOB, backed by the static pool

When no city pair qualifies, the road loops never run, so `(wx,wy)` retain
`(60,40)` from the clear loop's exit. The B4 write then indexes
`_map_square_flags[wp=1][40][60]` = offset `1*2400 + 40*60 + 60 = `**`4860`** — past
the logical `4800`. The OG just touches adjacent memory (DOS); a modern flat
allocation would fault. ReMoM makes it safe via the **static pool** backing
`_map_square_flags`: the write lands in addressable, `0xCC`-sentinel pool memory
rather than faulting. *(Historical: pre-Phase-5a ReMoM sized the buffer with
`+ WORLD_WIDTH + 1` → 4880 bytes to cover index 4860 in-arena; **Phase 5a retired
that padding**.)* Seed 12345 dodges this (its loops leave `(43,9)`); other seeds hit the `(60,40)` case.

## Sub-functions / external calls

- **`Path_Wrap`** — deterministic path between two squares into `path_wx_array`/`path_wy_array`, returns length.
- **`Range`** — Chebyshev-style distance (no world-wrap — see B1).
- `GET_LANDMASS` / `GET_TERRAIN_TYPE` / `GET_MAP_SQUARE_FLAG` / `SET_MAP_SQUARE_FLAG`.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Roads.asm` — IDA Pro 5.5 disassembly (authority); `Path_Wrap` call (`loc_4BBC8`), validity scan (`loc_4BBF9`), stale-coord EROAD write (`loc_4BF0F` region).
- [MAPGEN.c:578-579](../../MoM/src/MAPGEN.c#L578-L579) — call sites.
- [ALLOC.c:157](../../MoM/src/ALLOC.c#L157) / [STU_WRLD.c:1711](../../STU/src/STU_WRLD.c#L1711) — the `_map_square_flags` allocation (B4 `+WORLD_WIDTH+1` padding retired in Phase 5a; pool-backed now).
- [MAPGEN-Generate_Terrain_Specials.md](MAPGEN-Generate_Terrain_Specials.md) — sibling pass with the same pool-backed OOB pattern.
