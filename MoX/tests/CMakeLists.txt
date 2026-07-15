
include(GoogleTest)

# Tests run headless (no display).
add_executable(MOX_tests
    MOX_tests.cpp
    test_Util.cpp
    test_Allocate.cpp
    test_Fonts.cpp
    test_Graphics.cpp
    test_capture.cpp
)

target_include_directories(MOX_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(MOX_tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    MOX
    STU
    Platform_Headless
)

gtest_discover_tests(MOX_tests)

# Isolated unit tests for the Allocate_Pool module (PLAN-Static-Pool-Allocator
# Phase 1): compiles the module source directly and stubs Allocation_Error(),
# so it builds and runs with no MOX library dependency.
add_executable(test_Allocate_Pool
    test_Allocate_Pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Allocate_Pool.c
)

target_include_directories(test_Allocate_Pool PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(test_Allocate_Pool PRIVATE
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(test_Allocate_Pool)
