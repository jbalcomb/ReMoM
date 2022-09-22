//  CR_Restore_C()  ST_CRSR

#include "ST_CRSR.H";  // Crsr_None
#include "ST_GUI.H";  // g_CRSR_Curr, g_CRSR_Prev, g_CRSR_HaveSave, g_CRSR_Save_RSP, gsa_Cursor_Array, g_RSP_Idx
#include "ST_VGA.H";  // VRAM_BASE; g_RSP_Idx

#include "STU_BITS.H";
#include "STU_DBG.H";

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: CR_Restore_C(ScreenPage_Idx=%d)\n", __FILE__, __LINE__, ScreenPage_Idx);
#endif

    if ( g_CRSR_Prev == Crsr_None )
    {
        HERE("( g_CRSR_Prev == Crsr_None )");
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
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
#endif

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Prev: %d\n", __FILE__, __LINE__, g_CRSR_Prev);
    dlvfprintf("DEBUG: [%s, %d] Dst_Sgmt: 0x%04X\n", __FILE__, __LINE__, Dst_Sgmt);
    dlvfprintf("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, pScreenPage);
    dlvfprintf("DEBUG: [%s, %d] ScreenPage_Offset: 0x%04X  (%u)\n", __FILE__, __LINE__, ScreenPage_Offset);
    dlvfprintf("DEBUG: [%s, %d] Draw_Width: %d\n", __FILE__, __LINE__, Draw_Width);
    dlvfprintf("DEBUG: [%s, %d] Draw_Height: %d\n", __FILE__, __LINE__, Draw_Height);
    dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
#endif


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
                dlvfprintf("DEBUG: [%s, %d] %Fp: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pCRSR_Save_RSP, wordo, wordo);

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
                wordo = FPEEKW(Dst_Sgmt,Dst_Ofst);
                dlvfprintf("DEBUG: [%s, %d] Dst_Sgmt: 0x%04X  Dst_Ofst: 0x%04X  pScreenPage: %Fp  wordo: 0x%04X  (%u)\n", __FILE__, __LINE__, Dst_Sgmt, Dst_Ofst, pScreenPage, wordo, wordo);

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: CR_Restore_C(ScreenPage_Idx=%d)\n", __FILE__, __LINE__, ScreenPage_Idx);
#endif

}
