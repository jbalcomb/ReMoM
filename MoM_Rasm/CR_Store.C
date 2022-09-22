
#include "ST_CRSR.H";
#include "ST_GUI.H";  // g_CRSR_Curr, g_CRSR_HaveSave, gsa_Cursor_Array, g_RSP_Idx
#include "ST_VGA.H";  // g_RSP_Idx

#include "STU_BITS.H";
#include "STU_DBG.H";

//#define GET_LE_16(_p_) (((word)(((byte const *)(_p_))[0])) | (((word)(((byte const *)(_p_))[1])) << 8))

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: CR_Store_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
#endif

    if ( g_CRSR_Curr == Crsr_None )
    {
        HERE("( g_CRSR_Curr == Crsr_None )");
        if (g_CRSR_HaveSave == ST_TRUE)
        {
            HERE("( g_CRSR_HaveSave == ST_TRUE )");
            goto Done;
        }
        else
        {
            HERE("( g_CRSR_HaveSave != ST_TRUE )");
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
    pScreenPage = (word *)MK_FP(Src_Sgmt, 0);

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
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
#endif

    *pCRSR_Save_RSP++ = ScreenPage_Offset;
    *pCRSR_Save_RSP++ = Draw_Width;
    *pCRSR_Save_RSP++ = Draw_Height;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Curr: %d\n", __FILE__, __LINE__, g_CRSR_Curr);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_HaveSave: %d\n", __FILE__, __LINE__, g_CRSR_HaveSave);
    dlvfprintf("DEBUG: [%s, %d] Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, Src_Sgmt);
    dlvfprintf("DEBUG: [%s, %d] pScreenPage: %Fp\n", __FILE__, __LINE__, pScreenPage);
    dlvfprintf("DEBUG: [%s, %d] Row_Offset: 0x%04X  (%d)\n", __FILE__, __LINE__, Row_Offset);
    dlvfprintf("DEBUG: [%s, %d] Column_Offset: 0x%04X  (%d)\n", __FILE__, __LINE__, Column_Offset);
    dlvfprintf("DEBUG: [%s, %d] ScreenPage_Offset: 0x%04X  (%u)\n", __FILE__, __LINE__, ScreenPage_Offset);
    dlvfprintf("DEBUG: [%s, %d] Draw_Width: %d\n", __FILE__, __LINE__, Draw_Width);
    dlvfprintf("DEBUG: [%s, %d] Draw_Height: %d\n", __FILE__, __LINE__, Draw_Height);
    dlvfprintf("DEBUG: [%s, %d] pCRSR_Save_RSP: %Fp\n", __FILE__, __LINE__, pCRSR_Save_RSP);
#endif


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
                dlvfprintf("DEBUG: [%s, %d] itr_count: %d  itr_plane: %d  itr_width: %d  itr_height: %d\n", __FILE__, __LINE__, itr_count, itr_plane, itr_width, itr_height);
                //*pCRSR_Save_RSP++ = FPEEKW(FP_SEG(pScreenPage),Src_Ofst);
                wordo = FPEEKW(FP_SEG(pScreenPage),Src_Ofst);
                dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)][%d]: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pScreenPage, (Src_Ofst - Column_Offset), (Src_Ofst - Column_Offset), itr_plane, wordo, wordo);
                //dlvfprintf("DEBUG: [%s, %d] %Fp[0x%04X (%u)][%d]: wordo: 0x%04X (%u)\n", __FILE__, __LINE__, pScreenPage, Src_Ofst, Src_Ofst, itr_plane, wordo, wordo);
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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: CR_Store_C(CR_X=%d, CR_Y=%d, ScreenPage_Idx=%d)\n", __FILE__, __LINE__, CR_X, CR_Y, ScreenPage_Idx);
#endif

}
