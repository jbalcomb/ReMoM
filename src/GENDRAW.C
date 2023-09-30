/*
    WIZARDS.EXE
        ovr149

    MoO2
        GENDRAW
        TEXTBOX
        Message_Box_() |-> Do_Text_Box()

*/

#include "MoX.H"

// WZD o149p01
// GUI_Confirm_Dialog  
// WZD o149p02
// GUI_DrawConfrmDialog
// WZD o149p03
// GUI_WarningType1    

// WZD o149p04
// drake178: GUI_WarningType0()
void GUI_WarningType0(char * msg)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GUI_WarningType0(msg = %s)\n", __FILE__, __LINE__, msg);
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GUI_WarningType0(msg = %s)\n", __FILE__, __LINE__, msg);
#endif

}

// WZD o149p05
// GUI_ShowRedMessage  

// WZD o149p06
void GUI_DrawRedMessage(void)
{
    // Set_Font(4, 4, 4);

    // _SI_ = VGA_GetTextHeight(166, GUI_Dialog_Text@)

    // Set_Page_Off();

    // Set_Window(0, 0, 319, (GUI_Dialog_Top + _SI_ + 12);

//     if(GUI_RedMsg_Type == 0)
//     {
// VGA_WndDrawLBXImage(GUI_Dialog_Left, GUI_Dialog_Top, IMG_GUI_RedMessage1@);
//     }
//         
//     else
//     {
// VGA_WndDrawLBXImage(GUI_Dialog_Left, GUI_Dialog_Top, IMG_GUI_RedMessage2@);
//     }

    // Reset_Window();

//     if(GUI_RedMsg_Type == 0)
//     {
// Set_Font_Colors_15(4, COL_Warning1);
//     }
//         
//     else
//     {
// Set_Font_Colors_15(4, COL_Warning2);
//     }

    // Set_Font(4, 15, 15);

// mov     ax, 2
// push    ax                              ; Align
// push    [GUI_Dialog_Text@]              ; Text@
// mov     ax, 166
// push    ax                              ; Width
// mov     ax, [GUI_Dialog_Top]
// add     ax, 11
// push    ax                              ; Top
// mov     ax, [GUI_Dialog_Left]
// add     ax, 11
// push    ax                              ; Left
// call    Print_Paragraph   

// mov     ax, 2
// push    ax                              ; Align
// push    [GUI_Dialog_Text@]              ; Text@
// mov     ax, 166
// push    ax                              ; Width
// mov     ax, [GUI_Dialog_Top]
// add     ax, 11
// push    ax                              ; Top
// mov     ax, [GUI_Dialog_Left]
// add     ax, 10
// push    ax                              ; Left
// call    Print_Paragraph                 ; drake178: VGA_DrawText

    // Set_Alias_Color(184);

    // Set_Font(4, 4, 4);

// mov     ax, 2
// push    ax                              ; Align
// push    [GUI_Dialog_Text@]              ; Text@
// mov     ax, 166
// push    ax                              ; Width
// mov     ax, [GUI_Dialog_Top]
// add     ax, 10
// push    ax                              ; Top
// mov     ax, [GUI_Dialog_Left]
// add     ax, 10
// push    ax                              ; Left
// call    Print_Paragraph                 ; drake178: VGA_DrawText

//     if(GUI_RedMsg_Type == 0)
//     {
//     FLIC_Draw(GUI_Dialog_Left, (GUI_Dialog_Top + _SI_ + 10), IMG_GUI_RedMsg1Btm@);
//     }
//         
//     else
//     {
//     FLIC_Draw(GUI_Dialog_Left, (GUI_Dialog_Top + _SI_ + 10), IMG_GUI_RedMsg2Btm@);
//     }

}

// WZD o149p07
// GUI_ListSelect_Dlg  
// WZD o149p08
// GUI_DrawListSelector
// WZD o149p09
// GUI_SetListSelBounds
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
// EMM_Sandbox2VGAFILEH
// WZD o149p22
// EMM_VGAFILEH2Sandbox
