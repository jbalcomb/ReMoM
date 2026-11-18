/*
    STU_GARB -- see STU_GARB.h.

    xorshift32; deliberately separate from MoX's random.c so drawing garbage never shifts the game's Random() sequence.
*/

#include "STU_GARB.h"

#define STACK_GARBAGE_DEFAULT_SEED  0x5354524Bu  /* any non-zero value; xorshift32 never leaves 0 */

static uint32_t stack_garbage_seed = STACK_GARBAGE_DEFAULT_SEED;



void Set_Stack_Garbage_Seed(uint32_t seed)
{
    if(seed == 0)
    {
        seed = STACK_GARBAGE_DEFAULT_SEED;
    }
    stack_garbage_seed = seed;
}


uint32_t Get_Stack_Garbage_Seed(void)
{
    return stack_garbage_seed;
}


static uint32_t Stack_Garbage_Next(void)
{
    uint32_t x = stack_garbage_seed;
    x ^= (x << 13);
    x ^= (x >> 17);
    x ^= (x << 5);
    stack_garbage_seed = x;
    return x;
}


int16_t Stack_Garbage(void)
{
    return (int16_t)(Stack_Garbage_Next() >> 16);
}


int16_t Stack_Garbage_Range(int16_t min_value, int16_t max_value)
{
    uint32_t span = 0;
    if(max_value <= min_value)
    {
        return min_value;
    }
    span = (uint32_t)((int32_t)max_value - (int32_t)min_value + 1);
    return (int16_t)((int32_t)min_value + (int32_t)(Stack_Garbage_Next() % span));
}
