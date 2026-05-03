
# =========================================================================
#  Standalone Tools
#
#  Small CLI utilities that don't depend on the game engine.  These can be
#  run against save files produced by any version of Master of Magic (OG,
#  ReMoMber, HeMoM).
# =========================================================================


# --- savedump: dump a .GAM save file to structured text ---

add_executable(savedump
    savedump.c
    ${PROJECT_SOURCE_DIR}/src/Game_Save_Dump.c
)

target_include_directories(savedump PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(savedump PRIVATE STU)

set_target_properties(savedump PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# --- rmr2hms: translate a .RMR replay recording to a .hms scenario script ---

add_executable(rmr2hms
    rmr2hms.c
)

set_target_properties(rmr2hms PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
