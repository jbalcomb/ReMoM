#ifndef STU_UTST_H
#define STU_UTST_H

#include "../../MoX/src/MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif

void Set_Terrain(int16_t wx, int16_t wy, int16_t wp, int16_t terrain_type);

void Set_Terrain_All(int16_t wp, int16_t terrain_type);

#ifdef __cplusplus
}
#endif

#endif  /* STU_UTST_H */
