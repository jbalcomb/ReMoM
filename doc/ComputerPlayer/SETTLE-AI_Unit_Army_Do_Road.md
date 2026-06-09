SETTLE-AI_Unit_Army_Do_Road.md


C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr061\Build_RoadBuilder_Stack.ams
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr061\Build_RoadBuilder_Stack.c

Next_Turn_Proc()
|-> Next_Turn_Calc()
    |-> AI_Next_Turn()
         |-> AI_Execute_Orders()
            |-> AI_Unit_Army_Do_Road(unit_idx);
            |-> AI_Unit_Army_Do_Move()
                |-> Allocate_Reduced_Map()
                |-> MainScr_Create_Reduced_Map_Picture()
                |-> Stack_Move_To()
                    |-> Build_RoadBuilder_Stack()

---

# AI_Unit_Army_Do_Road — Walkthrough

**Sets up road construction for a builder stack.** When a stack carrying `us_BuildRoad` units is dispatched, this routine computes the road path to the unit's destination, sums the stack's construction power, then flips each builder to `us_GOTO` with a per-unit `Rd_Constr_Left` budget so the subsequent move step walks them along the path laying road. It does **not** move anything itself — `AI_Execute_Orders` calls `AI_Unit_Army_Do_Move` right after, for the actual movement.

> **Naming:** OG/IDA name is `AI_UNIT_BuildRoad` (`o100p12`). ReMoM renamed it to `AI_Unit_Army_Do_Road` (matching the `AI_Unit_Army_Do_Move` / `AI_Unit_Army_Do_Settle` sibling convention) and dropped the `__WIP` tag now that the body matches the asm.
>
> **Disassembly note:** this function's own disassembly is `ovr100\AI_UNIT_BuildRoad.asm` — it matches the production body line-for-line. The `ovr061\Build_RoadBuilder_Stack` referenced in the notes above is the *downstream* call reached via `Stack_Move_To` (per the call graph), a separate function.

**Location:** [MoM/src/SETTLE.c:696](../../MoM/src/SETTLE.c#L696) (ends ~[line 770](../../MoM/src/SETTLE.c#L770)). **WZD overlay:** ovr100, p12. **GEMINI variant:** deleted at done-done (was a malformed draft).

## Signature & caller

```c
void AI_Unit_Army_Do_Road(int16_t unit_idx);
```

- Called from [`AI_Execute_Orders`](SETTLE-AI_Execute_Orders.md) — the `us_BuildRoad` case ([SETTLE.c:161](../../MoM/src/SETTLE.c#L161)), immediately followed by `AI_Unit_Army_Do_Move(unit_idx)`.

## Inputs (globals read)

| Source | Used for |
|---|---|
| `_UNITS[unit_idx].{Finished, wx, wy, wp, dst_wx, dst_wy, owner_idx}` | gate + path endpoints + stack gather |
| `_unit_type_table[type].Construction` | per-unit construction power |
| `Unit_Has_Endurance(troop)` | +1 construction bonus |
| `movepath_x_array[2]` / `movepath_y_array[2]` | first path step (written by `OVL_GetRoadPath`) |

## Code walk

```c
if(_UNITS[unit_idx].Finished == ST_TRUE) { return; }            // already done this turn

/* extract wx/wy/wp, dst_wx/dst_wy, owner_idx from _UNITS[unit_idx] */
Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);   // gather the stack
path_length = OVL_GetRoadPath(unit_wx, unit_wy, unit_dst_wx, unit_dst_wy, unit_wp,
                              &movepath_x_array[2], &movepath_y_array[2]);                 // road path → movepath arrays

construction_points = 0;
for each troop:                                                  // pass 1 — total stack construction power
    if Status == us_BuildRoad:
        construction_points += _unit_type_table[type].Construction;
        if Unit_Has_Endurance(troop): construction_points++;

for each troop:                                                  // pass 2 — arm the builders
    if Status == us_BuildRoad:
        Status = us_GOTO;
        Rd_Constr_Left = Turns_To_Build_Road(movepath_x_array[2], movepath_y_array[2], wp);
        Rd_Constr_Left = Rd_Constr_Left / construction_points;   // share the work across the stack
        Rd_From_X = unit_wx;  Rd_From_Y = unit_wy;
```

| Step | Line | asm (`AI_UNIT_BuildRoad.asm`) |
|---|---|---|
| Finished gate → return | [710](../../MoM/src/SETTLE.c#L710) | `cmp Finished, e_ST_TRUE; jnz …; xor ax,ax` ✓ |
| extract 6 fields | [717-723](../../MoM/src/SETTLE.c#L717-L723) | six `cbw` loads ✓ |
| `Player_Army_At_Square` | [725](../../MoM/src/SETTLE.c#L725) | `call j_Player_Army_At_Square` ✓ |
| `OVL_GetRoadPath` | [729](../../MoM/src/SETTLE.c#L729) | `call j_OVL_GetRoadPath` ✓ |
| pass 1: sum `construction_points` (+Endurance) | [735-748](../../MoM/src/SETTLE.c#L735-L748) | `loc_82954` ✓ |
| pass 2: `us_GOTO`, `Rd_Constr_Left`, `Rd_From_X/Y` | [751-768](../../MoM/src/SETTLE.c#L751-L768) | `loc_829C3` ✓ |

**IDA-confirmed faithful** to [ovr100/AI_UNIT_BuildRoad.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr100/AI_UNIT_BuildRoad.asm).

## Notes / quirks (preserve)

- **`Rd_Constr_Left / construction_points`** ([761](../../MoM/src/SETTLE.c#L761)) is in the asm (`idiv construction_points`). A divide-by-zero is theoretically possible if a `us_BuildRoad` unit contributes 0 construction, but in practice `construction_points > 0` because only `Construction>0` units get `us_BuildRoad` (set by [`AI_Stacks_Do_RoadBuild`](AIMOVE-AI_Stacks_Do_RoadBuild.md)). OG-faithful.
- Status flips `us_BuildRoad → us_GOTO`: the builders then move along the computed path; the actual tile-by-tile road laying plays out over subsequent turns via the move / `Rd_Constr_Left` machinery.

## Related references

- [SETTLE-AI_Execute_Orders.md](SETTLE-AI_Execute_Orders.md) — caller (the `us_BuildRoad` case)
- [AIMOVE-AI_Stacks_Do_RoadBuild.md](AIMOVE-AI_Stacks_Do_RoadBuild.md) — the order-*setter* that assigns `us_BuildRoad` + the destination this routine paths to
- [SETTLE-AI_Unit_Army_Do_Move.md](SETTLE-AI_Unit_Army_Do_Move.md) — runs immediately after, to move the now-`us_GOTO` builders
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_UNIT_BuildRoad.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
