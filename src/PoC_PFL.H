#ifndef POC_PFL_H
#define POC_PFL_H

#include "MoX.H"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
    *screen* - MoX & MoM
*/
/*
    *window* - Platform-Layer & Platform
*/

/*
    Platform-Layer API Specification

        declared here to set expectations, defined/implemented in the platform-specific code to meet these expactations
*/

extern uint8_t g_State_Run;


// TODO  move this and Load_Font_File(), Load_Palette(), and Apply_Pallete() to the platform-layer
// extern uint8_t g_Video_Back_Buffer[];
extern uint8_t g_Palette[];
extern uint8_t g_Palette_XBGR[];



extern int16_t platform_mouse_button_status;
extern int16_t lock_mouse_button_status_flag;

void Pump_Events(void);
void Pump_Paints(void);
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status);

void Init_Platform(HINSTANCE hInstance, int nCmdShow);



#ifdef STU_MSDOS
void Init_Platform(void);
#endif
#ifdef STU_MSWIN
void Init_Platform(HINSTANCE hInstance, int nCmdShow);
void Pump_Events(void);
void Pump_Paints(void);
uint32_t Get_System_Clock_Counter(void);
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t mouse_click_x, int16_t mouse_click_y, int16_t mouse_button_status);
#endif
#ifdef STU_SDL2

#endif



#ifdef __cplusplus
}
#endif


#endif /* POC_PFL_H */
