/*
    WIZARDS.EXE
        ovr149

    MoO2
        GENDRAW
*/

#ifndef GENDRAW_H
#define GENDRAW_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD o149p01
// GUI_Confirm_Dialog  
// WZD o149p02
// GUI_DrawConfrmDialog
// WZD o149p03
// GUI_WarningType1    

// WZD o149p04
// drake178: GUI_WarningType0()
void GUI_WarningType0(char * msg);

// WZD o149p05
// GUI_ShowRedMessage  
// WZD o149p06
// GUI_DrawRedMessage  

// WZD o149p07
// drake178: GUI_ListSelect_Dlg()
// int16_t Selection_Box(int16_t item_count, char * list_text[], int16_t multi_page, char * title_string);
int16_t Selection_Box(int16_t item_count, char ** list_text, int16_t multi_page, char * title_string);

// WZD o149p08
// drake178: GUI_DrawListSelector()
void Selection_Box_Draw(void);

// WZD o149p09
// drake178: GUI_SetListSelBounds()
// void Selection_Box_Coords(int16_t item_count, char * list_text[], char * title_string);
void Selection_Box_Coords(int16_t item_count, char ** list_text, char * title_string);

// WZD o149p10
// GUI_LoadNotifyGfx   
// WZD o149p11
// GUI_Notify_Dialog   
// WZD o149p12
// GUI_DrawNotifyDialog
// WZD o149p13
// GUI_Familiar_Notify 
// WZD o149p14
// GUI_DrawFamiliarNtfy
// WZD o149p15
// HLP_NoResizeExpand  
// WZD o149p16
// HLP_DrawExpanding   
// WZD o149p17
// G_USW_DrawEHLines   
// WZD o149p18
// HLP_DrawExpandLines 
// WZD o149p19
// ITEM_GetViewDrawPos 
// WZD o149p20
// ITEM_ResetViewGrow  

// WZD o149p21
// drake178: EMM_Sandbox2VGAFILEH
void Save_ScreenSeg(void);

// WZD o149p22
// drake178: EMM_VGAFILEH2Sandbox
void Restore_ScreenSeg(void);



#ifdef __cplusplus
}
#endif

#endif  /* GENDRAW_H */
