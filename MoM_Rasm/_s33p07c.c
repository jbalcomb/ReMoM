// _s33p07c.c MOUSE_Init
// ST_GUI.H

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_GUI.H"

int MOUSE_Init(void)
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
    g_MOUSE_INT_Process = 0;                // ST_FALSE

    g_MOUSE_CurrentY = g_MOUSE_InitY;
    g_MOUSE_CurrentX = g_MOUSE_InitX;

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

    MOUSE_WaitRelease();                    // _s33p06

    c_sgmt = FP_SEG(MOUSE_INT_Handler);     // _s33p12
    c_ofst = FP_OFF(MOUSE_INT_Handler);

    asm {
        mov ax, c_sgmt
        mov es, ax
        mov dx, c_ofst
        mov ax, 0x0C
        mov cx, 1                           // User Interrupt Mask: 00000001 Cursor Position Change
        int 0x33
    }

    g_MOUSE_CursorDraw = 0;
    g_MOUSE_ClickRec1 = 0;
    g_MOUSE_ClickRec2 = 0;

    return ST_SUCCESS;
}
