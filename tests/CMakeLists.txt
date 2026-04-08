
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
    TIMEOUT 120
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
# Compares the save file against a known-good golden reference.
# The golden file was generated with seed=12345 and the same config.
# Any code change that alters world gen, player init, or save format
# will cause this to fail — which is exactly the point.

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
)

# --- Structured Field Comparison ---
# Compares the structured text dump field-by-field using a compiled C tool.
# When this fails, the output shows exactly which fields changed:
#   DIFF line 42:
#     actual:   player[0].spellranks[sbr_Chaos] = 3
#     expected: player[0].spellranks[sbr_Chaos] = 5

add_executable(compare_save_text compare_save_text.c)
add_executable(check_save_fields check_save_fields.c)

add_test(
    NAME HeMoM_WorldGen_Fields
    COMMAND $<TARGET_FILE:compare_save_text>
        ${HEMOM_TEST_WORKDIR}/SAVE9.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/golden_worldgen_seed12345.txt
)
set_tests_properties(HeMoM_WorldGen_Fields PROPERTIES
    FIXTURES_REQUIRED HeMoM_WorldGen
    LABELS "hemom;integration;worldgen;golden"
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
    TIMEOUT 120
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
    TIMEOUT 120
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
