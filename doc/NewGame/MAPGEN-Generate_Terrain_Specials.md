MAPGEN-Generate_Terrain_Specials.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Terrain_Specials.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Terrain_Specials.c

Init_New_Game()
    |-> Generate_Terrain_Specials(ARCANUS_PLANE);
    |-> Generate_Terrain_Specials(MYRROR_PLANE);

---

# `Generate_Terrain_Specials` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Generate_Terrain_Specials` | [MAPGEN.c:6234-6380](../../MoM/src/MAPGEN.c#L6234-L6380) | For one plane: clears the terrain-special and map-flag maps, then scatters terrain features (minerals/wild game/nightshade) on a coarse `radius`-spaced grid, sometimes converting grass/forest squares to mountain/hills/swamp first. |

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

Seeds each plane's per-square *terrain special* (bonus resource / encounter
modifier). It walks a coarse grid (`radius` 4 on Arcanus, 3 on Myrror), jitters a
target square within each cell, and — if the square is open grass/forest and not
already occupied — optionally reshapes it to mountain/hills/swamp, then stamps the
appropriate special (Wild Game on forest, a mineral on mountain/hills/desert,
Nightshade on swamp).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:422-423](../../MoM/src/MAPGEN.c#L422-L423) | Called once per plane; followed by `STU_GD_Dump_Terrain_Specials("118_…")` and `STU_GD_Dump_World_Map("119_…")` ([424-425](../../MoM/src/MAPGEN.c#L424-L425)). |

## RNG footprint (alignment-relevant)

Per coarse cell `(itr_wy, itr_wx)`:

- **Two `Random(radius*2)`** — unconditional, computing the jittered `wy`/`wx`
  ([6283-6284](../../MoM/src/MAPGEN.c#L6283-L6284)). These fire for every cell,
  before any skip path (asm `loc_4C00B`, `wy` draw then `wx` draw — same order).
- **`Random(2)`** — only when the square is *not* grassland but *is* forest ([6354](../../MoM/src/MAPGEN.c#L6354)).
- **`Random(7)`** — only when the square is grassland, or forest with the `Random(2)` hit ([6358](../../MoM/src/MAPGEN.c#L6358)).

So 2–4 draws per cell. The OOB caveat (B2) is the one place this can differ from OG.

## How it works

1. **Clear** ([6248-6265](../../MoM/src/MAPGEN.c#L6248-L6265)) — `wy/wx` over the full map (`< WORLD_HEIGHT` / `< WORLD_WIDTH`): zero `_map_square_terrain_specials` and `_map_square_flags` (indices 0..4799 only — the clear never touches the overflow padding).
2. **`radius`** ([6267-6275](../../MoM/src/MAPGEN.c#L6267-L6275)) — `4` on Arcanus, `3` on Myrror.
3. **Scatter grid** ([6277-6376](../../MoM/src/MAPGEN.c#L6277-L6376)) — `itr_wy`/`itr_wx` step by `radius`. Per cell:
   - jitter `wy = itr_wy + Random(radius*2)`, `wx = itr_wx + Random(radius*2)` (can overshoot the map — see B2);
   - scan lairs/towers/cities for `square_has_site` / `square_has_city`;
   - **skip** the square if it already has a special, has a city/site, or (B1) the dead corner test ([6327-6346](../../MoM/src/MAPGEN.c#L6327-L6346));
   - if grassland, or `forest && Random(2)==1` → `switch(Random(7)-1)`: `0..2`→Mountain, `3..5`→Hills, `6`→Swamp ([6348-6368](../../MoM/src/MAPGEN.c#L6348-L6368));
   - five sequential checks set the special: Forest→`TS_WILDGAME`, Mountain→`Mountain_Terrain_Special(wp)`, Hills→`Hills_Terrain_Special(wp)`, Swamp→`TS_NIGHTSHADE`, Desert→`Desert_Terrain_Special(wp)` ([6370-6374](../../MoM/src/MAPGEN.c#L6370-L6374)). These read the *post-switch* terrain, so a just-converted square gets its new special.

## OG bugs preserved (do **not** "fix" in baseline ReMoM)

### B1 — contradictory `&&` corner exclusion never fires ([MAPGEN.c:6334-6342](../../MoM/src/MAPGEN.c#L6334-L6342))

The asm skips a square only if `wy<=2 && wy>=38 && wx<=2 && wx>=58`:

```asm
loc_4C167:
    cmp wy, 2  / jg proceed      ; NOT taken => wy <= 2
    cmp wy, 38 / jl proceed      ; NOT taken => wy >= 38
    cmp wx, 2  / jg proceed      ; NOT taken => wx <= 2
    cmp wx, 58 / jl proceed      ; NOT taken => wx >= 58
    jmp skip
```

`wy<=2 && wy>=38` is impossible, so the corner test **never excludes anything**.
The in-code `/* OGBUG should all be ||, not && */` is correct; production
reproduces the dead `&&`, with bounds `WORLD_HEIGHT-2`/`WORLD_WIDTH-2` = 38/58
matching the asm. **Faithful; preserve.**

### B2 — out-of-bounds scatter, backed by the static pool ([MAPGEN.c:6283-6285](../../MoM/src/MAPGEN.c#L6283-L6285))

The jitter `itr + Random(radius*2)` can push the target past the map edge. Exact
maxima (`itr` stops at the last multiple of `radius` below the dimension, plus the
max roll `radius*2`):

| | Arcanus (radius 4) | Myrror (radius 3) | valid |
|---|---|---|---|
| `wy` | 36 + 8 = **44** | 39 + 6 = **45** | 0..39 |
| `wx` | 56 + 8 = **64** | 57 + 6 = **63** | 0..59 |

Worst byte offset is Myrror `wy=45, wx=63` → `2400 + 45*60 + 63 = 5163`. The OG has
**no guard** here — it indexes `_map_square_terrain_specials` and `_world_maps`
(via `p_world_map` and the `Square_Is_*` reads) out of bounds; benign in DOS real
mode (adjacent memory), an access violation in a modern flat allocation.

ReMoM reproduces the OG faithfully (no guard). The OOB read/write is made safe by
the **static pool** that backs these arenas: it lands in addressable,
`0xCC`-sentinel pool memory (a neighbouring arena or the trailing margin) rather
than faulting. `_map_square_flags` is only touched by the clear loop here
(≤4799), so it is not overrun by *this* function.

*Historical (pre-Phase-5a):* ReMoM instead sized these buffers to absorb the
overrun in-arena via `WORLD_OVERFLOW = 7 * WORLD_WIDTH = 420` padding — the
`_map_square_terrain_specials` arena (5248 B) and `_world_maps` (10464 B) both
covered the worst offset 5163. **Phase 5a retired that padding** once the pool
made the OOB safe on its own.

**RNG caveat:** the pool memory the overrun reads is not grass/forest, unlike the
OG's adjacent-heap garbage. So OOB cells classify as non-grass/non-forest and no-op —
the two cell-pick draws still fire (aligned), but where the OG's garbage *would*
have read as grass/forest and drawn `Random(2)`/`Random(7)`, production does not.
This residual desync is inherent (garbage reads can't be reproduced); it is not a
bug to fix.

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| Clear loops | `wy/wx < WORLD_HEIGHT/WIDTH` (`jl`), zero specials + flags | same ✓ |
| `radius` | `wp==0 → 4`, else `3` | `ARCANUS → 4`, else `3` ✓ |
| Scatter loops | `itr_wy/itr_wx += radius`, `< WORLD_HEIGHT/WIDTH` (`jge` exit) | same ✓ |
| Cell jitter | `wy = itr_wy + Random(radius*2)`, then `wx` (same order) | same ✓ (overshoot — see **B2**) |
| OOB handling | no guard; indexes OOB | no guard; OOB backed by the static pool — **B2** |
| Site scans | lair (`NUM_LAIRS`), tower (`NUM_TOWERS`), city (`_cities`) match on `wp/wx/wy` | same ✓ |
| Skip gate | special≠0, has-city, has-site, dead corner `&&` (2/38/2/58) | same ✓ (corner = **B1**) |
| Terrain gate | `Grassland \|\| (Forest && Random(2)==1)` (grassland short-circuits the `Random(2)`) | same ✓ |
| `Random(7)` switch | `off_4C371`: 0/1/2→Mountain, 3/4/5→Hills, 6→Swamp | same ✓ |
| Special assignment | Forest→WildGame, Mountain/Hills/Desert→`*_Terrain_Special(wp)`, Swamp→Nightshade (sequential, post-switch terrain) | same ✓ |

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — jitter `Random(radius*2)` ×2, plus `Random(2)`/`Random(7)`.
- **`Square_Is_{Grassland,Forest,Mountain,Hills,Swamp,Desert}_NewGame`** — terrain classifiers.
- **`Mountain_Terrain_Special` / `Hills_Terrain_Special` / `Desert_Terrain_Special`** — per-plane mineral pickers.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Terrain_Specials.asm` — IDA Pro 5.5 disassembly (authority); `off_4C371` terrain jump table; corner test `loc_4C167` (dead `&&`); no OOB guard.
- [MAPGEN.c:573-576](../../MoM/src/MAPGEN.c#L573-L576) — call sites + dumps.
- [ALLOC.c:98-157](../../MoM/src/ALLOC.c#L98-L157) — the arena allocations (padding retired in Phase 5a; `WORLD_OVERFLOW` at [MOM_DEF.h:287](../../MoX/src/MOM_DEF.h#L287) is now used only by tests). See [PLAN-Static-Pool-Allocator](../%23AI_Plans/PLAN-Static-Pool-Allocator.md).
