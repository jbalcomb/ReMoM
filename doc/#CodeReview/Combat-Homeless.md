Combat-Homeless.md

WIZARDS.EXE  ovr110
// WZD o110p05
Get_Effective_Ranged_Strength()

WIZARDS.EXE  ovr122
// WZD o122p10
Calc_Battlefield_Bonuses()

WIZARDS.EXE  ovr124
// WZD o124p21
Raze_City_Prompt_Draw()
---

# What this list is

Every combat-overlay function that has **not** been taken to done-done **and has no home**: 3 of the 237 `// WZD` overlay slots in the combat sources.

Miscellaneous means homeless. Of the 26 slots that are not done, 23 have a home — some finished review adjudicated the code that calls them, so they are that review's backlog, not a loose end. Those 23 are in the attribution table below, not in this list. The 3 above are what nothing claims.

The list is generated, not hand-kept. Regenerate it with:

```
python3 tools/review_coverage.py --homeless
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
| `DONE` | 202 | a DONE-DONE doc gives it a coverage-table row, or a tracker box is checked |
| `WALKED` | 0 | a DONE-DONE doc walks it under its own heading and declares it faithful, but never added a table row — a bookkeeping hole in the doc, not an unreviewed function |
| `CLAIMED` | 3 | named in a DONE-DONE doc's header block and never adjudicated anywhere |
| `NOT DONE` | 32 | no doc renders a verdict on it |
| `NO NAME` | 0 | an overlay slot nothing in the sources names |

## The `WALKED` bucket is empty

`Load_Combat_Terrain_Pictures` and `Map_Tile_EMS_Page_As_Sandbox` were the two entries here; both now carry coverage-table rows in [Combat-Combat_Screen.md](Combat-Combat_Screen.md), so nothing is walked-but-unrecorded any more.

## The `CLAIMED` functions

Two remain, both named in a DONE-DONE doc’s header block and adjudicated nowhere:

- **`Calc_Battlefield_Bonuses`**, claimed by [Combat-Combat_Screen.md](Combat-Combat_Screen.md). That doc’s header block records the call tree `Combat_Screen` reaches, not the set it adjudicated — [line 718](Combat-Combat_Screen.md#L718) says as much of the input-loop handlers — so a header-block name is an intent, not a verdict. It is going to [Combat-Combat.md](Combat-Combat.md).
- **`Raze_City_Prompt`**, claimed by [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md), which names it in its ASM listing block and its call tree but gave it no coverage-table row. Its own callee `Raze_City_Prompt_Draw` is homeless for exactly that reason — with the caller unadjudicated, attribution has nothing to route the callee to. Both are going to that review together.

`Combat_Grid_Screen_Coordinates` was the third entry here; it now carries a verdict in [Combat-Miscellaneous.md](Combat-Miscellaneous.md).

## Which review each one belongs to

A function belongs with whatever review covered the code that calls it. `--attribution` walks every call site of an uncovered function, finds the enclosing function, and reports which DONE-DONE review adjudicated *that*. The table below is `--attribution-md` verbatim:

```
python3 tools/review_coverage.py --attribution        full form, every candidate home
python3 tools/review_coverage.py --attribution-md     this table
```

| review it belongs to | n | functions |
| --- | --- | --- |
| [Combat-Combat_Screen.md](Combat-Combat_Screen.md) | 5 | `Combat_Next_Turn`, `Tactical_Combat_Draw_Buttons`, `Battle_Unit_Melee_Attack_Icon`, `Battle_Unit_Ranged_Attack_Icon`, `Combat_Cache_Read` |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | 3 | `Draw_Active_Unit_Stats_And_Icons`, `Draw_Active_Unit_Damage_Bar`, `Next_Battle_Unit_Nearest_Available` |
| [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md) | 3 | `Player_City_At_Square`, `Unit_Try_To_Move`, `Raze_City_Prompt` |
| [Combat-AI_Turn.md](Combat-AI_Turn.md) | 2 | `Do_Auto_Unit_Turn`, `Total_Ranged_Attack_Strength` |
| [Combat-Battle_Unit_Process_Attack.md](Combat-Battle_Unit_Process_Attack.md) | 2 | `Eliminated_Opponent`, `Battle_Unit_Is_Summoned_Creature` |
| [Combat-Combat.md](Combat-Combat.md) | 2 | `Combat_City_Capture`, `o153p24_empty_function` |
| [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) | 2 | `Screen_To_Combat_Grid_Cell_X_And_Offset`, `Screen_To_Combat_Grid_Cell_Y_And_Offset` |
| [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) | 1 | `Auto_Move_Unit` |
| [Combat-Combat_Summon.md](Combat-Combat_Summon.md) | 1 | `Battle_Unit_Pict_Open` |
| [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md) | 1 | `Apply_Mana_Leak` |
| [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) | 1 | `Reload_Battle_Unit_Move_Sound` |
| [Combat-Spell_Cast_AI_Select.md](Combat-Spell_Cast_AI_Select.md) | 1 | `Get_Effective_Melee_Strength` |
| [Combat-Spell_Cast_AI_Target.md](Combat-Spell_Cast_AI_Target.md) | 1 | `Target_Is_Visible` |

**3 have no covered caller at all** â€” their callers are themselves unreviewed, so no existing session claims them: `Get_Effective_Ranged_Strength`, `Calc_Battlefield_Bonuses`, `Raze_City_Prompt_Draw`.

## Cross-doc referrals

A claim of the form "X is covered in <other doc>" is a coverage claim like any other, and nothing was checking them. `--referrals` resolves each one against the actual verdict inventory.

```
python3 tools/review_coverage.py --referrals
```

**Ten are unsupported.** Five name a function that no doc renders a verdict on:

| doc making the claim | function | claimed covered in |
| --- | --- | --- |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Draw_Active_Unit_Stats_And_Icons` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md) |
| [Combat-Combat_Figure_Compose.md](Combat-Combat_Figure_Compose.md) | `Draw_Active_Unit_Damage_Bar` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md) |
| [Combat-Combat_Screen.md](Combat-Combat_Screen.md) | `Combat_Grid_Screen_Coordinates` | [Combat-Miscellaneous.md](Combat-Miscellaneous.md) |
| [Combat-Combat_Screen.md](Combat-Combat_Screen.md) | `Calc_Battlefield_Bonuses` | [Combat-Miscellaneous.md](Combat-Miscellaneous.md) |
| [Combat-Generate_Combat_Map.md](Combat-Generate_Combat_Map.md) | `Combat_Grid_Screen_Coordinates` | [Combat-Combat_Screen.md](Combat-Combat_Screen.md), [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) |

The last three of those are self-referential in effect: two reviews point at *this* file for `Combat_Grid_Screen_Coordinates` and `Calc_Battlefield_Bonuses`, and this file is a homeless list, not an adjudication. Both are also in the `CLAIMED` bucket above - the same two functions, reached by a different route.

Five more point at the wrong doc for a function that *is* covered somewhere: `Spell_Resistance_Modifier`, `Draw_Active_Unit_Window` (twice), `Build_Battlefield` and `Load_Battle_Unit`. Those are cheap to fix - the referral just needs to name the doc that actually holds the verdict.

## Known parsing edges

- Overlay prefixes are written three ways in the sources — `s163`, `o163`, `ovr163` — so marker keys are normalised to (overlay number, part number). Marker text in this list is reproduced exactly as it appears in the source, so a `grep` on any line finds it.
- Two slots have no production body, only a commented-out name: `s96p04` and `s96p06`. This bullet previously listed four, adding `111p06` and `o112p11` - **that was wrong**. Both of those have real bodies and are now walked in [Combat-Spell_Cast_AI_Target.md](Combat-Spell_Cast_AI_Target.md); the tool mis-reported them while they still carried drake178's `UU15_` / `UU_` prefixes, and started resolving them once they were renamed to `AITP_Disintegrate` and `AITP_Word_Of_Recall`. Treat a `notimpl` on a `UU`-prefixed slot as a prompt to open the file, not as a fact.
- The two functions that used to sit in the `WALKED` bucket are cited in [Combat-Combat_Screen.md](Combat-Combat_Screen.md) at production lines that no longer exist - 20387, 21605, 21616 and 22885 against a 23376-line [Combat.c](../../MoM/src/Combat.c). The true definitions are [Map_Tile_EMS_Page_As_Sandbox](../../MoM/src/Combat.c#L20215) at 20215 and [Load_Combat_Terrain_Pictures](../../MoM/src/Combat.c#L22713) at 22713. The line numbers in that doc drift every time `Combat.c` moves; the walks themselves are still good.
- Two header-block listings name `.asm` files that are not on disk: `ovr163/Allocate_Combat_Base_Blocks.asm` in [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) and `ovr098/Prepare_All_Battle_Units.asm` in [Combat-Init_Prep_Etc.md](Combat-Init_Prep_Etc.md). Both are production names written where the IDA name belongs.

## Adjudicated, but owned by nobody

Functions whose callers span three or more reviews land here by the `MISC_DOC_SPREAD` rule in [tools/review_coverage.py](../../tools/review_coverage.py) — no single review can own them, so their verdicts are recorded in this doc.

**`Combat_Grid_Screen_Coordinates`** ([Combat.c:22482](../../MoM/src/Combat.c#L22482), `ovr154/Combat_Grid_Screen_Coordinates.asm`, 44 asm lines) — **faithful**. Twelve call sites across four reviews: [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) (8), [Combat-Generate_Combat_Map.md](Combat-Generate_Combat_Map.md) (2), [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) (1), [Combat-Combat_Screen.md](Combat-Combat_Screen.md) (1).

`sx = ((cgx - cgy) * 16) + 158` is `sub` / `shl ax, 4` / `add 158` (asm:17-22); `sy = ((cgx + cgy) * 8) - 80` is `add` / `shl ax, 3` / `add -80` (asm:23-28). The sub-cell offsets follow — `(ox - oy) * 2` into `sx` via `shl ax, 1`, a bare `(ox + oy)` into `sy` — then both store through the out-pointers. Both accumulators are register locals with no stack slots, so their declaration order carries no constraint.

The asm parameter names invert what a reader expects: `cgc2` at `bp+6` is the **x** cell and `cgc1` at `bp+8` is the **y** cell, which is why production's `cgx, cgy` order is correct.
