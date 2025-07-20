
set(WIN32_SOURCES
    win_CLK.c
    win_EMS.c
    win_Fonts.c
    win_Input.c
    win_KD.c
    win_MD.c
    win_MoM.cpp
    win_PFL.cpp
    win_Video.c
)

set(SDL2_SOURCES
    sdl2_EMM.c
    sdl2_Input.c
    sdl2_KD.c
    sdl2_MD.c
    sdl2_MOM.c
    sdl2_PFL.c
    sdl2_Timer.c
)

set(MOMLIB_SOURCES
    AdvsrScr.c
    AIBUILD.c
    AIDATA.c
    AIDUDES.c
    AIMOVE.c
    AISPELL.c
    ALLOC.c
    ArmyList.c
    CITYCALC.c
    CityList.c
    CITYSCAP.c
    CityScr.c
    City_ovr55.c
    Combat.c
    CREDITS.c
    DIPLOMAC.c
    EVENTS.c
    Explore.c
    Help.c
    HIRE.c
    Init.c
    INITGAME.c
    INTRO.c
    ItemMake.c
    Items.c
    ItemScrn.c
    ItemView.c
    Lair.c
    LOADER.c
    LoadScr.c
    LVLMAKE.c
    MagicScr.c
    MainMenu.c
    MainScr.c
    MainScr_Maps.c
    MAPGEN.c
    MGC_DATA.c
    MOM_SCR.c
    MOM_DBG.c
    MOM_PFL.c
    MovePath.c
    NewGame.c
    NEXTTURN.c
    Outpost.c
    OverSpel.c
    ProdScr.c
    RACETYPE.c
    REPORT.c
    Roads.c
    SBookScr.c
    SCastScr.c
    SCORE.c
    Settings.c
    SETTLE.c
    special.c
    Spellbook.c
    Spells128.c
    Spells129.c
    Spells130.c
    Spells131.c
    Spells132.c
    Spells133.c
    Spells137.c
    SPLMASTR.c
    Surveyor.c
    Terrain.c
    UnitList.c
    UnitMove.c
    UnitStat.c
    UNITSTK.c
    UNITTYPE.c
    UnitView.c
    WIZVIEW.c
    WZD_o059.c
    WZD_o143.c
    WZD_o146.c
)



find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)



add_library(momlib STATIC ${MOMLIB_SOURCES})

set_target_properties(momlib PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(momlib PUBLIC
    MOX
    STU
    SDL2::Main
)

target_include_directories(momlib PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

target_compile_definitions(momlib PRIVATE
    $<$<CONFIG:Debug>:STU_DEBUG=TRUE>
)
target_compile_definitions(momlib PRIVATE
    $<$<CONFIG:Debug>:STU_TRACE=TRUE>
)
target_compile_definitions(momlib PRIVATE
    $<$<CONFIG:Debug>:_STU_SDL2=TRUE>
)



add_executable(sdl2_ReMoM ReMoM.c ${SDL2_SOURCES})

set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(sdl2_ReMoM PUBLIC
    momlib
    MOX
    STU
    SDL2::Main
)

target_include_directories(sdl2_ReMoM PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

target_compile_definitions(sdl2_ReMoM PRIVATE
    $<$<CONFIG:Debug>:STU_DEBUG=TRUE>
)
target_compile_definitions(sdl2_ReMoM PRIVATE
    $<$<CONFIG:Debug>:STU_TRACE=TRUE>
)
target_compile_definitions(sdl2_ReMoM PRIVATE
    $<$<CONFIG:Debug>:_STU_SDL2=TRUE>
)



if(CMAKE_COMPILER_IS_CLANG)
    target_compile_options(momlib PRIVATE
        "-Wall"
        "-Wno-missing-braces"
        "-Wno-tautological-compare"
        "-Wno-unknown-pragmas"
        "-Wno-unused-but-set-variable"
        "-Wno-unused-label"
        "-Wno-unused-variable"
        "-Wno-unused-function"
        "-Wno-writable-strings"
    )
endif()
if(CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(momlib PRIVATE
        "-Wall"
        "-Wno-int-to-pointer-cast"
        "-Wno-missing-braces"
        "-Wno-tautological-compare"
        "-Wno-unknown-pragmas"
        "-Wno-unused-but-set-variable"
        "-Wno-unused-label"
        "-Wno-unused-variable"
        "-Wno-unused-function"
    )
endif()
if(MSVC)
    target_compile_options(momlib PRIVATE
        "/W0"
        "/wd4068"  # "unknown pragma"
        "/wd4365"
        "/wd4820"
    )
endif()



add_subdirectory(MOX)

add_subdirectory(STU)
