
include(GoogleTest)

# Viewport: pure C, compiled straight in -- no platform backend or SDL needed.
add_executable(test_Platform_Viewport
	test_Viewport.cpp
	../viewport/PFL_Viewport.c
)

target_include_directories(test_Platform_Viewport PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(test_Platform_Viewport
	GTest::gtest
	GTest::gtest_main
)

gtest_discover_tests(test_Platform_Viewport
	PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
