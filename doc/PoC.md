
# Proof of Concept (PoC)

1. 320x200 VGA
2. 320x200 VGA |-> 640x400 XBGR
3. 640x400 XBGR

! convert 320x200 VGA to 640x400 XBGR
convert 320x200 VGA to 320x200 XBGR
convert 320x200 XBGR to 640x400 XBGR



## PoC Mode  ~== 

    PoC Mode 1:
        MoX draws to video_page_buffer
        convert, on-the-fly, from video_page_buffer[draw_page_num], using g_Palette_XBGR, to Buffer->Memory, which is allocated by the PFL
        PFL Buffer width, height, allocation size based on ... ¿ screen, window, WndDims ? depends on yay-stretch/nay-stretch
        PFL VBB 320*200*4
        Yay-Stretch
    PoC Mode 2:
        MoX draws to video_page_buffer
        pre-convert 320x200xVGA to 320x200xXBGR
        PFL VBB 320*200*4
        Nay-Stretch
    PoC Mode 3:
        MoX draws to video_page_buffer
        pre-convert 320x200xVGA to 320x200xXBGR
        pre-convert 320x200xXBGR to 640x400xXBGR
        PFL VBB 640*400*4
        Nay-Stretch
    PoC Mode 4:
        MoX draws to video_page_buffer_XBGR
        video_page_buffer_XBGR is assigned to PFL VBB
        PFL Update_Window_Display() uses video_page_buffer_XBGR
        *** can not use native draw functions ***  


Init_Platform()
    |->
        Init_Video_Back_Buffer()
        Init_Window_Back_Buffer()
Update_Window_Display()
Update_Window_Display_Pre()
Update_Window_Display_Pre_x2()



## 1. 320x200 VGA
native VBB and native drawing functions
Update_Window_Display() converts `320x200 VGA` to `320x200 XBGR` MS-DIB
StretchDIBits() stretches to `640x400`
Req'd: native VBB, width, height; MS-DIB VBB


## 2. 320x200 VGA |-> 640x400 XBGR
native VBB and native drawing functions
Update_Window_Display() converts `320x200 VGA` to `640x400 XBGR` MS-DIB
StretchDIBits() does not stretch


## 3. 640x400 XBGR
new drawing functions
//1oom: lbxgfx_draw_pixels_fmt0_scale()
¿ this would have to include fixing up all of the allocation - 4 * bytes ?
No? that's part of the next stage? where all the images are pre-upscaled/native x2 XBGR/RGBA?
