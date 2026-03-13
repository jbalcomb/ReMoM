
# WIN32 and SDL2 platform sources have been moved to platform/ library.
# MoX is now platform-agnostic engine code only.

set(MOX_SOURCES
    AIL.c
    Allocate.c
    angle.c
    capture.c
    CFG.c
    DOS.c
    EMM.c
    EXIT.c
    Fields.c
    file_ani.c
    Fonts.c
    GENDRAW.c
    GFILE.c
    Graphics.c
    FLIC_Draw.c
    Help.c
    Input.c
    IO_Buffer.c
    Keyboard.c
    LBX_Load.c
    LOADSAVE.c
    Mouse.c
    MOM_DAT.c
    MOX_DAT.c
    MOX_SET.c
    MOX_SND.c
    MOX_T4.c
    paragrph.c
    random.c
    SOUND.c
    special.c
    TEXTBOX.c
    Util.c
    Video.c
    Video2.c
)

add_library(MOX STATIC ${MOX_SOURCES})

# set_target_properties(MOX PROPERTIES LINKER_LANGUAGE C)
set_target_properties(MOX PROPERTIES POSITION_INDEPENDENT_CODE ON)

ReMoM_Apply_Common_Compile_Definitions(MOX)
ReMoM_Apply_Common_Compile_Options(MOX)

target_include_directories(MOX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

target_link_libraries(MOX
    Platform
)

if(NOT DISABLE_AUDIO)
    target_link_libraries(MOX
        SDL2::Core
        SDL2::Mixer
    )
else()
    target_link_libraries(MOX
        SDL2::Core
    )
endif()
