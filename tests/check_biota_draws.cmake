# check_biota_draws.cmake
#
# Asserts which terrain-special ("biota") overlay sprites the overland map draw
# actually blitted, by reading the BIOTA_DRAW trace lines emitted from
# Draw_Map_Biota (MoM/src/MainScr_Maps.c).
#
# Guards the 2024-08-29 regression (commit 1a558b31): the wild-game FLIC_Draw was
# commented out and UU_hunters_lodge_seg was blitted in its place, so every
# wild-game square rendered a hunters-lodge sprite.  The trace hook resolves the
# sprite handle ACTUALLY passed to FLIC_Draw, so a wrong-sprite substitution is
# visible here -- a branch-entry log would have reported "wildgame" either way.
#
# Usage:
#   cmake -DLOG=<remom_log_new.txt>
#         -DEXPECT_WILDGAME=<p/x/y[;p/x/y...]>
#         -P check_biota_draws.cmake
#
# Asserts:
#   1. no HUNTERSLODGE draw where the save has no TS_HUNTERSLODGE square
#   2. at least one WILDGAME draw   (catches "drawn as nothing")
#   3. the distinct set of WILDGAME squares drawn == EXPECT_WILDGAME

if(NOT DEFINED LOG)
    message(FATAL_ERROR "check_biota_draws: -DLOG=<logfile> is required")
endif()

if(NOT EXISTS "${LOG}")
    message(FATAL_ERROR "check_biota_draws: log file not found: ${LOG}\n"
                        "(the HeMoM run step did not produce a log -- did it reach the Main Screen?)")
endif()

file(READ "${LOG}" log_text)

# --- 1. hunters-lodge must not be drawn (the regression's signature) ---
string(REGEX MATCHALL "BIOTA_DRAW p=[0-9]+ x=[0-9]+ y=[0-9]+ sprite=HUNTERSLODGE" lodge_hits "${log_text}")
list(LENGTH lodge_hits lodge_count)
if(lodge_count GREATER 0)
    list(REMOVE_DUPLICATES lodge_hits)
    string(REPLACE ";" "\n    " lodge_pretty "${lodge_hits}")
    message(FATAL_ERROR
        "check_biota_draws: ${lodge_count} HUNTERSLODGE draw(s) found, expected 0.\n"
        "This is the 2024-08-29 signature: the hunters-lodge sprite is being blitted "
        "on wild-game squares instead of IMG_OVL_WildGame.\n"
        "    ${lodge_pretty}")
endif()

# --- 2 + 3. wild-game draws: present, and exactly on the expected squares ---
string(REGEX MATCHALL "BIOTA_DRAW p=[0-9]+ x=[0-9]+ y=[0-9]+ sprite=WILDGAME" wild_hits "${log_text}")
list(LENGTH wild_hits wild_total)
if(wild_total EQUAL 0)
    message(FATAL_ERROR
        "check_biota_draws: no WILDGAME draws found in ${LOG}.\n"
        "Either Draw_Map_Biota never ran, no explored wild-game square was in the "
        "viewport, or the wild-game draw call is disabled again.")
endif()

# Normalise each hit to p/x/y and de-duplicate (the map redraws every frame).
set(wild_squares "")
foreach(hit IN LISTS wild_hits)
    string(REGEX REPLACE "^BIOTA_DRAW p=([0-9]+) x=([0-9]+) y=([0-9]+) sprite=WILDGAME$" "\\1/\\2/\\3" sq "${hit}")
    list(APPEND wild_squares "${sq}")
endforeach()
list(REMOVE_DUPLICATES wild_squares)
list(SORT wild_squares)

set(expected "${EXPECT_WILDGAME}")
list(REMOVE_DUPLICATES expected)
list(SORT expected)

if(NOT "${wild_squares}" STREQUAL "${expected}")
    message(FATAL_ERROR
        "check_biota_draws: wild-game squares drawn do not match expectation.\n"
        "  expected: ${expected}\n"
        "  actual:   ${wild_squares}\n"
        "(${wild_total} total WILDGAME draw calls across all rendered frames)")
endif()

list(LENGTH wild_squares wild_distinct)
message(STATUS "check_biota_draws: OK -- ${wild_distinct} wild-game square(s) drawn with IMG_OVL_WildGame "
               "(${wild_total} draw calls), 0 hunters-lodge draws.")
