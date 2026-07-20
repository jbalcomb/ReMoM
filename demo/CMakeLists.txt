
# =========================================================================
#  demo_vga — stand-alone VGA / video-effect demo harness.
#
#  A separate executable that links the engine libraries and drives real MoX
#  video effects (PageFlip_Dissolve, VGA_PartCopyFromF3, ...) through the
#  platform layer.  It does not modify or depend on any game entry point
#  (ReMoM.c / MOM_main).  Add effects by dropping in effect_<name>.c and
#  registering them in demo_main.c (see demo_effects.h).
#
#  Usage:  demo_vga <effect>     (run with no args to list effects)
# =========================================================================

add_executable(demo_vga
    demo_main.c
    demo_harness.c
    effect_dissolve.c
    effect_earthquake.c
    effect_screen.c
    effect_summon.c
    effect_lose.c
    effect_win.c
    effect_collage.c
    effect_score.c
    ${PROJECT_SOURCE_DIR}/src/ReMoM_Init.c   # full engine boot for the `score` effect
)

set_target_properties(demo_vga PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(USE_WIN32)
    target_compile_definitions(demo_vga PUBLIC _STU_WIN)
elseif(USE_SDL3)
    target_compile_definitions(demo_vga PUBLIC USE_SDL3)
endif()

if(USE_WIN32)
    set(_sdl_main_lib "")
elseif(USE_SDL3)
    set(_sdl_main_lib SDL3::SDL3)
else()
    set(_sdl_main_lib SDL2::SDL2)
endif()

target_link_libraries(demo_vga PUBLIC
    momlib
    MOX
    STU
    Platform
    ${_sdl_main_lib}
)

target_include_directories(demo_vga PUBLIC
    ${PROJECT_SOURCE_DIR}
)

set_target_properties(demo_vga PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:demo_vga>"
)
