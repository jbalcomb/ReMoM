CONQUEST-End_Of_Game_Score.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr107\GAME_OVER.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr107\GAME_OVER.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr107\GAME_DrawScoreBrkdwn.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr107\GAME_DrawScoreBrkdwn.c

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> End_Of_Game_Score()
                |-> End_Of_Game_Score_Draw()   (registered via Assign_Auto_Function, also called each frame in the Esc loop)

---

# `End_Of_Game_Score` + `End_Of_Game_Score_Draw` — Review / Walkthrough

The end-of-game Hall-of-Fame flow: `Resolve_Wizard_Conquest` (win or lose) calls `End_Of_Game_Score`, which loads the HALOFAM art, marks the defeated wizards (all still-active ones if the human cast the Spell of Mastery), draws the score-breakdown screen (`End_Of_Game_Score_Draw`) until Esc, inserts the run into the persistent hi-score table, and shows the Hall of Fame. `End_Of_Game_Score_Draw` computes the final score from spells known, population, conquests, fame, elapsed turns, Spell-of-Mastery, and a difficulty multiplier, drawing each contribution's icon-row and text, then the running-total bar.

| Function | Location | asm | Role |
|---|---|---|---|
| `End_Of_Game_Score` | [SCORE.c:458-575](../../MoM/src/SCORE.c#L458-L575) | `GAME_OVER.asm` (WZD 107p01) | HALOFAM load, HoF images, SoM-winner `Defeated_Wizards` marking, opponent-portrait build, Esc input loop, hi-score upsert (466-byte `magic_set` write to Set file), `Hall_Of_Fame_Screen`, teardown. |
| `End_Of_Game_Score_Draw` | [SCORE.c:579-811](../../MoM/src/SCORE.c#L579-L811) | `GAME_DrawScoreBrkdwn.asm` (WZD 107p02) | Per-frame draw: score-breakdown renderer — spells/pop/conquests/fame/time/SoM contributions with icon rows, difficulty multiplier, running-total bar. Writes `GAME_Total_Score`. |

> **Status: DONE-DONE (2026-07-19) — faithful to the disassembly 1:1.** Both functions match the asm phase-for-phase and arg-for-arg (cross-checked against the Gemini `.c`). No reconstruction errors (no R-items). Non-defect items: **U1** the difficulty-multiplier jump table (`off_87A78` data not in the extraction — case bodies present and in-order, demo-verifiable); **S1** the `cnst_HOTKEY_Esc15[0]` hotkey-arg representation (same convention as the done-done animations); **S2** the `Gradient_Fill` arg-count normalization (OG pushes 5 or 6 args; production signature is 9, padded with `0`/`ST_NULL`); **S3** the faithful-but-dead `colors[4]` local.

## U1 — difficulty-multiplier jump table (data not in the listing)

`switch(_difficulty)` scales the final score ([775-793](../../MoM/src/SCORE.c#L775-L793)): `god_Intro → /2`, `god_Easy → *3/4`, `god_Normal → (none)`, `god_Hard → *2`, `god_Impossible → *3`, `default → (none)`. The asm guards `cmp bx, 4 / ja` (→ default, no change) then `jmp [cs:off_87A78 + bx*2]`. The five case bodies appear in the listing in order (`/2`, `*3/4`, no-op, `*2`, `*3`) and production maps `god_Intro..god_Impossible` (0..4) to the same bodies, which is consistent with an in-order table — but `off_87A78` isn't in the extraction, so the case→body mapping is **not byte-verified**. (Low stakes and demo-checkable: wrong entry = wrong difficulty scaling on the final total.)

## Verified faithful (phase notes)

### `End_Of_Game_Score`
- **Setup** ([468-486](../../MoM/src/SCORE.c#L468-L486)) — `m_new_score = 0` (asm `GAME_Total_Score = 0`); stop sounds / mouse none / deactivate auto-fn / clear fields / fade / `Fill(0,0,319,199,0)` / toggle / page-off; `LBX_Reload(HALOFAM,0)` + `FLIC_Draw(0,0)`; apply/clear/set palette, `Calculate_Remap_Colors`, `Copy_Off_To_Back`, `Copy_Off_To_Page4`, mouse default.
- **String buffers** ([487-488](../../MoM/src/SCORE.c#L487-L488)) — `GUI_String_1 = Near_Allocate_First(100)`; `GUI_String_2 = Near_Allocate_Next(100)` (First then Next — correct, unlike the `Conquest_Animation` R1 bug).
- **HoF images** ([489-493](../../MoM/src/SCORE.c#L489-L493)) — `LBX_Reload(HALOFAM,29)` Time, `_Next(30)` Book, `_Next(31)` Fame, `_Next(32)` SoM, `_Next(capital_race+15)` Pop.
- **Auto-fn** ([494](../../MoM/src/SCORE.c#L494)) — `Assign_Auto_Function(End_Of_Game_Score_Draw, 2)`.
- **SoM-winner marking** ([495-504](../../MoM/src/SCORE.c#L495-L504)) — if `GAME_SoM_Cast_By == _human_player_idx`, mark every still-active fortress's wizard into `Defeated_Wizards` (`Set_Bit_Field_Near`).
- **Opponent portraits** ([505-513](../../MoM/src/SCORE.c#L505-L513)) — for each `Test_Bit_Field_Near(itr, Defeated_Wizards) == 1`, `IMG_GAME_HoF_OppWiz[Conquests_Index++] = LBX_Reload_Next(HALOFAM, wizard_id[itr]+1)`.
- **Esc loop** ([514-533](../../MoM/src/SCORE.c#L514-L533)) — help list; `Mark_Time` / `Clear_Fields` / `Add_Hidden_Field(0,0,319,199, …Esc…, -1)` / `Get_Input`; leave on Esc; draw `End_Of_Game_Score_Draw()` when not leaving; `PageFlip_FX`; `Release_Time(2)`. No `stage`-cap (score screen loops until Esc only — matches asm).
- **Hi-score upsert** ([540-562](../../MoM/src/SCORE.c#L540-L562)) — first slot with `hof_scores[itr] < m_new_score`: shift `[itr..8] → [itr+1..9]` (names via `strcpy`, plus scores/races), insert `m_new_score`/`capital_race`/name, then `fopen(cnst_Set_File3, "wb")` + `fwrite(&magic_set, 466, 1, fp)` + `fclose`, break.
- **Teardown** ([563-574](../../MoM/src/SCORE.c#L563-L574)) — `Hall_Of_Fame_Screen()`; fade; `Load_Palette(0,-1)`; reset cycle; clear/set palette (`0,223`); `Calculate_Remap_Colors`; page-off / `Fill` / toggle / apply; mouse default.

### `End_Of_Game_Score_Draw`
- **Init** ([581-607](../../MoM/src/SCORE.c#L581-L607)) — `Bonus_Pts_String = ": "` (`cnst_HoF_String_0`); `space = " "`; `colors[0..3] = 117+itr` then `colors[0] = 240` (see **S3**); `xstart = 47`; page-off/reset/`Copy_Back_To_Off`; font shadow `(3,4,0,0)`, outline 250, alias 246; `Print_Centered(160, 3, player.name)`.
- **Spells (+1 each)** ([608-643](../../MoM/src/SCORE.c#L608-L643)) — count `spells_list[realm*40 + i] == sls_Known` over `6 × 40`; `score += count`; text row (`Print(xstart,15)`); Book icons `for i < count && i < 50`, `+8`/col, wrap at `i==24` to `xstart+4`, `Draw_Top += 3`.
- **Population (+pop/2)** ([644-681](../../MoM/src/SCORE.c#L644-L681)) — sum `_CITIES[].population` for human-owned; print full pop then halved bonus; `score += pop/2`; Pop icons `for i < pop/2 && i < 100`, `+9`/col, every 25th wrap to `xstart+4` (quotient 1 or 3) else `xstart`, `Draw_Top += 5`.
- **Conquests (+50 each)** ([682-716](../../MoM/src/SCORE.c#L682-L716)) — `Conquest_Count` = set bits in `Defeated_Wizards`; text (`0 → cnst_HoF_String_4`, else count); `score += count*50`; `Print(xstart,78)`; OppWiz portraits `+35`/col.
- **Fame (×2)** ([718-729](../../MoM/src/SCORE.c#L718-L729)) — `Player_Fame(human)`; `score += fame*2`; `FLIC_Draw(xstart,123,Fame)`; `Print(xstart+18,128)`.
- **Time bonus** ([730-758](../../MoM/src/SCORE.c#L730-L758)) — "Reign `turn/12+1400` to " prefix; bonus `= 2000 - turn*2` only if `_FORTRESSES[human].active && (GAME_SoM_Cast_By == -1 || == human)`; if `>= 0` prepend `cnst_HoF_String_1`; `FLIC_Draw(xstart,0x8E,Time)`; `Print(xstart+0x12,0x93)`; `score += bonus`.
- **Spell of Mastery (+250)** ([759-774](../../MoM/src/SCORE.c#L759-L774)) — `250` iff `GAME_SoM_Cast_By == human`; `FLIC_Draw(xstart,0xA1,SoM)`; `Print(xstart+0x12,0xA6)`; `score += bonus`.
- **Difficulty multiplier** ([775-793](../../MoM/src/SCORE.c#L775-L793)) — see **U1**.
- **Total + bar** ([794-810](../../MoM/src/SCORE.c#L794-L810)) — `Print(xstart,0xB4)` the total; `bar = min(score/40, 200)`; four `Gradient_Fill` border segments (see **S2**) + `Fill(48,190,250,196,0)` track + `Fill(49,191,bar+49,195,77)` fill; `m_new_score = score` (asm `GAME_Total_Score = di`).

## Spot notes (representation / style — not behavioral deviations)

- **S1 — hotkey arg representation.** `Add_Hidden_Field(0,0,319,199, …Esc…[0], -1)` passes the char value where the asm pushes `offset cnst_HOTKEY_Esc15`. Same `[0]`-representation convention accepted on the done-done `Win_Animation`/`Conquest_Animation`; accepted, not a divergence.
- **S2 — `Gradient_Fill` arg-count normalization.** The four OG call sites push **5 args** (`add sp, 0Ah`) for the top/left borders and **6 args** (`add sp, 0Ch`) for the bottom/right borders; production's `Gradient_Fill` signature is 9 params, so the reconstruction passes the matched 5/6 args and pads the remainder with `0` / `ST_NULL`. Every OG-supplied coordinate and color matches; the padding is a signature-normalization artifact, not a behavioral change.
- **S3 — dead `colors[4]` local.** `colors` is computed (`117+itr`, then `[0]=240`) but never read in the function; faithful to the asm's identically-unused `UU_colors_array`. Keep as-is (do not remove — it is present in the disassembly).

## Related references

- `…\ovr107\GAME_OVER.asm` + `…\ovr107\GAME_DrawScoreBrkdwn.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` files are a second opinion.
- [`Resolve_Wizard_Conquest`](CONQUEST-Resolve_Wizard_Conquest.md) — sole caller; invokes `End_Of_Game_Score` after the win (`Win_Animation`) or lose (`Lose_Animation`) cut-scene.
- Shares `GUI_String_1`/`GUI_String_2` (`Near_Allocate_First`/`_Next`) and `IMG_GAME_HoF_OppWiz[]` with the conquest cut-scenes; `Hall_Of_Fame_Screen` renders the persisted `magic_set.hof_*` table this function upserts into.
