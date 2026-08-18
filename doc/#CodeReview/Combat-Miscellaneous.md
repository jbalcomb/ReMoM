Combat-Miscellaneous.md

WIZARDS.EXE  ovr091
// WZD s91p08
Target_Is_Visible()

WIZARDS.EXE  ovr096
// WZD s96p04
UU_IDK_CMB_s7DE08()    // no production body — comment-only placeholder
// WZD s96p06
UU_IDK_Main_Screen_Draw()    // no production body — comment-only placeholder

WIZARDS.EXE  ovr098
// WZD o98p11
Ranged_Mouse_Image()

WIZARDS.EXE  ovr103
// WZD s103p02
Combat_Information_Window_Draw()
// WZD s103p03
Combat_Info_Effects()
// WZD s103p04
Combat_Info_Effects_Base()

WIZARDS.EXE  ovr111
// WZD 111p02
AITP_DarknessLight()
// WZD 111p03
AITP_Healing()
// WZD 111p04
AITP_WarpWood()
// WZD 111p05
AITP_WarpCreature()
// WZD 111p06
UU15_AITP_Disintegrate()    // no production body — comment-only placeholder
// WZD 111p07
AITP_DispelMagic()
// WZD 111p09
AITP_HolyWord()

WIZARDS.EXE  ovr112
// WZD o112p06
Spell_Resistance_Modifier()
// WZD o112p07
Combat_Casting_Cost_Multiplier()
// WZD o112p08
AITP_EarthToMud()
// WZD o112p09
AITP_Disrupt()
// WZD o112p10
AITP_CracksCall()
// WZD o112p11
UU_AITP_WordofRecall()    // no production body — comment-only placeholder
// WZD o112p12
AITP_RecallHero()

WIZARDS.EXE  ovr113
// WZD o113p01
Combat_Screen_Assign_Mouse_Images()
// WZD o113p02
Combat_Spell_Target_Screen_Draw()
// WZD o113p13
Combat_Spell_Dispel_Attempt()
// WZD o113p14
Combat_Cast_Spell_Message()

WIZARDS.EXE  ovr114
// WZD o114p02
AI_BU_ProcessAction()
// WZD o114p03
Sort_Battle_Units()
// WZD o114p06
AI_BU_AssignAction()
// WZD o114p07
Choose_Target_And_Action()

WIZARDS.EXE  ovr122
// WZD o122p10
Calc_Battlefield_Bonuses()
// WZD o122p13
Target_Unit_Value()

WIZARDS.EXE  ovr124
// WZD o124p01
Total_Ranged_Attack_Strength()
// WZD o124p03
AI_RestrictToCity__WIP()
// WZD o124p06
Get_Player_Mode()
// WZD o124p09
Combat_Load_Spell_Sound_Effect()
// WZD o124p18
Retreat_Check()
// WZD o124p21
Raze_City_Prompt_Draw()

WIZARDS.EXE  ovr139
// WZD ovr139p02
AI_EvaluateCmbtSpell()
// WZD ovr139p03
Effective_Battle_Unit_Strength()
// WZD ovr139p05
AI_UnitThreatRealms()
// WZD ovr139p06
AI_CombatSpellList()

WIZARDS.EXE  ovr153
// WZD ovr153p01
NX_IDK_CombatInit_Tactical()
// WZD ovr153p22
o153p22_empty_function()

WIZARDS.EXE  ovr154
// WZD ovr154p12
Combat_Grid_Screen_Coordinates()
// WZD ovr154p13
Apply_Earth_To_Mud()

WIZARDS.EXE  ovr163
// WZD ovr163p03
Wall_Rise_Load()

---

# What this list is

Every combat-overlay function that has **not** been taken to done-done **and has no home**: 49 of the 237 `// WZD` overlay slots in the combat sources.

Miscellaneous means homeless. Of the 96 slots that are not done, 47 have a home — some finished review adjudicated the code that calls them, so they are that review's backlog, not a loose end. Those 47 are in the attribution table below, not in this list. The 49 above are what nothing claims.

The list is generated, not hand-kept. Regenerate it with:

```
python3 tools/review_coverage.py --misc
```

Run the tool with no arguments for the full reconciliation report, or `--csv out.csv` for the per-slot matrix.

## How a function is classified

[tools/review_coverage.py](../../tools/review_coverage.py) compares four independent inventories and never guesses — where they disagree it reports the difference rather than picking a winner.

| inventory | what it is |
| --- | --- |
| **markers** | every `// WZD <overlay><part>` in [Combat.c](../../MoM/src/Combat.c), [COMBINIT.c](../../MoM/src/COMBINIT.c), [CMBMAGIC.c](../../MoM/src/CMBMAGIC.c), [CMBTAI.c](../../MoM/src/CMBTAI.c), [CMBTMVPT.c](../../MoM/src/CMBTMVPT.c), resolved to the name that follows it |
| **intent** | the `ovrNNN/Name.asm` paths in a review doc's header block, bridged to production names through the doc's rename ledger and its coverage table |
| **verdict** | the rows of a review doc's coverage table, each rendering a fidelity result on one function |
| **tracker** | the checked boxes in [stub_wip_todo.md](../#TODO/stub_wip_todo.md) |

That yields five states. **`CLAIMED` and `NOT DONE` qualify for this list; attribution then removes any of them that a finished review can be held responsible for.**

| state | count | meaning |
| --- | --- | --- |
| `DONE` | 139 | a DONE-DONE doc gives it a coverage-table row, or a tracker box is checked |
| `WALKED` | 2 | a DONE-DONE doc walks it under its own heading and declares it faithful, but never added a table row — a bookkeeping hole in the doc, not an unreviewed function |
| `CLAIMED` | 18 | named in a DONE-DONE doc's header block and never adjudicated anywhere |
| `NOT DONE` | 78 | no doc renders a verdict on it |
| `NO NAME` | 0 | an overlay slot nothing in the sources names |

## The two `WALKED` functions

Both need a coverage-table row added to [Combat-Combat_Screen.md](Combat-Combat_Screen.md); neither needs reviewing again.

- **`Load_Combat_Terrain_Pictures`** — [Combat.c:22883](../../MoM/src/Combat.c#L22883). Walked at [Combat-Combat_Screen.md:593](Combat-Combat_Screen.md#L593), opening "Faithful", with the `cts_Water` bare-zero test, the five terrain arms, all three load loops and the 30-byte filename buffer each checked against the listing.
- **`Map_Tile_EMS_Page_As_Sandbox`** — [Combat.c:20385](../../MoM/src/Combat.c#L20385). Walked at [Combat-Combat_Screen.md:604](Combat-Combat_Screen.md#L604) alongside `Claim_EMS_Page_For_Figure_Set` ("Both faithful"), which *does* have a row.

## The `CLAIMED` functions

Sixteen of the eighteen are listed in the header block of [Combat-Combat_Screen.md](Combat-Combat_Screen.md), which is consistent with that doc's own words — [line 718](Combat-Combat_Screen.md#L718) names thirteen input-loop handlers as "not covered, and the `faithful` verdicts do not extend to them". The header block records the call tree `Combat_Screen` reaches, not the set the review adjudicated; the two were never meant to match. `Combat_Screen_Draw` is the clearest case: its listing is named at [Combat-Combat_Screen.md:68](Combat-Combat_Screen.md#L68) and it is explicitly disclaimed at line 718.

The remaining two are `Raze_City_Prompt`, claimed by [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md), and `Combat_Grid_Screen_Coordinates`.

## Which review each one belongs to

A function belongs with whatever review covered the code that calls it. `--attribution` walks every call site of an uncovered function, finds the enclosing function, and reports which DONE-DONE review adjudicated *that*. The table below is `--attribution-md` verbatim:

```
python3 tools/review_coverage.py --attribution        full form, every candidate home
python3 tools/review_coverage.py --attribution-md     this table
```

| review it belongs to | n | functions |
| --- | --- | --- |
| [Combat-Combat_Screen.md](Combat-Combat_Screen.md) | 17 | `Switch_Active_Battle_Unit`, `Auto_Cast_Spell_And_Do_Combat_Turn`, `Combat_Next_Turn`, `Assign_Mouse_Images`, `Add_Combat_Enchantment_Fields`, `Turn_Off_Auto_Combat`, `Combat_Information_Window`, `Combat_Info_Effects_Count`, `Combat_Node_Type`, `Next_Battle_Unit`, `Combat_Cast_Spell_Error`, `Move_Confused`, `Calc_Battlefield_Bonuses`, `Combat_Structure`, `Undeployable_Battle_Units_On_Water`, `Combat_Cache_Read`, `Combat_Cache_Write` |
| [Combat-Spell_Cast.md](Combat-Spell_Cast.md) | 8 | `Update_Combat_Enchantments_Icon_And_Help`, `Cast_Spell_On_Battle_Unit`, `Combat_Compose_Spellbook_Background`, `Do_Legal_Spell_Check`, `Combat_Casting_Cost_Multiplier`, `Combat_Spell_Target_Screen`, `AI_SelectCmbtSpell`, `AI_SetCombatRealms` |
| [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md) | 3 | `Get_Effective_Hits`, `Eliminated_Opponent`, `Battle_Unit_Is_Summoned_Creature` |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | 3 | `Draw_Active_Unit_Stats_And_Icons`, `Draw_Active_Unit_Damage_Bar`, `Next_Battle_Unit_Nearest_Available` |
| [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) | 3 | `Combat_Grid_Screen_Coordinates`, `Screen_To_Combat_Grid_Cell_X_And_Offset`, `Screen_To_Combat_Grid_Cell_Y_And_Offset` |
| [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md) | 3 | `Player_City_At_Square`, `Unit_Try_To_Move`, `Raze_City_Prompt` |
| [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) | 2 | `Do_Auto_Unit_Turn`, `Auto_Move_Unit` |
| [Combat-Combat.md](Combat-Combat.md) | 2 | `Combat_City_Capture`, `o153p24_empty_function` |
| [Combat-Strategic_Combat.md](Combat-Strategic_Combat.md) | 2 | `Get_Effective_Ranged_Strength`, `Get_Effective_Melee_Strength` |
| [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) | 1 | `Combat_Screen_Draw` |
| [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md) | 1 | `Apply_Mana_Leak` |
| [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) | 1 | `Reload_Battle_Unit_Move_Sound` |
| [Combat-Summon_Demon.md](Combat-Summon_Demon.md) | 1 | `Battle_Unit_Pict_Open` |

**49 have no covered caller at all** — their callers are themselves unreviewed, so no existing session claims them: `Target_Is_Visible`, `UU_IDK_CMB_s7DE08`, `UU_IDK_Main_Screen_Draw`, `Ranged_Mouse_Image`, `Tactical_Combat_Draw_Buttons`, `Draw_Spell_Information_Window`, `Combat_Information_Window_Draw`, `Combat_Info_Effects`, `Combat_Info_Effects_Base`, `Battle_Unit_Melee_Attack_Icon`, `Battle_Unit_Ranged_Attack_Icon`, `Draw_Combat_Unit_Display`, `AITP_DarknessLight`, `AITP_Healing`, `AITP_WarpWood`, `AITP_WarpCreature`, `UU15_AITP_Disintegrate`, `AITP_DispelMagic`, `AITP_HolyWord`, `Spell_Resistance_Modifier`, `AITP_EarthToMud`, `AITP_Disrupt`, `AITP_CracksCall`, `UU_AITP_WordofRecall`, `AITP_RecallHero`, `Combat_Screen_Assign_Mouse_Images`, `Combat_Spell_Target_Screen_Draw`, `Combat_Spell_Dispel_Attempt`, `Combat_Cast_Spell_Message`, `AI_BU_ProcessAction`, `Sort_Battle_Units`, `Auto_Do_Combat_Turn`, `AI_BU_AssignAction`, `Choose_Target_And_Action`, `Target_Unit_Value`, `Total_Ranged_Attack_Strength`, `AI_RestrictToCity__WIP`, `Get_Player_Mode`, `Combat_Load_Spell_Sound_Effect`, `Retreat_Check`, `Raze_City_Prompt_Draw`, `AI_EvaluateCmbtSpell`, `Effective_Battle_Unit_Strength`, `AI_UnitThreatRealms`, `AI_CombatSpellList`, `NX_IDK_CombatInit_Tactical`, `o153p22_empty_function`, `Apply_Earth_To_Mud`, `Wall_Rise_Load`.

## Cross-doc referrals

A claim of the form "X is covered in <other doc>" is a coverage claim like any other, and nothing was checking them. `--referrals` does:

```
python3 tools/review_coverage.py --referrals
```

Four are false — the target doc renders no verdict on the named function, and neither does any other:

| doc making the claim | function | claimed covered in |
| --- | --- | --- |
| [Combat-Generate_Combat_Map.md:212](Combat-Generate_Combat_Map.md#L212) | `Combat_Grid_Screen_Coordinates` | [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) — the name does not appear in that doc at all |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Draw_Active_Unit_Stats_And_Icons` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md) |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Draw_Active_Unit_Damage_Bar` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md) |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Combat_Screen_Draw` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md), which disclaims it at [line 718](Combat-Combat_Screen.md#L718) |

Six more point at the wrong doc for a function that *is* covered somewhere: `Combat_Compose_Background`, `Build_Battlefield` and `Load_Battle_Unit` (all actually in [Combat-Combat_Screen.md](Combat-Combat_Screen.md)) and `Draw_Active_Unit_Window` (actually in [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md)).

## Known parsing edges

- Overlay prefixes are written three ways in the sources — `s163`, `o163`, `ovr163` — so marker keys are normalised to (overlay number, part number). Marker text in this list is reproduced exactly as it appears in the source, so a `grep` on any line finds it.
- Four slots have no production body, only a commented-out name: `s96p04`, `s96p06`, `111p06`, `o112p11` — all four are in this list. The overlay slot is unreconstructed rather than the function unreviewed.
- Both `WALKED` sections in [Combat-Combat_Screen.md](Combat-Combat_Screen.md) cite production lines that no longer exist — 23688, 21599 and 21614 against a 23577-line [Combat.c](../../MoM/src/Combat.c). The true definitions are at [22883](../../MoM/src/Combat.c#L22883) and [20385](../../MoM/src/Combat.c#L20385). The line numbers in that doc drifted; the walks themselves are still good.
- Two header-block listings name `.asm` files that are not on disk: `ovr163/Allocate_Combat_Base_Blocks.asm` in [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) and `ovr098/Prepare_All_Battle_Units.asm` in [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md). Both are production names written where the IDA name belongs.

## Adjudicated, but owned by nobody

Functions whose callers span three or more reviews land here by the `MISC_DOC_SPREAD` rule in [tools/review_coverage.py](../../tools/review_coverage.py) — no single review can own them, so their verdicts are recorded in this doc.

**`Combat_Grid_Screen_Coordinates`** ([Combat.c:22653](../../MoM/src/Combat.c#L22653), `ovr154/Combat_Grid_Screen_Coordinates.asm`, 44 asm lines) — **faithful**. Twelve call sites across four reviews: [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) (8), [Combat-Generate_Combat_Map.md](Combat-Generate_Combat_Map.md) (2), [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) (1), [Combat-Combat_Screen.md](Combat-Combat_Screen.md) (1).

`sx = ((cgx - cgy) * 16) + 158` is `sub` / `shl ax, 4` / `add 158` (asm:17-22); `sy = ((cgx + cgy) * 8) - 80` is `add` / `shl ax, 3` / `add -80` (asm:23-28). The sub-cell offsets follow — `(ox - oy) * 2` into `sx` via `shl ax, 1`, a bare `(ox + oy)` into `sy` — then both store through the out-pointers. Both accumulators are register locals with no stack slots, so their declaration order carries no constraint.

The asm parameter names invert what a reader expects: `cgc2` at `bp+6` is the **x** cell and `cgc1` at `bp+8` is the **y** cell, which is why production's `cgx, cgy` order is correct.
