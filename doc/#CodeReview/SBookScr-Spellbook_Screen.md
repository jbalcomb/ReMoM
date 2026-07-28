SBookScr-Spellbook_Screen.md

SmlBook_PageTurn__WIP()     ==>  SmlBook_PageTurn()
SmlBook_Compose__WIP()      ==>  SmlBook_Compose()

SBK_Dogears  ==>  g_spellbook_mode
SBK_OpenPage  ==>  g_spellbook_left_page
SBK_SomePageSaveVar  ==>  g_spellbook_last_left_page
SBK_BookManaLimit  ==>  g_spellbook_cast_mana_limit

OVL_DisableIncmBlink()  ==>  Main_Screen_Disable_Income_Warning()
OVL_EnableIncmBlink()   ==>  Main_Screen_Enable_Income_Warning()

Review targets:
- Spellbook_Screen           (WIZARDS ovr134 o134p03)
- Spell_Screen_Load          (WIZARDS ovr134 o134p01)
- Spellbook_Screen_Draw      (WIZARDS ovr134 o134p02)
- Questionable_Spell_Cast    (WIZARDS ovr134 o134p05 — asm proc: IDK_Check_Spell_Cast)
- SmlBook_Compose            (WIZARDS ovr117 o117p07)
- SmlBook_PageTurn           (WIZARDS ovr117 o117p11 — drake178: SBK_PageFlip_Small)

Ground truth (IDA Pro 5.5 disassembly — the authority):
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr134\Spellbook_Screen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr134\Spell_Screen_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr134\Spellbook_Screen_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr134\IDK_Check_Spell_Cast.asm   (production renamed the proc to Questionable_Spell_Cast)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\SmlBook_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\SmlBook_Compose__WIP.asm   (production dropped __WIP → SmlBook_Compose)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\SmlBook_PageTurn__WIP.asm  (production dropped __WIP → SmlBook_PageTurn)
(the sibling `.c` files are Gemini translations of the .asm — second opinions, NOT ground truth)

Spellbook_Screen()
    |-> Spell_Screen_Load()
    |-> Questionable_Spell_Cast()
    |-> SmlBook_PageTurn()
        |-> SmlBook_Compose()
    |-> Spellbook_Screen_Draw()
        |-> SmlBook_Draw()
        |-> SmlBook_Compose()

---

# `Spellbook_Screen` + supporting routines — Review / Walkthrough

The **Small Spellbook** — the overland casting book reached from the Main Screen "Spells" button. `Spellbook_Screen` owns the screen: it loads the two-page book art, builds the human player's overland spell list, runs the input loop (spell-field clicks, ESC, page-forward `F`, page-backward `B`), handles the abort-a-cast confirmation, and on exit either dispatches the selected spell to `Cast_Spell_Overland_Do` or dissolves back to the Main Screen. `Spell_Screen_Load` paints the static backdrop, `Spellbook_Screen_Draw` is the per-frame auto-draw callback (delegating to `SmlBook_Draw`), `SmlBook_Compose` composes one page's text/cost/turn-icons into a work bitmap, `SmlBook_PageTurn` is the 4-stage shear/scale page-flip animation, and `Questionable_Spell_Cast` is the pre-cast "are you sure? this cast will be wasted" gate for spells with no valid target or a redundant effect.

| Function | Location | asm | Role |
|---|---|---|---|
| `Spell_Screen_Load` | [SBookScr.c:117-128](../../MoM/src/SBookScr.c#L117-L128) | `Spell_Screen_Load.asm` (WZD 134p01) | Backdrop: reduced map, main-screen redraw, gradient fill, book FLIC, load help table. |
| `Spellbook_Screen_Draw` | [SBookScr.c:132-136](../../MoM/src/SBookScr.c#L132-L136) | `Spellbook_Screen_Draw.asm` (WZD 134p02) | Per-frame auto-draw: `Copy_Back_To_Off` then `SmlBook_Draw(16,10)`. |
| `Spellbook_Screen` | [SBookScr.c:140-418](../../MoM/src/SBookScr.c#L140-L418) | `Spellbook_Screen.asm` (WZD 134p03) | Screen owner: build book, locate casting page, per-frame help, input loop, abort/cast dispatch. |
| `Questionable_Spell_Cast` | [SBookScr.c:605-703](../../MoM/src/SBookScr.c#L605-L703) | `IDK_Check_Spell_Cast.asm` (WZD 134p05) | Six wasted/redundant-cast confirmation gates before a spell is committed. |
| `SmlBook_Compose` | [Spellbook.c:1118-1270](../../MoM/src/Spellbook.c#L1118-L1270) | `SmlBook_Compose__WIP.asm` (WZD 117p07) | Compose one small-book page (name, MP cost, turns-to-cast icon row) into a bitmap. |
| `SmlBook_PageTurn` | [Spellbook.c:1630-1831](../../MoM/src/Spellbook.c#L1630-L1831) | `SmlBook_PageTurn__WIP.asm` (WZD 117p11) | 4-stage shear/scale page-flip animation for the small/combat book. |

> **Status: DONE-DONE — all six faithful to the asm 1:1.** Cross-checked phase-for-phase and arg-for-arg against the disassembly; no open items (S1 fixed), no OG-faithful quirks (B-items). Three reconstruction errors were found and corrected (below); builds clean (MSVC-debug). `Questionable_Spell_Cast` is the production rename of the asm's `IDK_Check_Spell_Cast` (the asm proc carries no drake178 alias); `SmlBook_Compose`/`SmlBook_PageTurn` dropped their `__WIP` suffixes on completion.

## Reconstruction fixes applied (found while exercising this screen)

Three reconstruction errors — production diverged from the disassembly — surfaced while getting the small-book page-turn and the spell context-help to render correctly. Two are in shared primitives the reviewed code depends on. All corrected to match the asm (marked `/* CLAUDE */` in-code):

| Fix | File | Divergence → correction | asm authority |
|---|---|---|---|
| `Reduce_Bitmap` vertical resample | [FLIC_Draw.c:3869](../../MoX/src/FLIC_Draw.c#L3869) | `Vert_Remainder` never masked → added `Vert_Remainder &= 0x00FF;` (the asm's `xor bh,bh`). Without it `src` ran away and punched transparent holes → magenta (small) / dark (big) specks in **both** page-turns; isolated with a synthetic `Scale_Bitmap`-on-opaque harness. | `seg031/Reduce_Bitmap.asm` `loc_232B6` |
| `SmlBook_PageTurn` block-3 first shear | [Spellbook.c:1799](../../MoM/src/Spellbook.c#L1799) | `Shear_Bitmap_Y(0, 0, 9, -25, …)` → `shear1 = -16`; the innermost fold's top columns weren't lifted into the curl. | `ovr117/SmlBook_PageTurn__WIP.asm` `loc_9C443` |
| `Print_String` + `Print_String_To_Bitmap` CR/`0x14` handler | [Fonts.c:964](../../MoX/src/Fonts.c#L964) | `print_xpos += x;` → `print_xpos = x;` (asm `mov [g_VGA_TextDraw_X], [Left]` — SET, not add). The `+=` drifted CR/`0x14` lines rightward, so the `0x14`-separated spell-help stat fields (Target/Casting Cost/Upkeep) redrew at growing X — "printed multiple times at different coordinates". | `seg017/Print_String.asm` `loc_19C48`, `Print_String_To_Bitmap.asm` |

## Phase map (asm ↔ production)

### `Spell_Screen_Load` — faithful
| Phase | asm | production | fidelity |
|---|---|---|---|
| `Allocate_Reduced_Map`, `Set_Page_Off`, `Reset_Map_Draw`, `Main_Screen_Draw` | 6-12 | [119-122](../../MoM/src/SBookScr.c#L119-L122) | faithful |
| `Gradient_Fill(0,0,319,199,3)` | 14-31 | [123](../../MoM/src/SBookScr.c#L123) | faithful (+4 ST_NULL platform args — see notes) |
| `FLIC_Draw(16,10,_spellbook_small_seg)`, `Copy_Off_To_Back`, `Set_Page_On` | 32-45 | [124-126](../../MoM/src/SBookScr.c#L124-L126) | faithful |
| `LBX_Load_Data_Static(hlpentry,0,_help_entries,0,15,10)` | 46-59 | [127](../../MoM/src/SBookScr.c#L127) | faithful ("Spell Book Help") |

### `Spellbook_Screen_Draw` — faithful
| Phase | asm | production | fidelity |
|---|---|---|---|
| `Copy_Back_To_Off`; `SmlBook_Draw__WIP(16,10)` | 5-12 | [134-135](../../MoM/src/SBookScr.c#L134-L135) | faithful (`SmlBook_Draw__WIP`→`SmlBook_Draw`) |

### `Spellbook_Screen` — faithful
| Phase | asm | production | fidelity |
|---|---|---|---|
| `did_select=FALSE`, `Main_Screen_Disable_Income_Warning`, palette setup | 24-45 | [160-166](../../MoM/src/SBookScr.c#L160-L166) | faithful |
| `Spell_Screen_Load`; `Build_Spellbook(slt_Overland,6)`; `g_spellbook_mode=1` | 46-55 | [167-169](../../MoM/src/SBookScr.c#L167-L169) | faithful |
| locate casting-spell page → `g_spellbook_left_page` (odd→−1); else `g_spellbook_last_left_page` | `loc_B8BB3`-`loc_B8C3C` | [170-195](../../MoM/src/SBookScr.c#L170-L195) | faithful |
| `Assign_Auto_Function(Spellbook_Screen_Draw,2)`, fonts, `Clear_Fields`, `Set_Help_List(15)` | `loc_B8C44`-149 | [196-200](../../MoM/src/SBookScr.c#L196-L200) | faithful |
| add 6 left + 6 right spell fields, ESC/F/B fields | `loc_B8C8A`-289 | [203-213](../../MoM/src/SBookScr.c#L203-L213) | faithful (left edge = literal `137`) |
| per-frame left (`help[3+itr]`) + right (`help[9+itr]`) `_help_entries` population, `HLP_PAGE_BACK`→`HLP_SPELL_OF_RETURN` | `loc_B8DBB`-`loc_B8EED` | [218-253](../../MoM/src/SBookScr.c#L218-L253) | faithful |
| ESC → click + leave; `F`/`B` → `Play_Left_Click__DUPE`, `SmlBook_PageTurn`, `g_spellbook_left_page ±= 2` | `loc_B8EED`-`loc_B8F59` | [257-285](../../MoM/src/SBookScr.c#L257-L285) | faithful |
| left(<6)/right(≥6) field click, abort-current-cast confirm, `Questionable_Spell_Cast`, set cast | `loc_B8F5E`-`loc_B91DD` | [289-378](../../MoM/src/SBookScr.c#L289-L378) | faithful |
| re-`Assign_Auto_Function`; if not leaving, rebuild fields + `Set_Page_Off`/draw/`PageFlip_FX` | `loc_B91E6`-`loc_B9347` | [382-402](../../MoM/src/SBookScr.c#L382-L402) | faithful |
| teardown: `current_screen`, save page, `Clear_Fields`, income-warning off, help off; cast-or-dissolve | `@@LeaveScreen`-`@@Done` | [404-417](../../MoM/src/SBookScr.c#L404-L417) | faithful |

### `Questionable_Spell_Cast` — faithful
Reads `*confirm_result` in, runs six wasted/redundant-cast gates, writes the (possibly re-prompted or vetoed) flag back out. Checks 1-5 re-prompt via `Confirmation_Box`; check 6 hard-vetoes via `Warn0` + `confirm = 0`. Production swaps the asm's raw `.type`/spell-id literals for the named enums (verified equal: `scc_Target_Wizard`=6, `scc_Infusable_Spell`=0x11, `scc_Global_Enchantment`=9, `spl_Holy_Weapon`=124).

| Check | asm | production | fidelity |
|---|---|---|---|
| Plane Shift vs any Planar Seal → msg 12 | `loc_B9859`-`loc_B98AA` | [615-630](../../MoM/src/SBookScr.c#L615-L630) | faithful |
| Incarnation/Summon Hero/Champion vs no open slot → msg 13 | `loc_B98AA`-`loc_B98F8` | [632-640](../../MoM/src/SBookScr.c#L632-L640) | faithful |
| Disjunction(True)/Spell Binding vs no enemy overland ench → msg 14 | `loc_B98F8`-`loc_B9983` | [642-661](../../MoM/src/SBookScr.c#L642-L661) | faithful |
| `scc_Target_Wizard`/`scc_Infusable_Spell` vs no contacted enemy w/ active fortress → msg 15 | `loc_B9983`-`loc_B9A11` | [663-681](../../MoM/src/SBookScr.c#L663-L681) | faithful (`.Contacted[player_idx]`, `_players[0]` base) |
| `scc_Global_Enchantment` already active (`.oe_idx` into Globals) → msg 16 | `loc_B9A11`-`loc_B9A7B` | [683-691](../../MoM/src/SBookScr.c#L683-L691) | faithful |
| `spl_Holy_Weapon` + Holy Arms active → `Warn0`, force `confirm=0` | `loc_B9A7B`-`loc_B9AB5` | [693-701](../../MoM/src/SBookScr.c#L693-L701) | faithful |

### `SmlBook_Compose` — faithful
| Phase | asm | production | fidelity |
|---|---|---|---|
| `Draw_Picture_To_Bitmap(_spellbook_small_text,...)`; `for(itr1 < 6)` | 21-27 | [1130-1133](../../MoM/src/Spellbook.c#L1130-L1133) | faithful |
| per-spell colors, `casting_cost` (remaining if casting else `Casting_Cost`), `Turns_To_Cast` clamp 40 | `loc_B9096`-`loc_9B119` | [1135-1168](../../MoM/src/Spellbook.c#L1135-L1168) | faithful |
| `Turns_To_Cast<1`: Instant/Special (`type!=11`→Instant) + realm icon | `loc_9B122`-`loc_9B25F` | [1170-1191](../../MoM/src/Spellbook.c#L1170-L1191) | faithful |
| `Turns_To_Cast≥1`: clear region, icon row (wrap at 20 → second line y+13) | `loc_9B25F`-`loc_9B379` | [1192-1226](../../MoM/src/Spellbook.c#L1192-L1226) | faithful |
| name colors (198 if casting else 184/187), print name, MP-cost right-aligned (`?`/`N MP`) | `loc_9B379`-`loc_9B512` | [1228-1262](../../MoM/src/Spellbook.c#L1228-L1262) | faithful |
| empty-slot branch: clear full row | `loc_9B512` | [1264-1266](../../MoM/src/Spellbook.c#L1264-L1266) | faithful |

### `SmlBook_PageTurn` — faithful
| Phase | asm | production | fidelity |
|---|---|---|---|
| widths `{85,55,55,85}`, `Deactivate_Auto_Function`, `Open_File_Animation__HACK(book,0)`, `Allocate_Reduced_Map` (non-combat) | 20-38 | [1632-1652](../../MoM/src/Spellbook.c#L1632-L1652) | faithful (`__HACK`) |
| `Mark_Block`, alloc 1040 page bitmap + 1400 turning bitmap, decode 6 realm icons, `Cycle_Palette_Color` | `loc_9BE1A`-99 | [1653-1662](../../MoM/src/Spellbook.c#L1653-L1662) | faithful |
| stage loop `0..3`: left bg page (`+0`/`−2`), left dog-ear, compose, `Draw_Picture(+16,+21)`, title | `loc_9BEB8`-`loc_9BFC7` | [1663-1693](../../MoM/src/Spellbook.c#L1663-L1693) | faithful |
| right bg page (`+3`/`+1`), right dog-ear, compose, `Draw_Picture(+148,+21)`, title-if-category-differs | `loc_9BFC7`-`loc_9C0ED` | [1694-1724](../../MoM/src/Spellbook.c#L1694-L1724) | faithful |
| `Set_File_Animation_Frame(3−stage)` (backward), `Draw_File_Animation__HACK`, turning-page index | `loc_9C0ED`-`loc_9C133` | [1725-1751](../../MoM/src/Spellbook.c#L1725-L1751) | faithful (`__HACK`) |
| compose turning page, `Create_Picture(122,175)`, `Clipped_Copy_Bitmap(0,48,…)`, title-on-turning-page | `loc_9C133`-`loc_9C248` | [1752-1773](../../MoM/src/Spellbook.c#L1752-L1773) | faithful |
| `Scale_Bitmap(…,Stage_Text_Widths[stage],100)`, four stage-keyed `Shear_Bitmap_Y` cascades + `Draw_Picture_Windowed` | `loc_9C248`-`loc_9C5D0` | [1774-1817](../../MoM/src/Spellbook.c#L1774-L1817) | faithful (block-3 first shear `shear1=-16` — see fixes) |
| `PageFlip_FX`; after loop `Release_Block`, re-`Assign_Auto_Function` (combat/overland) | `loc_9C5D0`-`@@Done` | [1818-1830](../../MoM/src/Spellbook.c#L1818-L1830) | faithful |

## Platform notes (accepted — consistent with prior reviews)

- **`Gradient_Fill`** — asm is 5-arg (`x0,y0,x1,y1,3`, `add sp,0Ah`); production is 9-arg with four trailing `ST_NULL` (`remap_block, Slope, Scale, Seed`) at [SBookScr.c:123](../../MoM/src/SBookScr.c#L123) — the expanded ReMoM signature, same as the padded `Load_Palette`/`Play_Sound` conventions elsewhere.
- **Renamed platform reimplementations** (same call/args): `SmlBook_Draw__WIP`→`SmlBook_Draw`; `Open_File_Animation`→`Open_File_Animation__HACK` and `Draw_File_Animation`→`Draw_File_Animation__HACK` (both in `SmlBook_PageTurn`, matching `BigBook_PageTurn`); `Cycle_Palette_Color__STUB`→`Cycle_Palette_Color`.
- **`SmlBook_Compose` vestigial 3rd param** — the asm frame declares `Caster_ID = word ptr 26h` but never reads it; the small-book call site pushes only 2 args (`add sp, 20h`). Production's 2-arg signature `(spell_book_page, spellbook_bitmap)` is faithful to the actual behavior (the phantom param is correctly omitted, unlike the shared `CmbBook_Compose__WIP`, which does take `caster_idx`).
- **`Shear_Bitmap_Y` arg order** — signature is `(x1, shear1, x2, shear2, byte_ptr bitmap)` ([FLIC_Draw.h:210](../../MoX/src/FLIC_Draw.h#L210)); the asm pushes the bitmap first (so it is the last C arg). `SmlBook_PageTurn` and `BigBook_PageTurn` both pass it last — matching.
- **`Play_Left_Click__DUPE`** — the ESC/`F`/`B` paths use the `__DUPE` click helper; the spell-field click uses `Play_Left_Click`. The two are byte-identical OG duplicates (drake178: `RP_SND_LeftClickSound2`, "should not exist"); which copy a site calls is an OG link artifact — preserved as distinct symbols for fidelity.

## Related references

- Callers / dispatch: `Spellbook_Screen` is the human overland spell-select entry; its exit calls `Cast_Spell_Overland_Do` ([SBookScr.c:448](../../MoM/src/SBookScr.c#L448)) or `Dissolve_Main_Screen`. `Spellbook_Screen_Draw` is registered via `Assign_Auto_Function` and also called directly by `SmlBook_PageTurn` teardown.
- Siblings: `SmlBook_Draw` ([Spellbook.c:1496](../../MoM/src/Spellbook.c#L1496)) draws the two composed pages; `BigBook_PageTurn` ([Spellbook.c:2112](../../MoM/src/Spellbook.c#L2112)) is the big-book twin of `SmlBook_PageTurn`; `CmbBook_Compose__WIP`/`CmbBook_Draw__WIP` are the combat-book counterparts.
- `…\ovr134\*.asm` + `…\ovr117\*.asm` — IDA Pro 5.5 disassembly (**the authority**); the sibling `.c` files are Gemini second opinions.

---
