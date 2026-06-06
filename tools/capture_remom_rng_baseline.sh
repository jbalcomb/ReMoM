#!/usr/bin/env bash
#
# capture_remom_rng_baseline.sh — Launch ReMoMber for the one-time capture of
# the ReMoM-side baseline [RNG-CALL] trace that verify_rng_alignment.sh
# compares HeMoM against.
#
# What it does:
#   1. Builds ReMoMber under clang-debug (Linux) or MSVC-debug (Windows via
#      Git Bash / MSYS).
#   2. Launches ReMoMber with --seed ${SEED} --record menu_baseline.RMR,
#      capturing stderr to a raw log.
#   3. Waits for you to click through the new-game menus making the choices
#      that match assets/matchup_hemom.ini, then quit.
#   4. After ReMoMber exits, filters the [RNG-CALL] stream into
#      tests/baseline_seed${SEED}_rng.log.
#   5. Prints the next step (run verify_rng_alignment.sh to compare).
#
# Manual menu walkthrough (must match assets/matchup_hemom.ini):
#   Screen 0 (Options):
#     Difficulty = Normal,  Opponents = Four,
#     Landsize   = Large,   Magic     = Normal
#     → OK
#   Screen 1 (Select Wizard):
#     Click "Custom"  (NOT a preset — Rjak's books would be wrong)
#   Screen 2 (Portrait):
#     Click Rjak's portrait  (7th wizard, wizard_id = 6)
#   Screen 3 (Name):
#     Type "Rjak"  → Enter
#   Screen 4 (Books + Retorts):
#     Books: Life=2, Death=0, Chaos=2, Nature=2, Sorcery=4
#     Retort: Artificer only
#     → OK  (must consume all 11 picks)
#   Screen 5 (Spell picks):
#     For each realm with >1 book, accept default picks → OK
#     (Order: defined by TBL_Realm_Order)
#   Screen 6 (Race):
#     Click "High Men"
#   Screen 7 (Banner):
#     Click Yellow
#   → SAVE9.GAM is written, game starts, you quit ReMoMber.
#
# Usage:
#   cd /home/jbalcomb/STU/devel/ReMoM
#   ./tools/capture_remom_rng_baseline.sh                 # default seed=12345
#   ./tools/capture_remom_rng_baseline.sh 4242            # custom seed
#   SEED=4242 ./tools/capture_remom_rng_baseline.sh       # equivalent
#
# Exit codes:
#   0  Baseline captured successfully
#   1  Build or run failure
#   2  ReMoMber exited but no [RNG-CALL] lines were captured
#      (most likely the --seed flag wasn't honored)

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SEED="${1:-${SEED:-12345}}"

# cmake --build --preset reads CMakePresets.json from CWD, not from the source
# tree.  Run from the repo root so the script works regardless of where the
# user invoked it from.
cd "${REPO_ROOT}"

# === Platform selection (matches verify_rng_alignment.sh) ===================
if [ -d "${REPO_ROOT}/out/build/MSVC-debug/bin/Debug" ]; then
    BUILD_PRESET="MSVC-debug"
    BUILD_DIR="${REPO_ROOT}/out/build/${BUILD_PRESET}"
    BIN_DIR="${BUILD_DIR}/bin/Debug"
    REMOM_EXE="${BIN_DIR}/ReMoMber.exe"
    CMAKE_CONFIG="--config Debug"
elif command -v clang >/dev/null 2>&1; then
    BUILD_PRESET="clang-debug"
    BUILD_DIR="${REPO_ROOT}/out/build/${BUILD_PRESET}"
    BIN_DIR="${BUILD_DIR}/bin/Debug"
    REMOM_EXE="${BIN_DIR}/ReMoMber"
    CMAKE_CONFIG=""
else
    echo "FAIL: no usable build preset found (need clang-debug or MSVC-debug)"
    exit 1
fi

ASSETS="${REPO_ROOT}/assets"
TESTS_DIR="${REPO_ROOT}/tests"
mkdir -p "${TESTS_DIR}"

BASELINE_RNG_LOG="${TESTS_DIR}/baseline_seed${SEED}_rng.log"
REMOM_STDERR_LOG="${BIN_DIR}/remom_seed${SEED}_stderr.log"
RMR_FILE="${ASSETS}/menu_baseline_seed${SEED}.RMR"

echo "=== Step 1/4: Build ReMoMber (${BUILD_PRESET}) ==="
cmake --build --preset "${BUILD_PRESET}" --target ReMoMber ${CMAKE_CONFIG} >/dev/null
if [ ! -x "${REMOM_EXE}" ]; then
    echo "FAIL: ReMoMber binary not found at ${REMOM_EXE}"
    exit 1
fi
echo "  built: ${REMOM_EXE}"

echo
echo "=== Step 2/4: Walkthrough reminder ==="
cat <<'EOF'
  In ReMoMber, click through the new-game menus exactly as follows.
  Any deviation here is what shows up as a Layer-A trace divergence.

    Screen 0:  Diff=Normal, Opp=Four, Land=Large, Magic=Normal  → OK
    Screen 1:  Click "Custom"
    Screen 2:  Click Rjak's portrait (7th wizard, wizard_id = 6)
    Screen 3:  Type "Rjak"  → Enter
    Screen 4:  Books: Life=2, Death=0, Chaos=2, Nature=2, Sorcery=4
               Retort: Artificer  → OK
    Screen 5:  Each realm with >1 book: accept defaults → OK
    Screen 6:  Click "High Men"
    Screen 7:  Click Yellow

  Once SAVE9.GAM is written and the main screen comes up, quit ReMoMber.
EOF

echo
echo "=== Step 3/4: Launch ReMoMber --seed ${SEED} --record ${RMR_FILE##*/} ==="
echo "  stderr → ${REMOM_STDERR_LOG}"
echo "  Press Enter to launch..."
read -r _
cd "${BIN_DIR}"
rm -f SAVE9.GAM SAVE9.txt "${REMOM_STDERR_LOG}"
# Run interactively so the user can play through; stderr captures the RNG stream.
"${REMOM_EXE}" --seed "${SEED}" --record "${RMR_FILE}" 2> "${REMOM_STDERR_LOG}" || {
    echo "  NOTE: ReMoMber exit was non-zero (this is normal for a user-initiated quit)."
}

echo
echo "=== Step 4/4: Extract [RNG-CALL] log ==="
grep '^\[RNG-CALL\]' "${REMOM_STDERR_LOG}" > "${BASELINE_RNG_LOG}" || true
CALL_COUNT=$(wc -l < "${BASELINE_RNG_LOG}")
if [ "${CALL_COUNT}" -eq 0 ]; then
    echo "FAIL: no [RNG-CALL] lines captured"
    echo "  Either --seed wasn't honored, or stderr was redirected elsewhere."
    echo "  Check stderr log:  ${REMOM_STDERR_LOG}"
    exit 2
fi
echo "  captured ${CALL_COUNT} Random() calls → ${BASELINE_RNG_LOG}"
if [ -f "${RMR_FILE}" ]; then
    echo "  saved RMR replay  → ${RMR_FILE}"
fi

echo
echo "=== Done ==="
cat <<EOF
  Now run the verification:
    ./tools/verify_rng_alignment.sh ${SEED}

  If the diff is empty, P1 alignment is proven for seed=${SEED}.
  If not, the first divergent line tells you which Random() site differs.

  To re-baseline later via replay (no clicking):
    ${REMOM_EXE##${REPO_ROOT}/} --seed ${SEED} --replay ${RMR_FILE##${REPO_ROOT}/} 2> ${REMOM_STDERR_LOG##${REPO_ROOT}/}
    grep '^\[RNG-CALL\]' ${REMOM_STDERR_LOG##${REPO_ROOT}/} > ${BASELINE_RNG_LOG##${REPO_ROOT}/}
EOF
