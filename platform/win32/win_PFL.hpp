#pragma once

#include "../../MoX/src/MOX_TYPE.h"

#include <Windows.h>

void Init_Platform(HINSTANCE hInstance, int nCmdShow);

extern uint16_t VK_to_SCCS[];

// disable name mangling in C++
#ifdef __cplusplus
extern "C" {
#endif

    // TODO  extern void (*Exit_With_Message)(char* string);

    extern HWND g_Window;

    void MWA_Exit_With_Message(char* string);

    void MWA_Set_Mouse_Position(int16_t x, int16_t y);

#ifdef __cplusplus
}
#endif
