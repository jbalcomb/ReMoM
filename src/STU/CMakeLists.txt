
add_library(STU STATIC
    STU_CHK.c
    STU_DBG.c
    STU_DRAW.c
    STU_EMS.c
    STU_INT.c
    STU_JSON.c
    STU_SND.c
    STU_TST.c
    STU_UTIL.c
    STU_WAV.c
)

set_target_properties(STU PROPERTIES LINKER_LANGUAGE C)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
    target_compile_options(STU PRIVATE
        "-Wall"
        "-Wno-unknown-pragmas"
        "-Wno-unused-but-set-variable"
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
