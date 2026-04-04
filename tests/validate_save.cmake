# validate_save.cmake — Validate that a save file exists and has the correct size.
#
# Usage:
#   cmake -DSAVE_FILE=SAVE9.GAM -DEXPECTED_SIZE=123300 -P validate_save.cmake
#
# Exit codes:
#   0 = pass (file exists, correct size)
#   1 = fail (file missing or wrong size)

if(NOT DEFINED SAVE_FILE)
    message(FATAL_ERROR "SAVE_FILE not defined")
endif()

if(NOT DEFINED EXPECTED_SIZE)
    set(EXPECTED_SIZE 123300)
endif()

if(NOT EXISTS "${SAVE_FILE}")
    message(FATAL_ERROR "FAIL: ${SAVE_FILE} does not exist")
endif()

file(SIZE "${SAVE_FILE}" actual_size)

if(NOT actual_size EQUAL ${EXPECTED_SIZE})
    message(FATAL_ERROR "FAIL: ${SAVE_FILE} size=${actual_size}, expected=${EXPECTED_SIZE}")
endif()

message(STATUS "PASS: ${SAVE_FILE} exists, size=${actual_size} (expected ${EXPECTED_SIZE})")
