AISPELL-AITP_Earth_Gate.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_EarthGate.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_EarthGate.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Earth_Gate()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Earth_Gate:
                |-> AITP_Earth_Gate()

---

# `AITP_Earth_Gate` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Earth_Gate` | [AISPELL.c:3138-3172](../../MoM/src/AISPELL.c#L3138-L3172) | AI target picker for **Earth Gate**: scan the casting player's own cities and pick the **highest AI-valued** one that does **not** already have the Earth Gate enchantment. Returns `ST_TRUE` with `*targeted_city_idx` set, or `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3138](../../MoM/src/AISPELL.c#L3138)) is a direct, faithful reconstruction of `AITP_EarthGate.asm` (the earlier `return 0;` stub is gone); the production symbol added an underscore (`EarthGate` → `Earth_Gate`), the asm/`.c` filename keeps `EarthGate`. It compiles and links as part of AISPELL.c / momlib. No original-game bugs. Structurally identical to [`AITP_Wall_Of_Stone`](AISPELL-AITP_Wall_Of_Stone.md) — a single max-by-value scan with two gates; only the second gate differs (city-enchantment slot vs city-walls building). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Earth Gate**, a city enchantment. The AI wants it in its most valuable city that lacks it, so it scans owned cities for the highest `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`) entry whose `enchantments[EARTH_GATE]` slot is still clear. Returns a **city index**, like the other city-enchantment pickers.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2344](../../MoM/src/AISPELL.c#L2344) | **Probe**: if a target exists, Earth Gate is given a turn-scaled priority weight (`AI_OVL_SplPriorities[6] = _turn / 15`). |
| `AITP_City_Enchantment` | [AISPELL.c:3418](../../MoM/src/AISPELL.c#L3418) | Cast-time lookup (`switch(spell_idx) case spl_Earth_Gate`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_EarthGate.asm`.

```c
best_city_idx = ST_UNDEFINED;                                         // asm  si = -1
highest_value = 0;                                                    // asm  [Highest_Value] = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)               // asm  cx loop, cmp cx,[_cities]
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                   // asm  owner_idx (cbw) == Player_Index
    {
        if(_CITIES[itr_cities].enchantments[EARTH_GATE] == ST_FALSE)  // asm  cmp enchantments.EARTH_GATE,0 / jnz skip
        {
            if(_ai_all_own_city_values[itr_cities] > highest_value)   // asm  jle skip  =>  strictly greater
            {
                best_city_idx = itr_cities;                          // asm  si = cx
                highest_value = _ai_all_own_city_values[itr_cities]; // asm  [Highest_Value] = value
            }
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                   // asm  cmp si,-1 / xor ax,ax
*targeted_city_idx = best_city_idx;  return ST_TRUE;                  // asm  [di] = si / ax = 1
```

Three nested gates, then a strict max. **Faithful 1:1** — every branch, the `== ST_FALSE` (asm `cmp …,0 / jnz` skip-if-present) sense, the comparison direction, and both returns match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`cx*2`). |
| `_CITIES[i].enchantments[EARTH_GATE]` | `s_CITY.enchantments.EARTH_GATE` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `EARTH_GATE = 0x0F` ([MOM_DEF.h:1404](../../MoX/src/MOM_DEF.h#L1404)). This is the **per-city** enchantment array, distinct from the player `Globals[]` overland-enchantment table. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `ST_FALSE` | `0` | "enchantment not present" — `0` means the slot is clear. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Tie-break**: the `> highest_value` test is strictly-greater, so among qualifying cities of equal top value the **lowest city index** is kept (the scan runs `0 -> _cities`).

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_EarthGate.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_EarthGate.c` — Piethawn IDA-C.
- [AISPELL-AITP_Wall_Of_Stone.md](AISPELL-AITP_Wall_Of_Stone.md) — sibling `AITP_*` city target picker with the identical scan shape (gated on city walls instead of a city enchantment).
- [AISPELL-AITP_Move_Fortress.md](AISPELL-AITP_Move_Fortress.md) — sibling city-index picker (two-pass, garrison strength).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Earth Gate.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
