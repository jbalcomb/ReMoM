# validate_save.cmake — Validate that a save file exists, has the correct size and (optionally) starts with a magic.
#
# Usage:
#   cmake -DSAVE_FILE=SAVE9.GAM -DEXPECTED_SIZE=125748 [-DEXPECTED_MAGIC=ReMoMSav] -P validate_save.cmake
#
# A ReMoM save (MoX/src/SAVEFILE.c) is 125748 bytes: the 1.31 state (123300), cities 20 bytes wider each (+2000),
# and the header plus 27 section headers (16 + 27 * 16 = 448).
#
# Exit codes:
#   0 = pass (file exists, correct size, magic matches)
#   1 = fail

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

if(DEFINED EXPECTED_MAGIC)
    string(LENGTH "${EXPECTED_MAGIC}" magic_len)
    file(READ "${SAVE_FILE}" actual_magic_hex LIMIT ${magic_len} HEX)
    string(HEX "${EXPECTED_MAGIC}" expected_magic_hex)
    if(NOT actual_magic_hex STREQUAL expected_magic_hex)
        message(FATAL_ERROR "FAIL: ${SAVE_FILE} starts with bytes ${actual_magic_hex}, expected '${EXPECTED_MAGIC}' (${expected_magic_hex})")
    endif()
endif()

message(STATUS "PASS: ${SAVE_FILE} exists, size=${actual_size} (expected ${EXPECTED_SIZE})")
