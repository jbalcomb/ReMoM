

set(WINDOWS_SOURCES
    win_MoM.cpp

    win_CLK.C
    win_DIR.C
    win_EMS.C
    win_Fonts.C
    win_Input.C
    win_KD.C
    win_MD.C
    win_PFL.cpp
    win_Video.C
)

add_library(gamelib STATIC
    AdvsrScr.C
    AIBUILD.C
    AIDATA.C
    AIDUDES.C
    AIMOVE.C
    AISPELL.C
    ALLOC.C
    ArmyList.C
    CITYCALC.C
    CityList.C
    CITYSCAP.C
    CityScr.C
    City_ovr55.C
    Combat.C
    DIPLOMAC.C
    EVENTS.C
    Explore.C
    Help.C
    HIRE.C
    Init.C
    ItemMake.C
    Items.C
    ItemScrn.C
    ItemView.C
    Lair.C
    LOADER.C
    LoadScr.C
    LVLMAKE.C
    MagicScr.C
    MainMenu.C
    MainScr.C
    MainScr_Maps.C
    MOM.C
    MOM_SCR.C
    MOM_Data.C
    MOM_DBG.C
    MOM_PFL.C
    MovePath.C
    NewGame.C
    NEXTTURN.C
    Outpost.C
    OverSpel.C
    ProdScr.C
    RACETYPE.C
    REPORT.C
    Roads.C
    SBookScr.C
    SCastScr.C
    SCORE.C
    Settings.C
    SETTLE.C
    special.C
    Spellbook.C
    SPLMASTR.C
    Surveyor.C
    Terrain.C
    TEXTBOX.C
    UnitList.C
    UnitMove.C
    UnitStat.C
    UNITSTK.C
    UNITTYPE.C
    UnitView.C
    WIZVIEW.C
    WZD_o059.C
    WZD_o129.C
    WZD_o143.C

)

add_executable(game
    main.cpp
)
set_target_properties(game PROPERTIES WIN32_EXECUTABLE TRUE)

add_subdirectory(MOX)

add_subdirectory(STU)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR})

if ( WIN32 )
    target_link_libraries(game PUBLIC gamelib MOX STU winmm)
else ()
    target_link_libraries(game PUBLIC gamelib MOX STU)
endif ()

# target_compile_features(ReMoM PUBLIC cxx_std_23)
# target_compile_definitions(ReMoM PUBLIC -DNOMINMAX)
# target_compile_options(ReMoM PUBLIC /W4 /WX /Debug /Od /Z7)
target_compile_options(gamelib PUBLIC
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
