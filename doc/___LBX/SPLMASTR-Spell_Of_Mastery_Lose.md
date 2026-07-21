SPLMASTR-Spell_Of_Mastery_Lose.md

IMG_SBK_Anims  ==>  spl_anim_compose_seg

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose.asm       (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose.c         (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Draw.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Load.c

NX_j_Spell_Of_Mastery_Lose_Load()
    |-> Spell_Of_Mastery_Lose_Load()

NX_j_Spell_Of_Mastery_Lose()
    |-> Spell_Of_Mastery_Lose()

Next_Turn_Proc__WIP()
Cast_Spell_Overland_Do()
AI_Next_Turn()
    |-> j_Cast_Spell_Overland()
        |-> Cast_Spell_Overland()
            |-> j_Spell_Of_Mastery()
                |-> Spell_Of_Mastery()
                    |-> Spell_Of_Mastery_Lose()
                        |-> Spell_Of_Mastery_Lose_Load()
                        |-> Spell_Of_Mastery_Lose_Draw()
                            |-> Vanish_Bitmap__WIP()
                                |-> RNG_GFX_Random__WIP()
                                |-> rnd_bitfiddle__1oom()
                        |-> Lose_Animation()
                            |-> Lose_Animation_Draw()

MoO2
    Play_Diplomatic_Win_Flic_()     |-> Play_Cinematic_() ... Open_Smacker_Animation(); Draw_Smacker_Animation(); Close_Smacker_Animation(); 
    Play_Normal_Win_Flic_()         |-> Play_Cinematic_() ... Open_Smacker_Animation(); Draw_Smacker_Animation(); Close_Smacker_Animation(); 
    Play_Antaran_Win_Flic_()        |-> Play_Cinematic_() ... Open_Smacker_Animation(); Draw_Smacker_Animation(); Close_Smacker_Animation(); 
...why no Play_.*_Lose_Flic_()? ...Play_Loose_Flic_()?

MUSIC.LBX
// MUSIC.LBX, 109 "MOM64 XM"    "Winning Military"
// MUSIC.LBX, 110 "MOM66 XM"    "Losing Military"
// MUSIC.LBX, 111 "MOM65 XM"    "Losing Magic"

SPELLOSE.LBX

---

# `Spell_Of_Mastery_Lose` + `_Draw` + `_Load` — Review / Walkthrough

The Spell-of-Mastery **defeat** cut-scene: when an AI completes the Spell of Mastery, the human watches their wizard's sphere collapse — "Losing Magic" music, the caster's "*X* has cast the Spell of Mastery" caption, a sphere-grow → wobble-collapse → wizard-sphere reveal over `_osc_anim_ctr`, then the standard `Lose_Animation` + `End_Of_Game_Score` endgame.

| Function | Location | asm | Role |
|---|---|---|---|
| `Spell_Of_Mastery_Lose_Load` | [SPLMASTR.c:1684](../../MoM/src/SPLMASTR.c#L1684) | `Spell_Of_Mastery_Lose_Load.asm` (o138p01) | Loads SPELLOSE art (WIZLAB bg, familiar, sphere, wizard sphere/win), palette, caption. |
| `Spell_Of_Mastery_Lose_Draw` | [SPLMASTR.c:1738](../../MoM/src/SPLMASTR.c#L1738) | `Spell_Of_Mastery_Lose_Draw.asm` (o138p02) | Per-frame: caption + the 3-stage sphere-collapse keyed on `_osc_anim_ctr`. |
| `Spell_Of_Mastery_Lose` | [SPLMASTR.c:1824](../../MoM/src/SPLMASTR.c#L1824) | `Spell_Of_Mastery_Lose.asm` (o138p03) | Setup, music, the `_osc_anim_ctr < 43` frame loop, endgame handoff. |

> **Status: DONE-DONE — faithful 1:1.** All three functions match the disassembly call-for-call. The only deliberate deviations are **B1** (a preserved `Print_Centered` OGBUG) and **D1** (the `Respawn`→return-to-menu `EOG_HACK`).

## B1 — `Print_Centered` OGBUG (preserved)

[SPLMASTR.c:1762](../../MoM/src/SPLMASTR.c#L1762) `Print_Centered(160, 5, GUI_NearMsgString)` — the asm call is 4-arg (`…, 2`; `add sp, 8`); production drops the extra `2`, annotated `/* OGBUG */`. Preserved as-is; same quirk as `Cast_Spell_Of_Mastery_Draw`. The adjacent [1763](../../MoM/src/SPLMASTR.c#L1763) `Print_Centered(160, 25, strSpellOfMastery)` is genuinely 3-arg (`add sp, 6`) and correctly carries no OGBUG.

## D1 — endgame `Respawn` (accepted `EOG_HACK`)

[SPLMASTR.c:1859](../../MoM/src/SPLMASTR.c#L1859): asm `GAME_EXE_Swap(cnst_MAGIC_EXE_File2, cnst_MAGICEXE_arg0_2, &spellose[8], &spellose[8])` → `// DONT Respawn(...)`. The endgame returns via the flag path; this site sits under the `Spell_Of_Mastery` GUARD. See [PRD-Endgame-Return-To-Menu-Screen-State.md](../#AI_Plans/PRD-Endgame-Return-To-Menu-Screen-State.md).

## Verified faithful

- **`_Load`** ([1684-1734](../../MoM/src/SPLMASTR.c#L1684-L1734)) — every `LBX_Reload`/`_Next`, `Load_Palette_From_Animation`, caption (`_players[GAME_SoM_Cast_By].name` + `strHasCastThe`), and `Allocate_Next_Block(_screen_seg, 900)` matches the asm arg-for-arg. Two reconstructed variable renames: asm `pict_seg` → `spellose_sphere_seg`, `word_434F8` → `spellose_wizard_sphere_seg`.
- **`_Draw`** — `colors[16]=5`, `Get_Bitmap_Actual_Size`, `start_x = 35 + x1 + width/2`, the `<12 / <24 / <36` stage ladder, the three-stage `Shear_Bitmap_Y` wobble ([1779-1798](../../MoM/src/SPLMASTR.c#L1779-L1798), branched `(_osc_anim_ctr-12)/2 < 2 / < 4 / else`), `Scale_Bitmap(spl_anim_compose_seg, 100-(_osc_anim_ctr-12)*6, 100)`, `Vanish_Bitmap(spl_anim_compose_seg, (_osc_anim_ctr-12)*8)` ([1806](../../MoM/src/SPLMASTR.c#L1806)), `x = 61 + (_osc_anim_ctr-12)*714/200`, and every `FLIC_Draw`/`Draw_Picture` coordinate match. The dead `x = 61` store ([1775](../../MoM/src/SPLMASTR.c#L1775), overwritten before use) is faithful (asm `loc_C0898`).
- **`_Lose`** — `Clear_Fields`, `Stop_All_Sounds`, music load, `Fade_Out` ([1835](../../MoM/src/SPLMASTR.c#L1835)), the single-increment `_osc_anim_ctr < 43` loop ([1847-1854](../../MoM/src/SPLMASTR.c#L1847-L1854)), `Combat_Cache_Read`, `Lose_Animation(HUMAN_PLAYER_IDX)` (asm `GAME_LimboFallAnim`), `End_Of_Game_Score` (asm `GAME_OVER`), `s01p16_empty_function`. Platform notes: `Play_Sound` size arg ([1833](../../MoM/src/SPLMASTR.c#L1833), size HACK); `CLROFF()` ([1837](../../MoM/src/SPLMASTR.c#L1837)) = the asm's `Set_Page_Off` + `Fill(0,0,319,199,0)`.

## Related references

- `…\ovr138\Spell_Of_Mastery_Lose{,_Draw,_Load}.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` second opinions.
- Caller: `Spell_Of_Mastery` — invokes `_Lose` for a non-human SoM caster ([SPLMASTR.c:2073](../../MoM/src/SPLMASTR.c#L2073)); its endgame GUARD covers this path.
- [`Vanish_Bitmap`](FLIC_Draw-Vanish_Bitmap.md) ([FLIC_Draw.c:3712](../../MoX/src/FLIC_Draw.c#L3712)) — the dissolve driving the sphere-reveal in `_Draw`.
- [`Shear_Bitmap_Y`](../#CodeReview/FLIC_Draw-Shear_Bitmap_Y.md) ([FLIC_Draw.c:546](../../MoX/src/FLIC_Draw.c#L546)) — the vertical-shear wobble used by the `_Draw` collapse stages.
- Structural twin: `Cast_Spell_Of_Mastery` / `_Draw` (the SoM-*started* announcement) — shares the `Print_Centered` **B1** quirk.
