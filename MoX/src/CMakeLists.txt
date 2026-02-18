
set(WIN32_SOURCES
    win_CLK.c
    win_EMS.c
    win_Fonts.c
    win_Input.c
    win_KD.c
    win_MD.c
    win_MoM.cpp
    win_PFL.cpp
    win_Video.c
)

set(SDL2_SOURCES
    sdl2_EMM.c
    sdl2_Input.c
    sdl2_KD.c
    sdl2_MD.c
    sdl2_MOM.c
    sdl2_PFL.c
    sdl2_Timer.c
)


set(PFL_SOURCES
    MOM_PFL.c
)

set(MOX_SOURCES
    AIL.c
    Allocate.c
    angle.c
    capture.c
    CFG.c
    DOS.c
    EMM.c
    EXIT.c
    Fields.c
    file_ani.c
    Fonts.c
    GENDRAW.c
    GFILE.c
    Graphics.c
    FLIC_Draw.c
    Help.c
    Input.c
    IO_Buffer.c
    Keyboard.c
    LBX_Load.c
    LOADSAVE.c
    Mouse.c
    MOM_Data.c
    MOX_DAT.c
    MOX_SET.c
    MOX_SND.c
    MOX_T4.c
    paragrph.c
    random.c
    sdl2_Audio.c
    SOUND.c
    TEXTBOX.c
    Util.c
    Video.c
    Video2.c
)

add_library(MOX STATIC ${MOX_SOURCES} ${PFL_SOURCES} ${SDL2_SOURCES})

# set_target_properties(MOX PROPERTIES LINKER_LANGUAGE C)
set_target_properties(MOX PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(MOX PRIVATE
    _CRT_SECURE_NO_WARNINGS=TRUE
    $<$<CONFIG:Debug>:STU_DEBUG=TRUE>
    $<$<CONFIG:Debug>:STU_TRACE=TRUE>
    $<$<CONFIG:Debug>:_STU_SDL2=TRUE>
)

target_include_directories(MOX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

target_link_libraries(MOX
    SDL2::Core
    SDL2::Mixer
)
