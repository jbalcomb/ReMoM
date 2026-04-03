/**
 * Platform.h — The formal contract between the game engine (MoX) and the platform layer.
 *
 * This header defines the complete platform interface. It contains NO platform-specific types
 * (no SDL, no Win32). All platform implementations (SDL3, Win32, headless) must provide
 * definitions for every function declared here.
 *
 * Include hierarchy:
 *   MoM (game) -> MoX (engine) -> Platform.h (this file) -> platform/sdl3/ (implementation)
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>



/* ========================================================================= */
/*  Types                                                                    */
/* ========================================================================= */

/**
 * Platform-owned color type.
 * Layout-compatible with SDL_Color (4 bytes: R, G, B, A) so platform
 * implementations can cast directly without conversion.
 */
typedef struct PFL_Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} PFL_Color;



/* ========================================================================= */
/*  Shared Constants (originally from MoX engine headers)                    */
/*                                                                           */
/*  These let platform code use engine conventions without including          */
/*  MoX headers.  Guarded so they don't conflict if engine headers are       */
/*  also visible in the same translation unit.                               */
/* ========================================================================= */

#ifndef ST_TRUE
#define ST_TRUE   1
#endif
#ifndef ST_FALSE
#define ST_FALSE  0
#endif

#define ST_LEFT_BUTTON   1
#define ST_RIGHT_BUTTON  2

/** Game-space screen bounds. */
#define PLATFORM_SCREEN_XMIN  0
#define PLATFORM_SCREEN_YMIN  0
#define PLATFORM_SCREEN_XMAX  (PLATFORM_SCREEN_WIDTH - 1)
#define PLATFORM_SCREEN_YMAX  (PLATFORM_SCREEN_HEIGHT - 1)

/** Clamp-to-max: if b < a, set a = b. */
#ifndef SETMAX
#define SETMAX(a, b) do { if ((b) < (a)) { (a) = (b); } } while (0)
#endif

/** Clamp-to-min: if b > a, set a = b. */
#ifndef SETMIN
#define SETMIN(a, b) do { if ((b) > (a)) { (a) = (b); } } while (0)
#endif

/** Clamp a into [b, c]. */
#ifndef SETRANGE
#define SETRANGE(a, b, c) do { if (((c) <= (b)) || ((b) > (a))) { (a) = (b); } else if ((c) < (a)) { (a) = (c); } } while (0)
#endif



/* ========================================================================= */
/*  Constants                                                                */
/* ========================================================================= */

/** IBM-PC BIOS timer tick interval in milliseconds (~18.2 Hz). */
#define PLATFORM_MILLISECONDS_PER_FRAME  55

#define PLATFORM_MILLISECONDS_PER_SECOND 1000

/** Derived frame rate from BIOS tick interval. */
#define PLATFORM_FRAMES_PER_SECOND  (PLATFORM_MILLISECONDS_PER_SECOND / PLATFORM_MILLISECONDS_PER_FRAME)

/** Original VGA screen dimensions. */
#define PLATFORM_SCREEN_WIDTH   320
#define PLATFORM_SCREEN_HEIGHT  200

/** Default window dimensions (4x scale). */
#define PLATFORM_WINDOW_WIDTH   1280
#define PLATFORM_WINDOW_HEIGHT  800



#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================= */
/*  Lifecycle                                                                */
/* ========================================================================= */

/**
 * Initialize the platform layer: create window, renderer, audio, input.
 * Must be called once before any other Platform_* function.
 */
void Startup_Platform(void);

/**
 * Shut down the platform layer and release all resources.
 * No Platform_* functions may be called after this.
 */
void Shutdown_Platform(void);

/**
 * Return the current window-to-game scale factor (window_width / SCREEN_WIDTH).
 */
int Platform_Get_Scale(void);



/* ========================================================================= */
/*  Shared State                                                             */
/*                                                                           */
/*  These globals are DEFINED in the platform library and EXTERN'd here.     */
/*  The engine reads them; the platform layer writes them.                   */
/* ========================================================================= */

/** Set to non-zero by the platform when the user requests quit (e.g., window close). */
extern int quit_game_flag;

/** Non-zero when a key is available in the keyboard buffer. */
extern int8_t key_pressed;

/** Combined scan code (high byte) and character code (low byte) of the last key press. */
extern uint16_t scan_code_char_code;

/** When non-zero, mouse button status updates are suppressed. */
extern int lock_mouse_button_status_flag;

/** When non-zero, mouse input events are processed by the platform. */
extern int platform_mouse_input_enabled;

/** Per-frame mouse click flag: set when BUTTON_DOWN occurs this frame, cleared
 *  at the start of each Platform_Event_Handler() call.  Unlike mouse_buffer_button
 *  (which is sticky and never clears), this is non-zero only on the frame where
 *  the click actually happened.  Bit 0 = left, bit 1 = right. */
extern int16_t platform_frame_mouse_buttons;

/** Shadow palette: 256 colors in platform-neutral RGBA format. */
extern PFL_Color platform_palette_buffer[256];



/* ========================================================================= */
/*  Engine State (defined in MoX, read/written by platform)                  */
/*                                                                           */
/*  These globals are DEFINED in the engine (MoX) and EXTERN'd here so       */
/*  the platform layer can access them without including engine headers.      */
/* ========================================================================= */

/** Video page buffers — the engine's 8-bit framebuffers (320x200). */
extern uint8_t *video_page_buffer[];

/** Index of the current draw page (0 or 1). */
extern int16_t draw_page_num;

/** Actual pixel dimensions of the game screen (typically 320x200). */
extern int screen_pixel_width;
extern int screen_pixel_height;

/** 256-color palette: 768 bytes (256 * 3, 6-bit VGA DAC values). */
extern uint8_t *current_palette;

/** EMS/VGA file buffer — allocated by EMS_Startup(). */
extern uint8_t *_VGAFILEH_seg;

/** Mouse cursor position in game coordinates. */
extern int16_t pointer_x;
extern int16_t pointer_y;

/** Non-zero when cursor drawing is enabled. */
extern int16_t mouse_enabled;

/** Non-zero when the mouse interrupt handler is active (re-entrancy guard). */
extern int16_t mouse_interrupt_active;

/** Number of entries in the current mouse cursor region list. */
extern int16_t current_mouse_list_count;

/** Mouse buffer coordinates (click location). */
extern int16_t mouse_buffer_x;
extern int16_t mouse_buffer_y;

/** Input field count (used by replay to iterate field rectangles). */
extern int16_t fields_count;



/* ========================================================================= */
/*  Engine Callbacks (platform calls into engine)                            */
/*                                                                           */
/*  These functions are DEFINED in the engine (MoX) and CALLED by the        */
/*  platform layer for cursor rendering, mouse handling, etc.                */
/* ========================================================================= */

/** Return the current cursor image number (0 = none/hidden). */
int16_t Get_Pointer_Image_Number(void);

/** Save the background under the cursor at (x, y) on the draw page. */
void Save_Mouse_On_Page(int16_t x, int16_t y);

/** Draw the cursor sprite at (x, y) on the draw page. */
void Draw_Mouse_On_Page(int16_t x, int16_t y);

/** Restore the background previously saved by Save_Mouse_On_Page(). */
void Restore_Mouse_On_Page(void);

/** Update cursor shape if (x, y) falls in a different mouse region. */
void Check_Mouse_Shape(int16_t x, int16_t y);

/** Mouse interrupt handler: process a button event at window coordinates. */
void User_Mouse_Handler(int16_t buttons, int16_t mx, int16_t my);



/* ========================================================================= */
/*  Video                                                                    */
/* ========================================================================= */

/**
 * Synchronize the engine's software palette (256 x 3-byte RGB in MoX)
 * to the platform's palette buffer and apply it to the rendering surface.
 */
void Platform_Palette_Update(void);

/**
 * Update a single entry in the platform's hardware palette.
 * Used by palette cycling effects that need immediate per-color updates.
 * @param index  Palette index (0..255).
 * @param r      Red component (0..255).
 * @param g      Green component (0..255).
 * @param b      Blue component (0..255).
 */
void Platform_Set_Palette_Color(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * Read back a single entry from the platform's hardware palette.
 * @param index  Palette index (0..255).
 * @param r      [out] Red component (0..255).
 * @param g      [out] Green component (0..255).
 * @param b      [out] Blue component (0..255).
 */
void Platform_Get_Palette_Color(uint8_t index, uint8_t *r, uint8_t *g, uint8_t *b);

/**
 * Blit the engine's 8-bit draw page to the platform's display surface,
 * convert to the display pixel format, and present to the window.
 */
void Platform_Video_Update(void);



/* ========================================================================= */
/*  Events / Input                                                           */
/* ========================================================================= */

/**
 * Process all pending platform events (keyboard, mouse, window, quit).
 * Updates the shared state globals above. Called once per frame from Get_Input().
 *
 * During replay: reads the next Input_Frame instead of polling the OS.
 * During recording: captures the current input state after processing.
 */
void Platform_Event_Handler(void);

/**
 * Pump the platform event queue without processing events.
 * Keeps the OS from thinking the application is unresponsive during busy-waits.
 */
void Platform_Pump_Events(void);

/** Enable mouse input processing. */
void Platform_Mouse_Input_Enable(void);

/** Disable mouse input processing. */
void Platform_Mouse_Input_Disable(void);

/** Clear all pending entries in the keyboard buffer. */
void Platform_Keyboard_Buffer_Clear(void);

/**
 * Add a key press to the keyboard buffer.
 * @param mox_key      The MOX_KEY_* value (from Platform_Keys.h).
 * @param mox_mod      Modifier bitmask (MOX_MOD_SHIFT, MOX_MOD_ALT, etc.).
 * @param mox_character The ASCII character, or 0 for non-printable keys.
 */
void Platform_Keyboard_Buffer_Add_Key_Press(int mox_key, uint32_t mox_mod, char mox_character);

/**
 * Return the number of unread keys in the keyboard buffer.
 */
int Platform_Keyboard_Buffer_Pending_Count(void);

/**
 * Peek at the most recently written packed_key value without consuming it.
 * Returns 0 if the buffer is empty.
 */
uint32_t Platform_Keyboard_Buffer_Peek_Latest(void);

/** Begin accepting text input events (e.g., enable IME). */
void Hw_Textinput_Start(void);

/** Stop accepting text input events. */
void Hw_Textinput_Stop(void);



/* ========================================================================= */
/*  Mouse                                                                    */
/* ========================================================================= */

/**
 * Allocate memory for the VGA file buffer (historical name from DOS EMS).
 */
void EMS_Startup(void);

/**
 * Query the current mouse button state from the platform.
 * @return  ST_LEFT_BUTTON if left is pressed, ST_RIGHT_BUTTON if right is pressed, 0 if neither.
 */
int16_t Platform_Get_Mouse_Button_State(void);

/**
 * Warp (move) the mouse cursor to a position given in game coordinates.
 * The platform handles scaling to window coordinates.
 * @param game_x  X in game coordinates (0..319).
 * @param game_y  Y in game coordinates (0..199).
 */
void Platform_Warp_Mouse(int16_t game_x, int16_t game_y);

/**
 * Return the current window width in pixels.
 * Used by engine code that needs to convert between window and game coordinates.
 */
int Platform_Get_Window_Width(void);

/**
 * Query the current mouse position from the platform.
 * @param mx  Output: mouse X coordinate (0..319 in game space).
 * @param my  Output: mouse Y coordinate (0..199 in game space).
 */
void Platform_Get_Mouse_Position_XY(int *mx, int *my);

/**
 * If the mouse has moved since the last call, save/restore the background
 * under the cursor and redraw the cursor at the new position.
 */
void Platform_Maybe_Move_Mouse(void);



/* ========================================================================= */
/*  Timer                                                                    */
/* ========================================================================= */

/**
 * Return the number of milliseconds since platform startup.
 */
uint64_t Platform_Get_Millies(void);

/**
 * Sleep for the specified number of milliseconds.
 */
void Platform_Sleep_Millies(uint64_t ms);

/**
 * Read the system clock timer as a BIOS-style tick count (~18.2 Hz).
 * Used by Randomize() and the timing system.
 */
uint64_t Read_System_Clock_Timer(void);



/* ========================================================================= */
/*  Audio                                                                    */
/* ========================================================================= */

/**
 * Play a sound effect or music from a raw buffer (VOC, XMIDI, etc.).
 * The platform layer handles format detection and conversion.
 * @param sound_buffer       Pointer to the raw sound data.
 * @param sound_buffer_size  Size of the buffer in bytes.
 * @return  Implementation-defined status (typically -1 on no-op).
 */
int16_t Platform_Audio_Play_Sound(void *sound_buffer, uint32_t sound_buffer_size);



/* ========================================================================= */
/*  Frame Callback (for synthetic input injection)                           */
/* ========================================================================= */

/**
 * Register an optional per-frame callback for synthetic input injection.
 * Called once per Platform_Event_Handler() when no replay is active.
 * Used by HeMoM's synthetic player to inject scripted actions.
 * @param callback  Function to call each frame, or NULL to unregister.
 */
void Platform_Register_Frame_Callback(void (*callback)(void));



/* ========================================================================= */
/*  Win32 Legacy                                                             */
/* ========================================================================= */

#ifdef _STU_WIN
void Pump_Events(void);
void Pump_Paints(void);
void MWA_Set_Mouse_Position(int16_t x, int16_t y);
void MWA_Exit_With_Message(char *string);
#endif



#ifdef __cplusplus
}
#endif



/* ========================================================================= */
/*  Mouse Debug Log                                                          */
/*                                                                           */
/*  When MOUSE_DEBUG is defined, all MOUSE_LOG() calls write to              */
/*  mouse_debug.log in the working directory (not stderr, which gets         */
/*  redirected to CON on Win32 debug builds).                                */
/* ========================================================================= */

#ifdef MOUSE_DEBUG
#include <stdio.h>

static FILE *mouse_dbg_log_file = NULL;

static FILE *Mouse_Dbg_Log(void)
{
    if (mouse_dbg_log_file == NULL)
    {
        mouse_dbg_log_file = fopen("mouse_debug.log", "w");
        if (mouse_dbg_log_file != NULL) { setvbuf(mouse_dbg_log_file, NULL, _IONBF, 0); }
    }
    return (mouse_dbg_log_file != NULL) ? mouse_dbg_log_file : stderr;
}

#define MOUSE_LOG(...) fprintf(Mouse_Dbg_Log(), __VA_ARGS__)
#endif

#endif /* PLATFORM_H */
