AISPELL-AITP_Wall_Of_Stone.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Wall_Of_Stone.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Wall_Of_Stone.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Wall_Of_Stone()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Wall_Of_Stone:
                |-> AITP_Wall_Of_Stone()

---

# `AITP_Wall_Of_Stone` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Wall_Of_Stone` | [AISPELL.c:2728-2762](../../MoM/src/AISPELL.c#L2728-L2762) | AI target picker for **Wall of Stone**: scan the casting player's own cities and pick the **highest AI-valued** city that does **not** already have city walls. Returns `ST_TRUE` with `*city_idx` set, or `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:2728](../../MoM/src/AISPELL.c#L2728)) is a direct reconstruction of `AITP_Wall_Of_Stone.asm` — a single max-by-value scan over `_CITIES[]` with two gate conditions. It compiles as part of AISPELL.c / momlib. No original-game bugs. The asm is the authority.

## Purpose

One of the `AITP_*` "where do I aim this?" target pickers for friendly-city spells. Wall of Stone builds walls in one of the caster's own cities, so the picker wants the **most valuable wall-less city**. "Value" is the precomputed AI city-valuation table `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`), keyed by city index.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AITP_City_Enchantment` | [AISPELL.c:3082](../../MoM/src/AISPELL.c#L3082) | `switch(spell_idx) case spl_Wall_Of_Stone` — the actual target lookup when the spell is being cast. |
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2319](../../MoM/src/AISPELL.c#L2319) | Used as a **probe**: if a target exists, Wall of Stone is given a priority weight (`AI_OVL_SplPriorities[1] = _turn / 6`). |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_Wall_Of_Stone.asm`.

```c
best_city_idx = ST_UNDEFINED;                                          // asm  si = -1
highest_value = 0;                                                     // asm  [Highest_Value] = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                // asm  cx loop, cmp cx,[_cities]
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                    // asm  owner_idx (cbw) == Player_Index
    {
        if(_CITIES[itr_cities].bldg_status[bt_CityWalls] == bs_NotBuilt)   // asm  bldg_status+23h == bs_NotBuilt
        {
            if(_ai_all_own_city_values[itr_cities] > highest_value)    // asm  jle skip  =>  strictly greater
            {
                best_city_idx = itr_cities;                           // asm  si = cx
                highest_value = _ai_all_own_city_values[itr_cities];  // asm  [Highest_Value] = value
            }
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                    // asm  cmp si,-1 / xor ax,ax
*city_idx = best_city_idx;  return ST_TRUE;                            // asm  [di] = si / ax = 1
```

Three nested gates, then a strict max. **Faithful 1:1** — every branch, the comparison sense, and both returns match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); asm indexes it as a word array (`cx*2`). |
| `bt_CityWalls` | `bldg_status + 23h` | `35` ([MOM_DAT.h:630](../../MoX/src/MOM_DAT.h#L630)) = `0x23`. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `bs_NotBuilt` | `bs_NotBuilt` | same symbol. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Tie-break**: the `> highest_value` test is strictly-greater, so among cities of equal top value the **lowest city index** is kept (the scan runs `0 -> _cities`).

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*city_idx`.

## Related references

- `…\ovr156\AITP_Wall_Of_Stone.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_Wall_Of_Stone.c` — Piethawn IDA-C.
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Wall of Stone.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
