// _s33p11a.c MD_INT_SetMvBtns
// ST_GUI.H
//mov cx, 00101011b

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_GUI.H"


void MD_INT_SetMvBtns(void)
{
    unsigned short c_sgmt;
    unsigned short c_ofst;

    if ( g_MouseDriverInstalled != 0 )
    {

        asm {
            cli
        }

        c_sgmt = FP_SEG(MD_INT_Handler);    // _s33p12
        c_ofst = FP_OFF(MD_INT_Handler);

        asm {
            mov ax, c_sgmt
            mov es, ax
            mov dx, c_ofst
            mov ax, 0x0C
            mov cx, 00101011b               // User Interrupt Mask: 00101011 Middle, Right, Left Button Press, Cursor Position Change
            int 0x33
        }

        g_MD_CursorDraw = 1;

        asm {
            mov ax, 3
            int 33h
        }

        asm {
            sti
        }

    }

}
