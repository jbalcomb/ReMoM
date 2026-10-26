/*
    Integrity_Check - runtime integrity checkpoints.

    NOT original game code (no WIZARDS.EXE counterpart).  Cheap checks run at a few named points in the game loop, so
    corrupt state is reported where it first appears instead of as a crash somewhere downstream.  Every failure goes to
    the INTEGRITY log category at ERROR severity, with the checkpoint name.

    Checks:
        - game state: _map_plane, _active_world_x/_active_world_y, _units in range
        - _world_maps: every terrain index within TerType_Count (what Draw_Map_Terrain() asserts per square)
        - static-pool guard damage: names the arena that overran its end (only with REMOM_POOL_GUARDS=1; see Allocate_Pool.h)

    A state failure is logged once when it appears and once when it clears, not on every call, so a per-frame
    checkpoint cannot flood the log.  Guard damage is logged and then reset, so each overrun is reported once.

    Only call it where a game is loaded -- before that, _world_maps and the map focus hold whatever the allocator left.
*/

#ifndef INTEGRITY_CHECK_H
#define INTEGRITY_CHECK_H

#include "../../MoX/src/MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Run every check; log each new failure with `where`.  Returns the number of checks currently failing (0 = all good). */
int16_t Integrity_Checkpoint(const char * where);

/* Non-zero when the world-map terrain index at (wx, wy, wp) can be drawn: the plane and square are in range and the terrain index is within TerType_Count.  For draw code that must refuse a bad square instead of reading outside the terrain data. */
int16_t Integrity_Terrain_Square_Is_Valid(int16_t wx, int16_t wy, int16_t wp);

/* Draw code that refused squares calls this once per draw pass with the count (0 included).  Logged when refusing starts and when it stops -- tracked in the caller's *refusing_state -- not on every frame. */
void Integrity_Report_Refused_Squares(const char * where, int16_t * refusing_state, int16_t refused_count, int16_t first_wx, int16_t first_wy, int16_t wp);

#ifdef __cplusplus
}
#endif

#endif /* INTEGRITY_CHECK_H */
