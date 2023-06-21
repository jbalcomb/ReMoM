
#include "MoX_TYPE.H"

#include "MoM.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

int16_t platform_mouse_click_x;
int16_t platform_mouse_click_y;
int16_t platform_mouse_button_status;
// left, right, middle, x1, x2; wheel;

// WZD s35p05
int16_t MD_GetButtonStatus(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MD_GetButtonStatus()\n", __FILE__, __LINE__);
#endif

    // int16_t mouse_button_status;
    // mouse_button_status = 0b00000000;
    // if(mouse_driver_installed != ST_FALSE)
    // {
    //     mouse_button_status = mouse_right_button + mouse_left_button;
    // }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MD_GetButtonStatus() { platform_mouse_button_status = %d }\n", __FILE__, __LINE__, platform_mouse_button_status);
#endif

    return platform_mouse_button_status;
}
