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



enum e_TEXTBOX_BACKGROUND_COLOR
{
    tb_Blue,
    tb_Red,
    tb_Green,
    tb_Brown
};



// WZD o149p01
// drake178: GUI_Confirm_Dialog()
int16_t Confirmation_Box(char * text_string);

// WZD o149p02
// drake178: GUI_DrawConfrmDialog()
void Confirmation_Box_Draw(void);

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
// drake178: GUI_LoadNotifyGfx()
void Notify_Load(int16_t notify_color, SAMB_ptr notiy_pict_seg);

// WZD o149p11
// drake178: GUI_Notify_Dialog
void Notify1__STUB(int16_t Center, int16_t Top, int16_t notify_color, char * Msg, int16_t Gem, SAMB_ptr notiy_pict_seg, int16_t IMGLeft, int16_t IMGTop, SAMB_ptr IMG2, int16_t IMG2Left, int16_t IMG2Top, int16_t CSlide, int16_t BGShade);

// WZD o149p12
// drake178: GUI_DrawNotifyDialog
void Notify1_Draw(void);

// WZD o149p13
// drake178: GUI_Familiar_Notify()
void Notify2(int16_t Center, int16_t Top, int16_t notify_color, char * Msg, int16_t Gem, SAMB_ptr notiy_pict_seg, int16_t IMGLeft, int16_t IMGTop, SAMB_ptr IMG2, int16_t IMG2Left, int16_t IMG2Top, int16_t CSlide, int16_t BGShade);

// WZD o149p14
// drake178: GUI_DrawFamiliarNtfy()
void Notify2_Draw(void);

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
