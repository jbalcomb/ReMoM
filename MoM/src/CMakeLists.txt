
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
    SDL2::Main
)

target_include_directories(momlib PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)


target_compile_definitions(momlib PRIVATE
    _CRT_SECURE_NO_WARNINGS=TRUE
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



add_executable(sdl2_ReMoM ReMoM.c)

# set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
set_target_properties(sdl2_ReMoM PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(sdl2_ReMoM PUBLIC
    momlib
    MOX
    STU
    SDL2::Main
    SDL2::Mixer
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
    _CRT_SECURE_NO_WARNINGS=TRUE
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



# Deprecated since version 3.24: Use the CMAKE_C_COMPILER_ID variable instead.
# Starting with CMake 2.6, the CMAKE_C_COMPILER_ID variable should be used instead:
# if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
#   # GNU C compiler-specific logic.
# endif()
# # Targeting GNU, LCC, and QCC compilers for C:
# if(CMAKE_C_COMPILER_ID MATCHES "^(GNU|LCC|QCC)$")
#   # ...
# endif()
# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID
if(CMAKE_COMPILER_IS_CLANG)
    target_compile_options(momlib PRIVATE
        "-Wall"
        "-Wno-deprecated-declarations"
        "-Wno-int-to-pointer-cast"
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
        "-Wno-deprecated-declarations"
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
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(momlib PRIVATE
        "/W0"
        "/wd4068"  # "unknown pragma"
        "/wd4365"
        "/wd4820"
    )
endif()
if(CMAKE_COMPILER_IS_CLANG)
    target_compile_options(sdl2_ReMoM PRIVATE
        "-Wall"
        "-Wno-deprecated-declarations"
        "-Wno-int-to-pointer-cast"
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
    target_compile_options(sdl2_ReMoM PRIVATE
        "-Wall"
        "-Wno-deprecated-declarations"
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
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sdl2_ReMoM PRIVATE
        "/W0"
        "/wd4068"  # "unknown pragma"
        "/wd4365"
        "/wd4820"
    )
endif()



# if(WIN32)
#     add_custom_command(
#         TARGET "sdl2_ReMoM" POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:sdl2_ReMoM>"
#         "$<TARGET_RUNTIME_DLLS:sdl2_ReMoM>" USES_TERMINAL COMMAND_EXPAND_LISTS
#     )
# endif()

# # [build]   TARGET 'SDL2::Main' is IMPORTED and does not build here.
# if(WIN32)
#     add_custom_command(
#         TARGET "SDL2::Main" POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:SDL2::Main>"
#         "$<TARGET_RUNTIME_DLLS:SDL2::Main>" USES_TERMINAL COMMAND_EXPAND_LISTS
#     )
# endif()




# ¿ add_subdirectory(TootLib) ?
