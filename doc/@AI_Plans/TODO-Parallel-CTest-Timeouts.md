# TODO: Find out why DeMoM tests time out under parallel ctest

## What was seen

Under `ctest -j4`, `DeMoM_NextTurn_Setup` and `DeMoM_AIBehavior_Setup` time out (30 s / 20 s limits). Run one at a time, each finishes in about
0.5–1.5 s. Seen 2026-11-14 on the MSVC Debug build.

## Starting points

- Both run `DeMoM --newgame test_worldgen.ini` in the same working directory, `${DEMOM_TEST_WORKDIR}`, which most DeMoM tests share
  ([tests/CMakeLists.txt:199](../../tests/CMakeLists.txt#L199), [:252](../../tests/CMakeLists.txt#L252)). Both write the same save and log files
  there (SAVE*.GAM, `remom_log_*.txt`).
- The comment at [tests/CMakeLists.txt:252](../../tests/CMakeLists.txt#L252) already says "the suite runs serially; it shares SAVE*.GAM", but
  nothing enforces that for these 14 tests. Only the windowed combat replays take a lock (`RESOURCE_LOCK combat_savegame`,
  [tests/CMakeLists.txt:619](../../tests/CMakeLists.txt#L619)). So `-j` lets the rest collide.
- Questions to answer: is it a real hang (a file held open, a lock, a wait on a file another test is writing) or just slowness under contention?
  Which file do they collide on?

## Ways to fix, once the cause is known

- Give the tests that share the work directory a common `RESOURCE_LOCK`, so ctest never runs them at the same time.
- Or give each fixture chain its own working directory, the way the wild-game test did.
- Then confirm `ctest -j4` passes and record the wall-clock time against a serial run.
