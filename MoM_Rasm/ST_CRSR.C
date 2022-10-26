
#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_CRSR.H"    /* Crsr_None */

#include "ST_GUI.H"     /* g_CRSR_Curr, g_CRSR_Prev, g_CRSR_HaveSave, g_CRSR_Save_RSP, g_RSP_Idx */
#include "ST_VGA.H"     /* VRAM_BASE; g_RSP_Idx, gsa_Cursor_Array */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


// #define CURSOR_W    16
// #define CURSOR_H    16
// 
// static ui_cursor_area_t * ui_cursor_area_def_ptr = 0;
// static int ui_cursor_area_def_num = 1;
// static uint16_t ui_cursor_gfx_i_old = 0;
// 
// struct cursor_bg_s {
//     int x;
//     int y;
//     uint8_t data[CURSOR_W * CURSOR_H];
// };
// 
// static struct cursor_bg_s cursor_bg0;
// static struct cursor_bg_s cursor_bg1;

/*
_s33p30         CRL_Save_RSP                        GUI_SaveCursorArea_RSP          void GUI_SaveCursorArea_RSP(int X_Pos, int Y_Pos)       GUI_SaveCursorArea          
_s33p31         CRL_Save_DSP                        GUI_SaveCursorArea_DSP          void GUI_SaveCursorArea_DSP(int X_Pos, int Y_Pos)       GUI_SaveDrawCrsrArea          
_s33p32         IN_CRL_Save_RSP                     VGA_SaveCursorArea_RSP          void VGA_SaveCursorArea_RSP(int X_Pos, int Y_Pos)       VGA_SaveCursorArea
*/

/*
    saves the screen area under the cursor
    planar
*/
void CR_Store_C(int CR_X, int CR_Y, int ScreenPage_Idx)
{
    word Src_Sgmt;
    int Row_Offset;
    int Column_Offset;
    word ScreenPage_Offset;
    int Draw_Width;
    int Draw_Height;
    // byte * pSrc;
    word * pScreenPage;
    // byte * pDst;
    word * pCRSR_Save_RSP;
    word Src_Ofst;
    // word Dst_Ofst;
    int itr_plane;
    int itr_width;
    int itr_height;
    word wordo;
    int itr_count;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: CR_Store_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
// #endif

    if ( g_CRSR_Curr == Crsr_None )
    {
        // HERE("( g_CRSR_Curr == Crsr_None )");
        if (g_CRSR_HaveSave == ST_TRUE)
        {
            // HERE("( g_CRSR_HaveSave == ST_TRUE )");
            goto Done;
        }
        else
        {
            // HERE("( g_CRSR_HaveSave != ST_TRUE )");
            g_CRSR_HaveSave = ST_TRUE;
        }
    }

    /*
        Src_Sgmt
        Row_Offset
        Column_Offset
        ScreenPage_Offset
        Draw_Width
        Draw_Height
    */

    // pDst = (byte *)MK_FP((VRAM_BASE + ((ScreenPage_Idx - g_RSP_Idx) * 4) + (CR_Y * 5)), 0);
    //Src_Sgmt = VRAM_BASE + (((g_RSP_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    Src_Sgmt = VRAM_BASE + (((ScreenPage_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    
    pScreenPage = (word *) MK_FP(Src_Sgmt, 0);

    Row_Offset = (CR_X >> 2);  // PIXELS_PER_ADDRESS  e.g., 32 /4 = 8
    Column_Offset = CR_Y * 80;  // SCREEN_WIDTH_PER_PLANE  e.g., 20 * 80 - 1600
    ScreenPage_Offset = Column_Offset + Row_Offset;  // e.g., 1600 + 8 = 1608

    Draw_Width = ((Row_Offset + 3) - 80);
    if ( Draw_Width < 0 )
    {
        Draw_Width = 3;
    }

    Draw_Height = ((CR_Y + 16) - 200);
    if ( Draw_Height < 0 )
    {
        Draw_Height = 16;
    }

    //pCRSR_Save_RSP = (word *)(&g_CRSR_Save_RSP[0]);
    //pCRSR_Save_RSP = (word *)MK_FP(FP_SEG(pCRSR_Save_RSP),FP_OFF(pCRSR_Save_RSP));
    pCRSR_Save_RSP = (word *)(&g_CRSR_Save_RSP[0]);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
// #endif

    *pCRSR_Save_RSP++ = ScreenPage_Offset;
    *pCRSR_Save_RSP++ = Draw_Width;
    *pCRSR_Save_RSP++ = Draw_Height;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] g_CRSR_Curr: %d\n", __FILE__, __LINE__, g_CRSR_Curr);
//     dlvfprintf("DEBUG: [%s, %d] g_CRSR_HaveSave: %d\n", __FILE__, __LINE__, g_CRSR_HaveSave);
//     dlvfprintf("DEBUG: [%s, %d] Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, Src_Sgmt);
//     dlvfprintf("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, pScreenPage);
//     dlvfprintf("DEBUG: [%s, %d] Row_Offset: 0x%04X  (%d)\n", __FILE__, __LINE__, Row_Offset);
//     dlvfprintf("DEBUG: [%s, %d] Column_Offset: 0x%04X  (%d)\n", __FILE__, __LINE__, Column_Offset);
//     dlvfprintf("DEBUG: [%s, %d] ScreenPage_Offset: 0x%04X  (%u)\n", __FILE__, __LINE__, ScreenPage_Offset);
//     dlvfprintf("DEBUG: [%s, %d] Draw_Width: %d\n", __FILE__, __LINE__, Draw_Width);
//     dlvfprintf("DEBUG: [%s, %d] Draw_Height: %d\n", __FILE__, __LINE__, Draw_Height);
//     dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
// #endif


    /*
        Loops: Planes, Width, Height
    */

    outportb(e_GC_INDEX, e_GC_READMAP);
    itr_count = 0;
    for(itr_plane = 0; itr_plane < 4; itr_plane++)
    {
        outportb(e_GC_DATA, itr_plane);

        Src_Ofst = ScreenPage_Offset;

        itr_width = 0;
        while(itr_width < Draw_Width)
        {

            //itr_height = Draw_Height;
            //while(itr_height != 0)
            for(itr_height = 0; itr_height < Draw_Height; itr_height++)
            {
                itr_count++;
                //itr_count = ((itr_plane + 1) * (itr_width + 1) * (itr_height + 1));

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] itr_count: %d  itr_plane: %d  itr_width: %d  itr_height: %d\n", __FILE__, __LINE__, itr_count, itr_plane, itr_width, itr_height);
// #endif

                //*pCRSR_Save_RSP++ = FPEEKW(FP_SEG(pScreenPage),Src_Ofst);
                // wordo = FPEEKW(FP_SEG(pScreenPage),Src_Ofst);
                // #define FPEEKW( _sgmt_,_ofst_) ( *(( word _FAR *)MK_FP((_sgmt_),(_ofst_))) )
                wordo = ( (unsigned int) *( (((unsigned char *)pScreenPage) + Src_Ofst + 0) ) ) | ( (unsigned int) *( (((unsigned char *)pScreenPage) + Src_Ofst + 1) ) << 8 );

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)][%d]: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pScreenPage, (Src_Ofst - Column_Offset), (Src_Ofst - Column_Offset), itr_plane, wordo, wordo);
// #endif

                // dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)][%d]: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pScreenPage, Src_Ofst, Src_Ofst, itr_plane, wordo, wordo);
                *pCRSR_Save_RSP = wordo;
                pCRSR_Save_RSP++;
                //Src_Ofst += 78;  // line-delta  (2 + 78 = 80)
                Src_Ofst += 80;
            }

            itr_width++;
            Src_Ofst = ScreenPage_Offset + (itr_width * 2);  // + {0,2,4}

        }

    }

Done:

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: CR_Store_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
// #endif

}

//  CR_Restore_C()  ST_CRSR

/*
_s33p34         CRL_Restore_RSP                     GUI_RestoreCursorArea_RSP       VGA_RestoreCursrArea
_s33p35         CRL_Restore_DSP                     GUI_RestoreCursorArea_DSP       VGA_RestoreDrawCArea
*/

void CR_Restore_C(int ScreenPage_Idx)
{
    word Dst_Sgmt;
    word * pScreenPage;
    //word * pCRSR_Save_RSP;
    word * pCRSR_Save_RSP;
    word ScreenPage_Offset;
    int Draw_Width;
    int Draw_Height;
    byte mask;
    word itr_width;
    word itr_height;
    byte * pSrc;
    byte * pDst;
    word Dst_Ofst;
    word wordo;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: CR_Restore_C(ScreenPage_Idx=%d)\n", __FILE__, __LINE__, ScreenPage_Idx);
// #endif

    if ( g_CRSR_Prev == Crsr_None )
    {
        // HERE("( g_CRSR_Prev == Crsr_None )");
        goto Done;
    }

    /*
        Src_Sgmt
        Row_Offset
        Column_Offset
        ScreenPage_Offset
        Draw_Width
        Draw_Height
    */

    //pCRSR_Save_RSP = (word *)(&g_CRSR_Save_RSP[0]);
    //pCRSR_Save_RSP = (word *)MK_FP(FP_SEG(pCRSR_Save_RSP),FP_OFF(pCRSR_Save_RSP));
    pCRSR_Save_RSP = (word *)(&g_CRSR_Save_RSP[0]);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
// #endif

    ScreenPage_Offset = *pCRSR_Save_RSP++;
    Draw_Width = *pCRSR_Save_RSP++;
    Draw_Height = *pCRSR_Save_RSP++;

    //pDst = (byte *)MK_FP((VRAM_BASE + (g_RSP_Idx << 2), 0);
    //Src_Sgmt = VRAM_BASE + (((g_RSP_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    //Src_Sgmt = VRAM_BASE + (((g_RSP_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    Dst_Sgmt = VRAM_BASE + (((ScreenPage_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
    //pScreenPage = MK_FP(Src_Sgmt);
    pScreenPage = (word *)MK_FP(Dst_Sgmt, 0);
    Dst_Ofst = ScreenPage_Offset;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] g_CRSR_Prev: %d\n", __FILE__, __LINE__, g_CRSR_Prev);
//     dlvfprintf("DEBUG: [%s, %d] Dst_Sgmt: 0x%04X\n", __FILE__, __LINE__, Dst_Sgmt);
//     dlvfprintf("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, pScreenPage);
//     dlvfprintf("DEBUG: [%s, %d] ScreenPage_Offset: 0x%04X  (%u)\n", __FILE__, __LINE__, ScreenPage_Offset);
//     dlvfprintf("DEBUG: [%s, %d] Draw_Width: %d\n", __FILE__, __LINE__, Draw_Width);
//     dlvfprintf("DEBUG: [%s, %d] Draw_Height: %d\n", __FILE__, __LINE__, Draw_Height);
//     dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
// #endif


    /*
        Loops: Planes, Width, Height
    */

    mask = 1;
    outportb(e_SC_INDEX, e_SC_MAPMASK);
    while(mask != 16)
    {
        outportb(e_SC_DATA, mask);

        itr_width = 0;
        while(itr_width < Draw_Width)
        {

            for(itr_height = 0; itr_height < Draw_Height; itr_height++)
            {
                //pScreenPage[Dst_Ofst] = *pCRSR_Save_RSP++;
                //dlvfprintf("DEBUG: [%s, %d] pScreenPage[%u]: 0x%04X\n", __FILE__, __LINE__, Dst_Ofst, pScreenPage[Dst_Ofst]);
                wordo = *pCRSR_Save_RSP;

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] %Fp: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pCRSR_Save_RSP, wordo, wordo);
// #endif

                // //FPOKEW(FP_SEG(pScreenPage),Src_Ofst,wordo);
                // pScreenPage[Dst_Ofst] = wordo;
                // wordo = FPEEKW(FP_SEG(pScreenPage),Dst_Ofst);
                // dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)]: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pScreenPage, Dst_Ofst, Dst_Ofst, wordo, wordo);
                // 
                // FPOKEW(FP_SEG(pScreenPage),Dst_Ofst,wordo);
                // wordo = FPEEKW(FP_SEG(pScreenPage),Dst_Ofst);
                // dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)]: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pScreenPage, Dst_Ofst, Dst_Ofst, wordo, wordo);

                pScreenPage = (word *)MK_FP(Dst_Sgmt, Dst_Ofst);
                *pScreenPage = wordo;
                // wordo = FPEEKW(Dst_Sgmt,Dst_Ofst);
                // #define FPEEKW( _sgmt_,_ofst_) ( *(( word _FAR *)MK_FP((_sgmt_),(_ofst_))) )
                wordo = ((unsigned int)*((unsigned char *)Dst_Sgmt + Dst_Ofst + 0)) | (unsigned int)((unsigned int)*((unsigned char *)Dst_Sgmt + Dst_Ofst + 1) << 8);

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] Dst_Sgmt: 0x%04X  Dst_Ofst: 0x%04X  pScreenPage: %Fp  wordo: 0x%04X  (%u)\n", __FILE__, __LINE__, Dst_Sgmt, Dst_Ofst, pScreenPage, wordo, wordo);
// #endif

                pCRSR_Save_RSP++;
                //Dst_Ofst += 78;
                Dst_Ofst += 80;
            }

            itr_width++;
            Dst_Ofst = ScreenPage_Offset + (itr_width * 2);

        }

        mask = (mask << 1);
        Dst_Ofst = ScreenPage_Offset;
    }

Done:

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: CR_Restore_C(ScreenPage_Idx=%d)\n", __FILE__, __LINE__, ScreenPage_Idx);
// #endif
}

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

// #ifdef STU_DEBUG
//     int DBG_Row_Offset;
//     int DBG_Column_Offset;
//     word DBG_ScreenPage_Offset;
//     word DBG_Src_Sgmt;
//     word * DBG_pScreenPage;
//     word wordo;
// #endif

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: CR_Draw_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
// #endif

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

    // SMLM pSrc = (byte *)MK_FP(gsa_Cursor_Array + (16 * (g_CRSR_Curr - 1)), 0);
    pSrc = p_Cursor_Array + (SZ_PARAGRAPH_B * (16 * (g_CRSR_Curr - 1)));
    Src_Ofst = 0;

    pDst = (byte *)MK_FP((VRAM_BASE + ((ScreenPage_Idx - g_RSP_Idx) * 4) + (CR_Y * 5)), 0);
    Dst_Ofst = row_offset;

// #ifdef STU_DEBUG
// /*
//     DEBUG: as seen in Store & Restore
// */
//     DBG_Row_Offset = (CR_X >> 2);  // PIXELS_PER_ADDRESS
//     DBG_Column_Offset = CR_Y * 80;  // SCREEN_WIDTH_PER_PLANE
//     DBG_ScreenPage_Offset = DBG_Column_Offset + DBG_Row_Offset;
//     DBG_Src_Sgmt = VRAM_BASE + (((ScreenPage_Idx << 8) & 0xFF00) * 4);  // ScreenPage Segment Address
//     DBG_pScreenPage = (word *)MK_FP(DBG_Src_Sgmt, 0);
//     dlvfprintf("DEBUG: [%s, %d] Row_Offset: %d\n", __FILE__, __LINE__, DBG_Row_Offset);
//     dlvfprintf("DEBUG: [%s, %d] Column_Offset: %d\n", __FILE__, __LINE__, DBG_Column_Offset);
//     dlvfprintf("DEBUG: [%s, %d] ScreenPage_Offset: %u\n", __FILE__, __LINE__, DBG_ScreenPage_Offset);
//     dlvfprintf("DEBUG: [%s, %d] Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, DBG_Src_Sgmt);
//     dlvfprintf("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, DBG_pScreenPage);
// #endif

    outportb(e_SC_INDEX, e_SC_MAPMASK);

    mask = (1 << (CR_X & 3));


// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] pDst: %Fp\n", __FILE__, __LINE__, pDst);
//     dlvfprintf("DEBUG: [%s, %d] Dst_Ofst: %u\n", __FILE__, __LINE__, Dst_Ofst);
//     dlvfprintf("DEBUG: [%s, %d] mask: 0x%02X (%u)\n", __FILE__, __LINE__, mask, mask);
// #endif

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

// #ifdef STU_DEBUG
//         dlvfprintf("DEBUG: [%s, %d] itr_count: %d  itr_plane: %d  itr_width: %d  itr_height: %d\n", __FILE__, __LINE__, itr_count, itr_plane, itr_width, itr_height);
// #endif

        baito = pSrc[Src_Ofst++];
        if (baito != 0)  // Color Table Index Zero AKA TRANSPARENT
        {
            pDst[Dst_Ofst] = baito;
        }

// #ifdef STU_DEBUG
//         dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)]: 0x%02X (%u)\n", __FILE__, __LINE__, pDst, Dst_Ofst, Dst_Ofst, baito, baito);
// #endif

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

// #ifdef STU_DEBUG
//     outportb(e_GC_INDEX, e_GC_READMAP);
//     outportb(e_GC_DATA, 0x00);
//     wordo = farpeekw(0xA064,0x0008);
//     dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
//     outportb(e_GC_DATA, 0x01);
//     wordo = farpeekw(0xA064,0x0008);
//     dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
//     outportb(e_GC_DATA, 0x02);
//     wordo = farpeekw(0xA064,0x0008);
//     dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
//     outportb(e_GC_DATA, 0x03);
//     wordo = farpeekw(0xA064,0x0008);
//     dlvfprintf("DEBUG: [%s, %d] farpeekw(0xA064,0x0008) wordo: 0x%04X (%u)\n", __FILE__, __LINE__, wordo, wordo);
// #endif

Done:

//#ifdef STU_DEBUG
//    dlvfprintf("DEBUG: [%s, %d] END: CR_Draw_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
//#endif
}
