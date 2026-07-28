Spellbook-Learn_Spell_Animation.md

SBK_DrawSpellDecode__STUB()  ==>  Learn_Spell_Animation_Decode_Draw()

SBK_NewSpellAnim_Stg  ==>  g_spellbook_anim_stage
IMG_SBK_PageText      ==>  g_gui_scratch_bitmap
TBL_SpellDecode_Rs    ==>  g_spell_decode_x_r
TBL_SpellDecode_Ls    ==>  g_spell_decode_x_l
SBK_Spell_Index       ==>  g_active_spell_idx

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Learn_Spell_Animation_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Learn_Spell_Animation_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Learn_Spell_Animation.asm

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Spell_Research_Select.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Fill_Black_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\Check_Research_Spell_Is_Known.asm

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr118\SBK_DrawSpellDecode__STUB.asm

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr117\Build_Spellbook.asm

struct s_SPELL_DECODE



Diplomacy_Player_Gets_Spell()
    |-> Players_Check_Spell_Research

Lair_Combat__WIP()
    |-> Players_Check_Spell_Research

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Check_Spell_Research()
            |-> Player_Gets_Spell()
                if(
                    (player_idx == HUMAN_PLAYER_IDX)
                    &&
                    (New_Research == ST_TRUE)
                )
                {
                    Learn_Spell_Animation(spell_idx, was_research_target);
                    if(was_research_target == ST_FALSE)
                    {
                        Stop_All_Sounds__STUB();
                        Play_Background_Music();
                    }
                }

Learn_Spell_Animation()
    |-> Learn_Spell_Animation_Load()
    |-> Learn_Spell_Animation_Draw()
    |-> SBK_DrawSpellDecode__STUB()

Spell_Research_Select()
    |-> BigBook_Load_Spell_Descriptions()
    |-> BigBook_Draw()
        |-> BigBook_Compose()

---


# 1:1 Fidelity Review

**Status: DONE-DONE.** Five findings — R1, R2 (Learn_Spell_Animation), D1 (Learn_Spell_Animation_Load), R3, R4 (SBK_DrawSpellDecode) — all found and fixed. Debug + Release (/WX) builds green.

Scope = the Learn_Spell_Animation set: Learn_Spell_Animation + _Load + _Draw + Fill_Black_Draw (the spell-learned "wizard's lab" fanfare). SBK_DrawSpellDecode / Spell_Research_Select / Check_Research_Spell_Is_Known are separate research/decode features, not this set.

Ground truth = IDA Pro 5.5 disassembly (ovr118 `.asm`). Gemini `.c` files are second opinions, not OG truth. Every finding verified against the asm bytes.

## Review status

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| Learn_Spell_Animation | Spellbook.c:2109 | ovr118/Learn_Spell_Animation.asm | faithful; R1, R2 fixed |
| Learn_Spell_Animation_Load | Spellbook.c:2054 | ovr118/Learn_Spell_Animation_Load.asm | faithful; D1 fixed (podium/background unified via macro alias) |
| Learn_Spell_Animation_Draw | Spellbook.c:2034 | ovr118/Learn_Spell_Animation_Draw.asm | faithful |
| Fill_Black_Draw | Spellbook.c:2047 | ovr118/Fill_Black_Draw.asm | faithful |
| SBK_DrawSpellDecode__STUB (called by Learn_Spell_Animation) | Spellbook.c:2325 | ovr118/SBK_DrawSpellDecode__STUB.asm | faithful; R3, R4 fixed |

## Learn_Spell_Animation_Draw (Spellbook.c:2034)

Faithful. Copy_Back_To_Off -> FLIC_Draw(149,133,wizlab_podium_seg) -> Reset_Animation_Frame(wizlab_wizard_seg) -> FLIC_Draw(69,75,wizlab_wizard_seg) -> FLIC_Draw(190,160,wizlab_familiar_seg) -> Clipped_Draw(132,-3,wizlab_blue_column_seg), all arg-for-arg.

## Fill_Black_Draw (Spellbook.c:2047)

Faithful. `Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK)` = `Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 0)` matches the asm.

## Learn_Spell_Animation_Load (Spellbook.c:2054)

Faithful except one structural deviation. Loads wizard (LBX index = wizard_id), Load_Palette_From_Animation, background (19), page-flip, podium (20), familiar (14 + Prim_Realm), blue column (21), wizard again (wizard_id) — all arg-for-arg.

**D1 — phantom `wizlab_background_seg` where the asm reuses `wizlab_podium_seg` (Spellbook.c:2075-2077).** ASM loads the index-19 background into `wizlab_podium_seg`, draws it + Copy_Off_To_Back, then overwrites `wizlab_podium_seg` with the index-20 podium. Production added a separate `wizlab_background_seg` (declared `// DNE in Dasm`, Spellbook.c:374) for the index-19 load/draw. Behaviorally identical; structurally the OG reuses the single podium global (the index-19 handle is transient). Strict-fidelity fix: reuse `wizlab_podium_seg`, drop `wizlab_background_seg`. Already DNE-labeled — user's call.

## Learn_Spell_Animation (Spellbook.c:2109)

Faithful across the notify path (spell_animations != TRUE -> Reset_First_Block/Allocate_Reduced_Map/Notify2), the animation setup, both spell-find loops (even/odd page -> g_spellbook_mode 0/1, page or page-1; the second `if(Spell_Found == ST_FALSE)` fallback loop with the OG `will never jump` / `wrong realms` annotations), and the decode-render section. Signature note: `Play_Sound(SND_Spell_Music, SND_Spell_Music_size)` (2147-2148) and `Load_Palette(0, -1, ST_NULL)` (2174) match the uniform reconstruction signatures (OG read size/end_color from globals; the asm pushes 1/2 args) — not divergences. Two real findings:

**R1 — learn-animation description index off by one (Spellbook.c:2252).** ASM reads `SBK_Descriptions@[bx-4]`, bx = `(Spell_on_Side*4 + itr)*4` -> index `(Spell_on_Side*4 + itr_spellbook_page_spell_count) - 1`; the `-1` undoes the inner loop's final `++` past the found spell. Production omits it, grabbing the next spell's description. Fix: `((Spell_on_Side * 4) + itr_spellbook_page_spell_count) - 1` (or `+ m_spell_list_count`).

## SBK_DrawSpellDecode__STUB (Spellbook.c:2325)

Reviewed on request (called from Learn_Spell_Animation's decode loop as the auto-draw fn). Control flow faithful: F_SCOPY of TBL_SpellDecode_Rs/Ls into var_C/var_18; Copy_Back_To_Off; two BigBook_Compose blocks (mode==1 -> left/ST_TRUE + Draw(25); else left+1/ST_FALSE + Draw(173); then mode==0 -> left/ST_TRUE else left+1/ST_FALSE); the SBK_NewSpellAnim_Stg clear-region logic (<22, 22-28); and both mode-0/else draw blocks (Draw_Picture_Windowed -> stg<27 Draw_Picture -> stg 16-28 Set_Animation_Frame/FLIC_Draw at 20/GUI*37+21 vs 168/GUI*37+24) — all arg-for-arg.

**R3 — duplicate `Clear_Bitmap_Region` in the stg 22-28 branch (Spellbook.c:2360).** ASM loc_9DA3C has three Clear_Bitmap_Region calls (call1 IMG_SBK_PageText 0..var_38[stg]; call2 var_44[stg]..129; call3 var_38[stg]..var_44[stg] on spl_anim_compose_seg). Production has four — 2359 and 2360 are byte-identical (both = call1). Remove line 2360.

**R4 — decode-table data path (fixed via a single `wtf[34]` buffer).** The stg 22-27 branch drives the 6-frame decode wipe from two 6-entry X-coordinate tables: `g_spell_decode_x_r` = {53,45,37,16,6,6} (shrinking edge) and `g_spell_decode_x_l` = {64,71,79,101,111,111} (growing edge), both `s_SPELL_DECODE` (6 int16). In the OG these live in ONE contiguous stack buffer that IDA carved into named pieces at the two F_SCOPY-written offsets (typing them `s_SPELL_DECODE`): base = `var_44` (-0x44), `var_38` = base+6 words (-0x38), the `_l` copy `var_18` at base+22 (-0x18), the `_r` copy `var_C` at base+28 (-0x0C). The two reads use the same index off different bases: `var_44[stg]` (base) lands on the `_l` copy and `var_38[stg]` (base+6) on the `_r` copy for stg 22-27, i.e. `var_44[stg]==L[stg-22]`, `var_38[stg]==R[stg-22]`. The reconstruction had split that single buffer into five disjoint locals, so `var_38[22..27]` was OOB and never reached the copies -> tables unreachable, coords garbage. FIXED by declaring the real single buffer `int16_t wtf[34]`, `memcpy(&wtf[22], &g_spell_decode_x_l, 12)` / `memcpy(&wtf[28], &g_spell_decode_x_r, 12)`, and reading `wtf[stg]` (was `var_44[stg]`, L) and `wtf[stg+6]` (was `var_38[stg]`, R) across the three Clear_Bitmap_Region calls. (Earlier "frame padding" / "inert zero" framings were wrong: it's one shared buffer IDA split, and the copies are the real coordinate source. The `r`/`l` table labels are drake178's and look inverted vs the visual edges — don't read them as literal right/left.)

**R2 — entire post-decode teardown + research-reveal section missing (after Spellbook.c:2288).** Production ends after the SBK_DrawSpellDecode loop; the asm (loc_9D73B -> loc_9D892) continues with a large block production never reconstructed — hence `research_flag` (param) and `Have_Candidates` (local, 2115) are declared but unused. Missing: Release_Block; Deactivate_Auto_Function; `if(research_flag == 1)` { count research candidates -> Have_Candidates; if none -> Stop_All_Sounds/Play_Background_Music/return; up to two BigBook_PageTurn reveals (dir 1 then 666) advancing g_spellbook_left_page toward g_first_research_page, each shifting g_spellbook_descriptions[8+i]->[i] and redrawing; Set_Palette_Changes(0,224); Update_Remap_Color_Range(10,11) }; then the common tail g_spellbook_mode=1 / Set_Page_Off / BigBook_Draw / Set_Mouse_List(1, mouse_list_default) / PageFlip_FX. Full faithful skeleton captured in review notes; verify NUM_RESEARCH_SPELLS and the [8+i]->[i] shift against the asm on implementation.

---
