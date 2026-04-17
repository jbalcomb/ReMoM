#ifndef STU_VLD_H
#define STU_VLD_H

#include "STU_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif



void Validate_Units_Status(int8_t status);
void Validate_Terrain_Types(void);

int16_t Validate_Lair_Record(int16_t lair_idx);
int16_t Validate_All_Lairs(void);

int16_t Validate_City_Record(int16_t city_idx);
int16_t Validate_All_Cities(void);
int16_t Validate_All_Neutral_Cities(void);



#ifdef __cplusplus
}
#endif

#endif /* STU_VLD_H */
