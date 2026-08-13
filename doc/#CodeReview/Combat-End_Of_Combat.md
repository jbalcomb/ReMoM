Combat-End_Of_Combat.md

SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  Combat-Strategic_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

End_Of_Combat__WIP()         ==>  End_Of_Combat()
STK_ComposeFleeLost__STUB()  ==>  Build_Flee_Loss_Message()

OVL_Action_Structure  ==> _combat_environ_idx
CMB_ScrollMsg_Type    ==>  combat_results_scroll_message

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\End_Of_Combat__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Retreat_From_Combat.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\STK_ComposeFleeLost__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Process_Retreating_Units.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Raze_Check.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Raze_City_Prompt.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Rampage_Combat_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr055\Apply_Damage_To_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Combat_Results_Scroll.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Combat_Results_Scroll_Text.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Combat_Results_Scroll_Draw.asm

Combat_Screen()
    |-> End_Of_Combat()
        |-> Evict_Weakest_Unit()
        |-> Retreat_From_Combat()
            |-> Build_Flee_Loss_Message()
            |-> Process_Retreating_Units()
        |-> Raze_Check()
        |-> Raze_City_Prompt()
        |-> City_Gold()
        |-> Player_Add_Gold()
        |-> Rampage_Combat_City()
        |-> Apply_Damage_To_City()
        |-> Remove_Item()
        |-> Kill_Unit()
        |-> Battle_Unit_Hit_Points()
        |-> Change_Relations()
        |-> Combat_Results_Scroll()
            |-> Combat_Results_Scroll_Text()
            |-> Combat_Results_Scroll_Draw()
        |-> Undead_Animation()

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-10).** No findings open. Eleven functions, 6,028 asm lines, every one walked against its listing.

The whole of `WIZARDS.EXE ovr123` — `End_Of_Combat` plus its six same-overlay subfunctions, 4,081 asm lines — plus the retreat chain it calls into in `ovr098`: `Retreat_From_Combat`, `Process_Retreating_Units` and `Build_Flee_Loss_Message`, a further 1,699 lines, and `Raze_Check` in `ovr098`'s sibling overlay `ovr124` (248 lines). Every function got a full body walk.

Forty-three findings were raised across the three sittings and all forty-three are applied. The `ovr098` set had a failure mode the `ovr123` set did not: four separate loops had been reduced to their bodies, leaving a bare `if` indexing `battle_units[]` with an index the previous loop left at `_combat_total_unit_count`.

`End_Of_Combat` itself is faithful in its **shape**: the frame matches the declaration order exactly, every loop bound and every call argument list checks out, and the long chains of `&&`/`||` guards are reproduced clause for clause. What it is not faithful in is **polarity and arithmetic**. Fourteen findings, and they cluster: six inverted comparisons, three flipped operators (`=` for `-=`, `-=` for `+=`, `/` for `*`), two hoisted statements, one missing `break`, one broken inner loop, one added store the asm never makes, and one undeclared identifier that stops the build.

Three of the subfunctions (`City_Gold`, `Rampage_Combat_City`, and parts of `Combat_Results_Scroll_Text`) carry `/* GEMINI */` markers. Those are the ones with invented guards and reversed declaration order — the pattern is consistent with a translation that reads the asm's intent rather than its instructions.

`Find_Undead_Creator_Type` was marked done-done on 2026-07-23. It was re-walked here because it is in the call chain; one finding and one deviation survived that pass.

## Renames

| dasm | production |
| --- | --- |
| `End_Of_Combat__WIP` | `End_Of_Combat` |
| `STK_ComposeFleeLost__STUB` | `Build_Flee_Loss_Message` |
| `Evict_Weakest_From_Full_Stack` | `Evict_Weakest_Unit` — the asm name carries the "full stack" precondition |
| `MsgType` / `CMB_ScrollMsg_Type` | `end_of_combat_message_type` / `combat_results_scroll_message` |
| `e_CMB_ScrollMsg_Type` + `e_COMBAT_SCROLL_MESSAGE` | merged into `e_COMBAT_SCROLL_MESSAGE_TYPE` (`csmt_`) |
| `Conv_String` | `temp_buffer` in `End_Of_Combat`, `buffer` in `Build_Flee_Loss_Message` |
| `Keep_City` | `spare` — the asm name says what it holds; reverting is still open |
| `s_LAIR.gold` / `.mana` | `Loot_Gold` / `Loot_Mana` (offsets 0x0A / 0x0C, `WIZARDS.inc:349-367`) |
| `s_BATTLE_UNIT.Max_Figures` | `figure_max` |
| `s_ITEM.enchantments` | `Powers` |
| `_players.Globals.Zombie_Mastery` | `_players[].Globals[ZOMBIE_MASTERY]` |

`Retreat_From_Combat` locals: `Fleeing_Units_Lost`/`Fleeing_Death_Count` → `lost_unit_types`/`lost_unit_count`; `Wind_Walker` → `has_wind_walker`; `Checked_X`/`Checked_Y` → `checked_wx`/`checked_wy`; `Min_X`/`Min_Y` → `min_wx`/`min_wy`; `Diameter` → `scan_span`; `Attacker_Fleeing_Count`/`Defender_Fleeing_Count` case-fixed.

`Build_Flee_Loss_Message` locals: `Unit_Types`+`Type_Counts` → `unit_type_tally[MAX_STACK][2]`; `Record_Count` → `tally_count`; `Record_Loopvar` → `itr_tally`; `Last_Char_Pointer` → `last_char_idx`; `Added_Count` split into `type_matched` and `phrases_emitted`.

`Process_Retreating_Units` locals: `Wind_Walker` → `has_wind_walker`; `Move_Possible` → `retreat_possible`; `Transport_Capacity` → `transport_capacity`. `Retreat_From_Combat` keeps `carry_capacity` for the same concept, where it shadows the `s_BATTLE_UNIT` field it sums.

## Scope

| function | production | listing | asm lines | coverage |
| --- | --- | --- | --- | --- |
| `End_Of_Combat` | [Combat.c:14756](../../MoM/src/Combat.c#L14756) | `ovr123/End_Of_Combat__WIP.asm` | 2493 | **full body walk — faithful** |
| `Combat_Results_Scroll_Text` | [Combat.c:15792](../../MoM/src/Combat.c#L15792) | `ovr123/Combat_Results_Scroll_Text.asm` | 613 | **full body walk — faithful** |
| `Rampage_Combat_City` | [Combat.c:16024](../../MoM/src/Combat.c#L16024) | `ovr123/Rampage_Combat_City.asm` | 342 | **full body walk — faithful** |
| `Combat_Results_Scroll` | [Combat.c:15644](../../MoM/src/Combat.c#L15644) | `ovr123/Combat_Results_Scroll.asm` | 309 | **full body walk — faithful** |
| `Find_Undead_Creator_Type` | [Combat.c:15561](../../MoM/src/Combat.c#L15561) | `ovr123/Find_Undead_Creator_Type.asm` | 171 | **full body walk — faithful** |
| `City_Gold` | [Combat.c:15986](../../MoM/src/Combat.c#L15986) | `ovr123/City_Gold.asm` | 116 | **full body walk — faithful** |
| `Combat_Results_Scroll_Draw` | [Combat.c:15630](../../MoM/src/Combat.c#L15630) | `ovr123/Combat_Results_Scroll_Draw.asm` | 37 | **full body walk — faithful** |
| `Raze_Check` | [Combat.c:17471](../../MoM/src/Combat.c#L17471) | `ovr124/Raze_Check.asm` | 248 | **full body walk — faithful** |
| `Retreat_From_Combat` | [Combat.c:3771](../../MoM/src/Combat.c#L3771) | `ovr098/Retreat_From_Combat.asm` | 1039 | **full body walk — faithful** |
| `Process_Retreating_Units` | [Combat.c:4230](../../MoM/src/Combat.c#L4230) | `ovr098/Process_Retreating_Units.asm` | 429 | **full body walk — faithful** |
| `Build_Flee_Loss_Message` | [Combat.c:5201](../../MoM/src/Combat.c#L5201) | `ovr098/STK_ComposeFleeLost__STUB.asm` | 231 | **full body walk — faithful** |

The `ovr123` jump table lives in `ovr123/_misc.asm` (17 lines) and is decoded under *The battle-result switch* below.

---

# Findings

None open.

---

# Deviations — structurally different, behaviour unchanged

These do not change what the code does. They are listed because the target is 1:1 with the disassembly, not functional equivalence.

## D1 — inverted `if`/`else` arms

Borland lays the `if` body down first and jumps to the `else`, so the physically-first block is the source's `if`. Fourteen sites have the arms exchanged:

| production | asm | the asm's `if` condition |
| --- | --- | --- |
| [Combat.c:14825](../../MoM/src/Combat.c#L14825) | asm:145-172 | `_UNITS[...].owner_idx == player_idx` (confusion) |
| [Combat.c:14828](../../MoM/src/Combat.c#L14828) | asm:174-215 | `controller_idx == player_idx` (uninvolved) |
| [Combat.c:14851](../../MoM/src/Combat.c#L14851) | asm:262-297 | `controller_idx == player_idx` (possession) |
| [Combat.c:14857](../../MoM/src/Combat.c#L14857) | asm:280-296 | `controller_idx == _combat_attacker_player` |
| [Combat.c:14995](../../MoM/src/Combat.c#L14995) | asm:845-847 | `player_idx == _combat_attacker_player` |
| [Combat.c:15010](../../MoM/src/Combat.c#L15010) | asm:903-940 | `player_idx == HUMAN_PLAYER_IDX` (raze) |
| [Combat.c:15094](../../MoM/src/Combat.c#L15094) | asm:1141-1147 | `(Abilities & UA_FANTASTIC) != 0` |
| [Combat.c:15105](../../MoM/src/Combat.c#L15105) | asm:1149-1160 | `Summoned_Unit == ST_TRUE` |
| [Combat.c:15137](../../MoM/src/Combat.c#L15137) | asm:1213-1219 | `player_idx == NEUTRAL_PLAYER_IDX` (destruction) |
| [Combat.c:15215](../../MoM/src/Combat.c#L15215) | asm:1424-1472 | `No_Secondaries == ST_TRUE` |
| [Combat.c:15424](../../MoM/src/Combat.c#L15424) | asm:2185-2211 | `winner_player_idx == _combat_attacker_player` |
| [Combat.c:15530](../../MoM/src/Combat.c#L15530) | asm:2380-2400 | `winner_player_idx != NEUTRAL_PLAYER_IDX` |
| [Combat.c:15576](../../MoM/src/Combat.c#L15576) | `Find_Undead_Creator_Type` asm:19-45 | `controller_idx == player_idx && (LifeSteal \|\| CreateUndead)` |
| [Combat.c:16091](../../MoM/src/Combat.c#L16091) | `Rampage_Combat_City` asm:100-125 | see R21 |

## D2 — ternaries reconstructed as `if`/`else`

Five sites where the asm computes a value into a register through two paths and stores it **once** — the signature of a conditional expression. An `if`/`else` with an assignment in each arm emits two stores.

| production | asm | faithful form |
| --- | --- | --- |
| [Combat.c:14857-14863](../../MoM/src/Combat.c#L14857-L14863) | asm:280-296 | `controller_idx = (controller_idx == attacker) ? defender : attacker;` |
| [Combat.c:15905-15917](../../MoM/src/Combat.c#L15905-L15917) | `..._Text` asm:355-364 | `strcat(message, (GUI_Multipurpose_Int < 0) ? cnst_ScrlFame_Msg_2 : cnst_ScrlFame_Msg_3);` |
| [Combat.c:15327-15336](../../MoM/src/Combat.c#L15327-L15336) | asm:2124-2143 | `Kill_Unit(unit_idx, (status == bus_Gone \|\| (mutations & UM_UNDEAD)) ? kt_Disappeared : kt_Normal);` |
| [Combat.c:15501-15507](../../MoM/src/Combat.c#L15501-L15507) | asm:2344-2352 | `strcat(GUI_NearMsgString, (Zombies_Raised > 1) ? cnst_Zombie_Msg_1 : cnst_Zombie_Msg_2);` |
| [Combat.c:15516-15522](../../MoM/src/Combat.c#L15516-L15522) | asm:2390-2400 | `strcat(GUI_NearMsgString, (Undead_Created > 1) ? cnst_Undead_Msg_4 : cnst_Undead_Msg_5);` |

## D3 — operand order in sums

The asm loads the left operand first. Four sites read the other way round: [Combat.c:15627](../../MoM/src/Combat.c#L15627) (`_scroll_text_top + _scroll_text_height`, asm loads height first), and the `Print_Centered` y-coordinates for message types 1/6, 2/7, 4 and 5 ([Combat.c:15825](../../MoM/src/Combat.c#L15825) onward) written as `25 + _scroll_text_top` where types 3 and 12 in the same switch correctly read `_scroll_text_top + 25`. Also `100 + upkeep` at [Combat.c:15613](../../MoM/src/Combat.c#L15613) where the asm loads `upkeep` and does `add ax, 100`.

## D4 — `==` operand order

`if(a == b)` loads `a` into AX. `Rampage_Combat_City` asm:129-138 loads `Primary_Unit` and `Secondary_Unit`; [Combat.c:16077](../../MoM/src/Combat.c#L16077) and [Combat.c:16081](../../MoM/src/Combat.c#L16081) write `current_u_type == primary_unit`.

## D5 — port-model and reconstruction aids with no asm counterpart

Listed for completeness, not as defects:

- `static int16_t Buildings_Lost[36]` at [Combat.c:14762](../../MoM/src/Combat.c#L14762) — a documented HACK. The OG has it at `bp-6Eh` and hands `&Buildings_Lost` to the file-scope `CMB_LostBuildings@`; `static` is how the port keeps that pointer valid.
- `assert()` at [Combat.c:15185-15179](../../MoM/src/Combat.c#L15185-L15179).
- `case 0:` in the battle-result switch at [Combat.c:15811](../../MoM/src/Combat.c#L15811) — the asm has no case 0; `dec bx` makes 0 wrap to 0xFFFF and fail the `jbe`, landing on the default, which is the same continuation point. Harmless, but invented.
- `stu_strcpy` / `stu_strcat` for the asm's `_fstrcpy` / `strcat`, with the original far-pointer calls preserved commented-out ([Combat.c:15874](../../MoM/src/Combat.c#L15874), [Combat.c:15885](../../MoM/src/Combat.c#L15885), [Combat.c:15894](../../MoM/src/Combat.c#L15894)). Good practice — noted so it is not re-flagged.
- `Print` with an explicit `.name` for the asm's `Print_Far` with a struct pointer ([Combat.c:15963](../../MoM/src/Combat.c#L15963)), original preserved above it. `s_BLDG_DATA.name` is at offset 0 (`WIZARDS.inc:221`), so the two are the same address.
- `= 0` initialisers on locals the OG leaves uninitialised. This is the file-wide house style and is not called out per-site; the one place it changes observable behaviour is noted under OG behaviours.

---

# Verified faithful

## `Combat_Results_Scroll_Draw` — full body walk, no defects

Six statements, all matching. `Set_Window(0, 0, SCREEN_XMAX, _scroll_text_top + _scroll_text_height)`, `Clipped_Draw(54, _scroll_text_top, _scroll_paper_seg)`, `Reset_Window()`, `FLIC_Draw(54, _scroll_text_top + _scroll_text_height, _combat_results_scroll_bottom_seg)`, then the tail call to `Combat_Results_Scroll_Text`. The only note is D4's operand order on the first sum.

## `End_Of_Combat` — the frame

The one thing this function gets exactly right, and it is worth recording because it is what made the rest of the walk tractable. Fifteen stack locals, and the declaration order in [Combat.c:14762-14772](../../MoM/src/Combat.c#L14762-L14772) matches the frame offset order one for one:

| offset | asm name | production |
| --- | --- | --- |
| -6Eh | `Buildings_Lost` | `Buildings_Lost[36]` |
| -26h | `Rare_Foe_Defeated` | `Rare_Foe_Defeated` |
| -24h | `Diplomatic_Value` | `Diplomatic_Value` |
| -22h | `Summoned_Unit` | `Summoned_Unit` |
| -20h | `Population_Loss_Percent` | `Population_Loss_Percent` |
| -1Eh | `Destruction_Chance` | `Destruction_Chance` |
| -1Ch | `No_Secondaries` | `No_Secondaries` |
| -1Ah | `BU_CombatHits` | `BU_CombatHits` |
| -18h | `Conv_String` (10 bytes) | `temp_buffer[LEN_TEMP_BUFFER]` — `LEN_TEMP_BUFFER` is 10 |
| -0Eh | `Undead_Created` | `Undead_Created` |
| -0Ch | `Zombies_Raised` | `Zombies_Raised` |
| -0Ah | `Experience_Gained` | `Experience_Gained` |
| -8 | `Surviving_Unit_Count` | `Surviving_Unit_Count` |
| -6 | `battle_unit_owner_idx` | `battle_unit_owner_idx` |
| -4 | `enchantments` (dword) | `uint32_t enchantments` |

Parameters at +6/+8/+0Ah/+0Ch are all single words — `item_count` and `item_list` are near pointers, matching the production signature.

`Combat_Results_Scroll` and `Combat_Results_Scroll_Text` also have correct declaration order. Only `Rampage_Combat_City` (R20) does not.

## `End_Of_Combat` — the recall and regeneration passes

asm:352-500. Everything checked and correct: the `(status <= bus_Dead) || (status == bus_Flee) || (status == bus_Recalled)` chain; the 32-bit regeneration test folding `Abilities & UA_REGENERATION` with `enchantments & UE_REGENERATION`; the three `summon_w*` writes; `Finished = ST_TRUE`; the `Evict_Weakest_From_Full_Stack` call; and the `owner_idx = 100` marker gated on `owner == winner && owner == attacker`.

## `End_Of_Combat` — the zombie pass

asm:706-822. All five gates correct (`Zombie_Mastery > 0`, `status == bus_Dead`, `race < rt_Arcane`, `Hero_Slot == -1`, `Surviving_Unit_Count < 9`) and all thirteen body statements in the asm's order, including both `figure_cnt` and `figure_max` taking `_unit_type_table[ut_Zombies].Figures`. Only the XP award (R4) is inverted.

## `End_Of_Combat` — the hero-item sweep

asm:1621-1855. The three-way split is exact: `Remove_Item` when the unit is `bus_Gone`; otherwise append to `item_list` while `*item_count < 18`; otherwise `Remove_Item`. The slot clear to `ST_UNDEFINED` correctly sits **outside** the `> ST_UNDEFINED` test — the asm's `jmp @@Next_Hero_Item` from the skip path lands on the clear. Loop bound `NUM_HERO_ITEMS` is 3, matching `e_NUM_HERO_ITEMS`.

## `End_Of_Combat` — population loss and city damage

asm:1180-1237. The population-loss loop reproduces the asm exactly, down to the `(population - 1) > itr` operand order and the `Random(100) <= Population_Loss_Percent` sense. `Buildings_Lost` is cleared across `NUM_BUILDINGS`; `Destruction_Chance` starts at `_combat_city_damage` and is clamped at 75; `Apply_Damage_To_City(_combat_environ_idx, IDK_population_lost, Destruction_Chance, &Buildings_Lost[0])` is gated on `_active_battle_unit != 668`. Only the `Population_Loss_Percent` seed (R7) and the missing `break` (R8) are wrong.

## `End_Of_Combat` — diplomacy, experience and the closing messages

asm:2075-2493. `Change_Relations(-Diplomatic_Value, attacker, defender, 8, ST_NULL, ST_NULL)` with its three-clause guard; the lair guard consolidation including the `(guard1_count & 0x0F) == 0 && (guard2_count & 0x0F) > 0` promotion and the `ut_Magic_Spirit` undead suppression; the experience loop's `(race < rt_Arcane) || (type == ut_Chosen)` clause; the `Combat_Results_Scroll` gate; the zombie and undead message assembly; and the closing `Set_Mouse_List(1, mouse_list_hourglass)` when the attacker is not human. All faithful apart from R13, R14 and the D2 ternaries.

## `Combat_Results_Scroll` — the scroll-paper selection

asm:20-115. The three-level selection is reproduced exactly:

| condition | `SCROLL.LBX` index |
| --- | --- |
| `combat_results_scroll_message == 4` | 11 (CMBLOSE) |
| city siege, won (types 1/5/6), human attacker | 9 (CMBSCRL1) — and `City_Capture = ST_TRUE` |
| city siege, won, non-human attacker | 10 (CMBWIN) |
| city siege, lost, human attacker | 10 |
| city siege, lost, non-human attacker | 9 |
| not a city siege, won | 10 |
| not a city siege, lost | 11 |

`City_Capture` is set on exactly one of the seven paths, which is what gates the interactive raze prompt later. Also verified: the `Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED)` argument list; both `Add_Hot_Key` calls; `Assign_Auto_Function(Combat_Results_Scroll_Draw, 1)`; the `_scroll_text_top = (SCREEN_YMAX - (22 + _scroll_text_height)) / 2` centring including its signed-division rounding; and the `Clear_Fields` / `Add_Hidden_Field` pair that runs whenever the prompt was shown, regardless of the answer.

## `Combat_Results_Scroll_Text` — the battle-result switch

`ovr123/_misc.asm` holds the table. The dispatch is `bx = combat_results_scroll_message - 1`, bounds-checked with `cmp bx, 11 / jbe`, so only values 1-12 reach it:

| `combat_results_scroll_message` | table slot | `MESSAGE.LBX` index |
| --- | --- | --- |
| 1 **and** 6 | `sw_BattleResult_00` (6 jumps to it) | 5 |
| 2 **and** 7 | `sw_BattleResult_06` (1 jumps to it) | 6 |
| 3 | `sw_BattleResult_02` | 7, plus the `GUI_NearMsgString` paragraph |
| 4 | `sw_BattleResult_03` | 8 |
| 5 | `sw_BattleResult_04` | 9 |
| 8, 9, 10, 11 | `sw_BattleResult_07` | none — falls into the common tail |
| 12 | `sw_BattleResult_12` | none — city name + `cnst_CityLost_Msg` |

The slot names are IDA's numbering of table positions, not case values — slot 00 is case 1. Production's grouping of `case 1: case 6:` and `case 2: case 7:` matches the `jmp` aliasing exactly.

## `Combat_Results_Scroll_Text` — the rest

The colour-array setup (`colors1[0..4] = 47`, `colors2[0..4] = 55`, then `colors2[0] = 247`, `colors1[0] = 245`, in that order), `text_height = 92`, the three `_active_battle_unit` sentinel blocks (666 / 668 / 667) with 668 and 667 writing into the local buffer while case 12 writes into `GUI_NearMsgString`, the fame and gold paragraphs, the `GAME_RazeCity` early return, the population and destroyed-buildings blocks, the two-column building list with its `(itr % 2) * 90` x-offset and `(itr % 2) == 1` row advance, and the closing floor — all faithful.

The floor is worth spelling out because the macro name reads backwards. asm:594-597:

```
cmp     [CMB_Scroll_MinHeight], _SI_text_height
jle     short @@Done
mov     _SI_text_height, [CMB_Scroll_MinHeight]
```

`SETMIN(a, b)` is `if((b) > (a)) { (a) = (b); }` ([MOX_DEF.h:95](../../MoX/src/MOX_DEF.h#L95)), so `SETMIN(text_height, CMB_Scroll_MinHeight)` raises `text_height` to the floor — which is what the asm does. Correct despite the name.

## `City_Gold` — the arithmetic

The long multiply/divide was traced through the stack. `empire_population` is pushed as a long first, the city population second, `gold_reserve` ends up in DX:AX, `LXMUL@` takes the product and `LDIV@` divides by what was pushed first — so the expression is `(gold_reserve * city_population) / empire_population`, which is what [Combat.c:16017](../../MoM/src/Combat.c#L16017) computes. Both loops and the neutral-owner branch match.

## `Rampage_Combat_City` — the new lair record

asm:274-341. Seventeen field writes in the asm's order, all correct: both guard types, both counts scaled by 17 (`0x11` — one guard in each nibble), `wx`/`wy`/`wp` from the combat coordinates, `intact = ST_TRUE`, `type = lt_Ruins`, `Loot_Gold = CMB_Gold_Reward`, and the six zeroed fields ending with all three `Item_Values`. The field mapping was checked against `WIZARDS.inc:349-367` — `gold` is at 0x0A and `mana` at 0x0C, matching `Loot_Gold` / `Loot_Mana` in [gd_lair_fields.h:16](../../MoM/src/gd_lair_fields.h#L16).

`Change_City_Ownership(_combat_environ_idx, NEUTRAL_PLAYER_IDX)` and `Destroy_City(_combat_environ_idx)` also verified against the push order.

---

# Deviations — `ovr098`

- **Inverted `if`/`else` arms.** `Retreat_From_Combat.asm:24-31` puts `player_idx == _combat_attacker_player` first, so the `if` body is `fleeing_player_idx = _combat_defender_player`; [Combat.c:3786](../../MoM/src/Combat.c#L3786) leads with `!=`. Same at [Combat.c:5211](../../MoM/src/Combat.c#L5211), where `Build_Flee_Loss_Message.asm:19-21` (`jg` past the far `jmp`) gives `troop_count > 0` as the `if` and the empty-string `strcpy` as the `else`; production uses an early return.
- **Missing redundant reset.** `Process_Retreating_Units.asm:137` writes `mov [bp+Wind_Walker], e_ST_FALSE` on entry to the troops block. The block is only reached when `Wind_Walker` is already `ST_FALSE`, so the store has no effect, but it is a store the asm makes and [Combat.c:4292](../../MoM/src/Combat.c#L4292) does not.
- **Y-loop guard folded into the condition.** `Retreat_From_Combat.asm:913-914` tests `Checked_Y >= WORLD_HEIGHT` **inside** the body and jumps to the increment, so the loop keeps counting to `Min_Y + Diameter`. [Combat.c:4151](../../MoM/src/Combat.c#L4151) folds it into the `while` condition and exits instead. Nothing else happens in those iterations, so the outcome matches.
- **Return value.** `Retreat_From_Combat` is `void` in production. The asm zeroes AX on two of its three exits — the early-out at `asm:96-97` and the end of the attacker branch (`asm:702`, `jmp @@JmpDone__Return_ZERO`) — but the defender branch falls into `@@Done` with AX untouched. `End_Of_Combat` ignores the result either way, so nothing depends on it; recorded because a `void` function would not have produced the two `xor ax, ax`.

---

# Verified faithful — `ovr098`

**Frames.** All three match. `Retreat_From_Combat` has thirteen slotted locals and [Combat.c:3770-3782](../../MoM/src/Combat.c#L3770-L3782) declares them in frame order; `Process_Retreating_Units` has six and [Combat.c:4238-4243](../../MoM/src/Combat.c#L4238-L4243) matches; `Build_Flee_Loss_Message` has eight and [Combat.c:5203-5209](../../MoM/src/Combat.c#L5203-L5209) matches.

**`Retreat_From_Combat`.** The `wp == 9` combat-summon kill; the fleeing rolls in both branches — `Random(2) == 1` for a non-hero and `Random(4) != 1` for a hero in the attacker branch, `Random(2) == 1` and `Random(4) == 1` setting `bus_Dead` in the defender branch; the Black Sleep / Confusion / Web + `Web_HP > 0` chain in both; the road-building reset (`us_Ready`, `Rd_Constr_Left = ST_UNDEFINED`) at all three sites; the `Min_Y`/`Min_X` derivation including the `WORLD_WIDTH` wrap; `Diameter = 3`; the `Square_Has_Lair` / `Player_City_At_Square` pair of `ST_UNDEFINED` tests; and both `Build_Flee_Loss_Message` call sites.

**`Process_Retreating_Units`.** The `Army_At_Square_2` argument list; the three-clause entry guard; the tower scan with its compound loop condition and `goto Return_FALSE`; the four `Unit_Has_*` travel tests in both the sailable and non-sailable branches; the `Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count)` argument list at all three call sites with the `troop_count++` / `bus_Active` pair following each; the non-sailable `Unit_Has_Sailing` abort; and the `return_value` / `goto` exit structure mirroring `@@JmpJmpDone__Return_FALSE` and `@@JmpJmpDone__Return_TRUE`.

**`Build_Flee_Loss_Message`.** The `Type_Counts` clear over `MAX_STACK`; the duplicate-detection inner loop with its `Added_Count` flag; `Add_Comma_Or_And(&Added_Count, Record_Count, GUI_NearMsgString)`; the `itoa` / `strcat` / `cnst_Space_6` / unit-name sequence; the `Huntress` and `Priestess` exclusions from the plural trim; `strlen(...) - 1` and the `'s'` test; and the closing `cnst_Dot8`.

`s_UNIT_TYPE.name` is a `dw` — a pointer, not an inline string (`WIZARDS.inc:221` is `s_BLDG_DATA`'s inline `name db 20 dup(?)`, which is the other convention) — so `push [word ptr _unit_type_table.name+bx]` passing it straight to `strcat` matches production's `char **name` at [UNITTYPE.h:275](../../MoM/src/UNITTYPE.h#L275).

## `Raze_Check` — full body walk, no defects

The AI's decide-whether-to-raze roll, reached only from `End_Of_Combat` and `Combat_City_Capture`. Five slotted locals in frame order, `score` in DI.

Checked and correct: the neutral-owner early-out; the six-way personality switch against `ovr124/_misc.asm:5-10` (indices 0-5 = `PRS_Maniacal`…`PRS_Peaceful`, bounds-checked by `cmp bx, 5` / `ja`) with `+25` / `+10` / nothing / nothing / the Lawful `Hidden_Rel` split / `-200`; the `Aggressive`-into-`Chaotic` fallthrough that does nothing; the owner-blind `total_surviving < 3` → `+10` bonus; both landmass lookups at `wp * WORLD_SIZE + wy * WORLD_WIDTH + wx`; the own-versus-owner unit tally on that landmass; `-200` when the former owner has nothing left there, otherwise `((own * 50) / owner) - 50` as a **16-bit** multiply (`imul dx` then `cwd`, which discards the high word); the Chaotic floor of 10; and `Random(100) <= score` for the verdict.

---

---

# OG behaviours preserved

Faithful to the disassembly; not to be "fixed".

- **`Summoned_Unit` is read uninitialised.** asm:1131-1160 never writes it before the neutral-unit scan, and the scan only writes it if it finds a neutral-controlled unit. When there is none, the `Summoned_Unit == ST_TRUE` test at asm:1149 reads whatever was on the stack. Production's `= 0` initialiser at [Combat.c:14760](../../MoM/src/Combat.c#L14760) makes this deterministic — the one place the house-style initialisers change observable behaviour. Worth a comment rather than a change.
- **`City_Gold` divides with no zero guard** — see R25. The OG would fault on a zero-population empire; the situation cannot arise because the city being valued is counted in its own empire's total.
- **The building-message compaction shifts only `city_idx`.** Entries are 4 bytes (`shl bx, 2`) but the copy at asm:1256-1263 moves one byte per slot. Whatever else the record holds is left behind.
- **The hero-item slot is cleared unconditionally.** asm:1811-1854 writes `-1` even on the path that skipped the body because the slot was already `<= -1`.
- **`_active_battle_unit` carries sentinels.** 666 (rampage, no ruins), 667 (raiders took the city), 668 (rampage, ruins created) are stashed in a variable otherwise holding a battle-unit index, and `Combat_Results_Scroll_Text` reads them back to pick messages.
- **`Combat_Results_Scroll_Text` returns early on the raze path**, skipping the `CMB_Scroll_MinHeight` floor — asm:471 is `jmp @@Done`, past the clamp at `loc_A722B`. Production preserves this by putting the clamp inside the `else`; the in-code `¿ BUG:` note at [Combat.c:15979](../../MoM/src/Combat.c#L15979) records the observation without acting on it.
- **`Find_Undead_Creator_Type` indexes the wrong player's hero array** — `_players[player_idx].Heroes[_UNITS[battle_units[itr1].unit_idx].Hero_Slot]` where the unit under `itr1` may belong to someone else. Recorded as B1 in `SPLMASTR-Undead_Animation.md`; unchanged.
- **`Rampage_Combat_City` scales guard counts by 17 and truncates to a byte.** Four or more of one guard type overflows the low nibble into the high one.

---

# Listing notes

- `Rampage_Combat_City` asm:83 renders the battle-unit stride as `mov dx, size s_BATTLE_UNIT+s_BATTLE_UNIT.melee`, and `End_Of_Combat` asm:1573 does the same. Every other one of the ~140 sites in this overlay that indexes `battle_units` uses the plain `size s_BATTLE_UNIT`, and the surrounding instruction sequence is identical at all of them. Treat these two as IDA symbolisation artifacts, not as a different computation.
- `End_Of_Combat` asm:850 and asm:625 write `_combat_wx__som_started_anim_ctr`; production uses `_combat_wx` ([Combat.h:1414](../../MoM/src/Combat.h#L1414)). The compound label is Piethawn's, marking storage reused for a second purpose.
- The `_fstrcpy` calls at asm:2032 and `Combat_Results_Scroll_Text` asm:165 push `0` as the segment of a data-segment object. Recorded because it looks wrong; it is moot in the port, which uses near copies.

---

# Build state

All forty-four findings raised against this chain are applied. `cmake --build --preset MSVC-debug --target ReMoM` is **clean** — `momlib.lib` and `ReMoM.exe` both link, with no errors and no warnings anywhere in the chain.

The test suite has not been run. Several fixes move RNG calls: `Random(10)` is no longer scoped to the non-hero arm of the diplomatic bonus, the `Summoned_Unit` scan now stops at the first neutral unit, and `Raze_Check`'s landmass lookups now read the right plane so AI raze decisions change on Myrror. The combat goldens were already pending the `Generate_Combat_Map` work; both sets need regenerating together.

Two findings were withdrawn rather than fixed. They argued a local was absent from the OG source because it had no frame slot. That inference is invalid — the compiler eliminates locals — and it should not appear in future reviews.

