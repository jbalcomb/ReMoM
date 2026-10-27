include(GoogleTest)

add_executable(MOD_tests
    test_MOD_Hook.cpp
    test_MOD_Lua.cpp
)

target_link_libraries(MOD_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    MOD
    MOX
    STU
    Platform_Headless
)

gtest_discover_tests(MOD_tests
    PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)
