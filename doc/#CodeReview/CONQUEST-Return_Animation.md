CONQUEST-Return_Animation.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_ReturnDialog.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_ReturnDialog.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_DrawReturnAnim.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_DrawReturnAnim.c

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> Banish_Wizard()
                |-> Return_Animation()
                    |-> Return_Animation_Draw()   (registered via Assign_Auto_Function, called each frame)

---

# `Return_Animation` + `Return_Animation_Draw` — Review / Walkthrough

The Spell-of-Return cut-scene: when a banished (non-human) wizard begins casting the Spell of Return, `Banish_Wizard` calls `Return_Animation`, which shows the vortex/conehead animation with the headline `"<wizard> begins casting the Spell Of Return."` and loops until Esc or `m_conquest_anim_stage > 40`.

| Function | Location | asm | Role |
|---|---|---|---|
| `Return_Animation` | [CONQUEST.c:916-958](../../MoM/src/CONQUEST.c#L916-L958) | `GAME_ReturnDialog.asm` (WZD 093p15) | Builds the headline string, loads the vortex (VORTEX 0) + cmbtfx (CMBTFX 27) art, registers the draw callback, runs the Esc/`stage>40` input loop, tears down. |
| `Return_Animation_Draw` | [CONQUEST.c:962-977](../../MoM/src/CONQUEST.c#L962-L977) | `GAME_DrawReturnAnim.asm` (WZD 093p16) | Per-frame draw: conehead FLIC + vortex FLIC + the paragraph text; registered via `Assign_Auto_Function`. |

> **Status: DONE-DONE (2026-07-18) — faithful to the disassembly 1:1.** Both functions match phase-for-phase and arg-for-arg, cross-checked against the Gemini `.c` translations. No deviations.

## `Return_Animation` (asm `GAME_ReturnDialog.asm`)

Signature `void Return_Animation(int16_t player_idx)` ↔ asm `Player_Index` (bp+6) — the banished wizard casting Return. Called by `Banish_Wizard` at [907](../../MoM/src/CONQUEST.c#L907) as `Return_Animation(loser_idx)`.

Faithful points:
- `strcpy(Unused_SpaceString, cnst_Conquest_Msg0)`; `GUI_String_1 = Near_Allocate_First(100)`; `strcpy(name)` + `strcat(cnst_SoReturn_Msg3 " begins casting the Spell Of Return.")` — [922-925](../../MoM/src/CONQUEST.c#L922-L925).
- `conquest_effect_seg = LBX_Reload(vortex, 0, _screen_seg)` and `conquest_conehead_seg = LBX_Reload_Next(cmbtfx, 27, _screen_seg)` ([926-927](../../MoM/src/CONQUEST.c#L926-L927)) match asm `LBX_Reload` (VORTEX 0) → `conquest_effect_seg` and `LBX_Reload_Next` (CMBTFX 27) → `IMG_GAME_ConeHead`.
- `Assign_Auto_Function(Return_Animation_Draw, 6)`; then the **redundant double** `m_conquest_anim_stage = 0` ([929](../../MoM/src/CONQUEST.c#L929) and [931](../../MoM/src/CONQUEST.c#L931)) — faithful to the asm's two `mov [m_conquest_anim_stage], 0` (lines 64 and 67, straddling the `xor si,si` that zeroes `leave_screen`).
- Input loop: `Mark_Time` / `Clear_Fields` / `Add_Hidden_Field(0, 0, 0x13F, 0xC7, cnst_HOTKEY_Esc10, -1)` / `Get_Input`; `leave_screen = ST_UNDEFINED` on Esc **or** `m_conquest_anim_stage > 40` (asm `cmp …,28h / jle`); draw only when `leave_screen == 0` ([933-953](../../MoM/src/CONQUEST.c#L933-L953)).
- Teardown `Clear_Fields / Reset_Window / Clear_Fields / Deactivate_Auto_Function` ([954-957](../../MoM/src/CONQUEST.c#L954-L957)).

## `Return_Animation_Draw` (asm `GAME_DrawReturnAnim.asm`)

Faithful points (asm `si=0x33`/`di=0x1C` → production `x=51`/`y=28`):
- `Set_Page_Off` / `Copy_Back_To_Off` ([968-969](../../MoM/src/CONQUEST.c#L968-L969)).
- `FLIC_Draw(x, y, conquest_conehead_seg)` then `FLIC_Draw(x+8, y+8, conquest_effect_seg)` ([970-971](../../MoM/src/CONQUEST.c#L970-L971)) — asm `FLIC_Draw(si, di, ConeHead)` / `FLIC_Draw(si+8, di+8, ZappedWiz)`.
- `Set_Outline_Color(16)` / `Set_Font_Style_Shadow_Down(4,4,0,0)` / `Set_Alias_Color(190)` ([972-974](../../MoM/src/CONQUEST.c#L972-L974)) — asm `10h` / `(4,4,0,0)` / `0BEh`.
- `Print_Paragraph(x+7, y+114, 165, GUI_String_1, 2)` ([975](../../MoM/src/CONQUEST.c#L975)) — asm `Print_Paragraph(si+7, di+72h, 0A5h, GUI_String_1, 2)` (`72h`=114, `0A5h`=165).
- `m_conquest_anim_stage++` ([976](../../MoM/src/CONQUEST.c#L976)).

## Related references

- `…\ovr093\GAME_ReturnDialog.asm` + `…\ovr093\GAME_DrawReturnAnim.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` files are a second opinion.
- [`Banish_Wizard`](../../MoM/src/CONQUEST.c#L855) — sole caller; invokes `Return_Animation(loser_idx)` on the non-human banishment path (then awards spells via `WIZ_ConquestSpells`).
- `Assign_Auto_Function` / `Deactivate_Auto_Function` — register/unregister `Return_Animation_Draw` as the per-frame auto callback.
- Shares the `m_conquest_anim_stage` counter, `GUI_String_1`, `conquest_effect_seg`, and `conquest_conehead_seg` globals with the [`Conquest_Animation`](CONQUEST-Conquest_Animation.md) cut-scene.
