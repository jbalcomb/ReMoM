#pragma once

#include "MoX.H"
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

#ifdef __cplusplus
}
#endif
