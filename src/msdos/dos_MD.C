
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



// WZD s35p12
// drake178: MOUSE_INT_Handler()
// MoO2  Module: mouse  User_Mouse_Handler()
void User_Mouse_Handler(int16_t max, int16_t buttons, int16_t mcx, int16_t mdx)
{
// Current_Graphics_Register= word ptr -0Ah
// Current_Bit_Mask= word ptr -8
// Current_Read_Map= word ptr -6
// Current_Map_Mask= word ptr -4

// cli
// pushf
// push    ds
// push    ax
// cld
// mov     ax, seg dseg
// mov     ds, ax

    mouse_x = (mcx / 2);
    mouse_y = mdx;

    if(mouse_interrupt_active == ST_FALSE)
    {
        mouse_interrupt_active = ST_TRUE;

        Check_Mouse_Buffer((mcx = (mcx / 2)), mdx, buttons);

        if(mouse_enabled == ST_FALSE)
        {

            mouse_enabled = ST_FALSE;

            // mov     [bp+Current_Map_Mask], ax
            // mov     [bp+Current_Graphics_Register], ax
            // mov     [bp+Current_Read_Map], ax
            // mov     [bp+Current_Bit_Mask], ax

            if(current_mouse_list_count > 2)
            {
                Check_Mouse_Shape(mcx, mdx);
            }

            Restore_Mouse_On_Page();                // mouse_background_buffer -> video_page_buffer[draw_page_num]
            Save_Mouse_On_Page(mouse_x, mouse_y);   // video_page_buffer[draw_page_num] -> mouse_background_buffer
            Draw_Mouse_On_Page(mouse_x, mouse_y);   // mouse_palette -> video_page_buffer[draw_page_num]

            // mov     ax, [bp+Current_Map_Mask]
            // mov     ax, [bp+Current_Read_Map]
            // mov     ax, [bp+Current_Bit_Mask]
            // mov     ax, [bp+Current_Graphics_Register]

            mouse_enabled = ST_TRUE;

        }

        mouse_interrupt_active = ST_FALSE;

    }

}
