

int g_GUI_Windows = 0;                      // dseg:43B0

extern int g_GUI_WindowCount = 1;           // dseg:4398


int g_MOUSE_Hardware;                       // dseg:4D20 
int g_GUI_FocusedControl = -1;              // dseg:4D22

struct GUI_WINDOW g_GUI_VirtualScreen = {0, 0, 0, 319, 199, 0};   // dseg:4D2E ; a record of size 0Ch, this is off by 2 bytes based on the coordinates it contains

int g_GUI_EscapeOverride = 0;               // dseg:4D44


void far * gfp_GUI_Control_Table;           // dseg:A8A2

int g_GUI_Delay;                            // dseg:A88E
int g_MOUSE_Emu_Y;                          // dseg:A890
int g_MOUSE_Emu_X;                          // dseg:A892
