// _s24p04c.c

#include "ST_DEF.H"

#include "ST_VGA.H"


void VGA_ReadScreenLine(unsigned int ScreenLine, unsigned char * Buffer)
{
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    int ScreenWidth_bytes;
    int itr_ScreenWidth_bytes;

    DstOfst = FP_OFF(Buffer);
    SrcOfst = ScreenLine * 80;
    DstSgmt = _DS;
    SrcSgmt = gsa_DSP_Addr;

    ScreenWidth_bytes = 80;
    while(ScreenWidth_bytes-- > 0)
    {
        outportb(0x3CE, 4);
        outportb(0x3CF, 0);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
        outportb(0x3CE, 4);
        outportb(0x3CF, 1);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
        outportb(0x3CE, 4);
        outportb(0x3CF, 2);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
        outportb(0x3CE, 4);
        outportb(0x3CF, 2);
        *((unsigned char *)MK_FP(DstSgmt, DstOfst)) = *((unsigned char *)MK_FP(SrcSgmt, SrcOfst));  // ~== MOVSB
        SrcSgmt--;
    }

}