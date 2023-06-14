
/*
    Initialize the Windows Desktop Application

    WNDCLASSA WindowClass
    HINSTANCE hInstance
    HWND hWnd

    WNDCLASS & RegisterClass()

    HINSTANCE hInstance
    CreateWindow()
    ShowWindow()
    UpdateWindow()


    WNDCLASSA WindowClass
        style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        lpfnWndProc = MainWindowCallback;
        hInstance = hInstance;
        lpszClassName = "MomRasmWindowClass";

    CreateWindowExA(
        0,
        WindowClass.lpszClassName,
        "MoM Reassembly",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0
    );

*/
/*
    ignore hPrevInstance
    ignore lpCmdLine, use GetCommandLine()
    ignore nShowCmd
*/
/*
    WinUser.h
    
/ *
 * ShowWindow() Commands
 * /
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11
*/

#include <windows.h>

#include <Win32.hpp>

// extern const char lpszClassName[];
// extern const char lpszWindowName[];
const char lpszClassName[] = "STU_MoM_WindowClass";
const char lpszWindowName[] = "MoM - STU";
// ? can/how can you pass around a function pointer variable ?
// WindowClass.lpfnWndProc = MainWindowCallback;
// extern HINSTANCE g_Instance;
// extern HWND g_Window;
// extern HDC g_DeviceContext;
HINSTANCE g_Instance;
HWND g_Window;
HDC g_DeviceContext;


extern LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);


BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);


// One & Only One - Class, Window, DeviceContext, ...
BOOL Init_WDA_Game(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
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
                                window_width, /* int nWidth   CW_USEDEFAULT, */
                                window_height, /* int nHeight  CW_USEDEFAULT, */
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
