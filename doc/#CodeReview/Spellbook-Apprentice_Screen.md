Spellbook-Apprentice_Screen.md

SBK_LoadSpellDescs__WIP()  ==>  BigBook_Load_Spell_Descriptions()

SBK_Candidate_Page      ==>  g_first_research_page
SBK_Research_Incomes[]  ==>  g_research_income_by_realm 
SBK_Descriptions[]      ==>  g_spellbook_descriptions

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr076\Advisor_Screen.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Apprentice_Screen__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\SBK_LoadSpellDescs__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\BigBook_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\BigBook_Compose__WIP.asm

Advisor_Screen()
    case 2:
        Apprentice_Screen()
            |-> BigBook_Load_Spell_Descriptions()
            |-> BigBook_PageTurn()
            |-> BigBook_Draw()
                |-> BigBook_Compose()

---


# 1:1 Fidelity Review

**Status: DONE-DONE.** Nine reconstruction errors (R1-R9) found and fixed across the five in-scope functions. One in-scope OGBUG (per-realm research income, BigBook_Compose) verified and annotated; preserved.

Ground truth = IDA Pro 5.5 disassembly (ovr118 `.asm`). Gemini `.c` files are second opinions, not OG truth. Every finding verified against the asm bytes.

## Review status

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| Apprentice_Screen | Spellbook.c:2506 | ovr118/Apprentice_Screen__WIP.asm | faithful; R1, R2, R3 fixed |
| BigBook_Draw | Spellbook.c:1971 | ovr118/BigBook_Draw.asm | faithful |
| BigBook_Compose | Spellbook.c:2635 | ovr118/BigBook_Compose__WIP.asm | faithful; R4, R5, R6 fixed |
| BigBook_PageTurn | Spellbook.c:1810 | ovr118/BigBook_PageTurn.asm | faithful; R7, R8 fixed |
| BigBook_Load_Spell_Descriptions (SBK_LoadSpellDescs__WIP) | Spellbook.c:2450 | ovr118/SBK_LoadSpellDescs__WIP.asm | faithful; R9 fixed |

Out of scope (separate ovr118 features, not the Apprentice/BigBook display): Learn_Spell_Animation (+ _Load/_Draw/Fill_Black_Draw) is the spell-learned fanfare; Spell_Research_Select is the research picker; Check_Research_Spell_Is_Known is a research helper; SBK_DrawSpellDecode is part of the spell-decode path.

## Apprentice_Screen (Spellbook.c:2506)

Structure faithful (Build_Spellbook(slt_Library, 4); g_spellbook_mode=2; BIGBOOK FLIC load; 16x g_spellbook_descriptions alloc; BigBook_Load_Spell_Descriptions; Assign_Auto_Function(BigBook_Draw, 2); three Add_Hidden_Field; input loop with dogear paging that shifts g_spellbook_descriptions[8+i]->[i], BigBook_PageTurn, BigBook_Draw; teardown g_first_research_page = page). Screen-bound ESC field verified: (SCREEN_XMIN,25,SCREEN_XMAX,SCREEN_YMAX) = (0,25,319,199) matches asm. Three reconstruction errors:

**R1 — missing `Reset_Cycle_Palette_Color()` at function start.**
ASM: `call Reset_Cycle_Palette_Color` is the first call, before `Build_Spellbook`. Production jumps straight to `Build_Spellbook(slt_Library, 4)` (Spellbook.c:2514). Fix: add `Reset_Cycle_Palette_Color();` as the first statement.

**R2 — left-page help loop garbled `m_spellbook_pages` indexing (Spellbook.c:2543-2561).**
ASM loc_9DF1E uses `m_spellbook_pages[g_spellbook_left_page].spell[itr]` for all three accesses (spell-check, Spell_Of_Return check, help_idx assignment). Production instead has:
- spell-check (2546): `m_spellbook_pages[(g_spellbook_left_page + itr)].spell[itr]`
- abs/Spell_Of_Return (2549): `m_spellbook_pages->spell[(g_spellbook_left_page + itr)]`
- assignment (2555): `m_spellbook_pages->spell[((g_spellbook_left_page + 0) + itr)]`
Three different wrong forms; none equal the asm's `m_spellbook_pages[g_spellbook_left_page].spell[itr]` (they diverge for itr>0). The count-check at 2544 is correct. Fix: use `m_spellbook_pages[g_spellbook_left_page].spell[itr]` in all three.

**R3 — right-page help loop garbled `m_spellbook_pages` indexing (Spellbook.c:2563-2583).**
ASM loc_9DFCC uses `m_spellbook_pages[g_spellbook_left_page + 1].spell[itr]`. Production has `m_spellbook_pages[((g_spellbook_left_page + 1) + itr)].spell[itr]` (2568) and `m_spellbook_pages->spell[((g_spellbook_left_page + 1) + itr)]` (2571/2577). Fix: `m_spellbook_pages[g_spellbook_left_page + 1].spell[itr]` in all three. (Also cosmetic: 2571 uses literal `214`; the left loop uses `spl_Spell_Of_Return` — same value, prefer the constant.)

## BigBook_Draw (Spellbook.c:1971)

Faithful. Anim-stage %8, `Cycle_Palette_Color(198,29,41,63,44,56,63,2)` (asm `Cycle_Palette_Color__STUB`), Mark_Block + 1800-para alloc, both corner LBX reloads (idx 7/8), Copy_Back_To_Off, the `mode>1` dogear block (guards `page != 0` and `page != count && page < count-2`), both `BigBook_Compose`/`Draw_Picture_Windowed` passes (25,-20 / 173,-20), the `mode==0` "Choose Research" text, Release_Block — all arg-for-arg.

## BigBook_Compose (Spellbook.c:2635)

Structure faithful (description_batch fold; per-spell name+desc render; casting-cost vs research-cost branch on `page < g_first_research_page`; g_spellbook_descriptions paragraph at `description_batch*4 + itr`; count/category early-returns; flag-gated title x=66/58). Three reconstruction errors:

**R4 — researching-spell name highlight color wrong (Spellbook.c:2668).** ASM loc_9E215 fills `colors[0..15] = 198`; production uses 168. Fix -> 198.

**R5 — researching-spell description highlight color wrong (Spellbook.c:2693).** ASM loc_9E303 fills `colors[0..15] = 198`; production uses 170. Fix -> 198. (ASM uses 198 for both highlight spots; production diverged to 168/170.)

**R6 — duplicate `strcat(cnst_SpaceOpenBrace4)` in casting-cost line (Spellbook.c:2726).** ASM casting-cost branch: strcat(cost) -> strcat(" (") -> itoa(turns) -> strcat(turns), one " (". Production inserts " (" a second time at 2726, yielding `...cost ( (turns`. The research branch (2763) is correct with one. Remove line 2726.

## BigBook_PageTurn (Spellbook.c:1810)

Structure faithful (stage_text_widths 90/60/60/90; Open_File_Animation(book_lbx,1); Mark_Block + 1800 alloc; corner LBX 7/8; 4-stage loop composing left+right backgrounds, the turning page, and applying one of four shear-distortion blocks; teardown Assign_Auto_Function(BigBook_Draw,2) + Release_Block). All four shear blocks match the asm arg-for-arg (coords + Draw_Picture_Windowed offsets), as do the four (direction,stage) condition combos. Two divergences from the direction-index restructuring:

**R7 — direction==1 right-background page index off by one (Spellbook.c:1853+1863).** ASM loc_9CA83: for direction==1, Page_Index = g_spellbook_left_page + 3, jumping past the `inc`. Production sets +3 (1853) then applies the unconditional `page_index++` (1863) -> +4. direction 0/666 correctly rely on the ++. Fix: direction==1 must land on +3 (e.g. line 1853 -> `g_spellbook_left_page + 2`, or skip ++ for that case).

**R8 — direction==666 stage<2 on-top page uses stale index (Spellbook.c:1877-1880).** ASM: at PageTurn_Stage < 2, any direction >= 1 (both 1 and 666) -> Page_Index = g_spellbook_left_page + 1. Production only sets it `if(direction == 1)`, so direction==666 keeps the stale right-background value (g_first_research_page + 1). Fix: set `page_index = g_spellbook_left_page + 1;` unconditionally in the stage<2 block (already inside `if(direction != 0)`).

## BigBook_Load_Spell_Descriptions / BigBook_Load_Spell_Descriptions (Spellbook.c:2450)

Faithful (clear-16 loop; two page batches — `itr = g_spellbook_left_page..+1` filling g_spellbook_descriptions[(itr-g_spellbook_left_page)*4 + count], and `itr = page..+1` filling [(itr-page+2)*4 + count]; each loads via `LBX_Load_Data_Static(desc_lbx, 0, buffer, abs(spell), 1, 110)` when `count < page.count`, else empty). One operator error:

**R9 — third loop out-of-range guard `&&` should be `||` (Spellbook.c:2490-2494).** ASM loc_9DCD9: clear branch taken when `itr < 0` OR `itr >= m_spellbook_page_count` (`or itr/jge` then `cmp/jl`). Production uses `&&`, which is never true, so the clear branch is dead and the normal branch runs for out-of-range `itr` -> OOB `m_spellbook_pages[itr]` read. The parallel second loop (2461-2465) correctly uses `||`. Fix: `||`.


---
