
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Video.H"



/*
    Initialized Data
*/
// WZD dseg:76DA
int16_t screen_window_x1 = 0;
// WZD dseg:76DC
int16_t screen_window_y1 = 0;
// WZD dseg:76DE
int16_t screen_window_x2 = 319;
// WZD dseg:76E0
int16_t screen_window_y2 = 199;



// WZD s14p04
// MoO2  Module: graphics  Set_Window(signed integer (2 bytes) x1, signed integer (2 bytes) y1, signed integer (2 bytes) x2, signed integer (2 bytes) y2)
void Set_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if ( x1 < 0 ) { x1 = 0; }
    if ( y1 < 0 ) { y1 = 0; }
    // if (x2 > screen_pixel_width) { x2 = screen_pixel_width - 1;}
    // if (y2 > screen_pixel_height) { y2 = screen_pixel_height - 1;}
    if ( x2 > 319 ) { x2 = 319; }
    if ( y2 > 199 ) { y2 = 199; }
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
    screen_window_x1 = 0;
    screen_window_x2 = 319;
    screen_window_y1 = 0;
    screen_window_y2 = 199;
}


// WZD s16p01
void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color)
{
    int16_t itr_x;
    int16_t itr_y;
    // void ZeroMemory([in] PVOID  Destination, [in] SIZE_T Length);

    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            *(current_video_page + (itr_y * 320) + itr_x) = color;
        }
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
