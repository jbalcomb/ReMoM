// _s33p10a.c MD_INT_SetMvOnly
// ST_GUI.H

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_GUI.H"


void MD_INT_SetMvOnly(void)
{
    unsigned short c_sgmt;
    unsigned short c_ofst;

    if ( g_MouseDriverInstalled != 0 )
    {

        asm {
            cli
        }

        c_sgmt = FP_SEG(MD_INT_Handler);     // _s33p12
        c_ofst = FP_OFF(MD_INT_Handler);

        asm {
            mov ax, c_sgmt
            mov es, ax
            mov dx, c_ofst
            mov ax, 0x0C
            mov cx, 00000001b               // User Interrupt Mask: 00000001 Cursor Position Change
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
