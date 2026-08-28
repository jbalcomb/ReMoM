Combat-End_Of_Combat.md

SEEALSO:  Combat-Combat.md
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
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Player_City_At_Square.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Unit_Try_To_Move.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Retreat_From_Combat.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\STK_ComposeFleeLost__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Process_Retreating_Units.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Raze_Check.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Raze_City_Prompt.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Raze_City_Prompt_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Rampage_Combat_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr055\Apply_Damage_To_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Combat_Results_Scroll.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Combat_Results_Scroll_Text.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr123\Combat_Results_Scroll_Draw.asm

// WZD o124p21
Combat.c Raze_City_Prompt_Draw() is byte-identical to GENDRAW.c Confirmation_Box_Draw(), presumably copied to this overlay to avoid code-overlay swapping.

Combat_Screen()
    |-> End_Of_Combat()
        |-> Evict_Weakest_Unit()
        |-> Retreat_From_Combat()
            |-> Build_Flee_Loss_Message()
            |-> Process_Retreating_Units()
        |-> Raze_Check()
        |-> Raze_City_Prompt()
            |-> Raze_City_Prompt_Draw()
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

**Status: DONE-DONE (2026-08-27).** No findings open. Fifteen functions, 6,477 asm lines, every one walked against its listing. D5 added 2026-08-17 — a deviation found while reviewing a shared callee, no reopening required. Reopened 2026-08-25 to adjudicate `Raze_City_Prompt` and `Raze_City_Prompt_Draw`, both faithful; see *The raze prompt* below.

The whole of `WIZARDS.EXE ovr123` — `End_Of_Combat` plus its six same-overlay subfunctions, 4,081 asm lines — plus the retreat chain it calls into in `ovr098`: `Retreat_From_Combat`, `Process_Retreating_Units` and `Build_Flee_Loss_Message`, a further 1,699 lines, and `Raze_Check` in `ovr098`'s sibling overlay `ovr124` (248 lines). Every function got a full body walk.

Forty-three findings were raised across the three sittings and all forty-three are applied. The `ovr098` set had a failure mode the `ovr123` set did not: four separate loops had been reduced to their bodies, leaving a bare `if` indexing `battle_units[]` with an index the previous loop left at `_combat_total_unit_count`.

`End_Of_Combat` itself is faithful in its **shape**: the frame matches the declaration order exactly, every loop bound and every call argument list checks out, and the long chains of `&&`/`||` guards are reproduced clause for clause. What it is not faithful in is **polarity and arithmetic**. Fourteen findings, and they cluster: six inverted comparisons, three flipped operators (`=` for `-=`, `-=` for `+=`, `/` for `*`), two hoisted statements, one missing `break`, one broken inner loop, one added store the asm never makes, and one undeclared identifier that stops the build.

Three of the subfunctions (`City_Gold`, `Rampage_Combat_City`, and parts of `Combat_Results_Scroll_Text`) carry `/* GEMINI */` markers. Those are the ones with invented guards and reversed declaration order — the pattern is consistent with a translation that reads the asm's intent rather than its instructions.

`Find_Undead_Creator_Type` was marked done-done on 2026-07-23. It was re-walked here because it is in the call chain; one finding and one deviation survived that pass.

## Renames

| dasm | production |
| --- | --- |
| `End_Of_Combat` | `End_Of_Combat` |
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
| `End_Of_Combat` | [Combat.c:13944](../../MoM/src/Combat.c#L13944) | `ovr123/End_Of_Combat__WIP.asm` | 2493 | **full body walk — faithful** |
| `Combat_Results_Scroll_Text` | [Combat.c:14977](../../MoM/src/Combat.c#L14977) | `ovr123/Combat_Results_Scroll_Text.asm` | 613 | **full body walk — faithful** |
| `Rampage_Combat_City` | [Combat.c:15182](../../MoM/src/Combat.c#L15182) | `ovr123/Rampage_Combat_City.asm` | 342 | **full body walk — faithful** |
| `Combat_Results_Scroll` | [Combat.c:14829](../../MoM/src/Combat.c#L14829) | `ovr123/Combat_Results_Scroll.asm` | 309 | **full body walk — faithful** |
| `Find_Undead_Creator_Type` | [Combat.c:14746](../../MoM/src/Combat.c#L14746) | `ovr123/Find_Undead_Creator_Type.asm` | 171 | **full body walk — faithful** |
| `City_Gold` | [Combat.c:15144](../../MoM/src/Combat.c#L15144) | `ovr123/City_Gold.asm` | 116 | **full body walk — faithful** |
| `Combat_Results_Scroll_Draw` | [Combat.c:14815](../../MoM/src/Combat.c#L14815) | `ovr123/Combat_Results_Scroll_Draw.asm` | 37 | **full body walk — faithful** |
| `Raze_Check` | [Combat.c:16478](../../MoM/src/Combat.c#L16478) | `ovr124/Raze_Check.asm` | 248 | **full body walk — faithful** |
| `Raze_City_Prompt` | [Combat.c:16615](../../MoM/src/Combat.c#L16615) | `ovr124/Raze_City_Prompt.asm` | 205 | **full body walk — faithful** |
| `Raze_City_Prompt_Draw` | [Combat.c:16684](../../MoM/src/Combat.c#L16684) | `ovr124/Raze_City_Prompt_Draw.asm` | 126 | **full body walk — faithful** |
| `Player_City_At_Square` | [Combat.c:4198](../../MoM/src/Combat.c#L4198) | `ovr098/Player_City_At_Square.asm` | 67 | **full body walk — faithful** |
| `Unit_Try_To_Move` | [Combat.c:4427](../../MoM/src/Combat.c#L4427) | `ovr098/Unit_Try_To_Move.asm` | 51 | **full body walk — faithful** |
| `Retreat_From_Combat` | [Combat.c:3762](../../MoM/src/Combat.c#L3762) | `ovr098/Retreat_From_Combat.asm` | 1039 | **full body walk — faithful** |
| `Process_Retreating_Units` | [Combat.c:4221](../../MoM/src/Combat.c#L4221) | `ovr098/Process_Retreating_Units.asm` | 429 | **full body walk — faithful** |
| `Build_Flee_Loss_Message` | [Combat.c:5081](../../MoM/src/Combat.c#L5081) | `ovr098/STK_ComposeFleeLost__STUB.asm` | 231 | **full body walk — faithful** |

The `ovr123` jump table lives in `ovr123/_misc.asm` (17 lines) and is decoded under *The battle-result switch* below.

---

# Findings

None open.

---

# Deviations — structurally different, behaviour unchanged

These do not change what the code does. They are listed because the target is 1:1 with the disassembly, not functional equivalence.

## D1 — inverted `if`/`else` arms

Borland lays the `if` body down first and jumps to the `else`, so the physically-first block is the source's `if`. Twelve sites have the arms exchanged:

| production | asm | the asm's `if` condition |
| --- | --- | --- |
| [Combat.c:14193](../../MoM/src/Combat.c#L14193) | asm:145-172 | `_UNITS[...].owner_idx == player_idx` (confusion) |
| [Combat.c:14202](../../MoM/src/Combat.c#L14202) | asm:174-215 | `controller_idx == player_idx` (uninvolved) |
| [Combat.c:14223](../../MoM/src/Combat.c#L14223) | asm:262-297 | `controller_idx == player_idx` (possession) |
| [Combat.c:14225](../../MoM/src/Combat.c#L14225) | asm:280-296 | `controller_idx == _combat_attacker_player` |
| [Combat.c:14363](../../MoM/src/Combat.c#L14363) | asm:873-876 | `winner_player_idx == _combat_attacker_player` |
| [Combat.c:14374](../../MoM/src/Combat.c#L14374) | asm:903-940 | `player_idx == HUMAN_PLAYER_IDX` (raze) |
| [Combat.c:14462](../../MoM/src/Combat.c#L14462) | asm:1141-1147 | `(Abilities & UA_FANTASTIC) != 0` |
| [Combat.c:14473](../../MoM/src/Combat.c#L14473) | asm:1149-1160 | `Summoned_Unit == ST_TRUE` |
| [Combat.c:14503](../../MoM/src/Combat.c#L14503) | asm:1213-1219 | `player_idx == NEUTRAL_PLAYER_IDX` (destruction) |
| [Combat.c:14583](../../MoM/src/Combat.c#L14583) | asm:1424-1472 | `No_Secondaries == ST_TRUE` |
| [Combat.c:14943](../../MoM/src/Combat.c#L14943) | `Find_Undead_Creator_Type` asm:19-45 | `controller_idx == player_idx && (LifeSteal \|\| CreateUndead)` |
| [Combat.c:15417](../../MoM/src/Combat.c#L15417) | `Rampage_Combat_City` asm:100-125 | see R21 |

Two rows were removed on 2026-08-25 rather than re-anchored. Their original anchors had drifted out of `End_Of_Combat` entirely, and re-derivation found no construct matching what they claimed: the cited `asm:2185-2211` is the `Undead_Created > 0` guard around the lair guard-type writes, and `asm:2380-2400` is the `cnst_Zombie_Msg_3` `strcat`. Neither contains an `if`/`else` pair. Every comparison against `_combat_attacker_player` (asm:297, 492, 874, 1126, 1217, 2104) and against `NEUTRAL_PLAYER_IDX` (asm:45, 878, 1128, 1138, 1219, 1308, 1317, 1504, 2102, 2124, 2205, 2434) was checked; the only one that is a genuine arm inversion is asm:873-876, which is now the row above at [Combat.c:14363](../../MoM/src/Combat.c#L14363). The rest are guards with no `else`, which is not what this deviation records.

## D2 — ternaries reconstructed as `if`/`else`

Five sites where the asm computes a value into a register through two paths and stores it **once** — the signature of a conditional expression. An `if`/`else` with an assignment in each arm emits two stores.

| production | asm | faithful form |
| --- | --- | --- |
| [Combat.c:14225-14232](../../MoM/src/Combat.c#L14225-L14232) | asm:280-296 | `controller_idx = (controller_idx == attacker) ? defender : attacker;` |
| [Combat.c:15252-15259](../../MoM/src/Combat.c#L15252-L15259) | `..._Text` asm:355-364 | `strcat(message, (GUI_Multipurpose_Int < 0) ? cnst_ScrlFame_Msg_2 : cnst_ScrlFame_Msg_3);` |
| [Combat.c:14700-14709](../../MoM/src/Combat.c#L14700-L14709) | asm:2124-2143 | `Kill_Unit(unit_idx, (status == bus_Gone \|\| (mutations & UM_UNDEAD)) ? kt_Disappeared : kt_Normal);` |
| [Combat.c:14868-14874](../../MoM/src/Combat.c#L14868-L14874) | asm:2344-2352 | `strcat(GUI_NearMsgString, (Zombies_Raised > 1) ? cnst_Zombie_Msg_1 : cnst_Zombie_Msg_2);` |
| [Combat.c:14899-14905](../../MoM/src/Combat.c#L14899-L14905) | asm:2390-2400 | `strcat(GUI_NearMsgString, (Undead_Created > 1) ? cnst_Undead_Msg_4 : cnst_Undead_Msg_5);` |

## D3 — operand order in sums

The asm loads the left operand first. Four sites read the other way round: [Combat.c:15000](../../MoM/src/Combat.c#L15000) (`_scroll_text_top + _scroll_text_height`, asm loads height first), and the `Print_Centered` y-coordinates for message types 1/6, 2/7, 4 and 5 ([Combat.c:15188](../../MoM/src/Combat.c#L15188) onward) written as `25 + _scroll_text_top` where types 3 and 12 in the same switch correctly read `_scroll_text_top + 25`. Also `100 + upkeep` at [Combat.c:15199](../../MoM/src/Combat.c#L15199) where the asm loads `upkeep` and does `add ax, 100`.

## D4 — port-model and reconstruction aids with no asm counterpart

Listed for completeness, not as defects:

- `static int16_t Buildings_Lost[NUM_BUILDINGS]` at [Combat.c:14130](../../MoM/src/Combat.c#L14130) — a documented HACK. The OG has it at `bp-6Eh` and hands `&Buildings_Lost` to the file-scope `combat_results_lost_buildings@`; `static` is how the port keeps that pointer valid.
- `stu_strcpy` / `stu_strcat` for the asm's **near** string copies. `End_Of_Combat__WIP.asm` calls only the near library routines (`strcpy` twice, `strcat` seven times, no `_fstrcpy` at all), so [Combat.c:14867](../../MoM/src/Combat.c#L14867) and [Combat.c:14882](../../MoM/src/Combat.c#L14882) are faithful as near copies. The three `_fstrcpy` calls in this overlay are all in `Combat_Results_Scroll_Text.asm`, each copying a city name out of the far `_CITIES` block, and production now calls `_fstrcpy` at all three — asm:167 at [Combat.c:15221](../../MoM/src/Combat.c#L15221), asm:256 at [Combat.c:15236](../../MoM/src/Combat.c#L15236), asm:302 at [Combat.c:15243](../../MoM/src/Combat.c#L15243). The listing's destinations differ between them: asm:167 writes the DS global `GUI_NearMsgString`, while asm:256 and asm:302 write a **stack local** (`lea ax, [bp+Msg_String]`) and still pass segment 0, which works only because `SS == DS` in this model — see the *Listing notes* entry on the sentinel.
- `= 0` initialisers on locals the OG leaves uninitialised. This is the file-wide house style and is not called out per-site; the one place it changes observable behaviour is noted under OG behaviours.

---

## D5 — `Get_Paragraph_Max_Height` is called with three arguments

`seg019/Get_Paragraph_Max_Height.asm` declares exactly two parameters — `max_width` at `bp+6` and `string` at `bp+8` — and never reads `bp+0Ah`. Every call site in the original passes **three** words anyway and cleans six bytes. `Combat_Results_Scroll_Text` does it four times:

```
mov     ax, 2
push    ax
mov     ax, offset GUI_NearMsgString
push    ax
mov     ax, 175
push    ax
call    Get_Paragraph_Max_Height
add     sp, 6
```

| production | asm | third argument |
| --- | --- | --- |
| [Combat.c:15206](../../MoM/src/Combat.c#L15206) | `ovr123/Combat_Results_Scroll_Text.asm:129-136` | `xor ax, ax` — **0** |
| [Combat.c:15231](../../MoM/src/Combat.c#L15231) | `asm:225-232` | 2 |
| [Combat.c:15239](../../MoM/src/Combat.c#L15239) | `asm:278-285` | 2 |
| [Combat.c:15246](../../MoM/src/Combat.c#L15246) | `asm:325-332` | 2 |

The value is the paragraph print type — 0 left, 1 right, 2 centre, 3 full width, per the `switch` in `Get_Paragraph_Max_Width` at [paragrph.c:515-538](../../MoX/src/paragrph.c#L515-L538). It tracks the `Print_Paragraph` call immediately above it at each site: 0 at the first, 2 at the other three. So this is not a stray word copied from the neighbouring width call — the original passes the alignment deliberately, to a function that cannot use it. Height is `paragraph_line_y_start[max_lines - 1] + font_height`, and line breaking happens in `Mark_Paragraph`, which takes no alignment at all.

**Not reproducible in C99.** A long-argument call is a constraint violation, exactly like the short-argument `Gradient_Fill` calls recorded elsewhere. Production passes two arguments at all four sites, which is behaviourally identical since the callee ignores the third. The comments at those four lines record the original form; they are deviation notes, not pending work.

The fifth site is in `ovr113/GUI_DrawNearMessage.asm:51-57`, third argument 2 — the only other listing in `ovr113` that calls `Get_Paragraph_Max_Height`. Its production counterpart has not been identified here; whichever review owns `GUI_DrawNearMessage` should record it.

# Verified faithful

## D6 — `Set_Font_Style` is called with three arguments

The same shape as D5, and tree-wide rather than specific to this chain. Production declares `Set_Font_Style` with four parameters ([Fonts.h:209](../../MoX/src/Fonts.h#L209), defined at [Fonts.c:2135](../../MoX/src/Fonts.c#L2135)) and every call site in the port passes four. In the original, **53 of the 89 call sites across the listings push only three words and clean six bytes**; 36 push four. Both raze functions are three-word sites — `Raze_City_Prompt` asm:62-69, `Raze_City_Prompt_Draw` asm:7-14, 56-64 and 102-109 — and production supplies `ST_NULL` as the missing fourth.

That substitution is not free: `color3` is read at [Fonts.c:2141](../../MoX/src/Fonts.c#L2141) and again at [Fonts.c:2165](../../MoX/src/Fonts.c#L2165), where it selects the special-colour row. The original leaves whatever the stack happened to hold there; the port pins it to 0. Listed rather than filed as a finding because the decision is codebase-wide and predates this chain — adjudicating it belongs to whichever review owns `Set_Font_Style`, not here.

## D7 — field hotkeys are strings in the original, character codes in the port

`Add_Button_Field` and `Add_Hidden_Field` take their hotkey as a `char *` in the listings: `Raze_City_Prompt` pushes `str_hotkey_N__ovr124` (asm:93), `str_hotkey_R__ovr124` (asm:110), `str_hotkey_ESC__ovr124` (asm:142) and `str_empty_string__ovr124` (asm:127). Production declares the parameter `int16_t hotkey` ([Fields.h:678](../../MoX/src/Fields.h#L678), [Fields.h:685](../../MoX/src/Fields.h#L685)) and passes `'N'`, `'R'`, `''` and `ST_NULL`. The mapping is consistent — an empty hotkey string becomes no hotkey — and applies across the whole port, not just here.

---

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

The long multiply/divide was traced through the stack. `empire_population` is pushed as a long first, the city population second, `gold_reserve` ends up in DX:AX, `LXMUL@` takes the product and `LDIV@` divides by what was pushed first — so the expression is `(gold_reserve * city_population) / empire_population`, which is what [Combat.c:15357](../../MoM/src/Combat.c#L15357) computes. Both loops and the neutral-owner branch match.

## `Rampage_Combat_City` — the new lair record

asm:274-341. Seventeen field writes in the asm's order, all correct: both guard types, both counts scaled by 17 (`0x11` — one guard in each nibble), `wx`/`wy`/`wp` from the combat coordinates, `intact = ST_TRUE`, `type = lt_Ruins`, `Loot_Gold = combat_results_gold`, and the six zeroed fields ending with all three `Item_Values`. The field mapping was checked against `WIZARDS.inc:349-367` — `gold` is at 0x0A and `mana` at 0x0C, matching `Loot_Gold` / `Loot_Mana` in [gd_lair_fields.h:16](../../MoM/src/gd_lair_fields.h#L16).

`Change_City_Ownership(_combat_environ_idx, NEUTRAL_PLAYER_IDX)` and `Destroy_City(_combat_environ_idx)` also verified against the push order.

---

# Deviations — `ovr098`


- **Missing redundant reset.** `Process_Retreating_Units.asm:137` writes `mov [bp+Wind_Walker], e_ST_FALSE` on entry to the troops block. The block is only reached when `Wind_Walker` is already `ST_FALSE`, so the store has no effect, but it is a store the asm makes and [Combat.c:4292](../../MoM/src/Combat.c#L4292) does not.
- **Y-loop guard folded into the condition.** `Retreat_From_Combat.asm:913-914` tests `Checked_Y >= WORLD_HEIGHT` **inside** the body and jumps to the increment, so the loop keeps counting to `Min_Y + Diameter`. [Combat.c:4151](../../MoM/src/Combat.c#L4151) folds it into the `while` condition and exits instead. Nothing else happens in those iterations, so the outcome matches.
- **Return value.** `Retreat_From_Combat` is `void` in production. The asm zeroes AX on two of its three exits — the early-out at `asm:96-97` and the end of the attacker branch (`asm:702`, `jmp @@JmpDone__Return_ZERO`) — but the defender branch falls into `@@Done` with AX untouched. `End_Of_Combat` ignores the result either way, so nothing depends on it; recorded because a `void` function would not have produced the two `xor ax, ax`.

---

# Verified faithful — `ovr098`

**Frames.** All three match. `Retreat_From_Combat` has thirteen slotted locals and [Combat.c:3782-3796](../../MoM/src/Combat.c#L3782-L3796) declares them in frame order; `Process_Retreating_Units` has six and [Combat.c:4241-4251](../../MoM/src/Combat.c#L4241-L4251) matches; `Build_Flee_Loss_Message` has eight and [Combat.c:5203-5209](../../MoM/src/Combat.c#L5203-L5209) matches.

**`Retreat_From_Combat`.** The `wp == 9` combat-summon kill; the fleeing rolls in both branches — `Random(2) == 1` for a non-hero and `Random(4) != 1` for a hero in the attacker branch, `Random(2) == 1` and `Random(4) == 1` setting `bus_Dead` in the defender branch; the Black Sleep / Confusion / Web + `Web_HP > 0` chain in both; the road-building reset (`us_Ready`, `Rd_Constr_Left = ST_UNDEFINED`) at all three sites; the `Min_Y`/`Min_X` derivation including the `WORLD_WIDTH` wrap; `Diameter = 3`; the `Square_Has_Lair` / `Player_City_At_Square` pair of `ST_UNDEFINED` tests; and both `Build_Flee_Loss_Message` call sites.

**`Process_Retreating_Units`.** The `Army_At_Square_2` argument list; the three-clause entry guard; the tower scan with its compound loop condition and `goto Return_FALSE`; the four `Unit_Has_*` travel tests in both the sailable and non-sailable branches; the `Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count)` argument list at all three call sites with the `troop_count++` / `bus_Active` pair following each; the non-sailable `Unit_Has_Sailing` abort; and the `return_value` / `goto` exit structure mirroring `@@JmpJmpDone__Return_FALSE` and `@@JmpJmpDone__Return_TRUE`.

**`Build_Flee_Loss_Message`.** The `Type_Counts` clear over `MAX_STACK`; the duplicate-detection inner loop with its `Added_Count` flag; `Add_Comma_Or_And(&Added_Count, Record_Count, GUI_NearMsgString)`; the `itoa` / `strcat` / `cnst_Space_6` / unit-name sequence; the `Huntress` and `Priestess` exclusions from the plural trim; `strlen(...) - 1` and the `'s'` test; and the closing `cnst_Dot8`.

`s_UNIT_TYPE.name` is a `dw` — a pointer, not an inline string (`WIZARDS.inc:221` is `s_BLDG_DATA`'s inline `name db 20 dup(?)`, which is the other convention) — so `push [word ptr _unit_type_table.name+bx]` passing it straight to `strcat` matches production's `char **name` at [UNITTYPE.h:275](../../MoM/src/UNITTYPE.h#L275).

## The raze prompt — `Raze_City_Prompt` and `Raze_City_Prompt_Draw`

Both faithful, 331 asm lines between them, no findings. These are the two `ovr124` functions the review named in its header block and call tree from the start but never gave a coverage row; they were adjudicated on 2026-08-25.

### `Raze_City_Prompt` ([Combat.c:16615](../../MoM/src/Combat.c#L16615), asm 205)

The modal confirm dialog behind the raze decision: save screen state, load the art, build four fields, spin an input loop, restore, return the choice.

**Declaration order is exact.** Seven stack locals, and the frame orders them from `-0Eh` up:

| bp | listing | production |
| --- | --- | --- |
| -0Eh | `Keep_City` | `spare` |
| -0Ch | `Label_Ctrl_Index` | `window_fld` |
| -0Ah | `Esc_Hotkey_Index` | `hotkey_esc` |
| -8 | `Raze_Button_Index` | `raze_button_fld` |
| -6 | `NoRaze_Button_Index` | `no_button_fld` |
| -4 | `input_field_idx` | `input_field_idx` |
| -2 | `Dialog_Box_Height` | `message_box_height` |

Production declares them in exactly that order, then the two register locals `message_height` (SI) and `leave_screen` (DI), which carry no ordering constraint.

**The listing's `Keep_City` name is the right one, and the Renames table's open question resolves in its favour.** The value is `ST_FALSE` on the raze path (asm:178) and `ST_TRUE` on the spare path (asm:184), and it is what the function returns (asm:198). `spare` says the same thing; nothing here argues for reverting beyond taste.

**The two button fields carry each other's art, and that is what the listing does.** `no_button_fld` gets `confirmation_button_yes_seg` (asm:95) and `raze_button_fld` gets `confirmation_button_no_seg` (asm:112). The production comments at [Combat.c:16859-16860](../../MoM/src/Combat.c#L16859-L16860) record why: COMPIX entry 81 *is* the "no raze" button and 82 *is* the "raze" button, so the `_yes_`/`_no_` halves of those global names describe the dialog's answer, not the button's caption. Preserved, correctly.

**Loop shape.** `xor di, di / jmp short loc_A8E67` (asm:163-164) enters at the test, and `or di, di / jz loc_A8E28` (asm:191-192) is the `while(leave_screen == ST_FALSE)`. Inside, `cmp input_field_idx, -1 / jz` (asm:171-172) jumps **to** the body and `cmp ax, Raze_Button_Index / jnz` (asm:174-175) skips **past** it — `||`, matching production's two-clause guard. The second `if` is independent, not an `else`, since asm:179 is reached by fall-through.

**Everything else checks out term for term:** both `LBX_Reload_Next` art loads and their two globals, `Get_Paragraph_Max_Height(166, message)`, `message_box_height = 34 + message_height`, the `(SCREEN_HEIGHT - message_box_height) / 2` centring via the signed-halve idiom at asm:85-87, all four field-add argument lists read off the push order, `Assign_Auto_Function(Raze_City_Prompt_Draw, 1)` as a far pointer plus the `1`, and the five-call teardown at asm:193-197.

### `Raze_City_Prompt_Draw` ([Combat.c:16684](../../MoM/src/Combat.c#L16684), asm 126)

The dialog's redraw callback, installed by the prompt and also called directly at the bottom of each input iteration (asm:187-188).

**No stack locals** — `push bp / mov bp, sp / push si` with no `sub sp`. `paragraph_height` is SI, and unlike its neighbours production leaves it uninitialised, matching the listing exactly.

Straight-line, no branches. Set the font, measure the paragraph, size the window to `message_box_y + paragraph_height + 12`, `Clipped_Draw` the top art, reset the window, `FLIC_Draw` the bottom art at `+ 10`, then three `Print_Paragraph` passes: two shadow passes in `COL_ConfirmShadows` at `(x+10, y+10)` and `(x+9, y+10)`, then `Set_Alias_Color(18)` and the real text in `COL_Dialog_Text` at `(x+9, y+9)`. Every literal, every global, and all five `Print_Paragraph` arguments (`add sp, 0Ah`) match.

The header comment credits drake178's observation that this is byte-identical to `Confirmation_Box_Draw` in `GENDRAW.C`, copied into the overlay to avoid a swap. Nothing in the listing contradicts that.

**One listing artifact.** `loc_A8F24:` (asm:61) sits between `mov ax, 4` and its `push ax`, splitting a single argument setup. Nothing in this listing jumps to it. It has no bearing on the reconstruction — noted so the next reader does not go looking for a branch.

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
- **`Combat_Results_Scroll_Text` returns early on the raze path**, skipping the `CMB_Scroll_MinHeight` floor — asm:471 is `jmp @@Done`, past the clamp at `loc_A722B`. Production preserves this by putting the clamp inside the `else`; the in-code `¿ BUG:` note at [Combat.c:15318](../../MoM/src/Combat.c#L15318) records the observation without acting on it.
- **`Find_Undead_Creator_Type` indexes the wrong player's hero array** — `_players[player_idx].Heroes[_UNITS[battle_units[itr1].unit_idx].Hero_Slot]` where the unit under `itr1` may belong to someone else. Recorded as B1 in `SPLMASTR-Undead_Animation.md`; unchanged.
- **`Rampage_Combat_City` scales guard counts by 17 and truncates to a byte.** Four or more of one guard type overflows the low nibble into the high one.

---

# Listing notes

- `Rampage_Combat_City` asm:83 renders the battle-unit stride as `mov dx, size s_BATTLE_UNIT+s_BATTLE_UNIT.melee`, and `End_Of_Combat` asm:1573 does the same. Every other one of the ~140 sites in this overlay that indexes `battle_units` uses the plain `size s_BATTLE_UNIT`, and the surrounding instruction sequence is identical at all of them. Treat these two as IDA symbolisation artifacts, not as a different computation.
- `End_Of_Combat` asm:850 and asm:625 write `_combat_wx__som_started_anim_ctr`; production uses `_combat_wx` ([Combat.h:1414](../../MoM/src/Combat.h#L1414)). The compound label is Piethawn's, marking storage reused for a second purpose.
- The `_fstrcpy` calls in `Combat_Results_Scroll_Text.asm` (asm:167, 256, 302) push `0` as the segment of the destination. This was recorded here as looking wrong; it is not. `seg009/_fstrcpy.asm:13-21` tests both segment arguments against zero and substitutes `DS`, so **segment 0 means "near, use DS"** — a deliberate sentinel that lets a four-word interface take a near destination without the caller synthesising a segment. The calls pair it with a genuine far source (`push [word ptr _CITIES+2] / push dx` at asm:161-162). The routine is SimTex's own, not a vendor one: it sits in `seg009` and carries no `library function` attribute, whereas `strcpy`, `strcat` and `strlen` sit in `seg000` and are all FLIRT-matched as library code. It is hand-rolled because the binary is medium model — far code, near data — so the library `strcpy` cannot reach an LBX-loaded far block. All of this is moot in the port, which uses near copies.

---

# Build state

All forty-four findings raised against this chain are applied. `cmake --build --preset MSVC-debug --target ReMoM` is **clean** — `momlib.lib` and `ReMoM.exe` both link, with no errors and no warnings anywhere in the chain.

The test suite has not been run. Several fixes move RNG calls: `Random(10)` is no longer scoped to the non-hero arm of the diplomatic bonus, the `Summoned_Unit` scan now stops at the first neutral unit, and `Raze_Check`'s landmass lookups now read the right plane so AI raze decisions change on Myrror. The combat goldens were already pending the `Generate_Combat_Map` work; both sets need regenerating together.

Two findings were withdrawn rather than fixed. They argued a local was absent from the OG source because it had no frame slot. That inference is invalid — the compiler eliminates locals — and it should not appear in future reviews.

---

# The two `ovr098` retreat helpers

`o98p07` and `o98p09`, walked 2026-08-27, 118 asm lines. Both faithful, no findings. They are the
square-lookup and the actually-move-the-unit halves of the flee path this review already covers
through `Retreat_From_Combat` and `Process_Retreating_Units`.

## `Player_City_At_Square` ([Combat.c:4198](../../MoM/src/Combat.c#L4198), asm 67)

Finds the caller's own city on a given world square, or `ST_UNDEFINED`.

**No frame.** No `sub sp`; `city_idx` is SI and `itr_cities` is CX, and only SI is preserved — CX is
caller-saved, so the listing never pushes it.

**The loop condition carries the early-out.** asm:56-60 is the whole test:

```
loc_7F7D9:
cmp     _CX_itr, [_cities]
jge     short @@Done          ; itr >= _cities  -> exit
cmp     _SI_city_idx, -1
jz      short loc_7F77A       ; still unfound   -> body
@@Done:
```

Two conditions, both in the `for`'s continuation clause, so the loop stops on the first match rather
than scanning on. [Combat.c:4227](../../MoM/src/Combat.c#L4227) writes
`(itr_cities < _cities) && (city_idx == ST_UNDEFINED)` — same shape, same place.

**The four comparisons are in a non-obvious order and production preserves it.** The listing tests
`wp` (asm:21-23), then `owner_idx` (asm:30-32), then `wy` (asm:39-41), then `wx` (asm:49-51) — plane
first, coordinates last. Every one is a `jnz` to the loop tail, so it is a short-circuiting `&&`
chain. Production lists them in that order at
[Combat.c:4230-4233](../../MoM/src/Combat.c#L4230-L4233). Re-sorting them to the more natural
`wx, wy, wp, owner_idx` would be a structural deviation for no gain.

### Listing note — there are two functions with this name, and only one is ours

`ovr097/UU_Player_City_At_Square.asm` is a **different body**, not a duplicate: it opens with the
`owner_idx` test rather than `wp`, and its comparison chain is shorter. Production implements the
`ovr098` version — the `wp, owner_idx, wy, wx` order settles it. Do not diff production against the
`ovr097` listing and conclude the reconstruction is wrong.

## `Unit_Try_To_Move` ([Combat.c:4427](../../MoM/src/Combat.c#L4427), asm 51)

Moves one unit to a square if the destination stack has room; returns whether it moved.

**One stack local**, `troop_space` at `bp-2`, plus `_DX_unit_idx` caching the parameter in DX.
Production's second local, `return_value`, is marked `DNE in Dasm` in place — the listing returns
through AX directly (`xor ax, ax` at asm:20, `mov ax, e_ST_TRUE` at asm:45) with no slot for it. The
variable exists only to give the `if`/`else` a single exit, and is correctly flagged as a port aid.

`troop_space = MAX_STACK - troop_count` (asm:14-16); `MAX_STACK` is 9 in
[MOM_DEF.h:119](../../MoX/src/MOM_DEF.h#L119) and `e_MAX_STACK = 9` in `WIZARDS.inc:9202`, so the
constant matches. The guard is `cmp troop_space, 1 / jge` (asm:17-18) — the fall-through returns
`ST_FALSE`, so the source condition is `troop_space < 1`, which is how production writes it.

The three stores go out in the order `wx`, `wy`, `wp` (asm:29-44), matching
[Combat.c:4464-4466](../../MoM/src/Combat.c#L4464-L4466).

**A note on the parameter types.** IDA declares `wx`, `wy` and `wp` as `byte ptr` at `bp+6`, `bp+8`
and `bp+0Ah` — but those are two-byte stack slots, and the spacing proves it. The `byte` typing is
IDA reading `mov al, [bp+wx]` (asm:29) and inferring the width from the access, not from the frame.
They are `int16_t` parameters whose low byte is what the code uses, which is exactly what production
declares, with an `(int8_t)` cast on each store because `s_UNIT.wx` is a byte field. Do not take the
listing's `byte ptr` as evidence the signature should change.
