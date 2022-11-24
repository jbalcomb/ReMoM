// _s33p07c.c MD_Init
// ST_GUI.H

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_GUI.H"


int MD_Init(void)
{
    unsigned short c_sgmt;
    unsigned short c_ofst;

    asm {
        mov ax, 0
        mov bx, 0
        mov cx, 0
        mov dx, 0
        int 0x33
    }

    if ( _AX == 0 )
        return ST_FAILURE;

    asm {
        mov ax, 0x03
        int 0x33
    }

    g_MouseDriverInstalled = 1;             // ST_TRUE
    g_MD_INT_InProcess = 0;                 // ST_FALSE

    g_MD_CurrentY = g_MD_InitY;
    g_MD_CurrentX = g_MD_InitX;

    asm {
        mov ax, 0x07
        mov bx, 0
        mov cx, 0
        mov dx, 638
        int 0x33
    }

    asm {
        mov ax, 0x08
        mov bx, 0
        mov cx, 0
        mov dx, 199
        int 0x33
    }

    MD_WaitRelease();                       // _s33p06

    c_sgmt = FP_SEG(MD_INT_Handler);        // _s33p12
    c_ofst = FP_OFF(MD_INT_Handler);

    asm {
        mov ax, c_sgmt
        mov es, ax
        mov dx, c_ofst
        mov ax, 0x0C
        mov cx, 00000001b                   // User Interrupt Mask: 00000001 Cursor Position Change
        int 0x33
    }

    g_MD_CursorDraw = 0;
    g_MD_ClickRec1 = 0;
    g_MD_ClickRec2 = 0;

    return ST_SUCCESS;
}
