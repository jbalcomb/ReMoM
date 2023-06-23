MoX - Simtex Game Engine

# Software Mouse Cursor

Per-Screen
clear both screen page buffers [0,1] {on,off}
load palette
draw screen background
draw screen components
if Fst, copy draw page to display page


e.g.,
    Main Menu Screen
        Main_Menu_Screen_Draw()
        Toggle_Pages()


e.g.,
    Main Screen
        ...
        Main_Screen_Draw()
            Set_Page_Off()
            Main_Screen_Draw_Do_Draw()
                Draw(main_background_seg)

        PageFlip_FX(~default)
            Apply_Palette()
                VGA DAC WRITE  *this changes what is displayed!*
            Toggle_Pages()
                get current mouse x,y
                Draw_Fields()
                update current and previous pointer image number and (hotspot) offset
                Save_Mouse_Off_Page(mx,my)
                Draw_Mouse_Off_Page(mx,my)
                Page_Flip()
                Restore_Mouse_Off_Page()
                Copy_Mouse_Off_To_Mouse_Back()

    So, ...
        before Main_Screen_Draw()
            nothing
        after Main_Screen_Draw()
            vbb[1] has full draw
        after PageFlip_FX(~default)
            vbb[1] got mouse cursor drawn
            vbb[1] is now being displayed
            vbb[0] is set to be drawn
            vbb[0] has no mouse cursor image drawn


    Here, ...
        Toggle_Pages()
            Save_Mouse_Off_Page(mx,my)
            Draw_Mouse_Off_Page(mx,my)
            Page_Flip()
        the cursor image should be one the screen





? "CRL", "CRH", "DSP": (C)ursor (R)? (L)ow/(H)-Level (?C/Asm?) (D)raw/(Render) (S)creen (P)age ?




Save_Mouse_Off_Page(mx,my)
Cursor_Save_Bkup[]

? Save_Mouse_On_Page ? AKA CRL_Save_RSP uses Cursor_Save_Main[]



MoO2
Module: mouse2
    uses mouse_x and mouse_y, instead of function parameters
    does not test for cursor image none or ~ have_mouse_save
...
last_on_page_mouse_x = mouse_x
last_on_page_mouse_y = mouse_y
draw_width, draw_height
screen_offset
__off_page_buffer
mouse_background_buffer_pointer
src = screen_offset + __off_page_buffer
dst = mouse_background_buffer_pointer

Elsewhere, ...
mouse_background_buffer[624]
mouse_off_page_buffer[624]
mouse_background_buffer_pointer
mouse_off_page_buffer_pointer
__back_page_buffer      allocated in Create_Screen_Buffers()  Allocate_Space(size + 46)  screen_pixel_size = screen_pixel_width2 * screen_pixel_height2  (640 * 480)
__off_page_buffer       allocated in Create_Screen_Buffers()  Allocate_Space(size + 46)
last_off_page_mouse_x
last_off_page_mouse_y
last_on_page_mouse_y
last_on_page_mouse_x
Init_Mouse_Driver()
mouse_background_buffer_pointer = mouse_background_buffer[] ; 624 bytes (24 * 26)
  mouse_off_page_buffer_pointer =   mouse_off_page_buffer[] ; 624 bytes (24 * 26)
OON XREF to mouse_off_page_buffer_pointer
mouse_off_page_buffer XREF'd by Save_Mouse_From_Off_Page() and Restore_Mouse_From_Off_Page

cseg01:0012419D Save_Mouse_From_Off_Page proc near
cseg01:00145719 Save_Mouse_Off_Page_ proc near

Save_Mouse_From_Off_Page()
    uses __off_page_buffer


cseg01:0012439D
Draw_Mouse_Hi_Res() |-> Draw_Mouse_Buffer_()

cseg01:0014529D
Draw_Mouse_Buffer_()
    src = mouse_image_asm
    dst = 0xA0000 + screen_offset
cseg01:0014595F
Draw_Mouse_Off_Page_Buffer_()
    src = mouse_image_asm
    dst = __off_page_buffer + screen_offset






Save, Restore, Draw
WZD  seg035
MGC  seg033
p30  AKA CRL_Save_RSP();
p31  AKA Save_Mouse_Off_Page();  CRL_Save_DSP();
p32  AKA IN_CRL_Save_RSP();  VGA_SaveCursorArea();
void Save_Mouse(int16_t x, int16_t y)

? VGA_DisplayCursor() VGA_DrawCursor() ?
