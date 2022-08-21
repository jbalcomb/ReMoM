
/*
    IBM-PC VGA ==> Win32 GDI

    IBM-PC VGA:
    VGA DAC Palette
    256 Colors, 6 bits-per-pixel, {R,B,G}

    Win32 GDI:
    DIB_PAL_COLORS
    256 colors, 8 bits-per-pixel, {x,B,G,R}
    RGBQUAD {BYTE rgbBlue; BYTE rgbGreen; BYTE rgbRed; BYTE rgbReserved}

    ???
        convert VGA RAM & VGA DAC PAL to 24-bit Bitmap; memcpy(); StretchDIBits();

        MoM-DOS
            use MoM's SRAM shadow
            add draw code for VRAM shadow - #ifdef WIN32
        convert to 8bpp
        build 24-bit BMP/DIB/DDB
        memcpy() to StretchDIBits(.., &BitmapMemory, ...);
        ...
        since we're displaying our bitmap on screen using StretchDIBits
            don't need a bitmap handle or bitmap DC or even a DIB section
            Bitmap Memory can be allocated directly by us.
    ???

*/

