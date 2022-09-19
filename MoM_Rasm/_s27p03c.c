// _s27p03c.c  FLIC_Draw_EMM_C

#include <DOS.H>

#include "ST_HEAD.H"
#include "ST_TYPE.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_VGA.H"

#include "STU_BITS.H"
#include "STU_DBG.H"

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

//void draw_flic_frame(byte * src_buff, byte * dst_buff, word width, byte mask)
void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, SAMB_addr SAMB_data_FLIC_HDR, int Frame_Index)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    // int result;
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
    byte fh_Shading;
    byte _FAR * fp_SrcSgmt;  // EMM_PFBA           : 0
    byte _FAR * fp_DstSgmt;  // VRAM + Row Offset  : 0
    byte _FAR * fp_Src;      // EMM_PFBA           : EMM Offset
    byte _FAR * fp_Dst;      // VRAM + Row Offset  : Column Offset
    byte _FAR * fp_FlicHeader;
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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_Draw_EMM_C(ScreenPage_X=%d, ScreenPage_Y=%d, SAMB_data_FLIC_HDR=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR, Frame_Index);
#endif

        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝ PART I: Set-Up                                                   ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

    fh_Width = FPEEKW(SAMB_data_FLIC_HDR, 0x00);                 // FlicHdr_Width                MAINSCRN_LBX_000,0: 320     40 01
    fh_EmmHandleNumber = FPEEKB(SAMB_data_FLIC_HDR, 0x0A);       // FlicHdr_EmmHandleNumber      MAINSCRN_LBX_000,0: 6       06
    fh_EmmLogicalPageIndex = FPEEKB(SAMB_data_FLIC_HDR, 0x0B);   // FlicHdr_EmmLogicalPageIndex  MAINSCRN_LBX_000,0: 0       00
    fh_EmmLogicalPageOffset = FPEEKW(SAMB_data_FLIC_HDR, 0x0C);  // FlicHdr_EmmLogicalPageOffset MAINSCRN_LBX_000,0: 0x02C0  C0 02 00 00
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_Width: %u\n", __FILE__, __LINE__, fh_Width);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmHandleNumber: %u\n", __FILE__, __LINE__, fh_EmmHandleNumber);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmLogicalPageIndex: %u\n", __FILE__, __LINE__, fh_EmmLogicalPageIndex);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmLogicalPageOffset: 0x%04X\n", __FILE__, __LINE__, fh_EmmLogicalPageOffset);
#endif

    EMM_MapMulti4(fh_EmmLogicalPageIndex, fh_EmmHandleNumber);

    fh_FrameDataOffset = FPEEKDW(EMM_PageFrameBaseAddress, (fh_EmmLogicalPageOffset + (4 * Frame_Index) + FlicHdr_FrameOffsetTable));
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataOffset: 0x%08X\n", __FILE__, __LINE__, fh_FrameDataOffset);
#endif

    tmp_EmmPage = fh_EmmLogicalPageIndex  + ( (fh_FrameDataOffset + 1) / 16384 );
    tmp_EmmOfst = fh_EmmLogicalPageOffset + ( (fh_FrameDataOffset + 1) % 16384 );
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmPage: %u\n", __FILE__, __LINE__, tmp_EmmPage);
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmOfst: 0x%04X\n", __FILE__, __LINE__, tmp_EmmOfst);
#endif
    if ( tmp_EmmOfst > 0xC000 )
    {
        tmp_EmmPage += 3;
        tmp_EmmOfst -= 0xC000;
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmPage: %u\n", __FILE__, __LINE__, tmp_EmmPage);
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmOfst: 0x%04X\n", __FILE__, __LINE__, tmp_EmmOfst);
#endif

    fh_Shading = FPEEKB(EMM_PageFrameBaseAddress, fh_EmmLogicalPageOffset + FlicHdr_Shading);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_Shading: %u\n", __FILE__, __LINE__, fh_Shading);
#endif

    inregs.x.dx = fh_EmmHandleNumber;
    inregs.x.bx = tmp_EmmPage;
    inregs.h.al = 0;
    inregs.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs, &outregs);

    inregs.x.dx = fh_EmmHandleNumber;
    inregs.x.bx = tmp_EmmPage + 1;
    inregs.h.al = 1;
    inregs.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs, &outregs);

    if (fh_Shading != 0)
    {
        //FLIC_Draw_EMM_R(ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR);
        HERE("FLIC_Draw_EMM_R(ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR);");
        Quit();
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

    fp_Src = (byte _FAR *)MK_FP(EMM_PageFrameBaseAddress, tmp_EmmOfst);  // MAINSCRN_LBX_000: E000:0062F (0x02C0 + 0x0000036E + 1)
    fp_Dst = (byte _FAR *)MK_FP(gsa_DSP_Addr + ( ScreenPage_Y * (((320/4)/16)) ), row_offset);  // MAINSCRN_LBX_000: A400:0000
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fp_Src: %p\n", __FILE__, __LINE__, fp_Src);
    dlvfprintf("DEBUG: [%s, %d] fp_Dst: %p\n", __FILE__, __LINE__, fp_Dst);
#endif

    outportb(e_SC_INDEX, e_SC_MAPMASK);

Column_Loop:
    fp_Dst = (byte _FAR *)MK_FP(FP_SEG(fp_Dst), row_offset);
    if ( FP_OFF(fp_Src) > 0xC000 )
    {
        tmp_EmmPage += 3;
        EMM_MapMulti4(tmp_EmmPage, fh_EmmHandleNumber);
        tmp_EmmOfst = FP_OFF(fp_Src);
        tmp_EmmOfst -= 0xC000;
        fp_Src = (byte _FAR *)MK_FP(EMM_PageFrameBaseAddress, tmp_EmmOfst);
    }

    outportb(e_SC_DATA, mask);

    baito = *fp_Src++;
    if (baito == 0xFF)  /* Type: skip */
    {
        goto Next_Column;
    }

    if (baito == 0x80)  /* Type: copy */
    {
        packet_byte_count = *fp_Src++;
        do
        {
            sequence_byte_count = *fp_Src++;
            delta_byte_count = *fp_Src++;
            fp_Dst += (width_stride * delta_byte_count);
            packet_byte_count -= sequence_byte_count + 2;
            do
            {
                baito = *fp_Src++;
                if (baito >= 224)  /* op: repeat */  /* (& 11100000) */
                {
                    itr_op_repeat = (baito - 224) + 1;
                    --sequence_byte_count;
                    baito = *fp_Src++;
                    while (itr_op_repeat) {
                        *fp_Dst = baito;
                        fp_Dst += width_stride;
                        --itr_op_repeat;
                    }
                }
                else  /* op: copy */
                {
                    *fp_Dst = baito;
                    fp_Dst += width_stride;
                }
            } while ( sequence_byte_count-- );
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
            } while ( sequence_byte_count-- );
        } while (packet_byte_count >= 1);
        goto Next_Column;
    }

// decrement column count, increment map mask [, increment offset]
Next_Column:
    column_count--;
    if (column_count != 0)
    {
        // itr++;  rot = itr % 4
        mask = mask * 2;  // {1,2,4,8} * 2 = {2,4,8,16}
        if (mask >= 9)
        {
            mask = 1;
            row_offset++;
        }
        goto Column_Loop;
    }

Done:

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_EMM_C(ScreenPage_X=%d, ScreenPage_Y=%d, SAMB_data_FLIC_HDR=0x%03X, Frame_Index=%d)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR, Frame_Index);
#endif

}
