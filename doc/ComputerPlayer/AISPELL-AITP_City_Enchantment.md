AISPELL-AITP_City_Enchantment.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\Pick_Target_For_City_Enchantment__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\Pick_Target_For_City_Enchantment__WIP.c

Cast_Spell_Overland()
    case scc_City_Enchantment_Positive:
        |-> AITP_City_Enchantment()
    case scc_City_Enchantment_Negative:
        |-> AITP_City_Enchantment()

---

# `AITP_City_Enchantment` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_City_Enchantment` | [AISPELL.c:4173-4382](../../MoM/src/AISPELL.c#L4173-L4382) | The **city-targeted spell router**. For `stt_Friendly_City` it dispatches to the matching own-city `AITP_*` picker (17 spells). For `stt_Enemy_City` it scans for the highest-value contacted, non-neutral, non-consecrated, non-warded enemy city that isn't already cursed with the same effect, and returns its index. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:4173](../../MoM/src/AISPELL.c#L4173)) is a faithful reconstruction of `Pick_Target_For_City_Enchantment__WIP.asm` (production renamed `Pick_Target_For_City_Enchantment__WIP` → `AITP_City_Enchantment`; the OG drake178 picker names `AITP_WallofStone` etc. become production `AITP_Wall_Of_Stone` etc.). It compiles and links as part of AISPELL.c / momlib. The friendly-city dispatch, the enemy-city scan gates, the ward logic, and both per-spell switch tables match the asm — verified against the byte-level jump tables. The asm is the authority.

## Purpose

`Cast_Spell_Overland` routes both city-enchantment categories here: `scc_City_Enchantment_Positive` (own-city buffs) and `scc_City_Enchantment_Negative` (enemy-city curses). The first argument (`spell_target_type`) selects which half runs:

- **`stt_Friendly_City`** — pure dispatch: a 17-way switch on `spell_idx` to the spell's own-city `AITP_*` picker, returning that picker's result.
- **`stt_Enemy_City`** — an inline scan: classify the curse as Chaos-warded (`consecration = ST_FALSE`) or Death-warded (`consecration = ST_TRUE`), then walk `_CITIES[]` for the highest-`_ai_all_enemy_city_values` city that is non-neutral, not Consecrated, not protected by the relevant ward, contacted, (for Evil Presence) owned by a Death caster, and not already carrying the same curse — returning that city index.

## How it's reached

| Caller | Site | `spell_target_type` | Notes |
|---|---|---|---|
| `Cast_Spell_Overland` | [OverSpel.c:1166](../../MoM/src/OverSpel.c#L1166) | `stt_Friendly_City` | `scc_City_Enchantment_Positive` (own-city buffs); dynamic `spell_idx`. |
| `Cast_Spell_Overland` | [OverSpel.c:1272](../../MoM/src/OverSpel.c#L1272) | `stt_Friendly_City` | second positive-enchantment site; dynamic `spell_idx`. |
| `Cast_*` (per-spell) | Spells129/130/132.c | varies | individual cast routines call it with a fixed `spell_idx` — e.g. [Cast_Wall_Of_Stone](../../MoM/src/Spells132.c#L1413) / [Cast_Move_Fortress](../../MoM/src/Spells132.c#L1522) (`stt_Friendly_City`), [Cast_Call_The_Void](../../MoM/src/Spells129.c#L499) / [Cast_Earthquake](../../MoM/src/Spells129.c#L1590) (`stt_Enemy_City`). |

## Friendly-city dispatch (verified against the byte-level jump table)

The asm switch uses value table `word_E7FED` (17 entries) + target table `off_E800F` (`WIZARDS.asm:375047-375068`). Decoded byte-for-byte:

| `spell_idx` | value | asm target | OG picker | Production case |
|---|---|---|---|---|
| `spl_Wall_Of_Darkness` | `0B2h` | `loc_E7D00` | `AITP_WallofDarkness` | [4189](../../MoM/src/AISPELL.c#L4189) |
| `spl_Wall_Of_Stone` | `3` | `loc_E7D0B` | `AITP_WallofStone` | [4190](../../MoM/src/AISPELL.c#L4190) |
| `spl_Move_Fortress` | `1Ch` | `loc_E7D13` | `AITP_MoveFortress` | [4191](../../MoM/src/AISPELL.c#L4191) |
| `spl_Earth_Gate` | `25h` | `loc_E7D1B` | `AITP_EarthGate` | [4192](../../MoM/src/AISPELL.c#L4192) |
| `spl_Flying_Fortress` | `4Dh` | `loc_E7D23` | `AITP_FlyingFortress` | [4193](../../MoM/src/AISPELL.c#L4193) |
| `spl_Wall_Of_Fire` | `57h` | `loc_E7D2B` | `AITP_WallofFire` | [4194](../../MoM/src/AISPELL.c#L4194) |
| `spl_Altar_Of_Battle` | `92h` | `loc_E7D3B` | `AITP_AltarofBattle` | [4195](../../MoM/src/AISPELL.c#L4195) |
| `spl_Heavenly_Light` | `8Bh` | `loc_E7D33` | `AITP_HeavenlyLight` | [4196](../../MoM/src/AISPELL.c#L4196) |
| `spl_Inspirations` | `98h` | `loc_E7D43` | `AITP_Inspirations` | [4197](../../MoM/src/AISPELL.c#L4197) |
| `spl_Stream_Of_Life` | `94h` | `loc_E7D4B` | `AITP_StreamofLife` | [4198](../../MoM/src/AISPELL.c#L4198) |
| `spl_Astral_Gate` | `99h` | `loc_E7D53` | `AITP_AstralGate` | [4199](../../MoM/src/AISPELL.c#L4199) |
| `spl_Prosperity` | `91h` | `loc_E7D5B` | `AITP_Prosperity` | [4200](../../MoM/src/AISPELL.c#L4200) |
| `spl_Consecration` | `9Bh` | `loc_E7D63` | `AITP_Consecration` | [4201](../../MoM/src/AISPELL.c#L4201) |
| `spl_Cloud_Of_Shadow` | `0B9h` | `loc_E7D6B` | `AITP_CloudofShadow` | [4202](../../MoM/src/AISPELL.c#L4202) |
| `spl_Summoning_Circle` | `0CBh` | `loc_E7D73` | `AITP_Summoning_Circle` | [4203](../../MoM/src/AISPELL.c#L4203) |
| `spl_Dark_Rituals` | `0A3h` | `loc_E7D7C` | `AITP_DarkRituals` | [4204](../../MoM/src/AISPELL.c#L4204) |
| `spl_Gaias_Blessing` | `1Dh` | `loc_E7D84` | `AITP_GaiasBlessing` | [4205](../../MoM/src/AISPELL.c#L4205) |
| *default* | — | `loc_E7D8D` | `Cast_Spell_Target_Error` | [4206](../../MoM/src/AISPELL.c#L4206) |

(Switch-case *order* is irrelevant in C — only set membership matters; all 17 cases are present.)

## Enemy-city scan

```c
// classify the curse's ward family                                     // asm loc_E7DA1..
if(spell_idx is Earthquake | Chaos_Rift | Call_The_Void)        consecration = ST_FALSE;   // Chaos-warded
else if(spell_idx is Evil_Presence | Cursed_Lands | Pestilence | Famine) consecration = ST_TRUE;  // Death-warded
else Cast_Spell_Target_Error(spell_idx);

enemy_city_idx = ST_UNDEFINED;  best_enemy_city_value = 0;
for(itr_cities = 0; itr_cities < _cities; itr_cities++)                  // asm loc_E7DF7 loop
{
    if(_ai_all_enemy_city_values[itr_cities] <= best_enemy_city_value) continue;   // asm jg
    if(_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX)                  continue;
    if(_CITIES[itr].enchantments[CONSECRATION] != 0)                 continue;     // consecrated → immune

    if(consecration == ST_FALSE) { if(_CITIES[itr].enchantments[CHAOS_WARD] != 0) continue; }   // asm loc_E7E3C
    else /* ST_TRUE */           { if(_CITIES[itr].enchantments[DEATH_WARD] != 0) continue; }   // asm loc_E7E60

    if(spell_idx == spl_Evil_Presence && _players[owner].spellranks[sbr_Death] == 0) continue;   // asm loc_E7E78 (+8)
    if(Test_Bit_Field(player_idx, &_CITIES[itr].contacts) == 0)       continue;     // must be contacted

    duped = ST_FALSE;
    switch(spell_idx) {                                                             // already-cursed check
        case spl_Earthquake: case spl_Call_The_Void:                  break;        // no per-city dup
        case spl_Chaos_Rift:    if(enchantments[CHAOS_RIFT])    duped = ST_TRUE; break;   // asm loc_E7EF1
        case spl_Evil_Presence: if(enchantments[EVIL_PRESENCE]) duped = ST_TRUE; break;   // asm loc_E7F0F
        case spl_Famine:        if(enchantments[FAMINE])        duped = ST_TRUE; break;   // asm loc_E7F63
        case spl_Cursed_Lands:  if(enchantments[CURSED_LANDS])  duped = ST_TRUE; break;   // asm loc_E7F2B
        case spl_Pestilence:    if(enchantments[PESTILENCE])    duped = ST_TRUE; break;   // asm loc_E7F47
    }
    if(duped == ST_FALSE) { best_enemy_city_value = …value; enemy_city_idx = itr; }       // asm loc_E7F7F
}
if(enemy_city_idx == ST_UNDEFINED) return ST_FALSE;
*city_idx = enemy_city_idx; return ST_TRUE;
```

**Faithful 1:1** — the ward gate at [4260-4272](../../MoM/src/AISPELL.c#L4260-L4272) (skip if the relevant ward is set: Chaos Ward for the Chaos curses `consecration == ST_FALSE`, Death Ward for the Death curses `consecration == ST_TRUE`), the Consecration immunity, the Evil-Presence Death-caster requirement, the contact requirement, and the dup-already-cursed switch all match the asm.

The dup `switch` is value table `word_E7FD1` (7 entries) + targets at `+0Eh` (`WIZARDS.asm:375038-375046`), decoded byte-for-byte:
`1Ah`Earthquake→`loc_E7EEE`(empty), `6Eh`Chaos_Rift→`loc_E7EF1`(CHAOS_RIFT), `77h`Call_The_Void→`loc_E7F0D`(empty), `0B7h`Evil_Presence→`loc_E7F0F`(EVIL_PRESENCE), `0BDh`Famine→`loc_E7F63`(FAMINE), `0BEh`Cursed_Lands→`loc_E7F2B`(CURSED_LANDS), `0C4h`Pestilence→`loc_E7F47`(PESTILENCE). Production's six non-empty dup cases ([4303-4341](../../MoM/src/AISPELL.c#L4303-L4341)) each read the matching enchantment field.

### Symbol / layout cross-checks (verified)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_enemy_city_values[]` | `AI_Enemy_City_Values` | `int16_t *` ([MOM_DAT.h:3037](../../MoX/src/MOM_DAT.h#L3037)). |
| `enchantments[CONSECRATION]` | `+CONSECRATION` | `0x16` ([MOM_DEF.h:1411](../../MoX/src/MOM_DEF.h#L1411)). |
| `enchantments[CHAOS_WARD]` / `[DEATH_WARD]` | `+CHAOS_WARD` / `+DEATH_WARD` | `0x0B` / `0x0D` ([MOM_DEF.h:1400](../../MoX/src/MOM_DEF.h#L1400), [1402](../../MoX/src/MOM_DEF.h#L1402)). |
| `enchantments[CHAOS_RIFT/EVIL_PRESENCE/CURSED_LANDS/PESTILENCE/FAMINE]` | same | `0x01` / `0x03` / `0x04` / `0x05` / `0x07` ([MOM_DEF.h:1390-1396](../../MoX/src/MOM_DEF.h#L1390-L1396)). |
| `spellranks[sbr_Death]` | `(_players.spellranks+8)` | `sbr_Death = 4`, offset `+8` ([MOM_DAT.h:736](../../MoX/src/MOM_DAT.h#L736), [1473](../../MoX/src/MOM_DAT.h#L1473)). |
| `NEUTRAL_PLAYER_IDX` | `5` | ([MOM_DEF.h:393](../../MoX/src/MOM_DEF.h#L393)). |
| `_CITIES[i]` stride | `imul size s_CITY` | `sizeof(s_CITY) = 114`. |

## OG quirks

- No preserved bugs. One naming note: the **`consecration` local is the OG's own name** but it does *not* track the Consecration enchantment — it is a Chaos-vs-Death ward-family flag (`ST_FALSE` = Chaos-warded curse, `ST_TRUE` = Death-warded curse). The actual `CONSECRATION` enchantment check (city immunity) is separate and applies to all seven enemy-city curses.
- The Consecration immunity check runs for **all** seven enemy-city curses, including the Chaos ones — faithful to the asm (the in-code `¿ BUGBUG Chaos Rift ?` note at [4229](../../MoM/src/AISPELL.c#L4229) is a production question, not a deviation).

## Sub-functions / external calls

- 17 friendly-city `AITP_*` pickers, each `(player_idx, city_idx)`.
- **`Test_Bit_Field(player_idx, &city.contacts)`**, **`Cast_Spell_Target_Error(spell_idx)`** ([AISPELL.h:222](../../MoM/src/AISPELL.h#L222)).
- Reads `_CITIES[]`, `_cities`, `_ai_all_enemy_city_values[]`, `_players[]`; writes `*city_idx`.

## Related references

- `…\ovr156\Pick_Target_For_City_Enchantment__WIP.asm` — IDA Pro 5.5 disassembly (the authority; jump tables `word_E7FED`/`off_E800F` and `word_E7FD1` at `WIZARDS.asm:375038-375068`).
- `…\ovr156\Pick_Target_For_City_Enchantment__WIP.c` — Piethawn IDA-C.
- [AISPELL-Get_Map_Square_Target_For_Spell.md](AISPELL-Get_Map_Square_Target_For_Spell.md) — the sibling map-square router.
- [AISPELL-AITP_Wall_Of_Darkness.md](AISPELL-AITP_Wall_Of_Darkness.md) — the Wall of Darkness picker (now dispatched from the friendly-city switch).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
