# compare_save_text.cmake — Structured field-by-field comparison of save dump text files.
#
# Usage:
#   cmake -DACTUAL=SAVE9.txt -DEXPECTED=golden.txt -P compare_save_text.cmake
#
# Reads both files line by line and reports every difference by field name.
# Stops after MAX_DIFFS differences (default: 20) to avoid flooding output.
#
# Exit codes:
#   0 = pass (identical)
#   1 = fail (differences found)

if(NOT DEFINED ACTUAL)
    message(FATAL_ERROR "ACTUAL not defined")
endif()

if(NOT DEFINED EXPECTED)
    message(FATAL_ERROR "EXPECTED not defined")
endif()

if(NOT DEFINED MAX_DIFFS)
    set(MAX_DIFFS 20)
endif()

if(NOT EXISTS "${ACTUAL}")
    message(FATAL_ERROR "FAIL: actual file does not exist: ${ACTUAL}")
endif()

if(NOT EXISTS "${EXPECTED}")
    message(FATAL_ERROR "FAIL: expected (golden) file does not exist: ${EXPECTED}")
endif()

# Read both files into lists (one element per line)
file(STRINGS "${ACTUAL}" actual_lines)
file(STRINGS "${EXPECTED}" expected_lines)

list(LENGTH actual_lines actual_count)
list(LENGTH expected_lines expected_count)

if(NOT actual_count EQUAL expected_count)
    message(WARNING "Line count mismatch: actual=${actual_count} expected=${expected_count}")
endif()

# Compare up to the shorter file's length
if(actual_count LESS expected_count)
    set(compare_count ${actual_count})
else()
    set(compare_count ${expected_count})
endif()

set(diff_count 0)
set(line_num 0)

foreach(idx RANGE 0 ${compare_count})
    if(idx EQUAL compare_count)
        break()
    endif()

    list(GET actual_lines ${idx} actual_line)
    list(GET expected_lines ${idx} expected_line)

    math(EXPR line_num "${idx} + 1")

    if(NOT actual_line STREQUAL expected_line)
        math(EXPR diff_count "${diff_count} + 1")
        if(diff_count LESS_EQUAL ${MAX_DIFFS})
            message(STATUS "DIFF line ${line_num}:")
            message(STATUS "  actual:   ${actual_line}")
            message(STATUS "  expected: ${expected_line}")
        endif()
    endif()
endforeach()

# Report extra lines
if(actual_count GREATER expected_count)
    math(EXPR extra "${actual_count} - ${expected_count}")
    math(EXPR diff_count "${diff_count} + ${extra}")
    message(STATUS "DIFF: actual has ${extra} extra lines (${expected_count}+1 .. ${actual_count})")
elseif(expected_count GREATER actual_count)
    math(EXPR extra "${expected_count} - ${actual_count}")
    math(EXPR diff_count "${diff_count} + ${extra}")
    message(STATUS "DIFF: expected has ${extra} extra lines (${actual_count}+1 .. ${expected_count})")
endif()

if(diff_count GREATER 0)
    if(diff_count GREATER ${MAX_DIFFS})
        math(EXPR hidden "${diff_count} - ${MAX_DIFFS}")
        message(STATUS "... and ${hidden} more differences (${diff_count} total)")
    endif()
    message(FATAL_ERROR "FAIL: ${diff_count} field differences found between actual and expected")
endif()

message(STATUS "PASS: all ${actual_count} lines match")
