
# Platform library — all platform-specific code lives here.
# Only this library links SDL3.
# MoX and MoM link Platform, never SDL directly.

set(PLATFORM_SOURCES
    sdl3/sdl3_EMM.c
    sdl3/sdl3_Init.c
    sdl3/sdl3_Input.c
    sdl3/sdl3_KD.c
    sdl3/sdl3_MD.c
    sdl3/sdl3_PFL.c
    sdl3/sdl3_SDLK.c
    sdl3/sdl3_State.c
    sdl3/sdl3_Timer.c
    sdl3/sdl3_Video.c
    replay/Replay.c
)

# SDL3 is always required; SDL3_mixer is optional (controlled by DISABLE_AUDIO).
set(PLATFORM_LINK_LIBS SDL3::SDL3)

if(NOT DISABLE_AUDIO)
    list(APPEND PLATFORM_SOURCES sdl3/sdl3_Audio.c)
    list(APPEND PLATFORM_LINK_LIBS SDL3_mixer::SDL3_mixer)
endif()

add_library(Platform STATIC ${PLATFORM_SOURCES})

# Public: Platform.h, Platform_Keys.h, Platform_Replay.h — visible to consumers (MoX, MoM, etc.)
target_include_directories(Platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Private: SDL3 headers (needed by platform implementations)
target_include_directories(Platform PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/sdl3
)

target_link_libraries(Platform PRIVATE ${PLATFORM_LINK_LIBS})
