
#include "ST_CRSR.H";
#include "ST_GUI.H";  // g_CRSR_Curr, g_CRSR_HaveSave, gsa_Cursor_Array, g_RSP_Idx
#include "ST_VGA.H";  // g_RSP_Idx

void CR_Draw_C(int CR_X, int CR_Y, int ScreenPage_Idx)
{

    int row_offset; // Offset_In_Scanline;  // AKA Row_Offset
    int draw_width;
    int draw_height;
    int itr_width;
    int itr_height;
    unsigned char mask;  // Sequence Controller - Memory-Map Mask/Bitfield
    byte baito;
    byte * pSrc;
    byte * pDst;
    word Src_Ofst;
    word Dst_Ofst;
    word Src_Sgmt;
    int itr_count;
    int itr_plane;
#ifdef DEBUG
    int DBG_Row_Offset;
    int DBG_Column_Offset;
    word DBG_ScreenPage_Offset;
    word DBG_Src_Sgmt;
    word * DBG_pScreenPage;
    word wordo;
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: CR_Draw_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
#endif

    if ( g_CRSR_Curr == 0 ) { goto Done; }
    if ( CR_Y >= PIXY ) { goto Done; }
    if ( CR_X >= PIXX ) { goto Done; }
    //if ( draw_height = (PIXY - CR_Y) > (PIXY - (PIXY - CURSOR_HEIGHT)) ) { draw_height = CURSOR_HEIGHT; }
    //if ( draw_width = (PIXX - CR_X) > (PIXX - (PIXX - CURSOR_WIDTH)) ) { draw_width = CURSOR_WIDTH; }
    draw_height = (PIXY - CR_Y);
    if ( draw_height > (PIXY - (PIXY - CURSOR_HEIGHT)) ) { draw_height = CURSOR_HEIGHT; }
    draw_width = (PIXX - CR_X);
    if ( draw_width > (PIXX - (PIXX - CURSOR_WIDTH)) ) { draw_width = CURSOR_WIDTH; }
    itr_width = draw_width;
    itr_height = draw_height;

    row_offset = (CR_X >> 2);

    pSrc = (byte *)MK_FP(gsa_Cursor_Array + (16 * (g_CRSR_Curr - 1)), 0);
    Src_Ofst = 0;

    pDst = (byte *)MK_FP((VRAM_BASE + ((ScreenPage_Idx - g_RSP_Idx) * 4) + (CR_Y * 5)), 0);
    Dst_Ofst = row_offset;

#ifdef DEBUG
/*
    DEBUG: as seen in Store & Restore
*/
    DBG_Row_Offset = (CR_X >> 2);  // PIXELS_PER_ADDRESS
    DBG_Column_Offset = CR_Y * 80;  // SCREEN_WIDTH_PER_PLANE
    DBG_ScreenPage_Offset = DBG_Column_Offset + DBG_Row_Offset;
    DBG_Src_Sgmt = VRAM_BASE + (((ScreenPage_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    DBG_pScreenPage = (word *)MK_FP(DBG_Src_Sgmt, 0);
    dlvfprintf("DEBUG: [%s, %d] Row_Offset: %d\n", __FILE__, __LINE__, DBG_Row_Offset);
    dlvfprintf("DEBUG: [%s, %d] Column_Offset: %d\n", __FILE__, __LINE__, DBG_Column_Offset);
    dlvfprintf("DEBUG: [%s, %d] ScreenPage_Offset: %u\n", __FILE__, __LINE__, DBG_ScreenPage_Offset);
    dlvfprintf("DEBUG: [%s, %d] Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, DBG_Src_Sgmt);
    dlvfprintf("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, DBG_pScreenPage);
#endif

    outportb(e_SC_INDEX, e_SC_MAPMASK);

    mask = (1 << (CR_X & 3));


#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] pDst: %Fp\n", __FILE__, __LINE__, pDst);
    dlvfprintf("DEBUG: [%s, %d] Dst_Ofst: %u\n", __FILE__, __LINE__, Dst_Ofst);
    dlvfprintf("DEBUG: [%s, %d] mask: 0x%02X (%u)\n", __FILE__, __LINE__, mask, mask);
#endif

    itr_count = 0;
    itr_plane = 0;

Column_Loop:
    Dst_Ofst = row_offset;
    outportb(e_SC_DATA, mask);

    itr_height = draw_height;
    while(itr_height != 0)
    {
        itr_count++;
        //itr_count = ((itr_plane + 1) * (itr_width + 1) * (itr_height + 1));
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] itr_count: %d  itr_plane: %d  itr_width: %d  itr_height: %d\n", __FILE__, __LINE__, itr_count, itr_plane, itr_width, itr_height);
#endif
        baito = pSrc[Src_Ofst++];
        if (baito != 0)  // Color Table Index Zero AKA TRANSPARENT
        {
            pDst[Dst_Ofst] = baito;
        }
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)]: 0x%02X (%u)\n", __FILE__, __LINE__, pDst, Dst_Ofst, Dst_Ofst, baito, baito);
#endif
        Dst_Ofst += LINE_STRIDE;  // 80 = (320/4) = (SCREEN_WIDTH_PIXELS / PIXELS_PER_ADDRESS)
        itr_height--;
    }

Next_Column:
    Src_Ofst = Src_Ofst + (16 - draw_height);
    itr_width--;
    if (itr_width != 0)
    {
        itr_plane++;
		mask = (mask << 1);
		if (mask == 16)
        {
            mask = 1;
            row_offset++;
        }
        goto Column_Loop;
    }

#ifdef DEBUG
    outportb(e_GC_INDEX, e_GC_READMAP);
    outportb(e_GC_DATA, 0x00);
    wordo = farpeekw(0xA064,0x0008);
    dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
    outportb(e_GC_DATA, 0x01);
    wordo = farpeekw(0xA064,0x0008);
    dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
    outportb(e_GC_DATA, 0x02);
    wordo = farpeekw(0xA064,0x0008);
    dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
    outportb(e_GC_DATA, 0x03);
    wordo = farpeekw(0xA064,0x0008);
    dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
#endif

Done:

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: CR_Draw_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
#endif

}
