
#include "MoX.H"



// YNM  int16_t platform_mouse_click_x;
// YNM  int16_t platform_mouse_click_y;
int16_t platform_mouse_button_status;
// left, right, middle, x1, x2; wheel;

int16_t lock_mouse_button_status_flag = ST_FALSE;

// WZD s35p05
int16_t Mouse_Button(void)
{

    // int16_t mouse_button_status;
    // mouse_button_status = 0b00000000;
    // if(mouse_driver_installed != ST_FALSE)
    // {
    //     mouse_button_status = mouse_right_button + mouse_left_button;
    // }

    Pump_Events();

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

// WZD s35p11
void User_Mouse_Handler(int16_t buttons, int16_t l_mx, int16_t l_my)
{
    if(l_mx < SCREEN_XMIN || l_my < SCREEN_YMIN || (l_mx / 2) > SCREEN_XMAX || (l_my / 2) > SCREEN_YMAX)
    {
        return;
    }

    g_mouse_x = l_mx / 2;
    g_mouse_y = l_my / 2;
    
    platform_mouse_button_status = buttons;

    if (mouse_interrupt_active == ST_FALSE)
    {
        mouse_interrupt_active = ST_TRUE;

        Check_Mouse_Buffer((l_mx / 2), (l_my / 2), buttons);

        if (mouse_enabled == ST_TRUE)
        {
            mouse_enabled = ST_FALSE;
            if (current_mouse_list_count >= 2)
            {
                Check_Mouse_Shape((l_mx / 2), (l_my/ 2));
            }
            Restore_Mouse_On_Page();                     // mouse_background_buffer           ->  video_page_buffer[draw_page_num]
            Save_Mouse_On_Page((l_mx / 2), (l_my / 2));  // video_page_buffer[draw_page_num]  ->  mouse_background_buffer
            Draw_Mouse_On_Page((l_mx / 2), (l_my / 2));  // mouse_palette                     ->  video_page_buffer[draw_page_num]
            mouse_enabled = ST_TRUE;
        }
        mouse_interrupt_active = ST_FALSE;
    }

}

// WZD s35p21
void Set_Mouse_Position(int16_t l_mx, int16_t l_my)
{
    MWA_Set_Mouse_Position(l_mx, l_my);
}
