AISPELL-AITP_Move_Fortress.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_MoveFortress.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_MoveFortress.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()   // probe: does a target exist?
                    |-> AITP_Move_Fortress()

Cast_Spell_Overland()
    // friendly-city target lookup at cast time
    |-> AITP_City_Enchantment()
        switch(spell_idx)
            case spl_Move_Fortress:
                |-> AITP_Move_Fortress()

---

# `AITP_Move_Fortress` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Move_Fortress` | [AISPELL.c:3079-3129](../../MoM/src/AISPELL.c#L3079-L3129) | AI target picker for **Move Fortress**: find the player's own city with the **strongest garrison** that is **stronger than the current fortress city's** garrison, and return it as the new fortress site. Returns `ST_TRUE` with `*city_idx` set, or `ST_FALSE` if no city beats the fortress. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:3079](../../MoM/src/AISPELL.c#L3079)) is a direct, faithful reconstruction of `AITP_MoveFortress.asm` (the earlier `return 0;` stub is gone); the production symbol dropped the run-together spelling (`MoveFortress` → `Move_Fortress`), the asm/`.c` filename keeps `MoveFortress`. It compiles and links as part of AISPELL.c / momlib. No catchment scan and no OG quirks; one minor, inert deviation noted below. The asm is the authority.

## Purpose

The `AITP_*` target picker for **Move Fortress**, which relocates the wizard's fortress to one of their other cities. The AI wants the fortress in its best-defended city, so it picks the owned city with the highest garrison strength — but only if that strength **exceeds** the fortress city's current garrison (otherwise the move isn't worth it and it returns "no target"). "Garrison strength" is the precomputed table `_ai_all_own_garrison_strengths[]` (asm `AI_Own_Garr_Strs`), keyed by city index. Returns a **city index**, like [`AITP_Wall_Of_Stone`](AISPELL-AITP_Wall_Of_Stone.md).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:2338](../../MoM/src/AISPELL.c#L2338) | **Probe**: if a target exists, Move Fortress is given a turn-scaled priority weight (`AI_OVL_SplPriorities[5] = _turn / 15`). |
| `AITP_City_Enchantment` | [AISPELL.c:3386](../../MoM/src/AISPELL.c#L3386) | Cast-time lookup (`switch(spell_idx) case spl_Move_Fortress`) — returns the chosen city index for the actual cast. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_MoveFortress.asm`.

```c
/* loop 1 — locate the player's fortress city */
for(itr_cities = 0; itr_cities < _cities; itr_cities++)              // asm  si loop 1
{
    if(_CITIES[itr_cities].wx == _FORTRESSES[player_idx].wx &&       // asm  city.wx == FORTRESSES[di].wx
       _CITIES[itr_cities].wy == _FORTRESSES[player_idx].wy &&       // asm  city.wy == FORTRESSES[di].wy
       _CITIES[itr_cities].wp == _FORTRESSES[player_idx].wp)         // asm  city.wp == FORTRESSES[di].wp
    {
        fortress_city = itr_cities;                                  // asm  Fortress_City = si
    }
}

target_city_idx   = ST_UNDEFINED;                                   // asm  Target_City = -1
garrison_strength = _ai_all_own_garrison_strengths[fortress_city];  // asm  Garrison_Strength = AI_Own_Garr_Strs[Fortress_City]

/* loop 2 — strongest owned garrison that beats the fortress */
for(itr_cities = 0; itr_cities < _cities; itr_cities++)             // asm  si loop 2
{
    if(itr_cities == fortress_city)            continue;            // asm  si==Fortress_City -> skip
    if(_CITIES[itr_cities].owner_idx == player_idx)                 // asm  owner_idx (cbw) == di
    {
        if(_ai_all_own_garrison_strengths[itr_cities] > garrison_strength)  // asm  jle skip  =>  strictly greater
        {
            target_city_idx   = itr_cities;                        // asm  Target_City = si
            garrison_strength = _ai_all_own_garrison_strengths[itr_cities];
        }
    }
}

if(target_city_idx == ST_UNDEFINED)  return ST_FALSE;              // asm  cmp Target_City,-1 / xor ax,ax
*city_idx = target_city_idx;  return ST_TRUE;                      // asm  [City@] = Target_City / ax = 1
```

**Faithful 1:1** — both loops, the 3-field fortress-coordinate match (no owner check in loop 1, matching the asm), the `garrison_strength` seed from the fortress city, the fortress-self skip, the owner gate, the strict-greater compare, and both returns all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_garrison_strengths[]` | `AI_Own_Garr_Strs` | `extern int16_t *` ([MOM_DAT.h:3035](../../MoX/src/MOM_DAT.h#L3035)); word-indexed (`si*2`). |
| `_FORTRESSES[player_idx]` | `_FORTRESSES + di*4` | `extern struct s_FORTRESS *` ([MOM_DAT.h:4089](../../MoX/src/MOM_DAT.h#L4089)); `sizeof(s_FORTRESS) = 4` (asm `shl dx, 2`) — [FORTRESS_RECORD_SIZE](../../MoX/src/MOM_DAT.h#L267). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 0x72 = 114`. |
| `.wx/.wy/.wp` | `al` byte loads | city/fortress coords are bytes; compared after promotion. |

## Deviation (minor, inert) — `fortress_city` initializer

Production declares `int16_t fortress_city = 0;`, but the asm gives `Fortress_City` **no initializer** — it is written only inside loop 1 when a city matches the fortress coordinates. If *no* city matched, the asm would read `AI_Own_Garr_Strs[garbage]` whereas the C reads index `0`. This never happens in practice: a wizard who can cast Move Fortress has a fortress, and the fortress always sits in one of their cities, so loop 1 always sets `fortress_city`. The `= 0` is a harmless defensive default, not a behavior change on any reachable path. (Listed for strict bookkeeping; not a reconstruction error to "fix back".)

## OG quirks

None. No catchment scan, no terrain test, no preserved original-game bugs.

- **Tie-break**: the `> garrison_strength` test is strictly-greater, so among owned cities of equal strongest garrison the **lowest city index** wins. A city must *strictly* beat the fortress's own garrison to be chosen.

## Sub-functions / external calls

- None. The function only reads `_CITIES[]`, `_FORTRESSES[]`, `_ai_all_own_garrison_strengths[]`, and `_cities`, and writes through `*city_idx`.

## Related references

- `…\ovr156\AITP_MoveFortress.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\AITP_MoveFortress.c` — Piethawn IDA-C.
- [AISPELL-AITP_Wall_Of_Stone.md](AISPELL-AITP_Wall_Of_Stone.md) — sibling `AITP_*` city target picker (also returns a city index; single-pass max scan).
- [AISPELL-AI_Select_Spell_Group_City_Enchantment.md](AISPELL-AI_Select_Spell_Group_City_Enchantment.md) — the leaf picker that probes this function and weights Move Fortress.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
