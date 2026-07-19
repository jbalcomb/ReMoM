CONQUEST-Lose_Animation.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_LimboFallAnim.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_LimboFallAnim.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_DrawLimboFall.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\GAME_DrawLimboFall.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\VGA_WndDrawRotateImg.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\VGA_WndDrawRotateImg.c     FLIC_Draw.c
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\VGA_WndDrawImageRect.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\VGA_WndDrawImageRect.c     FLIC_Draw.c
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\UU_VGA_WndDrawTransform.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\UU_VGA_WndDrawTransform.c  FLIC_Draw.c
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\VGA_RotateRect.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\VGA_RotateRect.c           FLIC_Draw.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg034\VGA_DrawTexture.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg034\VGA_DrawTexture.c      angle.c
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg034\VGA_DrawTexture_R.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg034\VGA_DrawTexture_R.c    angle.c

Production names for the seg030/seg034 chain are the leaked MoO2 "bitmap" module debug symbols:
  VGA_WndDrawRotateImg  → Draw_Bitmap_Rotated            (FLIC_Draw.c)
  VGA_RotateRect        → Get_Rotated_Pixels             (FLIC_Draw.c)
  VGA_WndDrawImageRect  → Draw_Bitmap_Animation_Block    (FLIC_Draw.c)
  VGA_DrawTexture       → VGA_DrawTexture                (angle.c)
  VGA_DrawTexture_R     → VGA_DrawTexture_R              (angle.c)

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> Lose_Animation()
                |-> Lose_Animation_Draw()   (registered via Assign_Auto_Function, called each frame)
                        |-> Draw_Bitmap_Rotated()
                                |-> Get_Rotated_Pixels()
                                |-> Draw_Bitmap_Animation_Block()
                                        |-> VGA_DrawTexture()
                                        |-> VGA_DrawTexture_R()

(Co-consumer of Draw_Bitmap_Animation_Block outside this chain: UU_VGA_WndDrawTransform, seg030.)

---

# `Lose_Animation` + `Lose_Animation_Draw` + the bitmap draw chain — Review / Walkthrough

The human-defeat cut-scene: when `Resolve_Wizard_Conquest` sees the **human** was defeated, it plays this "limbo fall" — the losing wizard's `LOSE.LBX` "hands-up" portrait spiralling/shrinking into the vortex file-animation (scale + rotate per `TBL_DefeatAnimSteps`), with lose music, until Esc or `m_conquest_anim_stage > 45`.

| Function | Location | asm | Role |
|---|---|---|---|
| `Lose_Animation` | [CONQUEST.c:885-968](../../MoM/src/CONQUEST.c#L885-L968) | `GAME_LimboFallAnim.asm` (WZD 093p12) | Setup, LOSE-art load, vortex file-animation, lose music, the Esc/`stage>45` input loop, teardown. |
| `Lose_Animation_Draw` | [CONQUEST.c:972-988](../../MoM/src/CONQUEST.c#L972-L988) | `GAME_DrawLimboFall.asm` (WZD 093p13) | Per-frame draw: advances the file-animation, then (`stage < 27`) scales + rotates the wizard portrait per the `TBL_DefeatAnimSteps` spiral table. |

> **Status: DONE-DONE (2026-07-19) — builds clean; test/demo renders the rotated fall correctly.** `Lose_Animation` and `Lose_Animation_Draw` are faithful 1:1 to the disassembly (game logic). The bitmap draw chain (`Draw_Bitmap_Rotated` → `Get_Rotated_Pixels` + `Draw_Bitmap_Animation_Block` → `VGA_DrawTexture`/`_R`) is a **functional port of the VGA Mode Y planar rasterizer to the SDL2 linear framebuffer** — not a byte-1:1 reconstruction (planar→linear is a platform rewrite), verified by the working demo. The one preserved OG bug (**B1**, the inverted Y-extent seed) is intact. Non-defects: the `Play_Sound` size HACK and the `str_empty_string[0]` hotkey-arg representation (S1).

## `Lose_Animation` (asm `GAME_LimboFallAnim.asm`)

Signature `void Lose_Animation(int16_t player_idx)` ↔ asm `Player_Index` (bp+6) — the defeated (human) wizard. Called by `Resolve_Wizard_Conquest` at [336](../../MoM/src/CONQUEST.c#L336) on the human-defeat branch. Faithful 1:1:
- **Setup** ([892-902](../../MoM/src/CONQUEST.c#L892-L902)) — `Stop_All_Sounds` / `Set_Mouse_List(1, none)` / `Clear_Fields` / `Fade_Out` / `Set_Page_Off` / `Fill(0,0,319,199,0)` / `Toggle_Pages` / `Load_Palette(6,-1)` / `Apply_Palette` / `Set_Page_Off`; `GAME_LimboFall_Stage = 0`.
- **Art** ([917-920](../../MoM/src/CONQUEST.c#L917-L920)) — `IMG_GAME_WizHandsUp = LBX_Reload(lose, wizard_id + 1, screen)` (asm `cbw / inc ax`); `IMG_GAME_LimboFall = Allocate_Next_Block(screen, 0x190)`; `Open_File_Animation(lose, 0)` (VORTEX).
- **Music** ([921-926](../../MoM/src/CONQUEST.c#L921-L926)) — `if(background_music==1)` load `music[MUSIC_LOSE_Military]` + `Play_Sound`. (The `music_data_seg_size` 2nd arg is the platform HACK — asm `Play_Sound` takes one segment arg; marked `// HACK DNE in Dasm` at [888](../../MoM/src/CONQUEST.c#L888).)
- **Loop** ([927-951](../../MoM/src/CONQUEST.c#L927-L951)) — `Assign_Auto_Function(Lose_Animation_Draw, 4)`; leave on Esc **or** `stage > 45` (asm `cmp …,45 / jle`); draw only when `leave_screen == 0`, then `PageFlip_FX / Release_Time(4)`.
- **Teardown** ([952-967](../../MoM/src/CONQUEST.c#L952-L967)) — `…/ Load_Palette(0,-1) / Clear_Palette_Changes(0,0xFF) / Set_Palette_Changes(0,0xDF) / Calculate_Remap_Colors / … / Play_Background_Music`.

## `Lose_Animation_Draw` (asm `GAME_DrawLimboFall.asm`)

Faithful 1:1 ([972-988](../../MoM/src/CONQUEST.c#L972-L988)): `Set_Page_Off` / `Draw_File_Animation`; then `if(GAME_LimboFall_Stage < 27)` (asm `cmp …,27 / jge`): read `TBL_DefeatAnimSteps[stage].x`/`.y`, `Draw_Picture_To_Bitmap(IMG_GAME_WizHandsUp, IMG_GAME_LimboFall)`, `Scale_Bitmap(…, .s, .s)`, `Draw_Bitmap_Rotated(x, y, .a, IMG_GAME_LimboFall)` ([980-984](../../MoM/src/CONQUEST.c#L980-L984)); then `m_conquest_anim_stage++` / `GAME_LimboFall_Stage++`. `TBL_DefeatAnimSteps` (struct `Spiral_Anim_Step`, [CONQUEST.c:83](../../MoM/src/CONQUEST.c#L83)) is byte-verified against the `dseg:511C` data — all 27 entries (`x`, `y`, `a`=angle, `s`=scale, `Unused_7h`), incl. the signed-negative Ys and the 8-byte stride.

## Bitmap draw chain (Mode Y → SDL2 linear port)

`Draw_Bitmap_Rotated` scales+rotates the falling portrait. The original is VGA **Mode Y** (planar: 4 planes, 80-byte rows, plane-masked writes); the port targets an **SDL2 linear** framebuffer (320-byte rows, one byte/pixel). The behavior matches the asm; the *addressing* is deliberately linearized.

| Function | Location | Mode Y asm | Verdict |
|---|---|---|---|
| `Draw_Bitmap_Rotated` | [FLIC_Draw.c:1101-1119](../../MoX/src/FLIC_Draw.c#L1101-L1119) | `VGA_WndDrawRotateImg.asm` | faithful — peek `w/h`, `Get_Rotated_Pixels`, `Draw_Bitmap_Animation_Block` |
| `Get_Rotated_Pixels` | [FLIC_Draw.c:1366](../../MoX/src/FLIC_Draw.c#L1366) | `VGA_RotateRect.asm` | faithful — `Cos/Sin` corner rotation, scales `*12/10`,`*22/25` (asm `0Ch/0Ah`,`16h/19h`) |
| `Draw_Bitmap_Animation_Block` | [FLIC_Draw.c:1128](../../MoX/src/FLIC_Draw.c#L1128) | `VGA_WndDrawImageRect.asm` | linearized texture-mapper; **B1 OGBUG preserved** |
| `VGA_DrawTexture` | [angle.c:421](../../MoX/src/angle.c#L421) | `VGA_DrawTexture.asm` | linear column blitter; horizontal `+1`/col restored |
| `VGA_DrawTexture_R` | [angle.c:527](../../MoX/src/angle.c#L527) | `VGA_DrawTexture_R.asm` | linear + translucency remap |

**The Mode Y → linear translation (what changed and why it's still correct):**
- **Addressing.** Mode Y pixel `(x,y)` = byte `y*80 + x/4`, plane `x&3`. The port uses a single linear index `y*SCREEN_WIDTH + x`; down-a-row is `+= SCREEN_WIDTH` (320, not the Mode Y 80). The caller (`Draw_Bitmap_Animation_Block`) now emits all screen offsets on the 320 stride (`min/max = y*SCREEN_WIDTH`, `next_draw_line = -SCREEN_WIDTH`).
- **Horizontal step.** Mode Y advanced X by rotating the plane mask (byte `++` every 4th column). With planes gone, that becomes an explicit `dest_pixel_index++` per column ([angle.c:505](../../MoX/src/angle.c#L505) and the `_R` twin). Without it every column would overprint the previous one.
- **The three pointers** (asm had one param + two globals; the SDL2 refactor made them explicit): destination `vga_frame_buffer = current_video_page` ([angle.c:422](../../MoX/src/angle.c#L422)/[526](../../MoX/src/angle.c#L526)) — the video-page base, exactly `Draw_Picture`'s `screen_start` less its offset; source `params->pict_seg` — the image **past its header** (asm passed `IMG_Seg+1`); translucency table `remap_color_palettes` (a global, used only by `_R`).
- **Translucency remap** (`_R`, [angle.c:574-577](../../MoX/src/angle.c#L574-L577)): `remap_color_palettes[(texel-232)*256 + screen_pixel]` — `232`=`0xE8`, and `256` = the asm's `(al-0xE8)*16` **paragraphs**. Guards match: skip `0`, remap `>= 0xE8`, else copy.

### Preserved OG bug (B1 — faithful, do NOT "fix")

`Draw_Bitmap_Animation_Block` seeds the Y-extent scan `max_y = 10000; min_y = 0;` ([FLIC_Draw.c:1160](../../MoX/src/FLIC_Draw.c#L1160)) — inverted (a bounding box should seed `max = -10000`, `min = 10000`), so `if(y<min_y)`/`if(y>max_y)` can't find the true extent. This is **faithful to the asm** (`VGA_WndDrawImageRect.asm` seeds the identical `10000`/`0`) and flagged in-code as `// BUG`.

## Spot note — S1 (hotkey arg representation, not a behavioral deviation)

`Add_Hidden_Field(0, 0, 319, 199, (int16_t)str_empty_string__ovr093[0], -1)` ([935](../../MoM/src/CONQUEST.c#L935)) passes the char value `str_empty_string[0]` (= 0) where the asm pushes `offset cnst_ZeroString_7`. The string is empty, so both mean "no hotkey" — the full-screen field only catches a click/keypress to advance. Same `[0]` convention as the done-done `Conquest_Animation`; accepted, not a divergence.

## Related references

- `…\ovr093\GAME_LimboFallAnim.asm` + `…\ovr093\GAME_DrawLimboFall.asm` — IDA Pro 5.5 disassembly (**the authority**) for the game logic; the `seg030`/`seg034` asm are the Mode Y source for the bitmap chain; Gemini `.c` files are a second opinion.
- [`Resolve_Wizard_Conquest`](CONQUEST-Resolve_Wizard_Conquest.md) — sole caller; plays `Lose_Animation(city_owner_idx)` on the human-defeat branch (before `GAME_OVER` / `MAGIC.EXE` swap).
- `TBL_DefeatAnimSteps` — the 27-entry spiral table (`x`, `y`, `a`, `s`) driving the fall.
- Shares `m_conquest_anim_stage`, `IMG_GAME_WizHandsUp`, `IMG_GAME_LimboFall`, `GAME_LimboFall_Stage` with the [`Conquest_Animation`](CONQUEST-Conquest_Animation.md) machinery. The bitmap chain (`Draw_Bitmap_*`, `Get_Rotated_Pixels`, `VGA_DrawTexture*`) is shared MoX platform code (MoO2 "bitmap" module), used well beyond this cut-scene.
