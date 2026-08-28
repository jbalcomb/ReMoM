Combat-AI_Turn.md

WZD ovr114 CMBTAI.c

SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoX-Combat-Auto.md  (Auto, not just AI)

NOTE: drake178 defined 'player mode' (~AI_Player_Mode()) as {0:Hopeless, 1:Losing, 2:Disadvantage, 3:Advantage, 4:Winning, 5:Certain}, but 3 includes where the enemy is stronger, so went with the more generic "level" interpretation

AI_SetBasicAttacks__WIP()  ==>  AI_Set_All_Action_Modes()
Get_Player_Mode()          ==>  AI_Player_Mode()
Retreat_Check()            ==>  AI_Retreat_Check()
AI_BU_ProcessAction()      ==>  AI_Execute_Unit_Action()
AI_BU_AssignAction()       ==>  AI_Set_Unit_Action_Mode()
AI_GetCombatRallyPt()      ==>  AI_Stage_Point_For_Unit()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\Auto_Do_Combat_Turn.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\AI_SetBasicAttacks__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\Sort_Battle_Units.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\AI_GetCombatRallyPt.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\AI_BU_ProcessAction.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\AI_BU_AssignAction.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr114\Choose_Target_And_Action.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Get_Player_Mode.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Retreat_Check.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Target_Unit_Value.asm

OON XREF:  Auto_Do_Combat_Turn() |-> AI_Stage_Point_For_Unit()

OON XREF:  Choose_Target_And_Action() |-> Target_Unit_Value()

OON XREF:  AI_Set_Unit_Action_Mode() |-> Choose_Target_And_Action()

OON XREF:  AI_Execute_Unit_Action() |-> AI_Set_Unit_Action_Mode()

Combat_Screen()
Combat_Next_Turn()
    |-> Auto_Cast_Spell_And_Do_Combat_Turn()
        |-> Auto_Do_Combat_Turn()
            |-> AI_Set_All_Action_Modes()
                |-> AI_Player_Mode()

Check_For_Winner()
    |-> AI_Retreat_Check()
        |-> AI_Player_Mode()
                |-> AI_Player_Mode()

Check_For_Winner()
    |-> AI_Retreat_Check()
        |-> AI_Player_Mode()
            |-> Sort_Battle_Units()
            |-> AI_Stage_Point_For_Unit()
            |-> AI_Execute_Unit_Action()
                |-> AI_Set_Unit_Action_Mode()
                    |-> Choose_Target_And_Action()
                        |-> Target_Unit_Value()

Auto_Do_Combat_Turn()
    |-> Total_Ranged_Attack_Strength()
    |-> AI_Set_All_Action_Modes()
        |-> AI_Player_Mode()
    |-> Sort_Battle_Units()
    |-> AI_Stage_Point_For_Unit()
    |-> Switch_Active_Battle_Unit()
    |-> Assign_Combat_Grids()
    |-> AI_Set_Unit_Action_Mode()
    |-> Battle_Unit_Is_Within_City()
    |-> AI_Execute_Unit_Action()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-27).** No findings open. Thirteen functions, 5,689 asm lines, all walked end to end against the listings. R15, R16 and R17 are applied. `Auto_Move_Unit`'s reused parameter pair was split into `rally_cgx`/`rally_cgy` plus `max_x`/`max_y` locals on 2026-08-27 and is recorded as D8 - the seed ahead of the quadrant test is load-bearing and must not be removed. The two `Mark_Time()` / `Release_Time(1)` calls in `Auto_Move_Unit`'s animation loop are a deliberate, owned port-side pacing hack marked `/* HACK */`; they have no counterpart in `ovr114/Auto_Move_Unit.asm` and are recorded under *Listing notes*, not as a defect. Fourteen findings were raised and all fourteen are fixed and verified in the current sources, so the findings section is empty. Builds clean. Nine of the ten were homeless before this doc.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Auto_Move_Unit` | [CMBTAI.c:1478](../../MoM/src/CMBTAI.c#L1478) | `ovr114/Auto_Move_Unit.asm` | 1,022 | **full body walk — faithful** |
| `Do_Auto_Unit_Turn` | [CMBTAI.c:1369](../../MoM/src/CMBTAI.c#L1369) | `ovr114/Do_Auto_Unit_Turn.asm` | 229 | **full body walk — faithful** |
| `Total_Ranged_Attack_Strength` | [Combat.c:15314](../../MoM/src/Combat.c#L15314) | `ovr124/Ranged_Attack_Strength.asm` | 58 | **full body walk — faithful** |
| `Combat_Next_Turn` | [Combat.c:3727](../../MoM/src/Combat.c#L3727) | `ovr098/Combat_Next_Turn.asm` | 53 | **full body walk — D2** |
| `AI_Execute_Unit_Action` | [CMBTAI.c:251](../../MoM/src/CMBTAI.c#L251) | `ovr114/AI_Execute_Unit_Action.asm` | 1,097 | **full body walk — faithful** |
| `Choose_Target_And_Action` | [CMBTAI.c:1052](../../MoM/src/CMBTAI.c#L1052) | `ovr114/Choose_Target_And_Action.asm` | 844 | **full body walk — faithful** |
| `Target_Unit_Value` | [Combat.c:13672](../../MoM/src/Combat.c#L13672) | `ovr122/Target_Unit_Value.asm` | 584 | **full body walk — faithful** |
| `AI_Set_Unit_Action_Mode` | [CMBTAI.c:892](../../MoM/src/CMBTAI.c#L892) | `ovr114/AI_BU_AssignAction.asm` | 418 | **full body walk — faithful** |
| `Auto_Do_Combat_Turn` | [CMBTAI.c:694](../../MoM/src/CMBTAI.c#L694) | `ovr114/Auto_Do_Combat_Turn.asm` | 407 | **full body walk — faithful** |
| `AI_Set_All_Action_Modes` | [CMBTAI.c:104](../../MoM/src/CMBTAI.c#L104) | `ovr114/AI_SetBasicAttacks__WIP.asm` | 368 | **full body walk — faithful** |
| `AI_Stage_Point_For_Unit` | [CMBTAI.c:585](../../MoM/src/CMBTAI.c#L585) | `ovr114/AI_GetCombatRallyPt.asm` | 240 | **full body walk — faithful** |
| `AI_Player_Mode` | [Combat.c:15709](../../MoM/src/Combat.c#L15709) | `ovr124/Get_Player_Mode.asm` | 182 | **full body walk — faithful** |
| `AI_Retreat_Check` | [Combat.c:16414](../../MoM/src/Combat.c#L16414) | `ovr124/Retreat_Check.asm` | 107 | **full body walk — faithful** |
| `Sort_Battle_Units` | [CMBTAI.c:544](../../MoM/src/CMBTAI.c#L544) | `ovr114/Sort_Battle_Units.asm` | 102 | **full body walk — faithful** |

## What belongs here, and why

This is the **AI combat turn**: everything that happens when one side is played by the computer, from posture decisions down to a single unit choosing whom to hit. The boundary is deciding and executing unit actions — not casting spells, and not the screen.

The cohort is a closed tree with one entry point. `Auto_Do_Combat_Turn` has exactly one caller, [Combat.c:3666](../../MoM/src/Combat.c#L3666), inside `Auto_Cast_Spell_And_Do_Combat_Turn`. Below it every edge is internal and single-source: `AI_Execute_Unit_Action` is the only caller of `AI_Set_Unit_Action_Mode` (five sites, [CMBTAI.c:373](../../MoM/src/CMBTAI.c#L373) through [CMBTAI.c:509](../../MoM/src/CMBTAI.c#L509)), and `AI_Set_Unit_Action_Mode` is the only caller of `Choose_Target_And_Action` ([CMBTAI.c:969](../../MoM/src/CMBTAI.c#L969), [CMBTAI.c:921](../../MoM/src/CMBTAI.c#L921)). `AI_Set_All_Action_Modes`, `Sort_Battle_Units` and `AI_Stage_Point_For_Unit` are each called only from `Auto_Do_Combat_Turn`. All seven live in `ovr114` and in [CMBTAI.c](../../MoM/src/CMBTAI.c).

`AI_Set_All_Action_Modes` and `AI_Stage_Point_For_Unit` carried checked tracker boxes before this review, but those recorded a rename verification only, not a fidelity review, which is why they are in scope here.

### What does *not* belong here

- **`Auto_Cast_Spell_And_Do_Combat_Turn`** ([Combat.c:3652](../../MoM/src/Combat.c#L3652)), the caller. It is done-done in [Combat-Combat_Screen.md](Combat-Combat_Screen.md) and sits in `ovr098`, not `ovr114`. Its listing is named in the header block above for call-tree context only; it renders no verdict here. **Note:** the tracker still carries a second, stale row for it under its old name `AI_CMB_PlayTurn__WIP` with a wrong line number.
- **`AI_Select_Combat_Spell`** and the rest of `ovr139`, owned by [Combat-Spell_Cast_AI_Select.md](Combat-Spell_Cast_AI_Select.md). The AI's spell choice is a separate layer that this one calls into.
- **`Do_Auto_Unit_Turn`** and **`Auto_Move_Unit`** — the other two `ovr114` slots. They are the overland auto-move path and attribute to [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md).
- **`Set_Movement_Cost_Map`**, **`Combat_Move_Path_Find`**, **`Total_Ranged_Attack_Strength`**, **`Battle_Unit_Is_Within_City`**, **`Switch_Active_Battle_Unit`**, **`Assign_Combat_Grids`** — helpers with callers across several reviews.

## Findings

None outstanding. Fourteen were raised during this review and all fourteen are fixed and verified in the current sources; the history is in git.

## Deviations

## D2 — `Combat_Next_Turn` drops the `winner` local

`Combat_Next_Turn.asm` opens `sub sp, 2` for a single slot IDA names `winner`, and asm:27-30 is `call j_Check_For_Winner / mov [bp+winner], ax / cmp [bp+winner], ST_UNDEFINED / jnz @@Done` — the result is stored to the slot and then compared *from memory*. That is the two-statement form, `winner = Check_For_Winner();` followed by `if (winner == ST_UNDEFINED)`. Production at [Combat.c:3758](../../MoM/src/Combat.c#L3758) collapses both into one expression and declares no local, so the frame has nothing in it. Behaviour is identical; the structure is not.


### D1 - `AI_Stage_Point_For_Unit` adds a target-validity guard

[CMBTAI.c:683-688](../../MoM/src/CMBTAI.c#L683-L688) returns `(0, 0)` when `target_battle_unit_idx` is `ST_UNDEFINED`. **The listing has no such test.** asm:140-179 runs straight from the vortex loop into the argument pushes, reading `target_battle_unit_idx` as a signed byte at asm:147 and asm:159 and indexing `battle_units[]` with it both times:

```
mov     al, [es:bx+s_BATTLE_UNIT.target_battle_unit_idx]
cbw
mov     dx, size s_BATTLE_UNIT
imul    dx
les     bx, [battle_units]
add     bx, ax
push    [es:bx+s_BATTLE_UNIT.cgy]
```

With `-1` that is `battle_units[-1]`. On the original hardware it read garbage coordinates; on a modern host it is out of bounds. The guard is a host-safety addition of the same family as the `999` stand-in in [Combat-Combat_Spell_Target_Screen.md](Combat-Combat_Spell_Target_Screen.md), and the `OGBUG` note above it correctly identifies the root cause as ordering in the caller - `Auto_Do_Combat_Turn` computes the stage point at [CMBTAI.c:868](../../MoM/src/CMBTAI.c#L868) *before* `AI_Set_Unit_Action_Mode` has assigned fresh targets, so the chosen melee unit can still be holding `ST_UNDEFINED`.

Recorded rather than proposed for removal: the original behaviour here is genuinely undefined, so no test should pin either result.

### D2 - three of `Sort_Battle_Units`' locals are byte-wide in the listing

`current_battle_unit_moves2` at `bp-6`, `previous_battle_unit_moves2` at `bp-5` and `current_battle_unit_idx` at `bp-1` are all **`byte ptr`**, and `previous_list_idx` lives in `cl` rather than a full register. [CMBTAI.c:552](../../MoM/src/CMBTAI.c#L552) declares all five `int16_t`.

Widths are facts, unlike slot counts, so this is a genuine narrowing the reconstruction dropped - the OG source declared those three as `char`. Nothing observable changes: `movement_points` is itself a byte field, battle-unit indices run 0..35, and the loop's `cmp previous_list_idx, -1` terminates identically at either width. Recorded so the frame map and the declarations can be reconciled if the widths are ever restored.

### D3 - pointer aliases where the listing recomputes the index

Production introduces `bu_ptr`, `target_ptr`, `vortex_ptr` and similar in five of the seven functions, taking `&battle_units[itr]` once and dereferencing through it. The listing recomputes `imul size s_BATTLE_UNIT` / `les bx, [battle_units]` at every single access - for example four times in the twelve lines at asm:142-177 of `AI_Stage_Point_For_Unit`. The aliases are a readability restructure over the same accesses and change nothing observable.

### D4 - `AI_Set_Unit_Action_Mode` is called with an action enum for a boolean flag

[CMBTAI.c:889](../../MoM/src/CMBTAI.c#L889) and [CMBTAI.c:864](../../MoM/src/CMBTAI.c#L864) read `AI_Set_Unit_Action_Mode(itr_battle_units, bua_Ready)`, where the parameter is `no_spells_flag`. `bua_Ready` is `0` ([Combat.h:821](../../MoM/src/Combat.h#L821)) and asm:274-275 is `xor ax, ax` / `push ax`, so **the value is right**. Only the spelling is misleading - the five call sites inside `AI_Execute_Unit_Action` pass plain `0` and `1` for the same parameter. Worth changing to `ST_FALSE` or `0`; it is not a fidelity error.

### D5 - the melee equality arm gains a `target_idx` guard

[CMBTAI.c:1280](../../MoM/src/CMBTAI.c#L1280) reads `else if(target_value == highest_value && target_idx > -1)`. **The listing has no such test on this path** - asm:751 jumps straight from the equality compare into the health computation at `loc_9410D`, which indexes `battle_units[target_battle_unit_idx]` at asm:784 onward.

The only `cmp [bp+target_battle_unit_idx], -1` in the whole listing is at asm:656, inside the **ranged** branch, where production reproduces it correctly at [CMBTAI.c:1281](../../MoM/src/CMBTAI.c#L1281).

Reaching the unguarded path needs `target_value == highest_value` while `target_idx` is still `ST_UNDEFINED`, i.e. `Target_Unit_Value` returning exactly `-100` on a first candidate - so `battle_units[-1]`. Same host-safety family as D1: harmless on the original hardware, out of bounds here. Recorded rather than proposed for removal.

### D6 - two `STU_DEBUG_BREAK()` guards with no asm counterpart

[CMBTAI.c:462](../../MoM/src/CMBTAI.c#L462) traps on `bua_Healing`; [CMBTAI.c:552](../../MoM/src/CMBTAI.c#L552) traps in the `default` arm. The listing has neither - jump-table slot 6 and the out-of-range `jmp` at asm:61 both go to `jt_bua_06` and clamp silently.

Both are deliberate and both stay, on the same reasoning already recorded as D2 in [Combat-Combat_Spell_Animation.md](Combat-Combat_Spell_Animation.md): `STU_DEBUG_BREAK()` expands to `( (void)0 )` in release ([STU_DBG.h:15](../../STU/src/STU_DBG.h#L15)), so shipped behaviour is byte-identical to the original, and in Debug they surface exactly the conditions this cohort keeps producing.

They are not equivalent guards, though:

- **The `bua_Healing` trap catches a live OGBUG.** That action really is assigned, and the original really does drop it. Without the trap the failure is invisible - the unit just wastes its turn.
- **The `default` trap is a corruption detector.** It cannot fire on the normal path: `AI_Set_Unit_Action_Mode` folds `bua_Ready` and `bua_No_Spells` into `bua_Shoot` / `bua_Stab` before storing ([CMBTAI.c:958-968](../../MoM/src/CMBTAI.c#L958-L968)), so `action` is always inside 100..110 by the time this function sees it. Anything reaching `default` means the field was corrupted elsewhere.

### D7 - two `ST_UNDEFINED` guards added inside the focus-fire loop

[CMBTAI.c:989-997](../../MoM/src/CMBTAI.c#L989-L997) adds two `continue` guards before the loop dereferences `battle_units[itr_bu_ptr->target_battle_unit_idx]` and `battle_units[target_battle_unit_idx]`. **The listing has neither** - asm:324-330 goes straight from the controller check into reading `target_battle_unit_idx` and indexing with it.

Both prevent `battle_units[-1]`, the same host-safety family as D1 and D5, and both are already marked in place. Note the `OGBUG` label on them reads as though the guard *is* the bug; they are guards *against* an original defect.

### D8 - `Auto_Move_Unit`'s reused parameter pair is split into four variables

The listing reuses the two incoming parameter slots as the movement box's max corner. `Max_X= word ptr 0Eh` and `Max_Y= word ptr 10h` arrive holding the **rally point** - [CMBTAI.c:1473](../../MoM/src/CMBTAI.c#L1473) passes `rally_cgx, rally_cgy`, and the fallback call at [CMBTAI.c:1465](../../MoM/src/CMBTAI.c#L1465) passes `stomp_cgx, stomp_cgy` for both the destination and the rally arguments specifically to make the equality test at asm:236-239 true. After that test the same two slots are overwritten as the box max, consumed only by the path-cell bounds test at asm:468 and asm:482.

The seven stores are at asm:243, 245, 264, 273, 279, 288, 301 and 311, and the four reads that feed `Min_X` / `Min_Y` at asm:271, 286, 299 and 309 - each on a path where its slot has not yet been written.

The listing writes `e_COMBAT_GRID_WIDTH` and `e_COMBAT_GRID_HEIGHT` where production writes `COMBAT_GRID_XMAX` and `COMBAT_GRID_YMAX`. Not a divergence: [Combat.h:143-153](../../MoM/src/Combat.h#L143-L153) gives both pairs the same values, 21 and 22.

Production splits this into `rally_cgx` / `rally_cgy` (parameters, now read-only) and `max_x` / `max_y` (locals, `DNE in Dasm`). Two slots in the original, four variables in the port.

**The seed at [CMBTAI.c:1590-1591](../../MoM/src/CMBTAI.c#L1590-L1591) is what makes the split faithful, and it must not be removed.** Three paths through the quadrant selection never store a max corner at all - branch A (`delta_x < delta_y`) with `rp_origin_y_2 < dst_cgy` leaves Y unwritten, branch B (`delta_x > delta_y`) with `rp_origin_x_2 < dst_cgx` leaves X unwritten, and branch C (`delta_x == delta_y`) leaves either. On those paths the original's box max **is** the rally coordinate, reached by falling through without a store. Without `max_x = rally_cgx; max_y = rally_cgy;` ahead of the test, those paths would run with a max of 0 and collapse the movement box.

The four `min_x = Max_X` / `min_y = Max_Y` statements are reads of the *incoming* value on every path that reaches them - no branch writes the slot before them - so they became `min_x = rally_cgx` / `min_y = rally_cgy`. That is the same value and it makes the real semantic visible: on those branches the rally coordinate becomes the box's **min** bound, which the shared `Max_` name hid completely.

Behaviour is unchanged on every path. Recorded because the target is 1:1 with the listing and this is a genuine structural difference, not a rename.

## Verified faithful

### `AI_Retreat_Check` ([Combat.c:16414](../../MoM/src/Combat.c#L16414), asm 107)

Faithful apart from R5.

**What it decides.** Whether the computer concedes the battle. It is consulted once per `Check_For_Winner` call, behind a guard that makes it AI-only - [Combat.c:7335-7340](../../MoM/src/Combat.c#L7335-L7340) requires `_combat_remote_player != NEUTRAL_PLAYER_IDX`, no wizard city siege, and past turn one. A human player never reaches it.

**It gives up immediately unless the level is 0.** asm:17-21 calls `AI_Player_Mode` and returns `ST_FALSE` unless the answer is 0 - [Combat.c:17110-17114](../../MoM/src/Combat.c#L17110-L17114). Everything below that line only runs at mode 0, which is why **R3 and R4 reach into this function**: R4 shifts the boundary between modes 2 and 3, and R3 can make `AI_Player_Mode` divide by zero instead of returning 5.

The listing calls it near - asm:17 is `call near ptr Get_Player_Mode`, both functions living in `ovr124`.

**The tally accepts two statuses.** asm:32-40 is `status == bus_Active` **or** `status == bus_Uninvolved` - `jz` to the body on the first, `jnz` past on the second - so units that have not joined the fight still count as at risk. [Combat.c:17120](../../MoM/src/Combat.c#L17120).

**Three counters, one pass.** Every own-controlled unit increments `Risked_Unit_Count`; `Hero_Slot > -1` increments the DI hero tally at asm:62-64; and `(Abilities & UA_CREATEOUTPOST) || (Construction > 0)` increments `Builder_Count` at asm:71-81 - [Combat.c:17127-17141](../../MoM/src/Combat.c#L17127-L17141). The builder test is a genuine `||`: `jnz` **to** the increment on the ability bit, then `jle` past it on the Construction value.

**Two independent reasons to flee.** asm:89-90 is `or di, di` / `jg` - any hero at all - and asm:91-93 is `Risked_Unit_Count == Builder_Count`, i.e. nothing left but non-combatants. [Combat.c:17152](../../MoM/src/Combat.c#L17152) and [Combat.c:17147](../../MoM/src/Combat.c#L17147). Both fall into the shared `@@JmpDone_Return_TRUE` tail at asm:94, which is Borland's cross-jump; writing the two returns out separately is the correct reconstruction.

**Ownership.** Its only caller, `Check_For_Winner` ([Combat.c:7288](../../MoM/src/Combat.c#L7288)), is **not** AI-specific and belongs with [Combat-Combat.md](Combat-Combat.md) - it answers "is this combat over" for both sides and is called from eleven sites across the screen loop and turn advance. The boundary between the two docs therefore cuts across that call edge, which is deliberate: `AI_Retreat_Check` is reached only from a function owned elsewhere, and nothing else calls it.

### `AI_Player_Mode` ([Combat.c:15709](../../MoM/src/Combat.c#L15709), asm 182)

Faithful apart from R3 and R4. **This one is not exclusive to the cohort** - see the ownership note below.

**What it returns.** A level on a six-point scale, **0 to 5**, where 0 is the worst position for `player_idx` and 5 the best. Levels 0 and 5 are produced by the two early guards; 1 through 4 come from the ratio ladder. Writing `r` for `enemy_effective_strength / own_effective_strength`, and noting that every rung but the middle one uses **integer** division so only the truncated quotient matters:

| level | condition | meaning in ratio terms |
| --- | --- | --- |
| 0 | `floor(r) >= 4`, or own side has no strength or no attack | enemy at least 4x |
| 1 | `floor(r) == 3` | enemy 3x to just under 4x |
| 2 | `r >= 1.5` | enemy 1.5x to just under 3x |
| 3 | everything else | roughly `1/3 < r < 1.5` - from you 3x stronger, through parity, to the enemy half again stronger |
| 4 | `floor(1/r) == 3` | you 3x to just under 4x |
| 5 | `floor(1/r) >= 4`, or enemy has no strength or no attack | you at least 4x |

**The scale is asymmetric, and level 3 is not a good position.** The enemy needs only **1.5x** to push you off level 3, while you need a full **3x** to climb off it - so the AI registers trouble at twice the sensitivity it registers success, and level 3 covers being mildly outmatched as well as being ahead. Rung 3 is the only one that cross-multiplies (`shl ax, 1` against `imul 3` at asm:145-149) rather than dividing, which is how the original expresses a fractional 1.5 threshold that integer division cannot reach; every other rung is limited to whole-number ratios, which is why the winning side's scale jumps straight from 1x to 3x.

Because 0, 1, 4 and 5 all demand whole-number ratios of 3 or more, **levels 2 and 3 are where most real battles sit** - which is why the `switch` in `AI_Set_All_Action_Modes` folds 0 and 1 into one arm and gives 2 an elaborate one of its own.

drake178 labelled the six {Hopeless, Losing, Disadvantage, Advantage, Winning, Certain} and that naming is echoed in the tree at [Combat.c:16349](../../MoM/src/Combat.c#L16349), [Combat.c:17474](../../MoM/src/Combat.c#L17474) and [Combat.c:17508](../../MoM/src/Combat.c#L17508). **This review does not use those names** - "Advantage" for a band that includes being 1.5x outmatched is actively misleading. Levels are referred to by number throughout.

**Frame - order matches.** `enemy_attack_strength` `-0Ah`, `own_attack_strength` `-8`, `effective_strength` `-6`, `Threat_Ratio_Level` `-4`, `enemy_effective_strength` `-2`, with `itr` in SI and `own_effective_strength` in DI. [Combat.c:16350-16356](../../MoM/src/Combat.c#L16350-L16356) declares them in that order.

**The tally filter has an unusual disjunction.** asm:28-48 is `status == bus_Active` **and** any of `_combat_turn < 2`, `target_battle_unit_idx != ST_UNDEFINED`, or `controller_idx == HUMAN_PLAYER_IDX` - [Combat.c:16363-16373](../../MoM/src/Combat.c#L16363-L16373). So on turns 0 and 1 every active unit counts, and after that only units that have picked a target or belong to the human. That is what stops a freshly-deployed AI stack from reading as a threat before it has oriented.

**Two accumulators per side.** `Effective_Battle_Unit_Strength(itr)` into the effective total, and raw `melee + ranged` into the attack total - asm:56-96, [Combat.c:16383-16389](../../MoM/src/Combat.c#L16383-L16389).

**The ratio ladder is five rungs and a default**, tested in order: `enemy/own > 3` gives 0, `> 2` gives 1, the cross-multiplied `enemy*2 >= own*3` gives 2, `own/enemy > 3` gives 5, `> 2` gives 4, else 3 - asm:127-172 against [Combat.c:16403-16426](../../MoM/src/Combat.c#L16403-L16426). Only the middle rung uses cross-multiplication rather than division, which is how the original avoids a third divide; it is also the rung R4 gets wrong.

**Both divides are safe in the original.** `idiv own_effective_strength` at asm:130 and asm:138 is protected by the first guard, and `idiv [bp+enemy_effective_strength]` at asm:158 and asm:166 by the second. R3 removes the second protection.

**Ownership.** Three callers, in two files: `AI_Set_All_Action_Modes` ([CMBTAI.c:162](../../MoM/src/CMBTAI.c#L162)) here, `AI_Retreat_Check` ([Combat.c:17115](../../MoM/src/Combat.c#L17115)) which is `o124p18` and still homeless, and `AI_Select_Combat_Spell` ([Combat.c:17446](../../MoM/src/Combat.c#L17446)) which belongs to [Combat-Spell_Cast_AI_Select.md](Combat-Spell_Cast_AI_Select.md). It is adjudicated here by decision rather than by call graph: two of the three callers are combat-turn AI, and this is the only review that analyses what the value is used *for*. If `AI_Retreat_Check` later lands somewhere else, this verdict still stands and should not be duplicated.

### `Sort_Battle_Units` ([CMBTAI.c:544](../../MoM/src/CMBTAI.c#L544), asm 102)

Faithful, all 102 lines, apart from the widths in D2.

**It is an ascending insertion sort on `movement_points`** - slowest unit first. The shift condition at asm:77-79 is `previous_battle_unit_moves > current_battle_unit_moves`, so equal keys do not move and the sort is stable.

**Frame - order matches.** `current_battle_unit_moves2` `bp-6`, `previous_battle_unit_moves2` `bp-5`, `itr` `bp-4`, `current_battle_unit_idx` `bp-1`, `sub sp, 6`, with `troop_list` in SI and `previous_list_idx` in CL. [CMBTAI.c:552](../../MoM/src/CMBTAI.c#L552) declares them in that order.

**The outer loop starts at 1.** asm:16 is `mov [bp+itr], 1` and asm:92-95 tests `cmp ax, [bp+troop_count]` / `jge` - [CMBTAI.c:600](../../MoM/src/CMBTAI.c#L600).

**The inner test is two-part and short-circuits on the index first.** asm:74-79 is `cmp previous_list_idx, -1` / `jle` to exit, then the moves comparison - so the source is `previous_list_idx > -1 && previous > current`, matching [CMBTAI.c:613](../../MoM/src/CMBTAI.c#L613) written as `>= 0`. The ordering matters because `previous_battle_unit_moves` is only refreshed inside the guarded reload below, so the stale value must never be reached.

**The reload is guarded separately.** asm:60-73 decrements, re-tests `cmp previous_list_idx, -1` / `jle` to skip, and only then reloads - [CMBTAI.c:619-623](../../MoM/src/CMBTAI.c#L619-L623).

**The element size is two bytes throughout.** Every subscript is `shl ax, 1` on the sign-extended index, so `troop_list` really is `int16_t *` - asm:49, asm:56, asm:65, asm:87. The one place the listing loads a byte, asm:21, is the store into the byte-wide `current_battle_unit_idx`, not a narrower array.

**The placement is outside the loop.** asm:80-90 writes `troop_list[previous_list_idx + 1] = current_battle_unit_idx` once, after the shifting ends - [CMBTAI.c:627](../../MoM/src/CMBTAI.c#L627).

### `AI_Stage_Point_For_Unit` ([CMBTAI.c:585](../../MoM/src/CMBTAI.c#L585), asm 240)

Faithful apart from D1.

**No stack frame.** `push bp` / `mov bp, sp` with no `sub sp` - only `battle_unit_idx` in DI and `itr` in SI. Production's five locals have no slots, which says nothing either way about the original source; the compiler eliminates locals freely.

**The early-out takes two actions.** asm:18 and asm:25 test `bua_MoveAndStab` then `BUA_RangedAttack`, returning the unit's own cell - [CMBTAI.c:593-598](../../MoM/src/CMBTAI.c#L593-L598). IDA's `BUA_RangedAttack` is `101`, the same value production calls `bua_Shoot` ([Combat.h:826](../../MoM/src/Combat.h#L826)).

**Three writes into the cost map, in the listing's order.** The target's cell gets `2`, other active units get `INF`, then every vortex cell gets `INF` - asm:78, asm:106, asm:135 against [CMBTAI.c:660](../../MoM/src/CMBTAI.c#L660), [CMBTAI.c:617](../../MoM/src/CMBTAI.c#L617) and [CMBTAI.c:674](../../MoM/src/CMBTAI.c#L674). The target test comes first, so a target that is also active still ends up at `2`.

**`Combat_Move_Path_Find` takes four words.** Pushes at asm:153, 165, 171, 177 are target cgy, target cgx, own cgy, own cgx, so the source order is `(own_cgx, own_cgy, target_cgx, target_cgy)` with `add sp, 8` - [CMBTAI.c:689-694](../../MoM/src/CMBTAI.c#L689-L694).

**The path scan skips step 0 and stops when the next step would exhaust movement.** asm:185-188 is the bound test then `or _SI_itr, _SI_itr` / `jz` to continue - [CMBTAI.c:645-651](../../MoM/src/CMBTAI.c#L645-L651). asm:216-217 is `sub dx, ax` / `jg` to continue, so the source breaks on `move_points - cost <= 0` - [CMBTAI.c:846](../../MoM/src/CMBTAI.c#L846).

**The cost lookup really is a grid-offset index.** asm:199-213 computes `_cmbt_mvpth_y[itr-1] * COMBAT_GRID_WIDTH + _cmbt_mvpth_x[itr-1]` and adds that to `_cmbt_mvpth_c` - so `_cmbt_mvpth_c` is addressed by cell, not by path step, even though the two arrays beside it are stepwise. [CMBTAI.c:707-708](../../MoM/src/CMBTAI.c#L707-L708) has it right. The `mov ah, 0` at asm:214 is a **zero**-extend, which is why production's `cost` is `unsigned char`.

**Both exits converge.** asm:218 is reached from the loop-bound `jge` and by falling out of the movement test, and both then write `_cmbt_mvpth_x[itr-1]` / `_cmbt_mvpth_y[itr-1]` through the two out-pointers - [CMBTAI.c:734](../../MoM/src/CMBTAI.c#L734).

**`movement_points` is re-read every iteration in the listing** (asm:189-194) where production hoists it to [CMBTAI.c:697](../../MoM/src/CMBTAI.c#L697). Nothing writes it inside the loop, so the hoist is invisible.

### `Auto_Do_Combat_Turn` ([CMBTAI.c:694](../../MoM/src/CMBTAI.c#L694), asm 407)

Walked end to end, all 407 lines. R1 and R2 are open against it; nothing else diverges.

**The `else` at [CMBTAI.c:779-782](../../MoM/src/CMBTAI.c#L779-L782) is the defender declining to shelter, not the attacker.** asm:144 (`loc_9336D`) is reached from asm:96 - the `_ai_stay_in_city != ST_TRUE` branch - and the whole block is already inside `player_idx == _combat_defender_player`. The attacker leaves at asm:90-92 without touching `_ai_battlefield_city_walls` at all. Worth stating because the arm reads like an attacker case and is not one.

**The stuck-neutral cleanup is three-way.** asm:19-24 is `player_idx == NEUTRAL_PLAYER_IDX`, `player_idx == _combat_attacker_player`, `_ai_immobile_counter > 3`, then a loop that kills every active neutral unit outright - [CMBTAI.c:760-772](../../MoM/src/CMBTAI.c#L760-L772).

**Ranged strength is compared across sides, not accumulated.** Both `+=` at [CMBTAI.c:846](../../MoM/src/CMBTAI.c#L846) and [CMBTAI.c:779](../../MoM/src/CMBTAI.c#L779) start from a zeroed local, so they are effectively assignments; the listing does the same.

**The wall bitmask is rebuilt from the battlefield every turn** - stone, darkness, fire - [CMBTAI.c:787-799](../../MoM/src/CMBTAI.c#L787-L799), and then cleared outright for the attacker at [CMBTAI.c:783](../../MoM/src/CMBTAI.c#L783).

**Two independent conditions make a defender abandon the walls.** Overwhelming enemy ranged with none of our own at [CMBTAI.c:771](../../MoM/src/CMBTAI.c#L771), or any hostile vortex / Wrack / Call Lightning / Mana Leak at [CMBTAI.c:827-830](../../MoM/src/CMBTAI.c#L827-L830). Both clear `_ai_battlefield_city_walls` and `_ai_stay_in_city`.

**The stage point is only computed when this side is not out-ranged.** asm guards the whole block on `our >= their`; the else writes `(0, 0)` at asm:228-230 - [CMBTAI.c:800-824](../../MoM/src/CMBTAI.c#L800-L824).

**The median melee unit supplies the stage point.** asm:214-223 computes `(Melee_Unit_Count - 1) / 2` with the `cwd` / `sub ax, dx` / `sar ax, 1` signed-halve idiom, scales by two for the `int16_t` element and indexes `Melee_Unit_List` - [CMBTAI.c:868](../../MoM/src/CMBTAI.c#L868). Because `Sort_Battle_Units` sorts ascending on movement points, that is the *median-speed* unit.

**Two passes, and the second is heroes only.** Pass one takes active, mobile, non-sleeping units - [CMBTAI.c:863-888](../../MoM/src/CMBTAI.c#L863-L888); pass two revisits the same list and acts only on units with `Hero_Slot > -1`, after setting `_ai_disable_hero_melee_safety_check` - [CMBTAI.c:890-907](../../MoM/src/CMBTAI.c#L890-L907). Each unit gets `Switch_Active_Battle_Unit`, `Assign_Combat_Grids`, `AI_Set_Unit_Action_Mode`, then `AI_Execute_Unit_Action`.

**A defender caught outside its own walls is processed with no stage point.** [CMBTAI.c:875-877](../../MoM/src/CMBTAI.c#L875-L877) passes `(0, 0)` in that case, which `AI_Execute_Unit_Action` reads as "use the target's own position" - see [CMBTAI.c:261-267](../../MoM/src/CMBTAI.c#L261-L267).

**The wall bitmask constants match.** asm:73, asm:80 and asm:87 OR in `1`, `4` and `2` for walled, darkness and fire respectively - against `BATTLEFIELD_CITY_WALL_STONE` `0x1`, `_DARKNESS` `0x4` and `_FIRE` `0x2` at [Combat.h:38-40](../../MoM/src/Combat.h#L38-L40). Note the listing's order is stone, darkness, fire, which is not the numeric order of the constants; production follows the listing.

**The three combat-enchantment indices resolve correctly.** The listing addresses named struct members - `s_COMBAT_ENCHANTMENTS.Wrack.Attkr`, `.Call_Lightning.Attkr`, `.Mana_Leak.Attkr` at asm:129, asm:132 and asm:135 - where production indexes a flat `int8_t *` with `WRACK_ATTKR` 8, `CALL_LIGHTNING_ATTKR` 18 and `MANA_LEAK_ATTKR` 26 ([Combat.h:906](../../MoM/src/Combat.h#L906), [916](../../MoM/src/Combat.h#L916), [924](../../MoM/src/Combat.h#L924)). `s_COMBAT_ENCHANTMENTS` is `0x1E` bytes of two-byte `s_COMBAT_ENCHANTMENT_STATUS` entries, which puts `Wrack` at member 4 (byte 8), `Call_Lightning` at member 9 (byte 18) and `Mana_Leak` at member 13 (byte 26). All three line up.

**The abandon-walls tests are `> 30` and `== 0`.** asm:119 is `cmp [bp+Their_Last_Ranged_Str], 30` / `jle` to skip and asm:121 is `cmp [bp+Our_Last_Ranged_Str], 0` / `jnz` to skip - [CMBTAI.c:771](../../MoM/src/CMBTAI.c#L771). The four-way hazard test that follows is a flat `||` with the Mana Leak arm carrying an extra `player_idx < _num_players` at asm:137 - [CMBTAI.c:827-830](../../MoM/src/CMBTAI.c#L827-L830).

**The ranged-strength helper is renamed, not different.** asm:55 and asm:65 call `j_Ranged_Attack_Strength`; production calls `Total_Ranged_Attack_Strength` ([Combat.c:15908](../../MoM/src/Combat.c#L15908)), which is `o124p01` - the same `ovr124` function under its production name.

**The melee-list filter is three-deep.** `controller_idx == player_idx`, `status == bus_Active`, then `action == bua_Stab || action == bua_MoveAndStab` - asm:166-190, [CMBTAI.c:855-861](../../MoM/src/CMBTAI.c#L855-L861). The store is `Melee_Unit_List[Melee_Unit_Count]` with `shl bx, 1` for the two-byte element at asm:192-196.

**The vortex tally reuses the unit-loop register.** asm:101-117 walks `_vortexes[]` in SI, the same register pass one later uses for battle units. Production gives it a separate `itr_vortexes` local - storage sharing, not a divergence.

**Pass two's filter is five flat tests to one target.** asm:334-372 checks controller, status, movement points, `Hero_Slot > -1` (with `shl ax, 5` for the `s_UNIT` stride at asm:359-361) and Black Sleep, all branching to `loc_935B4`. Production nests it as a 3-way `if` around a 2-way `if` at [CMBTAI.c:842-845](../../MoM/src/CMBTAI.c#L842-L845); when the inner `if` is the whole body of the outer, Borland emits exactly this, so the two forms are indistinguishable here and the nesting is not a divergence.

## Provenance

When this review was written, five of the seven bodies carried a `/* GEMINI */` marker - `AI_Set_All_Action_Modes`, `AI_Execute_Unit_Action`, `AI_Stage_Point_For_Unit`, `AI_Set_Unit_Action_Mode` and `Choose_Target_And_Action` - identifying them as machine translations rather than hand reconstructions. **Those markers have since been removed from the source**, so the provenance now lives only here. It matters because machine translations are second opinions, not ground truth: everything asserted in this doc was checked against the `.asm`, never taken from the translated body.

The one fully walked translated body, `AI_Stage_Point_For_Unit`, came out **accurate on every operand** - including the non-obvious one, that `_cmbt_mvpth_c` is indexed by grid cell while the two arrays beside it are indexed by path step. Its only departure from the listing is the deliberate, commented guard in D1. That is a good sign for the other four but not a substitute for walking them.

R1 is the counter-example: declaration order is wrong in two of the five translated bodies and in `Auto_Do_Combat_Turn`, which was not one of them, so the marker never predicted where the problems were.

---

# The turn-driver additions

Four `NOT DONE` slots this document already named in its call tree. Walked 2026-08-25. Three are complete; the fourth is explicitly unfinished and marked so.

## `Combat_Next_Turn` ([Combat.c:3727](../../MoM/src/Combat.c#L3727), asm 53)

Faithful in every call and every guard, with one deviation (D2 below). The sequence is: clear `_scanned_battle_unit` and the two frame flags, `Set_Mouse_List(1, mouse_list_default)`, clear the local-turn flag, and if the AI has not yet taken its turn re-clear both frame flags and run it. Then the winner check; if the battle continues, `Begin_Combat_Turn()`, the defender-is-AI turn, set the local-turn flag, and the auto-combat turn.

Renames in play: `CMB_HumanTurn` is `m_turn_is_local`, `CMB_AIGoesFirst` is `m_cp_took_turn`, `j_CMB_PrepareTurn__WIP` is `Begin_Combat_Turn`, and the two player globals follow the `_combat_remote_player` / `_combat_local_player` pair already recorded in [Combat-Combat_Screen.md](Combat-Combat_Screen.md).

## `Total_Ranged_Attack_Strength` ([Combat.c:15314](../../MoM/src/Combat.c#L15314), asm 58)

Faithful, all 58 lines. IDA calls it `Ranged_Attack_Strength`. No stack locals — `battle_unit_idx` is SI and `amount` is DI, and `xor di, di` at asm:11 is the explicit `amount = 0;` statement production writes after the declarations.

Three nested guards — `status == bus_Active` (asm:22), `controller_idx == player_idx` (asm:29-31), then `Battle_Unit_Has_Ranged_Attack(...)` tested with `or ax, ax / jz` for `!= ST_FALSE` (asm:37-38).

**The OG bug is real and production carries it correctly.** asm:44-46 is `mov al, [es:bx+s_BATTLE_UNIT.ranged] / cbw / mov amount, ax` — a plain assignment inside the loop, so the running total is discarded and only the last qualifying unit's `ranged` survives. Production writes `=`, not `+=`, and the in-code note says what it should have been and how that was deduced. Faithful.

## `Do_Auto_Unit_Turn` ([CMBTAI.c:1369](../../MoM/src/CMBTAI.c#L1369), asm 229)

Faithful, all 229 lines.

**Declaration order is exact.** Five stack locals from `-0Ah` up — `stomp_cgy`, `stomp_cgx`, `path_count`, `city_area_cgy`, `city_area_cgx` — declared in that order, then the register local `itr_battle_units`.

**The stomp-around guard is a three-clause mixture and production reproduces it clause for clause.** asm:131-144: `cmp target, ST_UNDEFINED / jle` to the second test, then `cmp [map], INF / jz` into the body, then `cmp target, ST_UNDEFINED / jz` into the body, then `jmp @@Done`. That is `((target > ST_UNDEFINED) && (map[...] == INF)) || (target == ST_UNDEFINED)`, with `&& controller_idx == _combat_attacker_player` appended at asm:148-155.

**The search bounds are the city block.** `city_area_cgx` runs 5..8 (asm:158, 198) and `city_area_cgy` runs 10..13 (asm:161, 194) — the same `MIN_CGX_CITY`..`MAX_CGX_CITY` / `MIN_CGY_CITY`..`MAX_CGY_CITY` span that `Update_Move_Map_City_Perimeter_Restrictions` fences. `path_count` seeds at 999 and the three winner stores happen in the order `stomp_cgx`, `stomp_cgy`, `path_count` (asm:185-190), which production keeps.

Both `Auto_Move_Unit` calls check out from push order: the stomp call passes `(battle_unit_idx, stomp_cgx, stomp_cgy, ST_UNDEFINED, stomp_cgx, stomp_cgy)` and the closing call passes the parameters through unchanged.

## `Auto_Move_Unit` ([CMBTAI.c:1478](../../MoM/src/CMBTAI.c#L1478), asm 1,022)

Walked end to end. Two findings, both one-character inversions; everything else is faithful.

**The frame is exact.** Nineteen stack locals from `-26h` up, declared in production in that order: `move_anim_speed`, `First_Step_Index`, `move_sound_seg` (asm `sound_buffer`), `Move_Visible`, `delta_y` (asm `Y_Distance`), `delta_x` (asm `X_Distance`), `Min_Y`, `Min_X`, `RP_Origin_Y_2`, `RP_Origin_X_2`, `First_Step`, `Attack_Step`, `Last_Target_Y`, `Last_Target_X`, `Origin_Y`, `Origin_X`, `Facing_Y_Offset` (asm `facing_cgy`), `Facing_X_Offset` (asm `facing_cgx`), `itr_battle_units`. The production locals that follow — `itr_grid` (DI), `DBG_path_cost`, `move_sound_seg_size` (marked `DNE in Dasm`), `movement_points`, `DBG_last_movement_points` — are register-class or instrumentation.

**asm:1-155** duplicates the parent's whole move-map setup. drake178's note on the original calls it out — *"recreates the same move map and path as the parent function ... this is one horrible way of doing things"* — and the duplication is genuine.

**The quadrant clamp (asm:236-311) is three mutually exclusive `if`s, not an `if`/`else if` chain.** `delta_x < delta_y`, `delta_x > delta_y` and `delta_x == delta_y` are tested independently at asm:260, 275 and 290, each falling through to the next. Production writes three separate `if`s. `COMBAT_GRID_WIDTH` and `COMBAT_GRID_XMAX` are both 21, and `COMBAT_GRID_HEIGHT` and `COMBAT_GRID_YMAX` are both 22, so the listing's `e_COMBAT_GRID_WIDTH` / `e_COMBAT_GRID_HEIGHT` and production's `COMBAT_GRID_XMAX` / `COMBAT_GRID_YMAX` are the same values.

**The invisibility guard is a four-term `||` with a trailing `&&`, and every mask is on the correct half.** asm:319-357 tests `enchantments`, `item_enchantments`, `Abilities` and `_UNITS[].enchantments`; the three 32-bit tests all use `and dx, UE_INVISIBILITY / and ax, 0`, i.e. the **low** word. asm:365-366 appends `&& controller_idx != HUMAN_PLAYER_IDX`.

**The illusions test resolves correctly.** asm:374-394: `loc_947D7` sets `Move_Visible = ST_TRUE` and `loc_947DE` sets it `ST_FALSE`. Tracing both clauses, the source is `if ((controller_idx != attacker && !attacker_sees) || (controller_idx != defender && !defender_sees)) Move_Visible = ST_FALSE; else Move_Visible = ST_TRUE;`, and the `jz loc_947B8` at asm:375 is the `||` short-circuit into the second clause. Production matches, including the outer `else` that sets `ST_TRUE` for visible units.

**The move loop's exit condition is tail-merged with the movement-point check, and production's `break` form is right.** asm:817-853: the loop test `itr_grid < movement_path_grid_cell_count` and the `First_Step != ST_TRUE && movement_points <= cost` check both jump to the same exit label, so Borland merged them ahead of the body. Production writes the second as a leading `if (...) break;` inside the body — the un-merged source form.

**Two `itr_grid` adjustments that look wrong and are not.** After the attack step, asm:602-603 is `dec _DI_itr_grid / jmp loc_94BFD`, and `loc_94BFD` is the `inc` — net zero, so the same path cell is retried. Production's `itr_grid--` before the `for`'s `itr_grid++` is the same thing, and it sits outside the `figure_cnt` if/else exactly as the listing's convergence point does.

**The out-of-box exit is a `break`, not a `continue`,** and production already has it right with the reasoning in place: asm:507 `jmp loc_94C51` leaves the loop entirely, and [CMBTAI.c:1847](../../MoM/src/CMBTAI.c#L1847) breaks, under the comment *"a path cell outside the movement box that is not the destination ends the move loop."*

**The tail (asm:854-1015) checks out call for call:** `move_anim_ctr = 0`, `mid_move = ST_FALSE`, the `itr_grid > 1` split for `Origin_*` and `Last_Target_*` (with the shared `Last_Target_Y` store tail-merged in the listing and written out in both arms in production), the two facing offsets, the `itr_grid != 0` position update, and the `MV_TELEPORT || MV_MERGING` movement-point split — `-= 2` on one side, the path-cost subtraction on the other — then `return ST_TRUE`.

# Listing notes — `Auto_Move_Unit`

- **The movement-animation loop carries an owned pacing hack.** [CMBTAI.c:1819](../../MoM/src/CMBTAI.c#L1819) and [CMBTAI.c:1823](../../MoM/src/CMBTAI.c#L1823) call `Mark_Time()` and `Release_Time(1)`; `ovr114/Auto_Move_Unit.asm` calls neither. The loop body itself is faithful — `move_anim_ctr += move_anim_speed` (asm:711-720), `Combat_Screen_Draw` (asm:721), `PageFlip_FX` (asm:722) — and the counter is seeded to 1 beforehand (asm:694). The two timing calls are a deliberate port-side addition, tagged `/* HACK */` and owned as such: without them the eight frames present as fast as the host can draw. `Mark_Time` / `Release_Time` are genuine OG functions (90 call sites each across 84 listings) so the idiom is authentic even though this listing does not use it. Do not file this as a defect.

- **IDA has the two destination parameters swapped, and production is right.** The listing declares `destination_cgy = word ptr 8` and `destination_cgx = word ptr 0Ah`, but the body proves the opposite: asm:189-190 reads `[bp+destination_cgy]` and stores it into `s_BATTLE_UNIT.target_cgx`, while asm:196-197 reads `[bp+destination_cgx]` and stores it into `s_BATTLE_UNIT.target_cgy`; asm:233-234 loads `s_BATTLE_UNIT.cgy` into the `bp+0Ah` slot. Both call sites in `Do_Auto_Unit_Turn` agree — the X coordinate is pushed to land at `bp+8`. Production's `Auto_Move_Unit(int16_t battle_unit_idx, int16_t dst_cgx, int16_t dst_cgy, ...)` is correct. This is an IDA labelling artifact of the same family as the `cnt_` enum trap; do **not** "fix" production to match the listing's parameter names.
