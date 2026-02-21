# SEEALSO MoX/test/CMakeLists.txt ...notes, experimentation, etc.

include(GoogleTest)

add_executable(MOM_tests
    MOM_tests.cpp
    test_NEWG_ClearLandmasses__WIP.cpp
    test_NEWG_TileIsleExtend__WIP.cpp
    test_TILE_SetLandMass__WIP.cpp
    test_City_Maximum_Size_NewGame.cpp
    test_Init_New_Game.cpp
    test_Set_Upper_Lair_Guardian_Count.cpp
    test_Square_Food2_NewGame.cpp
    test_STR_ListSeparator.cpp
    test_Generate_Towers.cpp
    test_Generate_Nodes.cpp
    test_Tile_Checks.cpp
    test_Square_Terrain_Checks.cpp
    test_Terrain_Specials.cpp
    test_Utility_Functions.cpp
    test_Build_Landmass_And_Nodes.cpp
    test_Create_Unit_NewGame.cpp
    test_NEWG_SetLands.cpp
    test_NEWG_Node_Functions.cpp
)

target_include_directories(MOM_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(MOM_tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    SDL2::Core
    SDL2::Mixer
    momlib
    MOX
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
        COMMAND ${CMAKE_COMMAND} -E copy 
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

gtest_discover_tests(MOM_tests)