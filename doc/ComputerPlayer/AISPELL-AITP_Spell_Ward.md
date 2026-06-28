AISPELL-AITP_Spell_Ward.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_SpellWard__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_SpellWard__STUB.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Spell_Ward()

Cast_Spell_Ward()   // Spells130.c — the Spell Ward cast handler
    |-> AITP_SpellWard_Wrapper__STUB()   // reorders args (city_idx, magic_realm, player_idx)
        |-> AITP_Spell_Ward()

---

# `AITP_Spell_Ward` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Spell_Ward` | [AISPELL.c:3820-3921](../../MoM/src/AISPELL.c#L3820-L3921) | AI target picker for **Spell Ward**: of the caster's own cities, pick the **highest AI-valued** one lacking the ward matching the **human player's** realm (primary first, then secondary), falling back to a random realm 10% of the time. Returns the city (`*targeted_city_idx`) **and** which realm to ward (`*magic_realm`). |

> **Status: FAITHFUL to the asm (1:1), but DOES NOT COMPILE — one typo blocks it.** The body ([AISPELL.c:3820](../../MoM/src/AISPELL.c#L3820)) is a faithful reconstruction of `AITP_SpellWard__STUB.asm` — the structure, the primary→secondary realm chain, the ward slots, and the `Random(10)==1` / `Random(5)-1` fallback all match. **Blocker:** the value array is misspelled **`_ai_all_city_own_values`** (10×) — it must be **`_ai_all_own_city_values`** (the global the sibling pickers use; asm `AI_Own_City_Values`). That is C2065 (undeclared identifier) and fails the whole AISPELL.c TU. Fix the typo and it is 1:1. The asm is the authority.

## R1 (reconstruction error) — misspelled value array

`_ai_all_city_own_values` is undeclared — the real global is `_ai_all_own_city_values` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)), the same `AI_Own_City_Values` every other `AITP_*` city picker reads. The word order `city_own` ↔ `own_city` is transposed. It appears at [AISPELL.c:3838](../../MoM/src/AISPELL.c#L3838), 3843, 3849, 3855, 3861, 3867, 3873, 3879, 3885, 3891. Build error:

```
AISPELL.c(3838): error C2065: '_ai_all_city_own_values': undeclared identifier
```

Replace all 10 occurrences with `_ai_all_own_city_values`.

## Purpose

Spell Ward protects a city against a chosen magic realm. The AI casts it to blunt the **human** player's spells, so it scans its own cities for the most valuable one that lacks a ward against the human's realm — trying the human's **primary** realm first, then the **secondary**, and otherwise (10% chance) warding a random realm. Unlike the other city-enchantment pickers it returns **two** outputs: the city and the realm.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2410](../../MoM/src/AISPELL.c#L2410) | **Probe**: if a target exists, Spell Ward is given a fixed priority weight (`AI_OVL_SplPriorities[17] = 100`). |
| `AITP_SpellWard_Wrapper__STUB` | [AISPELL.c:5130-5133](../../MoM/src/AISPELL.c#L5130-L5133) | Cast-time adapter: takes `(city_idx, magic_realm, player_idx)` and calls `AITP_Spell_Ward(player_idx, city_idx, magic_realm)` — the arg reorder that lets the 3-output picker fit the dispatch convention. Invoked by `Cast_Spell_Ward` ([Spells130.c:1785](../../MoM/src/Spells130.c#L1785)). |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_SpellWard__STUB.asm`. Realm codes: `sbr_Nature=0, sbr_Sorcery=1, sbr_Chaos=2, sbr_Life=3, sbr_Death=4` ([MOM_DAT.h:732-736](../../MoX/src/MOM_DAT.h#L732-L736)). (Value array shown corrected.)

```c
best_city_idx = ST_UNDEFINED;                                        // asm  di = -1
highest_value = 0;                                                   // asm  Highest_Value = 0
human_player_primary_realm   = _players[HUMAN_PLAYER_IDX].Prim_Realm; // asm  _players[0].Prim_Realm
human_player_secondary_realm = _players[HUMAN_PLAYER_IDX].Sec_Realm;  // asm  _players[0].Sec_Realm

for(itr_cities = 0; itr_cities < _cities; itr_cities++)              // asm  si loop
{
    if(_CITIES[itr_cities].owner_idx == player_idx)                  // asm  owner_idx (cbw) == player_idx
    {
        if(_ai_all_own_city_values[itr_cities] > highest_value)      // asm  jg  (outer value gate, strictly greater)
        {
            if     (primary   == sbr_Nature  && enchantments[NATURE_WARD]  == ST_FALSE) { …; Target_Realm = sbr_Nature;  }
            else if(primary   == sbr_Chaos   && enchantments[CHAOS_WARD]   == ST_FALSE) { …; Target_Realm = sbr_Chaos;   }
            else if(primary   == sbr_Sorcery && enchantments[SORCERY_WARD] == ST_FALSE) { …; Target_Realm = sbr_Sorcery; }
            else if(primary   == sbr_Death   && enchantments[DEATH_WARD]   == ST_FALSE) { …; Target_Realm = sbr_Death;   }
            else if(primary   == sbr_Life    && enchantments[LIFE_WARD]    == ST_FALSE) { …; Target_Realm = sbr_Life;    }
            else if(secondary == sbr_Nature  && enchantments[NATURE_WARD]  == ST_FALSE) { …; Target_Realm = sbr_Nature;  }
            else if(secondary == sbr_Chaos   && enchantments[CHAOS_WARD]   == ST_FALSE) { …; Target_Realm = sbr_Chaos;   }
            else if(secondary == sbr_Sorcery && enchantments[SORCERY_WARD] == ST_FALSE) { …; Target_Realm = sbr_Sorcery; }
            else if(secondary == sbr_Death   && enchantments[DEATH_WARD]   == ST_FALSE) { …; Target_Realm = sbr_Death;   }
            else if(secondary == sbr_Life    && enchantments[LIFE_WARD]    == ST_FALSE) { …; Target_Realm = sbr_Life;    }
            else if(Random(10) == 1)                                                    { …; Target_Realm = Random(5)-1; }
            // each taken branch also sets best_city_idx = itr_cities and highest_value = value
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                 // asm  cmp di,-1 / xor ax,ax
*targeted_city_idx = best_city_idx;                                 // asm  [city_idx]   = di
*magic_realm       = Target_Realm;                                  // asm  [magic_realm] = Target_Realm
return ST_TRUE;                                                     // asm  ax = 1
```

**Faithful 1:1** (once the typo is fixed) — the **value gate is the outer test** here (asm `jg` at `loc_E7628`, before any realm check), the 5+5 realm chain is in the asm's order (Nature, Chaos, Sorcery, Death, Life × primary then secondary), the `Random(10)==1` fallback assigns `Random(5)-1` as the realm, and the dual-output return matches.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` (currently mis-typed) | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`si*2`). |
| `_players[HUMAN_PLAYER_IDX]` | `_players` (index 0) | `HUMAN_PLAYER_IDX = 0` ([MOM_DEF.h:392](../../MoX/src/MOM_DEF.h#L392)); the wards target the **human's** realms. |
| `enchantments[{NATURE,SORCERY,CHAOS,LIFE,DEATH}_WARD]` | `s_CITY.enchantments.*_WARD` | `0x09 / 0x0A / 0x0B / 0x0C / 0x0D` ([MOM_DEF.h:1398-1402](../../MoX/src/MOM_DEF.h#L1398-L1402)). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `Random(10)`, `Random(5)` | `call Random` (args 10, 5) | fallback chance + random realm `Random(5)-1` → `0..4`. |

## OG quirks

None original-game. Behavioral notes:

- **Value gate is outer**: a city is only considered when its value strictly exceeds the running best — *before* the ward chain. A more valuable city that already has the relevant ward (so the chain falls through) still updates nothing, but it does **not** block a later, less valuable city from being chosen — the `highest_value` only advances when a branch is taken. Faithful to the asm.
- **Targets the human's realms**, read from `_players[0]` — the AI specifically counters the human player.
- **Random fallback** (`Random(10)==1`): ~10% of eligible cities get a random-realm ward even when no realm condition matched.

## Sub-functions / external calls

- **`Random(n)`** — the game RNG, called for the fallback chance and the random realm.
- Reads `_CITIES[]`, `_ai_all_own_city_values[]`, `_players[]`, `_cities`; writes through `*targeted_city_idx` and `*magic_realm`.

## Related references

- `…\ovr156\AITP_SpellWard__STUB.asm` — IDA Pro 5.5 disassembly (the authority; OG filename keeps `__STUB`).
- `…\ovr156\AITP_SpellWard__STUB.c` — Piethawn IDA-C.
- `…\ovr156\AITP_SpellWard_Wrapper__STUB.asm` — the arg-reordering cast-time wrapper.
- [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md), [AISPELL-AITP_Dark_Rituals.md](AISPELL-AITP_Dark_Rituals.md) — sibling city pickers (single output; this one returns city + realm).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Spell Ward.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
