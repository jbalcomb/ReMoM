
include(GoogleTest)

add_executable(MOX_tests
    MOX_tests.cpp
    test_Util.cpp
    test_Allocate.cpp
    test_Fonts.cpp
    test_Graphics.cpp
    test_capture.cpp
    test_EMS.cpp
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
    Platform
)

gtest_discover_tests(MOX_tests)
