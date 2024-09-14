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
void Clipped_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
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
void Clipped_Dot(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

    if(CLIPPED())
    {
        return;
    }

    Dot(x1, y1, color);

}


// WZD s14p08
// drake178: VGA_WndDrawLine()
// MoO2  DNE
// 1oom  uidraw.c  ui_draw_line_limit()
void Clipped_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    Clipped_Line_Base(x1, y1, x2, y2, ST_FALSE, color, ST_NULL, ST_NULL, ST_NULL);
}


// WZD s14p09
// drake178: VGA_WndDrawPtrnLine()
// MoO2  
// 1oom  uidraw.c  ui_draw_line_limit_ctbl()
// ; int __cdecl __far Clipped_Multi_Colored_Line(int x1, int y1, int x2, int y2, char *colortbl@, int colornum, int colorpos)
void Clipped_Multi_Colored_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colortbl[], int colornum, int colorpos)
{
    Clipped_Line_Base(x1, y1, x2, y2, 1, 0, &colortbl[0], colornum, colorpos);
}


// WZD s14p10
// drake178: VGA_WndDrawLineBase()
// MoO2  Module: line  Clip_Line()
// MoO2  Module: strings  Swap_Short()
// MoO2  ~ Multi_Colored_Line_()
// 1oom  ui_draw_line_limit_do()
/*
; calculates the arguments for, and calls, either
; Line() or Multi_Colored_Line() in a way that
; obeys any limits set through Set_Window()
*/
/*
    Eh?
        clipped_flag = {F,T}
        swap (y1,y1)
        swap (x1,x2)

*/
void Clipped_Line_Base(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t Patterned, uint8_t color, uint8_t colortbl[], int16_t colornum, int16_t colorpos)
{
    uint32_t y_delta;
    uint32_t x_delta;
    int16_t P_Off_Cutoff;
    int16_t swap_y;
    int16_t swap_x;
    int16_t clipped_flag;


    clipped_flag = ST_FALSE;

    P_Off_Cutoff = 0;

    if(x1 == x2)  /* ¿ line is vertical ? */
    {
        if(
            (x1 < screen_window_x1)
            ||
            (x2 > screen_window_x2)
        )
        {
            clipped_flag = ST_TRUE;
        }
        else
        {
            if(y2 < y1)
            {
                swap_y = y2;
                y2 = y1;
                y1 = swap_y;
                colorpos = (colornum - 1) - colorpos;  /* IDGI */
            }
            if(
                (y2 < screen_window_y1)
                ||
                (y1 > screen_window_y2)
            )
            {
                clipped_flag = ST_TRUE;
            }
            else
            {
                SETMIN(y1, screen_window_y1);
                SETMAX(y2, screen_window_y2);
            }
        }
    }
    else  /* ¿ line is NOT vertical ? ... ¿ but, may be horizontal ? */
    {
        if(x2 < x1)
        {
            swap_x = x2;
            x2 = x1;
            x1 = swap_x;

            swap_y = y2;
            y2 = y1;
            y1 = swap_y;

            colorpos = ((colornum - 1) - colorpos);
        }
        y_delta = y2 - y1;
        x_delta = x2 - x1;
        if(x1 < screen_window_x1)
        {
            P_Off_Cutoff = (screen_window_x1 - x1); // count of pixels clipped from the start of the line
            y2 += (y_delta * (screen_window_x1 - x1)) / x_delta;
            x1 = screen_window_x1;
        }
        if(x1 > x2)
        {
            clipped_flag = ST_TRUE;
        }
        else
        {
            if(x2 > screen_window_x2)
            {

                y2 = y1 + (y_delta * (screen_window_x2 - x1)) / x_delta;
                x2 = screen_window_x2;
            }
            if(x2 < x1)
            {
                clipped_flag = ST_TRUE;
            }
        }
    }

    if(y1 == y2)  /* vertical line */
    {
        if(
            (y1 < screen_window_y1)
            ||
            (y2 > screen_window_y2)
        )
        {
            clipped_flag = ST_TRUE;
        }
        if(x2 < x1)
        {
            swap_x = x2;
            x2 = x1;
            x1 = swap_x;
        }
        if(
            (x2 < screen_window_x1)
            ||
            (x1 > screen_window_x2)
        )
        {
            clipped_flag = ST_TRUE;
        }
        if(x1 < screen_window_x1)
        {
            P_Off_Cutoff = (screen_window_x1 - x1);
            x1 = screen_window_x1;
        }
        if(x2 > screen_window_x2)
        {
            x2 = screen_window_x2;
        }
    }
    else  /* NOT vertical line */
    {
        if(y2 < y1)
        {
            swap_y = y2;
            y2 = y1;
            y1 = swap_y;
            swap_x = x2;
            x2 = x1;
            x1 = swap_x;
        }
        x_delta = x2 - x1;
        y_delta = y2 - y1;
        if(y1 < screen_window_y1)
        {
            x1 += (x_delta * (screen_window_y1 - y1)) / y_delta;
            y1 = screen_window_y1;
        }
        if(y1 > y2)
        {
            clipped_flag = ST_TRUE;
        }
        if(y2 > screen_window_y2)
        {
            x2 = x1 + (x_delta * (screen_window_y2 - y1)) / y_delta;
            y2 = screen_window_y2;
        }
        if(y2 < y1)
        {
            clipped_flag = ST_TRUE;
        }
    }

    if(clipped_flag == ST_FALSE)
    {
        if(Patterned == ST_FALSE)
        {
            Line(x1, y1, x2, y2, color);
        }
        else
        {
            // Multi_Colored_Line(x1, y1, x2, y2, colortbl, colornum, ((colorpos + P_Off_Cutoff) % colornum));
            ui_draw_line_ctbl(x1, y1, x2, y2, colortbl, colornum, ((colorpos + P_Off_Cutoff) % colornum));
        }
    }

}

// static void ui_draw_line_limit_do(int x0, int y0, int x1, int y1, uint8_t color, const uint8_t *colortbl, int colornum, int colorpos, int scale)
static void ui_draw_line_limit_do(int x1, int y1, int x2, int y2, uint8_t color, const uint8_t *colortbl, int colornum, int colorpos)
{
    if (x1 == x2)
    {
        if((x1 < screen_window_x1) || (x1 > screen_window_x2))
        {
            return;
        }

        if(y2 < y1)
        {
            int t = y1; y1 = y2; y2 = t;
            colorpos = colornum - 1 - colorpos;
        }
        if((y2 < screen_window_y1) || (y1 > screen_window_y2))
        {
            return;
        }
        SETMAX(y1, screen_window_y1);
        SETMIN(y2, screen_window_y2);
    }
    else
    {
        int x_delta, y_delta;
        if(x2 < x1)
        {
            int t;
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
            colorpos = colornum - 1 - colorpos;
        }
        y_delta = y2 - y1;
        x_delta = x2 - x1;
        if(x1 < screen_window_x1)
        {
            y1 += (y_delta * (screen_window_x1 - x1)) / x_delta;
            x1 = screen_window_x1;
        }
        if(x1 > x2)
        {
            return;
        }
        if(x2 > screen_window_x2)
        {
            y2 = y1 + (y_delta * (screen_window_x2 - x1)) / x_delta;
            x2 = screen_window_x2;
        }
        if(x2 < x1)
        {
            return;
        }
    }

    if(y1 == y2)
    {
        if((y1 < screen_window_y1) || (y1 > screen_window_y2))
        {
            return;
        }
        if(x2 < x1)
        {
            int t = x1; x1 = x2; x2 = t;
        }
        if((x2 < screen_window_x1) || (x1 > screen_window_x2))
        {
            return;
        }
        SETMAX(x1, screen_window_x1);
        SETMIN(x2, screen_window_x2);
    }
    else
    {
        int x_delta, y_delta;
        if(y2 < y1)
        {
            int t;
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        x_delta = x2 - x1;
        y_delta = y2 - y1;
        if(y1 < screen_window_y1)
        {
            x1 += (x_delta * (screen_window_y1 - y1)) / y_delta;
            y1 = screen_window_y1;
        }
        if(y1 > y2)
        {
            return;
        }
        if(y2 > screen_window_y2)
        {
            x2 = x1 + (x_delta * (screen_window_y2 - y1)) / y_delta;
            y2 = screen_window_y2;
        }
        if(y2 < y1)
        {
            return;
        }
    }

    if (colortbl) {
        // ui_draw_line_ctbl(x1, y1, x2, y2, colortbl, colornum, colorpos, scale);
        ui_draw_line_ctbl(x1, y1, x2, y2, colortbl, colornum, colorpos);
    } else {
        // Line(x1, y1, x2, y2, color, scale);
        Line(x1, y1, x2, y2, color);
    }
}

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
void DOS_PrintString__STUB(char * string)
{
// string= word ptr  6
// 
// _SI_string = si
// push    bp
// mov     bp, sp
// push    _SI_string
// 
// mov     _SI_string, [bp+string]
// 
// mov     dx, [bp+string]
// mov     ah, 9
// int     21h                             ; DOS - PRINT STRING
//                                         ; DS:DX -> string terminated by "$"
// pop     _SI_string
// pop     bp
// retf
}



/*
    WZD seg016
*/

// WZD s16p01
// MoO2  Module: graphics  Fill()
// 1oom  uidraw.c  ui_draw_filled_rect()
void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
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
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
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
// drake178: VGA_DrawPatternLine()
// MoO2  Multi_Colored_Line()
void Multi_Colored_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colortbl[], int16_t colornum, int16_t colorpos)
{
    uint8_t * Repeat_Mark;
    int16_t line_increment;
    int16_t x_slope;
    int16_t length;
    int16_t y_slope;

    int16_t x_delta;  // _CX_
    int16_t y_delta;  // _DX_
    int16_t x_error;
    int16_t y_error;
    int16_t temp;  // _SI_
    uint8_t * color_ptr;  // _SI_
    uint8_t * video_memory;  // ES:DI
    uint8_t color;  // _AL_

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

    color_ptr = &colortbl[colorpos];
    Repeat_Mark = &colortbl[colornum];

    video_memory = current_video_page + ((y1 * SCREEN_WIDTH) + x1);

    x_error = 256 / 2;
    y_error = 256 / 2;

    while(length--)
    {
        // ~ color = colortbl[colorpos]
        color = *color_ptr++;

        if(color != ST_TRANSPARENT)
        {
            *video_memory = color;
        }

        if(color_ptr == Repeat_Mark)
        {
            color_ptr = &colortbl[0];
        }

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

// void ui_draw_line_ctbl(int x0, int y0, int x1, int y1, const uint8_t *colortbl, int colornum, int pos, int scale)
void ui_draw_line_ctbl(int x1, int y1, int x2, int y2, const uint8_t *colortbl, int colornum, int pos)
{
    int xslope = 0, yslope = 0, yinc, length;    /* BUG? xslope and yslope not cleared by MOO1 */
    int temp;

    if(x2 < x1)
    {
        temp = x2;
        x2 = x1;
        x1 = temp;
        temp = y2;
        y2 = y1;
        y1 = temp;
    }

    {
        int dx, dy;
        dx = x2 - x1;
        dy = y2 - y1;
        // yinc = UI_SCREEN_W * scale;
        yinc = SCREEN_WIDTH;
        if(dy < 0)
        {
            dy = -dy;
            // yinc = -UI_SCREEN_W * scale;
            yinc = -SCREEN_WIDTH;
        }
        if(dx < dy)
        {
            length = dy + 1;
            yslope = 256;
            if (dy != 0) {
                xslope = (dx << 8) / dy;
            }
        }
        else
        {
            length = dx + 1;
            if(dx != 0)
            {
                xslope = 256;
                yslope = (dy << 8) / dx;
            }
        }
    }

    {
        // uint8_t *p = hw_video_get_buf() + (y1 * UI_SCREEN_W + x1) * scale;
        uint8_t *p = current_video_page + ((y1 * SCREEN_WIDTH) + x1);
        int xerr, yerr;

        xerr = 256 / 2;
        yerr = 256 / 2;

        while(length--) {
            uint8_t color;
            color = colortbl[pos++];
            if(pos >= colornum)
            {
                pos = 0;
            }
            if(color != 0)
            {
                // if (scale == 1) {
                //     *p = color;
                // } else {
                //     gfxscale_draw_pixel(p, color, UI_SCREEN_W, scale);
                // }
                *p = color;
            }
            xerr += xslope;
            if((xerr & 0xFF00) != 0)
            {
                xerr &= 0x00FF;
                // p += scale;
                p += 1;
            }
            yerr += yslope;
            if((yerr & 0xFF00) != 0)
            {
                yerr &= 0x00FF;
                p += yinc;
            }
        }
    }
}


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
