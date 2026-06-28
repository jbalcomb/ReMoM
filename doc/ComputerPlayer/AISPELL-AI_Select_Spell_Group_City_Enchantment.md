AISPELL-AI_Select_Spell_Group_City_Enchantment.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickCityBuff.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_OVL_PickCityBuff.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()
                |-> AI_Select_Spell_Group()
                |-> AI_Select_Spell_Group_City_Enchantment()
                    |-> AITP_* (one target-picker per city spell)

---

# `AI_Select_Spell_Group_City_Enchantment` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Select_Spell_Group_City_Enchantment` | [AISPELL.c:1668-1890](../../MoM/src/AISPELL.c#L1668-L1890) | When the category picker chose "city enchant" (category 3), decide *which* city/terrain spell the AI casts: weight each spell **only if both** it is known **and** its `AITP_*` target-picker finds a valid target, scale by realm suppression, then weighted-random pick and return its `spl_*` id (0 = none). |

> **Status: BUILDS CLEAN — faithful to the asm.** The body ([AISPELL.c:1668-1890](../../MoM/src/AISPELL.c#L1668-L1890)) is a Piethawn IDA-C paste, adapted to production. The gating logic, suppression scaling, and dispatch `switch` all match `AI_OVL_PickCityBuff.asm` — the switch reproduces the 20-entry jump table 1:1, including the slot-4 `return 0` gap (handled by the absent `case 4` → `default`). `players_spell_list` keeps the `- 1`. The 19 `AITP_*` target-pickers are stubbed, the 6 spell constants corrected to `*_Of_*`, the locals are `int16_t`, and `choice` rename applied. `cmake --build --preset MSVC-debug` compiles and links with no errors or warnings (momlib.lib, HeMoM.exe). The asm + the `_misc.asm` jump-table bytes are the authority.

## Purpose

Leaf picker for category 3 (city enchantment) of [`AI_Select_Spell_Group`](AISPELL-AI_Select_Spell_Group.md). It differs from the Summon and Unit-Enchant pickers in one key way: a spell only earns a weight if its dedicated **`AITP_*` (AI Target Picker)** returns `1` (a valid target exists). The target-pickers also write the chosen target back through `&Target_City` / `&Target_X/Y/Plane` / `&Target_Realm`, though those outputs are not consumed here — they are recomputed by the actual cast. After suppression scaling, `Get_Weighted_Choice` selects a slot and the `switch` maps it to a spell id.

### The priority array

`AI_OVL_SplPriorities` is byte-offset addressed in the asm (**C index = byte/2**). Slot 4 is never assigned (dead) — it maps to a `return 0` entry in the jump table.

| Slot | Spell | Weight | Target picker |
|---|---|---|---|
| 1 | Wall_Of_Stone | `_turn / 6` | `AITP_Wall_Of_Stone` |
| 2 | Transmute | 200 | `AITP_Transmute` |
| 3 | Change_Terrain | 50 | `AITP_Change_Terrain` |
| 4 | *(unused)* | — | — |
| 5 | Move_Fortress | `_turn / 15` | `AITP_Move_Fortress` |
| 6 | Earth_Gate | `_turn / 15` | `AITP_Earth_Gate` |
| 7 | Gaias_Blessing | 300 | `AITP_Gaias_Blessing` |
| 8 | Flying_Fortress | 100 | `AITP_Flying_Fortress` |
| 9 | Wall_Of_Fire | `_turn / 20` | `AITP_Wall_Of_Fire` |
| 10 | Heavenly_Light | `_turn / 25` | `AITP_Heavenly_Light` |
| 11 | Stream_Of_Life | `_turn / 10` | `AITP_Stream_Of_Life` |
| 12 | Inspirations | 300 | `AITP_Inspirations` |
| 13 | Prosperity | 300 | `AITP_Prosperity` |
| 14 | Astral_Gate | 0 | `AITP_Astral_Gate` |
| 15 | Dark_Rituals | 100 | `AITP_Dark_Rituals` |
| 16 | Cloud_Of_Shadow | `_turn / 20` | `AITP_Cloud_Of_Shadow` |
| 17 | Spell_Ward | 100 | `AITP_Spell_Ward` |
| 18 | Consecration | 100 | `AITP_Consecration` |
| 19 | Wall_Of_Darkness | `_turn / 20` | `AITP_WallofDarkness` |
| 20 | Altar_Of_Battle | 50 | `AITP_AltarofBattle` |

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) | Dispatched when `AI_Select_Spell_Group` returns category 3. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AI_OVL_PickCityBuff.asm`. `players_spell_list` is 1-based (`spells_list - 1`, asm 19) like the sibling pickers.

### Build target-gated weights ([1682-1798](../../MoM/src/AISPELL.c#L1682-L1798))

Clear slots 0..49, then per spell: `if (known && AITP_xxx(player_idx, &Target...) == 1) slot = weight;` (table above). Turn-scaled weights (`_turn / N`) favor the spell more as the game progresses; the rest are flat. Slot 4 is skipped (asm jumps `+6` → `+0Ah`). **Faithful** in structure — every gate matches the asm's known-check + `AITP_*` call + `cmp ax,1`.

### Realm-suppression scaling ([1800-1843](../../MoM/src/AISPELL.c#L1800-L1843))

- `SPL_IsLifeSupressed()`: slots 10–14, 18, 20 → `*2/3` (asm 341-380).
- `CRP_SPL_IsNatSuppressed()`: slots 1–7 → `*2/3` (asm 381-404).
- `SPL_IsDthSuppressed()`: slots 15–16, 19 → `<20 → 0`, `<50 → /3`, else `/2` (asm 405-489).
- `SPL_IsChsSuppressed()`: slot 9 → same banding (asm 490-529).

All index ranges, thresholds, and factors match the asm.

### Pick + dispatch ([1845-1888](../../MoM/src/AISPELL.c#L1845-L1888))

`choice = Get_Weighted_Choice(AI_OVL_SplPriorities, 50);` then `switch(choice)` returns the spell. Switching on `choice` directly, `case N` matches jump-table entry `bx = N-1` (`off_E695E`, in `_misc.asm`). The table's lone `return 0` entry — `bx` 3 (dead slot 4) — is reproduced by the **absence of `case 4`** falling to `default: return 0;`. **Verified entry-by-entry against the table bytes; faithful 1:1.**

## Build status

`cmake --build --preset MSVC-debug` builds clean (0 errors, 0 warnings; momlib.lib and HeMoM.exe link). This picker's blockers were all resolved: the 19 `AITP_*` target-pickers are stubbed, the 6 spell constants use the correct `*_Of_*` spelling (`spl_Wall_Of_Stone`, etc.), the locals are `int16_t`, and `Random_Result` was renamed `choice`.

The `AITP_*` are stubs that return 0, so the picker's per-spell gates currently never fire — the function is structurally complete and compiles, but won't pick a real city spell until the target-pickers (o156p17-35) and the `Pick_Target_For_*` dispatchers are reconstructed for real. Those are tracked separately.

## OG quirks (preserve when reconstructing)

- **Dead slot 4**: never assigned a weight; the jump table carries a `return 0` entry for it (reproduced by the missing `case 4`).
- **Astral_Gate weight is 0** (slot 14): assigned but always 0, so although it has a target-picker and a jump-table entry, it can never actually be chosen.
- Target outputs (`Target_City`, `Target_X/Y/Plane`, `Target_Realm`) are written by the pickers but **not read** by this function — it only uses the `== 1` "found a target" result.

## Sub-functions / external calls

- **`Get_Weighted_Choice(int16_t *, int16_t)`** ([random.h:31](../../MoX/src/random.h#L31)) — weighted pick over the 50-slot array.
- **`AITP_*` (19 target-pickers)** — one per city spell; return `1` if a valid target exists and write it back through the pointer args. Only `AITP_Change_Terrain` and `AITP_Spell_Ward` are reconstructed so far.
- **`SPL_IsLifeSupressed` / `CRP_SPL_IsNatSuppressed` / `SPL_IsDthSuppressed` / `SPL_IsChsSuppressed`** ([AISPELL.c:1894+](../../MoM/src/AISPELL.c#L1894)) — realm-suppression predicates (implemented).
- **Globals:** `AI_OVL_SplPriorities[50]`, `_players[].spells_list`, `_turn`.

## Related references

- `…\ovr156\AI_OVL_PickCityBuff.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\_misc.asm` — contains the `off_E695E` jump-table bytes that pin the `bx → spell` map.
- `…\ovr156\AI_OVL_PickCityBuff.c` — Piethawn IDA-C (what was pasted).
- [AISPELL-AI_Select_Spell_Group.md](AISPELL-AI_Select_Spell_Group.md) — the category picker that dispatches here.
- [AISPELL-AI_Select_Spell_Group_Summon.md](AISPELL-AI_Select_Spell_Group_Summon.md) / [AISPELL-AI_Select_Spell_Group_Unit_Enchantment.md](AISPELL-AI_Select_Spell_Group_Unit_Enchantment.md) — sibling leaf pickers.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
