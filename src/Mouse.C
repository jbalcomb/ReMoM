
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Mouse.H"

#include "Fonts.H"
#include "Input.H"
#include "Video.H"


// extern uint8_t g_Video_Back_Buffer[];  // MoM_main.C



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

// TODO  _mouse_list_arrow


int16_t init_mouse_x = 158;                     // MGC dseg:43AA
int16_t init_mouse_y = 100;                     // MGC dseg:43AC

// DONT  mouse_driver_installed = ST_FALSE;              // MGC dseg:4390

int16_t mouse_enabled = ST_FALSE;               // MGC dseg:438E
int16_t mouse_state = ST_FALSE;                 // MGC dseg:43AE

int16_t mouse_x = 158;
int16_t mouse_y = 100;
int16_t current_mouse_list_count = 1;
struct s_mouse_list * current_mouse_list;

// MoM dseg:43B2 00 00 00 00 00 00 00 00+    CR_Save_RSP db 1200 dup(  0)
// MoM dseg:4862 00 00 00 00 00 00 00 00+    CR_Save_DSP db 1200 dup(  0)
// MoO2 mouse_background_buffer_pointer, offset mouse_background_buffer ; 624 bytes (24 * 26)
// MoO2 mouse_off_page_buffer_pointer, offset mouse_off_page_buffer ; 624 bytes (24 * 26)
uint16_t mouse_background_buffer[600];
uint16_t mouse_off_page_buffer[600];


int16_t current_pointer_image_number = 0;
int16_t previous_pointer_image_number = 0;
int16_t current_pointer_offset = 0;



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

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Set_Mouse_List(count = %d, list = %p)\n", __FILE__, __LINE__, count, list);
#endif

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

#ifdef DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Set_Mouse_List(count = %d, list = %p)\n", __FILE__, __LINE__, count, list);
#endif
}


// MGC s33p02
// void GUI_FindWindow(int X_Pos, int Y_Pos);
void Check_Mouse_Shape(int16_t x, int16_t y)
{
    int16_t count;
    int16_t list_index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Check_Mouse_Shape(x = %d, y = %d)\n", __FILE__, __LINE__, x, y);
#endif

    previous_pointer_image_number = current_pointer_image_number;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: previous_pointer_image_number: %d\n", __FILE__, __LINE__, previous_pointer_image_number);
#endif

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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: current_pointer_offset: %d\n", __FILE__, __LINE__, current_pointer_offset);
#endif
    current_pointer_image_number = current_mouse_list[list_index].image_num;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: current_pointer_image_number: %d\n", __FILE__, __LINE__, current_pointer_image_number);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Check_Mouse_Shape(x = %d, y = %d)\n", __FILE__, __LINE__, x, y);
#endif
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


// MGC s33p07
int16_t Init_Mouse_Driver(void)
{

    // DONT  mouse_installed = ST_TRUE;

    // TODO  g_MD_INT_InProcess = ST_FALSE;
    // mouse_interrupt_active

    // ~== MoO2 Set_Pointer_Position
    mouse_x = init_mouse_x;
    mouse_y = init_mouse_y;

    // TODO  Mouse_Wait()  AKA MD_WaitRelease()  // MGC s33p06

    mouse_enabled = ST_FALSE;

    // TODO  g_MD_ClickRec1 = 0;  /* ? ST_FALSE ? */
    // TODO  g_MD_ClickRec2 = 0;  /* ? ST_FALSE ? */

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
    // return mouse_x;
    // return g_Mouse_X;
    // return g_Mouse_X/320;
    // return (g_Mouse_X % 320);
    return (g_Mouse_X / 2);
}

// MGC s33p19
int16_t Pointer_Y(void)
{
    // return mouse_y;
    // return g_Mouse_Y;
    // return g_Mouse_Y/200;
    // return (g_Mouse_Y % 200);
    return (g_Mouse_Y / 2);
}


// ? DSP & RSP ? Front & Back || On & Off || "draw_page_num" vs. "current_page_flag" {1,2,3} ?


// MGC s33p30
// MGC s33p31
// MGC s33p32
void Save_Mouse(int16_t x, int16_t y)
{
    uint8_t * page_buffer;
    uint16_t * mouse_buffer;
    uint16_t screen_page_offset;
    uint16_t width;
    uint16_t height;
    uint16_t itr_width;
    uint16_t itr_height;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Save_Mouse(x = %d, y = %d)\n", __FILE__, __LINE__, x, y);
#endif

    if(x + 16 < 320) { width  = 16; } else { width  = 320 - x; }
    if(y + 16 < 200) { height = 16; } else { height = 200 - y; }

    screen_page_offset = ((y * 320) + x);

    mouse_buffer = &mouse_off_page_buffer[0];
    *mouse_buffer++ = screen_page_offset;
    *mouse_buffer++ = width;
    *mouse_buffer++ = height;

    // page_buffer = g_Video_Back_Buffer + screen_page_offset;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Save_Mouse(x = %d, y = %d)\n", __FILE__, __LINE__, x, y);
#endif
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

    // page_buffer = g_Video_Back_Buffer + screen_page_offset;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Mouse(x = %d, y = %d)\n", __FILE__, __LINE__, x, y);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] mouse_palette: %p\n", __FILE__, __LINE__, mouse_palette);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] current_pointer_image_number: %d\n", __FILE__, __LINE__, current_pointer_image_number);
#endif
    // mouse_image_width  =  16
    // mouse_image_height =  16
    // mouse_image_pixers = 256  (100h)
    mouse_image = mouse_palette + ((current_pointer_image_number - 1) * (16 * 16));
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] mouse_image: %p\n", __FILE__, __LINE__, mouse_image);
#endif

    // CURSOR_WIDTH  PIXX
    // CURSOR_HEIGHT PIXY
    if(x + 16 < 320) { width  = 16; } else { width  = 320 - x; }
    if(y + 16 < 200) { height = 16; } else { height = 200 - y; }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] width: %d\n", __FILE__, __LINE__, width);
    dbg_prn("DEBUG: [%s, %d] height: %d\n", __FILE__, __LINE__, height);
#endif

    // bbuff_pos = g_Video_Back_Buffer + ((y * 320) + x);
    bbuff_pos = current_video_page + ((y * 320) + x);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] bbuff_pos: %p\n", __FILE__, __LINE__, bbuff_pos);
#endif


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
// #ifdef STU_DEBUG
//     // dbg_prn("DEBUG: [%s, %d] pixel: %02X\n", __FILE__, __LINE__, pixel);
//     dbg_prn("DEBUG: [%s, %d] pixel[%d]: %02X\n", __FILE__, __LINE__, ((itr_height * 16) + itr_width), pixel);
// #endif
            if(pixel != 0x00)
            {
                *(bbuff_pos + (itr_height * 320) + itr_width) = pixel;
            }

            itr_width++;
        }
        itr_height++;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Mouse(x = %d, y = %d)\n", __FILE__, __LINE__, x, y);
#endif

}
