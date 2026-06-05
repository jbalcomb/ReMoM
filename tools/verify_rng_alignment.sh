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
#   hemom_seed${SEED}_stderr.log  — full HeMoM stderr capture
#   hemom_seed${SEED}_rng.log     — filtered [RNG-CALL] stream
#
# Exit codes:
#   0  HeMoM ran and (if baseline present) trace matched
#   1  HeMoM build or run failure
#   2  Trace divergence vs baseline (first divergent line printed)
#   3  No baseline yet — see printed instructions for capture
#   4  Baseline is stale — source has been modified since it was captured;
#      see printed instructions for re-capture (manual or via --replay)

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SEED="${1:-${SEED:-12345}}"

# cmake --build --preset reads CMakePresets.json from CWD, not from the source
# tree.  Run from the repo root so the script works regardless of where the
# user invoked it from.
cd "${REPO_ROOT}"

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
BASELINE_RNG_LOG="${TESTS_DIR}/baseline_seed${SEED}_rng.log"

# Outputs land in the build's bin dir, mirroring regenerate_goldens.sh style.
HEMOM_STDERR_LOG="${BIN_DIR}/hemom_seed${SEED}_stderr.log"
HEMOM_RNG_LOG="${BIN_DIR}/hemom_seed${SEED}_rng.log"

echo "=== Step 1/5: Build HeMoM (${BUILD_PRESET}) ==="
cmake --build --preset "${BUILD_PRESET}" --target HeMoM ${CMAKE_CONFIG} >/dev/null
if [ ! -x "${HEMOM_EXE}" ]; then
    echo "FAIL: HeMoM binary not found at ${HEMOM_EXE}"
    exit 1
fi
echo "  built: ${HEMOM_EXE}"

echo
echo "=== Step 2/5: Run HeMoM --newgame --seed ${SEED} ==="
if [ ! -f "${NEWGAME_CONFIG}" ]; then
    echo "FAIL: newgame config not found at ${NEWGAME_CONFIG}"
    exit 1
fi
cd "${BIN_DIR}"
rm -f SAVE9.GAM SAVE9.txt "${HEMOM_STDERR_LOG}" "${HEMOM_RNG_LOG}"
# stderr carries the [RNG-CALL] stream; stdout is regular logging.
"${HEMOM_EXE}" --newgame "${NEWGAME_CONFIG}" --seed "${SEED}" \
    >/dev/null 2> "${HEMOM_STDERR_LOG}" || {
    echo "FAIL: HeMoM exited non-zero"
    echo "  last 20 lines of stderr:"
    tail -n 20 "${HEMOM_STDERR_LOG}" | sed 's/^/    /'
    exit 1
}
if [ ! -f SAVE9.GAM ]; then
    echo "FAIL: HeMoM did not produce SAVE9.GAM"
    exit 1
fi
echo "  produced: SAVE9.GAM"

echo
echo "=== Step 3/5: Extract [RNG-CALL] log ==="
grep '^\[RNG-CALL\]' "${HEMOM_STDERR_LOG}" > "${HEMOM_RNG_LOG}" || true
CALL_COUNT=$(wc -l < "${HEMOM_RNG_LOG}")
if [ "${CALL_COUNT}" -eq 0 ]; then
    echo "FAIL: no [RNG-CALL] lines captured"
    echo "  Check that Random_at() tracing is wired up and --seed was honored."
    echo "  See MoX/src/random.c:318-341 ('if (_cmd_line_seed != 0)')."
    exit 1
fi
echo "  captured ${CALL_COUNT} Random() calls → ${HEMOM_RNG_LOG}"

echo
echo "=== Step 4/5: Verify baseline freshness ==="
RMR_FILE="${ASSETS}/menu_baseline_seed${SEED}.RMR"

# Locate ReMoMber.  On Linux clang-debug it shares the bin dir with HeMoM.
# On Windows MSVC-headless-debug only builds HeMoM, so fall back to MSVC-debug
# where the capture script keeps ReMoMber.
REMOM_EXE=""
REMOM_PRESET=""
for candidate in \
    "${BIN_DIR}/ReMoMber.exe|${BUILD_PRESET}" \
    "${BIN_DIR}/ReMoMber|${BUILD_PRESET}" \
    "${REPO_ROOT}/out/build/MSVC-debug/bin/Debug/ReMoMber.exe|MSVC-debug" ; do
    path="${candidate%|*}"
    preset="${candidate##*|}"
    if [ -f "${path}" ]; then
        REMOM_EXE="${path}"
        REMOM_PRESET="${preset}"
        break
    fi
done

if [ -f "${BASELINE_RNG_LOG}" ] && [ -n "${REMOM_EXE}" ]; then
    # Rebuild ReMoMber (incremental — no-op if up to date) so its mtime
    # reflects the current source.  If the rebuild touches the binary, mtime
    # advances and the staleness check below will fire.
    # cmake --build --preset reads CMakePresets.json from CWD; Step 2 cd'd into
    # BIN_DIR, so run from REPO_ROOT in a subshell.
    ( cd "${REPO_ROOT}" && cmake --build --preset "${REMOM_PRESET}" --target ReMoMber ${CMAKE_CONFIG} ) >/dev/null 2>&1 \
        || echo "  WARNING: failed to rebuild ReMoMber for staleness check; continuing with existing binary"

    if [ "${REMOM_EXE}" -nt "${BASELINE_RNG_LOG}" ]; then
        echo "  FAIL: baseline is stale — ReMoMber binary is newer than the baseline."
        echo "         The baseline was captured against an earlier build of ReMoMber, so"
        echo "         compile-time __LINE__ values (and possibly Random() call counts) no"
        echo "         longer match the current binary.  Re-capture before comparing."
        echo "    ReMoMber: $(date -r "${REMOM_EXE}" '+%Y-%m-%d %H:%M:%S')"
        echo "    baseline: $(date -r "${BASELINE_RNG_LOG}" '+%Y-%m-%d %H:%M:%S')"
        echo
        echo "  To re-capture the baseline:"
        if [ -f "${RMR_FILE}" ]; then
            echo "    A — replay the recorded menu sequence (no clicking):"
            echo "      cd \"${BIN_DIR}\""
            echo "      SDL_VIDEODRIVER=offscreen \"${REMOM_EXE}\" --seed ${SEED} --replay \"${RMR_FILE}\" 2> baseline_stderr.log"
            echo "      grep '^\\[RNG-CALL\\]' baseline_stderr.log > \"${BASELINE_RNG_LOG}\""
            echo
            echo "    B — re-record from scratch (manual menu clicking):"
            echo "      ./tools/capture_remom_rng_baseline.sh ${SEED}"
        else
            echo "    No RMR replay file at ${RMR_FILE#${REPO_ROOT}/}."
            echo "    Capture a fresh baseline (manual menu clicking):"
            echo "      ./tools/capture_remom_rng_baseline.sh ${SEED}"
        fi
        exit 4
    fi
    echo "  baseline is current (ReMoMber binary not newer than baseline)"
elif [ -f "${BASELINE_RNG_LOG}" ]; then
    echo "  WARNING: ReMoMber binary not found; cannot verify baseline freshness."
    echo "           Searched: ${BIN_DIR}, ${REPO_ROOT}/out/build/MSVC-debug/bin/Debug."
fi

echo
echo "=== Step 5/5: Compare against baseline ==="
if [ ! -f "${BASELINE_RNG_LOG}" ]; then
    cat <<EOF
  No baseline log yet at:
    ${BASELINE_RNG_LOG}

  To capture one from ReMoM (one-time, then replayable):
    1. Build ReMoMber under the same preset as HeMoM.
    2. Decide on a canonical menu-click sequence that matches:
         ${NEWGAME_CONFIG}
       (same wizard / race / banner / difficulty / landsize / magic / opponents).
    3. Record the sequence:
         ./ReMoMber --seed ${SEED} --record menu_baseline.RMR
       and click through the new-game menus; quit after SAVE9.GAM is written.
    4. Replay it with stderr capture to produce the baseline:
         ./ReMoMber --seed ${SEED} --replay menu_baseline.RMR 2> baseline_stderr.log
         grep '^\\[RNG-CALL\\]' baseline_stderr.log > ${BASELINE_RNG_LOG}
    5. Commit both menu_baseline.RMR (under assets/) and ${BASELINE_RNG_LOG#${REPO_ROOT}/}.
    6. Re-run this script — it will compare automatically from now on.

  HeMoM log ready for inspection at:
    ${HEMOM_RNG_LOG}
EOF
    exit 3
fi

BASE_COUNT=$(wc -l < "${BASELINE_RNG_LOG}")

# HeMoM exits at Save_SAVE_GAM(8); ReMoM continues past that into the Main
# Screen and runs PreInit_Overland -> Player_Research_Spells (~240 extra
# Random calls at Spells128.c:121).  So a passing HeMoM log is a strict
# line-for-line PREFIX of the baseline, not the full thing.
if [ "${CALL_COUNT}" -gt "${BASE_COUNT}" ]; then
    echo "  FAIL: HeMoM made more Random() calls than the baseline"
    echo "    baseline calls: ${BASE_COUNT}"
    echo "    hemom calls:    ${CALL_COUNT}  (over by $((CALL_COUNT - BASE_COUNT)))"
    echo "  This means HeMoM is drawing Random() in a code path the baseline"
    echo "  doesn't reach — investigate the call sites past line ${BASE_COUNT}."
    exit 2
fi

BASELINE_PREFIX="${BIN_DIR}/baseline_seed${SEED}_rng_prefix.log"
head -n "${CALL_COUNT}" "${BASELINE_RNG_LOG}" > "${BASELINE_PREFIX}"

if diff -q "${BASELINE_PREFIX}" "${HEMOM_RNG_LOG}" >/dev/null; then
    TAIL_COUNT=$((BASE_COUNT - CALL_COUNT))
    if [ "${TAIL_COUNT}" -eq 0 ]; then
        echo "  PASS: HeMoM log matches baseline exactly (${CALL_COUNT} calls)"
    else
        echo "  PASS: HeMoM log is a strict prefix of baseline"
        echo "    matched calls:        ${CALL_COUNT}"
        echo "    baseline post-save:   ${TAIL_COUNT} extra calls (ReMoM Main Screen / PreInit_Overland — HeMoM exits earlier)"
    fi

    # Visible evidence: sample lines + a diff command anyone can re-run.
    HEMOM_HASH=$(md5sum "${HEMOM_RNG_LOG}" | awk '{print $1}')
    PREFIX_HASH=$(md5sum "${BASELINE_PREFIX}" | awk '{print $1}')
    echo
    echo "  Evidence (verify yourself):"
    echo
    echo "    first matched line:"
    echo "      $(head -n 1 "${HEMOM_RNG_LOG}")"
    echo "    last matched line:"
    echo "      $(tail -n 1 "${HEMOM_RNG_LOG}")"
    echo
    echo "    md5sum of HeMoM log:           ${HEMOM_HASH}"
    echo "    md5sum of baseline prefix:     ${PREFIX_HASH}  ← equal == every byte matches"
    echo
    echo "    re-run the comparison:"
    echo "      diff ${BASELINE_PREFIX} ${HEMOM_RNG_LOG}"
    echo "      # expect: empty output (zero bytes)"
    echo
    echo "    or against the committed baseline:"
    echo "      diff <(head -n ${CALL_COUNT} ${BASELINE_RNG_LOG}) ${HEMOM_RNG_LOG}"

    # Leave BASELINE_PREFIX in place so the user can re-run the diff above.
    exit 0
fi

# Prefix differs — that's a real divergence inside HeMoM's range.
echo "  FAIL: trace divergence within HeMoM's range"
echo "    baseline calls: ${BASE_COUNT}"
echo "    hemom calls:    ${CALL_COUNT}"
echo
echo "  First divergent line:"
diff "${BASELINE_PREFIX}" "${HEMOM_RNG_LOG}" | head -n 8 | sed 's/^/    /'
echo
echo "  Full diff:"
echo "    diff ${BASELINE_PREFIX} ${HEMOM_RNG_LOG}"
exit 2
