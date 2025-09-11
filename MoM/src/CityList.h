/*
    Cities Screen

    WIZARDS.EXE
        ovr065
    
    MoO2:
        Module: ¿ Planets/Colonies Screen ?
*/

#ifndef CITYLIST_H
#define CITYLIST_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr065
*/

// WZD o65p01
void CityList_Screen(void);

// WZD o65p02
void CityList_Draw_Reduced_Map(void);

// WZD o65p03
void CityList_Screen_Load(void);

// WZD o65p04
void CityList_Screen_Draw(void);

// WZD o65p05
// ~ MoO2 Build_Global_Colony_List_()
void Build_City_List(void);

// WZD o65p06
void Update_Cities_List(void);

// WZD o65p07
void CityList_Add_List_Fields(void);

// WZD o65p08
void CityList_Set_List_Item_Count(void);


#ifdef __cplusplus
}
#endif

#endif  /* CITYLIST_H */
