AISPELL-AITP_Unit_Enchantment.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_Pick_Target_For_Unit_Enchantment__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_Pick_Target_For_Unit_Enchantment__STUB.c

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

# `AITP_Unit_Enchantment` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Unit_Enchantment` | [AISPELL.c:4463-4517](../../MoM/src/AISPELL.c#L4463-L4517) | AI target picker for **unit enchantments**. Requires a friendly-unit target type, then dispatches on `spell_idx` (34 cases) to `Select_Unit_For_Enchantment` (the "pick best own unit" helper), passing the spell's 32-bit unit-enchantment mask plus five filter flags. Returns the helper's result; `ST_FALSE` on wrong target type or unknown spell. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:4463](../../MoM/src/AISPELL.c#L4463)) is a faithful reconstruction of `IDK_Pick_Target_For_Unit_Enchantment__STUB.asm` (production renamed `IDK_Pick_Target_For_Unit_Enchantment__STUB` → `AITP_Unit_Enchantment`; the asm/`.c` filename keeps the OG IDA name). It compiles and links as part of AISPELL.c / momlib. The `stt_Friendly_Unit` gate, the 34-case jump table (verified byte-for-byte), and every per-spell argument tuple match the asm. The asm is the authority.

## Purpose

The `AITP_*`-style picker for the **unit-enchantment** spell group (Resist Elements, Giant Strength, Flight, Invisibility, Bless, Lionheart, Black Channels, … 34 spells). The AI has already decided to cast a unit enchantment; this routine picks *which own unit* receives it. It forwards to `Select_Unit_For_Enchantment` (drake178 `Select_Unit_For_Enchantment`, IDA-C `AI_Pick_Target_Unit`), passing the spell's **`UE_*` enchantment mask** (so the helper skips units that already have the buff) plus boolean filters. The chosen unit index is returned through `*unit_idx`.

## How it's reached

| Caller | Site | `spell_idx` / target type | Notes |
|---|---|---|---|
| `Cast_Spell_Overland` | [OverSpel.c:897](../../MoM/src/OverSpel.c#L897) | dynamic, `stt_Friendly_Unit` | `scc_Unit_Enchantment_*` group dispatch (the general path). |
| `Cast_Chaos_Channels` | [Spells129.c:943](../../MoM/src/Spells129.c#L943) | `spl_Chaos_Channels`, `0` (=`stt_Friendly_Unit`) | in-switch case → mask `0`, `exclude_heroes=1`. |
| `Cast_Lycantrophy` | [Spells132.c:1300](../../MoM/src/Spells132.c#L1300) | `spl_Lycanthropy`, `stt_Friendly_Unit` | in-switch case → mask `0`, flags `1,0,1,0,0`. |
| `Cast_Word_Of_Recall` | [Spells130.c:1552](../../MoM/src/Spells130.c#L1552) | `spl_Word_Of_Recall`, `stt_Friendly_Unit` | **not** a switch case → `default` → `Cast_Spell_Target_Error`, returns 0 (caller-side mismatch, not this function's defect). |
| `Cast_Black_Wind` | [Spells130.c:198](../../MoM/src/Spells130.c#L198) | `spl_Black_Wind`, `stt_Enemy_Group` | target type `!= stt_Friendly_Unit` → early `Cast_Spell_Target_Error`, returns `ST_FALSE`. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `IDK_Pick_Target_For_Unit_Enchantment__STUB.asm`.

```c
if(spell_target_type != stt_Friendly_Unit)   // asm  cmp arg_0,0 / jz proceed   (stt_Friendly_Unit == 0)
{
    Cast_Spell_Target_Error(spell_idx);
    return ST_FALSE;                          // asm  loc_E83AD → xor ax,ax
}

switch(spell_idx)                             // asm  word_E83C0 (34 values) + off_E8404 (34 targets)
{
    case spl_Resist_Elements: return Select_Unit_For_Enchantment(player_idx, UE_RESIST_ELEMENTS, unit_idx, 0, 0, 0, 0, 0);  // loc_E80A7
    case spl_Giant_Strength:  return Select_Unit_For_Enchantment(player_idx, UE_GIANT_STRENGTH,  unit_idx, 1, 0, 0, 0, 0);  // loc_E8235
    /* …30 more spells, each: (UE_mask, then 5 flags)… */
    case spl_Blur:            return 0;                                                                   // loc_E816D (no picker call)
    default: Cast_Spell_Target_Error(spell_idx); return 0;                                                // loc_E83A3
}
```

**Faithful 1:1** — the `arg_0 == 0` (`stt_Friendly_Unit`) gate, the 34-entry jump table, the per-spell mask + 5-flag tuples, the `spl_Blur` early `return 0` (asm `loc_E816D`, the only case that does **not** call the picker), and the `default` error path all match.

### The merged 32-bit mask (why 8 args, not 9)

The OG pushes **9 words** but cleans **`add sp, 12h` = 18 bytes = 8 arguments**, because the unit-enchantment mask is a single **4-byte** value: the asm sets `ax` (high word) and `dx` (low word) and `push ax; push dx` lays down one 32-bit `long`. So the call shape is:

```
Select_Unit_For_Enchantment(player_idx, UE_mask /*int32*/, &unit_idx, exclude_heroes, include_only_heroes, exclude_low_type, enable_special_scoring, unknown_flag_5)
```

Production collapses the IDA-C's two 16-bit args (`block_mask_low`/`block_mask_high`) into one `UE_*` constant where `value = (block_high << 16) | block_low`. The `UE_*` constants live in [MOM_DEF.h:920-951](../../MoX/src/MOM_DEF.h#L920-L951) and are the same flags used by Combat.c / MainScr.c against the unit's 32-bit `enchantments` field.

### Jump table & per-spell tuples (verified against the asm)

`word_E83C0` (34 spell-id values) + `off_E8404` (34 targets), `WIZARDS.asm:375642-375675`; 34 values match `cx = 22h`. Every `loc_*` block was decoded (mask = `(ax<<16)|dx`; the five `push`es before `push si` = the flags) and matched to its production case. **All 34 masks and all 34 flag tuples match.** The non-zero flags:

| Spell | mask | flags `[excl_heroes, incl_only_heroes, excl_low, special_scoring, flag5]` | asm `loc` |
|---|---|---|---|
| Giant Strength | `UE_GIANT_STRENGTH` | `1,0,0,0,0` | `loc_E8235` |
| Eldritch Weapon | `UE_ELDRITCH_WEAPON` | `1,0,0,0,0` | `loc_E81EF` |
| Flame Blade | `UE_FLAME_BLADE` | `1,0,0,0,0` | `loc_E8372` |
| Holy Weapon | `UE_HOLY_WEAPON` | `1,0,0,0,0` | `loc_E821E` |
| Chaos Channels | `0` | `1,0,0,0,0` | `loc_E81E4` |
| Lycanthropy | `0` | `1,0,1,0,0` | `loc_E832C` |
| Spell Lock | `UE_SPELL_LOCK` | `0,1,0,0,0` | `loc_E8187` |
| Water Walking | `UE_WATER_WALKING` | `0,0,0,1,0` | `loc_E80E1` |
| Flight | `UE_FLIGHT` | `0,0,0,1,0` | `loc_E8170` |
| Planar Travel | `UE_PLANAR_TRAVEL` | `0,0,0,1,0` | `loc_E82D2` |

The other 23 picker spells use all-zero flags; `spl_Blur` makes no call.

## OG quirks (preserve when reconstructing)

- **`spl_Blur` returns 0 without calling the picker** (asm `loc_E816D` → `loc_E83B5: xor ax,ax`). The only in-table case with no `Select_Unit_For_Enchantment` call.
- The first parameter is the spell **target type**; the gate is "must be a friendly-unit target" (`stt_Friendly_Unit == 0`), matching the asm's literal `cmp arg_0, 0`.

## Sub-functions / external calls

- **`Select_Unit_For_Enchantment(player_idx, UE_mask, &unit_idx, exclude_heroes, include_only_heroes, exclude_low_type, enable_special_scoring, unknown_flag_5)`** — drake178 `sub_E8448` / IDA-C `AI_SelectBestTransportUnit`; the per-unit scorer that returns the chosen own unit. Defined at [AISPELL.c:4521](../../MoM/src/AISPELL.c#L4521); prototype at [AISPELL.h:237](../../MoM/src/AISPELL.h#L237) (separate review, o156p42).
- **`Cast_Spell_Target_Error(spell_idx)`** ([AISPELL.h:222](../../MoM/src/AISPELL.h#L222)).

## Related references

- `…\ovr156\IDK_Pick_Target_For_Unit_Enchantment__STUB.asm` — IDA Pro 5.5 disassembly (the authority; jump table `word_E83C0` / `off_E8404` at `WIZARDS.asm:375642-375675`).
- `…\ovr156\IDK_Pick_Target_For_Unit_Enchantment__STUB.c` — Piethawn IDA-C (note: keeps the two separate 16-bit mask args and the always-zero 9th `arg_10`; production merges the masks and drops the dead arg).
- [AISPELL-Select_Unit_For_Enchantment.md](AISPELL-Select_Unit_For_Enchantment.md) — `Select_Unit_For_Enchantment` (drake178 `sub_E8448`, o156p42), the unit scorer this dispatches to: selects the highest-value (`Upkeep`) eligible own unit in the AI's best non-garrison field stack, falling back to the Fortress stack.
- [AISPELL-AITP_City_Enchantment.md](AISPELL-AITP_City_Enchantment.md) — the city-enchantment sibling router.
- [AISPELL-Get_Map_Square_Target_For_Spell.md](AISPELL-Get_Map_Square_Target_For_Spell.md) — the map-square sibling router.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
