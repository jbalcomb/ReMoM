/*
    WIZARDS.EXE
        ovr069
    
    Roads

*/

#ifndef ROADS_H
#define ROADS_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:C130
extern int16_t main_cancel_button_field;



/*
    WIZARDS.EXE ovr069
*/

// WZD o069p01
void Road_Build_Screen(void);

// WZD o069p02
void Road_Build_Screen_Add_Fields(void);

// WZD o069p03
void Road_Build_Screen_Draw(void);

// WZD o069p04
void Draw_Road_Building_Window(void);

// WZD o069p05
void Draw_Road_Build_Path_Line(void);

// WZD o069p06
// Road_Build_Path()
void Road_Build_Path(int16_t x2, int16_t y2);

// WZD o069p07
int16_t Road_Build_Moves2_Max(void);

// WZD o069p08
void Active_Stack_Roadbuilders(int16_t * troop_count, int16_t troops[]);

// WZD o069p09
void Set_Army_Road_Building(int16_t troop_count, int16_t troops[], int16_t dst_wx, int16_t dst_wy);



#ifdef __cplusplus
}
#endif

#endif  /* ROADS_H */
