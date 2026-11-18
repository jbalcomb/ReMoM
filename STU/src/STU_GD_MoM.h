/*
    STU_GD_MoM -- the Master of Magic bindings for STU_GD: which game arrays are dumped, under which [GD] label, with which field map.

    Every function emits [GD] lines for one game array (or array family) tagged with point, in the format documented in STU_GD.h.
    Call them through the STU_GD_ONCE / STU_GD_DO call-site macros so they compile out without STU_GD_CAPTURE and cost nothing
    when REMOM_GD_CAPTURE is unset.  Reverse-engineering evaluation code: no counterpart in the original game.

    Moved from MoM/src/INITGAME.c, MAPGEN.c and AIMOVE.c (gd_dump_*).
*/

#ifndef STU_GD_MOM_H
#define STU_GD_MOM_H

#include "STU_GD.h"

#ifdef __cplusplus
extern "C" {
#endif

/* records, through the generated gd_*_fields.h field maps */
void STU_GD_Dump_Players(const char * point);       /* _players[0.._num_players)                                   */
void STU_GD_Dump_Nodes(const char * point);         /* _NODES[NUM_NODES]                                           */
void STU_GD_Dump_Towers(const char * point);        /* _TOWERS[NUM_TOWERS]                                         */
void STU_GD_Dump_Fortresses(const char * point);    /* _FORTRESSES[NUM_FORTRESSES]                                 */
void STU_GD_Dump_Lairs(const char * point);         /* _LAIRS[NUM_LAIRS]                                           */
void STU_GD_Dump_Cities(const char * point);        /* _cities, then _CITIES[NUM_CITIES]  (compare-gd masks n >= _cities) */
void STU_GD_Dump_Heroes(const char * point);        /* _HEROES2[NUM_PLAYERS], unallocated players skipped           */
void STU_GD_Dump_Units(const char * point);         /* _units, then _UNITS[NUM_UNITS]  (compare-gd masks n >= _units)     */
void STU_GD_Dump_Items(const char * point);         /* _ITEMS[NUM_ITEMS]                                           */

/* per-square world grids, one line per (plane, row) */
void STU_GD_Dump_World_Map(const char * point);                 /* _world_maps (p_world_map, int16)       */
void STU_GD_Dump_Landmasses(const char * point);                /* _landmasses                            */
void STU_GD_Dump_Terrain_Specials(const char * point);          /* _map_square_terrain_specials           */
void STU_GD_Dump_Map_Square_Flags(const char * point);          /* _map_square_flags                      */
void STU_GD_Dump_Square_Explored(const char * point);           /* _square_explored                       */
void STU_GD_Dump_Movement_Mode_Cost_Maps(const char * point);   /* movement_mode_cost_maps, 6 modes       */
void STU_GD_Dump_Connectivity_Grids(const char * point);        /* connectivity_grid_land / _sea          */

/* AI scratch arrays */
void STU_GD_Dump_AI_Evaluation_Map(const char * point);         /* g_ai_evaluation_map, <=100 per line                                       */
void STU_GD_Dump_AI_Landmass_Squares(const char * point);       /* _ai_landmass_land_squares heads, and lists/wx/wy up to the used node count */
void STU_GD_Dump_AI_Dock_Squares(const char * point);           /* _ai_landmass_dock_squares, same layout                                    */
void STU_GD_Dump_AI_Continents(const char * point);             /* _ai_continents wx/wy/type per (plane, player)                             */

#ifdef __cplusplus
}
#endif

#endif /* STU_GD_MOM_H */
