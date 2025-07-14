/*
    WIZARDS.EXE
        ovr094

*/
#ifndef SURVEYOR_H
#define SURVEYOR_H

#include "MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr094
*/

// WZD o094p01
void Surveyor_Screen(void);

// WZD o094p02
void Surveyor_Screen_Add_Fields(void);

// WZD o094p03
void Surveyor_Screen_Draw(void);

// WZD o094p04
void Surveyor_Window_Display(void);

// WZD o094p05
void Surveyor_IDK_Print_Terrain_And_Effect(int16_t IDK_type, int16_t IDK_IDK);

// WZD o094p06
int16_t Surveyor_Lairs(int16_t wx, int16_t wy, int16_t wp);

// WZD o094p07
int16_t Surveyor_Nodes(int16_t wx, int16_t wy, int16_t wp);

// WZD o094p08
int16_t Surveyor_Cities(int16_t wx, int16_t wy, int16_t wp);

// WZD o094p09
void Surveyor_IDK_Set_Font_Stuff__1(void);

// WZD o094p10
void Surveyor_IDK_Set_Font_Stuff__2(void);

// WZD o094p11
int16_t Surveyor_IDK_RiverMouth(int16_t wx, int16_t wy, int16_t wp);



#ifdef __cplusplus
}
#endif

#endif  /* SURVEYOR_H */
