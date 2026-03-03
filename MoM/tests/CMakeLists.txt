
# SEEALSO MoX/test/CMakeLists.txt ...notes, experimentation, etc.

include(GoogleTest)

add_executable(MOM_tests
    MOM_tests.cpp
    test_NEWG_ClearLandmasses__WIP.cpp
    test_NEWG_SetBaseLands__WIP.cpp
    test_NEWG_TileIsleExtend__WIP.cpp
    test_TILE_SetLandMass__WIP.cpp
    test_River_Path.cpp
    test_City_Maximum_Size_NewGame.cpp
    test_Init_New_Game.cpp
    test_Set_Upper_Lair_Guardian_Count.cpp
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
    momlib
    MOX
    STU
)

ReMoM_Add_Target_Support_Files(MOM_tests
    COPY_SDL2
    COPY_SDL2_MIXER
    COPY_LBX
    COPY_SDL2D
    COPY_GMOCK
)

gtest_discover_tests(MOM_tests
    PROPERTIES
        ENVIRONMENT "${REMOM_TEST_PATH_ENV}"
)
