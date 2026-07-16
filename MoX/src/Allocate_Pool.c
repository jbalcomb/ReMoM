/*
    Allocate_Pool - static pool backing for the SimTex Space Alloc arenas.
    See Allocate_Pool.h for the design contract and layout diagram.
*/

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


void Pool_Init(void)
{
    memset(pool_storage, POOL_SENTINEL_BYTE, POOL_SIZE);
    pool_next_offset = POOL_LEADING_GUARD;
    pool_peak_offset = POOL_LEADING_GUARD;
    pool_initialized = 1;
}


uint8_t * Pool_Carve(uint32_t bytes)
{
    uint8_t * carved = NULL;

    if(pool_initialized == 0) { Pool_Init(); }

    if(bytes > ((POOL_SIZE - POOL_FIXED_MARGIN) - pool_next_offset))
    {
        Allocation_Error(1, (uint16_t)(bytes / 16));  /* fatal; error 1 == out of memory */
        return NULL;  /* not reached */
    }

    carved = &pool_storage[pool_next_offset];
    pool_next_offset += bytes;

    if(pool_next_offset > pool_peak_offset) { pool_peak_offset = pool_next_offset; }

    return carved;
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
