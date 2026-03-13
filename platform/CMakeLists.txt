
# Platform library — all platform-specific code lives here.
# Only this library links SDL2 (will become SDL3 in Phase 3).
# MoX and MoM link Platform, never SDL directly.

set(PLATFORM_SDL2_SOURCES
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
)

if(NOT DISABLE_AUDIO)
    list(APPEND PLATFORM_SDL2_SOURCES
        sdl3/sdl3_Audio.c
    )
endif()

add_library(Platform STATIC ${PLATFORM_SDL2_SOURCES})

set_target_properties(Platform PROPERTIES POSITION_INDEPENDENT_CODE ON)

ReMoM_Apply_Common_Compile_Definitions(Platform)
ReMoM_Apply_Common_Compile_Options(Platform)

# Public: Platform.h, Platform_Keys.h, Platform_Replay.h — visible to consumers (MoX, MoM, etc.)
target_include_directories(Platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Private: SDL2 headers and MoX engine headers (needed by platform implementations)
target_include_directories(Platform PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/sdl3
    ${PROJECT_SOURCE_DIR}/MoX/src
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

if(NOT DISABLE_AUDIO)
    target_link_libraries(Platform PRIVATE
        SDL2::Core
        SDL2::Mixer
    )
else()
    target_link_libraries(Platform PRIVATE
        SDL2::Core
    )
endif()
