
add_library(MOX STATIC
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
    SOUND.c
    TEXTBOX.c
    Util.c
    Video.c
    Video2.c
)

set_target_properties(MOX PROPERTIES LINKER_LANGUAGE C)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
    target_compile_options(MOX PRIVATE
        "-Wall"
        "-Wno-char-subscripts"
        "-Wno-unknown-pragmas"
        "-Wno-unused-but-set-variable"
        "-Wno-unused-function"
        "-Wno-unused-label"
        "-Wno-unused-variable"
        "-Wno-writable-strings"
    )
endif()
if(MSVC)
    target_compile_options(momlib PRIVATE
        "/W0"
        "/wd4068"   # "unknown pragma"
        "/wd4365"
        "/wd4820"
    )
endif()
