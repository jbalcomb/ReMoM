CONQUEST-Win_Animation.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_PlayVictoryAnim.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_PlayVictoryAnim.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_Draw_WIN_Anim.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_Draw_WIN_Anim.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> Win_Animation()
                |-> Win_Animation_Draw()   (registered via Assign_Auto_Function, called each frame)

---

# `Win_Animation` + `Win_Animation_Draw` — Review / Walkthrough

The victory cut-scene: when `Resolve_Wizard_Conquest` finds the last computer player eliminated (`CP_Is_Dead`), it plays the human's win — the WIN.LBX background + wizard "hands-up" portrait + red-sparkle / planets file-animation, with win music, scrolling the four-line "Having conquered both the world of Arcanus and Myrror, I and only I, remain the one and true Master of Magic." over `m_conquest_anim_stage`, until Esc or `stage > 90`.

| Function | Location | asm | Role |
|---|---|---|---|
| `Win_Animation` | [CONQUEST.c:865-978](../../MoM/src/CONQUEST.c#L865-L978) | `GAME_PlayVictoryAnim.asm` (WZD 093p10) | Cache combat, load WIN art + win music, the Esc/`stage>90` input loop, teardown, restore combat cache. |
| `Win_Animation_Draw` | [CONQUEST.c:982-1019](../../MoM/src/CONQUEST.c#L982-L1019) | `GAME_Draw_WIN_Anim.asm` (WZD 093p11) | Per-frame draw: the hands-up speech frames, red sparkle, planets file-animation, and the staged 4-line victory text. |

> **Status: DONE-DONE (2026-07-19) — faithful to the disassembly 1:1; builds clean; test/demo looks perfect.** Both functions match the asm phase-for-phase and arg-for-arg (cross-checked against the Gemini `.c`). The only non-verifiable item is **U1**, the wizard→hands jump table — its `off_79C7F` data isn't in the extraction, so the case→value mapping (consistent with the sequential asm labels, and demo-correct for the tested wizard) stands as-is. Non-defects: the `Play_Sound` size HACK, the `cnst_HOTKEY_Esc10[0]` hotkey-arg representation (S1), and a preserved OG bug (B1, the `_CITIES` clobber).

## U1 — wizard → hands-image jump table (data not in the listing)

`switch(wizard_id)` picks the hands art ([890-898](../../MoM/src/CONQUEST.c#L890-L898)): `1→20, 2→19, 3→21, 4→22, 5→18, default→17`. The asm computes `bx = wizard_id - 1`, guards `cmp bx, 0Ch / ja` (→ default 17), then `jmp [cs:off_79C7F + bx*2]`. The five targets set `si` to `20,19,21,22,18` in that label order; production maps `case 1..5` to the same values, which is consistent with an in-order table — but `off_79C7F` isn't in the extraction, so the case→value mapping is **not byte-verified**. (Low stakes: wrong entry = wrong wizard's victory hands.)

## Verified faithful (phase notes)

### `Win_Animation`
- **Combat cache + `_CITIES` reuse** ([874-876](../../MoM/src/CONQUEST.c#L874-L876)) — `Combat_Cache_Write()`; `_CITIES = Allocate_First_Block(714, World_Data)` (asm `2CAh` + `SA_MK_FP0`). See **B1**.
- **Setup** ([877-886](../../MoM/src/CONQUEST.c#L877-L886)) — sounds/mouse/fields/fade, `Fill(0,0,319,199,0)`, `Load_Palette(5,-1)`, `Apply_Palette`.
- **Art** ([888-927](../../MoM/src/CONQUEST.c#L888-L927)) — WIN 0 background + `FLIC_Draw(0,0)`; hands at `FLIC_Draw(22,143)` (asm `16h,8Fh`); `IMG_GAME_WizHandsUp = LBX_Reload(win, wizard_id + 3)`; `IMG_GAME_RedSparkle = LBX_Reload(win, 1, World_Data)`; `Open_File_Animation(win, 2)`.
- **Music** ([928-934](../../MoM/src/CONQUEST.c#L928-L934)) — `MUSIC_WIN_Military` + `Play_Sound` (size = HACK).
- **Loop** ([936-960](../../MoM/src/CONQUEST.c#L936-L960)) — `Assign_Auto_Function(Win_Animation_Draw, 4)`; leave on Esc **or** `stage > 90` (asm `cmp …,90 / jle`); draw when not leaving — `Win_Animation_Draw()` ([956](../../MoM/src/CONQUEST.c#L956)).
- **Teardown** ([961-977](../../MoM/src/CONQUEST.c#L961-L977)) — `…Load_Palette(0,-1) / Clear_Palette_Changes(0,0xFF) / Set_Palette_Changes(0,0xDF) / … / Play_Background_Music`; then **`Combat_Cache_Read()`** ([977](../../MoM/src/CONQUEST.c#L977), asm `j_Combat_Cache_Read`).

### `Win_Animation_Draw`
- Dead `strcpy(niu_space, " ")` ([987](../../MoM/src/CONQUEST.c#L987)); `Set_Page_Off / Reset_Window / Copy_Back_To_Off`.
- **Speech frames** ([991-996](../../MoM/src/CONQUEST.c#L991-L996)) — `Speech_Anim_Frame = FLIC_Get_CurrentFrame(WizHandsUp)`; `Set_Animation_Frame(WizHandsUp, 0)`; `for(itr=0; itr <= Speech_Anim_Frame; itr++) FLIC_Draw(93, 2, WizHandsUp)` (asm `si <= Speech_Anim_Frame`).
- `FLIC_Draw(0,0, IMG_GAME_RedSparkle)` ([997](../../MoM/src/CONQUEST.c#L997)); `Draw_File_Animation` ([998](../../MoM/src/CONQUEST.c#L998)); `Set_Font_Style_Outline_Heavy(5,5,0,0)` / `Set_Outline_Color(0)`.
- **Staged text** ([1001-1016](../../MoM/src/CONQUEST.c#L1001-L1016)) — `stage < 15 → WIN_Msg_1`, `< 30 → _2`, `< 45 → _3`, `< 60 → _4`; no final `else` (≥ 60 keeps the last message), matching the asm's `cmp 15/30/45/60 / jge` ladder.
- `Print_Centered(160, 180, GUI_String_1)` ([1017](../../MoM/src/CONQUEST.c#L1017), asm `0A0h,0B4h`); `m_conquest_anim_stage++`.

## Preserved OG bug (B1 — faithful, do NOT "fix")

`_CITIES = Allocate_First_Block(714, World_Data)` ([876](../../MoM/src/CONQUEST.c#L876)) overwrites the live city-data pointer with a fresh 714-byte block just for this cut-scene — faithful to the asm (`Allocate_First_Block(2CAh) → _CITIES`), flagged in-code (`// ; OGBUG: overwrites the city data pointer, only to be overwritten itself by an LBXE_LoadReplace…` at [875](../../MoM/src/CONQUEST.c#L875)). `Combat_Cache_Write`/`Combat_Cache_Read` bracket the routine to restore combat state around it.

## Spot note — S1 (hotkey arg representation, not a behavioral deviation)

`Add_Hidden_Field(0,0,319,199, (int16_t)cnst_HOTKEY_Esc10[0], -1)` ([944](../../MoM/src/CONQUEST.c#L944)) passes the char value `cnst_HOTKEY_Esc10[0]` (= 0x1B) where the asm pushes `offset cnst_HOTKEY_Esc10`. Same `[0]`-representation convention as the done-done `Conquest_Animation`; accepted, not a divergence.

## Related references

- `…\ovr093\GAME_PlayVictoryAnim.asm` + `…\ovr093\GAME_Draw_WIN_Anim.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` files are a second opinion.
- [`Resolve_Wizard_Conquest`](CONQUEST-Resolve_Wizard_Conquest.md) — sole caller; plays `Win_Animation(_human_player_idx)` on the last-AI-eliminated branch (after `CP_Is_Dead`), before `End_Of_Game_Score` / `MAGIC.EXE`.
- Structural twin of [`Lose_Animation`](CONQUEST-Lose_Animation.md) — same auto-function loop shape; shares `m_conquest_anim_stage`, `IMG_GAME_WizHandsUp`, `GUI_String_1`.
