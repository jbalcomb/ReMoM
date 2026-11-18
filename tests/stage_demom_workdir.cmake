# stage_demom_workdir.cmake -- give one DeMoM test group a clean working directory.
#
#   cmake -DWORK_DIR=<dir> "-DFILES=<file>|<file>|..." -P stage_demom_workdir.cmake
#
# Deletes WORK_DIR, recreates it, and copies FILES into it ('|'-separated, because a ';' list would be
# split into separate arguments on the add_test() command line).  DeMoM writes its saves, dumps, logs
# and MAGIC.SET to its working directory, so each group gets its own: groups can run in parallel, and
# other DeMoM runs in the bin dir (other sessions, manual runs) can no longer overwrite a test's files.
# The game data (LBX, CONFIG.MOM) is not copied -- the tests point REMOM_DATA_DIR at the bin dir.

if(NOT WORK_DIR)
    message(FATAL_ERROR "stage_demom_workdir.cmake: WORK_DIR not set")
endif()

file(REMOVE_RECURSE "${WORK_DIR}")
file(MAKE_DIRECTORY "${WORK_DIR}")

string(REPLACE "|" ";" _files "${FILES}")
foreach(_file IN LISTS _files)
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "stage_demom_workdir.cmake: missing file to stage: ${_file}")
    endif()
    file(COPY "${_file}" DESTINATION "${WORK_DIR}")
endforeach()
