MAPGEN-Animate_Oceans.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Animate_Oceans.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Animate_Oceans.c

Init_New_Game()
    |-> Animate_Oceans();

---

# `Animate_Oceans` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Animate_Oceans` | [MAPGEN.c:7122-7156](../../MoM/src/MAPGEN.c#L7122-L7156) | Sprinkles the animated-wave ocean variant: walks every tile (both planes) and, with a 1-in-5 chance per ocean square, swaps `tte_Ocean` for `tte_OceanAnim`. |

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

Purely cosmetic: most ocean tiles render as static `tte_Ocean`, but a scattered ~20% are switched to `tte_OceanAnim` (`0x259` = 601) so the sea shows moving waves. It's the last terrain pass in `Init_New_Game`.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:646](../../MoM/src/MAPGEN.c#L646) | Followed by `gd_dump_world_map("29_Animate_Oceans_W")` ([647](../../MoM/src/MAPGEN.c#L647)). Single call; both planes handled by its own `wp` loop. |

## RNG footprint (the alignment-relevant part)

**One `Random(5)` per ocean tile** — [MAPGEN.c:7137](../../MoM/src/MAPGEN.c#L7137):

```
if(p_world_map[wp][wy][wx] == tte_Ocean)
{
    if(Random(5) == 1) { p_world_map[wp][wy][wx] = tte_OceanAnim; }
}
```

Total draw count = the number of `tte_Ocean` squares on the finished map (both planes). On a roll of 1 the tile becomes animated; otherwise it stays. **1:1 with the asm** (`Animate_Oceans.asm`: `cmp [es:bx], tte_Ocean` → `mov ax,5 / call Random` → `mov [es:bx], tte_OceanAnim`).

The **second** block — the Myrror case ([7144-7151](../../MoM/src/MAPGEN.c#L7144-L7151)) — is **dead** and consumes **no** `Random()` (see B1). So the only RNG here is the one `Random(5)` per ocean tile, in both the OG and ReMoM — alignment-safe.

## How it works

Triple loop over both planes × 40 × 60. Each ocean tile (`== tte_Ocean`) gets a `Random(5)`; a roll of 1 (20%) sets it to `tte_OceanAnim`, else it's left alone. The `wp` loop covers both planes, so Myrror oceans are animated by this one block too.

## Findings

### B1 — dead Myrror-ocean block ([MAPGEN.c:7144-7151](../../MoM/src/MAPGEN.c#L7144-L7151)) — OGBUG, faithful

```
if(p_world_map[wp][wy][wx] == (TerType_Count + tte_Ocean))
{
    if(Random(5) == 1) { p_world_map[wp][wy][wx] = (TerType_Count + tte_OceanAnim); }
}
```

This guards for `TerType_Count + tte_Ocean` (`TerType_Count` = `0x2FA` = 762, [TerrType.h:76](../../MoM/src/TerrType.h#L76)). The OG author coded it as if Myrror tiles were stored offset by `TerType_Count` — but they aren't: both planes use the same `0..761` terrain indices, distinguished by `wp`. No map square is ever `762`, so the condition is **always false** (the production comment at [7144](../../MoM/src/MAPGEN.c#L7144): *"will always jump"*). The asm has the identical dead block (`cmp [es:bx], TerType_Count+tte_Ocean` / `jnz`), so its `Random(5)` never fires.

It is also **redundant** — block 1 already matches `tte_Ocean` on both planes, so Myrror oceans get animated regardless. Net effect: none. **Faithful; preserve** — do not "fix" the condition.

## Verification against the asm

- **Block 1** — `cmp tte_Ocean` → `Random(5)` → `tte_OceanAnim` (`0x259`): 1:1.
- **Block 2** — `cmp (TerType_Count + tte_Ocean)` → `Random(5)` → `(TerType_Count + tte_OceanAnim)`: 1:1 (dead in both; production uses the asm's named constants — `TerType_Count`, `tte_Ocean`, `tte_OceanAnim` — not magic literals).
- Loop bounds (both planes × 40 × 60) and tile reads (`p_world_map`, word-strided) match.
- The only live RNG is the one `Random(5)` per `tte_Ocean` tile.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — `1..n`; one `Random(5)` per ocean tile.
- **`p_world_map`** ([MOM_DAT.h:4095](../../MoX/src/MOM_DAT.h#L4095)) — read/written; constants `tte_Ocean` (`0x0`), `tte_OceanAnim` (`0x259`), `TerType_Count` (`0x2FA`).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Animate_Oceans.asm` — IDA Pro 5.5 disassembly (authority); `call Random` (×2; block-2's is dead), `mov [es:bx], TerType_Count+tte_OceanAnim`.
- [MAPGEN.c:646-647](../../MoM/src/MAPGEN.c#L646-L647) — call site + world-map dump.
- [MAPGEN-Shuffle_Terrains.md](MAPGEN-Shuffle_Terrains.md) — sibling per-tile RNG terrain pass; `tte_OceanAnim` is treated as ocean by `Square_Is_Ocean`.
