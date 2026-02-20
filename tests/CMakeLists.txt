
include(GoogleTest)

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test GTest::gtest_main)
gtest_discover_tests(hello_test)
# gtest_discover_tests(hello_test     DISCOVERY_MODE PRE_TEST)
