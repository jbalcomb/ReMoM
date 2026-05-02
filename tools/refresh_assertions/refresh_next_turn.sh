#!/usr/bin/env bash
#
# refresh_next_turn.sh — regenerate tests/assert_continue_next_turn.txt by
# running the NextTurn scenario N times under MSVC-release and computing the
# field-stable subset across all runs.  Fields whose values vary across runs
# get commented out with a NONDET marker recording the observed values.
#
# This is the multi-iteration variant required because some fields in this
# test (notably AI research-spell picks) read run-to-run-varying memory and
# would otherwise produce a flaky test.  See doc/Devel-Test-Assertions.md.
#
# Usage:
#     ./tools/refresh_assertions/refresh_next_turn.sh [N=5]
#
# After it runs, review the diff and commit if the changes look intentional:
#     git diff -- tests/assert_continue_next_turn.txt

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/../.."; pwd)"
# On Git Bash, $REPO_ROOT is like /c/STU/devel/ReMoM — Python on Windows can't
# resolve that.  cygpath -m normalises to mixed format (C:/STU/devel/ReMoM)
# which both bash and Python handle cleanly.  No-op on non-MSYS platforms.
if command -v cygpath >/dev/null 2>&1; then
    REPO_ROOT="$(cygpath -m "${REPO_ROOT}")"
fi
PRESET="MSVC-release"
RUN_DIR="${REPO_ROOT}/out/build/${PRESET}/bin/Release"
ASSETS="${REPO_ROOT}/assets"
TESTS_DIR="${REPO_ROOT}/tests"
ASSERTS_FILE="${TESTS_DIR}/assert_continue_next_turn.txt"
N="${1:-5}"

if [ ! -d "${REPO_ROOT}/out/build/${PRESET}" ]; then
    echo "FAIL: ${PRESET} build directory not found.  Configure first:"
    echo "    cmake --preset ${PRESET}"
    exit 1
fi

echo "=== Step 1/3: build HeMoM (${PRESET}) ==="
cmake --build "${REPO_ROOT}/out/build/${PRESET}" --config Release --target HeMoM > /dev/null
echo "  built"

echo
echo "=== Step 2/3: run NextTurn scenario ${N} times in ${RUN_DIR} ==="
cd "${RUN_DIR}"
rm -f run_*.txt SAVE1.GAM SAVE1.txt SAVE9.GAM SAVE9.txt
DUMPS=""
for i in $(seq 1 "${N}"); do
    ./HeMoM.exe --newgame "${ASSETS}/test_worldgen.ini" > /dev/null 2>&1
    ./HeMoM.exe --continue --scenario "${ASSETS}/test_continue_next_turn.hms" --dump-save SAVE1.GAM > /dev/null 2>&1
    cp SAVE1.txt "run_${i}.txt"
    DUMPS="${DUMPS}${DUMPS:+,}${RUN_DIR}/run_${i}.txt"
    echo "  run ${i}: $(stat -c '%s' "run_${i}.txt") bytes"
done

echo
echo "=== Step 3/3: compute stable subset and refresh ${ASSERTS_FILE##*/} ==="
cd "${REPO_ROOT}"
python tools/find_stable_fields.py \
    --dumps="${DUMPS}" \
    --asserts="${ASSERTS_FILE}" \
    --in-place

echo
echo "Done.  Review with:"
echo "    git diff -- tests/assert_continue_next_turn.txt"
echo "If the changes look intentional, commit them.  Lines starting with"
echo "'# NONDET' indicate fields that varied across runs and have been set"
echo "aside (commented out) until the underlying determinism is fixed."
