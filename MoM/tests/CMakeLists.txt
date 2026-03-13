
# SEEALSO MoX/test/CMakeLists.txt ...notes, experimentation, etc.

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
    TEST_LIST MOM_TEST_DISCOVERED
    WORKING_DIRECTORY $<TARGET_FILE_DIR:MOM_tests>
    PROPERTIES
        ENVIRONMENT_MODIFICATION PATH=path_list_prepend:${REMOM_TEST_PATH_ENV}
)

add_test(
    NAME MOM_tests_stage_runtime
    COMMAND ${CMAKE_COMMAND}
        -DTEST_BIN_DIR=$<TARGET_FILE_DIR:MOM_tests>
        -DSDL2_DLL_FILE_PATH=${SDL2_DLL_FILE_PATH}
        -DSDL2D_DLL_FILE_PATH=${SDL2D_DLL_FILE_PATH}
        -DGMOCK_DLL_FILE_PATH=${GMOCK_DLL_FILE_PATH}
        -DSDL2_MIXER_DLL_FILE_PATH=${SDL2_MIXER_DLL_FILE_PATH}
        -DDISABLE_AUDIO=${DISABLE_AUDIO}
        -P ${CMAKE_SOURCE_DIR}/cmake/stage_test_runtime.cmake
)

set_tests_properties(MOM_tests_stage_runtime PROPERTIES
    FIXTURES_SETUP MOM_tests_runtime
)

if(MOM_TEST_DISCOVERED)
    set_tests_properties(${MOM_TEST_DISCOVERED} PROPERTIES
        FIXTURES_REQUIRED MOM_tests_runtime
    )
endif()
