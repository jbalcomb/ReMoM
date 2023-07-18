#ifndef MOM_PFL_H
#define MOM_PFL_H

/*
    ~ MoM Platform-Layer API
    include in both sides
    decl here, defn/impl in platform code
    e.g.,
        Input wants mouse_x, mouse_y
        so, this lets it know they exist
    ...maybe should be Platform_Input.hpp? ~IN{,KD,MD,JD,GD,HD,etc.}
    ? definitely don't declare them here and in the MoM, just to avoid including platform code in the game code ?
    ? doesn't make sense to move the defn to the platform code ?
    ? defn should be with impl ?
    But, ...
        the enums for KD are in Input, because they are used elsewhere
        ? move them to their own include ? //SDL
        ? include everything everywhere ? //GLFW
    
*/

#include "MoX_TYPE.H"

#include <windows.h>



/*
    Input
        Keyboard
            Press
        Mouse
            Move
            Click
*/



#ifdef __cplusplus
extern "C" {
#endif



// set in Win32_Evnt.cpp, get in win_MD.C
// YNM  extern int16_t platform_mouse_click_x;
// YNM  extern int16_t platform_mouse_click_y;
extern int16_t platform_mouse_button_status;
extern int16_t lock_mouse_button_status_flag;

extern uint8_t g_Key_Pressed;
extern uint16_t g_Last_Key_Pressed;
extern uint16_t scan_code_char_code;  // Platform

extern HWND g_Window;

extern uint8_t g_State_Run;



void Check_Game_Files(void);



// TODO(JimBalcomb,20221216): Services that the *platform layer* provides to the *game*
void Pump_Events(void);
void Pump_Paints(void);
uint32_t Get_System_Clock_Counter(void);



// TODO(JimBalcomb,20221216): Services that the *game* provides to the *platform layer*

void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t mouse_click_x, int16_t mouse_click_y, int16_t mouse_button_status);


#ifdef __cplusplus
}
#endif


#endif /* MOM_PFL_H */
