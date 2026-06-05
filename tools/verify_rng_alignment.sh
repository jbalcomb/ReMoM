#!/usr/bin/env bash
#
# verify_rng_alignment.sh — Layer-A determinism check for HeMoM --newgame.
#
# What it does:
#   1. Builds HeMoM under clang-debug (Linux) or MSVC-headless-debug (Windows
#      via Git Bash / MSYS).
#   2. Runs HeMoM in --newgame mode with --seed ${SEED}, capturing stderr.
#   3. Filters the [RNG-CALL] stream emitted by Random_at()
#      (MoX/src/random.c:318-341) into a clean trace file.
#   4. If a baseline ReMoM trace exists at the expected path, diffs the two
#      traces and reports the first divergent Random() call site.
#      If no baseline exists, prints the steps to capture one.
#
# Why this works:
#   Every Random() call is the dominant source of game-state divergence after
#   the seed is set.  If the per-call seed-before/seed-after/result triple
#   matches between ReMoM-via-menus and HeMoM-headless, the worlds are
#   identical by construction.  See doc/Devel-HeMoM-Newgame-Path-Alignment.md
#   for the full audit.
#
# Usage:
#   cd /home/jbalcomb/STU/devel/ReMoM
#   ./tools/verify_rng_alignment.sh                 # default seed=12345
#   ./tools/verify_rng_alignment.sh 4242            # custom seed
#   SEED=4242 ./tools/verify_rng_alignment.sh       # equivalent
#
# Outputs (under the build's bin dir):
#   hemom_rng_seed${SEED}.trace.raw   — full HeMoM stderr capture
#   hemom_rng_seed${SEED}.trace       — filtered [RNG-CALL] stream
#
# Exit codes:
#   0  HeMoM ran and (if baseline present) trace matched
#   1  HeMoM build or run failure
#   2  Trace divergence vs baseline (first divergent line printed)
#   3  No baseline yet — see printed instructions for capture

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SEED="${1:-${SEED:-12345}}"

# === Platform selection =====================================================
# Match the existing tools/ convention: Linux uses clang-debug, Windows uses
# MSVC-headless-debug.  If both exist, prefer the one already built.
if [ -d "${REPO_ROOT}/out/build/MSVC-headless-debug/bin/Debug" ]; then
    BUILD_PRESET="MSVC-headless-debug"
    BUILD_DIR="${REPO_ROOT}/out/build/${BUILD_PRESET}"
    BIN_DIR="${BUILD_DIR}/bin/Debug"
    HEMOM_EXE="${BIN_DIR}/HeMoM.exe"
    CMAKE_CONFIG="--config Debug"
elif [ -d "${REPO_ROOT}/out/build/clang-debug/bin/Debug" ] || command -v clang >/dev/null 2>&1; then
    BUILD_PRESET="clang-debug"
    BUILD_DIR="${REPO_ROOT}/out/build/${BUILD_PRESET}"
    BIN_DIR="${BUILD_DIR}/bin/Debug"
    HEMOM_EXE="${BIN_DIR}/HeMoM"
    CMAKE_CONFIG=""
else
    echo "FAIL: no usable build preset found (need clang-debug or MSVC-headless-debug)"
    exit 1
fi

ASSETS="${REPO_ROOT}/assets"
TESTS_DIR="${REPO_ROOT}/tests"
NEWGAME_CONFIG="${ASSETS}/test_worldgen.ini"

# Baseline lives under tests/ alongside the world-gen goldens.
BASELINE_TRACE="${TESTS_DIR}/baseline_rng_seed${SEED}.trace"

# Outputs land in the build's bin dir, mirroring regenerate_goldens.sh style.
HEMOM_TRACE_RAW="${BIN_DIR}/hemom_rng_seed${SEED}.trace.raw"
HEMOM_TRACE="${BIN_DIR}/hemom_rng_seed${SEED}.trace"

echo "=== Step 1/4: Build HeMoM (${BUILD_PRESET}) ==="
cmake --build --preset "${BUILD_PRESET}" --target HeMoM ${CMAKE_CONFIG} >/dev/null
if [ ! -x "${HEMOM_EXE}" ]; then
    echo "FAIL: HeMoM binary not found at ${HEMOM_EXE}"
    exit 1
fi
echo "  built: ${HEMOM_EXE}"

echo
echo "=== Step 2/4: Run HeMoM --newgame --seed ${SEED} ==="
if [ ! -f "${NEWGAME_CONFIG}" ]; then
    echo "FAIL: newgame config not found at ${NEWGAME_CONFIG}"
    exit 1
fi
cd "${BIN_DIR}"
rm -f SAVE9.GAM SAVE9.txt "${HEMOM_TRACE_RAW}" "${HEMOM_TRACE}"
# stderr carries the [RNG-CALL] stream; stdout is regular logging.
"${HEMOM_EXE}" --newgame "${NEWGAME_CONFIG}" --seed "${SEED}" \
    >/dev/null 2> "${HEMOM_TRACE_RAW}" || {
    echo "FAIL: HeMoM exited non-zero"
    echo "  last 20 lines of stderr:"
    tail -n 20 "${HEMOM_TRACE_RAW}" | sed 's/^/    /'
    exit 1
}
if [ ! -f SAVE9.GAM ]; then
    echo "FAIL: HeMoM did not produce SAVE9.GAM"
    exit 1
fi
echo "  produced: SAVE9.GAM"

echo
echo "=== Step 3/4: Extract [RNG-CALL] trace ==="
grep '^\[RNG-CALL\]' "${HEMOM_TRACE_RAW}" > "${HEMOM_TRACE}" || true
CALL_COUNT=$(wc -l < "${HEMOM_TRACE}")
if [ "${CALL_COUNT}" -eq 0 ]; then
    echo "FAIL: no [RNG-CALL] lines captured"
    echo "  Check that Random_at() tracing is wired up and --seed was honored."
    echo "  See MoX/src/random.c:318-341 ('if (_cmd_line_seed != 0)')."
    exit 1
fi
echo "  captured ${CALL_COUNT} Random() calls → ${HEMOM_TRACE}"

echo
echo "=== Step 4/4: Compare against baseline ==="
if [ ! -f "${BASELINE_TRACE}" ]; then
    cat <<EOF
  No baseline trace yet at:
    ${BASELINE_TRACE}

  To capture one from ReMoM (one-time, then replayable):
    1. Build ReMoMber under the same preset as HeMoM.
    2. Decide on a canonical menu-click sequence that matches:
         ${NEWGAME_CONFIG}
       (same wizard / race / banner / difficulty / landsize / magic / opponents).
    3. Record the sequence:
         ./ReMoMber --seed ${SEED} --record menu_baseline.RMR
       and click through the new-game menus; quit after SAVE9.GAM is written.
    4. Replay it with stderr capture to produce the baseline:
         ./ReMoMber --seed ${SEED} --replay menu_baseline.RMR 2> baseline.raw
         grep '^\\[RNG-CALL\\]' baseline.raw > ${BASELINE_TRACE}
    5. Commit both menu_baseline.RMR (under assets/) and ${BASELINE_TRACE#${REPO_ROOT}/}.
    6. Re-run this script — it will compare automatically from now on.

  HeMoM trace ready for inspection at:
    ${HEMOM_TRACE}
EOF
    exit 3
fi

BASE_COUNT=$(wc -l < "${BASELINE_TRACE}")

# HeMoM exits at Save_SAVE_GAM(8); ReMoM continues past that into the Main
# Screen and runs PreInit_Overland -> Player_Research_Spells (~240 extra
# Random calls at Spells128.c:121).  So a passing HeMoM trace is a strict
# line-for-line PREFIX of the baseline, not the full thing.
if [ "${CALL_COUNT}" -gt "${BASE_COUNT}" ]; then
    echo "  FAIL: HeMoM made more Random() calls than the baseline"
    echo "    baseline calls: ${BASE_COUNT}"
    echo "    hemom calls:    ${CALL_COUNT}  (over by $((CALL_COUNT - BASE_COUNT)))"
    echo "  This means HeMoM is drawing Random() in a code path the baseline"
    echo "  doesn't reach — investigate the call sites past line ${BASE_COUNT}."
    exit 2
fi

BASELINE_PREFIX="${BIN_DIR}/baseline_rng_seed${SEED}.trace.prefix"
head -n "${CALL_COUNT}" "${BASELINE_TRACE}" > "${BASELINE_PREFIX}"

if diff -q "${BASELINE_PREFIX}" "${HEMOM_TRACE}" >/dev/null; then
    TAIL_COUNT=$((BASE_COUNT - CALL_COUNT))
    if [ "${TAIL_COUNT}" -eq 0 ]; then
        echo "  PASS: HeMoM trace matches baseline exactly (${CALL_COUNT} calls)"
    else
        echo "  PASS: HeMoM trace is a strict prefix of baseline"
        echo "    matched calls:        ${CALL_COUNT}"
        echo "    baseline post-save:   ${TAIL_COUNT} extra calls (ReMoM Main Screen / PreInit_Overland — HeMoM exits earlier)"
    fi
    rm -f "${BASELINE_PREFIX}"
    exit 0
fi

# Prefix differs — that's a real divergence inside HeMoM's range.
echo "  FAIL: trace divergence within HeMoM's range"
echo "    baseline calls: ${BASE_COUNT}"
echo "    hemom calls:    ${CALL_COUNT}"
echo
echo "  First divergent line:"
diff "${BASELINE_PREFIX}" "${HEMOM_TRACE}" | head -n 8 | sed 's/^/    /'
echo
echo "  Full diff:"
echo "    diff ${BASELINE_PREFIX} ${HEMOM_TRACE}"
exit 2
