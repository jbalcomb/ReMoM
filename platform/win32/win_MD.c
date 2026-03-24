/**
 * win_MD.c — Win32 mouse handling (platform-level only).
 *
 * Game-level Mouse_Button(), Mouse_Movement_Handler(), Mouse_Button_Handler(),
 * User_Mouse_Handler(), and Set_Mouse_Position() live in MoX/src/Mouse.c.
 *
 * This file is intentionally minimal — mouse events are handled in
 * Win_Window_Proc (win_PFL.cpp) which calls the engine callbacks
 * declared in Platform.h (User_Mouse_Handler, Update_Mouse_Position, etc.).
 */

/* This file is currently empty — all Win32 mouse handling is in win_PFL.cpp. */
