
include(GoogleTest)

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test GTest::gtest_main)
gtest_discover_tests(hello_test
	PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)



# =========================================================================
#  DeMoM Integration Tests
#
#  These tests use the DeMoM windowless executable to run game scenarios
#  and validate results. Each test is a pair:
#    1. Run DeMoM with a config + scenario (must exit 0)
#    2. Validate the output (save file exists, correct size)
#
#  The tests use CTest fixtures so the validation step only runs if
#  DeMoM completed successfully.
# =========================================================================

set(DEMOM_TEST_ASSETS "${PROJECT_SOURCE_DIR}/assets")
set(DEMOM_TEST_WORKDIR "$<TARGET_FILE_DIR:DeMoM>")

# Each DeMoM test group runs in its own working directory under the test build dir.  DeMoM writes its
# saves, dumps, logs and MAGIC.SET to the working directory; when every test ran in the bin dir, any
# other DeMoM run there (another session, a manual run, a parallel test group) overwrote SAVE1.GAM /
# SAVE9.GAM / the dumps mid-test.  The game data (LBX, CONFIG.MOM) stays in the bin dir, where
# copy_assets stages it, and REMOM_DATA_DIR points DeMoM at it.
#
# demom_stage_workdir(GROUP [DEBUG_ONLY] FILES f...) adds DeMoM_<GROUP>_Stage: wipe the group's
# directory and copy FILES in (FIXTURES_SETUP DeMoM_<GROUP>_Workdir).  Every DeMoM run in the group
# requires that fixture.  DEBUG_ONLY matches a group whose tests are CONFIGURATIONS Debug.
set(DEMOM_TEST_DATA_DIR "$<TARGET_FILE_DIR:DeMoM>")
set(DEMOM_WORK_ROOT "${CMAKE_CURRENT_BINARY_DIR}/demom_work")

function(demom_stage_workdir group)
    cmake_parse_arguments(STAGE "DEBUG_ONLY" "" "FILES" ${ARGN})
    string(REPLACE ";" "|" files "${STAGE_FILES}")
    if(STAGE_DEBUG_ONLY)
        set(configurations CONFIGURATIONS Debug)
    else()
        set(configurations "")
    endif()
    add_test(
        NAME DeMoM_${group}_Stage
        ${configurations}
        COMMAND ${CMAKE_COMMAND}
            -DWORK_DIR=${DEMOM_WORK_ROOT}/${group}
            -DFILES=${files}
            -P "${CMAKE_CURRENT_SOURCE_DIR}/stage_demom_workdir.cmake"
    )
    set_tests_properties(DeMoM_${group}_Stage PROPERTIES
        FIXTURES_SETUP DeMoM_${group}_Workdir
        LABELS "demom;integration;stage"
    )
endfunction()

demom_stage_workdir(WorldGen         FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET")
demom_stage_workdir(Continue         FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET")
demom_stage_workdir(NextTurn         FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET")
demom_stage_workdir(AIBehavior       DEBUG_ONLY FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET")
demom_stage_workdir(Combat_Strategic FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET" "${DEMOM_TEST_ASSETS}/SAVECMBT.GAM")
demom_stage_workdir(Cast_Summon      FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET" "${DEMOM_TEST_ASSETS}/SAVECMBT.GAM")
demom_stage_workdir(AI_Dead_Units    FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET" "${DEMOM_TEST_ASSETS}/SAVECMBT.GAM")
demom_stage_workdir(L61              FILES "${DEMOM_TEST_ASSETS}/MAGIC.SET")

# --- Smoke Test: World Generation ---
# Verifies: engine init, config parsing, wizard profile, world gen,
#           event init, finalize tables, save file creation, clean exit.

add_test(
    NAME DeMoM_WorldGen_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --newgame "${DEMOM_TEST_ASSETS}/test_worldgen.ini"
        --scenario "${DEMOM_TEST_ASSETS}/test_worldgen_smoke.hms"
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/WorldGen"
)
set_tests_properties(DeMoM_WorldGen_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_WorldGen_Workdir
    TIMEOUT 30
    FIXTURES_SETUP DeMoM_WorldGen
    LABELS "demom;integration;worldgen"
)

add_test(
    NAME DeMoM_WorldGen_Validate
    COMMAND ${CMAKE_COMMAND}
        -DSAVE_FILE=${DEMOM_WORK_ROOT}/WorldGen/SAVE9.GAM
        -DEXPECTED_SIZE=123300
        -P "${CMAKE_CURRENT_SOURCE_DIR}/validate_save.cmake"
)
set_tests_properties(DeMoM_WorldGen_Validate PROPERTIES
    FIXTURES_REQUIRED DeMoM_WorldGen
    LABELS "demom;integration;worldgen"
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
# Replaced by DeMoM_WorldGen_Fields below, which uses check_save_fields with
# an explicit list of meaningful fields (assert_worldgen.txt).  The dump
# tooling and tools/regenerate_goldens.sh are kept for diagnostic use even
# though no test compares against the .GAM directly anymore.

add_test(
    NAME DeMoM_WorldGen_Golden
    COMMAND ${CMAKE_COMMAND}
        -DACTUAL=${DEMOM_WORK_ROOT}/WorldGen/SAVE9.GAM
        -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/golden_worldgen_seed12345.GAM
        -P "${CMAKE_CURRENT_SOURCE_DIR}/compare_save.cmake"
)
set_tests_properties(DeMoM_WorldGen_Golden PROPERTIES
    FIXTURES_REQUIRED DeMoM_WorldGen
    LABELS "demom;integration;worldgen;golden"
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
    NAME DeMoM_WorldGen_Fields
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/WorldGen/SAVE9.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_worldgen.txt
)
set_tests_properties(DeMoM_WorldGen_Fields PROPERTIES
    FIXTURES_REQUIRED DeMoM_WorldGen
    LABELS "demom;integration;worldgen"
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
# last-row values.  Reuses the DeMoM_WorldGen fixture's SAVE9.txt (no extra run).
add_test(
    NAME DeMoM_OOB_Autotiling
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/WorldGen/SAVE9.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_oob_autotiling.txt
)
set_tests_properties(DeMoM_OOB_Autotiling PROPERTIES
    FIXTURES_REQUIRED DeMoM_WorldGen
    LABELS "demom;integration;worldgen;oob"
)



# =========================================================================
#  Continue + Save Test
#
#  This test creates its own game-save (does not depend on DeMoM_WorldGen
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
    NAME DeMoM_Continue_Setup
    COMMAND $<TARGET_FILE:DeMoM>
        --newgame "${DEMOM_TEST_ASSETS}/test_worldgen.ini"
        --scenario "${DEMOM_TEST_ASSETS}/test_worldgen_smoke.hms"
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/Continue"
)
set_tests_properties(DeMoM_Continue_Setup PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_Continue_Workdir
    TIMEOUT 30
    FIXTURES_SETUP DeMoM_Continue
    LABELS "demom;integration;continue"
)

add_test(
    NAME DeMoM_Continue_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --continue
        --scenario "${DEMOM_TEST_ASSETS}/test_continue_save.hms"
        --dump-save SAVE1.GAM
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/Continue"
)
set_tests_properties(DeMoM_Continue_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    TIMEOUT 30
    TIMEOUT 60
    FIXTURES_REQUIRED "DeMoM_Continue_Workdir;DeMoM_Continue"
    FIXTURES_SETUP DeMoM_Continue_Saved
    LABELS "demom;integration;continue"
)

add_test(
    NAME DeMoM_Continue_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/Continue/SAVE1.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_continue_save.txt
)
set_tests_properties(DeMoM_Continue_Assertions PROPERTIES
    FIXTURES_REQUIRED DeMoM_Continue_Saved
    LABELS "demom;integration;continue"
)



# =========================================================================
#  Continue + Next Turn Test
#
#  Extends the Continue + Save test with one Next Turn advancement.  Creates
#  its own game-save via --newgame (independent of DeMoM_Continue), runs a
#  scenario that names the home city, patrols the starting stack, clicks
#  Next Turn, waits for the AI to finish, then saves to slot 1 and quits.
#
#  Assertions cover the scenario-caused state changes plus the Next Turn
#  effects (game.turn advancing, AI unit movement orders).  Load-time
#  recomputation is deliberately not asserted.
# =========================================================================

add_test(
    NAME DeMoM_NextTurn_Setup
    COMMAND $<TARGET_FILE:DeMoM>
        --newgame "${DEMOM_TEST_ASSETS}/test_worldgen.ini"
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/NextTurn"
)
set_tests_properties(DeMoM_NextTurn_Setup PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_NextTurn_Workdir
    TIMEOUT 30
    FIXTURES_SETUP DeMoM_NextTurn
    LABELS "demom;integration;nextturn"
)

add_test(
    NAME DeMoM_NextTurn_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --continue
        --scenario "${DEMOM_TEST_ASSETS}/test_continue_next_turn.hms"
        --dump-save SAVE1.GAM
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/NextTurn"
)
set_tests_properties(DeMoM_NextTurn_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    TIMEOUT 90
    FIXTURES_REQUIRED "DeMoM_NextTurn_Workdir;DeMoM_NextTurn"
    FIXTURES_SETUP DeMoM_NextTurn_Saved
    LABELS "demom;integration;nextturn"
)

add_test(
    NAME DeMoM_NextTurn_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/NextTurn/SAVE1.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_continue_next_turn.txt
)
set_tests_properties(DeMoM_NextTurn_Assertions PROPERTIES
    FIXTURES_REQUIRED DeMoM_NextTurn_Saved
    LABELS "demom;integration;nextturn"
)



# =========================================================================
#  AI Behavior Test
#
#  Runs 5 turns windowless with --ai-metrics enabled, then evaluates the
#  resulting CSV files against behavioral assertions using check_ai_metrics.
#
#  Assertions cover:
#    - Liveness: players have units/cities, turn data exists
#    - Bounds: gold/mana/unit counts stay in sane ranges
# =========================================================================

add_executable(check_ai_metrics check_ai_metrics.c)

add_test(
    NAME DeMoM_AIBehavior_Setup
    # Debug-only: this fixture only feeds the Debug-config Run/Evaluate below, so
    # gate it too -- otherwise it runs a dangling ~1.5s newgame in every other
    # config for a fixture nothing there requires.
    CONFIGURATIONS Debug
    COMMAND $<TARGET_FILE:DeMoM>
        --newgame "${DEMOM_TEST_ASSETS}/test_worldgen.ini"
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/AIBehavior"
)
# TIMEOUT was 10s when --newgame world-gen took ~0.26s.  A fixed ~1.5s cost was
# since added to every new game (measured ~1.4-1.75s solo, flat across land sizes
# -- NOT content-scaled, so it won't creep as worlds grow), and ~5.5s under
# parallel contention.  10s became flaky under load.  20s = a few x the realistic
# sequential worst-case, enough to
# absorb slow-CI variance while still catching a real hang promptly.
set_tests_properties(DeMoM_AIBehavior_Setup PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_AIBehavior_Workdir
    TIMEOUT 60
    FIXTURES_SETUP DeMoM_AIBehavior
    LABELS "demom;integration;ai"
)

# DeMoM_AIBehavior_Run + DeMoM_AIBehavior_Evaluate are Debug-config-only.
#
# Two reasons:
#   1. --ai-metrics is gated by #ifdef STU_DEBUG (DeMoM logs "ai-metrics
#      ignored (not a debug build)" under Release), so the Evaluate step
#      has nothing to assert against in Release builds.
#   2. The scenario_tail_save_and_quit.hms tail's click (62,177) on the
#      Save/Load Quit button is flaky under MSVC-release windowless --
#      ~1-of-3 runs hang because the click doesn't fire the screen
#      transition back to Main Menu, so quit_game_flag never gets set
#      and the game idles until TIMEOUT. Under Debug it always works.
#      Root cause is in windowless input dispatch, not in our metrics or
#      the scenario itself.
#
# "Debug-only" is expressed via add_test()'s CONFIGURATIONS argument, NOT a
# configure-time `if(CMAKE_BUILD_TYPE STREQUAL "Debug")`.  Under a multi-config
# generator (Ninja Multi-Config, Visual Studio) CMAKE_BUILD_TYPE is empty -- the
# config is chosen at build/test time (`ctest -C Debug`) -- so that `if()` was
# always false and these tests never registered at all (the AI-metrics coverage
# silently vanished; Setup became a dangling fixture).  CONFIGURATIONS Debug is
# honored by both single- and multi-config generators: the test registers, and
# runs only when the active config is Debug.
#
# It must be the add_test() argument.  There is no CONFIGURATIONS test property:
# set_tests_properties(... CONFIGURATIONS Debug) is silently ignored, and the
# three tests ran (and Evaluate failed) under MSVC-release.
add_test(
    NAME DeMoM_AIBehavior_Run
    CONFIGURATIONS Debug
    COMMAND $<TARGET_FILE:DeMoM>
        --continue
        --scenario "${DEMOM_TEST_ASSETS}/test_ai_5turns.hms"
        --ai-metrics
        --dump-save SAVE1.GAM
        --seed1 12345
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/AIBehavior"
)
set_tests_properties(DeMoM_AIBehavior_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    TIMEOUT 120
    FIXTURES_REQUIRED "DeMoM_AIBehavior_Workdir;DeMoM_AIBehavior"
    FIXTURES_SETUP DeMoM_AIBehavior_Done
    LABELS "demom;integration;ai"
)

add_test(
    NAME DeMoM_AIBehavior_Evaluate
    CONFIGURATIONS Debug
    COMMAND $<TARGET_FILE:check_ai_metrics>
        "${DEMOM_WORK_ROOT}/AIBehavior"
        "${CMAKE_CURRENT_SOURCE_DIR}/assert_ai_5turns.txt"
)
set_tests_properties(DeMoM_AIBehavior_Evaluate PROPERTIES
    FIXTURES_REQUIRED DeMoM_AIBehavior_Done
    LABELS "demom;integration;ai"
)



# =========================================================================
#  Combat: strategic auto-resolve tracer  (PLAN-Combat-Testing.md, phase 1)
#
#  Loads the checked-in fixture save SAVECMBT.GAM (staged to the group's work
#  dir by DeMoM_Combat_Strategic_Stage), patches diplomacy to war, pins the RNG, and resolves a
#  stack-vs-stack strategic combat by invoking the combat dispatcher
#  directly: attacker player 2 (units 12,15,17) vs defender player 3
#  (stack of unit 500).  DeMoM writes DEMOM_COMBAT.txt; the assertion step
#  pins the outcome field-by-field against assert_combat_strategic.txt.
#  Characterization test: values reflect current behavior for seed 12345.
# =========================================================================

add_test(
    NAME DeMoM_Combat_Strategic_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --combat 500 12,15,17
        --seed1 12345
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/Combat_Strategic"
)
set_tests_properties(DeMoM_Combat_Strategic_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_Combat_Strategic_Workdir
    TIMEOUT 60
    FIXTURES_SETUP DeMoM_Combat_Strategic
    LABELS "demom;integration;combat"
)

add_test(
    NAME DeMoM_Combat_Strategic_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/Combat_Strategic/DEMOM_COMBAT.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_strategic.txt
)
set_tests_properties(DeMoM_Combat_Strategic_Assertions PROPERTIES
    FIXTURES_REQUIRED DeMoM_Combat_Strategic
    LABELS "demom;integration;combat"
)



# =========================================================================
#  Overland Summon: map focus after the human player's creature summon
#  (GitHub issue #21)
#
#  Loads SAVECMBT.GAM, kills the last two units so dead records sit past the
#  end of _UNITS[], casts Magic Spirit (201) for the human player through the
#  real Cast_Spell_Overland(), then draws the main screen.  The bug read the
#  record past the new unit and set _map_plane = -1; the draw then crashed.
#  DeMoM writes DEMOM_SUMMON.txt; see assert_cast_summon.txt.
# =========================================================================

add_test(
    NAME DeMoM_Cast_Summon_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --cast-summon 201
        --seed1 12345
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/Cast_Summon"
)
set_tests_properties(DeMoM_Cast_Summon_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_Cast_Summon_Workdir
    TIMEOUT 60
    FIXTURES_SETUP DeMoM_Cast_Summon
    LABELS "demom;integration;spells"
)

add_test(
    NAME DeMoM_Cast_Summon_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/Cast_Summon/DEMOM_SUMMON.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_cast_summon.txt
)
set_tests_properties(DeMoM_Cast_Summon_Assertions PROPERTIES
    FIXTURES_REQUIRED DeMoM_Cast_Summon
    LABELS "demom;integration;spells"
)



# =========================================================================
#  AI turn with dead units in _UNITS[]  (BUGS_AND_QUIRKS #137)
#
#  Loads SAVECMBT.GAM, kills units 21 (hero), 75 (human hero), 300 and 776
#  (normal; 776 on Myrror) with the real Kill_Unit() without compacting, so
#  the dead records sit in the middle of _UNITS[].  Probes the real
#  AI_Player_Calculate_Target_Values() on the Kill_Unit() state (owner -1,
#  wp -1), runs the real AI_Next_Turn() (which turns them into owner -1,
#  wp 0), and probes again.  Dead units -- heroes included -- go through
#  Effective_Unit_Strength() and its bounded _players[-1] / _HEROES2[-1]
#  reads.  DeMoM writes DEMOM_AI_DEAD_UNITS.txt; see assert_ai_dead_units.txt.
# =========================================================================

add_test(
    NAME DeMoM_AI_Dead_Units_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --ai-dead-units 21,75,300,776
        --seed1 12345
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/AI_Dead_Units"
)
# TIMEOUT: 60 like the other fixture runs (measured 0.67-0.89s).
set_tests_properties(DeMoM_AI_Dead_Units_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_AI_Dead_Units_Workdir
    TIMEOUT 60
    FIXTURES_SETUP DeMoM_AI_Dead_Units
    LABELS "demom;integration;ai"
)

add_test(
    NAME DeMoM_AI_Dead_Units_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/AI_Dead_Units/DEMOM_AI_DEAD_UNITS.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_ai_dead_units.txt
)
set_tests_properties(DeMoM_AI_Dead_Units_Assertions PROPERTIES
    FIXTURES_REQUIRED DeMoM_AI_Dead_Units
    LABELS "demom;integration;ai"
)



# =========================================================================
#  L61: Second Game in One Process == First Game in a Fresh Process
#
#  The original restarted the program between games (Respawn MAGIC.EXE on the
#  way to the menu, a fresh WIZARDS.EXE on the way into a game), so every game
#  started from clean data.  ReMoM runs every game in one process.
#
#  Both runs start at the Main Menu (--menu) with the same --seed1 and create
#  their games through the New Game screens with the same choices:
#    - Fresh:  New Game -> save slot 1.
#    - Second: New Game -> Game -> Quit -> Main Menu -> the same New Game ->
#              save slot 1.
#  The second run's whole SAVE1 dump is checked against the fresh run's, every
#  field.  Any difference is state that game 1 left behind (L61, AF8, AF11).
#  The fresh run is fully repeatable (66068 of 66068 fields match run to run).
# =========================================================================

add_test(
    NAME DeMoM_L61_Fresh_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --menu --seed1 12345
        --scenario "${DEMOM_TEST_ASSETS}/test_l61_fresh.hms"
        --dump-save SAVE1.GAM
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/L61"
)
set_tests_properties(DeMoM_L61_Fresh_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    FIXTURES_REQUIRED DeMoM_L61_Workdir
    TIMEOUT 60
    FIXTURES_SETUP DeMoM_L61_Fresh_Ran
    LABELS "demom;integration;l61"
)

# The scripted New Game really happened (a dropped click otherwise saves an empty game and exits 0).
add_test(
    NAME DeMoM_L61_Fresh_Assertions
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/L61/SAVE1.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/assert_l61_fresh.txt
)
set_tests_properties(DeMoM_L61_Fresh_Assertions PROPERTIES
    FIXTURES_REQUIRED DeMoM_L61_Fresh_Ran
    FIXTURES_SETUP DeMoM_L61_Fresh_Checked
    LABELS "demom;integration;l61"
)

add_test(
    NAME DeMoM_L61_Fresh_Keep
    COMMAND ${CMAKE_COMMAND} -E copy
        ${DEMOM_WORK_ROOT}/L61/SAVE1.txt
        ${DEMOM_WORK_ROOT}/L61/L61_FRESH_SAVE1.txt
)
set_tests_properties(DeMoM_L61_Fresh_Keep PROPERTIES
    FIXTURES_REQUIRED "DeMoM_L61_Fresh_Ran;DeMoM_L61_Fresh_Checked"
    FIXTURES_SETUP DeMoM_L61_Fresh
    LABELS "demom;integration;l61"
)

add_test(
    NAME DeMoM_L61_Second_Game_Run
    COMMAND $<TARGET_FILE:DeMoM>
        --menu --seed1 12345
        --scenario "${DEMOM_TEST_ASSETS}/test_l61_second_game.hms"
        --dump-save SAVE1.GAM
    WORKING_DIRECTORY "${DEMOM_WORK_ROOT}/L61"
)
set_tests_properties(DeMoM_L61_Second_Game_Run PROPERTIES
    ENVIRONMENT "REMOM_DATA_DIR=${DEMOM_TEST_DATA_DIR}"
    TIMEOUT 90
    FIXTURES_REQUIRED "DeMoM_L61_Workdir;DeMoM_L61_Fresh"
    FIXTURES_SETUP DeMoM_L61_Second_Game
    LABELS "demom;integration;l61"
)

add_test(
    NAME DeMoM_L61_Compare
    COMMAND $<TARGET_FILE:check_save_fields>
        ${DEMOM_WORK_ROOT}/L61/SAVE1.txt
        ${DEMOM_WORK_ROOT}/L61/L61_FRESH_SAVE1.txt
)
set_tests_properties(DeMoM_L61_Compare PROPERTIES
    FIXTURES_REQUIRED DeMoM_L61_Second_Game
    LABELS "demom;integration;l61"
)



# =========================================================================
#  Tactical Combat: Scenario 1  (channel 1 -- real battle screen, scripted)
#
#  4 Human Halberdiers (player 0) attack 4 Stag Beetles (player 2); resolved
#  via the in-combat Auto button.  Outcome: attackers wiped 0/4, defender 517
#  survives (10 dmg), AI summons a Fire Elemental.  See assert_combat_s1.txt.
#
#  The full scenario runs WINDOWED -- ReMoM drives the real tactical screen
#  from combat_s1.hms, which needs a display.  So the replay is gated behind
#  ENABLE_WINDOWED_COMBAT_TESTS and executed on demand.  There is no windowless
#  CI check: tactical combat is a rendering screen and cannot run under
#  Platform_Windowless (see BRA-Combat-Testing.md), so the assertions in
#  assert_combat_s1.txt can only be produced and checked through the windowed
#  replay below.
# =========================================================================

# --- On-demand windowed replay: regenerate SAVE1 through the real screen ---
# Needs a display; OFF by default.  Enable with -DENABLE_WINDOWED_COMBAT_TESTS=ON
# then run:  ctest -R Combat_S1_Replay
option(ENABLE_WINDOWED_COMBAT_TESTS "Enable windowed (display-required) tactical combat replay tests" OFF)
if(ENABLE_WINDOWED_COMBAT_TESTS)

    add_test(
        NAME Combat_S1_Replay_Stage
        COMMAND ${CMAKE_COMMAND} -E copy
            ${DEMOM_TEST_ASSETS}/SAVECMBT.GAM
            ${DEMOM_TEST_WORKDIR}/SAVE9.GAM
    )
    set_tests_properties(Combat_S1_Replay_Stage PROPERTIES
        FIXTURES_SETUP Combat_S1_Replay_Staged
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S1_Replay
        COMMAND $<TARGET_FILE:ReMoM>
            --continue --seed1 12345
            --scenario ${DEMOM_TEST_ASSETS}/combat_s1.hms
        WORKING_DIRECTORY "${DEMOM_TEST_WORKDIR}"
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
            ${DEMOM_TEST_WORKDIR}/SAVE1.GAM
            ${DEMOM_TEST_WORKDIR}/SAVE1.txt
    )
    set_tests_properties(Combat_S1_Replay_Dump PROPERTIES
        FIXTURES_REQUIRED Combat_S1_Replayed
        FIXTURES_SETUP Combat_S1_Replay_Dumped
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S1_Replay_Check
        COMMAND $<TARGET_FILE:check_save_fields>
            ${DEMOM_TEST_WORKDIR}/SAVE1.txt
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
#  Same structure as scenario 1: on-demand windowed replay gated by
#  ENABLE_WINDOWED_COMBAT_TESTS.  No windowless CI check -- tactical combat
#  cannot run under Platform_Windowless (see BRA-Combat-Testing.md).
# =========================================================================

# --- On-demand windowed replay (needs a display; OFF by default) ---
# The windowed replays share SAVE1.GAM/SAVE9.GAM in the work dir, so RESOURCE_LOCK
# serializes them against scenario 1's replay.
if(ENABLE_WINDOWED_COMBAT_TESTS)

    add_test(
        NAME Combat_S2_Replay_Stage
        COMMAND ${CMAKE_COMMAND} -E copy
            ${DEMOM_TEST_ASSETS}/SAVECMBT.GAM
            ${DEMOM_TEST_WORKDIR}/SAVE9.GAM
    )
    set_tests_properties(Combat_S2_Replay_Stage PROPERTIES
        FIXTURES_SETUP Combat_S2_Replay_Staged
        RESOURCE_LOCK combat_savegame
        LABELS "combat;tactical;windowed"
    )

    add_test(
        NAME Combat_S2_Replay
        COMMAND $<TARGET_FILE:ReMoM>
            --continue --seed1 12345
            --scenario ${DEMOM_TEST_ASSETS}/combat_s2.hms
        WORKING_DIRECTORY "${DEMOM_TEST_WORKDIR}"
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
            ${DEMOM_TEST_WORKDIR}/SAVE1.GAM
            ${DEMOM_TEST_WORKDIR}/SAVE1_s2.txt
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
            ${DEMOM_TEST_WORKDIR}/SAVE1_s2.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/assert_combat_s2.txt
    )
    set_tests_properties(Combat_S2_Replay_Check PROPERTIES
        FIXTURES_REQUIRED Combat_S2_Replay_Dumped
        LABELS "combat;tactical;windowed"
    )

endif()

# --- Fail-soft: missing game data ---
# DeMoM with nothing on its search path must print the actionable message and
# exit cleanly (windowless Platform_Show_Error -> stderr), not crash.  Runs in an
# empty working dir with REMOM_DATA_DIR cleared so nothing resolves.
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/no_data")
add_test(
    NAME DeMoM_Preflight_Missing_Data
    COMMAND $<TARGET_FILE:DeMoM>
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/no_data"
)
set_tests_properties(DeMoM_Preflight_Missing_Data PROPERTIES
    PASS_REGULAR_EXPRESSION "Missing Game Data"
    ENVIRONMENT "REMOM_DATA_DIR="
    TIMEOUT 20
    LABELS "demom;integration;preflight"
)



# =========================================================================
#  Performance regression gate  (PRD/PLAN-Performance-Management.md)
#
#  Compares a .fwv zone capture against the committed reference baseline.
#  Asserts what is actually reproducible on the reference host:
#    - STRUCTURE: identical zone set and per-zone instance counts, exact.
#      No timing component, so it never flakes; catches a changed code path.
#    - CEILINGS: p50/total within 2.5x of baseline.  Deliberately loose --
#      two back-to-back captures of the same scenario measured p50 varying
#      0.46..1.66x and max 0.12..24.46x, so a tighter bound would flake.
#      See the header comment in check_perf_baseline.c for the measurements.
#
#  This test compares the two COMMITTED captures, so it needs no display and
#  no game run -- it guards the comparator and the reference data themselves.
#  Gating a FRESH capture additionally needs a windowed run driven by
#  assets/perf_modea_lategame.hms, which is display-dependent (Mode B); that
#  step is not wired yet.
# =========================================================================

add_executable(check_perf_baseline check_perf_baseline.c)

set(PERF_BASELINE_DIR "${PROJECT_SOURCE_DIR}/doc/PlatformLayer/perf-baselines")

add_test(
    NAME Perf_Baseline_Compare
    COMMAND $<TARGET_FILE:check_perf_baseline>
        "${PERF_BASELINE_DIR}/median5_2026-07-29_SDL2_release_lategame.fwv"
        "${PERF_BASELINE_DIR}/repeat_2026-07-29_SDL2_release_lategame.fwv"
        --ratio 2.0 --min-samples 12
)
set_tests_properties(Perf_Baseline_Compare PROPERTIES
    TIMEOUT 30
    LABELS "perf;regression"
)
