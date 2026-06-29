AISPELL-Select_Unit_For_Enchantment.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\sub_E8448.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\sub_E8448.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                case scc_Unit_Enchantment_Normal_Only:
                    |-> AITP_Unit_Enchantment()
                        |-> Select_Unit_For_Enchantment()

Cast_Chaos_Channels
    |-> AITP_Unit_Enchantment()
        |-> Select_Unit_For_Enchantment()
Cast_Black_Wind
    |-> AITP_Unit_Enchantment()
        |-> Select_Unit_For_Enchantment()
Cast_Word_Of_Recall
    |-> AITP_Unit_Enchantment()
        |-> Select_Unit_For_Enchantment()
Cast_Lycantrophy
    |-> AITP_Unit_Enchantment()
        |-> Select_Unit_For_Enchantment()

---

# `Select_Unit_For_Enchantment` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Select_Unit_For_Enchantment` | [AISPELL.c:4521-4698](../../MoM/src/AISPELL.c#L4521-L4698) | The unit-level worker behind `AITP_Unit_Enchantment`: **find the best own unit to receive a unit enchantment**. Picks the AI's most valuable **non-garrison** stack (else the stack on its Fortress), then scores the units on that tile and returns the highest-scoring eligible one that doesn't already carry the enchantment. Returns `ST_TRUE` with `*targeted_unit_idx`, `ST_FALSE` if none qualifies. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:4521](../../MoM/src/AISPELL.c#L4521)) is a faithful reconstruction of `sub_E8448.asm` (production renamed drake178's `sub_E8448` → `Select_Unit_For_Enchantment`; the asm/`.c` filename keeps the OG name; IDA-C labels it `AI_SelectBestTransportUnit`). It compiles and links as part of AISPELL.c / momlib. The three-phase scan, all unit filters, the upkeep scoring, the movement-spell special scoring, and the preserved `_ai_all_own_stacks[-1]` OOB read all match the asm. The asm is the authority.

## Purpose

`AITP_Unit_Enchantment` decides *which spell* to cast and the per-spell filters; `Select_Unit_For_Enchantment` does the actual unit selection. The AI prefers to buff a unit in its strongest **field army** (a stack not sitting in a city), falling back to the garrison on its **Fortress** if it has no field stacks. Within that stack it scores candidates by **`Upkeep`** (a unit-quality proxy) and returns the best one that passes the spell's filters and does not already have the enchantment.

The IDA-C name `AI_SelectBestTransportUnit` over-narrows it — the "transport" flavor comes only from the optional movement-spell scoring ([special scoring](#movement-spell-special-scoring-flag4)); the routine serves all 33 unit-enchantment pickers, not just movement spells.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AITP_Unit_Enchantment` | [AISPELL.c:4476-4509](../../MoM/src/AISPELL.c#L4476-L4509) | the **sole** caller — each of its 33 non-`Blur` switch cases calls here with the spell's `UE_*` mask + 5 flags. |

## Signature & the merged 32-bit mask

```c
int16_t Select_Unit_For_Enchantment(int16_t player_idx, int32_t unit_enchantment, int16_t * targeted_unit_idx,
                                     int16_t flag1, int16_t flag2, int16_t flag3, int16_t flag4, int16_t flag5)
```

The OG passes the enchantment as **two 16-bit words** (`arg_2` = low / `status_18`, `arg_4` = high / `status_1A`) and tests them separately (`loc_E870A`: `dx = enchant_lo & arg_2; ax = enchant_hi & arg_4; or dx,ax; jz keep`). Production folds them into one `int32_t unit_enchantment` (the `UE_*` constant) and tests `_UNITS[i].enchantments & unit_enchantment` — the **correct** merge, since `UE_value = (block_high << 16) | block_low` and `_UNITS[].enchantments` is the 32-bit field at unit offset `+18h`/`+1Ah`. Folding two words into one `int32` frees a stack slot, so the OG's dead trailing `arg_10` (always `0`, never read by the callee) survives as the unused `flag5` parameter. Net: 8 params either way.

## Code walk — three phases

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `sub_E8448.asm`.

### Phase 1 — best non-garrison field stack ([4540-4566](../../MoM/src/AISPELL.c#L4540-L4566), asm `loc_E845F`..`loc_E852A`)

```c
best_stack_idx = ST_UNDEFINED;  max_score = 0;
for(itr_stacks ...) {                                  // over _ai_all_own_stack_count
    found_city_at_stack = ST_FALSE;
    for(itr_cities ...) if(stack[itr].wx/wy/wp == _CITIES[itr_cities].wx/wy/wp) { found_city_at_stack = ST_TRUE; break; }
    if(found_city_at_stack == ST_FALSE)                // field stack only
        if(stack[itr].value > max_score) { best_stack_idx = itr; max_score = stack[itr].value; }
}
```

### Phase 2 — Fortress fallback ([4568-4583](../../MoM/src/AISPELL.c#L4568-L4583), asm `loc_E8533`..`loc_E85C6`)

```c
if(best_stack_idx == ST_UNDEFINED)
    for(itr_stacks ...) if(stack[itr].wx/wy/wp == _FORTRESSES[player_idx].wx/wy/wp) best_stack_idx = itr;
```

### Phase 3 — best unit on that stack's tile ([4588-4686](../../MoM/src/AISPELL.c#L4588-L4686), asm `loc_E85C6`..`loc_E8805`)

```c
best_unit_idx = ST_UNDEFINED;  max_score = 0;
for(itr_units ...) {                                   // over _units
    if(unit.wx/wy/wp != _ai_all_own_stacks[best_stack_idx].wx/wy/wp) continue;   // OGBUG: OOB when best_stack_idx == -1
    if(flag1 && unit.type >= ut_Magic_Spirit)   continue;   // exclude Fantastic         (0x9A)
    if(flag2 && unit.type <  ut_Magic_Spirit)   continue;   // exclude non-Fantastic
    if(_unit_type_table[unit.type].Attribs_2 != 0)          continue;   // skip summoned
    if(_unit_type_table[unit.type].Abilities & UA_CREATEOUTPOST) continue; // skip settlers (0x20)
    if(_unit_type_table[unit.type].Abilities & UA_MELD)     continue;   // skip melders (0x400)
    if(flag3 && unit.type <= ut_BarbSwordsmen)  continue;   // exclude cheap racial fodder (0x28)
    if(unit.enchantments & unit_enchantment)    continue;   // already buffed
    if(flag4 != ST_TRUE && unit.type == ut_Floating_Island) continue;   // 0xBE

    current_unit_score = _unit_type_table[unit.type].Upkeep;  // +1A8h
    unit_plane = unit.wp;                                     // set, never read (NIU, asm var_6)
    if(flag4 == ST_TRUE) {                                    // movement-spell scoring
        if(_unit_type_table[unit.type].Abilities & UA_CREATEOUTPOST) current_unit_score *= 20;  // (unreachable — see quirks)
        if((Move_Flags & MV_FLYING) || (Move_Flags & MV_SWIMMING))   current_unit_score = 0;
    }
    if(current_unit_score > max_score) { best_unit_idx = itr_units; max_score = current_unit_score; }
}
if(best_unit_idx == ST_UNDEFINED) return ST_FALSE;
*targeted_unit_idx = best_unit_idx; return ST_TRUE;
```

**Faithful 1:1** — every filter, the upkeep score, the special-scoring branch, the unconditional `_ai_all_own_stacks[best_stack_idx]` index (`loc_E85D5`, no sign guard), and both returns match the asm.

### Flag parameters (set per-spell by `AITP_Unit_Enchantment`)

`flag1`-`flag4` are eligibility/scoring toggles; `flag5` is dead. The "Spells that pass `1`" column is the set of `AITP_Unit_Enchantment` cases that enable each (verified against the asm push tuples).

| Param | asm arg | When set (`1`), the picker… | Spells that pass `1` |
|---|---|---|---|
| `flag1` (exclude Fantastic) | `arg_8` | skips units with `type >= ut_Magic_Spirit` (154) — keeps only normal/racial units (drops Magic-Spirit-and-above: summoned + heroes) | Giant Strength, Eldritch Weapon, Flame Blade, Holy Weapon, Chaos Channels, Lycanthropy |
| `flag2` (exclude non-Fantastic) | `arg_A` | skips units with `type < ut_Magic_Spirit` — keeps only Magic-Spirit-and-above | Spell Lock |
| `flag3` (exclude low type) | `arg_C` | skips units with `type <= ut_BarbSwordsmen` (40) — drops the cheapest racial fodder | Lycanthropy |
| `flag4` (special scoring) | `arg_E` | switches on movement-spell scoring (settlers ×20, already-flying/swimming → score 0) **and** un-gates `ut_Floating_Island` | Water Walking, Flight, Planar Travel |
| `flag5` (unused) | `arg_10` | nothing — never read (the OG's dead trailing arg) | none (`0` in all 34 calls) |

The remaining 23 picker spells pass all flags `0` (plain "highest-upkeep eligible unit").

### Movement-spell special scoring (`flag4`)

Set only for the three movement enchantments (Water Walking, Flight, Planar Travel). When on: settlers (`UA_CREATEOUTPOST`) get score ×20, and units that already **fly or swim** score `0` (no value in giving movement to an already-mobile unit). This is the branch that earned the IDA-C's "transport" name. It also un-gates `ut_Floating_Island` (the Floating-Island skip applies only when `flag4` is **off**).

### Symbol / layout cross-checks (verified)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_stacks[]` | `AI_Own_Stacks` | stride `0Ah`=10; `.wx/.wy/.wp` at `+0/+1/+2`, `.value` at `+4`. |
| `_CITIES[i].wx/wy/wp` | `+0Fh/+10h/+11h` | `sizeof(s_CITY)=0x72`. |
| `_FORTRESSES[player_idx]` | `_FORTRESSES`, `shl 2` | stride 4 (wx/wy/wp). |
| `_UNITS[i]` | `shl 5` | stride `0x20`; `.type` at `+5`, `.enchantments` (int32) at `+18h`. |
| `_unit_type_table[t]` | `imul 24h` | stride `0x24`; `Upkeep +1A8h`, `Attribs_2 +1B0h`, `Move_Flags +1B4h`, `Abilities +1BAh`. |
| `ut_Magic_Spirit` / `ut_BarbSwordsmen` / `ut_Floating_Island` | `9Ah` / `28h` / `0BEh` | `154` / `40` / `190` ([UNITTYPE.h:167](../../MoM/src/UNITTYPE.h#L167), [53](../../MoM/src/UNITTYPE.h#L53), [203](../../MoM/src/UNITTYPE.h#L203)). |
| `UA_CREATEOUTPOST` / `UA_MELD` | `20h` / `400h` | ([MOM_DEF.h:705](../../MoX/src/MOM_DEF.h#L705), [710](../../MoX/src/MOM_DEF.h#L710)). |
| `MV_FLYING` / `MV_SWIMMING` | `8` / `4` | ([MOM_DEF.h:524](../../MoX/src/MOM_DEF.h#L524), [523](../../MoX/src/MOM_DEF.h#L523)). |

## OG quirks (preserve when reconstructing)

- **OOB read `_ai_all_own_stacks[best_stack_idx]` when `best_stack_idx == -1`** ([4591-4601](../../MoM/src/AISPELL.c#L4591-L4601), tagged `OGBUG`). The asm (`loc_E85D5`) indexes the stack array with no sign guard, so when the AI owns no stacks at all it reads the element just before the array. Benign — that state only arises with zero stacks, where the unit loop matches nothing and the function returns `ST_FALSE` regardless. Preserved (do **not** add a `best_stack_idx >= 0` guard — that would deviate from the asm).
- **`unit_plane` is a dead store** — set to `_UNITS[i].wp` ([4657](../../MoM/src/AISPELL.c#L4657)) and never read (asm `var_6`). NIU; preserved.
- **The ×20 settler boost is unreachable** — the special-scoring branch multiplies score by 20 for `UA_CREATEOUTPOST` units ([4666](../../MoM/src/AISPELL.c#L4666)), but settlers were already skipped by the `UA_CREATEOUTPOST` filter ([4623](../../MoM/src/AISPELL.c#L4623)) earlier in the loop, so it can never fire. Faithfully preserved (asm has the same dead branch).
- **Last-match wins in the Fortress fallback** — phase 2 keeps assigning `best_stack_idx` without breaking, so if two stacks share the Fortress tile the higher-indexed one wins. Matches the asm.

## Sub-functions / external calls

- None. Reads `_ai_all_own_stacks[]`, `_ai_all_own_stack_count`, `_CITIES[]`, `_cities`, `_FORTRESSES[]`, `_UNITS[]`, `_units`, `_unit_type_table[]`; writes through `*targeted_unit_idx`.

## Related references

- `…\ovr156\sub_E8448.asm` — IDA Pro 5.5 disassembly (the authority; OG name `sub_E8448`).
- `…\ovr156\sub_E8448.c` — Piethawn IDA-C (`AI_SelectBestTransportUnit`; keeps the two 16-bit mask words and the dead 9th `arg_10`).
- [AISPELL-AITP_Unit_Enchantment.md](AISPELL-AITP_Unit_Enchantment.md) — the sole caller; documents the 34-case mask/flag dispatch that feeds this worker.
