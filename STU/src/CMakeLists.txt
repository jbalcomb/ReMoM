
set(LIBSTU_SOURCES
    AI_METRICS.c
    STU_BRAK.c
    STU_DBG.c
    STU_DRAW.c
    STU_EMS.c
    STU_FIO.c
    STU_FMT.c
    STU_FON.c
    STU_GRAF.c
    STU_HASH.c
    STU_INIT.c
    STU_INT.c
    STU_JSON.c
    STU_LOG.c
    STU_MSG.c
    STU_PRN.c
    STU_SND.c
    STU_TST.c
    STU_UTIL.c
    STU_VLD.c
    STU_WAV.c
    STU_WRLD.c
    STU_XTRC.c
    lbx_manifest.c
    ${PROJECT_SOURCE_DIR}/ext/stu_compat.c
)

add_library(STU STATIC ${LIBSTU_SOURCES})

# STU_BRAK.c symbolizes crash stacks via DbgHelp on Windows.  PUBLIC so every consumer of STU
# (ReMoMber, HeMoM, tests, tools) links it too -- a static lib cannot carry it for them.
# The MSVC build also gets it from STU_BRAK.c's #pragma comment(lib), but clang/MinGW do not
# honour that pragma, so state it here as well.
if(WIN32)
    target_link_libraries(STU PUBLIC dbghelp)
endif()

target_include_directories(STU PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
