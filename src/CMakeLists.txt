

set(WIN32_SOURCES
    win32/win_MoM.cpp

    win32/win_CLK.C
    win32/win_DIR.C
    win32/win_EMS.C
    win32/win_Fonts.C
    win32/win_Input.C
    win32/win_KD.C
    win32/win_MD.C
    win32/win_PFL.cpp
    win32/win_Video.C
)

set(SDL2_SOURCES
    sdl2/sdl2_MoM.cpp

    sdl2/sdl2_CLK.C
    sdl2/sdl2_DIR.C
    sdl2/sdl2_EMS.C
    sdl2/sdl2_Fonts.C
    sdl2/sdl2_Input.C
    sdl2/sdl2_KD.C
    sdl2/sdl2_MD.C
    sdl2/sdl2_PFL.cpp
    sdl2/sdl2_Video.C
)

set(MOMLIB_SOURCES
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

# if(_STU_WIN32)
#     add_library(momlib STATIC ${MOMLIB_SOURCES} ${WIN32_SOURCES})
# endif()
# if(_STU_SDL2)
#     add_library(momlib STATIC ${MOMLIB_SOURCES} ${SDL2_SOURCES})
# endif()
add_library(momlib STATIC ${MOMLIB_SOURCES} ${SDL2_SOURCES})

add_executable(ReMoM ReMoM.C)

add_subdirectory(MOX)

add_subdirectory(STU)




# if ( WIN32 )
#     set_target_properties(ReMoM PROPERTIES WIN32_EXECUTABLE TRUE)
# else ()
#     
# endif ()

# if ( WIN32 )
#     target_link_libraries(ReMoM PUBLIC momlib MOX STU winmm)
#     target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR})
# else ()
#     target_link_libraries(ReMoM PUBLIC momlib MOX STU SDL2 SDL2main)
#     target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR} ${SDL_INCLUDE_DIR})
# endif ()
target_link_libraries(ReMoM PUBLIC momlib MOX STU SDL2 SDL2main)
# target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR} ${SDL_INCLUDE_DIR})
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
# include_directories ( SYSTEM ${SDLIMAGE_INCLUDE_DIR} )
include_directories(SYSTEM ${PROJECT_SOURCE_DIR} )
get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")

set(REMOM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR})
message(DEBUG "REMOM_INCLUDE_DIRS: ${REMOM_INCLUDE_DIRS}")
list(APPEND REMOM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)
message(DEBUG "REMOM_INCLUDE_DIRS: ${REMOM_INCLUDE_DIRS}")

target_include_directories(momlib PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL_INCLUDE_DIR}
)
target_include_directories(ReMoM PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL_INCLUDE_DIR}
)

get_target_property(include_dirs momlib INCLUDE_DIRECTORIES)
message(STATUS "Include directories for momlib: ${include_dirs}")
get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")

# target_compile_features(ReMoM PUBLIC cxx_std_23)
# target_compile_definitions(ReMoM PUBLIC -DNOMINMAX)
# target_compile_options(ReMoM PUBLIC /W4 /WX /Debug /Od /Z7)
# target_compile_options(ReMoM PUBLIC "-Wall")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
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
if(MSVC)
    target_compile_options(momlib PRIVATE
        "/W0"
        "/wd4068"  # "unknown pragma"
        "/wd4365"
        "/wd4820"
    )
endif()
