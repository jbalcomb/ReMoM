
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

// s21p07
void FLIC_Load_Palette(SAMB_ptr fp_FLIC_Header, int Frame_Index)
{
    SAMB_ptr fp_FLIC_File;
    unsigned int Color_Index;
    unsigned int Color_Count;
    byte_ptr flic_palette_data;
    unsigned int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Load_Palette(fp_FLIC_Header = %p, Frame_Index = %d)\n", __FILE__, __LINE__, fp_FLIC_Header, Frame_Index);
#endif

    if ( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header) == 0 )
    {
        DLOG("( fp_FLIC_Header->EMM_Handle_Number == 0 )");
        fp_FLIC_File = (SAMB_ptr) MK_PTR(fp_FLIC_Header, 0);
    }
    else
    {
        DLOG("( fp_FLIC_Header->EMM_Handle_Number != 0 )");
        EMM_MAP_PAGE(0, FLIC_Get_EMM_Handle_Number(fp_FLIC_Header), FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_Header));
        fp_FLIC_File = (SAMB_ptr) MK_PTR(fp_EMM_PFBA, FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_Header));
    }

    if ( (Frame_Index == 0) || (FLIC_Get_Frame_Palettes(fp_FLIC_File) == 0) )
    {
        DLOG("( (frame_index == 0) || (frame_palettes == 0) )");
        Color_Index = FLIC_Get_Palette_Color_Start(fp_FLIC_File);
        Color_Count = FLIC_Get_Palette_Color_Count(fp_FLIC_File);
        flic_palette_data = ( fp_FLIC_File + FLIC_Get_Palette_Data_Offset(fp_FLIC_File) );
    }
    else
    {
        DLOG("( (frame_index != 0) || (frame_palettes != 0) )");
        flic_palette_data = FLIC_Get_Frame_Palette_Data_Offset(fp_FLIC_File,Frame_Index);  // ? nonportable pointer conversion ?
        Color_Index = FLIC_Get_Frame_Palette_Color_Index(fp_FLIC_File,Frame_Index);
        Color_Count = FLIC_Get_Frame_Palette_Color_Count(fp_FLIC_File,Frame_Index);
    }

    for(itr = Color_Index; itr < Color_Count; itr++)
    {
        *(p_Palette + itr) = *(flic_palette_data + itr);
        *(p_Palette + 768 + itr) = 1;
    }

    for(itr = 0; itr < Color_Count; itr++)
    {
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 3) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 0) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 2) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 1) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 1) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 2) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 0) = 0x00;
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 3) = 0x00;
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 2) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 0) << 2);
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 1) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 1) << 2);
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 0) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 2) << 2);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Load_Palette(fp_FLIC_Header = %p, Frame_Index = %d)\n", __FILE__, __LINE__, fp_FLIC_Header, Frame_Index);
#endif

}
