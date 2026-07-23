# BRA — Demo Reel (marketing video) Architecture Decision

**Status:** Decided (honest-footage-only) — working record, open questions flagged inline
**Date:** 2026-07-21
**Author role:** Business Requirements Analyst (decision record)
**Related:** `PRD-Scripted-Demo-Capture.md` (capture/render machinery), `PLAN-Demo-Reel-Scenes.md` (shot list)

This records what the demo reel *is for*, the constraint that reshaped the plan, and the decisions
taken. It is a decision record, not an implementation plan.

---

## 1. What this is

A 2–4 minute video for **marketing** — published to YouTube to show that ReMoM is Master of Magic.
The audience's question is *"is this really the game?"* Everything below follows from that being a
**claim about the software**, made to strangers, that they cannot verify themselves.

This is categorically different from the testing work. `BRA-Endgame-Testing.md` and
`BRA-Combat-Testing.md` govern artefacts whose audience is *us*, where scaffolding is legitimate
because everyone reading the result knows what it is. Here the audience is outside the project and is
being asked to believe something.

---

## 2. DECIDED — the footage must be the real game, played

**The reel must not contain a single frame produced by the `demo/` video test harness (`demo_vga`).**

The harness was built to observe rendering paths in isolation. It reaches that state by calling
engine functions directly and applying setup fixups the game never performs — `effect_somwin.c`
clears `magic_master_idx`, hand-allocates `GUI_String_1` (a buffer every real screen allocates for
itself), and invokes `Spell_Of_Mastery()` on a save where nobody cast it. The pixels it emits are
identical to the game's. **The claim they make is not.**

Filmed and captioned "Spell of Mastery victory", that footage asserts *the game reaches this state and
plays this sequence*. What was demonstrated is that the rendering function draws correctly when called
with hand-arranged arguments. Those are different claims, and only the first is what a viewer takes
away. The gap is invisible in the footage and undetectable by the audience — which is precisely what
makes it misrepresentation rather than shorthand.

This is not a statement about how impressive the harness is. It is excellent at its job, it found a
real 16-bit-wraparound reconstruction bug in `Spell_Of_Mastery`, and it stays. Its output is simply
not admissible as evidence to strangers.

**Cost of the decision, accepted:** scenes 6 (summoning) and 8 (Spell of Mastery → victory → Hall of
Fame) were planned as free `demo_vga` captures. They now require real scripted play — scene 8 in
particular means genuinely reaching an endgame. That is the most expensive scene in the reel and the
decision is to pay for it.

### 2.1 The rule this generalises to
> **Footage published as evidence about the software must be produced by the same path a player's
> would be.** Real entry points, real saves, real input. No test harness, no direct function calls,
> no state fixups, no debug builds standing in for the shipped one.

Corollaries worth stating, because they are the ways this rule gets bent without anyone lying:
- **State setup is allowed; path fabrication is not.** *(Amended 2026-07-21 — the original wording of
  this corollary, "if a save cannot be reached by playing, it cannot be filmed", was too broad and is
  withdrawn.)* The test is **not** how the starting state arose. It is:
  1. is the state one a real game **could legitimately hold**, and
  2. does **everything from that point to the final frame** execute as the game's own logic?

  Arranging preconditions and then letting the game play it out is staging a scene. Calling the
  presentation function directly — what `demo_vga` does — is faking the footage. Only the second
  misrepresents. Guardrails in §9.
- **Speed changes must be honest.** Cuts and elisions are normal editing. Silently speeding up
  gameplay to make it look faster is not. If the reel timelapses, it should look like a timelapse.
- **Bugs stay in.** If a scene shows a glitch, re-record it or fix the bug — do not patch it out of
  the video.
- **Same build a player gets.** Currently the render uses `MSVC-sdl2-debug` (see §4). Once there is a
  shipping build, the reel should come from something equivalent to it.

---

## 3. DECIDED — scene 2 derives from `Stage_0.hms`

Scene 2 (new game / wizard select) is built on `assets/Stage_0.hms` rather than recorded from
scratch. That script already drives the full New Game flow — game options, Custom wizard, portrait,
name, retorts, spell books, race, banner, home-city naming — with coordinates derived from the actual
`Add_Hidden_Field` definitions in `NewGame.c` and documented inline. It also already ends by quitting,
which satisfies the clean-finalize rule.

It is a *basis*, not a drop-in: it exists to reproduce a specific PRNG-matchup wizard (Rjak / Orc /
Yellow), and it saves to a slot mid-script. The demo version drops the save/quit tail and re-times
the waits for viewing rather than for machine speed.

**Open:** which wizard the reel should pick. Rjak comes free with `Stage_0.hms`; Merlin is MoM's
default face; Sss'ra or Horus show the portrait art better. Deferred to the human.

---

## 4. DECIDED — `ReMoM --load <SAVE.GAM>` (implemented)

Scenes 3, 4, 5 and 7 each need a specific starting state. Without a load flag every scene had to
click through the Load Game menu first — the same fragile ~5-second prefix on four scenes, coupled to
save-slot ordering and menu coordinates.

Implemented in `src/ReMoM.c`, mirroring HeMoM's `--load`. It deliberately adds **no new load
sequence**: it sets `g_load_save_gam_name_override` and enters the existing `scr_Continue` path, which
performs the game's own `Load_WZD_Resources()` + `Load_SAVE_GAM()`. This matters under §2 — a bespoke
loader would be another way the filmed path diverges from a player's.

Verified: `--load SAVE1.GAM` and `--load SAVE9.GAM` land directly on the overland Main Screen with
different game state (379 sampled byte differences between captured frames), confirming the filename
is honoured and the intro is skipped.

**Found while verifying — open issue, not a blocker.** `--load SAVETEST.GAM` and
`--load combat_s1_baseline.GAM` **abort with exit code 3** (`abort()`, no stderr output), while
`SAVE1.GAM` and `SAVE9.GAM` load fine. All four files are 123,300 bytes. The failure is
content-dependent, not filename-dependent, and `SAVETEST.GAM` loads successfully through a different
path (`Load_SAVE_GAM(ST_UNDEFINED)` + `Loaded_Game_Update`, as `demo_vga score`/`somwin` do). This
looks like a **pre-existing engine issue on the `scr_Continue` path**, surfaced rather than caused by
`--load`. It does not block the reel (the numbered slots work), but it should be understood before
anyone trusts `--continue` generally.

---

## 5. DECIDED — music: accept per-scene audio, cross-fade the seams

Each scene captures whatever its screen plays, so the reel would cut abruptly at every boundary. The
decision is to keep the real per-scene game audio and cross-fade across the cuts in post, rather than
muting the game and laying one continuous track over everything.

This is consistent with §2: the audio a viewer hears is the audio the game produced for the thing on
screen. A single overlaid track would decouple sound from what is being demonstrated.

---

## 6. DECIDED — chapter titles: yes, worked out later

Titles/chapter cards are wanted. They are a post step, added over the rendered scenes; nothing is
drawn in-engine. Design deferred.

---

## 7. Consequences for the shot list

`PLAN-Demo-Reel-Scenes.md` is amended:
- Scenes 6 and 8 change source from `demo_vga` to real scripted play.
- Scene 8 becomes the reel's long pole and needs a save legitimately played to the brink of a
  Spell of Mastery victory.
- The proposal to add `--capture` to `demo/demo_main.c` is **dropped for reel purposes.** (It may
  still be independently worth doing for *testing* — recording a harness run to compare renders
  frame-by-frame is a legitimate internal use. That is a separate decision under the testing BRAs,
  and any such capture must never be cut into published footage.)

---

## 8. Open questions

1. **Which wizard for scene 2** (§3).
2. ~~How is the scene-8 endgame save produced?~~ **Answered — see §9** (DBG state patching).
3. ~~Does the reel show a defeat as well as a victory?~~ **Answered — see §10**: record every endgame
   path, then cut from the set.
4. **Is a debug build acceptable for the first cut?** Currently required, because the Win32 backend is
   silent and the SDL2 build is what has audio. Acceptable for now; revisit against §2's
   "same build a player gets" once a shipping configuration exists.

---

## 9. DECIDED — scene 8 is set up by DBG state patching, not a hand-played save

Reaching a Spell of Mastery victory by playing takes hours, and the resulting save is a one-off that
nobody can reproduce. Instead, patch the *preconditions* after the save loads and let the game play
the endgame out for real.

**Mechanism (verified to exist).** `TST_Patch_Game_Data()` — `STU/src/STU_TST.c`, called from
`Loaded_Game_Update()` at [LoadScr.c:874](../../MoM/src/LoadScr.c#L874), i.e. immediately after a save
load, which is exactly the right seam. Helpers already present but commented out:
`TST_Cheat_Power(player_idx)` (STU_TST.c:18) and `TST_Learn_Spell(player_idx, spell_idx)`
(STU_TST.c:39).

**Why this satisfies §2.** Patching "this wizard knows Spell of Mastery and has the power to cast it"
sets a state a real game could hold. Everything after — the casting UI, turn processing, the endgame
branch, the animations, the score screen — is the game's own code doing its own work. Contrast
`demo_vga`, which calls `Spell_Of_Mastery()` directly with hand-allocated buffers and never runs the
logic that decides the game is over. The first is staging; the second is faking.

**Guardrails (binding):**
1. **Reachable state only.** Values a legitimate game could hold. Giving a wizard a spell and mana is
   fine; impossible states are not.
2. **Patch at load only, never mid-sequence.** The patch happens once, before the scene starts. No
   nudging state while the camera rolls.
3. **No skipping logic between state and footage.** The endgame must be *decided* by the game. Never
   call `Spell_Of_Mastery()`, `Win_Animation()` or any presentation function directly.
4. **Patches are demo/test scaffolding and must never ship.** See the blocker below.

### 9.1 RESOLVED — inert by default, not debug-gated

An earlier draft of this section called for gating `TST_Patch_Game_Data()` behind `STU_DEBUG`, on the
grounds that demo scaffolding must not ship. **That was wrong and is withdrawn.** The reel is filmed
from the **Release** binary an end user downloads (§2.1, "same build a player gets"), so a
`STU_DEBUG` gate would mean the setup never runs in the build being filmed.

The real resolution: **the capability ships and is inert by default.** `g_tst_patch_scenario` is
`NULL` unless ReMoM's `--patch <name>` names a scenario; with no flag the function returns having
touched nothing. This is exactly how `--capture`, `--load`, `--record`, `--replay` and `--scenario`
already behave — all present in Release, all doing nothing unless invoked.

*Verified:* a run without `--patch` produces zero `[TST]` log lines and changes no game state; a run
with `--patch som_ready` applies and logs the scenario.

**Accepted consequence:** a curious player can pass `--patch` and cheat. That is a cheat facility,
not a correctness bug, and it is the price of filming the same binary players get. If it ever becomes
unacceptable the fix is to strip scenarios from release builds — not to debug-gate the mechanism,
which would break the filming requirement.

Mechanics and the rules for writing scenarios: [Scenario-Patching.md](../%23Devel/Scenario-Patching.md).


---

## 10. DECIDED — record every endgame path; cut the reel from that set

Record all of them, rather than only what the reel ships:

| Path | Trigger | Note |
|---|---|---|
| Win — magic | Human casts Spell of Mastery | `Spell_Of_Mastery` → `Win_Animation` → `End_Of_Game_Score` |
| Lose — magic | AI casts Spell of Mastery | `Spell_Of_Mastery_Lose` → `Lose_Animation` → score |
| Win — military | Human eliminates all rival wizards | The conquest path, not the spell path |
| Lose — military | Human eliminated / permanently banished | |
| Banish → return | Human banished, then completes Spell of Return | Not an endgame — a *recovery*. Arguably the most interesting of the five, and the least seen. |

**Two reasons to record all five even though the reel ships one or two:**

1. **The reel should be cut from a surplus.** "We'll have to see how it feels" is exactly right — five
   endgame sequences will not fit 2:45, and which one lands best is not knowable in advance. Record
   the set, choose in the edit.
2. **They double as endgame *presentation* test artifacts.** `BRA-Endgame-Testing.md` splits the
   endgame into logic (screen-free, assertable) and presentation (cutscenes, "verified by watching,
   not by asserting field values"). These recordings *are* the watching half, in reproducible form —
   a scripted, re-renderable capture of every endgame cutscene. That reframes the cost: this is not
   reel overhead, it is a testing deliverable that the reel happens to draw from.

**Consequence:** the endgame recordings should be driven from the same `.hms` + `--load` + DBG-patch
machinery as the rest, and kept in `showcase/` alongside the reel scenes even though most will not
appear in the published video.

**Open:** whether the banish → Spell of Return path can be set up by patching alone, or needs a
genuine banishment first. Unknown until someone tries it.
