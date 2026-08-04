Combat-Battle_Unit_Process_Attack.md

BU_ProcessAttack__WIP()                ==>  Battle_Unit_Process_Attack()
BU_ApplyDamage__WIP__SEGRAX()          ==>  BU_ApplyDamage()
BU_ApplyDamage()                       ==>  Battle_Unit_Commit_Damage()
Apply_Battle_Unit_Damage_From_Spell()  ==>  Compute_Battle_Unit_Damage_From_Spell()
CMB_AttackRoll__SEGRAX()               ==>  Combat_Roll_Damage_Dealt()    ~ Combat_Attack_Roll()
CMB_DefenseRoll__SEGRAX()              ==>  Combat_Roll_Damage_Blocked()  ~ Combat_Defense_Roll()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\BU_ProcessAttack__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Battle_Unit_Attack_Immunities.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Battle_Unit_Attack_Magic_Realm.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Battle_Unit_Has_Ranged_Attack.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Range_To_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Combat_Resistance_Check.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Combat_Effective_Resistance.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Battle_Unit_Defense_Special.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\CMB_AttackRoll__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\CMB_DefenseRoll__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Battle_Unit_Is_Within_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Combat_Grid_Cell_Has_City_Wall.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr110\Battle_Unit_Heal.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Apply_Battle_Unit_Damage_From_Spell.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\BU_ApplyDamage__WIP__SEGRAX.asm

OON XREF:  Battle_Unit_Attack() |-> Battle_Unit_Attack_Target() |-> BU_ProcessAttack__WIP()

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Battle_Unit_Attack()
            |-> Battle_Unit_Attack_Target()
                |-> Battle_Unit_Process_Attack
                    |-> Battle_Unit_Attack_Immunities()
                    |-> Battle_Unit_Attack_Magic_Realm()
                    |-> Battle_Unit_Has_Ranged_Attack()
                    |-> Range_To_Battle_Unit()
                    |-> Compute_Battle_Unit_Damage_From_Spell()
                    |-> Combat_Resistance_Check()
                        |-> Combat_Effective_Resistance()
                    |-> Battle_Unit_Defense_Special()
                    |-> Battle_Unit_Is_Within_City()
                    |-> Combat_Grid_Cell_Has_City_Wall()
                    |-> Battle_Unit_Heal()
                    |-> Combat_Roll_Damage_Dealt()
                    |-> Combat_Roll_Damage_Blocked()
        |-> Move_Battle_Unit()

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open; no divergences. Thirteen functions compared, two deferred (2026-08-04).**

Twenty-four findings were raised and closed over the course of this review (R1-R24). Six of them changed observable combat maths — see the balance note under Build state. Not compiled: see Build state.

## Scope

Anchors are function **start** lines. `Combat.c` has shifted repeatedly through this review; start lines are the stable reference.

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Battle_Unit_Process_Attack` | [Combat.c:16259](../../MoM/src/Combat.c#L16259) | `ovr122/BU_ProcessAttack__WIP.asm` (1236 lines) | faithful (R1-R9 fixed) |
| `Battle_Unit_Attack_Immunities` | [Combat.c:15945](../../MoM/src/Combat.c#L15945) (`static`) | `ovr122/Battle_Unit_Attack_Immunities.asm` | faithful |
| `Battle_Unit_Attack_Magic_Realm` | [Combat.c:16885](../../MoM/src/Combat.c#L16885) | `ovr122/Battle_Unit_Attack_Magic_Realm.asm` + `_misc.asm` | faithful (R11 fixed) |
| `Battle_Unit_Has_Ranged_Attack` | [Combat.c:17807](../../MoM/src/Combat.c#L17807) | `ovr122/Battle_Unit_Has_Ranged_Attack.asm` | faithful (R10 fixed) |
| `Range_To_Battle_Unit` | [Combat.c:17579](../../MoM/src/Combat.c#L17579) | `ovr122/Range_To_Battle_Unit.asm` | faithful |
| `Combat_Resistance_Check` | [Combat.c:15846](../../MoM/src/Combat.c#L15846) | `ovr122/Combat_Resistance_Check.asm` | faithful |
| `Combat_Effective_Resistance` | [Combat.c:15866](../../MoM/src/Combat.c#L15866) | `ovr122/Combat_Effective_Resistance.asm` | faithful (R12-R15 fixed) |
| `Battle_Unit_Defense_Special` | [Combat.c:16719](../../MoM/src/Combat.c#L16719) | `ovr122/Battle_Unit_Defense_Special.asm` | faithful (R16, R17 fixed) |
| `Combat_Roll_Damage_Dealt` | [Combat.c:15805](../../MoM/src/Combat.c#L15805) | `ovr122/CMB_AttackRoll__SEGRAX.asm` | faithful |
| `Combat_Roll_Damage_Blocked` | [Combat.c:15826](../../MoM/src/Combat.c#L15826) | `ovr122/CMB_DefenseRoll__SEGRAX.asm` | faithful |
| `Battle_Unit_Heal` | [Combat.c:10667](../../MoM/src/Combat.c#L10667) | `ovr110/Battle_Unit_Heal.asm` | faithful (R18, R19 fixed) |
| `Compute_Battle_Unit_Damage_From_Spell` | [Combat.c:13455](../../MoM/src/Combat.c#L13455) | `ovr113/Apply_Battle_Unit_Damage_From_Spell.asm` | faithful (R21-R24 fixed) |
| `Battle_Unit_Commit_Damage` | [Combat.c:13620](../../MoM/src/Combat.c#L13620) | `ovr113/BU_ApplyDamage__WIP__SEGRAX.asm` | faithful (R20 fixed) |

**Deferred to the BU-init pass:** `BU_Init_Battle_Unit` and `BU_Apply_Battlefield_Effects__WIP`, both `ovr116`, both reached from `Battle_Unit_Heal`. `BU_Init_Battle_Unit`'s handling of `movement_points` is what makes the save/restore at [10733-10736](../../MoM/src/Combat.c#L10733-L10736) necessary, so it is worth taking early in that pass.

**Already reviewed elsewhere:** `Battle_Unit_Is_Within_City` and `Combat_Grid_Cell_Has_City_Wall` in [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md); DONE-DONE.

**Engine/platform, not compared:** `Random`, `abs`, `SPUSH@`, `Eliminated_Opponent`, `Update_Sees_Illusions`.

`asm:N` refers to the listing named in the row above.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared, and an emitted comparison is not treated as evidence of the expression that produced it — in particular, `if (A) X else Y` and `if (!A) Y else X` compile to the same bytes, so branch layout alone is never a finding.

**Return-value test.** A stray `xor ax, ax` before an early exit is not evidence of an `int` return. Before recording one, check whether any *other* exit path loads `ax`, and whether any caller reads the result. If neither holds it is a compiler artifact — see the retired entry under Divergences.

---

# Findings

None open.

---

# Divergences

None open.

**Retired: the `xor ax, ax` return-value question.** An earlier pass recorded `Battle_Unit_Process_Attack`'s `void` signature as a divergence because asm:108 sets `ax` before its early bail. It is a compiler artifact:

- **No other exit sets it.** asm:383, asm:556 and the loop fall-out at asm:1227 are bare jumps to `@@Done`, and `@@Done` (asm:1229) drops straight into `pop di` / `pop si` / `mov sp, bp` / `pop bp` / `retf` with no load of `ax`.
- **No caller reads it.** All ten call sites in `ovr122/Battle_Unit_Attack_Target.asm` (asm:85, 205, 270, 322, 431, 545, 659, 697, 785, 832) are followed by `add sp, 0Eh` then `xor _SI_itr_damage_types, _SI_itr_damage_types`.

The IDA-derived `ovr122/BU_ProcessAttack__WIP.c:10` also declares it `void`.

The same test clears `Move_Battle_Unit` — see [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md). **`Combat_Move_Path_Find` has not been re-checked under this test** and its doc still carries the claim.

---

# Battle_Unit_Process_Attack

## Frame map

`sub sp, 2Eh` (asm:34) = 46 bytes, plus two register locals. Parameters occupy `bp+6` through `bp+12h`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-2Eh` | `Save_Mod` | `resistance_modifier` |
| `bp-2Ch` (**dword**) | `defender_enchantments` | `defender_enchantments` (`uint32_t`) |
| `bp-28h` | `new_damage_array` (3 words) | `new_damage_array[NUM_DAMAGE_TYPES]` |
| `bp-22h` | `defender_front_figure_damage` | `defender_front_figure_damage` |
| `bp-20h` | `Target_Damage` | `accumulated_figure_damage` |
| `bp-1Eh` | `defender_toblock` | `defender_toblock` |
| `bp-1Ch` | `uu_var_1C` | `niu_var` |
| `bp-1Ah` | `attack_tohit` | `attack_tohit` |
| `bp-18h` | `attack_immunities` | `attack_immunities` |
| `bp-16h` | `Type_Specific_ATK_Flags` | `noninnate_attack_attributes` |
| `bp-14h` | `attack_attributes` | `attack_attributes` |
| `bp-12h` | `attack_strength` | `attack_strength` |
| `bp-10h` | `attack_magic_realm` | `attack_magic_realm` |
| `bp-0Eh` | `attack_type` | `attack_type` |
| `bp-0Ch` | `defense_special` | `defense_special` |
| `bp-0Ah` | `Range_Penalty` | `range_tohit_penalty` |
| `bp-8` | `Attack_Damage` | `remaining_attack_damage` |
| `bp-6` | `Blur_Loop_Var` | `blur_rolls` |
| `bp-4` | `Loop_Var_2` | `itr2` and `healing` |
| `bp-2` | `itr` | `itr` |
| `di` | `_DI_attacker_battle_unit_idx` | (the parameter, used directly) |
| `si` | `_SI_defender_battle_unit_idx` | (the parameter, used directly) |

`niu_var` is written twice (asm:83, asm:1200) and never read — a faithful write-only local.

**`itr` and `itr2` are not interchangeable.** The listing uses `itr` (`bp-2`) only for loops *outside* the per-figure loop and for that loop's own counter; all three loops *inside* the body use `Loop_Var_2` (`bp-4`) — the poison loop (asm:981, 1022, 1031), the `new_damage_array` sum (asm:1035, 1044, 1046), and the drain into `damage_types` (asm:1201, 1219, 1221). Collapsing them onto one variable makes the outer loop non-terminating for units with five or more figures.

`healing` is a second production name for the same slot, holding the Life Steal resistance result (asm:823-830).

## Faithful — verified, leave alone

- **Prologue order** — `front_figure_damage` as a byte with `cbw`, the three-way enchantment OR, then `niu_var`/`attack_strength`/`remaining_attack_damage` zeroed and `attack_tohit` seeded from `tohit` (asm:39-93).
- **Enchantment sources** — `_UNITS[].enchantments | battle_units[].enchantments | battle_units[].item_enchantments`, all 32-bit (asm:57-82). `UE_INVULNERABILITY` is `0x80000000`, tested in the **high** word (asm:951-954); `UE_INVISIBILITY` is `0x00008000`, tested in the **low** word (asm:323-324).
- **Output array cleared before the `figure_count` bail** (asm:94-109). The listing's bound is the named `e_NUM_DAMAGE_TYPES` (asm:104) where production writes a literal `3`; same value.
- **`attack_tohit` accumulates** — melee adds `melee_tohit - toblock` (asm:275-289); ranged adds `ranged_tohit` (asm:211). Both build on the base `tohit` rather than replacing it. The ranged branch writes no `attack_tohit` of its own (asm:136-182).
- **Ranged extras** — `srat_Lightning` adds `Att_ArmorPrc`, `srat_MultiGaze` adds `Att_DoomDmg`, in that order (asm:183-193).
- **Range penalty** — gated on `attack_mode == am_Ranged` and a non-zero `Battle_Unit_Has_Ranged_Attack`, skipped when `ranged_type / 10 == rag_Magic`, computed as `Range_To_Battle_Unit(attacker, defender) / 3` with the signed `cwd`/`idiv` idiom, clamped to 1 under `UA_LONGRANGE` when positive, then subtracted (asm:195-249). The `else` arm zeroes `attack_strength` (asm:250-251).
- **`Battle_Unit_Attack_Magic_Realm` argument order** — `(ranged_type, attacker)` in the ranged branch (asm:160-171), `(0, attacker)` in the melee branch (asm:298-303).
- **Invisibility to-hit penalty** — enchantment bit **or** `UA_INVISIBILITY`, and **attacker** not `USA_IMMUNITY_ILLUSION`, then `dec` (asm:321-342).
- **Counter suppression** — `Suppression / 2` via the signed `cwd`/`sub`/`sar` idiom (asm:344-356).
- **Flying-melee bail** — defender flying, `attack_mode == am_Melee`, attacker not flying, `Counter != ST_TRUE`, `ranged_type < srat_Thrown` (asm:358-383).
- **Immolation** — `Attribs_2 & Ab_Immolation` (asm:399); `USA_IMMOLATION` is `0x0008` ([MOM_DEF.h:689](../../MoX/src/MOM_DEF.h#L689)) and `Ab_Immolation` the same bit ([MOM_DAT.h:424](../../MoX/src/MOM_DAT.h#L424)). `Compute_Battle_Unit_Damage_From_Spell(spl_Fireball, defender, &new_damage_array[0], 4)` by push order (asm:401-408).
- **Stoning Gaze and Death Gaze** — both loop to `figure_cnt`, both pass `-abs(Spec_Att_Attrib)`, both take `> 0`; Stoning adds `hits` to index 2 (asm:463), Death to index 0 (asm:529). Test order is StoneGaze-then-MultiGaze, then MultiGaze-then-DeathGaze (asm:411-414, asm:477-480).
- **Black Sleep forces Doom damage** (asm:543-552).
- **`Battle_Unit_Defense_Special(defender, attack_type, immunities, attributes, magic_realm)`** by push order (asm:558-565).
- **City-wall defence bonus** — defender inside, attacker outside, battlefield walled, then `+3` on a wall cell or `+1` otherwise (asm:571-605). `Combat_Grid_Cell_Has_City_Wall(cgx, cgy)` by push order (asm:590-597).
- **Touch attacks** — Dispel Evil (`sbr_Life`, `-4` and a further `-5` for `UM_UNDEAD`, index 2), Stoning Touch (`sbr_Nature`, `-1` extra when non-innate, index 2), Death Touch (`sbr_Death`, `-3` extra, index 0), Life Steal (`sbr_Death`, no extra, index 1, plus `Battle_Unit_Heal(attacker, healing, 1)` under `SpFx != 0`), Destruction (`sbr_Chaos`, modifier 0, index 2) — asm:619-863. All five sit under the defender's `USA_IMMUNITY_MAGIC` gate (asm:615).
- **Doom-damage halving** — `noninnate_attack_attributes & Att_DoomDmg` **set** yields `attack_strength / 2` on the fallthrough; **clear** yields full `attack_strength` at `loc_A355E` (asm:865-877).
- **Attack roll accumulates** — `add [bp+Attack_Damage], ax` (asm:886), against the Doom arm's `mov` (asm:877).
- **Blur** — attacker-side and defender-side arms, each requiring the matching `combat_enchantments` counter `> 0` and the **defender** not `USA_IMMUNITY_ILLUSION`; then `Random(10) == 10` decrements inside a loop bounded by the live `remaining_attack_damage` (asm:887-942).
- **`Combat_Roll_Damage_Blocked(defense_special, defender_toblock)`** by push order, subtracted, then the Invulnerability `-2`, then clamp to 0 (asm:944-961).
- **Poison** — attacker's raw `attack_attributes & Att_Poison` (not the merged local), defender not `USA_IMMUNITY_POISON`, loop bounded by `Poison_Strength`, realm `-1` and modifier `0`, result taken `> 0`, `UA_CREATEUNDEAD` routing to index 1 else index 0 (asm:963-1032).
- **`accumulated_figure_damage` seeding** — zeroed, summed over the three `new_damage_array` entries, then `defender_front_figure_damage` added (asm:1034-1049).
- **Figure-consumption loop** — runs **while `hits <= (remaining_attack_damage + accumulated_figure_damage)`**; `jg` at asm:1170 is the exit (asm:1159-1171).
- **Both arms of the loop body** — the negative arm transfers `hits + remaining` and advances the accumulator (asm:1062-1101); the overkill arm transfers a full `hits`, then re-rolls defence unless `Att_DoomDmg`, applies the Invulnerability `-2`, and clamps (asm:1103-1158).
- **Post-loop tail** — remaining damage into index 1 or 0, `defender_front_figure_damage = accumulated + remaining`, both counters zeroed, then the three-element drain of `new_damage_array` into `damage_types` clearing as it goes (asm:1172-1222).
- **Outer loop** — `itr` from 0 while `< figure_count` (asm:1223-1228).

## OGBUGs — faithful, do not fix

- **`toblock` is double-counted.** The defender's block is subtracted from the attacker's to-hit *and* passed to `Combat_Roll_Damage_Blocked`, so it reduces damage twice. Melee only in the original — the ranged branch has no such subtraction. `Att_EldrWeap`'s `defender_toblock -= 1` runs after the to-hit expression, so it weakens only the defence roll.
- **Thrown and breath attacks never get the ranged to-hit bonus.** `am_ThrownOrBreath` is 1, so it takes the ranged branch (`> am_Melee`) but fails the `== am_Ranged` gate — asm:195-196 jumps clear of the whole block. Everything feeding `ranged_tohit` is lost: item to-hit, weapon quality, the missile bonus. These attacks roll on bare unit-type `To_Hit`.
- **Blur, twice over** — the illusion immunity is read off the **defender** in both arms when it should be the attacker (compare the invisibility block, which correctly uses the attacker); and the loop bound is the live counter, so each success both removes a point and shortens the run, hard-capping the reduction at 50%.
- **`Battle_Unit_Attack_Immunities`' weapon-immunity composite** compares a *group* (`ranged_type / 10`) against `srat_Thrown` (100) and can never be true — flagged in the source.
- **`Battle_Unit_Heal` lets Life Steal ignore Irreversible Damage.** The `damage_amount_healable` clamp only runs when `overheal_flag == 0`, so a Life Steal heal is not bounded by the reversible pool and can restore figures that `damage[2]` should keep dead.

**Not a bug, but inert:** the `ip_Power_Drain` item power maps to `Att_PwrDrain` and reaches `attack_attributes`, but nothing consumes it. Across the whole disassembly `Att_PwrDrain` appears exactly twice — the `or ax, Att_PwrDrain` in `ovr116/BU_Apply_Item_Attack_Specials.asm:129` and the `USW_AttackFlag` display row in `dseg/_misc.asm:1267`. No code reads it.

---

# Combat_Roll_Damage_Dealt

Faithful. One d10 per point of attack strength, counting hits.

```
asm:32  mov     ax, 8
asm:34  sub     ax, [bp+To_Hit]
asm:35  cmp     ax, [bp+Die_Roll]
asm:37  jle     short loc_A22EC        ; (8 - To_Hit) <= Die_Roll -> hit
asm:39  cmp     [bp+Die_Roll], 0Ah
asm:41  jnz     short loc_A22ED        ; natural 10 -> hit
```

Production expresses the same short-circuit in the same operand order. `Random(10)` (asm:22), loop bounded by `Attack_Strength` (asm:47), successes returned from `di` (asm:49).

# Combat_Roll_Damage_Blocked

Faithful. One d10 per point of defence, counting blocks.

```
asm:14  mov     ax, 10
asm:16  call    Random
asm:18  mov     dx, 8
asm:19  sub     dx, [bp+To_Block]
asm:20  cmp     ax, dx
asm:21  jl      short loc_A231D        ; roll < (8 - To_Block) -> miss
```

Loop bounded by `Defense` (asm:26). The asymmetry between the two is faithful: the attack roll grants a natural 10 unconditionally, the defence roll does not. The defence listing has no `sub sp` at all — every value lives in a register — while production declares three locals. That is storage, not computation.

# Combat_Resistance_Check

Faithful. `Combat_Effective_Resistance(battle_unit, magic_realm) + resistance_modifier` into `resistance` (asm:12-22), `Random(10)` into `chance` (asm:23-27), then `cmp chance, resistance` / `jle` returning 0, else `chance - resistance` (asm:28-36).

The struct is passed **by value** — `battle_unit = s_BATTLE_UNIT ptr 6`, so `resistance_modifier` sits at `bp+74h` and `magic_realm` at `bp+76h`, and the `SPUSH@` copy is unwound with `add sp, 70h` (asm:20).

The `fail` local is tagged `// DNE in Dasm` because the listing returns through `ax` without a stack slot. That is storage, not computation.

# Combat_Effective_Resistance

Faithful. Six adjustments in listing order:

| adjustment | condition | asm |
| --- | --- | --- |
| `+30` | `Hero_Slot > -1` and `HSA_CHARMED` | asm:31-67 |
| `+30` | `USA_IMMUNITY_MAGIC` and `magic_realm >= sbr_Nature` | asm:69-74 |
| `+30` | `UE_RIGHTEOUSNESS` and (`sbr_Chaos` or `sbr_Death`) | asm:76-87 |
| `+10` / `+3` | (`sbr_Chaos` or `sbr_Nature`), Elemental Armor else Resist Elements | asm:89-110 |
| `+3` | `UE_BLESS` and (`sbr_Chaos` or `sbr_Death`) | asm:112-124 |
| `+5` | `UE_RESIST_MAGIC` and `magic_realm >= sbr_Nature` | asm:126-134 |

Every constant's word half matches the listing's split masks — `UE_ELEMENTAL_ARMOR` `0x00000400` and `UE_RESIST_ELEMENTS` `0x00000200` in the low word; `UE_BLESS` `0x02000000`, `UE_RESIST_MAGIC` `0x00040000`, `UE_RIGHTEOUSNESS` `0x40000000` and `HSA_CHARMED` `0x10000000` in the high word — so the single `uint32_t` mask is correct throughout. `sbr_Nature` is 0 and `sbr_NONE` is -1 ([MOM_DAT.h:731-737](../../MoX/src/MOM_DAT.h#L731-L737)), so `>= sbr_Nature` excludes only `sbr_NONE` and `ST_UNDEFINED`.

# Battle_Unit_Attack_Immunities

Faithful. `attack_attributes & Att_Illusion` is tested before the melee/ranged split (asm:17-21). Melee arm (asm:118-139): `Weapon_Plus1 == 0` sets `USA_IMMUNITY_WEAPON`, then `melee_attack_attributes & Att_Illusion` sets `USA_IMMUNITY_ILLUSION`. Ranged arm (asm:26-115): ranged illusion, then `ranged_type / 10 == rag_Missile` → MISSILES, `ranged_type == srat_FireBreath` → FIRE, `ranged_type / 10 == rag_Magic` → MAGIC, then the composite weapon-immunity test.

Cosmetic only: one of the four illusion tests uses `!= 0` where the other three use `> 0`, against `jle` in all four listing sites. `Att_Illusion` is `0x0040`, so the results agree.

# Battle_Unit_Attack_Magic_Realm

Faithful. A 21-entry value table (`_misc.asm:4-9`) and jump table (`_misc.asm:10-30`) implement the switch:

| listing arm | attack types | realm |
| --- | --- | --- |
| `jt_atk_mr_00` | `rat_NONE` | race-derived |
| `01,02,03,04,05,15` | Rock, Cannon, Bow, Sling, Unknown, Thrown | `sbr_NONE` |
| `06,07,09,12,16,17,19` | Lightning, Fireball, Deathbolt, Drow, FireBreath, Lightning breath, MultiGaze | `sbr_Chaos` |
| `08` | Sorcery | `sbr_Sorcery` |
| `10,11,13,14,18` | Icebolt, Pr_Shaman, Sprite, Nat_Bolt, StoneGaze | `sbr_Nature` |
| `20` | DeathGaze | `sbr_Death` |

`jt_atk_mr_18` is `xor si, si` followed by `jmp short loc_A3BAD` (asm:97-99) — an explicit exit, no fall-through into the `sbr_Death` arm. No-match falls to `loc_A3BAD` returning the initial `e_ST_UNDEFINED` (asm:14, asm:25), matching production's absent `default:`.

# Battle_Unit_Has_Ranged_Attack

Faithful. `cmp [ranged_type], 0` / `jle` requires **`> 0`**, and `cmp [ranged_type], srat_Thrown` / `jge` requires `< 100` (asm:16-24). `rat_NONE` is 0 and `rat_UNDEF` is -1 ([UNITTYPE.h:227-228](../../MoM/src/UNITTYPE.h#L227-L228)) — the lower bound must exclude `rat_NONE`, as the note at [UNITTYPE.h:229](../../MoM/src/UNITTYPE.h#L229) records.

# Range_To_Battle_Unit

Faithful. `abs(BU_1.cgx - BU_2.cgx)` into `delta_x`, `abs(BU_1.cgy - BU_2.cgy)` into `delta_y`, then `cmp delta_x, delta_y` / `jle` returning `delta_y`, else `delta_x` (asm:16-62). Chebyshev distance.

# Battle_Unit_Defense_Special

Faithful. Fifteen steps in listing order: `type = 0`; the three-source enchantment OR; `effective_defense = defense`; the illusion-immunity XOR and early `return 0` (asm:63-80); Large Shield `+2` when not melee (asm:82-91); `Attribs_1 & attack_immunities` non-zero and not melee → `type = 2` (asm:93-103); `USA_IMMUNITY_WEAPON` → `type = 1` (asm:105-114); `USA_IMMUNITY_MAGIC` with `magic_realm > ST_UNDEFINED` and not melee → `type = 2` (asm:116-127); on Chaos or Death, `UE_BLESS` `+3` and the two-source Righteousness test → `type = 2` (asm:129-172); on Chaos or Nature and not melee, Elemental Armor `+10` else Resist Elements `+3` (asm:174-196); `Att_ArmorPrc` halves with the signed idiom (asm:198-204); `type == 1` floors at 10 (asm:206-210); `type == 2` returns 50 (asm:212-215); else returns `effective_defense`.

The Righteousness test reads the raw `_UNITS[]` and `battle_units[]` enchantment words separately (asm:152-166), **not** the merged local — do not "simplify" it. `USA_LARGE_SHIELD` in the listing and `UA_LARGESHIELD` in production are both `2`.

# Battle_Unit_Heal

Faithful. `damage_amount_healable = damage[1] + damage[0]` — the reversible pool, excluding `damage[2]` (asm:18-35); the `overheal_flag == 0` clamp (asm:37-43); the three-element copy, `damages[0] -= healing_amount` with spillover into `damages[1]` and both floored (asm:45-78); the write-back, which **stores** rather than accumulates (asm:87-96); the byte-wise `front_figure_damage -= healing_amount` (asm:101-115); the figure-restore loop, whose test carries **both** the `healing_balance < 0` and `figure_max > figure_cnt` conditions (asm:155-172); the `> 0` write-back; the `overheal_flag == 0` surplus discard (asm:184-189); `abs`; the `figure_max <= healing_balance` Extra_Hits block (asm:195-232); and the `movement_points_saved` save/restore bracketing `BU_Init_Battle_Unit` and `BU_Apply_Battlefield_Effects__WIP` (asm:234-268).

The save/restore is necessary because `BU_Init_Battle_Unit` rebuilds `movement_points` — see the note at [Combat.c:2679](../../MoM/src/Combat.c#L2679). The same idiom appears three more times in `Combat.c` under the name `temp_movement_points`.

# Compute_Battle_Unit_Damage_From_Spell

Faithful.

The `damage` accumulator is the part most likely to be "tidied" back into a bug. The listing keeps it as a running register (`_SI_damage`) across the whole per-attack body: the Doom arm **assigns** it (asm:214, a `mov`), the roll path **accumulates** into it (asm:223 `add`, asm:229 `sub`), and it is **cleared at the end of every pass** (asm:327 `xor`, between the `damage_total` accumulate at asm:326 and the `figures_lost` clear at asm:328). All three are load-bearing together — switching the roll path to an assignment without the per-pass clear, or vice versa, double-counts leftover damage on every multi-attack spell (`Att_AREAFLAG`, `Att_WarpLghtn`). The zero clamp at asm:238-240 lives **inside** the roll branch; the Doom arm jumps clear of it at asm:215. Production [13539-13553](../../MoM/src/Combat.c#L13539-L13553) and [13603-13605](../../MoM/src/Combat.c#L13603-L13605).

`attack_roll` and `defense_roll` ([13473-13474](../../MoM/src/Combat.c#L13473-L13474)) are tagged `// DNE in Dasm` — deliberate debug taps so the two roll results are inspectable at a breakpoint. They do not change the emitted shape, which is still `+=` / `-=` against the accumulator.

Both early exits (Righteousness against a Chaos or Death spell, then `USA_IMMUNITY_MAGIC`) are function-level returns (asm:77-107). Spell-record field widths all match: `attributes` 16-bit from `Params2_3`, `immunities` and `strength` zero-extended bytes, `magic_realm` sign-extended. `Att_EldrWeap` decrements `toblock`; the override is taken when `> 0`, else `spell->strength`. `Battle_Unit_Defense_Special(battle_unit_idx, rat_Nat_Bolt, …)` argument order confirmed by push order (asm:159-172). The `Att_AREAFLAG` / `Att_WarpLghtn` / else attack-count chain sets `Att_DMGLIMIT` on the first arm (asm:175-196). Black Sleep forces Doom (asm:198-207). Inside the loop: the `itr == 0` front-figure fold-in (asm:241-247), the negative-`front_figure_damage` absorption (asm:248-258), the `Att_DMGLIMIT` single-figure case (asm:260-274), the `while` overkill loop with its Doom-gated defence re-roll (asm:275-314), `damage_total += damage + figures_lost × hits` (asm:316-326), and the Warp Lightning decrement (asm:329-331).

The tail correctly re-reads `battle_units[].front_figure_damage` from the **field** rather than the local (asm:340-349), then floors `damage_total` at 0 and writes it to `damage_types[0]` (asm:350-356).

# Battle_Unit_Commit_Damage

Faithful.

Both guards are function-level returns — `total_damage <= 0` (asm:31-33) and `status != bus_Active` (asm:40-42), each jumping to `@@Done` at asm:347, which is the epilogue. The 200 clamp uses `<= 200` on the sum (asm:60-68). `figures_lost = total / hits` capped at `figure_cnt` (asm:108-138), subtracted (asm:140-154), and `front_figure_damage = total % hits` via the `push dx` / `pop ax` remainder (asm:155-174).

The death block (asm:176-346) zeroes `figure_cnt`, sets `_combat_winner` from `Eliminated_Opponent()`, marks summoned units `wp = 9`, then selects the death **kind** from whichever damage type dominates — `bus_Gone` for irreversible, `bus_Drained` for undeath (or `bus_Gone` when `wp == 9`), `bus_Dead` for regular — and calls `Update_Sees_Illusions()`.

`damage[3]` is `uint8_t` ([MOM_DAT.h:1967](../../MoX/src/MOM_DAT.h#L1967)), so production's comparisons are unsigned and match the listing's `jb`/`jbe` throughout, despite the 200 clamp exceeding `int8_t` range.

Cosmetic only: the summoned-creature test uses `== ST_TRUE` where asm:198 is `or ax, ax` / `jz` — "non-zero", not "equals 1".

---

## Build state

**Clean.** `cmake --build --preset MSVC-debug` run by the user on 2026-08-04, after the full rename pass and all twenty-four finding fixes. `cmake` was not on the PATH in the agent shell during the review, so this is the user's result, not an agent tool call.

```
cd /c/STU/devel/ReMoM && cmake --build --preset MSVC-debug 2>&1 | tail -200
```

**Balance note.** R5 changed the to-hit arithmetic: melee now rolls at `tohit + melee_tohit - toblock` and ranged at `tohit + ranged_tohit`, where the base `tohit` from the unit-type record was previously discarded. R12 and R13 restored two `+30` resistance bonuses that did nothing before. R11 stopped five Nature-realm attack types resolving as Death. R18 and R19 turned healing from damage-doubling into healing, and removed a hang. Combat outcomes will differ measurably from any pre-review baseline; that is the original behaviour, not a regression.

**Rename reach — settled.** `Battle_Unit_Commit_Damage`'s ~20 call sites across `Combat.c`, `Spells129.c`, `Spells131.c` and `Spells133.c` are all renamed, as are the two prose references in `CMBTAI.c:938` and `:958` that a compile would not have caught. The header prototypes carry `resolve_for_real`; [Combat.h:1847](../../MoM/src/Combat.h#L1847) still spells its array parameter `damage_array[]` against the definition's `damage_types[]`, which is advisory and cosmetic.

`doc/Combat/MoX-Combat-Attack.md` still quotes `CMB_AttackRoll__SEGRAX`, `CMB_DefenseRoll__SEGRAX` and `BU_ApplyDamage` in its pseudo-code (lines 27, 29, 30, 31, 80). That file documents the **disassembly**, not production, so keeping the listing names there is defensible — noted rather than flagged.

**Anchors.** Scope-table anchors were re-read against `Combat.c` on 2026-08-04 and are function start lines. `Combat.c` shifts under every review pass, so the inline `Combat.c#L…` references in the body sections below are approximate; the `asm:N` references are stable.

Production anchors in this document were re-read against `Combat.c` on 2026-08-04 and are current as of that pass.

## Related references

- [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) — deferred the two `ovr113` functions to this review; that deferral is now closed. DONE-DONE.
- [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) — `Battle_Unit_Is_Within_City`, `Combat_Grid_Cell_Has_City_Wall`; DONE-DONE.
- [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) — the caller chain into this function.
