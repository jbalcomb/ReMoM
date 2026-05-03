# Test Assertion Files — Workflow

This page describes how the integration tests' field-level assertions are
maintained, and what to do when intentional code changes invalidate them.

## What's an assertion file

An "assertion file" is a curated list of `field = expected_value` lines that
[check_save_fields.exe](../tests/check_save_fields.c) compares against a save
dump.  Assertion files live in `tests/assert_*.txt`.  Each one targets a
specific test scenario:

| File | Used by | What it asserts |
|---|---|---|
| `assert_worldgen.txt`             | `HeMoM_WorldGen_Fields`         | Post-newgame state for seed=12345 |
| `assert_continue_save.txt`        | `HeMoM_Continue_Assertions`     | Post-`test_continue_save.hms` state |
| `assert_continue_next_turn.txt`   | `HeMoM_NextTurn_Assertions`     | Post-NextTurn state |
| `assert_ai_5turns.txt`            | `HeMoM_AIBehavior_Evaluate`     | AI metrics CSV (separate format) |

The assertion file is *the test's contract* — only fields listed there are
checked.  Anything not listed is intentionally unconstrained.

## Why not just diff the whole save?

The save format is fixed-size and reserves slots/padding for content that
isn't populated yet (unused trailing hero/item/unit slots, struct padding
between fields).  Those bytes come from whatever the OS handed back at
allocation time and vary run-to-run.  A whole-file or whole-field diff would
flag every one of those bytes as a regression — a false positive.

Curated assertions check only the fields the game actually reads back.

## When to refresh assertions

Refresh after intentional changes to:

- world-gen logic
- AI decision-making
- per-turn computation
- a scenario file (changing what the artificial human player does)
- the save format itself (rare, but it has happened)

Do **not** refresh just to make a failing test pass.  A test failure is
either a real regression or a stale assertion; figure out which before
acting.  If it's a regression, fix the code; if it's stale, refresh.

## Single-run refresh: `tools/refresh_asserts.py`

For deterministic assertions — fields whose values are stable across runs
of the same scenario.  Most assertions fall here.

```
python tools/refresh_asserts.py \
    --dump=path/to/SAVE_X.txt \
    --asserts=tests/assert_FOO.txt \
    --in-place
```

This rewrites the right-hand sides only.  Comments, blank lines, ordering,
and line endings are preserved.  Fields named in the assertion file but not
present in the dump are emitted with a `MISSING_FROM_DUMP` marker so the
loss is visible in `git diff`.

## Multi-run refresh: `tools/find_stable_fields.py`

For tests where some fields are non-deterministic across runs (e.g. AI
research-spell picks read run-to-run-varying memory).  Pass dumps from
multiple independent runs and the tool keeps only fields whose values are
identical across all of them.  Varying fields get commented out with a
`# NONDET` marker that records what was observed:

```
python tools/find_stable_fields.py \
    --dumps=run1.txt,run2.txt,run3.txt,run4.txt,run5.txt \
    --asserts=tests/assert_FOO.txt \
    --in-place
```

Recommended `N >= 5` to give the comparator enough samples to distinguish
"truly stable" from "got lucky on N=2 runs."

NONDET output looks like:
```
# NONDET player[0].researching_spell_idx -- observed values across 5 runs: 41, 81, 43, 2, 82  (set aside until determinism is restored)
# player[0].researching_spell_idx = 41
```

The original assertion is kept *commented out* so it can be re-enabled
later if the underlying determinism gets fixed.  The check_save_fields tool
ignores comment lines — those fields no longer constrain the test.

## Per-test wrapper scripts

Wrappers under `tools/refresh_assertions/` package the build-run-dump-refresh
chain for a specific test.  Currently:

| Wrapper | What it does |
|---|---|
| `refresh_next_turn.sh [N=5]` | Builds HeMoM, runs the NextTurn scenario N times, refreshes `assert_continue_next_turn.txt` keeping only the stable fields |

Add more wrappers as new tests need them, following the same pattern.

## Process — full checklist

1. **Decide whether to refresh.**  Look at the diff between actual and
   expected; convince yourself this is intentional drift, not a regression.
2. **Run the appropriate refresh tool.**  Single-run for deterministic
   tests, multi-run wrapper for the non-deterministic ones.
3. **Inspect `git diff -- tests/assert_*.txt`.**  Every changed line must
   make sense.  Pay special attention to:
   - Fields that flipped from a sensible value to `MISSING_FROM_DUMP` —
     usually means a field was renamed in the dump, or the dump section is
     no longer being emitted.
   - New `# NONDET` lines — usually means a field that *was* deterministic
     before is now subject to run-to-run variation.  That's worth
     investigating before accepting.
4. **Verify the test passes** at least 3 times in a row (use ctest's
   `--repeat until-pass:N` or just rerun manually).
5. **Commit** with a message that names what changed and why:
   ```
   refresh assertions for X after Y change
   ```
   Reference the source-tree commit that justified the refresh, so future
   readers can trace the reason back.

## Determinism caveat

The refresh tools don't fix the underlying non-determinism — they document
it.  Every `# NONDET` entry is technically lost test coverage.  The right
long-term fix is to make those fields deterministic (zero-init the relevant
allocations, or only serialize the in-use range of each pool), at which
point the commented-out assertions can be reactivated.

See also:
- `tools/regenerate_goldens.sh` — regenerates the binary `.GAM` golden plus
  its text dump, used by `HeMoM_WorldGen_Golden` (currently DISABLED — the
  binary comparison was found to be non-determinism-sensitive even after
  filtering).  Kept for diagnostic dump generation.
