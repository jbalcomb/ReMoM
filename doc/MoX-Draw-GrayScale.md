
Meh.
Accidentally got it working...
...no idea at which point I had it fixed
...4+ idiotic bugs
Try again, someday, maybe ... color space interpolation intensity/lightness, because in RGB space



has already
    drawn the banner colored background to the off-screen video buffer
    copied the Unit Figure to the unit draw workarea buffer
    replaced the banner color indices in the unit draw workarea buffer

calls Gray_Scale_Bitmap() with color_start = 1



Intensity_Scale_Tbl@ is allocated in Load_Font_File()
which only happens once
it is 96 PR 1536 B
there is no start-up data - nothing gets loaded / put into it



calculating its total intensity,
scaling it down to 4 bits
and adding the color index passed to the function,
which should indicate the start of an array of 16 colors in the palette that are increasing intensities of one specific color hue
 (does not necessarily have to be gray)


// DS:SI  src  p_Palette[0]
// ES:DI  dst  Intensity_Scale_Tbl@[0]

loops through all 256 colors in the current shadow palette
    ...LODSB, LODSB, LODSB...loads 768 bytes
    ...STOSB...stores 256 bytes

e.g.,
8,4,4
00001000
00000100
00000100
00001000 + (00001000 > 1) = 00001000 + 00000100 = 00001100  8
00000100 + (00000100 > 1) = 00000100 + 00000010 = 00000110  6
00000100                  = 00000100            = 00000100  4
8 + 6 + 4 = 00010010
00010010 >> 4 = 00000001
00000001 + 1 = 00000010


Then, ...

loops through 'pict_size' bytes...
// DS:SI  src  Intensity_Scale_Tbl@[16]
// ES:DI  dst  pict_seg[sizeof(header)]

