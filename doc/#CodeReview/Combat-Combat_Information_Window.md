Combat-Combat_Information_Window.md

SEEALSO:  Combat-Combat_Screen.md

_combat_info_cloud_of_shadow  ==>  _combat_info_cloud_of_shadow
_combat_info_heavenly_light  ==>  _combat_info_heavenly_light
_combat_per_side_effect_rows    ==>  _combat_per_side_effect_rows

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Information_Window.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Information_Window_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Info_Effects_Count.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Info_Effects.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\Combat_Info_Effects_Base.asm

Combat_Information_Window()
    |-> Combat_Info_Effects_Count()
    |-> Combat_Information_Window_Draw()
        |-> Combat_Info_Effects_Count()
    |-> Combat_Info_Effects()
        |-> Combat_Info_Effects_Base()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-25).** Five functions of `ovr103`, 1,197 asm lines, **all five walked end to end and all five faithful.** Two findings were raised and both are fixed. Four of the five were homeless before this doc; `Combat_Info_Effects_Count` transferred in from [Combat-Combat_Screen.md](Combat-Combat_Screen.md).

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Information_Window_Draw` | [Combat.c:5961](../../MoM/src/Combat.c#L5961) | `ovr103/Combat_Information_Window_Draw.asm` | 328 | **full body walk — faithful** |
| `Combat_Information_Window` | [Combat.c:5851](../../MoM/src/Combat.c#L5851) | `ovr103/Combat_Information_Window.asm` | 288 | **full body walk — faithful** |
| `Combat_Info_Effects_Base` | [Combat.c:6154](../../MoM/src/Combat.c#L6154) | `ovr103/Combat_Info_Effects_Base.asm` | 271 | **full body walk — faithful** |
| `Combat_Info_Effects` | [Combat.c:6079](../../MoM/src/Combat.c#L6079) | `ovr103/Combat_Info_Effects.asm` | 171 | **full body walk — faithful** |
| `Combat_Info_Effects_Count` | [Combat.c:6255](../../MoM/src/Combat.c#L6255) | `ovr103/Combat_Info_Effects_Count.asm` | 139 | **full body walk — faithful** |

## What belongs here, and why

This is the **combat information window**: the ESC-dismissable overlay listing every global and battlefield enchantment in force, split between the two combatants. `Combat_Information_Window` owns the screen - allocates the icon set and the fourteen info-item records, computes the window geometry, then runs an input loop that redraws through `Combat_Information_Window_Draw` until ESC. The two `Combat_Info_Effects*` functions fill the item list: `_Base` handles everything tied to the battlefield itself (node dispel, node aura, city-siege enchantments, and the two global enchantments scanned across all players), and `Combat_Info_Effects` adds the per-wizard globals for each of the two combatants.

**`Combat_Info_Effects_Count` transferred in.** It previously carried a scope-table row and a walked section in [Combat-Combat_Screen.md](Combat-Combat_Screen.md), because the combat screen's layout path calls it too. It belongs here: it counts exactly what `Combat_Info_Effects` and `Combat_Info_Effects_Base` build, over the same globals in the same order, and reading it beside them is the only way to see that. It was re-walked for this review rather than inheriting the earlier verdict; the result agrees.

### What does *not* belong here

- The rest of `ovr103` - `BU_CreateImage`, `CMB_LoadResources`, `Next_Battle_Unit`, `Move_Confused`, `Combat_Node_type` and the two `zCombat_Cast_Spell_*` stubs. Overlay membership is not the boundary; none of them touch the info window.

## Findings

None outstanding. Two were raised during the walk and both are fixed: the Charm of Life block in `Combat_Info_Effects` filled its info item with Crusade's icon, help index and name; and both global-enchantment loops in `Combat_Info_Effects_Base` were missing the early exit the listing performs.

## Deviations

### D1 - one reused stack slot is split into five named locals

The listing gives `Combat_Information_Window_Draw` a single word at `bp-2`, and IDA names it `mid_y2__bot_y1__bot_start_y` because it holds three different values at three points; the DI register does the same duty twice more. [Combat.c:5969-5973](../../MoM/src/Combat.c#L5969-L5973) declares five separate locals - `top_y2`, `mid_start_y`, `mid_y2`, `bot_y1`, `bot_start_y` - each commented with which of the originals it stands in for.

Recorded rather than proposed for change. Register and slot sharing is not evidence of how many variables the source declared, the arithmetic is identical either way, and the reuse is documented in place. The same function keeps its superseded single-local version commented out directly above the split, which is the house convention.

## Verified faithful

### `Combat_Info_Effects_Count` ([Combat.c:6255](../../MoM/src/Combat.c#L6255), asm 139)

Faithful, all 139 lines. **No stack frame at all** - `push bp` / `mov bp, sp` with no `sub sp` - and production's four locals are all register-resident, `battle_effects_count` in CX, `player_idx` in DI, and `itr_combatants`/`itr_players` sharing SI at different points.

It counts what the other two build, in the same order: the four per-wizard globals across both combatants (asm:21-47), the central-structure node test, the `_combat_node_type` test, the city-siege pair, then the two all-player global scans.

**Both three-way tests are `||` chains that converge on one increment.** asm:55-69 sends `CS_SorceryNode`, `CS_ChaosNode` and `CS_NatureNode` to the same `inc` through two intermediate `jmp`s, and asm:71-85 does the same for node type 0, 1 and 2. [Combat.c:6287-6303](../../MoM/src/Combat.c#L6287-L6303) writes both as `||`.

**The node chain shows the `cnt_` label swap again and is again correct.** asm:74 tests `cnt_Chaos` and asm:76 tests `cnt_Nature`, which under IDA's enum are values 1 and 2; production tests `cnt_Sorcery`, `cnt_Nature`, `cnt_Chaos`, which under its own corrected enum are 0, 1, 2. Both sides cover exactly {0, 1, 2}, so the set is identical and only the spelling differs.

**It already has the early exits that R2 was about.** asm:109 `jmp short loc_847B6` and asm:126 `jmp short @@Done` leave their loops on the first hit, and [Combat.c:6322](../../MoM/src/Combat.c#L6322) and [Combat.c:6330](../../MoM/src/Combat.c#L6330) both carry `break;`. That is worth knowing beside `Combat_Info_Effects_Base`, which performs the identical scan: the two functions must agree or the window is sized for a different number of rows than it draws.

**The `OGBUG: missing Evil Omens` note at [Combat.c:6254](../../MoM/src/Combat.c#L6254) is faithful, and it is not this function's alone.** Neither listing mentions Evil Omens - `Combat_Info_Effects_Base` does not scan for it either, so the enchantment is absent from both the tally and the item list. The note would read better on both functions, or on neither with a single note at the cohort level.

### `Combat_Information_Window` ([Combat.c:5851](../../MoM/src/Combat.c#L5851), asm 288)

Faithful, all 288 lines.

**Frame - order matches.** `full_screen_esc_field` `-6`, `input_field_idx` `-4`, `leave_screen` `-2`, with `sub sp, 6`; [Combat.c:5853-5855](../../MoM/src/Combat.c#L5853-L5855) declares those three in that order, and `itr`/`window_height` are register locals.

**The two allocation sizes are exact.** asm:54-56 is `Near_Allocate_First(80)` for `GUI_String_1`; asm:62-64 is `Near_Allocate_Next(30)` fourteen times. Production writes the second as `sizeof(struct s_CMB_InfoItem)` at [Combat.c:5870](../../MoM/src/Combat.c#L5870) - the struct is 2 + 2 + 26 = **30** bytes ([Combat.h:1188](../../MoM/src/Combat.h#L1188)), and IDA's `struc s_CMB_InfoItem` carries `sizeof=0x1E` to match. The literal-30 version is kept commented directly above it.

**The geometry arithmetic matches term for term.** `((count / 2) * 19) + 9` else `4` (asm:145-156, the `cwd`/`sub`/`sar` signed halve); then `+ (_combat_per_side_effect_rows * 20) + start_y` with `+ 21` only on the positive branch (asm:160-175); then `start_y = 100 - (offset / 2)` (asm:178-183). [Combat.c:5904-5922](../../MoM/src/Combat.c#L5904-L5922).

**Both bounded loops match their constants:** fourteen icon reloads from LBX slots 42..55 (asm:114-129), and twenty help entries cleared to `ST_UNDEFINED`/0/0/0/0 (asm:198-223).

### `Combat_Information_Window_Draw` ([Combat.c:5961](../../MoM/src/Combat.c#L5961), asm 328)

Faithful, all 328 lines - see D1 for the local split.

**Frame - order matches.** `in_per_side_section` `-0Eh`, `colors` `-0Ch` (four bytes), `section_top_offset` `-8`, `cell_y` `-6`, `cell_x` `-4`, the shared slot `-2`; `sub sp, 0Eh` reconciles at fourteen bytes. [Combat.c:5963-5967](../../MoM/src/Combat.c#L5963-L5967) plus the D1 group.

**Three windowed draws, in order:** top strip clipped to `(0, 0, 319, top_y2)`, middle to `(0, 0, 319, mid_y2)`, bottom to `(0, bot_y1, 319, 199)` - and the bottom's `Set_Window` is skipped entirely when `_combat_per_side_effect_rows > 0`, which is the `jg`/`Reset_Window` split at asm:78-93. [Combat.c:5979-6010](../../MoM/src/Combat.c#L5979-L6010).

**The font setup is four calls with exact constants** - outline 254, `Set_Font_Colors_15(2, &colors[0])` with `colors[0] = 250` and `colors[1] = 177`, shadow-down `(2, 15, 0, 0)`, spacing width 2. asm:100-125, [Combat.c:6012-6015](../../MoM/src/Combat.c#L6012-L6015).

**`section_top_offset` takes four different values** depending on where it is set: `15 + (count / 2) * 19` or `11` for the side headings (asm:131-143), then `10` or `26` for the item grid (asm:197-202), then `31` when the loop crosses into the per-side section. All four reproduced at [Combat.c:6022-6044](../../MoM/src/Combat.c#L6022-L6044) and [Combat.c:6058](../../MoM/src/Combat.c#L6058).

**The item loop is bounded by a computed expression**, `((_combat_per_side_effect_rows / 2) + _combat_info_item_count) > cell_idx` (asm:314-320), and the `in_per_side_section` flip needs all three of `itr >= count`, `count > 0` and `Drawing_Sides == 0` (asm:208-215). [Combat.c:6047-6058](../../MoM/src/Combat.c#L6047-L6058). Cell placement is `start_x + (itr % 2) * 112 + 11` across and `start_y + section_top_offset + (itr / 2) * 19` down.

### `Combat_Info_Effects_Base` ([Combat.c:6154](../../MoM/src/Combat.c#L6154), asm 271)

Faithful apart from R2. No stack frame at all - `itr` and `idx` are both register locals, matching the listing.

**All ten icon indices check out**, read out of the listing as byte offsets and halved: node dispel Sorcery `+18h`→12, Chaos `+0Ah`→5, Nature `+14h`→10; node aura Sorcery `+1Ah`→13, `+16h`→11, `+0Ch`→6; Cloud of Darkness `+10h`→8, Holy Light `+4`→2, Chaos Surge `+8`→4, Eternal Night `+0Eh`→7. Every one matches [Combat.c:6165-6237](../../MoM/src/Combat.c#L6165-L6237).

**The node-aura block looks swapped in the listing and is not.** asm:81-84 pairs `cnt_Chaos` with the Nature aura and `cnt_Nature` with the Chaos aura - because IDA's enum has those two backwards (`WIZARDS.inc`: `cnt_Chaos = 1`, `cnt_Nature = 2`). Production redefines `cnt_` with the **correct** values at [MOM_DAT.h:834-836](../../MoX/src/MOM_DAT.h#L834-L836) - `cnt_Nature = 1`, `cnt_Chaos = 2` - so [Combat.c:6184-6203](../../MoM/src/Combat.c#L6184-L6203) pairs value 1 with the Nature aura and value 2 with the Chaos aura, matching the listing **by value**. This is the documented `cnt_` trap; the reconstruction already navigates it, and there is no defect here.

**The tail pads to an even count**: if `idx` is odd, one more item gets `icon_seg = ST_UNDEFINED` and `idx` advances, then `_combat_info_item_count = idx` (asm:259-266, [Combat.c:6244-6249](../../MoM/src/Combat.c#L6244-L6249)). The window's two-column layout depends on that.

### `Combat_Info_Effects` ([Combat.c:6079](../../MoM/src/Combat.c#L6079), asm 171)

Faithful apart from R1.

**Frame - order matches.** `computer_player_battle_effect_count` `-4`, `itr_combatants` `-2`; [Combat.c:6081-6082](../../MoM/src/Combat.c#L6081-L6082). `info_common_count` is SI and `player_idx` is DI, both named in the listing's own register aliases.

**The two-combatant loop runs the AI player first, then the human.** `player_idx` starts at `_combat_ai_player` (asm:18) and is unconditionally reassigned to `_human_player_idx` at the bottom of each pass (asm:140), with `info_common_count` reset to `_combat_info_item_count + 1` - the odd-column start. The AI's running total is captured on the first pass only, `itr_combatants == 0` (asm:144-147). [Combat.c:6089-6131](../../MoM/src/Combat.c#L6089-L6131).

**`_combat_per_side_effect_rows` takes the larger of the two sides**: `(total - ai_count) > ai_count ? (total - ai_count) : ai_count` (asm:155-165), which [Combat.c:6134-6141](../../MoM/src/Combat.c#L6134-L6141) writes out as the same if/else.

**The `< NEUTRAL_PLAYER_IDX` guard is a long-branch fixup** - asm:22-24 is `jl short` to the body with a `jmp` past it, so the surviving `jl` reads un-inverted. [Combat.c:6092](../../MoM/src/Combat.c#L6092).
