# stage_test_runtime.cmake
# Ensures required runtime DLLs are present in the test binary directory
# before test execution.

if(NOT DEFINED TEST_BIN_DIR OR TEST_BIN_DIR STREQUAL "")
    message(FATAL_ERROR "TEST_BIN_DIR is required")
endif()

file(MAKE_DIRECTORY "${TEST_BIN_DIR}")

set(_required_dlls)

if(DEFINED SDL2_DLL_FILE_PATH AND NOT SDL2_DLL_FILE_PATH STREQUAL "")
    if(EXISTS "${SDL2_DLL_FILE_PATH}")
        list(APPEND _required_dlls "${SDL2_DLL_FILE_PATH}")
    else()
        message(FATAL_ERROR "Required SDL2 runtime not found: ${SDL2_DLL_FILE_PATH}")
    endif()
else()
    message(FATAL_ERROR "SDL2_DLL_FILE_PATH is not set")
endif()

set(_sdl2d_runtime_path "${SDL2D_DLL_FILE_PATH}")
if((NOT _sdl2d_runtime_path) OR (_sdl2d_runtime_path STREQUAL ""))
    get_filename_component(_sdl2_runtime_dir "${SDL2_DLL_FILE_PATH}" DIRECTORY)
    set(_sdl2d_candidate "${_sdl2_runtime_dir}/SDL2d.dll")
    if(EXISTS "${_sdl2d_candidate}")
        set(_sdl2d_runtime_path "${_sdl2d_candidate}")
    endif()
endif()

if(DEFINED _sdl2d_runtime_path AND NOT _sdl2d_runtime_path STREQUAL "")
    if(EXISTS "${_sdl2d_runtime_path}")
        list(APPEND _required_dlls "${_sdl2d_runtime_path}")
    else()
        message(FATAL_ERROR "Required SDL2 debug runtime not found: ${_sdl2d_runtime_path}")
    endif()
else()
    message(FATAL_ERROR "SDL2D_DLL_FILE_PATH is not set")
endif()

if(DEFINED GMOCK_DLL_FILE_PATH AND NOT GMOCK_DLL_FILE_PATH STREQUAL "")
    if(EXISTS "${GMOCK_DLL_FILE_PATH}")
        list(APPEND _required_dlls "${GMOCK_DLL_FILE_PATH}")
    else()
        message(FATAL_ERROR "Required gmock runtime not found: ${GMOCK_DLL_FILE_PATH}")
    endif()
else()
    message(FATAL_ERROR "GMOCK_DLL_FILE_PATH is not set")
endif()

if(DEFINED DISABLE_AUDIO AND DISABLE_AUDIO)
    # No SDL2_mixer runtime needed.
else()
    if(DEFINED SDL2_MIXER_DLL_FILE_PATH AND NOT SDL2_MIXER_DLL_FILE_PATH STREQUAL "")
        if(EXISTS "${SDL2_MIXER_DLL_FILE_PATH}")
            list(APPEND _required_dlls "${SDL2_MIXER_DLL_FILE_PATH}")
        else()
            message(FATAL_ERROR "Required SDL2_mixer runtime not found: ${SDL2_MIXER_DLL_FILE_PATH}")
        endif()
    endif()
endif()

foreach(_dll IN LISTS _required_dlls)
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${_dll}" "${TEST_BIN_DIR}"
        RESULT_VARIABLE _copy_rv
    )

    if(NOT _copy_rv EQUAL 0)
        message(FATAL_ERROR "Failed to copy runtime DLL '${_dll}' to '${TEST_BIN_DIR}'")
    endif()
endforeach()

message(STATUS "Staged runtime DLLs to: ${TEST_BIN_DIR}")
