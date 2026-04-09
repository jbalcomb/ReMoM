
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
    ${PROJECT_SOURCE_DIR}/src/HeMoM_SaveDump.c
)

target_include_directories(savedump PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(savedump PRIVATE STU)

set_target_properties(savedump PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
