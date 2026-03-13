function(ReMoM_Apply_Common_Compile_Options target_name)
    if(CMAKE_COMPILER_IS_CLANG)
        target_compile_options(${target_name} PRIVATE
            "-Wall"
            "-Wno-comment"
            "-Wno-deprecated-declarations"
            "-Wno-int-to-pointer-cast"
            "-Wno-missing-braces"
            "-Wno-sequence-point"
            "-Wno-tautological-compare"
            "-Wno-unknown-pragmas"
            "-Wno-unused-but-set-variable"
            "-Wno-unused-label"
            "-Wno-unused-value"
            "-Wno-unused-variable"
            "-Wno-unused-function"
            "-Wno-writable-strings"
        )
    endif()

    if(CMAKE_COMPILER_IS_GNUCC)
        target_compile_options(${target_name} PRIVATE
            "-Wall"
            "-Wno-comment"
            "-Wno-deprecated-declarations"
            "-Wno-int-to-pointer-cast"
            "-Wno-missing-braces"
            "-Wno-sequence-point"
            "-Wno-tautological-compare"
            "-Wno-unknown-pragmas"
            "-Wno-unused-but-set-variable"
            "-Wno-unused-label"
            "-Wno-unused-value"
            "-Wno-unused-variable"
            "-Wno-unused-function"
        )
    endif()

    if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target_name} PRIVATE
            "/W0"
            "/wd4068"
            "/wd4365"
            "/wd4820"
        )
    endif()
endfunction()
