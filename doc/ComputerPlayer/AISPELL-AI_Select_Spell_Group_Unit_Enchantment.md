AISPELL-AI_Select_Spell_Group_Unit_Enchantment.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickUnitBuff.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickUnitBuff.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_Unit_Enchantment()

---

# `AI_Select_Spell_Group_Unit_Enchantment` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_Unit_Enchantment` | [AISPELL.c:1356-1647](../../MoM/src/AISPELL.c#L1356-L1647) | When the category picker chose "unit enchant" (category 2), decide *which* unit-buff spell the AI casts: build a flat weighted priority over 40 unit-enchant spells, scale by active realm-suppression globals, then weighted-random pick one and return its `spl_*` id (0 = none). |

> **Status: BUILDS CLEAN — faithful to the asm, carries preserved OG quirks.** The body ([AISPELL.c:1356-1647](../../MoM/src/AISPELL.c#L1356-L1647)) is a Piethawn IDA-C paste, adapted to the production codebase. The priority table, the four suppression-scaling blocks, and the dispatch `switch` are all faithful to `AI_OVL_PickUnitBuff.asm` — and the switch reproduces the 40-entry jump table 1:1, **including** the three `return 0` gaps (cases 7, 8, 22), unlike the [Summon picker](AISPELL-AI_Select_Spell_Group_Summon.md). `players_spell_list` keeps the `- 1`, and the `Globals[...]` accesses use the production array form. `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings. The asm + the `_misc.asm` jump-table bytes are the authority.

## Purpose

Leaf picker for category 2 (unit enchantment) of [`AI_Select_Spell_Group`](AISPELL-AI_Select_Spell_Group.md). Unlike the Summon picker, weights here are **flat constants** (no `casting_cost`, no squaring) — a hand-tuned desirability per unit-buff spell. After realm-suppression scaling it calls `Get_Weighted_Choice` over the 50-slot `AI_OVL_SplPriorities` array and maps the winning slot to a spell id.

### The priority array

`AI_OVL_SplPriorities` is addressed by **byte offset** in the asm; **C index = byte-offset / 2**. Slots 8 and 22 are never assigned (dead); slot 7 (Blur) is set to 0 *and* dispatches to `return 0`, so Blur can never actually be cast.

| Slot | Spell | Wt | Slot | Spell | Wt |
|---|---|---|---|---|---|
| 1 | Resist_Elements | 5 | 21 | Cloak_Of_Fear | 10 |
| 2 | Stone_Skin | 15 | 22 | *(unused)* | — |
| 3 | Elemental_Armor | 5 | 23 | Wraith_Form | 5 |
| 4 | Iron_Skin | 30 | 24 | Water_Walking | 30 |
| 5 | Regeneration | 10 | 25 | Path_Finding | 5 |
| 6 | Resist_Magic | 5 | 26 | Entangle | 0 |
| 7 | Blur | 0 | 27 | Flight | 25 |
| 8 | *(unused)* | — | 28 | Wind_Walking | 50 |
| 9 | Magic_Immunity | 20 | 29 | Endurance | 10 |
| 10 | Flame_Blade | 30 | 30 | Plane_Shift | 20¹ |
| 11 | Eldritch_Weapon | 15 | 31 | Planar_Travel | 5 |
| 12 | Immolation | 10 | 32 | Natures_Cures | 5 |
| 13 | Holy_Weapon | 10 | 33 | Enchant_Road | 10 |
| 14 | Bless | 5 | 34 | Spell_Lock | 10 |
| 15 | Holy_Armor | 10 | 35 | Invisibility | 40 |
| 16 | Heroism | 10 | 36 | Chaos_Channels | 25² |
| 17 | True_Sight | 5 | 37 | Lycanthropy | 25³ |
| 18 | Invulnerability | 20 | 38 | Black_Channels | 25 |
| 19 | Lionheart | 30 | 39 | Guardian_Wind | 10 |
| 20 | Righteousness | 5 | 40 | Giant_Strength | 10 |

¹ Plane_Shift only if no wizard has Planar Seal up. ² Chaos_Channels only if caster lacks Doom Mastery. ³ Lycanthropy only if caster lacks Zombie Mastery.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 2. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickUnitBuff.asm`. `players_spell_list` is 1-based (`spells_list - 1`, asm 14) like the sibling pickers.

### Build flat weights ([1365-1489](../../MoM/src/AISPELL.c#L1365-L1489))

Clear slots 0..49, then for each known unit-buff spell set its slot to a fixed weight (table above). Three gated entries match the asm:
- **Plane_Shift** (slot 30): `+20` only if `spl_Plane_Shift` known **and** none of players 0–3 has Planar Seal active (asm 135-145) — see [quirk](#og-quirks-preserve-when-reconstructing).
- **Chaos_Channels** (slot 36): `+25` only if known and the caster's Doom Mastery is inactive (asm 167-175).
- **Lycanthropy** (slot 37): `+25` only if known and the caster's Zombie Mastery is inactive (asm 177-185).

### Realm-suppression scaling ([1491-1557](../../MoM/src/AISPELL.c#L1491-L1557))

- `SPL_IsLifeSupressed()`: slots 13–20, 29, 30, 31 → `*2/3` (asm 199-239).
- `CRP_SPL_IsNatSuppressed()`: slots 1–5, 24, 25, 26, 32 → `*2/3`; slot 40 → `/2` (asm 240-292).
- `SPL_IsDthSuppressed()`: slots 21–23, 37, 38 → `<20 → 0`, `<50 → /3`, else `/2` (asm 293-412).
- `SPL_IsChsSuppressed()`: slots 10–12, 36 → same banding (asm 413-497).

All index ranges, thresholds, and factors match the asm.

### Pick + dispatch ([1559-1645](../../MoM/src/AISPELL.c#L1559-L1645))

`choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);` then `switch(choice)` returns the spell. Production switches on `choice` directly (slot index), so `case N` matches jump-table entry `bx = N-1` (`off_E59B8`, in `_misc.asm`). The table's three `return 0` entries — `bx` 6 (Blur), `bx` 7 (dead slot 8), `bx` 21 (dead slot 22) — are reproduced as `case 7: return 0;`, `case 8: return 0;`, `case 22: return 0;`. **Verified entry-by-entry against the table bytes; faithful 1:1**, plus `default: return 0` for `choice` 0 or > 40.

## Build status

`cmake --build --preset MSVC-debug` builds clean (0 errors, 0 warnings in this function; `momlib.lib` and `HeMoM.exe` link). The `Globals[...]` accesses use the production array form (`Globals[PLANAR_SEAL]` / `[DOOM_MASTERY]` / `[ZOMBIE_MASTERY]`) matching the sibling pickers and the `uint8_t Globals[NUM_OVERLAND_ENCHANTMENTS]` type ([MOM_DAT.h:1515](../../MoX/src/MOM_DAT.h#L1515)).

## OG quirks (preserve when reconstructing)

- **Planar Seal check covers only players 0–3** ([1451-1455](../../MoM/src/AISPELL.c#L1451-L1455)): the asm hard-codes four `cmp`s at offsets `0`, `4C8h`, `990h`, `0E58h` (players 0,1,2,3), not all `NUM_PLAYERS` (6). Plane_Shift's bonus ignores Planar Seal on players 4–5. Preserve.
- **Blur is unreachable** (slot 7): assigned weight 0, and its jump-table slot returns 0, so it can never be chosen or cast.
- **Dead slots 8 and 22**: never assigned; the jump table carries `return 0` entries for them.

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t *, int16_t)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted pick over the 50-slot array.
- **`SPL_IsLifeSupressed` / `CRP_SPL_IsNatSuppressed` / `SPL_IsDthSuppressed` / `SPL_IsChsSuppressed`** ([AISPELL.c:1673+](../../MoM/src/AISPELL.c#L1673)) — realm-suppression predicates (implemented).
- **Globals:** `AI_OVL_SplPriorities[50]`, `_players[]` (`spells_list`, `Globals[]`), `spell_data_table[]` (none read here — weights are constant).

## Related references

- `…\ovr156\AI_OVL_PickUnitBuff.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\_misc.asm` — contains the `off_E59B8` jump-table bytes that pin the `bx → spell` map.
- `…\ovr156\AI_OVL_PickUnitBuff.c` — Piethawn IDA-C (what was pasted; carries the `.Globals.NAME` member-access).
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Select_Spell_Group_Summon.md](AISPELL-AI_Select_Spell_Group_Summon.md) — sibling leaf picker (category 1).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
