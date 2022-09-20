// _s21p01c.c VGA_DAC_Write
// ST_VGA.H

#include <CONIO.H>  /* inp(), outp() */
#include <DOS.H>  /* disable(), enable() */

#include "ST_SA.H"
#include "ST_VGA.H"

#include "STU_DBG.H"

/*
    
    VGA DAC Write
        0x03C8
    LODSB
    OUT DX,AL
    LODSB
    OUT DX,AL
    LODSB
    OUT DX,AL

_DS = gsa_Palette
_SI = 0
N/A _ES = 
_DI = 768

*/

// 1oom :: uipal.c :: void ui_palette_set_n(void)
void VGA_DAC_Write(void)
{
    unsigned char _FAR *ptr_Palette;
    int ofstPalette;
    int ofstPaletteFlags;
    int itrVgaDacColors;
    int itrVgaDacWrites;
    int itrPaletteFlags;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_DAC_Write()\n", __FILE__, __LINE__);
#endif

    //*ptr_Palette = (unsigned char _FAR *)MK_FP(gsa_Palette, 0);
    //*ptr_Palette = SA_MK_FP0(gsa_Palette);
    ptr_Palette = (unsigned char _FAR *)MK_FP(gsa_Palette, 0);
    
    ofstPalette = 0;
    ofstPaletteFlags = 768;
    itrVgaDacColors = 0;
    itrVgaDacWrites = 0;

//    dlvfprintf("DEBUG: [%s, %d] ptr_Palette: %Fp\n", __FILE__, __LINE__, ptr_Palette);
//    dlvfprintf("DEBUG: [%s, %d] ofstPalette: %d\n", __FILE__, __LINE__, ofstPalette);
//    dlvfprintf("DEBUG: [%s, %d] ofstPaletteFlags: %d\n", __FILE__, __LINE__, ofstPaletteFlags);
//    dlvfprintf("DEBUG: [%s, %d] itrVgaDacColors: %d\n", __FILE__, __LINE__, itrVgaDacColors);
//    dlvfprintf("DEBUG: [%s, %d] itrVgaDacWrites: %d\n", __FILE__, __LINE__, itrVgaDacWrites);

    do
    {
        if ( itrVgaDacWrites == 0 )
        {
//            HERE("( itrVgaDacWrites == 0 )");
            itrVgaDacWrites = 128;
            // @@waitnosync
            //while( inp(INPUT_STATUS) & VRETRACE );
            do {} while( inport(0x3DA) & 0x08 );
            // @@waitsync
            //while( ! inp(INPUT_STATUS) & VRETRACE );
            do {} while( ! inport(0x3DA) & 0x08 );
        }
//        else
//        {
            //HERE("( itrVgaDacWrites != 0 )");
            ofstPalette += 3;
            if ( ptr_Palette[ofstPaletteFlags] != 0 )
            {
                ofstPalette -= 3;
                //disable();  // asm CLI

                //outp( PALETTE_INDEX, 0 );
                outportb( 0x3C8, itrVgaDacColors );
                //outp( PALETTE_INDEX, palette[i] );
                outportb( 0x3C9, ptr_Palette[ofstPalette++] );
                outportb( 0x3C9, ptr_Palette[ofstPalette++] );
                outportb( 0x3C9, ptr_Palette[ofstPalette++] );

                --itrVgaDacWrites;
                //enable();  // asm STI
            }
//        }
        itrVgaDacColors++;
        ofstPaletteFlags++;
    } while ( itrVgaDacColors < 256 );

    ofstPaletteFlags = 768;
    for ( itrPaletteFlags = 0; itrPaletteFlags < 256; itrPaletteFlags++ )
    {
        ptr_Palette[ofstPaletteFlags++] = 0;
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_DAC_Write()\n", __FILE__, __LINE__);
#endif
}
