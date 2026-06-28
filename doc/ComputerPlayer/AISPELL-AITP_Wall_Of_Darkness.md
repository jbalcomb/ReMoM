AISPELL-AITP_Wall_Of_Darkness.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_WallofDarkness.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_WallofDarkness.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Wall_Of_Darkness()

// NOTE: no `case spl_Wall_Of_Darkness` in the cast-time dispatch
//       (Pick_Target_For_City_Enchantment__WIP) — see "How it's reached".

---

# `AITP_Wall_Of_Darkness` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Wall_Of_Darkness` | [AISPELL.c:3453-3488](../../MoM/src/AISPELL.c#L3453-L3488) | AI target picker for **Wall of Darkness**: scan the casting player's own cities and pick the **highest AI-valued** one that does **not** already have the Wall of Darkness enchantment. Returns `ST_TRUE` with `*targeted_city_idx` set, or `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3453](../../MoM/src/AISPELL.c#L3453)) is a direct, faithful reconstruction of `AITP_WallofDarkness.asm` (the earlier `return 0;` stub is gone). It compiles and links as part of AISPELL.c / momlib. No original-game bugs. Same single-pass max-by-value scan as [`AITP_Earth_Gate`](AISPELL-AITP_Earth_Gate.md) / [`AITP_Wall_Of_Fire`](AISPELL-AITP_Wall_Of_Fire.md) / the other city-enchantment pickers, differing only in the gated city-enchantment slot (`WALL_OF_DARKNESS`). The asm is the authority.

## Purpose

The `AITP_*` target picker for **Wall of Darkness**, a city enchantment. The AI wants it in its most valuable city that lacks it, so it scans owned cities for the highest `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`) entry whose `enchantments[WALL_OF_DARKNESS]` slot is still clear. Returns a **city index**, using the standard **non-inverted** (`== ST_FALSE`) test.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2422](../../MoM/src/AISPELL.c#L2422) | **Probe**: if a target exists, Wall of Darkness is given a turn-scaled priority weight (`AI_OVL_SplPriorities[19] = _turn / 20`). |

**No cast-time dispatch case.** Unlike its siblings, there is **no `case spl_Wall_Of_Darkness`** in the friendly-city cast dispatch (`Pick_Target_For_City_Enchantment__WIP`). Across AISPELL.c, `AITP_Wall_Of_Darkness` is called only from the probe above; `spl_Wall_Of_Darkness` otherwise appears only at the spell-selection return ([AISPELL.c:2517](../../MoM/src/AISPELL.c#L2517)). So the probe sets a priority, but the cast-time target re-lookup is not routed through this function in AISPELL.c — either Wall of Darkness is targeted by a different path or the dispatch case is absent. This is a dispatch-wiring observation outside `AITP_Wall_Of_Darkness` itself; the function is correct.

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_WallofDarkness.asm`.

```c
best_city_idx = ST_UNDEFINED;                                            // asm  si = -1
highest_value = 0;                                                       // asm  [Highest_Value] = 0

for(itr_cities = 0; itr_cities < _cities; itr_cities++)                  // asm  cx loop, cmp cx,[_cities]
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                      // asm  owner_idx (cbw) == Player_Index
    {
        if(_CITIES[itr_cities].enchantments[WALL_OF_DARKNESS] == ST_FALSE) // asm  cmp enchantments.WALL_OF_DARKNESS,0 / jnz skip
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
| `_CITIES[i].enchantments[WALL_OF_DARKNESS]` | `s_CITY.enchantments.WALL_OF_DARKNESS` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `WALL_OF_DARKNESS = 0x17` ([MOM_DEF.h:1412](../../MoX/src/MOM_DEF.h#L1412)). Per-city array, distinct from the player `Globals[]` overland-enchantment table. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `ST_FALSE` | `0` | "enchantment not present" — `0` means the slot is clear. |

## OG quirks

None in the function. No dead code, no off-by-ones, no preserved original-game bugs.

- **Tie-break**: the `> highest_value` test is strictly-greater, so among qualifying cities of equal top value the **lowest city index** is kept (the scan runs `0 -> _cities`).
- **Missing cast-time dispatch** (above): a wiring gap/observation in `Pick_Target_For_City_Enchantment__WIP`, not a defect of this picker.

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_ai_all_own_city_values[]`, and `_cities`, and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_WallofDarkness.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_WallofDarkness.c` — Piethawn IDA-C.
- [AISPELL-AITP_Wall_Of_Fire.md](AISPELL-AITP_Wall_Of_Fire.md), [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md), [AISPELL-AITP_Cloud_Of_Shadow.md](AISPELL-AITP_Cloud_Of_Shadow.md) — sibling city-enchantment pickers with the identical scan shape.
- [AISPELL-AITP_Flying_Fortress.md](AISPELL-AITP_Flying_Fortress.md) — sibling whose enchantment test is *inverted* (contrast).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Wall of Darkness.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
