set(STU_SOURCES
    STU_CHK.c
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
)

add_library(STU STATIC ${STU_SOURCES})

get_target_property(inc_dir STU INCLUDE_DIRECTORIES)
message(STATUS "STU INCLUDE_DIRECTORIES: ${inc_dir}")

set_target_properties(STU PROPERTIES LINKER_LANGUAGE C)

target_include_directories(STU PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

target_link_libraries(STU SDL2::Core SDL2::Mixer)

get_target_property(inc_dir STU INCLUDE_DIRECTORIES)
message(STATUS "STU INCLUDE_DIRECTORIES: ${inc_dir}")
