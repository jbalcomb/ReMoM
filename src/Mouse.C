
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Mouse.H"

#include "Fonts.H"
#include "Input.H"
#include "Video.H"


/*
int16_t example_mouse_list_count = 2;
struct s_mouse_list example_mouse_list[2] = {
    {0, 0, 0, 0, 319, 199},
    {3, 1, 32, 20, 158, 100}
};

int16_t mouse_list_none_count = 1;
struct s_mouse_list mouse_list_none[1] = {
    {0, 0, 0, 0, 319, 199}
};

int16_t mouse_list_default_count = 1;
struct s_mouse_list mouse_list_default[1] = {
    {1, 0, 0, 0, 319, 199}
};
*/
// TODO  _mouse_list_arrow
struct s_mouse_list mouse_list_default[1] = {
    {1, 0, 0, 0, 319, 199}
};


int16_t init_mouse_x = 158;                     // MGC dseg:43AA
int16_t init_mouse_y = 100;                     // MGC dseg:43AC

// DONT  mouse_driver_installed = ST_FALSE;              // MGC dseg:4390

int16_t mouse_enabled = ST_FALSE;               // MGC dseg:438E
int16_t mouse_state = ST_FALSE;                 // MGC dseg:43AE

int16_t mouse_x = 158;
int16_t mouse_y = 100;

// MoM dseg:43B2 00 00 00 00 00 00 00 00+    CR_Save_RSP db 1200 dup(  0)
// MoM dseg:4862 00 00 00 00 00 00 00 00+    CR_Save_DSP db 1200 dup(  0)
// MoO2 mouse_background_buffer_pointer, offset mouse_background_buffer ; 624 bytes (24 * 26)
// MoO2 mouse_off_page_buffer_pointer, offset mouse_off_page_buffer ; 624 bytes (24 * 26)
uint16_t mouse_background_buffer[600];
uint16_t mouse_off_page_buffer[600];


// WZD dseg:78B8
int16_t current_pointer_image_number = 0;
// WZD dseg:78BA
int16_t previous_pointer_image_number = 0;
// WZD dseg:78BC 00 00                   MOUSE_CursorDraw dw 0
// WZD dseg:78BE 00 00                   MOUSE_DriverPresent dw 0
// WZD dseg:78C0 00 00                   MOUSE_Usable dw 0
// WZD dseg:78C2 9E 00                   MOUSE_CurrentX dw 9Eh
// WZD dseg:78C4 64 00                   MOUSE_CurrentY dw 64h
// WZD dseg:78C6
int16_t current_mouse_list_count = 1;
// WZD dseg:78C8
int16_t current_pointer_offset = 0;
// WZD dseg:78CA 00 00                   MOUSE_ClickRec1 dw 0
// WZD dseg:78CC 00 00                   MOUSE_ClickX dw 0
// WZD dseg:78CE 00 00                   MOUSE_ClickY dw 0
// WZD dseg:78D0 00 00                   MOUSE_ClickBtns dw 0
// WZD dseg:78D2 00 00                   MOUSE_ClickRec2 dw 0
// WZD dseg:78D4 00 00                   MOUSE_INT_Process dw 0
// WZD dseg:78D6 00 00                   GUI_HaveAreaSave dw 0
// WZD dseg:78D8 9E 00                   MOUSE_InitX dw 9Eh
// WZD dseg:78DA 64 00                   MOUSE_InitY dw 64h
// WZD dseg:78DC 00 00                   MOUSE_CDraw_Save dw 0
// WZD dseg:78DE
struct s_mouse_list * current_mouse_list;


/* BEGIN: Initialized Data [Data Segment - DSEG]*/
// ...

// WZD dseg:78BC  MOUSE_CursorDraw dw 0
// WZD dseg:78BE
// MGC dseg:4390
// drake178 MOUSE_DriverPresent
// MoO2 mouse_driver_installed
int16_t mouse_driver_installed;
// WZD dseg:78C0  MOUSE_Usable dw 0
// WZD dseg:78C2  MOUSE_CurrentX dw 9Eh
// WZD dseg:78C4  MOUSE_CurrentY dw 64h

// ...
/* END: Initialized Data [Data Segment - DSEG]*/





/*
    MAGIC.EXE    seg033
    WIZARDS.EXE  seg035
*/

// MGC s33p01
// WZD s35p01
void Set_Mouse_List(int16_t count, struct s_mouse_list * list)
{
    int tmp_count;
    struct s_mouse_list * tmp_list;

    tmp_list = list;
    tmp_count = count;

    current_mouse_list_count = count;
    current_mouse_list = list;

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


// MGC s33p02
// void GUI_FindWindow(int X_Pos, int Y_Pos);
void Check_Mouse_Shape(int16_t x, int16_t y)
{
    int16_t count;
    int16_t list_index;

    previous_pointer_image_number = current_pointer_image_number;

    list_index = 0;
    count = current_mouse_list_count;

    while(--count)
    {
        if( (x > current_mouse_list[count].x1) &&
            (x < current_mouse_list[count].x2) &&
            (y > current_mouse_list[count].y1) &&
            (y < current_mouse_list[count].y2) )
        {
            list_index = count;
        }
    }

    current_pointer_offset = current_mouse_list[list_index].center_offset;
    current_pointer_image_number = current_mouse_list[list_index].image_num;

}

// MGC s33p03
int16_t Get_Pointer_Image_Number(void)
{
    return current_pointer_image_number;
}

// MGC s33p04
int16_t Get_Pointer_Offset(void)
{
    return current_pointer_offset;
}

// WZD s35p05
// Platform-Layer int16_t MD_GetButtonStatus(void)

// WZD s35p07
// MGC s33p07
// AKA MD_Init()
int16_t Init_Mouse_Driver(void)
{
    // INT 33, 0
    // if _AX != 0x000
    // INT 33, 3
    mouse_driver_installed = ST_TRUE;
    // mouse_interrupt_active = ST_FALSE;  // AKA g_MD_INT_InProcess

    // INT 33, 7  638
    // INT 33, 8  199

    // ~== MoO2 Set_Pointer_Position
    mouse_x = init_mouse_x;
    mouse_y = init_mouse_y;

    // Mouse_Wait()  AKA MD_WaitRelease()

    // INT 33, C  MD_INT_Handler

    mouse_enabled = ST_FALSE;

    // g_MD_ClickRec1 = 0;  /* ? ST_FALSE ? */
    // g_MD_ClickRec2 = 0;  /* ? ST_FALSE ? */

    return ST_SUCCESS;
}


// MGC s33p13
void Save_Mouse_State(void)
{
    mouse_state = mouse_enabled;
    mouse_enabled = ST_FALSE;
}

// MGC s33p14
void Restore_Mouse_State(void)
{
    mouse_enabled = mouse_state;
}

// MGC s33p18
int16_t Pointer_X(void)
{
    return mouse_x;
}

// MGC s33p19
int16_t Pointer_Y(void)
{
    return mouse_y;
}

// WZD s35p20
// MD_MoveCursor
void Set_Pointer_Position(int16_t x, int16_t y)
{
//     MOUSE_CurrentX = x;
//     MOUSE_CurrentY = y;
// 
//     if(MOUSE_DriverPresent != ST_FALSE)
//     {
//         Set_Mouse_Position(x, y);
//     }
}

// WZD s35p21
// AKA MD_MoveCursorINT() AKA MOUSE_MoveCursorINT()
void Set_Mouse_Position(int16_t x, int16_t y)
{
// asm mov cx, [bp+x]
// asm shl cx, 1
// asm mov dx, [bp+y]
// asm mov ax, 4
// asm cli
// asm int 33h
// asm sti
}



// ? DSP & RSP ? Front & Back || On & Off || "draw_page_num" vs. "current_page_flag" {1,2,3} ?


// MGC s33p30  AKA CRL_Save_RSP();
// MGC s33p31  AKA Save_Mouse_Off_Page();
// MGC s33p32  AKA IN_CRL_Save_RSP();  VGA_SaveCursorArea();
void Save_Mouse(int16_t x, int16_t y)
{
    uint8_t * page_buffer;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    if(x + 16 < 320) { width  = 16; } else { width  = 320 - x; }
    if(y + 16 < 200) { height = 16; } else { height = 200 - y; }

    screen_page_offset = ((y * 320) + x);

    mouse_buffer = &mouse_off_page_buffer[0];
    *mouse_buffer++ = screen_page_offset;
    *mouse_buffer++ = width;
    *mouse_buffer++ = height;

    page_buffer = current_video_page + screen_page_offset;

    itr_height = 0;
    while(itr_height++ < height)
    {
        itr_width = 0;
        while(itr_width++ < width)
        {
            *mouse_buffer++ = *(page_buffer + (itr_height * 320) + itr_width);
        }
    }

}

// MGC s33p33
void Copy_Mouse(void)
{
    uint16_t * src_mouse_buffer;
    uint16_t * dst_mouse_buffer;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Copy_Mouse()\n", __FILE__, __LINE__);
#endif

/*
    mov     si, offset CR_Save_DSP
    mov     di, offset CR_Save_RSP
    mov     cx, 600                 ; 600 words, 1200 bytes
    rep movsw                       ; Move Byte(s) from String to String
*/

    src_mouse_buffer = &mouse_off_page_buffer[0];
    dst_mouse_buffer = &mouse_background_buffer[0];

    for(itr = 0; itr < 600; itr++)
    {
        *dst_mouse_buffer++ = *src_mouse_buffer++;
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Copy_Mouse()\n", __FILE__, __LINE__);
#endif
}

// MGC s33p34
// MGC s33p35
// MGC s33p36
void Restore_Mouse(void)
{
    uint8_t * page_buffer;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    mouse_buffer = &mouse_background_buffer[0];
    screen_page_offset = *mouse_buffer++;
    width = *mouse_buffer++;
    height = *mouse_buffer++;

    page_buffer = current_video_page + screen_page_offset;

    itr_height = 0;
    while(itr_height++ < height)
    {
        itr_width = 0;
        while(itr_width++ < width)
        {
            *(page_buffer + (itr_height * 320) + itr_width) = *mouse_buffer++;
        }
    }

}

// MGC s33p37
// MGC s33p38
// MGC s33p39
// MGC s33p40
void Draw_Mouse(int16_t x, int16_t y)
{
    uint8_t * mouse_image;
    uint8_t * bbuff_pos;
    int16_t width;
    int16_t height;
    int16_t itr_width;
    int16_t itr_height;
    uint8_t pixel;

    // mouse_image_width  =  16
    // mouse_image_height =  16
    // mouse_image_pixels = 256  (100h)
    mouse_image = mouse_palette + ((current_pointer_image_number - 1) * (16 * 16));

    // CURSOR_WIDTH  PIXX
    // CURSOR_HEIGHT PIXY
    if(x + 16 < 320) { width  = 16; } else { width  = 320 - x; }
    if(y + 16 < 200) { height = 16; } else { height = 200 - y; }

    bbuff_pos = current_video_page + ((y * 320) + x);


// int width = 20;   // total width of patch
// int height = 10;  // total height of patch
// int wt;           // temporary width
// while (height-- > 0)
// {
//     wt = width;
//     while (wt-- > 0)
//     {
//         puts("*");
//     }
//     puts("\n");
// }

    itr_height = 0;
    while(itr_height < height)
    {
        itr_width = 0;
        while(itr_width < width)
        {
            pixel = *(mouse_image + (itr_height * 16) + itr_width);
            if(pixel != 0x00)  /* Palette Index 0: TRANSPARENT */
            {
                *(bbuff_pos + (itr_height * 320) + itr_width) = pixel;
            }
            itr_width++;
        }
        itr_height++;
    }
}
