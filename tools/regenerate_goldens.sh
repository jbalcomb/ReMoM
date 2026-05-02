#!/usr/bin/env bash
#
# regenerate_goldens.sh — Regenerate the test golden files from the current
# source tree and place them under tests/.
#
# When to run:
#   You believe the current world-gen / save-format output is correct and want
#   the goldens to match it.  E.g. after an intentional change to world gen,
#   hero ability assignment, or save layout.
#
# What it does:
#   1. Builds HeMoM and savedump under MSVC-headless-debug.
#   2. Runs HeMoM in --newgame mode with the deterministic seed=12345 config
#      (assets/test_worldgen.ini + assets/test_worldgen_smoke.hms).
#   3. Dumps the resulting SAVE9.GAM to text via savedump.
#   4. Copies both into tests/ as the new golden files.
#   5. Prints a summary suitable for use in a commit message.
#
# After running:
#   - Inspect the diff in `git status` and `git diff -- tests/golden_*`.
#   - If you accept the new goldens, commit them with the printed summary.
#   - The .GAM is normally hidden by the *.gam rule in .gitignore, but the
#     `!tests/golden_*.GAM` exception below the rule re-allows it.
#
# Usage:
#   cd c:/STU/devel/ReMoM
#   ./tools/regenerate_goldens.sh

set -euo pipefail

REPO_ROOT="c:/STU/devel/ReMoM"
# Goldens MUST be regenerated under the same preset/config the tests run against,
# otherwise allocator differences (e.g. MSVC's Debug 0xCD fill vs Release zeros)
# leak into the save and the comparison is forever stuck.  Tests run under
# MSVC-release (see CMakePresets.json testPresets), so we build there too.
HEMOM_BUILD="${REPO_ROOT}/out/build/MSVC-release"
HEMOM_DIR="${HEMOM_BUILD}/bin/Release"
ASSETS="${REPO_ROOT}/assets"
TESTS_DIR="${REPO_ROOT}/tests"

NEWGAME_CONFIG="${ASSETS}/test_worldgen.ini"
NEWGAME_SCENARIO="${ASSETS}/test_worldgen_smoke.hms"

GOLDEN_GAM="${TESTS_DIR}/golden_worldgen_seed12345.GAM"
GOLDEN_TXT="${TESTS_DIR}/golden_worldgen_seed12345.txt"

echo "=== Step 1/4: Build HeMoM and savedump (MSVC-release) ==="
cmake --build "${HEMOM_BUILD}" --config Release --target HeMoM    >/dev/null
cmake --build "${HEMOM_BUILD}" --config Release --target savedump >/dev/null
echo "  built: HeMoM, savedump"

echo
echo "=== Step 2/4: Generate SAVE9.GAM from --newgame (seed=12345) ==="
if [ ! -x "${HEMOM_DIR}/HeMoM.exe" ]; then
    echo "FAIL: HeMoM.exe not found at ${HEMOM_DIR}"
    exit 1
fi
cd "${HEMOM_DIR}"
rm -f SAVE9.GAM SAVE9.txt
./HeMoM.exe --newgame "${NEWGAME_CONFIG}" --scenario "${NEWGAME_SCENARIO}" > /dev/null 2>&1
if [ ! -f SAVE9.GAM ]; then
    echo "FAIL: HeMoM did not produce SAVE9.GAM"
    exit 1
fi
echo "  generated: ${HEMOM_DIR}/SAVE9.GAM"

echo
echo "=== Step 3/4: Dump SAVE9.GAM to text ==="
./savedump.exe SAVE9.GAM SAVE9.txt > /dev/null
if [ ! -f SAVE9.txt ]; then
    echo "FAIL: savedump did not produce SAVE9.txt"
    exit 1
fi
echo "  dumped: ${HEMOM_DIR}/SAVE9.txt"

echo
echo "=== Step 4/4: Copy into tests/ as the new goldens ==="
cp "${HEMOM_DIR}/SAVE9.GAM" "${GOLDEN_GAM}"
cp "${HEMOM_DIR}/SAVE9.txt" "${GOLDEN_TXT}"
echo "  wrote: ${GOLDEN_GAM}"
echo "  wrote: ${GOLDEN_TXT}"

# === Summary for the commit message ============================================
SOURCE_COMMIT=$(cd "${REPO_ROOT}" && git rev-parse --short HEAD)
SOURCE_BRANCH=$(cd "${REPO_ROOT}" && git rev-parse --abbrev-ref HEAD)
GAM_BYTES=$(wc -c < "${GOLDEN_GAM}")
TXT_BYTES=$(wc -c < "${GOLDEN_TXT}")

echo
echo "=== Done ==="
echo
echo "Suggested commit message:"
echo
echo "  refresh test goldens for world-gen seed=12345"
echo
echo "  Regenerated via tools/regenerate_goldens.sh from ${SOURCE_BRANCH} @ ${SOURCE_COMMIT}."
echo "  Inputs: ${NEWGAME_CONFIG##*/} + ${NEWGAME_SCENARIO##*/}."
echo
echo "    tests/golden_worldgen_seed12345.GAM (${GAM_BYTES} bytes)"
echo "    tests/golden_worldgen_seed12345.txt (${TXT_BYTES} bytes)"
echo
echo "Review with:"
echo "  git diff -- tests/golden_worldgen_seed12345.txt   # text golden — readable diff"
echo "  git status -- tests/golden_worldgen_seed12345.GAM # binary — confirm tracked"
echo
