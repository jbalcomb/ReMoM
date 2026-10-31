# Amiga 68K port reports — triage

Source: `doc/@TODO/ReMoM-fixes-for-Claude.txt`, from the Amiga 68K port of ReMoM ([angree/Amiga-68K-ReMoM](https://github.com/angree/Amiga-68K-ReMoM)), reported by player Solo Kazuki against `jbalcomb/ReMoM` `a9cc082`. Their fixes live in their `build/remom-patch.py`.

Their ReMoM-vs-OG labels are, in their words, "our judgement, not checked against the disassembly". This document checks each item against the IDA 5.5 disassembly (`STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovrNNN/*.asm`) and classifies it:

- **ReMoM** — reconstruction error: the C does not match the asm. Fix it.
- **OGBUG** — the asm does it too. Do not fix in baseline ReMoM; mark `OGBUG` in the source.
- **Not a bug** — the report is wrong.
- **Architecture** — caused by ReMoM's design (static pool, single process, platform layer); there is no asm counterpart to be faithful to, so it needs a design decision.

**Also found** lists deviations noticed in the same code while checking an item. They were not reported.

Line numbers below are the item's line in the TODO file (`L10` = line 10). Code links are to the current tree.

## Summary

| ToDo | Item | Function | Verdict | Status |
|---|---|---|---|---|
[x] | L10 | `Update_MovePathMap`, `Eval_Move_Path__WIP` — hero test — a, b, c, d | ReMoM | fixed |
[x] | L11 | `Eval_Move_Path__WIP` — second loop reads `Units[]` | ReMoM | fixed |
[x] | L12 | `Update_MovePathMap`, `Eval_Move_Path__WIP` — `- 1` transport seat — a, b, c | OGBUG | `OGBUG` notes in place |
[x] | L13 | `Square_Has_City` — skips city 0, reads past the end | ReMoM | fixed |
[x] | L14 | `Build_Moveable_Stack` — a, b (c not needed) | ReMoM | fixed |
[x] | L15 | `Build_RoadBuilder_Stack` — a, b, c | ReMoM (a, b); OGBUG (b's extra seat, c) | fixed; `OGBUG` notes in place |
[x] | L16 | `Move_Stack` — `Stack_Moves` vs `Stack_Moves_Active` | Not a bug (faithful) | no code change; XREF note fixed (AF5) |
[x] | L17 | leaving a Tower keeps the old plane | ReMoM — in `Units_In_Tower`, not `Move_Units_Draw` | fixed |
[x] | L18 | `All_Units_In_Towers` — `else in_tower = FALSE` | ReMoM | fixed |
[x] | L19 | `Set_Army_Road_Building` — `Turns_To_Build_Road(wx, wx, ...)` | ReMoM | fixed |
[x] | L20 | "It will take N turns" counts every engineer | OGBUG | `OGBUG` note in place |
[x] | L23 | `Combat` — defender player never set for city/lair | ReMoM | fixed |
[x] | L24 | centroid divide by zero — a, b, c | OGBUG | `/* HACK */` guards in place |
[x] | L25 | `Select_Unit_For_Enchantment` — `[-1]` read, no owner test | OGBUG | `OGBUG` note in place (the `[-1]` read) |
[x] | L26 | `Cast_Spell_Overland` — unit enchantment applied with no target | ReMoM | fixed |
[x] | L27 | `Cast_Spell_Overland` — Nature's Wrath test runs on a failed cast | ReMoM | fixed |
[x] | L28 | `Generate_Home_Cities` — capital on a node/tower/lair at distance 1 — a, b, c | OGBUG | `OGBUG` notes in place |
[x] | L31 | `WIZ_HireHero` — clears the human's hero items | ReMoM | fixed |
[x] | L32 | `Cast_Spell_Overland_Do` crafting — a, c, d (b is AF9) | ReMoM (a, c); OGBUG (d) | a, c fixed; d `OGBUG` note in place (`[136]` restored) |
[x] | L33 | `Item_Make_Screen` — left-arrow wrap index | ReMoM | fixed |
[x] | L34 | `Spell_Casting_Screen__WIP` — `screen_changed` never cleared | ReMoM | fixed |
[x] | L35 | `Learn_Spell_Animation` — row written to the wrong variable | ReMoM | fixed |
[x] | L36 | `Cast_Spell_Overland` — summon map focus `_UNITS[_units]` | ReMoM | **fixed in v0.0.9 (#21)** |
[x] | L37 | `Hero_LevelUp_Popup_Draw` — missing `* 17` | ReMoM | fixed |
[x] | L40 | `s_UV_List` allocated as 1520 bytes — a, b, c | ReMoM (porting) | fixed |
[x] | L41 | `City_Sell_Building` — city and building swapped | ReMoM — the prototype, like #19 | fixed |
[x] | L42 | City screen farmer/worker clicks | Not a bug (faithful) | no code change |
[x] | L43 | `Outpost_Cityscape_Draw` — `[type * 10]` | ReMoM | fixed |
[x] | L44 | `Building_Allows_List` — `&&`; unit half unimplemented — a, b | ReMoM | fixed |
[x] | L45 | Load screen stores file number, compares row | ReMoM | fixed |
[x] | L46 | Continue skips the WZD startup palette steps | Architecture | fixed; palette confirmed in game |
[x] | L47 | `Create_Reduced_Map_Picture` — city explored test | ReMoM | fixed |
[x] | L48 | `Create_Blank_Picture` — writes pixels 1..length | ReMoM | fixed |
[x] | L49 | `Add_Unit_Action_Fields` — special button with an empty stack | ReMoM | fixed |
[x] | L50 | Cartographer — y from `m_cartograph_x`; terrain unfinished — a, b | ReMoM | fixed; confirmed in game |
[x] | L51 | History — fixed `months[8]`; label `"history"` | ReMoM | fixed |
[x] | L52 | Banner screen — extra name print | ReMoM | fixed |
[x] | L53 | Surveyor — IDA text inside a string literal | ReMoM | fixed |
[x] | L54 | `Notify2` box past x = 319 with the Chaos mascot | OGBUG | `OGBUG` note in place |
[x] | L57 | `Load_WZD_Resources` on every Load/Save visit | Architecture | fixed (load once) |
[x] | L58 | `Open_File_Animation__HACK` leaks a pool arena per call | Architecture | fixed (streamed through VGAFILEH, as the original); zap checked in game |
[x] | L61 | New Game in the same process keeps the old game's data | Architecture | fixed (per-game reset); `HeMoM_L61_*` passes on MSVC and clang |

### Also found

Not in the reporter's list. Each was noticed while checking the item named in the last column.

| ToDo | Item | Where | Verdict | Status | Found under |
|---|---|---|---|---|---|
[x] | AF1 | `Update_MovePathMap` — `_landmasses` read without the plane offset | [MainScr.c:6112](../../MoM/src/MainScr.c#L6112), [6168](../../MoM/src/MainScr.c#L6168) | ReMoM | fixed | L10 |
[x] | AF2 | `Update_MovePathMap` — `Player_Army_At_Square` on the destination instead of the unit's square | [MainScr.c:6123](../../MoM/src/MainScr.c#L6123) | ReMoM | fixed | L10 |
[x] | AF3 | `Update_MovePathMap` — capacity test inside the loop instead of after it | [MainScr.c:6140](../../MoM/src/MainScr.c#L6140), [6194](../../MoM/src/MainScr.c#L6194) | ReMoM | fixed | L10 |
[x] | AF4 | `Build_RoadBuilder_Stack` — capacity test `<` instead of `<=` | [UNITSTK.c:1309](../../MoM/src/UNITSTK.c#L1309) | ReMoM | fixed (L15b) | L15 |
[x] | AF5 | XREF note lists `Move_Stack` as a caller of `j_Stack_Moves_Active` | [MainScr.c:4358](../../MoM/src/MainScr.c#L4358) | wrong note | fixed | L16 |
[x] | AF6 | `Cast_Spell_Overland_Do` AI branch reads `var_8`, never written | [SBookScr.c:450](../../MoM/src/SBookScr.c#L450) | OGBUG | `OGBUG` notes in place (renamed `cost_reduction`) | L32 |
[x] | AF7 | `Notify2` calls `Notify1_Draw` | [GENDRAW.c:1627](../../MoX/src/GENDRAW.c#L1627) | OGBUG | `OGBUG` note in place | L54 |
[x] | AF8 | `_ai_landmass_war_targets` not reset by New Game | [MOM_DAT.c:906](../../MoX/src/MOM_DAT.c#L906) | Architecture | fixed (L61 reset) | L24 |
[x] | AF9 | `Cast_Spell_Overland_Do` AI crafting cost: `remaining * cost_reduction / 100` instead of `remaining - (remaining * cost_reduction / 100)` — every AI Enchant Item / Create Artifact ends at cost 0 and is cancelled | [SBookScr.c:485](../../MoM/src/SBookScr.c#L485), [503](../../MoM/src/SBookScr.c#L503) | ReMoM | fixed | L32 |
[x] | AF10 | `Conquest_Animation` early-return guard tests `loser != NEUTRAL` instead of `loser == NEUTRAL` — no wizard-on-wizard conquest animation ever plays | [CONQUEST.c:370-378](../../MoM/src/CONQUEST.c#L370-L378) | ReMoM | fixed | L58 |
[x] | AF11 | `_given_chance_to_rename_home_city` not reset by New Game — the second game in a process never offers the home-city rename | [MainScr.c:88](../../MoM/src/MainScr.c#L88) | Architecture | fixed (L61 reset) | L61 |

---

## Movement, stacks, ships

### L10 — boarding a ship: hero test inverted (a, b, c, d) — **ReMoM**

The passenger count is decremented for units that do not need a seat. ReMoM decremented for `type > ut_Chosen` (every normal unit), so only heroes took a seat. In the asm the `dec` is reached when the type is **at or below** `ut_Chosen`, i.e. for heroes:

| | Function | Loop | ReMoM | Asm |
|---|---|---|---|---|
| a | `Update_MovePathMap` | first block (stack on the source square) | [MainScr.c:6132](../../MoM/src/MainScr.c#L6132) | `ovr095/Update_MovePathMap.asm`: `cmp [es:bx+s_UNIT.type], ut_Chosen` / `jbe short loc_7CE1C` |
| b | `Update_MovePathMap` | second block | [MainScr.c:6185](../../MoM/src/MainScr.c#L6185) | `ovr095/Update_MovePathMap.asm`: `cmp [es:bx+s_UNIT.type], ut_Chosen` / `jbe short loc_7CFD1` |
| c | `Eval_Move_Path__WIP` | units already on the destination | [MainScr.c:6555](../../MoM/src/MainScr.c#L6555) | `ovr097/Eval_Move_Path__WIP.asm`: `cmp [es:bx+s_UNIT.type], ut_Chosen` / `jbe short loc_7E3D6` |
| d | `Eval_Move_Path__WIP` | units moving | [MainScr.c:6570](../../MoM/src/MainScr.c#L6570) | `ovr097/Eval_Move_Path__WIP.asm`: `cmp [es:bx+s_UNIT.type], ut_Chosen` / `jbe short loc_7E477` |

All four now read `type <= ut_Chosen`.

**Also found** in `Update_MovePathMap`:

- [MainScr.c:6116](../../MoM/src/MainScr.c#L6116) and [6171](../../MoM/src/MainScr.c#L6171) index `_landmasses` without the plane: `_landmasses[(dst_wy * WORLD_WIDTH) + dst_wx]`. The asm adds `wp * e_WORLD_SIZE` both times (`mov ax, [bp+wp]` / `mov dx, e_WORLD_SIZE` / `imul dx`). On Myrror the ocean test reads Arcanus. These are the only two `_landmasses` reads in the tree without the plane offset. **ReMoM.**
- [MainScr.c:6127](../../MoM/src/MainScr.c#L6127) calls `Player_Army_At_Square(dst_wx, dst_wy, ...)` for the units being checked; the asm pushes `[bp+unit_wy]`, `[bp+unit_wx]` (`loc_7CD62`). **ReMoM.**
- In both blocks the capacity test (`(count - 1) <= capacity && count <= 9` -> cost 2) is inside the `for` loop in ReMoM; in the asm it runs once, after the loop (`loc_7CE2D`, `loc_7CFE2`). Same result whenever the loop runs, but with zero units the asm still sets cost 2 and ReMoM does not. **ReMoM** (structural).

### L11 — `Eval_Move_Path__WIP` second loop reads `Units[]` — **ReMoM**

[MainScr.c:6567-6580](../../MoM/src/MainScr.c#L6567-L6580)

The loop that counts the *moving* units' seats iterates `troop_count` but indexes `Units[]` (the units already on the destination). The asm indexes the `troops` parameter:

```
ovr097/Eval_Move_Path__WIP.asm  loc_7E3ED:
                                mov     ax, _SI_itr_troops
                                shl     ax, 1
                                mov     bx, [bp+troops]
```

Fix: `troops[itr_troops]` throughout that loop (six reads).

### L12 — the `- 1` lets two ships carry one unit fewer (a, b, c) — **OGBUG**

The asm subtracts one before comparing to the transport capacity:

| | Function | Test | ReMoM | Asm |
|---|---|---|---|---|
| a | `Update_MovePathMap` | first block | [MainScr.c:6144](../../MoM/src/MainScr.c#L6144) | `ovr095/Update_MovePathMap.asm`: `mov ax, [bp+checked_troops_boatriders_count]` / `dec ax` / `cmp ax, [bp+dst_troops_carry_capacity]` (`loc_7CE2D`) |
| b | `Update_MovePathMap` | second block | [MainScr.c:6198](../../MoM/src/MainScr.c#L6198) | same sequence at `loc_7CFE2` |
| c | `Eval_Move_Path__WIP` | destination + moving units | [MainScr.c:6585](../../MoM/src/MainScr.c#L6585) | `ovr097/Eval_Move_Path__WIP.asm`: `mov ax, [bp+dst_boatriders_count]` / `add ax, [bp+src_boatriders_count]` / `dec ax` / `cmp ax, [bp+Transport_Capacity]` |

ReMoM matches the asm at all three, and the reporter cites the MoM wiki "Transport" page for this behaviour, so the code stays. Each site has an `OGBUG` note on the line above ([6142](../../MoM/src/MainScr.c#L6142), [6196](../../MoM/src/MainScr.c#L6196), [6584](../../MoM/src/MainScr.c#L6584)).

### L13 — `Square_Has_City` skips city 0 and reads one past the end — **ReMoM**

[MainScr.c:6662](../../MoM/src/MainScr.c#L6662)

`while((itr_cities++ < _cities) && ...)` increments before the body, so the body sees cities `1.._cities`: city 0 (usually the human's capital) is never found, and `_CITIES[_cities]` is read. The asm is a plain `for` from 0, incrementing at the bottom:

```
ovr097/Square_Has_City.asm   xor     _CX_itr, _CX_itr
                             ...
                             @@NextCity:
                             inc     _CX_itr
                             loc_7E666:
                             cmp     _CX_itr, [_cities]
                             jge     short @@Done
```

Now `for(itr_cities = 0; (itr_cities < _cities) && (square_has_city == ST_UNDEFINED); itr_cities++)`, the same shape as `Square_Has_City_Dupe` ([MainScr.c:4508](../../MoM/src/MainScr.c#L4508)).

### L14 — `Build_Moveable_Stack` — **ReMoM** (a, b; c is the reporter's guard, not needed)

[UNITSTK.c:1002](../../MoM/src/UNITSTK.c#L1002)

| | Problem | ReMoM | Asm |
|---|---|---|---|
| a | Self-crossing (third) pass inverted | [UNITSTK.c:1097-1107](../../MoM/src/UNITSTK.c#L1097-L1107) | `ovr061/Build_Moveable_Stack.asm`, `loc_53F9C`: each test `cmp ax, e_ST_TRUE` / `jz @@Check_LandLubber_GoTo`; the last `jnz @@NextStackUnit3` |
| b | No-passenger branch tests `_UNITS[itr_stack]` | [UNITSTK.c:1127](../../MoM/src/UNITSTK.c#L1127) | `loc_54052`: `mov ax, [word ptr _unit_stack.unit_idx+bx]` before both the `Status` and the `Transport` test |
| c | Reporter's `MAX_STACK` limits | — | none |

- **a:** the asm adds units that cross water on their own (any of the five travel checks true), skipping `us_GOTO`. ReMoM required all five false, which added the passengers a second time; the list overran `unit_array[MAX_STACK]` on `Move_Stack`'s stack (the crash after Next Turn). Now `== ST_TRUE` joined by `||`.
- **b:** now `_UNITS[_unit_stack[itr_stack].unit_idx]` for both tests.
- **c:** not in the asm and not needed. With a and b fixed every path adds each stack unit at most once: ships (`MV_SAILING`, so never boatriders), boatriders (fail all five pass-3 tests; swimming is inside `Unit_Has_WaterTravel`, and wind walking makes `Army_Boatriders` return 0), and self-crossers are disjoint; the no-passenger path is `Transport > 0` plus `Transport < 1`. The count is at most `_unit_stack_count` ≤ 9.

### L15 — `Build_RoadBuilder_Stack` passenger loop (a, b, c)

[UNITSTK.c:1309](../../MoM/src/UNITSTK.c#L1309)

```
ovr061/Build_RoadBuilder_Stack.asm   cmp     [bx], _DI_unit_idx
                                     jnz     short loc_5441A          ; skip when not the next boatrider
                                     mov     ax, [bp+passenger_count]
                                     cmp     ax, [bp+transport_capacity]
                                     jg      short loc_5441A          ; skip when passenger_count > capacity
                                     ...add unit...
                                     cmp     [es:bx+s_UNIT.type], ut_Chosen
                                     jbe     short loc_5441A          ; hero -> no seat
                                     inc     [bp+passenger_count]
```

| | Problem | Verdict | Status |
|---|---|---|---|
| a | `boatriders[boatrider_idx] != unit_idx` — added everything *but* the passengers: the ship twice (taking a seat), crew left behind | ReMoM | fixed: `==` |
| b | capacity test `<`; the asm's `jg`-to-skip is `<=` (AF4). One more passenger than the capacity boards on Go To; `Build_Moveable_Stack` uses `<` (`jl`) | ReMoM; the extra seat itself is OGBUG | fixed: `<=`; `OGBUG` note at [UNITSTK.c:1308](../../MoM/src/UNITSTK.c#L1308) |
| c | capacity is tested before the hero test, so a hero is turned away once the boats are full (the reporter exempted heroes) | OGBUG | `OGBUG` note at [UNITSTK.c:1307](../../MoM/src/UNITSTK.c#L1307) |

### L16 — `Move_Stack` uses `Stack_Moves`, not `Stack_Moves_Active` — **Not a bug (faithful)**

[UNITSTK.c:260](../../MoM/src/UNITSTK.c#L260), [265](../../MoM/src/UNITSTK.c#L265)

Every call in the full listing (`in/WIZARDS.asm`):

| Caller | Calls |
|---|---|
| `Move_Stack` | `j_Stack_Moves` (twice) |
| `Main_Screen`, `zMove_Stack_DirKey`, `Update_Stack_Active` | `j_Stack_Moves_Active` |

ReMoM's `Stack_Moves` / `Stack_Moves_Active` bodies match the asm functions of the same names (only `Stack_Moves_Active` tests `_unit_stack.active`). If fast units stop with the slow ones, the original does it too.

The reporter was misled by ReMoM's own note at [MainScr.c:4355-4358](../../MoM/src/MainScr.c#L4355-L4358), which lists `Move_Stack` as a caller of `j_Stack_Moves_Active`. That note was wrong — the third caller is `Update_Stack_Active` (`in/WIZARDS.asm` line 110610, inside `proc Update_Stack_Active`). The note now says so.

### L17 — leaving a Tower while viewing Myrror lands the stack on Arcanus — **ReMoM, in `Units_In_Tower`**

[WZD_o059.c:424](../../MoM/src/WZD_o059.c#L424)

The reporter's fix (`wp = map_p` in `Move_Units_Draw`) has no asm counterpart: neither the C nor the asm `Move_Units_Draw` writes `s_UNIT.wp`. The original does it in `Units_In_Tower`: when the stack is no longer on a tower square, each unit that was `in_tower` gets the plane parameter, then `in_tower` is cleared. ReMoM clears `in_tower` but drops the plane write:

```
ovr059/Units_In_Tower.asm   cmp     [es:bx+s_UNIT.in_tower], e_ST_TRUE
                            jnz     short loc_522F8
                            ...
                            mov     al, [byte ptr bp+wp]
                            mov     [es:bx+s_UNIT.wp], al
                            ...
                            mov     [es:bx+s_UNIT.in_tower], e_ST_FALSE
```

Now `_UNITS[unit_idx].wp = (int8_t)map_p;` before the `in_tower` clear in the `else` branch ([WZD_o059.c:480](../../MoM/src/WZD_o059.c#L480)).

### L18 — `All_Units_In_Towers` only keeps the last tower's result — **ReMoM**

[WZD_o059.c:500-521](../../MoM/src/WZD_o059.c#L500-L521)

ReMoM's `else` set `in_tower = ST_FALSE` for every tower that did not match, so a match on an earlier tower was overwritten. In the asm, a non-matching tower jumps straight to `@@NextTower`; `in_tower` is cleared once per unit before the tower loop (`loc_52319`), and a match sets it to `FALSE` (Planar Seal) or `TRUE`. Now the same: the `else` is gone and the seal test is nested inside the `wx`/`wy` match.

### L19 — `Set_Army_Road_Building` passes `wx` twice — **ReMoM**

[Roads.c:687](../../MoM/src/Roads.c#L687)

```
ovr069/Set_Army_Road_Building.asm   push    [_map_plane]
                                    mov     al, [es:bx+s_UNIT.wy]
                                    push    ax
                                    mov     al, [es:bx]               ; offset 0 = s_UNIT.wx
                                    push    ax
                                    call    j_Turns_To_Build_Road
```

Now `Turns_To_Build_Road(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _map_plane)` ([Roads.c:687](../../MoM/src/Roads.c#L687)).

### L20 — unselected engineers count toward road building — **OGBUG**

[Roads.c:148-161](../../MoM/src/Roads.c#L148-L161)

`Road_Build_Screen` sums `Construction` over the whole `_unit_stack` with no `active` test (`ovr069/Road_Build_Screen.asm`, `loc_5B932`). On OK, only the selected builders get the order (`Active_Stack_Roadbuilders`, [Roads.c:206](../../MoM/src/Roads.c#L206)), but `Set_Army_Road_Building` divides each one's `Rd_Constr_Left` by that same whole-stack `m_construction_points` — the asm only reads it (`cmp [m_construction_points], 0` / `idiv [m_construction_points]`), never re-sums it. The estimate matches what happens, but engineers left behind speed up the road. ReMoM is faithful.

Not the reporter's fix (switching the sum to `Active_Stack_Roadbuilders` changes OG behaviour). `OGBUG` note at [Roads.c:147](../../MoM/src/Roads.c#L147).

## Combat, lairs, AI

### L23 — `Combat` never sets the defender player for a city or a lair — **ReMoM**

[Combat.c:3094](../../MoM/src/Combat.c#L3094), [3143-3180](../../MoM/src/Combat.c#L3143-L3180)

ReMoM's `combat_defender_player_idx` starts at 0 and is set only in `case 0` (stack). For `case 1` (city) and `case 5` (lair) it stays 0 — the human — so the treaty check, the garrison count and everything after them treat the human as the defender.

In the asm the defender player is held in `DI`, loaded from the `defender_player_idx` parameter at entry and overwritten only in `case 0`:

```
ovr096/Combat__WIP.asm   loc_7D599:                                ; case 0
                         ...
                         mov     al, [es:bx+s_UNIT.owner_idx]
                         cbw
                         mov     _DI_defender_idx, ax
                         ...
                         loc_7D6FA:
                         cmp     _DI_defender_idx, e_NEUTRAL_PLAYER_IDX
```

So for a city or lair the original sees the parameter. Callers pass the city owner (`Move_Units` -> `attackee_idx`, see [MainScr.c:5351](../../MoM/src/MainScr.c#L5351)) or `e_NEUTRAL_PLAYER_IDX` (`Lair_Combat_Do`). This is about the value the reads observe, not about how many locals the source declared.

Now `combat_defender_player_idx = defender_player_idx;` at [Combat.c:3101](../../MoM/src/Combat.c#L3101), next to `defender_idx = defender_player_idx;` — together they hold what `DI` holds after `mov _DI_defender_idx, [bp+defender_player_idx]`. `defender_idx` is used only while `DI` is a unit index (the neutral test and `case 0` `wx`/`wy`/`wp`); `combat_defender_player_idx` only once it is a player index.

### L24 — centroid divide by zero (a, b, c) — **OGBUG**

The dock-square centroid is divided by the dock-square count with no zero test. All three asm functions go straight from the loop exit to `idiv`:

| | Function | ReMoM | Asm |
|---|---|---|---|
| a | `AI_Stacks_Roamers_Target_Or_Deploy` | [AIMOVE.c:1696](../../MoM/src/AIMOVE.c#L1696) | `ovr158/AI_Stacks_Roamers_Target_Or_Deploy.asm`: `cmp [bp+Next_Tile_ChainIndex], e_ST_UNDEFINED` ... `idiv [bp+Tile_Count__current_dock_square_distance]` |
| b | `AI_Reevaluate_Continent` | [AIMOVE.c:5584](../../MoM/src/AIMOVE.c#L5584) | `ovr158/AI_Reevaluate_Continent.asm`: `cmp [bp+landmass_node_index], e_ST_UNDEFINED` ... `idiv [bp+landmass_node_wx__landmass_node_count]` |
| c | `AI_Evaluate_Continents` | [AIMOVE.c:6820](../../MoM/src/AIMOVE.c#L6820) | `ovr162/AI_Evaluate_Continents.asm`: `cmp [bp+landmass_node_index], e_ST_UNDEFINED` ... `idiv [bp+world_offset]` |

In the original a zero count is a "Divide error" crash. All three carry a `/* HACK */` guard (count 0 -> centroid 0,0) with the original divisions commented out above it, and keep their `OGBUG` notes.

The reporter's other point — the war target that led to a zero count came from the previous game — is a separate bug: **AF8** below.

### L25 — `Select_Unit_For_Enchantment` reads `_ai_all_own_stacks[-1]` and ignores the owner — **OGBUG**

[AISPELL.c:4517](../../MoM/src/AISPELL.c#L4517)

`ovr156/Select_Unit_For_Enchantment.asm` has no `owner_idx` test anywhere, and the only `ST_UNDEFINED` test on `best_stack_idx` is the fortress fallback (asm line 121), not a guard before Phase 4 reads `_ai_all_own_stacks[best_stack_idx]`. ReMoM matches and already notes the `[-1]` read as `OGBUG` ([AISPELL.c:4580](../../MoM/src/AISPELL.c#L4580)).

The missing owner test is not a separate bug: Phase 4 matches units on the square of one of the AI's own stacks, and units of different players never share a square, so an owner test could not fail. The AI only reaches the human's units through the `[-1]` read, which is already noted. Nothing to add.

### L26 — unit enchantment applied when no target was chosen — **ReMoM**

[OverSpel.c:1127-1146](../../MoM/src/OverSpel.c#L1127-L1146)

After the target is chosen (AI: `AITP_Unit_Enchantment`; human: `Spell_Casting_Screen__WIP`), ReMoM always runs the animation and `_UNITS[spell_target_idx].enchantments |= ...`. The asm checks first:

```
ovr135/Cast_Spell_Overland.asm   @@After_CastUnitEnch:
                                 cmp     [bp+Cast_Successful], e_ST_TRUE
                                 jz      short loc_BB92B
                                 jmp     loc_BBA16
```

So with no AI target — or a human cancel — the original casts nothing; ReMoM enchants whatever `spell_target_idx` holds (0 when never set). Now wrapped: everything from `@@After_CastUnitEnch` through the `Black_Channels` mutation is inside `if(cast_can_continue == ST_TRUE)`.

### L27 — Nature's Wrath test runs after a failed cast — **ReMoM**

[OverSpel.c:1898-1940](../../MoM/src/OverSpel.c#L1898-L1940)

ReMoM's `if(cast_can_continue == ST_TRUE)` covers only the owner loop; the Nature's Wrath test after it runs on every cast, reading whatever `MultiPurpose_Local_Var` last held. In the asm one branch skips both:

```
ovr135/Cast_Spell_Overland.asm   cmp     [bp+Cast_Successful], e_ST_TRUE
                                 jnz     short loc_BCB36           ; past the Call_Forth_The_Force_Of_Nature call
                                 mov     [bp+MultiPurpose_Local_Var], e_ST_UNDEFINED
                                 ...
                                 call    j_Call_Forth_The_Force_Of_Nature
                                 loc_BCB36:
```

Now the Nature's Wrath block is inside the `if`, with the owner loop.

### L28 — capital placed on a node/tower/lair square (a, b, c) — **OGBUG**

[MAPGEN.c:1084-1086](../../MoM/src/MAPGEN.c#L1084-L1086): after six failed rounds `minimum_site_distance` bottoms out at 1. Each of the three site tests then compares against `minimum_site_distance / 2` = 0, which no distance is below, so the same square is allowed.

| | Test | ReMoM | `OGBUG` note | Asm (`MAGIC/ovr051/Generate_Home_Cities.asm`) |
|---|---|---|---|---|
| a | nodes, `Delta_XY_With_Wrap` | [MAPGEN.c:1149](../../MoM/src/MAPGEN.c#L1149) | [1148](../../MoM/src/MAPGEN.c#L1148) | `sar ax, 1` line 161 |
| b | towers, `Range` | [MAPGEN.c:1161](../../MoM/src/MAPGEN.c#L1161) | [1160](../../MoM/src/MAPGEN.c#L1160) | `sar ax, 1` line 198 |
| c | lairs, `Delta_XY_With_Wrap` | [MAPGEN.c:1174](../../MoM/src/MAPGEN.c#L1174) | [1173](../../MoM/src/MAPGEN.c#L1173) | `sar ax, 1` line 246 |

ReMoM matches at all three, each with a definite `OGBUG` note directly above. (`Generate_Home_City__GEMINI` carries the same notes; it is not production.)

## Spells, items, heroes

### L31 — `WIZ_HireHero` clears the human's hero items — **ReMoM**

[Spells132.c:513](../../MoM/src/Spells132.c#L513)

`_players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].Items[itr] = ST_UNDEFINED;` — the asm indexes `_players` with `_SI_player_idx` (`ovr132/WIZ_HireHero.asm`, `loc_B4BF6`). When an AI hires into slot N, the human's hero in slot N lost its items. Now `_players[player_idx]`.

### L32 — `Cast_Spell_Overland_Do` crafting branch (a, b, c, d)

[SBookScr.c:468-513](../../MoM/src/SBookScr.c#L468-L513), asm `ovr134/Cast_Spell_Overland_Do.asm` (`loc_B9455` Enchant Item, `loc_B9563` Create Artifact).

History (`git blame`): `f90e1106` (2024-10-07) stubbed the human `Item_Make_Screen` call as a TODO and pasted the AI lines below it; `042c748b4` (2025-06-26) un-commented the real human lines but left the pasted AI lines, and changed `_ITEMS[136].cost` to `[137]`.

| | Problem | Verdict | Status |
|---|---|---|---|
| a | human branches also ran `Make_Item` … `Remove_Item` (the asm `jmp`s past the AI block after `Allocate_Reduced_Map`) | ReMoM | fixed |
| b | AI cost formula — moved to **AF9** (not part of the report) | — | see AF9 |
| c | `else if(spell_idx == spl_Create_Artifact)`; the asm is a separate `if` (`loc_B955A: cmp _DI_spell_idx, spl_Create_Artifact`) | ReMoM (structure only) | fixed ([SBookScr.c:490](../../MoM/src/SBookScr.c#L490)) |
| d | human `casting_cost_original = _ITEMS[MAX_ITEM_COUNT].cost` (137); the asm reads `[es:bx+1AB2h]` right after `les bx, [_ITEMS]` — `s_ITEM` is 0x32 bytes with `cost` at 0x22 (`MAGIC.inc`), so 0x1AB2 = 136 * 50 + 34 = `_ITEMS[136].cost`. Nothing copies 137 to 136: `Item_Make_Screen` builds the design with `Create_Item_Record(137)` and returns the reduced cost; `Create_Item_Record` writes only its `item_idx`; slot 136 is `Add_Item`'s prefab staging slot (`_fmemcpy` to `_ITEMS + 6800`), and `Activate_Item` only fills slots 0-131. So the original sets the human's `casting_cost_original` from whatever prefab was last staged — not the item being made | OGBUG — ReMoM's `[137]` silently fixed it | `_ITEMS[136]` restored with an `OGBUG` note ([SBookScr.c:477](../../MoM/src/SBookScr.c#L477), [495](../../MoM/src/SBookScr.c#L495)) |

`var_8` is now `cost_reduction`: the AI-side twin of the `Casting_Cost_Reduction` percentage that `Item_Make_Screen` applies for the human ([ItemMake.c:1544-1546](../../MoM/src/ItemMake.c#L1544-L1546)). It is read but never written in the asm — **OGBUG** (AF6), noted in both AI branches.

### L33 — `Item_Make_Screen` left arrow wraps to the wrong icon — **ReMoM**

[ItemMake.c:1492-1497](../../MoM/src/ItemMake.c#L1492-L1497)

```
ovr115/Item_Make_Screen.asm   dec     [m_itemmake_item_icon_idx]
                              ...
                              lea     ax, [bp+Item_Type_Icon_Ranges+2]   ; .count
                              add     bx, ax
                              mov     ax, [m_itemmake_item_icon_idx]
                              add     ax, [bx]                           ; idx + count
                              mov     [m_itemmake_item_icon_idx], ax
```

The index has just dropped to `start - 1`, so `idx + count` is the last icon of the type. Now `m_itemmake_item_icon_idx += Item_Type_Icon_Ranges[weapon_type_num].count;`.

### L34 — `Spell_Casting_Screen__WIP` never clears `screen_changed` — **ReMoM**

[SCastScr.c:729-740](../../MoM/src/SCastScr.c#L729-L740)

```
ovr070/Spell_Casting_Screen__WIP.asm   loc_5CD0F:
                                       mov     [bp+screen_changed], e_ST_FALSE
```

The asm clears it at the end of every loop pass; ReMoM never did, so after one minimap/plane click the screen stopped redrawing. Now `screen_changed = ST_FALSE;` after the draw block, inside the loop ([SCastScr.c:740](../../MoM/src/SCastScr.c#L740)).

### L35 — learn-spell animation reads the wrong row — **ReMoM**

[Spellbook.c:2187](../../MoM/src/Spellbook.c#L2187), [2216](../../MoM/src/Spellbook.c#L2216), [2361](../../MoM/src/Spellbook.c#L2361)

The original uses one global for the row: `Learn_Spell_Animation` writes `[GUI_Multipurpose_Int]` (`ovr118/Learn_Spell_Animation.asm` lines 211, 301) and the decode draw reads it. ReMoM writes `m_spell_list_count` — a variable it introduced ("DNE in Dasm; uses GUI_Multipurpose_Int", [Spellbook.c:360](../../MoM/src/Spellbook.c#L360)) — while `Learn_Spell_Animation_Decode_Draw` reads `GUI_Multipurpose_Int`. Now both writes (2187, 2216) go to `GUI_Multipurpose_Int`, with a `TODO` to move the pair to `m_spell_list_count` together.

### L36 — summon map focus — **fixed in v0.0.9**

GitHub #21. Found independently by the reporter.

### L37 — `Hero_LevelUp_Popup_Draw` window too short — **ReMoM**

[LVLMAKE.c:360](../../MoM/src/LVLMAKE.c#L360)

```
ovr102/Hero_LevelUp_Popup_Draw.asm   sar     ax, 1
                                     mov     dx, 17
                                     imul    dx
                                     ...
                                     add     dx, 49
```

`y = lvlmake_base_y + 49 + ((count + 1) / 2) * 17`. ReMoM dropped the `* 17` (present at [LVLMAKE.c:281](../../MoM/src/LVLMAKE.c#L281)); now restored at 360.

## City, screens, UI

### L40 — `s_UV_List` is 1760 bytes but allocated as 1520 (a, b, c) — **ReMoM (porting)**

| | Site | Status |
|---|---|---|
| a | [LVLMAKE.c:228](../../MoM/src/LVLMAKE.c#L228) | fixed (`sizeof`, original kept under `/*HACK */`) |
| b | [ProdScr.c:411](../../MoM/src/ProdScr.c#L411) | fixed — the reported one (`product_fields[]` was overwritten) |
| c | [UnitView.c:2538](../../MoM/src/UnitView.c#L2538) | fixed |

The original is an array of 40 records of 38 bytes = 1520. ReMoM reshaped it into a struct of arrays whose `picts[]` is `SAMB_INT` (pointer-sized), so on 64-bit it is 1760 bytes, but the `Near_Allocate_Next(1520)` constant was kept. The tail overruns the next near allocation (`product_fields[]` on the production screen). The constant is faithful; the struct is not. Fix: `Near_Allocate_Next(sizeof(struct s_UV_List))`.

### L41 — selling a building removes the wrong building in the wrong city — **ReMoM, in the prototype** (same as #19)

[City_ovr55.c:1064](../../MoM/src/City_ovr55.c#L1064), [CityScr.c:808](../../MoM/src/CityScr.c#L808)

The asm frame puts the building first:

```
ovr055/City_Sell_Building.asm   bldg_idx= word ptr  6
                                city_idx= word ptr  8
                                gold_amount= word ptr  0Ah
```

and `City_Screen__WIP` pushes `building_value`, `_city_idx`, `cityscape_bldg_idx` — building first. So [CityScr.c:808](../../MoM/src/CityScr.c#L808) `City_Sell_Building(cityscape_bldg_idx, _city_idx, building_value)` is faithful; ReMoM's prototype `(city_idx, bldg_idx, gold_amount)` was swapped. Now `(bldg_idx, city_idx, gold_amount)` in both the definition and [City_ovr55.h:65](../../MoM/src/City_ovr55.h#L65), as was done for `City_Remove_Building` in #19. CityScr.c:808 is the only caller.

Aside: the original pushes `_city_idx` as an extra, unused argument to `City_Sell_Building_Value` (the callee reads only `[bp+6]`). ReMoM's one-argument version is fine.

### L42 — city screen farmer/worker clicks — **Not a bug (faithful)**

[CityScr.c:943-971](../../MoM/src/CityScr.c#L943-L971), asm `ovr054/City_Screen__WIP.asm` lines 1556-1615.

The report: "clicking citizen i set i+1 farmers (first -> 0), so clicking any farmer but the first did nothing. Now: click farmer i -> i farmers; click worker i -> i+1 farmers; never below the minimum. [ReMoM, our reading of the intended rule]"

ReMoM matches the asm branch for branch:

```
ovr054/City_Screen__WIP.asm   cmp     [bp+required_farmer_count], 0
                              jnz     short loc_48CAC          ; minimum > 0
                              cmp     [es:bx+s_CITY.farmer_count], 0
                              jnz     short loc_48C78
                              mov     al, [byte ptr bp+itr]
                              inc     al                       ; no farmers yet: itr + 1
                              ...
                              loc_48C78:
                              cmp     [bp+itr], 0
                              jnz     short loc_48C93
                              ...  farmer_count, 0             ; first citizen: 0
                              loc_48C93:
                              mov     al, [byte ptr bp+itr]
                              inc     al                       ; otherwise: itr + 1
                              ...
                              loc_48CAC:
                              cmp     ax, [bp+required_farmer_count]
                              jge     short loc_48CC7
                              mov     al, [byte ptr bp+required_farmer_count]   ; itr < minimum: the minimum
```

The original never asks whether the clicked citizen is a farmer or a worker; it uses only the citizen's position `i` in the row, farmers on the left:

| Situation | Original (ReMoM) | Reporter |
|---|---|---|
| no minimum, no farmers yet, click citizen `i` | `i + 1` farmers | `i + 1` farmers (it's a worker) |
| no minimum, click the first citizen while there are farmers | 0 farmers | 0 farmers (`i = 0`) |
| click farmer `i`, `i > 0` | `i + 1` farmers — the clicked farmer stays a farmer, the ones to its right become workers | `i` farmers — the clicked farmer becomes a worker too |
| click worker `i` | `i + 1` farmers | `i + 1` farmers |
| minimum > 0, click `i` < minimum | the minimum | the minimum ("never below the minimum") |

The one difference is clicking a farmer other than the first: the original keeps the clicked one a farmer, the reporter turns it into a worker. Their description of the original is also not quite right — "clicking any farmer but the first did nothing" holds only for the last farmer, where `i + 1` is already the count; with 3 farmers, clicking the middle one leaves 2. With a minimum above 0 the first citizen cannot be clicked down to 0 (that toggle is only in the no-minimum branch), which their "never below the minimum" matches.

The reporter's rule is a UI change they prefer ("our reading of the intended rule"), not a defect in the original. No code change, no note.

### L43 — Klackon outpost cityscape indexes past the array — **ReMoM**

[CITYSCAP.c:3031](../../MoM/src/CITYSCAP.c#L3031), [3033](../../MoM/src/CITYSCAP.c#L3033)

```
ovr144/Outpost_Cityscape_Draw.asm   mov     ax, [bp+race_house_type]
                                    mov     dx, 10
                                    imul    dx
                                    mov     bx, ax
                                    push    [cityscape_houses_seg+bx]      ; byte offset into 2-byte entries
```

`* 10` is a byte offset into an array of segment words, i.e. element `* 5` — as ReMoM already does at [CITYSCAP.c:1379](../../MoM/src/CITYSCAP.c#L1379). Now `cityscape_houses_seg[(race_house_type * 5)]` at both lines.

### L44 — `Building_Allows_List` (a, b) — **ReMoM**

| | Problem | ReMoM | Asm (`ovr056/Building_Allows_List__WIP.asm`) | Status |
|---|---|---|---|---|
| a | `reqd_bldg_1 == bldg_idx && reqd_bldg_2 == bldg_idx` | [ProdScr.c:672-676](../../MoM/src/ProdScr.c#L672-L676) | `jz loc_4F00E` after each compare — either matches | fixed: `\|\|` |
| b | unit half was `// TODO` | [ProdScr.c:717-747](../../MoM/src/ProdScr.c#L717-L747) | `loc_4F101`–`loc_4F1B4`: race is the city's or `rt_Standard`; unit requires `bldg_idx`; either prerequisite `!= bs_NotBuilt`; add `100 + itr` | fixed, reconstructed from the asm (not the reporter's version, which used `0` for `rt_Standard` and `bs_NotBuilt`) |

Also: the helper `BLD_AllowedByRace__WIP_TRUE` was a stub (`return Building`); now `City_Race_Can_Unlock_Building` ([ProdScr.c:1216](../../MoM/src/ProdScr.c#L1216)), reconstructed from `ovr056/BLD_AllowedByRace__WIP_TRUE.asm` — returns the building only if a prerequisite is unbuilt and the race's `cant_build` list, propagated 50 passes through the prerequisite tree, does not exclude it. The first parameter is never read (`uu_allowed_by_bldg_idx`).

`reqd_terrain` is a union alias of `reqd_bldg_1`, so the terrain branch matches.

### L45 — Load screen: first slot unselectable, slot N loads SAVE(N+1) — **ReMoM**

[LoadScr.c:368](../../MoM/src/LoadScr.c#L368)

```
ovr160/Load_Screen.asm   mov     bx, [save_game_count]
                         shl     bx, 1
                         mov     ax, itr
                         dec     ax
                         mov     [save_game_slots+bx], ax
```

The original stores the 0-based row (`itr - 1`); ReMoM stored the file number `itr` (1..8), so the list only gated the Load button wrongly: row `r` was enabled when SAVE`r`.GAM existed, and `Load_SAVE_GAM(r)` then read SAVE`(r+1)`.GAM. The save side sets its index from the row directly and was never affected. Present since the file was created. Now `= (itr - 1);`.

### L46 — Continue: the map is drawn in the menu palette — **Architecture**

[MOM_SCR.c:120-172](../../MoM/src/MOM_SCR.c#L120-L172)

In the original, Continue launches a fresh `WIZARDS.EXE` (`GAME_WizardsLaunch__WIP(e_SAVE9GAM)`), whose start-up loads and applies the game palette. ReMoM already lists those steps as comments at [MOM_SCR.c:131-140](../../MoM/src/MOM_SCR.c#L131-L140) (`Load_Palette(0, ST_UNDEFINED)`, `Calculate_Remap_Colors()`, `Cycle_Palette_Color(198, ...)`, `Apply_Palette()`, `Fade_In()`), but the single-process `Continue` path never ran them. Decided: run those WZD start-up steps in the Continue case (not `Load_Screen`'s sequence, as the reporter did). Now in place after `Load_SAVE_GAM(8)`, before `Loaded_Game_Update()` ([MOM_SCR.c:172-177](../../MoM/src/MOM_SCR.c#L172-L177)). Builds; palette confirmed correct in game.

### L47 — minimap shows unexplored enemy cities — **ReMoM**

[MainScr_Maps.c:3224](../../MoM/src/MainScr_Maps.c#L3224)

The `// DELETEME` line just above used the city's square; the macro replacement switched to `minimap_square_x/y` — leftovers from the terrain loop. The asm tests the city square (`ovr150/Create_Reduced_Map_Picture.asm`: `explore_data_ptr[(_DI_city_world_y * e_WORLD_WIDTH) + _SI_city_world_x]`). Now `GET_SQUARE_EXPLORED(city_world_x, city_world_y, wp)` ([MainScr_Maps.c:3223](../../MoM/src/MainScr_Maps.c#L3223)).

### L48 — `Create_Blank_Picture` fills pixels 1..length — **ReMoM**

[FLIC_Draw.c:881-901](../../MoX/src/FLIC_Draw.c#L881-L901)

```
seg030/Create_Blank_Picture.asm   mov     di, 16                       ; SZ_FLIC_HDR
                                  mov     ax, [bp+color]
                                  mov     cx, [bp+length]
                                  rep stosb
```

The original fills `length` bytes from the first pixel. `while(itr_length++ < length)` writes pixels 1..length: pixel 0 is left as-is and one byte past the picture is written. Now a `for` from 0 to `length - 1` ([FLIC_Draw.c:897](../../MoX/src/FLIC_Draw.c#L897)).

### L49 — BUILD/PURIFY button stays after the stack is gone — **ReMoM**

[MainScr.c:2037-2060](../../MoM/src/MainScr.c#L2037-L2060)

```
ovr057/Add_Unit_Action_Fields.asm   cmp     [_unit_stack_count], 0
                                    jg      short loc_5101E
                                    jmp     @@NoSpecialAction
```

In the asm the special-button test is nested inside `_unit_stack_count > 0`; ReMoM had it after that block, unconditionally. Now nested inside.

### L50 — Cartographer: units and cities drawn too high; terrain unfinished (a, b) — **ReMoM**

| | Problem | Status |
|---|---|---|
| a | `y = m_cartograph_x + wy * 4 ...` for units and cities; the asm adds `[m_cartograph_y]` | fixed: [AdvsrScr.c:1549](../../MoM/src/AdvsrScr.c#L1549), [1584](../../MoM/src/AdvsrScr.c#L1584) |
| b | terrain map was a `/* HACK */` empty picture; `Cartograph_Screen_Draw_Map_Terrain__NOWORKIE` unfinished | reconstructed from `ovr150/Cartograph_Screen_Draw_Map_Terrain__NOWORKIE.asm` as [Cartograph_Screen_Draw_Map_Terrain](../../MoM/src/MainScr_Maps.c#L3282) (color LUT at `cartograph_seg[0]`, picture at +16 paragraphs, 4x4 samples of each terrain picture, partial-explore edges via `off_DAFD8[flag - 1]`); caller draws `cartograph_seg + 16 paragraphs` |

Not the reporter's version (explored squares in minimap colours). Also fixed in the caller while matching it to the asm: tower dot colour 140 (`ST_GREEN`, was 7), unit owner test against `_current_player_idx` (`[_human_player_idx]` in the Dasm), and `var_8` split into `legend_row` / `uu_drew_neutral_city`.

Builds and passes HeMoM; screen confirmed in game. All four Cartographer functions (`Cartographer_Screen`, `Cartographer_Screen_Draw`, `Cartograph_Screen_Draw_Map`, `Cartograph_Screen_Draw_Map_Terrain`) match the asm; `Cartographer_Screen_Draw` also gained the asm's early return after drawing the hovered city name.

### L51 — History header always "September"; menu label — **ReMoM**

[AdvsrScr.c:811](../../MoM/src/AdvsrScr.c#L811), [AdvsrScr.c:63](../../MoM/src/AdvsrScr.c#L63)

1. ```
   ovr076/Powergraph_Screen_Draw.asm   mov     ax, [_turn]
                                       mov     bx, 12
                                       cwd
                                       idiv    bx
                                       mov     [bp+WTF__turns_months], dx
                                       mov     ax, [bp+WTF__turns_months]
                                       mov     dx, 12
                                       imul    dx                       ; byte offset into char[12][12]
                                       lea     dx, [bp+months]
                                       add     ax, dx
   ```

   That is `months[_turn % 12]`. ReMoM's first attempt (commented out) used the `* 12` byte offset as an index; the code fell back to a fixed `months[8]`. Now `months[turns_months]`.
2. The original string is `cnst_Info_Msg_4 db 'Historian',0` (`in/WIZARDS.asm`); ReMoM had `"history"`; now `"Historian"`.

### L52 — banner screen covers the wizard name's shadow — **ReMoM**

[NewGame.c:3084](../../MoM/src/NewGame.c#L3084)

`MAGIC/ovr050/Newgame_Screen_7_Draw__WIP.asm` makes four `Print_Centered` calls: the title, then the name at x = 78, 77, 77 (shadow, shadow, light). ReMoM has a fifth — `Print_Centered(77, 120, ...)` after the light text — with no asm counterpart. Now removed; the three name prints are (78,120), (77,120), (77,119) as in the asm.

### L53 — Surveyor string contains IDA text — **ReMoM**

[Surveyor.c:181](../../MoM/src/Surveyor.c#L181)

`char str_Nightshade__ovr094[] = "Nightshade',0 ; should use dseg:2";` — the IDA `',0` terminator and a comment were pasted into the literal. Now `"Nightshade"`.

### L54 — `Notify2` box runs past x = 319 with the Chaos mascot — **OGBUG**

[GENDRAW.c:1596-1604](../../MoX/src/GENDRAW.c#L1596-L1604)

```
ovr149/Notify2.asm   cmp     [notify2_mascot_x1+bx], 0
                     ...
                     mov     ax, 6
                     sub     ax, [notify2_mascot_x1+bx]
                     mov     [message_box_x], ax
```

ReMoM matches. With the Chaos mascot (`x1 = -22`) and a full-width paragraph the original's box also runs off the right edge; on DOS it wraps into the next scanline the same way. `OGBUG` note at [GENDRAW.c:1596](../../MoX/src/GENDRAW.c#L1596); the reporter's shift-left fix is not taken.

Also checked: `Notify2` calling `Notify1_Draw()` (marked `BUGBUG` in ReMoM) — the asm does it too (`call near ptr Notify1_Draw`). **OGBUG**, correctly left.

## Memory and the single process

### L57 — Load/Save screen reloads WZD resources on every visit — **Architecture**

[MOM_SCR.c:147](../../MoM/src/MOM_SCR.c#L147), [189](../../MoM/src/MOM_SCR.c#L189)

Both calls are ReMoM `/* HACK */` lines with no asm counterpart: in the original, WIZARDS.EXE loaded these resources once at start-up. Each visit carves new arenas from the never-freed static pool. Decided: load once. `Load_WZD_Resources_Once()` ([LOADER.c:238](../../MoM/src/LOADER.c#L238)) guards with `g_wzd_resources_loaded`; both `MOM_SCR.c` sites ([148](../../MoM/src/MOM_SCR.c#L148), [198](../../MoM/src/MOM_SCR.c#L198)) call it.

### L58 — `Open_File_Animation__HACK` leaks a pool arena per call — **Architecture**

[FLIC_Draw.c:2243](../../MoX/src/FLIC_Draw.c#L2243)

The original streams the animation: `Open_File_Animation` loads up to 16300 bytes of the entry into logical page 0 of the `VGAFILEH` EMM handle, and `Draw_File_Animation` loads one frame at a time into page 1 and draws it from there. ReMoM's `__HACK` replacements called `LBX_Load` (a new `sa_Single` arena) on every open. Decided: the faithful module, with `_VGAFILEH_seg` (the platform's EMM page stand-in) as the one reusable buffer.

- seg030: `Open_File_Animation`, `Draw_File_Animation`, `Load_File_Animation_Frame`, `Get_File_Animation_Frame`, `Set_File_Animation_Frame` in [FLIC_Draw.c](../../MoX/src/FLIC_Draw.c#L2250); seg010 `LBX_Get_Entry_Data` in [LBX_Load.c:870](../../MoX/src/LBX_Load.c#L870).
- seg025: `Draw_File_Animation_Frame_ASM`, `Map_File_Animation_Frame_ASM` and `Remap_Draw_File_Animation_Frame_ASM`, translated to C in [file_ani.c](../../MoX/src/file_ani.c); linear screen buffer instead of planar VGA, and the frame-data offset passed as a parameter instead of in SI.
- The seg009 DOS wrappers (`lbx_seek`, `lbx_read_sgmt`) are direct `fseek`/`fread`, as everywhere else in `LBX_Load.c`.
- The size check is `>= 65536` (the asm errors once the high word is 1).

Exercised by `demo_vga conquest <winner> <loser>` ([effect_conquest.c](../../demo/effect_conquest.c)), whose stages 11-18 stream and draw the ZAP file animation (CONQUEST.LBX 17); headless runs reach stage 31 without error. Building it found AF10. The drawn zap was checked in the game and works.

### L61 — New Game without restarting keeps the previous game's data — **Architecture**

The original reached a new game through a fresh `MAGIC.EXE` / `WIZARDS.EXE`, so every data segment started clean: the in-game Quit respawned `MAGIC.EXE` (the commented-out `Respawn` at [LoadScr.c:463](../../MoM/src/LoadScr.c#L463)). ReMoM runs everything in one process, so anything a new game does not write itself kept the previous game's value. This is the root of AF8 and AF11.

**Fix:** `Reset_Game_State()` ([MOM_SCR.c:73-111](../../MoM/src/MOM_SCR.c#L73-L111)) puts the data a new game relies on back to what a fresh process has. It runs on the way back into the Main Menu from a game ([MOM_SCR.c:166-171](../../MoM/src/MOM_SCR.c#L166-L171)) — the point where the original respawned — guarded by `dirty_game_state`, which the Main Screen case sets ([MOM_SCR.c:400](../../MoM/src/MOM_SCR.c#L400)). Every way into a game (New Game, Continue, the menu's Load) reaches the Main Screen; the first Main Menu entry at start-up does not reset, so the first game is unchanged, and the in-game Load never passes through the menu, as in the original.

| Reset | Fresh-process value | Why |
|---|---|---|
| re-seed as start-up ([Init.c:75-83](../../MoM/src/Init.c#L75-L83)) | `Set_Random_Seed(_cmd_line_seed)` / `Randomize()` | world generation otherwise starts from game 1's random state |
| `_turn` | 0 | nothing in the game writes 0; with it stale the turn-0 rename popup (AF11) is skipped too |
| `_given_chance_to_rename_home_city` | `ST_FALSE` | AF11 |
| `magic_master_idx`, `m_magic_winner_idx` | `ST_UNDEFINED` | end-of-game winner |
| `_players[]` (whole array, includes AF8's `_ai_landmass_war_targets` and `research_spells`) | 0 (static) | New Game leaves diplomacy, historian, research progress, ratios, `magic_strategy` etc. alone; stale `research_spells` blocked `Player_Research_Spells` from marking any spell researchable |
| `_CITIES`, `_LAIRS`, `_UNITS`, `_ITEMS` | `POOL_SENTINEL_BYTE` (0xCC) | neutral-city creation ([MAPGEN.c:5693-5734](../../MoM/src/MAPGEN.c#L5693-L5734)) never writes `farmer_count` / `food_units` / `production_units`, which the first city calculation reads; lairs 99-101 are never generated but `Set_Upper_Lair_Guardian_Count` rescales all `NUM_LAIRS`; units and items past this game's count kept game 1's |
| first 16 bytes of `_UNITS`, `_ITEMS`, `_LAIRS` | the `Allocate_Space` block header | those pointers *are* the block header ([Allocate.c:349-370](../../MoX/src/Allocate.c#L349-L370)), so element 0 starts on it; saved at the first Main Menu entry ([MOM_SCR.c:172-178](../../MoM/src/MOM_SCR.c#L172-L178)) and copied back after the fill |

**Test:** `HeMoM_L61_*` ([tests/CMakeLists.txt](../../tests/CMakeLists.txt)). Two runs, both `HeMoM --menu --seed1 12345`, both creating their games through the New Game screens:

- [test_l61_fresh.hms](../../assets/test_l61_fresh.hms): one game, saved at turn 0. `HeMoM_L61_Fresh_Assertions` ([assert_l61_fresh.txt](../../tests/assert_l61_fresh.txt)) checks the scripted game really happened — HeMoM exits 0 even when a dropped click sends the script onto the wrong screens.
- [test_l61_second_game.hms](../../assets/test_l61_second_game.hms): a game played 3 turns (enough for the AI to set non-zero AF8 war targets on both planes), Game → Quit → Main Menu, then the same game, saved at turn 0.

`HeMoM_L61_Compare` checks every field of the second run's SAVE1 dump against the fresh run's: 66068 of 66068 match, on MSVC and clang. Anything a later change leaves stale between games shows up there.

### AF8 — `_ai_landmass_war_targets` survives into the next game — **Architecture**

[MOM_DAT.c:906](../../MoX/src/MOM_DAT.c#L906)

Split out of L24. The original ran `MAGIC.EXE` (new game) and then a fresh `WIZARDS.EXE`, so this WIZARDS.EXE data started from its load image every game. ReMoM is one process, and nothing resets the array between games, so an AI war target from the previous map can name a landmass that does not exist on the new one — which is how the reporter reached the zero dock-square count in L24. One instance of L61. Fixed by L61's per-game reset: the array lives inside `_players[5].spells_list` ([ALLOC.c:300-301](../../MoM/src/ALLOC.c#L300-L301)), which the reset zeroes with the rest of `_players`. `HeMoM_L61_Compare` covers it — game 1 plays 3 turns, after which players 1-4 have non-zero targets on both planes.

### AF9 — AI crafting cost formula drops `remaining -` — **ReMoM**

[SBookScr.c:485](../../MoM/src/SBookScr.c#L485), [503](../../MoM/src/SBookScr.c#L503)

```
ovr134/Cast_Spell_Overland_Do.asm   mov     ax, [_players.casting_cost_remaining+bx]
                                    ...
                                    mov     ax, [bp+var_8]              ; cost_reduction
                                    call    LXMUL@                      ; remaining * cost_reduction (32-bit)
                                    ...
                                    call    LDIV@                       ; / 100
                                    ...
                                    mov     ax, [_players.casting_cost_remaining+bx]
                                    pop     dx
                                    sub     ax, dx                      ; remaining - that
```

ReMoM stores `(remaining * cost_reduction) / 100` — the reduction instead of the reduced cost. With `cost_reduction = 0` the cost becomes 0 and the `casting_cost_remaining == 0` test cancels the spell, so the AI never completes an item spell. In the asm (with `cost_reduction` uninitialized, AF6) the cost is reduced by a garbage percentage instead. Present since `f90e1106` (2024-10-07). Now, both AI branches:

```c
_players[player_idx].casting_cost_remaining = (_players[player_idx].casting_cost_remaining - (int16_t)(((int32_t)_players[player_idx].casting_cost_remaining * (int32_t)cost_reduction) / 100));
```


### AF10 — `Conquest_Animation` early-return guard inverted — **ReMoM**

[CONQUEST.c:370-378](../../MoM/src/CONQUEST.c#L370-L378)

```
ovr093/Conquest_Animation.asm   cmp     [bp+player_idx], e_NEUTRAL_PLAYER_IDX
                                jz      short loc_78FA6          ; winner is neutral -> go on
                                cmp     city_owner_idx, e_NEUTRAL_PLAYER_IDX
                                jnz     short loc_78FA6          ; loser is not neutral -> go on
                                xor     ax, ax
                                jmp     @@Done
```

The asm returns only when the winner is a wizard and the loser is neutral. ReMoM tested `loser_Idx != NEUTRAL_PLAYER_IDX`, which returns for every wizard-on-wizard conquest, so the animation only ever played for raiders (whose path skips the ZAP file animation). The JZ/JNZ inversion pattern. Present since the file was added (`84a11ad3`, 2025-09-21). Found while building the `demo_vga conquest` effect to exercise L58's `Draw_File_Animation`. Now:

```c
(loser_Idx == NEUTRAL_PLAYER_IDX)
```

### AF11 — `_given_chance_to_rename_home_city` survives into the next game — **Architecture**

[MainScr.c:88](../../MoM/src/MainScr.c#L88), [1934-1951](../../MoM/src/MainScr.c#L1934-L1951)

Found by the L61 feasibility run (game 1 → Quit → Main Menu → New Game through the UI, headless). `Main_Screen` offers the rename popup when `_turn == 0` and this flag is `ST_FALSE`, then sets it `ST_TRUE`. It starts `ST_FALSE` only from the load image, and nothing clears it, so in the second game the popup never appears. One instance of L61. Fixed by L61's per-game reset, which sets it back to `ST_FALSE` (and `_turn` back to 0, which the popup also needs).
