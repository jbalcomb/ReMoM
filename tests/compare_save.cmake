# compare_save.cmake — Byte-level comparison of a save file against a golden reference.
#
# Usage:
#   cmake -DACTUAL=SAVE9.GAM -DEXPECTED=golden.GAM -P compare_save.cmake
#
# Computes MD5 hashes of both files and compares them.
# If they differ, reports the file sizes and hash values so you know
# the output changed (though not yet which bytes — use a hex diff tool for that).
#
# Exit codes:
#   0 = pass (byte-identical)
#   1 = fail (different)

if(NOT DEFINED ACTUAL)
    message(FATAL_ERROR "ACTUAL not defined")
endif()

if(NOT DEFINED EXPECTED)
    message(FATAL_ERROR "EXPECTED not defined")
endif()

if(NOT EXISTS "${ACTUAL}")
    message(FATAL_ERROR "FAIL: actual file does not exist: ${ACTUAL}")
endif()

if(NOT EXISTS "${EXPECTED}")
    message(FATAL_ERROR "FAIL: expected (golden) file does not exist: ${EXPECTED}")
endif()

file(SIZE "${ACTUAL}" actual_size)
file(SIZE "${EXPECTED}" expected_size)

file(MD5 "${ACTUAL}" actual_md5)
file(MD5 "${EXPECTED}" expected_md5)

if(NOT actual_size EQUAL expected_size)
    message(FATAL_ERROR "FAIL: size mismatch — actual=${actual_size} expected=${expected_size}\n  actual_md5=${actual_md5}\n  expected_md5=${expected_md5}\n  actual=${ACTUAL}\n  expected=${EXPECTED}")
endif()

if(NOT actual_md5 STREQUAL expected_md5)
    message(FATAL_ERROR "FAIL: content mismatch (same size=${actual_size})\n  actual_md5=${actual_md5}\n  expected_md5=${expected_md5}\n  actual=${ACTUAL}\n  expected=${EXPECTED}\n\nUse a hex diff tool to find the divergence:\n  diff <(xxd ${EXPECTED}) <(xxd ${ACTUAL})")
endif()

message(STATUS "PASS: byte-identical (size=${actual_size} md5=${actual_md5})")
