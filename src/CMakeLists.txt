
add_executable(ReMoMber ReMoM.c)

# set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
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


# Copy game assets (LBX files, CONFIG.MOM, etc.) into the exe output directory.
# The game opens these by bare filename from the working directory.
add_custom_command(TARGET ReMoMber POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:ReMoMber>"
    COMMENT "Copying game assets to output directory"
    VERBATIM
)

# Set VS debugger working directory to the exe output dir so the game
# finds its asset files (LBX, CONFIG.MOM, etc.) when launched from the IDE.
set_target_properties(ReMoMber PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:ReMoMber>"
)
