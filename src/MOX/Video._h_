/*
    Video
    VGA

    WIZARDS.EXE
        seg026
        seg027
        seg028
*/

#ifndef VIDEO_H
#define VIDEO_H

#include "MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



extern int16_t draw_page_num;
extern uint8_t * current_video_page;
// extern uint8_t * video_page_buffer[];
extern uint8_t * video_memory;
extern uint8_t * video_page_buffer[];
extern uint8_t * draw_page;



/*
    WIZARDS.EXE  seg026
*/


// WZD s26p01
void VGA_MosaicFlip__STUB(void);

// WZD s26p02
// UU_VGA_SlowMosaicFlip()

// WZD s26p03
// RP_VGA_CutRight()

// WZD s26p04
// UU_VGA_InterleaveOut()

// WZD s26p05
// UU_VGA_InterleaveIn()

// WZD s26p06
// UU_VGA_CrossSlideOut()

// WZD s26p07
// UU_VGA_CrossSlideIn()

// WZD s26p08
// UU_VGA_SplitCutOut()

// WZD s26p09
// UU_VGA_CopyLine()

// WZD s26p10
void GUI_PageFlipPrep(void);

// WZD s26p11
void Toggle_Pages_No_Draw_Fields(void);

// WZD s26p12
// UU_VGA_ColWipeLeft()

// WZD s26p13
// UU_VGA_ColWipeRight()

// WZD s26p14
// VGA_ShiftCopyFromF3()



/*
    WIZARDS.EXE  seg027
*/

// WZD s27p01
// VGA_PartCopyFromF3()

// WZD s27p02
// UU_VGA_SliceFlip()

// WZD s27p03
// UU_VGA_SquaresFlip()

// WZD s27p04
// UU_VGA_CopyRect()

// WZD s27p05
// UU_VGA_InterleavedFlip()

// WZD s27p06
// UU_VGA_CrossSlideFlip()

// WZD s27p07
// UU_VGA_WipeLeft()

// WZD s27p08
// UU_VGA_WipeRight()



/*
    WIZARDS.EXE  seg028
*/

// Set_Page_On()
// Set_Page_Off()
// Check_Default_Video_Page()
// Page_Flip()
// Copy_On_To_Off_Page()
// Copy_On_To_Off_YM()
// Copy_On_To_Off_NM()
// Copy_Off_To_Back()
// Copy_Back_To_Off()
// Copy_Off_To_Page4()
// Copy_Page4_To_Off()
// UU_VGA_CopyToDraw()

// WZD s28p01
void Set_Page_On(void);

// WZD s28p02
void Set_Page_Off(void);

// WZD s28p03
void Check_Default_Video_Page(void);

// WZD s28p04
/* PLATFORM */  void Page_Flip(void);

// WZD s28p05
// MoO2  Copy_On_To_Off_Page()
void Copy_On_To_Off_Page(void);

// WZD s28p06
void Copy_On_To_Off_YM(void);

// WZD s28p07
void Copy_On_To_Off_NM(void);

// WZD s28p08
void Copy_Off_To_Back(void);

// WZD s28p09
void Copy_Back_To_Off(void);

// WZD s28p10
// drake178: Copy_Off_To_Page4()
// copy VGA RAM 0xA000 to VGA RAM 0xAC00 - frame-buffer 0 to frame-buffer 3
void Copy_Off_To_Page4(void);

// WZD s28p11
// drake178: VGA_LoadDraw_Frame4()
// copy VGA RAM 0xAC00 to VGA RAM 0xA000 - frame-buffer 3 to frame-buffer 0
void Copy_Page4_To_Off(void);



#ifdef __cplusplus
}
#endif

#endif  /* VIDEO_H */
