#pragma once

#include <windows.h>



void Init_VideoBackBuffer(void);
void WndInit(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndEvnt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
