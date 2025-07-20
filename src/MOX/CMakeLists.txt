
set(MOX_SOURCES
    AIL.c
    Allocate.c
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
    Input.c
    IO_Buffer.c
    Keyboard.c
    LBX_Load.c
    LOADSAVE.c
    Mouse.c
    MOM_Data.c
    MOX_DAT.c
    MOX_SET.c
    MOX_SND.c
    MOX_T4.c
    paragrph.c
    random.c
    sdl2_Audio.c
    SOUND.c
    TEXTBOX.c
    Util.c
    Video.c
    Video2.c
)

add_library(MOX STATIC ${MOX_SOURCES})

# set_target_properties(MOX PROPERTIES LINKER_LANGUAGE C)

target_include_directories(MOX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

target_link_libraries(MOX
    SDL2::Core
    SDL2::Mixer
)
