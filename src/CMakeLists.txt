
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
    MoM.C
    MoM_Data.C
    MoM_DBG.C
    MoM_PFL.C
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
    SOUND.C
    SPLMASTR.C
    Surveyor.C
    Terrain.C
    TEXTBOX.C
    UnitList.C
    UnitMove.C
    UnitStat.C
    UNITSTK.C
    UnitType.C
    UnitView.C
    WIZVIEW.C
    WZD_o059.C
    WZD_o129.C
    WZD_o143.C

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

add_executable(game
    win_MoM.cpp
)

add_subdirectory(MOX)

add_subdirectory(STU)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(game PUBLIC MOX STU)

# target_compile_features(ReMoM PUBLIC cxx_std_23)
# target_compile_definitions(ReMoM PUBLIC -DNOMINMAX)
# target_compile_options(ReMoM PUBLIC /W4 /WX /Debug /Od /Z7)
