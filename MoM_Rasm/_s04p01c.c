// _s04p01c.c DISK_FileFind

#include <DOS.H>

#include "ST_HEAD.H"

#include "STU_DBG.H"


#define DOS_GET_DTA    0x2F
#define DOS_FIND_FIRST 0x4E
#define DOS_FIND_NEXT 0x4F


int DISK_FileFind(const char *pathname, char *Read_Buffer)
{
    union  REGS  inregs1;
    union  REGS  outregs1;
    struct SREGS segregs1;
    // int          result1;
    union  REGS  inregs2;
    union  REGS  outregs2;
    struct SREGS segregs2;
    // int          result2;
    union  REGS  inregs3;
    union  REGS  outregs3;
    struct SREGS segregs3;
    // int          result3;

    unsigned int bkup_DS;
    unsigned int dta_sgmt;
    unsigned int dta_ofst;
    char _FAR * fptr_Src;
    char _FAR * fptr_Dst;

// #ifdef DEBUG
//     char _FAR *dta;
// #endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: DISK_FileFind(pathname=%s)\n", __FILE__, __LINE__, pathname);
#endif

    bkup_DS = _DS;

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] bkup_DS: %d\n", __FILE__, __LINE__, bkup_DS);
// #endif

    MOUSE_Disable_CDraw();

// #ifdef DEBUG
//     //dta = _dos_getdta();
//     dta = getdta();
//     dlvfprintf("The current disk transfer address is: %Fp\n", dta);
// #endif

    inregs1.h.ah = DOS_GET_DTA;
    //result1 = intdosx( &inregs1, &outregs1, &segregs1 );
    intdosx( &inregs1, &outregs1, &segregs1 );

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.ax: 0x%04X\n", __FILE__, __LINE__, outregs1.x.ax);
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.bx: 0x%04X\n", __FILE__, __LINE__, outregs1.x.bx);
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.cx: 0x%04X\n", __FILE__, __LINE__, outregs1.x.cx);
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.dx: 0x%04X\n", __FILE__, __LINE__, outregs1.x.dx);
//     dlvfprintf("DEBUG: [%s, %d] segregs1.ds: 0x%04X\n", __FILE__, __LINE__, segregs1.ds);
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.si: 0x%04X\n", __FILE__, __LINE__, outregs1.x.si);
//     dlvfprintf("DEBUG: [%s, %d] segregs1.es: 0x%04X\n", __FILE__, __LINE__, segregs1.es);
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.di: 0x%04X\n", __FILE__, __LINE__, outregs1.x.di);
//     dlvfprintf("DEBUG: [%s, %d] result1: %d\n", __FILE__, __LINE__, result1);
// #endif

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] outregs1.x.bx: 0x%04X\n", __FILE__, __LINE__, outregs1.x.bx);
//     dlvfprintf("DEBUG: [%s, %d] segregs1.es: 0x%04X\n", __FILE__, __LINE__, segregs1.es);
// #endif
    dta_ofst = outregs1.x.bx + 0x1E;
    dta_sgmt = segregs1.es;
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] dta_ofst: 0x%04X\n", __FILE__, __LINE__, dta_ofst);
//     dlvfprintf("DEBUG: [%s, %d] dta_sgmt: 0x%04X\n", __FILE__, __LINE__, dta_sgmt);
// #endif

    if ( pathname[0] == 0 )
    {
        inregs2.h.ah = DOS_FIND_NEXT;
        // result2 = intdosx( &inregs2, &outregs2, &segregs2 );
        intdosx( &inregs2, &outregs2, &segregs2 );

        if  (outregs2.x.cflag)
        {
            goto Failure;
        }
        else
        {
            goto Success;
        }
    }
    else
    {
        inregs3.h.ah = DOS_FIND_FIRST;
        inregs3.x.cx = 0;
        inregs3.x.dx = FP_OFF(pathname);
        segregs3.ds = FP_SEG(pathname);
        // result3 = intdosx( &inregs3, &outregs3, &segregs3 );
        intdosx( &inregs3, &outregs3, &segregs3 );

        if  (outregs3.x.cflag)
        {
            goto Failure;
        }
        else
        {
            goto Success;
        }
    }

    // if ( outregs2.x.cflag || outregs3.x.cflag )
    // {
    //     Read_Buffer[0] = '\0';
    //     Ret_val = e_ST_FAILURE;
    //     MOUSE_CDraw_Restore();
    // }

Failure:
//    HERE("Failure");
    Read_Buffer[0] = '\0';
    MOUSE_CDraw_Restore();
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: DISK_FileFind()\n", __FILE__, __LINE__);
#endif
    return ST_FAILURE;

Success:
//    HERE("Success");
// //     _ES = bkup_DS;
// //     _DI = (unsigned int)&Read_Buffer;
// //     _DS = dta_sgmt;
// //     _SI = dta_ofst;
// // #ifdef DEBUG
// //     dlvfprintf("DEBUG: [%s, %d] _ES: 0x%04X\n", __FILE__, __LINE__, _ES);
// //     dlvfprintf("DEBUG: [%s, %d] _DI: 0x%04X\n", __FILE__, __LINE__, _DI);
// //     dlvfprintf("DEBUG: [%s, %d] _DS: 0x%04X\n", __FILE__, __LINE__, _DS);
// //     dlvfprintf("DEBUG: [%s, %d] _SI: 0x%04X\n", __FILE__, __LINE__, _SI);
// // #endif
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] _ES = bkup_DS: 0x%04X\n", __FILE__, __LINE__, bkup_DS);
//     dlvfprintf("DEBUG: [%s, %d] _DI = (unsigned int)&Read_Buffer: 0x%04X\n", __FILE__, __LINE__, (unsigned int)&Read_Buffer);
//     dlvfprintf("DEBUG: [%s, %d] _DS = dta_sgmt: 0x%04X\n", __FILE__, __LINE__, dta_sgmt);
//     dlvfprintf("DEBUG: [%s, %d] _SI = dta_ofst: 0x%04X\n", __FILE__, __LINE__, dta_ofst);
// #endif

    fptr_Src = (char _FAR *) MK_FP(dta_sgmt, dta_ofst);
    //fptr_Dst = (char _FAR *) MK_FP(bkup_DS, (unsigned int)&Read_Buffer);
    fptr_Dst = (char _FAR *) MK_FP(FP_SEG(Read_Buffer), FP_OFF(Read_Buffer));

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);
//     dlvfprintf("DEBUG: [%s, %d] fptr_Dst: %Fp\n", __FILE__, __LINE__, fptr_Dst);
// #endif

    while ( (*fptr_Dst = *fptr_Src) != '\0' ) {
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] *fptr_Src: %c\n", __FILE__, __LINE__, *fptr_Src);
// #endif
        ++fptr_Src;
        ++fptr_Dst;
    }

    _DS = bkup_DS;

    MOUSE_CDraw_Restore();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: DISK_FileFind(pathname=%s, Read_Buffer=%s)\n", __FILE__, __LINE__, pathname, Read_Buffer);
#endif
    return ST_SUCCESS;
}
