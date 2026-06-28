AISPELL-AITP_Disenchant.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Disenchant.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Disenchant.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Disenchant_True:
                        case spl_Disenchant_Area:
                            |-> AITP_Disenchant()

---

# `AITP_Disenchant` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Disenchant` | [AISPELL.c:5355-5474](../../MoM/src/AISPELL.c#L5355-L5474) | AI target picker for **Disenchant Area / Disenchant True**: pick a map square to dispel, trying four phases in order — (1) a human city with Flying Fortress, (2) the AI's own strongest warped node, (3) the AI's own most-valuable cursed city, (4) a human enchanted unit on the fortress landmass. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp`, `ST_FALSE` if all four miss. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying one preserved OG bug.** The body ([AISPELL.c:5355](../../MoM/src/AISPELL.c#L5355)) reconstructs `AITP_Disenchant.asm` and matches it across all four phases. It compiles and links as part of AISPELL.c / momlib, and the cast-time dispatch ([AISPELL.c:5036](../../MoM/src/AISPELL.c#L5036)) is live. The one headline issue is an original-game bug faithfully preserved: phase 4 reads `_UNITS[-1]` ([B1](#b1-ogbug--units-1-out-of-bounds)). The asm is the authority.

## Purpose

`Get_Map_Square_Target_For_Spell` routes **Disenchant True / Disenchant Area** here. The picker tries four prioritized targets and returns the first hit. Phases 1 and 4 target the **human** player (strip Flying Fortress / enchanted units); phases 2 and 3 clean up the **AI's own** warped nodes and cursed cities.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5036](../../MoM/src/AISPELL.c#L5036) | `switch(spell_idx)` cases `spl_Disenchant_True` / `spl_Disenchant_Area` → here (active). |

## Code walk (four phases, first hit wins)

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_Disenchant.asm`.

1. **Human city w/ Flying Fortress, 50%** ([5364-5379](../../MoM/src/AISPELL.c#L5364-L5379), asm `loc_E99AC`): for each city `owner_idx == HUMAN_PLAYER_IDX` (0) with `enchantments[FLYING_FORTRESS] != 0`, `if(Random(2) == 1)` → return that city. ✓
2. **Own strongest warped node** ([5381-5404](../../MoM/src/AISPELL.c#L5381-L5404), asm `loc_E9A46`): over `NUM_NODES` (`0x1E`), `owner_idx == player_idx`, `flags & NF_WARPED`, max `power` → return that node. ✓
3. **Own most-valuable cursed city** ([5406-5437](../../MoM/src/AISPELL.c#L5406-L5437), asm `loc_E9B02`): `owner_idx == player_idx` with any of `CURSED_LANDS` / `FAMINE` / `CHAOS_RIFT` / `EVIL_PRESENCE`, max `_ai_all_own_city_values[]` → return that city. ✓
4. **Human enchanted unit on fortress landmass** ([5439-5473](../../MoM/src/AISPELL.c#L5439-L5473), asm `loc_E9BD2`): `fortress_landmass = _landmasses[fortress wp/wy/wx]`; for each unit `owner_idx == HUMAN` with `enchantments != 0` ([5448](../../MoM/src/AISPELL.c#L5448)) on `fortress_landmass`, max `_unit_type_table[type].cost` → return ([B1](#b1-ogbug--units-1-out-of-bounds)). Else `return 0`. ✓

The phase-4 "any enchantment?" test reads the whole 4-byte `uint32_t` field — `_UNITS[itr].enchantments != 0` ([5448](../../MoM/src/AISPELL.c#L5448)) — which matches the asm's `word@0 | word@2` over the `0x18..0x1B` field ([MOM_DAT.h:1849](../../MoX/src/MOM_DAT.h#L1849)).

## B1 (OGBUG) — `_UNITS[-1]` out-of-bounds

Phase 4's success block ([AISPELL.c:5462-5470](../../MoM/src/AISPELL.c#L5462-L5470)) checks `Highest_Cost_Unit != ST_UNDEFINED` but indexes `_UNITS[target_city_idx]`:

```c
if(Highest_Cost_Unit != ST_UNDEFINED)
{
    *targeted_wx = _UNITS[target_city_idx].wx;   /* OGBUG  OOB  target_city_idx is -1 */
    ...
}
```

`target_city_idx` is guaranteed `-1` (`ST_UNDEFINED`) here (phase 3 returned otherwise), so this reads `_UNITS[-1]`. The asm does the same — `loc_E9D04` checks `Highest_Cost_Unit` but loads coordinates from `_UNITS[Target]` (asm `mov ax,[bp+Target]`, the phase-3 city var = -1). **Preserved, not fixed** — the in-code comment documents it. (Reconstruction note: "fixing" it to `Highest_Cost_Unit` would change AI behavior; don't.)

## OG quirks (preserve when reconstructing)

- **B1** — `_UNITS[-1]` out-of-bounds in phase 4 (above).
- Phases 1 & 4 read the **human** player (`owner_idx == 0`); phases 2 & 3 the **AI's own** (`== player_idx`) — faithful asymmetry.

## Sub-functions / external calls

- **`Random(2)`** — the game RNG (phase-1 coin flip).
- Reads `_CITIES[]`, `_NODES[]`, `_UNITS[]`, `_FORTRESSES[]`, `_landmasses[]`, `_unit_type_table[]`, `_ai_all_own_city_values[]`, `_cities`, `_units`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_NODES[i]` stride / count | `imul 30h` / `< 1Eh` | `sizeof(s_NODE)=0x30`; `NUM_NODES = 30`. `NF_WARPED = 1` ([MOM_DAT.h:452](../../MoX/src/MOM_DAT.h#L452)). |
| city `enchantments[…]` | `s_CITY.enchantments.*` | `CHAOS_RIFT=0x01, EVIL_PRESENCE=0x03, CURSED_LANDS=0x04, FAMINE=0x07, FLYING_FORTRESS=0x08` ([MOM_DEF.h:1389-1396](../../MoX/src/MOM_DEF.h#L1389-L1396)). |
| `_UNITS[i].enchantments` | `word@0x18 \| word@0x1A` | `uint32_t` @ `0x18` ([MOM_DAT.h:1849](../../MoX/src/MOM_DAT.h#L1849)); `_UNITS` stride `0x20`. |
| `_landmasses[wp*2400 + wy*60 + wx]` | `_landmasses + wp*0960h + wy*3Ch + wx` | byte read. |
| `_FORTRESSES[idx]` / `_unit_type_table[t].cost` | `idx*4` / `t*24h` | `sizeof(s_FORTRESS)=4`, `sizeof(s_UNIT_TYPE)=0x24`. |

## Related references

- `…\ovr156\AITP_Disenchant.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_Disenchant.c` — Piethawn IDA-C.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../../MoM/src/AISPELL.c#L5002)) — the map-square router.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
