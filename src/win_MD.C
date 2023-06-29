
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM.H"


// YNM  int16_t platform_mouse_click_x;
// YNM  int16_t platform_mouse_click_y;
int16_t platform_mouse_button_status;
// left, right, middle, x1, x2; wheel;

int16_t lock_mouse_button_status_flag = ST_FALSE;

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


// WZD s35p10
void Mouse_Movement_Handler(void)
{
    lock_mouse_button_status_flag = ST_TRUE;
}

// WZD s35p11
void Mouse_Button_Handler(void)
{
    // YNM  platform_mouse_click_x = 0;
    // YNM  platform_mouse_click_y = 0;
    platform_mouse_button_status = 0;
    lock_mouse_button_status_flag = ST_FALSE;
}
