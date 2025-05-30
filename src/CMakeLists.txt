
set(WIN32_SOURCES
    win_CLK.C
    win_DIR.C
    win_EMS.C
    win_Fonts.C
    win_Input.C
    win_KD.C
    win_MD.C
    win_MoM.cpp
    win_PFL.cpp
    win_Video.C
)

set(SDL2_SOURCES
    sdl2_DIR.C
    sdl2_EMM.C
    sdl2_Input.C
    sdl2_KD.C
    sdl2_MD.C
    sdl2_MOM.C
    sdl2_PFL.C
    sdl2_Timer.C
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
    CREDITS.C
    DIPLOMAC.C
    EVENTS.C
    Explore.C
    Help.C
    HIRE.C
    Init.C
    INITGAME.C
    INTRO.C
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
    MAPGEN.C
    MGC_Data.C
    MOM.C
    MOM_SCR.C
    MOM_DBG.C
    MOM_PFL.C
    MovePath.C
    NEWGAME.C
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
    Spells132.C
    SPLMASTR.C
    Surveyor.C
    Terrain.C
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
add_library(momlib STATIC ${MOMLIB_SOURCES})
set_target_properties(momlib PROPERTIES LINKER_LANGUAGE C)
target_include_directories(momlib PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(SDL2_DIR "$ENV{SDL2DIR}")
find_package(SDL2 REQUIRED)

message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_LIBDIR: ${SDL2_LIBDIR}")
message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARY}")
message(STATUS "SDL2MAIN_LIBRARY: ${SDL2MAIN_LIBRARY}")
#   set(_sdl2_implib "${SDL2_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
message(STATUS "CMAKE_STATIC_LIBRARY_PREFIX: ${CMAKE_STATIC_LIBRARY_PREFIX}")
message(STATUS "CMAKE_SHARED_LIBRARY_SUFFIX: ${CMAKE_SHARED_LIBRARY_SUFFIX}")
message(STATUS "CMAKE_STATIC_LIBRARY_SUFFIX: ${CMAKE_STATIC_LIBRARY_SUFFIX}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")



add_compile_definitions(_STU_SDL2)
add_compile_definitions(STU_DEBUG)



add_executable(sdl2_ReMoM ReMoM.C ${SDL2_SOURCES})
set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
# target_link_libraries(sdl2_ReMoM PUBLIC momlib MOX STU SDL2 SDL2main)
# target_link_libraries(sdl2_ReMoM PUBLIC momlib MOX STU C:/devellib/SDL2-2.32.2/lib/x64/SDL2.lib C:/devellib/SDL2-2.32.2/lib/x64/SDL2main.lib)
# target_link_libraries(sdl2_ReMoM PUBLIC momlib MOX STU ${SDL2_LIBRARIES})
# target_link_libraries(sdl2_ReMoM PUBLIC momlib MOX STU SDL2 SDL2main)
# target_link_libraries(sdl2_ReMoM PUBLIC momlib MOX STU ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
target_link_libraries(sdl2_ReMoM PUBLIC momlib MOX STU C:/devellib/SDL2-2.32.2/lib/x64/SDL2.lib C:/devellib/SDL2-2.32.2/lib/x64/SDL2main.lib)
target_include_directories(sdl2_ReMoM PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL2_INCLUDE_DIR}
)
get_target_property(sdl2_ReMoM_include_dirs sdl2_ReMoM INCLUDE_DIRECTORIES)
message(STATUS "Include directories for sdl2_ReMoM: ${sdl2_ReMoM_include_dirs}")

add_subdirectory(MOX)
set_target_properties(MOX PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(STU)
set_target_properties(STU PROPERTIES LINKER_LANGUAGE C)



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
# target_include_directories(ReMoM PUBLIC ${PROJECT_SOURCE_DIR} ${SDL_INCLUDE_DIR})
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
# include_directories ( SYSTEM ${SDLIMAGE_INCLUDE_DIR} )
include_directories(SYSTEM ${PROJECT_SOURCE_DIR} )
# get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")

set(REMOM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR})
message(DEBUG "REMOM_INCLUDE_DIRS: ${REMOM_INCLUDE_DIRS}")
list(APPEND REMOM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)
message(DEBUG "REMOM_INCLUDE_DIRS: ${REMOM_INCLUDE_DIRS}")

get_target_property(include_dirs momlib INCLUDE_DIRECTORIES)
message(STATUS "Include directories for momlib: ${include_dirs}")
# get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")

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
