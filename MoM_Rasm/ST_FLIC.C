
#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_FLIC.H"
#include "ST_SA.H"
#include "ST_EMM.H"     /* fp_EMM_PFBA */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <STRING.H>
//void* __cdecl memcpy(
//    _Out_writes_bytes_all_(_Size) void* _Dst,
//    _In_reads_bytes_(_Size)       void const* _Src,
//    _In_                          size_t      _Size
//    );
// J:\STU\DBWD\BORLANDC\INCLUDE\STRING.H
// void _FAR * _Cdecl _FARFUNC memcpy(void _FAR *__dest, const void _FAR *__src, size_t __n);
// void _FAR * _CType _FARFUNC memmove(void _FAR *__dest, const void _FAR *__src, size_t __n);

// // 1oom::lbxgfx.c::static void lbxgfx_draw_pixels_fmt0(uint8_t *pixbuf, uint16_t w, uint8_t *data, uint16_t pitch)
// void FLIC_RLE_Decode(BYTE * FLIC_Frame_Data)
// {
//     
// }

// s28p11
/*
    Update Current Frame
    Test Palette, EMM, & Remap
    Dispatch to Draw functions
*/
void FLIC_Draw(int x, int y, SAMB_ptr fp_FLIC_Header)
{
    static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local
    struct s_FLIC_HDR * p_PS_FLIC_Header;
    SAMB_ptr fp_FLIC_File;
    int current_frame_index;
    byte flag_palette;
    byte flag_emm;
    byte flag_remap;
    unsigned long int flic_frame_offset;
    unsigned long int FLIC_Frame_Data_sgmt;
    unsigned long int FLIC_Frame_Data_ofst;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw(x = %d, y = %d, fp_FLIC_Header = %p)\n", __FILE__, __LINE__, x, y, fp_FLIC_Header);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fp_FLIC_Header: %p)\n", __FILE__, __LINE__, fp_FLIC_Header);
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fp_FLIC_File: %p)\n", __FILE__, __LINE__, fp_FLIC_File);
#endif

    flag_palette  = ( FLIC_Get_Palette_Header_Offset(fp_FLIC_Header) == 0 ) ? 0 : 1;
    flag_emm      = ( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header)     == 0 ) ? 0 : 1;
    // ...not in fp_FLIC_Header, if it is the In-EMM copy from LBX_Load_Header_Only()
    flag_remap    = ( FLIC_Get_Remap(fp_FLIC_File)                   == 0 ) ? 0 : 1;
    // GET_1B_OFS( (_ptr_), FLIC_HDR_POS_REMAP)

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flag_palette: %u)\n", __FILE__, __LINE__, flag_palette);
    dbg_prn("DEBUG: [%s, %d] flag_emm: %u)\n", __FILE__, __LINE__, flag_emm);
    dbg_prn("DEBUG: [%s, %d] flag_remap: %u)\n", __FILE__, __LINE__, flag_remap);
#endif

    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, FP_SEG(fp_FLIC_Header), sizeof(PS_FLIC_Header));
    // // memcpy((void _FAR *)&PS_FLIC_Header, (const void _FAR *)sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // p_PS_FLIC_Header = &PS_FLIC_Header;
    // memcpy((void *)p_PS_FLIC_Header, (void *)fp_FLIC_Header, sizeof(PS_FLIC_Header));

    current_frame_index = PS_FLIC_Header.Current_Frame;
    PS_FLIC_Header.Current_Frame += 1;
    if ( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )
    {
        DLOG("( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )");
        // // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Current_Frame);
        // FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Current_Frame);
        // // SET_2B_OFS(fp_FLIC_Header, FLIC_HDR_CurrentFrame, PS_FLIC_Header.Current_Frame);
    }
    else
    {
        DLOG("( PS_FLIC_Header.Current_Frame >= PS_FLIC_Header.Frame_Count )");
        // // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Loop_Frame);
        // FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Loop_Frame);
        // // SET_2B_OFS(fp_FLIC_Header, FLIC_HDR_CurrentFrame, PS_FLIC_Header.Loop_Frame);
    }

    // if ( PS_FLIC_Header.Palette_Header_Offset != 0 )
    // {
    //     DLOG("( PS_FLIC_Header.Palette_Header_Offset != 0 )");
    //     FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), current_frame_index);
    // }
    if( flag_palette == ST_TRUE )
    {
        DLOG("( flag_palette == ST_TRUE )");
        // FLIC_Load_Palette(fp_FLIC_Header, current_frame_index);
    }

    /*
        FLIC Frame Offset

    */
    // flic_frame_offset
    // FLIC_Frame_Data_sgmt
    // FLIC_Frame_Data_ofst
    // Offset to FLIC File + Offset to FLIC Frame Offset Table + (4 * FLIC Frame Index)
    flic_frame_offset = GET_4B_OFS(fp_FLIC_File, FLIC_HDR_POS_FRAME_OFFSET_TABLE);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X)\n", __FILE__, __LINE__, flic_frame_offset);
#endif
    // ? (flic_frame_offset / 16) is how many segments need to be added to the segment portion of the far pointer ?
    // What about the +1 part?
    // 


    if( (flag_emm == ST_FALSE) && (flag_remap == ST_FALSE) )
    {
        DLOG("( (flag_emm == ST_FALSE) && (flag_remap == ST_FALSE) )");
        // FLIC_Draw_Frame();
        // _s27p01a.c  void FLIC_Draw_A(int x, int y, int width, unsigned int FLIC_Frame_Data_ofst, unsigned int FLIC_Frame_Data_sgmt);
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FLIC_Frame_Data_ofst, FLIC_Frame_Data_sgmt);
    }
    if( (flag_emm == ST_FALSE) && (flag_remap == ST_TRUE) )
    {
        DLOG("( (flag_emm == ST_FALSE) && (flag_remap == ST_TRUE) )");
        // FLIC_Draw_Frame_Remap();
        // _s27p02.asm  void FLIC_Draw_R(int x, int y, int width, unsigned int FLIC_Frame_Data_ofst, unsigned int FLIC_Frame_Data_sgmt);
        // FLIC_Draw_R(x, y, PS_FLIC_Header.Width, FLIC_Frame_Data_ofst, FLIC_Frame_Data_sgmt);
    }
    if( (flag_emm == ST_TRUE) && (flag_remap == ST_FALSE) )
    {
        DLOG("( (flag_emm == ST_TRUE) && (flag_remap == ST_FALSE) )");
        // FLIC_Draw_Frame_EMM();
        // _s27p03c.c  void FLIC_Draw_EMM_C(int x, int y, SAMB_addr sa_FLIC_Header, int current_frame_index);
        // FLIC_Draw_EMM_C(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
    }
    if( (flag_emm == ST_TRUE) && (flag_remap == ST_TRUE) )
    {
        DLOG("( (flag_emm == ST_TRUE) && (flag_remap == ST_TRUE) )");
        // FLIC_Draw_Frame_EMM_Remap();
        // _s27p03c.c  void FLIC_Draw_EMM_C(int x, int y, SAMB_addr sa_FLIC_Header, int current_frame_index);
        // FLIC_Draw_EMM_C(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw(x = %d, y = %d, fp_FLIC_Header = %p)\n", __FILE__, __LINE__, x, y, fp_FLIC_Header);
#endif
}
