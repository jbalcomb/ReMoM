
include(GoogleTest)

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test GTest::gtest_main)
gtest_discover_tests(hello_test
	PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
