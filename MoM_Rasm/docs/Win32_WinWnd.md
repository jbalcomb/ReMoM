
Windows Desktop Application
WinMain() & Window Procedure [WndProc()]

Once:
    WinMain()
Forever:
    WndProc()

Step 0:
    WinMain()
Step n:
    WndProc()

First:
    WinMain()
Next:
    WndProc()


WinMain()
    Initialize Window
    Message Loop
        ? Process Messages ?

Glow-Balls:
    Window's Application Instance
    Application's Window Instance
    Application State - Keep Running
    Video Back Buffer - Pixels & Colors [BITMAP]
    ? Decl, Defn, Impl ?


? Win32GetWindowDimension() |-> |-> GetClientRect()

? |-> Win32ResizeDIBSection()

? |-> Win32DisplayBufferInWindow() |-> StretchDIBits()

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);
            Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

    while (GlobalRunning)
    {
        /* Process Message */
        /* Draw Back Buffer */
        win32_window_dimension Dimension = Win32GetWindowDimension(Window);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
    }

Note:
    There are two events that happen before the user gets the application window:
        WM_ACTIVATEAPP
        WM_SIZE
    ...  
    also, ...  
    https://www.informit.com/articles/article.aspx?p=28595&seqNum=4  
    When a window is resized, not only is a WM_SIZE message sent, but a WM_PAINT message is sent as well!  


WINUSERAPI
HWND
WINAPI
CreateWindowExA(
    _In_ DWORD dwExStyle,
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam);

    g_Window =  CreateWindowExA(0,
                                lpszClassName,
                                lpszWindowName,
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

dwExStyle  
    NULL  

dwStyle  
    WS_OVERLAPPEDWINDOW | WS_VISIBLE  

#### Window Dimensions:
    X
        CW_USEDEFAULT
    Y
        CW_USEDEFAULT
    nWidth
        CW_USEDEFAULT
    nHeight
        CW_USEDEFAULT
