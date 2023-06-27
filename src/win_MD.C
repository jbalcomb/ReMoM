
#include "MoX_TYPE.H"

#include "MoM.H"


int16_t platform_mouse_click_x;
int16_t platform_mouse_click_y;
int16_t platform_mouse_button_status;
// left, right, middle, x1, x2; wheel;

// WZD s35p05
int16_t MD_GetButtonStatus(void)
{

    // int16_t mouse_button_status;
    // mouse_button_status = 0b00000000;
    // if(mouse_driver_installed != ST_FALSE)
    // {
    //     mouse_button_status = mouse_right_button + mouse_left_button;
    // }

    return platform_mouse_button_status;
}
