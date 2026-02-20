
# SEEALSO MoX/test/CMakeLists.txt ...notes, experimentation, etc.

include(GoogleTest)

add_executable(MOM_tests
    MOM_tests.cpp
    test_City_Maximum_Size_NewGame.cpp
    test_Square_Food2_NewGame.cpp
    test_STR_ListSeparator.cpp
 )

target_include_directories(MOM_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(MOM_tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    SDL2::Core
    SDL2::Mixer
    momlib
    MOX
    STU
)

if(WIN32)
    add_custom_command(
        TARGET MOM_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:MOM_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoM/tests: Copying SDL2.dll"
    )
endif()

if(WIN32)
    add_custom_command(
        TARGET MOM_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${SDL2_MIXER_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:MOM_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoM/tests: Copying SDL2_mixer.dll"
    )
endif()

file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx")
add_custom_command(
    TARGET MOM_tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LBX_FILE_LIST}
        $<TARGET_FILE_DIR:MOM_tests>
    COMMAND_EXPAND_LISTS
    COMMENT "./MoM/tests: Copying LBX files..."
)

if(WIN32)
    add_custom_command(
        TARGET MOM_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${SDL2D_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:MOM_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoM/tests: Copying SDL2d.dll"
    )
endif()

if(WIN32)
    add_custom_command(
        TARGET MOM_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            ${GMOCK_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:MOM_tests>
        COMMAND_EXPAND_LISTS
        COMMENT "./MoM/tests: Copying gmock.dll"
    )
endif()

gtest_discover_tests(MOM_tests)
# gtest_discover_tests(MOM_tests      DISCOVERY_MODE PRE_TEST)
