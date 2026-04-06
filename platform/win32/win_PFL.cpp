/**
 * win_PFL.cpp — Win32 platform implementation.
 *
 * Implements Platform.h functions using the Win32 API.
 * No engine headers are included — only Platform.h and win_PFL.h.
 */

#include "../../STU/src/STU_DBG.h"

#include <Windows.h>
#include "windowsx.h"   /* GET_X_LPARAM(), GET_Y_LPARAM() */
#include <timeapi.h>    /* timeBeginPeriod() */

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"
#include "win_PFL.h"



/* ========================================================================= */
/*  Win32 Backend State                                                      */
/* ========================================================================= */

HINSTANCE win_instance;
HWND win_window;
HDC win_device_context;

int win_window_width = PLATFORM_WINDOW_WIDTH;
int win_window_height = PLATFORM_WINDOW_HEIGHT;

uint64_t win_ticks_startup;

struct win32_offscreen_buffer win_video_back_buffer;

/* Optional per-frame callback for synthetic input injection (HeMoM). */
static void (*platform_frame_callback)(void) = NULL;

void Platform_Register_Frame_Callback(void (*callback)(void))
{
    platform_frame_callback = callback;
}

static const CHAR win_window_class_name[] = "MoM_Wnd_Cls";
static const CHAR win_window_title[] = "(ReMoM) Master of Magic v1.31 - Win32";



/* ========================================================================= */
/*  Forward Declarations                                                     */
/* ========================================================================= */

static void Win_Init_Back_Buffer(struct win32_offscreen_buffer *buffer, int width, int height);
static void Win_Init_Window(void);
static LRESULT CALLBACK Win_Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void Win_Convert_Engine_Pixels_To_Back_Buffer(struct win32_offscreen_buffer *buffer);
static void Win_Blit_Back_Buffer(struct win32_offscreen_buffer *buffer, HDC device_context, int window_width, int window_height);



/* ========================================================================= */
/*  Lifecycle                                                                */
/* ========================================================================= */

void Startup_Platform(void)
{
    int w = win_window_width;
    int h = win_window_height;

#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: BEGIN: Startup_Platform()\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Startup_Platform()\n", __FILE__, __LINE__);
#endif

    assert(w >= PLATFORM_SCREEN_WIDTH && "window width must be >= PLATFORM_SCREEN_WIDTH");
    assert(h >= PLATFORM_SCREEN_HEIGHT && "window height must be >= PLATFORM_SCREEN_HEIGHT");

    win_ticks_startup = (uint64_t)GetTickCount64();

    Win_Init_Back_Buffer(&win_video_back_buffer, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT);

    Win_Init_Window();

    /* Set Windows scheduler granularity to 1ms for better Sleep() precision */
    timeBeginPeriod(1);

    Build_Key_Xlat();

#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: END: Startup_Platform()\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: END: Startup_Platform()\n", __FILE__, __LINE__);
#endif
}

void Shutdown_Platform(void)
{
#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: BEGIN: Shutdown_Platform()\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Shutdown_Platform()\n", __FILE__, __LINE__);
#endif
    ShowCursor(TRUE);
    timeEndPeriod(1);
    if (win_window)
    {
        DestroyWindow(win_window);
        win_window = NULL;
    }
#ifdef STU_DEBUG
    printf("DEBUG: [%s, %d]: END: Shutdown_Platform()\n", __FILE__, __LINE__);
    dbg_prn("DEBUG: [%s, %d]: END: Shutdown_Platform()\n", __FILE__, __LINE__);
#endif
}

int Platform_Get_Scale(void)
{
    if (win_window_width <= 0)
    {
        return 1;
    }
    return win_window_width / PLATFORM_SCREEN_WIDTH;
}



/* ========================================================================= */
/*  Video                                                                    */
/* ========================================================================= */

void Platform_Palette_Update(void)
{
    int itr;

    for (itr = 0; itr < 256; itr++)
    {
        platform_palette_buffer[itr].r = (*(current_palette + (itr * 3) + 2) << 2);
        platform_palette_buffer[itr].g = (*(current_palette + (itr * 3) + 1) << 2);
        platform_palette_buffer[itr].b = (*(current_palette + (itr * 3) + 0) << 2);
        platform_palette_buffer[itr].a = 0xFF;
    }
}

void Platform_Get_Palette_Color(uint8_t index, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = platform_palette_buffer[index].r;
    *g = platform_palette_buffer[index].g;
    *b = platform_palette_buffer[index].b;
}

void Platform_Set_Palette_Color(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    platform_palette_buffer[index].r = r;
    platform_palette_buffer[index].g = g;
    platform_palette_buffer[index].b = b;
    platform_palette_buffer[index].a = 255;
}

/**
 * Convert the engine's 8-bit framebuffer to 32-bit XBGR pixels in the platform
 * back buffer, applying the current palette.  Touches engine state.
 * Called by Platform_Video_Update() — only safe after Allocate_Data_Space().
 */
static void Win_Convert_Engine_Pixels_To_Back_Buffer(struct win32_offscreen_buffer *buffer)
{
    uint8_t *source_pixels;
    uint32_t *dest_pixels;
    uint32_t *palette_xbgr;
    int width;
    int height;
    int itr_x;
    int itr_y;
    uint8_t color_index;
    uint32_t color;

    source_pixels = video_page_buffer[draw_page_num];
    dest_pixels = (uint32_t *)buffer->Memory;
    palette_xbgr = (uint32_t *)platform_palette_buffer;  /* PFL_Color is RGBA, Win32 DIB is XBGR — layout-compatible for StretchDIBits */

    width = screen_pixel_width;
    height = screen_pixel_height;

    for (itr_y = 0; itr_y < height; itr_y++)
    {
        for (itr_x = 0; itr_x < width; itr_x++)
        {
            color_index = *(source_pixels + (itr_y * width) + itr_x);
            color = palette_xbgr[color_index];
            *(dest_pixels + (itr_y * width) + itr_x) = color;
        }
    }
}

/**
 * Blit the platform's back buffer to the window.  Touches no engine state.
 * Safe to call any time after Win_Init_Back_Buffer() — including from WM_PAINT
 * before the engine has initialized.
 */
static void Win_Blit_Back_Buffer(struct win32_offscreen_buffer *buffer, HDC device_context, int window_width, int window_height)
{
    StretchDIBits(device_context, 0, 0, window_width, window_height, 0, 0, buffer->Width, buffer->Height, buffer->Memory, &buffer->Info, DIB_RGB_COLORS, SRCCOPY);
}

void Platform_Video_Update(void)
{
    assert(video_page_buffer[draw_page_num] != NULL);
    assert(win_video_back_buffer.Memory != NULL);

    Win_Convert_Engine_Pixels_To_Back_Buffer(&win_video_back_buffer);
    Win_Blit_Back_Buffer(&win_video_back_buffer, win_device_context, win_window_width, win_window_height);
}



/* ========================================================================= */
/*  Events / Input                                                           */
/* ========================================================================= */

void Platform_Event_Handler(void)
{
#ifdef MOUSE_DEBUG
    MOUSE_LOG("MOUSEt=%llu HANDLER_START ptr=%d,%d\n", (unsigned long long)Platform_Get_Millies(), pointer_x, pointer_y);
#endif
    platform_frame_mouse_buttons = 0;

    Win_Pump_Messages();

    /* Synthetic player: inject scripted input. */
    if (platform_frame_callback != NULL)
    {
        platform_frame_callback();
    }
}

/* CLAUDE: Pump events AND refresh cursor, matching SDL backends. */
void Platform_Pump_Events(void)
{
    Win_Pump_Messages();
    Platform_Maybe_Move_Mouse();
}

void Win_Pump_Messages(void)
{
    MSG message;

    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT)
        {
            quit_game_flag = ST_TRUE;
        }
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void Platform_Mouse_Input_Enable(void)
{
    platform_mouse_input_enabled = ST_TRUE;
}

void Platform_Mouse_Input_Disable(void)
{
    platform_mouse_input_enabled = ST_FALSE;
}



/* ========================================================================= */
/*  Legacy Win32 API (declared in Platform.h under #ifdef _STU_WIN)          */
/* ========================================================================= */

void Pump_Events(void)
{
    Win_Pump_Messages();
}

void Pump_Paints(void)
{
    InvalidateRect(win_window, NULL, FALSE);
    MSG message;
    while (PeekMessageA(&message, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}



/* ========================================================================= */
/*  Mouse                                                                    */
/* ========================================================================= */

int16_t Platform_Get_Mouse_Button_State(void)
{
    int16_t buttons = 0;
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { buttons |= ST_LEFT_BUTTON; }
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { buttons |= ST_RIGHT_BUTTON; }
    return buttons;
}

void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    int scale = Platform_Get_Scale();
    POINT point;
    point.x = (LONG)(game_x * scale);
    point.y = (LONG)(game_y * scale);
    ClientToScreen(win_window, &point);
    SetCursorPos(point.x, point.y);
}

void MWA_Set_Mouse_Position(int16_t x, int16_t y)
{
    Platform_Warp_Mouse(x, y);
}

int Platform_Get_Window_Width(void)
{
    RECT client_rect;
    GetClientRect(win_window, &client_rect);
    return client_rect.right - client_rect.left;
}

void Platform_Get_Mouse_Position_XY(int *mx, int *my)
{
    int scale = Platform_Get_Scale();
    POINT cursor;
    GetCursorPos(&cursor);
    ScreenToClient(win_window, &cursor);

    if (scale < 1) { scale = 1; }
    *mx = (int)(cursor.x / scale);
    *my = (int)(cursor.y / scale);

    SETRANGE(*mx, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
    SETRANGE(*my, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
}

void Platform_Maybe_Move_Mouse(void)
{
    int mx, my;
    int16_t smx, smy;

    if (platform_mouse_input_enabled == ST_FALSE) { return; }

    Platform_Get_Mouse_Position_XY(&mx, &my);
    smx = (int16_t)mx;
    smy = (int16_t)my;

    if (smx != pointer_x || smy != pointer_y)
    {
#ifdef MOUSE_DEBUG
        MOUSE_LOG("MOUSEt=%llu MAYBE_MOVE ptr=%d,%d -> %d,%d\n", (unsigned long long)Platform_Get_Millies(), pointer_x, pointer_y, smx, smy);
#endif
        if (mouse_enabled == ST_TRUE)
        {
            Restore_Mouse_On_Page();
        }
        pointer_x = smx;
        pointer_y = smy;
        if (mouse_enabled == ST_TRUE)
        {
            Check_Mouse_Shape(smx, smy);
            Save_Mouse_On_Page(smx, smy);
            Draw_Mouse_On_Page(smx, smy);
        }
    }
}

void Hw_Textinput_Start(void)
{
    /* Win32: no IME integration needed for this game */
}

void Hw_Textinput_Stop(void)
{
    /* Win32: no IME integration needed for this game */
}



/* ========================================================================= */
/*  Audio (stub — Win32 backend does not implement audio)                    */
/* ========================================================================= */

int16_t Platform_Audio_Play_Sound(void *sound_buffer, uint32_t sound_buffer_size)
{
    (void)sound_buffer;
    (void)sound_buffer_size;
    return -1;
}



/* ========================================================================= */
/*  Window Initialization                                                    */
/* ========================================================================= */

static void Win_Init_Back_Buffer(struct win32_offscreen_buffer *buffer, int width, int height)
{
    int bitmap_memory_size;

    if (buffer->Memory != NULL)
    {
        VirtualFree(buffer->Memory, 0, MEM_RELEASE);
    }

    buffer->BytesPerPixel = 4;
    buffer->Info.bmiHeader.biSize = sizeof(buffer->Info.bmiHeader);
    buffer->Info.bmiHeader.biPlanes = 1;
    buffer->Info.bmiHeader.biBitCount = 32;
    buffer->Info.bmiHeader.biCompression = BI_RGB;
    buffer->Width = width;
    buffer->Height = height;
    buffer->Pitch = buffer->Width * buffer->BytesPerPixel;
    buffer->Info.bmiHeader.biWidth = buffer->Width;
    buffer->Info.bmiHeader.biHeight = -buffer->Height;  /* negative value: top-down */

    bitmap_memory_size = buffer->BytesPerPixel * (buffer->Width * buffer->Height);
    buffer->Memory = VirtualAlloc(NULL, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    assert(buffer->Memory != NULL && "VirtualAlloc failed for video back buffer");

    memset(buffer->Memory, 0, bitmap_memory_size);
}

static void Win_Init_Window(void)
{
    WNDCLASSEX wcex = { 0 };
    RECT rect;
    DWORD style;

    win_instance = GetModuleHandle(NULL);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Win_Window_Proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = win_instance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = win_window_class_name;
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    rect.left = 0;
    rect.top = 0;
    rect.right = win_window_width;
    rect.bottom = win_window_height;
    style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRectEx(&rect, style, FALSE, 0);

    win_window = CreateWindowEx(0, win_window_class_name, win_window_title, style, CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left), (rect.bottom - rect.top), NULL, NULL, win_instance, NULL);
    assert(win_window != NULL && "CreateWindowEx failed");

    ShowWindow(win_window, SW_SHOW);
    UpdateWindow(win_window);

    win_device_context = GetDC(win_window);  /* requires CS_OWNDC */
}



/* ========================================================================= */
/*  Window Procedure                                                         */
/* ========================================================================= */

static LRESULT CALLBACK Win_Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT client_rect;
            int client_width;
            int client_height;

            HDC hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &client_rect);
            client_width = client_rect.right - client_rect.left;
            client_height = client_rect.bottom - client_rect.top;
            /* Blit only the platform back buffer.  No engine state touched. */
            Win_Blit_Back_Buffer(&win_video_back_buffer, hdc, client_width, client_height);
            EndPaint(hWnd, &ps);
        } break;


        case WM_MOUSEMOVE:
        {
            if (platform_mouse_input_enabled)
            {
                int scale = Platform_Get_Scale();
                int16_t game_x;
                int16_t game_y;
                if (scale < 1) { scale = 1; }
                game_x = (int16_t)(GET_X_LPARAM(lParam) / scale);
                game_y = (int16_t)(GET_Y_LPARAM(lParam) / scale);
                SETRANGE(game_x, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
                SETRANGE(game_y, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
#ifdef MOUSE_DEBUG
                MOUSE_LOG("MOUSEt=%llu WM_MOUSEMOVE wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)GET_X_LPARAM(lParam), (int)GET_Y_LPARAM(lParam), game_x, game_y);
#endif
                User_Mouse_Handler(0, game_x, game_y);
            }
        } break;


        case WM_KEYDOWN:
        {
            int vk_code = (int)wParam;
            int mox_key = 0;
            uint32_t mox_mod = 0;
            char mox_character = 0;

            if (vk_code >= 0 && vk_code < 256)
            {
                mox_key = VK_to_MOX_KEY[vk_code];
            }

            if (GetKeyState(VK_SHIFT) & 0x8000)   { mox_mod |= MOX_MOD_SHIFT; }
            if (GetKeyState(VK_CONTROL) & 0x8000)  { mox_mod |= MOX_MOD_CTRL; }
            if (GetKeyState(VK_MENU) & 0x8000)     { mox_mod |= MOX_MOD_ALT; }

            /* Ctrl+Alt+Q to quit */
            if ((mox_mod & MOX_MOD_CTRL) && (mox_mod & MOX_MOD_ALT) && vk_code == 'Q')
            {
                PostQuitMessage(0);
            }

            /* Generate character code for printable keys */
            if (vk_code >= 'A' && vk_code <= 'Z')
            {
                mox_character = (char)(vk_code + 32);  /* lowercase */
                if (mox_mod & MOX_MOD_SHIFT) { mox_character = (char)vk_code; }  /* uppercase */
            }
            else if (vk_code >= '0' && vk_code <= '9')
            {
                mox_character = (char)vk_code;
            }
            else if (vk_code == VK_OEM_MINUS || vk_code == VK_SUBTRACT)
            {
                mox_character = '-';
            }
            else if (vk_code == VK_OEM_PLUS || vk_code == VK_ADD)
            {
                mox_character = '+';
                if (vk_code == VK_OEM_PLUS && !(mox_mod & MOX_MOD_SHIFT)) { mox_character = '='; }
            }
            else if (vk_code == VK_SPACE)
            {
                mox_character = ' ';
            }
            else if (vk_code == VK_RETURN)
            {
                mox_character = '\r';
            }
            else if (vk_code == VK_BACK)
            {
                mox_character = '\b';
            }
            else if (vk_code == VK_ESCAPE)
            {
                mox_character = 0x1B;
            }
            else if (vk_code == VK_TAB)
            {
                mox_character = '\t';
            }

            /* Also set legacy scan_code_char_code for backwards compatibility */
            if (vk_code >= 0 && vk_code < 256)
            {
                /* CLAUDE */  /* Legacy: populate scan_code_char_code from the old VK_to_SCCS table pattern */
                /* The packed_key / keyboard buffer system is the primary path now */
            }

            if (mox_key != MOX_KEY_UNKNOWN && mox_key < MOX_KEY_OVERRUN)
            {
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            }
        } break;


        case WM_LBUTTONDOWN:
        {
            if (platform_mouse_input_enabled)
            {
                int scale = Platform_Get_Scale();
                int16_t game_x;
                int16_t game_y;
                if (scale < 1) { scale = 1; }
                game_x = (int16_t)(GET_X_LPARAM(lParam) / scale);
                game_y = (int16_t)(GET_Y_LPARAM(lParam) / scale);
                SETRANGE(game_x, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
                SETRANGE(game_y, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
#ifdef MOUSE_DEBUG
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=1 gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), game_x, game_y);
#endif
                platform_frame_mouse_buttons |= 1;
                User_Mouse_Handler(ST_LEFT_BUTTON, game_x, game_y);
            }
        } break;

        case WM_RBUTTONDOWN:
        {
            if (platform_mouse_input_enabled)
            {
                int scale = Platform_Get_Scale();
                int16_t game_x;
                int16_t game_y;
                if (scale < 1) { scale = 1; }
                game_x = (int16_t)(GET_X_LPARAM(lParam) / scale);
                game_y = (int16_t)(GET_Y_LPARAM(lParam) / scale);
                SETRANGE(game_x, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
                SETRANGE(game_y, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
#ifdef MOUSE_DEBUG
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=2 gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), game_x, game_y);
#endif
                platform_frame_mouse_buttons |= 2;
                User_Mouse_Handler(ST_RIGHT_BUTTON, game_x, game_y);
            }
        } break;


        case WM_SIZE:
        {
            RECT client_rect;
            GetClientRect(win_window, &client_rect);
            win_window_width = client_rect.right - client_rect.left;
            win_window_height = client_rect.bottom - client_rect.top;
        } break;

        case WM_ACTIVATEAPP:
        {
            /* no-op */
        } break;

        case WM_CREATE:
        {
            ShowCursor(FALSE);
        } break;

        case WM_CLOSE:
        {
            quit_game_flag = ST_TRUE;
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
