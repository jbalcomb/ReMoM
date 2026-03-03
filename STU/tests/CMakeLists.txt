
include(GoogleTest)

add_executable(test_STU test_STU.cpp)

target_link_libraries(test_STU
	GTest::gtest
	GTest::gtest_main
	STU
)

gtest_discover_tests(test_STU
	PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
