Combat-Assign_Combat_Grids.md

BU_SetCityMovement()  ==>  Update_Move_Map_City_Area_Restrictions()
BU_GetInstaMoveType__WIP()  ==>  Battle_Unit_Instant_Movement_Mode()
BU_GetCmbtMoveType()  ==>  Battle_Unit_Movement_Icon()
Battle_Unit_Movement_Icon()  ==>  Battle_Unit_Movement_Mode()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Assign_Combat_Grids.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Set_Movement_Cost_Map.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Get_Combat_Grid_Cell_X.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr154\Get_Combat_Grid_Cell_Y.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr163\Allocate_Combat_Base_Blocks.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Combat_Grid_Cell_Has_City_Wall.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\BU_SetCityMovement.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Battle_Unit_Is_Within_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr121\Unit_Moves2.asm

Update_Move_Map_City_Area_Restrictions
    |-> Battle_Unit_Is_Within_City

Assign_Combat_Grids()
    |-> Set_Movement_Cost_Map()
        |-> Battle_Unit_Movement_Icon()
        |-> Battle_Unit_Instant_Movement_Mode()
    |-> Update_Move_Map_City_Area_Restrictions()
    |-> Battle_Unit_Instant_Movement_Mode()
    |-> Unit_Moves2()
    |-> Combat_Grid_Cell_Has_City_Wall()
    |-> Combat_Move_Path_Valid()

Assign_Combat_Grids()
    |-> j_Assign_Combat_Grids()
        |-> Combat_Screen()
        |-> Turn_Off_Auto_Combat()
        |-> Auto_Do_Combat_Turn()
        |-> AI_EvaluateCmbtSpell()

---


# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-03).** All eight functions are 1:1 with their listings.

`Combat.c` and `NEXTTURN.c` compile clean, no warnings; `MOM_tests` runs 157/157 green.

## Scope

| Function | Production | ASM (ground truth) |
| --- | --- | --- |
| `Assign_Combat_Grids` | [Combat.c:3483-3607](../../MoM/src/Combat.c#L3483-L3607) | `ovr091/Assign_Combat_Grids.asm` |
| `Set_Movement_Cost_Map` | [Combat.c:3001-3056](../../MoM/src/Combat.c#L3001-L3056) | `ovr091/Set_Movement_Cost_Map.asm` |
| `Battle_Unit_Instant_Movement_Mode` | [Combat.c:3738-3752](../../MoM/src/Combat.c#L3738-L3752) | `ovr091/BU_GetInstaMoveType__WIP.asm` |
| `Combat_Grid_Cell_Has_City_Wall` | [Combat.c:21750-21785](../../MoM/src/Combat.c#L21750-L21785) | `ovr124/Combat_Grid_Cell_Has_City_Wall.asm` |
| `Update_Move_Map_City_Area_Restrictions` | [Combat.c:20912-21085](../../MoM/src/Combat.c#L20912-L21085) | `ovr124/BU_SetCityMovement.asm` |
| `Unit_Moves2` | [NEXTTURN.c:1529-1622](../../MoM/src/NEXTTURN.c#L1529-L1622) | `ovr121/Unit_Moves2.asm` |
| `Battle_Unit_Is_Within_City` | [Combat.c:21728-21746](../../MoM/src/Combat.c#L21728-L21746) | `ovr124/Battle_Unit_Is_Within_City.asm` |
| `Battle_Unit_Movement_Mode` | [Combat.c:5627-5655](../../MoM/src/Combat.c#L5627-L5655) | `ovr098/Battle_Unit_Movement_Icon.asm` |

`Combat_Move_Path_Valid` has its own review in [Combat-Combat_Move_Path_Valid.md](Combat-Combat_Move_Path_Valid.md); every other function in the call graph is covered here.

`asm:N` refers to the listing named in the row above.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are **not** used to infer how many variables the source declared, and an emitted comparison is not treated as evidence of the expression that produced it — the listing shows what the compiler emitted, not what was written.

**On line numbers.** `Combat.c` moved repeatedly during this review, so citations are deliberately sparse: section anchors and finding sites only, with structure carried by asm line references, which are stable. Re-verify any production line before relying on it.

---

# Assign_Combat_Grids

## Frame map

`sub sp, 12h` (asm:16) = 18 bytes, plus two register locals.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-12h` | `Can_Teleport` | `has_instant_movement_mode` |
| `bp-10h` | `Teleport_Type` | `instant_movement_mode` |
| `bp-0Eh` | `moves2` | `moves2` |
| `bp-0Ch` | `uu_max_moves2` | `uu_max_moves2` |
| `bp-0Ah` | `uu_count_of_reachable_cells` | `uu_count_of_reachable_cells` |
| `bp-8` | `uu_cgy` | `uu_cgy` |
| `bp-6` | `uu_cgx` | `uu_cgx` |
| `bp-4` | `cgy_offset` | `cgy_offset` |
| `bp-2` | `itr__itr_x` | `itr` and `itr_x` both map here |
| `si` | `_SI_itr_y` | `itr_y` |
| `di` | `useable_moves2` | `useable_moves2` |

## Faithful — verified, leave alone

- **Call and statement order** — `Set_Movement_Cost_Map` (asm:21) -> battle-unit `INF` loop (asm:23-57) -> vortex `INF` loop (asm:58-84) -> central structure (asm:85-89) -> walled/flying-fortress gate (asm:90-100) -> instant-move mode (asm:102-114) -> `moves2`/`uu_max_moves2`/`uu_cgx`/`uu_cgy` (asm:116-146) -> clear the action map to `COMBAT_CELL_NO_ACTION` (asm:147-165) -> wall-crusher 99 block (asm:166-211) -> unit indices (asm:213-254) -> `useable_moves2` (asm:255-275) -> `uu_count_of_reachable_cells = 0` (asm:277) -> `Combat_Move_Path_Valid` (asm:278-292) -> reachability loop (asm:293-324) -> own cell (asm:325-343). Production [3497-3606](../../MoM/src/Combat.c#L3497-L3606) matches step for step.
- **Central structure square** — asm:89 writes displacement `0EDh` = 237 = `11 * 21 + 6`, i.e. cgx 6 / cgy 11. Production [3512](../../MoM/src/Combat.c#L3512) writes `COMBAT_STRUCTURE_IDX`, which evaluates to 237. Pinned by [test_Combat_Structure_Index.cpp](../../MoM/tests/test_Combat_Structure_Index.cpp).
- **The walled/flying-fortress gate is an OR** — asm:92-96 falls through when `walled == ST_TRUE`, otherwise calls only when `FLYING_FORTRESS > 0` (`jle` skips). Production [3515](../../MoM/src/Combat.c#L3515).
- **Instant-move detection** — flag cleared, then set iff the mode is 6 or 7 (asm:102-114). Production [3522-3530](../../MoM/src/Combat.c#L3522-L3530).
- **Wall-crusher block conditions** — `walled == ST_TRUE` (asm:167), `controller_idx == _combat_attacker_player` (asm:174-177), `Abilities & UA_WALLCRUSHER` (asm:183), in that order. Production [3545](../../MoM/src/Combat.c#L3545).
- **`Combat_Grid_Cell_Has_City_Wall(itr_x, itr_y)` argument order** — asm:191-192 pushes `itr_y` then `itr_x`, so cdecl right-to-left makes X first. Production [3556](../../MoM/src/Combat.c#L3556).
- **Unit-index assignment** — `status == bus_Active` (asm:221) and `Image_Effect != 5` (asm:228), storing the low byte of the index (asm:246). Production [3574](../../MoM/src/Combat.c#L3574).
- **`useable_moves2`** — zeroed (asm:255), set to `movement_points` when no instant-move mode (asm:256-265), otherwise 20 only when `movement_points > 0` (asm:273-275, `jle` skips). Production [3580](../../MoM/src/Combat.c#L3580) / [3589](../../MoM/src/Combat.c#L3589).
- **Reachability scan** — `cgy_offset = itr_y * COMBAT_GRID_WIDTH` (asm:296-299), test `== 1` (asm:308), write and bump the counter (asm:314-315). Production [3596](../../MoM/src/Combat.c#L3596).
- **Own cell stamped last** — asm:325-343, after the reachability loop. Production [3606](../../MoM/src/Combat.c#L3606).
- **Five write-only locals** — `moves2` (asm:123), `uu_max_moves2` (asm:132), `uu_cgx` (asm:139), `uu_cgy` (asm:146), `uu_count_of_reachable_cells` (asm:277, 315) are written and never read in the listing. OG code; they stay.

Grid bounds are a mix of symbol and literal in the listing — `e_COMBAT_GRID_WIDTH`/`_HEIGHT` at asm:160/164/323, bare `21` at asm:39/67/206/319, bare `22` at asm:210 — same values throughout.

---

# Set_Movement_Cost_Map

Faithful. `movement_type = Battle_Unit_Movement_Icon(...)` (asm:12-15), then `Battle_Unit_Instant_Movement_Mode` (asm:16-21) overrides it when the result is `> 0` (`or si,si` / `jle`, asm:22-24). Production [3007-3012](../../MoM/src/Combat.c#L3007-L3012).

The dispatch is a jump table with a preceding unsigned bound check (`cmp bx, 7` / `jbe`, asm:27-32) — what Borland emits for a dense `switch` over 0..7 with no `default`. Cases: 0/1 -> `MoveCost_Ground`, 2 -> `MoveCost_Teleport`, 3 -> `MoveCost_Ground2`, 4 -> `MoveCost_Sailing`, 5 -> the per-cell minimum loop, 6/7 -> `MoveCost_Teleport`. Production [3013](../../MoM/src/Combat.c#L3013) onward. The listing reaches case 2 via `jmp jt_BU_GetMoveMap_06_07`, the two identical `memcpy` blocks having been tail-merged.

Case 5 (asm:61-119): nested loops, inner bound 21 (asm:114), outer 22 (asm:118), row stride 21. The comparison at asm:84 has `al` holding Sailing with `jbe` taking the Sailing branch — the per-cell minimum of Sailing and Ground. Production [3034](../../MoM/src/Combat.c#L3034) onward expresses it as "if Sailing > Ground take Ground, else Sailing", the same selection.

`instant_movement_type` and `itr_y` share `si` in the listing. That is ordinary register allocation across non-overlapping live ranges and says nothing about how many variables the source declared; the same caution applies to shared **stack** slots.

---

# Battle_Unit_Instant_Movement_Mode

Faithful, 1:1. `Move_Flags & MV_TELEPORT` -> 6 (asm:12-14), `Move_Flags & MV_MERGING` -> 7 (asm:23-25), otherwise 0 (asm:28). No stack locals in the frame; production declares none. Production [3740](../../MoM/src/Combat.c#L3740) / [3744](../../MoM/src/Combat.c#L3744).

The two values are slots in the 0..7 movement-mode space `Set_Movement_Cost_Map` switches on, shared with `Battle_Unit_Movement_Mode`; `0` means "no override", which is why the caller tests `> 0`. Mode 6 dispatches to `BU_Teleport` and mode 7 to `BU_TunnelTo` in `Move_Battle_Unit__WIP` — so `MV_MERGING` and tunnelling are the same thing in this code.

---

# Combat_Grid_Cell_Has_City_Wall

Faithful. Parameters land as `cgc2` at `bp+6` and `cgc1` at `bp+8` (asm:3-4), so the first argument is X and the second Y.

Guard chain, in the listing's order: `walled == ST_TRUE` (asm:16), `cgx` in 5..8 (asm:18-21), `cgy` in 10..13 (asm:22-25), then the interior exclusion (asm:26-34). Production [21755](../../MoM/src/Combat.c#L21755) onward.

The exclusion is **the interior of the wall ring, not a structure footprint**. The city area is the 4x4 block cgx 5..8 x cgy 10..13; walls occupy its 12 perimeter cells, and the 4 interior cells — cgx 6..7 x cgy 11..12 — can never be wall segments:

```
        cgx=5   cgx=6   cgx=7   cgx=8
cgy=10   wall    wall    wall    wall
cgy=11   wall   inner   inner    wall
cgy=12   wall   inner   inner    wall
cgy=13   wall    wall    wall    wall
```

The gate at (8,12) is a perimeter cell; the central structure at (6,11) is an interior cell, which is why it sits inside the walls. Production names the bounds `MIN/MAX_CGX_CITY_INNER` and `MIN/MAX_CGY_CITY_INNER` ([21771](../../MoM/src/Combat.c#L21771)), derived from the city-area constants — the equality-test shape asm:26-34 uses is preserved. `Target_Unit_Value` [19100](../../MoM/src/Combat.c#L19100) tests the same region with the same constants.

The array index at asm:36-46 is `(cgy - 10) * 8 + (cgx - 5) * 2` — a `[4][4]` of 2-byte elements, matching production [21779](../../MoM/src/Combat.c#L21779).

**The interior exclusion is load-bearing, not belt-and-braces.** `walls[4][4]` has 16 entries but only the 12 perimeter cells can legitimately be wall segments. `Generate_Combat_Map` knows this: it sets all 16 to 1 and then explicitly clears the four interior entries ([Combat.c:26777](../../MoM/src/Combat.c#L26777) for cities, [26863](../../MoM/src/Combat.c#L26863) for fortresses). **Wall of Stone does not** — it sets all 16 with no interior clear ([Spells133.c:493](../../MoM/src/Spells133.c#L493)), which the OG comment directly above it already records: *"BUG: would also mark the center sections although there is no wall there (but the spell can't be cast in combat in v1.31 anyway)"* ([Spells133.c:480](../../MoM/src/Spells133.c#L480)). So the exclusion in this function is what keeps those four from being reported as wall segments — unreachable in v1.31 because the spell cannot be cast in combat, but the guard is doing real work, not duplicating a guarantee.

---

# Update_Move_Map_City_Area_Restrictions

The listing has **no stack locals** (no `sub sp`); `si` and `di` carry `j` and `i`.

## Faithful — verified, leave alone

- **The four corner writes**, each gated on its own wall being present (`jnz` skips when absent): `walls[0][0]` -> `0D7h` = 215 (cgy 10 / cgx 5, asm:14-17); `walls[0][3]` -> `0DAh` = 218 (asm:20-23); `walls[3][0]` -> `116h` = 278 (asm:26-29); `walls[3][3]` -> `119h` = 281 (asm:32-35). Production [20922-20937](../../MoM/src/Combat.c#L20922-L20937), index and condition.
- **Central structure blocked unless the battlefield is a city** — asm:38-41, writing `0EDh` = 237. Production [20944](../../MoM/src/Combat.c#L20944).
- **Four separate movement-flag early-outs** — `MV_FLYING` (asm:48), `MV_TELEPORT` (asm:57), `MV_MERGING` (asm:66), `UA_NONCORPOREAL` (asm:75), each its own `test`/`jz` re-deriving the `battle_units` address. Production [20949-20961](../../MoM/src/Combat.c#L20949-L20961) has four independent tests, not a combined mask.
- **Walled/flying-fortress gate** — continue when `walled == ST_TRUE` or `FLYING_FORTRESS != 0` (asm:79-85, `jnz` here rather than the `jle` used in `Assign_Combat_Grids`). Production [20966](../../MoM/src/Combat.c#L20966).
- **Inside/outside split on `Battle_Unit_Is_Within_City`** (asm:87-94). Production [21008](../../MoM/src/Combat.c#L21008).
- **Gate skip** — `cmp i, 3` / `cmp j, 2` / `cmp [FLYING_FORTRESS], 0` (asm:102-107). Production [20980](../../MoM/src/Combat.c#L20980) writes `(MIN_CGX_CITY + i) == CGX_GATE` and `(MIN_CGY_CITY + j) == CGY_GATE`, which constant-folds to exactly `i == 3` and `j == 2`.
- **Inside branch**, `i` outer / `j` inner (asm:96-100), then `walls[j][i] == ST_TRUE || FLYING_FORTRESS > 0` (asm:119-125, `jg`). Production [20974](../../MoM/src/Combat.c#L20974), [20990](../../MoM/src/Combat.c#L20990). The four write blocks appear in the order `i==0`, `j==0`, `i==3`, `j==3` (asm:127, 147, 152, 171) — production [20995](../../MoM/src/Combat.c#L20995), [20998](../../MoM/src/Combat.c#L20998), [21011](../../MoM/src/Combat.c#L21011), [21023](../../MoM/src/Combat.c#L21023). Every index checks out: `(j+10)*W+4` (asm:130-136), `0C1h` = 193 (asm:140), `12Ah` = 298 (asm:145), `i+0C2h` (asm:150), `(j+10)*W+9` (asm:154-160), `0C6h` = 198 (asm:164), `12Fh` = 303 (asm:169), `i+12Bh` (asm:174).
- **Outside branch** — same gate skip and wall test, single write to `(j + MIN_CGY_CITY) * W + i + MIN_CGX_CITY` (asm:219-226). Production [21031](../../MoM/src/Combat.c#L21031) onward, write at [21051](../../MoM/src/Combat.c#L21051).
- **Tail loop** — `i` outer, `j` inner (asm:268, 271), body `(j + MIN_CGY_CITY) * W + (i + MIN_CGX_CITY)` (asm:274-281). Production [21078](../../MoM/src/Combat.c#L21078) / [21082](../../MoM/src/Combat.c#L21082).
- **`walls[j][i]` indexing** — `j << 3` plus `i << 1` (asm:111-118, 205-212), row-major `[4][4]` of 2-byte elements with `j` as the row.
- **Uniform battle-unit access** — the listing re-derives `battle_units + idx * size` at all seven sites (asm:43-48, 52-56, 61-65, 70-74, 237-241, 247-251, 254-258). Production reaches all seven through `bu_ptr` ([20914](../../MoM/src/Combat.c#L20914)), one form throughout.

---

# Unit_Moves2

Two findings, above. The rest is faithful.

## Frame map

`sub sp, 0Ch` (asm) = 12 bytes: one dword plus four words, plus two register locals.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-0Ch` (dword) | `enchantments` | `enchantments` (`uint32_t`) |
| `bp-8` | `hero_items` | `hero_items` |
| `bp-6` | `wind_mastery` | `wind_mastery` |
| `bp-4` | `item_moves2` | `item_moves2` |
| `bp-2` | `endurance` | `endurance` |
| `di` | `_DI_movement_points` | `moves2` |
| `si` | `_SI_itr_hero_items__num_players` | `itr_hero_items` and `itr_players` |

## Faithful — verified, leave alone

- **Init order** — `item_moves2 = 0`, `endurance = ST_FALSE`, then `enchantments = 0` (high word then low). Production [1539-1541](../../MoM/src/NEXTTURN.c#L1539-L1541).
- **Hero gate** — `cmp [s_UNIT.Hero_Slot], -1` / `jg`, i.e. proceed when `> -1`. Production [1545](../../MoM/src/NEXTTURN.c#L1545).
- **`hero_items` pointer** — `_players.Heroes.Items + owner_idx * size s_WIZARD + Hero_Slot * size s_OWNED_HERO`. Production [1547](../../MoM/src/NEXTTURN.c#L1547).
- **Item loop** — `0 .. NUM_HERO_ITEMS`, skipping empty slots via `cmp [word ptr bx], e_ST_UNDEFINED` / `jle`. Production [1548-1550](../../MoM/src/NEXTTURN.c#L1548-L1550). Production spells the sentinel as the literal `-1` where the listing symbolizes `e_ST_UNDEFINED` ([MOX_BASE.h:75](../../MoX/src/MOX_BASE.h#L75)) — same value.
- **Body order inside the loop** — endurance test, then `enchantments |=`, then `item_moves2 +=`. Production [1552-1558](../../MoM/src/NEXTTURN.c#L1552-L1558).
- **The item index is the slot's contents, not the slot number** — the listing loads `hero_items[itr]` and only then scales by `size s_ITEM`. Production [1556-1557](../../MoM/src/NEXTTURN.c#L1556-L1557) keeps the indirection. `s_ITEM.Powers` sits at offset `0x2E` ([MOM_DAT.h:1219](../../MoX/src/MOM_DAT.h#L1219)), which is the field the listing ORs as a dword.
- **`moves2 = moves2_base` before the unit-enchantment OR** — asm order. Production [1564-1565](../../MoM/src/NEXTTURN.c#L1564-L1565). The listing names the field `Move_Halves` (`WIZARDS.inc:672`); production renamed it `moves2_base` ([UNITTYPE.h:283](../../MoM/src/UNITTYPE.h#L283)) — same field, `s_UNIT_TYPE` offset `0x09`.
- **Flight floor 6** — `moves2 < 6` outer, `enchantments & UE_FLIGHT` inner. Production [1566](../../MoM/src/NEXTTURN.c#L1566).
- **Mutation flight floor 4** — mutation test outer, `moves2 < 4` inner (the reverse nesting of the block above, and the listing agrees). Production [1573](../../MoM/src/NEXTTURN.c#L1573).
- **`moves2 += item_moves2` after the endurance bonus.** Production [1588](../../MoM/src/NEXTTURN.c#L1588).
- **Wind Mastery gated on `Transport > 0`.** Production [1592](../../MoM/src/NEXTTURN.c#L1592).
- **Per-player tally** — `Globals[WIND_MASTERY] > 0`, then `+1` for the owner and `-1` for everyone else. Production [1597-1606](../../MoM/src/NEXTTURN.c#L1597-L1606).
- **The two multipliers are separate `if`s, not `if`/`else`** — `cmp [wind_mastery], 0` / `jle`, then `cmp [wind_mastery], 0` / `jge`. The `*3/2` and `/2` both use the signed `cwd` / `sub` / `sar 1` divide-by-two idiom. Production [1609-1616](../../MoM/src/NEXTTURN.c#L1609-L1616).

---

# Battle_Unit_Is_Within_City

One finding (R1, above); everything else matches.

Returns TRUE when the unit stands anywhere in the 4x4 city area — cgx 5..8 x cgy 10..13, the same block `Combat_Grid_Cell_Has_City_Wall` works over, perimeter and interior alike. The listing returns `e_ST_TRUE` on the fall-through and `xor ax, ax` on the bail path; production returns `ST_TRUE` / `ST_FALSE` ([21740](../../MoM/src/Combat.c#L21740) / [21744](../../MoM/src/Combat.c#L21744)). Production's `MIN`/`MAX_CGX_CITY` and `MIN`/`MAX_CGY_CITY` evaluate to the listing's bare 5 / 8 / 10 / 13.

Only one live call sits in this chain — [20972](../../MoM/src/Combat.c#L20972), the inside/outside branch selector in `Update_Move_Map_City_Area_Restrictions`. The call at [20972](../../MoM/src/Combat.c#L20972) is inside the dead `& 0` block. The function has 19 callers in total, most of them in `CMBTAI.c`; a fuller treatment belongs with those.

---

# Battle_Unit_Movement_Mode

Faithful. Tested in the listing's order — `MV_FLYING` -> 2, `MV_SWIMMING` -> 5, `MV_FORESTER` -> 1, `MV_MOUNTAINEER` -> 1, `MV_SAILING` -> 4, default 1. Production [5630-5652](../../MoM/src/Combat.c#L5630-L5652).

The listing reaches the value 1 through a single shared `loc_8023D: mov ax, 1`, with the `MV_FORESTER` and `MV_MOUNTAINEER` branches jumping straight to it rather than each storing 1. That is what tail-merging four identical `mov ax,1` blocks looks like, so production's separate `icon_idx = 1;` assignments are consistent with it. (The listing also carries several unreachable `jmp short loc_8023D` instructions immediately after `jmp short @@JmpDone` — dead bytes, no production counterpart needed.)

**Worth recording:** this function returns only 1, 2, 4 or 5, and `Battle_Unit_Instant_Movement_Mode` returns only 0, 6 or 7. Those two are the sole producers of the `movement_type` that `Set_Movement_Cost_Map` switches on, so the reachable case set is {1, 2, 4, 5, 6, 7}. **Case 3 — `MoveCost_Ground2` — is unreachable**, and case 0 is only reachable as the "no override" sentinel, which shares its block with case 1. Both are faithful to the listing's jump table; the dead arm is original.

Its other two callers ([7057](../../MoM/src/Combat.c#L7057), [9989](../../MoM/src/Combat.c#L9989)) are outside this chain.

---

## OG-faithful behaviors (not bugs — do not "fix")

- **The entire flying-fortress tail of `Update_Move_Map_City_Area_Restrictions` is unreachable.** `test [s_BATTLE_UNIT.Abilities], 0` (asm:242) ANDs against zero, so ZF is always set and the `jz @@Done` at asm:243 always takes; asm:244-289 is dead in the shipped binary. Production reproduces it as `(bu_ptr->Abilities & 0) == 0` with the OGBUG note at [20943](../../MoM/src/Combat.c#L20943).
- **`Central_Structure != CS_City` blocks the lair square in open-field battles too**, since anything that is not a city takes the branch (asm:38-41).
- **`Assign_Combat_Grids` blocks the central-structure square without checking whether anything occupies it** — asm:85-89 tests only `!= CS_None`. Flagged at [3509](../../MoM/src/Combat.c#L3509).
- **Five write-only locals** in `Assign_Combat_Grids` — all have frame slots, all stay.

## Build state

`cmake --build --preset MSVC-debug` (2026-08-03): [Combat.c](../../MoM/src/Combat.c) and [NEXTTURN.c](../../MoM/src/NEXTTURN.c) compile clean, no warnings; `momlib.lib` and `MOM_tests.exe` build; `MOM_tests` runs 157/157 green.

No `stub_wip_todo.md` boxes to check for any of the six functions.

## Related references

- [Combat-Combat_Move_Path_Valid.md](Combat-Combat_Move_Path_Valid.md) — the reachability solver `Assign_Combat_Grids` calls; DONE-DONE.
- [Combat-Combat_Move_Path_Find.md](Combat-Combat_Move_Path_Find.md) — the path solver on the other side of the same cost map.
- [test_Combat_Structure_Index.cpp](../../MoM/tests/test_Combat_Structure_Index.cpp) — pins the central-structure square against the `0EDh` displacement in `ovr091/Assign_Combat_Grids.asm` and `ovr124/BU_SetCityMovement.asm`.
- [CMBTDEF.h](../../MoM/src/CMBTDEF.h) — city-area, gate, lair, city-interior and action-map cell constants.
- [Combat.c](../../MoM/src/Combat.c) — `Move_Battle_Unit__WIP` repeats the same cost-map seeding sequence; `Battle_Unit_Action` consumes the action map. Neither is reviewed yet.
- [MOX_DEF.h:399](../../MoX/src/MOX_DEF.h#L399) — `ITEM_POWER` still reads item powers through `GET_4B_OFS(..., 0x2E)`; the commented-out form directly above it uses `.Powers`, which now exists.
- C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr121\Unit_Moves2.asm
- C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\BU_SetCityMovement.asm
