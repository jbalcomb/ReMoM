## stage_test_data.cmake
# Creates a `stage_test_data` custom target that copies repository `.lbx` files
# into the build tree under `${CMAKE_BINARY_DIR}/test_data` so test executables
# can run with required game data available in a predictable location.

file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx")

if(LBX_FILE_LIST)
    set(TEST_DATA_DIR "${CMAKE_BINARY_DIR}/test_data")
    add_custom_target(stage_test_data
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_DATA_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Staging LBX files to: ${TEST_DATA_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LBX_FILE_LIST}
            ${TEST_DATA_DIR}
        COMMAND_EXPAND_LISTS
        COMMENT "Copying .lbx data files into build/test_data"
    )
else()
    message(STATUS "No .lbx files found in source root; stage_test_data will be a no-op.")
endif()
