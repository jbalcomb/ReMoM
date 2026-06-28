AISPELL-AITP_Consecration.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Consecration.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_Consecration.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Consecration()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Consecration:
                |-> AITP_Consecration()

---

# `AITP_Consecration` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Consecration` | [AISPELL.c:3925-3971](../../MoM/src/AISPELL.c#L3925-L3971) | AI target picker for **Consecration**: only if the **human player** uses a **Death or Chaos** realm (or a 10% random roll), pick the **highest AI-valued** owned city that does **not** already have Consecration. Returns `ST_TRUE` with `*targeted_city_idx` set, or `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3925](../../MoM/src/AISPELL.c#L3925)) is a direct, faithful reconstruction of `AITP_Consecration.asm`. It compiles and links as part of AISPELL.c / momlib. No original-game bugs. It is the city-enchantment max-by-value scan plus an **outer realm/random gate** wrapping the whole loop. The asm is the authority.

## Purpose

The `AITP_*` target picker for **Consecration**, a Life city enchantment that protects against Death/Chaos effects. The AI only bothers when the **human** player fields a Death or Chaos realm (the threats Consecration answers) — primary *or* secondary — or, failing that, a flat 10% of the time. When the gate passes, it scans owned cities for the highest `_ai_all_own_city_values[]` (asm `AI_Own_City_Values`) entry whose `enchantments[CONSECRATION]` slot is clear. Returns a **city index**.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2416](../../MoM/src/AISPELL.c#L2416) | **Probe**: if a target exists, Consecration is given a fixed priority weight (`AI_OVL_SplPriorities[18] = 100`). |
| `AITP_City_Enchantment` | [AISPELL.c:4147](../../MoM/src/AISPELL.c#L4147) | Cast-time lookup (`switch(spell_idx) case spl_Consecration`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_Consecration.asm`. Realm codes: `sbr_Nature=0, sbr_Sorcery=1, sbr_Chaos=2, sbr_Life=3, sbr_Death=4` ([MOM_DAT.h:732-736](../../MoX/src/MOM_DAT.h#L732-L736)).

```c
best_city_idx = ST_UNDEFINED;                                        // asm  di = -1
highest_value = 0;                                                   // asm  Highest_Value = 0
human_player_primary_realm   = _players[0].Prim_Realm;              // asm  _players[0].Prim_Realm
human_player_secondary_realm = _players[0].Sec_Realm;              // asm  _players[0].Sec_Realm

if(human_player_primary_realm   == sbr_Death                         // asm  cmp Primary,sbr_Death   / jz enter
|| human_player_secondary_realm == sbr_Death                         // asm  cmp Secondary,sbr_Death / jz enter
|| human_player_primary_realm   == sbr_Chaos                         // asm  cmp Primary,sbr_Chaos   / jz enter
|| human_player_secondary_realm == sbr_Chaos                         // asm  cmp Secondary,sbr_Chaos / jz enter
|| Random(10) == 1)                                                  // asm  Random(10); cmp 1; jnz skip-loop
{
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)          // asm  si loop
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)             // asm  owner_idx (cbw) == Player_Index
        {
            if(_ai_all_own_city_values[itr_cities] > highest_value)  // asm  jle skip  =>  strictly greater (value gate first)
            {
                if(_CITIES[itr_cities].enchantments[CONSECRATION] == ST_FALSE)  // asm  cmp …CONSECRATION,0 / jnz skip
                {
                    best_city_idx = itr_cities;                     // asm  di = si
                    highest_value = _ai_all_own_city_values[itr_cities];
                }
            }
        }
    }
}

if(best_city_idx == ST_UNDEFINED)  return ST_FALSE;                 // asm  cmp di,-1 / xor ax,ax
*targeted_city_idx = best_city_idx;  return ST_TRUE;                // asm  [City@] = di / ax = 1
```

**Faithful 1:1** — the four realm comparisons + `Random(10)==1` short-circuit gate, the loop, the **value-gate-before-enchantment-gate** inner order, the strict-greater compare, and both returns all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_city_values[]` | `AI_Own_City_Values` | `extern int16_t *` ([MOM_DAT.h:3039](../../MoX/src/MOM_DAT.h#L3039)); word-indexed (`si*2`). |
| `_CITIES[i].owner_idx` | `[es:bx+s_ITEM_POWER.item_types]` | IDA mislabels offset 0 of the city record (a union artifact); it **is** `owner_idx`. Production uses `owner_idx` — correct. |
| `_CITIES[i].enchantments[CONSECRATION]` | `s_CITY.enchantments.CONSECRATION` | per-city `uint8_t enchantments[NUM_CITY_ENCHANTMENTS]` ([MOM_DAT.h:1749](../../MoX/src/MOM_DAT.h#L1749)); `CONSECRATION = 0x16` ([MOM_DEF.h:1411](../../MoX/src/MOM_DEF.h#L1411)). |
| `_players[0]` | `_players` (index 0) | the **human** player (`HUMAN_PLAYER_IDX = 0`); the AI reacts to the human's realms. |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `Random(10)` | `call Random` (arg 10) | 10% fallback chance when the realm conditions don't apply. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Outer realm/random gate**: the entire scan is skipped unless the human runs Death/Chaos or the 10% roll hits — by design (Consecration counters Death/Chaos), not a bug.
- **Tie-break**: the `> highest_value` test is strictly-greater, so among qualifying cities of equal top value the **lowest city index** is kept.

## Sub-functions / external calls

- **`Random(n)`** — the game RNG, for the 10% fallback gate.
- Otherwise reads `_CITIES[]`, `_ai_all_own_city_values[]`, `_players[]`, and `_cities`, and writes through `*targeted_city_idx`.

## Related references

- `…\ovr156\AITP_Consecration.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_Consecration.c` — Piethawn IDA-C.
- [AISPELL-AITP_Spell_Ward.md](AISPELL-AITP_Spell_Ward.md) — sibling that also reacts to the human's realms (per-realm wards; returns city + realm).
- [AISPELL-AITP_Dark_Rituals.md](AISPELL-AITP_Dark_Rituals.md) — sibling with an extra gate (Temple) around the value scan.
- [AISPELL-AITP_Earth_Gate.md](AISPELL-AITP_Earth_Gate.md) — base-shape sibling city-enchantment picker.
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Consecration.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
