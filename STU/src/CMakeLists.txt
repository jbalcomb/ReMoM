
set(STU_SOURCES
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
    STU_WAV.c
    STU_WRLD.c
)

add_library(STU STATIC ${STU_SOURCES})

# set_target_properties(STU PROPERTIES LINKER_LANGUAGE C)
set_target_properties(STU PROPERTIES POSITION_INDEPENDENT_CODE ON)

ReMoM_Apply_Common_Compile_Definitions(STU)
ReMoM_Apply_Common_Compile_Options(STU)

# target_include_directories(STU PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${SDL2_INCLUDE_DIR}
#     ${SDL2_MIXER_INCLUDE_DIR}
# )
target_include_directories(STU PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(STU
    SDL2::Core
)
