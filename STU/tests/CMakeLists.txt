
include(GoogleTest)

add_executable(test_STU
	test_STU_LOG.cpp
	test_STU_LOG_strip.cpp
	test_STU_UTIL.cpp
	test_STU_GRAF.cpp
	test_STU_HASH.cpp
)

set_source_files_properties(test_STU_LOG_strip.cpp PROPERTIES
	COMPILE_DEFINITIONS "STU_LOG_MIN_SEVERITY=LOG_SEV_INFO"
)

target_link_libraries(test_STU
	GTest::gtest
	GTest::gtest_main
	STU
)

gtest_discover_tests(test_STU
	PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
