
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "Mox_Data.H"

#include "Graphics.H"

#include "Video.H"

#include <stdio.h>
#include <stdlib.h>
// #include <dos.h>


void plot_pixel(int x, int y, byte color);
void line_slow(int x1, int y1, int x2, int y2, byte color);
void line_fast(int x1, int y1, int x2, int y2, byte color);


// WZD s14p04
// MoO2  Module: graphics  Set_Window(signed integer (2 bytes) x1, signed integer (2 bytes) y1, signed integer (2 bytes) x2, signed integer (2 bytes) y2)
void Set_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if ( x1 < SCREEN_XMIN ) { x1 = SCREEN_XMIN; }
    if ( y1 < SCREEN_YMIN ) { y1 = SCREEN_YMIN; }
    // if (x2 > screen_pixel_width) { x2 = screen_pixel_width - 1;}
    // if (y2 > screen_pixel_height) { y2 = screen_pixel_height - 1;}
    if ( x2 > SCREEN_XMAX ) { x2 = SCREEN_XMAX; }
    if ( y2 > SCREEN_YMAX ) { y2 = SCREEN_YMAX; }
    if ( x1 > x2 ) { SWAP(x2,x1); }
    if ( y1 > y2 ) { SWAP(y2,y1); }

    screen_window_x1 = x1;
    screen_window_x2 = x2;
    screen_window_y1 = y1;
    screen_window_y2 = y2;
}

// WZD s14p05
// MoO2  Module: graphics  Reset_Window()
void Reset_Window(void)
{
    // screen_window_x2 = screen_pixel_width - 1;
    // screen_window_y2 = screen_pixel_height - 1;
    screen_window_x1 = SCREEN_XMIN;
    screen_window_x2 = SCREEN_XMAX;
    screen_window_y1 = SCREEN_YMIN;
    screen_window_y2 = SCREEN_YMAX;
}


// WZD s14p06
// drake178: VGA_WndDrawFilldRect
// MoO2 Fill() just uses a clip flag
void Clipped_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{

    if( x2 >= screen_window_x1 && x1 <= screen_window_x2 && y2 >= screen_window_y1 && y1 <= screen_window_y2)
    {
        if(x1 < screen_window_x1)
        {
            x1 = screen_window_x1;
        }
        if(y1 < screen_window_y1)
        {
            y1 = screen_window_y1;
        }
        if(x2 > screen_window_x2)
        {
            x2 = screen_window_x2;
        }
        if(y2 > screen_window_y2)
        {
            y2 = screen_window_y2;
        }

        Fill(x1, y1, x2, y2, color);
    }
    // else { return 0; }
    
}


// WZD s16p01
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
// VGA_PutPixel


// WZD s16p03
// drake178: VGA_DrawLine()
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color);
/*
Per drake178:
    Bresenham's line-drawing algorithm (limited to the increasing X directions)
*/
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Line()\n", __FILE__, __LINE__);
// #endif

    // int16_t swap_x_1_2;
    // int16_t swap_y_1_2;
    // 
    // // @@Swap_X2X1_Y2Y1
    // if(x2 < x1)
    // {
    //     swap_x_1_2 = x1;
    //     x2 = x1;
    //     x1 = swap_x_1_2;
    // 
    //     swap_y_1_2 = y2;
    //     y2 = y1;
    //     y1 = swap_y_1_2;
    // }

    line_fast(x1, y1, x2, y2, color);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Line()\n", __FILE__, __LINE__);
// #endif

}


// ¿ Festest Possible Intel-CPU Absolute Value *Function* ?
#define ABS(x)  ( x = (x >= 0) ? x : -x );

#define sgn(x)  ( (x < 0) ? -1 : ((x > 0) ? 1 : 0) )    /* macro to return the sign of a number */

// byte * VGA = (byte *)0xA0000000L;        /* this points to video memory. */


/**************************************************************************
 *  plot_pixel                                                            *
 *    Plot a pixel by directly writing to video memory, with no           *
 *    multiplication.                                                     *
 **************************************************************************/

void plot_pixel(int x, int y, byte color)
{
  /*  y*320 = y*256 + y*64 = y*2^8 + y*2^6   */
  // VGA[ (y << 8) + (y << 6) + x] = color;
  current_video_page[ (y << 8) + (y << 6) + x] = color;

}


/**************************************************************************
 *  line_slow                                                             *
 *    draws a line using multiplication and division.                     *
 **************************************************************************/

void line_slow(int x1, int y1, int x2, int y2, byte color)
{
    int dx,dy,sdx,sdy,px,py,dxabs,dyabs,i;
    float slope;

    dx = x2 - x1;      /* the horizontal distance of the line */
    dy = y2 - y1;      /* the vertical distance of the line */
    dxabs = abs(dx);
    dyabs = abs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);

    if (dxabs >= dyabs)  /* the line is more horizontal than vertical */
    {
        slope = (float)dy / (float)dx;
        for(i = 0; i != dx; i += sdx)
        {
            px = i + x1;
            py = slope * i + y1;
            plot_pixel(px, py, color);
        }
    }
    else /* the line is more vertical than horizontal */
    {
        slope = (float)dx / (float)dy;
        for(i = 0; i != dy; i += sdy)
        {
            px = slope * i + x1;
            py = i + y1;
            plot_pixel(px, py, color);
        }
    }
}


/**************************************************************************
 *  line_fast                                                             *
 *    draws a line using Bresenham's line-drawing algorithm, which uses   *
 *    no multiplication or division.                                      *
 **************************************************************************/

void line_fast(int x1, int y1, int x2, int y2, byte color)
{
    int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
    uint8_t * screen_page;
    uint16_t screen_page_offset;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: line_fast(x1 = %d, y1 = %d, x2 = %d, y2 = %d, color = %d)\n", __FILE__, __LINE__, x1, y1, x2, y2, color);
// #endif

    dx = x2 - x1;      /* the horizontal distance of the line */
    dy = y2 - y1;      /* the vertical distance of the line */
    dxabs = abs(dx);
    dyabs = abs(dy);
    sdx = sgn(dx);
    sdy = sgn(dy);
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    screen_page = video_page_buffer[1 - draw_page_num];

    *(screen_page + (py * SCREEN_WIDTH) + px) = color;

    if (dxabs >= dyabs) /* the line is more horizontal than vertical */
    {
        for(i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if(y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            // plot_pixel(px, py, color);
            *(screen_page + (py * SCREEN_WIDTH) + px) = color;
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for(i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if(x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            // plot_pixel(px, py, color);
            *(screen_page + (py * SCREEN_WIDTH) + px) = color;
        }
    }
}


// WZD s16p04
// VGA_DrawPatternLine

// WZD s16p05
// UU_VGA_CreateColorWave

// WZD s16p06
// UU_VGA_DiagColumns

// WZD s16p07
// RNG_Direct_LFSR

// WZD s16p08
// VGA_Shade_Rect

// WZD s16p09
// VGA_Grayscale_Rect

// WZD s16p10
// UU_VGA_Columns

// WZD s16p11
// UU_VGA_ScrambleRect
