AISPELL-AITP_Flying_Fortress.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_FlyingFortress.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_FlyingFortress.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Flying_Fortress()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Flying_Fortress:
                |-> AITP_Flying_Fortress()

---

# `AITP_Flying_Fortress` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Flying_Fortress` | [AISPELL.c:3174-3209](../../MoM/src/AISPELL.c#L3174-L3209) | AI target picker for **Flying Fortress**: locate the player's fortress city and return it as the target — **but only if it already has the Flying Fortress enchantment** (an inverted test, [B1](#b1-ogbug--inverted-enchantment-test)). Returns `ST_TRUE` with `*targeted_city_idx`, or `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying a preserved OG bug.** The body ([AISPELL.c:3174](../../MoM/src/AISPELL.c#L3174)) is a direct, faithful reconstruction of `AITP_FlyingFortress.asm` (the earlier `return 0;` stub is gone); the production symbol added underscores (`FlyingFortress` → `Flying_Fortress`), the asm/`.c` filename keeps `FlyingFortress`. It compiles and links as part of AISPELL.c / momlib. The presence test is **inverted** relative to the sibling city-enchantment pickers — faithfully reproduced from the asm `jnz` ([B1](#b1-ogbug--inverted-enchantment-test)). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Flying Fortress**, a city enchantment that applies to the wizard's fortress. Unlike the all-cities scanners ([Earth Gate](AISPELL-AITP_Earth_Gate.md), [Gaia's Blessing](AISPELL-AITP_Gaias_Blessing.md)), it considers **only the fortress city** — found by matching `_CITIES[].wx/wy/wp` against `_FORTRESSES[player_idx]` — and there is no city-value comparison. It then checks whether that city carries the Flying Fortress enchantment and (per [B1](#b1-ogbug--inverted-enchantment-test)) returns it as a target only when it **already does**.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2356](../../MoM/src/AISPELL.c#L2356) | **Probe**: if a target is returned, Flying Fortress is given a fixed priority weight (`AI_OVL_SplPriorities[8] = 100`). |
| `AITP_City_Enchantment` | [AISPELL.c:3611](../../MoM/src/AISPELL.c#L3611) | Cast-time lookup (`switch(spell_idx) case spl_Flying_Fortress`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_FlyingFortress.asm`.

```c
/* loop — locate the player's fortress city (no owner check; matched by coords) */
for(itr_cities = 0; itr_cities < _cities; itr_cities++)             // asm  si loop
{
    if(_CITIES[itr_cities].wx == _FORTRESSES[player_idx].wx &&      // asm  city.wx == FORTRESSES[di].wx
       _CITIES[itr_cities].wy == _FORTRESSES[player_idx].wy &&      // asm  city.wy == FORTRESSES[di].wy
       _CITIES[itr_cities].wp == _FORTRESSES[player_idx].wp)        // asm  city.wp == FORTRESSES[di].wp
    {
        fortress_city_idx = itr_cities;                            // asm  Fortress_City = si
    }
}

if(_CITIES[fortress_city_idx].enchantments[FLYING_FORTRESS] != ST_FALSE)  // asm  cmp …FLYING_FORTRESS,0 / jnz E6F96
{
    target_city_idx = fortress_city_idx;                          // asm  (enchant != 0) Target_City = Fortress_City
}
else
{
    target_city_idx = ST_UNDEFINED;                               // asm  (enchant == 0) Target_City = -1
}

if(target_city_idx == ST_UNDEFINED)  return ST_FALSE;             // asm  cmp Target_City,-1 / xor ax,ax
*targeted_city_idx = target_city_idx;  return ST_TRUE;            // asm  [City@] = Target_City / ax = 1
```

**Faithful 1:1** — the fortress-coordinate match loop, the inverted enchantment test (asm `jnz` → set target on *present*), and both returns all match the asm. There is no `_ai_all_own_city_values[]` lookup here (no best-value scan).

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_FORTRESSES[player_idx]` | `_FORTRESSES + di*4` | `extern struct s_FORTRESS *` ([MOM_DAT.h:4089](../../MoX/src/MOM_DAT.h#L4089)); `sizeof(s_FORTRESS) = 4` (asm `shl dx, 2`). |
| `_CITIES[i].enchantments[FLYING_FORTRESS]` | `s_CITY.enchantments.FLYING_FORTRESS` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `FLYING_FORTRESS = 0x08` ([MOM_DEF.h:1397](../../MoX/src/MOM_DEF.h#L1397)). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `ST_FALSE` | `0` | `0` means the enchantment slot is clear. |

## B1 (OGBUG) — inverted enchantment test

Every sibling city-enchantment picker qualifies a city when the enchantment is **absent** (`enchantments[X] == ST_FALSE`). This one does the opposite: it returns the fortress city as a target only when `enchantments[FLYING_FORTRESS] != ST_FALSE` — i.e. when the fortress **already has** Flying Fortress. The asm confirms it: line 68 `cmp [enchantments.FLYING_FORTRESS], 0`, line 69 `jnz` → set `Target_City = Fortress_City` on *nonzero* (present); the `== 0` (absent) path sets `-1`.

Consequence: the AI returns "no target" precisely when a fortress *needs* Flying Fortress, and returns a target (driving `AI_OVL_SplPriorities[8] = 100`) only when the fortress already has it — i.e. it can only ever schedule a redundant re-cast, never a useful one. The production doxygen ([AISPELL.c:3163-3165](../../MoM/src/AISPELL.c#L3163-L3165)) notes the inversion explicitly.

Faithfully reproduced from the asm — **preserved, not fixed**. Flipping the test to `== ST_FALSE` would change AI behavior; don't.

## OG quirks (preserve when reconstructing)

- **B1** — inverted enchantment presence test (above): the dominant issue.
- **Fortress-only**: only the fortress city is ever considered; no all-cities value scan (matches the asm and the spell's nature).
- **`fortress_city_idx` initializer**: production declares `int16_t fortress_city_idx = 0;`; the asm leaves `Fortress_City` uninitialized until the coordinate-match loop sets it. Inert in practice (a fortress always sits in one of the player's cities). Listed for strict bookkeeping; not a reconstruction error to "fix back".

## Sub-functions / external calls

- None. The function only reads `_CITIES[]` and `_FORTRESSES[]` (and `_cities`), and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_FlyingFortress.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_FlyingFortress.c` — Piethawn IDA-C.
- [AISPELL-AITP_Move_Fortress.md](AISPELL-AITP_Move_Fortress.md) — sibling that also locates the fortress city by coordinate match.
- [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md), [AISPELL-AITP_Gaias_Blessing.md](AISPELL-AITP_Gaias_Blessing.md) — sibling city-enchantment pickers that use the **non-inverted** (`== ST_FALSE`) test.
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Flying Fortress.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
