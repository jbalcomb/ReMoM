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

#include "Fonts.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"

#include "Graphics.h"



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
/* GEMINI */
// MoO2  Module: graphics  Interlaced_Fill()
<<<<<<< HEAD
/* COPILOT */
=======
/**
 * @brief Draws a single-pixel rectangle outline with two independent edge colors.
 *
 * Renders a rectangle using four `Line()` calls, splitting the edges into two
 * color groups to create a simple 2D bevel (raised or sunken panel) effect:
 *   - Top and left edges  : Color1 (typically the highlight color)
 *   - Bottom and right edges : Color2 (typically the shadow color)
 *
 * The bottom and right edges each start one pixel inward from their respective
 * corner to avoid overwriting the corner pixel already drawn by the top/left pass.
 *
 * This function draws directly to `current_video_page` via `Line()` without
 * clip-region checking. For a clipped variant see `UU_VGA_WndDrawRect()`.
 * For a double-bordered version see `UU_VGA_DrawDblRect()`.
 *
 * @param x1     Left pixel coordinate of the rectangle.
 * @param y1     Top pixel coordinate of the rectangle.
 * @param x2     Right pixel coordinate of the rectangle.
 * @param y2     Bottom pixel coordinate of the rectangle.
 * @param Color1 Palette index used for the top and left edges.
 * @param Color2 Palette index used for the bottom and right edges.
 */
>>>>>>> origin/claude/upbeat-williams
void UU_Interlaced_Fill(int x1, int y1, int x2, int y2, int Color1, int Color2)
{
    register int _SI_x1;
    register int _DI_y1;

    _SI_x1 = x1;
    _DI_y1 = y1;

    /* Top edge */
    Line(_SI_x1, _DI_y1, x2, _DI_y1, Color1);

    /* Left edge */
    Line(_SI_x1, _DI_y1, _SI_x1, y2, Color1);

<<<<<<< HEAD
    /* Bottom edge: starts x1+1 to avoid overwriting the bottom-left corner */
    Line(_SI_x1 + 1, y2, x2, y2, Color2);

    /* Right edge: starts y1+1 to avoid overwriting the top-right corner */
    Line(x2, _DI_y1 + 1, x2, y2, Color2);
}

// WZD s14p12
// drake178: 
// MoO2  
// UU_VGA_DrawRect()
/* COPILOT */
void /* far */ UU_VGA_DrawRect(int Left, int Top, int Width, int Height, int Color)
{
=======
    /* Bottom edge */
    Line(_SI_x1 + 1, y2, x2, y2, Color2);

    /* Right edge */
    Line(x2, _DI_y1 + 1, x2, y2, Color2);
}


// WZD s14p12
/* GEMINI */
/**
 * @brief Draws a single-color rectangle outline specified by top-left corner, width, and height.
 *
 * Renders the four edges of a rectangle using `Line()`, which draws directly to
 * `current_video_page` without clip-region checking. The rectangle coordinates are
 * expressed as a top-left origin plus extent (Width × Height) rather than two corner
 * pairs; the function computes Right and Bottom internally:
 *   - Right  = Left + Width  - 1
 *   - Bottom = Top  + Height - 1
 *
 * The bottom and right edges each start one pixel inward from their respective corners
 * to avoid overwriting the corner pixel already drawn by the top/left pass.
 *
 * Edge drawing order:
 *  1. Top    : (Left, Top)       → (Left + Width - 1, Top)
 *  2. Left   : (Left, Top)       → (Left, Top + Height - 1)
 *  3. Bottom : (Left + 1, Top + Height - 1) → (Left + Width - 1, Top + Height - 1)
 *  4. Right  : (Left + Width - 1, Top + 1)  → (Left + Width - 1, Top + Height - 1)
 *
 * For a clipped variant (respects `Set_Window()`) see `UU_VGA_WndDrawRect()`.
 * For a two-color bevel variant see `UU_Interlaced_Fill()`.
 * For a double-bordered variant see `UU_VGA_DrawDblRect()`.
 *
 * @param Left   X pixel coordinate of the left edge of the rectangle.
 * @param Top    Y pixel coordinate of the top edge of the rectangle.
 * @param Width  Width of the rectangle in pixels (includes both edge columns).
 * @param Height Height of the rectangle in pixels (includes both edge rows).
 * @param Color  Palette index used for all four edges.
 */
void /* far */ UU_VGA_DrawRect(int Left, int Top, int Width, int Height, int Color)
{
    /* Local variables (mapped from stack frame) */
    /* [bp-04h] -> Bottom : int */
    /* [bp-02h] -> Right  : int */
>>>>>>> origin/claude/upbeat-williams
    int Bottom;
    int Right;
    int si_Left;
    int di_Top;

<<<<<<< HEAD
=======
    /* Registers si and di are used as register variables for Left and Top */
>>>>>>> origin/claude/upbeat-williams
    si_Left = Left;
    di_Top = Top;

    Right = si_Left + Width - 1;
    Bottom = di_Top + Height - 1;

    /* Top edge: (Left, Top) to (Right, Top) */
    Line(si_Left, di_Top, Right, di_Top, Color);

    /* Left edge: (Left, Top) to (Left, Bottom) */
    Line(si_Left, di_Top, si_Left, Bottom, Color);

    /* Bottom edge: (Left + 1, Bottom) to (Right, Bottom) */
<<<<<<< HEAD
    Line(si_Left + 1, Bottom, Right, Bottom, Color);

    /* Right edge: (Right, Top + 1) to (Right, Bottom) */
=======
    /* Note: Starts at Left + 1 to avoid double-drawing the bottom-left corner */
    Line(si_Left + 1, Bottom, Right, Bottom, Color);

    /* Right edge: (Right, Top + 1) to (Right, Bottom) */
    /* Note: Starts at Top + 1 to avoid double-drawing the top-right corner */
>>>>>>> origin/claude/upbeat-williams
    Line(Right, di_Top + 1, Right, Bottom, Color);
}


// WZD s14p13
<<<<<<< HEAD
// drake178: 
// MoO2  
// UU_VGA_WndDrawRect()
/* COPILOT */
=======
/* GEMINI */
/**
 * @brief Draws a clipped single-color rectangle outline within the current screen window.
 *
 * Renders the four edges of a rectangle using `Clipped_Line()`, which respects the
 * clip region established by `Set_Window()`. Coordinates outside the window are silently
 * clipped rather than drawn.
 *
 * The rectangle is specified by its top-left corner and its Width/Height dimensions.
 * Bottom and right edges are drawn starting one pixel inward from their respective
 * corners to avoid overwriting the corner pixel already placed by the top/left pass.
 *
 * Edge drawing order:
 *  1. Top    : (Left, Top)       → (Left + Width - 1, Top)
 *  2. Left   : (Left, Top)       → (Left, Top + Height - 1)
 *  3. Bottom : (Left + 1, Top + Height - 1) → (Left + Width - 1, Top + Height - 1)
 *  4. Right  : (Left + Width - 1, Top + 1)  → (Left + Width - 1, Top + Height - 1)
 *
 * @param Left   X pixel coordinate of the left edge of the rectangle.
 * @param Top    Y pixel coordinate of the top edge of the rectangle.
 * @param Width  Width of the rectangle in pixels (includes both edge columns).
 * @param Height Height of the rectangle in pixels (includes both edge rows).
 * @param Color  Palette index used for all four edges.
 */
>>>>>>> origin/claude/upbeat-williams
void /* far */ UU_VGA_WndDrawRect(int Left, int Top, int Width, int Height, char Color)
{
    int Bottom;
    int Right;
    int si_Left;
    int di_Top;

    si_Left = Left;
    di_Top = Top;

    Right = si_Left + Width - 1;
    Bottom = di_Top + Height - 1;

    /* Top edge: (Left, Top) to (Right, Top) */
    Clipped_Line(si_Left, di_Top, Right, di_Top, (int)Color);

    /* Left edge: (Left, Top) to (Left, Bottom) */
    Clipped_Line(si_Left, di_Top, si_Left, Bottom, (int)Color);

    /* Bottom edge: (Left + 1, Bottom) to (Right, Bottom) */
    Clipped_Line(si_Left + 1, Bottom, Right, Bottom, (int)Color);

    /* Right edge: (Right, Top + 1) to (Right, Bottom) */
    Clipped_Line(Right, di_Top + 1, Right, Bottom, (int)Color);
}

// WZD s14p14
<<<<<<< HEAD
// drake178: 
// MoO2  
// UU_VGA_DrawDblRect()
/* COPILOT */
=======
/* GEMINI */
/**
 * @brief Draws a double-bordered rectangle using four independent edge colors.
 *
 * Renders two concentric rectangles to produce a 3D border effect (raised or sunken
 * panel appearance). Each rectangle has independently colored top/left and bottom/right
 * edges, allowing beveled highlight and shadow combinations.
 *
 * The outer rectangle spans (x1, y1) to (x2, y2). The inner rectangle is inset by one
 * pixel on all sides. The bottom and right edges of each rectangle are offset by one
 * pixel inward from the corner to avoid overdrawing the corner pixel already set by the
 * top/left pass.
 *
 * Edge assignment:
 * - Outer top    + left  : Color1
 * - Inner top    + left  : Color2
 * - Outer bottom + right : Color3
 * - Inner bottom + right : Color4
 *
 * @param x1     Left pixel coordinate of the outer rectangle.
 * @param y1     Top pixel coordinate of the outer rectangle.
 * @param x2     Right pixel coordinate of the outer rectangle.
 * @param y2     Bottom pixel coordinate of the outer rectangle.
 * @param Color1 Color for the top and left edges of the outer border.
 * @param Color2 Color for the top and left edges of the inner border.
 * @param Color3 Color for the bottom and right edges of the outer border.
 * @param Color4 Color for the bottom and right edges of the inner border.
 */
>>>>>>> origin/claude/upbeat-williams
void UU_VGA_DrawDblRect(int x1, int y1, int x2, int y2, int Color1, int Color2, int Color3, int Color4)
{
    int si_x1;
    int di_y1;

    si_x1 = x1;
    di_y1 = y1;

    /* Draw outer rectangle */
    /* Top edge: (x1, y1) to (x2, y1) */
    Line(si_x1, di_y1, x2, di_y1, Color1);
    /* Left edge: (x1, y1) to (x1, y2) */
    Line(si_x1, di_y1, si_x1, y2, Color1);
    /* Bottom edge: (x1+1, y2) to (x2, y2) */
    Line(si_x1 + 1, y2, x2, y2, Color3);
    /* Right edge: (x2, y1+1) to (x2, y2) */
    Line(x2, di_y1 + 1, x2, y2, Color3);

    /* Shrink coordinates for inner rectangle */
    si_x1++;
    di_y1++;
    x2--;
    y2--;

    /* Draw inner rectangle */
    /* Top edge: (si, di) to (x2, di) */
    Line(si_x1, di_y1, x2, di_y1, Color2);
    /* Left edge: (si, di) to (si, y2) */
    Line(si_x1, di_y1, si_x1, y2, Color2);
    /* Bottom edge: (si+1, y2) to (x2, y2) */
    Line(si_x1 + 1, y2, x2, y2, Color4);
    /* Right edge: (x2, di+1) to (x2, y2) */
    Line(x2, di_y1 + 1, x2, y2, Color4);
}


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
            // TODO  DLOG("switch(fill_type)  case 0:");
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
            // TODO  DLOG("switch(fill_type)  case 3:");
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
            // TODO  DLOG("switch(fill_type)  case 15:");
            Tint_Fill(x1, y1, x2, y2, remap_block);
        } break;
        case 16:
        {
            // TODO  DLOG("switch(fill_type)  case 16:");

        } break;
        default:
        {
            // TODO  DLOG("switch(fill_type)  default:");

        } break;
        
    }
}

// WZD s14p16
// drake178: DOS_PrintString()
// MoO2  DNE
// 1oom
/*
*/
/*

*/
void DOS_PrintString__STUB(char * string)
{
// mov     _SI_string, [bp+string]
// mov     dx, [bp+string]
// mov     ah, 9
// int     21h                             ; DOS - PRINT STRING
//                                         ; DS:DX -> string terminated by "$"
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
    int16_t temp = 0;  // _SI_
    int16_t x_delta = 0;  // _CX_
    int16_t y_delta = 0;  // _DX_
    int16_t line_increment = 0;
    int16_t length = 0;
    int16_t x_slope = 0;
    int16_t y_slope = 0;
    int16_t x_error = 0;
    int16_t y_error = 0;
    uint8_t * video_memory = 0;  // ES:DI

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
    uint8_t * Repeat_Mark = 0;
    int16_t line_increment = 0;
    int16_t x_slope = 0;
    int16_t length = 0;
    int16_t y_slope = 0;

    int16_t x_delta = 0;  // _CX_
    int16_t y_delta = 0;  // _DX_
    int16_t x_error = 0;
    int16_t y_error = 0;
    int16_t temp = 0;  // _SI_
    uint8_t * color_ptr = 0;  // _SI_
    uint8_t * video_memory = 0;  // ES:DI
    uint8_t color = 0;  // _AL_

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
// UU_VGA_CreateColorWave

// WZD s16p06
// UU_VGA_DiagColumns

// WZD s16p07
// 1oom  rnd.c  rnd_bitfiddle()
uint16_t rnd_bitfiddle__1oom(uint16_t ax)
{
    int16_t loops = 0;  // _CX_
    uint16_t bx;
    uint16_t dx;

    if (ax == 0) {
        return 0x35c8;
    }

    loops = 8;

    do {
        dx = ax;    // mov     dx, ax
        bx = ax;    // mov     bx, ax      ; using 1..16 indexing:
        bx >>= 1;   // shr     bx, 1
        ax ^= bx;   // xor     ax, bx      ; bit #15
        bx >>= 1;   // shr     bx, 1
        ax ^= bx;   // xor     ax, bx      ; bit #14
        bx >>= 1;   // shr     bx, 1
        bx >>= 1;   // shr     bx, 1
        ax ^= bx;   // xor     ax, bx      ; bit #12
        bx >>= 1;   // shr     bx, 1
        bx >>= 1;   // shr     bx, 1
        ax ^= bx;   // xor     ax, bx      ; bit #10
        bx >>= 1;   // shr     bx, 1
        ax ^= bx;   // xor     al, bh      ; bit #5

                    // shr     ax, 1
        dx >>= 1;   // rcr     dx, 1
        if(ax & 1) { dx |= 0x8000; }

        ax = dx;    // mov     ax, dx
    } while (--loops);
    return ax;
}


// WZD s16p08
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
// Gray_Scale_Fill

// WZD s16p10
// drake178: UU_VGA_Columns()
// MoO2  
// UU_VGA_Columns

// WZD s16p11
// drake178: UU_VGA_ScrambleRect()
// MoO2  
// UU_VGA_ScrambleRect
