/**
 * headless_State.c — Platform shared state variable definitions (headless).
 *
 * These globals are declared as extern in Platform.h and defined here.
 * They form the data boundary between the engine and the platform layer.
 */

#include "../../platform/include/Platform.h"

int quit_game_flag;

int8_t key_pressed;

uint16_t scan_code_char_code;

PFL_Color platform_palette_buffer[256];

int lock_mouse_button_status_flag = 0;

int platform_mouse_input_enabled = 0;

int16_t platform_frame_mouse_buttons = 0;
