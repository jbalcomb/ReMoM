// TITLE _s27c.c FLIC_Draw_...
// ST_FLIC.H

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_VGA.H"

#include "STU_DBG.H"

unsigned char VGA_WriteMapMasks3[4] = { 0x01, 0x02, 0x04, 0x08 };


#define e_GC_INDEX   0x03CE
#define e_GC_DATA    0x03CF
#define e_GC_READMAP   0x04
#define e_SC_INDEX   0x03C4
#define e_SC_DATA    0x03C5
#define e_SC_MAPMASK   0x02

// Code Segment Variables from Dasm
static unsigned int tmpFlicHdrEmmLogicalPageOffset;
static unsigned int tmpFlicHdrEmmHandleNumber;
static unsigned int tmpFlicHdrWidth;
static unsigned int tmp_SI_EmmPhysOfst;

// ; draws an LBX image into the current draw segment,
// ; column by column, decoding its RLE along the way

// _s27p01
void FLIC_Draw_C(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, unsigned int FlicOfst, unsigned int FlicSgmt)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned char _FAR * fprSrc;
    unsigned char _FAR * fprDst;
    unsigned char _FAR * fptrFlic;
	byte writemask;
    int tmp_FlicWidth;
    byte b1;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_Draw_C(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%04X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
#endif

    // inregs.x.dx = e_SC_INDEX;
    // inregs.h.al, e_SC_MAPMASK;
    // outportb(inregs.x.dx, inregs.h.al);
    outportb(e_SC_INDEX, e_SC_MAPMASK);

    //fptrFlic = (unsigned char *) MK_FP(FlicSgmt, FlicOfst);

    DstSgmt = g_DrawScreenPage_SgmtAddr + (ScreenPage_Y * 4) + ScreenPage_Y;
    DstOfst = ScreenPage_X / 4;
    SrcOfst = FlicOfst;
    SrcSgmt = FlicSgmt;
    fptrDst = (unsigned char *) MK_FP(DstSgmt, DstOfst);
    fptrSrc = (unsigned char *) MK_FP(SrcSgmt, SrcOfst);

    //#define VGAMAPMASK(x) asm{cli;mov dx,SC_INDEX;mov al,SC_MAPMASK;mov ah,x;out dx,ax;sti;}
    //byte	pixmasks[4] = {1,2,4,8};
    //byte mask;
	//mask = pixmasks[x&3];
	//VGAMAPMASK(mask);
	//*(byte far *)MK_FP(SCREENSEG,bufferofs+(ylookup[y]+(x>>2))) = color;

    writemask = VGA_WriteMapMasks3[(ScreenPage_X & 0x03)];  // ~== X mod 4


    tmp_FlicWidth = FlicWidth;


    // ? BX = CX ? = FlicWidth

    outportb(e_SC_DATA, writemask);

    b1 = *data++;


#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_Draw_C(ScreenPage_X=%d, ScreenPage_Y=%d, FlicWidth=%d, Img_Off=0x%04X, Img_Seg=0x%04X)\n", __FILE__, __LINE__, ScreenPage_X, ScreenPage_Y, FlicWidth, Img_Off, Img_Seg);
#endif

}
