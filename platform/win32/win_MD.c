/**
 * win_MD.c — Win32 mouse handling (platform-level only).
 *
 * Game-level Mouse_Button(), Mouse_Movement_Handler(), Mouse_Button_Handler(),
 * User_Mouse_Handler(), and Set_Mouse_Position() live in MoX/src/Mouse.c.
 * Live mouse events are handled in Win_Window_Proc (win_PFL.cpp).
 *
 * This file carries the Win32 hardware-cursor support, mirroring
 * platform/sdl2/sdl2_MD.c.  Baseline (doc/#AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md)
 * showed the software cursor only reaches the screen at the ~18 fps present
 * cadence.  Here we build a real HCURSOR from the current game cursor sprite so
 * the OS/compositor tracks it at native rate, decoupled from the framebuffer
 * present -- the modern equivalent of the DOS hardware sprite.  Opt-in via
 * REMOM_HW_CURSOR=1; when off, Active() returns 0 and every hook is a no-op
 * branch (identical behaviour to the software cursor).
 */

#include "../../platform/include/Platform.h"
#include "win_PFL.h"   /* Windows.h + extern "C" guard */

#include <stdlib.h>  /* CLAUDE: getenv() for the HW-cursor gate */
#include <string.h>  /* CLAUDE: strcmp(), memset() */

/* ========================================================================= */
/*  CLAUDE: Hardware-cursor (Platform-Input follow-on), Win32 port.          */
/* ========================================================================= */

/* Sprite geometry for the shipped 320x200 build.  These mirror MOM_DEF.h
   CURSOR_WIDTH/HEIGHT and MOX_DEF.h ST_TRANSPARENT (same as sdl2_MD.c); a
   production version should thread those macros through the interface rather
   than restating them.  Both cursor dimensions are 16, transparent index is 0. */
#define HWCUR_DIM          16
#define HWCUR_TRANSPARENT  0

static HCURSOR hwcur_current    = NULL;
static int     hwcur_gate       = -1;   /* -1 = unresolved, else 0/1 from REMOM_HW_CURSOR */
static int     hwcur_last_image = -1;

int Platform_HW_Cursor_Active(void)
{
    if (hwcur_gate < 0)
    {
        const char * env = getenv("REMOM_HW_CURSOR");
        hwcur_gate = (env != NULL && env[0] != '\0' && strcmp(env, "0") != 0) ? 1 : 0;
    }
    return hwcur_gate;
}

/* Re-assert our cursor.  Called from WM_SETCURSOR: Windows resets the window to
   the class cursor (NULL here) on every mouse move over the client area, which
   would erase the SetCursor() done in Refresh, so it must be set again there. */
void Win_HW_Cursor_Apply(void)
{
    if (!Platform_HW_Cursor_Active()) { return; }
    SetCursor(hwcur_current);
}

void Platform_HW_Cursor_Refresh(void)
{
    int             image;
    int             scale;
    int             dim;
    int             px, py, sx, sy;
    const uint8_t * sprite;
    BITMAPV5HEADER  bi;
    HDC             screen_dc;
    void *          bits = NULL;
    uint32_t *      dst;
    HBITMAP         color_bmp;
    HBITMAP         mask_bmp;
    ICONINFO        ii;
    HCURSOR         newcur;

    if (!Platform_HW_Cursor_Active()) { return; }

    image = Get_Pointer_Image_Number();
    if (image == hwcur_last_image) { return; }   /* no shape change since the last build */
    hwcur_last_image = image;

    if (image == 0)   /* crsr_None -> no cursor */
    {
        SetCursor(NULL);   /* hide before destroying the handle that was active */
        if (hwcur_current != NULL) { DestroyIcon(hwcur_current); hwcur_current = NULL; }
        return;
    }
    if (mouse_palette == NULL)   /* sprite bank not loaded yet (pre-Load_Palette) -- retry next frame */
    {
        hwcur_last_image = -1;
        return;
    }

    scale = Platform_Get_Scale();
    if (scale < 1) { scale = 1; }
    dim = HWCUR_DIM * scale;

    /* 32-bit top-down BGRA DIB with per-pixel alpha (BI_BITFIELDS masks match the
       (a<<24)|(r<<16)|(g<<8)|b word written below). */
    memset(&bi, 0, sizeof(bi));
    bi.bV5Size        = sizeof(BITMAPV5HEADER);
    bi.bV5Width       = dim;
    bi.bV5Height      = -dim;   /* negative = top-down */
    bi.bV5Planes      = 1;
    bi.bV5BitCount    = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask     = 0x00FF0000;
    bi.bV5GreenMask   = 0x0000FF00;
    bi.bV5BlueMask    = 0x000000FF;
    bi.bV5AlphaMask   = 0xFF000000;

    screen_dc = GetDC(NULL);
    color_bmp = CreateDIBSection(screen_dc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, &bits, NULL, 0);
    ReleaseDC(NULL, screen_dc);
    if (color_bmp == NULL || bits == NULL)
    {
        if (color_bmp != NULL) { DeleteObject(color_bmp); }
        return;
    }

    sprite = mouse_palette + ((image - 1) * (HWCUR_DIM * HWCUR_DIM));
    dst = (uint32_t *)bits;

    for (py = 0; py < HWCUR_DIM; py++)
    {
        for (px = 0; px < HWCUR_DIM; px++)
        {
            uint8_t  idx = sprite[px * HWCUR_DIM + py];   /* column-major, matching Draw_Mouse_On_Page_ */
            uint32_t argb;
            if (idx == HWCUR_TRANSPARENT)
            {
                argb = 0x00000000u;
            }
            else
            {
                PFL_Color c = platform_palette_buffer[idx];
                /* NOTE: on Win32, platform_palette_buffer is filled XBGR-swapped -- .r is loaded from
                   the palette's blue byte and .b from its red byte (Platform_Set_Palette_Color /
                   Palette_Update, win_PFL.cpp), so the raw bytes line up for StretchDIBits (see the
                   "layout-compatible" note in Win_Convert_Engine_Pixels_To_Back_Buffer).  Our DIB's red
                   channel therefore comes from c.b and blue from c.r -- the opposite of sdl2_MD.c, whose
                   SDL2 palette fills .r with true red.  Getting this wrong renders gold as blue. */
                argb = ((uint32_t)0xFFu << 24) | ((uint32_t)c.b << 16) | ((uint32_t)c.g << 8) | (uint32_t)c.r;
            }
            /* nearest-neighbour upscale by the window scale so the OS cursor matches the drawn size */
            for (sy = 0; sy < scale; sy++)
            {
                for (sx = 0; sx < scale; sx++)
                {
                    dst[(py * scale + sy) * dim + (px * scale + sx)] = argb;
                }
            }
        }
    }

    /* Monochrome AND mask.  Unused for blending because the color bitmap carries
       per-pixel alpha, but CreateIconIndirect still requires a mask of matching
       size; an all-zero mask lets the alpha channel decide every pixel. */
    mask_bmp = CreateBitmap(dim, dim, 1, 1, NULL);
    if (mask_bmp == NULL)
    {
        DeleteObject(color_bmp);
        return;
    }

    memset(&ii, 0, sizeof(ii));
    ii.fIcon    = FALSE;   /* a cursor, not an icon */
    ii.xHotspot = 0;       /* hotspot (0,0): the engine hit-tests off the sprite top-left */
    ii.yHotspot = 0;
    ii.hbmMask  = mask_bmp;
    ii.hbmColor = color_bmp;

    newcur = (HCURSOR)CreateIconIndirect(&ii);
    DeleteObject(color_bmp);
    DeleteObject(mask_bmp);

    if (newcur != NULL)
    {
        SetCursor(newcur);
        if (hwcur_current != NULL) { DestroyIcon(hwcur_current); }
        hwcur_current = newcur;
    }
}
