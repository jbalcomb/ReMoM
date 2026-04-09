#!/usr/bin/env bash
#
# record_next_turn.sh — Interactive workflow to capture a Next Turn session
# for use in building a new test case.
#
# Usage:
#   cd c:/STU/devel/ReMoM
#   ./tools/record_next_turn.sh
#
# What it does:
#   1. Generates a fresh deterministic SAVE9.GAM using the headless newgame
#      path (seed=12345 from test_worldgen.ini).
#   2. Dumps SAVE9.GAM to SAVE9.txt as the baseline.
#   3. Copies SAVE9.GAM into the MSVC-debug (ReMoMber) build directory.
#   4. Launches ReMoMber with --continue, the pre-Next-Turn scenario script,
#      and --record to capture your session.  The scripted portion runs
#      automatically (city-name popup, Claudesville, patrol).  When the
#      synthetic player reaches `end`, control hands back to you.  You play
#      through the next turn, handle any popups, save to slot 1, and quit.
#   5. After ReMoMber exits, dumps SAVE1.GAM to SAVE1.txt.
#   6. Diffs SAVE9.txt against SAVE1.txt and saves the result.
#
# Output files (all in the MSVC-debug bin/Debug directory):
#   SAVE9.GAM, SAVE9.txt       — baseline
#   SAVE1.GAM, SAVE1.txt       — post-session
#   next_turn_diff.txt         — the diff to examine
#   next_turn.RMR              — the raw input recording (diagnostic only)

set -euo pipefail

REPO_ROOT="c:/STU/devel/ReMoM"
HEADLESS_DIR="${REPO_ROOT}/out/build/MSVC-headless-debug/bin/Debug"
REMOMBER_DIR="${REPO_ROOT}/out/build/MSVC-debug/bin/Debug"
ASSETS="${REPO_ROOT}/assets"

PRE_SCENARIO="${ASSETS}/test_continue_pre_next_turn.hms"
NEWGAME_CONFIG="${ASSETS}/test_worldgen.ini"
NEWGAME_SCENARIO="${ASSETS}/test_worldgen_smoke.hms"

echo "=== Step 0/5: Rebuild MSVC-headless-debug (HeMoM) and MSVC-debug (ReMoMber) ==="
# Both presets consume shared sources (HeMoM_Player.c, platform/*, etc.).  A
# stale build on either side silently breaks this workflow, so rebuild both
# before doing anything else.
cmake --build "${REPO_ROOT}/out/build/MSVC-headless-debug" --config Debug --target HeMoM     >/dev/null
cmake --build "${REPO_ROOT}/out/build/MSVC-headless-debug" --config Debug --target savedump  >/dev/null || true
cmake --build "${REPO_ROOT}/out/build/MSVC-debug"          --config Debug --target ReMoMber  >/dev/null
cmake --build "${REPO_ROOT}/out/build/MSVC-debug"          --config Debug --target savedump  >/dev/null
echo "  rebuilt: HeMoM (MSVC-headless-debug), ReMoMber + savedump (MSVC-debug)"

echo
echo "=== Step 1/5: Generate fresh SAVE9.GAM (headless, seed=12345) ==="
if [ ! -x "${HEADLESS_DIR}/HeMoM.exe" ]; then
    echo "FAIL: HeMoM.exe not found at ${HEADLESS_DIR}"
    echo "      Build the MSVC-headless-debug preset first."
    exit 1
fi
cd "${HEADLESS_DIR}"
./HeMoM.exe --newgame "${NEWGAME_CONFIG}" --scenario "${NEWGAME_SCENARIO}" > /dev/null 2>&1
if [ ! -f SAVE9.GAM ]; then
    echo "FAIL: HeMoM did not produce SAVE9.GAM"
    exit 1
fi
echo "  generated: ${HEADLESS_DIR}/SAVE9.GAM"

echo
echo "=== Step 2/5: Copy SAVE9.GAM and baseline dump to ReMoMber build ==="
if [ ! -x "${REMOMBER_DIR}/ReMoMber.exe" ]; then
    echo "FAIL: ReMoMber.exe not found at ${REMOMBER_DIR}"
    echo "      Build the MSVC-debug preset first."
    exit 1
fi
if [ ! -x "${REMOMBER_DIR}/savedump.exe" ]; then
    echo "FAIL: savedump.exe not found at ${REMOMBER_DIR}"
    echo "      Build the savedump target under MSVC-debug."
    exit 1
fi
cp "${HEADLESS_DIR}/SAVE9.GAM" "${REMOMBER_DIR}/SAVE9.GAM"
cd "${REMOMBER_DIR}"
./savedump.exe SAVE9.GAM SAVE9.txt > /dev/null
echo "  copied:  ${REMOMBER_DIR}/SAVE9.GAM"
echo "  dumped:  ${REMOMBER_DIR}/SAVE9.txt"

echo
echo "=== Step 3/5: Launching ReMoMber for manual Next Turn session ==="
echo
echo "    The scripted portion will run automatically:"
echo "      - Wait for city-name popup"
echo "      - Clear default, type 'Claudesville', press Enter"
echo "      - Click Patrol button"
echo "      - Hand control back to you"
echo
echo "    When the scenario stops, YOU take over:"
echo "      - Click Next Turn (or press N)"
echo "      - Handle any popups that appear"
echo "      - Save to slot 1 (Game menu -> click slot -> type a name -> Save)"
echo "      - Quit back to DOS"
echo
echo "    The entire session is being recorded to next_turn.RMR."
echo
read -p "Press Enter to launch ReMoMber... "

rm -f next_turn.RMR
./ReMoMber.exe --continue --scenario "${PRE_SCENARIO}" --record next_turn.RMR

echo
echo "=== Step 4/5: Dumping SAVE1.GAM ==="
if [ ! -f SAVE1.GAM ]; then
    echo "FAIL: SAVE1.GAM not found — did you save to slot 1?"
    exit 1
fi
./savedump.exe SAVE1.GAM SAVE1.txt > /dev/null
echo "  dumped:  ${REMOMBER_DIR}/SAVE1.txt"

echo
echo "=== Step 5/5: Diff SAVE9.txt vs SAVE1.txt ==="
# diff exits 1 when files differ — expected.  Don't abort on that.
set +e
diff SAVE9.txt SAVE1.txt > next_turn_diff.txt
DIFF_RC=$?
set -e

if [ ${DIFF_RC} -eq 0 ]; then
    echo "  WARNING: no differences — the save files are identical."
elif [ ${DIFF_RC} -eq 1 ]; then
    DIFF_LINES=$(wc -l < next_turn_diff.txt)
    echo "  diff written: ${REMOMBER_DIR}/next_turn_diff.txt (${DIFF_LINES} lines)"
else
    echo "FAIL: diff returned unexpected exit code ${DIFF_RC}"
    exit 1
fi

echo
echo "Done.  Output files in: ${REMOMBER_DIR}"
echo "  SAVE9.txt            — baseline dump"
echo "  SAVE1.txt            — post-session dump"
echo "  next_turn_diff.txt   — examine this to decide assertions"
echo "  next_turn.RMR        — raw input recording (diagnostic)"
