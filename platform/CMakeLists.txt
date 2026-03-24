
# Platform library — all platform-specific code lives here.
# Only this library links SDL directly.
# MoX and MoM link Platform, never SDL directly.

option(USE_SDL3 "Use SDL3 instead of SDL2 (default: SDL2)" OFF)
option(USE_WIN32 "Use native Win32 backend instead of SDL (no SDL dependency)" OFF)
option(MOUSE_DEBUG "Enable verbose mouse input debug logging to stderr" OFF)

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
    add_compile_definitions(NO_SOUND_LIBRARY)
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
    if(NOT DISABLE_AUDIO)
        list(APPEND PLATFORM_SOURCES sdl2/sdl2_Audio.c)
        list(APPEND PLATFORM_LINK_LIBS SDL2_mixer::SDL2_mixer)
    endif()
endif()

add_library(Platform STATIC ${PLATFORM_SOURCES})

if(USE_WIN32)
    target_compile_definitions(Platform PUBLIC _STU_WIN)
elseif(USE_SDL3)
    target_compile_definitions(Platform PUBLIC USE_SDL3)
endif()

if(MOUSE_DEBUG)
    target_compile_definitions(Platform PUBLIC MOUSE_DEBUG)
endif()

# Public: Platform.h, Platform_Keys.h, Platform_Replay.h — visible to consumers (MoX, MoM, etc.)
target_include_directories(Platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Private: backend headers (needed by platform implementations)
target_include_directories(Platform PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_BACKEND_DIR}
)

target_link_libraries(Platform PRIVATE ${PLATFORM_LINK_LIBS})
