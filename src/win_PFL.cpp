#include <windows.h>
#include "windowsx.h"   /* GET_X_LPARAM(), GET_Y_LPARAM() */
#include <timeapi.h>    /* timeBeginPeriod() */
#include <stdint.h>

#include "win_PFL.hpp"

#include "MoX.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



/*
    Platform-Layer Screen Buffer / Window Surface
*/



// DELETE  uint8_t g_State_Run;

// DELETE  uint8_t g_Palette[768];  // ~== IBM-PC/MS-DOS Video Card's hardware VGA-DAC buffer
// DELETE  uint8_t g_Palette_XBGR[1024];
uint8_t PFL_Palette[1024];  // Platform's Shadow Palette: 256 colors * bits/bytes/components per color as required by the platform  e.g., RGB, XBGR, RGBA



uint16_t VK_to_SCCS[128] = {
    /*   0 0x00 */  0x0000,
    /*   1 0x01 */  0x0000,
    /*   2 0x02 */  0x0000,
    /*   3 0x03 */  0x0000,
    /*   4 0x04 */  0x0000,
    /*   5 0x05 */  0x0000,
    /*   6 0x06 */  0x0000,
    /*   7 0x07 */  0x0000,
    /*   8 0x08 */  0x0000,
    /*   9 0x09 */  0x0000,
    /*  10 0x0A */  0x0000,
    /*  11 0x0B */  0x0000,
    /*  12 0x0C */  0x0000,
    /*  13 0x0D */  0x0000,
    /*  14 0x0E */  0x0000,
    /*  15 0x0F */  0x0000,
    /*  16 0x10 */  0x0000,
    /*  17 0x11 */  0x0000,
    /*  18 0x12 */  0x0000,
    /*  19 0x13 */  0x0000,
    /*  20 0x14 */  0x0000,
    /*  21 0x15 */  0x0000,
    /*  22 0x16 */  0x0000,
    /*  23 0x17 */  0x0000,
    /*  24 0x18 */  0x0000,
    /*  25 0x19 */  0x0000,
    /*  26 0x1A */  0x0000,
    /*  27 0x1B */  0x011B,  /* VK_ESCAPE */
    /*  28 0x1C */  0x0000,
    /*  29 0x1D */  0x0000,
    /*  30 0x1E */  0x0000,
    /*  31 0x1F */  0x0000,
    /*  32 0x20 */  0x3920,  /* VK_SPACE */
    /*  33 0x21 */  0x0000,
    /*  34 0x22 */  0x0000,
    /*  35 0x23 */  0x0000,
    /*  36 0x24 */  0x0000,
    /*  37 0x25 */  0x0000,
    /*  38 0x26 */  0x0000,
    /*  39 0x27 */  0x0000,
    /*  40 0x28 */  0x0000,
    /*  41 0x29 */  0x0000,
    /*  42 0x2A */  0x0000,
    /*  43 0x2B */  0x0000,
    /*  44 0x2C */  0x0000,
    /*  45 0x2D */  0x0000,
    /*  46 0x2E */  0x0000,
    /*  47 0x2F */  0x0000,
    /*  48 0x30 */  0x0B30,  /* VK_0 */
    /*  49 0x31 */  0x0231,  /* VK_1 */
    /*  50 0x32 */  0x0332,  /* VK_2 */
    /*  51 0x33 */  0x0433,  /* VK_3 */
    /*  52 0x34 */  0x0534,  /* VK_4 */
    /*  53 0x35 */  0x0635,  /* VK_5 */
    /*  54 0x36 */  0x0736,  /* VK_6 */
    /*  55 0x37 */  0x0837,  /* VK_7 */
    /*  56 0x38 */  0x0938,  /* VK_8 */
    /*  57 0x39 */  0x0A39,  /* VK_9 */
    /*  58 0x3A */  0x0000,  /* unassigned */
    /*  59 0x3B */  0x0000,  /* unassigned */
    /*  60 0x3C */  0x0000,  /* unassigned */
    /*  61 0x3D */  0x0000,  /* unassigned */
    /*  62 0x3E */  0x0000,  /* unassigned */
    /*  63 0x3F */  0x0000,  /* unassigned */
    /*  64 0x40 */  0x0000,  /* unassigned */
    /*  65 0x41 */  0x1E61,  /* VK_A */
    /*  66 0x42 */  0x3062,  /* VK_B */
    /*  67 0x43 */  0x2E63,  /* VK_C */
    /*  68 0x44 */  0x2064,  /* VK_D */
    /*  69 0x45 */  0x1265,  /* VK_E */
    /*  70 0x46 */  0x2166,  /* VK_F */
    /*  71 0x47 */  0x2267,  /* VK_G */
    /*  72 0x48 */  0x2368,  /* VK_H */
    /*  73 0x49 */  0x1769,  /* VK_I */
    /*  74 0x4A */  0x246A,  /* VK_J */
    /*  75 0x4B */  0x256B,  /* VK_K */
    /*  76 0x4C */  0x266C,  /* VK_L */
    /*  77 0x4D */  0x326D,  /* VK_M */
    /*  78 0x4E */  0x316E,  /* VK_N */
    /*  79 0x4F */  0x186F,  /* VK_O */
    /*  80 0x50 */  0x1970,  /* VK_P */
    /*  81 0x51 */  0x1071,  /* VK_Q */
    /*  82 0x52 */  0x1372,  /* VK_R */
    /*  83 0x53 */  0x1F73,  /* VK_S */
    /*  84 0x54 */  0x1474,  /* VK_T */
    /*  85 0x55 */  0x1675,  /* VK_U */
    /*  86 0x56 */  0x2F76,  /* VK_V */
    /*  87 0x57 */  0x1177,  /* VK_W */
    /*  88 0x58 */  0x2D78,  /* VK_X */
    /*  89 0x59 */  0x1579,  /* VK_Y */
    /*  90 0x5A */  0x2C7A,  /* VK_Z */
    /*  91 0x5B */  0x0000,
    /*  92 0x5C */  0x0000,
    /*  93 0x5D */  0x0000,
    /*  94 0x5E */  0x0000,
    /*  95 0x5F */  0x0000,
    /*  96 0x60 */  0x0000,
    /*  97 0x61 */  0x0000,
    /*  98 0x62 */  0x0000,
    /*  99 0x63 */  0x0000,
    /* 100 0x64 */  0x0000,
    /* 101 0x65 */  0x0000,
    /* 102 0x66 */  0x0000,
    /* 103 0x67 */  0x0000,
    /* 104 0x68 */  0x0000,
    /* 105 0x69 */  0x0000,
    /* 106 0x6A */  0x0000,
    /* 107 0x6B */  0x0000,
    /* 108 0x6C */  0x0000,
    /* 109 0x6D */  0x0000,
    /* 110 0x6E */  0x0000,
    /* 111 0x6F */  0x0000,
    /* 112 0x70 */  0x0000,
    /* 113 0x71 */  0x0000,
    /* 114 0x72 */  0x0000,
    /* 115 0x73 */  0x0000,
    /* 116 0x74 */  0x0000,
    /* 117 0x75 */  0x0000,
    /* 118 0x76 */  0x0000,
    /* 119 0x77 */  0x0000,
    /* 120 0x78 */  0x0000,
    /* 121 0x79 */  0x0000,
    /* 122 0x7A */  0x0000,
    /* 123 0x7B */  0x0000,
    /* 124 0x7C */  0x0000,
    /* 125 0x7D */  0x0000,
    /* 126 0x7E */  0x0000,
    /* 127 0x7F */  0x0000
};



struct win32_window_dimension
{
    int Width;
    int Height;
};

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void* Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};



// const WCHAR szWindowClass[] = L"MoM_Wnd_Cls";
// const WCHAR szTitle[] = L"(MoM-Rasm) Master of Magic v1.31 - Reassembly";
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
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status);
struct win32_window_dimension Get_Window_Dimensions(HWND Window);
void Update_Window_Display(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Convert_320x200xVGA_To_320x200xXBGR(uint8_t * p_320x200xVGA, uint32_t* p_320x200xXBGR);
void Init_Platform(HINSTANCE hInstance, int nCmdShow);
void Init_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height);
void WndInit(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndEvnt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



void Pump_Events(void)
{
        // char FPSBuffer[256];
        // sprintf_s(FPSBuffer, sizeof(FPSBuffer), "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
        // OutputDebugStringA(FPSBuffer);


    Pump_Events_Cnt++;
    MSG Message;
    while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
    {
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Message.message: %d\n", __FILE__, __LINE__, Message.message);
#endif
        if (Message.message == WM_QUIT)
        {
            // DELETE  g_State_Run = false;
            quit_game_flag = ST_TRUE;
        }
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }
}

void Pump_Paints(void)
{
    Pump_Paints_Cnt++;
    InvalidateRect(g_Window, NULL, FALSE);
    MSG Message;
    while (PeekMessageA(&Message, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
    {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }
}


// ¿ ~== Mouse_Int_Handler() ?
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y)
{
    if (lock_mouse_button_status_flag != ST_TRUE)
    {
        mouse_x = platform_mouse_x / 2;
        mouse_y = platform_mouse_y / 2;
        // mouse_x = platform_mouse_x / display_scale;
        // mouse_y = platform_mouse_y / display_scale;
    }
    if (MOUSE_INT_Process == ST_FALSE)
    {
        MOUSE_INT_Process = ST_TRUE;
        // TODO  MOUSE_SaveClick();
        if (mouse_enabled != ST_FALSE)
        {
            mouse_enabled = ST_FALSE;
            if (current_mouse_list_count >= 2)
            {
                Check_Mouse_Shape(mouse_x, mouse_y);
            }
            Restore_Mouse_On_Page();                // mouse_background_buffer -> video_page_buffer[draw_page_num]
            Save_Mouse_On_Page(mouse_x, mouse_y);   // video_page_buffer[draw_page_num] -> mouse_background_buffer
            Draw_Mouse_On_Page(mouse_x, mouse_y);   // mouse_palette -> video_page_buffer[draw_page_num]
            mouse_enabled = ST_TRUE;
        }
        MOUSE_INT_Process = ST_FALSE;
    }
}

void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status)
{
    if(lock_mouse_button_status_flag != ST_TRUE)
    {
        mouse_x = platform_mouse_x / 2;
        mouse_y = platform_mouse_y / 2;
        // mouse_x = platform_mouse_x / display_scale;
        // mouse_y = platform_mouse_y / display_scale;
        platform_mouse_button_status = mouse_button_status;
    }
}

struct win32_window_dimension Get_Window_Dimensions(HWND Window)
{
    win32_window_dimension WndDim;
    
    RECT ClientAreaRect;
    GetClientRect(Window, &ClientAreaRect);
    WndDim.Width = ClientAreaRect.right - ClientAreaRect.left;
    WndDim.Height = ClientAreaRect.bottom - ClientAreaRect.top;
    
    return WndDim;
}

void Update_Window_Display(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
// DELETE      // Apply_Palette()
// DELETE      int16_t itr;
// DELETE      for(itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
// DELETE      {
// DELETE          if(*(p_Palette + 768 + itr) == 1)  // TODO  ~ #define Palette Flags Offset
// DELETE          {
// DELETE              *(PFL_Palette + (itr * 4) + 3) = 0x00;
// DELETE              *(PFL_Palette + (itr * 4) + 2) = (*(p_Palette + (itr * 3) + 0) << 2);
// DELETE              *(PFL_Palette + (itr * 4) + 1) = (*(p_Palette + (itr * 3) + 1) << 2);
// DELETE              *(PFL_Palette + (itr * 4) + 0) = (*(p_Palette + (itr * 3) + 2) << 2);
// DELETE          }
// DELETE      }


    // Convert_320x200xVGA_To_320x200xXBGR(p_320x200xVGA, p_320x200xXBGR);
    uint8_t * p_320x200xVGA;
    uint32_t * p_320x200xXBGR;
    uint32_t * p_XBGR;

    p_320x200xVGA = video_page_buffer[draw_page_num];
    p_320x200xXBGR = (uint32_t*)Buffer->Memory;
    p_XBGR = (uint32_t*)PFL_Palette;  // ~== IBM-PC VGA-DAC

    int width;
    int height;
    int itr_width;
    int itr_height;
    width = screen_pixel_width;
    height = screen_pixel_height;
    uint8_t color_map_index;
    uint32_t color;
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            color_map_index = *(p_320x200xVGA + (itr_height * width) + itr_width);
            color = p_XBGR[color_map_index];
            // assert(color != 0);
// #ifdef STU_DEBUG
// //     if(color_map_index == 0x72)
// //     {
// //         dbg_prn("DEBUG: [%s, %d]: PFL_Palette[%d]: %08X\n", __FILE__, __LINE__, color_map_index, color);
// //     }
//     // if( ((itr_height >= 111) || (itr_height <= 111+16)) && ((itr_width >= 121) || (itr_height <= 121+18)) )
//     if( ((itr_height >= 111) || (itr_height < 111+16)) && ((itr_width >= 121) || (itr_height < 121+18)) )
//     {
//         dbg_prn("DEBUG: [%s, %d]: PFL_Palette[%d]: %08X\n", __FILE__, __LINE__, color_map_index, color);
//     }
// #endif
            *(p_320x200xXBGR + (itr_height * width) + itr_width) = color;
        }
    }


    StretchDIBits(DeviceContext, 
                  0, 0, WindowWidth, WindowHeight,      /* destination rectangle (window)   */
                  0, 0, Buffer->Width, Buffer->Height,  /* source rectangle (bitmap buffer) */
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
    
}


void Convert_320x200xVGA_To_320x200xXBGR(uint8_t * p_320x200xVGA, uint32_t* p_320x200xXBGR)
{
    unsigned int* p_XBGR;
    int itr;
    int width;
    int height;
    int itr_width;
    int itr_height;
    uint8_t color_map_index;
    uint32_t color;

// DELETE      p_XBGR = (uint32_t*)g_Palette_XBGR;  // ~== IBM-PC VGA-DAC
    p_XBGR = (uint32_t*)PFL_Palette;  // ~== IBM-PC VGA-DAC

    width = 320;
    height = 320;
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            color_map_index = *(p_320x200xVGA + (itr_height * 320) + itr_width);
            color = p_XBGR[color_map_index];
            *(p_320x200xXBGR + (itr_height * 320) + itr_width) = color;
        }
    }

}

void Init_Platform(HINSTANCE hInstance, int nCmdShow)
{

    Init_Window_Back_Buffer(&PlatformVideoBackBuffer, screen_pixel_width, screen_pixel_height);

    WndInit(hInstance, nCmdShow);



    // Getting the performance frequency.
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    // NOTE: Set the Windows scheduler granularity to 1ms, so that our Sleep() can be more granular
    UINT DesiredSchedulerMS = 1;
    int32_t SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

    // TODO(casey): How do we reliably query on this on Windows?
    int MonitorRefreshHz = 60;
    // int GameUpdateHz = MonitorRefreshHz / 2;  // 30 FPS
    // int GameUpdateHz = MonitorRefreshHz / 4;  // 15 FPS
    int GameUpdateHz = MonitorRefreshHz / 6;  // 10 FPS
    // int GameUpdateHz = MonitorRefreshHz / 12;  // 5 FPS
    float TargetSecondsPerFrame = 1.0f / (float)GameUpdateHz;

}

// ¿ this one makes sense for when the platform layer is handling the conversion from *native* MoX code ?
void Init_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height)
{
    int BitmapMemorySize;
    if(Buffer->Memory != NULL)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }
    Buffer->BytesPerPixel = 4;
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;  // TODO make this 24bpp for XBGR and 32bpp for RGBA
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // negative value: top-down
    BitmapMemorySize = Buffer->BytesPerPixel * (Buffer->Width * Buffer->Height);
    Buffer->Memory = VirtualAlloc(nullptr, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if(Buffer->Memory == NULL)
    {
        // ErrorExit(TEXT("VirtualAlloc"));
    }
    memset(Buffer->Memory, 1, BitmapMemorySize);
}

void WndInit(HINSTANCE hInstance, int nCmdShow)
{
    g_Instance = hInstance;
//     WNDCLASSEXW wcex = { 0 };
    WNDCLASSEX wcex = { 0 };
//     wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndEvnt;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = g_Instance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;
//     RegisterClassExW(&wcex);
    RegisterClassEx(&wcex);
    RECT   rect = {0, 0, default_window_width, default_window_height};
    DWORD  dwStyle = WS_OVERLAPPEDWINDOW;
    BOOL   bMenu = false;
    DWORD  dwExStyle = NULL;
    AdjustWindowRectEx(&rect, dwStyle, bMenu, dwExStyle);

//     g_Window = CreateWindowExW(NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
//                                 CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left), (rect.bottom - rect.top), 
//                                 NULL, NULL, g_Instance, NULL);
    g_Window = CreateWindowEx(NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
                                CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left), (rect.bottom - rect.top), 
                                NULL, NULL, g_Instance, NULL);
    if (!g_Window)
    {
        // return FALSE;
    }
    ShowWindow(g_Window, nCmdShow);
    UpdateWindow(g_Window);
    g_DeviceContext = GetDC(g_Window);  // requires WNDCLASS style CS_OWNDC
}


LRESULT CALLBACK WndEvnt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // HBRUSH hBrush = CreateSolidBrush(RGB(106, 90, 205));  // "Slate Blue"
            // FillRect(hdc, &ps.rcPaint, hBrush);
            // DeleteObject(hBrush);
            
            // RECT ClientAreaRect;
            // GetClientRect(hWnd, &ClientAreaRect);
            // int ClientAreaWidth = ClientAreaRect.right - ClientAreaRect.left;
            // int ClientAreaHeight = ClientAreaRect.bottom - ClientAreaRect.top;

            struct win32_window_dimension WndDim = Get_Window_Dimensions(hWnd);

            Update_Window_Display(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);
            
            // Update_Window_Display_2x_XBGR(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);

            EndPaint(hWnd, &ps);
        } break;


        case WM_MOUSEMOVE:
        {
            OutputDebugStringA("WM_MOUSEMOVE\n");
            POINT ptMouse;
            GetCursorPos(&ptMouse);
            ScreenToClient(g_Window, &ptMouse);
            Update_Mouse_Position((int16_t)ptMouse.x, (int16_t)ptMouse.y);

        } break;


        // https://yakvi.github.io/handmade-hero-notes/html/day6.html#keyboardinput
        // case WM_SYSKEYDOWN:
        // case WM_SYSKEYUP:
        // case WM_KEYDOWN:
        // case WM_KEYUP:
        case WM_KEYDOWN:
        {
            OutputDebugStringA("WM_KEYDOWN\n");
            int VKCode = wParam;  // the virtual-key code of the key
            int key_shift = GetKeyState(VK_SHIFT);  // VK_SHIFT    0x10  SHIFT key
            int key_ctrl = GetKeyState(VK_CONTROL); // VK_CONTROL  0x11  CTRL key
            int key_alt = GetKeyState(VK_MENU);     // VK_MENU     0x12  ALT key
            if (GetKeyState(VK_LMENU) & 0x8000)
            {
                // Left ALT key is down.
            }
            if (GetKeyState(VK_RMENU) & 0x8000)
            {
                // Right ALT key is down.
            }

            if(VKCode == VK_ESCAPE && _global_esc == ST_TRUE)
            {
                PostQuitMessage(0);
            }
            if (key_ctrl && key_alt && VKCode == 'Q')
            {
                PostQuitMessage(0);
            }

            key_pressed = ST_TRUE;
            scan_code_char_code = VK_to_SCCS[VKCode];


        } break;


        case WM_LBUTTONDOWN:
        {
            OutputDebugStringA("WM_LBUTTONDOWN\n");
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            Update_Mouse_Button_Status((int16_t)pt.x, (int16_t)pt.y, 0b00000001);
        } break;
        case WM_RBUTTONDOWN:
        {
            OutputDebugStringA("WM_RBUTTONDOWN\n");
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            Update_Mouse_Button_Status((int16_t)pt.x, (int16_t)pt.y, 0b00000010);
        } break;


        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
            RECT ClientAreaRect;
            GetClientRect(g_Window, &ClientAreaRect);
            // int ClientAreaWidth = ClientAreaRect.right - ClientAreaRect.left;
            // int ClientAreaHeight = ClientAreaRect.bottom - ClientAreaRect.top;
            // Resize_Window_Back_Buffer(&PlatformVideoBackBuffer, ClientAreaWidth, ClientAreaHeight);
            HBRUSH hBrush = CreateSolidBrush(RGB(106, 90, 205));  // "Slate Blue"
            FillRect(g_DeviceContext, &ClientAreaRect, hBrush);
            DeleteObject(hBrush);

        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_CREATE:
        {
            OutputDebugStringA("WM_CREATE\n");
            ShowCursor(FALSE);
            // struct win32_window_dimension WndDim = Get_Window_Dimensions(hWnd);
            // Init_Window_Back_Buffer(&PlatformVideoBackBuffer, WndDim.Width, WndDim.Height);
        } break;

        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            quit_game_flag = ST_TRUE;
            current_screen = scr_Quit_To_DOS;
            // DELETE  g_State_Run = false;
        } break;

        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            PostQuitMessage(0);
        } break;



        default:
            return DefWindowProc(hWnd, message, wParam, lParam);

    }

    return 0;
}
