
#include <Windows.h>
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Mouse.h"
#include "windowsx.h"   /* GET_X_LPARAM(), GET_Y_LPARAM() */
#include <stdint.h>

#include "win_PFL.hpp"

#include "../../MoX/src/Video.h"
#include "../../MoX/src/Video2.h"
#include "../../MoX/src/Input.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/MOX_BASE.h"

#include "MOM_PFL.h"
#include "MOM_SCR.h"



/*
    Platform-Layer Screen Buffer / Window Surface
*/





/*
    translate MWA Virtual Key Code to Scan Code / Character Code
*/
uint16_t VK_to_SCCS[256] = {
    /*   0 0x00 */  0x0000,
    /*   1 0x01 */  0x0000,
    /*   2 0x02 */  0x0000,
    /*   3 0x03 */  0x0000,
    /*   4 0x04 */  0x0000,
    /*   5 0x05 */  0x0000,
    /*   6 0x06 */  0x0000,
    /*   7 0x07 */  0x0000,
    /*   8 0x08 */  SCCC_BACKSPACE, /* VK_BACK */
    /*   9 0x09 */  0x0000,
    /*  10 0x0A */  0x0000,
    /*  11 0x0B */  0x0000,
    /*  12 0x0C */  0x0000,
    /*  13 0x0D */  SCCC_ENTER,     /* VK_RETURN */
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
    /*  27 0x1B */  SCCC_ESCAPE,    /* VK_ESCAPE */
    /*  28 0x1C */  0x0000,
    /*  29 0x1D */  0x0000,
    /*  30 0x1E */  0x0000,
    /*  31 0x1F */  0x0000,
    /*  32 0x20 */  SCCC_SPACE,     /* VK_SPACE */
    /*  33 0x21 */  0x0000,
    /*  34 0x22 */  0x0000,
    /*  35 0x23 */  0x0000,
    /*  36 0x24 */  0x0000,
    /*  37 0x25 */  SCCC_LEFT,      /* VK_LEFT  */
    /*  38 0x26 */  SCCC_UP,        /* VK_UP    */
    /*  39 0x27 */  SCCC_RIGHT,     /* VK_RIGHT */
    /*  40 0x28 */  SCCC_DOWN,      /* VK_DOWN  */
    /*  41 0x29 */  0x0000,
    /*  42 0x2A */  0x0000,
    /*  43 0x2B */  0x0000,
    /*  44 0x2C */  0x0000,
    /*  45 0x2D */  0x0000,
    /*  46 0x2E */  SCCC_DELETE,    /* VK_DELETE */
    /*  47 0x2F */  0x0000,
    /*  48 0x30 */  SCCC_0,  /* VK_0 */
    /*  49 0x31 */  SCCC_1,  /* VK_1 */
    /*  50 0x32 */  SCCC_2,  /* VK_2 */
    /*  51 0x33 */  SCCC_3,  /* VK_3 */
    /*  52 0x34 */  SCCC_4,  /* VK_4 */
    /*  53 0x35 */  SCCC_5,  /* VK_5 */
    /*  54 0x36 */  SCCC_6,  /* VK_6 */
    /*  55 0x37 */  SCCC_7,  /* VK_7 */
    /*  56 0x38 */  SCCC_8,  /* VK_8 */
    /*  57 0x39 */  SCCC_9,  /* VK_9 */
    /*  58 0x3A */  0x0000,  /* unassigned */
    /*  59 0x3B */  0x0000,  /* unassigned */
    /*  60 0x3C */  0x0000,  /* unassigned */
    /*  61 0x3D */  0x0000,  /* unassigned */
    /*  62 0x3E */  0x0000,  /* unassigned */
    /*  63 0x3F */  0x0000,  /* unassigned */
    /*  64 0x40 */  0x0000,  /* unassigned */
    /*  65 0x41 */  SCCC_A,  /* VK_A */
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
    /*  96 0x60 */  0x0000,  /* VK_NUMPAD0 */
    /*  97 0x61 */  SCCC_KP_1,  /* VK_NUMPAD1 */
    /*  98 0x62 */  SCCC_KP_2,  /* VK_NUMPAD2 */
    /*  99 0x63 */  SCCC_KP_3,  /* VK_NUMPAD3 */
    /* 100 0x64 */  SCCC_KP_4,  /* VK_NUMPAD4 */
    /* 101 0x65 */  0x0000,  // SCCC_KP_5,  /* VK_NUMPAD5 */
    /* 102 0x66 */  SCCC_KP_6,  /* VK_NUMPAD6 */
    /* 103 0x67 */  SCCC_KP_7,  /* VK_NUMPAD7 */
    /* 104 0x68 */  SCCC_KP_8,  /* VK_NUMPAD8 */
    /* 105 0x69 */  SCCC_KP_9,  /* VK_NUMPAD9 */
    /* 106 0x6A */  0x0000,
    /* 107 0x6B */  SCCC_PLUS,  /* VK_ADD	0x6B	Add key */
    /* 108 0x6C */  0x0000,
    /* 109 0x6D */  SCCC_MINUS,  /* VK_SUBTRACT  0x6D  Subtract key */
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
    /* 127 0x7F */  0x0000,
    /* 128 0x */  0x0000,
    /* 129 0x */  0x0000,
    /* 130 0x */  0x0000,
    /* 131 0x */  0x0000,
    /* 132 0x */  0x0000,
    /* 133 0x */  0x0000,
    /* 134 0x */  0x0000,
    /* 135 0x */  0x0000,
    /* 136 0x */  0x0000,
    /* 137 0x */  0x0000,
    /* 138 0x */  0x0000,
    /* 139 0x */  0x0000,
    /* 140 0x */  0x0000,
    /* 141 0x */  0x0000,
    /* 142 0x */  0x0000,
    /* 143 0x */  0x0000,
    /* 144 0x */  0x0000,
    /* 145 0x */  0x0000,
    /* 146 0x */  0x0000,
    /* 147 0x */  0x0000,
    /* 148 0x */  0x0000,
    /* 149 0x */  0x0000,
    /* 150 0x */  0x0000,
    /* 151 0x */  0x0000,
    /* 152 0x */  0x0000,
    /* 153 0x */  0x0000,
    /* 154 0x */  0x0000,
    /* 155 0x */  0x0000,
    /* 156 0x */  0x0000,
    /* 157 0x */  0x0000,
    /* 158 0x */  0x0000,
    /* 159 0x */  0x0000,
    /* 160 0x */  0x0000,
    /* 161 0x */  0x0000,
    /* 162 0x */  0x0000,
    /* 163 0x */  0x0000,
    /* 164 0x */  0x0000,
    /* 165 0x */  0x0000,
    /* 166 0x */  0x0000,
    /* 167 0x */  0x0000,
    /* 168 0x */  0x0000,
    /* 169 0x */  0x0000,
    /* 170 0x */  0x0000,
    /* 171 0x */  0x0000,
    /* 172 0x */  0x0000,
    /* 173 0x */  0x0000,
    /* 174 0x */  0x0000,
    /* 175 0x */  0x0000,
    /* 176 0x */  0x0000,
    /* 177 0x */  0x0000,
    /* 178 0x */  0x0000,
    /* 179 0x */  0x0000,
    /* 180 0x */  0x0000,
    /* 181 0x */  0x0000,
    /* 182 0x */  0x0000,
    /* 183 0x */  0x0000,
    /* 184 0x */  0x0000,
    /* 185 0x */  0x0000,
    /* 186 0x */  0x0000,
    /* 187 0xBB */  SCCC_PLUS,
    /* 188 0xBC */  0x0000,
    /* 189 0xBD */  SCCC_MINUS,  /* VK_OEM_MINUS	0xBD	For any country/region, the - key */
    /* 190 0x */  0x0000,
    /* 191 0x */  0x0000,
    /* 192 0x */  0x0000,
    /* 193 0x */  0x0000,
    /* 194 0x */  0x0000,
    /* 195 0x */  0x0000,
    /* 196 0x */  0x0000,
    /* 197 0x */  0x0000,
    /* 198 0x */  0x0000,
    /* 199 0x */  0x0000,
    /* 200 0x */  0x0000,
    /* 201 0x */  0x0000,
    /* 202 0x */  0x0000,
    /* 203 0x */  0x0000,
    /* 204 0x */  0x0000,
    /* 205 0x */  0x0000,
    /* 206 0x */  0x0000,
    /* 207 0x */  0x0000,
    /* 208 0x */  0x0000,
    /* 209 0x */  0x0000,
    /* 210 0x */  0x0000,
    /* 211 0x */  0x0000,
    /* 212 0x */  0x0000,
    /* 213 0x */  0x0000,
    /* 214 0x */  0x0000,
    /* 215 0x */  0x0000,
    /* 216 0x */  0x0000,
    /* 217 0x */  0x0000,
    /* 218 0x */  0x0000,
    /* 219 0x */  0x0000,
    /* 220 0x */  0x0000,
    /* 221 0x */  0x0000,
    /* 222 0x */  0x0000,
    /* 223 0x */  0x0000,
    /* 224 0x */  0x0000,
    /* 225 0x */  0x0000,
    /* 226 0x */  0x0000,
    /* 227 0x */  0x0000,
    /* 228 0x */  0x0000,
    /* 229 0x */  0x0000,
    /* 230 0x */  0x0000,
    /* 231 0x */  0x0000,
    /* 232 0x */  0x0000,
    /* 233 0x */  0x0000,
    /* 234 0x */  0x0000,
    /* 235 0x */  0x0000,
    /* 236 0x */  0x0000,
    /* 237 0x */  0x0000,
    /* 238 0x */  0x0000,
    /* 239 0x */  0x0000,
    /* 240 0x */  0x0000,
    /* 241 0x */  0x0000,
    /* 242 0x */  0x0000,
    /* 243 0x */  0x0000,
    /* 244 0x */  0x0000,
    /* 245 0x */  0x0000,
    /* 246 0x */  0x0000,
    /* 247 0x */  0x0000,
    /* 248 0x */  0x0000,
    /* 249 0x */  0x0000,
    /* 250 0x */  0x0000,
    /* 251 0x */  0x0000,
    /* 252 0x */  0x0000,
    /* 253 0x */  0x0000,
    /* 254 0x */  0x0000,
    /* 255 0x */  0x0000
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
struct win32_window_dimension Get_Window_Dimensions(HWND Window);
void Update_Window_Display(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Convert_320x200xVGA_To_320x200xXBGR(uint8_t * p_320x200xVGA, uint32_t* p_320x200xXBGR);
void Init_Platform(HINSTANCE hInstance, int nCmdShow);
void Init_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height);
void WndInit(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndEvnt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// WZD s35p21
/*
        case WM_MOUSEMOVE:
        {
            OutputDebugStringA("WM_MOUSEMOVE\n");
            POINT ptMouse;
            GetCursorPos(&ptMouse);
            ScreenToClient(g_Window, &ptMouse);
            // DELETE  Update_Mouse_Position((int16_t)ptMouse.x, (int16_t)ptMouse.y);
            User_Mouse_Handler(0b00000000, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
        } break;
*/
void MWA_Set_Mouse_Position(int16_t x, int16_t y)
{
    POINT point = { 0, 0 };
    point.x = (x * 2);
    point.y = (y * 2);
    ClientToScreen(g_Window, &point);
    SetCursorPos(point.x, point.y);
}


void Pump_Events(void)
{
    Pump_Events_Cnt++;

    MSG Message;

    while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
    {
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: Message.message: %d\n", __FILE__, __LINE__, Message.message);
// #endif
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

struct win32_window_dimension Get_Window_Dimensions(HWND Window)
{
    win32_window_dimension WndDim = { 0, 0 };
    RECT ClientAreaRect = { 0, 0, 0, 0 };

    GetClientRect(Window, &ClientAreaRect);

    WndDim.Width = ClientAreaRect.right - ClientAreaRect.left;
    WndDim.Height = ClientAreaRect.bottom - ClientAreaRect.top;
    
    return WndDim;
}

void Update_Window_Display(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{

    // Convert_320x200xVGA_To_320x200xXBGR(p_320x200xVGA, p_320x200xXBGR);
    uint8_t * p_320x200xVGA;
    uint32_t * p_320x200xXBGR;
    uint32_t * p_XBGR;

    p_320x200xVGA = video_page_buffer[draw_page_num];
    p_320x200xXBGR = (uint32_t*)Buffer->Memory;
    p_XBGR = (uint32_t*)platform_palette_buffer;  // ~== IBM-PC VGA-DAC

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
            *(p_320x200xXBGR + (itr_height * width) + itr_width) = color;  // ¿ ~== SDL_SetPaletteColors(sdl2_surface_RGB666->format->palette, platform_palette_buffer, 0, 256); ?
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
    unsigned int * p_XBGR = 0;
    int itr = 0;
    int width = 0;
    int height = 0;
    int itr_width = 0;
    int itr_height = 0;
    uint8_t color_map_index = 0;
    uint32_t color = 0;

    p_XBGR = (uint32_t*)platform_palette_buffer;  // ~== IBM-PC VGA-DAC

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

    // TODO  Exit_With_Message = MWA_Exit_With_Message;


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
    // Buffer->Memory = VirtualAlloc(nullptr, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    Buffer->Memory = VirtualAlloc(NULL, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if(Buffer->Memory == NULL)
    {
        // ErrorExit(TEXT("VirtualAlloc"));
    }
    

    // memset(Buffer->Memory, 1, BitmapMemorySize);
    // Severity	Code	Description	Project	File	Line	Suppression State
    // Warning	C6387	'Buffer->Memory' could be '0':  this does not adhere to the specification for the function 'memset'.ReMoM	C : \STU\devel\ReMoM\src\win_PFL.cpp	533
    if(Buffer->Memory == 0)
    {
        abort();
    }
    else
    {
        memset(Buffer->Memory, 1, BitmapMemorySize);
    }

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

    // ShowWindow(g_Window, nCmdShow);
    // Severity	Code	Description	Project	File	Line	Suppression State
    // Warning	C6387	'g_Window' could be '0':  this does not adhere to the specification for the function 'ShowWindow'.ReMoM	C : \STU\devel\ReMoM\src\win_PFL.cpp	572
    if (g_Window == 0)
    {
        abort();
    }
    else
    {
        ShowWindow(g_Window, nCmdShow);
    }
    
    // UpdateWindow(g_Window);
    // Severity	Code	Description	Project	File	Line	Suppression State
    // Warning	C6387	'g_Window' could be '0':  this does not adhere to the specification for the function 'UpdateWindow'.See line 572 for an earlier location where this can occur	ReMoM	C : \STU\devel\ReMoM\src\win_PFL.cpp	573
    if (g_Window == 0)
    {
        abort();
    }
    else
    {
        UpdateWindow(g_Window);
    }

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
            // DELETE  Update_Mouse_Position((int16_t)ptMouse.x, (int16_t)ptMouse.y);
            User_Mouse_Handler(0 /*0b00000000*/, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
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

            // DELETEME  if(VKCode == VK_ESCAPE && _global_esc == ST_TRUE)
            // DELETEME  {
            // DELETEME      PostQuitMessage(0);
            // DELETEME  }
            if (key_ctrl && key_alt && VKCode == 'Q')
            {
                PostQuitMessage(0);
            }

            key_pressed = ST_TRUE;
            scan_code_char_code = VK_to_SCCS[VKCode];

            if (VKCode == VK_RETURN)
            {
                // MOX_DBG_BREAK;
            }


        } break;


        case WM_LBUTTONDOWN:
        {
            OutputDebugStringA("WM_LBUTTONDOWN\n");
            POINT ptMouse = { 0, 0 };
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);
            User_Mouse_Handler(1 /*0b00000001*/, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
        } break;
        case WM_RBUTTONDOWN:
        {
            OutputDebugStringA("WM_RBUTTONDOWN\n");
            // @Simbey
            // This is that Microsoft C++ Guidelines thing to which I was referring. ~ 20240915
            // Severity	Code	Description	Project	File	Line	Suppression State
            // Message	lnt - uninitialized - local	Local variable is not initialized.ReMoM	C : \STU\devel\ReMoM\src\win_PFL.cpp	670
            POINT ptMouse = { 0, 0 };
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);
            User_Mouse_Handler(2 /*0b00000010*/, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
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
