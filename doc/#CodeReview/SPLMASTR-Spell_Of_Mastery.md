SPLMASTR-Spell_Of_Mastery.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery.asm       (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery.c         (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Draw.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Load.c

---

# `Spell_Of_Mastery` + `_Draw` + `_Load` — Review / Walkthrough

The Spell-of-Mastery **announcement** cut-scene: the wizard's lab, a blue column, and each surviving wizard's portrait paraded in turn (60 frames apiece, skipping anyone casting Spell of Return) while "Spell Mastery Started" plays. If the caster is *not* the human, it first detours through the whole [`Spell_Of_Mastery_Lose`](SPLMASTR-Spell_Of_Mastery_Lose.md) defeat sequence. It ends in `Win_Animation` + `End_Of_Game_Score` + the DOS `Respawn`.

| Function | Location | asm | Role |
|---|---|---|---|
| `Spell_Of_Mastery_Load` | [SPLMASTR.c:1958](../../MoM/src/SPLMASTR.c#L1958) | `Spell_Of_Mastery_Load.asm` (o138p06) | Loads the WIZLAB backdrop, blue column, caster portrait, twinkle. |
| `Spell_Of_Mastery_Draw` | [SPLMASTR.c:1976](../../MoM/src/SPLMASTR.c#L1976) | `Spell_Of_Mastery_Draw.asm` (o138p07) | Per-frame: column + the portrait's 60-frame blink cycle + twinkle. |
| `Spell_Of_Mastery` | [SPLMASTR.c:2016](../../MoM/src/SPLMASTR.c#L2016) | `Spell_Of_Mastery.asm` (o138p08) | Setup, the per-wizard parade loop, endgame handoff. |

> **Status: DONE-DONE — faithful 1:1.** All three functions match the disassembly call-for-call. **B1** is a preserved OG hazard; **D1-D6** are accepted `EOG_HACK` / platform deviations; **N1** notes two globals still carrying IDA placeholder names.

## B1 — unbounded parade cursor walk (OG hazard, preserved)

[SPLMASTR.c:2066](../../MoM/src/SPLMASTR.c#L2066) `while(_players[++parade_player_idx].casting_spell_idx == spl_Spell_Of_Return) {}` has no upper bound (asm `loc_C125D`, 122-129 — the asm has none either).

It is normally protected by the frame accounting: `parade_frame_total` ([2052](../../MoM/src/SPLMASTR.c#L2052)) budgets 60 frames per opponent and refunds 60 for each Spell-of-Return caster ([2057](../../MoM/src/SPLMASTR.c#L2057)), so the number of gate firings equals the number of eligible opponents and the cursor lands exactly on the last one. An *AI* caster is also never a Return-caster, so there is always a valid stop.

The degenerate case: the **human** casts the Spell of Mastery while **every** opponent is simultaneously Spell-of-Return casting. Then `parade_frame_total == 12`, the gate fires once, and the cursor has no eligible opponent to land on — so it overshoots onto the neutral slot.

That overshoot stays in bounds. `_players` is `[NUM_PLAYERS]` = 6, indices 0-5 ([MOX_DAT.c:17](../../MoX/src/MOX_DAT.c#L17)); `NEUTRAL_PLAYER_IDX` is 5 ([MOM_DEF.h:394](../../MoX/src/MOM_DEF.h#L394)); and `_num_players = opponents + 1` never exceeds 5 — so the walk tops out at index 5. The neutral player never casts, so its `casting_spell_idx` is `spl_NONE` (0) against `spl_Spell_Of_Return` (214, [Spellbook.h:240](../../MoM/src/Spellbook.h#L240)), and the walk halts there rather than running on. `Open_File_Animation__HACK` then loads a portrait from the neutral slot's `wizard_id`.

**Severity, stated plainly: it's a sub-second cosmetic flicker, in a rare state, in a cutscene that plays once at the end of a won game, and it's faithful.** Roughly five of the twelve frames show the wrong portrait, and it is realistically only reachable at two players — where "every opponent banished" just means "the one opponent is banished." **And it can't be fixed** — the asm has no bound either, so any guard would be a deviation.

Recorded only so it is not rediscovered as a reconstruction error.

## D1-D6 — accepted `EOG_HACK` / platform deviations

- **D1 — endgame GUARD.** [2022](../../MoM/src/SPLMASTR.c#L2022) `if(magic_master_idx != ST_UNDEFINED) { return; }` has no asm counterpart; it is the `EOG_HACK` re-entry guard. See [PRD-Endgame-Return-To-Menu-Screen-State.md](../#AI_Plans/PRD-Endgame-Return-To-Menu-Screen-State.md).
- **D2 — early return on the defeat path.** [2029](../../MoM/src/SPLMASTR.c#L2029) `return;` after `Spell_Of_Mastery_Lose()`. In OG that call never comes back — `Spell_Of_Mastery_Lose` ends in `GAME_EXE_Swap`, and IDA renders this function's whole epilogue as unreachable raw bytes (`Spell_Of_Mastery.asm:239-247` = `add sp,8` / `pop di` / `pop si` / `mov sp,bp` / `pop bp` / `retf`). With the `Respawn` commented out the call returns, so the `return` restores the OG's one-way-door semantics; without it the defeat path falls through into the victory parade, `Win_Animation` ([2096](../../MoM/src/SPLMASTR.c#L2096)) and a second `End_Of_Game_Score` ([2097](../../MoM/src/SPLMASTR.c#L2097)). Nothing else intercepts it: `magic_master_idx` is set only at [AIDUDES.c:267](../../MoM/src/AIDUDES.c#L267), *after* `Cast_Spell_Overland` and therefore after this function returns.
- **D3 — endgame `Respawn`.** [2099](../../MoM/src/SPLMASTR.c#L2099): asm `GAME_EXE_Swap` (237) commented out; the endgame returns via the flag path.
- **D4 — 16-bit wrap restored explicitly.** [2064](../../MoM/src/SPLMASTR.c#L2064) `if((((int16_t)(_osc_anim_ctr + 65530)) % 60) == 0)`. The asm's `add ax, 65530` (114-118) is a 16-bit register add that wraps, so the gate fires at `_osc_anim_ctr` = 6, 66, 126 — the wizard-cycle boundary `_Draw` keys on. `_osc_anim_ctr` is `int16_t` ([MOM_DAT.h:4733](../../MoX/src/MOM_DAT.h#L4733)) and MSVC `int` is 32-bit, so without the cast the expression promotes, never wraps, and first fires at 50 (65530 mod 60 = 10 → `ctr ≡ 50 mod 60`) — leaving frames 7-49 calling `Draw_File_Animation__HACK()` on an animation `Open_File_Animation__HACK()` has not opened. `int16_t` (not `uint16_t`) matches the asm's signed `cwd`/`idiv`.
- **D5 — `Play_Sound` size argument.** asm `Play_Sound` takes one argument (67-69, single `pop cx`); production threads `SND_Spell_Music_size = lbxload_entry_length` ([2035](../../MoM/src/SPLMASTR.c#L2035)) into a second parameter ([2048](../../MoM/src/SPLMASTR.c#L2048)). Same platform size HACK accepted in `_Lose`.
- **D6 — `Load_Palette` third argument.** asm passes **two** arguments (199-205: two pushes, two `pop cx`); production passes three ([2090](../../MoM/src/SPLMASTR.c#L2090)), matching the reconstruction's 3-arg signature ([Fonts.h:515](../../MoX/src/Fonts.h#L515)) used codebase-wide.

`Add_Hidden_Field` ([2051](../../MoM/src/SPLMASTR.c#L2051)) is worth noting but is not a deviation introduced here: the asm pushes `offset str_hotkey_ESC__ovr138` (a pointer) where production passes `(int16_t)str_hotkey_ESC__ovr138[0]` (the `0x1B` byte, [SPLMASTR.c:160](../../MoM/src/SPLMASTR.c#L160)), because the reconstruction declares the parameter `int16_t hotkey` ([Fields.h:686](../../MoX/src/Fields.h#L686)). The sibling call in `Cast_Spell_Of_Mastery` ([1914](../../MoM/src/SPLMASTR.c#L1914)) does the same. That is a signature-modelling question for `Add_Hidden_Field`, not for this function.

The three functions are `static` ([1958](../../MoM/src/SPLMASTR.c#L1958), [1976](../../MoM/src/SPLMASTR.c#L1976)) where the OG has far procs reached through `j_` thunks — a linkage modernisation, not a behavioural change.

## N1 — two globals still carrying IDA placeholder names

Not bugs; the asm supplies names the reconstruction has not adopted.

| Production | asm label | What it is |
|---|---|---|
| `word_43494` ([1962](../../MoM/src/SPLMASTR.c#L1962)) | `som_wizlab_seg` (`_Load.asm:14`) | WIZLAB.LBX #19 backdrop, drawn once at (0,0) then cached to back; never referenced again. Reads as `wizlab_background_seg`. |
| `word_434C4` ([1971](../../MoM/src/SPLMASTR.c#L1971)) | `som_twinkle_seg` (`_Load.asm:53`) | SPLMASTR.LBX #28, drawn at (95,55) for the first 9 frames of each wizard's cycle ([2008](../../MoM/src/SPLMASTR.c#L2008)) and rewound per wizard ([2068](../../MoM/src/SPLMASTR.c#L2068)). |

`IDK_wizard_id_thing_seg` ([85](../../MoM/src/SPLMASTR.c#L85)) is the same class — SPLMASTR.LBX `14 + wizard_id`, the caster portrait at (69,75) driving the 60-frame blink cycle. Reads as `som_wizard_portrait_seg`.

## Verified faithful

- **`_Load`** ([1958-1974](../../MoM/src/SPLMASTR.c#L1958-L1974)) — 1:1 with all 56 lines of asm: `Set_Page_Off`, `LBX_Reload(wizlab, 19, _screen_seg)`, `FLIC_Draw(0,0,…)`, `Copy_Off_To_Back`, `Set_Page_On`, `LBX_Reload(splmastr, 29, _screen_seg)`, `LBX_Reload(splmastr, 14 + _players[cast_spell_of_mastery_player_idx].wizard_id, World_Data)`, `LBX_Reload_Next(splmastr, 28, World_Data)` — every argument and target matches.
- **`_Draw`** ([1976-2013](../../MoM/src/SPLMASTR.c#L1976-L2013)) — 1:1. `Copy_Back_To_Off`, `FLIC_Draw(90, 0, wizlab_blue_column_seg)`, the `_osc_anim_ctr <= 6` reset branch (asm `loc_C111C`), and the 60-frame cycle: `rem < 10 → frame rem/2` (`loc_C1060`), `rem > 50 → frame (60-rem)/2` (`loc_C1092`), else `frame 4` (`loc_C10BB`). The `(6 + (_num_players-1)*60) > _osc_anim_ctr` gate (78-84) and the nested `rem < 9` twinkle (86-98) both match. The asm places the `<= 6` branch last (`loc_C111C`); production's `if/else` ordering is the same test on the same constant.
- **`_Mastery` body** ([2016-2100](../../MoM/src/SPLMASTR.c#L2016-L2100)) — every call in `Spell_Of_Mastery.asm` maps in order: `m_magic_winner_idx = player_idx` (14-15, asm `GAME_SoM_Cast_By`), `Combat_Cache_Write` (16), `Deactivate_Auto_Function` (17), the `!= HUMAN_PLAYER_IDX` detour (18-21), `Stop_All_Sounds__STUB` (26), music load (27-34), `Set_Mouse_List` (35-41), `Fade_Out` (42), `_Load` (43-44), `CLROFF()` = `Set_Page_Off` + `Fill(0,0,319,199,0)` (45-57), `Toggle_Pages`/`Copy_On_To_Off_Page`/`Set_Page_Off`/`_Draw`/`Toggle_Pages`/`Fade_In` (58-64), the music gate (65-69), `Clear_Fields` (71), `Add_Hidden_Field` (72-86), `parade_frame_total = 12 + (_num_players-1)*60` (87-92), the Spell-of-Return subtraction loop (93-108), the `while((_osc_anim_ctr < parade_frame_total) && (Get_Input() != full_screen_esc))` head (186-193, including the short-circuit order), the `(_osc_anim_ctr + 65530) % 60 == 0` gate (114-120, constant preserved verbatim — see **D4**), the `while(_players[++parade_player_idx].casting_spell_idx == spl_Spell_Of_Return)` skip (122-129), `Open_File_Animation` (130-139), `Reset_Animation_Frame` (142-144), the `parade_frame_total - 6` block-reload (146-163), the `_osc_anim_ctr == 7` reload (165-174), the frame tail `Mark_Time`/`Set_Page_Off`/`_Draw`/`Toggle_Pages`/`Release_Time(3)`/`inc` (176-185), and the endgame tail `Stop_All_Sounds__STUB`/`Play_Background_Music`/`Fade_Out`/`Load_Palette`/`CLROFF()`/`Toggle_Pages`/`Copy_On_To_Off_Page`/`Fade_In`/`Combat_Cache_Read`/`Win_Animation(0)`/`End_Of_Game_Score`/`s01p16_empty_function` (196-228). `Win_Animation(HUMAN_PLAYER_IDX)` = asm `j_GAME_PlayVictoryAnim(0)`; `End_Of_Game_Score` = asm `j_GAME_OVER`.
- **Locals** — `parade_frame_total` (asm `var_4`) is the parade's frame budget; `parade_player_idx` (asm `_SI_IDK`) is the roster cursor. Both filter on `spl_Spell_Of_Return` over the same index range, which is what keeps the frame count and the cursor in step (see **B1**).

## Related references

- `…\ovr138\Spell_Of_Mastery{,_Draw,_Load}.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` second opinions.
- Caller: [`Cast_Spell_Overland`](../../MoM/src/OverSpel.c#L811) — the only call site, annotated `OG-MoM NO_RETURN`.
- [`Spell_Of_Mastery_Lose`](SPLMASTR-Spell_Of_Mastery_Lose.md) — the defeat branch taken at [2028](../../MoM/src/SPLMASTR.c#L2028); **D2** is about returning from it.
- Structural twin: [`Cast_Spell_Of_Mastery`](SPLMASTR-Cast_Spell_Of_Mastery.md) / `_Draw` — the SoM-*started* announcement, shares the `Add_Hidden_Field` and `Load_Palette` shapes.
- [PRD-Endgame-Return-To-Menu-Screen-State.md](../#AI_Plans/PRD-Endgame-Return-To-Menu-Screen-State.md) — the `EOG_HACK` SET/REACT/GUARD model that **D1**/**D2** implement.
- [OverSpel.c:812](../../MoM/src/OverSpel.c#L812) `g_ai_recompute_needed = ST_TRUE;  /* ¿ OGBUG unreachable code ? */` — unreachable in OG for the same reason as **D2**; `EOG_HACK` makes it reachable.
