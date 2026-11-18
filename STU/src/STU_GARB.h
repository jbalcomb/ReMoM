/*
    STU_GARB -- stand-in values for the original game's stack garbage.

    For reads of uninitialized locals / registers in the original game (OGBUG sites). The original read whatever an earlier call
    left in that stack slot or register; these supply a value from a PRNG of their own, so the game's Random() stream
    (random_seed) is never touched and replays / --seed1 runs stay aligned.
*/

#ifndef STU_GARB_H
#define STU_GARB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Set_Stack_Garbage_Seed(uint32_t seed);

uint32_t Get_Stack_Garbage_Seed(void);

/* any int16_t value; for garbage that is only compared or added */
int16_t Stack_Garbage(void);

/* a value in [min_value, max_value]; for garbage that is used as an index or a loop count, so ReMoM does not read out of bounds */
int16_t Stack_Garbage_Range(int16_t min_value, int16_t max_value);

#ifdef __cplusplus
}
#endif

#endif /* STU_GARB_H */
