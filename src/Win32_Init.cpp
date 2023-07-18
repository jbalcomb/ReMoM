#include <windows.h>
#include <Win32.hpp>

#include "MoX.H"


// One & Only One (OON) - Class, Window, DeviceContext, ...
BOOL Init_WDA_Game(HINSTANCE hInstance, int nShowCmd)
{
    InitApplication(hInstance);
    InitInstance(hInstance, nShowCmd);
    g_DeviceContext = GetDC(g_Window);  // requires WNDCLASS style CS_OWNDC
    return (TRUE);
}

//----< define and register window "class" >-------------------

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASSA WindowClass = {0};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = hInstance;
    // WindowClass.hIcon;  // TODO(JimBalcomb,20220821): create and add icon for MoM-Rasm app
    // WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.lpszClassName = lpszClassName;
    return(RegisterClassA(&WindowClass));
}

//----< create and display window >----------------------------

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_Instance = hInstance;

    OutputDebugStringA("CALL: CreateWindowExA()\n");
    g_Window =  CreateWindowExA(0,
                                lpszClassName,
                                lpszWindowName,
                                /* WS_OVERLAPPEDWINDOW | WS_VISIBLE, */
                                /* WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_POPUP, */
                                /* WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN, */
                                /* WS_OVERLAPPEDWINDOW | WS_VISIBLE & ~WS_CAPTION, */
                                WS_POPUP,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                window_pixel_width,  /* int nWidth  */
                                window_pixel_height, /* int nHeight */
                                0,
                                0,
                                hInstance,
                                0
                );

    // TODO(JimBalcomb,20221223): target client-area size, rather that window size
    // AdjustWindowRect function (winuser.h)

    OutputDebugStringA("CALL: ShowWindow()\n");
    ShowWindow(g_Window, nCmdShow);  // nCmdShow: default == SW_SHOWDEFAULT
    OutputDebugStringA("CALL: UpdateWindow()\n");
    UpdateWindow(g_Window);

    return (TRUE);
}
