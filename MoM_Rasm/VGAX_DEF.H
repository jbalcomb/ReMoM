
/*
    VGA
    "Unchained" / "Tweaked"
        turn off chain-4 mode
        turn off long mode
        turn on byte mode
    "Mode X"
        320 x 240, 256 color, 256 KB VRAM
        4:3, square
    "Mode Y"
        320 x 200, 256 color, 256 KB VRAM
        8:5, non-square
*/

/*
dword * ptr = (dword *)VGA;             / * used for faster screen clearing * /

outp(SC_INDEX,  MEMORY_MODE);           / * turn off chain-4 mode * /
outp(SC_DATA,   0x06);

outpw(SC_INDEX, ALL_PLANES);            / * set map mask to all 4 planes * /
for(i = 0; i < 0x4000; i++)             / * clear all 256K of memory * /
    *ptr++ = 0;

outp(CRTC_INDEX,UNDERLINE_LOCATION);    / * turn off long mode * /
outp(CRTC_DATA, 0x00);

outp(CRTC_INDEX,MODE_CONTROL);          / * turn on byte mode * /
outp(CRTC_DATA, 0xe3);
*/
