
add_executable(ReMoMber ReMoM.c)

# set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
set_target_properties(ReMoMber PROPERTIES POSITION_INDEPENDENT_CODE ON)

ReMoM_Apply_Common_Compile_Definitions(ReMoMber)

if(NOT DISABLE_AUDIO)
    target_compile_definitions(ReMoMber PRIVATE SOUND_ENABLED)
else()
    target_compile_definitions(ReMoMber PRIVATE SOUND_DISABLED)
endif()

target_link_libraries(ReMoMber PUBLIC
    momlib
    MOX
    STU
    SDL2::Main
)

target_include_directories(ReMoMber PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

ReMoM_Apply_Common_Compile_Options(ReMoMber)

ReMoM_Add_Target_Support_Files(ReMoMber
    COPY_SDL2
    COPY_SDL2_MIXER
    COPY_LBX
)
