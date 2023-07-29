
#include "MoX_TYPE.H"
#include "ST_DEF.H"
#include "seg021.H"

#include "ST_EMM.H"     /* fp_EMM_PFBA */
#include "ST_FLIC.H"    /* accessor macros; */
#include "ST_SA.H"
#include "ST_VGA.H"     /* p_Palette  */

#include <CONIO.H>      /* inp(), outp() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


// s21p01
/*
    
    VGA DAC Write
        0x03C8
    LODSB
    OUT DX,AL
    LODSB
    OUT DX,AL
    LODSB
    OUT DX,AL

_DS = sa_Palette
_SI = 0
N/A _ES = 
_DI = 768
*/
// 1oom :: uipal.c :: void ui_palette_set_n(void)
void VGA_DAC_Write(void)
{
    int ofstPalette;
    int ofstPaletteFlags;
    int itrVgaDacColors;
    int itrVgaDacWrites;
    int itrPaletteFlags;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] BEGIN: VGA_DAC_Write()\n", __FILE__, __LINE__);
// #endif

    ofstPalette = 0;
    ofstPaletteFlags = 768;
    itrVgaDacColors = 0;
    itrVgaDacWrites = 0;

//    dbg_prn("DEBUG: [%s, %d] ptr_Palette: %Fp\n", __FILE__, __LINE__, ptr_Palette);
//    dbg_prn("DEBUG: [%s, %d] ofstPalette: %d\n", __FILE__, __LINE__, ofstPalette);
//    dbg_prn("DEBUG: [%s, %d] ofstPaletteFlags: %d\n", __FILE__, __LINE__, ofstPaletteFlags);
//    dbg_prn("DEBUG: [%s, %d] itrVgaDacColors: %d\n", __FILE__, __LINE__, itrVgaDacColors);
//    dbg_prn("DEBUG: [%s, %d] itrVgaDacWrites: %d\n", __FILE__, __LINE__, itrVgaDacWrites);

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
            if ( p_Palette[ofstPaletteFlags] != 0 )
            {
                ofstPalette -= 3;
                //disable();  // asm CLI

                //outp( PALETTE_INDEX, 0 );
                outportb( 0x3C8, itrVgaDacColors );
                //outp( PALETTE_INDEX, palette[i] );
                outportb( 0x3C9, p_Palette[ofstPalette++] );
                outportb( 0x3C9, p_Palette[ofstPalette++] );
                outportb( 0x3C9, p_Palette[ofstPalette++] );

                --itrVgaDacWrites;
                //enable();  // asm STI
            }
//        }
        itrVgaDacColors++;
        ofstPaletteFlags++;
    } while ( itrVgaDacColors < 256 );

    // NOTE(JimBalcomb,20221110): Presently, this feels like where the palette is a done deal for whatever is being drawn, so the BMP BGRX can be saved/exported/dumped.



    ofstPaletteFlags = 768;
    for ( itrPaletteFlags = 0; itrPaletteFlags < 256; itrPaletteFlags++ )
    {
        p_Palette[ofstPaletteFlags++] = 0;
        // *(p_Palette + 768 + itrPaletteFlags) = 0;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] END: VGA_DAC_Write()\n", __FILE__, __LINE__);
// #endif

}
