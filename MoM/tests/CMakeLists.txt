
include(GoogleTest)

add_executable(MOM_tests
    MOM_tests.cpp
    test_Reload_Melee_Sound.cpp
    test_Set_Gibs.cpp
    test_Clear_Gibs.cpp
    test_Ranged_Mouse_Image.cpp
    test_NEWG_ClearLandmasses__WIP.cpp
    test_NEWG_SetBaseLands__WIP.cpp
    test_NEWG_TileIsleExtend__WIP.cpp
    test_TILE_SetLandMass__WIP.cpp
    test_River_Path.cpp
    test_Desert_Autotile.cpp
    test_Animate_Oceans.cpp
    test_City_Maximum_Size_NewGame.cpp
    test_Init_New_Game.cpp
    test_Set_Upper_Lair_Guardian_Count.cpp
    test_Square_Food2_NewGame.cpp
    test_STR_ListSeparator.cpp
    test_Create_Lair.cpp
    test_Generate_Lairs.cpp
 )

target_include_directories(MOM_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(MOM_tests PUBLIC
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    momlib
    MOX
    STU
    Platform
)

gtest_discover_tests(MOM_tests)
