/**
 * sdl3_State.c — Platform shared state variable definitions.
 *
 * These globals are declared as extern in Platform.h and defined here.
 * They form the data boundary between the engine and the platform layer.
 */

#include "../../platform/include/Platform.h"

/* from MOM_PFL.c */

int quit_game_flag;

/*
Module: key
char (1 bytes) key_pressed
Address: 02:001B42E4
*/
int8_t key_pressed;

uint16_t scan_code_char_code;

/* ~== IBM-PC/MS-DOS Video Card's hardware VGA-DAC buffer */
PFL_Color platform_palette_buffer[256];

int lock_mouse_button_status_flag = 0;

int platform_mouse_input_enabled = 0;
