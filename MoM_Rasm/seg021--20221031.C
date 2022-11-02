
#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "seg021.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"  /* GET_FLIC_HDR_EMM_HANDLE_NUMBER() */
#include "ST_SA.H"
#include "ST_VGA.H"  /* p_Palette  */

#include <CONIO.H>  /* inp(), outp() */

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

/*
    MGC  s21p07  void FLIC_LoadPalette(SAMB_addr sa_FLIC_Header, int Frame_Index)

    sa_FLIC_Header: SAMB_data of FLIC LBX Entry
                    If 'In-EMM' (Reserved EMM), just a copy of the FLIC_Header, with populated EMM Handle, Page, and Offset member values

    Address of actual LBX Entry FLIC data...
    sa_FLIC_Header OR (EMM_PageFrameBaseAddress + fp_FLIC_Header->EMM_Logical_Page_Offset)

*/
// // s21p07
// void FLIC_LoadPalette_Shim(SAMB_addr sa_FLIC_Header, int Frame_Index)
// {
//     unsigned int DstSgmt;
//     unsigned int DstOfst;
//     unsigned int SrcSgmt;
//     unsigned int SrcOfst;
//     struct s_FLIC_HDR * fp_FLIC_Header;
// 
//     SrcSgmt = sa_FLIC_Header;
//     DstSgmt = FP_SEG(p_Palette);
// 
//     fp_FLIC_Header = (struct s_FLIC_HDR *)MK_FP(sa_FLIC_Header, 0);
// 
//     if ( fp_FLIC_Header->EMM_Handle_Number != 0 )
//     {
// 
//     }
// }

// void FLIC_Load_Palette_FP_EMM(SAMB_ptr fp_FLIC_Header, int Frame_Index)
// {
// 
// }

// void FLIC_Load_Palette_EMM(SAMB_ptr fp_FLIC_Header, int Frame_Index)
// {
// 
// }

void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int frame_index)
{
    SAMB_ptr p_FLIC_File;
    byte_ptr palette_header;
    byte_ptr palette_data;
    byte_ptr frame_palette_header;
    byte_ptr frame_palette_data;

    word palette_header_offset;
    word palette_data_offset;
    word start;
    word count;
    byte frame_palette_flag;
    word frame_palette_header_offset;
    word frame_palette_data_offset;

    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Width(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Width(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Height(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Height(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Current_Frame(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Current_Frame(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Count(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Count(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Loop_Frame(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Loop_Frame(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Handle_Number(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Handle_Number(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Number(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Number(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Palette_Header_Offset(p_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Palette_Header_Offset(p_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Type(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Type(p_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Shading(p_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Shading(p_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset_Table(p_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset_Table(p_FLIC_Header));
#endif

    if ( FLIC_Get_EMM_Handle_Number(p_FLIC_Header) == 0 )
    {
        p_FLIC_File = MK_FP(FP_SEG(p_FLIC_Header),FP_OFF(p_FLIC_Header)+0);
        // p_FLIC_File = p_FLIC_Header;
        // p_FLIC_File = &p_FLIC_Header[0];
        // p_FLIC_File = (p_FLIC_Header + 0);
    }
    else
    {
        EMM_MAP_PAGE(0, 4, FLIC_Get_EMM_Logical_Page_Number(p_FLIC_Header));

        p_FLIC_File = MK_FP(EMM_PageFrameBaseAddress, FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_Header));
        // p_FLIC_File = &p_EMM_PFBA[FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_Header)];
        // p_FLIC_File = (p_EMM_PFBA + FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_Header));
    }
    // SM2LM  fp_Src = (unsigned char *)MK_FP(SrcSgmt, SrcOfstBase);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Width(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Width(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Height(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Height(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Current_Frame(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Current_Frame(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Count(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Count(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Loop_Frame(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Loop_Frame(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Handle_Number(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Handle_Number(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Number(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Number(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_File): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Offset(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Palette_Header_Offset(p_FLIC_File): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Palette_Header_Offset(p_FLIC_File));

    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Type(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Type(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Shading(p_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Shading(p_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset_Table(p_FLIC_File): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset_Table(p_FLIC_File));
#endif

    palette_header_offset = FLIC_Get_Palette_Header_Offset(p_FLIC_File);
    palette_header = &p_FLIC_File[palette_header_offset];
    
    frame_palette_flag = palette_header[6];
    // FLIC_Get_Frame_Palettes(palette_header)

    if ( (frame_index == 0) || (frame_palette_flag == 0) )
    {
        palette_data_offset = GET_2B_OFS(palette_header,0x00);
        // start = GET_2B_OFS(palette_header,0x02);
        // count = GET_2B_OFS(palette_header,0x04);
        start = FLIC_Get_Palette_Color_Start(palette_header);
        count = FLIC_Get_Palette_Color_Count(palette_header);
        palette_data = &p_FLIC_File[palette_data_offset];
        // FLIC_Get_Palette_Data_Offset
    }
    else
    {
        // FLIC Segment Address + FLIC Palette Header Offset + sizeof(FLIC Palette Header) + (4 * frame_num - 1)
        // frame_palette_data
        frame_palette_header_offset = palette_header_offset + 8 + (4 * (frame_index - 1));
        frame_palette_header = &p_FLIC_File[frame_palette_header_offset];
        frame_palette_data_offset = GET_2B_OFS(frame_palette_header,0x00);
        palette_data = &p_FLIC_File[frame_palette_data_offset];
        start = GET_1B_OFS(frame_palette_header,0x01);
        count = GET_1B_OFS(frame_palette_header,0x02);
    }

    for(itr = 0; itr < count; itr++)
    {
        *(p_Palette + itr) = *(palette_data + itr);
        *(p_Palette + 768 + itr) = 1;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

}

// s21p07
void FLIC_LoadPalette_Redux(SAMB_addr sa_FLIC_Header, int Frame_Index)
{
    byte_ptr p_FLIC_Header;
    byte _FAR * fp_FLIC_Header;
    byte _FAR * fp_FLIC_File;
    byte _FAR * fp_FLIC_File_EMM;
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned int SrcOfstBase;
    byte _FAR * fp_Src;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    byte _FAR * fp_Dst;
    unsigned int FH_Palette_Header_Offset;
    unsigned char FH_Frames_Have_Palettes;
    unsigned int FH_Palette_Data_Offset;
    unsigned int FH_Palette_Color_Index;
    unsigned int FH_Palette_Color_Count;
    unsigned int Color_Index;
    unsigned int Color_Count;
    unsigned char _FAR * fp_FlicPalette;
    unsigned int itr_Color_Count;
    unsigned char _FAR * fp_FLIC_Palette_Header_Offset;
    unsigned char _FAR * fp_FLIC_Palette_Header;
    unsigned long ul_fp_FLIC_Palette_Header_Offset;

    word palette_header_offset;
    byte_ptr palette_header;
    word palette_data_offset;
    byte_ptr palette_data;
    word start;
    word count;
    byte frame_palette_flag;
    word frame_palette_header_offset;
    byte_ptr frame_palette_header;
    word frame_palette_data_offset;
    byte_ptr frame_palette_data;
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_LoadPalette_Redux(sa_FLIC_Header=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, sa_FLIC_Header, Frame_Index);
#endif

    fp_FLIC_Header = (byte_ptr) MK_FP(sa_FLIC_Header, 0);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Width(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Width(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Height(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Height(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Current_Frame(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Current_Frame(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Count(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Count(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Loop_Frame(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Loop_Frame(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Handle_Number(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Handle_Number(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_Header));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Palette_Header_Offset(fp_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Palette_Header_Offset(fp_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Type(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Type(fp_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Shading(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Shading(fp_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset_Table(fp_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset_Table(fp_FLIC_Header));
#endif

// #define FLIC_Get_Palette_Data_Offset(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_PAL_HDR_POS_PALETTE_DATA_OFFSET) )
// 
// #define FLIC_Get_Color_start(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_PAL_HDR_POS_COLOR_START) )
// 
// #define FLIC_Get_Color_Count(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_PAL_HDR_POS_COLOR_COUNT) )
// 
// #define FLIC_Get_Frame_Palettes(_ptr_) ( GET_1B_OFS(_ptr_,FLIC_PAL_HDR_POS_FRAME_PALETTES) )

    // if ( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header) != 0 )
    // {
    //     FLIC_Load_Palette_EMM(sa_FLIC_Header, Frame_Index);
    // }

    if ( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header) == 0 )
    {
        DLOG("( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header) == 0 )");
        SrcSgmt = sa_FLIC_Header;
        SrcOfstBase = 0;
        fp_FLIC_File = MK_FP(sa_FLIC_Header,0);
    }
    else
    {
        DLOG("( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header) != 0 )");
        SrcSgmt = EMM_PageFrameBaseAddress;
        SrcOfstBase = GET_FLIC_HDR_EMM_LOGICAL_PAGE_OFFSET(fp_FLIC_Header);
        // EMM_MAP_PAGE(0, GET_FLIC_HDR_EMM_HANDLE(fp_FLIC_Header), GET_FLIC_HDR_EMM_LOGICAL_PAGE(fp_FLIC_Header))
        // _AL = 0;
        // _BL = 0;
        // _BH = 0;
        // _DX = 4;
        // _AH = EMS_MAPPAGE;
        // geninterrupt(EMS_INT);
        EMM_MAP_PAGE(0, 4, 0);
        fp_FLIC_File = MK_FP(EMM_PageFrameBaseAddress, FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_Header));
    }
    fp_Src = (unsigned char *)MK_FP(SrcSgmt, SrcOfstBase);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Width(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Width(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Height(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Height(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Current_Frame(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Current_Frame(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Count(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Count(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Loop_Frame(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Loop_Frame(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Handle_Number(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Handle_Number(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_File): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Palette_Header_Offset(fp_FLIC_File): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Palette_Header_Offset(fp_FLIC_File));

    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Type(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Type(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Shading(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Shading(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset_Table(fp_FLIC_File): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset_Table(fp_FLIC_File));
#endif


// #define FLIC_LBX_Get_Palette_Header_Offset(_ptr_)   ( GET_LE_16( (unsigned char _FAR *) MK_FP( FP_SEG((_ptr_)), ( FP_OFF((_ptr_)) + 0x0C ) ) ) )
    // fp_FLIC_Palette_Header_Offset = (unsigned char _FAR *)MK_FP(SrcSgmt, SrcOfstBase + 0x0E);

// #define MK_FP(_sgmt_,_ofst_) ( (void *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )
// #define FP_SEG(_fp_) ( (unsigned int) ( (unsigned long)((void *)(_fp_)) >> 16) )
// #define FP_OFF(_fp_) ( (unsigned int) (_fp_) )
    // // ul_fp_FLIC_Palette_Header_Offset = AddressOf(fp_FLIC_Palette_Header_Offset);
    // fp_FLIC_Palette_Header = (unsigned char _FAR *)MK_FP(SrcSgmt, SrcOfstBase + GET_LE_16(fp_FLIC_Palette_Header_Offset));

// #define FPGET_WORD(_fp_) ( *((word _FAR *)MK_FP(FP_SEG(_fp_), FP_OFF(_fp_))) )
// #define GET_LE_16(_ptr_) ( (((unsigned int)(((unsigned char _FAR *)(_ptr_))[1])) << 8) | ((unsigned int)(((unsigned char _FAR *)(_ptr_))[0])) )
// FH_Palette_Header_Offset



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FLIC_LBX_Get_Palette_Header_Offset(fp_Src): 0x%04X\n", __FILE__, __LINE__, FLIC_LBX_Get_Palette_Header_Offset(fp_Src));
#endif    // Get (fp_FLIC_Header->Palette_Header_Offset)
    // ?unneeded? FH_Palette_Header_Offset = FLIC_LBX_Get_Palette_Header_Offset(fp_Src);
    FH_Palette_Header_Offset = FLIC_LBX_Get_Palette_Header_Offset(fp_Src);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FH_Palette_Header_Offset: 0x%04X\n", __FILE__, __LINE__, FH_Palette_Header_Offset);
#endif    // Get (fp_FLIC_Header->Palette_Header_Offset)
    // Get (fp_FLIC_Header->Palette_Header->Frames_Have_Palettes)
    // ?unneeded? FH_Frames_Have_Palettes = FLIC_LBX_Get_Palette_PerFrame_Flag(fp_Src);
    FH_Frames_Have_Palettes = FLIC_LBX_Get_Palette_PerFrame_Flag(fp_Src);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FH_Frames_Have_Palettes: 0x%02X\n", __FILE__, __LINE__, FH_Frames_Have_Palettes);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FLIC_LBX_Get_Palette_PerFrame_Flag(fp_Src): 0x%02X\n", __FILE__, __LINE__, FLIC_LBX_Get_Palette_PerFrame_Flag(fp_Src));
#endif
    if ( (Frame_Index == 0) || (FLIC_LBX_Get_Palette_PerFrame_Flag(fp_Src) == 0) )
    {
        DLOG("( (Frame_Index == 0) || (FH_Frames_Have_Palettes == 0) )");
        // fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);
        FH_Palette_Data_Offset = FLIC_LBX_Get_Palette_Data_Offset(fp_Src);
        FH_Palette_Color_Index = FLIC_LBX_Get_Palette_Color_Index(fp_Src);
        FH_Palette_Color_Count = FLIC_LBX_Get_Palette_Color_Count(fp_Src);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FH_Palette_Data_Offset: 0x%04X\n", __FILE__, __LINE__, FH_Palette_Data_Offset);
    dbg_prn("DEBUG: [%s, %d] FH_Palette_Color_Index: 0x%04X (%ud)\n", __FILE__, __LINE__, FH_Palette_Color_Index, FH_Palette_Color_Index);
    dbg_prn("DEBUG: [%s, %d] FH_Palette_Color_Count: 0x%04X (%ud)\n", __FILE__, __LINE__, FH_Palette_Color_Count, FH_Palette_Color_Count);
#endif
        Color_Index = FLIC_LBX_Get_Palette_Color_Index(fp_Src);
        Color_Count = FLIC_LBX_Get_Palette_Color_Count(fp_Src);

        fp_FlicPalette = (unsigned char _FAR *) MK_FP(SrcSgmt, SrcOfstBase + FH_Palette_Data_Offset);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fp_FlicPalette: %Fp\n", __FILE__, __LINE__, fp_FlicPalette);
#endif
    }
    else
    {
        DLOG("( (Frame_Index != 0) & (FH_Frames_Have_Palettes != 0) )");
        // SrcOfst = SrcOfst + 8 + ( (Frame_Index - 1) * 4 );
        // fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);
        // ffh_image_offset = fptr_Src[0];
        // ffh_color_count = fptr_Src[1];
        // ffh_first_color = fptr_Src[2];
        // if ( ffh_color_count == 0 ) { goto Done; }
        // tmp_Color_Count = ffh_color_count;
        // SrcOfst = SrcOfstBase + ffh_image_offset;
    }

    for(itr = Color_Index; itr < Color_Count; itr++)
    {
        *(p_Palette + itr) = *(palette_data + itr);
        *(p_Palette + 768 + itr) = 1;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_LoadPalette_Redux(sa_FLIC_Header=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, sa_FLIC_Header, Frame_Index);
#endif

}

// s21p07
void FLIC_LoadPalette(SAMB_addr sa_FLIC_Header, int Frame_Index)
{
    struct s_FLIC_HDR * fp_FLIC_Header;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned int SrcOfstBase;
    unsigned int *fptr_Dst;
    unsigned int *fptr_Src;
    unsigned char *fptr_DstByte;
    unsigned char *fptr_SrcByte;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_LoadPalette(sa_FLIC_Header=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, sa_FLIC_Header, Frame_Index);
#endif

// #ifdef STU_DEBUG
//     if ( sa_FLIC_Header == DBG_MAINSCRN_000 )
//     {
//         dbg_prn("DEBUG: [%s, %d] ( sa_FLIC_Header == DBG_MAINSCRN_000 )\n", __FILE__, __LINE__);
//     }
// #endif

    fp_FLIC_Header = (struct s_FLIC_HDR *)MK_FP(sa_FLIC_Header, 0);

    SrcSgmt = sa_FLIC_Header;
    DstSgmt = FP_SEG(p_Palette);

    if ( fp_FLIC_Header->EMM_Handle_Number != 0 )
    {
        // HERE("( fp_FLIC_Header->EMM_Handle_Number != 0 )");

        SrcOfstBase = fp_FLIC_Header->EMM_Logical_Page_Offset;

        // ST_EMM.H  #define EMM_MAP_PAGE(_epp_,_ehn_,_elp_)
        // EMM_MAP_PAGE(_AL, _DX, _BX)
        
        _DX = fp_FLIC_Header->EMM_Handle_Number;  // DX = EMM handle
        _BL = fp_FLIC_Header->EMM_Logical_Page_Number;
        _BH = 0;            // BX = logical page number (0 to total allocated minus 1)
        _AL = 0x00;         // AL = physical page number (0-3)
        _AH = 0x44;         // INT 67,44 - Map Logical Page Into Physical Page Window (LIM EMS)
        geninterrupt(0x67); // EMS_INT - INT 67 - Expanded Memory Specification

        _DX = fp_FLIC_Header->EMM_Handle_Number;  // DX = EMM handle
        _BL = fp_FLIC_Header->EMM_Logical_Page_Number;
        _BH = 0;            // BX = logical page number (0 to total allocated minus 1)
        _AL = 0x01;         // AL = physical page number (0-3)
        _AH = 0x44;         // INT 67,44 - Map Logical Page Into Physical Page Window (LIM EMS)
        geninterrupt(0x67); // EMS_INT - INT 67 - Expanded Memory Specification

        //_DS = g_EMM_PageFrame_Base_Address
        SrcSgmt = EMM_PageFrameBaseAddress;
    }
    else
    {
        // HERE("( fp_FLIC_Header->EMM_Handle_Number == 0 )");

        SrcOfstBase = 0;
    }

// @@PaletteDataOffset:
//     _SI = 0x0E;  // FLIC_HDR.Palette_Data_Offset
//     _SI = _SI + _DI;
// asm lodsw                                //; AX = [DS:SI]
    SrcOfst = SrcOfstBase + 0x0E;  // FLIC Offset to FLIC Palette Header Offset, from BoF, maybe in EMM

    // tmp_Palette_Header_Offset = (unsigned int)*SrcSgmt[SrcOfst++];
    fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);
// #endif

    tmp_Palette_Header_Offset = fptr_Src[0];

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] tmp_Palette_Header_Offset: %04Xh %ud\n", __FILE__, __LINE__, tmp_Palette_Header_Offset, tmp_Palette_Header_Offset);
// #endif

    // _AX = _AX + _DI;
    // _SI = _AX;
    // _BX = _AX;

    SrcOfst = SrcOfstBase + tmp_Palette_Header_Offset;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] SrcOfst: %04Xh %ud\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
// #endif

    //fh_frame_colors = (unsigned char)*SrcSgmt[SrcOfst + 0x06];
    fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);
// #endif

    //fh_frames_have_palettes = fptr_Src[0x06];
    //fh_frames_have_palettes = FPEEKB(SrcSgmt, SrcOfst + 0x06);  // FH_Frames_Have_Palettes = FLIC_LBX_Get_Palette_PerFrame_Flag(fp_Src);
    // GET_1B_SEG_OFS_REL(SrcSgmt, SrcOfst, FlicPalHdr_FramesHavePalettes);
    fh_frames_have_palettes = *( (byte *) MK_FP(SrcSgmt, (SrcOfst + FlicPalHdr_FramesHavePalettes)) );

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] fh_frames_have_palettes: %02Xh %ud\n", __FILE__, __LINE__, fh_frames_have_palettes, fh_frames_have_palettes);
// #endif

    if ( (Frame_Index == 0) || (fh_frames_have_palettes == 0) )
    {
        // @@UseFlicPalette
        // HERE("( (Frame_Index == 0) || (fh_frame_colors == 0) )");

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
        fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);
        fh_palette_data_offset = fptr_Src[0];  // Offset to FLIC Palette Data
        fh_palette_color_index = fptr_Src[1];   // Index of Color
        fh_palette_color_count = fptr_Src[2];   // Count of Colors

// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] fh_palette_data_offset: %04Xh %ud\n", __FILE__, __LINE__, fh_palette_data_offset, fh_palette_data_offset);
//         dbg_prn("DEBUG: [%s, %d] fh_palette_color_index: %04Xh %ud\n", __FILE__, __LINE__, fh_palette_color_index, fh_palette_color_index);
//         dbg_prn("DEBUG: [%s, %d] fh_palette_color_count: %04Xh %ud\n", __FILE__, __LINE__, fh_palette_color_count, fh_palette_color_count);
// #endif

        tmp_Color_Count = fh_palette_color_count;
        SrcOfst = SrcOfstBase + fh_palette_data_offset;

// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] SrcOfst: %04X %u\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
// #endif

    }
    else
    {
        // @@UseFramePalette
        // HERE("( (Frame_Index != 0) & (fh_frame_colors != 0) )");

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

// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] SrcOfst: %04X %u\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
// #endif

        // ffh_image_offset = (unsigned int)*SrcSgmt[SrcOfst++];
        // ffh_color_count = (unsigned char)*SrcSgmt[SrcOfst++];
        // ffh_first_color = (unsigned char)*SrcSgmt[SrcOfst++];
        fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);
        ffh_image_offset = fptr_Src[0];
        ffh_color_count = fptr_Src[1];
        ffh_first_color = fptr_Src[2];

// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] ffh_image_offset: %04X %u\n", __FILE__, __LINE__, ffh_image_offset, ffh_image_offset);
//         dbg_prn("DEBUG: [%s, %d] ffh_color_count: %04X %u\n", __FILE__, __LINE__, ffh_color_count, ffh_color_count);
//         dbg_prn("DEBUG: [%s, %d] ffh_first_color: %04X %u\n", __FILE__, __LINE__, ffh_first_color, ffh_first_color);
// #endif

        if ( ffh_color_count == 0 )
        {
            // HERE("( ffh_color_count == 0 )");
            goto Done;
        }
        // HERE("( ffh_color_count != 0 )");

        tmp_Color_Count = ffh_color_count;
        SrcOfst = SrcOfstBase + ffh_image_offset;

// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] SrcOfst: %04X %u\n", __FILE__, __LINE__, SrcOfst, SrcOfst);
// #endif

    }

// @@SetDestinations:
//     mov  bx, di
//     shl  di, 1
//     add  di, bx
//     add  bx, 768                         ; + 300h = VGA_DACmod_Segment@

    DstOfst = (fh_palette_color_index * 3);         // AKA fh_first_color + (fh_first_color * 2)
    ofstPaletteFlags = DstOfst + 768;  // 768 = 256 colors * 3 bytes each

    fptr_DstByte = (unsigned char *) MK_FP(DstSgmt, 0);
    fptr_SrcByte = (unsigned char *) MK_FP(SrcSgmt, 0);

    tmp_Color_Count_Bytes = (tmp_Color_Count * 3);
    for ( itr_Color_Count = 0; itr_Color_Count < tmp_Color_Count_Bytes; itr_Color_Count++ )
    {
        // dbg_prn("DEBUG: [%s, %d] fptr_SrcByte[%u]: %02X %u)\n", __FILE__, __LINE__, itr_Color_Count, fptr_SrcByte[(SrcOfst + itr_Color_Count)], fptr_SrcByte[(SrcOfst + itr_Color_Count)]);
        //fptr_Dst[(DstOfst++] = fptr_Src[SrcOfst++];
        fptr_DstByte[(DstOfst + itr_Color_Count)] = fptr_SrcByte[(SrcOfst + itr_Color_Count)];
        fptr_DstByte[ofstPaletteFlags++] = 1;
    }

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_LoadPalette(sa_FLIC_Header=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, sa_FLIC_Header, Frame_Index);
#endif

}
