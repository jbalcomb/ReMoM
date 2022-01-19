// STU_VGA.C
// STU_VGA.H
// SEE: VGA_DAC.md
// http://digiassn.blogspot.com/2006/04/cdos-video-programming-with-turbo-c.html

#include <DOS.H>
#include <STDIO.H>

/*

maybe, maybe not VGA Mode 'Mode-Y' specific

MoM
_s21p01c.c  VGA_DAC_Write()

*/
/*

    VGA Ports:

    PALETTE_INDEX   0x3C8
    PALETTE_DATA    0x3C9

    INPUT_STATUS
    VRETRACE
*/
/*
    Vertical Retrace / Vertical Blank / Display Enable

*/
/*
    VGA DAC
    256 colors
    RGB
    3 bytes per color

*/
/*

PEL Address Read Mode Register (port 3C7h)
PEL Address Write Mode Register (port 3C8h)
PEL Data Register (port 3C9h)

the PEL Address Read Mode Register (port 3C7h) is used to specify the palette entry to be read
the values are read from the PEL Data Register
the PEL Address Read Mode Register auto-increments after each triplet is read
...
Output the value of the first color entry to be read to the PEL Address Read Mode Register.
Read the PEL Data Register to obtain the red component value.
Read the PEL Data Register to obtain the green component value.
Read the PEL Data Register to obtain the blue component value.
If more colors are to be read, repeat steps 4-6.
...
*/

void STU_VGA_DAC_Read(unsigned char *palette)
{
    unsigned int itr;

    outport( 0x3C7, 0 );

    for ( itr = 0; itr < 768; itr++ )
    {
        palette[itr] = inportb( 0x3C9 );
    }
}

void STU_VGA_DAC_Write(unsigned char *palette)
{
    unsigned int itr;

    outport( 0x3C8, 0 );

    for ( itr = 0; itr < 768; itr++ )
    {
        outport( 0x3C9, palette[itr] );
    }
}

void STU_VGA_DAC_Dump(char *PaletteFilename)
{
    unsigned int itr;
    unsigned char palette[768];
    //char PaletteFilename[13] = "VGADAC.BIN";
    FILE *fdPaletteFile;

    fdPaletteFile = fopen(PaletteFilename, "wb");

    outport( 0x3C7, 0 );

    //STU_VGA_DAC_Read(&palette);
    for ( itr = 0; itr < 768; itr++ )
    {
        palette[itr] = inportb( 0x3C9 );
    }

    fwrite(palette, 768, 1, fdPaletteFile);

    fclose(fdPaletteFile);
    
}

// 6bpp, 8bpp
void DAC2PAL(unsigned char *DAC, unsigned char *PAL)
{
    unsigned int itr;
    unsigned char dac6bpp;
    unsigned char pal8bpp;
    
    for ( itr = 0; itr < 768; itr++ )
    {
        dac6bpp = DAC[itr];
        pal8bpp = (dac6bpp << 2);
        PAL[itr] = pal8bpp;
    }

}

// 8bpp, 6bpp
void PAL2DAC(unsigned char *PAL, unsigned char *DAC)
{

}
