// ~== ST_FLIC
// seg027

#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_FLIC.H"

#include "ST_EMM.H"
#include "ST_EXIT.H"  /* Exit() */
#include "ST_SA.H"  /* SAMB_addr; farpokew(); */
#include "ST_VGA.H"     /* e_SC_INDEX, e_SC_DATA, e_SC_MAPMASK, etc. */


// #include "STU_BITS.H"   /* fh_FrameDataOffset = FPEEKDW(); fh_Shading = FPEEKB() */

#define FPEEKB( _sgmt_,_ofst_) ( *(( byte _FAR *)MK_FP((_sgmt_),(_ofst_))) )

// NIU #define FPEEKW( _sgmt_,_ofst_) ( *(( word _FAR *)MK_FP((_sgmt_),(_ofst_))) )

#define FPEEKDW(_sgmt_,_ofst_) ( *((dword _FAR *)MK_FP((_sgmt_),(_ofst_))) )


#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



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
void DELETE__FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, SAMB_addr sa_FLIC_Header, int Frame_Index)
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
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_EMM_C(ScreenPage_X = %d, ScreenPage_Y = %d, sa_FLIC_Header = 0x%04X, Frame_Index = %d)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, sa_FLIC_Header, Frame_Index);
#endif

        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝ PART I: Set-Up                                                   ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

    fp_FLIC_Header = (SAMB_ptr) MK_FP(sa_FLIC_Header,0);



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
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Remap(fp_FLIC_Header): %d)\n", __FILE__, __LINE__, FLIC_Get_Remap(fp_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset_Table(fp_FLIC_Header): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset_Table(fp_FLIC_Header));
#endif


    // fh_Width = FPEEKW(sa_FLIC_Header, 0x00);                 // FlicHdr_Width                MAINSCRN_LBX_000,0: 320     40 01
    // fh_EmmHandleNumber = FPEEKB(sa_FLIC_Header, 0x0A);       // FlicHdr_EmmHandleNumber      MAINSCRN_LBX_000,0: 6       06
    // fh_EmmLogicalPageIndex = FPEEKB(sa_FLIC_Header, 0x0B);   // FlicHdr_EmmLogicalPageIndex  MAINSCRN_LBX_000,0: 0       00
    // fh_EmmLogicalPageOffset = FPEEKW(sa_FLIC_Header, 0x0C);  // FlicHdr_EmmLogicalPageOffset MAINSCRN_LBX_000,0: 0x02C0  C0 02 00 00
    fh_Width = FLIC_Get_Width(fp_FLIC_Header);
    fh_EmmHandleNumber = FLIC_Get_EMM_Handle_Number(fp_FLIC_Header);
    fh_EmmLogicalPageIndex = FLIC_Get_EMM_Logical_Page_Number(fp_FLIC_Header);
    fh_EmmLogicalPageOffset = FLIC_Get_EMM_Logical_Page_Offset(fp_FLIC_Header);

    EMM_MapMulti4(fh_EmmLogicalPageIndex, fh_EmmHandleNumber);

    fp_FLIC_File = (byte_ptr)MK_FP(EMM_PageFrameBaseAddress,fh_EmmLogicalPageOffset);


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
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Remap(fp_FLIC_File): %d)\n", __FILE__, __LINE__, FLIC_Get_Remap(fp_FLIC_File));
    dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset(fp_FLIC_File,Frame_Index): 0x%08X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset(fp_FLIC_File,Frame_Index));
#endif


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


// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Width(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Width(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Height(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Height(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Current_Frame(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Current_Frame(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Count(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Count(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Loop_Frame(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Loop_Frame(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Handle_Number(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Handle_Number(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Number(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Number(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_EMM_Logical_Page_Offset(fp_Src): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_EMM_Logical_Page_Offset(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Palette_Header_Offset(fp_Src): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Palette_Header_Offset(fp_Src));
// 
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Type(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Frame_Type(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Remap(fp_Src): %d)\n", __FILE__, __LINE__, FLIC_Get_Remap(fp_Src));
//     dbg_prn("DEBUG: [%s, %d] FLIC_Get_Frame_Offset_Table(fp_Src): 0x%04X)\n", __FILE__, __LINE__, FLIC_Get_Frame_Offset_Table(fp_Src));
// #endif


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
    row_offset = (ScreenPage_X / 4);  // this gets increment at the end of the column loop to be the start of the next column

    mask = g_VGA_WriteMapMasks3[(ScreenPage_X & 0x03)];  // ~== x modulo 4  (x % 4, x|4)

    fp_Src = (byte *) MK_FP(EMM_PageFrameBaseAddress, tmp_EmmOfst);  // MAINSCRN_LBX_000: E000:0062F (0x02C0 + 0x0000036E + 1)
    fp_Dst = (byte *) MK_FP(gsa_DSP_Addr + (ScreenPage_Y * (((320/4)/16))), row_offset);  // MAINSCRN_LBX_000: A400:0000

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
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw_EMM_C(ScreenPage_X = %d, ScreenPage_Y = %d, sa_FLIC_Header = 0x%04X, Frame_Index = %d)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, sa_FLIC_Header, Frame_Index);
#endif
}
