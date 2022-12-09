
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

#include "Win32.hpp"

#include "windows.h"



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
                g_State_Run = false;
                g_KbHit = TRUE;
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

            if (VKCode == 'W')
            {
                OutputDebugStringA("W\n");
            } 
            else if (VKCode == 'A')
            {
                OutputDebugStringA("A\n");
            } 
            else if (VKCode == 'S')
            {
                OutputDebugStringA("S\n");
            } 
            else if (VKCode == 'D')
            {
                OutputDebugStringA("D\n");
            } 
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
