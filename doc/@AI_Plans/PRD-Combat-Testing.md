# PRD — Deterministic End-to-End Combat Testing

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-09
**Companion:** `PLAN-Combat-Testing.md` (codebase survey, tier details, build-out order)

---

## Problem Statement

Combat is the largest and most intricate subsystem in ReMoM — tactical battles, strategic auto-resolve, combat AI, spells, city walls, lairs — and none of it is covered by automated tests. Every change to combat code (reconstruction fixes, STU_LOG migration, spell work) is verified by hand-playing battles, which is slow, unrepeatable, and blind to regressions in the paths not exercised that day. There is no way to state with evidence that a change to combat left every other combat behavior intact.

## Solution

Extend the existing headless test harness (HeMoM) and unit-test target so that complete combats — both tactical and strategic — run deterministically from a pinned random seed with known inputs, and their outputs are checked against curated assertion files, in three tiers:

1. **Kernel tests** (GTest): the attack/defense/damage/resistance math and unit-conversion functions, tested in isolation with synthesized battle units.
2. **Direct-invoke combat runs** (HeMoM `--combat` mode): load a crafted save, seed the RNG at combat entry, invoke the combat dispatcher directly, run the battle to completion with both sides AI-driven, dump the results, assert.
3. **End-to-end trigger tests** (HeMoM scenario scripts): move a stack onto an enemy through the real overland path so combat triggers, resolves, and writes back exactly as in normal play.

All tests run under CTest with the existing fixture pattern and are picked up by the standard HeMoM test invocation. Assertions pin current behavior (characterization tests), including original game bugs — a failure means "behavior changed," never "behavior is wrong."

## User Stories

1. As a ReMoM developer, I want a complete combat to run headlessly from a pinned seed, so that I can reproduce any battle exactly, on any machine, without playing it by hand.
2. As a ReMoM developer, I want strategic (auto-resolve) combat covered by a deterministic test, so that changes to auto-resolve math are caught the moment they alter an outcome.
3. As a ReMoM developer, I want tactical combat covered by an AI-vs-AI test that runs the real battle-screen turn loop, so that the turn sequencing, AI action selection, and winner determination are exercised end to end.
4. As a ReMoM developer, I want the attack, defense, damage, and resistance kernels unit-tested with synthesized battle units, so that I can pinpoint exactly which calculation changed when an integration test fails.
5. As a ReMoM developer, I want the overland-unit-to-battle-unit conversion tested, so that stat, special-ability, level, and item-power application regressions are caught in isolation.
6. As a ReMoM developer, I want unit deployment/placement tested, so that starting-position regressions (including city-wall layouts) are caught.
7. As a ReMoM developer reconstructing code from disassembly, I want characterization tests that pin current behavior including OGBUGs, so that a faithful reconstruction is never "fixed" into unfaithfulness by a well-meaning change.
8. As a ReMoM developer, I want combat results dumped as human-readable key/value text, so that I can diff two runs and see exactly which field diverged.
9. As a ReMoM developer, I want assertion files with per-field opt-outs for known-nondeterministic values, so that one flaky field doesn't force abandoning the whole assertion set.
10. As a ReMoM developer, I want legality/range rules (liveness and bounds) alongside exact-value assertions, so that outputs are checked for being *valid* (no negative HP, winner is a participant, casualties ≤ participants) even in scenarios where exact values aren't pinned yet.
11. As a ReMoM developer, I want a scenario matrix covering open-field fights, city assaults with walls, lair battles, retreat/flee, and the turn-limit timeout, so that each structurally distinct combat path has at least one pinned outcome.
12. As a ReMoM developer, I want combat tests to trigger through the real overland move path in at least one test, so that the combat-entry decision (tactical vs strategic) and the shared post-combat writeback (unit move-in, city ownership, loot, diplomacy) are validated, not just the battle itself.
13. As a contributor, I want combat tests to run in the standard test suite invocation, so that I don't need to know a special command to find out whether I broke combat.
14. As a developer pushing to main, I want combat tests included in the pre-push gate's test stage, so that combat regressions can't land silently.
15. As an AI/computer-player developer, I want deterministic AI-vs-AI battles with dumped per-unit results, so that I can measure the behavioral effect of a combat-AI change as a diff between two seeded runs.
16. As a ReMoM developer, I want scripted human tactical decisions injectable in a test, so that specific manual action sequences (move here, shoot that, cast this) can be regression-tested later without a human at the keyboard.

## Implementation Decisions

- **No new executable target.** The headless harness (HeMoM) already owns launch-to-game-state setup, seed handling, save load/dump, and scenario input injection. It gains a `--combat` mode. A separate binary would duplicate the newgame RNG-parity plumbing and burden both build systems.
- **Kernel tests live in the existing MoM unit-test target**, which already links the game library against the headless platform.
- **The combat dispatcher is the seam.** The dispatcher does all combat setup, then selects tactical (human participant, strategic-only option off) or strategic resolution; shared post-combat writeback follows either. The `--combat` mode invokes the dispatcher directly with prepared state.
- **Determinism contract:** the RNG is a pure function of the global seed; the `--combat` mode re-seeds immediately before combat entry so overland history cannot shift combat rolls. Outcomes remain seed-stream-sensitive to RNG-consuming code changes inside combat itself — an accepted property, shared with the existing worldgen tests.
- **Tactical battles run AI-vs-AI via the existing auto-combat flag**, which drives both sides through the AI turn path with no input; the turn-limit rule (defender wins past the cap) guarantees termination.
- **Scripted human decisions reuse the existing scenario-script input injection** (one action per frame through the same path as record/replay); no new bridge is built.
- **Setup is test data, not machinery:** crafted saves (produced by a seeded newgame plus a positioning script, or checked in) that place an attacker adjacent to a defender. Fixture chaining (Setup produces the save, Run consumes it) follows the existing pattern.
- **New output module: a combat results dump** — sibling of the existing save dump, same key/value text format — capturing winner, per-battle-unit final status/HP/figures/XP, gold reward, buildings lost, city damage, and RNG call count. Written after combat finalization.
- **Record-and-replay (.RMR) is out of the combat-logic loop.** It is frame/coordinate-based and reserved for input-path/pixel regressions; scenario scripts are the semantic input tool.
- **Both build systems** (CMake and Autotools) must list any new sources.
- **Original-game-code edit rules apply:** any hook added inside combat code is comment-marked, and no original code is deleted.

## Testing Decisions

- **A good test here checks external behavior:** the battle's inputs are (save state, seed, scenario) and its outputs are (winner, casualties, unit end-states, rewards, post-combat overland state). Tests assert on those — never on internal call sequences or intermediate globals mid-battle.
- **Characterization, not specification:** expected values are captured from real seeded runs of the current code. OGBUGs are pinned as-is per the project's faithfulness rule.
- **Two assertion styles, both with prior art in the repo:** exact `key = value` field assertions with commented `NONDET` opt-outs (the worldgen/continue-save pattern), and `LIVENESS`/`BOUNDS` range rules (the AI-metrics pattern) for legality properties.
- **Modules under test:** Tier 1 — attack/defense roll kernels, damage application, resistance checks, overland→battle unit conversion, deployment, winner determination. Tier 2 — the full strategic resolver and the full tactical loop (AI-vs-AI), via the dispatcher. Tier 3 — the overland trigger path and shared post-combat writeback.
- **CTest fixtures** chain Setup → Run → Assertions so assertion steps only run after a successful battle run (the existing HeMoM fixture pattern). All new tests carry the HeMoM name prefix so the standard suite invocation includes them; the suite is always run whole.
- **Prior art:** the seeded worldgen smoke/field tests, the continue/next-turn save-assertion tests, the Debug-only AI-behavior metrics test, and the seeded worldgen GTest modules.

## Out of Scope

- **Fixing any combat behavior.** This PRD adds observation, not correction; OGBUGs stay.
- **Byte-level golden saves.** Known invalid due to uninitialized padding regions; curated field assertions only.
- **Record-and-replay CTest wiring.** `.RMR`-based demo/replay testing remains a separate concern.
- **Pixel/rendering verification of the combat screen.** Only logic outcomes are asserted; drawing is exercised (headless) but not checked.
- **Exhaustive spell coverage.** The initial scenario matrix covers structural combat variants; per-spell combat tests are follow-on work as spell walkthroughs complete.
- **Performance testing** of combat resolution.

## Further Notes

- **The one unverified premise** is that the tactical battle screen's allocation/graphics/sound load path runs to completion under the headless platform. Other full screens do; the combat screen never has. The build-out therefore starts with a strategic-only tracer bullet (no battle screen), and headless-tactical is proven (and patched if needed) as its own step before the scenario matrix is built on top of it.
- **Seed-stream sensitivity is a maintenance cost:** any change that adds/removes an RNG draw inside combat shifts every downstream roll, so assertion files for affected scenarios must be re-captured. This is accepted and already familiar from the worldgen tests; the re-seed-at-combat-entry decision confines the blast radius to combat-internal changes.
- Existing half-built scaffolding (the self-contained tactical screen test harnesses wired-but-commented into the dispatcher) is the in-code precedent for direct invocation; the `--combat` mode supersedes it for automated testing but need not remove it.
- The turn-limit rule (defender wins past the cap) doubles as the termination guarantee for AI-vs-AI runs, so no test-side timeout logic is needed beyond CTest's standard per-test timeout.
