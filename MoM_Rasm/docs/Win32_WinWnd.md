
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
