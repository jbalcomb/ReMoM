// _s12p11c.c EMM_Map4

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_EMM.H"


void EMM_Map4(int EmmHandle, int EmmLogicalPage)
{
    union  REGS  inregs;
    union  REGS  outregs;

//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Map4(EmmHandle=%d, EmmLogicalPage=%d)\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage);
//#endif

    inregs.x.dx = EmmHandle;
    inregs.x.bx = EmmLogicalPage;
    inregs.h.ah = 0x44;
    inregs.h.al = 0x00;
    int86(EMS_INT, &inregs, &outregs);

    inregs.x.dx = EmmHandle;
    inregs.x.bx = EmmLogicalPage + 1;
    inregs.h.ah = 0x44;
    inregs.h.al = 0x01;
    int86(EMS_INT, &inregs, &outregs);

    inregs.x.dx = EmmHandle;
    inregs.x.bx = EmmLogicalPage + 2;
    inregs.h.ah = 0x44;
    inregs.h.al = 0x02;
    int86(EMS_INT, &inregs, &outregs);

    inregs.x.dx = EmmHandle;
    inregs.x.bx = EmmLogicalPage + 3;
    inregs.h.ah = 0x44;
    inregs.h.al = 0x03;
    int86(EMS_INT, &inregs, &outregs);
    
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] END: EMM_Map4(EmmHandle=%d, EmmLogicalPage=%d)\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage);
//#endif

}
