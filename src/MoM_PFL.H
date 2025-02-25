#ifndef MOX_PFL_H
#define MOX_PFL_H

#include "MoX/MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif

/*
    set expectations to be met for the platform-layer by the platform-specific code
        decl here
        defn/impl there
*/

extern int quit_game_flag;

extern int key_pressed;  // win_KD.C  Keyboard_Status(), Read_Key()
extern uint16_t scan_code_char_code;  // win_KD.C  Read_Key()



/*
    ~ MoM Platform-Layer API
    include in both sides
    decl here, defn/impl in platform code
    e.g.,
        Input wants mouse_x, mouse_y
        so, this lets it know they exist
    ...maybe should be Platform_Input.hpp? ~IN{,KD,MD,JD,GD,HD,etc.}
    ¿ definitely don't declare them here and in the MoM, just to avoid including platform code in the game code ?
    ¿ doesn't make sense to move the defn to the platform code ?
    ¿ defn should be with impl ?
    But, ...
        the enums for KD are in Input, because they are used elsewhere
        ¿ move them to their own include ? //SDL
        ¿ include everything everywhere ? //GLFW
    
*/

// extern uint8_t g_Key_Pressed;
// extern uint16_t g_Last_Key_Pressed;
// extern uint16_t scan_code_char_code;  // Platform

// TODO(JimBalcomb,20221216): Services that the *platform layer* provides to the *game*

// TODO(JimBalcomb,20221216): Services that the *game* provides to the *platform layer*

// Meh? void Check_Game_Files(void);





// win_PFL.cpp
// used by Get_Input() in win_Input.C
void Pump_Events(void);
// used by Page_Flip() in win_Video.C
void Pump_Paints(void);
// used by Set_Mouse_Position() in win_MD.C
void MWA_Set_Mouse_Position(int16_t x, int16_t y);


// win_DIR.C
int32_t LOF(char * file_name);


// win_EMS.C
void EMS_Startup(void);


// win_MD.C
extern int16_t platform_mouse_button_status;
extern int16_t lock_mouse_button_status_flag;
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status);



/*
    Fonts
*/
// DELETE  extern uint8_t g_Palette[];
// DELETE  extern uint8_t g_Palette_XBGR[];
extern uint8_t PFL_Palette[];

/*
    Fonts
*/
// void Apply_Palette(void);



#ifdef __cplusplus
}
#endif

#endif /* MOX_PFL_H */
