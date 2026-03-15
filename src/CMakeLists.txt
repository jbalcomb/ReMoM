
add_executable(ReMoMber ReMoM.c)

# set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
set_target_properties(ReMoMber PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(ReMoMber PUBLIC
    momlib
    MOX
    STU
    Platform
    SDL3::SDL3
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
