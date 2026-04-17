
set(LIBSTU_SOURCES
    AI_METRICS.c
    STU_DBG.c
    STU_DRAW.c
    STU_EMS.c
    STU_FMT.c
    STU_INT.c
    STU_JSON.c
    STU_MSG.c
    STU_PRN.c
    STU_SND.c
    STU_TST.c
    STU_UTIL.c
    STU_VLD.c
    STU_WAV.c
    STU_WRLD.c
    ${PROJECT_SOURCE_DIR}/ext/stu_compat.c
)

add_library(STU STATIC ${LIBSTU_SOURCES})

target_include_directories(STU PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
