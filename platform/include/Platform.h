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

/** Default window dimensions (2x scale). */
#define PLATFORM_WINDOW_WIDTH   640
#define PLATFORM_WINDOW_HEIGHT  400



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
void Shudown_Platform(void);



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

/** Shadow palette: 256 colors in platform-neutral RGBA format. */
extern PFL_Color platform_palette_buffer[256];



/* ========================================================================= */
/*  Video                                                                    */
/* ========================================================================= */

/**
 * Synchronize the engine's software palette (256 x 3-byte RGB in MoX)
 * to the platform's palette buffer and apply it to the rendering surface.
 */
void Platform_Palette_Update(void);

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

/** Begin accepting text input events (e.g., enable IME). */
void hw_textinput_start(void);

/** Stop accepting text input events. */
void hw_textinput_stop(void);



/* ========================================================================= */
/*  Mouse                                                                    */
/* ========================================================================= */

/**
 * Allocate memory for the VGA file buffer (historical name from DOS EMS).
 */
void EMS_Startup(void);

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

/**
 * Callback from the platform to the engine: update the engine's mouse position.
 * @param platform_mouse_x  X in game coordinates.
 * @param platform_mouse_y  Y in game coordinates.
 */
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y);

/**
 * Callback from the platform to the engine: update the engine's mouse button state.
 * @param platform_mouse_x      X in game coordinates.
 * @param platform_mouse_y      Y in game coordinates.
 * @param mouse_button_status   Button bitmask.
 */
void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status);



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
/*  Win32 Legacy                                                             */
/* ========================================================================= */

#ifdef _STU_WIN
void Pump_Events(void);
void Pump_Paints(void);
void MWA_Set_Mouse_Position(int16_t x, int16_t y);
#endif



#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H */
