AISPELL-AITP_Gaias_Blessing.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_GaiasBlessing.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_GaiasBlessing.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Gaias_Blessing()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Gaias_Blessing:
                |-> AITP_Gaias_Blessing()

---

# `AITP_Gaias_Blessing` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Gaias_Blessing` | [AISPELL.c:3218-3252](../../MoM/src/AISPELL.c#L3218-L3252) | AI target picker for **Gaia's Blessing**: scan the casting player's own cities and pick the **highest AI-valued** one that does **not** already have the Gaia's Blessing enchantment. Returns `ST_TRUE` with `*targeted_city_idx` set, or `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3218](../../MoM/src/AISPELL.c#L3218)) is a direct, faithful reconstruction of `AITP_GaiasBlessing.asm` (the earlier `return 0;` stub is gone); the production symbol added underscores (`GaiasBlessing` → `Gaias_Blessing`), the asm/`.c` filename keeps `GaiasBlessing`. It compiles and links as part of AISPELL.c / momlib. No original-game bugs. The asm is **byte-for-byte the same shape** as [`AITP_Earth_Gate`](AISPELL-AITP_Earth_Gate.md) — only the gated city-enchantment slot differs (`GAIAS_BLESSING` vs `EARTH_GATE`). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Gaia's Blessing**, a city enchantment. The AI wants it in its most valuable city that lacks it, so it scans owned cities for the highest `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`) entry whose `enchantments[GAIAS_BLESSING]` slot is still clear. Returns a **city index**, like the other city-enchantment pickers.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2350](../../MoM/src/AISPELL.c#L2350) | **Probe**: if a target exists, Gaia's Blessing is given a high fixed priority weight (`AI_OVL_SplPriorities[7] = 300`). |
| `AITP_City_Enchantment` | [AISPELL.c:3505](../../MoM/src/AISPELL.c#L3505) | Cast-time lookup (`switch(spell_idx) case spl_Gaias_Blessing`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_GaiasBlessing.asm`.

```c
best_city_idx = ST_UNDEFINED;                                            // asm  si = -1
highest_value = 0;                                                       // asm  [Highest_Value] = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                  // asm  cx loop, cmp cx,[_cities]
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                      // asm  owner_idx (cbw) == Player_Index
    {
        if(_CITIES[itr_cities].enchantments[GAIAS_BLESSING] == ST_FALSE) // asm  cmp enchantments.GAIAS_BLESSING,0 / jnz skip
        {
            if(_ai_all_own_city_values[itr_cities] > highest_value)      // asm  jle skip  =>  strictly greater
            {
                best_city_idx = itr_cities;                            // asm  si = cx
                highest_value = _ai_all_own_city_values[itr_cities];   // asm  [Highest_Value] = value
            }
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                      // asm  cmp si,-1 / xor ax,ax
*targeted_city_idx = best_city_idx;  return ST_TRUE;                     // asm  [di] = si / ax = 1
```

Three nested gates, then a strict max. **Faithful 1:1** — every branch, the `== ST_FALSE` (asm `cmp …,0 / jnz` skip-if-present) sense, the comparison direction, and both returns match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`cx*2`). |
| `_CITIES[i].enchantments[GAIAS_BLESSING]` | `s_CITY.enchantments.GAIAS_BLESSING` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `GAIAS_BLESSING = 0x11` ([MOM_DEF.h:1406](../../MoX/src/MOM_DEF.h#L1406)). Per-city array, distinct from the player `Globals[]` overland-enchantment table. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `ST_FALSE` | `0` | "enchantment not present" — `0` means the slot is clear. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Tie-break**: the `> highest_value` test is strictly-greater, so among qualifying cities of equal top value the **lowest city index** is kept (the scan runs `0 -> _cities`).

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_GaiasBlessing.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_GaiasBlessing.c` — Piethawn IDA-C.
- [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md) — sibling `AITP_*` city-enchantment picker with the identical scan shape (gated on `EARTH_GATE`).
- [AISPELL-AITP_Wall_Of_Stone.md](AISPELL-AITP_Wall_Of_Stone.md) — sibling city-index picker (gated on city walls).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Gaia's Blessing.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
