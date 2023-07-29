
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
                    MoX/MoM MOUSE_INT_Process and mouse_enabled flag control whether the cursor image get updated with the retore-save-draw procedures
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
?
Event-Driven waits for an Event (Message)
Polling continues cycling, optionally processing Events (Messages)
?
So, what business do you have, if you don't have an event?
i.e, No State-Change, No Update?
Y/N/M internal vs. external?
no input from user, but AI has state/input; also, play video or audio - next frame, next sample
? "HI vs. AI" ?
? "NPC vs. PC" ?
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







? this is like what Casey put in handmade_platform ?

? The 'Main Game Loop' MUST BE in the Platform-Layer. ?
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

? Screen_Control() has no return value and upon return MoM assumes Exit_With_Message() ?
? So, Screen_Control() is the Main Game Loop ?

### Font File
Init_STGE:  default font file
Init_MoM:   font file
