
add_subdirectory(unit)

include(GoogleTest)

add_executable(hello_test hello_test.cpp)

set_target_properties(hello_test PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(hello_test GTest::gtest_main)

gtest_discover_tests(hello_test)
