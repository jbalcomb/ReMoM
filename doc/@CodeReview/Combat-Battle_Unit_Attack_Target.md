Combat-Battle_Unit_Attack_Target.md

BU_AttackTarget__WIP()  ==>  Battle_Unit_Attack_Target()
BU_CauseFear()  ==>  BU_CauseFear__NOOP()
BU_CauseFear__NOOP()  ==> Apply_Fear_Attack()

Battle_Unit_Attack_Target()
    Can_Attack_Again    ==>  has_haste_and_shot
    Source_Unit_Damage  ==>  attacker_damage_taken
    Feared_Figures      ==>  feared_figure_count
    Figs                ==>  counter_attack_figures
    Target_Damage_Sum   ==>  defender_damage_total
    SpFx                ==>  resolve_for_real

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\BU_AttackTarget__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Check_Attack_Ranged.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\BU_CauseFear__NOOP.asm

OON XREF:  Battle_Unit_Attack() |-> Battle_Unit_Attack_Target() |-> Battle_Unit_Process_Attack()

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> BU_MeleeWallCheck()                                 sibling of Battle_Unit_Attack, not a callee
        |-> Battle_Unit_Attack()
            |-> Battle_Unit_Attack_Target()
                |-> Check_Attack_Ranged()
                |-> Battle_Unit_Process_Attack()            done-done
                |-> Check_Wall_Of_Fire_Attack()             done-done
                |-> Apply_Fear_Attack()
        |-> Move_Battle_Unit()                              done-done

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open; no divergences. Three functions compared (2026-08-04).**

Four findings were raised and closed (R1-R4). R1 restored roughly 120 asm lines of missing logic; R3 was writing past the end of a caller-supplied array. Not compiled — see Build state.

## Scope

Anchors are function **start** lines.

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Battle_Unit_Attack_Target` | [Combat.c:12276](../../MoM/src/Combat.c#L12276) | `ovr122/BU_AttackTarget__WIP.asm` (858 lines) | faithful (R1-R3 fixed) |
| `Check_Attack_Ranged` | [Combat.c:11442](../../MoM/src/Combat.c#L11442) | `ovr113/Check_Attack_Ranged.asm` | faithful (R4 fixed) |
| `Apply_Fear_Attack` | [Combat.c:13895](../../MoM/src/Combat.c#L13895) | `ovr122/BU_CauseFear__NOOP.asm` | faithful |

**Already reviewed elsewhere:** `Battle_Unit_Process_Attack` in [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md) and `Check_Wall_Of_Fire_Attack` in [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md); both DONE-DONE.

`BU_MeleeWallCheck` is not reached from this function. It is also **not** reached from `Battle_Unit_Attack`, as an earlier version of this note claimed — `ovr122/Battle_Unit_Attack.asm` contains no call to it. Its callers are `ovr091/Battle_Unit_Action.asm` and `ovr098/Assign_Mouse_Images.asm`, matching production at [Combat.c:3342](../../MoM/src/Combat.c#L3342) and [Combat.c:5328](../../MoM/src/Combat.c#L5328). It belongs to a `Battle_Unit_Action` review.

`asm:N` refers to the listing named in the row above.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared, and an emitted comparison is not treated as evidence of the expression that produced it — `if (A) X else Y` and `if (!A) Y else X` compile to the same bytes, so branch layout alone is never a finding.

**IDA labels are not evidence.** `BU_CauseFear__NOOP` labels its two parameters `BU_Index` (`bp+6`) and `Attacker` (`bp+8`), but the code treats `bp+6` as the fear source and `bp+8` as the target. Parameter roles were established from what the body does with each, not from the names.

---

# Findings

None open.

---

# Divergences

None open.

---

# Battle_Unit_Attack_Target

## Frame map

`sub sp, 12h` (asm:20) = 18 bytes, plus two register locals. Parameters occupy `bp+6` through `bp+10h`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-12h` | `Can_Attack_Again` | `has_haste_and_shot` |
| `bp-10h` | `Source_Unit_Damage` | `attacker_damage_taken` |
| `bp-0Eh` | `Feared_Figures` | `feared_figure_count` |
| `bp-0Ch` | `ranged_attack_check` | `ranged_attack_check` |
| `bp-0Ah` | `Figs` | `counter_attack_figures` |
| `bp-8` | `Target_Damage_Sum` | `defender_damage_total` |
| `bp-6` | `Damage_Array` (3 words) | `damage_types[NUM_DAMAGE_TYPES]` |
| `si` | `_SI_itr_damage_types` | `itr_damage_types` |
| `di` | `_DI_attacker_battle_unit_idx` | (the parameter, used directly) |

The local `Damage_Array` and the two caller arrays are distinct. This function passes `&damage_types[0]` down as `Battle_Unit_Process_Attack`'s out-parameter, then folds the result into `defender_damage_types` or `attacker_damage_types` depending on which side attacked.

**`resolve_for_real` is tested two different ways, deliberately.** `!= ST_FALSE` at [16088](../../MoM/src/Combat.c#L16088) (asm:457, `cmp [bp+SpFx], 0` / `jz`) but `== ST_TRUE` at [16107](../../MoM/src/Combat.c#L16107), [16147](../../MoM/src/Combat.c#L16147) and [16180](../../MoM/src/Combat.c#L16180) (asm:490, 632, 733, `cmp [bp+SpFx], 1` / `jnz`). The original really does use "non-zero" for the Wall of Fire gate and "exactly 1" for the three Cause Fear gates. Do not normalise them to a single boolean test.

## Faithful — verified, leave alone

- **Prologue** — `feared_figure_count = 0`, then the paired clear of both caller arrays in one loop, then `attacker_damage_taken = 0`, then `Check_Attack_Ranged(attacker, defender)` (asm:24-48). Production [16023-16031](../../MoM/src/Combat.c#L16023-L16031). The `Check_Attack_Ranged` call precedes the `ranged_attack_flag` split in both.
- **Thrown/breath arm** — gated on `ranged_type >= srat_Thrown` (asm:247-248), one `Battle_Unit_Process_Attack(…, am_ThrownOrBreath, …, 0, …)` plus accumulate, then a Haste repeat gated additionally on `ranged_type < srat_StoneGaze` (asm:294-302). Production [16037-16056](../../MoM/src/Combat.c#L16037-L16056).
- **Counter-gaze block** — gated on the defender **not** Black Slept and `ranged_type >= srat_StoneGaze` (asm:341-359); `defender_damage_total` summed over the three defender entries plus `front_figure_damage`; `counter_attack_figures = figure_cnt - (total / hits)` when positive, else `figure_cnt` (asm:361-417); the call with `Counter = 1` (asm:419-431); then **one** loop banking the result into `attacker_damage_types` and `attacker_damage_taken` together (asm:433-455). Production [16059-16085](../../MoM/src/Combat.c#L16059-L16085).
- **Wall of Fire** — `if (resolve_for_real != ST_FALSE) Check_Wall_Of_Fire_Attack(attacker)` (asm:457-461). Production [16088-16091](../../MoM/src/Combat.c#L16088-L16091). The listing reaches it through a thunk labelled `j_BU_WallofFire__NOOP`; the `proc` it lands on does real work.
- **Attacker-still-alive gate** — `status == bus_Active`, else jump to `@@Done` (asm:463-470). Production [16096](../../MoM/src/Combat.c#L16096).
- **First Strike arm** — `attack_attributes & Att_1stStrike` and the defender lacking `UA_NEGATEFIRSTSTRIKE` (asm:472-488); the fear call (asm:490-499); a loop adding `attacker_damage_types[itr] / hits` to `feared_figure_count` (asm:501-524); then the call sized `figure_cnt - feared_figure_count` and accumulate (asm:525-563). Production [16101-16123](../../MoM/src/Combat.c#L16101-L16123).
- **Defender counter-attack** — gated on the defender not Black Slept (asm:570-572); the `counter_attack_figures` computation; the fear subtraction (asm:632-641); the `> 0` guard (asm:643-645); the call with `Counter = 1` and accumulate into the attacker array; then a Haste repeat gated on the **defender's** `bue_Haste`, reusing the same figure count (asm:678-715). Production [16128-16165](../../MoM/src/Combat.c#L16128-L16165).
- **Non-first-strike arm** — the complementary gate (asm:717-731); the fear call; `attacker_damage_taken += attacker front_figure_damage` then `feared_figure_count += attacker_damage_taken / hits` (asm:744-764); the call and accumulate; then a Haste repeat on the **attacker's** `bue_Haste` reusing the same `feared_figure_count` (asm:805-850). Production [16174-16197](../../MoM/src/Combat.c#L16174-L16197).
- **Ranged arm** — the `ranged > 0` and `ranged_attack_check == 0` gate (asm:53-64); on failure the `-5` loop (asm:226-238, production [16219-16222](../../MoM/src/Combat.c#L16219-L16222)); on success the first call and accumulate (asm:66-103), then the Haste block (asm:104-223). Production [16215-16264](../../MoM/src/Combat.c#L16215-L16264).
- **Haste re-attack resource cost** — `has_haste_and_shot = ST_FALSE` on entry (asm:113); magic ranged attacks or a `USA_CASTER_20`/`USA_CASTER_40` unit pay **3 mana** and need `mana > 6` (asm:139-156); everything else pays **1 ammo** and needs `ammo > 1`, setting the flag *before* the decrement (asm:165-182); the second call runs only if the flag is set (asm:184-223). Production [16232-16262](../../MoM/src/Combat.c#L16232-L16262).

The `-5` loop and the Haste block are on **different** branches: `loc_A27D3` is reachable only from the two early jumps at asm:60 and asm:64, and `loc_A27D1` — the post-Haste label — jumps to `loc_A27E9`, clearing the `-5` loop entirely.

## OGBUGs — faithful, do not fix

- **The Haste re-attack tests the wrong unit's `ranged_type`.** asm:114 indexes `battle_units[]` with `_SI_itr_damage_types`, which is 3 at that point because the accumulate loop at asm:87-103 has just exited — so the `rag_Magic` test reads `battle_units[3].ranged_type` rather than the attacker's. Every other access in the block uses `_DI_attacker_battle_unit_idx`. Production [16236](../../MoM/src/Combat.c#L16236) reproduces it with a comment.
- **The feared-figure count is applied to the wrong side in two of the three fear sites.** All three call `Apply_Fear_Attack(attacker, defender)`, and the first parameter is the fear source, so the return is the count of **defender** figures that failed their save. The counter-attack site subtracts it from `counter_attack_figures`, the defender's count — correct (asm:641). The first-strike site (asm:499 → asm:540) and the non-first-strike site (asm:742 → asm:780) subtract it from the **attacker's** `figure_cnt`. Production [16107-16118](../../MoM/src/Combat.c#L16107-L16118) and [16180-16187](../../MoM/src/Combat.c#L16180-L16187), both annotated.
- **The attacker's damage-taken figure count is derived by dividing damage totals by `hits`** (asm:501-524, asm:752-764), which over-counts for gaze and effect damage that is not figure-based. Production [16113-16116](../../MoM/src/Combat.c#L16113-L16116) and [16185-16186](../../MoM/src/Combat.c#L16185-L16186).
- **The `-5` fallback** subtracts five from every damage type when a ranged attack is disallowed (asm:226-238, production [16219-16222](../../MoM/src/Combat.c#L16219-L16222)).
- **Wall of Darkness ignores innate Illusions Immunity** in `Check_Attack_Ranged` — the second gate tests only `attacker_enchantments & UE_TRUE_SIGHT` (asm:126-131), never `Attribs_1 & USA_IMMUNITY_ILLUSION`, which the *first* gate does test. Production [13745](../../MoM/src/Combat.c#L13745), annotated.

**A retired misdiagnosis.** An earlier note in the source claimed the Cause Fear calls had their parameters in the wrong order. They do not — `bp+6` is the fear source at all three sites and all three pass `(attacker, defender)`. The defect is where the return value lands, as described above. The listing's own `Attacker` label on `bp+8` is the likely source of the confusion.

---

# Check_Attack_Ranged

Faithful. Returns `{0: allow, 1: disallow by Invisibility, 2: disallow by Wall of Darkness}`.

- **Both enchantment locals are three-source 32-bit ORs** — `battle_units[].enchantments | battle_units[].item_enchantments | _UNITS[unit_idx].enchantments`, for the defender (asm:17-52) and the attacker (asm:53-88). Production [13720-13721](../../MoM/src/Combat.c#L13720-L13721).
- **First gate** — attacker True Sight **or** `USA_IMMUNITY_ILLUSION` leaves `result` at 0 (asm:89-105); otherwise defender `UE_INVISIBILITY` **or** `UA_INVISIBILITY` sets 1 (asm:106-121). Production [13722-13741](../../MoM/src/Combat.c#L13722-L13741).
- **Second gate** — `battlefield->wall_of_darkness == 1`, attacker lacking True Sight, defender inside the city (`== ST_TRUE`), attacker outside (`or ax, ax` / `jnz`) sets 2 (asm:122-142). Production [13742-13755](../../MoM/src/Combat.c#L13742-L13755).

Word halves match the listing's split masks: `UE_TRUE_SIGHT` is `0x00400000` and is tested in the high word (asm:92-93, asm:128-129); `UE_INVISIBILITY` is `0x00008000` and is tested in the low word (asm:109-110).

# Apply_Fear_Attack

Faithful.

The fear **source** is the first parameter — tested for `Ab_CauseFear` in `Attribs_2` (asm:20) and for `UE_Cloak_of_Fear` across three separate enchantment sources: the battle unit's own, its items, and the `_UNITS[]` record (asm:29, 40, 56), each masked in the low word. The **target** is the second parameter — skipped entirely if it has `USA_IMMUNITY_DEATH` (asm:66), otherwise one `Combat_Resistance_Check(battle_units[target], 0, sbr_Death)` per figure, counting results `> 0` (asm:70-102). The loop bound is the target's `figure_cnt`.

Production [17840-17866](../../MoM/src/Combat.c#L17840-L17866) matches throughout. `Ab_CauseFear` is `0x20` and `USA_CAUSEFEAR` is `0x0020` — same bit. `UE_CLOAK_OF_FEAR` is `0x00000008`, a low-word bit, matching the listing's masking.

---

## Build state

**Clean.** `cmake --build --preset MSVC-debug` run by the user on 2026-08-04, after all four finding fixes and the six-name local rename. `cmake` was not on the PATH in the agent shell during the review, so this is the user's result, not an agent tool call.

**Header prototypes match.** [Combat.h:1844](../../MoM/src/Combat.h#L1844) carries `defender_damage_types[]` / `attacker_damage_types[]` and `resolve_for_real`, matching the definition. [Combat.h:1847](../../MoM/src/Combat.h#L1847) carries `resolve_for_real` for `Battle_Unit_Process_Attack`; its array parameter is still spelled `damage_array[]` against the definition's `damage_types[]` — advisory only, and cosmetic.

**Balance note.** R1 restored the hasted ranged second shot along with its mana and ammo costs — ranged units with Haste now fire twice and spend the resource, where before they fired once and spent nothing. R2 flipped the counter-gaze from firing only against Black Slept defenders to only against awake ones.

**Anchors.** Scope-table anchors were re-read against `Combat.c` on 2026-08-04 and are function start lines. `Combat.c` shifts under every review pass, so the inline `Combat.c#L…` references in the body sections below are approximate; the `asm:N` references are stable.

## Related references

- [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md) — the callee this function drives, called at nine sites across its arms; DONE-DONE.
- [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) — `Check_Wall_Of_Fire_Attack`; DONE-DONE.
- [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) — the caller, and the home of `BU_MeleeWallCheck`.
