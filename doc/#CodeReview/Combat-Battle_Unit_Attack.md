Combat-Battle_Unit_Attack.md

Battle_Unit_Attack__WIP()  ==>  Battle_Unit_Attack()

Melee_Animation()
    Anim_Duration            ==>  anim_frame_count
    Anim_Frame_Index         ==>  itr_anim_frame

Ranged_Animation()
    Travel_Distance          ==>  travel_percent_step
    Progress_Counter         ==>  travel_percent_elapsed

globals
    CMB_ProjectileFrame      ==>  projectile_anim_frame
    RP_CMB_ProjectileFrame2  ==>  niu_projectile_anim_frame_2

struct s_MISSILE
    Travel_Percent           ==>  travel_percent

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Battle_Unit_Attack.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Ranged_Animation.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Melee_Animation.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Battle_Unit_Moves2.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Set_Gibs.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Clear_Gibs.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Reload_Melee_Sound.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Make_Missiles.asm

OON XREF:  Battle_Unit_Attack() |-> Battle_Unit_Attack_Target() |-> Battle_Unit_Process_Attack()

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Battle_Unit_Attack()
            |-> Check_Attack_Ranged()                           done-done
            |-> Range_To_Battle_Unit()                          done-done
            |-> Battle_Unit_Has_Ranged_Attack()                 done-done
            |-> LBX_Load_Data_Static()                          platform
            |-> Warn1()                                         GUI
            |-> Battle_Unit_Attack_Target()                     done-done
            |-> Melee_Animation()
            |   |-> Set_Gibs()
            |   |-> Clear_Gibs()
            |   |-> Reload_Melee_Sound()
            |-> Ranged_Animation()
            |   |-> Range_To_Battle_Unit()                      done-done
            |   |-> Make_Missiles()
            |   |-> Set_Gibs()
            |   |-> Clear_Gibs()
            |-> Battle_Unit_Moves2()
            |-> Battle_Unit_Commit_Damage()                     done-done
            |-> Combat_Grid_Cell_Has_City_Wall()                done-done
            |-> Set_Page_Off()                                  platform
            |-> Combat_Screen_Draw()                            screen
            |-> PageFlip_FX()                                   platform

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open; no divergences. Eight functions compared, nine findings raised and closed, seven locals/globals renamed. Builds clean (2026-08-04).**

Nine reconstruction errors were raised across four functions (R1-R9) and all nine are closed, each re-read against the listings after the fix.

Four were behavioural gaps rather than transcription slips. The Wall Crusher ability rolled its 25%/50% and discarded the result, so walls were indestructible. Ranged attacks blocked by Invisibility or Wall of Darkness aborted only for the human player; AI attackers and auto-combat resolved an attack the original refuses. Nine of the thirteen projectile types fell through the switch and rendered as a lightning bolt with no sound — bows, slings, death bolts, ice bolts, priests, drow and sprites among them. And `Battle_Unit_Moves2` graded Wind Mastery against the constant `NUM_PLAYERS` instead of the live `_num_players`, letting a stale slot swing a transport's combat movement by 50%.

Two smaller ones were silent data corruption: `Make_Missiles` divided where the original takes a remainder, collapsing a volley onto one figure slot, and `Battle_Unit_Moves2` dropped `item_enchantments` from its enchantment union, losing item-granted Flight outright.

## Scope

Anchors are function **start** lines.

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Battle_Unit_Attack` | [Combat.c:17247](../../MoM/src/Combat.c#L17247) | [ovr122/Battle_Unit_Attack.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr122/Battle_Unit_Attack.asm) (854 lines) | faithful (R1-R4 fixed) |
| `Ranged_Animation` | [Combat.c:13202](../../MoM/src/Combat.c#L13202) | [ovr113/Ranged_Animation.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/Ranged_Animation.asm) (469 lines) + [ovr113/_misc.asm:29-57](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/_misc.asm#L29-L57) | faithful (R5 fixed) |
| `Melee_Animation` | [Combat.c:13461](../../MoM/src/Combat.c#L13461) | [ovr113/Melee_Animation.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/Melee_Animation.asm) (192 lines) | faithful |
| `Make_Missiles` | [Combat.c:27163](../../MoM/src/Combat.c#L27163) | [ovr163/Make_Missiles.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr163/Make_Missiles.asm) (198 lines) | faithful (R6 fixed) |
| `Battle_Unit_Moves2` | [Combat.c:20372](../../MoM/src/Combat.c#L20372) | [ovr124/Battle_Unit_Moves2.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr124/Battle_Unit_Moves2.asm) (301 lines) | faithful (R7-R9 fixed) |
| `Set_Gibs` | [Combat.c:20219](../../MoM/src/Combat.c#L20219) | [ovr124/Set_Gibs.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr124/Set_Gibs.asm) (145 lines) | faithful |
| `Clear_Gibs` | [Combat.c:20269](../../MoM/src/Combat.c#L20269) | [ovr124/Clear_Gibs.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr124/Clear_Gibs.asm) (21 lines) | faithful |
| `Reload_Melee_Sound` | [Combat.c:20102](../../MoM/src/Combat.c#L20102) | [ovr124/Reload_Melee_Sound.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr124/Reload_Melee_Sound.asm) (48 lines) | faithful |

**Already reviewed elsewhere:** `Battle_Unit_Attack_Target` in [Combat-Battle_Unit_Attack_Target.md](Combat-Battle_Unit_Attack_Target.md); `Check_Attack_Ranged` in the same doc; `Battle_Unit_Process_Attack` in [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md); `Battle_Unit_Commit_Damage`, `Range_To_Battle_Unit`, `Battle_Unit_Has_Ranged_Attack` and `Combat_Grid_Cell_Has_City_Wall` are all marked done-done. None were re-derived here.

**Not reviewed — next tier.** `Battle_Unit_Figure_Position`, `Combat_Grid_Screen_Coordinates` and `Get_Angle` (reached from `Make_Missiles`); `Combat_Screen_Draw`, `Set_Page_Off`, `PageFlip_FX`, `Warn1`, `LBX_Load_Data_Static`, `LBX_Reload_Next`, `Play_Sound`, `Mark_Block`, `Release_Block`, `Mark_Time`, `Release_Time` (platform and screen). Stated so the "faithful" verdicts above are not read as covering them.

`asm:N` refers to the listing named in the row above.

**Enum values come from `Piethawn/Piethawn/in/WIZARDS.inc`.** The `ovr*/` listings print IDA enum *symbols* (`rat_None`, `Prj_Arrow`, `rag_Magic`), never their numbers. `WIZARDS.inc` is the IDA database's own enum and struct definitions and carries the values — `rat_None = 255` at line 6911, `Prj_*` at 7481-7495, `Proj_*` at 7504-7511, `SFX_ATK_*` at 7159-7171, `rag_*` and `srat_*` around 6900. Use it rather than inferring a value from the symbol's name; D3 below is a case where the name pointed the wrong way and the value settled it.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared, and an emitted comparison is not treated as evidence of the expression that produced it — `if (A) X else Y` and `if (!A) Y else X` compile to the same bytes, so branch layout alone is never a finding.

---

# Findings

None open.

---

# Divergences

## D1 — declaration initialisers mask three original uninitialised reads

Production initialises every local at its declaration. In three places the listing does not, and the uninitialised value is reachable:

- **`defender_damage_total` / `attacker_damage_total`** in `Battle_Unit_Attack`. asm:33-35 jumps straight from the `defender_battle_unit_idx == 99` test to `@@Wall_Crusher`, skipping the zeroing at asm:281-282. `defender_damage_total` is then pushed to `Ranged_Animation` at asm:606 and both are pushed to `Melee_Animation` at asm:754-755 — stack garbage feeding `Set_Gibs`. Production reads `0` ([17255-17256](../../MoM/src/Combat.c#L17255-L17256)).
- **`endurance`** in `Battle_Unit_Moves2`. The frame slot `bp-2` is never written before the test at asm:275 unless a hero item sets it. Production reads `0` ([20379](../../MoM/src/Combat.c#L20379)). The `¿ NOBUG  as compiled, set to 0 ?` question at [20470-20471](../../MoM/src/Combat.c#L20470-L20471) is settled by the listing: the slot is *not* zeroed on entry, so the note above it is right.
- **`sound_buffer`** in `Ranged_Animation`. It lives in `di`; when no switch case matches and sound effects are enabled, nothing writes it (asm:304-306 only assigns `ST_UNDEFINED` when sound is *off*), so asm:374-378 tests and plays the caller's saved `DI`. Production reads `0` ([13211](../../MoM/src/Combat.c#L13211)) and adds a `/* HACK */` null guard at [13391](../../MoM/src/Combat.c#L13391).

These are the standard reconstruction convention, not slips. Listed so the divergence is on the record rather than discovered later.

## D2 — the two-value returns are HACKs, deliberate

`Play_Sound` and `LBX_Reload_Next` return a segment plus a length in the original's global `lbxload_entry_length`; production threads the length explicitly. Affects `Ranged_Animation` ([13211-13212](../../MoM/src/Combat.c#L13211-L13212), [13393](../../MoM/src/Combat.c#L13393)), `Melee_Animation` ([13466](../../MoM/src/Combat.c#L13466), [13495-13496](../../MoM/src/Combat.c#L13495-L13496)) and `Reload_Melee_Sound` ([20102](../../MoM/src/Combat.c#L20102)), all marked `/* HACK */`. Not findings.

## D3 — `rat_None` versus `rat_UNDEF` — resolved, not a divergence

`Battle_Unit_Attack.asm` stores the IDA enum symbol `rat_None` to `ranged_type` at asm:420, asm:457, asm:697 and asm:734. Production writes `rat_UNDEF`, which is `-1` ([UNITTYPE.h:227](../../MoM/src/UNITTYPE.h#L227)) — while `rat_NONE` is `0` ([UNITTYPE.h:228](../../MoM/src/UNITTYPE.h#L228)). The name looked like the wrong one had been picked.

It is the right one. The IDA enum defines

```
rat_None  = 255
```

at `Piethawn/Piethawn/in/WIZARDS.inc:6911`, and `ranged_type` is `int8_t` ([MOM_DAT.h:1913](../../MoX/src/MOM_DAT.h#L1913), [MOM_DAT.h:2014](../../MoX/src/MOM_DAT.h#L2014)). `255` and `-1` are the same byte, `0xFF`. Production's `rat_UNDEF` reproduces the store exactly, and the `¿ out of ammo / not enough mana ?` guess recorded at [UNITTYPE.h:227](../../MoM/src/UNITTYPE.h#L227) is confirmed — this function is where it comes from.

Consistent at all six such stores: [12074](../../MoM/src/Combat.c#L12074), [14294](../../MoM/src/Combat.c#L14294), [17385](../../MoM/src/Combat.c#L17385), [17394](../../MoM/src/Combat.c#L17394), [17465](../../MoM/src/Combat.c#L17465), [17474](../../MoM/src/Combat.c#L17474).

Note the stale copy at `STU-Extras/ReMoM/src/UNITTYPE.H:224`, which has `rat_None = -1` — that is an old snapshot of this project's own header, not ground truth. `WIZARDS.inc` is.

## D4 — `int` versus `void`: `xor ax, ax` at asm:103 is a compiler artifact

`@@JmpDone_Return` does `xor ax, ax` before `jmp @@Done`, which looks like `return 0`. It is not.

- **No other path sets AX for return.** The normal exit falls through `Set_Page_Off` / `Combat_Screen_Draw` / `PageFlip_FX` into `@@Done` (asm:843-847), leaving whatever `PageFlip_FX` returned. A function with a meaningful `int` return would set AX on both paths.
- **No caller reads a return value.** All nine call sites are bare statements: [Combat.c:3323](../../MoM/src/Combat.c#L3323), [3330](../../MoM/src/Combat.c#L3330), [3344](../../MoM/src/Combat.c#L3344), [3352](../../MoM/src/Combat.c#L3352), [3362](../../MoM/src/Combat.c#L3362), [CMBTAI.c:425](../../MoM/src/CMBTAI.c#L425), [460](../../MoM/src/CMBTAI.c#L460), [539](../../MoM/src/CMBTAI.c#L539), [2007](../../MoM/src/CMBTAI.c#L2007).

`void` is correct. Not a finding.

## D5 — the `DNE in Dasm` note at [17252](../../MoM/src/Combat.c#L17252) over-claims

`int16_t ranged_attack_flag = 0;  // DNE in Dasm, reuses range_to_target` asserts the original source had one variable. What the listing shows is one *frame slot*, `range_to_target__ranged_attack_flag= word ptr -14h` — storage sharing, which is a register-allocator outcome and never evidence of how many variables the source declared. Two locals compile to one slot whenever their live ranges are disjoint, and here they are. Advisory; the code is right either way.

---

# Battle_Unit_Attack

## Frame map

`sub sp, 18h` (asm:19) = 24 bytes, plus two register locals. Parameters occupy `bp+6` through `bp+0Ch`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-18h` | `Wall_Destroyed` | `Wall_Destroyed` |
| `bp-16h` | `ranged_attack_check` | `ranged_attack_check` |
| `bp-14h` | `range_to_target__ranged_attack_flag` | `range_to_target` and `ranged_attack_flag` (see D5) |
| `bp-12h` | `attacker_damage_array` (3 words) | `attacker_damage_array[3]` |
| `bp-0Ch` | `defender_damage_array` (3 words) | `defender_damage_array[3]` |
| `bp-6` | `defender_damage_total` | `defender_damage_total` |
| `bp-4` | `attacker_damage_total` | `attacker_damage_total` |
| `bp-2` | `itr_damage_type` | `itr_damage_type` |
| `bp+6` | `attacker_battle_unit_idx` | `attacker_battle_unit_idx` |
| `bp+8` | `defender_battle_unit_idx` | `defender_battle_unit_idx` |
| `bp+0Ah` | `cgx` | `cgx` |
| `bp+0Ch` | `cgy` | `cgy` |
| `si` | `_SI_attacker_battle_unit_idx` | (the parameter, used directly) |
| `di` | `_DI_defender_battle_unit_idx` | (the parameter, used directly) |

`cgx` and `cgy` are **overwritten from the defender** at asm:42-50 whenever `defender_battle_unit_idx != 99`. The caller's values survive only on the city-wall path. Production reproduces this at [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247); the `// WTF? same as was passed in?` note there is answered by the `== 99` case, where they are genuinely different.

## Faithful — verified, leave alone

- **AI immobility reset** — `controller_idx != HUMAN_PLAYER_IDX` sets `_ai_immobile_counter = ST_UNDEFINED` (asm:24-31). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **The melee-or-shoot decision at range ≤ 1** — `ranged > melee / 2` with the signed halving idiom `cbw / cwd / sub ax,dx / sar ax,1` (asm:144-164), then `Battle_Unit_Has_Ranged_Attack != 0` (asm:165-171), then the paired immunity test: `rag_Missile` against `USA_IMMUNITY_MISSILES` (asm:172-190) **or** `rag_Magic` against `USA_IMMUNITY_MAGIC` (asm:191-210). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247). The `; these checks are in the wrong logical order` note at [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) is faithful commentary on the original.
- **The `ranged_attack_check > 0` override** — `cmp 0 / jle` (signed) forces `ranged_attack_flag = ST_FALSE` (asm:221-223). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247). It applies to both the range ≤ 1 and range > 1 paths.
- **`Battle_Unit_Attack_Target` argument order** — pushes at asm:254-264 are `1`, `ranged_attack_flag`, `&attacker_damage_array`, `&defender_damage_array`, `defender`, `attacker`; right-to-left that is `(attacker, defender, defender_damage_array, attacker_damage_array, ranged_attack_flag, 1)`. The defender array is the **third** argument. Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Suppression increment on the defender** — byte `inc al` (asm:266-280). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **The damage-total fold** — one loop summing both arrays over three entries (asm:281-301). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Melee branch** — `Melee_Animation(attacker, defender, attacker_damage_total, defender_damage_total, defender.cgx, defender.cgy)` reading the coordinates from the **defender record**, not from `cgx`/`cgy` (asm:466-484); then `movement_points -= (Battle_Unit_Moves2(attacker) + 1) / 2` with the signed halving idiom (asm:485-508) and the `>= 0` clamp (asm:509-521). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Ranged branch** — `Ranged_Animation(attacker, defender, defender_damage_total, cgx, cgy)` (asm:306-312); `movement_points -= 20` plus clamp (asm:313-340); then the caster test `Hero_Slot > -1` **or** `Attribs_1 & (USA_CASTER_20 | USA_CASTER_40)`, **and** `ranged_type / 10 == rag_Magic` (asm:341-375) selecting the mana path over the ammo path. Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Ranged resource thresholds differ between the two ranged blocks, deliberately.** The main block disables the attack at `mana < 3` (asm:413, `jnb`); the Wall Crusher block at `mana < 5` (asm:690, `jnb`). Both are unsigned. Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) and [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247). Do not normalise them.
- **The caster test also differs between the two blocks.** The main block accepts a hero **or** a `USA_CASTER_*` unit (asm:352-360); the Wall Crusher block accepts **only** a hero (asm:636-638) — no `Attribs_1` test at all. Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) and [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Ammo path** — `ammo -= 1`, then `ammo <= 0` disables `ranged_type` and `ranged` (asm:429-463 and asm:706-740). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) and [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Damage commit order** — defender first, then attacker (asm:522-534). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Wall Crusher entry gate** — three separate early exits to `@@JmpDone_DrawFlip`: `Abilities & UA_WALLCRUSHER`, `controller_idx != _combat_defender_player` (with `cbw` widening the signed byte before the word compare), and `Combat_Grid_Cell_Has_City_Wall(cgx, cgy) != 0` (asm:536-564). Production's `&&` chain at [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) compiles to exactly this.
- **Wall Crusher, `defender != 99`** — the adjacency test only, no animation and no movement cost: `|Δcgx| > 1 || |Δcgy| > 1` rolls `Random(100) <= 25`, else `<= 50` (asm:786-828). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Wall Crusher, `defender == 99`** — the same adjacency split, but each arm also runs the full animation and resource spend: the far arm rolls 25%, calls `Ranged_Animation`, spends 20 movement **without a clamp**, and pays mana or ammo (asm:569-742); the near arm rolls 50%, calls `Melee_Animation` and spends `(Battle_Unit_Moves2 + 1) / 2`, again **without a clamp** (asm:743-783). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247). The two missing clamps are the original's, not an omission.
- **Message loads** — `LBX_Load_Data_Static(message_lbx_file__ovr122, 0, GUI_NearMsgString, 3, 1, 150)` for Invisibility and index `4` for Wall of Darkness (asm:84-96, asm:117-129). Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) and [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).

## OGBUGs — faithful, do not fix

- **A ranged attack does not end the unit's turn.** Both ranged paths subtract a flat `20` (asm:319, asm:617) instead of zeroing `movement_points`. Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) and [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247), both annotated. The v1.2 patch notes claim otherwise.
- **The melee-or-shoot decision at range ≤ 1 tests strength before capability.** `ranged > melee / 2` is evaluated before `Battle_Unit_Has_Ranged_Attack`, so a unit whose `ranged` stat is set but whose attack is unusable still takes the ranged arm's immunity branch. Annotated at [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247); the function header at [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247) calls the whole decision inconsistent.
- **The `rag_Magic` immunity test ignores Righteousness.** asm:204-210 tests `Attribs_1 & USA_IMMUNITY_MAGIC` and nothing else. Annotated at [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **Wall Crusher against a unit gets no animation.** The `defender != 99` arm rolls the dice and stops (asm:786-828) — no `Melee_Animation`, no `Ranged_Animation`, no movement cost, where the `== 99` arm does all three. Production [Battle_Unit_Attack](../../MoM/src/Combat.c#L17247).
- **The Wall Crusher ranged and melee arms spend movement without clamping to zero**, unlike the main attack paths (asm:611-625, asm:768-783). `movement_points` can go negative there.

---

# Ranged_Animation

Faithful.

**The projectile switch dispatches on `ranged_type - 10` through a 29-entry table** at [ovr113/_misc.asm:29-57](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/_misc.asm#L29-L57), so `index = line - 29` and `ranged_type = index + 10 = line - 19`. Thirteen live cases: 10 `rat_Rock`, 11 `rat_Cannon`, 20 `rat_Bow`, 21 `rat_Sling`, 30 `rat_Lightning`, 31 `rat_Fireball`, 32 `rat_Sorcery`, 33 `rat_Deathbolt`, 34 `rat_Icebolt`, 35 `rat_Pr_Shaman`, 36 `rat_Drow`, and 37 `rat_Sprite` / 38 `rat_Nat_Bolt` sharing one body at `loc_908B4`. Values are [UNITTYPE.h:234-252](../../MoM/src/UNITTYPE.h#L234-L252). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202).

**There is no `default:` clause.** `loc_908D8` is asm:303, and asm:304 is `cmp [magic_set.sound_effects], e_ST_FALSE` — the statement *after* the switch. It is the break target; an unmatched `ranged_type` simply skips the switch.

**Two traps for anyone re-deriving this table.** IDA auto-names switch targets `sw_jt_rat_NN` where `NN` is the **table index**, not the case value — `sw_jt_rat_10` is index 10, hence `ranged_type` 20. The shared label `sw_jt_rat_02_03_04_05_06_07_08_09_12_13_14_15_16_17_18_19` enumerates the unmatched indices, and 10 and 11 are deliberately absent from it. Separately, the `Prj_*` to `msl_*` mapping is value-verified, not name-matched: `WIZARDS.inc:7481-7495` defines all fifteen `Prj_*` constants and each equals its `msl_*` counterpart at [Combat.h:90-104](../../MoM/src/Combat.h#L90-L104); the sound constants likewise match one-for-one between `WIZARDS.inc:7159-7171` and [SOUND.h:237-249](../../MoX/src/SOUND.h#L237-L249).

- **Range derivation** — `Range_To_Battle_Unit` when the target is a unit (asm:21-28); a Chebyshev max of the two `abs` deltas against `cgx`/`cgy` when it is the city wall (asm:73-99). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202). The listing reuses the `travel_percent_step` slot as the `dist_x` temporary; production's separate `dist_x`/`dist_y` locals are storage sharing, not a divergence.
- **Travel step** — `33` when `range >= 3` **and** movement animations are on, else `49` (asm:100-109). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202).
- **Switch bounds** — `ranged_type - 10` bounds-checked `jbe 28` (asm:127-131). Unsigned, so `ranged_type < 10` wraps and fails the check. Out-of-range values jump to `loc_908D8`, the instruction after the switch — there is no `default:` clause.
- **`Make_Missiles` argument order** — for a unit target, `(attacker.figure_cnt, defender.figure_cnt, attacker.cgx, attacker.cgy, defender.cgx, defender.cgy, missile_type)` (asm:308-342); for the city wall, `Targets` is the literal `1` and the destination is `cgx`/`cgy` (asm:343-359). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202) and [Ranged_Animation](../../MoM/src/Combat.c#L13202).
- **The travel loop** — the final step sets both frame counters to `4` and calls `Set_Gibs(defender, Target_Damage_Sum)` only when the target is a unit; every other step advances `(frame + 1) % 3` (asm:382-410). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202).
- **Per-missile advance** — `travel_percent += travel_percent_step` clamped at `100` with a `jle` test, over `m_missile_count` entries (asm:414-442). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202).
- **Teardown** — `m_missile_count = 0`, `Clear_Gibs` only for a unit target, `Release_Block(World_Data)` only when sound effects are on (asm:450-461). Production [Ranged_Animation](../../MoM/src/Combat.c#L13202).

`projectile_anim_frame` and its shadow `niu_projectile_anim_frame_2` are advanced as a pair throughout, in both the `% 3` path and the `= 4` path (asm:372-373, 387-388, 399-410). Only the first is ever consumed — `ovr153/Combat_Screen_Map_Draw_Entities__WIP.asm:323` reads it, and the shadow's only read is its own self-increment. Both stores are faithful; keep them.

# Melee_Animation

Faithful.

- `anim_frame_count` is `5` when movement animations are on, `1` otherwise (asm:21-26). Production [Melee_Animation](../../MoM/src/Combat.c#L13461).
- `Set_Gibs(attacker, attacker_damage)` runs **before** the `defender == 99` split; `Set_Gibs(defender, defender_damage)` only inside the unit arm (asm:28-41). Production [Melee_Animation](../../MoM/src/Combat.c#L13461).
- The unit arm sets `Melee_Anim = 2` on the defender and cross-assigns all four target coordinates — defender's target to the attacker's cell, attacker's target to the defender's cell (asm:42-103). The wall arm sets only the attacker's target, from `cgx`/`cgy` (asm:105-119). Production [Melee_Animation](../../MoM/src/Combat.c#L13461).
- `Melee_Anim = 1` on the attacker is set **after** the split, on both paths (asm:120-126). Production [Melee_Animation](../../MoM/src/Combat.c#L13461).
- The frame loop is `Mark_Time` / `Set_Page_Off` / `Combat_Screen_Draw` / `PageFlip_FX` / `Release_Time(2)` (asm:143-158). Production [Melee_Animation](../../MoM/src/Combat.c#L13461).
- Teardown clears the attacker's gibs unconditionally, the defender's and its `Melee_Anim` only for a unit, then the attacker's `Melee_Anim` (asm:159-179). Production [Melee_Animation](../../MoM/src/Combat.c#L13461).

# Make_Missiles

Faithful.

- **Angle sectors** — `< 22 || > 337` → `Proj_Right`; then `< 69` `Proj_DnRight`, `< 112` `Proj_Down`, `< 158` `Proj_DnLeft`, `< 202` `Proj_Left`, `< 248` `Proj_UpLeft`, `< 292` `xor di, di`, else `Proj_UpRight` (asm:98-136). `Proj_Up` is `0` in both `WIZARDS.inc:7504` and [Combat.h:109](../../MoM/src/Combat.h#L109), so the `xor` is the `Proj_Up` case. The whole `e_msl__dir` enum matches the IDA enum value-for-value (`WIZARDS.inc:7504-7511`). Production [Make_Missiles](../../MoM/src/Combat.c#L27163).
- **`Get_Angle` takes `(Δx, Δy)`** — asm:88-93 pushes the Y delta first, so the X delta is the first argument. Production [Make_Missiles](../../MoM/src/Combat.c#L27163).
- **The origin is not the source figure position.** `Origin_X` is the raw cell screen X and `Origin_Y` the cell screen Y plus 8, captured *before* the figure offsets are folded into `Src_Screen_*` (asm:73-87). The angle is measured from that origin to the fully offset target. Production [Make_Missiles](../../MoM/src/Combat.c#L27163).
- Both `Src_Fig_Y` and `Tgt_Fig_Y` are decremented by 8 before use (asm:73-74). Production [Make_Missiles](../../MoM/src/Combat.c#L27163).
- `travel_percent` starts at `10`, not `0` (asm:178). Production [Make_Missiles](../../MoM/src/Combat.c#L27163).
- `m_missile_count = missile_count` is set once, after the loop (asm:191-192). Production [Make_Missiles](../../MoM/src/Combat.c#L27163).

# Battle_Unit_Moves2

Faithful.

- **The enchantment union is three-source** — `_UNITS[unit_idx].enchantments | battle_units[].enchantments | battle_units[].item_enchantments`, assembled through two `push`/`pop` half-word swaps (asm:39-64). Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372) writes the same three terms in a different order; `|` is commutative and every operand is a side-effect-free load, so load scheduling is not evidence of source order.
- **The Wind Mastery loop bound is the global `_num_players`** ([MOM_DAT.h:4240](../../MoX/src/MOM_DAT.h#L4240)), not a constant (asm:219). Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).
- **The Endurance bonus is one short-circuit `||`** — `(enchantments & UE_ENDURANCE) != 0 || endurance == ST_TRUE` (asm:265-278). There is no store to `[bp+endurance]` outside the hero-item loop at asm:136. Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).

- **Web short-circuit** — `Web_HP > 0` returns `0` (asm:23-27). Here `xor ax, ax` **is** a real return value: the normal exit does `mov ax, _SI_moves2` (asm:292) and the function is `int16_t`. Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).
- **Hero item scan** — three slots (`cmp itr, 3`, asm:153), each `> -1`, testing `_ITEMS[].enchantments & IP_Endurance` in the **low** word with the high word masked to zero (asm:126-134) and accumulating `_ITEMS[].moves2` (asm:138-149). `ip_Endurance` is index 13 in `e_ITEM_POWER` ([MOM_DEF.h:1004-1017](../../MoX/src/MOM_DEF.h#L1004-L1017)), so `1 << 13` = `0x2000` — a low-word bit, matching the listing. Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).
- **The hero-item loop does not OR item enchantments into the accumulator**, unlike `Unit_Moves2`. Confirmed absent from asm:110-154, and it costs nothing — see the analysis below.
- **Base moves** — `_unit_type_table[].Move_Halves` plus `item_moves2` (asm:156-174). Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).
- **Wind Mastery** — gated on `_unit_type_table[].Transport > 0` (asm:190); `+1` when the controller matches a wizard with the global set, `-1` otherwise (asm:195-216); `> 0` scales `moves2 * 3 / 2` and `< 0` scales `moves2 / 2`, both with the signed halving idiom (asm:221-237). Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).
- **Flight floors** — `moves2 < 6` with `UE_FLIGHT` raises to `6`; `moves2 < 4` with `CC_FLIGHT` in `_UNITS[].mutations` raises to `4` (asm:238-263). Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372). `UE_FLIGHT` is `0x00020000` ([MOM_DEF.h:938](../../MoX/src/MOM_DEF.h#L938)), a high-word bit — matching the listing's `and ax, UE_FLIGHT` with `and dx, 0`.
- **Haste doubles last**, after every other modifier (asm:280-290). Production [Battle_Unit_Moves2](../../MoM/src/Combat.c#L20372).

# Set_Gibs

Faithful.

`gibs = 3` for a wiped unit, else `(Damage + figure_cnt / 2) / (figure_cnt * 2)` with the signed halving idiom (asm:23-55); clamped to `5` (asm:57-59); forced to `1` when `Damage > 0` left it at `0` (asm:61-65). `figure_loss = (Damage + hits - 1) / hits` capped at `figure_cnt`, computed only when `Damage > 0` (asm:67-110). Twenty `Random(4) - 1` values into the blood-frame table (asm:112-126). The two unit fields are written last (asm:127-139). Production [Set_Gibs](../../MoM/src/Combat.c#L20219).

The listing's `CMB_BloodFrames` is production's `m_gibs_frames`.

# Clear_Gibs

Faithful. Two stores, `Atk_FigLoss = 0` and `gibs = 0`. Production [Clear_Gibs](../../MoM/src/Combat.c#L20269).

# Reload_Melee_Sound

Faithful.

Low nibble of `_unit_type_table[].Sound` (`mov ah, 0` / `and ax, 0Fh`, asm:23-25) selects the sound; index `8` loads `SFX_Silence` from `soundfx_lbx_file__ovr124__1of2`, everything else loads that index from `cmbtsnd_lbx_file__ovr124` (asm:27-41). Production [Reload_Melee_Sound](../../MoM/src/Combat.c#L20102). The `sound_seg_size` out-parameter is D2.

---

## In-source markers reconciled

Every `TODO` / `HACK` / `DNE` / `DEDU` / `BUGBUG` marker inside the eight reviewed functions, and what this review concluded about it.

| marker | verdict |
| --- | --- |
| [Combat.c:20399](../../MoM/src/Combat.c#L20399) — `ITEM_POWER(hero_items[itr], ip_Endurance)` | **Redundant but faithful.** `UE_ENDURANCE` is already in `item_enchantments` from [14765-14767](../../MoM/src/Combat.c#L14765-L14767), so this second check can only re-confirm what the union already carries. The original does it twice (asm:126-136); leave it. |
| [Combat.c:13226](../../MoM/src/Combat.c#L13226) — `range = dist_y;  // DNE in Dasm  BUGBUG?` | **Not a bug.** asm:83-99 reuses the `travel_percent_step` frame slot as the `dist_x` temporary, then `if (dist_x > range) range = dist_x`. Production's separate `dist_x` / `dist_y` locals are storage sharing, which is never evidence of source structure. Semantics match. |
| [Combat.c:13238](../../MoM/src/Combat.c#L13238), [13242](../../MoM/src/Combat.c#L13242) — `DEDU  99 / 3 ?` and `99 / 2 ?` | **Unresolvable from the bytes.** asm:106 and asm:109 store the bare literals `49` and `33`. The `99` derivation is consistent with the loop bound at asm:446 (`cmp [bp+Progress_Counter], 100` — the IDA frame label, unaffected by the production rename), but the listing contains no division — the guess can be neither confirmed nor refuted from the disassembly. |
| [13212](../../MoM/src/Combat.c#L13212), [13391](../../MoM/src/Combat.c#L13391), [13466](../../MoM/src/Combat.c#L13466), [20102](../../MoM/src/Combat.c#L20102) — `HACK` | Covered by **D2**. The original returns a segment plus a length in `lbxload_entry_length`; production threads the length explicitly. Deliberate. |
| [Combat.c:17252](../../MoM/src/Combat.c#L17252) — `ranged_attack_flag = 0;  // DNE in Dasm` | Covered by **D5**. One frame slot, `range_to_target__ranged_attack_flag= word ptr -14h`. Storage sharing, not proof the source had one variable. The comment over-claims; the code is right. |
| [Combat.c:20105](../../MoM/src/Combat.c#L20105) — `sound_num  // DNE in Dasm` | Same class as D5. asm:23-26 computes the nibble into `si` with no separate slot. Not a divergence. |

### Why the `Unit_Moves2` asymmetry is not an OGBUG

`Battle_Unit_Moves2`'s hero-item loop does not OR `_ITEMS[].enchantments` into its accumulator; `Unit_Moves2` does. That difference is real and will look wrong to anyone re-deriving this function. It loses nothing.

`Unit_Moves2` ([NEXTTURN.c:1529](../../MoM/src/NEXTTURN.c#L1529)) ORs raw `_ITEMS[].Powers` at [1556](../../MoM/src/NEXTTURN.c#L1556) and then tests `UE_FLIGHT` at [1568](../../MoM/src/NEXTTURN.c#L1568) and `UE_ENDURANCE` at [1581](../../MoM/src/NEXTTURN.c#L1581). That is only sound because the `ip_` and `UE_` bit spaces coincide:

| power | `ip_` index | `1 << ip_` | `UE_` constant |
| --- | --- | --- | --- |
| Endurance | 13 ([MOM_DEF.h:1017](../../MoX/src/MOM_DEF.h#L1017)) | `0x00002000` | `UE_ENDURANCE` = `0x00002000` ([MOM_DEF.h:934](../../MoX/src/MOM_DEF.h#L934)) |
| Flight | 17 ([MOM_DEF.h:1021](../../MoX/src/MOM_DEF.h#L1021)) | `0x00020000` | `UE_FLIGHT` = `0x00020000` ([MOM_DEF.h:938](../../MoX/src/MOM_DEF.h#L938)) |

The two functions reach the same result by different routes, because they hold different data:

- `Unit_Moves2` has only a `_UNITS[]` entry — no battle-unit record — so it must read `_ITEMS[].Powers` directly.
- `Battle_Unit_Moves2` has the battle-unit record, so it reads the pre-folded `battle_units[].item_enchantments`. `BU_Apply_Item_Powers` ([Combat.c:14634](../../MoM/src/Combat.c#L14634)) fills that field by walking the *same* three hero slots ([14651-14655](../../MoM/src/Combat.c#L14651-L14655)) and mapping `ip_Endurance` → `UE_ENDURANCE` ([14765-14767](../../MoM/src/Combat.c#L14765-L14767)) and `ip_Flight` → `UE_FLIGHT` ([14773-14775](../../MoM/src/Combat.c#L14773-L14775)).

`Battle_Unit_Moves2` consumes exactly those two bits from the union and both are mapped, so nothing is dropped. **Not an OGBUG.** ORing `_ITEMS[].Powers` in the loop would set the same bits a second time.

This is also what R7 was really about: until the `item_enchantments` term was restored to the union at [20385](../../MoM/src/Combat.c#L20385), item Flight and item Endurance genuinely never reached the function. The defect was in the union, not in the loop.

**Unverified:** that `BU_Apply_Item_Powers` always runs before `Battle_Unit_Moves2` is called. It is reached from `BU_Init_Battle_Unit`, deferred to a general BU-init pass, so the ordering has not been traced. If that ordering does not hold, the conclusion above needs revisiting.

## Build state

**Clean.** `cmake --build --preset MSVC-debug` run 2026-08-04, twice — once with all nine fixes in place, and again after the seven renames in the ledger at the top of this file. The second build rippled the `s_MISSILE.travel_percent` field rename through `NEXTTURN.c`, `UnitMove.c`, `UnitStat.c`, `Spells129/131/133.c`, `CMBTAI.c` and `CMBMAGIC.c`; every target linked, no warnings and no errors.

The nine fixes are the user's, applied across four passes; each was re-read against the listings afterwards. The set adds a `battlefield->walls[][]` write, a `%` operator, a struct-field OR, a global read and three `switch` labels, and moves two `return` statements out of their inner `if`.

**`Make_Missiles` Doxygen corrected.** [Combat.c:27151-27152](../../MoM/src/Combat.c#L27151-L27152) and [Combat.c:27161](../../MoM/src/Combat.c#L27161) described `Targets` as a divisor, left over from before R6; both now say modulus, matching the code at [Combat.c:27181](../../MoM/src/Combat.c#L27181). Comment-only, inside a reconstruction annotation block with no asm counterpart — no fidelity impact.

**Header prototypes checked.** [Combat.h:1859](../../MoM/src/Combat.h#L1859) (`Battle_Unit_Attack`), [1742](../../MoM/src/Combat.h#L1742) (`Ranged_Animation`), [1745](../../MoM/src/Combat.h#L1745) (`Melee_Animation`), [1953](../../MoM/src/Combat.h#L1953) (`Battle_Unit_Moves2`), [1929](../../MoM/src/Combat.h#L1929) (`Reload_Melee_Sound`), [1938](../../MoM/src/Combat.h#L1938) (`Set_Gibs`), [1941](../../MoM/src/Combat.h#L1941) (`Clear_Gibs`), [2167](../../MoM/src/Combat.h#L2167) (`Make_Missiles`) — all match their definitions in parameter count, order and type.

**Anchors.** All `Combat.c` references were re-verified on 2026-08-04 after the last fix landed. The nine fixes shifted the file unevenly — `Battle_Unit_Attack` by +66 lines, `Melee_Animation` by +79, `Battle_Unit_Moves2` by +41 — so the per-line cites in the **Faithful** sections had drifted into unrelated functions and have been re-pointed at their **function start**, which the scope table verifies. The **Divergences** section keeps line-precise cites, each re-derived by grepping for the construct it names. `asm:N` references are stable and were never affected; they remain the precise reference for anything in this doc.

## Related references

- [Combat-Battle_Unit_Attack_Target.md](Combat-Battle_Unit_Attack_Target.md) — the callee this function drives; DONE-DONE.
- [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md) — one level further down; DONE-DONE.
- [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) — `Check_Wall_Of_Fire_Attack`; DONE-DONE.
- `Battle_Unit_Action` — the caller. Not yet reviewed.
