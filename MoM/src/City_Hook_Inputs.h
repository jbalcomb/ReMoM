/*
    City_Hook_Inputs -- the city-economy mod hooks, with the inputs Caster of Magic's economy scripts expect.
*/

#ifndef CITY_HOOK_INPUTS_H
#define CITY_HOOK_INPUTS_H

#include <stdint.h>

#include "../../MOD/src/MOD_Hook.h"

#ifdef __cplusplus
extern "C" {
#endif

/* MOD_Hook_City_Value() for city city_idx, passing the hook's CoM2 inputs (FARMERS, BASEGOLD, ...).  Returns value
   unchanged, without computing anything, when the hook has no handler. */
int32_t City_Hook_Value(int16_t hook_id, int16_t city_idx, int32_t value);

#ifdef __cplusplus
}
#endif

#endif  /* CITY_HOOK_INPUTS_H */
