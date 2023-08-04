
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "Mox_Data.H"

#include "Graphics.H"

#include "Video.H"



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


// WZD s16p01
/*
Per drake178:
    Bresenham's line-drawing algorithm (limited to the increasing X directions)
*/
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{

    /*
        if x2 < x1 switch x1 & x2 and y1 & y2
    */

}
