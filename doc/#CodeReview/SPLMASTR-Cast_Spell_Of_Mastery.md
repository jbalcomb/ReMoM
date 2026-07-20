SPLMASTR-Cast_Spell_Of_Mastery.md

SoM_Started__STUB()      ==>  Cast_Spell_Of_Mastery()
SoM_Started_Draw__STUB() ==>  Cast_Spell_Of_Mastery_Draw()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Cast_Spell_Of_Mastery.asm       (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Cast_Spell_Of_Mastery.c         (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Cast_Spell_Of_Mastery_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Cast_Spell_Of_Mastery_Draw.c

AI_Spell_Select()
    |-> j_Cast_Spell_Overland_Do()
        |-> Cast_Spell_Overland_Do()
            |-> j_Cast_Spell_Of_Mastery()
                |-> Cast_Spell_Of_Mastery()
                    |-> Cast_Spell_Of_Mastery_Draw()   (also registered via Assign_Auto_Function)

Spellbook_Screen()
    |-> Cast_Spell_Overland_Do()
        |-> j_Cast_Spell_Of_Mastery()
            |-> Cast_Spell_Of_Mastery()
                |-> Cast_Spell_Of_Mastery_Draw()

---

# `Cast_Spell_Of_Mastery` + `Cast_Spell_Of_Mastery_Draw` — Review / Walkthrough

The "…has started casting the Spell of Mastery" full-screen announcement, shown when a wizard *begins* the multi-turn cast (not the victory — that's `Spell_Of_Mastery`). Fades to a dedicated palette, plays the SoM-started music, runs the VORTEX file-animation (swapping in frames 68/69/70 at ticks 17/33/49) with two centered caption lines, frame-paced until `_combat_wx` reaches 200 or Esc, then restores the main screen.

| Function | Location | asm | Role |
|---|---|---|---|
| `Cast_Spell_Of_Mastery_Draw` | [SPLMASTR.c:1962-1978](../../MoM/src/SPLMASTR.c#L1962-L1978) | `Cast_Spell_Of_Mastery_Draw.asm` (WZD 138p04) | Per-frame: draw all pending file-animation frames, print the two caption lines, `_combat_wx++`. |
| `Cast_Spell_Of_Mastery` | [SPLMASTR.c:1981-2051](../../MoM/src/SPLMASTR.c#L1981-L2051) | `Cast_Spell_Of_Mastery.asm` (WZD 138p05) | Setup, the frame-paced Esc/`_combat_wx < 200` animation loop, teardown back to the main screen. |

> **Status: DONE-DONE (2026-07-20) — faithful to the disassembly 1:1.** Both functions match the asm phase-for-phase and arg-for-arg (cross-checked against the Gemini `.c`); builds clean. One preserved OG quirk (**B1**, the extra `Print_Centered` argument, annotated in-code). Accepted platform conventions: the `Play_Sound` size arg, `__HACK`-suffixed anim calls, 3-arg `Load_Palette`, and the `[0]` hotkey representation — all consistent with the other reviewed cut-scenes. asm proc names already match production (`SoM_Started(_Draw)__STUB` → `Cast_Spell_Of_Mastery(_Draw)`).

## Preserved OG quirk (B1 — faithful, do NOT "fix")

`Cast_Spell_Of_Mastery_Draw`'s first caption call is 4-arg in the asm — `Print_Centered(160, 160, GUI_NearMsgString, 2)` (`add sp, 8`) — where every other `Print_Centered` (incl. the `strSpellOfMastery` line right below) is 3-arg. The trailing `2` is an extraneous argument the routine doesn't consume; production drops it to a 3-arg call and flags the original in-code ([SPLMASTR.c:1975](../../MoM/src/SPLMASTR.c#L1975)): `Print_Centered(160, 160, GUI_NearMsgString);  /* OGBUG  extra parameter - Print_Centered(160, 160, GUI_NearMsgString, 2); */`. Harmless; keep the annotation.

## Phase map (asm ↔ production)

### `Cast_Spell_Of_Mastery_Draw`
| Phase | asm | production | fidelity |
|---|---|---|---|
| `frame_count = Get_File_Animation_Frame() + 1` | lines 10-14 | [1966](../../MoM/src/SPLMASTR.c#L1966) | faithful |
| `Set_File_Animation_Frame(0)`; `Set_Page_Off` | 15-19 | [1967-1968](../../MoM/src/SPLMASTR.c#L1967-L1968) | faithful |
| draw all pending frames (`for i < frame_count`) | `loc_C0D56` | [1969-1972](../../MoM/src/SPLMASTR.c#L1969-L1972) | faithful (`Draw_File_Animation__HACK`) |
| `Set_Outline_Color(0)`, `Set_Font_Style_Outline(5,5,0,0)` | 29-42 | [1973-1974](../../MoM/src/SPLMASTR.c#L1973-L1974) | faithful |
| `Print_Centered(160,160,GUI_NearMsgString, 2)` | 43-52 | [1975](../../MoM/src/SPLMASTR.c#L1975) | **B1** (extra arg dropped, annotated) |
| `Print_Centered(160,180,strSpellOfMastery)` | 53-60 | [1976](../../MoM/src/SPLMASTR.c#L1976) | faithful |
| `_combat_wx++` | 61 | [1977](../../MoM/src/SPLMASTR.c#L1977) | faithful |

### `Cast_Spell_Of_Mastery`
| Phase | asm | production | fidelity |
|---|---|---|---|
| Deactivate auto-fn, stop sounds, build `GUI_NearMsgString` = name + `strHasStartedCastingThe` | 12-30 | [1984-1987](../../MoM/src/SPLMASTR.c#L1984-L1987) | faithful |
| `LBX_Reload(music,MUSIC_SoM_Started,SND_Music_Segment)`; `Open_File_Animation(spellscr,67)`; `_temp_sint_4=67` | 31-46 | [1988-1992](../../MoM/src/SPLMASTR.c#L1988-L1992) | faithful (+size, `__HACK`) |
| mouse none, fade, `Fill(0,0,319,199,0)`, copy/toggle, `Load_Palette(4,-1)`, page-off | 47-77 | [1993-2000](../../MoM/src/SPLMASTR.c#L1993-L2000) | faithful (3-arg `Load_Palette`) |
| `Cast_Spell_Of_Mastery_Draw()`, toggle, fade-in, `Assign_Auto_Function(…,2)` | 78-89 | [2001-2004](../../MoM/src/SPLMASTR.c#L2001-L2004) | faithful |
| if bg-music `Play_Sound`; `Clear_Fields`; `Add_Hidden_Field(0,0,319,199,ESC,-1)`; `_combat_wx=0` | 90-112 | [2005-2011](../../MoM/src/SPLMASTR.c#L2005-L2011) | faithful (Play_Sound size, `[0]` hotkey) |
| Esc / `_combat_wx < 200` loop: 17/33/49 anim swaps, then `Mark_Time` / `Set_Page_Off` / `Cast_Spell_Of_Mastery_Draw()` / `Toggle_Pages` / `Release_Time(3)` | `loc_C0EC9`-`loc_C0F2C` | [2012-2031](../../MoM/src/SPLMASTR.c#L2012-L2031) | faithful |
| Teardown: stop sounds, bg-music, fade, `Load_Palette(0,-1)`, `Allocate_Reduced_Map`, `Assign_Auto_Function(Main_Screen_Draw,2)` | `loc_C0F3E`-183 | [2032-2037](../../MoM/src/SPLMASTR.c#L2032-L2037) | faithful |
| mouse: human→default, else hourglass; `Set_Page_Off`/`Main_Screen_Draw`/toggle/copy/fade-in | 185-203 | [2038-2050](../../MoM/src/SPLMASTR.c#L2038-L2050) | faithful |

## Platform notes (accepted — consistent with prior cut-scene reviews)

- `Draw_File_Animation`→`Draw_File_Animation__HACK`, `Open_File_Animation`→`Open_File_Animation__HACK` — renamed platform reimplementations, same call/args.
- `Play_Sound` is 1-arg in the asm (segment only); production is 2-arg (`SND_Spell_Music`, `SND_Spell_Music_size`), with the size from `lbxload_entry_length` at [1989](../../MoM/src/SPLMASTR.c#L1989). Same size-HACK as `Win_Animation`.
- `Load_Palette` is 2-arg in the asm; production passes a 3rd `ST_NULL` ([1999](../../MoM/src/SPLMASTR.c#L1999), [2035](../../MoM/src/SPLMASTR.c#L2035)) — consistent palette-signature padding.
- `Add_Hidden_Field` hotkey arg: asm pushes `offset str_hotkey_ESC__ovr138`; production passes `(int16_t)str_hotkey_ESC__ovr138[0]` ([2010](../../MoM/src/SPLMASTR.c#L2010)) — the established `[0]` representation.
- `_combat_wx` is the asm's `_combat_wx__som_started_anim_ctr` — the combat-x global reused as the started-casting tick counter (see [Combat.c:850](../../MoM/src/Combat.c#L850)).

## Related references

- `…\ovr138\Cast_Spell_Of_Mastery.asm` + `…\ovr138\Cast_Spell_Of_Mastery_Draw.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` are second opinions.
- Callers: `Spellbook_Screen` → `Cast_Spell_Overland_Do` ([SBookScr.c:558](../../MoM/src/SBookScr.c#L558)) for the human; `AI_Spell_Select` → `Cast_Spell_Overland_Do` for the AI.
- Sibling: `Spell_Of_Mastery` (SPLMASTR.c:2208) — the *completion*/victory (distinct from this "started casting" announcement); its endgame path is covered in [PRD-Endgame-Return-To-Menu-Screen-State.md](../#AI_Plans/PRD-Endgame-Return-To-Menu-Screen-State.md).
