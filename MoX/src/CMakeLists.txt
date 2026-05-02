
set(LIBMOX_SOURCES
    AIL.c
    Allocate.c
    angle.c
    capture.c
    CFG.c
    DOS.c
    EMS/EMS.c
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
    MOM_DAT.c
    MOX_DAT.c
    MOX_SET.c
    MOX_SND.c
    MOX_T4.c
    MOX2.c
    paragrph.c
    random.c
    SOUND.c
    special.c
    TEXTBOX.c
    Timer.c
    Util.c
    Video.c
    Video2.c
)

add_library(MOX STATIC ${LIBMOX_SOURCES})

target_include_directories(MOX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# MoX needs the platform/include path for Platform.h / Platform_Keys.h /
# Platform_Replay.h, but does NOT link any platform library.  The actual
# implementation library (Platform_Headless or Platform) is linked by the
# executables (HeMoM, ReMoMber, tests) — keeping MoX library-agnostic lets
# the same MoX object code be linked into both headless and windowed builds.
target_include_directories(MOX PUBLIC
    ${PROJECT_SOURCE_DIR}/platform/include
)

# When the windowed backend is win32, MoX uses Pump_Events()/Pump_Paints()
# under #ifdef _STU_WIN.  The headless backend provides stubs for these so
# HeMoM still links cleanly.
if(USE_WIN32)
    target_compile_definitions(MOX PUBLIC _STU_WIN)
endif()
