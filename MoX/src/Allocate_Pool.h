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

    See doc/@AI_Plans/PRD-Static-Pool-Allocator.md and PLAN-Static-Pool-Allocator.md.
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



/*
    Guard mode - overrun detection (opt-in).

    Tight packing means an overrun off one arena silently lands in the next one. Guard mode puts POOL_GUARD_BYTES of
    POOL_GUARD_BYTE after every arena and records each arena's call site; Pool_Check_Guards() then reports every guard
    whose bytes changed, naming the arena that overran its end - the writer, not the victim. It also checks the leading
    guard and trailing margin.

    Off by default: the padding moves where OG-faithful overruns land, so the normal layout must stay the packed one.
    Pool_Init() turns it on when the environment variable REMOM_POOL_GUARDS is set to a non-zero value; tests call
    Pool_Init_Guard_Mode() directly. With guard mode off, Pool_Check_Guards() is a no-op that reports nothing.
*/

#define POOL_GUARD_BYTES            64
#define POOL_GUARD_BYTE             0xFD    /* distinct from POOL_SENTINEL_BYTE so an overrun copying sentinel-filled memory still shows */
#define POOL_MAX_TRACKED_ARENAS     4096
#define POOL_CARVE_LABEL_LEN        16

/* arena_idx values for the two regions that are not arenas */
#define POOL_GUARD_REGION_LEADING   (-1)
#define POOL_GUARD_REGION_TRAILING  (-2)

/* One damaged guard, as reported by Pool_Check_Guards(). */
struct s_POOL_GUARD_DAMAGE
{
    int32_t arena_idx;                          /* carve order (0 = first carve), or POOL_GUARD_REGION_* */
    uint8_t * arena_base;                       /* what Pool_Carve() returned; NULL for the leading/trailing regions */
    uint32_t arena_bytes;                       /* bytes requested for the arena */
    const char * site_file;                     /* call site recorded by Pool_Note_Carve_Site(), or NULL */
    int32_t site_line;
    char label[POOL_CARVE_LABEL_LEN];           /* label recorded by Pool_Note_Carve_Label() (e.g. an LBX name), or "" */
    int32_t label_num;                          /* e.g. the LBX entry number, or -1 */
    uint32_t first_bad_offset;                  /* offset into the guard of the first changed byte - how far past its end the arena was written */
    uint32_t bad_byte_count;                    /* changed bytes in the guard */
};

/* Same as Pool_Init(), with guard mode set explicitly instead of from REMOM_POOL_GUARDS. */
void Pool_Init_Guard_Mode(int guard_mode);

/* Non-zero when guard mode is on. */
int Pool_Guards_Enabled(void);

/* Record the source location of the next carve. Allocate.h wraps Allocate_Space() / Allocate_Space_No_Header() in a macro that calls this, so every arena is tagged with its allocating call site. Consumed by the next Pool_Carve(). */
void Pool_Note_Carve_Site(const char * file, int line);

/* Record a label for the next carve (e.g. the LBX name and entry an sa_Single load is for). Copied, so the caller's buffer need not persist. Consumed by the next Pool_Carve(). */
void Pool_Note_Carve_Label(const char * label, int label_num);

/* Scan every guard. Fills up to max_damage entries of damage[] and returns the total number of damaged guards found (which may exceed max_damage). Returns 0 when guard mode is off. */
int Pool_Check_Guards(struct s_POOL_GUARD_DAMAGE * damage, int max_damage);

/* Restore every damaged guard to POOL_GUARD_BYTE, so the next Pool_Check_Guards() reports only new damage. */
void Pool_Reset_Guards(void);

/* Number of arenas carved since Pool_Init(), and how many of them are tracked (at most POOL_MAX_TRACKED_ARENAS). */
uint32_t Pool_Arena_Count(void);
uint32_t Pool_Tracked_Arena_Count(void);

#ifdef __cplusplus
}
#endif

#endif /* ALLOCATE_POOL_H */
