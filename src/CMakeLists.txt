
# =========================================================================
#  ReMoMber — windowed Master of Magic.
# =========================================================================

add_executable(ReMoMber ReMoM.c ReMoM_Init.c HeMoM_Player.c)

set_target_properties(ReMoMber PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(USE_WIN32)
    target_compile_definitions(ReMoMber PUBLIC _STU_WIN)
elseif(USE_SDL3)
    target_compile_definitions(ReMoMber PUBLIC USE_SDL3)
endif()

if(USE_WIN32)
    set(_sdl_main_lib "")
elseif(USE_SDL3)
    set(_sdl_main_lib SDL3::SDL3)
else()
    set(_sdl_main_lib SDL2::SDL2)
endif()

target_link_libraries(ReMoMber PUBLIC
    momlib
    MOX
    STU
    Platform
    ${_sdl_main_lib}
)

target_include_directories(ReMoMber PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

add_custom_command(TARGET ReMoMber POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:ReMoMber>"
    COMMENT "Copying game assets to output directory"
    VERBATIM
)

set_target_properties(ReMoMber PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:ReMoMber>"
)



# =========================================================================
#  HeMoM — Headless Master of Magic.
#  Always built, always headless.  Links Platform_Headless directly so it
#  is not affected by which windowed backend the preset chose for ReMoMber.
# =========================================================================

add_executable(HeMoM HeMoM.c ReMoM_Init.c HeMoM_Player.c HeMoM_SaveDump.c)

set_target_properties(HeMoM PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(HeMoM PUBLIC
    momlib
    MOX
    STU
    Platform_Headless
)

target_include_directories(HeMoM PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

add_custom_command(TARGET HeMoM POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:HeMoM>"
    COMMENT "Copying game assets to HeMoM output directory"
    VERBATIM
)

set_target_properties(HeMoM PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:HeMoM>"
)
