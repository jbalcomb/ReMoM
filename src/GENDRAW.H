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





// WZD dseg:6F78
extern uint8_t COL_ConfirmShadows[5];
// WZD dseg:6F7D
extern uint8_t COL_Dialog_Text[5];


// WZD dseg:6F96
extern int16_t confirmation_box_x_offset;
// WZD dseg:6F98
extern int16_t confirmation_box_y_offset;
// WZD dseg:6F9A
extern int16_t notify_draw_offscr;



// WZD dseg:CA8E                                                 BEGIN:  ovr149  GENDRAW - Uninitialized Data

// WZD dseg:CA8E 00 00                                           HLP_ExpandLines dw 0                    ; DATA XREF: HLP_DrawExpanding+Bw ...
// WZD dseg:CA90 00 00                                           HLP_ExpandOrigBottom dw 0               ; DATA XREF: HLP_DrawExpanding+3Bw ...
// WZD dseg:CA92 00 00                                           HLP_ExpandOrigRight dw 0                ; DATA XREF: HLP_DrawExpanding+35w ...
// WZD dseg:CA94 00 00                                           HLP_ExpandOrigTop dw 0                  ; DATA XREF: HLP_DrawExpanding+2Fw ...
// WZD dseg:CA96 00 00                                           HLP_ExpandOrigLeft dw 0                 ; DATA XREF: HLP_DrawExpanding+29w ...
// WZD dseg:CA98 00 00                                           HLP_ExpandTgtBottom dw 0                ; DATA XREF: HLP_DrawExpanding+23w ...
// WZD dseg:CA9A 00 00                                           HLP_ExpandTgtRight dw 0                 ; DATA XREF: HLP_DrawExpanding+1Dw ...
// WZD dseg:CA9C 00 00                                           HLP_ExpandTgtTop dw 0                   ; DATA XREF: HLP_DrawExpanding+17w ...
// WZD dseg:CA9E 00 00                                           HLP_ExpandTgtLeft dw 0                  ; DATA XREF: HLP_DrawExpanding+11w ...

// WZD dseg:CAA0                                                 BEGIN:  Selection Box
// WZD dseg:CAA0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+selectbox_fields dw 0Ah dup(0)          ; DATA XREF: Selection_Box+217w ...
// WZD dseg:CAB4 00 00                                           selectbox_title dw 0                    ; DATA XREF: Selection_Box+1CBw ...
// WZD dseg:CAB6 00 00                                           selectbox_list dw 0                     ; DATA XREF: Selection_Box+1C5w ...
// WZD dseg:CAB8 00 00                                           selectbox_items dw 0                    ; DATA XREF: Selection_Box+1BFw ...
// WZD dseg:CABA 00 00                                           selectbox_first_item dw 0               ; DATA XREF: Selection_Box+1CEw ...
// WZD dseg:CABC 00 00                                           selectbox_highlight_item dw 0           ; DATA XREF: Selection_Box+1D4w ...
// WZD dseg:CABE 00 00                                           UU_selectbox dw 0
// WZD dseg:CAC0 00 00                                           selectbox_y2 dw 0                       ; DATA XREF: Selection_Box+441r ...
// WZD dseg:CAC2 00 00                                           selectbox_x2 dw 0                       ; DATA XREF: Selection_Box+2EFr ...
// WZD dseg:CAC4 00 00                                           selectbox_y1 dw 0                       ; DATA XREF: Selection_Box+2DAr ...
// WZD dseg:CAC6 00 00                                           selectbox_x1 dw 0                       ; DATA XREF: Selection_Box+321r ...
// WZD dseg:CAC8 00 00                                           selectbox_multi dw 0                    ; DATA XREF: Selection_Box+1B9w ...
// WZD dseg:CACA 00 00                                           GUI_ListSel_Count dw 0                  ; DATA XREF: Selection_Box+264r ...
// WZD dseg:CACA                                                 END:  Selection Box

// WZD dseg:CACC                                                 BEGIN:  Confirmation Box
// WZD dseg:CACC
extern SAMB_ptr confirmation_button_no_seg;            // ; 2 frame image appended in the LBX_Sandbox_Segment
// WZD dseg:CACE
extern SAMB_ptr confirmation_button_yes_seg;           // ; 2 frame image appended in the LBX_Sandbox_Segment
// WZD dseg:CAD0
extern SAMB_ptr confirmation_background_bottom_seg;    // ; single image appended in the LBX_Sandbox_Segment
// WZD dseg:CAD2
extern SAMB_ptr confirmation_background_top_seg;       // ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:CAD2                                                 END:  Confirmation Box

// WZD dseg:CAD4                                                 BEGIN: Notify
// WZD dseg:CAD4 00 00                                           notify_pict2_x dw 0                     ; DATA XREF: Notify1+11Cw ...
// WZD dseg:CAD6 00 00                                           notify_pict2_y dw 0                     ; DATA XREF: Notify1+122w ...
// WZD dseg:CAD8
extern int16_t notify_pict1_y;
// WZD dseg:CADA
extern int16_t notify_pict1_x;
// WZD dseg:CADC 00 00                                           notify_box_border dw 0                  ; DATA XREF: Notify1+10Aw ...
// WZD dseg:CADE 00 00                                           notify_pict2 dw 0                       ; DATA XREF: Notify1+100w ...
// WZD dseg:CAE0
extern SAMB_ptr notify_pict1;
// WZD dseg:CAFE
// WZD dseg:CAE2 00 00                                           notify_type dw 0                        ; DATA XREF: Notify1+103w ...
// WZD dseg:CAE4 00 00                                           UU_notify dw 0
// WZD dseg:CAE6 00 00 00 00 00 00 00 00                         notify_gem_box_seg dw 4 dup(0)          ; DATA XREF: Notify_Load+81w ...
// WZD dseg:CAEE 00 00 00 00 00 00 00 00                         notify_pict_box_seg dw 4 dup(0)         ; DATA XREF: Notify_Load+6Aw ...
// WZD dseg:CAF6 00 00 00 00 00 00 00 00                         notify_seg dw 4 dup(0)                  ; DATA XREF: Notify_Load+53w ...
extern int16_t notify_color_slide;
// WZD dseg:CB00
extern int16_t notify_color_slide_cycle;
// WZD dseg:CB02 00 00                                           IMG_GUI_RedMsg2Btm@ dw 0                ; DATA XREF: Warn+68w ...
// WZD dseg:CB04 00 00                                           IMG_GUI_RedMessage2@ dw 0               ; DATA XREF: Warn+51w ...
// WZD dseg:CB06 00 00                                           IMG_GUI_RedMsg1Btm@ dw 0                ; DATA XREF: Warn+3Aw ...
// WZD dseg:CB08 00 00                                           IMG_GUI_RedMessage1@ dw 0               ; DATA XREF: Warn+23w ...
// WZD dseg:CB0A 00 00                                           selectbk_scroll_locked_down_seg dw 0    ; DATA XREF: Selection_Box+144w ...
// WZD dseg:CB0C 00 00                                           selectbk_scroll_locked_up_seg dw 0      ; DATA XREF: Selection_Box+12Dw ...
// WZD dseg:CB0E 00 00                                           selectbk_scroll_bottom_seg dw 0         ; DATA XREF: Selection_Box+116w ...
// WZD dseg:CB10 00 00                                           selectbk_scroll_top_seg dw 0            ; DATA XREF: Selection_Box+FFw ...
// WZD dseg:CB12 00 00                                           selectbk_bottom_arrow_seg dw 0          ; DATA XREF: sub_BD8A8+25Aw ...
// WZD dseg:CB14 00 00                                           selectbk_top_arrow_seg dw 0             ; DATA XREF: sub_BD8A8+243w ...
// WZD dseg:CB16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+selectbk_button_22to26_seg dw 0Ah dup(0)
// WZD dseg:CB2A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+selectbk_button_12to16_seg dw 0Ah dup(0)
// WZD dseg:CB3E 00 00                                           selectbd_scroll_seg dw 0                ; DATA XREF: Selection_Box+BAw ...
// WZD dseg:CB40 00 00                                           selectbd_bottom_right_seg dw 0          ; DATA XREF: Selection_Box+A3w ...
// WZD dseg:CB42 00 00                                           selectbd_right_seg dw 0                 ; DATA XREF: Selection_Box+8Cw ...
// WZD dseg:CB44 00 00                                           selectbd_bottom_seg dw 0                ; DATA XREF: Selection_Box+75w ...
// WZD dseg:CB46 00 00                                           selectbd_top_seg dw 0                   ; DATA XREF: Selection_Box+5Ew ...
// WZD dseg:CB48 00 00                                           selectbd_bottom_left_seg dw 0           ; DATA XREF: Selection_Box+47w ...
// WZD dseg:CB4A 00 00                                           selectbd_left_seg dw 0                  ; DATA XREF: Selection_Box+30w ...
// WZD dseg:CB4C 00 00                                           notify_magic_idx dw 0                   ; DATA XREF: Notify1+16w ...

// WZD dseg:CB4E
extern char * message_box_text;
// WZD dseg:CB50
extern int16_t message_box_y;
// WZD dseg:CB52
extern int16_t message_box_x;
// ; determines whether to use the first or the second of the two different warning message backgrounds (both of which are red, but slightly different and use a different font color)
// WZD dseg:CB54 00 00                                           GUI_RedMsg_Type dw 0

// WZD dseg:CB54                                                 END:  ovr149  GENDRAW - Uninitialized Data ; 





// WZD o149p01
// drake178: GUI_Confirm_Dialog()
int16_t Confirmation_Box(char * text_string);

// WZD o149p02
// drake178: GUI_DrawConfrmDialog()
void Confirmation_Box_Draw(void);

// WZD o149p03
// drake178: GUI_WarningType1()
void Warn1(char * msg);

// WZD o149p04
// drake178: GUI_WarningType0()
void Warn0(char * msg);

// WZD o149p05
// drake178: GUI_ShowRedMessage()
void Warn(char * msg);

// WZD o149p06
// drake178: GUI_DrawRedMessage()
void Warn_Draw(void);

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
void Notify1(int16_t Center, int16_t Top, int16_t notify_color, char * Msg, int16_t Gem, SAMB_ptr notiy_pict_seg, int16_t IMGLeft, int16_t IMGTop, SAMB_ptr IMG2, int16_t IMG2Left, int16_t IMG2Top, int16_t CSlide, int16_t BGShade);

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
void HLP_NoResizeExpand__TODO(int16_t TL, int16_t TT, int16_t TR, int16_t TB, int16_t OL, int16_t OT, int16_t OR, int16_t OB, int16_t expand_flag);

// WZD o149p16
void HLP_DrawExpanding__TODO(int16_t TL, int16_t TT, int16_t TR, int16_t TB, int16_t FL, int16_t FT, int16_t FR, int16_t FB, int16_t OL, int16_t OT, int16_t OR, int16_t OB, int16_t expand_flag);

// WZD o149p17
void G_USW_DrawEHLines__TODO(void);

// WZD o149p18
void HLP_DrawExpandLines__TODO(int16_t OL, int16_t OT, int16_t OR, int16_t OB, int16_t TL, int16_t TT, int16_t TR, int16_t TB, int16_t C11, int16_t C12, int16_t C21, int16_t C22, int16_t C31, int16_t C32);

// WZD o149p19
void ITEM_GetViewDrawPos(int16_t OrigX, int16_t OrigY, SAMB_ptr IMG_Seg, int16_t FullAt, int16_t * Left, int16_t * Top);

// WZD o149p20
void ITEM_ResetViewGrow(void);

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
