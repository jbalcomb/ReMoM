Combat-Spell_Cast_AI_Target.md

SEEALSO:  C:\STU\devel\ReMoM\doc\ComputerPlayer\Combat-AITP_Combat_Spell.md
SEEALSO:  Combat-Combat_Spellbook_Screen.md
SEEALSO:  Combat-Spell_Cast.md
SEEALSO:  Combat-AI_Turn.md
SEEALSO:  Combat-Spell_Cast_AI_Select.md

UU15_AITP_Disintegrate()  ==>  AITP_Disintegrate()
UU_AITP_WordofRecall()   ==>  AITP_Word_Of_Recall()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_Combat_Spell.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_DispelMagic.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_WarpCreature.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_WarpWood.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_Healing.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_HolyWord.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_DarknessLight.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\UU15_AITP_Disintegrate.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\AITP_EarthToMud.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\AITP_CracksCall.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\AITP_RecallHero.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\AITP_Disrupt.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\UU_AITP_WordofRecall.asm

Combat_Cast_Spell_With_Caster()
    |-> Combat_Cast_Spell()

Combat_Cast_Spell()
    |-> AI_Prepare_Combat_Realm_Threats()
    |-> AI_Select_Combat_Spell()
    |-> AITP_Combat_Spell()

AI_Score_Combat_Spell()
    |-> AITP_Combat_Spell()                                         /* the probe - see Combat-Spell_Cast_AI_Select.md */

NOT:
    'Combat Screen'
    'Combat Spellbook'
    'Combat Spellbook Screen'
    'Combat Spell Target Screen'                                    Combat-Combat_Spell_Target_Screen.md
    'Combat Spell Cast'  (non-AI; yay HP, nay CP, NP, NPC, Monster)
    'Combat Spell Cast - AI - Select'                               Combat-Spell_Cast_AI_Select.md
    'Cast Spell On Unit'                                            Combat-Combat_Cast_Apply_Spell_Effect.md
    'Combat Spell Dispell'                                          Combat-Combat_Spell_Dispel.md

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-25).** Thirteen slots across `ovr111` and `ovr112`, **3,771 asm lines - the whole cohort walked end to end, and all thirteen are faithful.** Five findings were raised and all five are fixed. Every slot in this cohort was homeless before this doc.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `AITP_Combat_Spell` | [Combat.c:9227](../../MoM/src/Combat.c#L9227) | `ovr111/AITP_Combat_Spell.asm` | 1,665 | **full body walk — faithful** |
| `AITP_DispelMagic` | [Combat.c:9826](../../MoM/src/Combat.c#L9826) | `ovr111/AITP_DispelMagic.asm` | 445 | **full body walk — faithful** |
| `AITP_EarthToMud` | [CMBMAGIC.c:1504](../../MoM/src/CMBMAGIC.c#L1504) | `ovr112/AITP_EarthToMud.asm` | 267 | **full body walk — faithful** |
| `AITP_CracksCall` | [CMBMAGIC.c:1661](../../MoM/src/CMBMAGIC.c#L1661) | `ovr112/AITP_CracksCall.asm` | 239 | **full body walk — faithful** |
| `AITP_WarpCreature` | [Combat.c:9729](../../MoM/src/Combat.c#L9729) | `ovr111/AITP_WarpCreature.asm` | 210 | **full body walk — faithful** |
| `AITP_RecallHero` | [CMBMAGIC.c:1805](../../MoM/src/CMBMAGIC.c#L1805) | `ovr112/AITP_RecallHero.asm` | 162 | **full body walk — faithful** |
| `AITP_WarpWood` | [Combat.c:9689](../../MoM/src/Combat.c#L9689) | `ovr111/AITP_WarpWood.asm` | 161 | **full body walk — faithful** |
| `AITP_Healing` | [Combat.c:9654](../../MoM/src/Combat.c#L9654) | `ovr111/AITP_Healing.asm` | 128 | **full body walk — faithful** |
| `AITP_HolyWord` | [Combat.c:10588](../../MoM/src/Combat.c#L10588) | `ovr111/AITP_HolyWord.asm` | 76 | **full body walk — faithful** |
| `AITP_Disrupt` | [CMBMAGIC.c:1611](../../MoM/src/CMBMAGIC.c#L1611) | `ovr112/AITP_Disrupt.asm` | 66 | **full body walk — faithful** |
| `AITP_DarknessLight` | [Combat.c:9625](../../MoM/src/Combat.c#L9625) | `ovr111/AITP_DarknessLight.asm` | 54 | **full body walk — faithful** |
| `AITP_Disintegrate` | [Combat.c:9417](../../MoM/src/Combat.c#L9417) | `ovr111/UU15_AITP_Disintegrate.asm` | 172 | **full body walk — faithful** |
| `AITP_Word_Of_Recall` | [CMBMAGIC.c:1759](../../MoM/src/CMBMAGIC.c#L1759) | `ovr112/UU_AITP_WordofRecall.asm` | 126 | **full body walk — faithful** |

## What belongs here, and why

This is the **AI's combat spell target choice**: given a spell the AI has already decided to cast, which battle unit or grid square does it hit. The boundary is picking the target - not deciding *which* spell (that is [Combat-Spell_Cast_AI_Select.md](Combat-Spell_Cast_AI_Select.md)), and not applying the effect once picked.

`AITP_Combat_Spell` is the dispatcher: it switches on `spell_data_table[spell_idx].type` and either scores every battle unit inline or delegates to one of the ten leaf targeters. Both entry points into it are already-reviewed callers - `Combat_Cast_Spell` for the real cast, and `AI_Score_Combat_Spell` as a *probe* during spell selection, which is why a spell that cannot find a target scores `-1` there.

**On "already reviewed":** the header block previously described the dispatcher that way. It is not. `doc/ComputerPlayer/Combat-AITP_Combat_Spell.md` is a behavioural walkthrough of it, and several review docs refer to it in passing - [Combat-Combat_Cast_Apply_Spell_Effect.md](Combat-Combat_Cast_Apply_Spell_Effect.md) explicitly says it "is already homed in Combat-Spell_Cast_AI_Target.md, and the rest belong with it" - but **no CodeReview doc has ever given it a fidelity verdict**, and it carries no scope-table row anywhere. This review is its first.

### What does *not* belong here

- **`Cast_Spell_On_Battle_Unit`** (1,806 asm), which shares `ovr111` with six of these. It applies an effect; it does not select. Adjudicated in [Combat-Combat_Cast_Apply_Spell_Effect.md](Combat-Combat_Cast_Apply_Spell_Effect.md), which also warns that its `switch(spell_data_table[spell_idx].type)` reads almost identically to the dispatcher's and the two are easy to confuse.
- **`Combat_Cast_Spell`**, **`Combat_Casting_Cost_Multiplier`**, **`Combat_Spellbook_Screen`**, **`Do_Legal_Spell_Check`** and **`Spell_Resistance_Modifier`**, the other `ovr112` residents - [Combat-Spell_Cast.md](Combat-Spell_Cast.md).
- **`Compute_Battle_Unit_Damage_From_Spell`**, called from the Direct Damage arm - [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md), which also records the `Apply_*` → `Compute_*` rename this cohort's listing still uses.
- **`Effective_Battle_Unit_Strength`**, the scoring primitive five arms call - [Combat-Spell_Cast_AI_Select.md](Combat-Spell_Cast_AI_Select.md).

## Findings

None outstanding. Five were raised during the walk and all five are fixed: an `else if` chain in the `scc_Special_Spell` arm where the listing has nine independent `if`s; two `!= ST_UNDEFINED` tests where the listing compares `> -1`; `niu_local_variable` used in four leaves for a local that is in fact the returned target index; four stack locals declared in reverse frame order in `AITP_Word_Of_Recall`; and a missing 16-bit truncation cast in that same function.

## Deviations

### D1 - `bu_ptr` caches a pointer the listing never forms

Every handler in the dispatcher reaches its unit through `bu_ptr = &battle_units[battle_unit_idx]` ([Combat.c:9255](../../MoM/src/Combat.c#L9255) and eleven more). The listing has no such pointer: it re-emits `mov ax, di` / `mov dx, 6Eh` / `imul dx` / `les bx, [battle_units]` / `add bx, ax` at **every single field access** - five instructions per read, dozens of times per handler. The reconstruction is far more readable and arithmetically identical.

Recorded rather than proposed for change, consistent with how the same pattern was handled in [Combat-AI_Turn.md](Combat-AI_Turn.md). The one thing worth knowing is the staleness risk it introduces that the original cannot have: `battle_units` is reseatable, so a cached `bu_ptr` held across a call is a hazard the index form does not carry. No handler in this function does that.

### D2 - two intermediates the listing does not store

[Combat.c:9282](../../MoM/src/Combat.c#L9282) assigns `unit_resistance = Combat_Effective_Resistance(...)` and then computes `damage_types[dt_Drain] = 13 - unit_resistance`; asm:203-205 computes `13 - ax` and stores straight into `damage_types+2`, never touching `unit_resistance`. Likewise [Combat.c:9578](../../MoM/src/Combat.c#L9578) and [Combat.c:9587](../../MoM/src/Combat.c#L9587) write `return ST_UNDEFINED` where the listing jumps to the shared `return Picked_Target` exit - equivalent only because `selected_target_idx` is still `ST_UNDEFINED` at both points.

## Verified faithful

### `AITP_Combat_Spell` ([Combat.c:9227](../../MoM/src/Combat.c#L9227), asm 1,665)

Faithful apart from R1 and R2.

**Frame - order matches.** `damage_types` at `bp-14h` (three `int16_t`), `Resist_Modifier` `-0Eh`, `enchantments` `-0Ch` (32-bit, spanning `-0Ch..-9`), `Unit_Resist` `-8`, `Picked_Target` `-6`, `Target_Value` `-4`, `Highest_Value` `-2`, with `sub sp, 14h` reconciling at 20 bytes. [Combat.c:9229-9235](../../MoM/src/Combat.c#L9229-L9235) declares them in that order.

**The dispatch is a bounded jump table over spell *type*, not spell id.** asm:28-36 loads `spell_data_table[spell_idx].type`, tests `cmp bx, 23` / `jbe`, and indexes `off_8B1F3`. Twenty-four slots: **fifteen carry a real handler across twelve distinct labels, nine point at the default**. [Combat.c:9242](../../MoM/src/Combat.c#L9242) guards with `> scc_Combat_Banish`, which is type 23.

**Coverage reconciles exactly.** Production has fifteen `case` labels, matching the fifteen non-default slots, and the three shared bodies are paired correctly: Direct Damage Fixed/Variable both at `loc_8A236`, Combat Destroy Unit/Combat Banish both at `loc_8A489`, Battlefield Spell/Combat Counter Magic both at `loc_8AFBF`. The production arms also appear in the listing's address order.

**The twelve handlers, all walked:**

- **Direct Damage** (asm 37-272) - the Star Fires race gate, the Psionic Blast illusion gate, the Life Drain death gate, the Righteousness/Chaos-or-Death gate, then either a manual `13 - Combat_Effective_Resistance(..., sbr_Death)` clamped at zero for Life Drain or `Compute_Battle_Unit_Damage_From_Spell(spell_idx, idx, &damage_types[0], 25)`, and the damaged-unit heuristic `(100 - figure_cnt * hits) + front_figure_damage` applied only when the damage total is positive.
- **Combat Destroy Unit / Combat Banish** (asm 273-475) - the Petrify/stoning-immunity pair, the Dispel Evil race gate, the Banish `race < rt_Arcane` gate, then `Spell_Resistance_Modifier` added to `Combat_Effective_Resistance`, a `>= 10` reject, and the scaled `(strength * (10 - resist) + 9) / 10` - except for Disintegrate, which takes raw strength.
- **Unit Enchantment** (asm 476-681) - the four already-has-it gates (Stone Skin/Iron Skin, Resist Elements/Elemental Armor, Resist Magic/Wraith Form, Flight/flying), then the Berserk block: no target, self-target, target beyond `movement_points`, or a hero, each rejecting.
- **Unit Enchantment Normal Only** (asm 682-813) - own units, non-fantastic, no overlapping enchantment, Heroism capped at `Level > 2`.
- **Resistable Spell** (asm 814-1046) - the realm-immunity pairs (Sorcery/illusion, Death/death), Creature Binding's fantastic-only gate, Shatter's `melee <= 1 && ranged <= 1` reject.
- **Mundane Curse** (asm 1047-1240) - the same shape with the guards in a **different order** and, notably, **no `Spell_Resistance_Modifier`**: asm:1211 stores the resistance and never adds to it, which [Combat.c:9455](../../MoM/src/Combat.c#L9455) reproduces.
- **Unresistable Spell** (asm 1241-1428) - a genuine `if/else` (asm:1285 jumps past the else block, unlike R1's arm): Web checks non-corporeal and existing `Web_HP`, everything else checks the effect mask and the two immunities. Web against a flier gets `+2000`.
- **Battlefield Spell / Combat Counter Magic** (asm 1429-1478) - the side-indexed `combat_enchantments[Param0 + side]` already-active check returning `-1` directly, then the True Light / Darkness / Holy Word delegations, else `99`.
- **Special Spell** (asm 1479-1561) - the nine-way delegation. See R1.
- **Dispels** (asm 1562-1569), **Disenchants** (asm 1651-1653) - one call and one constant.
- **Summoning** (asm 1570-1650) - the `_units == 1000` bail, the nine-own-units and thirty-six-total caps, then a do-while that rolls `14 - Random(3)` or `7 + Random(3)` for x and `8 + Random(3)` for y up to 200 times. Both exits - the negative-terrain break at asm:1644 and the 200-try exhaustion falling through asm:1646 - land on the same `Picked_Target = 99`, which [Combat.c:9607](../../MoM/src/Combat.c#L9607) reproduces by setting it unconditionally after the loop.

**Borland idioms reconstructed correctly throughout**: long-branch fixups where the surviving `jcc` reads un-inverted (asm:50-51, 294-295, 1262-1263 and many more); split 32-bit masks where the constant lives in one word only - `UE_RIGHTEOUSNESS` is high-word (`and dx, 0` / `and ax, UE_RIGHTEOUSNESS`), `UE_IRONSKIN`, `UE_ELEMENTALARMOR` and `UE_WRAITHFORM` are low-word; and the `Param0`/`Params2_3` pair read as one 32-bit value, which production writes as `spell_data_table[spell_idx].enchantments` - a documented union member overlaying exactly those bytes ([MOM_DAT.h:1060-1068](../../MoX/src/MOM_DAT.h#L1060-L1068)).

**One argument order worth stating**, because it is easy to get backwards: at asm:617-619 the Berserk range check pushes the target index then `di`, so the call is `Range_To_Battle_Unit(battle_unit_idx, target_battle_unit_idx)`; asm:622-631 then pushes the result, loads `movement_points` into AX, pops the range into DX and compares `dx, ax`. The `jg` therefore rejects when **range exceeds movement points**, which is what [Combat.c:9369](../../MoM/src/Combat.c#L9369) says.

### The ten leaf targeters (1,808 asm)

All ten walked end to end, all ten faithful. Frames and declaration order match in every one - **no reversed frames anywhere in this cohort**, in contrast to [Combat-AI_Turn.md](Combat-AI_Turn.md) where four of ten failed. `AITP_HolyWord` and `AITP_Disrupt` have no stack frame at all, matching production's all-register locals; `AITP_DispelMagic`, `AITP_WarpCreature` and `AITP_WarpWood` each split a 32-bit value across `Enchants_LO`/`Enchants_HO`, folded correctly into one `int32_t` declared first.

**`AITP_DispelMagic`** ([Combat.c:9826](../../MoM/src/Combat.c#L9826), 445) - the largest. Two scoring phases: own units carrying dispellable debuffs, then enemy units carrying enchantments worth stripping. **All 27 scoring constants match position for position**, including the single `sub si, 10` that makes Spell Lock the one *negative* term. Phase 2's ten bonuses run 20/30/10/10/10/20/10/20/-10/10 and its tail 25/30/30/30/40/70/70. Two OGBUGs are preserved and documented in place: the signed 32-bit `enchantments > 0` at [Combat.c:9892](../../MoM/src/Combat.c#L9892), which makes an Invulnerability-only unit read as carrying nothing, and the `own_spell_realms & 0` term at [Combat.c:9906](../../MoM/src/Combat.c#L9906) that can never pass. A non-visible target `continue`s past the best-score compare entirely - [Combat.c:9899](../../MoM/src/Combat.c#L9899) says so.

**`AITP_EarthToMud`** ([CMBMAGIC.c:1504](../../MoM/src/CMBMAGIC.c#L1504), 267) - scores every grid square by proximity to units, adding `(3 - delta)` for each qualifying enemy and subtracting it for each qualifying own unit, where `delta` is whichever of the two axis distances is larger. All four `±(3 - delta)` forms verified against the listing's `add ax, 3 / sub ax, [bp+X_Dist]` and `mov ax, 3 / sub ax, ... / mov dx, di / sub dx, ax`. Loop bounds `15h`/`16h` are `COMBAT_GRID_WIDTH`/`COMBAT_GRID_HEIGHT` = 21/22 ([Combat.h:143-144](../../MoM/src/Combat.h#L143-L144)).

**`AITP_CracksCall`** ([CMBMAGIC.c:1661](../../MoM/src/CMBMAGIC.c#L1661), 239) - picks the strongest visible enemy ground unit with a `+30` bonus for one standing on a wall square, then falls back to the same five wall probes as `AITP_Disrupt`. Two subtleties both already documented: a non-visible target skips the best-threat compare ([CMBMAGIC.c:1686](../../MoM/src/CMBMAGIC.c#L1686)), and unlike `AITP_EarthToMud` this one does **not** exclude `MV_TELEPORT` units ([CMBMAGIC.c:1659](../../MoM/src/CMBMAGIC.c#L1659)) - confirmed, there is no such test in its guard chain.

**`AITP_Disrupt`** ([CMBMAGIC.c:1611](../../MoM/src/CMBMAGIC.c#L1611), 66) - five wall-square probes in fixed order. The unrebased-index OGBUG is documented precisely at [CMBMAGIC.c:1621](../../MoM/src/CMBMAGIC.c#L1621) and neighbours; **all five raw offsets check out** - `15D0h`, `15C0h`, `15B8h`, `15CEh`, `15B6h` map to flat indices 60, 52, 48, 59, 47, and every delta matches to the byte.

**`AITP_RecallHero`** ([CMBMAGIC.c:1805](../../MoM/src/CMBMAGIC.c#L1805), 162) - bails on the own-Fortress battle, then ranks own heroes by strength scaled by damage taken. The listing's `imul Max_HP` is followed by `cwd`, which overwrites the high half of the product before the divide; the `(int16_t)` cast reproduces that truncation and the comment already says so.

**`AITP_WarpCreature`** ([Combat.c:9729](../../MoM/src/Combat.c#L9729), 210), **`AITP_WarpWood`** ([Combat.c:9689](../../MoM/src/Combat.c#L9689), 161), **`AITP_Healing`** ([Combat.c:9654](../../MoM/src/Combat.c#L9654), 128) - the same shape: guard chain, score, keep the best. Warp Wood values `ranged * ammo * figure_cnt` and gives non-missile units a flat `-10`; Healing values `((figure_max - figure_cnt) * hits + front_figure_damage)` weighted by `melee + ranged`, zeroed when the unit is missing 2 or fewer hit points; Warp Creature scales by the resistance failure chance, `(strength * (10 - resist) + 9) / 10`.

**`AITP_HolyWord`** ([Combat.c:10588](../../MoM/src/Combat.c#L10588), 76) - three OGBUGs, all confirmed and all already documented at [Combat.c:10583-10585](../../MoM/src/Combat.c#L10583-L10585). The status test is `<= bus_Active`, so only **non**-active units are ever examined and an active enemy never qualifies. There is no Magic Immunity test. And **the listing never moves the result into AX before returning** - there is no `mov ax, si` or `mov ax, di` anywhere before its `retf`, so the original returns loop scratch. The reconstruction returns the intended found-flag, which is a necessary deviation: C cannot express "return whatever is in AX", and the scratch is in practice always >= 0, so the targeter never rejects either way.

**`AITP_DarknessLight`** ([Combat.c:9625](../../MoM/src/Combat.c#L9625), 54) - the smallest, and the one whose `niu_local_variable` name is *correct*: the True Light / Darkness discriminator is computed from `spell_idx` at [Combat.c:9633](../../MoM/src/Combat.c#L9633) and never read. With no `player_idx` parameter, there is no allegiance test either, so both spells return 99 for any active Life *or* Death unit, friend or foe.

## The two formerly-`UU` targeters

Both were tracked as `UU15_AITP_Disintegrate` and `UU_AITP_WordofRecall` - drake178's `UU` marking them as unreferenced. Nothing calls either one: the only occurrences of `UU15_AITP_Disintegrate` anywhere in the disassembly are its own `proc`/`endp` and the overlay thunk `stub111/j_UU15_AITP_Disintegrate.asm`. Disintegrate reaches a target through the dispatcher's `scc_Combat_Destroy_Unit` arm instead, which special-cases it to take raw `Effective_Battle_Unit_Strength` rather than the resistance-scaled value.

Both **do** have production bodies and both are now walked. `UU` is a property of the call graph, not of the function, so both have shed the prefix: **`AITP_Disintegrate`** and **`AITP_Word_Of_Recall`**.

### `AITP_Disintegrate` ([Combat.c:9417](../../MoM/src/Combat.c#L9417), asm 172)

Faithful. Frame order matches - `Retn_Value` `-0Ah`, `Highest_Value` `-8`, `Effective_Resist` `-6`, then the 32-bit enchantments occupying `-4..-1`; [Combat.c:9772](../../MoM/src/Combat.c#L9772) onward declares them in that order.

**Its resistance reject is `> 10`, not `>= 10`.** asm:118 is `cmp [bp+Effective_Resist], 0Ah` with `jle` to proceed, so a unit resisting at exactly 10 is still considered - unlike `AITP_WarpCreature` and the Destroy/Banish, Resistable and Mundane Curse arms, which all reject at `>= 10`. [Combat.c:9804](../../MoM/src/Combat.c#L9804) reproduces the OG inconsistency.

The spell is baked in at the instruction level: asm:102 indexes `spl_Disintegrate*24h` as a compile-time constant, so the function can only ever score for Disintegrate. Scoring is `(melee + ranged) * figure_cnt`, with the usual Magic Immunity, Righteousness, own-side, active and visibility gates ahead of it.

### `AITP_Word_Of_Recall` ([CMBMAGIC.c:1759](../../MoM/src/CMBMAGIC.c#L1759), asm 126)

Faithful. This is `AITP_RecallHero`'s twin - identical frame (`Current_HP` `-8`, `Max_HP` `-6`, `Retn_Value` `-4`, `Highest_Danger` `-2`) and near-identical body, differing only in the eligibility test: this one takes any own active unit costing `>= 200` ([CMBMAGIC.c:1783](../../MoM/src/CMBMAGIC.c#L1783)), where `AITP_RecallHero` requires a hero slot and bails on the own-Fortress battle.

Both share the `imul Max_HP` / **`cwd`** / `idiv` sequence, where the `cwd` discards the high half of the product before the divide. Both now carry the `(int16_t)` cast that reproduces it - [CMBMAGIC.c:1792](../../MoM/src/CMBMAGIC.c#L1792) and [CMBMAGIC.c:1839](../../MoM/src/CMBMAGIC.c#L1839). Without it the product keeps all 32 bits on a modern host and divides differently for any value above 32767.

## Note on the production comments

The dispatcher originally carried seven `/* BUG: ... */` comments, every one of which described a **leaf** targeter rather than the dispatcher. They have since been resolved: four were duplicates of notes that already existed, more precisely, at the leaf; two were wrong and removed - `AITP_EarthToMud` "ignores speed and terrain" describes an absent sophistication rather than a defect, and `AITP_RecallHero` "does not check for banishment" is answered by its own `status != bus_Active` test, since [Combat.h:800](../../MoM/src/Combat.h#L800) annotates `bus_Gone` as covering banished units; and the `AITP_DarknessLight` claim was relocated to its leaf.

Two dispatcher-local comments remain. [Combat.c:9601](../../MoM/src/Combat.c#L9601) now states the Summoning defect precisely - on 200 failed rolls the last rejected coordinates are returned with a 99 "target found" indication. [Combat.c:9241](../../MoM/src/Combat.c#L9241) still reads "switch jump table excludes Wall spells", a claim about spell **data** that cannot be settled from source; what the code shows is that nine of the 24 type slots reach the default and get no target - City Enchantment Positive/Negative (2, 3), Target Wizard (6), types 7 and 8, Global Enchantment (9), Crafting Spell (11), Infusable Spell (17) and Disjunctions (20).
