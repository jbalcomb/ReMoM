/*
    WIZARDS.EXE
        seg014
        seg015
        seg016

MoO2:
Module: graphcis
Module: regions
Module: shear
*/

#include "MoX.H"


#define Darken_Fill(_x1_,_y1_,_x2_,_y2_) { Tint_Fill((_x1_), (_y1_), (_x2_), (_y2_), 0); }




void plot_pixel(int x, int y, byte color);
void line_slow(int x1, int y1, int x2, int y2, byte color);
void line_fast(int x1, int y1, int x2, int y2, byte color);



// WZD dseg:76DA                                                 BEGIN : ovr014
// WZD dseg:76DA                                                 BEGIN : graphics

// WZD dseg:76DA
int16_t screen_window_x1 = SCREEN_XMIN;
// WZD dseg:76DC
int16_t screen_window_y1 = SCREEN_YMIN;
// WZD dseg:76DE
int16_t screen_window_x2 = SCREEN_XMAX;
// WZD dseg:76E0
int16_t screen_window_y2 = SCREEN_YMAX;

// WZD dseg:76E0                                                 END : graphics
// WZD dseg:76E2                                                 ¿ Init ?





// WZD s14p01
// MoO2  Module: init  void Init_Drivers()
/*  Init.C */  // void Init_Drivers(int input_type, char * font_file);


// WZD s14p02
// MoO2  DNE
/* Init.C */  // void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file)


// WZD s14p03
// MoO2  Module: fonts  Load_Font_File()
/* Fonts.C */  // Load_Font_File()


// WZD s14p04
// MoO2  Module: graphics  Set_Window(signed integer (2 bytes) x1, signed integer (2 bytes) y1, signed integer (2 bytes) x2, signed integer (2 bytes) y2)
// 1oom  uiobj.c  uiobj_set_limits()
void Set_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    SETMIN(x1, SCREEN_XMIN);
    SETMIN(y1, SCREEN_YMIN);
    SETMAX(x2, SCREEN_XMAX);
    SETMAX(y2, SCREEN_YMAX);

    if(x1 > x2) { SWAP(x2,x1); }
    if(y1 > y2) { SWAP(y2,y1); }

    screen_window_x1 = x1;
    screen_window_x2 = x2;
    screen_window_y1 = y1;
    screen_window_y2 = y2;
}


// WZD s14p05
// MoO2  Module: graphics  Reset_Window()
// 1oom  uiobj.c  uiobj_set_limits_all()
void Reset_Window(void)
{
    screen_window_x1 = SCREEN_XMIN;
    screen_window_x2 = SCREEN_XMAX;
    screen_window_y1 = SCREEN_YMIN;
    screen_window_y2 = SCREEN_YMAX;
}


// WZD s14p06
// MoO2  Module: graphics  Fill() Fill() just uses a clip flag
void Clipped_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{

    if(CLIPPED())
    {
        return;
    }

    SETMIN(x1, screen_window_x1);
    SETMIN(y1, screen_window_y1);
    SETMAX(x2, screen_window_x2);
    SETMAX(y2, screen_window_y2);
    
    Fill(x1, y1, x2, y2, color);
    
}



// WZD s14p07
// drake178: 
// MoO2  
void Clipped_Dot(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{

    if(CLIPPED())
    {
        return;
    }

    Dot(x1, y1, color);

}




// WZD s14p08
// drake178: 
// MoO2  
// VGA_WndDrawLine()

// WZD s14p09
// drake178: 
// MoO2  
// VGA_WndDrawPtrnLine()

// WZD s14p10
// drake178: 
// MoO2  
// VGA_WndDrawLineBase()

// WZD s14p11
// drake178: UU_VGA_DrawBiColorRect()
// MoO2  Module: graphics  Interlaced_Fill()
// UU_Interlaced_Fill()

// WZD s14p12
// drake178: 
// MoO2  
// UU_VGA_DrawRect()

// WZD s14p13
// drake178: 
// MoO2  
// UU_VGA_WndDrawRect()

// WZD s14p14
// drake178: 
// MoO2  
// UU_VGA_DrawDblRect()


// WZD s14p15
// drake178: VGA_RectangleFX()
// MoO2  Module: graphics  Gradient_Fill()
/*
only called for Field Type 10 
FFFFFFFF ft_StringList        = 0Ah

MoO2  void Gradient_Fill(int x1, int y1, int x2, int y2, int fill_type, int * color_array, int color_count, int ripple_count, int seed)

MoO1  fill_type { 1, 3, 7, 13,     15     }
MoM   fill_type { 1, 3, 7,     14, 15, 16 }
MoO2  fill_type { 1, 3,            15, 16 }

1: 
3: "darken"
7: 
13: 
14: 
15: 
16: Gray_Scale_Fill()

Tint_Fill()
Gray_Scale_Fill()
Fill()

    fill_type 3 and 15 are the same except 3 always uses color block 0 (grayscale?)

*/
void Gradient_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t fill_type, int16_t remap_block, int16_t Slope, int16_t Scale, int16_t Seed)
{
    switch(fill_type)
    {
        case 0:
        {
            DLOG("switch(fill_type)  case 0:");
        } break;
        case 1:
        {

        } break;
        case 2:
        {

        } break;
        case 3:
        {
            // ¿ ~== MoO2  Darken_Fill() ?  Yup. Exact same call.
            DLOG("switch(fill_type)  case 3:");
            Tint_Fill(x1, y1, x2, y2, 0);
        } break;
        case 4:
        {

        } break;
        case 5:
        {

        } break;
        case 6:
        {

        } break;
        case 7:
        {

        } break;
        case 8:
        {

        } break;
        case 9:
        {

        } break;
        case 10:
        {

        } break;
        case 11:
        {

        } break;
        case 12:
        {

        } break;
        case 13:
        {

        } break;
        case 14:
        {

        } break;
        case 15:
        {
            DLOG("switch(fill_type)  case 15:");
            Tint_Fill(x1, y1, x2, y2, remap_block);
        } break;
        case 16:
        {
            DLOG("switch(fill_type)  case 16:");

        } break;
        default:
        {
            DLOG("switch(fill_type)  default:");

        } break;
        
    }
}

// WZD s14p16
// drake178: DOS_PrintString()
// MoO2  
// 1oom
// DOS_PrintString



/*
    WZD seg016
*/

// WZD s16p01
// MoO2  Module: graphics  Fill()
// 1oom  uidraw.c  ui_draw_filled_rect()
void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{
//     int16_t itr_x;
//     int16_t itr_y;
//     // void ZeroMemory([in] PVOID  Destination, [in] SIZE_T Length);
// 
//     for(itr_y = 0; itr_y < SCREEN_HEIGHT; itr_y++)
//     {
//         for(itr_x = 0; itr_x < SCREEN_WIDTH; itr_x++)
//         {
//             *(current_video_page + (itr_y * SCREEN_WIDTH) + itr_x) = color;
//         }
//     }

    byte_ptr screen_page;
    int16_t width;
    int16_t height;
    uint16_t screen_page_offset;
    uint16_t itr_width;
    uint16_t itr_height;
    
    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    screen_page_offset = ((y1 * SCREEN_WIDTH) + x1);

    screen_page = current_video_page + screen_page_offset;

    itr_height = 0;
    while(itr_height < height)
    {
        itr_width = 0;
        while(itr_width < width)
        {
            *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = color;
            itr_width++;
        }
        itr_height++;
    }

}


// WZD s16p02
// MoO2  Module: graphics  Dot()
// 1oom  uidraw.c  ui_draw_pixel()
void Dot(int16_t x, int16_t y, uint8_t color)
{
    uint8_t * video_memory;

    video_memory = current_video_page + ((y * SCREEN_WIDTH) + x);

    *video_memory = color;
}


// WZD s16p03
// drake178: VGA_DrawLine()
// MoO2  Module: line  Line()
// 1oom  uidraw.c  // ui_draw_line1()
/*
~ Bresenham's Line-Drawing Algorithm  (¿ DDA ?)
always draws left to right  (increasing X direction)
X-Major || Y-Major
¿ *units* of 256 ?
~ single register error accumulation ... optimization to eliminate memory accesses
Per the byte-saving (Assembly) era, allows error conditions to fall through

*/
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{
    int16_t temp;  // _SI_
    int16_t x_delta;  // _CX_
    int16_t y_delta;  // _DX_
    int16_t line_increment;
    int16_t length;
    int16_t x_slope;
    int16_t y_slope;
    int16_t x_error;
    int16_t y_error;
    uint8_t * video_memory;  // ES:DI

    if(x2 < x1)
    {
        temp = x2;
        x2 = x1;
        x1 = temp;
        temp = y2;
        y2 = y1;
        y1 = temp;
    }

    x_delta = (x2 - x1);
    y_delta = (y2 - y1);

    line_increment = SCREEN_WIDTH;

    if(y_delta < 0)
    {
        y_delta = -(y_delta);
        line_increment = -(SCREEN_WIDTH);
    }

    if(x_delta < y_delta)
    {
        /* Y Major */
        length = y_delta + 1;
        y_slope = 256;
        if(y_delta != 0)  /* avoid division by zero error condition */
        {
            x_slope = (x_delta * 256) / y_delta;
        }
    }
    else
    {
        /* X Major */
        length = x_delta + 1;
        if(x_delta != 0)  /* avoid division by zero error condition */
        {
            x_slope = 256;
            y_slope = (y_delta * 256) / x_delta;
        }
    }

    video_memory = current_video_page + ((y1 * SCREEN_WIDTH) + x1);

    x_error = 256 / 2;
    y_error = 256 / 2;

    while(length--)
    {
        *video_memory = color;

        x_error += x_slope;

        // if((x_error & 0xFF00) != 0)
        if(x_error >= 256)
        {
            x_error &= 0x00FF;
            video_memory += 1;
        }

        y_error += y_slope;

        // if ((y_error & 0xFF00) != 0)
        if(y_error >= 256)
        {
            y_error &= 0x00FF;
            video_memory += line_increment;
        }
    }

}

// WZD s16p04
// drake178: VGA_DrawPatternLine90
// MoO2  
// VGA_DrawPatternLine

// WZD s16p05
// drake178: UU_VGA_CreateColorWave()
// MoO2  
// UU_VGA_CreateColorWave

// WZD s16p06
// drake178: UU_VGA_DiagColumns()
// MoO2  
// UU_VGA_DiagColumns

// WZD s16p07
// drake178: 
// MoO2  
// RNG_Direct_LFSR

// WZD s16p08
// drake178: VGA_Shade_Rect()
// MoO2  Module: graphics  Tint_Fill()
void Tint_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t remap_block)
{
    
    byte_ptr screen_page;
    int16_t width;
    int16_t height;
    uint16_t screen_page_offset;
    uint16_t itr_width;
    uint16_t itr_height;
    uint8_t remap_block_index;
    uint8_t remap_color;

    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    screen_page_offset = ((y1 * SCREEN_WIDTH) + x1);

    screen_page = current_video_page + screen_page_offset;

    itr_height = 0;
    while(itr_height < height)
    {
        itr_width = 0;
        while(itr_width < width)
        {
            remap_block_index = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
            remap_color = *(remap_color_palettes + (remap_block * (16 * 16)) + remap_block_index);
            *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width) = remap_color;
            itr_width++;
        }
        itr_height++;
    }

}

// WZD s16p09
// drake178: VGA_Grayscale_Rect()
// MoO2  Module: graphics  Gray_Scale_Fill()
// VGA_Grayscale_Rect

// WZD s16p10
// drake178: UU_VGA_Columns()
// MoO2  
// UU_VGA_Columns

// WZD s16p11
// drake178: UU_VGA_ScrambleRect()
// MoO2  
// UU_VGA_ScrambleRect
