/*
    Allocate_Pool - static pool backing for the SimTex Space Alloc arenas.

    Owns one static byte array and a bump-pointer cursor. Pool_Carve() hands out
    contiguous regions front-to-back; nothing is ever returned or freed. The pool
    exists so OG-faithful out-of-bounds accesses off an arena - overruns past its
    end and -1-style underruns before its start - land in addressable,
    deterministic, sentinel-initialized memory instead of faulting under ASan or
    page protection.

    Layout:

        pool_base                                                POOL_SIZE
        |<-- POOL_LEADING_GUARD -->|<-- carved arenas -->|<-- POOL_FIXED_MARGIN -->|

    The leading guard absorbs underruns off the first arena; the trailing margin
    absorbs overruns off the last one. Neither is ever handed out by Pool_Carve().
    Arenas in between get incidental coverage in both directions from tight
    packing.

    This module knows nothing about SAMB headers or any MoM concept.

    See doc/#AI_Plans/PRD-Static-Pool-Allocator.md and PLAN-Static-Pool-Allocator.md.
*/

#ifndef ALLOCATE_POOL_H
#define ALLOCATE_POOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Every byte of the pool is set to this by Pool_Init(); accidental OOB reads see visible garbage, not silent zero. */
#define POOL_SENTINEL_BYTE  0xCC

/* Reserved ahead of the first carve - absorbs -1-style underruns off the first arena. */
#define POOL_LEADING_GUARD  1024

/* Trailing margin - absorbs overruns off the last arena. Never carvable. */
#define POOL_FIXED_MARGIN   (8 * 1024)

/*
    Carvable capacity. It must hold every in-scope Allocate_Space() /
    Allocate_Space_No_Header() arena's (size + 1) * 16 bytes for whichever
    top-level allocation sequence a given process runs. The OG "Space Alloc"
    allocator never frees, so every arena is permanent by design and the pool
    only has to cover that persistent footprint - it is NOT stressed by
    repeated loads: in-game graphics reuse g_graphics_cache_seg via
    Graphics_Cache_Reset(), and the STU world-gen loop reuses its arenas via
    Clear_Game_Data(). Each of the two top-level sequences (the full-game
    Allocate_Data_Space path and the STU world-gen Allocate_Game_Data path)
    runs once per process.

    The full-game path dominates: its enumerated footprint is ~1.5 MiB
    (g_graphics_cache_seg alone is 1 MiB - see the breakdown in
    PRD-Static-Pool-Allocator.md). POOL_ARENA_CAPACITY carries headroom above
    POOL_MIN_ARENA_BYTES for the bounded set of one-time sa_Single LBX loads
    and font-entry buffers whose sizes are only known at run time. Pool_Carve()
    is fatal-on-exhaustion (Allocation_Error), so an undersize surfaces loudly
    rather than corrupting silently.
*/
/*
    POOL_MIN_ARENA_BYTES is the measured floor, POOL_ARENA_CAPACITY the shipped
    size. Measured via Pool_Bytes_Peak(): the HeMoM Continue scenario (full-game
    entry - loads the save, enters the game, which pulls in many one-time
    sa_Single LBX graphic buffers on top of the ~1.5 MiB enumerated arenas)
    peaks at ~4.58 MiB. The capacity carries ~3.5x margin over that for heavier
    game paths. Pool_Carve() is fatal-on-exhaustion, so any path that exceeds
    the capacity fails loudly (raise this constant and re-measure).
*/
#define POOL_MIN_ARENA_BYTES (5 * 1024 * 1024)    /* measured floor: ~4.58 MiB Continue-scenario peak, rounded up */
#define POOL_ARENA_CAPACITY  (16 * 1024 * 1024)   /* ~3.5x the measured peak */

#define POOL_SIZE           (POOL_LEADING_GUARD + POOL_ARENA_CAPACITY + POOL_FIXED_MARGIN)

/* Fill the entire pool (guard and margin included) with POOL_SENTINEL_BYTE and reset the carve cursor to just past the leading guard. Calling it again resets the pool - the carve sequence is deterministic after every init. */
void Pool_Init(void);

/* Hand out the next `bytes` bytes of the pool. Consecutive carves return adjacent addresses; the first carve after Pool_Init() returns pool_base + POOL_LEADING_GUARD. A request that will not fit ahead of the trailing margin is fatal via Allocation_Error() - the caller never sees NULL. */
uint8_t * Pool_Carve(uint32_t bytes);

/* Bytes reserved so far, counting the leading guard. Pool_Bytes_Used() + Pool_Bytes_Free() == POOL_SIZE at all times. */
uint32_t Pool_Bytes_Used(void);

/* Bytes not yet reserved, counting the trailing margin (the final POOL_FIXED_MARGIN bytes are reported free but are never carvable). */
uint32_t Pool_Bytes_Free(void);

/* High-water mark of Pool_Bytes_Used() across the process (reset by Pool_Init). Used to size POOL_ARENA_CAPACITY against the real peak. */
uint32_t Pool_Bytes_Peak(void);

#ifdef __cplusplus
}
#endif

#endif /* ALLOCATE_POOL_H */
