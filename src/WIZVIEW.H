/*
    Mirror Screen

    WIZARDS.EXE
        ovr074
*/

#ifndef WIZVIEW_H
#define WIZVIEW_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr074
*/

// WZD o74p01
void Mirror_Screen(int16_t player_idx, int16_t OL, int16_t OT, int16_t OR, int16_t OB);

// WZD o74p02
void Mirror_Screen_Draw(void);

// WZD o74p03
void Mirror_Screen_Load(void);


#ifdef __cplusplus
}
#endif


#endif  /* WIZVIEW_H */
