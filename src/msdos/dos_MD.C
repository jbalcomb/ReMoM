
#include "MoX_TYPE.H"

#include "Mouse.H"

// WZD s35p05
int16_t MD_GetButtonStatus(void)
{
    int16_t mouse_button_status;

//     if(MOUSE_DriverPresent != ST_FALSE)
//     {
//         // ; MS MOUSE - RETURN POSITION AND BUTTON STATUS
//         // ; Return: BX = button status, CX = column, DX = row
//         // asm mov ax, 3
//         // asm mov bx, 0
//         // asm mov cx, 0
//         // asm mov dx, 0
//         // asm int 33h
//         // asm and bx, 11b
//         // asm mov ax, bx
//     }
//     mouse_button_status = g_Mouse_Click_X;

    return mouse_button_status;
}
