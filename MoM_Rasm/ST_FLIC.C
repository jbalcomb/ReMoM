
#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_FLIC.H"
#include "ST_SA.H"
#include "ST_EMM.H"     /* fp_EMM_PFBA */
#include "ST_VGA.H"     /* e_SC_INDEX, e_SC_DATA, e_SC_MAPMASK, etc. */

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


// #include "STU_BITS.H"   /* fh_FrameDataOffset = FPEEKDW(); fh_Shading = FPEEKB() */

#define FPEEKB( _sgmt_,_ofst_) ( *(( byte _FAR *)MK_FP((_sgmt_),(_ofst_))) )

// NIU #define FPEEKW( _sgmt_,_ofst_) ( *(( word _FAR *)MK_FP((_sgmt_),(_ofst_))) )

#define FPEEKDW(_sgmt_,_ofst_) ( *((dword _FAR *)MK_FP((_sgmt_),(_ofst_))) )


unsigned char VGA_WriteMapMasks[4] = {0x01, 0x02, 0x04, 0x08}; // dseg:4372


// // 1oom::lbxgfx.c::static void lbxgfx_draw_pixels_fmt0(uint8_t *pixbuf, uint16_t w, uint8_t *data, uint16_t pitch)
// void FLIC_RLE_Decode(BYTE * FLIC_Frame_Data)
// {
//     
// }



// s27p01
/*
    Column-Wise

Part 1:
    set up
Part 2:
    read SRAM, write VRAM
    decode RLE
*/
// void draw_flic_frame(byte * src_buff, byte * dst_buff, word width, byte mask)
// void FLIC_Draw_A(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg)
void FLIC_Draw_Frame(int x, int y, int width, unsigned int FLIC_Frame_ofst, unsigned int FLIC_Frame_sgmt)
{
    byte * fp_SrcSgmt;  // EMM_PFBA           : 0
    byte * fp_DstSgmt;  // VRAM + Row Offset  : 0
    byte * fp_Src;      // EMM_PFBA           : EMM Offset
    byte * fp_Dst;      // VRAM + Row Offset  : Column Offset
    byte * fp_FlicHeader;
    word row_offset;
    byte mask;
    word column_count;
    byte baito;
    byte packet_byte_count;
    byte sequence_byte_count;
    byte delta_byte_count;
    word width_stride;
    byte itr_op_repeat;
    unsigned int itr_Src;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_Frame(x = %d, y = %d, width = %d, FLIC_Frame_ofst=0x%04X, FLIC_Frame_sgmt=0x%04X)\n", __FILE__, __LINE__, x, y, width, FLIC_Frame_ofst, FLIC_Frame_sgmt);
#endif

        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝  PART II: Read, Decode, Write                                    ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║  Preamble:                                                          ║
        ║      set SC_INDEX to SC_MAPMASK                                     ║
        ║      calculate VGA Memory-Map Mask for first pixel                  ║
        ║      calculate Offset in Row for first pixel                        ║
        ║                                                                     ║
        ║  Process Column Data:                                               ║
        ║      set the Column Offset                                          ║
        ║      set the VGA Memory-Map Mask                                    ║
        ║      read byte - Column Type                                        ║
        ║          branch on Column Type                                      ║
        ║              skip:                                                  ║
        ║              run:                                                   ║
        ║              decode:                                                ║
        ║                  branch on Op                                       ║
        ║                      copy                                           ║
        ║                      repeat                                         ║
        ║                                                                     ║
        ║  Postamble:                                                         ║
        ║      decrement the Column Count                                     ║
        ║      rotate the VGA Memory-Map Mask                                 ║
        ║      increment the Row Offset                                       ║
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

    width_stride = 80;
    column_count = width;
    row_offset = (x / 4);  // this gets increment at the end of the column loop to be the start of the next column

    mask = VGA_WriteMapMasks[(x & 0x03)];  // ~== x modulo 4  (x % 4, x|4)

    // fp_Src = (byte *) MK_FP(EMM_PageFrameBaseAddress, tmp_EmmOfst);  // MAINSCRN_LBX_000: E000:0062F (0x02C0 + 0x0000036E + 1)
    fp_Src = (byte *) MK_FP(FLIC_Frame_sgmt, FLIC_Frame_ofst);
    fp_Dst = (byte *) MK_FP(gsa_DSP_Addr + (y * (((320/4)/16))), row_offset);  // MAINSCRN_LBX_000: A400:0000

    outportb(e_SC_INDEX, e_SC_MAPMASK);

Column_Loop:
    fp_Dst = (byte *) MK_FP(FP_SEG(fp_Dst), row_offset);
    outportb(e_SC_DATA, mask);

    baito = *fp_Src++;
    if(baito == 0xFF)  /* Type: skip */
    {
        goto Next_Column;
    }

    if(baito == 0x80)  /* Type: copy */
    {
        packet_byte_count = *fp_Src++;
        do {
            sequence_byte_count = *fp_Src++;
            delta_byte_count = *fp_Src++;
            fp_Dst += (width_stride * delta_byte_count);
            packet_byte_count -= sequence_byte_count + 2;  // MAINSCRN_000_000, Column Index 0; 0 = 29 - (27 + 2)
            do {
                baito = *fp_Src++;  // this byte is the op-repeat or just the byte to copy
                if(baito >= 224)  /* op: repeat */  /* (& 11100000) */
                {
                    itr_op_repeat = (baito - 224) + 1;
                    sequence_byte_count--;  // ? decremented here, because of the byte read immediately following ?
                    baito = *fp_Src++;
                    while(itr_op_repeat--)
                    {
                        *fp_Dst = baito;
                        fp_Dst += width_stride;
                        //itr_op_repeat--;
                    }
                }
                else  /* op: copy */
                {
                    *fp_Dst = baito;
                    fp_Dst += width_stride;
                }
            } while (--sequence_byte_count);  // pre decr sequence_byte_count, not post decr
        } while (packet_byte_count >= 1);
        goto Next_Column;
    }

    if (baito == 0x00)  /* Type: copy */
    {
        packet_byte_count = *fp_Src++;
        do {
            sequence_byte_count = *fp_Src++;
            delta_byte_count = *fp_Src++;
            fp_Dst += (width_stride * delta_byte_count);
            packet_byte_count -= sequence_byte_count + 2;
            do {
                *fp_Dst = *fp_Src++;
                fp_Dst += width_stride;
            } while (--sequence_byte_count);  // pre decr sequence_byte_count, not post decr
        } while(packet_byte_count >= 1);
        goto Next_Column;
    }

// decrement column count, increment map mask [, increment offset]
Next_Column:
    column_count--;
    if(column_count != 0)
    {
        // itr++;  rot = itr % 4
        mask = mask * 2;  // {1,2,4,8} * 2 = {2,4,8,16}
        if(mask >= 9)
        {
            mask = 1;
            row_offset++;
        }
        goto Column_Loop;
    }

Done:
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_Frame(x = %d, y = %d, width = %d, FLIC_Frame_ofst=0x%04X, FLIC_Frame_sgmt=0x%04X)\n", __FILE__, __LINE__, x, y, width, FLIC_Frame_ofst, FLIC_Frame_sgmt);
#endif
}



// s27p03
/*
    Column-Wise

Part 1:
    set up
Part 2:
    read EMM, write VRAM
    decode RLE
*/
/*

    fp_SrcSgmt is EMM
    fp_DstSgmt is VRAM

*/
// void draw_flic_frame(byte * src_buff, byte * dst_buff, word width, byte mask)
void FLIC_Draw_Frame_EMM(int x, int y, SAMB_addr sa_FLIC_Header, int Frame_Index)
{
    
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    // int result;
    byte _FAR * fp_FLIC_Header;
    byte _FAR * fp_FLIC_File;
    byte _FAR * fp_FLIC;
    word fh_Width;
    byte fh_EmmHandleNumber;
    byte fh_EmmLogicalPageIndex;
    word fh_EmmLogicalPageOffset;
    word fh_FrameDataOffset_LoWord;
    word fh_FrameDataOffset_HiWord;
    dword fh_FrameDataOffset;
    dword fh_FrameDataOffset0;
    dword fh_FrameDataOffset1;
    dword fh_FrameDataSize;
    // unsigned char * fp_FlicHeader_Shading;
    byte fh_Shading;
    byte * fp_SrcSgmt;  // EMM_PFBA           : 0
    byte * fp_DstSgmt;  // VRAM + Row Offset  : 0
    byte * fp_Src;      // EMM_PFBA           : EMM Offset
    byte * fp_Dst;      // VRAM + Row Offset  : Column Offset
    byte * fp_FlicHeader;
    byte tmp_EmmHandle;
    byte tmp_EmmPage;
    word tmp_EmmOfst;
    word width;
    word row_offset;
    byte mask;
    word column_count;
    byte baito;
    byte packet_byte_count;
    byte sequence_byte_count;
    byte delta_byte_count;
    word width_stride;
    byte itr_op_repeat;
    unsigned int itr_Src;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_Frame_EMM(x = %d, y = %d, sa_FLIC_Header = 0x%04X, Frame_Index = %d)\n", __FILE__, __LINE__, x, y, sa_FLIC_Header, Frame_Index);
#endif

        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝ PART I: Set-Up                                                   ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

    fp_FLIC_Header = (SAMB_ptr) MK_FP(sa_FLIC_Header,0);

    fh_Width = FLIC_Get_Width(fp_FLIC_Header);
    fh_EmmHandleNumber = FLIC_Get_EMM_Handle_Number(fp_FLIC_Header);
    fh_EmmLogicalPageIndex = FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_Header);
    fh_EmmLogicalPageOffset = FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_Header);

    EMM_MapMulti4(fh_EmmLogicalPageIndex, fh_EmmHandleNumber);

    fp_FLIC_File = (byte_ptr)MK_FP(EMM_PageFrameBaseAddress,fh_EmmLogicalPageOffset);

    fh_FrameDataOffset = FPEEKDW(EMM_PageFrameBaseAddress, (fh_EmmLogicalPageOffset + (4 * Frame_Index) + FlicHdr_FrameOffsetTable));
    // fp_FLIC = (unsigned char *) MK_FP(EMM_PageFrameBaseAddress, fh_EmmLogicalPageOffset);
    // fh_FrameDataOffset = FLIC_Get_FrameDataOffset(fp_FLIC, Frame_Index);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fh_FrameDataOffset: 0x%08X\n", __FILE__, __LINE__, fh_FrameDataOffset);
#endif

    tmp_EmmPage = fh_EmmLogicalPageIndex  + ( (fh_FrameDataOffset + 1) / 16384 );
    tmp_EmmOfst = fh_EmmLogicalPageOffset + ( (fh_FrameDataOffset + 1) % 16384 );

    if ( tmp_EmmOfst > 0xC000 )
    {
        tmp_EmmPage += 3;
        tmp_EmmOfst -= 0xC000;  // 3 * 16384 EMM Page Size
    }

    fh_Shading = FPEEKB(EMM_PageFrameBaseAddress, fh_EmmLogicalPageOffset + FlicHdr_Shading);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fh_Shading: %d\n", __FILE__, __LINE__, fh_Shading);
#endif
    // // // fh_Shading = FLIC_Get_Remap(fp_FLIC);
    // // fh_Shading = fp_FLIC[17];
    // fp_FlicHeader_Shading = (unsigned char *)MK_FP(EMM_PageFrameBaseAddress, (fh_EmmLogicalPageOffset + FlicHdr_Shading));
    // fh_Shading = *fp_FlicHeader_Shading;
    fh_Shading = GET_2B_OFS(fp_FLIC_File, FLIC_HDR_POS_REMAP);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fh_Shading: %d\n", __FILE__, __LINE__, fh_Shading);
#endif
    fh_Shading = FLIC_Get_Remap(fp_FLIC_File);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fh_Shading: %d\n", __FILE__, __LINE__, fh_Shading);
#endif


    inregs.x.dx = fh_EmmHandleNumber;
    inregs.x.bx = tmp_EmmPage;
    inregs.h.al = 0;
    inregs.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs, &outregs);
    // EMM_MAP_PAGE(0, 4, 0);

    inregs.x.dx = fh_EmmHandleNumber;
    inregs.x.bx = tmp_EmmPage + 1;
    inregs.h.al = 1;
    inregs.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs, &outregs);
    // EMM_MAP_PAGE(0, 4, 0);

    if (fh_Shading != 0)
    {
        DLOG("(fh_Shading != 0)");
        //FLIC_Draw_EMM_R(ScreenPage_X, ScreenPage_Y, sa_FLIC_Header);
        Exit("(fh_Shading != 0)");
    }


        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝  PART II: Read, Decode, Write                                    ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║  Preamble:                                                          ║
        ║      set SC_INDEX to SC_MAPMASK                                     ║
        ║      calculate VGA Memory-Map Mask for first pixel                  ║
        ║      calculate Offset in Row for first pixel                        ║
        ║                                                                     ║
        ║  Process Column Data:                                               ║
        ║      set the Column Offset                                          ║
        ║      set the VGA Memory-Map Mask                                    ║
        ║      read byte - Column Type                                        ║
        ║          branch on Column Type                                      ║
        ║              skip:                                                  ║
        ║              run:                                                   ║
        ║              decode:                                                ║
        ║                  branch on Op                                       ║
        ║                      copy                                           ║
        ║                      repeat                                         ║
        ║                                                                     ║
        ║  Postamble:                                                         ║
        ║      decrement the Column Count                                     ║
        ║      rotate the VGA Memory-Map Mask                                 ║
        ║      increment the Row Offset                                       ║
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

    width_stride = 80;
    column_count = fh_Width;
    row_offset = (x / 4);  // this gets increment at the end of the column loop to be the start of the next column

    mask = VGA_WriteMapMasks[(x & 0x03)];  // ~== x modulo 4  (x % 4, x|4)

    fp_Src = (byte *) MK_FP(EMM_PageFrameBaseAddress, tmp_EmmOfst);  // MAINSCRN_LBX_000: E000:0062F (0x02C0 + 0x0000036E + 1)
    fp_Dst = (byte *) MK_FP(gsa_DSP_Addr + (y * (((320/4)/16))), row_offset);  // MAINSCRN_LBX_000: A400:0000

    outportb(e_SC_INDEX, e_SC_MAPMASK);

Column_Loop:
    fp_Dst = (byte *) MK_FP(FP_SEG(fp_Dst), row_offset);
    if(FP_OFF(fp_Src) > 0xC000)
    {
        tmp_EmmPage += 3;
        EMM_MapMulti4(tmp_EmmPage, fh_EmmHandleNumber);
        tmp_EmmOfst = FP_OFF(fp_Src);
        tmp_EmmOfst -= 0xC000;
        fp_Src = (byte *) MK_FP(EMM_PageFrameBaseAddress, tmp_EmmOfst);
    }
    outportb(e_SC_DATA, mask);

    baito = *fp_Src++;
    if(baito == 0xFF)  /* Type: skip */
    {

        goto Next_Column;
    }

    if(baito == 0x80)  /* Type: copy */
    {
        packet_byte_count = *fp_Src++;
        do {
            sequence_byte_count = *fp_Src++;
            delta_byte_count = *fp_Src++;
            fp_Dst += (width_stride * delta_byte_count);
            packet_byte_count -= sequence_byte_count + 2;  // MAINSCRN_000_000, Column Index 0; 0 = 29 - (27 + 2)
            do
            {
                baito = *fp_Src++;  // this byte is the op-repeat or just the byte to copy
                if(baito >= 224)  /* op: repeat */  /* (& 11100000) */
                {
                    itr_op_repeat = (baito - 224) + 1;
                    sequence_byte_count--;  // ? decremented here, because of the byte read immediately following ?
                    baito = *fp_Src++;
                    while(itr_op_repeat--)
                    {
                        *fp_Dst = baito;
                        fp_Dst += width_stride;
                        //itr_op_repeat--;
                    }
                }
                else  /* op: copy */
                {
                    *fp_Dst = baito;
                    fp_Dst += width_stride;
                }
            } while (--sequence_byte_count);  // pre decr sequence_byte_count, not post decr
        } while (packet_byte_count >= 1);
        goto Next_Column;
    }

    if (baito == 0x00)  /* Type: copy */
    {
        packet_byte_count = *fp_Src++;
        do {
            sequence_byte_count = *fp_Src++;
            delta_byte_count = *fp_Src++;
            fp_Dst += (width_stride * delta_byte_count);
            packet_byte_count -= sequence_byte_count + 2;
            do {
                *fp_Dst = *fp_Src++;
                fp_Dst += width_stride;
            } while(--sequence_byte_count);  // pre decr sequence_byte_count, not post decr
        } while(packet_byte_count >= 1);
        goto Next_Column;
    }

// decrement column count, increment map mask [, increment offset]
Next_Column:
    column_count--;
    if(column_count != 0)
    {
        // itr++;  rot = itr % 4
        mask = mask * 2;  // {1,2,4,8} * 2 = {2,4,8,16}
        if(mask >= 9)
        {
            mask = 1;
            row_offset++;
        }
        goto Column_Loop;
    }

Done:
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw_Frame_EMM(x = %d, y = %d, sa_FLIC_Header = 0x%04X, Frame_Index = %d)\n", __FILE__, __LINE__, x, y, sa_FLIC_Header, Frame_Index);
#endif
}


// s28p11
/*
    Update Current Frame
    Test Palette, EMM, & Remap
    Dispatch to Draw functions
*/
// ~== MoO2  void Draw(signed short int x, signed short int y, unsigned char * picture)
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
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
        FLIC_Draw_Frame(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
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
        // FLIC_Draw_EMM_C(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
        FLIC_Draw_Frame_EMM(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
    }
    if( (flag_emm == ST_TRUE) && (flag_remap == ST_TRUE) )
    {
        DLOG("( (flag_emm == ST_TRUE) && (flag_remap == ST_TRUE) )");
        // FLIC_Draw_Frame_EMM_Remap();
        // _s27p03c.c  void FLIC_Draw_EMM_C(int x, int y, SAMB_addr sa_FLIC_Header, int current_frame_index);
        // FLIC_Draw_EMM_C(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
        FLIC_Draw_Frame_EMM(x, y, FP_SEG(fp_FLIC_Header), current_frame_index);
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw(x = %d, y = %d, fp_FLIC_Header = %p)\n", __FILE__, __LINE__, x, y, fp_FLIC_Header);
#endif
}
