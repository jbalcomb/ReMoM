Combat-AITP_Combat_Spell.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_CombatSpell__STUB.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\AITP_CombatSpell__STUB__GEMINI.c   (Gemini translation of the .asm — second opinion, NOT ground truth)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr111\_misc.asm   (holds the jump table `off_8B1F3`)

Combat_Cast_Spell__WIP()            Combat.c
    |-> AITP_Combat_Spell()
Choose_Target_And_Action()          CMBTAI.c
    |-> AITP_Combat_Spell()

---

# `AITP_Combat_Spell` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Combat_Spell` | [Combat.c:11650-12062](../../MoM/src/Combat.c#L11650-L12062) | The combat-AI **combined target picker** for every combat spell / spell-like ability. Given `spell_idx`, it dispatches on the spell's **category** (`spell_data_table[spell_idx].type`), scans `battle_units[0.._combat_total_unit_count)` keeping the highest-scored candidate, or delegates to a per-spell sub-picker, and reports a battle-unit index / a `99` tile-or-global marker / `-1` can't-cast. For tile spells it also writes `*target_wx` / `*target_wy`. |

> **Status: FULLY RECONSTRUCTED — faithful to `AITP_CombatSpell__STUB.asm` case-for-case.** Every scan's guard order, every jump-table category, the return protocol, and the summon fall-through match the disassembly. The IDA/`.c`/`_misc.asm` filenames keep the OG name `AITP_CombatSpell__STUB`; production is `AITP_Combat_Spell`. The `.asm` is the authority; the Gemini `.c` is a second opinion.

## Signature

```c
int16_t AITP_Combat_Spell(int16_t spell_idx, int16_t player_idx, int16_t * target_wx, int16_t * target_wy)
```

Matches the asm frame: `Spell_Index` (bp+6), `Player_Index` (bp+8), `TargetX@` (bp+0Ah), `TargetY@` (bp+0Ch) — the last two are pointers (`mov bx,[bp+TargetX@]; mov [bx],…`). Loop index is `battle_unit_idx` (asm `di`).

## Return-value contract

`Picked_Target`/`Highest_Value` init to `-1` (`ST_UNDEFINED`). Every path returns `Picked_Target` through the common epilogue (`loc_8B1E6`), except the explicit early returns.

| Return | Meaning | Caller handling |
|---|---|---|
| `-1` (`ST_UNDEFINED`) | can't cast (no valid target / already-active global / population caps) | `if(target_idx > -1)` fails → ability not chosen ([CMBTAI.c:1211-1279](../../MoM/src/CMBTAI.c#L1211-L1279)). |
| `99` (`63h`) | castable, **no unit target** (tile / global / self; `*target_wx`/`*target_wy` may be set) | `if(Target != 99)` skips the battle-unit coordinate lookup ([Combat.c:13056](../../MoM/src/Combat.c#L13056)). |
| battle-unit index (`0..n`) | castable, **target that battle unit** | `battle_units[Target].cgx/cgy` used as the target square ([Combat.c:13059](../../MoM/src/Combat.c#L13059)). |

## Dispatch — the category jump table (verified from `_misc.asm`)

The `type > 23` guard — coded as `type > scc_Combat_Banish` ([Combat.c:11669](../../MoM/src/Combat.c#L11669)) — returns `-1` for any `type` above `0x17` (`cmp bx,17h / jbe / jmp loc_8B1E4`); the comment flags this as the **OG bug that excludes Wall spells**. Otherwise `jmp [cs:off_8B1F3 + type*2]`. Decoding `off_8B1F3` (`_misc.asm`, one `dw` per `type` 0..23):

| `type` | asm target | category / `case` label |
|---|---|---|
| `0x00` | `loc_8B117` | `scc_Summoning` — find a back-row tile |
| `0x01` | `loc_8A685` | `scc_Unit_Enchantment` — unit buffs (own) |
| `0x02`,`0x03` | `loc_8B1E4` | → `-1` (default) |
| `0x04` | `loc_8A236` | `scc_Direct_Damage_Fixed` |
| `0x05` | `loc_8B036` | `scc_Special_Spell` — per-spell pickers |
| `0x06`–`0x09` | `loc_8B1E4` | → `-1` (default) |
| `0x0A` | `loc_8AFBF` | `scc_Battlefield_Spell` — global enchantments |
| `0x0B` | `loc_8B1E4` | → `-1` (default) |
| `0x0C` | `loc_8A489` | `scc_Combat_Destroy_Unit` — resistance-kill |
| `0x0D` | `loc_8A9BD` | `scc_Resistable_Spell` — combat ench/debuff (enemy) |
| `0x0E` | `loc_8ADEF` | `scc_Unresistable_Spell` — Web / immobilization |
| `0x0F` | `loc_8A87B` | `scc_Unit_Enchantment_Normal_Only` — Heroism |
| `0x10` | `loc_8AC0E` | `scc_Mundane_Curse` — resistance-based debuffs |
| `0x11` | `loc_8B1E4` | → `-1` (default) |
| `0x12` | `loc_8B108` | `scc_Dispels` — Dispel Magic |
| `0x13` | `loc_8B1DD` | `scc_Disenchants` — no-target → `99` |
| `0x14` | `loc_8B1E4` | → `-1` (default) |
| `0x15` | `loc_8AFBF` | `scc_Combat_Counter_Magic` (alias of `0x0A`) |
| `0x16` | `loc_8A236` | `scc_Direct_Damage_Variable` (alias of `0x04`) |
| `0x17` | `loc_8A489` | `scc_Combat_Banish` (alias of `0x0C`) |

The production `switch` labels each `case` with its `scc_*` name and reproduces the aliasing (`case scc_Direct_Damage_Fixed: case scc_Direct_Damage_Variable:`, `case scc_Combat_Destroy_Unit: case scc_Combat_Banish:`, `case scc_Battlefield_Spell: case scc_Combat_Counter_Magic:`).

## Per-category scan — what each loop checks (all verified against the `.asm`)

Every scanning case walks `battle_unit_idx = 0.._combat_total_unit_count`, `continue`-skips on failed guards, scores survivors into `Target_Value`, keeps the max in `Highest_Value`/`Picked_Target`. Guard **order** matches the asm in each case (the fidelity-sensitive part — the order below is the asm's).

- **Direct damage (`loc_8A236`, [11676](../../MoM/src/Combat.c#L11676)):** immunity-magic → Star Fires (Death/Chaos only) → Psionic Blast (illusion-immune skip) → Life Drain (Death/death-immune skip) → Righteousness-vs-Chaos/Death → controller==player → status → visibility → damage (`Undeath_Dmg = 13 - Combat_Effective_Resistance(*bu, sbr_Death)` for Life Drain, else `Apply_Battle_Unit_Damage_From_Spell(spell_idx, battle_unit_idx, &Reg_Dmg, 25)`) → `Target_Value = Reg+Undeath+Irrev`, bonus `(100 - Cur_Figures*hits) + front_figure_damage`.
- **Resistance-kill (`loc_8A489`, [11733](../../MoM/src/Combat.c#L11733)):** controller → status → immunity-magic → stoning+Petrify → Dispel Evil (Death/Chaos only) → Banish (Fantastic only) → Righteousness → visibility → `Unit_Resist = Combat_Effective_Resistance + Spell_Resistance_Modifier`, skip if `>= 10` → Disintegrate uses raw strength, else `(strength*(10-resist)+9)/10`.
- **Unit buffs (`loc_8A685`, [11779](../../MoM/src/Combat.c#L11779)):** controller!=player → status → already-has-effect mask (`enchantments & spell_data_table[spell_idx].enchantments`, [11795](../../MoM/src/Combat.c#L11795)) → per-spell dup guards (Stone Skin/Resist Elements/Resist Magic/Flight) → Berserk range/hero checks (`Range_To_Battle_Unit(battle_unit_idx, target) > movement_points`, `Hero_Slot != -1`) → score `Cur_Figures*10 + melee`.
- **Heroism (`loc_8A87B`, [11812](../../MoM/src/Combat.c#L11812)):** controller!=player → status → `race >= rt_Arcane` → effect mask ([11829](../../MoM/src/Combat.c#L11829)) → `Level > 2` → score `Cur_Figures*10 + melee`.
- **Combat enchant/debuff (`loc_8A9BD`, [11833](../../MoM/src/Combat.c#L11833)):** controller → status → `Combat_Effects & spell_data_table[spell_idx].enchantments` ([11846](../../MoM/src/Combat.c#L11846)) → immunity-magic → Sorcery+illusion → Death+death → Righteousness → Creature Binding (Fantastic only) → Shatter (needs melee>1 or ranged>1) → visibility → resist `< 10` → `(strength*(10-resist)+9)/10`.
- **Resistance debuff (`loc_8AC0E`, [11869](../../MoM/src/Combat.c#L11869)):** immunity-magic FIRST → `Combat_Effects & spell_data_table[spell_idx].enchantments` ([11883](../../MoM/src/Combat.c#L11883)) → Sorcery+illusion → Righteousness → controller → status → `race >= rt_Arcane` → visibility → `Combat_Effective_Resistance` (no modifier here) `< 10` → `(strength*(10-resist)+9)/10`.
- **Web (`loc_8ADEF`, [11899](../../MoM/src/Combat.c#L11899)):** controller → status → Web branch (non-corporeal skip, already-webbed `Web_HP > 0` skip) vs. other-spell branch (`Combat_Effects & …enchantments` [11922](../../MoM/src/Combat.c#L11922), immunity-magic, Sorcery+illusion) → Righteousness → visibility → score `Effective_Battle_Unit_Strength`, `+2000` for a flying Web target.
- **Global enchant (`loc_8AFBF`, [11943](../../MoM/src/Combat.c#L11943)):** `Unit_Resist = (player==_combat_attacker_player)?0:1`; if `combat_enchantments[Param0 + Unit_Resist] > 0` **return -1** ([11948](../../MoM/src/Combat.c#L11948)); True Light/Darkness → `AITP_DarknessLight(spell_idx)`, Holy Word → `AITP_HolyWord(player_idx)`, else `99`.
- **Special pickers (`loc_8B036`, [11967](../../MoM/src/Combat.c#L11967)):** delegates by spell — Healing/Mass Healing → `AITP_Healing` (Mass promotes a hit to `99`), Warp Wood → `AITP_WarpWood`, Warp Creature → `AITP_WarpCreature`, Earth to Mud → `AITP_EarthToMud(player, wx, wy)`, Disrupt → `AITP_Disrupt(player, wx, wy)`, Recall Hero → `AITP_RecallHero`, Cracks Call → `AITP_CracksCall(player, wx, wy)`, Raise Dead → `99` + zero the tile.
- **Dispel Magic (`loc_8B108`, [12013](../../MoM/src/Combat.c#L12013)):** `AITP_DispelMagic(player_idx)`.
- **Summoning (`loc_8B117`, [12018](../../MoM/src/Combat.c#L12018)):** `_units == 1000` → `-1`; count own active units, `>= 9` or `_combat_total_unit_count >= 36` → `-1`; else pick a back-row tile (attacker `14 - Random(3)`, defender `7 + Random(3)`; row `8 + Random(3)`) up to 200 tries via `CMB_TargetRows`, then `Picked_Target = 99` unconditionally (see summon note below).
- **No-target (`loc_8B1DD`, [12051](../../MoM/src/Combat.c#L12051)):** `99`.

## Summon placement — the 200-try fall-through (`loc_8B117`)

Faithfully reproduces the OG bug: the retry loop only decides *which* coordinates you keep; reaching `99` is unconditional once the population guards pass. In the asm, `loc_8B1D6` (`Picked_Target = 99`) is entered **both** from the valid-tile `jl` and by falling through when `Unit_Resist >= 200`. Production ([Combat.c:12043-12048](../../MoM/src/Combat.c#L12043-L12048)) breaks out of the `do…while` on a valid tile (keeping the good coords) and then sets `Picked_Target = 99` after the loop, so 200-try exhaustion still resolves to `99` with the last (invalid) coords — the OG behavior, preserved rather than "fixed". The `CMB_TargetRows[y][x] < 0` test is a signed-byte compare (`int8_t *[]`, matching `cmp [byte ptr bx],0 / jl`).

## Combat_Effects / enchantment masks — uniform full-32-bit reads

The asm tests the accumulated unit enchantments (or `Combat_Effects`) against the spell's full 32-bit effect mask, held in the `s_SPELL_DATA` union ([MOM_DAT.h:1063-1085](../../MoX/src/MOM_DAT.h#L1063-L1085): `enchantments` u32 over `Param0`/`Param1`/`Params2_3`). All five sites read `spell_data_table[spell_idx].enchantments` directly — buffs [11795](../../MoM/src/Combat.c#L11795), Heroism [11829](../../MoM/src/Combat.c#L11829), combat debuff [11846](../../MoM/src/Combat.c#L11846), resistance debuff [11883](../../MoM/src/Combat.c#L11883), Web-else [11922](../../MoM/src/Combat.c#L11922) — which on MSVC (`enchantments` is 32-bit) equals the asm's paired `(LO & mask_low) | (HO & mask_high)` word tests.

## Structural deviation (functionally equivalent, noted for 1:1)

- **Case `scc_Special_Spell` / `scc_Summoning` early exits use `else if` / `return`, the asm is a flat `if`-chain to a common epilogue.** `loc_8B036` is a sequential run of `cmp spell_idx / jnz next` tests (each spell distinct, so an `else-if` chain is equivalent); `loc_8B117`'s `_units==1000`, `>=9`, `>=36` bail-outs `jmp loc_8B1DB → loc_8B1E6` (return `Picked_Target`, still `-1`) rather than `return ST_UNDEFINED`. Same values, restructured control flow.

## OG-bug annotations carried in the reconstruction (verify each against the `.asm` before acting)

In-code comments record: `type > 23` excludes Wall spells ([11668](../../MoM/src/Combat.c#L11668)); and, inside the delegated sub-pickers, Darkness/True Light realm-exclusivity/allegiance, Holy Word no-return-value / inactive-unit scan / magic-immunity, Earth to Mud speed/terrain, Disrupt & Cracks Call uninitialized wall offsets, Recall Hero banishment, Dispel Magic confusion/Invulnerability. **Only the `type > 23` exclusion, the global-enchant path, and the summon loop are defects of `AITP_Combat_Spell` itself — the rest belong to the sub-pickers' own walkthroughs.**

## Delegated sub-pickers (overlay 111 — separate functions)

All ten are now prototyped in `Combat.h` and defined in `Combat.c` (the `WZD 111p02`…`111p08` pickers), so `AITP_Combat_Spell`'s translation unit compiles with no implicit-declaration warnings:

| Sub-picker | prototype | definition | called from |
|---|---|---|---|
| `AITP_DarknessLight(spell_idx)` | Combat.h:1672 | [12065](../../MoM/src/Combat.c#L12065) | Global enchant (True Light / Darkness) |
| `AITP_Healing(player_idx)` | Combat.h:1675 | [12099](../../MoM/src/Combat.c#L12099) | Special (Healing / Mass Healing) |
| `AITP_WarpWood(player_idx)` | Combat.h:1678 | [12144](../../MoM/src/Combat.c#L12144) | Special (Warp Wood) |
| `AITP_WarpCreature(player_idx)` | Combat.h:1681 | [12196](../../MoM/src/Combat.c#L12196) | Special (Warp Creature) |
| `AITP_DispelMagic(player_idx)` | Combat.h:1687 | [12260](../../MoM/src/Combat.c#L12260) | Dispel Magic |
| `AITP_HolyWord(player_idx)` | Combat.h:1693 | [13068](../../MoM/src/Combat.c#L13068) | Global enchant (Holy Word) |
| `AITP_EarthToMud(player_idx, *cgx, *cgy)` | Combat.h:1723 | [14790](../../MoM/src/Combat.c#L14790) | Special (Earth to Mud) |
| `AITP_Disrupt(player_idx, *cgx, *cgy)` | Combat.h:1726 | [14910](../../MoM/src/Combat.c#L14910) | Special (Disrupt) |
| `AITP_CracksCall(player_idx, *cgx, *cgy)` | Combat.h:1729 | [14965](../../MoM/src/Combat.c#L14965) | Special (Cracks Call) |
| `AITP_RecallHero(player_idx)` | Combat.h:1735 | [15078](../../MoM/src/Combat.c#L15078) | Special (Recall Hero) |

Also calls `Target_Is_Visible` ([3741](../../MoM/src/Combat.c#L3741)), `Combat_Effective_Resistance`, `Spell_Resistance_Modifier`, `Effective_Battle_Unit_Strength`, `Apply_Battle_Unit_Damage_From_Spell`, `Range_To_Battle_Unit`, `Random` — all resolved.

## Build status

`Combat.c` compiles clean (`cmake --build --preset MSVC-debug`): no implicit-declaration errors in `AITP_Combat_Spell` or its sub-pickers. `momlib` does not yet link, but the sole remaining blocker is unrelated to this function — `Spells129.c(407)`: `'bu_ptr' undeclared` (a separate WIP). Nothing in the `AITP_Combat_Spell` family is blocking the build.

## Related references

- `…\ovr111\AITP_CombatSpell__STUB.asm` — IDA Pro 5.5 disassembly (**the authority**).
- `…\ovr111\_misc.asm` — holds the `off_8B1F3` category jump table.
- `…\ovr111\AITP_CombatSpell__STUB__GEMINI.c` — Gemini translation (second opinion; source of the OG-bug annotations above).
- [`Cast_Spell_On_Battle_Unit`](../../MoM/src/Combat.c#L12182) — applies the spell once a target is chosen (`…\ovr111\Cast_Spell_On_Battle_Unit.asm`).
- [MOM_DAT.h `s_SPELL_DATA` union](../../MoX/src/MOM_DAT.h#L1063-L1085) — the polymorphic 4-byte payload (`enchantments` / `Param0`/`Params2_3` / `unit_type` / `ce_idx` / `oe_idx` / `strength`).
