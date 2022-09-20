
#include <CONIO.H>  /* inp(), outp() */
#include <DOS.H>  /* disable(), enable() */

#include "seg021.H"

#include "ST_HEAD.H"
#include "ST_TYPE.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_SA.H"
#include "ST_VGA.H"

#include "STU_BITS.H"
#include "STU_DBG.H"


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

// s21p07
/*
    DS:SI
    ES:DI
    Segment:Offset
*/
void FLIC_LoadPalette(SAMB_addr FlicHdr_SgmtAddr, int Frame_Index)
{
    struct s_FLIC_HDR _FAR * fptr_FlicHdr_SgmtAddr;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned int tmp_SrcOfst;
    unsigned int _FAR *fptr_Dst;
    unsigned int _FAR *fptr_Src;
    unsigned char _FAR *fptr_DstByte;
    unsigned char _FAR *fptr_SrcByte;
    unsigned int tmp_Palette_Header_Offset;
    // unsigned int  fh_image_offset;
    // unsigned int  fh_first_color;
    // unsigned int  fh_color_count;
    unsigned int fh_palette_data_offset;
    unsigned int fh_palette_color_index;
    unsigned int fh_palette_color_count;
    byte fh_frames_have_palettes;
    unsigned int  ffh_image_offset;
    unsigned char ffh_colors_hi_lo;
    unsigned char ffh_first_color;
    unsigned char ffh_color_count;
    unsigned int tmp_Color_Count;
    unsigned int ofstPaletteFlags;
    unsigned int itr_Color_Count;
    unsigned int tmp_Color_Count_Bytes;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_LoadPalette(FlicHdr_SgmtAddr=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Frame_Index);
#endif

    if ( FlicHdr_SgmtAddr == DBG_MAINSCRN_000 )
    {
        dlvfprintf("DEBUG: [%s, %d] ( FlicHdr_SgmtAddr == DBG_MAINSCRN_000 )\n", __FILE__, __LINE__);
    }

    fptr_FlicHdr_SgmtAddr = (struct s_FLIC_HDR _FAR *)MK_FP(FlicHdr_SgmtAddr, 0);

    SrcSgmt = FlicHdr_SgmtAddr;
    DstSgmt = gsa_Palette;

    if ( fptr_FlicHdr_SgmtAddr->EMM_Handle_Number != 0 )
    {
        HERE("( fptr_FlicHdr_SgmtAddr->EMM_Handle_Number != 0 )");

        tmp_SrcOfst = fptr_FlicHdr_SgmtAddr->EMM_Logical_Page_Offset;

        _DX = fptr_FlicHdr_SgmtAddr->EMM_Handle_Number;
        _BL = fptr_FlicHdr_SgmtAddr->EMM_Logical_Page_Number;
        _BH = 0;
        _AL = 0x00;
        _AH = 0x44;
        geninterrupt(0x67);

        _DX = fptr_FlicHdr_SgmtAddr->EMM_Handle_Number;
        _BL = fptr_FlicHdr_SgmtAddr->EMM_Logical_Page_Number;
        _BH = 0;
        _AL = 0x01;
        _AH = 0x44;
        geninterrupt(0x67);

        //_DS = g_EMM_PageFrame_Base_Address
        SrcSgmt = EMM_PageFrameBaseAddress;
    }
    else
    {
        HERE("( fptr_FlicHdr_SgmtAddr->EMM_Handle_Number == 0 )");

        tmp_SrcOfst = 0;
    }

// @@PaletteDataOffset:
//     _SI = 0x0E;  // FLIC_HDR.Palette_Data_Offset
//     _SI = _SI + _DI;
// asm lodsw                                //; AX = [DS:SI]
    SrcOfst = tmp_SrcOfst + 0x0E;  // FLIC Offset to FLIC Palette Header Offset, from BoF, maybe in EMM

    // tmp_Palette_Header_Offset = (unsigned int)*SrcSgmt[SrcOfst++];
    fptr_Src = (unsigned int _FAR *)MK_FP(SrcSgmt, SrcOfst);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);
#endif

    tmp_Palette_Header_Offset = fptr_Src[0];
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] tmp_Palette_Header_Offset: %04Xh %ud\n", __FILE__, __LINE__, tmp_Palette_Header_Offset, tmp_Palette_Header_Offset);
#endif

    // _AX = _AX + _DI;
    // _SI = _AX;
    // _BX = _AX;

    SrcOfst = tmp_SrcOfst + tmp_Palette_Header_Offset;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] SrcOfst: %04Xh %ud\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
#endif

    //fh_frame_colors = (unsigned char)*SrcSgmt[SrcOfst + 0x06];
    fptr_Src = (unsigned int _FAR *)MK_FP(SrcSgmt, SrcOfst);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);
#endif

    //fh_frames_have_palettes = fptr_Src[0x06];
    fh_frames_have_palettes = FPEEKB(SrcSgmt, SrcOfst + 0x06);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_frames_have_palettes: %02Xh %ud\n", __FILE__, __LINE__, fh_frames_have_palettes, fh_frames_have_palettes);
#endif

    if ( (Frame_Index == 0) || (fh_frames_have_palettes == 0) )
    {
        // @@UseFlicPalette
        HERE("( (Frame_Index == 0) || (fh_frame_colors == 0) )");

// asm lodsw
// asm mov dx, ax                          ; image_offset
// asm add dx, di
// asm lodsw
// asm mov di, ax                          ; first_color
// asm lodsw
// asm mov cx, ax                          ; color_count
// asm mov si, dx

        // fh_image_offset = (unsigned int)*SrcSgmt[SrcOfst++];
        // fh_first_color = (unsigned int)*SrcSgmt[SrcOfst++];
        // fh_color_count = (unsigned int)*SrcSgmt[SrcOfst++];
        fptr_Src = (unsigned int _FAR *)MK_FP(SrcSgmt, SrcOfst);
        fh_palette_data_offset = fptr_Src[0];  // Offset to FLIC Palette Data
        fh_palette_color_index = fptr_Src[1];   // Index of Color
        fh_palette_color_count = fptr_Src[2];   // Count of Colors

#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] fh_palette_data_offset: %04Xh %ud\n", __FILE__, __LINE__, fh_palette_data_offset, fh_palette_data_offset);
        dlvfprintf("DEBUG: [%s, %d] fh_palette_color_index: %04Xh %ud\n", __FILE__, __LINE__, fh_palette_color_index, fh_palette_color_index);
        dlvfprintf("DEBUG: [%s, %d] fh_palette_color_count: %04Xh %ud\n", __FILE__, __LINE__, fh_palette_color_count, fh_palette_color_count);
#endif

        tmp_Color_Count = fh_palette_color_count;
        SrcOfst = tmp_SrcOfst + fh_palette_data_offset;
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] SrcOfst: %04X %u\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
#endif
    }
    else
    {
        // @@UseFramePalette
        HERE("( (Frame_Index != 0) & (fh_frame_colors != 0) )");

// asm dec cx
// asm shl cx, 1
// asm shl cx, 1
// asm add si, cx
// asm add si, 8                           ; ? Frame Offset to Frame Data ?
// asm lodsw                                ; image_offset
// asm mov dx, ax
// asm add dx, di
// asm lodsw
// asm cmp ah, 0                           ; color_count
// asm jz short @@Done_DoNothing

        // _SI = _SI + 8 + ... Frame_Index - 1 ... * 4 ... 
        SrcOfst = SrcOfst + 8 + ( (Frame_Index - 1) * 4 );
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] SrcOfst: %04X %u\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
#endif
        // ffh_image_offset = (unsigned int)*SrcSgmt[SrcOfst++];
        // ffh_color_count = (unsigned char)*SrcSgmt[SrcOfst++];
        // ffh_first_color = (unsigned char)*SrcSgmt[SrcOfst++];
        fptr_Src = (unsigned int _FAR *)MK_FP(SrcSgmt, SrcOfst);
        ffh_image_offset = fptr_Src[0];
        ffh_color_count = fptr_Src[1];
        ffh_first_color = fptr_Src[2];

#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] ffh_image_offset: %04X %u\n", __FILE__, __LINE__, ffh_image_offset, ffh_image_offset);
        dlvfprintf("DEBUG: [%s, %d] ffh_color_count: %04X %u\n", __FILE__, __LINE__, ffh_color_count, ffh_color_count);
        dlvfprintf("DEBUG: [%s, %d] ffh_first_color: %04X %u\n", __FILE__, __LINE__, ffh_first_color, ffh_first_color);
#endif

        if ( ffh_color_count == 0 )
        {
            HERE("( ffh_color_count == 0 )");
            goto Done;
        }
        HERE("( ffh_color_count != 0 )");

        tmp_Color_Count = ffh_color_count;
        SrcOfst = tmp_SrcOfst + ffh_image_offset;
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] SrcOfst: %04X %u\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
#endif
    }

// @@SetDestinations:
//     mov  bx, di
//     shl  di, 1
//     add  di, bx
//     add  bx, 768                         ; + 300h = VGA_DACmod_Segment@

    DstOfst = (fh_palette_color_index * 3);         // AKA fh_first_color + (fh_first_color * 2)
    ofstPaletteFlags = DstOfst + 768;

    fptr_DstByte = (unsigned char _FAR *)MK_FP(DstSgmt, 0);
    fptr_SrcByte = (unsigned char _FAR *)MK_FP(SrcSgmt, 0);

    tmp_Color_Count_Bytes = (tmp_Color_Count * 3);
    for ( itr_Color_Count = 0; itr_Color_Count < tmp_Color_Count_Bytes; itr_Color_Count++ )
    {
        // dlvfprintf("DEBUG: [%s, %d] fptr_SrcByte[%u]: %02X %u)\n", __FILE__, __LINE__, itr_Color_Count, fptr_SrcByte[(SrcOfst + itr_Color_Count)], fptr_SrcByte[(SrcOfst + itr_Color_Count)]);
        //fptr_Dst[(DstOfst++] = fptr_Src[SrcOfst++];
        fptr_DstByte[(DstOfst + itr_Color_Count)] = fptr_SrcByte[(SrcOfst + itr_Color_Count)];
        fptr_DstByte[ofstPaletteFlags++] = 1;
    }

Done:
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_LoadPalette(FlicHdr_SgmtAddr=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Frame_Index);
#endif
}
