Combat-Strategic_Combat.md

SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  Combat-Strategic_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

Strategic_Combat__WIP()  ==>  Strategic_Combat()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr110\Strategic_Combat_Allocate.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr110\Strategic_Combat__STUB.asm

Combat__WIP()
    |-> Strategic_Combat__WIP()
        |-> Strategic_Combat_Allocate()
        ...
        |-> End_Of_Combat()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-11).** No findings open. Two functions, 2,136 asm lines, both walked against their listings.

`WIZARDS.EXE ovr110` — `Strategic_Combat` and its allocator. This is the auto-resolve path: no combat screen, no grid, no turns in the tactical sense. It loads both stacks as battle units, reduces each side to three long accumulators (health, melee threat, ranged threat), runs at most three abstract rounds, picks a winner, applies damage until the loser's damage budget is spent, and hands off to `End_Of_Combat` with message type 6 or 7.

Eight findings were raised and all eight are applied, along with five structural deviations. They clustered in the spell-rank and threat-scaling block: five were copy-paste slips between the attacker and defender halves, which are near-identical 80-line mirrors, and one turned a `+=` into a `=` across twenty accumulators.

## Renames

| dasm | production |
| --- | --- |
| `Strategic_Combat__STUB` | `Strategic_Combat` |
| `BU_Apply_Battlefield_Effects__WIP` | `Battle_Unit_Special_Stats` (COMBINIT.c; recorded in `Combat-Init_Prep_Etc.md`) |
| `Battle_Unit_Commit_Damage` | `Battle_Unit_Commit_Damage` |
| `Allocate_Reduced_Map__1` | `Allocate_Reduced_Map` |
| `CMB_IDK_4PR` | `_niu_battlefield_effect` |

Locals, in frame order. The declaration block still matches the frame slot for slot after the renames.

| dasm | production | what it is |
| --- | --- | --- |
| `Weights` | `target_weights[(4 * MAX_STACK)]` | per-unit draw weights for `Get_Weighted_Choice` |
| `var_66` / `var_64` | `defender_spell_attack` / `attacker_spell_attack` | this round's spell contribution to strength |
| `var_62` / `var_60` | `defender_spell_healing` / `attacker_spell_healing` | this round's healing, added to `hits_left` |
| `var_5E` / `var_5C` | `defender_spell_ranged_pct` / `attacker_spell_ranged_pct` | percentage of ranged strength folded into the attack term |
| `var_56` / `var_54` | `defender_healing_magic` / `attacker_healing_magic` | realm-weighted healing budget |
| `var_52` / `var_50` | `defender_ranged_magic` / `attacker_ranged_magic` | realm-weighted ranged budget |
| `var_4E` / `var_4C` | `defender_attack_magic` / `attacker_attack_magic` | realm-weighted attack budget |
| `HP__Thrown` | `unit_current_hits` / `special_ranged_attack_strength` | one slot, two uses |
| `var_44` | `winner_health_pct` | `100 * hits_left / hits_start` for the winner |
| `var_42` | `winner_hits_floor` | the countdown's stopping bound |
| `var_40` | `winner_hits_countdown` | starts at the winner's raw hits, `-= 3` per damage application |
| `IDK_damage_defender` / `_attacker` | `defender_raw_hits` / `attacker_raw_hits` | unweighted total hits, unlike the `Get_Effective_Hits` sums |
| `var_3A` / `var_36` | `damage_to_attacker` / `damage_to_defender` | named by recipient; each has one producer and three consumers |
| `IDK_health_*__2` | `*_hits_start` | snapshot taken before the rounds |
| `IDK_health_*` | `*_hits_left` | the live pool the rounds mutate |
| `IDK_ranged_threat_*` | `*_ranged_strength` | offensive pool, attrited with health |
| `IDK_melee_threat_*` | `*_melee_strength` | same, melee |
| `Dmg_Array@` | `damage_types[NUM_DAMAGE_TYPES]` | |
| `MsgType__BU_Index` | `repeat_target_idx` / `end_of_combat_message_type` | one slot; a target latch during the damage loop, a message type after it |

The `__2` suffix had it backwards — the un-suffixed pair was the changing value and the suffixed pair the original.

The frame's second parameter is `_combat_attacker_count`; production names it `troop_count` and keeps a separate local under the dasm's name. Neither `_combat_attacker_count` nor `_combat_defender_count` is a global, so nothing is shadowed, but the leading underscore reads as one.

## Scope

| function | production | listing | asm lines | coverage |
| --- | --- | --- | --- | --- |
| `Strategic_Combat` | [Combat.c:7699](../../MoM/src/Combat.c#L7699) | `ovr110/Strategic_Combat__STUB.asm` | 2046 | **full body walk — R1-R8** |
| `Strategic_Combat_Allocate` | [Combat.c:7681](../../MoM/src/Combat.c#L7681) | `ovr110/Strategic_Combat_Allocate.asm` | 90 | **full body walk — faithful, 3 deviations** |

`Get_Effective_Hits`, `Get_Effective_Melee_Strength`, `Get_Effective_Ranged_Strength` and `Battle_Unit_Heal` share `ovr110` and are called from here, but they are not in this review.

---

# Findings

None open.

---

# Deviations — structurally different, behaviour unchanged

## `Strategic_Combat_Allocate` — two deviations, both deliberate

The asm allocates six blocks in one chain (`asm:13`, then five `Allocate_Next_Block` calls at `asm:24`, `46`, `57`, `68`, `79`): `battle_units` (249 PR), `combat_enchantments` (3), `CMB_IDK_4PR` (4), `_battlefield_holybonus` (3), `_battlefield_resistall` (3), `_battlefield_leadership` (3). Production reproduces all six, in order, with the 30-entry clear of `combat_enchantments` between the second and third. The `battle_units` size is computed as `(4 * MAX_STACK) * sizeof(struct s_BATTLE_UNIT)` rather than the OG's literal 249 paragraphs — same 36 units (3984 B / 110.67 B per unit in the OG), corrected for this port's wider struct.

- **`battlefield` is allocated first, ahead of everything.** The asm does not allocate it here at all. This is a port necessity rather than a slip — see the OG behaviour below — and it is marked `/* HACK */` at [Combat.c:8199](../../MoM/src/Combat.c#L8199).
- **`battle_units` therefore uses `Allocate_Next_Block`** ([Combat.c:8200](../../MoM/src/Combat.c#L8200)) where the asm uses `Allocate_First_Block`. The arena is still reset at the head of the chain, just by a different block.

---

# OG behaviours preserved

- **`battlefield` is read during strategic combat without being allocated for it.** `Strategic_Combat_Allocate` never allocates it, yet `Strategic_Combat` calls `Calc_Battlefield_Bonuses` (`asm:211`), which dereferences `battlefield` at [Combat.c:13874](../../MoM/src/Combat.c#L13874) and five more sites. In the OG `battlefield` is a two-byte segment handle that still points at whatever was allocated last, so the reads land somewhere valid and merely produce stale bonuses. In this port it is a real pointer and the read faults, which is what the `/* HACK */` allocation exists to prevent. The in-code note at [Combat.c:13873](../../MoM/src/Combat.c#L13873) records the same conclusion: per the disassembly, `battlefield` is not allocated for strategic combat at all.
- **The winner-determination pair is duplicated.** `asm:1852-1889` and `asm:1891-1929` are the same block with attacker and defender swapped, reached from the melee threat test. The in-code `// BUGBUG: duplicated code?` at [Combat.c:8764](../../MoM/src/Combat.c#L8764) is an accurate observation about the original.
- **Two of the three abstract rounds are separate code.** The ranged phase (`loc_8950E`) and the melee phase (`loc_8980F`) are independent near-copies rather than one parameterised loop, and the melee phase reuses `var_50` where the ranged phase uses `var_54` (`asm:1607` versus `asm:1290`).

---

# Build state

All eight findings and all five structural deviations are applied, and every local carries a descriptive name (see the ledger above). The declaration block still maps onto the frame slot for slot.

`cmake --build --preset MSVC-debug --target ReMoM` is **clean** — `MOX.lib`, `Platform.lib`, `STU.lib`, `momlib.lib` and `ReMoM.exe` all build and link with no errors and no warnings.

The test suite has not been run. Every finding in this review changes strategic-combat arithmetic — R8 alone flipped twenty accumulators from a halving to a 50% increase — so all AI-vs-AI and AI-vs-neutral resolutions move. The combat goldens were already pending the `Generate_Combat_Map` and `End_Of_Combat` work; all three sets need regenerating as one batch.

## Not reviewed

Four functions share `ovr110` and are out of scope here: `Get_Effective_Hits`, `Get_Effective_Melee_Strength`, `Get_Effective_Ranged_Strength` and `Battle_Unit_Heal`. The first three are called from the threat-accumulation loop and produce the numbers every finding above operates on, so they are the natural next scope if the overlay is to be closed out.
