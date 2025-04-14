




20250323:
Kilgore's ui_cursor_refresh() updates the on-page display, with mouse movement, but doesn't flip the pages.
Is there a reason it doesn't toggle the pages?
Because, 1oom doesn't use the back-page?





Update what the User sees
...screen/window...
change the palette indices in the video memory, in the video memory page that is being displayed
OR
change the color values in the palette / VGA DAC



change the address for the VGA video page

current_video_page holds the address of VRAM that is drawn to
Set_Page_Off(), Set_Page_On(), etc.



¿ everything that updates the VGA DAC directly ?
BA C8 03                                        mov     dx, e_DAC_WRITE
seg021:006A Apply_Palette         mov     dx, 3C8h                        ; e_DAC_WRITE
seg021:00E6 Cycle_Palette         mov     dx, 3C8h                                     
seg021:014E Cycle_Palette         mov     dx, 3C8h                                     
seg021:020C VGA_ShadeScreen       mov     dx, 3C8h                                     
seg021:027A VGA_ShadeScreen       mov     dx, 3C8h                                     
seg021:0322 VGA_SlideColors__STUB mov     dx, 3C8h                                     
seg021:035C VGA_SlideColors__STUB mov     dx, 3C8h                                     
seg021:03C5 UU_VGA_ColorWave      mov     dx, 3C8h                                     


## current_video_page

XREF:
    Fill()
    Dot()
    Line()
    Multi_Colored_Line()

WZD
XREF:
    UU_VGA_DiagColumns()
    Tint_Fill()
    Gray_Scale_Fill()
    UU_VGA_Columns()
    UU_VGA_ScrambleRect()
    UU_VGA_ScrambleRect()
    DBG_DrawTableCell()
    DBG_DrawTableCell()
    Print_Character()
    Print_Character_No_Alias()
    Print_Clipped_Character()
    PageFlip_GrowOut_CopyScreen()
    VGA_ReadScreenLine()
    Capture_Screen_Block()
    Draw_File_Animation_Frame__WIP()
    Remap_Draw_File_Animation_Frame__NOP()
    Set_Page_On()
    Set_Page_Off()
    Check_Default_Video_Page()
    UU_VGA_CopyToDraw()
    FLIC_Draw_Frame()
    FLIC_Remap_Draw_Frame()
    FLIC_Draw_Frame_EMM()
    FLIC_Draw_Frame_EMM_Remap()
    Clipped_Draw_Frame()
    Clipped_Remap_Draw_Frame__NOP()
    EMS_Clipped_Draw()
    EMS_Clipped_Remap_Draw()
    Draw_Picture_ASM()
    VGA_DrawTexture()
    VGA_DrawTexture_R()

MGC
XREF:
    RP_VGA_DiagColumns()
    VGA_Shade_Rect()
    VGA_Grayscale_Rect()
    RP_VGA_Columns()
    RP_VGA_ScrambleRect()
    RP_VGA_ScrambleRect()
    UU_DBG_DrawTableCell()
    UU_DBG_DrawTableCell()
    Print_Character()
    VGA_DrawChar_NoAA()
    VGA_DrawPartialChar()
    UU_LBX_IMG_CpyDrawFrame()
    VGA_ReadScreenLine()
    UU_VGA_CopyToLBX()
    VGA_FILEH_Draw()
    VGA_FILEH_Draw_R()
    Set_Page_On()
    Set_Page_Off()
    Check_Default_Video_Page()
    UU_VGA_CopyToDraw()
    FLIC_Draw_Frame()
    FLIC_Remap_Draw_Frame()
    FLIC_Draw_EMM()
    FLIC_Draw_Frame_EMM_Remap()
    VGA_DrawPartialImage()
    VGA_DrawPartImage_R()
    VGA_DrawPartEmsImg()
    VGA_DrawPartEmsImg_R()
    Draw_Picture_ASM()
    UU_VGA_DrawTexture()
    UU_VGA_DrawTexture_R()

