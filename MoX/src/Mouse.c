
#include "Mouse.h"

#include "Fonts.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"
#include "Video.h"

#include "../../platform/include/Platform.h"

#include <assert.h>  /* assert() */



// int16_t example_mouse_list_count = 2;
// struct s_mouse_list example_mouse_list[2] = {
//     {0, 0, 0, 0, SCREEN_XMAX, SCREEN_YMAX},
//     {3, 1, 32, 20, 158, 100}
// };

// int16_t mouse_list_none_count = 1;
struct s_mouse_list mouse_list_none[1] = {
    { crsr_None, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// int16_t mouse_list_default_count = 1;
// struct s_mouse_list mouse_list_default[1] = {
//     {crsr_Finger, 0, 0, 0, SCREEN_XMAX, SCREEN_YMAX}
// };

// TODO  _mouse_list_arrow
// struct s_mouse_list mouse_list_arrow[1] = {
//     {crsr_Ranged, 0, 0, 0, SCREEN_XMAX, SCREEN_YMAX}
// };

struct s_mouse_list mouse_list_default[1] = {
    { crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};



/*
    BEGIN: Mouse - Initialized Data
*/

// WZD dseg:78B8
int16_t current_pointer_image_number = 0;

// WZD dseg:78BA
int16_t previous_pointer_image_number = 0;

// WZD dseg:78BC
int16_t mouse_enabled = ST_FALSE;

// WZD dseg:78BE
// MoO2: mouse_driver_installed
int16_t mouse_driver_installed = ST_FALSE;

// WZD dseg:78C0
// drake178: MOUSE_Usable
/* OGBUG  written only by Set_Mouse_Usable() (from Init_Mouse_Keyboard()); its only reader, UU_MOUSE_Usable(), is called nowhere in WIZARDS.EXE or MAGIC.EXE */
int16_t mouse_usable = ST_FALSE;

// WZD dseg:78C2
// MoO2  Module: video  signed integer (2 bytes) mouse_x  Address: 02 : 001B3A38
/*
    ~== pomter_x
    (only?) used for the pointer  (AKA cursor image)
*/
// MoO2  Module: video  signed integer (2 bytes) mouse_y  Address: 02 : 001B3A36
// /int16_t g_mouse_x = 158;
int16_t pointer_x = 158;
// WZD dseg:78C4
/*
    ~== pomter_y
    (only?) used for the pointer  (AKA cursor image)
*/
// int16_t g_mouse_y = 100;
int16_t pointer_y = 100;

// WZD dseg:78C6
int16_t current_mouse_list_count = 1;

// WZD dseg:78C8
// DNE in MoO2 uses s_mouse_list.center_offset
int16_t current_pointer_offset = 0;

// WZD dseg:78CA
int16_t mouse_buffer_flag = 0;
// WZD dseg:78CC
int16_t mouse_buffer_x = 0;
// WZD dseg:78CE
int16_t mouse_buffer_y = 0;
// WZD dseg:78D0
int16_t mouse_buffer_button = 0;
// WZD dseg:78D2
int16_t mouse_buffer_flag2 = 0;

// WZD dseg:78D4
int16_t mouse_interrupt_active = ST_FALSE;

// WZD dseg:78D6
int16_t mouse_save_flag = ST_FALSE;

// WZD dseg:78D8
// int16_t init_mouse_x = 158;
int16_t init_pointer_x = 158;
// WZD dseg:78DA
// int16_t init_mouse_y = 100;
int16_t init_pointer_y = 100;

// WZD dseg:78DC
// drake178: MOUSE_CDraw_Save
int16_t mouse_state = ST_FALSE;

// WZD dseg:78DE
// HACK WTF gcc says multiple definition, between Mouse.c and SCastScr.c
struct s_mouse_list * _current_mouse_list;

// WZD dseg:78E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Cursor_Save_Main Cursor_Area_Copy <0>
// WZD dseg:7A66 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+REUSABLE_810bytes db 32Ah dup(0)
// WZD dseg:7D90 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Cursor_Save_Bkup Cursor_Area_Copy <0>
// WZD dseg:7F16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+REUSABLE_810bytes_2 db 32Ah dup(0)
// WZD dseg:78E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mouse_background_buffer dw 600 dup(0)
// WZD dseg:7D90 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Cursor_Save_Bkup dw 600 dup(0)
// MoO2: mouse_background_buffer_pointer, offset mouse_background_buffer ; 624 bytes (24 * 26)
// MoO2: mouse_off_page_buffer_pointer, offset mouse_off_page_buffer ; 624 bytes (24 * 26)
// drake178: Cursor_Save_Main
uint16_t mouse_background_buffer[600];
// drake178: Cursor_Save_Bkup
uint16_t mouse_off_page_buffer[600];


// ...
/* END: Initialized Data [Data Segment - DSEG]*/





/*
    MAGIC.EXE    seg033
    WIZARDS.EXE  seg035
*/


// WZD s35p01
void Set_Mouse_List(int16_t count, struct s_mouse_list * list)
{
    int tmp_count;
    struct s_mouse_list * tmp_list;

    tmp_list = list;
    tmp_count = count;

    current_mouse_list_count = count;
    _current_mouse_list = list;

    tmp_count--;
    if(tmp_count != 0)
    {
        *tmp_list = list[tmp_count];
        do
        {
            if((tmp_list->x1 == 0) && (tmp_list->y1 == 0))
            {
                break;
            }
            *tmp_list = list[tmp_count];
            tmp_list--;
            tmp_count--;
        } while(tmp_count != 0);

    }

    current_pointer_offset = list[tmp_count].center_offset;
    current_pointer_image_number = list[tmp_count].image_num;

}


// WZD s35p02
// drake178: GUI_FindWindow()
void Check_Mouse_Shape(int16_t l_mx, int16_t l_my)
{
    int16_t count;
    int16_t list_index;

    previous_pointer_image_number = current_pointer_image_number;

    list_index = 0;
    count = current_mouse_list_count;

    while(--count)
    {
        if(
            (l_mx >= _current_mouse_list[count].x1)
            &&
            (l_my >= _current_mouse_list[count].y1)
            &&
            (_current_mouse_list[count].x2 >= l_mx)
            &&
            (_current_mouse_list[count].y2 >= l_my)
        )
        {
            list_index = count;
        }
    }

    current_pointer_offset = _current_mouse_list[list_index].center_offset;
    current_pointer_image_number = _current_mouse_list[list_index].image_num;

}

// WZD s35p03
int16_t Get_Pointer_Image_Number(void)
{
    return current_pointer_image_number;
}

// WZD s35p04
int16_t Get_Pointer_Offset(void)
{
    return current_pointer_offset;
}

// WZD s35p05
/*
the current button state is returned as a button bitmask,
  which can be tested using the SDL_BUTTON(X) macros
  (where X is generally 1 for the left, 2 for middle, 3 for the right button)
x and y are set to the mouse cursor position relative to the focus window
*/
#if defined(__DOS16__)
/* CLAUDE */  /* the OG MS-DOS body (seg035 is hand-written asm: no bp frame, result left in AX); Borland inline asm, never compiled in a ReMoM build */
int16_t Mouse_Button(void)
{
    asm push es
    asm push ds
    asm mov ax, seg mouse_driver_installed
    asm mov ds, ax
    asm cmp word ptr [mouse_driver_installed], ST_FALSE
    /* OGBUG  with no mouse driver this returns AX = the DGROUP segment ('buttons held', nonzero) instead of 0; every live caller tests for a mouse first, and the one that does not (Wait_Ticks_Or_Input) has no callers */
    asm jz short Done
    asm mov ax, 3
    asm mov bx, 0
    asm mov cx, 0
    asm mov dx, 0
    asm cli
    asm int 33h
    asm sti
    asm and bx, 11b
    asm mov ax, bx
Done:
    asm pop ds
    asm pop es
}
#else
/* CLAUDE */  /* platform replacement for the OG MS-DOS body above */
int16_t Mouse_Button(void)
{
    Platform_Event_Handler();

    return Platform_Get_Mouse_Button_State();
}
#endif

// WZD s35p06
void Mouse_Wait(void)
{
// mov     ax, 3
// mov     bx, 0
// mov     cx, 0
// mov     dx, 0
// cli
// int     33h                             ; - MS MOUSE - RETURN POSITION AND BUTTON STATUS
//                                         ; Return: BX = button status, CX = column, DX = row
// sti
// and     bx, 3
// return _AX_
}


// WZD s35p07
int16_t Init_Mouse_Driver(void)
{
    // INT 33, 0
    // if _AX != 0x000
    // INT 33, 3
    mouse_driver_installed = ST_TRUE;
    mouse_interrupt_active = ST_FALSE;

    pointer_x = init_pointer_x;
    pointer_y = init_pointer_y;

    // INT 33, 7  638
    // INT 33, 8  199

    Mouse_Wait();

    // INT 33, 12  User_Mouse_Handler

    mouse_enabled = ST_FALSE;

    mouse_buffer_flag = ST_FALSE;
    mouse_buffer_flag2 = ST_FALSE;

    return ST_SUCCESS;
}


// WZD s35p08
// drake178: RP_MOUSE_SetUsable()
void Set_Mouse_Usable(void)
{
    mouse_usable = ST_TRUE;
}


// WZD s35p09
void Reset_System_Mouse(void)
{
    if(mouse_driver_installed)
    {
        Mouse_Wait();
        mouse_enabled = ST_FALSE;  // DNE in MoO2
        mouse_buffer_flag = ST_FALSE;
        mouse_buffer_flag2 = ST_FALSE;
        // MoO2  old_mouse_x = Pointer_X();
        // MoO2  old_mouse_y = Pointer_Y();
        init_pointer_x = Pointer_X();
        init_pointer_y = Pointer_Y();

// cli  AKA _disable_()
// mov     ax, 21h
// int     33h                             ; - MS MOUSE - SOFTWARE RESET
//                                         ; Return: AX = FFFFh if mouse driver installed
//                                         ; AX = 0021h if mouse driver not installed
//                                         ; BX = 2 if mouse driver is installed
// sti  AKA _enable_()

    }
    mouse_driver_installed = ST_FALSE;
}


// WZD s35p10
void Mouse_Movement_Handler(void)
{
    lock_mouse_button_status_flag = ST_TRUE;
}


// WZD s35p11
void Mouse_Button_Handler(void)
{
    // ITRY  platform_mouse_button_status = 0;
    lock_mouse_button_status_flag = ST_FALSE;
}


// WZD s35p12
// drake178: MOUSE_INT_Handler()
// MoO2  Module: mouse  User_Mouse_Handler()
/* CLAUDE: replaced hardcoded /2 and *2 with dynamic scale from window/screen dimensions */
void User_Mouse_Handler(int16_t buttons, int16_t l_mx, int16_t l_my)
{
    /* OG: int16_t scale = 2; */
    /* BEGIN:   CLAUDE */
    // float screen_scale = (float)Platform_Get_Window_Width() / (float)SCREEN_WIDTH;
    // int16_t gx = (int16_t)(l_mx / screen_scale);
    // int16_t gy = (int16_t)(l_my / screen_scale);
    int16_t gx = 0;
    int16_t gy = 0;
    int16_t on_image = Platform_Window_To_Game(l_mx, l_my, &gx, &gy);  /* the platform maps window -> game through where it shows the image (letterbox, high-DPI); PLAN-Fullscreen.md phase 1 */
    /* END:     CLAUDE */

    // if(l_mx < SCREEN_XMIN || l_my < SCREEN_YMIN || gx > SCREEN_XMAX || gy > SCREEN_YMAX)
    /* CLAUDE */ if(on_image == ST_FALSE)  /* the same off-screen test, done by the platform, which also knows about the black bars */
    {
#ifdef MOUSE_DEBUG
        MOUSE_LOG("MOUSEt=%llu UMH_OOB btn=%d wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), buttons, l_mx, l_my, gx, gy);
#endif
        return;
    }
#ifdef MOUSE_DEBUG
    // MOUSE_LOG("MOUSEt=%llu UMH btn=%d wx=%d wy=%d gx=%d gy=%d scale=%.2f enabled=%d interrupt=%d\n", (unsigned long long)Platform_Get_Millies(), buttons, l_mx, l_my, gx, gy, screen_scale, mouse_enabled, mouse_interrupt_active);
    /* CLAUDE */ MOUSE_LOG("MOUSEt=%llu UMH btn=%d wx=%d wy=%d gx=%d gy=%d scale=%d enabled=%d interrupt=%d\n", (unsigned long long)Platform_Get_Millies(), buttons, l_mx, l_my, gx, gy, Platform_Get_Scale(), mouse_enabled, mouse_interrupt_active);
#endif
    pointer_x = gx;
    pointer_y = gy;
    // ITRY  platform_mouse_button_status = buttons;
    if(mouse_interrupt_active == ST_FALSE)
    {
        mouse_interrupt_active = ST_TRUE;
        Check_Mouse_Buffer(gx, gy, buttons);
        if(mouse_enabled == ST_TRUE)
        {
            mouse_enabled = ST_FALSE;
            if(current_mouse_list_count >= 2)
            {
                Check_Mouse_Shape(gx, gy);
            }
            Restore_Mouse_On_Page();                     // mouse_background_buffer           ->  video_page_buffer[draw_page_num]
            Save_Mouse_On_Page(gx, gy);                  // video_page_buffer[draw_page_num]  ->  mouse_background_buffer
            Draw_Mouse_On_Page(gx, gy);                  // mouse_palette                     ->  video_page_buffer[draw_page_num]
            mouse_enabled = ST_TRUE;
        }
        mouse_interrupt_active = ST_FALSE;
    }
}


// WZD s35p13
void Save_Mouse_State(void)
{
    mouse_state = mouse_enabled;
    mouse_enabled = ST_FALSE;
}

// WZD s35p14
void Restore_Mouse_State(void)
{
    mouse_enabled = mouse_state;
}

// WZD s35p15
void UU_MOUSE_Usable(void)
{
    // TODO
}

// WZD s35p16
// Keyboard.c  int16_t Keyboard_Status(void)

// WZD s35p17
// Keyboard.c  uint8_t Read_Key(void)

// WZD s35p18
int16_t Pointer_X(void)
{
    assert(pointer_x <= SCREEN_XMAX);
    return pointer_x;
}

// WZD s35p19
int16_t Pointer_Y(void)
{
    assert(pointer_y <= SCREEN_YMAX);
    return pointer_y;
}

// WZD s35p20
// MD_MoveCursor
void Set_Pointer_Position(int16_t l_mx, int16_t l_my)
{
    pointer_x = l_mx;
    pointer_y = l_my;

    if(mouse_driver_installed)
    {
        Set_Mouse_Position(l_mx, l_my);
    }
}

// WZD s35p21
void Set_Mouse_Position(int16_t mx, int16_t my)
{
    // asm mov cx, [bp+x]
    // asm shl cx, 1
    // asm mov dx, [bp+y]
    // asm mov ax, 4
    // asm cli
    // asm int 33h
    // asm sti
    Platform_Warp_Mouse(mx, my);
}


// WZD s35p22
int16_t Mouse_Buffer_X(void)
{
    return mouse_buffer_x;
}


// WZD s35p23
int16_t Mouse_Buffer_Y(void)
{
    return mouse_buffer_y;
}


// WZD s35p24
int16_t Mouse_Buffer_Button(void)
{
    return mouse_buffer_button;
}


// WZD s35p25
int16_t Mouse_Buffer(void)
{
    int16_t value;
    value = mouse_buffer_flag;
    mouse_buffer_flag = ST_FALSE;
    return value;
}


// WZD s35p26
int16_t Mouse_Buffer2(void)
{
    int16_t value;
    value = mouse_buffer_flag2;
    mouse_buffer_flag2 = ST_FALSE;
    return value;
}

// WZD s35p27
void Set_Buffer_2(int16_t x, int16_t y)
{
    mouse_buffer_flag2 = ST_TRUE;
    mouse_buffer_x = x;
    mouse_buffer_y = y;
}


// WZD s35p28
void Set_Mouse_Buffer(int16_t x, int16_t y, int16_t buttons)
{
    mouse_buffer_flag2 = ST_TRUE;
    mouse_buffer_flag = ST_TRUE;
    mouse_buffer_x = x;
    mouse_buffer_y = y;
    mouse_buffer_button = buttons;
}


// WZD s35p29
void Check_Mouse_Buffer(int16_t x, int16_t y, int16_t buttons)
{
    // 1oom
    // buttons &= (MOUSE_BUTTON_MASK_LEFT | MOUSE_BUTTON_MASK_RIGHT);
    // mouse_buttons = buttons;
    // if(buttons) {
    // 1oom:  buttons &= (MOUSE_BUTTON_MASK_LEFT | MOUSE_BUTTON_MASK_RIGHT);
    buttons &= (MOUSE_BUTTON_MASK_LEFT | MOUSE_BUTTON_MASK_RIGHT);
    if(buttons != 0)
    {
#ifdef MOUSE_DEBUG
        MOUSE_LOG("MOUSEt=%llu BUFFERED x=%d y=%d btn=%d\n", (unsigned long long)Platform_Get_Millies(), x, y, buttons);
#endif
        mouse_buffer_button = buttons;
        mouse_buffer_flag = ST_TRUE;
        mouse_buffer_flag2 = ST_TRUE;
        mouse_buffer_x = x;
        mouse_buffer_y = y;
    }
}




// ? DSP & RSP ? Front & Back || On & Off || "draw_page_num" vs. "current_page_flag" {1,2,3} ?


// WZD s35p30
// AKA CRL_Save_RSP()
/*
vs. WZD s35p31  AKA Save_Mouse_Off_Page();  CRL_Save_DSP();
vs. WZD s35p32  AKA IN_CRL_Save_RSP();  VGA_SaveCursorArea();
*/
void Save_Mouse_On_Page(int16_t x, int16_t y)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    /* OGBUG  nothing ever resets mouse_save_flag: with no pointer image, only the first call saves the area under the cursor */
    if(current_pointer_image_number == 0)
    {
        if(mouse_save_flag != ST_FALSE)
        {
            return;
        }
        mouse_save_flag = ST_TRUE;
    }
    // clip the cursor image to the right and the bottom of the screen
    if(x + CURSOR_WIDTH  < SCREEN_WIDTH ) { width  = CURSOR_WIDTH ; } else { width  = SCREEN_WIDTH  - x; }
    if(y + CURSOR_HEIGHT < SCREEN_HEIGHT) { height = CURSOR_HEIGHT; } else { height = SCREEN_HEIGHT - y; }

    screen_page_offset = ((y * SCREEN_WIDTH) + x);

    mouse_buffer = &mouse_off_page_buffer[0];
    *mouse_buffer++ = screen_page_offset;
    *mouse_buffer++ = width;
    *mouse_buffer++ = height;

    screen_page = video_page_buffer[draw_page_num] + screen_page_offset;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *mouse_buffer++ = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
        }
    }

}


// WZD s35p31
void Save_Mouse_Off_Page(int16_t x, int16_t y)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    if(
        (current_pointer_image_number == 0)
        &&
        (previous_pointer_image_number == 0)
    )
    {
        if(mouse_save_flag != ST_FALSE)
        {
            return;
        }
        mouse_save_flag = ST_TRUE;
    }

    // clip the cursor image to the right and the bottom of the screen
    if(x + CURSOR_WIDTH  < SCREEN_WIDTH ) { width  = CURSOR_WIDTH ; } else { width  = SCREEN_WIDTH  - x; }
    if(y + CURSOR_HEIGHT < SCREEN_HEIGHT) { height = CURSOR_HEIGHT; } else { height = SCREEN_HEIGHT - y; }

    screen_page_offset = ((y * SCREEN_WIDTH) + x);

    mouse_buffer = &mouse_off_page_buffer[0];
    *mouse_buffer++ = screen_page_offset;
    *mouse_buffer++ = width;
    *mouse_buffer++ = height;

    screen_page = video_page_buffer[1 - draw_page_num] + screen_page_offset;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *mouse_buffer++ = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
        }
    }

}


// WZD s35p32
void Save_Mouse_On_Page_(int16_t x, int16_t y)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    // clip the cursor image to the right and the bottom of the screen
    if(x + CURSOR_WIDTH  < SCREEN_WIDTH ) { width  = CURSOR_WIDTH ; } else { width  = SCREEN_WIDTH  - x; }
    if(y + CURSOR_HEIGHT < SCREEN_HEIGHT) { height = CURSOR_HEIGHT; } else { height = SCREEN_HEIGHT - y; }

    screen_page_offset = ((y * SCREEN_WIDTH) + x);

    mouse_buffer = &mouse_off_page_buffer[0];
    *mouse_buffer++ = screen_page_offset;
    *mouse_buffer++ = width;
    *mouse_buffer++ = height;

    screen_page = video_page_buffer[draw_page_num] + screen_page_offset;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *mouse_buffer++ = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
        }
    }

}


// WZD s35p33
void Copy_Mouse_Off_To_Mouse_Back(void)
{
    uint16_t * src_mouse_buffer;
    uint16_t * dst_mouse_buffer;
    int16_t itr;

    src_mouse_buffer = &mouse_off_page_buffer[0];
    dst_mouse_buffer = &mouse_background_buffer[0];

    for(itr = 0; itr < 600; itr++)
    {
        *dst_mouse_buffer++ = *src_mouse_buffer++;
    }
}


// WZD s35p34
// AKA CRL_Restore_RSP()  VGA_RestoreCursrArea()
/*
vs. WZD s35p35  Restore_Mouse_Off_Page  CRL_Restore_DSP     VGA_RestoreDrawCArea
*/
void Restore_Mouse_On_Page(void)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;
    if(previous_pointer_image_number == crsr_None)
    {
        return;
    }
    mouse_buffer = &mouse_background_buffer[0];
    screen_page_offset = *mouse_buffer++;
    width = *mouse_buffer++;
    height = *mouse_buffer++;
    screen_page = video_page_buffer[draw_page_num] + screen_page_offset;
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = (uint8_t)*mouse_buffer++;
        }
    }
}


// WZD s35p35
void Restore_Mouse_Off_Page(void)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    if(previous_pointer_image_number == crsr_None)
    {
        return;
    }

    /* OGBUG  restores the OTHER page (1 - draw_page_num) from mouse_background_buffer, the buffer Save_Mouse_On_Page fills for the displayed page, while Save_Mouse_Off_Page saves into
       mouse_off_page_buffer; it is right only when Copy_Mouse_Off_To_Mouse_Back (off -> background) ran in between */
    mouse_buffer = &mouse_background_buffer[0];
    screen_page_offset = *mouse_buffer++;
    width = *mouse_buffer++;
    height = *mouse_buffer++;

    screen_page = video_page_buffer[1 - draw_page_num] + screen_page_offset;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = (uint8_t)*mouse_buffer++;
        }
    }

}


// WZD s35p36
void UU_VGA_RestoreToFrame3(void)
{
    // TODO
}

// WZD s35p37  Draw_Mouse_On_Page    CRH_Draw_RSP  GUI_DisplayCursor
void Draw_Mouse_On_Page(int16_t x, int16_t y)
{

    if(current_pointer_image_number != crsr_None)
    {
        Draw_Mouse_On_Page_(x, y);
    }

}


// WZD s35p38  Draw_Mouse_Off_Page   CRH_Draw_DSP  GUI_DrawCursor
void Draw_Mouse_Off_Page(int16_t x, int16_t y)
{

    if(current_pointer_image_number != crsr_None)
    {
        Draw_Mouse_Off_Page_(x, y);
    }

}


// WZD s35p39  Draw_Mouse_On_Page_   CRL_Draw_RSP  VGA_DisplayCursor
void Draw_Mouse_On_Page_(int16_t x, int16_t y)
{
    uint8_t * mouse_image;
    uint8_t * screen_page;
    uint16_t screen_page_offset;
    int16_t width;
    int16_t height;
    int16_t itr_width;
    int16_t itr_height;
    uint8_t pixel;

    /* CLAUDE: HW-cursor prototype -- the OS renders the cursor natively; suppress the software draw so
       there is no second, ~18 fps cursor.  Runtime-gated (REMOM_HW_CURSOR); no-op in the default path. */
    if(Platform_HW_Cursor_Active()) { return; }

    // clip the cursor image to the right and the bottom of the screen
    if(x + CURSOR_WIDTH  < SCREEN_WIDTH ) { width  = CURSOR_WIDTH;  } else { width  = SCREEN_WIDTH  - x; }
    if(y + CURSOR_HEIGHT < SCREEN_HEIGHT) { height = CURSOR_HEIGHT; } else { height = SCREEN_HEIGHT - y; }

    screen_page_offset = ((y * SCREEN_WIDTH) + x);

    mouse_image = mouse_palette + ((current_pointer_image_number - 1) * (CURSOR_WIDTH * CURSOR_HEIGHT));

    screen_page = video_page_buffer[draw_page_num] + screen_page_offset;
    
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            pixel = *(mouse_image + (itr_width * CURSOR_HEIGHT) + itr_height);
            if(pixel != ST_TRANSPARENT)
            {
                *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = pixel;
            }
        }
    }

}


// WZD s35p40  Draw_Mouse_Off_Page_  CRL_Draw_DSP  VGA_DrawCursor
void Draw_Mouse_Off_Page_(int16_t x, int16_t y)
{
    uint8_t * mouse_image;
    uint8_t * screen_page;
    uint16_t screen_page_offset;
    int16_t width;
    int16_t height;
    int16_t itr_width;
    int16_t itr_height;
    uint8_t pixel;

    /* CLAUDE: HW-cursor prototype -- suppress the software draw (see Draw_Mouse_On_Page_). Runtime-gated. */
    if(Platform_HW_Cursor_Active()) { return; }

    // clip the cursor image to the right and the bottom of the screen
    if(x + CURSOR_WIDTH  < SCREEN_WIDTH ) { width  = CURSOR_WIDTH ; } else { width  = SCREEN_WIDTH  - x; }
    if(y + CURSOR_HEIGHT < SCREEN_HEIGHT) { height = CURSOR_HEIGHT; } else { height = SCREEN_HEIGHT - y; }

    screen_page_offset = ((y * SCREEN_WIDTH) + x);

    mouse_image = mouse_palette + ((current_pointer_image_number - 1) * (CURSOR_WIDTH * CURSOR_HEIGHT));

    screen_page = video_page_buffer[1 - draw_page_num] + screen_page_offset;
    
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            pixel = *(mouse_image + (itr_width * CURSOR_HEIGHT) + itr_height);
            if(pixel != ST_TRANSPARENT)
            {
                *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = pixel;
            }
        }
    }

}
