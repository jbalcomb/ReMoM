


Microsoft Windows Platform-Layer (¿ "native" ?)
win_MoM.cpp
    #include <Windows.h>
    char MOM_FONT_FILE[] = "FONTS.LBX";
    char * GAME_FONT_FILE = &MOM_FONT_FILE[0];  // Create a Pointer to the Character-Array (string) at compile time.
    int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
        Debug_Log_Startup()
        Test_Run()
        ***Init_Platform(hInstance, nCmdShow)***
        Debug_Log_Shutdown()

win_PFL.hpp
    #include <Windows.h>
    void Init_Platform(HINSTANCE hInstance, int nCmdShow);
    extern uint16_t VK_to_SCCS[];
    extern HWND g_Window;
    void MWA_Exit_With_Message(char* string);
    void MWA_Set_Mouse_Position(int16_t x, int16_t y);

win_PFL.cpp
    #include <Windows.h>
    #include "windowsx.h"   /* GET_X_LPARAM(), GET_Y_LPARAM() */
    #include "win_PFL.hpp"
    #include "MOM_PFL.h"
    uint8_t PFL_Palette[1024];  // Platform's Shadow Palette: 256 colors * bits/bytes/components per color as required by the platform  e.g., RGB, XBGR, RGBA
    /* translate MWA Virtual Key Code to Scan Code / Character Code */
    uint16_t VK_to_SCCS[256] = { };
    struct win32_window_dimension { };
    struct win32_offscreen_buffer { };
    const CHAR szWindowClass[] = "MoM_Wnd_Cls";
    const CHAR szTitle[] = "(MoM-Rasm) Master of Magic v1.31 - Reassembly";
    HINSTANCE g_Instance;
    HWND g_Window;
    HDC g_DeviceContext;
    static int64_t GlobalPerfCountFrequency;  // HMH Day 18
    struct win32_offscreen_buffer PlatformVideoBackBuffer;  // Platform - Video Back Buffer
    int default_window_width = 640;
    int default_window_height = 400;
    int Pump_Events_Cnt = 0;
    int Pump_Paints_Cnt = 0;
    void Pump_Events(void);
    void Pump_Paints(void);
    struct win32_window_dimension Get_Window_Dimensions(HWND Window);
    void Update_Window_Display(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
    void Convert_320x200xVGA_To_320x200xXBGR(uint8_t * p_320x200xVGA, uint32_t* p_320x200xXBGR);
    void Init_Platform(HINSTANCE hInstance, int nCmdShow);
    void Init_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height);
    void WndInit(HINSTANCE hInstance, int nCmdShow);
    LRESULT CALLBACK WndEvnt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    // WZD s35p21
    void MWA_Set_Mouse_Position(int16_t x, int16_t y) { }







20230725:
These notes are so out of date as to be useless, if not misleading?
Just looking for a place to make sense of moving from Interrupts to Events w/w/o Polling?
...moving to win_PFL.cpp, don't want to just throw out Win32_Evnt.cpp

WM_MOUSEMOVE

WM_SYSKEYDOWN
WM_SYSKEYUP
WM_KEYDOWN
WM_KEYUP

WM_LBUTTONDOWN
WM_RBUTTONDOWN



WM_MOUSEMOVE    Update_Mouse_Position((int16_t)ptMouse.x, (int16_t)ptMouse.y);
                    MoX/MoM MOUSE_INT_Process and mouse_enabled flag control whether the cursor image get updated with the restore-save-draw procedures
                    but, also, my sad hack of lock_mouse_button_status_flag
WM_LBUTTONDOWN  Update_Mouse_Button_Status((int16_t)pt.x, (int16_t)pt.y, 0b00000001);
WM_RBUTTONDOWN  Update_Mouse_Button_Status((int16_t)pt.x, (int16_t)pt.y, 0b00000010);

Where stick the map of MS Window's Virtual-Keys to IBM-PC Scan-Code/Character-Code?
    for now, win_PFL.hpp
    probably should move to an odd, one-off of various platform's key codes
        ¿ more like SDL, less like GLFW ?

Elsewhere, ...
    uint16_t WindowsVirtualKeyCode_to_ScanCodeCharCode(uint16_t VK_Code, uint16_t Shift, uint16_t Control, uint16_t Alt)
    Get_Key_Modifiers()




make the game event-driven instead of polled

Event-Driven vs. Polling
¿¿¿
Event-Driven waits for an Event (Message)
Polling continues cycling, optionally processing Events (Messages)
???
So, what business do you have, if you don't have an event?
i.e, No State-Change, No Update?
Y/N/M internal vs. external?
no input from user, but AI has state/input; also, play video or audio - next frame, next sample
¿ "HI vs. AI" ?
¿ "NPC vs. PC" ?
"user"/"player"/"actor"/"character"


GetClientRect function (winuser.h)  
2021-10-13
Retrieves the coordinates of a window's client area.  
The client coordinates specify the upper-left and lower-right corners of the client area.  
Because client coordinates are relative to the upper-left corner of a window's client area,  
  the coordinates of the upper-left corner are (0,0).  

"client coordinates"  
"window's client area"  

GetWindowRect returns a rect in screen coordinates  
GetClientRect returns a rect in client coordinates  

InvalidateRect receives a rect in client coordinates.  
If you want to invalidate your entire client area,  
  then pass NULL to InvalidateRect.  
You could pass in the rect returned by GetClientRect,  
  but it is far simpler and clearer to pass NULL.  

A very simple explanation is that GetWindowRect() gives you the rectangle that includes the borders of the window.  
GetClientRect() gives you the rectangle that excludes the borders - the area that is allocated to the window specific drawing.  
Please note that GetWindowRect() returns a rectangle in screen coordinates - coordinates that are relative to the screen/monitor.  
GetClientRect() returns a rectangle that is relative to itself.  







¿ this is like what Casey put in handmade_platform ?

¿ The 'Main Game Loop' MUST BE in the Platform-Layer. ?
...given the need to poll and handle Event Messages
So, the platform-layer needs to known about the game...
...enough to be able to call some 'Do The Game' code...
Also, there needs to be some (minimal) shared state,
  as the platform-layer needs to know when to 'Quit The Game' / 'Exit The Main Game Loop'.
As I understand the nature of the thing,  
  the game code should make available some flag for the platform-layer to make use of.

Maybe the Windows Title/Name, icons, menus, etc. should be split off?
They are for the platform-layer, but expected per application, and probably a jagged-hierarchy shared across platforms?
e.g.,
window_title = lin_window_title = mac__window_title = win_window_title = "MoM - STU"


What about the input?


Meh. Something with 'types/typedefs'?






# Win32 & GDI

Win32_Main
Win32_Init
Win32_Evnt





# MoM (MGC & WZD)

Init STGE
Init MoM
Init MGC
Init WZD


Main Game Loop:
    Screen_Control()

¿ Screen_Control() has no return value and upon return MoM assumes Exit_With_Message() ?
¿ So, Screen_Control() is the Main Game Loop ?

### Font File
Init_STGE:  default font file
Init_MoM:   font file
