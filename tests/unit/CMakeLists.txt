
include(GoogleTest)

add_executable(testLearning testLearning.c)

target_link_libraries(hello_test GTest::gtest_main)

gtest_discover_tests(hello_test)
