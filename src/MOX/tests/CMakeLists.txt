
include(GoogleTest)

set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT) # Set default to echo to STDOUT

add_executable(MOX_tests MOX_tests.cpp)

# target_include_directories(TootLib_tests PRIVATE "${PROJECT_SOURCE_DIR}")
# target_include_directories(TootLib_tests PRIVATE ${PROJECT_SOURCE_DIR}/TootLib)
# target_include_directories(TootLib_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MOX_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/MOX
    ${GTEST_INCLUDE_DIRS}
)
get_target_property(include_dirs MOX_tests INCLUDE_DIRECTORIES)
message(STATUS "INCLUDE_DIRECTORIES: ${include_dirs}")

target_link_libraries(MOX_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    momlib
    MOX
)
get_target_property(include_dirs MOX_tests INCLUDE_DIRECTORIES)
message(STATUS "INCLUDE_DIRECTORIES: ${include_dirs}")

# gtest_discover_tests(MOX_tests)
add_test(NAME MOX_tests COMMAND MOX_tests)
# add_custom_command(TARGET MOX_test POST_BUILD COMMAND ctest --output-on-failure)

# add_custom_command(TARGET exe POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:exe> $<TARGET_RUNTIME_DLLS:exe>
#   COMMAND_EXPAND_LISTS
# )
