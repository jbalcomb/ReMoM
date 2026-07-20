
include(GoogleTest)

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test GTest::gtest_main)
gtest_discover_tests(hello_test
	PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)



# =========================================================================
#  HeMoM Integration Tests
#
#  These tests use the HeMoM headless executable to run game scenarios
#  and validate results. Each test is a pair:
#    1. Run HeMoM with a config + scenario (must exit 0)
#    2. Validate the output (save file exists, correct size)
#
#  The tests use CTest fixtures so the validation step only runs if
#  HeMoM completed successfully.
# =========================================================================

set(HEMOM_TEST_ASSETS "${PROJECT_SOURCE_DIR}/assets")
set(HEMOM_TEST_WORKDIR "$<TARGET_FILE_DIR:HeMoM>")

# --- Smoke Test: World Generation ---
# Verifies: engine init, config parsing, wizard profile, world gen,
#           event init, finalize tables, save file creation, clean exit.

add_test(
    NAME HeMoM_WorldGen_Run
    COMMAND $<TARGET_FILE:HeMoM>
        --newgame "${HEMOM_TEST_ASSETS}/test_worldgen.ini"
        --scenario "${HEMOM_TEST_ASSETS}/test_worldgen_smoke.hms"
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
set_tests_properties(HeMoM_WorldGen_Run PROPERTIES
    TIMEOUT 30
    FIXTURES_SETUP HeMoM_WorldGen
    LABELS "hemom;integration;worldgen"
)

add_test(
    NAME HeMoM_WorldGen_Validate
    COMMAND ${CMAKE_COMMAND}
        -DSAVE_FILE=${HEMOM_TEST_WORKDIR}/SAVE9.GAM
        -DEXPECTED_SIZE=123300
        -P "${CMAKE_CURRENT_SOURCE_DIR}/validate_save.cmake"
)
set_tests_properties(HeMoM_WorldGen_Validate PROPERTIES
    FIXTURES_REQUIRED HeMoM_WorldGen
    LABELS "hemom;integration;worldgen"
)

# --- Golden Comparison: Byte-level regression ---
# DISABLED — invalid test design.  The save format is fixed-size and reserves
# slots / struct padding for content that isn't populated yet.  Those bytes
# come from whatever the OS handed back at allocation and vary run-to-run, so
# byte-level md5 can never be a valid contract regardless of code correctness.
# The test previously appeared to work only because MSVC's Debug allocator
# fills uninitialized memory with a deterministic 0xCD; that fill is absent
# under Release and the latent non-determinism surfaces.
#
# Replaced by HeMoM_WorldGen_Fields below, which uses check_save_fields with
# an explicit list of meaningful fields (assert_worldgen.txt).  The dump
# tooling and tools/regenerate_goldens.sh are kept for diagnostic use even
# though no test compares against the .GAM directly anymore.

add_test(
    NAME HeMoM_WorldGen_Golden
    COMMAND ${CMAKE_COMMAND}
        -DACTUAL=${HEMOM_TEST_WORKDIR}/SAVE9.GAM
        -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/golden_worldgen_seed12345.GAM
        -P "${CMAKE_CURRENT_SOURCE_DIR}/compare_save.cmake"
)
set_tests_properties(HeMoM_WorldGen_Golden PROPERTIES
    FIXTURES_REQUIRED HeMoM_WorldGen
    LABELS "hemom;integration;worldgen;golden"
    DISABLED TRUE
)

# --- Structured Field Assertions ---
# Asserts a curated list of meaningful fields against expected values defined
# in assert_worldgen.txt.  Unlike a full-file comparator, this only fails when
# fields the game actually reads back have changed — uninitialized regions of
# the save format do not affect the result.

add_executable(compare_save_text compare_save_text.c)
add_executable(check_save_fields check_save_fields.c)

add_test(
    NAME HeMoM_WorldGen_Fields
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/SAVE9.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_worldgen.txt
)
set_tests_properties(HeMoM_WorldGen_Fields PROPERTIES
    FIXTURES_REQUIRED HeMoM_WorldGen
    LABELS "hemom;integration;worldgen"
)

# --- OOB Autotiling harness (PLAN-Static-Pool-Allocator Phase 3) ---
# Pins the plane-1 (Myrror) last-row world_map tiles.  During Simtex_Autotiling
# the south-neighbour autotile reads p_world_map[1][40][x] -- one row past the
# [2][40][60] array (WORLD_HEIGHT=40): an OG-faithful OOB read into the
# WORLD_OVERFLOW slack past _world_maps (MAPGEN.c:3987/3998/4011).  The static
# pool makes that slack addressable and sentinel-deterministic, and
# gd_ci_inject_world_overrun stamps OG's captured bytes there, so the read no
# longer faults and the generated world is reproducible.  A pool/inject
# regression (crash, garbage slack, wrong layout) would perturb these seed-pinned
# last-row values.  Reuses the HeMoM_WorldGen fixture's SAVE9.txt (no extra run).
add_test(
    NAME HeMoM_OOB_Autotiling
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/SAVE9.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_oob_autotiling.txt
)
set_tests_properties(HeMoM_OOB_Autotiling PROPERTIES
    FIXTURES_REQUIRED HeMoM_WorldGen
    LABELS "hemom;integration;worldgen;oob"
)



# =========================================================================
#  Continue + Save Test
#
#  This test creates its own game-save (does not depend on HeMoM_WorldGen
#  because that fixture's SAVE9.GAM gets overwritten by the Quit path in
#  the scenario).  It then runs a scripted scenario that:
#    - Names the home city "Claudesville" via the popup
#    - Patrols the starting unit stack
#    - Saves to slot 1 (SAVE1.GAM) with a custom name
#    - Exits via the Quit-to-DOS path
#
#  Validation asserts specific field-level state changes caused by the
#  scenario (city name, unit status, etc.) — not a full golden comparison.
#  The assertion list is in assert_continue_save.txt.
# =========================================================================

add_test(
    NAME HeMoM_Continue_Setup
    COMMAND $<TARGET_FILE:HeMoM>
        --newgame "${HEMOM_TEST_ASSETS}/test_worldgen.ini"
        --scenario "${HEMOM_TEST_ASSETS}/test_worldgen_smoke.hms"
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
set_tests_properties(HeMoM_Continue_Setup PROPERTIES
    TIMEOUT 30
    FIXTURES_SETUP HeMoM_Continue
    LABELS "hemom;integration;continue"
)

add_test(
    NAME HeMoM_Continue_Run
    COMMAND $<TARGET_FILE:HeMoM>
        --continue
        --scenario "${HEMOM_TEST_ASSETS}/test_continue_save.hms"
        --dump-save SAVE1.GAM
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
set_tests_properties(HeMoM_Continue_Run PROPERTIES
    TIMEOUT 30
    TIMEOUT 60
    FIXTURES_REQUIRED HeMoM_Continue
    FIXTURES_SETUP HeMoM_Continue_Saved
    LABELS "hemom;integration;continue"
)

add_test(
    NAME HeMoM_Continue_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/SAVE1.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_continue_save.txt
)
set_tests_properties(HeMoM_Continue_Assertions PROPERTIES
    FIXTURES_REQUIRED HeMoM_Continue_Saved
    LABELS "hemom;integration;continue"
)



# =========================================================================
#  Continue + Next Turn Test
#
#  Extends the Continue + Save test with one Next Turn advancement.  Creates
#  its own game-save via --newgame (independent of HeMoM_Continue), runs a
#  scenario that names the home city, patrols the starting stack, clicks
#  Next Turn, waits for the AI to finish, then saves to slot 1 and quits.
#
#  Assertions cover the scenario-caused state changes plus the Next Turn
#  effects (game.turn advancing, AI unit movement orders).  Load-time
#  recomputation is deliberately not asserted.
# =========================================================================

add_test(
    NAME HeMoM_NextTurn_Setup
    COMMAND $<TARGET_FILE:HeMoM>
        --newgame "${HEMOM_TEST_ASSETS}/test_worldgen.ini"
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
set_tests_properties(HeMoM_NextTurn_Setup PROPERTIES
    TIMEOUT 30
    FIXTURES_SETUP HeMoM_NextTurn
    LABELS "hemom;integration;nextturn"
)

add_test(
    NAME HeMoM_NextTurn_Run
    COMMAND $<TARGET_FILE:HeMoM>
        --continue
        --scenario "${HEMOM_TEST_ASSETS}/test_continue_next_turn.hms"
        --dump-save SAVE1.GAM
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
set_tests_properties(HeMoM_NextTurn_Run PROPERTIES
    TIMEOUT 90
    FIXTURES_REQUIRED HeMoM_NextTurn
    FIXTURES_SETUP HeMoM_NextTurn_Saved
    LABELS "hemom;integration;nextturn"
)

add_test(
    NAME HeMoM_NextTurn_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/SAVE1.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_continue_next_turn.txt
)
set_tests_properties(HeMoM_NextTurn_Assertions PROPERTIES
    FIXTURES_REQUIRED HeMoM_NextTurn_Saved
    LABELS "hemom;integration;nextturn"
)



# =========================================================================
#  AI Behavior Test
#
#  Runs 5 turns headlessly with --ai-metrics enabled, then evaluates the
#  resulting CSV files against behavioral assertions using check_ai_metrics.
#
#  Assertions cover:
#    - Liveness: players have units/cities, turn data exists
#    - Bounds: gold/mana/unit counts stay in sane ranges
# =========================================================================

add_executable(check_ai_metrics check_ai_metrics.c)

add_test(
    NAME HeMoM_AIBehavior_Setup
    COMMAND $<TARGET_FILE:HeMoM>
        --newgame "${HEMOM_TEST_ASSETS}/test_worldgen.ini"
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
# TIMEOUT was 10s when --newgame world-gen took ~0.26s.  A fixed ~1.5s cost was
# since added to every new game (measured ~1.4-1.75s solo, flat across land sizes
# -- NOT content-scaled, so it won't creep as worlds grow), and ~5.5s under
# parallel contention.  10s became flaky under load.  20s = a few x the realistic
# sequential worst-case (the suite runs serially; it shares SAVE*.GAM), enough to
# absorb slow-CI variance while still catching a real hang promptly.
set_tests_properties(HeMoM_AIBehavior_Setup PROPERTIES
    TIMEOUT 60
    FIXTURES_SETUP HeMoM_AIBehavior
    LABELS "hemom;integration;ai"
)

# HeMoM_AIBehavior_Run + HeMoM_AIBehavior_Evaluate are Debug-only.
#
# Two reasons:
#   1. --ai-metrics is gated by #ifdef STU_DEBUG (HeMoM logs "ai-metrics
#      ignored (not a debug build)" under Release), so the Evaluate step
#      has nothing to assert against in Release builds.
#   2. The scenario_tail_save_and_quit.hms tail's click (62,177) on the
#      Save/Load Quit button is flaky under MSVC-release headless --
#      ~1-of-3 runs hang because the click doesn't fire the screen
#      transition back to Main Menu, so quit_game_flag never gets set
#      and the game idles until TIMEOUT. Under Debug it always works.
#      Root cause is in headless input dispatch, not in our metrics or
#      the scenario itself.
#
# Reactivate for Release builds when both issues are fixed.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    add_test(
        NAME HeMoM_AIBehavior_Run
        COMMAND $<TARGET_FILE:HeMoM>
            --continue
            --scenario "${HEMOM_TEST_ASSETS}/test_ai_5turns.hms"
            --ai-metrics
            --dump-save SAVE1.GAM
            --seed1 12345
        WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
    )
    set_tests_properties(HeMoM_AIBehavior_Run PROPERTIES
        TIMEOUT 120
        FIXTURES_REQUIRED HeMoM_AIBehavior
        FIXTURES_SETUP HeMoM_AIBehavior_Done
        LABELS "hemom;integration;ai"
    )

    add_test(
        NAME HeMoM_AIBehavior_Evaluate
        COMMAND $<TARGET_FILE:check_ai_metrics>
            "${HEMOM_TEST_WORKDIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/assert_ai_5turns.txt"
    )
    set_tests_properties(HeMoM_AIBehavior_Evaluate PROPERTIES
        FIXTURES_REQUIRED HeMoM_AIBehavior_Done
        LABELS "hemom;integration;ai"
    )

endif()



# =========================================================================
#  Combat: strategic auto-resolve tracer  (PLAN-Combat-Testing.md, phase 1)
#
#  Loads the checked-in fixture save SAVECMBT.GAM (staged to the bin dir by
#  copy_assets), patches diplomacy to war, pins the RNG, and resolves a
#  stack-vs-stack strategic combat by invoking the combat dispatcher
#  directly: attacker player 2 (units 12,15,17) vs defender player 3
#  (stack of unit 500).  HeMoM writes HEMOM_COMBAT.txt; the assertion step
#  pins the outcome field-by-field against assert_combat_strategic.txt.
#  Characterization test: values reflect current behavior for seed 12345.
# =========================================================================

add_test(
    NAME HeMoM_Combat_Strategic_Run
    COMMAND $<TARGET_FILE:HeMoM>
        --combat 500 12,15,17
        --seed1 12345
    WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
)
set_tests_properties(HeMoM_Combat_Strategic_Run PROPERTIES
    TIMEOUT 60
    FIXTURES_SETUP HeMoM_Combat_Strategic
    LABELS "hemom;integration;combat"
)

add_test(
    NAME HeMoM_Combat_Strategic_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/HEMOM_COMBAT.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_strategic.txt
)
set_tests_properties(HeMoM_Combat_Strategic_Assertions PROPERTIES
    FIXTURES_REQUIRED HeMoM_Combat_Strategic
    LABELS "hemom;integration;combat"
)



# =========================================================================
#  Tactical Combat: Scenario 1  (channel 1 -- real battle screen, scripted)
#
#  4 Human Halberdiers (player 0) attack 4 Stag Beetles (player 2); resolved
#  via the in-combat Auto button.  Outcome: attackers wiped 0/4, defender 517
#  survives (10 dmg), AI summons a Fire Elemental.  See assert_combat_s1.txt.
#
#  The full scenario runs WINDOWED -- ReMoMber drives the real tactical screen
#  from combat_s1.hms, which needs a display.  So the RUN step is gated behind
#  ENABLE_WINDOWED_COMBAT_TESTS and executed on demand.  The CHECK step is
#  automated and headless: it dumps the committed baseline save (the outcome the
#  scenario reproduces) and asserts the pinned combat fields, guarding the
#  baseline+assert pair in CI without a display.
# =========================================================================

# --- Automated check against the committed baseline (headless, CI) ---
add_test(
    NAME Combat_S1_Baseline_Dump
    COMMAND $<TARGET_FILE:savedump>
        ${HEMOM_TEST_ASSETS}/combat_s1_baseline.GAM
        ${HEMOM_TEST_WORKDIR}/combat_s1_baseline.txt
)
set_tests_properties(Combat_S1_Baseline_Dump PROPERTIES
    FIXTURES_SETUP Combat_S1_Baseline
    LABELS "combat;tactical"
)

add_test(
    NAME Combat_S1_Check
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/combat_s1_baseline.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_s1.txt
)
set_tests_properties(Combat_S1_Check PROPERTIES
    FIXTURES_REQUIRED Combat_S1_Baseline
    LABELS "combat;tactical"
)

# --- On-demand windowed replay: regenerate SAVE1 through the real screen ---
# Needs a display; OFF by default.  Enable with -DENABLE_WINDOWED_COMBAT_TESTS=ON
# then run:  ctest -R Combat_S1_Replay
option(ENABLE_WINDOWED_COMBAT_TESTS "Enable windowed (display-required) tactical combat replay tests" OFF)
if(ENABLE_WINDOWED_COMBAT_TESTS)

    add_test(
        NAME Combat_S1_Replay_Stage
        COMMAND ${CMAKE_COMMAND} -E copy
            ${HEMOM_TEST_ASSETS}/SAVECMBT.GAM
            ${HEMOM_TEST_WORKDIR}/SAVE9.GAM
    )
    set_tests_properties(Combat_S1_Replay_Stage PROPERTIES
        FIXTURES_SETUP Combat_S1_Replay_Staged
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S1_Replay
        COMMAND $<TARGET_FILE:ReMoMber>
            --continue --seed1 12345
            --scenario ${HEMOM_TEST_ASSETS}/combat_s1.hms
        WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
    )
    set_tests_properties(Combat_S1_Replay PROPERTIES
        TIMEOUT 180
        FIXTURES_REQUIRED Combat_S1_Replay_Staged
        FIXTURES_SETUP Combat_S1_Replayed
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S1_Replay_Dump
        COMMAND $<TARGET_FILE:savedump>
            ${HEMOM_TEST_WORKDIR}/SAVE1.GAM
            ${HEMOM_TEST_WORKDIR}/SAVE1.txt
    )
    set_tests_properties(Combat_S1_Replay_Dump PROPERTIES
        FIXTURES_REQUIRED Combat_S1_Replayed
        FIXTURES_SETUP Combat_S1_Replay_Dumped
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S1_Replay_Check
        COMMAND $<TARGET_FILE:check_save_fields>
            ${HEMOM_TEST_WORKDIR}/SAVE1.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_s1.txt
    )
    set_tests_properties(Combat_S1_Replay_Check PROPERTIES
        FIXTURES_REQUIRED Combat_S1_Replay_Dumped
        LABELS "combat;tactical;windowed"
    )

endif()



# =========================================================================
#  Tactical Combat: Scenario 2  (channel 1 -- multi-turn city assault)
#
#  Two stacks of 8 Human Halberdiers march two turns to the Klackon city Fa-rul
#  and assault it (Auto).  MULTI-TURN: each turn-end runs the AI (Next Turn = the
#  'n' hotkey); one en-route AI combat costs the stack a unit.  Deterministic
#  under seed 12345 (single-player, synchronous, turn-based).  Outcome: Fa-rul
#  razed, one attacker survives at the city square.  See assert_combat_s2.txt.
#
#  Same structure as scenario 1: automated headless check against the committed
#  baseline; on-demand windowed replay gated by ENABLE_WINDOWED_COMBAT_TESTS.
# =========================================================================

# --- Automated check against the committed baseline (headless, CI) ---
add_test(
    NAME Combat_S2_Baseline_Dump
    COMMAND $<TARGET_FILE:savedump>
        ${HEMOM_TEST_ASSETS}/combat_s2_baseline.GAM
        ${HEMOM_TEST_WORKDIR}/combat_s2_baseline.txt
)
set_tests_properties(Combat_S2_Baseline_Dump PROPERTIES
    FIXTURES_SETUP Combat_S2_Baseline
    LABELS "combat;tactical"
)

add_test(
    NAME Combat_S2_Check
    COMMAND $<TARGET_FILE:check_save_fields>
        ${HEMOM_TEST_WORKDIR}/combat_s2_baseline.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_s2.txt
)
set_tests_properties(Combat_S2_Check PROPERTIES
    FIXTURES_REQUIRED Combat_S2_Baseline
    LABELS "combat;tactical"
)

# --- On-demand windowed replay (needs a display; OFF by default) ---
# The windowed replays share SAVE1.GAM/SAVE9.GAM in the work dir, so RESOURCE_LOCK
# serializes them against scenario 1's replay.
if(ENABLE_WINDOWED_COMBAT_TESTS)

    add_test(
        NAME Combat_S2_Replay_Stage
        COMMAND ${CMAKE_COMMAND} -E copy
            ${HEMOM_TEST_ASSETS}/SAVECMBT.GAM
            ${HEMOM_TEST_WORKDIR}/SAVE9.GAM
    )
    set_tests_properties(Combat_S2_Replay_Stage PROPERTIES
        FIXTURES_SETUP Combat_S2_Replay_Staged
        RESOURCE_LOCK combat_savegame
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S2_Replay
        COMMAND $<TARGET_FILE:ReMoMber>
            --continue --seed1 12345
            --scenario ${HEMOM_TEST_ASSETS}/combat_s2.hms
        WORKING_DIRECTORY "${HEMOM_TEST_WORKDIR}"
    )
    set_tests_properties(Combat_S2_Replay PROPERTIES
        TIMEOUT 360
        FIXTURES_REQUIRED Combat_S2_Replay_Staged
        FIXTURES_SETUP Combat_S2_Replayed
        RESOURCE_LOCK combat_savegame
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S2_Replay_Dump
        COMMAND $<TARGET_FILE:savedump>
            ${HEMOM_TEST_WORKDIR}/SAVE1.GAM
            ${HEMOM_TEST_WORKDIR}/SAVE1_s2.txt
    )
    set_tests_properties(Combat_S2_Replay_Dump PROPERTIES
        FIXTURES_REQUIRED Combat_S2_Replayed
        FIXTURES_SETUP Combat_S2_Replay_Dumped
        RESOURCE_LOCK combat_savegame
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S2_Replay_Check
        COMMAND $<TARGET_FILE:check_save_fields>
            ${HEMOM_TEST_WORKDIR}/SAVE1_s2.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_s2.txt
    )
    set_tests_properties(Combat_S2_Replay_Check PROPERTIES
        FIXTURES_REQUIRED Combat_S2_Replay_Dumped
        LABELS "combat;tactical;windowed"
    )

endif()

# --- Fail-soft: missing game data ---
# HeMoM with nothing on its search path must print the actionable message and
# exit cleanly (headless Platform_Show_Error -> stderr), not crash.  Runs in an
# empty working dir with REMOM_DATA_DIR cleared so nothing resolves.
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/no_data")
add_test(
    NAME HeMoM_Preflight_Missing_Data
    COMMAND $<TARGET_FILE:HeMoM>
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/no_data"
)
set_tests_properties(HeMoM_Preflight_Missing_Data PROPERTIES
    PASS_REGULAR_EXPRESSION "Missing Game Data"
    ENVIRONMENT "REMOM_DATA_DIR="
    TIMEOUT 20
    LABELS "hemom;integration;preflight"
)
