#ifndef MOX_PFL_H
#define MOX_PFL_H

#include "MOX/MOX_TYPE.H"

// #ifdef _STU_SDL2
// #include "SDL.h"
#include "C:\devellib\SDL2-2.32.2\include\SDL.h"
// #endif



#ifdef __cplusplus
extern "C" {
#endif

/*
    set expectations to be met for the platform-layer by the platform-specific code
        decl here
        defn/impl there
*/

extern int quit_game_flag;


// #ifdef _STU_SDL2
/*
Module: key
char (1 bytes) key_pressed
Address: 02:001B42E4
*/
extern int8_t key_pressed;
// #endif
// #ifdef _STU_WIN
// extern int key_pressed;  // win_KD.C  Keyboard_Status(), Read_Key()
// #endif

extern uint16_t scan_code_char_code;  // win_KD.C  Read_Key()

// ITRY  extern int platform_mouse_button_status;

extern int lock_mouse_button_status_flag;

extern int platform_mouse_input_enabled;



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



/*
    PLATFORM
*/

void Platform_Palette_Update(void);
void Platform_Video_Update(void);



// win_PFL.cpp
// used by Get_Input() in win_Input.C
#ifdef _STU_WIN
void Pump_Events(void);
#endif
// used by Page_Flip() in win_Video.C
#ifdef _STU_WIN
void Pump_Paints(void);
#endif
// used by Set_Mouse_Position() in win_MD.C
#ifdef _STU_WIN
void MWA_Set_Mouse_Position(int16_t x, int16_t y);
#endif

// win_DIR.C
int32_t LOF(char * file_name);


// win_EMS.C
void EMS_Startup(void);


// win_MD.C
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status);



/*
    Fonts
*/
// #ifdef _STU_SDL2
extern SDL_Color platform_palette_buffer[256];
// #endif
#ifdef _STU_WIN
extern uint8_t platform_palette_buffer[1024];  // Platform's Shadow Palette: 256 colors * bits/bytes/components per color as required by the platform  e.g., RGB, XBGR, RGBA
#endif

/*
    Fonts
*/
// void Apply_Palette(void);



#ifdef __cplusplus
}
#endif

#endif /* MOX_PFL_H */
