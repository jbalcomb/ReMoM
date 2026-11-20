/*
    remom_row_check.h -- the shared half of the "row check" (PLAN-Aspect-Ratio.md phases 11, 13 and 14).

    Which drawable rows and columns does a backend's NEAREST scaling give to which game pixels? At 4:3 the game's 200 rows are
    stretched to 240k drawable rows, so rows are uneven, and the Viewport's mouse mapping has to put every drawable row in the
    game row the backend drew there, or clicks land a row off. Two candidate rules:
      centre -- a drawable pixel shows the game pixel its centre falls in:            floor((p + 0.5 - rect) * game / size)
      edge   -- a drawable pixel shows the game pixel its top/left edge falls in:     floor((p - rect) * game / size)
    The Viewport uses the centre rule (an exact tie goes to the later game pixel).

    Each tool draws a 320x200 image whose every pixel encodes its own x and y (R = x & 255, B = x >> 8, G = y) into each of the
    rects below, reads the result back as 32-bit 0xAARRGGBB, and calls Row_Check_Report() for each rect. Used by:
      remom_video_probe --row-check    SDL2 render drivers
      remom_row_check_sdl3             SDL3 render drivers
      remom_row_check_gdi              GDI StretchDIBits with COLORONCOLOR (the Win32 backend's present)
*/

#ifndef REMOM_ROW_CHECK_H
#define REMOM_ROW_CHECK_H

#include <stdint.h>
#include <stdio.h>

#define ROW_CHECK_GAME_WIDTH     320
#define ROW_CHECK_GAME_HEIGHT    200
#define ROW_CHECK_TARGET_WIDTH   2048
#define ROW_CHECK_TARGET_HEIGHT  1280

struct row_check_case
{
    const char * name;
    int x, y, w, h;
};

static const struct row_check_case ROW_CHECK_CASES[] = {
    { "4:3 integer x4    1280x960 at 0,20",    0,  20, 1280,  960 },
    { "4:3 integer x3     960x720 at 160,40", 160,  40,  960,  720 },
    { "4:3 integer x5   1600x1200 at 0,0",      0,   0, 1600, 1200 },
    { "4:3 fit          1440x1080 at 240,0",  240,   0, 1440, 1080 },
    { "4:3 fit          1067x800  at 106,0",  106,   0, 1067,  800 },
    { "16:10 fit        1728x1080 at 96,0",    96,   0, 1728, 1080 },
};
#define ROW_CHECK_CASE_COUNT ((int)(sizeof(ROW_CHECK_CASES) / sizeof(ROW_CHECK_CASES[0])))

/* The texel the test image holds at game pixel (x, y). */
static uint32_t Row_Check_Texel(int x, int y)
{
    return 0xFF000000u | ((uint32_t)(x & 255) << 16) | ((uint32_t)y << 8) | (uint32_t)(x >> 8);
}

static int Row_Check_Floor_Divide(long long dividend, long long divisor)
{
    long long quotient = dividend / divisor;
    if((dividend % divisor != 0) && (dividend < 0)) { quotient--; }
    return (int)quotient;
}

/* The rules in exact integers: centre = floor((2(p - rect) + 1) * game / (2 size)), edge = floor((p - rect) * game / size). */
static int Row_Check_Centre(int p, int rect, int size, int game) { return Row_Check_Floor_Divide(((long long)2 * (p - rect) + 1) * game, (long long)2 * size); }
static int Row_Check_Edge(int p, int rect, int size, int game) { return Row_Check_Floor_Divide((long long)(p - rect) * game, size); }

/* Check one rect: rows down its middle column, columns along its middle row. pixels is the whole target, 0xAARRGGBB, pitch in
   pixels. Prints one line. Returns 1 if every row and column follows the centre rule. */
static int Row_Check_Report(const struct row_check_case * c, const uint32_t * pixels, int pitch)
{
    int rows_centre = 0, rows_edge = 0, rows_neither = 0, cols_centre = 0, cols_edge = 0, cols_neither = 0;
    int first_row_mismatch = -1, first_col_mismatch = -1;
    int p;

    for(p = c->y; p < c->y + c->h; p++)
    {
        uint32_t pixel = pixels[(long long)p * pitch + (c->x + c->w / 2)];
        int drawn = (int)((pixel >> 8) & 255);
        int centre = Row_Check_Centre(p, c->y, c->h, ROW_CHECK_GAME_HEIGHT);
        int edge = Row_Check_Edge(p, c->y, c->h, ROW_CHECK_GAME_HEIGHT);
        if(drawn == centre) { rows_centre++; }
        if(drawn == edge) { rows_edge++; }
        if(drawn != centre && drawn != edge) { rows_neither++; }
        if(drawn != centre && first_row_mismatch < 0) { first_row_mismatch = p; }
    }
    for(p = c->x; p < c->x + c->w; p++)
    {
        uint32_t pixel = pixels[(long long)(c->y + c->h / 2) * pitch + p];
        int drawn = (int)(((pixel >> 16) & 255) | ((pixel & 255) << 8));
        int centre = Row_Check_Centre(p, c->x, c->w, ROW_CHECK_GAME_WIDTH);
        int edge = Row_Check_Edge(p, c->x, c->w, ROW_CHECK_GAME_WIDTH);
        if(drawn == centre) { cols_centre++; }
        if(drawn == edge) { cols_edge++; }
        if(drawn != centre && drawn != edge) { cols_neither++; }
        if(drawn != centre && first_col_mismatch < 0) { first_col_mismatch = p; }
    }
    printf("    %-38s rows %4d: centre %4d edge %4d neither %3d | cols %4d: centre %4d edge %4d neither %3d", c->name, c->h, rows_centre, rows_edge, rows_neither, c->w, cols_centre, cols_edge, cols_neither);
    if(first_row_mismatch >= 0 || first_col_mismatch >= 0) { printf("  (first non-centre row %d, col %d)", first_row_mismatch, first_col_mismatch); }
    printf("\n");
    return (rows_centre == c->h) && (cols_centre == c->w);
}

#endif /* REMOM_ROW_CHECK_H */
