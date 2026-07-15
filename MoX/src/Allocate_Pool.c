/*
    Allocate_Pool - static pool backing for the SimTex Space Alloc arenas.
    See Allocate_Pool.h for the design contract and layout diagram.
*/

#include <string.h>

#include "Allocate_Pool.h"

/* Declared in Allocate.h; forward-declared here so this module stays free of MoX/MoM headers (the isolated unit-test target links a stub instead). Fatal: ends in Exit_With_Message(). */
void Allocation_Error(uint16_t error_num, uint16_t blocks);

static uint8_t pool_storage[POOL_SIZE];

/* Byte offset of the next carve, from pool_storage[0]. Starts at POOL_LEADING_GUARD; never exceeds (POOL_SIZE - POOL_FIXED_MARGIN). */
static uint32_t pool_next_offset = POOL_LEADING_GUARD;


void Pool_Init(void)
{
    memset(pool_storage, POOL_SENTINEL_BYTE, POOL_SIZE);
    pool_next_offset = POOL_LEADING_GUARD;
}


uint8_t * Pool_Carve(uint32_t bytes)
{
    uint8_t * carved = NULL;

    if(bytes > ((POOL_SIZE - POOL_FIXED_MARGIN) - pool_next_offset))
    {
        Allocation_Error(1, (uint16_t)(bytes / 16));  /* fatal; error 1 == out of memory */
        return NULL;  /* not reached */
    }

    carved = &pool_storage[pool_next_offset];
    pool_next_offset += bytes;

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
