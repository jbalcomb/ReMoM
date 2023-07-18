
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

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



/*
    BEGIN: Mouse - Initialized Data
*/

// WZD dseg:78B8
int16_t current_pointer_image_number = 0;

// WZD dseg:78BA
int16_t previous_pointer_image_number = 0;

// WZD dseg:78BC
// drake178: MOUSE_CursorDraw
int16_t mouse_enabled = ST_FALSE;

// WZD dseg:78BE
// drake178: MOUSE_DriverPresent
// MoO2: mouse_driver_installed
int16_t mouse_driver_installed;

// WZD dseg:78C0 00 00                   MOUSE_Usable dw 0

// WZD dseg:78C2 9E 00                   MOUSE_CurrentX dw 9Eh
int16_t mouse_x = 158;
// WZD dseg:78C4 64 00                   MOUSE_CurrentY dw 64h
int16_t mouse_y = 100;

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

// WZD dseg:78D6
// drake178: GUI_HaveAreaSave
int16_t mouse_save_flag = ST_FALSE;

// WZD dseg:78D8
// drake178: MOUSE_InitX
int16_t init_mouse_x = 158;
// WZD dseg:78DA
// drake178: MOUSE_InitY
int16_t init_mouse_y = 100;

// WZD dseg:78DC
// drake178: MOUSE_CDraw_Save
int16_t mouse_state = ST_FALSE;

// WZD dseg:78DE
struct s_mouse_list * current_mouse_list;

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


// WZD s35p02
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
// Platform-Layer int16_t MD_GetButtonStatus(void)

// WZD s35p07
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


// WZD s35p10
// PLATFORM  void Mouse_Movement_Handler(void)

// WZD s35p11
// PLATFORM void Mouse_Button_Handler(void)


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

// WZD s35p18
int16_t Pointer_X(void)
{
    return mouse_x;
}

// WZD s35p19
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


// WZD s35p30  AKA CRL_Save_RSP();
// WZD s35p31  AKA Save_Mouse_Off_Page();  CRL_Save_DSP();
// WZD s35p32  AKA IN_CRL_Save_RSP();  VGA_SaveCursorArea();

void Save_Mouse_On_Page(int16_t x, int16_t y)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    // if(current_pointer_image_number != 0)
    // if(previous_pointer_image_number != 0)
    // if(previous_pointer_image_number == 0 && mouse_save_flag != ST_FALSE)
    // mouse_save_flag == ST_TRUE
    // if(current_pointer_image_number == 0 && previous_pointer_image_number == 0 && GUI_HaveAreaSave == ST_FALSE) { return; }

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

void Save_Mouse_Off_Page(int16_t x, int16_t y)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    // if(current_pointer_image_number != 0)
    // if(previous_pointer_image_number != 0)
    // if(previous_pointer_image_number == 0 && mouse_save_flag != ST_FALSE)
    // mouse_save_flag == ST_TRUE
    // if(current_pointer_image_number == 0 && previous_pointer_image_number == 0 && GUI_HaveAreaSave == ST_FALSE) { return; }

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

// WZD s35p34  Restore_Mouse_On_Page   CRL_Restore_RSP     VGA_RestoreCursrArea
// WZD s35p35  Restore_Mouse_Off_Page  CRL_Restore_DSP     VGA_RestoreDrawCArea

void Restore_Mouse_On_Page(void)
{
    uint8_t * screen_page;
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

    screen_page = video_page_buffer[draw_page_num] + screen_page_offset;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = *mouse_buffer++;
        }
    }

}
void Restore_Mouse_Off_Page(void)
{
    uint8_t * screen_page;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;

    // if(previous_pointer_image_number != crsr_None)

    mouse_buffer = &mouse_background_buffer[0];
    screen_page_offset = *mouse_buffer++;
    width = *mouse_buffer++;
    height = *mouse_buffer++;

    screen_page = video_page_buffer[1 - draw_page_num] + screen_page_offset;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = *mouse_buffer++;
        }
    }

}

// WZD s35p37  Draw_Mouse_On_Page    CRH_Draw_RSP  GUI_DisplayCursor
// WZD s35p38  Draw_Mouse_Off_Page   CRH_Draw_DSP  GUI_DrawCursor
// WZD s35p39  Draw_Mouse_On_Page_   CRL_Draw_RSP  VGA_DisplayCursor
// WZD s35p40  Draw_Mouse_Off_Page_  CRL_Draw_DSP  VGA_DrawCursor

void Draw_Mouse_On_Page(int16_t x, int16_t y)
{

    if(current_pointer_image_number != crsr_None)
    {
        Draw_Mouse_On_Page_(x, y);
    }

}
void Draw_Mouse_Off_Page(int16_t x, int16_t y)
{

    if(current_pointer_image_number != crsr_None)
    {
        Draw_Mouse_Off_Page_(x, y);
    }

}
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

    // clip the cursor image to the right and the bottom of the screen
    if(x + CURSOR_WIDTH  < SCREEN_WIDTH ) { width  = CURSOR_WIDTH ; } else { width  = SCREEN_WIDTH  - x; }
    if(y + CURSOR_HEIGHT < SCREEN_HEIGHT) { height = CURSOR_HEIGHT; } else { height = SCREEN_HEIGHT - y; }

    screen_page_offset = ((y * SCREEN_WIDTH) + x);

    mouse_image = mouse_palette + ((current_pointer_image_number - 1) * (CURSOR_WIDTH * CURSOR_HEIGHT));

    screen_page = video_page_buffer[draw_page_num] + screen_page_offset;
    
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            pixel = *(mouse_image + (itr_height * CURSOR_HEIGHT) + itr_width);
            if(pixel != TRANSPARENT)
            {
                *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = pixel;
            }
        }
    }
}

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
            pixel = *(mouse_image + (itr_height * CURSOR_HEIGHT) + itr_width);
            if(pixel != TRANSPARENT)
            {
                *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = pixel;
            }
        }
    }

}
