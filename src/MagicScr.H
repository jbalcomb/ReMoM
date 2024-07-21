/*
    Magic Screen
    Mirrow Screen
    
    WIZARDS.EXE
        ovr073
        ovr074
    
    MoO2:
        Module: ¿  ?
*/

#ifndef MAGICSCR_H
#define MAGICSCR_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr073
*/

// WZD o73p01
void Magic_Screen(void);

// WZD o73p02
void Magic_Screen_Draw(void);

// WZD o73p03
void Magic_Screen_Add_Fields(void);

// WZD o73p04
void Build_Overland_Enchantment_List(void);

// WZD o73p05
void Magic_Screen_Load_Pictures(void);

// WZD o73p06
void Alchemy_Popup(int16_t start_x, int16_t y_start);

// WZD o73p07
void Alchemy_Popup_Draw(void);

// WZD o73p08
void Alchemy_Popup_Add_Fields(void);



/*
    WIZARDS.EXE  ovr074
*/

// WZD o74p01
// Mirror_Screen()

// WZD o74p02
// Mirror_Screen_Draw()

// WZD o74p03
// Mirror_Screen_Load()



#ifdef __cplusplus
}
#endif

#endif  /* MAGICSCR_H */
