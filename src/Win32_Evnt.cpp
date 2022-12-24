
/*
    Windows-specifc
    Handle Windows Event Messages

    ∵   Application-Type: Game
    ∴   1 Window's Window Window-Class
        1 Window's Window
        1 Window's Event Message Handler
*/

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM_main.H"

#include "Input.H"

#define STU_DEBUG 1
#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include "Win32.hpp"

#include "windows.h"
#include "windowsx.h"   /* GET_X_LPARAM(), GET_Y_LPARAM() */
#include "winuser.h"


uint8_t g_KbHit;



void Poll_Messages(void)
{
        MSG Message;
        while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT)
            {
                g_State_Run = false;
            }
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
}

LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            g_State_Run = false;

        } break;
    
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            g_State_Run = false;
        } break;

        // https://yakvi.github.io/handmade-hero-notes/html/day6.html#keyboardinput
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32_t VKCode = WParam;  // the virtual-key code of the key

            int16_t key_shift = GetKeyState(VK_SHIFT);    // VK_SHIFT    0x10  SHIFT key
            int16_t key_ctrl = GetKeyState(VK_CONTROL);  // VK_CONTROL  0x11  CTRL key
            int16_t key_alt = GetKeyState(VK_MENU);     // VK_MENU     0x12  ALT key


            if (key_ctrl && key_alt && VKCode == 'Q')
            {
                g_State_Run = false;
                OutputDebugStringA("Ctrl-Alt-Q\n");
            }

            if (VKCode == VK_ESCAPE)  // 0x1B
            {
                // g_State_Run = false;
                // g_KbHit = TRUE;
                g_Key_Pressed = ST_TRUE;
                g_Last_Key_Pressed = 0x1B;
                OutputDebugStringA("ESCAPE\n");
            } 

            if (VKCode == VK_SPACE)  // 0x20
            {
                g_KbHit = TRUE;
                g_Key_Pressed = ST_TRUE;
                g_Last_Key_Pressed = 0x20;
                OutputDebugStringA("SPACEBAR\n");
            } 

            if (VKCode == 'Q')
            {
                g_Key_Pressed = ST_TRUE;
                g_Last_Key_Pressed = 'Q';
                OutputDebugStringA("Q\n");
            } 

            if (VKCode == 'W')
            {
                OutputDebugStringA("W\n");
            } 
            if (VKCode == 'A')
            {
                OutputDebugStringA("A\n");
            } 
            if (VKCode == 'S')
            {
                OutputDebugStringA("S\n");
            } 
            if (VKCode == 'D')
            {
                OutputDebugStringA("D\n");
            } 
        } break;

        case WM_MOUSEMOVE:
        {
            // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
            // Do not use the LOWORD or HIWORD macros to extract the x- and y- coordinates of the cursor position ...
            // https://learn.microsoft.com/en-us/windows/win32/api/windowsx/nf-windowsx-get_x_lparam
            // Use GET_X_LPARAM instead of LOWORD to extract signed coordinate data. Negative screen coordinates may be returned on multiple monitor systems.
            int win32_mouse_x = GET_X_LPARAM(LParam); 
            int win32_mouse_y = GET_Y_LPARAM(LParam);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] win32_mouse_x: %d\n", __FILE__, __LINE__, win32_mouse_x);
    dbg_prn("DEBUG: [%s, %d] win32_mouse_y: %d\n", __FILE__, __LINE__, win32_mouse_y);
#endif
            POINTS win32_mouse_points = MAKEPOINTS(LParam);
            POINT ptMouse;
            GetCursorPos(&ptMouse);
            ScreenToClient(g_Window, &ptMouse);
            g_Mouse_X = (int16_t)ptMouse.x;
            g_Mouse_Y = (int16_t)ptMouse.y;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] g_Mouse_X: %d\n", __FILE__, __LINE__, g_Mouse_X);
    dbg_prn("DEBUG: [%s, %d] g_Mouse_Y: %d\n", __FILE__, __LINE__, g_Mouse_Y);
#endif

        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);
            Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}
