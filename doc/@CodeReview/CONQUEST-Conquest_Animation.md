CONQUEST-Conquest_Animation.md

Ground-truth disassembly (IDA Pro 5.5) + Gemini translations (second opinion, NOT ground truth):
  …\ovr093\WIZ_Conquest__WIP.asm / .c      → Conquest_Animation
  …\ovr093\GAME_DrawConqstAnim.asm / .c    → Conquest_Animation_Draw
  …\ovr093\GAME_DrawConquerors.asm / .c    → Conquest_Animation_Draw_Conquerors_Approach
  …\ovr093\GAME_Conqest_Scene2.asm / .c    → Conquest_Animation_Draw_Setup_Zap_Scene
  …\ovr093\GAME_DrawZappedWiz.asm / .c     → Conquest_Animation_Draw_Zapped_Wizard
  …\ovr093\GAME_DrawZapping.asm / .c       → Conquest_Animation_Draw_Zap_Strike
  …\ovr093\sub_79907.asm / .c              → Conquest_Animation_Draw_End_Pose

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> Banish_Wizard()
                |-> Conquest_Animation()

Conquest_Animation()
    |-> Conquest_Animation_Draw()   (registered via Assign_Auto_Function, called each frame)
            |-> Conquest_Animation_Draw_Conquerors_Approach()   stage 1-6
            |-> Conquest_Animation_Draw_Setup_Zap_Scene()       stage == 7
            |-> Conquest_Animation_Draw_Zapped_Wizard(1)        stage 7-10
            |-> Conquest_Animation_Draw_Zap_Strike()            stage 11-18
            |-> Conquest_Animation_Draw_Zapped_Wizard(0)        stage 19-22
            |-> Conquest_Animation_Draw_End_Pose()              stage > 6, neutral (Raiders) only

---

# `Conquest_Animation` + drawing functions — Review / Walkthrough

Plays the "banishes / defeats" conquest cut-scene: loads the WIZLAB/CONQUEST art, builds the headline string, plays win/lose music, then runs a staged FLIC/sprite animation (`m_conquest_anim_stage` 0…30) driving the conquering figures, the zap, and the zapped wizard, until Esc or stage > 30.

| Function | Location | asm | Role |
|---|---|---|---|
| `Conquest_Animation` | [CONQUEST.c:356-539](../../MoM/src/CONQUEST.c#L356-L539) | `WIZ_Conquest__WIP.asm` (WZD 093p02) | Setup, string build, art/music load, the 3-frame priming loop + input loop, teardown. |
| `Conquest_Animation_Draw` | [CONQUEST.c:543-603](../../MoM/src/CONQUEST.c#L543-L603) | `GAME_DrawConqstAnim.asm` (WZD 093p03) | Per-frame draw dispatcher keyed on `m_conquest_anim_stage`; registered via `Assign_Auto_Function`. |
| `Conquest_Animation_Draw_Conquerors_Approach` | [CONQUEST.c:607-625](../../MoM/src/CONQUEST.c#L607-L625) | `GAME_DrawConquerors.asm` (WZD 093p04) | Slides the axeboy / conquering-wizard / conehead sprites in (stage 1-6). |
| `Conquest_Animation_Draw_Setup_Zap_Scene` | [CONQUEST.c:629-682](../../MoM/src/CONQUEST.c#L629-L682) | `GAME_Conqest_Scene2.asm` (WZD 093p05) | Stage-7 hand-off: final sprite slide, reloads zap/zapped-wiz art, picks the scream SFX. |
| `Conquest_Animation_Draw_Zapped_Wizard` | [CONQUEST.c:686-736](../../MoM/src/CONQUEST.c#L686-L736) | `GAME_DrawZappedWiz.asm` (WZD 093p06) | Draws the growing zap + the zapped wizard frames (`flag` = no-deform pass). |
| `Conquest_Animation_Draw_Zap_Strike` | [CONQUEST.c:740-750](../../MoM/src/CONQUEST.c#L740-L750) | `GAME_DrawZapping.asm` (WZD 093p07) | The zap-in-progress frame (`Draw_File_Animation`). |
| `Conquest_Animation_Draw_End_Pose` | [CONQUEST.c:754-762](../../MoM/src/CONQUEST.c#L754-L762) | `sub_79907.asm` (WZD 093p08) | Static end-pose redraw for the neutral (Raiders) path. |

> **Status: DONE-DONE (2026-07-18) — faithful to the disassembly.** All seven functions match phase-for-phase (coordinate math, call order, and the dispatcher's stage ladder — including the stage-7 fall-through that runs both `Setup_Zap_Scene` and `Zapped_Wizard(1)`). Two items are **not** deviations and need no further work: **U1/U2** are jump-table dispatches whose data tables (`off_79749`, `off_798AE`) are absent from the extraction, so they cannot be byte-verified — the production mappings are consistent with the surrounding fall-through code and stand as-is; and the Play_Sound size argument is a documented platform HACK (faithful-by-intent).

## Jump-table dispatches — not byte-verifiable (data tables absent)

These two dispatches read from IDA jump tables whose data (`off_79749`, `off_798AE`) is **not** present in the extracted `.asm`, so they cannot be cross-checked at the byte level. The production mappings below are consistent with the surrounding fall-through code and stand as-is — nothing further to do unless the table bytes surface.

- **U1 — `Conquest_Animation_Draw_Setup_Zap_Scene` scream selection ([666-677](../../MoM/src/CONQUEST.c#L666-L677)).** `bx = wizard_id - 2`; `if(bx > 11)` → male ([660](../../MoM/src/CONQUEST.c#L660)), else `jmp [off_79749+bx*2]`. Production maps cases `0/6/9/10` → `SFX_FemaleScream`, all others → male. The `>11` unsigned guard matches the asm `ja`; the specific female cases depend on `off_79749`.
- **U2 — `Conquest_Animation_Draw_Zapped_Wizard` zapped-frame draw count ([708-731](../../MoM/src/CONQUEST.c#L708-L731)).** `if(frame <= 3) jmp [off_798AE+frame*2]` ([706](../../MoM/src/CONQUEST.c#L706)). Production maps frame `0→1` draw (no `Set_Animation_Frame`), `1→2`, `2→3`, `3→4` draws. The asm labels form a fall-through cascade (each case does one more `FLIC_Draw(0x42,0x10,ZappedWiz)`) consistent with this, but the index→label order lives in `off_798AE`.

## Faithful-by-intent — documented platform HACK

`Conquest_Animation` passes a size to `Play_Sound` ([487](../../MoM/src/CONQUEST.c#L487)) via `Music_Data_Seg_size` ([361](../../MoM/src/CONQUEST.c#L361), marked `// HACK DNE in Dasm`). The asm `Play_Sound` takes a single segment arg (`push Music_Data_Seg / call Play_Sound`). This is a deliberate platform-layer signature change, flagged in-code — not a reconstruction error.

## Verified faithful (spot notes)

- **`Conquest_Animation` body** matches phase-for-phase: the `!= NEUTRAL && != NEUTRAL` early return, `Set_Bit_Field_Near` on human conquest, the `city_count < 2` cap, palette/art loads (`LBX_Reload` wizlab 19, wizard art, conehead 14, axeboy 15, `Open_File_Animation` 17), the two-string allocation (`Near_Allocate_First` then `Near_Allocate_Next`, [405-406](../../MoM/src/CONQUEST.c#L405-L406)), the "banishes"(>1 city)/"defeats" vs neutral "Raiders … destroy … fortress" string build, win(109)/lose(110) music, the 3× priming loop, the Esc/`stage>30` input loop, and full teardown (palette 0, remap, `Play_Background_Music`).
- **`Conquest_Animation_Draw`** dispatcher matches asm `GAME_DrawConqstAnim.asm`: `stage==0` draws the wizlab wizard; the `stage>0` ladder is `if(<7) Conquerors_Approach; else { if(==7) Setup_Zap_Scene; if(<11) Zapped_Wizard(1); else if(<19){…scream…} Zap_Strike; else if(<23) Zapped_Wizard(0); }` — the `==7` case is a plain `if` so stage 7 falls through into `Zapped_Wizard(1)` ([554-588](../../MoM/src/CONQUEST.c#L554-L588)), matching the asm `loc_793F1 → loc_793FD` fall-through. The neutral `End_Pose` block ([590-598](../../MoM/src/CONQUEST.c#L590-L598)) and closing text (`Set_Font_Style_Outline_Heavy(5,5,0,0)` / `Set_Outline_Color(1)` / `Print_Centered(160,6,GUI_String_1)` / `stage++`) match.
- **`Conquest_Animation_Draw_Conquerors_Approach`** — all three sprite slides match: axeboy `si=0x42-(stage-1)*0x0B, di=-(stage+5)`; wizard (non-neutral) `si=0x5A-(stage-1)*0x0F, di=0x4E-(stage-1)*0x0D`; conehead `si=0x1E-(stage-1)*5, di=0x3C-(stage-1)*0x0A`, with the same `+0xFC/+0x58`, `+0xD0/+0x3C`, `+0x64/+0x78` offsets.
- **`Conquest_Animation_Draw_Setup_Zap_Scene`** — same three slides, then `Copy_Off_To_Back` and the art reloads (`wizard_id+33` wizard, `+18` zapped-wiz, `16` growzap, `32` conehead) all match; only the scream jump table (U1) is unverified.
- **`Conquest_Animation_Draw_Zapped_Wizard`** — the `flag`/no-deform split, `FLIC_Get_FrameCount`/`CurrentFrame`, both draw paths, and the trailing `Clipped_Draw(0xD0,0x3C)` / `Clipped_Draw(0x64,0x78)` match; only the frame jump table (U2) is unverified.
- **`Conquest_Animation_Draw_Zap_Strike`** and **`Conquest_Animation_Draw_End_Pose`** — 1:1 (neutral guard, `FLIC_Draw(0x45,0x4B)`, `Draw_File_Animation`, and the `0xD0/0x3C` + `0x64/0x78` / `0xFC/0x58` Clipped_Draws).

## Related references

- `…\ovr093\WIZ_Conquest__WIP.asm` + the six drawing `.asm` files — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` files are a second opinion.
- [`Resolve_Wizard_Conquest`](CONQUEST-Resolve_Wizard_Conquest.md) / `Banish_Wizard` — callers; both invoke `Conquest_Animation` after a fortress capture.
- `Assign_Auto_Function` / `Deactivate_Auto_Function` — register/unregister `Conquest_Animation_Draw` as the per-frame auto callback.
