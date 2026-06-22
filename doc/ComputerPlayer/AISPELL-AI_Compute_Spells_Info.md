AISPELL-AI_Compute_Spells_Info.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Compute_Spells_Info.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Compute_Spells_Info.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
            |-> AI_Spell_Select()
                |-> AI_Compute_Spells_Info()

---

# `AI_Compute_Spells_Info` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AI_Compute_Spells_Info` | [AISPELL.c:415-479](../../MoM/src/AISPELL.c#L415-L479) | Scans the player's known-spells inventory and marks, in `g_ai_spell_group_flags[]`, which overland-relevant AI spell-groups the player can cast — filtering out 21 combat-only / special-utility groups. |

Faithful to the disassembly `AI_Compute_Spells_Info.asm` in loop bounds, indexing, and the 21-way group filter (set and order match the asm `cmp`/`jz` chain). One structural difference remains — the known-spell test is written as an early-`continue` rather than the asm's branch-into-block shape; see [Structural note](#structural-note-vs-the-asm). Builds clean under `MSVC-debug`.

## Purpose

Called once per AI overland-spell decision. It rebuilds three globals from `_players[player_idx].spells_list[]`:

| Global | Allocation | Role | Consequential? |
|---|---|---|---|
| `g_ai_spell_group_flags[]` | `Near_Allocate_Next(92)` — [AIDATA.c:743](../../MoM/src/AIDATA.c#L743) | byte-per-group flag: `ST_TRUE` if the player knows a non-filtered spell in that AI-group | **Yes** — read by `AI_Select_Spell_Group` |
| `g_niu_ai_spell_info_list[]` | `Near_Allocate_Next(50)` — [AIDATA.c:744](../../MoM/src/AIDATA.c#L744) | packed list of accepted `spell_idx` bytes | No — "filled out but never used" (OG-dead, preserved; `niu` in the name) |
| `g_niu_ai_spell_info_count` | — [MOM_DAT.c:1058](../../MoX/src/MOM_DAT.c#L1058) | count of accepted spells | No — "calculated but never used" (OG-dead, preserved; `niu` in the name) |

Only `g_ai_spell_group_flags[]` is actually consumed downstream (`AI_Select_Spell_Group`, [AISPELL.c:382](../../MoM/src/AISPELL.c#L382), currently a `return 0` stub). The list/count pair is dead output that the OG still maintains, so production preserves it — the `g_niu_` prefix records that it is "not in use."

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `AI_Spell_Select` | [AISPELL.c](../../MoM/src/AISPELL.c) (per [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md)) | Once per AI overland-cast decision, before the picker consults `g_ai_spell_group_flags[]`. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); cross-checked against `AI_Compute_Spells_Info.asm` (the authority). Constants: `NUM_MAGIC_REALMS = 6` ([MOM_DEF.h:61](../../MoX/src/MOM_DEF.h#L61)), `NUM_SPELLS_PER_MAGIC_REALM = 40` ([MOM_DEF.h:121](../../MoX/src/MOM_DEF.h#L121)) → 240 spells. AI-group constants are `enum e_AI_SPELL_GROUP` ([AISPELL.h:19-99](../../MoM/src/AISPELL.h#L19-L99)); `SGRP_SoM = 90` is the maximum group.

### Clear the category table ([423-428](../../MoM/src/AISPELL.c#L423-L428))

`g_niu_ai_spell_info_count = 0;` then a loop zeroing `g_ai_spell_group_flags[itr]` for `itr = 0 .. SGRP_SoM` inclusive — 91 entries, covering every group `0 .. 90`. This matches the asm's `cmp [bp+itr], 90 / jle` (the named `SGRP_SoM` reads clearer than the asm literal `90`). The backing store is 92 bytes ([AIDATA.c:743](../../MoM/src/AIDATA.c#L743)); index 91 is one past the max group and, as in the asm, is intentionally left untouched. **Faithful.**

### Realm / spell double loop ([430-439](../../MoM/src/AISPELL.c#L430-L439))

`for itr_realms 0..5`, `for itr_spells 0..39`. The known-check indexes `spells_list` with the **0-based** flat index `(itr_realms * 40) + itr_spells` and skips the slot unless it is `sls_Known`. The asm builds the same 0-based index for the `spells_list` lookup. **Faithful** in indexing (see the [structural note](#structural-note-vs-the-asm) on the early-`continue`).

### Compute `spell_idx` ([441](../../MoM/src/AISPELL.c#L441))

`spell_idx = ((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells + 1);` — **1-based**, matching the asm's `inc ax`. The `+1` is required because `spell_data_table[]` is 1-based: `spl_NONE = 0` ([Spellbook.h:26](../../MoM/src/Spellbook.h#L26)) is a dummy entry and the first real spell `spl_Earth_To_Mud = 1` lives at `spell_data_table[1]`, while `spells_list[]` is 0-based. **Faithful.**

### Fetch and filter the AI-group ([443-466](../../MoM/src/AISPELL.c#L443-L466))

`ai_spell_group = spell_data_table[spell_idx].AI_Group;` then 21 `if(... ) continue;` guards. The 21 values, in this order, map exactly to the asm's `cmp`/`jz` chain (same set, same order):

| # | Production | asm | meaning |
|---|---|---|---|
| 1 | `SGRP_CombatHarm_1` (5) | `SGRP_CombatHarm_1` | combat damage |
| 2 | `SGRP_CombatCurse` (24) | `SGRP_CombatCurse` | |
| 3 | `SGRP_CombatHarm_2` (47) | `SGRP_CombatHarm_2` | |
| 4 | `SGRP_CombatHarm_3` (70) | `SGRP_CombatHarm_3` | |
| 5 | `SGRP_Disrupt` (12) | `SGRP_Disrupt` | |
| 6 | `SGRP_CombatMisc` (31) | `SGRP_CombatMisc` | |
| 7 | `SGRP_Haste` (55) | `SGRP_Haste` | |
| 8 | `SGRP_AnimateDead` (77) | `SGRP_AnimateDead` | |
| 9 | `SGRP_CombatSummon_1` (16) | `SGRP_CombatSummon_1` | |
| 10 | `SGRP_CombatSummon_2` (35) | `SGRP_CombatSummon_2` | |
| 11 | `SGRP_CombatSummon_3` (59) | `SGRP_CombatSummon_3` | |
| 12 | `81` (literal) | `81` (literal) | `SGRP_IDK` — combat-summon-4? |
| 13 | `SGRP_CE_1` (17) | `SGRP_CE_1` | combat enchantment |
| 14 | `SGRP_CE_2` (36) | `SGRP_CE_2` | |
| 15 | `SGRP_CE_3` (60) | `SGRP_CE_3` | |
| 16 | `SGRP_CE_4` (82) | `SGRP_CE_4` | |
| 17 | `SGRP_Dispel` (1) | `SGRP_Dispel` | Dispel Magic |
| 18 | `SGRP_DispelTrue` (20) | `SGRP_DispelTrue` | |
| 19 | `43` (literal) | `43` (literal) | unnamed group |
| 20 | `SGRP_SpellLock` (19) | `SGRP_SpellLock` | |
| 21 | `SGRP_Scouting` (0) | `or cx,cx / jz` | scouting / NONE |

`81` and `43` are kept as integer literals because the asm uses literals there too (no clean named constant); every other guard uses the matching `SGRP_*` name. The 21st guard renders the asm's `or cx,cx; jz` (test-for-zero) as `if(ai_spell_group == SGRP_Scouting)` (`SGRP_Scouting == 0`). **Filter set and order: faithful.**

### Accept the spell ([468-473](../../MoM/src/AISPELL.c#L468-L473))

For a surviving group: `g_niu_ai_spell_info_list[g_niu_ai_spell_info_count] = (unsigned char)spell_idx;` (dead output), `g_ai_spell_group_flags[ai_spell_group] = ST_TRUE;` (the real output), `g_niu_ai_spell_info_count++;`. The asm stores the low byte of `spell_idx` into the list, sets the flags byte to `e_ST_TRUE`, and increments the count. **Faithful.**

## Structural note (vs the asm)

The known-spell test is written as an early-exit:

```c
if(_players[player_idx].spells_list[...] != sls_Known) { continue; }
```

The asm tests *for* equality and branches into the accept path (`cmp …, sls_Known / jz process / jmp skip`), i.e. the branch-into-block shape `if(... == sls_Known) { … }`. The early-`continue` is functionally identical and the jump targets line up, but it is a restructure rather than a 1:1 mirror of the asm's branch. Recorded here as the one place the C shape differs from the disassembly; the behaviour is unchanged. (The 21 filter `continue`s, by contrast, each correspond directly to a `jz` to the loop tail.)

## Sub-functions / external calls

- **`spell_data_table[]`** ([MOM_DAT.h:3054](../../MoX/src/MOM_DAT.h#L3054)) — 1-based spell records; `.AI_Group` read here.
- **`_players[].spells_list[]`** — per-player spell-state flags (`sls_Known`), 0-based.
- **`g_ai_spell_group_flags[]` / `g_niu_ai_spell_info_list[]` / `g_niu_ai_spell_info_count`** — the three outputs (only the flags array is consumed); allocated in [AIDATA.c:743-744](../../MoM/src/AIDATA.c#L743-L744), declared in [MOM_DAT.c:1056-1066](../../MoX/src/MOM_DAT.c#L1056-L1066).
- **`AI_Select_Spell_Group`** ([AISPELL.c:382](../../MoM/src/AISPELL.c#L382)) — the downstream consumer of `g_ai_spell_group_flags[]` (currently a `return 0` stub).

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AI_Compute_Spells_Info.asm` — IDA Pro 5.5 disassembly (the authority; the `inc ax`, `jle 90`, and `cmp`/`jz` chain pin the indexing, loop bound, and filter set).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the caller; documents how `g_ai_spell_group_flags[]` feeds the picker.
- [MoM-AI-Data.md](../MoM-AI-Data.md) / [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md) — globals layout and the note that the list/count pair is defunct.
- `AISPELL.h` — `enum e_AI_SPELL_GROUP` (`SGRP_*`, 0..90).
- `MOM_DEF.h` — `NUM_MAGIC_REALMS` (6), `NUM_SPELLS_PER_MAGIC_REALM` (40).
