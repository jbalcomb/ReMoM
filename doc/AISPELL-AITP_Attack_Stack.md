AISPELL-AITP_Attack_Stack.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_OVL_HarmStack.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_OVL_HarmStack.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Ice_Storm:
                        case spl_Fire_Storm:
                        case spl_Black_Wind:
                        case spl_Stasis:
                            |-> AITP_Attack_Stack()

---

# `AITP_Attack_Stack` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Attack_Stack` | [AISPELL.c:5071-5174](../MoM/src/AISPELL.c#L5071-L5174) | AI target picker for the overland **offensive stack-target** spells (Ice Storm, Fire Storm, Black Wind, Stasis): find the **highest-value enemy stack** that is **visible** to one of the AI's own cities or units, and return its map square. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp` set, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:5071](../MoM/src/AISPELL.c#L5071)) is a faithful reconstruction of `AITP_OVL_HarmStack.asm` (the production renamed `AITP_OVL_HarmStack` → `AITP_Attack_Stack`; the asm/`.c` filename keeps the OG `AITP_OVL_HarmStack`). It compiles and links as part of AISPELL.c / momlib. The value scan, the city/unit visibility passes, and three preserved OG bugs all match the asm. The asm is the authority.

## Purpose

`Get_Map_Square_Target_For_Spell` routes the four overland stack-target spells — **Ice Storm, Fire Storm, Black Wind** (damage) and **Stasis** (disable) — here. The picker walks the precomputed enemy-stack list (`_ai_all_enemy_stacks`, valued by the AI) and, for the most valuable stack the AI can actually **see** (within a city's or unit's sight range), returns that stack's map square (`wx`, `wy`, `wp`).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5012](../MoM/src/AISPELL.c#L5012) | `switch(spell_idx)` cases `spl_Ice_Storm` / `spl_Fire_Storm` / `spl_Black_Wind` / `spl_Stasis` dispatch here. **Faithful** to the asm/IDA-C. |

## Code walk

Line refs are production [AISPELL.c](../MoM/src/AISPELL.c); the authority is `AITP_OVL_HarmStack.asm`.

```c
target_wx = ST_UNDEFINED;  highest_value = 0;                            // asm  Target_X=-1, Highest_Value=0

for(itr_enemy_stack_count = 0; itr_enemy_stack_count < _ai_all_enemy_stack_count; itr_enemy_stack_count++)  // asm si loop
{
    if((unsigned int)_ai_all_enemy_stacks[itr_enemy_stack_count].value > (unsigned int)highest_value)  // asm  ja (UNSIGNED)
    {
        stack_is_visible = ST_FALSE;
        enemy_stack_wx = _ai_all_enemy_stacks[itr_enemy_stack_count].wx;  // .wy, .wp likewise (cbw bytes)

        /* pass 1 — can any of my CITIES see it? */
        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {
            if(stack_is_visible != ST_FALSE) break;
            if(_CITIES[itr_cities].owner_idx == player_idx && _CITIES[itr_cities].wp == enemy_stack_wp)
            {
                scouting_radious = 2;
                if(_CITIES[itr_cities].bldg_status[bt_CityWalls] == bs_Built) scouting_radious = 3;  // asm bldg_status+23h
                if(_CITIES[itr_cities].bldg_status[bt_Oracle]    == bs_Built) scouting_radious = 5;  // asm bldg_status+12h
                if(Delta_XY_With_Wrap(enemy_stack_wx, enemy_stack_wy,
                                      _CITIES[itr_enemy_stack_count].wx, _CITIES[itr_enemy_stack_count].wy,
                                      WORLD_WIDTH) <= scouting_radious)                                // B1: enemy-stack index!
                { highest_value = …value; target_* = enemy_stack_*; stack_is_visible = ST_TRUE; }
            }
        }

        /* pass 2 — else can any of my UNITS see it? */
        if(stack_is_visible == ST_FALSE)
        {
            scouting_radious = 2;                                        // B3: hardcoded 2 for all units
            for(itr_cities = 0; itr_cities < _units; itr_cities++)
            {
                if(stack_is_visible != ST_FALSE) break;
                if(_UNITS[itr_cities].owner_idx == player_idx && _UNITS[itr_cities].wp == enemy_stack_wp)
                {
                    if(Delta_XY_With_Wrap(enemy_stack_wx, enemy_stack_wy,
                                          _UNITS[itr_enemy_stack_count].wx, _UNITS[itr_enemy_stack_count].wy,
                                          WORLD_WIDTH) <= scouting_radious)                            // B1: enemy-stack index!
                    { highest_value = …value; target_* = enemy_stack_*; stack_is_visible = ST_TRUE; }
                }
            }
        }
    }
}

if(target_wx == ST_UNDEFINED) return ST_FALSE;                          // asm  cmp Target_X,-1 / xor ax,ax
*targeted_wx = target_wx; *targeted_wy = target_wy; *targeted_wp = target_wp; return ST_TRUE;   // asm  store / ax=1
```

**Faithful 1:1** — the unsigned value compare (asm `ja`), the signed distance compare (asm `jg`), the per-city sight range, the city-then-unit early-out on `stack_is_visible`, the preserved B1 indexing, and both returns all match. (`WORLD_WIDTH == 60`, the literal the asm pushes.)

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_enemy_stacks[]` | `AI_Enemy_Stacks@` | `struct s_AI_TARGET *` ([MOM_DAT.h:3007](../MoX/src/MOM_DAT.h#L3007)); `.value` is `int16_t` ([MOM_DAT.h:116](../MoX/src/MOM_DAT.h#L116)); stride `imul 6`. |
| `_ai_all_enemy_stack_count` | `AI_Enemy_Stack_Count` | `int16_t` ([MOM_DAT.h:3013](../MoX/src/MOM_DAT.h#L3013)). |
| `bt_CityWalls` / `bt_Oracle` | `bldg_status+23h` / `+12h` | `35` / `18` ([MOM_DAT.h:630](../MoX/src/MOM_DAT.h#L630), [613](../MoX/src/MOM_DAT.h#L613)). |
| `Delta_XY_With_Wrap(...)` | `call Delta_XY_With_Wrap` | [special.h:41](../MoX/src/special.h#L41); wrap width `WORLD_WIDTH = 60`. |
| `_CITIES`/`_UNITS` stride | `imul 72h` / `shl ,5` | `sizeof(s_CITY)=114`, `sizeof(s_UNIT)=32`. |

## OG quirks (preserve when reconstructing)

- **B1 — visibility uses the enemy-stack index**: the `Delta_XY_With_Wrap` distance is measured to `_CITIES[itr_enemy_stack_count]` / `_UNITS[itr_enemy_stack_count]` (the **enemy-stack** loop index) instead of `itr_cities` (the city/unit actually being checked). Verified in the asm: `mov ax, si` before the `_CITIES` address calc (asm 113/121) and the `_UNITS` calc (asm 192/205). So the "can this city/unit see the stack" test compares the stack against an essentially arbitrary array slot — usually nonsense. Preserved.
- **B2 — ignores invisibility** ([in-code note](../MoM/src/AISPELL.c#L5068)): the stack's invisibility is never checked, so invisible stacks are treated as targetable.
- **B3 — ignores unit sight range** ([in-code note](../MoM/src/AISPELL.c#L5069)): the unit pass hardcodes `scouting_radious = 2` (asm `loc_E9563`) instead of each unit's actual scouting range.
- **Unsigned value compare**: the `value` field is a signed `int16_t`, but the OG compares it **unsigned** (`ja`); reproduced with the `(unsigned int)` casts at [5089-5090](../MoM/src/AISPELL.c#L5089-L5090).

All four are original-game behavior; keep them.

## Sub-functions / external calls

- **`Delta_XY_With_Wrap(x1, y1, x2, y2, width)`** — wrapped distance on the cylindrical map ([special.h:41](../MoX/src/special.h#L41)).
- Reads `_ai_all_enemy_stacks[]`, `_ai_all_enemy_stack_count`, `_CITIES[]`, `_UNITS[]`, `_cities`, `_units`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_OVL_HarmStack.asm` — IDA Pro 5.5 disassembly (the authority; OG name keeps `OVL_HarmStack`).
- `…\ovr156\AITP_OVL_HarmStack.c` — Piethawn IDA-C.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../MoM/src/AISPELL.c#L5002)) — the map-square target router that calls this (its `spl_Ice_Storm` / `spl_Fire_Storm` / `spl_Black_Wind` / `spl_Stasis` cases).
- [ComputerPlayer/AISPELL-AI_Spell_Select.md](ComputerPlayer/AISPELL-AI_Spell_Select.md) — the parent dispatcher.
