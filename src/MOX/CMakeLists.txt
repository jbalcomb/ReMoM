
add_library(MOX STATIC
    Allocate.C
    CFG.C
    DBG.C
    DOS.C
    EMM.C
    EXIT.C
    Fields.C
    file_ani.C
    Fonts.C
    GENDRAW.C
    Graphics.C
    FLIC_Draw.C
    Input.C
    IO_Buffer.C
    Keyboard.C
    LBX_Load.C
    LOADSAVE.C
    Mouse.C
    MOX_DAT.C
    MOX_SET.C
    MOX_SND.C
    MOX_T4.C
    paragrph.C
    random.C
    SOUND.C
    Util.C
    Video.C
    Video2.C
)

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
