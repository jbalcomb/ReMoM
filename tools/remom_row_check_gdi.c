/*
    remom_row_check_gdi -- the row check (remom_row_check.h) for GDI, the Win32 backend's present. PLAN-Aspect-Ratio.md phase 14.

    Draws the coordinate-encoding test image (a 320x200 top-down 32-bit DIB, like the Win32 back buffer) into each test rect with
    the same calls the Win32 present uses for integer and fit_nearest -- SetStretchBltMode(COLORONCOLOR), then StretchDIBits --
    into a memory DC holding a 32-bit DIB section, reads the DIB section's bits, and reports which game row and column each drawn
    row and column shows. No window, nothing shown on screen.

    (fit_sharp_bilinear draws the image at a whole-number scale first -- every game pixel exactly scale x scale pixels, so no rule
    is involved -- and then filters it with HALFTONE into the rect, so there is no nearest-neighbour row to check there.)

    Usage:
        remom_row_check_gdi

    Dev/QA tool, Windows only.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "remom_row_check.h"

int main(void)
{
    BITMAPINFO source_info;
    BITMAPINFO target_info;
    uint32_t * texels = NULL;
    void * target_bits = NULL;
    HDC screen_dc = NULL;
    HDC memory_dc = NULL;
    HBITMAP target_bitmap = NULL;
    HGDIOBJ old_bitmap = NULL;
    int case_index = 0;
    int failures = 0;

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("row check (GDI StretchDIBits, COLORONCOLOR): does it put each drawn row/column in the game pixel its centre falls in?\n");

    texels = (uint32_t *)malloc(sizeof(uint32_t) * ROW_CHECK_GAME_WIDTH * ROW_CHECK_GAME_HEIGHT);
    if(texels == NULL) { printf("out of memory\n"); return 1; }
    {
        int x, y;
        for(y = 0; y < ROW_CHECK_GAME_HEIGHT; y++)
        {
            for(x = 0; x < ROW_CHECK_GAME_WIDTH; x++)
            {
                texels[y * ROW_CHECK_GAME_WIDTH + x] = Row_Check_Texel(x, y);
            }
        }
    }

    memset(&source_info, 0, sizeof(source_info));
    source_info.bmiHeader.biSize = sizeof(source_info.bmiHeader);
    source_info.bmiHeader.biWidth = ROW_CHECK_GAME_WIDTH;
    source_info.bmiHeader.biHeight = -ROW_CHECK_GAME_HEIGHT;  /* top-down, as the Win32 back buffer */
    source_info.bmiHeader.biPlanes = 1;
    source_info.bmiHeader.biBitCount = 32;
    source_info.bmiHeader.biCompression = BI_RGB;

    target_info = source_info;
    target_info.bmiHeader.biWidth = ROW_CHECK_TARGET_WIDTH;
    target_info.bmiHeader.biHeight = -ROW_CHECK_TARGET_HEIGHT;

    screen_dc = GetDC(NULL);
    memory_dc = CreateCompatibleDC(screen_dc);
    target_bitmap = CreateDIBSection(screen_dc, &target_info, DIB_RGB_COLORS, &target_bits, NULL, 0);
    if(memory_dc == NULL || target_bitmap == NULL || target_bits == NULL) { printf("GDI setup failed (error %lu)\n", (unsigned long)GetLastError()); return 1; }
    old_bitmap = SelectObject(memory_dc, target_bitmap);

    for(case_index = 0; case_index < ROW_CHECK_CASE_COUNT; case_index++)
    {
        const struct row_check_case * c = &ROW_CHECK_CASES[case_index];
        int pixel;
        for(pixel = 0; pixel < ROW_CHECK_TARGET_WIDTH * ROW_CHECK_TARGET_HEIGHT; pixel++)
        {
            ((uint32_t *)target_bits)[pixel] = 0xFFFFFFFFu;
        }
        SetStretchBltMode(memory_dc, COLORONCOLOR);  /* as Win_Blit_Back_Buffer for integer and fit_nearest */
        StretchDIBits(memory_dc, c->x, c->y, c->w, c->h, 0, 0, ROW_CHECK_GAME_WIDTH, ROW_CHECK_GAME_HEIGHT, texels, &source_info, DIB_RGB_COLORS, SRCCOPY);
        GdiFlush();
        if(Row_Check_Report(c, (const uint32_t *)target_bits, ROW_CHECK_TARGET_WIDTH) == 0)
        {
            failures++;
        }
    }

    SelectObject(memory_dc, old_bitmap);
    DeleteObject(target_bitmap);
    DeleteDC(memory_dc);
    ReleaseDC(NULL, screen_dc);
    free(texels);
    printf("result: %s\n", (failures == 0) ? "GDI follows the centre rule" : "GDI did not follow the centre rule in at least one rect");
    return 0;
}
