MAPGEN-Shuffle_Terrains.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Shuffle_Terrains.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Shuffle_Terrains.c

Init_New_Game()
    |-> Shuffle_Terrains();

---

# `Shuffle_Terrains` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Shuffle_Terrains` | [MAPGEN.c:4622-4755](../../MoM/src/MAPGEN.c#L4622-L4755) | Cosmetic terrain-variant shuffler: walks every tile and, for the five "shufflable" base types (grassland, forest, desert, swamp, tundra), rolls `Random()` and replaces the tile with one of its interchangeable visual variants. |

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

Pure visual variety: the base terrain types each have several interchangeable tile graphics. For each tile of a shufflable base type, `Shuffle_Terrains` rolls a die and swaps in a randomly-chosen variant. It is the last terrain pass in `Init_New_Game` (after the autotilers) and changes only the *graphic*, never the terrain's gameplay identity.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:606](../../MoM/src/MAPGEN.c#L606) | After `Desert_Autotile` ([600](../../MoM/src/MAPGEN.c#L600)); followed by `gd_dump_world_map("25_Shuffle_Terrains_W")` ([607](../../MoM/src/MAPGEN.c#L607)). Single call (both planes handled by its own `wp` loop). |

## RNG footprint (the alignment-relevant part)

`Shuffle_Terrains` is **RNG-heavy**: it makes **one `Random()` call per tile** whose base type is grassland / forest / desert / swamp / tundra, across both planes. Its total draw count = the number of such tiles on the finished map.

| Base type | Trigger | Roll | Variants (cases) |
|---|---|---|---|
| Grassland | `== tte_Grasslands` ([4642](../../MoM/src/MAPGEN.c#L4642)) | `Random(4)` | 1→Grasslands1, 2→Grasslands4, 3→Grasslands2, 4→Grasslands3 |
| Forest | `== tte_Forest1` ([4665](../../MoM/src/MAPGEN.c#L4665)) | `Random(3)` | 1→Forest1, 2→Forest2, 3→Forest3 |
| Desert | `== tt_Desert1` ([4684](../../MoM/src/MAPGEN.c#L4684)) | `Random(4)` | 1→Desert1, 2→Desert2, 3→Desert3, 4→Desert4 |
| Swamp | `== tt_Swamp1` ([4707](../../MoM/src/MAPGEN.c#L4707)) | `Random(3)` | 1→Swamp1, 2→Swamp2, 3→Swamp3 |
| Tundra | `== tt_Tundra1` ([4726](../../MoM/src/MAPGEN.c#L4726)) | `Random(3)` | 1→Tundra1, 2→(none), 3→Tundra2, 4→Tundra3 (unreachable) — **OGBUG, see below** |

All five rolls match the asm: `mov ax,4/3/4/3/3 ; call Random` for grassland/forest/desert/swamp/tundra (`Shuffle_Terrains.asm` 61/156/235/330/409), each preceded by `cmp [es:bx], tte_Grasslands1 / tte_Forest1 / _AllDesert1 / tte_Swamp1 / tte_Tundra1`.

> **The `Random(n)` argument does not affect the RNG call sequence.** Each `Random()` consumes exactly one PRNG step regardless of `n` (it draws once, then `% n`). So what governs new-game alignment here is the *count* of shufflable tiles matching OG — determined by the terrain the earlier passes produced — not the per-roll modulus.

The five blocks are **independent `if`s** (not `else if`), matching the asm — but a shuffle only ever swaps a base type for one of its own variants, so a tile never re-triggers a later block.

## Preserved OG bug

### B1 — tundra: unreachable variant + dead roll ([MAPGEN.c:4726-4748](../../MoM/src/MAPGEN.c#L4726-L4748))

Tundra rolls `Random(3)` (outcomes 1/2/3) but the `switch` is written `1, 2(empty), 3, 4`: roll 1→Tundra1, roll 2→no-op (stays Tundra1), roll 3→Tundra2; **case 4 (Tundra3) is unreachable**. The asm has the same shape — a `cmp ax,1 / cmp ax,3 / cmp ax,4` chain (`Shuffle_Terrains.asm:415-419`) with `else`→no-op, mapping 1→Tundra1, 3→`_AllTundra2`, 4→`_AllTundra3`. So the OG author meant cases 1/2/3 but wrote 1/3/4, leaving Tundra3 never selected and roll 2 wasted. **Faithful; preserved** — production's `/* OGBUG */` comment ([4726](../../MoM/src/MAPGEN.c#L4726)) documents it. Do not "fix."

## OG-faithful quirks (not bugs)

- **Grassland variant order** ([4646-4662](../../MoM/src/MAPGEN.c#L4646-L4662)) — `1→Grasslands1, 2→Grasslands4, 3→Grasslands2, 4→Grasslands3` (note the 4/2/3 ordering). Matches the asm jump table `off_48C5B` bodies exactly.
- **Unused `terrtype` load** ([4633](../../MoM/src/MAPGEN.c#L4633)) — allocates 5×512 and `LBX_Load_Data_Static`s the `terrtype` table, then never reads it. The asm does the same (`terrtype` referenced only at the load, asm:18-28) — a copy-paste from the autotiler pattern. Faithful dead load.
- **`tt_Desert1` == `_AllDesert1`** (both `0xA5`, [TerrType.h:15](../../MoM/src/TerrType.h#L15)/[249](../../MoM/src/TerrType.h#L249)) — production and asm just use different names for the same constant.

## Verification against the asm

- Five `Random()` call sites + their preceding terrain `cmp`s — verified 1:1. Args: grassland 4, forest 3, desert 4, swamp 3, tundra 3 — all match.
- Grassland jump-table case order and tundra cmp-chain structure — verified 1:1.
- `terrtype` dead load — present in the asm, never used.
- Tile writes use the word-strided `p_world_map[wp][wy][wx]`. Function makes **no** RNG calls beyond the five shuffles.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — `1..n`; one call per shufflable tile.
- **`Near_Allocate_First` / `LBX_Load_Data_Static`** — load the (unused) `terrtype` table.
- **`p_world_map`** ([MOM_DAT.h:4095](../../MoX/src/MOM_DAT.h#L4095)) — read/written; terrain constants `tte_Grasslands{,2,3,4}`, `tte_Forest1`/`tt_Forest{2,3}`, `tt_Desert{1..4}`, `tt_Swamp{1,2,3}`, `tt_Tundra{1,2,3}`.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Shuffle_Terrains.asm` — IDA Pro 5.5 disassembly (authority); `call Random` at asm 63/158/237/332/411; grassland jump table `off_48C5B`.
- [MAPGEN.c:606-607](../../MoM/src/MAPGEN.c#L606-L607) — call site + world-map dump.
- [MAPGEN-River_Autotile.md](MAPGEN-River_Autotile.md) / [MAPGEN-River_Path.md](MAPGEN-River_Path.md) — neighboring terrain passes; `Simtex_Autotiling` ([MAPGEN.c:3925](../../MoM/src/MAPGEN.c#L3925)) for the same `terrtype` LBX table (where it *is* used).
