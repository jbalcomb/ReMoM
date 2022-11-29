
/*
*/

#include <windows.h>

#include "Win32_WinWnd.H"


int WndInit(void)
{

    WNDCLASSA WindowClass = {0};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WndProc;
    WindowClass.hInstance = g_hInstance;
    // WindowClass.hIcon;  // TODO(JimBalcomb,20220821): create and add icon for MoM-Rasm app
    WindowClass.lpszClassName = g_lpszClassName;

    OutputDebugStringA("DEBUG: BEGIN: WndInit()\n");

    RegisterClassA(&WindowClass);

    g_hWnd = CreateWindowExA(0, WindowClass.lpszClassName, g_lpszWindowName,
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  0, 0, g_hInstance, 0);

    OutputDebugStringA("DEBUG: END: WndInit()\n");

    return 0;
}
