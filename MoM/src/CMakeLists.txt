
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
    CONQUEST.c
    CREDITS.c
    DIPLOMAC.c
    EVENTS.c
    Explore.c
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

# find_package(SDL2 REQUIRED)
# # get_target_property(imported_location SDL2::Core IMPORTED_LOCATION)
# # message(STATUS "IMPORTED_LOCATION: ${imported_location}")
# # get_target_property(imported_location SDL2::Main IMPORTED_LOCATION)
# # message(STATUS "IMPORTED_LOCATION: ${imported_location}")
# # get_target_property(interface_include_directories SDL2::Main INTERFACE_INCLUDE_DIRECTORIES)
# # message(STATUS "INTERFACE_INCLUDE_DIRECTORIES: ${interface_include_directories}")
# # get_target_property(interface_include_directories SDL2::Main INTERFACE_INCLUDE_DIRECTORIES)
# # message(STATUS "INTERFACE_INCLUDE_DIRECTORIES: ${interface_include_directories}")
# find_package(SDL2_mixer REQUIRED)
# # get_target_property(imported_location SDL2::Mixer IMPORTED_LOCATION)
# # message(STATUS "IMPORTED_LOCATION: ${imported_location}")
# # get_target_property(interface_include_directories SDL2::Mixer INTERFACE_INCLUDE_DIRECTORIES)
# # message(STATUS "INTERFACE_INCLUDE_DIRECTORIES: ${interface_include_directories}")
# # >>> [cmake] -- IMPORTED_LOCATION: C:/devellib/SDL2-2.32.2/lib/x64/SDL2.lib
# # >>> [cmake] -- IMPORTED_LOCATION: imported_location-NOTFOUND
# # >>> [cmake] -- INTERFACE_INCLUDE_DIRECTORIES: interface_include_directories-NOTFOUND
# # >>> [cmake] -- INTERFACE_INCLUDE_DIRECTORIES: interface_include_directories-NOTFOUND
# # >>> [cmake] -- IMPORTED_LOCATION: C:/devellib/SDL2_mixer-2.8.1/lib/x64/SDL2_mixer.lib
# # >>> [cmake] -- INTERFACE_INCLUDE_DIRECTORIES: C:/devellib/SDL2_mixer-2.8.1/include

# add_library(momlib STATIC ${MOMLIB_SOURCES} ${SDL2_SOURCES})
add_library(momlib STATIC ${MOMLIB_SOURCES})

# set_target_properties(momlib PROPERTIES LINKER_LANGUAGE C)
set_target_properties(momlib PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(momlib PUBLIC
    MOX
    STU
    Platform
)

target_include_directories(momlib PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

ReMoM_Apply_Common_Compile_Definitions(momlib)

ReMoM_Apply_Common_Compile_Options(momlib)
