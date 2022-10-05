
#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "ST_FLIC.H"
#include "ST_SA.H"

/*
    MGC  seg027
*/
/*
void FLIC_Draw(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
void FLIC_Draw_A(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
void FLIC_Draw_C(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
// _s27p02
void FLIC_Draw_R(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
// void FLIC_Draw_R_A(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int Img_Off, unsigned int Img_Seg);
// _s27p03
//void FLIC_Draw_EMM(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
//void FLIC_Draw_EMM_A2(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
void FLIC_Draw_EMM_A3(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg, int Frame_Index);
// _s27p04
void FLIC_EMM_MapNextPages(void);
void FLIC_EMM_MapNextPages_A(void);
// _s27p05
void FLIC_Draw_EMM_R(int ScreenPage_X, int ScreenPage_Y);
// void FLIC_Draw_EMM_R_A(int ScreenPage_X, int ScreenPage_Y, unsigned int Img_Seg);
*/
/*
    MGC  seg028
*/
/*
// _s28p02
void FLIC_Prepare(int Width, int Height, unsigned int SAMB_head);
// _s28p11
void FLIC_Draw_XY(int Left, int Top, unsigned int FlicHdr_SgmtAddr);
// _s28p13
//void FLIC_BuildFrame(unsigned int FlicHdr_SgmtAddr, unsigned int Target_Seg);
// _s28p14
void FLIC_SetFrame(unsigned int FlicHdr_SgmtAddr, int Frame_Index);
// _s28p15
void FLIC_ResetFrame(unsigned int FlicHdr_SgmtAddr);
// _s28p16
unsigned int FLIC_GetCurFrame(unsigned int FlicHdr_SgmtAddr);
*/


// 1oom::lbxgfx.c::static void lbxgfx_draw_pixels_fmt0(uint8_t *pixbuf, uint16_t w, uint8_t *data, uint16_t pitch)
void FLIC_RLE_Decode(BYTE * FLIC_Frame_Data)
{
    
}
