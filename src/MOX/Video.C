/*
    Video
    VGA

    WIZARDS.EXE
        seg026
        seg027
        seg028
*/

#include "Video.H"

#include "Mouse.H"
#include "Video2.H"



/*
video_page_buffer
emulates 256KB of IBM-PC VRAM
[4] of 64KB

*/

// WZD dseg:76EE
int16_t draw_page_num = 0;

uint8_t * current_video_page;
// uint8_t * video_page_buffer[4];  // On, Off, Back, ¿ Back2 ?
uint8_t * video_memory;
uint8_t * video_page_buffer[4];  // On, Off, Back, ¿ Back2 ?
uint8_t * draw_page;



/*
    WIZARDS.EXE  seg026
*/

// seg026:000E 02 02 02 01 01 03 00 03 03 02 00 01 03 03 01 03+Mosaic_Rnd256_Bits db 2, 2, 2, 1, 1, 3, 0, 3, 3, 2, 0, 1, 3, 3, 1, 3, 3, 0, 3, 1, 2, 3, 0, 2, 1, 1, 2, 2, 1, 1, 3, 3, 2, 3, 3, 0, 2, 3, 0, 2, 0, 3, 2, 0, 2, 0, 0, 3, 1, 0, 1, 1, 1, 3, 3, 1, 1, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 3, 3, 1, 1, 2, 2, 1, 2, 2, 0, 2, 3, 1, 2, 2, 2, 0, 0
// seg026:000E 03 00 03 01 02 03 00 02 01 01 02 02 01 01 03 03+                                        ; DATA XREF: VGA_MosaicFlip__STUB+CCr ...
// seg026:000E 02 03 03 00 02 03 00 02 00 03 02 00 02 00 00 03+db 0, 1, 1, 0, 2, 0, 3, 2, 2, 1, 0, 3, 1, 2, 3, 0, 0, 1, 0, 2, 2, 3, 2, 1, 1, 2, 2, 0, 3, 0, 0, 2, 0, 0, 3, 3, 1, 2, 1, 0, 0, 1, 3, 1, 1, 1, 0, 0, 2, 3, 1, 0, 3, 3, 0, 1, 0, 0, 0, 1, 3, 1, 3, 3, 2, 2, 3, 2, 0, 1, 1, 0, 3, 0, 2, 0, 2, 1, 1, 3, 1, 2, 1, 3, 1
// seg026:000E 01 00 01 01 01 03 03 01 01 00 00 02 01 01 02 01+db 0, 1, 3, 3, 1, 1, 3, 2, 3, 2, 3, 1, 1, 2, 0, 2, 3, 3, 2, 2, 0, 2, 3, 3, 3, 2, 2, 0, 2, 0, 1, 0, 1, 3, 2, 1, 2, 2, 0, 1, 0, 2, 1, 1, 3, 0, 3, 3, 3, 0, 3, 0, 2, 1, 1, 0, 0, 2, 1, 2, 3, 3, 1, 0, 1, 3, 0, 2, 3, 0, 2, 1, 2, 3, 0, 2, 2, 0, 2, 3, 1, 0, 3, 3, 3
// seg026:000E 00 00 00 03 03 01 01 02 02 01 02 02 00 02 03 01+db 0
// seg026:010E 00 03 1E 12 1C 16 1D 0C 22 2F 1F 20 07 31 2E 0E+Mosaic_Rnd50_Tbl db 0, 3, 1Eh, 12h, 1Ch, 16h, 1Dh, 0Ch, 22h, 2Fh, 1Fh, 20h, 7, 31h, 2Eh, 0Eh, 26h, 2Bh, 23h, 28h, 0Bh, 9, 24h, 4, 21h, 1Ah, 27h, 0Fh, 5, 15h, 2, 10h, 19h, 0Ah, 30h, 0Dh, 18h, 25h, 11h, 13h, 2Ch, 6, 8, 14h, 1, 17h, 29h, 1Bh, 2Dh, 2Ah
// seg026:010E 26 2B 23 28 0B 09 24 04 21 1A 27 0F 05 15 02 10+                                        ; DATA XREF: VGA_MosaicFlip__STUB+7Er ...
// seg026:0140 02 09 1E 26 0D 1B 1F 01 13 08 18 24 25 1A 00 0B+Mosaic_Rnd40_Tbl db 2, 9, 1Eh, 26h, 0Dh, 1Bh, 1Fh, 1, 13h, 8, 18h, 24h, 25h, 1Ah, 0, 0Bh, 17h, 20h, 16h, 0Ch, 10h, 14h, 1Dh, 12h, 1Ch, 7, 0Ah, 23h, 0Eh, 19h, 5, 21h, 0Fh, 15h, 4, 11h, 6, 3, 27h, 22h
// seg026:0140 17 20 16 0C 10 14 1D 12 1C 07 0A 23 0E 19 05 21+                                        ; DATA XREF: VGA_MosaicFlip__STUB+B0r
// seg026:0168 00                                              db    0
// seg026:0169 00                                              db    0
// seg026:016A 00                                              db    0
// seg026:016B 00                                              db    0
// seg026:016C 00 00                                           Mosaic_InitVar dw 0                     ; DATA XREF: VGA_MosaicFlip__STUB+49w ...
// seg026:016E 00 00                                           Mosaic_Rnd40_Index dw 0                 ; DATA XREF: VGA_MosaicFlip__STUB+6Bw ...
// seg026:0170 00 00                                           Mosaic_Init64Times dw 0                 ; DATA XREF: VGA_MosaicFlip__STUB+67w ...
// seg026:0172 00 00                                           Mosaic_Rnd50_Index dw 0                 ; DATA XREF: VGA_MosaicFlip__STUB:loc_1EFD6w ...
// seg026:0174 00 00                                           VGA_ScanLine_Offset dw 0                ; DATA XREF: RP_VGA_CutRight+3Bw ...
// seg026:0176 00 00                                           VGA_Write_Map dw 0                      ; DATA XREF: RP_VGA_CutRight:loc_1F2D5w ...
// seg026:0178 00 00                                           VGA_Read_Map dw 0                       ; DATA XREF: RP_VGA_CutRight+49w ...
// seg026:017A 00 00                                           VGA_SlideInColumn dw 0                  ; DATA XREF: UU_VGA_InterleaveIn+10w ...
// seg026:017C 00 00                                           VGA_Line_Count dw 0                     ; DATA XREF: UU_VGA_CrossSlideOut+14w ...
// seg026:017E 00 00                                           VGA_Start_Offset dw 0                   ; DATA XREF: UU_VGA_CrossSlideOut+Dw ...
// seg026:0180 00 00                                           db 2 dup(0)
// seg026:0182 00 00                                           VGA_Horz_Slope dw 0                     ; DATA XREF: UU_VGA_CopyLine+8Dw ...
// seg026:0184 00 00                                           VGA_Vert_Slope dw 0                     ; DATA XREF: UU_VGA_CopyLine+75w ...
// seg026:0186 00 00                                           VGA_Y_Dir dw 0                          ; DATA XREF: UU_VGA_CopyLine+59w ...
// seg026:0188 00 00                                           VGA_Line_Length dw 0                    ; DATA XREF: UU_VGA_CopyLine+70w ...
// seg026:018A 00 00                                           VGA_Display_Seg dw 0                    ; DATA XREF: VGA_MosaicFlip__STUB+31w ...
// seg026:018C 00 00                                           VGA_Draw_Seg dw 0                       ; DATA XREF: VGA_MosaicFlip__STUB+3Ew ...
// seg026:018E 00 00                                           VGA_Read_Mask dw 0                      ; DATA XREF: UU_VGA_CopyLine+102w ...
// seg026:0190 00 00                                           VGA_Line_Offset dw 0                    ; DATA XREF: VGA_ShiftCopyFromF3+27w ...
// seg026:0192 00 00                                           VGA_Origin_Offset dw 0                  ; DATA XREF: VGA_ShiftCopyFromF3+36w ...

// WZD s26p01
void VGA_MosaicFlip__STUB(void)
{
    int16_t UU_y2;
    int16_t UU_x2;
    int16_t UU_y1;
    int16_t UU_x1;

    GUI_PageFlipPrep();

    UU_x1 = 0;
    UU_x2 = 319;
    UU_y1 = 0;
    UU_y2 = 49;





    Toggle_Pages_No_Draw_Fields();

}


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
/*
prepares for a GUI_PageFlip by saving the data segment and the cursor redraw state, then zeroing out the latter
*/
void GUI_PageFlipPrep(void)
{

// mov     ax, ds
// mov     [GUI_DS_Save], ax

    Save_Mouse_State();

}


// WZD s26p11
/*
exchanges the current draw and display frames by resetting the VGA read start address, and swaps cursors between the two images
requires a prior call to GUI_PageFlipPrep

same as Toggle_Pages(), except doesn't call Draw_Fields()

GUI_PageFlip()
Toggle_Pages_No_Draw_Fields();
*/
void Toggle_Pages_No_Draw_Fields(void)
{
    int16_t mx;  // DNE in Dasm
    int16_t my;  // DNE in Dasm

// mov     ax, seg dseg
// mov     ds, ax
// mov     ax, [GUI_DS_Save]
// mov     ds, ax
    
    mx = Pointer_X();
    my = Pointer_Y();

    Check_Mouse_Shape(mx, my);
    Save_Mouse_Off_Page(mx, my);
    Draw_Mouse_Off_Page(mx, my);

    Page_Flip();

    Restore_Mouse_Off_Page();
    Copy_Mouse_Off_To_Mouse_Back();
    Restore_Mouse_State();

}


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
void Set_Page_On(void)
{

    /*
        ~== VRAM + ((draw_page_num) * 4)  {0xA000, 0xA400}
    */

    current_video_page = video_page_buffer[(draw_page_num)];

    // // current_page_flag = 0;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 1;

}


// WZD s28p02
void Set_Page_Off(void)
{

    /*
        ~== VRAM + ((1 - draw_page_num) * 4)  {0xA000, 0xA400}
    */

    current_video_page = video_page_buffer[(1 - draw_page_num)];

    // // current_page_flag = 1;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 0;

}


// WZD s28p03
void Check_Default_Video_Page(void)
{

}



// WZD s28p04
/*
    switch on/off video memory page
    ...update what is displayer to the user

MS-DOS/IBM-PC: next hardware video refresh reads from different hardware video memory
MS Win: next WM_PAINT event, blit different video back buffer  (manually generate a WM_PAINT event)
SDL2: manually or next SDL_WINDOWEVENT_EXPOSED, blit, copy, and render different video back buffer

*/
void Page_Flip(void)
{

    // MS-DOS & IBM-PC VGA
        // CRT Controller - Address
        // VGA_WaitSync();
    // MS Windows
        // pointer to buffer to render/present
    draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'
    // 1oom/Kilgore  draw_page_num ^= 1;
    draw_page = video_page_buffer[draw_page_num];
    // DOMSDOS  VGA_WaitSync()
    Set_Page_Off();
    // HERE: what was drawn should now be displayed
#ifdef _STU_DOS

#endif
#ifdef _STU_LIN

#endif
#ifdef _STU_MAC

#endif
#ifdef _STU_SDL2
    Platform_Palette_Update();
    Platform_Video_Update();
#endif
#ifdef _STU_WIN
    Pump_Paints();
#endif


}



// WZD s28p05
// MoO2  Copy_Off_To_On_Page()
void Copy_On_To_Off_Page(void)
{

    if(Get_Pointer_Image_Number() != crsr_None)
    {
        Copy_On_To_Off_YM();
    }
    else
    {
        Copy_On_To_Off_NM();
    }

}

// WZD s28p06
/*
    ¿¿¿
        copy 4000 bytes at time, so 1/4 memory plane per itr?
        ¿ related to width of cursor image ?


    ???
*/
void Copy_On_To_Off_YM(void)
{
    int16_t mouse_x;
    int16_t mouse_y;
    uint8_t * src;
    uint8_t * dst;
    int16_t itr;
    int16_t itr_rep_movsb;
    int16_t tmp_mouse_x;
    int16_t tmp_mouse_y;

    Save_Mouse_State();

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();


    src = (uint8_t *)video_page_buffer[draw_page_num];
    dst = (uint8_t *)video_page_buffer[1 - draw_page_num];


    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / (screen_pixel_size / 4)))
    {
        itr_rep_movsb = 0;
        while(itr_rep_movsb++ < (screen_pixel_size / 4))
        {
            *dst++ = *src++;
        }

        Restore_Mouse_Off_Page();

        tmp_mouse_x = Pointer_X();
        tmp_mouse_y = Pointer_Y();
        if( (tmp_mouse_x != mouse_x) || (tmp_mouse_y != mouse_y) )
        {
            Check_Mouse_Shape(tmp_mouse_x, tmp_mouse_y);
            Restore_Mouse_On_Page();
            Save_Mouse_On_Page(tmp_mouse_x, tmp_mouse_y);
            Check_Mouse_Shape(tmp_mouse_x, tmp_mouse_y);
            Draw_Mouse_On_Page(tmp_mouse_x, tmp_mouse_y);
            Set_Pointer_Position(tmp_mouse_x, tmp_mouse_y);
        }

    }

    Restore_Mouse_Off_Page();

    Restore_Mouse_State();

}

// WZD s28p07
// TODO  this should probably just be Copy_On_To_Off(), since it's just like the other Copy_On/Offs
void Copy_On_To_Off_NM(void)
{
    uint8_t * src;
    uint8_t * dst;
    uint16_t itr;

    Save_Mouse_State();

    src = (uint8_t *)video_page_buffer[draw_page_num];
    dst = (uint8_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < screen_pixel_size)
    {
        *dst++ = *src++;
    }

    Restore_Mouse_State();
}

// WZD s28p08
// copy VGA RAM 0xA000 to VGA RAM 0xA800 - frame-buffer 0 to frame-buffer 2
void Copy_Off_To_Back(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[1 - draw_page_num];
    dst = (uint16_t *)video_page_buffer[2];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}

// WZD s28p09
// copy VGA RAM 0xA800 to VGA RAM 0xA000 - frame-buffer 2 to frame-buffer 0
void Copy_Back_To_Off(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[2];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}

// WZD s28p10
// drake178: VGA_SaveDraw_Frame4()
// copy VGA RAM 0xA000 to VGA RAM 0xAC00 - frame-buffer 0 to frame-buffer 3
void Copy_Off_To_Page4(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[1 - draw_page_num];
    dst = (uint16_t *)video_page_buffer[3];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }

}

// WZD s28p11
// drake178: VGA_LoadDraw_Frame4()
// copy VGA RAM 0xAC00 to VGA RAM 0xA000 - frame-buffer 3 to frame-buffer 0
void Copy_Page4_To_Off(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[3];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}
