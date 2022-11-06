
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






// // ~== MoO2
// // void Draw(signed short int x, signed short int y, unsigned char * picture)
// void Draw(signed short int x, signed short int y, SAMB_ptr FLIC_animation)
// {
//     static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local 
// 
//     signed short int x2;
//     signed short int y2;
// 
//     word Frame_Index;
// 
//     // dseg02:001B42A8 animation_header dd ?
// 
//     // ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
//     // void *memcpy(void *dest, const void * src, size_t n)
//     memcpy((void *)&PS_FLIC_Header, (void *)FLIC_animation, sizeof(PS_FLIC_Header));
// 
//     Frame_Index = PS_FLIC_Header.Current_Frame;
//     PS_FLIC_Header.Current_Frame += 1;
//     if ( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )
//     {
//         // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Current_Frame);
//         FLIC_Set_Current_Frame(FLIC_animation, PS_FLIC_Header.Current_Frame);
//     }
//     else
//     {
//         // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Loop_Frame);
//         FLIC_Set_Current_Frame(FLIC_animation, PS_FLIC_Header.Loop_Frame);
//     }
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
    dword flic_frame_offset;
    word flic_frame_offset_sgmt;
    word flic_frame_offset_ofst;
    byte_ptr fp_FLIC_Frame;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw(x = %d, y = %d, fp_FLIC_Header = %p)\n", __FILE__, __LINE__, x, y, fp_FLIC_Header);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fp_FLIC_Header: %p\n", __FILE__, __LINE__, fp_FLIC_Header);
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
    dbg_prn("DEBUG: [%s, %d] fp_FLIC_File: %p\n", __FILE__, __LINE__, fp_FLIC_File);
#endif

    flag_palette  = ( FLIC_Get_Palette_Header_Offset(fp_FLIC_Header) == 0 ) ? 0 : 1;
    flag_emm      = ( FLIC_Get_EMM_Handle_Number(fp_FLIC_Header)     == 0 ) ? 0 : 1;
    // ...not in fp_FLIC_Header, if it is the In-EMM copy from LBX_Load_Header_Only()
    flag_remap    = ( FLIC_Get_Remap(fp_FLIC_File)                   == 0 ) ? 0 : 1;
    // GET_1B_OFS( (_ptr_), FLIC_HDR_POS_REMAP)

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flag_palette: %u\n", __FILE__, __LINE__, flag_palette);
    dbg_prn("DEBUG: [%s, %d] flag_emm: %u\n", __FILE__, __LINE__, flag_emm);
    dbg_prn("DEBUG: [%s, %d] flag_remap: %u\n", __FILE__, __LINE__, flag_remap);
#endif

    // NOTE(JimBalcomb,20221104): found this over in seg028.C, in FLIC_Draw_XY_STU0(), not sure what my idea was, but I think it was based on something I saw in FliLib or Animator
    // ASSERT(sizeof(sa_FLIC_Header) == sizeof(PS_FLIC_Header))  // sizeof(struct s_FLIC_HDR) == 0x10  16 bytes, 1 paragraph/segment

    // ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, FP_SEG(fp_FLIC_Header), sizeof(PS_FLIC_Header));
    // // memcpy((void _FAR *)&PS_FLIC_Header, (const void _FAR *)sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // p_PS_FLIC_Header = &PS_FLIC_Header;
    // memcpy((void *)p_PS_FLIC_Header, (void *)fp_FLIC_Header, sizeof(PS_FLIC_Header));
    // memcpy((void _FAR *)&PS_FLIC_Header, (const void _FAR *)fp_FLIC_Header, sizeof(PS_FLIC_Header));
    memcpy((void *)&PS_FLIC_Header, (const void *)fp_FLIC_Header, sizeof(PS_FLIC_Header));

    current_frame_index = PS_FLIC_Header.Current_Frame;
    PS_FLIC_Header.Current_Frame += 1;
    if ( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )
    {
        DLOG("( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )");
        // // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Current_Frame);
        // FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Current_Frame);
        // // SET_2B_OFS(fp_FLIC_Header, FLIC_HDR_CurrentFrame, PS_FLIC_Header.Current_Frame);
        FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Current_Frame);
    }
    else
    {
        DLOG("( PS_FLIC_Header.Current_Frame >= PS_FLIC_Header.Frame_Count )");
        // // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Loop_Frame);
        // FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Loop_Frame);
        // // SET_2B_OFS(fp_FLIC_Header, FLIC_HDR_CurrentFrame, PS_FLIC_Header.Loop_Frame);
        FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Loop_Frame);
    }

    // if ( PS_FLIC_Header.Palette_Header_Offset != 0 ) { FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), current_frame_index); }
    if( flag_palette == ST_TRUE )
    {
        DLOG("( flag_palette == ST_TRUE )");
        // FLIC_Load_Palette(fp_FLIC_Header, current_frame_index);
    }



    /*
        FLIC Frame Offset

    */
    // Offset to FLIC File + Offset to FLIC Frame Offset Table + (4 * FLIC Frame Index)
    // Offset to FLIC File + Offset to FLIC Frame
    // Far Pointer Offset = (FLIC_Frame_Offset + 1) & 0x0F  AKA (FLIC_Frame_Offset + 1) % 16, where the remainder will be {0,...,15}; +1 so 0000 1111 becomes 0001 0000 
    // flic_frame_offset = GET_4B_OFS( fp_FLIC_File, FLIC_HDR_POS_FRAME_OFFSET_TABLE + (4 * current_frame_index) );
    flic_frame_offset = FLIC_Get_Frame_Offset(fp_FLIC_File, current_frame_index);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X\n", __FILE__, __LINE__, flic_frame_offset);
#endif
    flic_frame_offset_sgmt = (flic_frame_offset >> 4);          // ~== flic_frame_offset / 16
    flic_frame_offset_ofst = ((flic_frame_offset & 0x0F) + 1);  // ~== flic_frame_offset % 16
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset_sgmt: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_sgmt);
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset_ofst: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_ofst);
#endif

    fp_FLIC_Frame = (byte_ptr)MK_FP(FP_SEG(fp_FLIC_File)+flic_frame_offset_sgmt,flic_frame_offset_ofst);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fp_FLIC_Frame: %p\n", __FILE__, __LINE__, fp_FLIC_Frame);
#endif


    if( (flag_emm == ST_FALSE) && (flag_remap == ST_FALSE) )
    {
        DLOG("( (flag_emm == ST_FALSE) && (flag_remap == ST_FALSE) )");
        // FLIC_Draw_Frame();
        // _s27p01a.c  void FLIC_Draw_A(int x, int y, int width, unsigned int FLIC_Frame_Ofst, unsigned int FLIC_Frame_Sgmt);
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, flic_frame_offset_ofst, (sa_FLIC_Header + flic_frame_offset_sgmt));
        FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
    }
    if( (flag_emm == ST_FALSE) && (flag_remap == ST_TRUE) )
    {
        DLOG("( (flag_emm == ST_FALSE) && (flag_remap == ST_TRUE) )");
        // FLIC_Draw_Frame_Remap();
        // _s27p02.asm  void FLIC_Draw_R(int x, int y, int width, unsigned int FLIC_Frame_Ofst, unsigned int FLIC_Frame_Sgmt);
        // FLIC_Draw_R(x, y, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
        // FLIC_Draw_R(x, y, PS_FLIC_Header.Width, flic_frame_offset_ofst, (sa_FLIC_Header + flic_frame_offset_sgmt));
        FLIC_Draw_R(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
    }
    if( (flag_emm == ST_TRUE) && (flag_remap == ST_FALSE) )
    {
        DLOG("( (flag_emm == ST_TRUE) && (flag_remap == ST_FALSE) )");
        // FLIC_Draw_Frame_EMM();
        // _s27p03c.c  void FLIC_Draw_EMM_C(int x, int y, SAMB_addr sa_FLIC_Header, int current_frame_index);
        FLIC_Draw_EMM_C(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
    }
    if( (flag_emm == ST_TRUE) && (flag_remap == ST_TRUE) )
    {
        DLOG("( (flag_emm == ST_TRUE) && (flag_remap == ST_TRUE) )");
        // FLIC_Draw_Frame_EMM_Remap();
        // _s27p03c.c  void FLIC_Draw_EMM_C(int x, int y, SAMB_addr sa_FLIC_Header, int current_frame_index);
        FLIC_Draw_EMM_C(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw(x = %d, y = %d, fp_FLIC_Header = %p)\n", __FILE__, __LINE__, x, y, fp_FLIC_Header);
#endif
}
