/*
    Allocate_Pool - static pool backing for the SimTex Space Alloc arenas.
    See Allocate_Pool.h for the design contract and layout diagram.
*/

#include <stdlib.h>
#include <string.h>

#include "Allocate_Pool.h"

/* Declared in Allocate.h; forward-declared here so this module stays free of MoX/MoM headers (the isolated unit-test target links a stub instead). Fatal: ends in Exit_With_Message(). */
void Allocation_Error(uint16_t error_num, uint16_t blocks);

/* Compile-time check (C89-portable negative-array idiom; these .c files build as C90, so _Static_assert is unavailable): the capacity must cover the enumerated in-scope Allocate_Space footprint. A shrink below POOL_MIN_ARENA_BYTES makes this array size negative and fails the build. See Allocate_Pool.h. */
typedef char pool_capacity_covers_enumerated_minimum[(POOL_ARENA_CAPACITY >= POOL_MIN_ARENA_BYTES) ? 1 : -1];

static uint8_t pool_storage[POOL_SIZE];

/* Byte offset of the next carve, from pool_storage[0]. Starts at POOL_LEADING_GUARD; never exceeds (POOL_SIZE - POOL_FIXED_MARGIN). */
static uint32_t pool_next_offset = POOL_LEADING_GUARD;

/* Set by Pool_Init(). Pool_Carve() lazily inits on first use so production code needs no explicit startup call; tests that call Pool_Init() directly reset it. */
static int pool_initialized = 0;

/* High-water mark of pool_next_offset across the process. */
static uint32_t pool_peak_offset = POOL_LEADING_GUARD;

/* Guard mode - see Allocate_Pool.h. */
struct s_POOL_ARENA
{
    uint32_t offset;                        /* from pool_storage[0] */
    uint32_t bytes;                         /* as requested; the guard starts at offset + bytes */
    const char * site_file;
    int32_t site_line;
    char label[POOL_CARVE_LABEL_LEN];
    int32_t label_num;
};

static int pool_guard_mode = 0;
static struct s_POOL_ARENA pool_arenas[POOL_MAX_TRACKED_ARENAS];
static uint32_t pool_arena_count = 0;

/* Pending call site / label for the next carve; cleared when consumed. */
static const char * pool_next_site_file = NULL;
static int32_t pool_next_site_line = 0;
static char pool_next_label[POOL_CARVE_LABEL_LEN] = { 0 };
static int32_t pool_next_label_num = -1;


static void Pool_Clear_Pending_Site(void)
{
    pool_next_site_file = NULL;
    pool_next_site_line = 0;
    pool_next_label[0] = '\0';
    pool_next_label_num = -1;
}


void Pool_Init_Guard_Mode(int guard_mode)
{
    memset(pool_storage, POOL_SENTINEL_BYTE, POOL_SIZE);
    pool_next_offset = POOL_LEADING_GUARD;
    pool_peak_offset = POOL_LEADING_GUARD;
    pool_guard_mode = (guard_mode != 0) ? 1 : 0;
    pool_arena_count = 0;
    Pool_Clear_Pending_Site();
    pool_initialized = 1;
}


void Pool_Init(void)
{
    const char * env_guards = getenv("REMOM_POOL_GUARDS");
    Pool_Init_Guard_Mode(((env_guards != NULL) && (env_guards[0] != '\0') && (env_guards[0] != '0')) ? 1 : 0);
}


uint8_t * Pool_Carve(uint32_t bytes)
{
    uint8_t * carved = NULL;
    uint32_t footprint = 0;
    struct s_POOL_ARENA * arena = NULL;

    if(pool_initialized == 0) { Pool_Init(); }

    footprint = bytes + ((pool_guard_mode != 0) ? POOL_GUARD_BYTES : 0);

    if(footprint > ((POOL_SIZE - POOL_FIXED_MARGIN) - pool_next_offset))
    {
        Allocation_Error(1, (uint16_t)(bytes / 16));  /* fatal; error 1 == out of memory */
        return NULL;  /* not reached */
    }

    carved = &pool_storage[pool_next_offset];

    if(pool_arena_count < POOL_MAX_TRACKED_ARENAS)
    {
        arena = &pool_arenas[pool_arena_count];
        arena->offset = pool_next_offset;
        arena->bytes = bytes;
        arena->site_file = pool_next_site_file;
        arena->site_line = pool_next_site_line;
        memcpy(arena->label, pool_next_label, POOL_CARVE_LABEL_LEN);
        arena->label_num = pool_next_label_num;
    }
    pool_arena_count++;
    Pool_Clear_Pending_Site();

    pool_next_offset += bytes;

    if(pool_guard_mode != 0)
    {
        memset(&pool_storage[pool_next_offset], POOL_GUARD_BYTE, POOL_GUARD_BYTES);
        pool_next_offset += POOL_GUARD_BYTES;
    }

    if(pool_next_offset > pool_peak_offset) { pool_peak_offset = pool_next_offset; }

    return carved;
}


int Pool_Guards_Enabled(void)
{
    return pool_guard_mode;
}


void Pool_Note_Carve_Site(const char * file, int line)
{
    pool_next_site_file = file;
    pool_next_site_line = line;
}


void Pool_Note_Carve_Label(const char * label, int label_num)
{
    size_t itr = 0;
    if(label == NULL) { label = ""; }
    for(itr = 0; ((itr < (POOL_CARVE_LABEL_LEN - 1)) && (label[itr] != '\0')); itr++)
    {
        pool_next_label[itr] = label[itr];
    }
    pool_next_label[itr] = '\0';
    pool_next_label_num = label_num;
}


/* Scan [start, start + length) for bytes other than expected. Returns the number found; *first_bad gets the offset of the first. */
static uint32_t Pool_Scan_Guard(uint32_t start, uint32_t length, uint8_t expected, uint32_t * first_bad)
{
    uint32_t itr = 0;
    uint32_t bad_count = 0;
    for(itr = 0; itr < length; itr++)
    {
        if(pool_storage[(start + itr)] != expected)
        {
            if(bad_count == 0) { *first_bad = itr; }
            bad_count++;
        }
    }
    return bad_count;
}


static void Pool_Record_Damage(struct s_POOL_GUARD_DAMAGE * damage, int max_damage, int damage_count, int32_t arena_idx, const struct s_POOL_ARENA * arena, uint32_t first_bad, uint32_t bad_count)
{
    struct s_POOL_GUARD_DAMAGE * entry = NULL;
    if((damage == NULL) || (damage_count >= max_damage)) { return; }
    entry = &damage[damage_count];
    memset(entry, 0, sizeof(*entry));
    entry->arena_idx = arena_idx;
    entry->label_num = -1;
    if(arena != NULL)
    {
        entry->arena_base = &pool_storage[arena->offset];
        entry->arena_bytes = arena->bytes;
        entry->site_file = arena->site_file;
        entry->site_line = arena->site_line;
        memcpy(entry->label, arena->label, POOL_CARVE_LABEL_LEN);
        entry->label_num = arena->label_num;
    }
    entry->first_bad_offset = first_bad;
    entry->bad_byte_count = bad_count;
}


int Pool_Check_Guards(struct s_POOL_GUARD_DAMAGE * damage, int max_damage)
{
    int damage_count = 0;
    uint32_t itr = 0;
    uint32_t tracked = 0;
    uint32_t first_bad = 0;
    uint32_t bad_count = 0;
    const struct s_POOL_ARENA * arena = NULL;

    if((pool_initialized == 0) || (pool_guard_mode == 0)) { return 0; }

    /* Leading guard: an underrun off the first arena. */
    bad_count = Pool_Scan_Guard(0, POOL_LEADING_GUARD, POOL_SENTINEL_BYTE, &first_bad);
    if(bad_count > 0)
    {
        Pool_Record_Damage(damage, max_damage, damage_count, POOL_GUARD_REGION_LEADING, NULL, first_bad, bad_count);
        damage_count++;
    }

    tracked = (pool_arena_count < POOL_MAX_TRACKED_ARENAS) ? pool_arena_count : POOL_MAX_TRACKED_ARENAS;
    for(itr = 0; itr < tracked; itr++)
    {
        arena = &pool_arenas[itr];
        bad_count = Pool_Scan_Guard((arena->offset + arena->bytes), POOL_GUARD_BYTES, POOL_GUARD_BYTE, &first_bad);
        if(bad_count > 0)
        {
            Pool_Record_Damage(damage, max_damage, damage_count, (int32_t)itr, arena, first_bad, bad_count);
            damage_count++;
        }
    }

    /* Trailing margin: an overrun off the last arena. */
    bad_count = Pool_Scan_Guard((POOL_SIZE - POOL_FIXED_MARGIN), POOL_FIXED_MARGIN, POOL_SENTINEL_BYTE, &first_bad);
    if(bad_count > 0)
    {
        Pool_Record_Damage(damage, max_damage, damage_count, POOL_GUARD_REGION_TRAILING, NULL, first_bad, bad_count);
        damage_count++;
    }

    return damage_count;
}


void Pool_Reset_Guards(void)
{
    uint32_t itr = 0;
    uint32_t tracked = 0;

    if((pool_initialized == 0) || (pool_guard_mode == 0)) { return; }

    memset(&pool_storage[0], POOL_SENTINEL_BYTE, POOL_LEADING_GUARD);
    tracked = (pool_arena_count < POOL_MAX_TRACKED_ARENAS) ? pool_arena_count : POOL_MAX_TRACKED_ARENAS;
    for(itr = 0; itr < tracked; itr++)
    {
        memset(&pool_storage[(pool_arenas[itr].offset + pool_arenas[itr].bytes)], POOL_GUARD_BYTE, POOL_GUARD_BYTES);
    }
    memset(&pool_storage[(POOL_SIZE - POOL_FIXED_MARGIN)], POOL_SENTINEL_BYTE, POOL_FIXED_MARGIN);
}


uint32_t Pool_Arena_Count(void)
{
    return pool_arena_count;
}


uint32_t Pool_Tracked_Arena_Count(void)
{
    return (pool_arena_count < POOL_MAX_TRACKED_ARENAS) ? pool_arena_count : POOL_MAX_TRACKED_ARENAS;
}


uint32_t Pool_Bytes_Used(void)
{
    return pool_next_offset;
}


uint32_t Pool_Bytes_Free(void)
{
    return (POOL_SIZE - pool_next_offset);
}


uint32_t Pool_Bytes_Peak(void)
{
    return pool_peak_offset;
}
