


FLIC_Draw()
    |-> FLIC_Draw_Frame()
    |-> FLIC_Remap_Draw_Frame()

Draw_Picture()
    |-> Draw_Picture_ASM()
    |-> Remap_Draw_Picture_ASM()

Clipped_Draw()
    |-> Clipped_Draw_Frame()
    |-> Clipped_Remap_Draw_Frame()


AKA FLIC_Draw_XY_Wnd()
AKA FLIC_Draw_Windowed()
    |-> VGA_DrawPartialImage()
    |-> VGA_DrawPartImage_R()
    |-> VGA_DrawPartEmsImg()

// WZD s31p01
void Clipped_Draw_Frame(int16_t x1, int16_t y1, int16_t width, SAMB_ptr frame_data);
// WZD s31p02
void Clipped_Remap_Draw_Frame(int16_t x2, int16_t y2, int16_t width, SAMB_ptr frame_data);

¿ RLE ?
¿ ASM ?






¿ MoO2  "Draw" ... "Animated_Sprite" vs. "Picture" vs. "Bitmap" ?



¿¿¿
Frame Type / Full Frame Flag
Remap Colors Flag
¿ loop_frame as flag ?
¿ first byte of FLIC Frame ? ... Grr... I know I have a bunch of notes on this somewhere, from 2+ years ago for the Menu Screen.
???
OON 'getter' Get_Full_Store_Flag(SAMB_ptr p_FLIC_Header)  ... full_store_flag = FLIC_GET_FRAME_TYPE(p_FLIC_Header);
    then Add_Button_Info() ... p_fields[fields_count].Param3 = Get_Full_Store_Flag(pict_seg);


lodsb                                   ; Frame Byte #1: Op/Count


cmp     al, 0FFh                        ; Op = 255 = Skip Column
jz      short @@NextColumn2





MoO2
Module: animate
Module: remap


Draw()
Remap_Draw()
Draw_No_Glass()


Remap_Draw()
    |-> Remap_Draw_Animated_Sprite()
            picture_remap_color_list[]

Module: remap
    array (256 bytes) picture_remap_color_list
    Address: 02:001B479C
    Num elements:  256, Type:        unsigned integer (4 bytes) 





Starting from `cmp  al, 232`, the code is the exact same in FLIC_Remap_Draw_Frame() and Remap_Draw_Picture_ASM()

Remap_Draw_Picture_ASM()

@@Loop_Height:
lodsb
cmp     al, e_ST_TRANSPARENT
jz      short @@Copy_SkipPixel
cmp     al, 232
jnb     short @@Copy_YayRemap
@@Copy_YayRemap:
sub     al, 232
push    si
push    ds
mov     si, seg dseg
mov     ds, si
mov     si, ax
and     si, 0FFh
shl     si, 1
shl     si, 1
shl     si, 1
shl     si, 1
add     si, [remap_color_palettes]
mov     ds, si
mov     al, [_ES_screen_start:_DI_screen_pos]
mov     si, ax
and     si, 0FFh
lodsb
pop     ds
pop     si
stosb
add     _DI_screen_pos, 79
loop    @@Loop_Height

FLIC_Remap_Draw_Frame()

@@ULC_Copy_Loop:
lodsb
cmp     al, 232
jnb     short @@Copy_YayRemap
@@Copy_YayRemap:
sub     al, 232
push    si
push    ds
mov     si, seg dseg
mov     ds, si
mov     si, ax
and     si, 0FFh
shl     si, 1
shl     si, 1
shl     si, 1
shl     si, 1
add     si, [remap_color_palettes]
mov     ds, si
mov     al, [_ES_screen_start:_DI_screen_pos]
mov     si, ax
and     si, 0FFh
lodsb
pop     ds
pop     si
stosb
add     _DI_screen_pos, 79
loop    @@ULC_Copy_Loop
