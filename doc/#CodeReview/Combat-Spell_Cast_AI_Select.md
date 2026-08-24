Combat-Spell_Cast_AI_Select.md

SEEALSO:  C:\STU\devel\ReMoM\doc\ComputerPlayer\Combat-AITP_Combat_Spell.md
SEEALSO:  Combat-Combat_Spellbook_Screen.md
SEEALSO:  Combat-Spell_Cast.md
SEEALSO:  Combat-AI_Turn.md
SEEALSO:  Combat-Spell_Cast_AI_Target.md

NOTE: all six of ovr139

AI_SelectCmbtSpell()    ==>  AI_Select_Combat_Spell()
AI_EvaluateCmbtSpell()  ==>  AI_Score_Combat_Spell()
AI_SetCombatRealms()    ==>  AI_Prepare_Combat_Realm_Threats()
AI_UnitThreatRealms()   ==>  AI_Build_Unit_Realm_Threat_Percentages()
AI_CombatSpellList()    ==>  AI_Build_Castable_Combat_Spell_List()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr139\AI_SelectCmbtSpell.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr139\AI_EvaluateCmbtSpell.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr139\Effective_Battle_Unit_Strength.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr139\AI_SetCombatRealms.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr139\AI_UnitThreatRealms.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr139\AI_CombatSpellList.asm

OON XREF:  Combat_Cast_Spell() |-> AI_Prepare_Combat_Realm_Threats()

OON XREF:  Combat_Cast_Spell() |-> AI_Select_Combat_Spell()

OON XREF:  AI_Select_Combat_Spell() |-> AI_Score_Combat_Spell()

Combat_Cast_Spell_With_Caster()
    |-> Combat_Cast_Spell()

Combat_Cast_Spell()
    |-> AI_Prepare_Combat_Realm_Threats()
        |-> AI_Build_Castable_Combat_Spell_List()
        |-> AI_Build_Unit_Realm_Threat_Percentages()
            |-> Effective_Battle_Unit_Strength()
    |-> AI_Select_Combat_Spell()
        |-> AI_Build_Castable_Combat_Spell_List()
        |-> AI_Player_Mode()
        |-> AI_Score_Combat_Spell()
    |-> AITP_Combat_Spell()

NOT:
    'Combat Screen'
    'Combat Spellbook'
    'Combat Spellbook Screen'
    'Combat Spell Target Screen'                                    Combat-Combat_Spell_Target_Screen.md
    'Combat Spell Cast'  (non-AI; yay HP, nay CP, NP, NPC, Monster)
    'Combat Spell Cast - AI - Target'                               Combat-Spell_Cast_AI_Target.md
    'Cast Spell On Unit'                                            Combat-Combat_Cast_Apply_Spell_Effect.md
    'Combat Spell Dispell'                                          Combat-Combat_Spell_Dispel.md

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-24).** Six functions, 4,366 asm lines - all of `ovr139`, every body walked end to end. Three findings were raised and all three are fixed. All six were homeless before this doc.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `AI_Score_Combat_Spell` | [Combat.c:17454](../../MoM/src/Combat.c#L17454) | `ovr139/AI_EvaluateCmbtSpell.asm` | 3,093 | **full body walk — faithful** |
| `AI_Build_Unit_Realm_Threat_Percentages` | [Combat.c:18700](../../MoM/src/Combat.c#L18700) | `ovr139/AI_UnitThreatRealms.asm` | 527 | **full body walk — faithful** |
| `AI_Build_Castable_Combat_Spell_List` | [Combat.c:18786](../../MoM/src/Combat.c#L18786) | `ovr139/AI_CombatSpellList.asm` | 287 | **full body walk — faithful** |
| `AI_Prepare_Combat_Realm_Threats` | [Combat.c:18642](../../MoM/src/Combat.c#L18642) | `ovr139/AI_SetCombatRealms.asm` | 171 | **full body walk — faithful** |
| `Effective_Battle_Unit_Strength` | [Combat.c:18617](../../MoM/src/Combat.c#L18617) | `ovr139/Effective_Battle_Unit_Strength.asm` | 165 | **full body walk — faithful** |
| `AI_Select_Combat_Spell` | [Combat.c:17405](../../MoM/src/Combat.c#L17405) | `ovr139/AI_SelectCmbtSpell.asm` | 123 | **full body walk — faithful** |

## What belongs here, and why

This is the **AI's combat spell choice**: deciding *which* spell the computer casts, and the scoring machinery behind that decision. The boundary is choosing the spell - not choosing its target, not casting it, and not the screens a human uses instead.

The cohort is exactly `ovr139`, all six slots, and both entry points are in the same caller. `Combat_Cast_Spell` takes the AI branch at [CMBMAGIC.c:352](../../MoM/src/CMBMAGIC.c#L352) and calls `AI_Prepare_Combat_Realm_Threats` to build the per-side realm bookkeeping, then `AI_Select_Combat_Spell` at [CMBMAGIC.c:363](../../MoM/src/CMBMAGIC.c#L363) to pick the spell. Everything below those two is internal: `AI_Build_Castable_Combat_Spell_List` is called from both, `AI_Build_Unit_Realm_Threat_Percentages` only from `AI_Prepare_Combat_Realm_Threats`, `Effective_Battle_Unit_Strength` from `AI_Build_Unit_Realm_Threat_Percentages`, and `AI_Score_Combat_Spell` only from `AI_Select_Combat_Spell`.

### What does *not* belong here

- **`AITP_Combat_Spell`**, the target chooser, which `Combat_Cast_Spell` calls next - `Combat-Spell_Cast_AI_Target.md`.
- **`AI_Player_Mode`**, adjudicated in [Combat-AI_Turn.md](Combat-AI_Turn.md). `AI_Select_Combat_Spell` consumes its 0..5 result as the index into `AI_Score_Combat_Spell`'s five situational tables, so the two findings recorded against it there - an `&&` that should have been `||`, and a `>` that should have been `>=` - both moved the boundaries of *this* cohort's scoring input.
- **`Combat_Casting_Cost_Multiplier`** and **`Combat_Cast_Spell`** itself, both in [Combat-Spell_Cast.md](Combat-Spell_Cast.md).
- **`Get_Effective_Melee_Strength`**, **`Get_Effective_Ranged_Strength`** and **`Get_Effective_Hits`**, the three `ovr110` scoring primitives `Effective_Battle_Unit_Strength` calls. They are **unowned** - [Combat-Strategic_Combat.md](Combat-Strategic_Combat.md) names all three and explicitly puts them out of its scope, and no other doc claims them. They are not pulled in here either: they are called from both this cohort and the strategic-combat threat loop, so they want a review of `ovr110` rather than a home in a caller's.

## Findings

None outstanding. Three were raised against `AI_Score_Combat_Spell` during the walk - situational-table store order, five merged case groups, and two unrolled realm loops - and all three are fixed.

## Deviations

### D1 - locals are zero-initialised where the original leaves them untouched

The house convention across this tree is an `= 0` (or `= { 0, ... }`) initialiser on every local. The listings generally do not have one. `AI_Select_Combat_Spell` is the clearest case: `sub sp, 0C2h` at asm:14 is followed straight by `mov si, [bp+Caster_ID]`, with no prologue clear of the 184-byte `Spell_List`, yet [Combat.c:17407](../../MoM/src/Combat.c#L17407) declares it `= { 0, ... }`. `AI_Prepare_Combat_Realm_Threats` has the same pattern.

Not observable: the array is filled by `AI_Build_Castable_Combat_Spell_List` and read back only for the `GUI_Multipurpose_Int` entries it wrote. Recorded because it is a genuine difference from the listing - the compiler emits clearing code the original never runs - and because it is now uniform across the cohort: `AI_Score_Combat_Spell`'s five situational tables at [Combat.c:17456-17460](../../MoM/src/Combat.c#L17456-L17460) carry it too, over a frame that reserves them at `bp-4Ch` through `bp-1Ch` with no prologue clear.

## Verified faithful

### `AI_Build_Unit_Realm_Threat_Percentages` ([Combat.c:18700](../../MoM/src/Combat.c#L18700), asm 527)

Faithful, all 527 lines.

**Frame - order matches.** `Enchants_LO` at `bp-6` and `Enchants_HO` at `bp-4` are the two halves of one 32-bit value; production folds them into a single `int32_t enchantments` declared first, which occupies exactly `bp-6..bp-3` little-endian, with `total_strength` last at `bp-2`. `sub sp, 6` reconciles. `itr` and `battle_unit_idx` are register locals sharing SI at different points, which carries no ordering constraint.

**The ten realm attributions are in exact order.** Reading the `add [di+BOOKSHELF.*]` sites out of the listing: Chaos (asm:155), Nature (182), Chaos (200), Chaos (217), Death (280), Chaos (323), Nature (366), Death (397), Death (440), Life (483). [Combat.c:18728](../../MoM/src/Combat.c#L18728) through [Combat.c:18772](../../MoM/src/Combat.c#L18772) match one for one. **`BOOKSHELF.Sorcery` never appears** - see the note at the end of this section.

**Two attributions add to the realm bucket only; eight add to both.** Fire Breath / Lightning (asm:142-155) and Stone Gaze (asm:169-182) have no `add [bp+Total_Strength]` - that strength was already counted by the baseline `melee * figure_cnt` and `ranged * figure_cnt` at asm:89-121. The other eight write both. Production reproduces the split exactly.

**Weights are individually correct.** Fireball/Doom Bolt `+10` (asm:199-200), Immolation `+20` (asm:216-217), Life Steal `(Spec_Att_Attrib + 3) * figure_cnt` (asm:246-280), Destruction / Stoning Touch / Death Touch / Dispel Evil `figure_cnt * 5`, and Cause Fear `figure_cnt` with **no** multiplier (asm:387-397) - the one term of its kind, and production has it at [Combat.c:18761-18762](../../MoM/src/Combat.c#L18761-L18762).

**Four symbols are spelled differently in the IDA database; all four values check out** against `in/WIZARDS.inc`: `Ab_Immolation` = 8 = `USA_IMMOLATION`, `UE_Immolation` = 1 = `UE_IMMOLATION`, `Ab_CauseFear` = 20h = `USA_CAUSEFEAR` = 32, `UE_Cloak_of_Fear` = 8 = `UE_CLOAK_OF_FEAR`. Both `UE_` constants are low-word bits, which is why asm:211-212 and asm:377-378 mask the low word and `and ax, 0` the high one.

**Normalisation** is guarded on `total_strength != 0` (asm:491-492, `jz` past) and computes `realm_array[i] * 100 / total_strength` through `LXMUL@`/`LDIV@` with both operands sign-extended - [Combat.c:18775-18780](../../MoM/src/Combat.c#L18775-L18780).

**Sorcery is never populated.** No `BOOKSHELF.Sorcery` store exists anywhere in the 527 lines, so `realm_array[sbr_Sorcery]` is always 0 after normalisation. Its only consumer in the cohort is `spl_True_Sight` at [Combat.c:18298](../../MoM/src/Combat.c#L18298), where it is the primary scoring term - so that term is dead. This is original behaviour, not a reconstruction gap, and plausibly deliberate: Sorcery's unit-facing abilities are illusion and flight rather than damage, so there is nothing to attribute.

### `AI_Build_Castable_Combat_Spell_List` ([Combat.c:18786](../../MoM/src/Combat.c#L18786), asm 287)

Faithful, all 287 lines.

**Frame - order matches.** `Mana_Limit` `-0Ch`, `Hero_Spell_Index` `-0Ah`, `Spell_Cost` `-8`, `InRealm_Index` `-6`, `Realm_Index` `-4`, `Player_Index` `-2`, with `sub sp, 0Ch`; [Combat.c:18788-18793](../../MoM/src/Combat.c#L18788-L18793) declares those six in that order. `spell_idx` is SI and the caster index lives in DI, neither with a slot.

**The mana ceiling is derived two ways, and the second is recomputed.** For a wizard, `Cmbt_Skill_Left` then a `LXMUL@`/`LDIV@` of `mana_reserve * 10 / range_mod` compared with `jge` past the store, so the source tests `<` - asm:30-51. The listing then evaluates the entire expression a second time to perform the assignment (asm:52-72) rather than reusing the computed value, and [Combat.c:18800-18802](../../MoM/src/Combat.c#L18800-L18802) recomputes it too. For a battle unit, asm:89-90 is `mov al, [mana]` / `mov ah, 0` - a **zero**-extend, matching the `(uint8_t)` cast at [Combat.c:18808](../../MoM/src/Combat.c#L18808).

**Three enumerations, correctly separated.** The caster-creature path (asm:104-148) walks only `((race - 16) * 40) + i + 1` and ends in `jmp` to the function exit - a `return`, reproduced at [Combat.c:18824](../../MoM/src/Combat.c#L18824). The main sweep (asm:149-219) is 6 realms x 40 spells requiring `sls_Known`, pricing wizards through `Casting_Cost(player_idx, spell_idx, 1)` - push order at asm:183-186 confirms the argument order - and units through the raw table cost. The hero pass (asm:220-280) walks 4 slots.

**Every loop bound matches its manifest constant**: `cmp [bp+Realm_Index], 6` = `NUM_MAGIC_REALMS` ([MOM_DEF.h:61](../../MoX/src/MOM_DEF.h#L61)), `28h` = 40 = `NUM_SPELLS_PER_MAGIC_REALM` ([MOM_DEF.h:121](../../MoX/src/MOM_DEF.h#L121)), and `4` = `NUM_HERO_SPELL_SLOTS` ([MOM_DEF.h:138](../../MoX/src/MOM_DEF.h#L138)).

**Both documented OGBUGs are confirmed.** The hero pass is gated at asm:221 on `cmp di, 13h` / `jl` - **19, not 20** - so battle unit #19 never offers hero spells ([Combat.c:18847](../../MoM/src/Combat.c#L18847)). And asm:248-249 reads the hero spell byte with `mov al` / **`cbw`**, sign-extending it, so an innate spell id above 127 reads negative and is dropped by the `<= 0` test at asm:251-252 ([Combat.c:18852-18853](../../MoM/src/Combat.c#L18852-L18853)).

**One detail the other two loops do not share:** the hero pass compares `casting_cost` against the ceiling directly (asm:266-267) without storing to `Spell_Cost` first, where the caster-creature and main loops both do (`mov [bp+Spell_Cost], ax` at asm:133 and asm:198). [Combat.c:18856](../../MoM/src/Combat.c#L18856) reproduces that.

### `AI_Score_Combat_Spell` ([Combat.c:17454](../../MoM/src/Combat.c#L17454), asm 3,093)

Faithful. Walked in three passes: prologue to dispatch (asm 19-168), all 87 handlers (asm 169-3069), shared tail (asm 3070-3093).

#### Prologue (asm 19-168)

Faithful. This is the setup that runs before any per-spell scoring: count the enemy, bail if there is none, load the five situational tables, select the opponent's realm profile, probe for a target, then dispatch.

**Two early returns, both correct.** asm:54-58 is `cmp [bp+Enemy_Units], 0` / `jnz` past the return, so the source tests `== 0` and returns `-100` - [Combat.c:17481-17484](../../MoM/src/Combat.c#L17481-L17484). asm:155-158 is `jge` past the return, so the source tests `< 0` and returns `-1`; production returns `ST_UNDEFINED`, which [MOX_BASE.h:75](../../MoX/src/MOX_BASE.h#L75) defines as `-1`. [Combat.c:17545-17547](../../MoM/src/Combat.c#L17545-L17547).

**The enemy-count condition is `&&`, and it is built the hard way.** asm:39 `jz` and asm:47 `jnz` both jump *past* the increment, which is the `&&` signature; had either jumped *to* it, the source would be `||`. Production folds both into one `if` at [Combat.c:17476](../../MoM/src/Combat.c#L17476). Note the listing recomputes `si * 6Eh` and reloads `battle_units` separately for each of the two field reads (asm:30-35 and asm:40-45) rather than keeping the pointer - production indexes twice, which reproduces that.

**All thirty situational constants match**, and the store order is field-major, groups descending 5 to 1 - asm:60-94 writes all five `.Hopeless`, then all five `.Losing`, and so on through `.Certain`, which production reproduces. Reading them out of the listing by field: `group_5` = 500, 0, 10, 20, 30, 40; `group_4` = -900, 30, 20, 15, 10, 0; `group_3` = -900, 10, 20, 20, 20, 10; `group_2` = -900, 30, 35, 15, 0, 0; `group_1` = 0, 10, 20, 25, 10, -10. Index order is Hopeless, Losing, Disadvantage, Advantage, Winning, Certain - the `AI_Player_Mode` level, straight in as the subscript.

**The opponent's profile is selected, not the caster's.** asm:98-100 tests `Player_Index == _combat_attacker_player` and the *attacker* branch loads `AI_DEFR_RealmFlags` / `AI_DEFR_UnitRealms` - the defender's data. [Combat.c:17517-17518](../../MoM/src/Combat.c#L17517-L17518) and [Combat.c:17531-17532](../../MoM/src/Combat.c#L17531-L17532) match, and the disenchant test reads the opposing side's `.Dfndr` / `.Attkr` enchantment slots to match. The three-term test at asm:110-117 is `||` - the first two `jg` jump *to* the 500 assignment, the last `jle` jumps past it - [Combat.c:17520](../../MoM/src/Combat.c#L17520).

**Both docblock OGBUGs are now confirmed against the listing.**

*The `AITP_Combat_Spell` probe passes the same address twice.* asm:146-152 is `lea ax, [bp+Spell_Value]` / `push ax` / `lea ax, [bp+Spell_Value]` / `push ax` / `push [bp+Player_Index]` / `push [bp+Spell_Index]`, with `add sp, 8` confirming four near words. Right-to-left, that is `(spell_idx, player_idx, &spell_value, &spell_value)` - [Combat.c:17544](../../MoM/src/Combat.c#L17544). There is a second-order consequence the docblock does not state: asm:160 stores `0` into `Spell_Value` immediately after the call, so whatever the probe wrote through either pointer is destroyed before anything reads it. Only the return value survives.

*The realm-threat term is dead.* asm:107 reads `RP_AI_UnsetRealmVar1` on the attacker path and asm:129 reads `RP_AI_UnsetRealmVar2` on the defender path. Both are defined `dw 0` at `dseg/_misc.asm:3629-3630`, and a search of every listing in the tree finds exactly three reads - `ovr111/AITP_DispelMagic.asm:44` and these two - and **no write anywhere**. So `RP_Realm_Threat_Value` is permanently 0, and the eight body sites that read it (asm:184, 466, 533, 754, 973, 1654, 1762, 2079) each add nothing. Production mirrors this exactly: `g_ai_combat_unset_realm_flags` and `..._2` are declared at [Combat.c:768](../../MoM/src/Combat.c#L768) and [Combat.c:773](../../MoM/src/Combat.c#L773), read at [Combat.c:9867](../../MoM/src/Combat.c#L9867), [Combat.c:17544](../../MoM/src/Combat.c#L17544) and [Combat.c:17533](../../MoM/src/Combat.c#L17533), and never written - and C's static zero-initialisation reproduces the `dw 0`.

**The `rp_disenchant_prio` dead store is real too.** The listing writes it at asm:119, 122, 141 and 144 and never reads it - there is no other reference to `RP_Disenchant_Prio` in the 3,093 lines. Production computes it identically and likewise never uses it.

**The dispatch is a bounded jump table.** asm:161-168 is `mov bx, [bp+Spell_Index]` / `dec bx` / `cmp bx, 0CCh` / `jbe` / `shl bx, 1` / `jmp [cs:off_C3335+bx]` - so `spell_idx - 1` over 0..204, covering spells 1..205, with out-of-range falling to `loc_C3306`. The table is at `ovr139/_misc.asm:5`; of its 205 slots, **88 carry a real handler across 87 distinct labels and 117 point at the default**. [Combat.c:17550](../../MoM/src/Combat.c#L17550).


#### Handlers (asm 169-3069)

**Dispatch coverage reconciles exactly.** 88 table slots over 87 distinct labels; production has 87 `case` labels. `spl_Disenchant_True` / `spl_Disenchant_Area` correctly share `loc_C2FDC` via fall-through at [Combat.c:17890-17891](../../MoM/src/Combat.c#L17890-L17891), and the one-slot gap is `spl_Lionheart` - see the note below. No spell has a case it should not, and none is missing one that changes behaviour.

**Six shared bodies, all matching.** Twenty-five of the 87 handlers are two-line `jmp`s into a common body. Each body's spell set was compared against production's fall-through group and all six match exactly: the twelve-spell `+= group_5` body at `loc_C2D90`, the five-spell `+= group_4` body at `loc_C1BB7`, the six-spell `loc_C29EB`, and the pairs `loc_C2BAC`, `loc_C26D3`, `loc_C2406`.

**The 62 real bodies were walked.** No wrong constants, no wrong `group_N` table, no wrong `sbr_` subscript, no wrong `Opp_Spell_Realms` mask, no wrong bit constant. The distinctions that are easy to normalise away are all preserved, and they are numerous:

- `spell_value != 0` (Resist_Elements, True_Sight, Bless) vs `> 0` (Elemental_Armor, Magic_Immunity) vs `> 3` (Fireball) as the post-accumulation gate
- `= -100` (Cracks_Call, Wall_Of_Fire, Wall_Of_Darkness, True_Light, Darkness, Mana_Leak, Counter_Magic, Recall_Hero) vs `-= 100` (Earth_Elemental, Phantom_Warriors, Metal_Fires, Warp_Reality, Holy_Word, Terror, Wrack, Death_Spell, Raise_Dead, Word_Of_Recall) - the listing really does use `mov` in some handlers and `sub` in others, and every one is reproduced
- `status != bus_Active` everywhere except `spl_Raise_Dead` and `spl_Animate_Dead`, which use `<= bus_Active` (asm:1899, asm:2601)
- `Hero_Slot != ST_UNDEFINED` in Animate_Dead (asm:2627, `jnz`) vs `<= ST_UNDEFINED` in Recall_Hero (asm:3030, `jle`)
- Wrack's `Combat_Effective_Resistance(..., sbr_Death) + 1` and `counter * 5` against Death_Spell's `- 2` and `counter * 3`, in otherwise identical bodies
- `rp_realm_threat_value` at x1 (Resist_Elements, Bless), x2 (Elemental_Armor, True_Sight, Righteousness), x3 (Magic_Immunity), x4 (Counter_Magic)

**Borland idioms reconstructed correctly throughout:** signed halving (`cwd` / `sub ax,dx` / `sar ax,1`) as `/ 2`; `idiv bx` with `bx=4` as `/ 4`; long-branch fixups where the surviving `jcc` reads un-inverted (Wall_Of_Stone asm:217, Wall_Of_Fire asm:1271, Disenchant_True asm:2840); split 32-bit compares - `spl_Disenchant_True` alone carries four signed-long `> 0` tests (asm:2860-2864, 2886-2892, 2932-2936) plus a 32-bit equality split into high/low word compares at asm:2911-2914, all reconstructed as single C expressions; and tail-merged exits through `loc_C2D98` / `loc_C2D9C` written out at each call site.

**Three OGBUGs in the handlers, all confirmed and all faithfully preserved:**

- `(opp_spell_realms & 0)` can never pass - asm:179 and asm:461 are literally `test [bp+Opp_Spell_Realms], 0`. [Combat.c:17579](../../MoM/src/Combat.c#L17579) and [Combat.c:17659](../../MoM/src/Combat.c#L17659) carry the note.
- `spl_Animate_Dead` counts eligible units and never uses the total - asm:2650 jumps straight to the tail with DI live and unread. [Combat.c:18559](../../MoM/src/Combat.c#L18559).
- `spl_Recall_Hero`'s Spell-of-Return check sets `-100` and **falls through** to the checks below (asm:2991-2993, no jump to the end), where the otherwise-parallel `spl_Word_Of_Recall` bails out entirely (asm:657-659, `jmp` to the exit). [Combat.c:18574](../../MoM/src/Combat.c#L18574).

**`spl_Lionheart` is an empty case, not a missing one.** Its slot points at `loc_C2967`, a lone `jmp loc_C3306` - and `loc_C3306` is the shared tail, not a default body, so all 117 unmatched slots reach it too. An empty `case spl_Lionheart: break;` and no case at all are behaviourally identical; the distinct label is the only evidence the source had one. It is the only handler of that shape. Recorded rather than raised: the inference rests only on the presence of a distinct label, which is weaker than reading a constant or a jump.

#### Shared tail (asm 3070-3093)

`Casting_Cost_Reduction(player_idx, spell_idx)` - push order at asm:3071-3072 puts `Player_Index` first - signed-halved and added, then `Random(20)` added, then `Spell_Value` returned. [Combat.c:18604-18606](../../MoM/src/Combat.c#L18604-L18606).

**Two whole scoring inputs are dead in the original.** `rp_realm_threat_value` is read at eight handler sites and is permanently 0 (see *Provenance*), so those eight terms contribute nothing. And `opp_unit_realms[sbr_Sorcery]` is read by exactly one handler, `spl_True_Sight` at [Combat.c:18298](../../MoM/src/Combat.c#L18298) - where it is the primary term - but `AI_Build_Unit_Realm_Threat_Percentages` never writes the Sorcery slot, so it is always 0 as well. True Sight's score therefore reduces to the `(opp_spell_realms & 1)` bonus plus the tail.

### `AI_Select_Combat_Spell` ([Combat.c:17405](../../MoM/src/Combat.c#L17405), asm 123)

Faithful, all 123 lines.

**Frame - order matches.** `Spell_List` at `bp-0C2h` for 184 bytes (92 `int16_t`), then `Threat` `-0Ah`, `Player_Index` `-8`, `List_Index` `-6`, `Chosen_Spell` `-4`, `Highest_Value` `-2`, with the caster index in SI and `spell_score` in DI. [Combat.c:17407-17413](../../MoM/src/Combat.c#L17407-L17413) declares the six stack locals in that order; the SI copy is not declared separately, the `caster_idx` parameter is read directly.

**The caster test is `> 19`, twice.** asm:22 and asm:50 both compare SI against 19, which is `CASTER_IDX_BASE - 1` ([MOM_DEF.h:68](../../MoX/src/MOM_DEF.h#L68) puts the base at 20). The first picks the player index - subtract the base for a wizard, read `controller_idx` for a unit - and the second gates the cost multiplier, defaulting to `1` for a unit caster. [Combat.c:17414](../../MoM/src/Combat.c#L17414) and [Combat.c:17424](../../MoM/src/Combat.c#L17424). The listing really does re-test rather than reuse a flag.

**`spell_score` is reused for two unrelated things.** asm:58 stores the cost multiplier into DI, passes it to `AI_Build_Castable_Combat_Spell_List` at asm:76, and then asm:97 overwrites DI with each spell's score. Production keeps the same single local at [Combat.c:17413](../../MoM/src/Combat.c#L17413); the reuse is the original's, not a reconstruction shortcut.

**Every call passes the right one of the two indices.** This body carries both a caster index and a player index, and each of the three calls takes a different one - a natural place for a reconstruction to go wrong silently. All three are correct. asm:72 pushes SI (the caster) to `AI_Build_Castable_Combat_Spell_List`, asm:80 pushes `Player_Index` to `AI_Player_Mode`, and asm:94 pushes `Player_Index` as `AI_Score_Combat_Spell`'s first argument - [Combat.c:17432-17433](../../MoM/src/Combat.c#L17432-L17433) and [Combat.c:17436](../../MoM/src/Combat.c#L17436) match all three. The cost-multiplier call at asm:53-55 recomputes `si - 20` rather than reusing `Player_Index`, which already holds that value on the wizard path; [Combat.c:17426](../../MoM/src/Combat.c#L17426) recomputes it too.

**The two `player_idx` assignments are one store in the listing.** asm:44 is the only `mov [bp+Player_Index], ax`; both arms of the caster test converge on it. [Combat.c:17416](../../MoM/src/Combat.c#L17416) and [Combat.c:17420](../../MoM/src/Combat.c#L17420) write it out twice - a Borland tail-merge, the same shape as the two melee calls in `Effective_Battle_Unit_Strength`, and correct reconstruction rather than duplication.

**The redundant-looking re-zeroing is real.** [Combat.c:17422-17423](../../MoM/src/Combat.c#L17422-L17423) assigns `highest_value = 0` and `chosen_spell = 0` even though both carry `= 0` initialisers seven lines up. The listing stores both explicitly at asm:46 and asm:48, after the player-index computation and before the second caster test, so the assignments belong there.

**The list length comes from a global.** asm:114 bounds the scoring loop with `GUI_Multipurpose_Int`, which `AI_Build_Castable_Combat_Spell_List` sets as an out-parameter by side effect - [Combat.c:17434](../../MoM/src/Combat.c#L17434).

**The pick is strictly greater.** asm:100-102 is `cmp di, [bp+Highest_Value]` / `jle` to skip, so the first spell to reach a given score keeps it - [Combat.c:17437](../../MoM/src/Combat.c#L17437). With `highest_value` starting at 0, a spell scoring 0 or less is never chosen and the function returns `chosen_spell` still 0.

### `AI_Prepare_Combat_Realm_Threats` ([Combat.c:18642](../../MoM/src/Combat.c#L18642), asm 171)

Faithful, all 171 lines. Every OGBUG its docblock claims is real - I checked each against the listing.

**Frame - order matches.** `Spell_List` at `bp-0BAh` for 184 bytes, `Range_Modifier` at `bp-2`, with `list_idx` in SI and `spell_type` in DI. [Combat.c:18644-18647](../../MoM/src/Combat.c#L18644-L18647).

**Two near-identical passes, and the differences are the point.** Each builds the side's castable list, then ORs `1 << magic_realm` into that side's realm flags for every offensive spell - [Combat.c:18648-18669](../../MoM/src/Combat.c#L18648-L18669) for the attacker, [Combat.c:18670-18693](../../MoM/src/Combat.c#L18670-L18693) for the defender. Three asymmetries, all faithful:

- **The type tests are in a different order.** asm:39-47 checks Direct Damage Fixed, Direct Damage Variable, Destruction, Resistable, Mundane Curse; asm:105-113 swaps the second and third. Production reproduces both orders.
- **The defender pass counts one extra spell.** asm:119 tests Creature Binding in both passes, but asm:125 adds Chaos Channels to the defender's only. [Combat.c:18688](../../MoM/src/Combat.c#L18688) carries the note.
- **Neither flag global is ever cleared.** There is no zeroing store anywhere in the listing, so realm flags accumulate across combats. Production's docblock records it and reproduces it.

**IDA's `scc_Destruction_Spell` is production's `scc_Combat_Destroy_Unit`** - both 12 (`WIZARDS.inc:5913`, [MOM_DAT.h:766](../../MoX/src/MOM_DAT.h#L766)).

**The tail computes both sides' unit threat.** asm ends with two `AI_Build_Unit_Realm_Threat_Percentages` calls, attacker then defender, into the two realm arrays - [Combat.c:18694-18695](../../MoM/src/Combat.c#L18694-L18695).

### `Effective_Battle_Unit_Strength` ([Combat.c:18617](../../MoM/src/Combat.c#L18617), asm 165)

Faithful, all 165 lines. One stack local, `short_range_strength` at `bp-2`, with the accumulator in DI.

**Thrown weapons fold into melee.** asm:18-19 is `cmp [es:bx+s_BATTLE_UNIT.ranged_type], srat_Thrown` / `jl` to the alternate path, so a unit at or above `srat_Thrown` passes its `ranged` value in as short-range strength and everything else passes zero - [Combat.c:18621](../../MoM/src/Combat.c#L18621).

**The two melee calls are one call in the listing.** asm:73 is the only `Get_Effective_Melee_Strength`; both arms of the thrown test set up their arguments and converge on it. [Combat.c:18624](../../MoM/src/Combat.c#L18624) and [Combat.c:18629](../../MoM/src/Combat.c#L18629) write it out twice, which is the correct reconstruction of a Borland tail-merge rather than a duplication.

**Three additive terms, the third conditional.** Melee at asm:73, then `Get_Effective_Hits` at asm:113 on `(hits * figure_cnt) - front_figure_damage` weighted by defence, then `Get_Effective_Ranged_Strength` at asm:154 only when `Battle_Unit_Has_Ranged_Attack` is true (asm:118-121) - [Combat.c:18631-18634](../../MoM/src/Combat.c#L18631-L18634).

**Why this matters beyond this cohort.** It is the strength metric behind `AI_Player_Mode`'s six-point scale and `AI_Retreat_Check`'s concede decision, both in [Combat-AI_Turn.md](Combat-AI_Turn.md), and it is what makes those functions' `effective` totals different from raw printed attack values.

## Provenance

The production docblocks in this overlay are markedly more specific than elsewhere in the tree - [Combat.c:17448-17453](../../MoM/src/Combat.c#L17448-L17453) cites a jump table by address (`off_C3335`), a byte range in `in\WIZARDS.asm`, and three separate OGBUGs by mechanism. Everything in them this review checked was accurate:

- the defender-pass Chaos Channels asymmetry
- the type-test order swap between the two passes
- the realm flag globals never being cleared

Both claims that were outstanding are now **confirmed** - the aliased out-pointers and the never-written realm globals - along with the `rp_disenchant_prio` dead store. See the prologue walk above. The docblocks in this overlay have now been checked on six separate specific claims and have been right on all six.

