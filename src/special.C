/*
    WIZARDS.EXE
        seg023

    Moo2
        Module: special
            Interpolate_Line()
            Absolute_Interpolate_Line()
            Range()
            Delta_XY_With_Wrap()
            Path()
            Path_Wrap()
            isqrt()

*/

#include "MoX.H"



/*
    WIZARDS.EXE seg023
*/

// WZD s23p01
// UU_VGA_GetPointInDir()

// WZD s23p02
// TILE_GetDistance()

// WZD s23p03
// MoO2
// 1oom
/*
    calculate the distance between {x1,y1} and {x2,y2}
; returns a non-euclidean distance between two tiles,
; using the larger of the X and Y distances, but
; accounting for the map wrapping around (with Width)
*/
int16_t Delta_XY_With_Wrap(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t wrap_x)
{
    int16_t delta_x1;
    int16_t delta_x2;
    int16_t delta_y;
    int16_t range;

    delta_x1 = x2 - x1;
    delta_y = y2 - y1;

    // TODO  AbsVal(delta_x1);
    if(delta_x1 < 0)
    {
        delta_x1 *= -1;
    }

    if(delta_y < 0)
    {
        delta_y *= -1;
    }

    if(x2 < x1)
    {
        delta_x2 = x2 + wrap_x - x1;
    }
    else
    {
        delta_x2 = x2 - wrap_x - x1;
    }

    if(delta_x2 < 0)
    {
        delta_x2 *= -1;
    }

    if(delta_x1 > delta_x2)
    {
        delta_x1 = delta_x2;
    }
    
    if(delta_x1 > delta_y)
    {
        range = delta_x1;
    }
    else
    {
        range = delta_y;
    }

    return range;
}


// WZD s23p04
// drake178: UU_Tile_PlotLine()
// MoO2  Module: special  Path()
// 1oom  util_math.c  util_math_line_plot()
/*
Unused in MoM


*/
int Path(int x1, int y1, int x2, int y2, int *tblx, int *tbly)
{

}

int util_math_line_plot(int x1, int y1, int x2, int y2, int *tblx, int *tbly)
{
    int len = 0, delta_x, delta_y, dirx, diry, delta_add, zerr = 0x8000;

    delta_x = x2 - x1;
    if (delta_x < 0) {
        delta_x = -delta_x;
        dirx = -1;
    } else {
        dirx = 1;
    }
    delta_y = y2 - y1;
    if (delta_y < 0) {
        delta_y = -delta_y;
        diry = -1;
    } else {
        diry = 1;
    }
    if (delta_x < delta_y) {
        delta_add = (delta_x << 16) / delta_y;
        while (len < delta_y) {
            y1 += diry;
            zerr += delta_add;
            if (zerr >= 0x10000) {
                x1 += dirx;
            }
            zerr &= 0xffff;
            tblx[len] = x1;
            tbly[len] = y1;
            ++len;
        }
    } else if (delta_y < delta_x) {
        delta_add = (delta_y << 16) / delta_x;
        while (len < delta_x) {
            x1 += dirx;
            zerr += delta_add;
            if (zerr >= 0x10000) {
                y1 += diry;
            }
            zerr &= 0xffff;
            tblx[len] = x1;
            tbly[len] = y1;
            ++len;
        }
    } else {
        while (len < delta_y) {
            x1 += dirx;
            y1 += diry;
            tblx[len] = x1;
            tbly[len] = y1;
            ++len;
        }
    }
    return len;
}

// WZD s23p05
// drake178: TILE_PlotMapLine()
// 1oom util_math_get_route_len()
/*
MoO2
Module: special
    function (0 bytes) Path_Wrap
    Address: 01:0013498E
        Num params: 4
        Return type: signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) x1
            signed integer (2 bytes) y1
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            pointer (4 bytes) path_string_x
            pointer (4 bytes) path_string_y
            signed integer (2 bytes) MAP_X
            signed integer (4 bytes) length
            signed integer (4 bytes) delta_x
            signed integer (4 bytes) delta_y
            signed integer (4 bytes) sptr
            signed integer (4 bytes) sign_x
            signed integer (4 bytes) sign_y
            signed integer (4 bytes) cur_x
            signed integer (4 bytes) cur_y
            unsigned integer (4 bytes) remainder
            unsigned integer (4 bytes) delta_add
            signed integer (4 bytes) i
            signed integer (4 bytes) base_delta_x

*/
/*
IDGI

mov     ax, [bp+error_term]
add     ax, [bp+delta_add]
jnb     short loc_1E3CA  
dec     [bp+cur_y]
loc_1E3CA:
mov     [bp+error_term], ax

1oom
    remainder += delta_add;
    if (remainder >= 0x10000) {
        y0 += sign_y;
    }
    remainder &= 0xffff;

*/
/*
; plots a "line" between two tile coordinates using
; Bresenham's line-drawing algorithm, putting the X and
; Y coordinates of the resulting array of tiles into
; two separate return (byte) arrays
; returns the length of the array (tile distance)
*/
int16_t Path_Wrap(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t * path_string_x, int8_t * path_string_y, int16_t length)
{
    int32_t remainder;  // int16_t in Dasm  (can't match JNB / `test Carry Flag`)
    int32_t delta_add;  // int16_t in Dasm  (can't match JNB / `test Carry Flag`)
    int16_t cur_y;
    int16_t cur_x;
    int16_t sign_y;
    int16_t sign_x;
    int16_t delta_y;
    int16_t delta_x;
    int16_t distance;  // _SI_
    int16_t itr;  // _DI_

    if(abs(length - x2 + x1) < abs(x2 - x1))
    {
        x2 -= length;
    }

    if(abs(length - x1 + x2) < abs(x2 - x1))
    {
        x2 += length;
    }

    delta_x = (x2 - x1);
    delta_y = (y2 - y1);

    if(delta_x < 0)
    {
        delta_x = -(delta_x);
        sign_x = -1;
    }
    else
    {
        sign_x = 1;
    }

    if(delta_y < 0)
    {
        delta_y = -(delta_y);
        sign_y = -1;
    }
    else
    {
        sign_y = 1;
    }

    distance = 0;

    cur_x = x1;
    cur_y = y1;

    if(delta_x < delta_y)
    {
        remainder = 0x8000;
        delta_add = (delta_x << 16) / delta_y;
        if(sign_x == 1)
        {
            // process octants 1 & 6 (delta_x < delta_y; sign_x = 1)
            while(distance < delta_y)
            {
                cur_y += sign_y;
                remainder += delta_add;
                if(remainder >= 0x10000)
                {
                    cur_x++;  // move right
                }
                remainder &= 0xFFFF;
                path_string_x[distance] = cur_x;
                path_string_y[distance] = cur_y;
                distance++;
            }
        }
        else
        {
            // process octants 2 & 5 (delta_x < delta_y; sign_x = -1)
            while(distance < delta_y)
            {
                cur_y += sign_y;
                remainder += delta_add;
                if(remainder >= 0x10000)
                {
                    cur_x--;  // move left
                }
                remainder &= 0xFFFF;
                path_string_x[distance] = cur_x;
                path_string_y[distance] = cur_y;
                distance++;
            }
        }
    }
    else if(delta_y < delta_x)
    {
        remainder = 0x8000;
        delta_add = (delta_y << 16) / delta_x;
        if(sign_y == 1)
        {
            // process octants 0 & 7 (delta_x > delta_y; sign_y = 1)
            while(distance < delta_x)
            {
                cur_x += sign_x;
                remainder += delta_add;
                if(remainder >= 0x10000)
                {
                    cur_y++;  // move down
                }
                remainder &= 0xFFFF;
                path_string_x[distance] = cur_x;
                path_string_y[distance] = cur_y;
                distance++;
            }
        }
        else
        {
            // process octants 3 & 4 (delta_x > delta_y; sign_y = -1)
            while(distance < delta_x)
            {
                cur_x += sign_x;
                remainder += delta_add;
                if(remainder >= 0x10000)
                {
                    cur_y--;  // move up
                }
                remainder &= 0xFFFF;
                path_string_x[distance] = cur_x;
                path_string_y[distance] = cur_y;
                distance++;
            }
        }
    }
    else
    {
        // process diagonals (delta_x = delta_y)
        while(distance < delta_y)
        {
            cur_y += sign_y;
            cur_x += sign_x;
            path_string_x[distance] = cur_x;
            path_string_y[distance] = cur_y;
            distance++;
        }
    }

    for(itr = 0; itr < distance; itr++)
    {
        if(path_string_x[itr] < 0)
        {
            path_string_x[itr] += length;
        }
    }

    for(itr = 0; itr < distance; itr++)
    {
        if(path_string_x[itr] >= length)
        {
            path_string_x[itr] -= length;
        }
    }

    return distance;
}

static inline int util_math_route_step_len(int adx, int ady)
{
    int v = adx + ady;
    return v ? (v + 1) : 3;
}

int util_math_get_route_len(int x0, int y0, const int *tblx, const int *tbly, int len)
{
    int l;
    l = util_math_route_step_len(abs(x0 - tblx[0]), abs(y0 - tbly[0]));
    for (int i = 1; i < len; ++i) {
        l += util_math_route_step_len(abs(tblx[i - 1] - tblx[i]), abs(tbly[i - 1] - tbly[i]));
    }
    return l;
}
