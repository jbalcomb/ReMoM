#include <windows.h>
#include "MoX.H"
#include "PoC_PFL.H"
#include "PoC_MsWin.hpp"
#include <timeapi.h>    /* timeBeginPeriod() */



#define POC_MODE 5
// #define POC_MODE_1
// #define POC_MODE_2
// #define POC_MODE_3
// #define POC_MODE_4
// #define POC_MODE_5

int Pump_Events_Cnt = 0;
int Pump_Paints_Cnt = 0;



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



const WCHAR szWindowClass[] = L"PoC_MoM_Class";
const WCHAR szTitle[] = L"PoC - MoM";
HINSTANCE g_Instance;
HWND g_Window;
HDC g_DeviceContext;

static int64_t GlobalPerfCountFrequency;  // HMH Day 18


struct win32_offscreen_buffer PlatformVideoBackBuffer;  // Platform - Video Back Buffer
void * convert_320x200xVGA_320x200xXBGR;
void * convert_320x200xXBGR_640x400xXBGR;



inline LARGE_INTEGER Win32GetWallClock(void);
inline float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End);

void Pump_Events(void);
void Pump_Paints(void);
struct win32_window_dimension Get_Window_Dimensions(HWND Window);
// void Resize_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height);
void Update_Window_Display(struct win32_offscreen_buffer* Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Update_Window_Display_PreConvert(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Update_Window_Display_PreConvert_2x(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Update_Window_Display_2x(struct win32_offscreen_buffer* Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Update_Window_Display_2x_XBGR(struct win32_offscreen_buffer* Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Convert_320x200xVGA_To_320x200xXBGR(uint8_t * p_DrawPage, uint32_t* p_Buffer);
void Convert_320x200xXBGR_To_640x400xXBGR(uint32_t* p_320x200xXBGR, uint32_t* p_640x400xXBGR);
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);
void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status);

void Init_Platform(HINSTANCE hInstance, int nCmdShow);
void Init_Video_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height);
void Init_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height);
void WndInit(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndEvnt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



// HMH Day 18
inline LARGE_INTEGER Win32GetWallClock(void)
{
    LARGE_INTEGER microseconds;
    QueryPerformanceCounter(&microseconds);
    return microseconds;
}

// HMH Day 18
inline float Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    float Result = (float)(End.QuadPart - Start.QuadPart) / (float)GlobalPerfCountFrequency;
    return Result;
}



void Pump_Events(void)
{
    Pump_Events_Cnt++;
    MSG Message;
    while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
    {
        if (Message.message == WM_QUIT)
        {
            g_State_Run = false;
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
    win32_window_dimension WndDim;
    
    RECT ClientAreaRect;
    GetClientRect(Window, &ClientAreaRect);
    WndDim.Width = ClientAreaRect.right - ClientAreaRect.left;
    WndDim.Height = ClientAreaRect.bottom - ClientAreaRect.top;
    
    return WndDim;
}

// void Resize_Window_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height)
// {
//     int BitmapMemorySize;
//     if(Buffer->Memory != NULL)
//     {
//         VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
//     }
//     Buffer->Width = Width;
//     Buffer->Height = Height;
//     Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;
//     Buffer->Info.bmiHeader.biWidth = Buffer->Width;
//     Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // negative value: top-down
//     BitmapMemorySize = Buffer->BytesPerPixel * (Buffer->Width * Buffer->Height);
//     Buffer->Memory = VirtualAlloc(nullptr, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
//     if(Buffer->Memory == NULL)
//     {
//         // ErrorExit(TEXT("VirtualAlloc"));
//     }
// }


// ONLY convert pixels / color components
// assumes Buffer is *inhereted* from video_page_buffer
void Update_Window_Display(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    uint32_t* p_Buffer;
    unsigned int* p_XBGR;
    int itr;
    uint8_t * p_DrawPage;

    p_DrawPage = video_page_buffer[draw_page_num];
    p_Buffer = (uint32_t*)Buffer->Memory;
    p_XBGR = (uint32_t*)g_Palette_XBGR;

    for (itr = 0; itr < screen_pixel_size; itr++)
    {
        // *p_Buffer++ = *(p_XBGR + *(video_page_buffer[draw_page_num] + itr));
        *p_Buffer++ = *(p_XBGR + *(p_DrawPage + itr));
    }

    StretchDIBits(DeviceContext, 
                  0, 0, WindowWidth, WindowHeight,      /* destination rectangle (window)   */
                  0, 0, Buffer->Width, Buffer->Height,  /* source rectangle (bitmap buffer) */
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
    
}

void Update_Window_Display_PreConvert(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    uint8_t * p_DrawPage;
    uint32_t* p_Buffer;
    unsigned int* p_XBGR;
    int itr;
    int width;
    int height;
    int itr_width;
    int itr_height;
    uint8_t color_map_index;
    uint32_t color;
    uint32_t* p_BufferMemory;

    p_DrawPage = video_page_buffer[draw_page_num];
    p_Buffer = (uint32_t*)convert_320x200xVGA_320x200xXBGR;
    // uint8_t * p_320x200xVGA, uint32_t* p_320x200xXBGR
    Convert_320x200xVGA_To_320x200xXBGR(p_DrawPage, p_Buffer);

    p_BufferMemory  = (uint32_t*)Buffer->Memory;
    width = 320;
    height = 200;
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(p_BufferMemory + (itr_height * width) + itr_width) = *(p_Buffer + (itr_height * width) + itr_width);
        }
    }

    StretchDIBits(DeviceContext, 0, 0, width, height, 0, 0, width, height, Buffer->Memory, &Buffer->Info, DIB_RGB_COLORS, SRCCOPY);
    
}


void Update_Window_Display_PreConvert_2x(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    uint8_t * p_DrawPage;
    uint32_t* p_Buffer;
    unsigned int* p_XBGR;
    int itr;
    int width;
    int height;
    int itr_width;
    int itr_height;
    uint8_t color_map_index;
    uint32_t color;
    uint32_t* p_BufferMemory;
    uint8_t* p_320x200xVGA;
    uint32_t* p_320x200xXBGR;
    uint32_t* p_640x400xXBGR;

    p_320x200xVGA = video_page_buffer[draw_page_num];

    p_320x200xXBGR = (uint32_t*)convert_320x200xVGA_320x200xXBGR;

    p_640x400xXBGR = (uint32_t*)convert_320x200xXBGR_640x400xXBGR;


    Convert_320x200xVGA_To_320x200xXBGR(p_320x200xVGA, p_320x200xXBGR);


    Convert_320x200xXBGR_To_640x400xXBGR(p_320x200xXBGR, p_640x400xXBGR);


    p_Buffer = p_640x400xXBGR;
    p_BufferMemory  = (uint32_t*)Buffer->Memory;
    width = 640;
    height = 400;
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(p_BufferMemory + (itr_height * width) + itr_width) = *(p_Buffer + (itr_height * width) + itr_width);
        }
    }

    StretchDIBits(DeviceContext, 0, 0, width, height, 0, 0, width, height, Buffer->Memory, &Buffer->Info, DIB_RGB_COLORS, SRCCOPY);
    
}


void Update_Window_Display_2x(struct win32_offscreen_buffer* Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    uint32_t* p_BufferMemory;
    unsigned int* p_XBGR;
    int itr;
    int width;
    int height;
    int itr_width;
    int itr_height;
    uint8_t * p_DrawPage;

    p_DrawPage = video_page_buffer_2x[draw_page_num];
    p_XBGR = (uint32_t*)g_Palette_XBGR;
    p_BufferMemory  = (uint32_t*)Buffer->Memory;

    width = 640;
    height = 400;

    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            *(p_BufferMemory + (itr_height * width) + itr_width) = *(p_XBGR + *(p_DrawPage + (itr_height * width) + itr_width));
        }
    }

    StretchDIBits(DeviceContext, 0, 0, width, height, 0, 0, width, height, Buffer->Memory, &Buffer->Info, DIB_RGB_COLORS, SRCCOPY);

}

void Update_Window_Display_2x_XBGR(struct win32_offscreen_buffer* Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    int width;
    int height;
    width = 640;
    height = 400;

            // *(p_BufferMemory + (itr_height * width) + itr_width) = *(p_XBGR + *(p_DrawPage + (itr_height * width) + itr_width));
    
    StretchDIBits(DeviceContext, 0, 0, width, height, 0, 0, width, height, Buffer->Memory, &Buffer->Info, DIB_RGB_COLORS, SRCCOPY);

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

    p_XBGR = (uint32_t*)g_Palette_XBGR;

    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;
    for(itr_height = 0; itr_height < height; itr_height++)
    {
        for(itr_width = 0; itr_width < width; itr_width++)
        {
            color_map_index = *(p_320x200xVGA + (itr_height * SCREEN_WIDTH) + itr_width);
            color = p_XBGR[color_map_index];
            *(p_320x200xXBGR + (itr_height * SCREEN_WIDTH) + itr_width) = color;
        }
    }

}
// loops arranged in the right order
// clever use of fixed-point arithmetic
// on Intel x86/x64, floating point might be faster than fixed point
// replace the multiplications in the loops with additions ("can be trivially replaced")
// ¿ "do the last output - input equation first to try some prefetch optimzation" ?
// ¿ "combine the four 8-bit copy-assignments into one 32-bit, assuming pointers are 32bit aligned" ?
// ¿ "restrict keyword will help a lot, assuming no aliasing" ?
// ... CPU-specific optimizations, parallelize, use GPU, use SIMD ...
void Convert_320x200xXBGR_To_640x400xXBGR(uint32_t* p_320x200xXBGR, uint32_t* p_640x400xXBGR)
{
    int src_width;
    int src_height;
    int dst_width;
    int dst_height;
    int x_ratio;
    int y_ratio;
    int itr_dx;
    int itr_dy;

int y2_xsource;
int i_xdest;
int x2;
int y2_x2_colors;
int i_x_colors;
const int colors = 4;  // color components / bytes per pixel

    // uint32_t* src;
    // uint32_t* dst;
    uint8_t* src;
    uint8_t* dst;

    src_width = 320;
    src_height = 200;
    dst_width = 640;
    dst_height = 400;

    x_ratio = (int)((src_width  << 16) / dst_width);   // 320 / 640 = 0.5
    y_ratio = (int)((src_height << 16) / dst_height);  // 200 / 400 = 0.5

    src = (uint8_t *)p_320x200xXBGR;
    dst = (uint8_t *)p_640x400xXBGR;

    for(itr_dy = 0; itr_dy < dst_height; itr_dy++)
    {
        y2_xsource = ((itr_dy * y_ratio) >> 16) * src_width;  // e.g., itr_dy = 4  * 0.5 = 2  * 320 = 640
        i_xdest = itr_dy * dst_width;                         // e.g., itr_dy = 4  * 640 = 2560

        for(itr_dx = 0; itr_dx < dst_width; itr_dx++)
        {
            x2 = ((itr_dx * x_ratio) >> 16);
            y2_x2_colors = (y2_xsource + x2) * colors;
            i_x_colors = (i_xdest + itr_dx) * colors;

            dst[i_x_colors + 0] = src[y2_x2_colors + 0];
            dst[i_x_colors + 1] = src[y2_x2_colors + 1];
            dst[i_x_colors + 2] = src[y2_x2_colors + 2];
            dst[i_x_colors + 3] = src[y2_x2_colors + 3];
        }
    }

}

void resizeNearestNeighbor(const uint8_t* input, uint8_t* output, int sourceWidth, int sourceHeight, int targetWidth, int targetHeight)
{
    const int x_ratio = (int)((sourceWidth << 16) / targetWidth);
    const int y_ratio = (int)((sourceHeight << 16) / targetHeight) ;
    const int colors = 4;

    for (int y = 0; y < targetHeight; y++)
    {
        int y2_xsource = ((y * y_ratio) >> 16) * sourceWidth;
        int i_xdest = y * targetWidth;

        for (int x = 0; x < targetWidth; x++)
        {
            int x2 = ((x * x_ratio) >> 16) ;
            int y2_x2_colors = (y2_xsource + x2) * colors;
            int i_x_colors = (i_xdest + x) * colors;

            output[i_x_colors]     = input[y2_x2_colors];
            output[i_x_colors + 1] = input[y2_x2_colors + 1];
            output[i_x_colors + 2] = input[y2_x2_colors + 2];
            output[i_x_colors + 3] = input[y2_x2_colors + 3];
        }
    }
}

// void ResizeImageNearest(const T *src, int srcWidth, int srcHeight, T *dst, int dstWidth, int dstHeight, int numComponents) {
//     int srcPitch = srcWidth * numComponents;
// 
//     float ratioX = (float)srcWidth / dstWidth;
//     float ratioY = (float)srcHeight / dstHeight;
// 
//     for (int y = 0; y < dstHeight; y++) {
//         int iY = y * ratioY;
// 
//         int offsetY = iY * srcPitch;
// 
//         for (int x = 0; x < dstWidth; x++) {
//             int iX = x * ratioX;
// 
//             int offsetX = iX * numComponents;
//         
//             const T *srcPtrY = &src[offsetY];
// 
//             for (int i = 0; i < numComponents; i++) {
//                 *dst++ = srcPtrY[offsetX + i];
//             }
//         }
//     }
// }


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



void Init_Platform(HINSTANCE hInstance, int nCmdShow)
{

    switch(POC_MODE)
    {
        case 1:
        {
            Init_Window_Back_Buffer(&PlatformVideoBackBuffer, 320, 200);
        } break;
        case 2:
        {
            Init_Window_Back_Buffer(&PlatformVideoBackBuffer, 320, 200);
            convert_320x200xVGA_320x200xXBGR = VirtualAlloc(nullptr, (320*200*4), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        } break;
        case 3:
        {
            Init_Window_Back_Buffer(&PlatformVideoBackBuffer, 640, 400);
            convert_320x200xVGA_320x200xXBGR = VirtualAlloc(nullptr, (320*200*4), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            convert_320x200xXBGR_640x400xXBGR = VirtualAlloc(nullptr, (640*400*4), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        } break;
        case 4:
        {
            Init_Video_Back_Buffer(&PlatformVideoBackBuffer, 640, 400);
        } break;
        case 5:
        {
            Init_Video_Back_Buffer(&PlatformVideoBackBuffer, 640, 400);
        } break;
        
    }

    WndInit(hInstance, nCmdShow);



    // Getting the performance frequency.
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    // NOTE: Set the Windows scheduler granularity to 1ms
    // so that our Sleep() can be more granular
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



// ¿ this one only makes sense when MoX can produce a MS-DIB compatible buffer ?
void Init_Video_Back_Buffer(struct win32_offscreen_buffer * Buffer, int Width, int Height)
{
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

    Buffer->Memory = video_page_buffer_2x_XBGR[draw_page_num];
}

// ¿ this one makes sense for when the platform layer is handling the conversion from *notive* MoX code ?
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
}



void WndInit(HINSTANCE hInstance, int nCmdShow)
{
    g_Instance = hInstance;
    WNDCLASSEXW wcex = { 0 };
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
    RegisterClassExW(&wcex);
    RECT   rect = {0, 0, window_pixel_width, window_pixel_height};
    DWORD  dwStyle = WS_OVERLAPPEDWINDOW;
    BOOL   bMenu = false;
    DWORD  dwExStyle = NULL;
    AdjustWindowRectEx(&rect, dwStyle, bMenu, dwExStyle);

    g_Window = CreateWindowExW(NULL, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
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

            switch(POC_MODE)
            {
                case 1:
                {
                    Update_Window_Display(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);
                } break;
                case 2:
                {
                    Update_Window_Display_PreConvert(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);
                } break;
                case 3:
                {
                    Update_Window_Display_PreConvert_2x(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);
                } break;
                case 4:
                {
                    Update_Window_Display_2x(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);
                } break;
                case 5:
                {
                    Update_Window_Display_2x_XBGR(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);
                } break;
                
            }

            EndPaint(hWnd, &ps);
        } break;

        case WM_KEYDOWN:
        {
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

        // case WM_ACTIVATEAPP:
        // {
        //     OutputDebugStringA("WM_ACTIVATEAPP\n");
        // } break;

        case WM_CREATE:
        {
            ShowCursor(FALSE);
            // struct win32_window_dimension WndDim = Get_Window_Dimensions(hWnd);
            // Init_Window_Back_Buffer(&PlatformVideoBackBuffer, WndDim.Width, WndDim.Height);
        } break;

        case WM_CLOSE:
        {
            quit_game_flag = ST_TRUE;
            current_screen = scr_Quit_To_DOS;
            g_State_Run = false;
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;



        default:
            return DefWindowProc(hWnd, message, wParam, lParam);

    }

    return 0;
}
