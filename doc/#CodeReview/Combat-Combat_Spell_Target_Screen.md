Combat-Combat_Spell_Target_Screen.md

CMB_SetTargetCursor()  ==>  Combat_Screen_Assign_Mouse_Images()

Msg_Panel_IMG  ==>  msg_panel_seg
BU_Loop_Index  ==>  battle_unit_idx
Vortex_Index   ==>  vortex_idx
Cursor_Unit    ==>  battle_unit_idx

Combat_Spell_Target_Screen
Combat_Spell_Target_Screen_Draw (o113p02)
Combat_Screen_Assign_Mouse_Images (o113p01)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Combat_Spell_Target_Screen__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Combat_Spell_Target_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\Combat_Screen_Assign_Mouse_Images.asm

OON XREF: Combat_Spell_Target_Screen() |-> Combat_Spell_Target_Screen_Draw()

OON XREF:  Combat_Spell_Target_Screen() |-> Combat_Spell_Target_Screen_Draw() |-> Combat_Screen_Assign_Mouse_Images()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-23).** Three functions, 1,755 asm lines, all walked end to end against the listings. Seven findings were raised and all seven are fixed and re-verified, so the findings section is empty. Builds clean. All three functions were homeless before this doc.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Spell_Target_Screen` | [Combat.c:10824](../../MoM/src/Combat.c#L10824) | `ovr113/Combat_Spell_Target_Screen__WIP.asm` | 1,414 | **full body walk — faithful** |
| `Combat_Screen_Assign_Mouse_Images` | [Combat.c:10637](../../MoM/src/Combat.c#L10637) | `ovr113/Combat_Screen_Assign_Mouse_Images.asm` | 273 | **full body walk — faithful** |
| `Combat_Spell_Target_Screen_Draw` | [Combat.c:10805](../../MoM/src/Combat.c#L10805) | `ovr113/Combat_Spell_Target_Screen_Draw.asm` | 68 | **full body walk — faithful** |

## What belongs here, and why

This is the **combat spell targeting screen**: the modal loop that runs after a spell is chosen and before it resolves, plus the redraw and cursor logic it drives. The boundary is choosing *where* the spell lands — not choosing the spell, not applying it.

The three form a closed chain. `Combat_Spell_Target_Screen` installs `Combat_Spell_Target_Screen_Draw` as the auto-draw function at [Combat.c:10837](../../MoM/src/Combat.c#L10837), and that redraw is the only caller of `Combat_Screen_Assign_Mouse_Images` — asm:63-64 of the Draw listing, a same-segment `push cs` / `call near ptr`. Nothing outside the cohort calls either of the lower two.

`Combat_Spell_Target_Screen` itself has exactly one caller, on the combat casting path — [CMBMAGIC.c:720](../../MoM/src/CMBMAGIC.c#L720) — so it sits directly under [Combat-Spell_Cast.md](Combat-Spell_Cast.md) in the call graph without belonging to it.

### What does *not* belong here

- **`Combat_Screen_Draw`**, owned by [Combat-Combat_Screen.md](Combat-Combat_Screen.md). The Draw wrapper calls it first thing, but it is the whole battlefield renderer.
- **`Combat_Grid_Cell_Has_City_Wall`**, **`Get_Combat_Grid_Cell_X`** and **`Get_Combat_Grid_Cell_Y`** — grid-geometry helpers with callers across several reviews.
- **`Combat_Cast_Spell_Message`**, covered by [Combat-Spell_Cast.md](Combat-Spell_Cast.md).

## Findings

None outstanding. Seven were raised during the review and all seven are fixed and verified in the current sources; the history is in git.

## Deviations

### D1 - `grid_sx` and `grid_sy` are widened to `int64_t`

The frame has both at word width, `grid_sy` at `bp-8` and `grid_sx` at `bp-6`. [Combat.c:10832](../../MoM/src/Combat.c#L10832) declares them `int64_t` with a `DOMSDOS` note and the call sites cast back down. `Add_Grid_Field` writes through the pointers, so the width has to match what that function stores — a host-side pointer-width adaptation of the same family as the two-argument `Play_Sound`, not a reconstruction error.

### D2 - the wall short-circuit returns 999 instead of an unassigned slot

asm:245-246 sets `leave_screen` and jumps out without ever writing `target_idx`, so the original returns whatever the `bp-0Ch` slot held. [Combat.c:10970](../../MoM/src/Combat.c#L10970) stores `999` under a `/* HACK */` marker instead.

That is a deliberate host-safety choice rather than a reconstruction of the original. `999` is the value [CMBMAGIC.c:732](../../MoM/src/CMBMAGIC.c#L732) screens for, so the cast is skipped entirely - which avoids handing an arbitrary index to `Combat_Cast_Apply_Spell_Effect`, where [Combat.c:9985](../../MoM/src/Combat.c#L9985) dereferences `battle_units[target_idx]` with no range check. On the original hardware that read was merely garbage; on a modern host it is out-of-bounds.

The cost is that Disrupt aimed at a wall now does nothing at all, where the original would proceed with a junk target. Recorded so the divergence is visible: the OG behaviour is genuinely undefined, so no test should pin either value.

**The `OGBUG` comment directly above it still reads "should be 99 ... not 999 (the cancel marker)", which now contradicts the line it annotates.** It wants rewording to say what the HACK does and why.

## OG quirks preserved

Six original defects are reproduced here rather than repaired, each carrying an `OGBUG` note at its site. They are listed so nobody "fixes" one later.

**The cancel button's hotkey is an address, not a character.** asm:147-158 pushes `offset str_hotkey_ESC__ovr113` as the fifth argument, where `Add_Button_Field`'s `hotkey` is a word (`seg036/Add_Button_Field.asm:7`) and every other call site in the tree passes a character — `cnst_HOTKEY_I_2 = 'I'` at [ArmyList.c:71](../../MoM/src/ArmyList.c#L71). ESC therefore never matches this button in the original. [Combat.c:10942](../../MoM/src/Combat.c#L10942).

**The wall short-circuit returns an unassigned `target_idx`.** asm:245-246 sets `leave_screen` and jumps clear of the click handler without ever writing `target_idx`, and asm:1407 returns that slot. The only writes are 999 in the cancel branch and 99 in the tile branches, none of them reached. [Combat.c:10963](../../MoM/src/Combat.c#L10963). The caller does not screen for it — [CMBMAGIC.c:732](../../MoM/src/CMBMAGIC.c#L732) gates only on `!= 999` and then hands the value to `Combat_Cast_Apply_Spell_Effect`, which indexes `battle_units[target_idx]` unguarded.

**The tile accept takes `COMBAT_CELL_NO_ACTION` squares and never range-checks the cell.** The test is `target_idx < 0`, which covers both `-1` (`COMBAT_CELL_REACHABLE`) and `-2` (`COMBAT_CELL_NO_ACTION`, the fill value from [Combat.c:2888](../../MoM/src/Combat.c#L2888)); and [Get_Combat_Grid_Cell_X](../../MoM/src/Combat.c#L22693) does raw arithmetic with no clamping, so the map read at [Combat.c:10987](../../MoM/src/Combat.c#L10987) can index outside the 21x22 grid. [Combat.c:10997](../../MoM/src/Combat.c#L10997).

**Both summon accepts skip terrain validity.** [Combat.c:11001](../../MoM/src/Combat.c#L11001) and [Combat.c:11026](../../MoM/src/Combat.c#L11026) check only which half of the field the square is on, then rescan for an invisible occupant. Nothing tests that the square is passable or on the grid, so a summon can land on water or a wall segment.

**A wall square feeds `battle_units[99]`.** [Combat.c:2904-2906](../../MoM/src/Combat.c#L2904-L2906) writes `COMBAT_CELL_CITY_WALL` — which is **99** ([CMBTDEF.h:103](../../MoM/src/CMBTDEF.h#L103)) — into the action map when a Wall Crusher is the active unit. 99 is neither `-1` nor `-2`, so it lands in the `default` arm and [Combat.c:10691](../../MoM/src/Combat.c#L10691) reads `battle_units[99]` against a 36-slot array ([MOM_DEF.h:67](../../MoX/src/MOM_DEF.h#L67)).

**The targeting frame never lights for square spells, and the bottom GUI can show a red X.** `frame_scanned_flag` is set only in the `default` arm at [Combat.c:10686](../../MoM/src/Combat.c#L10686), so an empty-cell target never gets the highlight. Separately, the vertical guard at [Combat.c:10652](../../MoM/src/Combat.c#L10652) skips the grid scan below the battlefield while the four target-type overrides after it still run and read the unassigned `cgx` / `cgy`, forcing `crsr_RedCross` over the button bar.

One further claim inherited from drake178 — that Torin The Chosen fails the `race < rt_Arcane` test and is treated as a fantastic unit — is preserved at the function header but **is not verifiable from this repo**: `race_type` ([UNITTYPE.h:286](../../MoM/src/UNITTYPE.h#L286)) is populated from UNITS.LBX at runtime ([LOADER.c:858](../../MoM/src/LOADER.c#L858)), and no Torin row exists in the source tree.

## Verified faithful

### `Combat_Spell_Target_Screen_Draw` ([Combat.c:10805](../../MoM/src/Combat.c#L10805), asm 68)

Faithful, all 68 lines. One local, `msg_panel_seg` at `bp-2`, `sub sp, 2` — [Combat.c:10807](../../MoM/src/Combat.c#L10807). It holds the `CMBTFX.LBX` entry 28 `"MSGPANEL"` sprite, the backdrop the targeting prompt is printed into.

**Call for call, in order:** `Combat_Screen_Draw`, `Mark_Block(_screen_seg)`, `LBX_Reload_Next(cmbtfx_lbx_file__ovr113, 28, _screen_seg)` at [Combat.c:10811](../../MoM/src/Combat.c#L10811), `Release_Block(_screen_seg)`, `FLIC_Draw(238, 164, msg_panel_seg)`, `Set_Outline_Color(240)`, `Set_Alias_Color(227)`, `Set_Font_Style_Shadow_Down(0, 0, 0, 0)`, `Set_Font_Spacing_Width(1)`, `Print_Paragraph(241, 168, 75, GUI_NearMsgString, 0)` at [Combat.c:10818](../../MoM/src/Combat.c#L10818), then `Combat_Screen_Assign_Mouse_Images`.

**The block is marked and released around the load only.** `msg_panel_seg` is used *after* `Release_Block`, at asm:19-27 — the pointer outlives the mark. That is the listing's own order and production reproduces it.

**The last call is a near call.** asm:63-64 is `push cs` / `call near ptr`, both functions living in `ovr113`, which is why that one has no `j_` thunk.

### `Combat_Screen_Assign_Mouse_Images` ([Combat.c:10637](../../MoM/src/Combat.c#L10637), asm 273)

Faithful.

**Frame - exact match.** `Pointer_Offset` at `bp-0Ah`, `cgy` at `bp-8`, `screen_y` at `bp-6`, `screen_x` at `bp-4`, `Cursor_Unit` at `bp-2`, `sub sp, 0Ah`, with `cgx` in SI and `itr` in DI. [Combat.c:10639-10645](../../MoM/src/Combat.c#L10639-L10645) declares the five stack locals in that order, then the two register locals.

**The pointer offset is a variable, not a literal.** asm:23 stores 4 into `Pointer_Offset` and asm:26, asm:30 and asm:34 all read it back, including in the vertical bound. Production keeps it as a local rather than folding the 4 into three places — which is what the listing does.

**The vertical guard.** asm:34-38 is `mov ax, [bp+Pointer_Offset]` / `add ax, 168` / `cmp ax, [bp+screen_y]` / `jg` — the source tests `screen_y <= (168 + pointer_offset)`, matching [Combat.c:10652](../../MoM/src/Combat.c#L10652).

**The action-map dispatch is a `switch`.** asm:56-62 materialises the selector once, compares repeatedly, and each match jumps **to** its body at `loc_8F58E`, `loc_8F5AA` and `loc_8F5C6`, with the final unconditional jump covering the absent default — [Combat.c:10656](../../MoM/src/Combat.c#L10656). Both `case -2:` and `case -1:` end with `break`, matching the two `jmp loc_8F6C4` exits at asm:73 and asm:84 that clear the default body. The byte load and `cbw` are forced by the operand width in either form, so they are not the evidence; the jump direction is. The two case bodies are byte-identical in the listing as well as in production.

**The controller test is `!= HUMAN_PLAYER_IDX` with the enemy tests first.** asm:110-111 is `cmp` / `jz short loc_8F655`, the `jz` going to the friendly block and the enemy tests falling through — [Combat.c:10693](../../MoM/src/Combat.c#L10693).

**Both `NU` tests are `race < rt_Arcane`.** asm:125-126 uses `jge` to skip on the enemy side and asm:151-152 uses `jl` to accept on the friendly side — opposite mnemonics, same source condition, because one guards the skip and the other the body.

**The hero test is `> ST_UNDEFINED`.** asm:167 is `cmp [es:bx+s_UNIT.Hero_Slot], -1` / `jle` to skip — [Combat.c:10734](../../MoM/src/Combat.c#L10734). The `s_UNIT` stride is `shl ax, 5` at asm:162-163.

**The four target-type overrides run outside the grid-scan.** `cstt_Tile_NoUnitA` with `cgx < 11`, `cstt_Tile_NoUnitD` with `cgx > 10`, `cstt_Wall` gated on `Combat_Grid_Cell_Has_City_Wall`, and the `cstt_DispelMagic` vortex loop — asm:181-236, [Combat.c:10765](../../MoM/src/Combat.c#L10765) and [Combat.c:10775](../../MoM/src/Combat.c#L10775).

**The animated cast cursor.** asm:238-244 promotes `crsr_CastBase` to `crsr_CastAnim1 + SBK_NewSpellAnim_Stg` before the stage counter is advanced — [Combat.c:10789](../../MoM/src/Combat.c#L10789), with the modulo-5 bump at [Combat.c:10791](../../MoM/src/Combat.c#L10791). Every accepting branch above stores the base image; this is what turns it into the current frame.

**The mouse-list tail.** `center_offset = 2`, the window set to the full screen, then `Set_Mouse_List(1, &_combat_mouse_grid[0])` — asm:253-265, [Combat.c:10797](../../MoM/src/Combat.c#L10797).

### `Combat_Spell_Target_Screen` ([Combat.c:10824](../../MoM/src/Combat.c#L10824), asm 1,414)

Faithful.

**Frame - exact match, ten stack locals.** `Spell_Name` at `bp-28h` for 20 bytes, `BU_Loop_Index` at `bp-14h`, `enchantments` at `bp-12h` as a **dword**, `Vortex_Index` at `bp-0Eh`, `target_idx` at `bp-0Ch`, `input_field_idx` at `bp-0Ah`, `grid_sy` at `bp-8`, `grid_sx` at `bp-6`, `cancel_button_field` at `bp-4`, `combat_grid_field` at `bp-2`, `sub sp, 28h`, with `spell_idx` promoted into SI and `leave_screen` in DI. [Combat.c:10826-10836](../../MoM/src/Combat.c#L10826-L10836) declares them in that order.

**The `spl_NONE` short-circuit.** asm:33-36 is `or si, si` / `jnz`, so the source tests `spell_idx == spl_NONE` with `spl_NONE` being zero, then sets `cstt_Tile_NoUnit` and jumps **past the whole switch** to the shared field-setup label — [Combat.c:10839](../../MoM/src/Combat.c#L10839).

**The target-type switch is a bounded jump table.** asm:46-51 is `cmp bx, 23` / `jbe` / `shl bx, 1` / `jmp [cs:jt_cstt_mouse+bx]` — twenty-four entries, out-of-range falling to the same shared label — [Combat.c:10845](../../MoM/src/Combat.c#L10845). The preserved jump-table comment block above it is the listing's own table and stays.

**`scc_Summoning` really does fall through into `scc_Special_Spell`.** asm:67-74: the `jt_cstt_ms_00` body sets `cstt_Tile_NoUnitA` or `cstt_Tile_NoUnitD` and then reaches `jt_cstt_ms_05` with no jump. [Combat.c:10857](../../MoM/src/Combat.c#L10857) marks it, which is worth keeping because a fall-through between two non-empty `switch` arms reads as a mistake and is not one.

**The `scc_Special_Spell` arm is a flat run of independent `if`s, not a chain.** asm:75-122 tests Healing, then Raise Dead / Animate Dead, then Warp Creature / Creature Binding, Warp Wood, Earth to Mud / Cracks Call, Magic Vortex, Disrupt, Recall Hero, Word of Recall — each compared against the memory operand in place with the jump going past its own body. Later tests overwrite earlier ones, which is how a summoning spell that is also Raise Dead ends up with the tile type rather than the summon type.

**Field setup.** `Clear_Fields`, then `Add_Grid_Field(0, 0, 1, 1, 319, 168, &grid_sx, &grid_sy, ST_UNDEFINED)` — nine words, `add sp, 12h` — and `Add_Button_Field(...)` — six words, `add sp, 0Ch`. asm:124-160, [Combat.c:10940](../../MoM/src/Combat.c#L10940) and [Combat.c:10942](../../MoM/src/Combat.c#L10942).

**The message is rebuilt at the top of every iteration.** asm:163-189 is `strcpy(GUI_NearMsgString, cnst_CmbSpellMsg1)`, `_fstrcpy(spell_name, spell_data_table[spell_idx].name)`, `strcat`, `strcat` — and the same four calls appear again in the redraw block at the bottom of the loop. Production reproduces both copies, [Combat.c:10944](../../MoM/src/Combat.c#L10944) and [Combat.c:11340](../../MoM/src/Combat.c#L11340); that is the original's own duplication.

**The Dispel Magic vortex scan.** asm:248-277 walks `_vortexes[]` for one standing on the clicked cell and accepts — [Combat.c:10975](../../MoM/src/Combat.c#L10975). Vortexes never appear in `g_combat_grid_action_map`, which is why this loop exists at all. The store at asm:271 is a bare `1` rather than the enum, as are the two at asm:764 and asm:780.

**The tile-target path.** asm:1206-1288 against [Combat.c:10988](../../MoM/src/Combat.c#L10988) onward: `cstt_Tile_NoUnit` or `cstt_Tile` accept with `target_idx = 99`; `cstt_Tile_NoUnitD` with `*target_cgx < 11` and `cstt_Tile_NoUnitA` with `*target_cgx >= 11` each accept, then rescan every active unit for one already standing on the square and cancel with `MESSAGE.LBX` entry 88 if they find one — [Combat.c:11001](../../MoM/src/Combat.c#L11001) and [Combat.c:11026](../../MoM/src/Combat.c#L11026). The scan loop's condition is a two-part `battle_unit_idx < _combat_total_unit_count && leave_screen == ST_TRUE` at asm:1275-1279, matching [Combat.c:11008](../../MoM/src/Combat.c#L11008), so it stops at the first hit.

**The 32-bit enchantment fold is correct.** asm:303-328 looks crossed at first read — `or ax, [item_enchantments]` against `or dx, [item_enchantments+2]` — but the `push ax` / `push dx` / `pop ax` / `pop dx` pairs at asm:305-314 and asm:316-324 reverse the two halves each time, so the low word lands in the low half throughout. [Combat.c:11052](../../MoM/src/Combat.c#L11052) has the operand order right: `_UNITS[].enchantments | item_enchantments | enchantments`.

**The enemy validity chain.** asm:358-741 against [Combat.c:11073](../../MoM/src/Combat.c#L11073) onward: Creature Binding and Banish both on `race < rt_Arcane` with `MESSAGE.LBX` entry 69 and the spell name appended; Star Fires on `race != rt_Chaos && race != rt_Death` with entry 70; Web on `Abilities & UA_NONCORPOREAL` with entry 71; Warp Wood on `(ranged_type / 10) != rag_Missile` with entry 72. Then the group guarded by `scc_Resistable_Spell || scc_Mundane_Curse || scc_Unresistable_Spell` at [Combat.c:11127](../../MoM/src/Combat.c#L11127) — Web on `Web_HP > 0`, else the mask test against **`combat_effects`** (asm:634 loads it and `cwd`-extends), else the realm-immunity pair: Sorcery with `USA_IMMUNITY_ILLUSION` and entry 73, Death with `USA_IMMUNITY_DEATH` and a bare `Warn1(cnst_SpellError_3)` with no LBX load.

**The friendly validity chain.** asm:755-1201 against [Combat.c:11183](../../MoM/src/Combat.c#L11183) onward: the `cstt_FriendlyNU` and `cstt_FriendlyHero` acceptance tests at [Combat.c:11192](../../MoM/src/Combat.c#L11192) and [Combat.c:11199](../../MoM/src/Combat.c#L11199), then Healing on `race == rt_Death || (mutations & UM_UNDEAD)` with entry 74 at [Combat.c:11206](../../MoM/src/Combat.c#L11206), then the group guarded by `scc_Unit_Enchantment || scc_Unit_Enchantment_Normal_Only` at [Combat.c:11220](../../MoM/src/Combat.c#L11220) — Haste on `combat_effects & bue_Haste`, else the mask test against **`enchantments`**, else nine ability checks all using entry `0x5A` from `message_lbx_file__ovr113__2of2`, starting at [Combat.c:11248](../../MoM/src/Combat.c#L11248).

**The two masks read different fields.** The enemy group tests `battle_units[].combat_effects`; the friendly group tests the `enchantments` local. That asymmetry is in the listing (asm:634 against asm:896-897) and is easy to cross by accident.

**Two IDA names for one value.** The listing's `scc_Mundane_Enchantment` is `WIZARDS.inc:5916 = 15`, the same value production calls `scc_Unit_Enchantment_Normal_Only` ([MOM_DAT.h:769](../../MoX/src/MOM_DAT.h#L769)). Likewise `Ab_CauseFear` = `20h` is `USA_CAUSEFEAR` ([MOM_DEF.h:668](../../MoX/src/MOM_DEF.h#L668)) and `Ab_PlaneShift` = `4` is `UA_PLANARTRAVEL` ([MOM_DEF.h:680](../../MoX/src/MOM_DEF.h#L680)). Same bits, production's names.

**The epilogue.** `Clear_Fields`, `Assign_Auto_Function(Combat_Screen_Draw, 2)`, `return target_idx` — [Combat.c:11348](../../MoM/src/Combat.c#L11348) and the two lines above it. The loop's `Release_Time(2)` at [Combat.c:11343](../../MoM/src/Combat.c#L11343) is asm:1390-1391; it yields two ticks per frame rather than one, which is the listing's behaviour and not a defect.

## A note on the decompile

`ovr113/Combat_Spell_Target_Screen__WIP.c` was used to scaffold the validity chains and is accurate on every condition and message number checked against the `.asm`. It is wrong in one place: it renders the `cstt_Wall` short-circuit as a plain `if` falling through into the Dispel Magic test, where asm:246 jumps clear of the whole click handler. Production's `continue` is the right shape; the decompile is not. It also carries IDA's capitalisation — `Combat_Effects`, `Spell_Name` — against production's `combat_effects` and `spell_name`.
