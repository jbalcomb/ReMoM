
# Platform library — all platform-specific code lives here.
# Only the windowed Platform library links SDL directly.
# MoX and MoM include Platform.h but do NOT link any platform library;
# the executables link the appropriate platform implementation themselves.
#
# Two distinct libraries are always produced:
#   Platform_Headless — no display, no audio, no external deps.  HeMoM and
#                       the test executables link this.
#   Platform          — the windowed backend (SDL2 default, or USE_SDL3 /
#                       USE_WIN32).  ReMoMber links this.

option(USE_SDL3 "Use SDL3 instead of SDL2 for the windowed backend (default: SDL2)" OFF)
option(USE_WIN32 "Use native Win32 backend instead of SDL for the windowed backend (no SDL dependency)" OFF)
option(MOUSE_DEBUG "Enable verbose mouse input debug logging to stderr" OFF)



# =========================================================================
#  Platform_Headless — always built, no external dependencies.
# =========================================================================

add_library(Platform_Headless STATIC
    headless/headless_PFL.c
    headless/headless_KD.c
    headless/headless_Timer.c
    headless/headless_EMM.c
    headless/headless_Audio.c
    headless/headless_State.c
    replay/Replay.c
)

target_include_directories(Platform_Headless PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(Platform_Headless PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headless
)

target_compile_definitions(Platform_Headless PUBLIC NO_SOUND_LIBRARY)

if(MOUSE_DEBUG)
    target_compile_definitions(Platform_Headless PUBLIC MOUSE_DEBUG)
endif()



# =========================================================================
#  Platform — preset-selected windowed backend.
# =========================================================================

if(USE_WIN32)
    set(PLATFORM_BACKEND_DIR win32)
    set(PLATFORM_SOURCES
        win32/win_PFL.cpp
        win32/win_KD.c
        win32/win_MD.c
        win32/win_CLK.c
        win32/win_EMS.c
        win32/win_Exit.c
        win32/win_Video.c
        win32/win_State.c
        replay/Replay.c
    )
    set(PLATFORM_LINK_LIBS winmm)
    set(PLATFORM_DEFINES _STU_WIN NO_SOUND_LIBRARY)
elseif(USE_SDL3)
    set(PLATFORM_BACKEND_DIR sdl3)
    set(PLATFORM_SOURCES
        sdl3/sdl3_EMM.c
        sdl3/sdl3_Init.c
        sdl3/sdl3_KD.c
        sdl3/sdl3_MD.c
        sdl3/sdl3_PFL.c
        sdl3/sdl3_State.c
        sdl3/sdl3_Timer.c
        sdl3/sdl3_Video.c
        replay/Replay.c
    )
    set(PLATFORM_LINK_LIBS SDL3::SDL3)
    set(PLATFORM_DEFINES USE_SDL3)
    if(NOT DISABLE_AUDIO)
        list(APPEND PLATFORM_SOURCES sdl3/sdl3_Audio.c)
        list(APPEND PLATFORM_LINK_LIBS SDL3_mixer::SDL3_mixer)
    endif()
else()
    set(PLATFORM_BACKEND_DIR sdl2)
    set(PLATFORM_SOURCES
        sdl2/sdl2_EMM.c
        sdl2/sdl2_Init.c
        sdl2/sdl2_KD.c
        sdl2/sdl2_MD.c
        sdl2/sdl2_PFL.c
        sdl2/sdl2_State.c
        sdl2/sdl2_Timer.c
        sdl2/sdl2_Video.c
        replay/Replay.c
    )
    set(PLATFORM_LINK_LIBS SDL2::SDL2)
    set(PLATFORM_DEFINES "")
    if(NOT DISABLE_AUDIO)
        list(APPEND PLATFORM_SOURCES sdl2/sdl2_Audio.c)
        list(APPEND PLATFORM_LINK_LIBS SDL2_mixer::SDL2_mixer)
    endif()
endif()

add_library(Platform STATIC ${PLATFORM_SOURCES})

if(PLATFORM_DEFINES)
    target_compile_definitions(Platform PUBLIC ${PLATFORM_DEFINES})
endif()

if(MOUSE_DEBUG)
    target_compile_definitions(Platform PUBLIC MOUSE_DEBUG)
endif()

target_include_directories(Platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(Platform PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_BACKEND_DIR}
)

target_link_libraries(Platform PRIVATE ${PLATFORM_LINK_LIBS})
