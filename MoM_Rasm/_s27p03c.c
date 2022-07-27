// _s27p03c.c  FLIC_Draw_EMM_C
#include "ST_HEAD.H"

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
void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, unsigned int SAMB_data_FLIC_HDR, int Frame_Index)
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
#ifdef DEBUG
    if (SAMB_data_FLIC_HDR == DBG_MAINSCRN_000)
        dlvfprintf("DEBUG: [%s, %d] Drawing MAINSCRN_000\n", __FILE__, __LINE__);
        dlvfprintf("DEBUG: [%s, %d] Drawing MAINSCRN_000_%02u\n", __FILE__, __LINE__, Frame_Index);
    if (SAMB_data_FLIC_HDR == DBG_MAINSCRN_005)
        dlvfprintf("DEBUG: [%s, %d] Drawing MAINSCRN_005\n", __FILE__, __LINE__);
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
    dlvfprintf("DEBUG: [%s, %d] fh_Width: 0x%04X (%u)\n", __FILE__, __LINE__, fh_Width, fh_Width);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmHandleNumber: 0x%02X (%u)\n", __FILE__, __LINE__, fh_EmmHandleNumber, fh_EmmHandleNumber);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmLogicalPageIndex: 0x%02X (%u)\n", __FILE__, __LINE__, fh_EmmLogicalPageIndex, fh_EmmLogicalPageIndex);
    dlvfprintf("DEBUG: [%s, %d] fh_EmmLogicalPageOffset: 0x%04X (%u)\n", __FILE__, __LINE__, fh_EmmLogicalPageOffset, fh_EmmLogicalPageOffset);
#endif

    EMM_MapMulti4(fh_EmmLogicalPageIndex, fh_EmmHandleNumber);

    //SrcOfst = fh_EmmLogicalPageOffset + (4 * Frame_Index) + FlicHdr_FrameOffsetTable;  // ? 0x02d2 ? offset in LBX File in EMM to FLIC File Frame Offset Table for Frame_Index?
    //fh_FrameDataOffset_LoWord = FPEEKW(g_EMM_PageFrame_Base_Address, SrcOfst);
    //fh_FrameDataOffset_HiWord = FPEEKW(g_EMM_PageFrame_Base_Address, SrcOfst);
    fh_FrameDataOffset = FPEEKDW(g_EMM_PageFrame_Base_Address, (fh_EmmLogicalPageOffset + (4 * Frame_Index) + FlicHdr_FrameOffsetTable));
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataOffset: 0x%08lX (%lu)\n", __FILE__, __LINE__, fh_FrameDataOffset, fh_FrameDataOffset);  // MAINSCRN_LBX_000: 0x0000036E  6E 03 00 00
    dlvfprintf("DEBUG: [%s, %d] (fh_FrameDataOffset / 16384): 0x%08lX (%lu)\n", __FILE__, __LINE__, (fh_FrameDataOffset / 16384), (fh_FrameDataOffset / 16384));
#endif

#ifdef DEBUG
    fh_FrameDataOffset0 = FPEEKDW(g_EMM_PageFrame_Base_Address, (fh_EmmLogicalPageOffset + (4 * (Frame_Index + 0)) + FlicHdr_FrameOffsetTable));
    fh_FrameDataOffset1 = FPEEKDW(g_EMM_PageFrame_Base_Address, (fh_EmmLogicalPageOffset + (4 * (Frame_Index + 1)) + FlicHdr_FrameOffsetTable));
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataOffset0: 0x%08lX (%lu)\n", __FILE__, __LINE__, fh_FrameDataOffset0, fh_FrameDataOffset0);
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataOffset1: 0x%08lX (%lu)\n", __FILE__, __LINE__, fh_FrameDataOffset1, fh_FrameDataOffset1);
    fh_FrameDataSize = (fh_FrameDataOffset1 - fh_FrameDataOffset0);
    dlvfprintf("DEBUG: [%s, %d] fh_FrameDataSize: 0x%08lX (%lu)\n", __FILE__, __LINE__, fh_FrameDataSize, fh_FrameDataSize);
#endif
/*
mov  ax, [bp+Frame_Index]                       ; e.g., Frame_Index=2  ES=885D
shl  ax, 1                                      ; e.g., AX=4
shl  ax, 1                                      ; e.g., AX=8
add  si, ax                                     ; e.g., SI=02C8
add  si, e_FlicHdr_FrameOffsetTable             ; e.g., SI=02DA
mov  ax, [gsa_EMM_PageFrameBaseAddress]
mov  ds, ax                                     ; e.g., DS=E000
lodsw                                           ; e.g., AX=4ED4
mov  dx, ax                                     ; e.g., AX=4ED4
lodsw                                           ; e.g., AX=0000
inc  dx                                         ; e.g., DX=4ED5
adc  ax, 0                                      ; e.g., AX=0000
shl  dx, 1                                      ; e.g., DX=9DAA
rcl  ax, 1                                      ; e.g., AX=0000
shl  dx, 1                                      ; e.g., DX=3B54
rcl  ax, 1                                      ; e.g., AX=0001
shr  dx, 1                                      ; e.g., DX=1DAA
shr  dx, 1                                      ; e.g., DX=0ED5
add  al, [es:e_FlicHdr_EmmLogicalPageIndex]     ; e.g., AX=0001
adc  ah, 0                                      ; e.g., AX=0001
add  dx, [es:e_FlicHdr_EmmLogicalPageOffset]    ; e.g., DX=1195
*/

    tmp_EmmPage = fh_EmmLogicalPageIndex  + ( (fh_FrameDataOffset + 1) / 16384 );
    tmp_EmmOfst = fh_EmmLogicalPageOffset + ( (fh_FrameDataOffset + 1) % 16384 );
    if ( tmp_EmmOfst > 0xC000 )
    {
        HERE("tmp_EmmOfst > 0xC000");
        tmp_EmmPage += 3;
        tmp_EmmOfst -= 0xC000;
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmPage: %u\n", __FILE__, __LINE__, tmp_EmmPage);  // MAINSCRN_LBX_000: 
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmOfst: 0x%04X\n", __FILE__, __LINE__, tmp_EmmOfst);  // MAINSCRN_LBX_000: 
#endif

    fh_Shading = FPEEKB(g_EMM_PageFrame_Base_Address, fh_EmmLogicalPageOffset + FlicHdr_Shading);

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
        //FLIC_Draw_EMM_R(ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR);
        HERE("FLIC_Draw_EMM_R(ScreenPage_X, ScreenPage_Y, SAMB_data_FLIC_HDR);");



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

    fp_Src = (byte _FAR *)MK_FP(g_EMM_PageFrame_Base_Address, tmp_EmmOfst);  // MAINSCRN_LBX_000: E000:0062F (0x02C0 + 0x0000036E + 1)
    fp_Dst = (byte _FAR *)MK_FP(gsa_DSP_Addr + ( ScreenPage_Y * (((320/4)/16)) ), row_offset);  // MAINSCRN_LBX_000: A400:0000

// #ifdef DEBUG
//     for (itr_Src = 0; itr_Src < 256; itr_Src++)
//     {
//         baito = *fp_Src++;
//         dlvfprintf("DEBUG: [%s, %d] fp_Src[%u]: 0x%02X\n", __FILE__, __LINE__, itr_Src, baito);
//     }
//     baito = 0;
//     fp_Src = (byte _FAR *)MK_FP(g_EMM_PageFrame_Base_Address, tmp_EmmOfst);  // MAINSCRN_LBX_000: E000:0062F (0x02C0 + 0x0000036E + 1)
// #endif

    outportb(e_SC_INDEX, e_SC_MAPMASK);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fh_EmmHandleNumber: 0x%02X\n", __FILE__, __LINE__, fh_EmmHandleNumber);
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmPage: 0x%02X\n", __FILE__, __LINE__, tmp_EmmPage);
    dlvfprintf("DEBUG: [%s, %d] tmp_EmmOfst: 0x%04X\n", __FILE__, __LINE__, tmp_EmmOfst);
    dlvfprintf("DEBUG: [%s, %d] mask: 0x%02X\n", __FILE__, __LINE__, mask);
    dlvfprintf("DEBUG: [%s, %d] fp_Src: %Fp\n", __FILE__, __LINE__, fp_Src);
    dlvfprintf("DEBUG: [%s, %d] fp_Dst: %Fp\n", __FILE__, __LINE__, fp_Dst);
    if (SAMB_data_FLIC_HDR == DBG_MAINSCRN_000)
    {
        // if mask != 0x01
        // if fp_Src != E000:
        // if fp_Dst != A400:0000
        // if fh_EmmHandleNumber != 6
        // if tmp_EmmPage != 0
        // if tmp_EmmOfst != 062F
        // if ( !((tmp_EmmPage != 0) & (tmp_EmmOfst != 062F)) )

    }
#endif

Column_Loop:
    // p_Src = (byte _FAR *) MK_FP(SrcSgmt, SrcOfst);
    // p_Dst = (byte _FAR *) MK_FP(DstSgmt, DstOfst);
    fp_Dst = (byte _FAR *)MK_FP(FP_SEG(fp_Dst), row_offset);
    if ( FP_OFF(fp_Src) > 0xC000 )
    {
        HERE("FP_OFF(fp_Src) > 0xC000");
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] fp_Src: %Fp\n", __FILE__, __LINE__, fp_Src);  // E000:036F
    dlvfprintf("DEBUG: [%s, %d] FP_OFF(fp_Src): 0x%04X\n", __FILE__, __LINE__, FP_OFF(fp_Src));
#endif
        tmp_EmmPage += 3;
        EMM_MapMulti4(tmp_EmmPage, fh_EmmHandleNumber);
        tmp_EmmOfst = FP_OFF(fp_Src);
        tmp_EmmOfst -= 0xC000;
        fp_Src = (byte _FAR *)MK_FP(g_EMM_PageFrame_Base_Address, tmp_EmmOfst);
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] mask: 0x%02X\n", __FILE__, __LINE__, mask);  // {01,02,04,08}
    dlvfprintf("DEBUG: [%s, %d] fp_Src: %Fp\n", __FILE__, __LINE__, fp_Src);  // E000:{}
    dlvfprintf("DEBUG: [%s, %d] fp_Dst: %Fp\n", __FILE__, __LINE__, fp_Dst);  // {A000,A400}:{}
    // MAINSCRN_000:
    //     E000:{062F,...,35EF}
    //     A400:{0000,...,004F}
    // MAINSCRN_005:
    //     E000:{192B,...,BF23}
    //     A4CD:{0000,...,004F} ... A400 + 00CD (205d)
#endif

    outportb(e_SC_DATA, mask);

    baito = *fp_Src++;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] baito: 0x%02X\n", __FILE__, __LINE__, baito);  // MAINSCRN_LBX_000: 80 1A 18 00 E1 0D 0B 09 07 09 E2 0B 0D 0B E7 0D E1 15 E5 0D E7 15 0D 0B E1 0D E1 0B
#endif
#ifdef DEBUG
    if ( baito != 0xFF && baito != 0x80 && baito != 0x00 )
    {
        dlvfprintf("DEBUG: [%s, %d] FATAL: baito != [0xFF|0x80|0x00]: 0x%02X\n", __FILE__, __LINE__, baito);
        Quit("FLAGRANT ERROR!!");
    }
#endif
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
            } while (--sequence_byte_count);
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
            } while (--sequence_byte_count);
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
