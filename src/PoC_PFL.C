#include <windows.h>
#include "MoX.H"
#include "PoC_PFL.H"

/*
    Platform-Layer Screen Buffer / Window Surface
*/

uint8_t g_State_Run;


// TODO  move this and Load_Font_File(), Load_Palette(), and Apply_Pallete() to the platform-layer
uint8_t g_Palette[768];  // ~== IBM-PC/MS-DOS Video Card's hardware VGA-DAC buffer
uint8_t g_Palette_XBGR[1024];
