
add_executable(ReMoMber ReMoM.c)

# set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
set_target_properties(ReMoMber PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(ReMoMber PUBLIC
    momlib
    MOX
    STU
    SDL2::Main
    SDL2::Mixer
)

target_include_directories(ReMoMber PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/MOX
    ${PROJECT_SOURCE_DIR}/src/STU
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

# target_compile_definitions(ReMoMber PRIVATE
#     _CRT_SECURE_NO_WARNINGS=TRUE
# )
# target_compile_definitions(ReMoMber PRIVATE
#     $<$<CONFIG:Debug>:STU_DEBUG=TRUE>
# )
# target_compile_definitions(ReMoMber PRIVATE
#     $<$<CONFIG:Debug>:STU_TRACE=TRUE>
# )
# target_compile_definitions(ReMoMber PRIVATE
#     $<$<CONFIG:Debug>:_STU_SDL2=TRUE>
# )
target_compile_definitions(ReMoMber PRIVATE
    _CRT_SECURE_NO_WARNINGS=TRUE
    $<$<CONFIG:Debug>:STU_DEBUG=TRUE>
    $<$<CONFIG:Debug>:STU_TRACE=TRUE>
    $<$<CONFIG:Debug>:_STU_SDL2=TRUE>
)

if(CMAKE_COMPILER_IS_CLANG)
    target_compile_options(ReMoMber PRIVATE
        "-Wall"
        "-Wno-deprecated-declarations"
        "-Wno-int-to-pointer-cast"
        "-Wno-missing-braces"
        "-Wno-tautological-compare"
        "-Wno-unknown-pragmas"
        "-Wno-unused-but-set-variable"
        "-Wno-unused-label"
        "-Wno-unused-variable"
        "-Wno-unused-function"
        "-Wno-writable-strings"
    )
endif()
if(CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(ReMoMber PRIVATE
        "-Wall"
        "-Wno-deprecated-declarations"
        "-Wno-int-to-pointer-cast"
        "-Wno-missing-braces"
        "-Wno-tautological-compare"
        "-Wno-unknown-pragmas"
        "-Wno-unused-but-set-variable"
        "-Wno-unused-label"
        "-Wno-unused-variable"
        "-Wno-unused-function"
    )
endif()
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(ReMoMber PRIVATE
        "/W0"
        "/wd4068"  # "unknown pragma"
        "/wd4365"
        "/wd4820"
    )
endif()

if(WIN32)
    add_custom_command(
        TARGET ReMoMber
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:ReMoMber>
        COMMAND_EXPAND_LISTS
        COMMENT "./src: Copying SDL2.dll"
    )
endif()

if(WIN32)
    add_custom_command(
        TARGET ReMoMber
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${SDL2_MIXER_DLL_FILE_PATH}
            $<TARGET_FILE_DIR:ReMoMber>
        COMMAND_EXPAND_LISTS
        COMMENT "./src: Copying SDL2_mixer.dll"
    )
endif()

file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx")
add_custom_command(
    TARGET ReMoMber
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${LBX_FILE_LIST}
        $<TARGET_FILE_DIR:ReMoMber>
    COMMAND_EXPAND_LISTS
    COMMENT "./src: Copying MoMv131 LBX files"
)
