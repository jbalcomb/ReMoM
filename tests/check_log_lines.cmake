# check_log_lines.cmake -- check a DeMoM log for lines a scenario must (or must not) have produced.
#
#   cmake -DLOG=<file> "-DREQUIRE=<text>|<text>|..." "-DFORBID=<text>|..." -P check_log_lines.cmake
#
# Each REQUIRE text must appear in LOG and no FORBID text may.  Plain substring match, '|'-separated (a ';' list would be
# split into separate arguments on the add_test() command line).  For scenarios whose pass condition is "it got there and
# did not crash": the run step's exit code covers the crash, this step proves the scenario actually reached each point.

if(NOT LOG)
    message(FATAL_ERROR "check_log_lines.cmake: LOG not set")
endif()
if(NOT EXISTS "${LOG}")
    message(FATAL_ERROR "check_log_lines.cmake: log not found: ${LOG}")
endif()

file(READ "${LOG}" _log)
set(_failed 0)

string(REPLACE "|" ";" _require "${REQUIRE}")
foreach(_text IN LISTS _require)
    string(FIND "${_log}" "${_text}" _at)
    if(_at EQUAL -1)
        message(SEND_ERROR "check_log_lines.cmake: missing from ${LOG}: ${_text}")
        set(_failed 1)
    endif()
endforeach()

string(REPLACE "|" ";" _forbid "${FORBID}")
foreach(_text IN LISTS _forbid)
    string(FIND "${_log}" "${_text}" _at)
    if(NOT _at EQUAL -1)
        message(SEND_ERROR "check_log_lines.cmake: must not be in ${LOG}: ${_text}")
        set(_failed 1)
    endif()
endforeach()

if(_failed)
    message(FATAL_ERROR "check_log_lines.cmake: ${LOG} failed its checks")
endif()
message(STATUS "check_log_lines.cmake: ${LOG} OK")
