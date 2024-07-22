/*
    WIZARDS.EXE
        ovr077

    Moo2
        Module: ¿ ?

*/

#ifndef OUTPOST_H
#define OUTPOST_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE ovr077
*/

// WZD o077p01
// TILE_Settle()

// WZD o077p02
void Outpost_Screen(int16_t flag);

// WZD o077p03
void Outpost_Screen_Draw(void);

// WZD o077p04
void Outpost_Screen_Load(void);

// WZD o077p05
void Outpost_Garrison_Picture_Coords(int16_t slot, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);

// WZD o077p06
// NameOutpost_Dialog_Popup()

// WZD o077p07
// Draw_NameNewOutpost_Background()

// WZD o077p08
// TILE_CanBeSettled()

// WZD o077p09
// sub_67918()



#ifdef __cplusplus
}
#endif

#endif  /* OUTPOST_H */
