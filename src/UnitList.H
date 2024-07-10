/*
    Unit List Window

    WIZARDS.EXE
        ovr079
*/

#ifndef UNITLIST_H
#define UNITLIST_H

#include "MoM.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr079
*/

// WZD o79p01
int16_t Unit_List_Window(int16_t unit_idx, int16_t flag, int16_t x, int16_t y);

// WZD o79p02
void Unit_List_Window_Draw(void);

// WZD o79p03
void Unit_List_Window_Add_Fields(int16_t flag);



#ifdef __cplusplus
}
#endif


#endif  /* UNITLIST_H */
