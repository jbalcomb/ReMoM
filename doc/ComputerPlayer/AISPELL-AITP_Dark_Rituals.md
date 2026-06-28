AISPELL-AITP_Dark_Rituals.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_DarkRituals.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_DarkRituals.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Dark_Rituals()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Dark_Rituals:
                |-> AITP_Dark_Rituals()

---

# `AITP_Dark_Rituals` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Dark_Rituals` | [AISPELL.c:3739-3780](../../MoM/src/AISPELL.c#L3739-L3780) | AI target picker for **Dark Rituals**: scan the casting player's own cities and pick the **highest AI-valued** one that does **not** already have the Dark Rituals enchantment **and has a Temple built**. Returns `ST_TRUE` with `*targeted_city_idx` set, or `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3739](../../MoM/src/AISPELL.c#L3739)) is a direct, faithful reconstruction of `AITP_DarkRituals.asm` (the earlier `return 0;` stub is gone). It compiles and links as part of AISPELL.c / momlib. No original-game bugs. It is the city-enchantment max-by-value scan (like [`AITP_Earth_Gate`](AISPELL-AITP_Earth_Gate.md)) plus **one extra gate**: the city must have a **Temple** (`bs_Built` or `bs_Replaced`). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Dark Rituals**, a city enchantment that requires a Temple in the city. The AI wants it in its most valuable eligible city that lacks it, so it scans owned cities for the highest `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`) entry whose `enchantments[DARK_RITUALS]` slot is clear **and** whose `bldg_status[bt_Temple]` is built. Returns a **city index**.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2398](../../MoM/src/AISPELL.c#L2398) | **Probe**: if a target exists, Dark Rituals is given a fixed priority weight (`AI_OVL_SplPriorities[15] = 100`). |
| `AITP_City_Enchantment` | [AISPELL.c:3979](../../MoM/src/AISPELL.c#L3979) | Cast-time lookup (`switch(spell_idx) case spl_Dark_Rituals`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_DarkRituals.asm`.

```c
best_city_idx = ST_UNDEFINED;                                            // asm  si = -1
highest_value = 0;                                                       // asm  [Highest_Value] = 0

for(cx_city_idx = 0; cx_city_idx < _cities; cx_city_idx++)              // asm  cx loop, cmp cx,[_cities]
{
    if(_CITIES[cx_city_idx].owner_idx == player_idx)                     // asm  owner_idx (cbw) == Player_Index
    {
        if(_CITIES[cx_city_idx].enchantments[DARK_RITUALS] == ST_FALSE)  // asm  cmp enchantments.DARK_RITUALS,0 / jnz skip
        {
            if(_CITIES[cx_city_idx].bldg_status[bt_Temple] == bs_Built   // asm  cmp bldg_status+17h, bs_Built / jz qualify
            || _CITIES[cx_city_idx].bldg_status[bt_Temple] == bs_Replaced) // asm  cmp bldg_status+17h, bs_Replaced / jnz skip
            {
                if(_ai_all_own_city_values[cx_city_idx] > highest_value)  // asm  jle skip  =>  strictly greater
                {
                    best_city_idx = cx_city_idx;                       // asm  si = cx
                    highest_value = _ai_all_own_city_values[cx_city_idx];
                }
            }
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                     // asm  cmp si,-1 / xor ax,ax
*targeted_city_idx = best_city_idx;  return ST_TRUE;                    // asm  [di] = si / ax = 1
```

Four nested gates, then a strict max. **Faithful 1:1** — the enchantment-absence test, the Temple `Built || Replaced` short-circuit (asm `jz` qualify on Built, else `cmp Replaced / jnz` skip), the strict-greater compare, and both returns all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`cx*2`). |
| `_CITIES[i].enchantments[DARK_RITUALS]` | `s_CITY.enchantments.DARK_RITUALS` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `DARK_RITUALS = 0x02` ([MOM_DEF.h:1391](../../MoX/src/MOM_DEF.h#L1391)). |
| `_CITIES[i].bldg_status[bt_Temple]` | `s_CITY.bldg_status + 17h` | `bt_Temple = 23 = 0x17` ([MOM_DAT.h:618](../../MoX/src/MOM_DAT.h#L618)). |
| `bs_Built` / `bs_Replaced` | `bs_Built` / `bs_Replaced` | `1` / `0` ([MOM_DAT.h:686-687](../../MoX/src/MOM_DAT.h#L686-L687)). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `ST_FALSE` | `0` | "enchantment not present" — `0` means the slot is clear. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Temple requirement**: the extra `bldg_status[bt_Temple]` gate reflects the spell's rule (Dark Rituals needs a Temple) — by design, not a bug.
- **Tie-break**: the `> highest_value` test is strictly-greater, so among qualifying cities of equal top value the **lowest city index** is kept (the scan runs `0 -> _cities`).

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_DarkRituals.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_DarkRituals.c` — Piethawn IDA-C.
- [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md), [AISPELL-AITP_Inspirations.md](AISPELL-AITP_Inspirations.md), [AISPELL-AITP_Prosperity.md](AISPELL-AITP_Prosperity.md) — sibling city-enchantment pickers with the base scan shape (no building gate).
- [AISPELL-AITP_Flying_Fortress.md](AISPELL-AITP_Flying_Fortress.md) — sibling whose enchantment test is *inverted* (contrast).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Dark Rituals.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
