CITYCALC-City_Maximum_Size.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Maximum_Size.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\City_Maximum_Size.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()
            |-> City_Growth_Rate()
                |-> City_Maximum_Size()
                    |-> City_Food_Terrain()                 CITYCALC
                    |-> City_Food_WildGame()                CITYCALC

---

# `City_Maximum_Size` — Walkthrough

**Location:** [CITYCALC.c:1912-1946](../../MoM/src/CITYCALC.c#L1912-L1946) (~35 lines, ends [line 1946](../../MoM/src/CITYCALC.c#L1946)).
**WZD overlay:** ovr142, p10 (drake178 IDA label: `CTY_GetMaxPop`).
**Called from:** [`City_Growth_Rate`](../../MoM/src/CITYCALC.c#L2512), `Compute_Base_Values_For_Map_Square` (the `*MaxPop` fill at [4012](../../MoM/src/CITYCALC.c#L4012)), and `NX_j_City_Maximum_Size` (per the XREF header comment [1906-1909](../../MoM/src/CITYCALC.c#L1906-L1909)).

> Verified against the **`.asm`** (75 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.**

## Purpose

Computes a city's **maximum population (uncapped)** from food supply plus building/enchantment modifiers: the terrain catchment food (`City_Food_Terrain`), halved under Famine, `+2` for a Granary, `+3` for a Farmers' Market, plus wild-game food (`City_Food_WildGame`). The header comment notes it does *not* apply the hard population cap — callers clamp.

## Signature

```c
int16_t City_Maximum_Size(int16_t city_idx)
```

Local: `maximum_size` (`_SI_`).

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `City_Maximum_Size.asm`.

1. **Terrain food base** ([1916](../../MoM/src/CITYCALC.c#L1916), asm:11-16) — `maximum_size = City_Food_Terrain(city_idx)`.
2. **Famine halving** ([1918-1921](../../MoM/src/CITYCALC.c#L1918-L1921), asm:17-28) — if `enchantments[FAMINE] > 0`, `maximum_size /= 2`. The asm's `cwd; sub ax,dx; sar ax,1` is the signed divide-by-2 (truncate toward zero) idiom = C `/ 2`.
3. **Granary +2** ([1923-1930](../../MoM/src/CITYCALC.c#L1923-L1930), asm:29-45) — if `bldg_status[GRANARY]` is `bs_Built` **or** `bs_Replaced` → `+2`. `GRANARY = 0x1D` matches asm `bldg_status+1Dh`.
4. **Farmers' Market +3** ([1932-1939](../../MoM/src/CITYCALC.c#L1932-L1939), asm:46-62) — if `bldg_status[FARMERS_MARKET]` is `bs_Built` **or** `bs_Replaced` → `+3`. `FARMERS_MARKET = 0x1E` matches asm `bldg_status+1Eh`.
5. **Wild-game food** ([1943](../../MoM/src/CITYCALC.c#L1943), asm:63-68) — `maximum_size += City_Food_WildGame(city_idx)`.
6. **Return** ([1945](../../MoM/src/CITYCALC.c#L1945), asm:69) — `return maximum_size`.

## Notes (faithful — do not "fix")

- **Preserved `// BUGBUG  ¿ no FORESTERS_GUILD ?` dev annotation** ([1941](../../MoM/src/CITYCALC.c#L1941)) — questions why `FORESTERS_GUILD` (`0x1F`, the building index immediately after `FARMERS_MARKET`) is not checked alongside Granary / Farmers' Market. The asm has no `FORESTERS_GUILD` test either (it jumps straight from the Farmers' Market block at asm:62 to the `City_Food_WildGame` call at asm:63-68), so production is faithful. Left as an open DEDU — do not add a check.
- **Granary / Farmers' Market accept `bs_Built` OR `bs_Replaced`** ([1924-1926](../../MoM/src/CITYCALC.c#L1924-L1926), [1933-1935](../../MoM/src/CITYCALC.c#L1933-L1935)) — both build states grant the bonus; matches the asm's two-compare `jz …/jnz …` structure per building (asm:35-43, 52-60). Faithful.

## Related references

- `…\ovr142\City_Maximum_Size.asm` — IDA Pro 5.5 disassembly (ground truth, 75 lines).
- `…\ovr142\City_Maximum_Size.c` — Gemini translation of the asm (matches production).
- Callees: [`City_Food_Terrain`](../../MoM/src/CITYCALC.c#L1638) ([`CITYCALC-City_Food_Terrain.md`](CITYCALC-City_Food_Terrain.md)), [`City_Food_WildGame`](../../MoM/src/CITYCALC.c#L1663) ([`CITYCALC-City_Food_WildGame.md`](CITYCALC-City_Food_WildGame.md)).
- Constants/fields: `_CITIES[].enchantments[FAMINE]`, `_CITIES[].bldg_status[]`, `GRANARY = 0x1D`, `FARMERS_MARKET = 0x1E`, `FORESTERS_GUILD = 0x1F` (unused here), `bs_Built`, `bs_Replaced` — defined in [MOM_DEF.h](../../MoX/src/MOM_DEF.h#L612).
