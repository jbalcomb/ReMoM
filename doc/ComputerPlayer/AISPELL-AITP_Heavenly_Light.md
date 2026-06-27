AISPELL-AITP_Heavenly_Light.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_HeavenlyLight.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_HeavenlyLight.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Heavenly_Light()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> Pick_Target_For_City_Enchantment__WIP()
        switch(spell_idx)
            case spl_Heavenly_Light:
                |-> AITP_Heavenly_Light()

---

# `AITP_Heavenly_Light` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Heavenly_Light` | [AISPELL.c:3459-3493](../../MoM/src/AISPELL.c#L3459-L3493) | AI target picker for **Heavenly Light**: scan the casting player's own cities and pick the **highest AI-valued** one that does **not** already have the Heavenly Light enchantment. Returns `ST_TRUE` with `*targeted_city_idx` set, or `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3459](../../MoM/src/AISPELL.c#L3459)) is a direct, faithful reconstruction of `AITP_Heavenly_Light.asm` (the earlier `return 0;` stub is gone); the production symbol added underscores (`HeavenlyLight` → `Heavenly_Light`), the asm/`.c` filename keeps `HeavenlyLight`. It compiles and links as part of AISPELL.c / momlib. No original-game bugs. Same single-pass max-by-value scan as [`AITP_Earth_Gate`](AISPELL-AITP_Earth_Gate.md) / [`AITP_Gaias_Blessing`](AISPELL-AITP_Gaias_Blessing.md) / [`AITP_Wall_Of_Fire`](AISPELL-AITP_Wall_Of_Fire.md), differing only in the gated city-enchantment slot (`HEAVENLY_LIGHT`). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Heavenly Light**, a city enchantment. The AI wants it in its most valuable city that lacks it, so it scans owned cities for the highest `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`) entry whose `enchantments[HEAVENLY_LIGHT]` slot is still clear. Returns a **city index**, using the standard **non-inverted** (`== ST_FALSE`) test.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2368](../../MoM/src/AISPELL.c#L2368) | **Probe**: if a target exists, Heavenly Light is given a turn-scaled priority weight (`AI_OVL_SplPriorities[10] = _turn / 25`). |
| `Pick_Target_For_City_Enchantment__WIP` | [AISPELL.c:3718](../../MoM/src/AISPELL.c#L3718) | Cast-time lookup (`switch(spell_idx) case spl_Heavenly_Light`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_Heavenly_Light.asm`.

```c
best_city_idx = ST_UNDEFINED;                                            // asm  si = -1
highest_value = 0;                                                       // asm  [Highest_Value] = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                  // asm  cx loop, cmp cx,[_cities]
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                      // asm  owner_idx (cbw) == Player_Index
    {
        if(_CITIES[itr_cities].enchantments[HEAVENLY_LIGHT] == ST_FALSE) // asm  cmp enchantments.HEAVENLY_LIGHT,0 / jnz skip
        {
            if(_ai_all_own_city_values[itr_cities] > highest_value)      // asm  jle skip  =>  strictly greater
            {
                best_city_idx = itr_cities;                            // asm  si = cx
                highest_value = _ai_all_own_city_values[itr_cities];   // asm  [Highest_Value] = value
            }
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                     // asm  cmp si,-1 / xor ax,ax
*targeted_city_idx = best_city_idx;  return ST_TRUE;                    // asm  [di] = si / ax = 1
```

Three nested gates, then a strict max. **Faithful 1:1** — every branch, the `== ST_FALSE` (asm `cmp …,0 / jnz` skip-if-present) sense, the comparison direction, and both returns match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`cx*2`). |
| `_CITIES[i].enchantments[HEAVENLY_LIGHT]` | `s_CITY.enchantments.HEAVENLY_LIGHT` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `HEAVENLY_LIGHT = 0x15` ([MOM_DEF.h:1410](../../MoX/src/MOM_DEF.h#L1410)). Per-city array, distinct from the player `Globals[]` overland-enchantment table. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `ST_FALSE` | `0` | "enchantment not present" — `0` means the slot is clear. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Tie-break**: the `> highest_value` test is strictly-greater, so among qualifying cities of equal top value the **lowest city index** is kept (the scan runs `0 -> _cities`).

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_Heavenly_Light.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_Heavenly_Light.c` — Piethawn IDA-C.
- [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md), [AISPELL-AITP_Gaias_Blessing.md](AISPELL-AITP_Gaias_Blessing.md), [AISPELL-AITP_Wall_Of_Fire.md](AISPELL-AITP_Wall_Of_Fire.md) — sibling city-enchantment pickers with the identical scan shape.
- [AISPELL-AITP_Flying_Fortress.md](AISPELL-AITP_Flying_Fortress.md) — sibling whose enchantment test is *inverted* (contrast).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Heavenly Light.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
