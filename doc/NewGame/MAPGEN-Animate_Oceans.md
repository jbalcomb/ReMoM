MAPGEN-Animate_Oceans.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Animate_Oceans.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Animate_Oceans.c

Init_New_Game()
    |-> Animate_Oceans();

---

# `Animate_Oceans` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Animate_Oceans` | [MAPGEN.c:7043-7075](../../MoM/src/MAPGEN.c#L7043-L7075) | Sprinkles the animated-wave ocean variant: walks every tile (both planes) and, with a 1-in-5 chance per ocean square, swaps `tte_Ocean` for `tte_OceanAnim`. |

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

Purely cosmetic: most ocean tiles render as static `tte_Ocean`, but a scattered ~20% are switched to `tte_OceanAnim` (`0x259` = 601) so the sea shows moving waves. It's the last terrain pass in `Init_New_Game`.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:646](../../MoM/src/MAPGEN.c#L646) | Followed by `gd_dump_world_map("29_Animate_Oceans_W")` ([647](../../MoM/src/MAPGEN.c#L647)). Single call; both planes handled by its own `wp` loop. |

## How it works

Triple loop over both planes × (rows) × (cols). Each ocean tile (`== tte_Ocean`) gets a `Random(5)`; a roll of 1 (20%) sets it to `tte_OceanAnim`, else it's left alone. The `wp` loop covers both planes, so Myrror oceans are animated by this one block too.

The row/column bounds are **inclusive** (`<=`), matching the asm — see B2; this is load-bearing for RNG alignment, not an off-by-one to "clean up."

## RNG footprint (the alignment-relevant part)

**One `Random(5)` per ocean-tile *visit*** — [MAPGEN.c:7056-7062](../../MoM/src/MAPGEN.c#L7056-L7062):

```
if(p_world_map[wp][wy][wx] == tte_Ocean)
{
    if(Random(5) == 1) { p_world_map[wp][wy][wx] = tte_OceanAnim; }
}
```

The draw count is **not** simply the number of ocean squares: because the loop bounds are inclusive (B2), the phantom column `wx == WORLD_WIDTH` aliases the next row's column 0, so each column-0 tile of rows `1..WORLD_HEIGHT` is **visited twice in a row** and, if ocean, draws `Random(5)` twice. The asm does exactly the same, so production is alignment-correct **only because it now uses the same inclusive bounds** — the earlier `<` form silently dropped those extra draws and desynced the PRNG.

The **second** block — the Myrror case ([7064-7069](../../MoM/src/MAPGEN.c#L7064-L7069)) — is **dead** and consumes **no** `Random()` (see B1).

## Findings

### B1 — dead Myrror-ocean block ([MAPGEN.c:7064-7069](../../MoM/src/MAPGEN.c#L7064-L7069)) — OGBUG, faithful

```
if(p_world_map[wp][wy][wx] == (TerType_Count + tte_Ocean))
{
    if(Random(5) == 1) { p_world_map[wp][wy][wx] = (TerType_Count + tte_OceanAnim); }
}
```

This guards for `TerType_Count + tte_Ocean` (`TerType_Count` = `0x2FA` = 762, [TerrType.h:76](../../MoM/src/TerrType.h#L76)). The OG author coded it as if Myrror tiles were stored offset by `TerType_Count` — but they aren't: both planes use the same `0..761` terrain indices, distinguished by `wp`. No map square is ever `762`, so the condition is **always false** (the production comment at [7063](../../MoM/src/MAPGEN.c#L7063): *"will always jump"*). The asm has the identical dead block (`cmp [es:bx], TerType_Count+tte_Ocean` / `jnz`), so its `Random(5)` never fires.

It is also **redundant** — block 1 already matches `tte_Ocean` on both planes, so Myrror oceans get animated regardless. Net effect: none. **Faithful; preserve** — do not "fix" the condition.

### B2 — inclusive loop bounds (`<=`) overrun the map by one row and one column ([MAPGEN.c:7051](../../MoM/src/MAPGEN.c#L7051), [7053](../../MoM/src/MAPGEN.c#L7053)) — OGBUG, faithful

```
for(wy = 0; wy <= WORLD_HEIGHT; wy++)   /* OGBUG  OOB AVRL */
    for(wx = 0; wx <= WORLD_WIDTH; wx++)   /* OGBUG  OOB AVRL */
```

The asm exits each inner loop with `jg` against the **counts** `e_WORLD_HEIGHT = 40` / `e_WORLD_WIDTH = 60` (`MAGIC.inc`), so it iterates `wy = 0..40` (41 rows) and `wx = 0..60` (61 cols) — **one past** the valid `0..39` / `0..59`:

```asm
loc_4CEFB:  cmp [bp+wx], e_WORLD_WIDTH    ; 60
            jg  short loc_4CF04            ; exit only when wx > 60  => wx = 0..60
loc_4CF05:  cmp wy, e_WORLD_HEIGHT         ; 40
            jg  short loc_4CF0D            ; exit only when wy > 40  => wy = 0..40
```

The map is flat memory (`offset = wp*WORLD_SIZE + wy*WORLD_WIDTH + wx`, word-strided), so:

- **Phantom column** `wx = 60`: `offset(wy,60) = wy*60+60 = (wy+1)*60+0 = offset(wy+1,0)`. The loop ends a row at `wx=60` and immediately re-reads that same tile as `wx=0` of the next row → **column-0 tiles of rows 1..40 are processed twice, consecutively**. This is the source of the *sequential-pair* divergences when the older `<` form was in place.
- **Phantom row** `wy = 40`: reaches `offset = wp*2400 + 2400`, i.e. into the next plane's data (for `wp=0`) or past the logical array (for `wp=1`).

These out-of-bounds reads/writes are made safe by the **static pool** backing `_world_maps`: the max access — plane 1, `wy=40`, `wx=60` = tile `4860` — lands in addressable, `0xCC`-sentinel pool memory rather than faulting. *(Historical: pre-Phase-5a ReMoM over-allocated `_world_maps` with `WORLD_OVERFLOW` padding to keep the overrun in-arena; **Phase 5a retired that padding**.)* **Faithful; preserve** — the inclusive bounds are required for both RNG alignment and the doubled column-0 animation.

## Verification against the asm

- **Block 1** — `cmp tte_Ocean` → `Random(5)` → `tte_OceanAnim` (`0x259`): 1:1.
- **Block 2** — `cmp (TerType_Count + tte_Ocean)` → `Random(5)` → `(TerType_Count + tte_OceanAnim)`: 1:1 (dead in both; production uses the asm's named constants — `TerType_Count`, `tte_Ocean`, `tte_OceanAnim` — not magic literals).
- **Loop bounds** — `wp = 0..1` (`cmp wp,2 / jge`), `wy = 0..40` and `wx = 0..60` (both `jg` against counts 40/60, i.e. inclusive): match (see B2).
- Tile reads (`p_world_map`, word-strided) match the asm's `wp*e_WORLD_SIZE_DW + wy*e_WORLD_WIDTH_DW + wx*2` addressing.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — `1..n`; one `Random(5)` per ocean-tile visit.
- **`p_world_map`** ([MOM_DAT.h:4095](../../MoX/src/MOM_DAT.h#L4095)) — read/written; constants `tte_Ocean` (`0x0`), `tte_OceanAnim` (`0x259`), `TerType_Count` (`0x2FA`).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Animate_Oceans.asm` — IDA Pro 5.5 disassembly (authority); `jg`-based inclusive loop tails (`loc_4CEFB`, `loc_4CF05`, `loc_4CF0E`), `call Random` (×2; block-2's is dead).
- `MAGIC.inc` — `e_WORLD_HEIGHT = 40`, `e_WORLD_WIDTH = 60`, `e_WORLD_WIDTH_DW = 120`, `e_WORLD_SIZE_DW = 4800`.
- [MAPGEN.c:646-647](../../MoM/src/MAPGEN.c#L646-L647) — call site + world-map dump.
- [MAPGEN-Shuffle_Terrains.md](MAPGEN-Shuffle_Terrains.md) — sibling per-tile RNG terrain pass; `tte_OceanAnim` is treated as ocean by `Square_Is_Ocean`.
