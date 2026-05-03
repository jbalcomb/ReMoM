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
/* CLAUDE: needed for Platform_Record_Active() / Replay_Capture_Frame() used in Platform_Event_Handler. */
#include "../../platform/include/Platform_Replay.h"
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

/**
 * @brief Process one platform input frame for the Win32 backend.
 *
 * Pumps pending Win32 messages, clears the per-frame mouse button state,
 * optionally invokes the registered synthetic-input callback, and then records
 * the resolved input frame when replay recording is active.
 *
 * This function is the main per-frame input entry point for the Win32 platform
 * layer. It updates global platform input state as a side effect and does not
 * return a value.
 *
 * @note The synthetic frame callback is intended for scripted input injection
 * and is executed after OS events are processed.
 *
 * @note Replay capture occurs after message pumping so the recorded frame
 * matches the final input state seen by the engine for that frame.
 */

void Platform_Event_Handler(void)
{
#ifdef MOUSE_DEBUG
    MOUSE_LOG("MOUSEt=%llu HANDLER_START ptr=%d,%d\n", (unsigned long long)Platform_Get_Millies(), pointer_x, pointer_y);
#endif

    /* CLAUDE: do NOT reset platform_frame_mouse_buttons here.  Clicks that arrive during Release_Time's Platform_Pump_Events spin-wait are OR'd into this flag between frames.  Resetting at the top would clobber them before Replay_Capture_Frame could read them.  Reset is now done at the END of this function, after capture. */

    /* CLAUDE: Replay — if active, inject recorded frame instead of pumping OS events.  Matches sdl2/sdl3/headless backends.  Without this, --replay on win32 opens the RMR but never injects any frames. */
    if (Platform_Replay_Active())
    {
        MSG cancel_msg;
        int cancelled = 0;
        if (PeekMessageA(&cancel_msg, 0, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE))
        {
            cancelled = 1;
        }
        else if (PeekMessageA(&cancel_msg, 0, WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_NOREMOVE))
        {
            cancelled = 1;
        }
        else if (PeekMessageA(&cancel_msg, 0, WM_RBUTTONDOWN, WM_RBUTTONDOWN, PM_NOREMOVE))
        {
            cancelled = 1;
        }

        if (cancelled)
        {
            fprintf(stderr, "REPLAY: cancelled by user input\n");
            Platform_Replay_Stop();
            /* Fall through to live input. */
        }
        else
        {
            if (Replay_Inject_Frame())
            {
                /* Frame injected successfully — skip pump/capture this tick. */
                platform_frame_mouse_buttons = 0;
                return;
            }
            /* Replay ended — fall through to live input. */
        }
    }

    Win_Pump_Messages();

    /* Artificial human player: inject scripted input. */
    if (platform_frame_callback != NULL)
    {
        platform_frame_callback();
    }

    /* CLAUDE: Record — capture input state after processing OS events.  Matches sdl2/sdl3/headless backends.  Without this, --record produces an empty RMR on win32. */
    if (Platform_Record_Active())
    {
        Replay_Capture_Frame();
    }

    /* CLAUDE: reset per-frame click edge AFTER capture so clicks arriving during the next spin-wait accumulate into the following frame's edge. */
    platform_frame_mouse_buttons = 0;
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
                /* CLAUDE: User_Mouse_Handler() expects window-space coords and divides by scale internally.  Passing pre-scaled (game-space) coords double-scales and causes the cursor to jump up-left before Platform_Maybe_Move_Mouse() corrects it. */
                int16_t win_x = (int16_t)GET_X_LPARAM(lParam);
                int16_t win_y = (int16_t)GET_Y_LPARAM(lParam);
#ifdef MOUSE_DEBUG
                int scale = Platform_Get_Scale();
                if (scale < 1) { scale = 1; }
                MOUSE_LOG("MOUSEt=%llu WM_MOUSEMOVE wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (int)(win_x / scale), (int)(win_y / scale));
#endif
                User_Mouse_Handler(0, win_x, win_y);
            }
        } break;


        case WM_SYSKEYDOWN:
        {
            /* CLAUDE: diagnostic — some keys (F10, Alt+key, etc.) arrive as WM_SYSKEYDOWN rather than WM_KEYDOWN.  Log them so we can see if missed keys are landing here. */
#ifdef STU_DEBUG
            trc_prn("WIN_KEY t=%llu WM_SYSKEYDOWN vk=0x%02X (%d) wParam=0x%llX lParam=0x%llX repeat=%d\n", (unsigned long long)Platform_Get_Millies(), (int)wParam, (int)wParam, (unsigned long long)wParam, (unsigned long long)lParam, (int)(lParam & 0xFFFF));
#endif
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

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

            if (GetKeyState(VK_SHIFT)   & 0x8000)  { mox_mod |= MOX_MOD_SHIFT; }
            if (GetKeyState(VK_CONTROL) & 0x8000)  { mox_mod |= MOX_MOD_CTRL; }
            if (GetKeyState(VK_MENU)    & 0x8000)  { mox_mod |= MOX_MOD_ALT; }

            /* CLAUDE: diagnostic — trace raw WM_KEYDOWN before translation, to help find keys that are received but dropped. */
#ifdef STU_DEBUG
            trc_prn("WIN_KEY t=%llu WM_KEYDOWN vk=0x%02X (%d) mod=0x%X mox_key=0x%X repeat=%d\n", (unsigned long long)Platform_Get_Millies(), vk_code, vk_code, (unsigned)mox_mod, (unsigned)mox_key, (int)(lParam & 0xFFFF));
#endif

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
            /* CLAUDE: Backspace, Enter, and Tab leave mox_character at 0 so Read_Key()
               returns the MoX key code (e.g., ST_KEY_BACKSPACE = 0x0B), not the ASCII
               character (e.g., '\b' = 0x08). Setup_Input_Box_Popup() and other input
               loops switch on the MoX key code, not the ASCII byte.
               Escape happens to work either way: ST_KEY_ESCAPE == 0x1B == ASCII ESC. */
            else if (vk_code == VK_ESCAPE)
            {
                mox_character = 0x1B;
            }

            /* Also set legacy scan_code_char_code for backwards compatibility */
            if (vk_code >= 0 && vk_code < 256)
            {
                /* CLAUDE */  /* Legacy: populate scan_code_char_code from the old VK_to_SCCS table pattern */
                /* The packed_key / keyboard buffer system is the primary path now */
            }

            /* CLAUDE: diagnostic — log whether the key survived translation and reached the keyboard buffer, or was dropped (and why). */
            if (mox_key != MOX_KEY_UNKNOWN && mox_key < MOX_KEY_OVERRUN)
            {
#ifdef STU_DEBUG
                trc_prn("WIN_KEY t=%llu BUFFERED vk=0x%02X mox_key=0x%X mox_mod=0x%X mox_char=0x%02X ('%c')\n", (unsigned long long)Platform_Get_Millies(), vk_code, (unsigned)mox_key, (unsigned)mox_mod, (unsigned char)mox_character, (mox_character >= 0x20 && mox_character < 0x7F) ? mox_character : '.');
#endif
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            }
            else
            {
#ifdef STU_DEBUG
                trc_prn("WIN_KEY t=%llu DROPPED vk=0x%02X mox_key=0x%X reason=%s\n", (unsigned long long)Platform_Get_Millies(), vk_code, (unsigned)mox_key, (mox_key == MOX_KEY_UNKNOWN) ? "UNKNOWN" : "OVERRUN");
#endif
            }
        } break;


        case WM_LBUTTONDOWN:
        {
            if (platform_mouse_input_enabled)
            {
                /* CLAUDE: pass window-space coords; User_Mouse_Handler divides by scale internally.  See WM_MOUSEMOVE note. */
                int16_t win_x = (int16_t)GET_X_LPARAM(lParam);
                int16_t win_y = (int16_t)GET_Y_LPARAM(lParam);
#ifdef MOUSE_DEBUG
                int scale = Platform_Get_Scale();
                if (scale < 1) { scale = 1; }
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=1 wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (int)(win_x / scale), (int)(win_y / scale));
#endif
#ifdef STU_DEBUG
                trc_prn("WIN_BTN t=%llu WM_LBUTTONDOWN wx=%d wy=%d prev_edge=0x%X\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (unsigned)platform_frame_mouse_buttons);
#endif
                platform_frame_mouse_buttons |= 1;
                User_Mouse_Handler(ST_LEFT_BUTTON, win_x, win_y);
            }
        } break;

        case WM_RBUTTONDOWN:
        {
            if (platform_mouse_input_enabled)
            {
                /* CLAUDE: pass window-space coords; User_Mouse_Handler divides by scale internally.  See WM_MOUSEMOVE note. */
                int16_t win_x = (int16_t)GET_X_LPARAM(lParam);
                int16_t win_y = (int16_t)GET_Y_LPARAM(lParam);
#ifdef MOUSE_DEBUG
                int scale = Platform_Get_Scale();
                if (scale < 1) { scale = 1; }
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=2 wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (int)(win_x / scale), (int)(win_y / scale));
#endif
#ifdef STU_DEBUG
                trc_prn("WIN_BTN t=%llu WM_RBUTTONDOWN wx=%d wy=%d prev_edge=0x%X\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (unsigned)platform_frame_mouse_buttons);
#endif
                platform_frame_mouse_buttons |= 2;
                User_Mouse_Handler(ST_RIGHT_BUTTON, win_x, win_y);
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
            /* CLAUDE: Match SDL behavior — enable/disable mouse input on focus change.
               wParam is TRUE when the window is being activated, FALSE on deactivation. */
            if (wParam)
            {
                Platform_Mouse_Input_Enable();
            }
            else
            {
                Platform_Mouse_Input_Disable();
            }
        } break;

        case WM_CREATE:
        {
            /* CLAUDE: Enable mouse input on window creation so it works from the first frame
               (WM_ACTIVATEAPP also enables it on focus, but that may not fire before first input). */
            Platform_Mouse_Input_Enable();
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
