/*
    Level Make

    WIZARDS.EXE
        ovr102
*/
#ifndef LVLMAKE_H
#define LVLMAKE_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr120
*/

// WZD o102p01
void Hero_LevelUp_Popup(int16_t unit_idx);

// WZD o102p02
void Hero_LevelUp_Popup_Draw(void);

// WZD o102p03
void Hero_Build_Specials_List(int16_t unit_idx, struct s_UV_List * specials_list, int16_t * specials_count);



#ifdef __cplusplus
}
#endif

#endif  /* LVLMAKE_H */
