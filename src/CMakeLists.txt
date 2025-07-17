
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

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)


# set(SDL2_DIR "$ENV{SDL2DIR}")
message(STATUS "ENV{SDL2DIR}: $ENV{SDL2DIR}")
message(STATUS "ENV{SDL2MIXERDIR}: $ENV{SDL2MIXERDIR}")
# set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_LIBDIR: ${SDL2_LIBDIR}")
message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARY}")
message(STATUS "SDL2MAIN_LIBRARY: ${SDL2MAIN_LIBRARY}")



message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")



# if(_STU_WIN32)
#     add_library(momlib STATIC ${MOMLIB_SOURCES} ${WIN32_SOURCES})
# endif()
# if(_STU_SDL2)
#     add_library(momlib STATIC ${MOMLIB_SOURCES} ${SDL2_SOURCES})
# endif()
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

add_subdirectory(MOX)

add_subdirectory(STU)



# if ( WIN32 )
#     set_target_properties(ReMoM PROPERTIES WIN32_EXECUTABLE TRUE)
# else ()
#     
# endif ()

# # if ( WIN32 )
# #     target_link_libraries(ReMoM PUBLIC momlib MOX STU winmm)
# #     target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR})
# # else ()
# #     target_link_libraries(ReMoM PUBLIC momlib MOX STU SDL2 SDL2main)
# #     target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR} ${SDL_INCLUDE_DIR})
# # endif ()
# # target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR} ${SDL_INCLUDE_DIR})
# message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
# # include_directories ( SYSTEM ${SDLIMAGE_INCLUDE_DIR} )
# include_directories(SYSTEM ${PROJECT_SOURCE_DIR} )
# # get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# # message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")
# 
# set(REMOM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR})
# message(DEBUG "REMOM_INCLUDE_DIRS: ${REMOM_INCLUDE_DIRS}")
# list(APPEND REMOM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)
# message(DEBUG "REMOM_INCLUDE_DIRS: ${REMOM_INCLUDE_DIRS}")

get_target_property(momlib_include_dirs momlib INCLUDE_DIRECTORIES)
message(STATUS "momlib INCLUDE_DIRECTORIES: ${momlib_include_dirs}")
get_target_property(sdl2_ReMoM_include_dirs sdl2_ReMoM INCLUDE_DIRECTORIES)
message(STATUS "sdl2_ReMoM INCLUDE_DIRECTORIES: ${sdl2_ReMoM_include_dirs}")
# get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")

# target_compile_features(ReMoM PUBLIC cxx_std_23)
# target_compile_definitions(ReMoM PUBLIC -DNOMINMAX)
# target_compile_options(ReMoM PUBLIC /W4 /WX /Debug /Od /Z7)
# target_compile_options(ReMoM PUBLIC "-Wall")
# target_compile_definitions(momlib PRIVATE STU_DEBUG=TRUE)
# target_compile_definitions(momlib PRIVATE STU_TRACE=TRUE)

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
