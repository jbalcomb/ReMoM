
add_library(STU STATIC
    STU_CHK.C
    STU_DBG.C
    STU_JSON.C
    STU_UTIL.C
)

target_compile_options(STU PUBLIC
    "-Wall"
    "-Wno-unknown-pragmas"
    "-Wno-unused-but-set-variable"
    "-Wno-unused-variable"
    "-Wno-writable-strings"
)
