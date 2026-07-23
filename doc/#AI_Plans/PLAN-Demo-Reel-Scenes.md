# PLAN: Demo Reel — scene list

The shot list for the 2-4 minute ReMoM showcase reel.  Capture/render machinery is done and verified
(see [PRD-Scripted-Demo-Capture.md](PRD-Scripted-Demo-Capture.md)); this is the content.

## Editorial intent

This is a **remake** demo, not a Let's Play.  The viewer's question is *"is this really Master of
Magic?"* — so the reel optimises for **recognisability**, not for depth of play.  Iconic screens,
held long enough to read, in the order a player would meet them.  Open on the title, close on the
victory.  No narration in the first cut; the game's own music carries it.

Target 2:45.  Every scene is independently rendered and re-cuttable — a scene that goes wrong costs
one scene, never the reel.

## Scene list

| # | Scene | Source | Target | Why it's in the reel |
|---|---|---|---|---|
| 1 | Title / intro | free-run `.hms` | 0:15 | Instantly recognisable. Zero input — just let it play. Proves the FLIC/animation path. |
| 2 | New game → wizard select | `.hms` | 0:25 | The portraits, spell-book picks and retorts are pure MoM iconography. |
| 3 | Overland, both planes | `.hms` from save | 0:30 | The heart of the game: terrain, cities, units, and the Arcanus/Myrror switch. |
| 4 | City screen | `.hms` from save | 0:20 | Cityscape with buildings, production, population. Dense and unmistakable. |
| 5 | Spellbook / research | `.hms` from save | 0:15 | The page-turn animation is distinctive and shows off the renderer. |
| 6 | Summoning animation | **DONE** — `06_summon.hms` (real spellbook cast of War Bears) | 0:10 | Cinematic. Reached by actually casting the summon. |
| 7 | Tactical combat | `.hms` from `SAVECMBT.GAM` | 0:30 | The most impressive system, and the hardest to script. |
| 8 | Spell of Mastery → victory → Hall of Fame | **DONE** — `08a_som_begin.hms` + `08b_som_complete.hms` | 0:30 | The payoff, and the reel's long pole. Both halves render. |

Total ≈ 2:55, trimmed to ~2:45 in the cut.

## RULED OUT: using `demo_vga` for scenes 6 and 8

An earlier draft of this plan proposed capturing these two scenes from the `demo_vga` video test
harness, since it already drives those sequences with no scripting. **That is rejected** — see
[BRA-Demo-Reel.md](BRA-Demo-Reel.md) §2.

The harness reaches those states by calling engine functions directly with setup fixups the game never
performs (clearing `magic_master_idx`, hand-allocating `GUI_String_1`, invoking `Spell_Of_Mastery()`
on a save where nobody cast it). The pixels are identical to the game's; the claim they make to a
viewer is not. This reel is marketing — published evidence about the software to an audience that
cannot check it — so every frame must come from the same path a player's would.

Both scenes therefore need real scripted play, and scene 8 needs an endgame save that was genuinely
played to that point. That is the most expensive part of the reel; the cost is accepted.


## `ReMoMber --load <SAVE.GAM>` — IMPLEMENTED

Scenes 3, 4, 5 and 7 each begin in a specific game state.  `--load` now exists in `src/ReMoM.c`,
mirroring HeMoM's, so no scene has to click through the Load Game menu first.

It adds no new load sequence: it sets `g_load_save_gam_name_override` and enters the existing
`scr_Continue` path, so the game performs its own `Load_WZD_Resources()` + `Load_SAVE_GAM()`.

Verified: `--load SAVE1.GAM` lands directly on the overland Main Screen, intro skipped, and loading
two different saves yields visibly different frames.

**Known issue:** `--load SAVETEST.GAM` and `--load combat_s1_baseline.GAM` abort (exit 3) while the
numbered slots load fine — content-dependent, and `SAVETEST.GAM` loads fine via a different path.
Looks pre-existing on `scr_Continue`, surfaced not caused by `--load`.  Use numbered slot saves for
scenes until it is understood.  See BRA §4.


## Starting states

Curated saves already in `assets/`.  Each scene should name the save it needs; if none fits, make one
and commit it beside the script.

| Scene | Candidate save | Note |
|---|---|---|
| 3 Overland | a numbered slot (`SAVE1`..`SAVE9`) | **Not `SAVETEST.GAM`** — it aborts under `--load` (see known issue above). Needs both planes explored enough to be worth showing. |
| 4 City | a numbered slot | Wants a developed city, not an outpost. Likely needs a purpose-made save, played and saved to a slot. |
| 5 Spellbook | any mid-game | Needs a decent spell selection to look impressive. |
| 7 Combat | `SAVECMBT.GAM` / `combat_s1_baseline.GAM` | Purpose-built combat saves exist, with `combat_s1.hms` as a starting point. Note `combat_s1_baseline.GAM` aborts under `--load`; re-save to a numbered slot. |

## Authoring loop (per scene)

1. `ReMoMber.exe --record showcase/NN_name.RMR [--load SAVE.GAM] [--seed1 N]` — play the scene.
2. `rmr2hms showcase/NN_name.RMR showcase/NN_name.hms` — convert to text.
3. **Hand-edit for pacing.** This is the real work. A recording of a human hunting for a button reads
   as hesitant; a demo wants confident moves and deliberate pauses. Coarsen the waits, delete
   fumbles, add comments.
4. `python tools/render_demo.py --scene NN_name --scenario showcase/NN_name.hms --game out/build/MSVC-sdl2-debug/bin/Debug/ReMoMber.exe`
5. Watch it. Repeat from 3 — that iteration is cheap and is the whole point of the format.

Final cut:
```
python tools/render_demo.py --concat "out/demo/[0-9]*.mp4" --output out/demo/reel.mp4
```

## Rules for every scene

- **End by quitting the game.** A scene that exits cleanly finalizes the capture properly; otherwise
  `--max-seconds` kills it and the repair paths have to patch the WAV and video tails.
- **Pin the seed** (`--seed1 N`) on anything touching RNG, so a re-render is identical.
- **Keep scenes short.** Divergence risk grows with length, and a short scene is cheap to re-record.
- **Hold still long enough to read.** Roughly 3 s minimum on any screen with text; the viewer has
  never seen it before, and it is a 320x200 screen scaled up.
- **Prefer motion.** Animation, scrolling, page turns and combat read far better than static screens.

## Decisions taken (see BRA-Demo-Reel.md)

1. **Scene 2 basis** — built on `assets/Stage_0.hms`, which already drives the whole New Game flow with
   coordinates derived from `NewGame.c` field definitions.  Drop its save/quit tail and re-time the
   waits for viewing.  *Which wizard* is still open (Rjak comes free with the script).
2. **Chapter titles** — yes, added in post over the rendered scenes.  Design deferred.
3. **Music** — keep the real per-scene game audio; cross-fade the seams in post.  No single overlaid
   track, so the sound stays tied to what is on screen.
