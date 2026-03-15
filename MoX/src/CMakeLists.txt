
set(LIBMOX_SOURCES
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

add_library(MOX STATIC ${LIBMOX_SOURCES})

target_include_directories(MOX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MOX
    Platform
)
