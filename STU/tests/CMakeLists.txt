
include(GoogleTest)

add_executable(test_STU test_STU.cpp)

target_link_libraries(test_STU
	GTest::gtest
	GTest::gtest_main
	STU
)

gtest_discover_tests(test_STU)
# gtest_discover_tests(test_STU     DISCOVERY_MODE PRE_TEST)
