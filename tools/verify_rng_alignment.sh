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
NEWGAME_CONFIG="${ASSETS}/matchup_hemom.ini"

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
STDERR_BYTES=$(stat -c %s "${HEMOM_STDERR_LOG}" 2>/dev/null || echo 0)
STDERR_LINES=$(wc -l < "${HEMOM_STDERR_LOG}")
echo "  input  : ${HEMOM_STDERR_LOG}"
echo "           ${STDERR_BYTES} bytes, ${STDERR_LINES} lines (full HeMoM stderr)"
echo "  filter : grep '^[RNG-CALL]'"
grep '^\[RNG-CALL\]' "${HEMOM_STDERR_LOG}" > "${HEMOM_RNG_LOG}" || true
CALL_COUNT=$(wc -l < "${HEMOM_RNG_LOG}")
RNG_BYTES=$(stat -c %s "${HEMOM_RNG_LOG}" 2>/dev/null || echo 0)
if [ "${CALL_COUNT}" -eq 0 ]; then
    echo "  output : 0 lines — FAIL"
    echo
    echo "FAIL: no [RNG-CALL] lines captured"
    echo "  Check that Random_at() tracing is wired up and --seed was honored."
    echo "  See MoX/src/random.c:318-341 ('if(_cmd_line_seed != 0)')."
    exit 1
fi
echo "  output : ${HEMOM_RNG_LOG}"
echo "           ${RNG_BYTES} bytes, ${CALL_COUNT} lines ([RNG-CALL] only)"
echo "           dropped: $((STDERR_LINES - CALL_COUNT)) non-RNG lines (other stderr output)"

echo
echo "=== Step 4/5: Verify baseline freshness ==="
RMR_FILE="${ASSETS}/menu_baseline_seed${SEED}.RMR"

# Locate ReMoM.  On Linux clang-debug it shares the bin dir with HeMoM.
# On Windows MSVC-headless-debug only builds HeMoM, so fall back to MSVC-debug
# where the capture script keeps ReMoM.
REMOM_EXE=""
REMOM_PRESET=""
for candidate in \
    "${BIN_DIR}/ReMoM.exe|${BUILD_PRESET}" \
    "${BIN_DIR}/ReMoM|${BUILD_PRESET}" \
    "${REPO_ROOT}/out/build/MSVC-debug/bin/Debug/ReMoM.exe|MSVC-debug" ; do
    path="${candidate%|*}"
    preset="${candidate##*|}"
    if [ -f "${path}" ]; then
        REMOM_EXE="${path}"
        REMOM_PRESET="${preset}"
        break
    fi
done

if [ -f "${BASELINE_RNG_LOG}" ] && [ -n "${REMOM_EXE}" ]; then
    # Rebuild ReMoM (incremental — no-op if up to date) so its mtime
    # reflects the current source.  If the rebuild touches the binary, mtime
    # advances and the staleness check below will fire.
    # cmake --build --preset reads CMakePresets.json from CWD; Step 2 cd'd into
    # BIN_DIR, so run from REPO_ROOT in a subshell.
    ( cd "${REPO_ROOT}" && cmake --build --preset "${REMOM_PRESET}" --target ReMoM ${CMAKE_CONFIG} ) >/dev/null 2>&1 \
        || echo "  WARNING: failed to rebuild ReMoM for staleness check; continuing with existing binary"

    if [ "${REMOM_EXE}" -nt "${BASELINE_RNG_LOG}" ]; then
        echo "  FAIL: baseline is stale — ReMoM binary is newer than the baseline."
        echo "         The baseline was captured against an earlier build of ReMoM, so"
        echo "         compile-time __LINE__ values (and possibly Random() call counts) no"
        echo "         longer match the current binary.  Re-capture before comparing."
        echo "    ReMoM: $(date -r "${REMOM_EXE}" '+%Y-%m-%d %H:%M:%S')"
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
    echo "  baseline is current (ReMoM binary not newer than baseline)"
elif [ -f "${BASELINE_RNG_LOG}" ]; then
    echo "  WARNING: ReMoM binary not found; cannot verify baseline freshness."
    echo "           Searched: ${BIN_DIR}, ${REPO_ROOT}/out/build/MSVC-debug/bin/Debug."
fi

echo
echo "=== Step 5/5: Compare against baseline ==="
if [ ! -f "${BASELINE_RNG_LOG}" ]; then
    cat <<EOF
  No baseline log yet at:
    ${BASELINE_RNG_LOG}

  To capture one from ReMoM (one-time, then replayable):
    1. Build ReMoM under the same preset as HeMoM.
    2. Decide on a canonical menu-click sequence that matches:
         ${NEWGAME_CONFIG}
       (same wizard / race / banner / difficulty / landsize / magic / opponents).
    3. Record the sequence:
         ./ReMoM --seed ${SEED} --record menu_baseline.RMR
       and click through the new-game menus; quit after SAVE9.GAM is written.
    4. Replay it with stderr capture to produce the baseline:
         ./ReMoM --seed ${SEED} --replay menu_baseline.RMR 2> baseline_stderr.log
         grep '^\\[RNG-CALL\\]' baseline_stderr.log > ${BASELINE_RNG_LOG}
    5. Commit both menu_baseline.RMR (under assets/) and ${BASELINE_RNG_LOG#${REPO_ROOT}/}.
    6. Re-run this script — it will compare automatically from now on.

  HeMoM log ready for inspection at:
    ${HEMOM_RNG_LOG}
EOF
    exit 3
fi

BASE_COUNT=$(wc -l < "${BASELINE_RNG_LOG}")
BASE_BYTES=$(stat -c %s "${BASELINE_RNG_LOG}" 2>/dev/null || echo 0)

# === Show all four files involved in the comparison ===
REMOM_STDERR_REF="${BIN_DIR}/remom_seed${SEED}_stderr.log"
echo "  Inputs to the comparison:"
echo
printf "    [%s] %s\n" "HeMoM stderr     " "${HEMOM_STDERR_LOG}"
printf "    %20s %s bytes, %s lines  (full HeMoM stderr; passed through grep)\n" "" "${STDERR_BYTES}" "${STDERR_LINES}"
echo
printf "    [%s] %s\n" "HeMoM rng        " "${HEMOM_RNG_LOG}"
printf "    %20s %s bytes, %s lines  (the [RNG-CALL] subset; comparison LHS)\n" "" "${RNG_BYTES}" "${CALL_COUNT}"
echo
printf "    [%s] %s\n" "baseline rng     " "${BASELINE_RNG_LOG}"
printf "    %20s %s bytes, %s lines  (committed ReMoM baseline; comparison RHS source)\n" "" "${BASE_BYTES}" "${BASE_COUNT}"
echo
if [ -f "${REMOM_STDERR_REF}" ]; then
    REMOM_STDERR_BYTES=$(stat -c %s "${REMOM_STDERR_REF}" 2>/dev/null || echo 0)
    REMOM_STDERR_LINES=$(wc -l < "${REMOM_STDERR_REF}" 2>/dev/null || echo 0)
    printf "    [%s] %s\n" "ReMoM stderr ref " "${REMOM_STDERR_REF}"
    printf "    %20s %s bytes, %s lines  (from last baseline capture; not consumed here)\n" "" "${REMOM_STDERR_BYTES}" "${REMOM_STDERR_LINES}"
else
    printf "    [%s] %s\n" "ReMoM stderr ref " "(not present — committed baseline rng is enough)"
fi

# HeMoM exits at Save_SAVE_GAM(8); ReMoM continues past that into the Main
# Screen and runs PreInit_Overland -> Player_Research_Spells (~240 extra
# Random calls at Spells128.c:121).  So a passing HeMoM log is a strict
# line-for-line PREFIX of the baseline, not the full thing.
echo
if [ "${CALL_COUNT}" -gt "${BASE_COUNT}" ]; then
    echo "  Line-count check: HeMoM (${CALL_COUNT}) > baseline (${BASE_COUNT}) → FAIL"
    echo
    echo "  FAIL: HeMoM made more Random() calls than the baseline"
    echo "    over by:        $((CALL_COUNT - BASE_COUNT)) calls"
    echo "  This means HeMoM is drawing Random() in a code path the baseline"
    echo "  doesn't reach — investigate the call sites past line ${BASE_COUNT}."
    exit 2
fi
echo "  Line-count check: HeMoM (${CALL_COUNT}) ≤ baseline (${BASE_COUNT}) → ok, can compare as prefix"

# === Slice baseline down to HeMoM's length so byte-for-byte compare is meaningful ===
BASELINE_PREFIX="${BIN_DIR}/baseline_seed${SEED}_rng_prefix.log"
echo
echo "  Slicing: head -n ${CALL_COUNT} ${BASELINE_RNG_LOG##${REPO_ROOT}/} > ${BASELINE_PREFIX##${REPO_ROOT}/}"
head -n "${CALL_COUNT}" "${BASELINE_RNG_LOG}" > "${BASELINE_PREFIX}"
PREFIX_BYTES=$(stat -c %s "${BASELINE_PREFIX}")
PREFIX_LINES=$(wc -l < "${BASELINE_PREFIX}")
echo "           → ${PREFIX_BYTES} bytes, ${PREFIX_LINES} lines"

# === Show sample rows side-by-side so eyeballs can sanity-check ===
HEMOM_HASH=$(md5sum "${HEMOM_RNG_LOG}" | awk '{print $1}')
PREFIX_HASH=$(md5sum "${BASELINE_PREFIX}" | awk '{print $1}')
MID_LINE=$((CALL_COUNT / 2))
echo
echo "  Sample lines (HeMoM rng vs baseline-prefix; want every triple to be identical):"
echo "    call=1   (first)"
echo "      HeMoM    : $(head -n 1 "${HEMOM_RNG_LOG}")"
echo "      baseline : $(head -n 1 "${BASELINE_PREFIX}")"
echo "    call=${MID_LINE} (mid)"
echo "      HeMoM    : $(sed -n "${MID_LINE}p" "${HEMOM_RNG_LOG}")"
echo "      baseline : $(sed -n "${MID_LINE}p" "${BASELINE_PREFIX}")"
echo "    call=${CALL_COUNT} (last)"
echo "      HeMoM    : $(tail -n 1 "${HEMOM_RNG_LOG}")"
echo "      baseline : $(tail -n 1 "${BASELINE_PREFIX}")"

# === Run the actual byte-for-byte diff and surface its result ===
echo
echo "  Compare: diff -q ${BASELINE_PREFIX##${REPO_ROOT}/} ${HEMOM_RNG_LOG##${REPO_ROOT}/}"
DIFF_OUTPUT=$(diff -q "${BASELINE_PREFIX}" "${HEMOM_RNG_LOG}" 2>&1 || true)
if [ -z "${DIFF_OUTPUT}" ]; then
    echo "           → (no output; files are byte-equal)"
else
    echo "           → ${DIFF_OUTPUT}"
fi
echo "  md5sum HeMoM   : ${HEMOM_HASH}"
echo "  md5sum baseline: ${PREFIX_HASH}"

if [ "${HEMOM_HASH}" = "${PREFIX_HASH}" ] && [ -z "${DIFF_OUTPUT}" ]; then
    TAIL_COUNT=$((BASE_COUNT - CALL_COUNT))
    echo
    if [ "${TAIL_COUNT}" -eq 0 ]; then
        echo "  PASS: HeMoM log matches baseline exactly (${CALL_COUNT} calls; both md5 = ${HEMOM_HASH})"
    else
        echo "  PASS: HeMoM log is a strict prefix of baseline"
        echo "    matched calls:        ${CALL_COUNT}  (both md5 = ${HEMOM_HASH})"
        echo "    baseline post-save:   ${TAIL_COUNT} extra calls (ReMoM Main Screen / PreInit_Overland — HeMoM exits earlier)"
    fi
    echo
    echo "  Reproduce by hand (copy-paste, no cd required):"
    echo "    diff ${BASELINE_PREFIX} ${HEMOM_RNG_LOG}"
    echo "    md5sum ${BASELINE_PREFIX} ${HEMOM_RNG_LOG}"
    echo "    diff <(head -n ${CALL_COUNT} ${BASELINE_RNG_LOG}) ${HEMOM_RNG_LOG}"

    # Leave BASELINE_PREFIX in place so the user can re-run the diff above.
    exit 0
fi

# Prefix differs — that's a real divergence inside HeMoM's range.
echo
echo "  FAIL: trace divergence within HeMoM's range"
echo "    baseline calls: ${BASE_COUNT}"
echo "    hemom calls:    ${CALL_COUNT}"

# Find the first divergent line number so we can show context.
FIRST_DIFF=$(diff "${BASELINE_PREFIX}" "${HEMOM_RNG_LOG}" | awk -F'[c,]' '/^[0-9]/ {print $1; exit}')
if [ -n "${FIRST_DIFF}" ]; then
    CTX_FROM=$((FIRST_DIFF > 2 ? FIRST_DIFF - 2 : 1))
    CTX_TO=$((FIRST_DIFF + 2))
    echo
    echo "  First divergence at line ${FIRST_DIFF}.  Context (lines ${CTX_FROM}-${CTX_TO}):"
    echo
    echo "    --- baseline prefix ---"
    sed -n "${CTX_FROM},${CTX_TO}p" "${BASELINE_PREFIX}" | sed 's/^/    /'
    echo
    echo "    --- HeMoM rng ---"
    sed -n "${CTX_FROM},${CTX_TO}p" "${HEMOM_RNG_LOG}" | sed 's/^/    /'
fi

echo
echo "  Raw diff (first 8 lines):"
diff "${BASELINE_PREFIX}" "${HEMOM_RNG_LOG}" | head -n 8 | sed 's/^/    /'
echo
echo "  Reproduce by hand:"
echo "    diff ${BASELINE_PREFIX} ${HEMOM_RNG_LOG}"
echo "    sed -n '${FIRST_DIFF}p' ${BASELINE_PREFIX}   # baseline at divergence"
echo "    sed -n '${FIRST_DIFF}p' ${HEMOM_RNG_LOG}    # hemom at divergence"
exit 2
