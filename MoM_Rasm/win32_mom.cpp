
#include <windows.h>

#define internal static 
#define local_persist static 
#define global_variable static 

global_variable bool KeepRunning;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;

internal void
Win32ResizeDIBSection(int Width, int Height)
{

    if (BitmapMemory) // Same as writing (BitmapMemory != 0) or (BitmapMemory != NULL)
    {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
        // Optionally, you can check if the result of VirtualFree is not zero.
        // Print out an error message if it is.
    }
    int BytesPerPixel = 4;
    int BitmapMemorySize = BytesPerPixel * (Width * Height);
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 

}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    StretchDIBits(DeviceContext, 
                  X, Y, Width, Height,
                  X, Y, Width, Height,
                  BitmapMemory,
                  &BitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            KeepRunning = false;

        } break;
    
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            KeepRunning = false;
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);

            EndPaint(Window, &Paint);
        } break;

        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;
            Win32ResizeDIBSection(Width, Height);
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        default:
        {
            Result = DefWindowProc(Window, Message, wParam, lParam);
        } break;
    }

    return Result;
}

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR     CommandeLine,
        int       ShowCode)
{

    // WNDCLASSEXW wcex = {0};
    // wcex.cbSize = sizeof(WNDCLASSEX);
    // wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // wcex.lpfnWndProc    = MainWindowCallback;
    // wcex.hInstance      = Instance;
    // // wcex.hIcon
    // wcex.lpszClassName  = "MomRasmWindowClass";
    // // wcex.hIconSm
    // //RegisterClassExW(&wcex);

    WNDCLASSA WindowClass = {0};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    // WindowClass.hIcon;  // TODO(JimBalcomb,20220821): create and add icon for MoM-Rasm app
    WindowClass.lpszClassName = "MomRasmWindowClass";

    RegisterClassA(&WindowClass);

    HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "MoM Reassembly",
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  0, 0, Instance, 0);

    KeepRunning = true;
    while (KeepRunning)
    {
        MSG Message;
        BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
        if (MessageResult > 0)              // 0 is the WM_QUIT message, -1 is invalid window handle
        {
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
        else
        {
            break;                          // break out of the loop
        }
    }
    
    return 0;
}
