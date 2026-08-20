Combat-Combat_Cast_Apply_Spell_Effect.md

G_CMB_SpellEffect()          ==>  Combat_Cast_Apply_Spell_Effect()
Cast_Spell_On_Battle_Unit()  ==>  Combat_Cast_Apply_Spell_Effect()
BU_LifeDrain__WIP()  ==>  Apply_Life_Drain()
TILE_DispelMagic()   ==>  Combat_Cast_Dispel()
Cast_Dispel_Magic()  ==>  Combat_Cast_Dispel()
Cast_Disenchant()    ==>  Combat_Cast_Disenchant()
SPL_GetResistMod()   ==>  Spell_Resistance_Modifier()

CMB_NearDispel_UEs  ==>  dispel_enchantment_bits
CMB_NearDispel_UCs  ==>  dispel_combat_effect_bits

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\Cast_Spell_On_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr131\Cast_Dispel_Magic.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr131\Cast_Disenchant.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr131\Apply_Cracks_Call.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Apply_Warp_Creature.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\BU_LifeDrain__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\Magic_Vortex_Create.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Apply_Earth_To_Mud.asm

Combat_Cast_Spell()
    |-> j_Cast_Spell_On_Battle_Unit()
        |-> Combat_Cast_Apply_Spell_Effect()

AI_BU_ProcessAction()
    |-> j_Cast_Spell_On_Battle_Unit()
        |-> Combat_Cast_Apply_Spell_Effect()

Combat_Cast_Apply_Spell_Effect()
    |-> Compute_Battle_Unit_Damage_From_Spell()
    |-> Battle_Unit_Commit_Damage()

NOTE: In ***Combat-Combat_Summon.md***, ...
Combat_Cast_Apply_Spell_Effect()
    switch(spell_data_table[spell_idx].type)
        case scc_Summoning:
            Create_Unit()
            Prepare_Battle_Unit_Summons()
            Battle_Unit_Summon_Animation()

Claude said:
"""
Scope — 8 functions, 4,478 asm lines
The boundary is effect application, not selection and not animation.
Combat_Cast_Apply_Spell_Effect (1,806 asm) plus the seven per-spell appliers it is the only combat caller of:
function                asm
Combat_Cast_Dispel      1499
Combat_Cast_Disenchant   693
Apply_Warp_Creature      129
Apply_Cracks_Call        109
Apply_Life_Drain          95
Apply_Earth_To_Mud        93
Magic_Vortex_Create       54
Apply_Earth_To_Mud was homeless in Combat-Miscellaneous.md; this gives it an owner.
Deliberately excluded: the six AITP_* pickers that share ovr111 with the dispatcher — overlay membership isn't the boundary.
They select targets and belong with AITP_Combat_Spell in Combat-Spell_Cast_AI_Target.md.
Also excluded: the animation layer, the damage/resistance helpers (already in Combat-Battle_Unit_Process_Attack.md), and Cast_Raise_Dead/Cast_Animate_Dead (in Combat-Combat_Summon.md).
"""

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-19).** Nine functions, 4,572 asm lines, all walked end to end against their listings and faithful. Twenty-eight reconstruction errors were raised and fixed across the pass; the tree builds clean.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Cast_Apply_Spell_Effect` | [Combat.c:9955](../../MoM/src/Combat.c#L9955) | `ovr111/Cast_Spell_On_Battle_Unit.asm` | 1806 | **full body walk — faithful** |
| `Combat_Cast_Dispel` | [Spells131.c:274](../../MoM/src/Spells131.c#L274) | `ovr131/Cast_Dispel_Magic.asm` | 1499 | **full body walk — faithful** |
| `Combat_Cast_Disenchant` | [Spells131.c:68](../../MoM/src/Spells131.c#L68) | `ovr131/Cast_Disenchant.asm` | 693 | **full body walk — faithful** |
| `Apply_Warp_Creature` | [Spells133.c:103](../../MoM/src/Spells133.c#L103) | `ovr133/Apply_Warp_Creature.asm` | 129 | **full body walk — faithful** |
| `Apply_Cracks_Call` | [Spells131.c:647](../../MoM/src/Spells131.c#L647) | `ovr131/Apply_Cracks_Call.asm` | 109 | **full body walk — faithful** |
| `Apply_Life_Drain` | [Spells133.c:420](../../MoM/src/Spells133.c#L420) | `ovr133/BU_LifeDrain__WIP.asm` | 95 | **full body walk — faithful** |
| `Apply_Earth_To_Mud` | [Combat.c:22584](../../MoM/src/Combat.c#L22584) | `ovr154/Apply_Earth_To_Mud.asm` | 93 | **full body walk — faithful** |
| `Magic_Vortex_Create` | [Spells133.c:1327](../../MoM/src/Spells133.c#L1327) | `ovr133/Magic_Vortex_Create.asm` | 54 | **full body walk — faithful** |
| `Spell_Resistance_Modifier` | [CMBMAGIC.c:1405](../../MoM/src/CMBMAGIC.c#L1405) | `ovr112/Spell_Resistance_Modifier.asm` | 94 | **full body walk — faithful** |

## What belongs here, and why

This is the **spell-effect application layer**: one dispatcher plus the per-spell appliers it is the only caller of. The boundary is application, not selection and not animation.

The seven companions all have the same profile — they are called from inside `Combat_Cast_Apply_Spell_Effect`'s switch, they have no other combat caller, and none of them is claimed by any existing review:

- `Combat_Cast_Dispel` and `Combat_Cast_Disenchant` are the `scc_Dispels` (18) and `scc_Disenchants` (19) arms, called at [Combat.c:10477](../../MoM/src/Combat.c#L10477) and [10524](../../MoM/src/Combat.c#L10524). At 1,499 and 693 asm lines they are the two largest pieces of open work in this scope after the dispatcher itself.
- `Apply_Warp_Creature`, `Apply_Cracks_Call`, `Apply_Life_Drain`, `Apply_Earth_To_Mud` and `Magic_Vortex_Create` are the small per-spell appliers reached from the `scc_Special_Spell` (5) arm.

`Apply_Warp_Creature` has a second caller in `Spells133.c` and `Combat_Cast_Dispel` has one in `Spells131.c`; neither of those files has a review that claims them, so the primary caller decides the home.

`Apply_Earth_To_Mud` currently sits in [Combat-Miscellaneous.md](Combat-Miscellaneous.md) as homeless. Moving it here gives it the owner it lacked — its only caller is this dispatcher.

### What does *not* belong here

- **The `AITP_*` target pickers**, even though six of them share `ovr111` with the dispatcher. They select a target; they do not apply an effect. `AITP_Combat_Spell` is already homed in [Combat-Spell_Cast_AI_Target.md](Combat-Spell_Cast_AI_Target.md), and the rest belong with it. Overlay membership is not the boundary here — `AITP_Combat_Spell` has its own `switch(spell_data_table[spell_idx].type)` at [Combat.c:9243](../../MoM/src/Combat.c#L9243) with case labels that read almost identically to this function's, which makes the two easy to confuse.
- **The animation functions** — `Combat_Spell_Animation__WIP`, `Wall_Rise`, `Battle_Unit_Summon_Animation`. Every switch arm calls one, but they are the combat spell-animation layer that [Combat-Spell_Cast.md](Combat-Spell_Cast.md) records as having no session of its own.
- **The damage and resistance helpers** — `Compute_Battle_Unit_Damage_From_Spell`, `Battle_Unit_Commit_Damage`, `Combat_Resistance_Check`, `Combat_Effective_Resistance`, `Battle_Unit_Heal`. All are already covered by [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md).
- **`Cast_Raise_Dead` and `Cast_Animate_Dead`**, reached from the `scc_Special_Spell` arm but already covered by [Combat-Combat_Summon.md](Combat-Combat_Summon.md).
- Nothing else. `Spell_Resistance_Modifier` was excluded here in an earlier draft on the claim that it had been walked in [Combat-Combat_Spell_Dispel.md](Combat-Combat_Spell_Dispel.md). **That claim was false** - it appears nowhere in that doc, and the coverage tool was correctly reporting the function as homeless the whole time. It is in scope here and walked below.

## Verified faithful

### `Spell_Resistance_Modifier` ([CMBMAGIC.c:1405](../../MoM/src/CMBMAGIC.c#L1405), asm 94)

**No stack frame.** `push bp` / `mov bp, sp` with no `sub sp`. `resist_mod` lives in DX and `spell_idx` is copied into BX at asm:8 - Borland promoting the parameter - so production using the parameter directly and declaring the single local is right.

**Twenty independent `if`s, not an `else if` chain.** Every test is `cmp _BX_spell_idx, spl_X` / `jnz` to the next test, with no jump past the remaining comparisons after a match. All twenty run on every call and a later match would overwrite an earlier one; no spell appears twice, so nothing depends on it. [CMBMAGIC.c:1405](../../MoM/src/CMBMAGIC.c#L1405) reproduces the flat chain rather than collapsing it into a `switch` or an `else if` ladder, which is what the listing shows.

**The order and the values.** Shatter 0, Warp Creature -1, Weakness -2, Black Sleep -2, Possession -1, Black Wind -1, Terror +1, Wrack +1, Word of Death -5, Death Spell -2, Death Wish 0, Dispel Evil -4, Holy Word -2, Petrify 0, Vertigo 0, Banish -3, Confusion -4, Stasis -5, Creature Binding -2, Great Unsummoning -3. The four zero cases are `xor dx, dx` in the listing against `mov dx, N` for the rest, which is just how Borland materialises a zero constant - the source assigned `0` at all four.

**The explicit zero before the chain.** asm:9 is `xor _DX_resist_mod, _DX_resist_mod` before the first test, matching the statement at [CMBMAGIC.c:1407](../../MoM/src/CMBMAGIC.c#L1407) rather than only the declaration's initialiser.

**Return.** asm:90 is `mov ax, _DX_resist_mod`, then the degenerate `jmp short $+2` that the return-value pattern leaves behind - [CMBMAGIC.c:1436](../../MoM/src/CMBMAGIC.c#L1436).


### `Combat_Cast_Disenchant` ([Spells131.c:68](../../MoM/src/Spells131.c#L68), asm 693)

**Frame - exact match.** `spells` at `bp-30h` spanning thirty bytes, then `enemy_player_idx` at `bp-12h`, `player_idx` at `bp-10h`, `dispell2_notify_background_seg` at `bp-0Eh`, `Msg_Count` at `bp-0Ch`, `ptr_enchantments` as a **dword** at `bp-0Ah`, `spell_idx` at `bp-6`, `itr2` at `bp-4`, `Enchant_Offset` at `bp-2`, `sub sp, 30h`. 70-80 declares them in that order with `itr1` in SI and `threshold` in DI.

**The fifteen-entry spells table is byte-for-byte identical.** Extracted from the listing and diffed against 81-95: same index, same value, same order, `spl_True_Light` through `spl_Blur`.

**The side selection.** asm:60-70 is `cmp player_idx, [_combat_attacker_player]` / `jnz`; the attacker arm takes `s_COMBAT_ENCHANTMENT_STATUS.Dfndr` as the offset and `_combat_defender_player` as the enemy, the other arm takes offset 0 and the attacker - [111](../../MoM/src/Spells131.c#L111). The `NEUTRAL_PLAYER_IDX` early-out at asm:71-73 is [120](../../MoM/src/Spells131.c#L120).

**The per-slot guard and clear.** asm:82 is `cmp [byte ptr es:bx], 0` / `jg` on `combat_enchantments[itr1 + Enchant_Offset]` - [127](../../MoM/src/Spells131.c#L127) - and asm:182-188 writes the same slot back to zero - [139](../../MoM/src/Spells131.c#L139).

**The roll arithmetic is 32-bit.** `Calculate_Dispel_Difficulty(casting_cost, enemy_player_idx, magic_realm)` pushed right to left with `add sp, 6`, the result added to `strength`, then `cwd`-widened operands through `LXMUL@` and `LDIV@`. Production carries the `(int32_t)` casts at all four sites.

**`Notify2` and the message split.** Thirteen words, `add sp, 1Ah`, with `cmp [bp+Msg_Count], 5` / `jl` and `cmp [bp+Msg_Count], 4` / `jge` gating the name-plus-`cnst_Dispel_Msg` build against record 19. `Msg_Count` is a plain local here, incremented at asm:274, not the pointer parameter its `Combat_Cast_Dispel` counterpart uses.


**Frame — exact match.** `Not_Moved_Yet` at `bp-18h`, `Moves_Left` at `bp-16h`, `damage_types` at `bp-14h` (six bytes, three words), `enchantments` at `bp-0Eh` (a `dword`, four bytes), `resistance_modifier` at `bp-0Ah`, then `bp-8`, `bp-6`, `bp-4`, `bp-2`, with `sub sp, 18h`. [Combat.c:9957-9967](../../MoM/src/Combat.c#L9957-L9967) declares them in that order.

IDA names two of those slots `figure_count__did_create_unit` and `resist_fails__ce_idx`, and production splits each into two locals. Two source locals with disjoint live ranges sharing one slot is exactly as consistent with the listing as one local reused, so the split is not a fidelity question either way.

`spell_idx` and `target_idx` are copied from their stack parameters into DI and SI at asm:24-25 — Borland promoting a heavily-used parameter to a register, not evidence of extra locals. Production using the parameters directly is right.

**The caster chain.** asm:26-30 is `cmp [bp+caster_idx], e_CASTER_IDX_BASE` / `jl`, so the wizard case is the fall-through: `player_idx = caster_idx - CASTER_IDX_BASE`, else `battle_units[caster_idx].controller_idx`. [Combat.c:9992-9999](../../MoM/src/Combat.c#L9992-L9999) matches.

**The hero-item guard constant.** asm:87 compares against `e_CASTER_IDX_BASE_sub1` with `jl`, i.e. `caster_idx < 19`. [Combat.c:9987](../../MoM/src/Combat.c#L9987) writes `caster_idx <= MAX_BATTLE_UNIT_COUNT`, and `MAX_BATTLE_UNIT_COUNT` is 18 ([MOM_DEF.h:65](../../MoX/src/MOM_DEF.h#L65)) against `CASTER_IDX_BASE` 20 ([MOM_DEF.h:68](../../MoX/src/MOM_DEF.h#L68)) — the same boundary spelled with a different constant. IDA's choice of symbol is a guess about which name the source used and settles nothing; the values agree.

**The dispatch is a bounded `switch`, and production has it as one.** asm:185-198 reads `spell_data_table[spell_idx].type`, sign-extends it, guards with `cmp bx, 23` / `jbe` — an *unsigned* compare, the standard bounded-switch range check — and jumps through `jt_cscc_cast`, a 24-entry table. Out-of-range falls to `@@Done`, which is the absent `default`. [Combat.c:10009](../../MoM/src/Combat.c#L10009) is the matching `switch`, and the jump table is preserved as a comment above it.

**Every arm has its `break`.** All twelve populated arms end in `jmp @@Done`; `jt_cscc_03_06_07_08_09_11_17_20` at asm:1797 is an empty label falling straight into `@@Done` at asm:1798, and `jt_cscc_00` reaches the end by falling through it. Given how often a missing `break` has turned up elsewhere in this codebase, that is worth stating explicitly.

**The `scc_Direct_Damage_Fixed` arm.** asm:199-226 matches [Combat.c:10061-10067](../../MoM/src/Combat.c#L10061-L10067) call for call: `Combat_Spell_Animation__WIP` with six words, `Apply_Battle_Unit_Damage_From_Spell` with four (`add sp, 8`) which is production's `Compute_Battle_Unit_Damage_From_Spell(spell_idx, target_idx, &damage_types[0], 0)`, `BU_ApplyDamage__WIP__SEGRAX` with two (`pop cx` twice) which is `Battle_Unit_Commit_Damage(target_idx, &damage_types[0])`, then the three screen calls.

**The `damage_types` clear.** asm:172-184 is a bottom-tested loop over three words, matching [Combat.c:10005-10008](../../MoM/src/Combat.c#L10005-L10008).

**The unit-enchantment arm is a 32-bit OR, and production has the width right.** asm:898-927 loads `battle_units[target_idx].enchantments` as a high/low word pair, ORs `s_SPELL_DATA.Param0` into the low word and `s_SPELL_DATA.Params2_3` into the high word, and stores both back. Those four bytes at 0x20-0x23 are the `enchantments` u32 that [MOM_DAT.h:1051](../../MoX/src/MOM_DAT.h#L1051) documents as the `scc_Unit_Enchantment` view of the same storage, so [Combat.c:10030](../../MoM/src/Combat.c#L10030) naming the field rather than the two generic `Param` halves is the same 32 bits and the correct spelling. The Haste special case above it is `cmp spell_idx, spl_Haste` / `jnz` at asm:880-881, so `==` is the source condition.

**The arm's tail re-initialises the target, and both helpers are renames.** asm:965 calls `j_BU_Init_Battle_Unit` and asm:975 calls `j_BU_Apply_Battlefield_Effects__WIP`, each with a far pointer to `battle_units[target_idx]` cleaned by two `pop cx`. Those are `Battle_Unit_Regular_Stats` and `Battle_Unit_Special_Stats` in production, the renames recorded at [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md).

**The `Not_Moved_Yet` clobber is an OGBUG and is preserved.** asm:936 clears the flag, asm:944-946 sets it when the unit has not moved, and asm:978 clears it again immediately before the test at asm:979 — so the `jnz` always fires and the "restore full movement" branch at asm:981-991 is dead in the original. [Combat.c:10047-10048](../../MoM/src/Combat.c#L10047-L10048) reproduces the second assignment in the same place. **Do not "fix" this.**

**This arm's re-init is the reference shape for a `target_idx` block** - [Combat.c:10035](../../MoM/src/Combat.c#L10035) onward. It has no `status == bus_Active` guard, puts `Not_Moved_Yet` after the `Battle_Unit_Moves2` call, and carries the clobber, all matching asm:932-1000.

**`case scc_City_Enchantment_Positive` passes the cast spell, not a literal.** asm:1392-1398 pushes `caster_idx` then `spell_idx` and calls `j_Wall_Rise` with two `pop cx`, so the source is `Wall_Rise(spell_idx, caster_idx)` — [Combat.c:10059](../../MoM/src/Combat.c#L10059). That is a different call from the `scc_Special_Spell` arm's hardcoded `Wall_Rise(spl_Wall_Of_Stone, caster_idx)` at [Combat.c:10085](../../MoM/src/Combat.c#L10085).

**The Spell Save modifier reads the item record.** asm:158-165 does `imul sizeof(s_ITEM)` / `les [_ITEMS]` / `mov al, [es:bx+s_ITEM.spell_save]` / `cbw` / `sub`, so the subtracted quantity is `_ITEMS[item_idx].spell_save` sign-extended from a byte — [Combat.c:10001](../../MoM/src/Combat.c#L10001). The slot guard at [9995](../../MoM/src/Combat.c#L9995) is `cmp [...], -1` / `jle`, i.e. `> ST_UNDEFINED`, and the bound at [9992](../../MoM/src/Combat.c#L9992) matches `cmp [bp+itr], 3` with `NUM_HERO_ITEM_SLOTS` = 3.

**Every screen-update site is accounted for, including the one that deliberately is not the macro.** The asm has nine full `Set_Page_Off` / `Combat_Screen_Draw` / `PageFlip_FX` triples - asm:223, 489, 611, 685, 792, 866, 929, 1155, 1286 - and production has ten, all written out longhand. The counts reconcile because **asm:223-225 serves two arms**: `jt_cscc_22` ends at asm:344 with `jmp loc_8BE68`, jumping back into `jt_cscc_04`'s tail to share the damage-commit and screen update. That is Borland's cross-jump optimisation, the same tail-merge seen in `Combat_Cast_Spell_Error`, so both arms having their own `Battle_Unit_Commit_Damage` and screen update in production is the correct reconstruction and not a duplication to collapse.

Per arm: 4 and 22 share one triple, 12/23 has one, 13/16 has two, 14 has two, 1/15 has one, 18 has one, 19 has one. Arms 0, 2, 5 and 10/21 have none in the asm and none in production.

**The function tail is two calls, not three.** asm:1798-1800 is `@@Done:` followed by `call j_Combat_Screen_Draw` / `call PageFlip_FX` with **no `Set_Page_Off`**, and [Combat.c:10595-10596](../../MoM/src/Combat.c#L10595-L10596) matches. Every other screen update in the function is the full three-call sequence, so this one is easy to "correct" by mistake.

**`.strength` is `uint8_t`, which is what the asm expects.** The three `scc_Direct_Damage_Variable` blocks load the base damage with `mov al, [Param0]` / `mov ah, 0` - zero-extension. [MOM_DAT.h:1083](../../MoX/src/MOM_DAT.h#L1083) declares `strength` as `uint8_t` in the polymorphic payload's damage arm, so production's `Moves_Left = spell_data_table[spell_idx].strength;` widens the same way. Reaching for the generic `.Param0` here would have been an `int8_t` and sign-extended instead.

**The three variable-damage blocks compute damage outside their guard.** `scc_Direct_Damage_Variable` (22) has three near-identical blocks - the bolt group, Psionic Blast and Fireball - and in each the asm's `jle` skips only the `Moves_Left +=`, with the damage call sitting at the join afterwards: joins at `loc_8BEF9` (asm:271), `loc_8BF47` (asm:303) and `loc_8BF96` (asm:335). Production has the call after the `if(tscc > 0)` in all three - [Combat.c:10571](../../MoM/src/Combat.c#L10571), [10581](../../MoM/src/Combat.c#L10581) and [10591](../../MoM/src/Combat.c#L10591). That matters: each of these six spells is damage-computed twice in the original, generically at the head of the arm and then again with the spell-specific base overwriting `damage_types`, so a cast at exactly base cost still has to reach the second computation. The arithmetic inside each guard is right too - the bolt group adds the raw remainder, Psionic Blast halves it with the `cwd` / `sub ax, dx` / `sar ax, 1` signed-halve idiom, and Fireball divides by 3 with `idiv`.

**`Apply_Life_Drain` takes four parameters.** asm:238-244 pushes `tscc`, `caster_idx`, `&damage_types` and `target_idx` and cleans eight bytes, and the callee's own frame declares `BU_Index` at `bp+6`, `Dmg@` at `bp+8`, `Caster` at `bp+0Ah` and `Mana` at `bp+0Ch`, reading the fourth with `mov si, [bp+Mana]` in its prologue. [Combat.c:10555](../../MoM/src/Combat.c#L10555) passes all four and [Spells133.h:33](../../MoM/src/Spells133.h#L33) declares all four. The body is still a stub that writes `1` into the three damage slots, so the interface is right ahead of the reconstruction rather than after it.

**The remaining eight arms are otherwise faithful.** Each was checked for call order, argument count and cleanup, and branch polarity:

- **`scc_Summoning` (0)** - `Create_Unit(.unit_type, player_idx, 0, 0, 9, 2000)` with six words and `add sp, 0Ch`, then `cmp e_ST_TRUE` / `jnz` giving `== ST_TRUE`, then `Prepare_Battle_Unit_Summons` (four words) and `Battle_Unit_Summon_Animation` (five words, `add sp, 0Ah`). The arm reaches the end by falling through the empty `jt_cscc_03_06_07_08_09_11_17_20` label.
- **`scc_Special_Spell` (5)** - the longest arm, and every one of its twelve spell tests matches: the three-way `!=` guard on the animation, Wall of Stone, Warp Wood, Healing, Creature Binding, Warp Creature, the Recall pair, Magic Vortex, Earth to Mud, Cracks Call, Disrupt, Raise Dead and Animate Dead, in that order with matching argument counts. Creature Binding correctly has **no** re-init in either the asm or production, while Warp Creature has one that runs unconditionally after its resistance test.
- **`scc_Battlefield_Spell` / `scc_Combat_Counter_Magic` (10, 21)** - the five-way `!=` guard, `ce_idx` from `.Param0`, `!= _combat_attacker_player` incrementing it, and `== spl_Counter_Magic` choosing `(int8_t)tscc` over `ST_TRUE`.
- **`scc_Combat_Destroy_Unit` / `scc_Combat_Banish` (12, 23)** - the Spell Lock early-out, Disintegrate's `Combat_Effective_Resistance(...) + resistance_modifier < 10` with `damage_types[2] = 200`, the Dispel Evil undead check, the Petrify immunity skip, and the per-figure `Combat_Resistance_Check` loop bounded by `figure_cnt`. Both struct-by-value calls go through `SPUSH@` with `add sp, 70h` / `add sp, 72h`.
- **`scc_Resistable_Spell` / `scc_Mundane_Curse` (13, 16)** - `resist_fails > 0` gating the effect, the Possession / Creature Binding controller swap, the `== HUMAN_PLAYER_IDX` re-ready, and Black Sleep's finish. The `<= 0` path taking its own screen update at `loc_8C30F` is a real separate join, not a shared one.
- **`scc_Unresistable_Spell` (14)** - the three-way immunity guard, including that `UE_RIGHTEOUSNESS` is masked out of the **high** word (`and dx, 0` / `and ax, UE_RIGHTEOUSNESS`) where `UE_SPELL_LOCK` in arm 12/23 is masked out of the low word. Web's `Web_HP = 12`, finish and zeroed movement all match.
- **`scc_Dispels` (18)** and **`scc_Disenchants` (19)** - the `_page_flip_effect = pfe_Dissolve` / update / `pfe_None` bracket, and `Combat_Cast_Dispel` with five words (`add sp, 0Ah`) against `Combat_Cast_Disenchant` with two.

**One naming inconsistency, not a fidelity issue.** Arms 13/16 and 14 both apply `or ax, [word ptr es:bx+s_SPELL_DATA.Param0]` to `combat_effects`, but production spells the same word two ways - `.ce_idx` at [Combat.c:10333](../../MoM/src/Combat.c#L10333) and `.Param0` at [Combat.c:10420](../../MoM/src/Combat.c#L10420). Both read the same two bytes, so neither is wrong against the asm, but `ce_idx` means "city enchantment index" and these arms are writing a combat-effect bitmask.

### `Combat_Cast_Dispel` ([Spells131.c:274](../../MoM/src/Spells131.c#L274), asm 1499)

Faithful. Ten findings were raised across the walk and all are fixed.

**Frame - exact match.** `notify_background_seg` at `bp-12h`, `enemy_player_idx` at `bp-10h`, `Dispel_Target` at `bp-0Eh`, `Test_Flag` at `bp-0Ch`, `test` as a **dword** at `bp-0Ah`, `Flag_Loop_Var` at `bp-6`, `itr` at `bp-4`, `player_idx` at `bp-2`, `sub sp, 12h`. [Spells131.c:276-284](../../MoM/src/Spells131.c#L276-L284) declares them in that order with `battle_unit_idx` in DI, and the `notify_count` parameter is copied into SI at entry - Borland promoting a parameter, not an extra local. IDA's `Dispel_Target`, `Test_Flag` and `test` are production's `threshold`, `combat_effect_bit` and `enchantment_bit`; `enchantment_bit` is `int32_t`, which is what makes the signed 32-bit bit test below reproduce.

**The 46-entry table is byte-for-byte identical.** `dispel_enchantment_bits` is 64 bytes (32 entries, the width of the `uint32_t` enchantment mask) and `dispel_combat_effect_bits` 30 bytes (15 entries, for the 16-bit `combat_effects` mask), both from `Near_Allocate_Next` after a `Near_Allocate_Mark` at [285](../../MoM/src/Spells131.c#L285). Extracting every `mov [word ptr bx+N], spl_*` from the listing and diffing against [288](../../MoM/src/Spells131.c#L288)-[333](../../MoM/src/Spells131.c#L333) matches on array, index and value in the same interleaved order. Entry N is the spell owning bit N: entry 0 is `spl_Immolation` against `UE_IMMOLATION` 0x1, entry 14 `spl_Spell_Lock` against 0x4000, entry 24 `spl_Heroism` against 0x01000000.

**Prologue.** asm:128-132 is `cmp caster_idx, e_CASTER_IDX_BASE_sub1` / `jle`, then `add ax, -20` on the wizard arm - [337](../../MoM/src/Spells131.c#L337). `enemy_player_idx` is chosen at [343](../../MoM/src/Spells131.c#L343) by `cmp player_idx, [_combat_attacker_player]` / `jnz`, attacker taking the defender.

**The vortex pass.** asm:154-223, reproduced at [352](../../MoM/src/Spells131.c#L352): the two coordinate `cmp` / `jz` pairs, the difficulty keyed on `_vortexes[itr].owner_idx`, and `Delete_Structure` at [360](../../MoM/src/Spells131.c#L360) taking five words (`add sp, 0Ah`, the far pointer counting two) followed by the `itr--` / `_vortex_count--` compact-in-place pair.

**Seven dispel rolls, all 32-bit.** Every one is `Calculate_Dispel_Difficulty(...) + strength`, then `cwd`-widened operands through `LXMUL@` and `LDIV@` - seven pairs in the listing. The `+ 500` appears at exactly one, asm:1348-1351, and belongs to the combat-effects loop at [396](../../MoM/src/Spells131.c#L396), identifiable by its `enemy_player_idx` argument at asm:1325. **The asm's block layout does not follow source order here**, so reading the rolls top to bottom would put the `+ 500` on the wrong one.

**Three loops, three bounds.** asm:1482 `cmp [bp+Flag_Loop_Var], 14` / `jge` for combat effects at [378](../../MoM/src/Spells131.c#L378), asm:624 and asm:912 `cmp ..., 31` for the two enchantment passes at [444](../../MoM/src/Spells131.c#L444) and [486](../../MoM/src/Spells131.c#L486). Bit 31 is therefore never tested, which is why Invulnerability - entry 31 - can never be dispelled.

**The bit test is a signed 32-bit `> 0`.** asm:381-388 is `or ax, ax` / `jge` on the high word then `or dx, dx` / `ja` on the low - Borland's shape for a signed `long` comparison, reproduced by the `int32_t` local.

**The curse sweep's Haste guard uses two different effects.** asm:1287-1291 excludes `bue_Haste` outright, then excludes `bue_Confusion` only when `Confusion_State == 2` - [384](../../MoM/src/Spells131.c#L384). Both operands matter; making them the same effect collapses the second clause.

**Spell Lock, Mass Invisibility, Holy Weapon.** The Spell Lock branch at [431](../../MoM/src/Spells131.c#L431) tests only `_UNITS[].enchantments`, so a combat-cast Spell Lock does not protect. The Mass Invisibility skip is two-armed - `Mass_Invisibility.Attkr` at [493](../../MoM/src/Spells131.c#L493) and `.Dfndr` at [504](../../MoM/src/Spells131.c#L504), asm:641 and asm:652 - while the Holy Weapon skip at [515](../../MoM/src/Spells131.c#L515) sits outside that split, on the `loc_B2837` join, and keys on the **caster's** `player_idx`.

**Heroism and Flight.** asm:482 and asm:780 are `cmp [bp+Flag_Loop_Var], 24`, asm:506 and asm:804 `cmp ..., 17`. Production compares `bit_idx` against `BIT_POS(UE_HEROISM)` and `BIT_POS(UE_FLIGHT)` at [455](../../MoM/src/Spells131.c#L455) and [460](../../MoM/src/Spells131.c#L460) and their second-loop copies, which constant-fold to 24 and 17. The `UE_` macros are masks, so comparing `bit_idx` against them directly is always false - a trap worth remembering, since the header carries the bit number in a trailing comment.

**Haste and Confusion tails** test with `test [es:bx+s_BATTLE_UNIT.Combat_Effects], bue_Haste` at asm:921 and `bue_Confusion` at asm:1080 - bitwise, not equality - at [562](../../MoM/src/Spells131.c#L562) and [596](../../MoM/src/Spells131.c#L596).

**Five notification blocks, and they are not interchangeable.** Each is `Mark_Block` / `LBX_Reload_Next(..., 51, ...)` / name-or-record-19 / `Notify2` / `*notify_count += 1` / `Release_Block`, with the inner split `cmp [word ptr _SI_D_Count], 4` / `jge`. But the guard placement differs per block and the asm is explicit about it:

| block | asm | `Mark_Block` | `Release_Block` | production |
| --- | --- | --- | --- | --- |
| combat effects | 1366-1477 | before guard | after join `loc_B2FC3` - outside | [422](../../MoM/src/Spells131.c#L422) |
| overland ench. | 525-620 | after guard | before `loc_B27CF` - inside | [480](../../MoM/src/Spells131.c#L480) |
| combat ench. | 750-907 | before guard | after join `loc_B2A82` - outside | [553](../../MoM/src/Spells131.c#L553) |
| Haste | 969-1072 | before guard | after join `loc_B2C00` - outside | [586](../../MoM/src/Spells131.c#L586) |
| Confusion | 1137-1265 | before guard | after join `loc_B2DC7` - outside | [632](../../MoM/src/Spells131.c#L632) |

The name source differs too - `dispel_enchantment_bits` at asm:556 and asm:843, `spl_Haste` at asm:1008, `spl_Confusion` at asm:1201, `dispel_combat_effect_bits` at asm:1412. `Notify2` takes thirteen words (`add sp, 1Ah`) at every site, x and y built as `160 + (*notify_count * 10)` and `20 + (*notify_count * 25)`. Only the combat-effects block stages its x through `threshold` - asm:1469 is `push [bp+Dispel_Target]` where the others compute into AX inline - and [418](../../MoM/src/Spells131.c#L418) reproduces that.

**Epilogue.** `Near_Allocate_Undo` releasing the two tables, then `CMB_ComposeBackgrnd__WIP`, at [641](../../MoM/src/Spells131.c#L641). Nothing returned in AX.

**The warped-node pass.** asm:288-341 is a five-way `&&` chain - `NF_WARPED` set, `owner_idx == player_idx`, then wx / wy / wp against the combat location - reproduced at [169](../../MoM/src/Spells131.c#L169). The clear at asm:377-389 is `flags ^= NF_WARPED` written back as a byte. `NUM_NODES` is 30, matching `cmp _SI_itr1, e_NUM_NODES`.

**The city-enchantment pass.** `ptr_enchantments` is loaded as a far pointer to `battlefield->city_enchantments` at asm:466-470, the bound is `cmp [bp+Enchant_Offset], 26` at asm:648 against `NUM_CITY_ENCHANTMENTS` = 26 at [216](../../MoM/src/Spells131.c#L216), and `Get_Spell_For_City_Enchantment(Enchant_Offset)` takes one word. The Wall of Fire and Wall of Darkness zeroing at asm:596-612 sits **after** the `LBX_Reload_Next`, which [235](../../MoM/src/Spells131.c#L235) reproduces, and the message record is 46 at [250](../../MoM/src/Spells131.c#L250) where the first pass uses 45.

**Both notification blocks now match.** Record 52 at asm:550 out of `..._1of2` for the combat-enchantment pass and out of `..._2of2` for the city pass - the two blocks share a record number but not a file. `Notify2` takes a bare `160` for x at both.

**The closing pass.** asm:652-687 zeroes `Msg_Count`, walks the battle units testing `status == bus_Active`, and calls `Cast_Dispel_Magic(cgx, cgy, caster_idx, strength, &Msg_Count)` - five words, `add sp, 0Ah`, through the `push cs` / `call near ptr` same-segment idiom - at [266](../../MoM/src/Spells131.c#L266). The epilogue is bare register pops and `retf`; unlike its sibling this function has no `Near_Allocate_Mark` to undo.


### `Combat_Cast_Disenchant` ([Spells131.c:68](../../MoM/src/Spells131.c#L68), asm 693)

Faithful. Ten findings were raised across the walk and all are fixed.

Four passes, each with its own difficulty source: combat enchantments keyed on `enemy_player_idx` (asm:104), the warped node on a literal `-1` (asm:346), the vortex on `_vortexes[itr].owner_idx` (asm:419), and the city enchantment on `ptr_enchantments[idx] - 1` (asm:503-507). Three of the four preserved comments describe those choices, and two of them - *"every Magic Vortex that does not belong to the caster"* and *"every city enchantment that was not placed by the caster"* - are what pin the two owner tests to `!=` (asm:408-409 `jz` to the increment, asm:486-488 `jnz` into the body).

**The enchantment loop walks pairs.** `add _SI_itr1, 2` at asm:280 against `cmp ..., 30`, with the spell table indexed by `slot / 2` - the `cwd` / `sub ax, dx` / `sar bx, 1` idiom at asm:86-94 followed by a `shl bx, 1` to scale the two-byte element. Fifteen table entries over thirty slots.

**Neither notification block staggers horizontally.** Both push a bare `mov ax, 160` for x (asm:270, asm:637) with y as `Msg_Count * 25 + 40`, and both load record **52** - but out of `..._1of2` for the combat-enchantment pass and `..._2of2` for the city pass. Message records are 45 and 46 respectively, against 19 in `Combat_Cast_Dispel`.

**None of the four rolls adds 500.** All four are `cmp ax, _DI_threshold` (asm:136, 375, 447, 539). That `+ 500` exists only on a single roll in `Combat_Cast_Dispel`.

**`ptr_enchantments` is a real source local.** The dword at `bp-0Ah` is written once at asm:466-470 with a computed far pointer - `battlefield` segment into the high word, offset plus `s_BATTLEFIELD.city_enchantments` into the low - and reloaded through `les bx, [bp+ptr_enchantments]` at asm:474, 480, 502 and 543. In the same blocks `spell_data_table` (asm:497, 511) and `battlefield` (asm:559, 564) are reloaded straight from their globals, and Borland 3.0 has no optimisation that invents a stack temporary to cache an address across statements. Inlining those four uses would now be the deviation.

**The closing pass** zeroes `Msg_Count`, walks the battle units on `status == bus_Active`, and calls `Cast_Dispel_Magic(cgx, cgy, caster_idx, strength, &Msg_Count)` through the `push cs` / `call near ptr` same-segment idiom. The epilogue is bare register pops - unlike its sibling this function has no `Near_Allocate_Mark` to undo.

### The five appliers

All five are faithful.

**`Magic_Vortex_Create` ([Spells133.c:1327](../../MoM/src/Spells133.c#L1327), asm 54).** No stack frame at all - `push bp` / `mov bp, sp` with no `sub sp`, `cgx` and `cgy` promoted into CX and SI. Six field stores in the asm's order - `cgx`, `cgy`, `move_cgx`, `move_cgy`, `stage = 0`, `owner_idx` - then `inc [_vortex_count]`. Every store indexes `_vortexes[_vortex_count]`, which is why the increment has to come last.

**`Apply_Earth_To_Mud` ([Combat.c:22570](../../MoM/src/Combat.c#L22570), asm 93).** Two nested loops from `-2` to `< 3` (asm:13, 16, 79, 85), the `CTG_Grass` / `CTG_Dirt` test as a genuine `||` - `jz` to the body on grass, `jnz` past it on dirt - and four stores per matching cell: `muds = ST_TRUE`, then `MoveCost_Ground`, `MoveCost_Ground2` and `MoveCost_Sailing` all to 12, in that order. The `combat_terrain_type_group` local at [Combat.c:22574](../../MoM/src/Combat.c#L22574) has no asm counterpart, and its `DNE in Dasm` marker is accurate: the listing re-reads `terrain_group` for the second comparison at asm:37 rather than caching it.

**`Apply_Cracks_Call` ([Spells131.c:643](../../MoM/src/Spells131.c#L643), asm 109).** Frame matches - `wall_cgy` at `bp-0Ah`, `wall_cgx` at `bp-8`, `damage_types` at `bp-6` spanning six bytes, with `battle_unit_idx` in SI and `itr` in DI. The `damage_types` clear runs inside the unit loop, the four-way `&&` covers cgx, cgy, `MV_FLYING` and `UA_NONCORPOREAL`, and `itr` is then **reused** to hold `Random(4)` (asm:64) before the `== 1` test - a genuine variable reuse, not two locals. The wall block computes `wall_cgx` before `wall_cgy` and stores `walls[wall_cgy][wall_cgx] = 2` with a row stride of 8 (`shl 3`) and a two-byte element (`shl 1`).

**`Apply_Warp_Creature` ([Spells133.c:103](../../MoM/src/Spells133.c#L103), asm 129).** `warp` starts at `-2` and the loop test at asm:120-122 is `or di, di` / `jge` - a `while(warp < 0)`. The dispatch loads `warp` into AX and compares 1, 2, 3 with a final `jmp` to the loop test, which is the sparse-`switch` shape; the degenerate `jmp short $+2` at asm:119 is case 3's trailing `break`. Each case sets a **different** sentinel when its effect is already present - `-1`, `-2`, `-3` at asm:44, 72 and 100 - and production reproduces all three.

**`Apply_Life_Drain` ([Spells133.c:420](../../MoM/src/Spells133.c#L420), asm 95).** Faithful. `itr` is declared before `save_mod`, matching `Loop_Var` at `bp-4` and `Save_Mod` at `bp-2`. `extra_save_bonus` is `(mana - casting_cost) / 5` clamped at zero by `or si, si` / `jge`, then negated into `save_mod`. `Combat_Resistance_Check(battle_units[target_idx], save_mod, sbr_Death)` goes through `SPUSH@` with `add sp, 72h`. The result lands in `damage_types[dt_Drain]` - asm:63 is `mov [bx+2], di`, and `dt_Drain` is 1 - at [Spells133.c:439](../../MoM/src/Spells133.c#L439). The caster split at asm:64 is `cmp [bp+Caster], 14h` / `jl`, so a wizard accumulates `spell_casting_skill += damage * 3` and a battle unit takes `Battle_Unit_Heal(caster_idx, damage, 1)`. The `add` / `adc` pair at asm:78-79 is a 32-bit accumulate, which [Spells133.c:442](../../MoM/src/Spells133.c#L442) satisfies because `spell_casting_skill` is `int32_t` ([MOM_DAT.h:1521](../../MoX/src/MOM_DAT.h#L1521)).


