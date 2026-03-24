


Those IDE errors are just IntelliSense not finding SDL3 headers (it doesn't have the CMake include paths). Not a real build error. Let me continue with the SDL2 backend.



"Master of Magic" (MoM) was coded for MS-DOS, so ReMoM has to recreate the platform layer to use a library like SDL or run natively like Win32, to run on Linux, Mac, Windows, etc. Review the code for the platform layer, for Win32, SDL2, SDL3. The choice of platform should be based on the build target. Any game or game-engine code that must be platform dependent should be pulled out into the platform layer. The platform layer code should behave as the game and game-engine code originally expected, for example keyboard scancode should be mimicked to appear as they did in MS-DOS. Summarize the state of the platform seperation and make a plan for completing the goal and fixing any existing issues, like names or boundary violations.

I'll start by exploring the project structure to understand the platform layer organization.
Update Todos
Explore project structure and CMake build system
Review platform layer headers and interfaces
Review Win32 platform implementation
Review SDL2 platform implementation
Review SDL3 platform implementation
Identify platform boundary violations in game/engine code
Summarize state and create completion plan
Good. Now let me read the public platform headers and dig into the implementations across all three backends.
Update Todos
Explore project structure and CMake build system
Review platform layer headers and interfaces
Review Win32 platform implementation
Review SDL2 platform implementation
Review SDL3 platform implementation
Review legacy sdl2_ files still in MoX/src/
Identify platform boundary violations in game/engine code
Summarize state and create completion plan
Now let me review all three backends and the old MoX files in parallel.
Agent:Review Win32 platform backend
IN
Thoroughly review all files in c:\STU\devel\ReMoM\platform\win32\. For each file, report:
1. What Platform.h functions it implements (if any)
2. What platform-specific APIs it uses (Win32, etc.)
3. Any functions NOT declared in Platform.h that it exposes
4. Whether it includes any SDL headers
5. Any naming convention issues (should use Proper_Snake_Case for functions, lower_snake_case for variables, UPPER_SNAKE_CASE for constants)
6. Whether the implementation looks complete or is a stub
Read every .c, .cpp, .h, and .hpp file in platform/win32/.
Agent:Review SDL2 platform backend
IN
Thoroughly review all source files in c:\STU\devel\ReMoM\platform\sdl2\. For each file, report:
1. What Platform.h functions it implements
2. What SDL2-specific APIs it uses
3. Any functions NOT declared in Platform.h that it exposes (boundary violations)
4. Whether it includes any engine headers directly (it should only include Platform.h and Platform_Keys.h from platform/include/)
5. Any naming convention issues (should use Proper_Snake_Case for functions, lower_snake_case for variables, UPPER_SNAKE_CASE for constants)
6. Any globals that should be in Platform.h but aren't
Also read all .h files in platform/sdl2/ and report which ones are backend-private vs which expose types to the engine.
Read every file: sdl2_Audio.c, sdl2_Audio.h, sdl2_EMM.c, sdl2_Init.c, sdl2_Input.c, sdl2_KD.c, sdl2_KD.h, sdl2_MD.c, sdl2_MOM.h, sdl2_PFL.c, sdl2_PFL.h, sdl2_SCCC.h, sdl2_SDLK.c, sdl2_SDLK.h, sdl2_State.c, sdl2_Timer.c, sdl2_Video.c, MOM_PFL.h, PFL_Audio_Internal.h
Agent:Review SDL3 platform backend
IN
Thoroughly review all source files in c:\STU\devel\ReMoM\platform\sdl3\. For each file, report:
1. What Platform.h functions it implements
2. What SDL3-specific APIs it uses
3. Any functions NOT declared in Platform.h that it exposes (boundary violations)
4. Whether it includes any engine headers directly (it should only include Platform.h and Platform_Keys.h from platform/include/)
5. Any naming convention issues (should use Proper_Snake_Case for functions, lower_snake_case for variables, UPPER_SNAKE_CASE for constants)
6. Any globals that should be in Platform.h but aren't
7. Whether it still references any SDL2-era names or patterns (the sdl3/ dir still has many files named sdl2_*.h)
Read every file: sdl3_Audio.c, sdl3_EMM.c, sdl3_Init.c, sdl3_Input.c, sdl3_KD.c, sdl3_MD.c, sdl3_PFL.c, sdl3_SDLK.c, sdl3_State.c, sdl3_Timer.c, sdl3_Video.c, MOM_PFL.h, PFL_Audio_Internal.h, sdl2_Audio.h, sdl2_KD.h, sdl2_MOM.h, sdl2_PFL.h, sdl2_SCCC.h, sdl2_SDLK.h
Agent:Review old MoX sdl2_ files
IN
Review all sdl2_* files in c:\STU\devel\ReMoM\MoX\src\. These appear to be legacy platform files that haven't been moved to the platform/ directory yet. For each file, report:
1. What functions it defines
2. Whether those functions overlap with Platform.h declarations
3. Whether it includes SDL headers directly
4. Whether it's still referenced by other MoX source files (grep for its functions/includes in MoX/src/)
5. Whether it appears to be actively compiled (check MoX/src/CMakeLists.txt)
Files to read: sdl2_Audio.c, sdl2_Audio.h, sdl2_boolvec.h, sdl2_EMM.c, sdl2_Input.c, sdl2_KD.c, sdl2_KD.h, sdl2_MD.c, sdl2_MOM.c, sdl2_MOM.h, sdl2_PFL.c, sdl2_PFL.h, sdl2_SCCC.h, sdl2_SDLK.c, sdl2_SDLK.h, sdl2_Timer.c, sdl2_types.h

Also check if MoX/src/CMakeLists.txt references any of these files.
Agent:Find platform boundary violations
IN
Search the MoX/ and MoM/ directories (game and engine code) for platform boundary violations. Specifically search for:

1. Any #include of SDL headers (SDL.h, SDL2, SDL3, SDL_*.h) in MoX/src/*.c or MoM/src/*.c files
2. Any direct use of SDL functions (SDL_*, SDL_Event, SDL_Window, etc.) in MoX/src/*.c or MoM/src/*.c
3. Any #include of win32 headers (windows.h, wingdi.h, etc.) in MoX/src/*.c or MoM/src/*.c
4. Any direct use of Win32 functions (CreateWindow, GetMessage, etc.) in MoX/src/*.c or MoM/src/*.c
5. Any references to "sdl2_" prefixed headers from MoX/src/*.c files that are NOT sdl2_* files themselves
6. Any #include of platform backend headers (sdl2_MOM.h, sdl2_PFL.h, etc.) from MoX/src/*.c or MoM/src/*.c
7. Any use of DOS-specific APIs (int86, inp, outp, bioskey) that need platform abstraction
8. Check the MoX/src/CMakeLists.txt for what files are built and whether any sdl2_ files are in the build

Be very thorough — these violations are the core issue we need to catalog.
I've launched 5 parallel research agents to review:

Win32 backend (platform/win32/)
SDL2 backend (platform/sdl2/)
SDL3 backend (platform/sdl3/)
Legacy MoX files (MoX/src/sdl2_*)
Boundary violations (SDL/Win32 leaking into MoX/MoM game code)
I'll also check the other subdirectories while waiting.

Globpattern: "MoM/src/**/*"
Found 101 files
Read c:\STU\devel\ReMoM\src\CMakeLists.txt
Read c:\STU\devel\ReMoM\MoM\CMakeLists.txt
Globpattern: "STU/**/*"
Found 43 files
Let me also check for SDL includes in the main exe and STU library while agents work.

Read c:\STU\devel\ReMoM\src\ReMoM.c
Grep "SDL|sdl2_|sdl3_" (in c:\STU\devel\ReMoM\STU\src)
4 lines of output
Read c:\STU\devel\ReMoM\platform\replay\Replay.c
Important finding: ReMoM.c:78-90 directly includes SDL headers. Let me also read the platform/main.cpp file and check how the replay system references MoX internals.

Read c:\STU\devel\ReMoM\platform\main.cpp
Read c:\STU\devel\ReMoM\platform\replay\Replay.c (lines 51-100)
Now let me wait for the agents to finish and gather their findings. Let me check the MoM CMakeLists to see what it links.

Globpattern: "MoM/src/CMakeLists.txt"
Found 1 file
Read c:\STU\devel\ReMoM\MoM\src\CMakeLists.txt
Good. Now let me check the STU library for SDL dependencies.

Read c:\STU\devel\ReMoM\STU\src\CMakeLists.txt
Read c:\STU\devel\ReMoM\STU\src\STU_SND.c
Now I have a solid picture. Let me check if the agents have completed.

I'm waiting for the 5 research agents to complete. Let me check their progress.

TaskOutput task: "abeb236c391ec86f4"
OUT
<retrieval_status>success</retrieval_status>

<task_id>abeb236c391ec86f4</task_id>

<task_type>local_agent</task_type>

<status>completed</status>

<output>
Perfect! Now I have all the information I need. Let me compile the comprehensive report:

## Comprehensive Review of platform/win32/ Files

### **File 1: win_CLK.c**

**Platform.h Functions Implemented:**
- `Read_System_Clock_Timer()` (line 48) - Returns BIOS-style tick count (converts Win32 GetTickCount to DOS ticks)
- `Mark_Time()` (line 170) - Records current tick count
- `Time_Passed()` (line 176) - Returns ticks elapsed since last Mark_Time call
- `Timer_Value()` (line 181) - Returns current tick count
- `Release_Time()` (line 61) - Busy-waits until specified number of ticks have passed

**Platform-Specific APIs Used:**
- `GetTickCount()` - Win32 API to get system uptime in milliseconds
- `Windows.h` - Standard Windows header

**Functions NOT in Platform.h:**
- `Release_Time()` - Utility function not declared in Platform.h
- `Mark_Time()` and `Time_Passed()` appear to be legacy; Platform.h defines `Platform_Get_Millies()` instead

**SDL Headers:** None

**Naming Convention Issues:**
- All function names use `Proper_Snake_Case` correctly (e.g., `Read_System_Clock_Timer`, `Mark_Time`, `Release_Time`)
- Variable `tick_count` uses `lower_snake_case` correctly

**Implementation Status:** 
- Functional but incomplete. The legacy DOS-style timing API is partially implemented. Modern Platform.h functions like `Platform_Get_Millies()` and `Platform_Sleep_Millies()` are not implemented in this file. Contains extensive TODOs for HMH Day 10 performance profiling code.

---

### **File 2: win_EMS.c**

**Platform.h Functions Implemented:**
- `EMS_Startup()` (line 10) - Allocates memory for VGA file buffer (historical EMM/EMS name)

**Platform-Specific APIs Used:**
- `malloc()` - Standard C memory allocation
- `memset()` - Standard C memory clearing
- `<windows.h>` - For BYTE type definition via MOX_TYPE.h
- `<memory.h>` - Standard memory functions

**Functions NOT in Platform.h:**
- None; this is the only function

**SDL Headers:** None

**Naming Convention Issues:**
- Function name `EMS_Startup()` uses `Proper_Snake_Case` correctly
- Variable `_VGAFILEH_seg` uses leading underscore (non-standard but used as external global)
- `byte_ptr` is a typedef (should verify if this matches coding standards)

**Implementation Status:**
- Minimal but complete. Simply allocates and zeros a 80KB buffer (16384 * 5 bytes). The malloc approach is appropriate for Win32.

---

### **File 3: win_Exit.c**

**Platform.h Functions Implemented:**
- `MWA_Exit_With_Message()` (line 22) - Displays error message and closes window

**Platform-Specific APIs Used:**
- `MessageBoxA()` - Win32 API for modal dialog
- `PostMessage()` with `WM_CLOSE` - Win32 window message
- `<Windows.h>` - Windows API header
- Calls to external functions: `Stop_Music__STUB()`, `Reset_System_Mouse()`, `Audio_Uninit__STUB()`

**Functions NOT in Platform.h:**
- `MWA_Exit_With_Message()` - Declared in win_PFL.hpp (Win32-specific, not in Platform.h contract)

**SDL Headers:** None

**Naming Convention Issues:**
- Function name `MWA_Exit_With_Message()` uses `Proper_Snake_Case` correctly
- Parameter `string` uses `lower_snake_case` correctly

**Implementation Status:**
- Incomplete stub. Most functionality commented as TODO or redirected to stubs. Only implements Windows message box and window close.

---

### **File 4: win_Input.c**

**Platform.h Functions Implemented:**
- `Get_Input()` (line 17) - Returns field index based on mouse input (legacy engine function)

**Platform-Specific APIs Used:**
- Calls `Pump_Events()` - Custom event pump (defined elsewhere)
- Calls `Interpret_Mouse_Input()` - External mouse handler
- `PoC_PFL.h` and `MOM_PFL.h` - Project-specific headers

**Functions NOT in Platform.h:**
- `Get_Input()` - This is a legacy MoX function, not part of the Platform.h contract

**SDL Headers:** None

**Naming Convention Issues:**
- Function name `Get_Input()` uses `Proper_Snake_Case` correctly
- Variable `field_index` uses `lower_snake_case` correctly
- Variable `input_delay` uses `lower_snake_case` correctly

**Implementation Status:**
- Stub/incomplete. Accesses external globals: `input_delay`, `fields_count`, `mouse_installed`. Comments suggest GUI-only input path is not implemented ("TODO CRP_GUI_KeyInputOnly"). Depends on `Interpret_Mouse_Input()` from external code.

---

### **File 5: win_KD.c**

**Platform.h Functions Implemented:**
- `Keyboard_Status()` (line 77) - Returns whether a key is available (ST_TRUE/ST_FALSE)
- `Read_Key()` (line 117) - Returns translated key value from scan code/char code

**Platform-Specific APIs Used:**
- `GetKeyState()` - Win32 API to query modifier key states (VK_SHIFT, VK_CONTROL, VK_MENU)
- Calls `Pump_Events()` - Custom event pump
- `PoC_PFL.h` and `MOM_PFL.h` - Project headers

**Functions NOT in Platform.h:**
- Both `Keyboard_Status()` and `Read_Key()` are legacy MoX functions, not in Platform.h contract

**SDL Headers:** None

**Naming Convention Issues:**
- Function names `Keyboard_Status()` and `Read_Key()` use `Proper_Snake_Case` correctly
- Variable names like `keyboard_status`, `return_key` use `lower_snake_case` correctly
- SCCC_ constants use `UPPER_SNAKE_CASE` correctly

**Implementation Status:**
- Functional but legacy. Implements full keyboard translation table with 200+ case statements for scan code/character mappings. Contains extensive Win32-specific notes about keyboard input (WM_KEYDOWN, scan codes, extended keys). Accesses external global `key_pressed` and `scan_code_char_code`.

---

### **File 6: win_MD.c**

**Platform.h Functions Implemented:**
- `Mouse_Button()` (line 14) - Returns current mouse button status
- `Mouse_Movement_Handler()` (line 31) - Sets lock flag when mouse moves
- `Mouse_Button_Handler()` (line 37) - Clears button status and unlocks flag
- `User_Mouse_Handler()` (line 46) - Main mouse event handler (not in Platform.h but called by platform layer)
- `Set_Mouse_Position()` (line 82) - Sets mouse cursor position

**Platform-Specific APIs Used:**
- Calls `Pump_Events()` - Custom event pump
- Calls external MoX engine functions: `Check_Mouse_Buffer()`, `Check_Mouse_Shape()`, `Restore_Mouse_On_Page()`, `Save_Mouse_On_Page()`, `Draw_Mouse_On_Page()`
- `win_PFL.hpp` for `MWA_Set_Mouse_Position()`

**Functions NOT in Platform.h:**
- `Mouse_Button()` - Legacy MoX function
- `Mouse_Movement_Handler()` - Legacy MoX function
- `Mouse_Button_Handler()` - Legacy MoX function
- `User_Mouse_Handler()` - Internal platform function (not in Platform.h contract)
- `Set_Mouse_Position()` - Legacy MoX wrapper

**SDL Headers:** None

**Naming Convention Issues:**
- All function names use `Proper_Snake_Case` correctly (e.g., `Mouse_Button`, `User_Mouse_Handler`, `Set_Mouse_Position`)
- Variable names like `l_mx`, `l_my`, `buttons` use `lower_snake_case` correctly

**Implementation Status:**
- Functional but legacy. Implements complete mouse handling including position tracking, button detection, and cursor drawing. Heavy integration with MoX engine's mouse drawing system. Accesses multiple external globals: `platform_mouse_button_status`, `lock_mouse_button_status_flag`, `pointer_x`, `pointer_y`, `mouse_interrupt_active`, `mouse_enabled`, `current_mouse_list_count`, `draw_page_num`.

---

### **File 7: win_PFL.hpp**

**Platform.h Functions Declared (C++ header):**
- `Init_Platform()` - Initializes platform layer
- `MWA_Exit_With_Message()` - Exit with error message (Win32-specific)
- `MWA_Set_Mouse_Position()` - Set mouse position (Win32-specific)

**Platform-Specific Types/Data:**
- Declares external array `VK_to_SCCS[]` - Virtual Key to Scan Code/Char Code translation table

**SDL Headers:** None

**Naming Convention Issues:**
- Function declarations use `Proper_Snake_Case` correctly
- Prefixes like `MWA_` (Microsoft Windows API) follow naming standard

**Implementation Status:**
- Header only. Provides C++ interop with `extern "C"` block. Declares Win32-specific functions that are NOT in the Platform.h contract.

---

### **File 8: win_PFL.cpp**

**Platform.h Functions Implemented (Partial):**
- `Init_Platform()` (line 480) - Initializes platform: window, back buffer, performance counter, scheduler granularity, game update frequency
- `Pump_Events()` (line 365) - Processes Windows message queue, sets `quit_game_flag` on WM_QUIT
- `Pump_Paints()` (line 386) - Forces WM_PAINT processing
- `MWA_Set_Mouse_Position()` (line 355) - Sets cursor position using Windows API

**Platform-Specific APIs Used (Extensive):**
- `GetTickCount()` - System timer
- `QueryPerformanceFrequency()` - High-resolution timer initialization
- `timeBeginPeriod()` - Scheduler granularity
- Window API: `CreateWindowEx()`, `ShowWindow()`, `UpdateWindow()`, `GetDC()`, `RegisterClassEx()`
- Message handling: `PeekMessageA()`, `TranslateMessage()`, `DispatchMessageA()`, `PostMessage()`, `PostQuitMessage()`
- Drawing: `GetClientRect()`, `BeginPaint()`, `EndPaint()`, `StretchDIBits()`, `VirtualAlloc()`, `VirtualFree()`, `memset()`
- Input: `GetKeyState()`, `GetCursorPos()`, `ScreenToClient()`, `ClientToScreen()`, `SetCursorPos()`, `ShowCursor()`
- macros: `GET_X_LPARAM()`, `GET_Y_LPARAM()` from `windowsx.h`

**Functions NOT in Platform.h:**
- `Pump_Events()` - Declared in Platform.h as legacy Win32 function (line 265)
- `Pump_Paints()` - Declared in Platform.h as legacy Win32 function (line 266)
- `Get_Window_Dimensions()` (line 398) - Internal helper
- `Update_Window_Display()` (line 411) - Internal rendering function
- `Convert_320x200xVGA_To_320x200xXBGR()` (line 453) - Internal pixel format conversion (unused)
- `Init_Window_Back_Buffer()` (line 510) - Internal buffer initialization
- `WndInit()` (line 550) - Internal window creation
- `WndEvnt()` (line 615) - Window procedure callback (LRESULT CALLBACK)

**SDL Headers:** None (only comment reference on line 438)

**Naming Convention Issues:**
- Function names use mixed conventions:
  - Win32 API callbacks like `WndEvnt()` use single word (correct for callbacks)
  - Utility functions like `Get_Window_Dimensions()` use `Proper_Snake_Case` correctly
  - Internal functions like `Update_Window_Display()`, `Convert_320x200xVGA_To_320x200xXBGR()` use `Proper_Snake_Case` correctly
- Variable names mostly use `lower_snake_case` correctly (e.g., `window_width`, `bitmap_memory_size`, `color_map_index`)
- Global variables: `g_Instance`, `g_Window`, `g_DeviceContext` follow Win32 naming conventions (lowercase g_ prefix)
- Constants like `default_window_width` should be `DEFAULT_WINDOW_WIDTH` (UPPER_SNAKE_CASE)

**Implementation Status:**
- Fairly complete. Implements most Win32 platform layer functionality:
  - Window creation and management
  - Message loop and event processing
  - VGA palette to 32-bit XBGR pixel conversion
  - Screen scaling/stretching to window size
  - Keyboard and mouse input handling (WM_KEYDOWN, WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_MOUSEMOVE)
  - Includes lots of debug output via `OutputDebugStringA()`
- **Gaps:**
  - `Platform_Get_Millies()` not implemented
  - `Platform_Sleep_Millies()` not implemented
  - `Platform_Event_Handler()` (new API) not implemented - still using legacy `Pump_Events()`
  - `Platform_Video_Update()` (new API) not implemented - still using `Update_Window_Display()`
  - `Platform_Palette_Update()` not implemented
  - `Platform_Set_Palette_Color()` not implemented
  - Modern Platform.h keyboard buffer API not implemented

---

### **File 9: win_Video.c**

**Platform.h Functions Implemented:**
- None directly implemented - file is essentially empty/stub

**Platform-Specific APIs Used:**
- None (file contains only comments)

**Functions NOT in Platform.h:**
- None

**SDL Headers:** None

**Naming Convention Issues:**
- N/A (no code)

**Implementation Status:**
- Stub. Contains only comments documenting the original MoX/Video.c `Page_Flip()` function. The actual video page flipping and rendering is implemented in `win_PFL.cpp` instead.

---

## Summary Table

| File | Platform.h Functions | SDL Headers | Naming Issues | Status |
|------|----------------------|-------------|---------------|--------|
| win_CLK.c | Read_System_Clock_Timer, Mark_Time, Time_Passed, Timer_Value, Release_Time | None | OK | Incomplete (legacy API) |
| win_EMS.c | EMS_Startup | None | OK | Complete |
| win_Exit.c | MWA_Exit_With_Message | None | OK | Stub |
| win_Input.c | Get_Input (legacy) | None | OK | Stub |
| win_KD.c | Keyboard_Status, Read_Key (legacy) | None | OK | Functional |
| win_MD.c | Mouse_Button, Set_Mouse_Position (legacy) | None | OK | Functional |
| win_PFL.hpp | Init_Platform, MWA_Exit_With_Message (declares) | None | OK | Header only |
| win_PFL.cpp | Init_Platform, Pump_Events, Pump_Paints, MWA_Set_Mouse_Position | None (comment only) | Some (constants should be UPPER_SNAKE_CASE) | Fairly complete but gaps in new Platform.h API |
| win_Video.c | None (stub) | None | N/A | Stub |

---

## Key Observations

1. **Legacy Architecture:** The win32 platform layer is built around legacy MoX functions, not the modern Platform.h contract
2. **Heavy Win32 Integration:** Extensive use of Windows API throughout (messages, dialogs, device contexts, virtual memory)
3. **Missing Modern API:** Platform.h functions like `Platform_Event_Handler()`, `Platform_Video_Update()`, `Platform_Palette_Update()`, `Platform_Get_Millies()`, `Platform_Sleep_Millies()` are not implemented
4. **Mixed Code Quality:** Some files are complete (win_PFL.cpp), others are stubs (win_Exit.c, win_Video.c)
5. **No SDL:** The Win32 implementation has no SDL dependencies, as expected
6. **Naming:** Generally follows Proper_Snake_Case for functions and lower_snake_case for variables, though some constants should be UPPER_SNAKE_CASE
</output>
TaskOutput task: "a0318a0c495a681e0"
OUT
<retrieval_status>success</retrieval_status>

<task_id>a0318a0c495a681e0</task_id>

<task_type>local_agent</task_type>

<status>completed</status>

<output>
Excellent! Now I have enough information. Let me compile my comprehensive review report.

## Comprehensive Review of platform/sdl2/ Files

### File-by-File Analysis

#### **sdl2_Audio.c**
- **Platform.h Functions Implemented**: None directly
  - `sdl2_Audio_Init()` and `sdl2_Audio_Deinit()` are called from `Startup_Platform()` but are NOT declared in Platform.h
  - These are backend-internal initialization functions
- **SDL2 APIs Used**: `Mix_Init()`, `Mix_OpenAudio()`, `Mix_VolumeMusic()`, `Mix_CloseAudio()`, `Mix_Quit()`, `Mix_LoadWAV_RW()`, `Mix_PlayChannel()`, `SDL_RWFromMem()`, `SDL_GetError()`
- **Non-Platform.h Functions Exposed**:
  - `sdl2_Audio_Init()` (declared in sdl2_Audio.h)
  - `sdl2_Audio_Deinit()` (declared in sdl2_Audio.h)
  - `Play_Sound_SDL2_Mixer()` (backend-specific, not in Platform.h)
- **Engine Headers Included Directly**: YES - includes `../../STU/src/STU_DBG.h`
- **Naming Issues**: None detected in public functions
- **Globals**: `wav_buffers[]`, `wav_buffer_idx`, `audio_initialized` (extern), `ui_data_mus`, `ui_data_music_i`, `opt_music_enabled`, `opt_music_volume`, `mustbl` struct

#### **sdl2_Audio.h**
- **Backend-Private vs Engine-Exposed**: BACKEND-PRIVATE
  - Declares `sdl2_Audio_Init()` and `sdl2_Audio_Deinit()`
  - Includes `../../MoX/src/MOX_TYPE.h` (engine boundary violation)

#### **sdl2_EMM.c**
- **Platform.h Functions Implemented**: `EMS_Startup()` (declared in Platform.h)
- **SDL2 APIs Used**: None
- **Non-Platform.h Functions**: None
- **Engine Headers**: YES - includes `../../MoX/src/EMM.h`, `MOX_DEF.h`, `MOX_TYPE.h`
- **Naming**: Follows Proper_Snake_Case for function name
- **Globals**: `_VGAFILEH_seg` (internal)

#### **sdl2_Init.c**
- **Platform.h Functions Implemented**: 
  - `Startup_Platform()` 
  - `Shudown_Platform()` (note: misspelled "Shutdown")
  - `Platform_Mouse_Input_Enable()`
  - `Platform_Mouse_Input_Disable()`
- **SDL2 APIs Used**: `SDL_Init()`, `SDL_GetTicks()`, `SDL_GetPerformanceCounter()`, `SDL_CreateWindow()`, `SDL_GetWindowSize()`, `SDL_CreateRenderer()`, `SDL_CreateRGBSurface()`, `SDL_CreateRGBSurfaceWithFormat()`, `SDL_CreateTexture()`, `SDL_ShowCursor()`, `SDL_DISABLE`, `SDL_ENABLE`, `SDL_Quit()`, `SDL_DestroyRenderer()`, `SDL_DestroyWindow()`
- **Non-Platform.h Functions**: None
- **Engine Headers**: YES - includes MOX headers and keyboard/audio backends
- **Naming**: Correct (Proper_Snake_Case)
- **Boundary Violation**: Calls backend-specific `build_key_xlat()` and `sdl2_Audio_Init()` from init

#### **sdl2_Input.c**
- **Platform.h Functions Implemented**: None directly; implements lower-level game input
- **SDL2 APIs Used**: None
- **Non-Platform.h Functions**: `Get_Input()` (game-level, called from MoX)
- **Engine Headers**: YES - includes `../../MoX/src/Fields.h`, `Input.h`, `Video.h`
- **Naming**: Correct (Proper_Snake_Case)
- **Boundary Violation**: Calls `Platform_Event_Handler()` from game input code

#### **sdl2_KD.c** (Keyboard Driver)
- **Platform.h Functions Implemented**: 
  - `Platform_Keyboard_Event()` (processes SDL_Event - **BOUNDARY VIOLATION**: takes SDL_Event*)
  - `Platform_Keyboard_Buffer_Clear()`
  - `Platform_Keyboard_Buffer_Add_Key_Press()`
  - `Platform_Keyboard_Buffer_Pending_Count()`
  - `Platform_Keyboard_Buffer_Peek_Latest()`
  - `hw_textinput_start()`
  - `hw_textinput_stop()`
- **SDL2 APIs Used**: `SDL_GetScancodeName()`, `SDL_GetKeyName()`, `SDL_StartTextInput()`, `SDL_StopTextInput()`, `SDL_PushEvent()`, `SDLK_*` constants
- **Non-Platform.h Functions**:
  - `Keyboard_Status()` (backward compatibility, game-level)
  - `Read_Key()` (backward compatibility, game-level)
  - `build_key_xlat()` (internal)
  - `mod_xlat()` (internal)
  - `kbd_set_pressed()` (internal)
  - `kbd_is_pressed()` (internal)
  - `sdl2_get_key_code_name()` (debugging)
- **Engine Headers**: YES - includes MOX keyboard headers
- **Naming Issues**: 
  - VIOLATION: `sdl2_get_key_code_name()` should be `Sdl2_Get_Key_Code_Name()`
  - VIOLATION: `mod_xlat()` should be `Mod_Xlat()` 
  - VIOLATION: `kbd_set_pressed()` should be `Kbd_Set_Pressed()`
  - VIOLATION: `kbd_is_pressed()` should be `Kbd_Is_Pressed()`
- **Globals**: 
  - `platform_keyboard_buffer` (exported via sdl2_KD.h)
  - `key_xlat_key[]`, `key_xlat_scan[]` (static)
  - `hw_textinput_active` (static)

#### **sdl2_KD.h**
- **Backend-Private vs Engine-Exposed**: MIXED (some public, some private)
  - Exposes MOX key enums and KBD_GET_* macros (engine-level, appropriate)
  - Declares `Platform_Keyboard_Event()` with **SDL_Event* parameter** - BOUNDARY VIOLATION
  - Also in `sdl2_MOM.h` (duplicate declaration)

#### **sdl2_MD.c** (Mouse Driver)
- **Platform.h Functions Implemented**: None directly
- **SDL2 APIs Used**: `SDL_GetMouseState()`, `SDL_WarpMouseInWindow()`
- **Non-Platform.h Functions**:
  - `Mouse_Button()` (game-level, called from MoX)
  - `Mouse_Movement_Handler()`
  - `Mouse_Button_Handler()`
  - `User_Mouse_Handler()`
  - `Set_Mouse_Position()`
- **Engine Headers**: YES - includes Mouse.h, Video.h
- **Naming**: Correct (Proper_Snake_Case)
- **Globals**: None new (uses `pointer_x`, `pointer_y` from engine)

#### **sdl2_MOM.h**
- **Backend-Private vs Engine-Exposed**: BACKEND-SEMI-PUBLIC
  - Declares platform-facing functions that should be in Platform.h
  - **BOUNDARY VIOLATION**: Declares `Platform_Keyboard_Event(SDL_Event *)` - SDL_Event should not be exposed

#### **sdl2_PFL.c** (Main Platform Implementation)
- **Platform.h Functions Implemented**:
  - `Platform_Get_Scale()`
  - `Platform_Get_Millies()`
  - `Platform_Sleep_Millies()`
  - `Platform_Get_Mouse_Position_XY()`
  - `Platform_Maybe_Move_Mouse()`
  - `Platform_Event_Handler()` (main event loop)
  - `Platform_Window_Event()`
  - `DBG_Frame_Reset()` (debug only)
- **SDL2 APIs Used**: `SDL_GetTicks()`, `SDL_Delay()`, `SDL_GetMouseState()`, `SDL_WarpMouseInWindow()`, `SDL_GetWindowID()`, `SDL_SetWindowGrab()`, `SDL_BUTTON()`, SDL_Event types
- **Non-Platform.h Functions**:
  - `kilgore_video_get_window_id()` (static, internal)
  - `kilgore_mouse_set_xy_from_hw()` (static, internal)
  - `kilgore_video_input_grab()` (static, internal)
  - `kilgore_mouse_grab()` (static, internal)
  - `kilgore_mouse_ungrab()` (static, internal)
  - `kilgore_mouse_toggle_grab()` (static, internal)
  - `kilgore_mouse_set_limits()` (static, internal)
  - `kilgore_mouse_set_scale()` (static, internal)
  - `kilgore_mouse_move()` (static, internal)
  - `Platform_Update_Mouse_Position()` (static, internal)
- **Engine Headers**: YES - includes MOX_BASE, MOX_DEF, MOX_KEYS, Video headers
- **Naming**: 
  - VIOLATION: `kilgore_video_get_window_id()`, `kilgore_mouse_*()` should use Proper_Snake_Case (should start with capital letter or be in Platform.h)
  - Static functions are private implementation details
- **Globals Exposed**:
  - `sdl2_window`, `sdl2_renderer`, `sdl2_surface_RGB666`, `sdl2_surface_ARGB8888`, `sdl2_texture`, `sdl2_texture_upscaled` (extern in sdl2_PFL.h)
  - `sdl2_window_title[]` (extern)
  - `sdl2_blit_rect` (extern)
  - `sdl2_ticks_startup`, `sdl2_frame_count`, `sdl2_ticks_mark_time`, `sdl2_ticks_release_time` (extern)
  - `sdl2_performance_counter` (extern)
  - `platform_frame_mouse_buttons` (in Platform.h)
  - `kilgore_option_mouse_speed`, `kilgore_mouse_grabbed` (internal)

#### **sdl2_PFL.h**
- **Backend-Private vs Engine-Exposed**: BACKEND-PRIVATE (SDL-specific declarations)
  - Declares SDL_Window, SDL_Renderer, SDL_Surface, SDL_Texture globals
  - Declares `Platform_Event_Handler()` and other functions that ARE in Platform.h
  - Contains BOOLVEC and utility macros (internal)
  - Declares `Platform_Window_Event(SDL_WindowEvent*)` - **BOUNDARY VIOLATION**

#### **sdl2_SCCC.h** & **sdl2_SDLK.c**
- **Purpose**: Translation tables from SDL key codes to SCCC (Scan Code / Character Code)
- **Functions**: None; just data tables
- **Naming**: `SDLK_To_SCCC[]` follows conventions
- **Boundary**: backend-internal translation

#### **sdl2_State.c**
- **Platform.h Functions**: None; defines extern globals declared in Platform.h
- **Definitions**:
  - `quit_game_flag`
  - `key_pressed`
  - `scan_code_char_code`
  - `platform_palette_buffer[256]`
  - `lock_mouse_button_status_flag`
  - `platform_mouse_input_enabled`
- **Correctly includes**: `../../platform/include/Platform.h`

#### **sdl2_Timer.c**
- **Platform.h Functions Implemented**:
  - `Read_System_Clock_Timer()`
  - `Mark_Time()` (game-level, via Timer.h)
  - `Release_Time()` (game-level)
  - `Release_Time_Seconds()` (game-level)
- **SDL2 APIs Used**: `SDL_GetTicks()`, `SDL_PumpEvents()`
- **Non-Platform.h Functions**: Game-level functions (not in Platform.h)
- **Engine Headers**: YES - includes Timer.h, Video.h
- **Naming**: Correct

#### **sdl2_Video.c**
- **Platform.h Functions Implemented**:
  - `Platform_Palette_Update()`
  - `Platform_Set_Palette_Color()`
  - `Platform_Video_Update()`
- **SDL2 APIs Used**: `SDL_SetPaletteColors()`, `SDL_LockSurface()`, `SDL_UnlockSurface()`, `SDL_LockTexture()`, `SDL_UnlockTexture()`, `SDL_LowerBlit()`, `SDL_RenderCopy()`, `SDL_RenderPresent()`
- **Non-Platform.h Functions**: None
- **Engine Headers**: YES - includes Fonts.h, MOX_BASE, Video.h, Video2.h
- **Naming**: Correct
- **Correctly includes**: `../../platform/include/Platform.h`

#### **MOM_PFL.h**
- **Purpose**: Legacy MoX-side declarations for platform functions
- **Issues**: 
  - Contains duplicate/outdated declarations
  - Mixes SDL2 platform (`#ifdef _STU_SDL2`) with Win32 (`#ifdef _STU_WIN`)
  - Some declarations reference non-existent functions
  - Should be retired in favor of direct Platform.h inclusion

#### **PFL_Audio_Internal.h**
- **Purpose**: Internal definitions for audio backend (byte macros, constants)
- **Design**: Good - avoids platform -> engine dependency by duplicating macros instead of including MoX headers
- **No Boundary Issues**: Declares only forward refs to engine callbacks

---

### Summary of Issues Found

#### **BOUNDARY VIOLATIONS (SDL types crossing platform boundary)**
1. **sdl2_KD.h**: `Platform_Keyboard_Event(SDL_Event * sdl2_event)` - declares SDL_Event* in public interface
2. **sdl2_MOM.h**: `Platform_Keyboard_Event(SDL_Event * sdl2_event)` - duplicate SDL_Event* exposure
3. **sdl2_PFL.h**: `Platform_Window_Event(SDL_WindowEvent *sdl2_window_event)` - SDL type in public interface
4. **sdl2_PFL.c** line 302: `Platform_Window_Event()` implementation accepts SDL_WindowEvent*

#### **ENGINE HEADER DEPENDENCIES (inappropriate direct includes)**
1. **sdl2_Audio.c**: includes `../../STU/src/STU_DBG.h` (STU is debug layer, acceptable for now)
2. **Most .c files**: include MOX headers (appropriate for implementation, not ideal for platform-specific code)

#### **NAMING CONVENTION VIOLATIONS**
1. **sdl2_KD.c**:
   - `sdl2_get_key_code_name()` → should be `Sdl2_Get_Key_Code_Name()` or removed (not in Platform.h)
   - `mod_xlat()` → should be `Mod_Xlat()` or made static/private
   - `kbd_set_pressed()` → should be `Kbd_Set_Pressed()` or made static/private
   - `kbd_is_pressed()` → should be `Kbd_Is_Pressed()` or made static/private

2. **sdl2_PFL.c**:
   - `kilgore_video_get_window_id()` → inconsistent naming (starts lowercase, kilgore prefix)
   - `kilgore_mouse_*()` → all should start with uppercase or be made static
   - These are static functions, so internal naming is less critical, but still violates style

3. **sdl2_State.c** (typo in sdl2_Init.c):
   - Function `Shudown_Platform()` is misspelled (should be `Shutdown_Platform()`)

#### **HEADER ORGANIZATION ISSUES**
1. **MOM_PFL.h**: Legacy mixed Win32/SDL2 declarations; should be removed in favor of Platform.h
2. **sdl2_MOM.h**: Duplicate declarations that should only be in Platform.h
3. **sdl2_Audio.h**: Exposes backend functions that shouldn't be public API

#### **MISSING FROM PLATFORM.H BUT NEEDED**
- `Platform_Get_Scale()` - implemented but should be declared in Platform.h
- Backend-level functions like `build_key_xlat()` that are called from init

#### **GLOBALS NOT EXPORTED IN PLATFORM.H**
- SDL-specific globals (sdl2_window, sdl2_renderer, etc.) in sdl2_PFL.h are appropriate for backend-only
- However, they should not be accessible from engine code except through Platform.h functions

---

### Recommendations

1. **Remove SDL types from Platform.h boundary**: `Platform_Keyboard_Event()` and `Platform_Window_Event()` should use opaque types or callback patterns
2. **Consolidate platform declarations**: Retire MOM_PFL.h and sdl2_MOM.h; all public functions should be declared in Platform.h only
3. **Fix naming violations**: Make `kilgore_*` and utility functions either static or properly named
4. **Fix typo**: `Shudown_Platform()` → `Shutdown_Platform()`
5. **Audit engine header includes**: sdl2_PFL.c includes many MOX headers that it doesn't need to use directly

**Files reviewed**: sdl2_Audio.c, sdl2_Audio.h, sdl2_EMM.c, sdl2_Init.c, sdl2_Input.c, sdl2_KD.c, sdl2_KD.h, sdl2_MD.c, sdl2_MOM.h, sdl2_PFL.c, sdl2_PFL.h, sdl2_SCCC.h, sdl2_SDLK.c, sdl2_SDLK.h, sdl2_State.c, sdl2_Timer.c, sdl2_Video.c, MOM_PFL.h, PFL_Audio_Internal.h

**Absolute file paths**:
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_Audio.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_Audio.h
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_EMM.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_Init.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_Input.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_KD.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_KD.h
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_MD.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_MOM.h
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_PFL.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_PFL.h
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_SCCC.h
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_SDLK.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_SDLK.h
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_State.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_Timer.c
- /c/STU/devel/ReMoM/platform/sdl2/sdl2_Video.c
- /c/STU/devel/ReMoM/platform/sdl2/MOM_PFL.h
- /c/STU/devel/ReMoM/platform/sdl2/PFL_Audio_Internal.h
</output>
TaskOutput task: "a6fc6e30eb16dec1f"
OUT
<retrieval_status>success</retrieval_status>

<task_id>a6fc6e30eb16dec1f</task_id>

<task_type>local_agent</task_type>

<status>completed</status>

<output>
Now let me compile a comprehensive report. Based on my review of all the files, here is my analysis:

## Comprehensive SDL3 Platform Layer Audit Report

### File-by-File Analysis

#### **sdl3_Audio.c**
**Platform.h Functions Implemented:**
- None directly (audio is not part of the public Platform.h interface)

**SDL3-Specific APIs Used:**
- `SDL3_mixer/SDL_mixer.h` for audio playback (Mix_LoadWAV, Mix_PlayChannel, Mix_Music)
- Direct references to SDL3 mixer APIs

**Boundary Violations (Not in Platform.h):**
- `sdl2_Audio_Init()`, `sdl2_Audio_Deinit()` - declared in sdl2_Audio.h
- `Play_Sound_SDL2_Mixer()` - internal helper
- `Convert_VOC_To_WAV()` - audio conversion utility
- `hw_audio_music_*()` functions (release, play, fadeout, stop, volume) - music control suite
- `fmt_mus_convert_xmid()` - music format conversion

**Engine Header Includes:**
- MoX headers: EMM.h, MOX_DEF.h, MOX_TYPE.h included indirectly
- STU headers: STU_DBG.h (debug logging)
- Audio implementation relies on engine-level sound structures

**Naming Issues:**
- Naming appears correct (Proper_Snake_Case for functions like `Play_Sound_SDL2_Mixer`, `hw_audio_music_play`)
- However, variables use lower_snake_case correctly

**Exposed Globals:**
- `audio_initialized` (int16_t) - from SOUND.C, extern'd
- `ui_data_mus` (uint8_t*) - music data pointer
- `ui_data_music_i` (int) - music index
- `opt_music_enabled` (bool) - configuration state
- `opt_music_volume` (int) - configuration state
- `mustbl` (struct mus_s) - music playback table

#### **sdl3_EMM.c**
**Platform.h Functions Implemented:**
- None

**SDL3-Specific APIs Used:**
- None (standard C library only)

**Boundary Violations:**
- `EMS_Startup()` - declares only this function; no Platform.h equivalent

**Engine Header Includes:**
- MoX headers: EMM.h, MOX_DEF.h, MOX_TYPE.h (engine memory management)

**Globals:**
- `_VGAFILEH_seg` - external VGA file handle buffer

#### **sdl3_Init.c**
**Platform.h Functions Implemented:**
- `Startup_Platform()` - initializes SDL3, window, renderer, surfaces
- `Shudown_Platform()` (note: typo "Shudown" not "Shutdown") - cleanup
- `Platform_Mouse_Input_Enable()`
- `Platform_Mouse_Input_Disable()`

**SDL3-Specific APIs Used:**
- `SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)`
- `SDL_GetTicks()`
- `SDL_GetPerformanceCounter()`
- `SDL_CreateWindow()`
- `SDL_GetWindowSize()`
- `SDL_SetWindowPosition(SDL_WINDOWPOS_CENTERED)`
- `SDL_CreateRenderer()`
- `SDL_SetRenderVSync()`
- `SDL_SetWindowTitle()`
- `SDL_CreateSurface()` with SDL_PIXELFORMAT_INDEX8 and SDL_PIXELFORMAT_ARGB8888
- `SDL_CreatePalette()`, `SDL_SetSurfacePalette()`, `SDL_DestroyPalette()`
- `SDL_CreateTexture()`
- `SDL_HideCursor()`, `SDL_ShowCursor()`
- SDL3_mixer initialization

**Boundary Violations:**
- None (all proper Platform.h functions)

**Engine Header Includes:**
- Multiple MoX headers: Mouse.h, MOX_DEF.h, MOX_TYPE.h, sdl2_Audio.h
- STU headers: STU_DBG.h
- Platform internal headers: sdl2_KD.h, sdl2_MOM.h, sdl2_PFL.h, sdl2_SDLK.h

**Naming Issues:**
- `Shudown_Platform()` - **TYPO**: should be `Shutdown_Platform()` (Proper_Snake_Case violation)
- `build_key_xlat()` called here - lowercase (should follow Proper_Snake_Case)

**Issue:**
- Calls `build_key_xlat()` from sdl3_KD.c but function name violates naming convention

#### **sdl3_Input.c**
**Platform.h Functions Implemented:**
- None directly (Get_Input is from original game, not Platform.h)

**SDL3-Specific APIs Used:**
- None directly (delegates to Platform_Event_Handler)

**Boundary Violations:**
- `Get_Input()` - game-level input function, not platform-specific

**Engine Header Includes:**
- Multiple MoX headers: Fields.h, Input.h, MOX_DEF.h, MOX_TYPE.h, Video.h

#### **sdl3_KD.c** (Keyboard Driver)
**Platform.h Functions Implemented:**
- `Platform_Keyboard_Event()` - handles SDL_EVENT_KEY_DOWN/UP, SDL_EVENT_TEXT_INPUT
- `Platform_Keyboard_Buffer_Clear()`
- `Platform_Keyboard_Buffer_Add_Key_Press()`
- `Platform_Keyboard_Buffer_Pending_Count()`
- `Platform_Keyboard_Buffer_Peek_Latest()`

**SDL3-Specific APIs Used:**
- `SDL_GetScancodeName()`
- `SDL_GetKeyName()`
- `SDL_EVENT_KEY_DOWN`, `SDL_EVENT_KEY_UP`, `SDL_EVENT_TEXT_INPUT`
- `SDL_Scancode`, `SDL_Keycode`, `SDL_Keymod` enums
- `SDL_SCANCODE_COUNT` for array bounds
- `SDL_KMOD_SHIFT | SDL_KMOD_CTRL | SDL_KMOD_ALT` modifier masks
- `SDL_StartTextInput()`, `SDL_StopTextInput()`

**Boundary Violations (Not in Platform.h):**
- `Keyboard_Status()` - game-level function
- `Read_Key()` - game-level function
- `build_key_xlat()` - **NAMING ISSUE**: lower_snake_case instead of Proper_Snake_Case
- `mod_xlat()` - **NAMING ISSUE**: lower_snake_case instead of Proper_Snake_Case
- `kbd_set_pressed()` - **NAMING ISSUE**: lower_snake_case
- `kbd_is_pressed()` - **NAMING ISSUE**: lower_snake_case
- `sdl2_get_key_code_name()` - sdl2 prefix for SDL3 code
- `hw_textinput_start()` - **NAMING ISSUE**: lower_snake_case
- `hw_textinput_stop()` - **NAMING ISSUE**: lower_snake_case
- `Platform_Translated_Key()` - static helper
- `Platform_Localized_Key()` - static helper
- `Platform_Typed_mox_character()` - static helper

**Engine Header Includes:**
- Multiple MoX headers: Keyboard.h, MOX_DEF.h, MOX_KEYS.h, MOX_TYPE.h
- External: stu_compat.h (external utilities)

**Naming Issues:**
- **Critical**: `build_key_xlat()`, `mod_xlat()`, `kbd_set_pressed()`, `kbd_is_pressed()`, `hw_textinput_start()`, `hw_textinput_stop()` all violate Proper_Snake_Case convention
- `sdl2_get_key_code_name()` - still uses sdl2 prefix despite being SDL3 code

**Globals:**
- `sdl2_key_code_name` (char[4096]) - static buffer for key names
- `key_xlat` (static int[SDL_SCANCODE_COUNT]) - scancode to MOX_KEY translation table (correctly sized for SDL3)
- `hw_textinput_active` (static bool) - text input mode flag
- `platform_keyboard_buffer` (struct s_KEYBOARD_BUFFER) - extern from Platform
- `sdl2_window` - extern reference

#### **sdl3_MD.c** (Mouse Driver)
**Platform.h Functions Implemented:**
- None directly in public interface

**SDL3-Specific APIs Used:**
- `SDL_GetMouseState()`
- `SDL_BUTTON_LMASK`, `SDL_BUTTON_RMASK` - button state masks
- `SDL_WarpMouseInWindow()`

**Boundary Violations:**
- `Mouse_Button()` - game-level function
- `Mouse_Movement_Handler()`, `Mouse_Button_Handler()` - game-level handlers
- `User_Mouse_Handler()` - game-level handler
- `Set_Mouse_Position()` - game-level function

**Engine Header Includes:**
- Multiple MoX headers: Mouse.h, MOX_DEF.h, MOX_TYPE.h
- Reference to sdl2_Audio.h in includes (inappropriate?)

**Naming Issues:**
- Uses `Proper_Snake_Case` consistently for public functions

#### **sdl3_PFL.c** (Main Platform Loop & Event Handling)
**Platform.h Functions Implemented:**
- `Platform_Get_Scale()`
- `Platform_Get_Millies()`
- `Platform_Sleep_Millies()`
- `Platform_Get_Mouse_Position_XY()`
- `Platform_Maybe_Move_Mouse()`
- `Platform_Event_Handler()` - **CRITICAL FUNCTION**: main event loop

**SDL3-Specific APIs Used:**
- `SDL_GetMouseState()` with pointer casts
- `SDL_GetWindowID()`
- `SDL_GetTicks()` for millisecond timing
- `SDL_Delay()` for sleep
- `SDL_ShowCursor()`, `SDL_HideCursor()`
- `SDL_PumpEvents()`, `SDL_PollEvent()`, `SDL_PeepEvents()`
- `SDL_EVENT_*` event types (KEY_DOWN, KEY_UP, MOUSE_BUTTON_DOWN/UP, MOUSE_MOTION, QUIT, WINDOW_*)
- `SDL_PushEvent()` for event injection (replay support)
- Extensive replay injection and event filtering

**Boundary Violations (Not in Platform.h):**
- `kilgore_mouse_grab()` - static helper
- `kilgore_mouse_ungrab()` - static helper
- `kilgore_mouse_toggle_grab()` - static helper
- `kilgore_mouse_set_limits()` - static helper
- `kilgore_mouse_set_scale()` - static helper
- `kilgore_mouse_move()` - static helper
- `kilgore_video_get_window_id()` - static helper
- `kilgore_mouse_set_xy_from_hw()` - static helper
- `Platform_Update_Mouse_Position()` - static helper
- `Platform_Window_Event()` - static helper (called from Platform_Event_Handler)
- `DBG_Frame_Reset()` - debug helper (EXPOSED as public)

**Engine Header Includes:**
- Multiple MoX headers: MOX_BASE.h, MOX_DEF.h, MOX_KEYS.h, MOX_TYPE.h, Mouse.h, Video.h
- Platform_Replay.h - record/replay support

**Naming Issues:**
- Functions starting with `kilgore_` should use Proper_Snake_Case (e.g., `Kilgore_Mouse_Grab()`)
- Debug function `DBG_Frame_Reset()` exposed (should be static?)

**Globals Defined:**
- `sdl2_window` (SDL_Window*) - main window
- `sdl2_renderer` (SDL_Renderer*)
- `sdl2_surface_RGB666` (SDL_Surface*)
- `sdl2_surface_ARGB8888` (SDL_Surface*)
- `sdl2_texture` (SDL_Texture*)
- `sdl2_texture_upscaled` (SDL_Texture*) - unused upscaling texture
- `platform_screen_scale` (int) - initialized to 4
- `sdl2_window_width` (int)
- `sdl2_window_height` (int)
- `sdl2_window_title` (char[]) - "[MoM-Rasm] Master of Magic v1.31 - Reassembly"
- `sdl2_blit_rect` (SDL_Rect)
- `sdl2_ticks_startup`, `sdl2_frame_count`, `sdl2_ticks_mark_time`, `sdl2_ticks_release_time` (uint64_t)
- `sdl2_performance_counter` (uint64_t)
- `platform_frame_mouse_buttons` (int16_t) - per-frame click flag
- `kilgore_option_mouse_speed` (int) - mouse speed configuration
- `kilgore_mouse_grabbed` (static bool)
- `kilgore_option_relative_mouse` (static bool)
- Multiple static `kilgore_mouse_*` variables for mouse state management

#### **sdl3_SDLK.c**
**Platform.h Functions Implemented:**
- None (data only)

**SDL3-Specific APIs Used:**
- None (static key translation tables)

**Boundary Violations:**
- Defines `SDLK_to_SCCS[256]` (note: typo "SCCS" instead of "SCCC") - key translation table

**Naming Issues:**
- Array name `SDLK_to_SCCS` should follow UPPER_SNAKE_CASE (currently does)
- However, the suffix is misspelled/inconsistent: should be `SDLK_to_SCCC` (matching SCCC naming elsewhere)

#### **sdl3_State.c**
**Platform.h Functions Implemented:**
- None (state variable definitions only)

**SDL3-Specific APIs Used:**
- None (pure C definitions)

**Boundary Violations:**
- None (all state variables are extern in Platform.h)

**Engine Header Includes:**
- Only Platform.h (correct boundary)

**Globals Defined:**
- `quit_game_flag` (int)
- `key_pressed` (int8_t) - from SOUND.C
- `scan_code_char_code` (uint16_t)
- `platform_palette_buffer` (PFL_Color[256]) - VGA DAC buffer
- `lock_mouse_button_status_flag` (int)
- `platform_mouse_input_enabled` (int)

#### **sdl3_Timer.c**
**Platform.h Functions Implemented:**
- `Platform_Get_Millies()` - returns SDL_GetTicks() * 1000
- `Platform_Sleep_Millies()`

**SDL3-Specific APIs Used:**
- `SDL_GetTicks()` for millisecond timing
- `SDL_Delay()` for sleep
- `SDL_PumpEvents()` - keep event queue alive during waits

**Boundary Violations (Not in Platform.h):**
- `Read_System_Clock_Timer()` - game-level timing function
- `Mark_Time()` - game-level timing function
- `Release_Time()` - game-level timing function
- `Release_Time_Seconds()` - game-level timing function

**Engine Header Includes:**
- Multiple MoX headers: Mouse.h, MOX_DEF.h, MOX_TYPE.h, Timer.h, Video.h

#### **sdl3_Video.c**
**Platform.h Functions Implemented:**
- `Platform_Palette_Update()` - copies MoX palette to platform hardware palette
- `Platform_Set_Palette_Color()` - updates single palette entry
- `Platform_Video_Update()` - main video refresh function

**SDL3-Specific APIs Used:**
- `SDL_GetSurfacePalette()`, `SDL_SetSurfacePalette()`, `SDL_SetPaletteColors()`
- `SDL_LockSurface()`, `SDL_UnlockSurface()`
- `SDL_LockTexture()`, `SDL_UnlockTexture()`
- `SDL_BlitSurface()` - 8-bit to 32-bit color conversion
- `SDL_RenderTexture()` - render texture to window
- `SDL_RenderPresent()` - flip back buffer
- Proper SDL3 surface/texture pipeline

**Boundary Violations:**
- None (all proper Platform.h functions)

**Engine Header Includes:**
- Multiple MoX headers: Fonts.h, MOX_BASE.h, MOX_TYPE.h, Video.h, Video2.h
- sdl2_PFL.h (for extern declarations)

**Naming Issues:**
- Calls `DBG_Frame_Reset()` at end of `Platform_Video_Update()` (debug instrumentation)

---

### Header Files Analysis

#### **MOM_PFL.h**
**Purpose:** Platform-specific declarations for MoM engine integration

**Issues:**
- Uses SDL_Color from SDL3 directly (appropriate)
- Declares both old and new globals
- Mix of SDL2/SDL3 era naming
- Conditionally declares `platform_palette_buffer` as either SDL_Color[] (SDL3) or uint8_t[] (Windows)
- Contains WIN32-specific declarations alongside SDL code

#### **PFL_Audio_Internal.h**
**Purpose:** Internal audio helper definitions

**Issues:**
- Properly isolated from engine (good encapsulation)
- Contains only utility macros and type stubs
- Avoids forward declaring actual audio functions

#### **sdl2_Audio.h**
**Purpose:** Audio subsystem interface

**Issues:**
- Still uses "sdl2" prefix despite being SDL3 code
- Declares: `sdl2_Audio_Init()`, `sdl2_Audio_Deinit()`
- Includes SDL3_mixer (correct)

#### **sdl2_KD.h** (Keyboard Driver Header)
**Purpose:** Keyboard definitions and key translation

**Issues:**
- Still uses "sdl2" prefix despite SDL3 code
- Defines MOX_MOD_* constants (correct Proper_Snake_Case)
- Defines enum e_MOX_KEY with all keyboard codes
- Declares `Platform_Keyboard_Event()`, `Platform_Keyboard_Buffer_Add_Key_Press()`, etc.
- Naming violation in helper function declarations: `build_key_xlat()` - should be `Build_Key_Xlat()`

#### **sdl2_MOM.h**
**Purpose:** MoM-specific platform functions

**Issues:**
- Still uses "sdl2" prefix
- Declares mouse input enable/disable functions
- Declares keyboard event handlers

#### **sdl2_PFL.h**
**Purpose:** Platform low-level definitions

**Issues:**
- Still uses "sdl2" prefix
- Declares SDL object externs (sdl2_window, sdl2_renderer, etc.) - these should probably be in a proper Platform.h internal header
- Declares timing functions with proper Proper_Snake_Case
- Includes boolvec and comparison macros from 1oom code
- Declares `Platform_Event_Handler()` correctly

#### **sdl2_SCCC.h**
**Purpose:** SCCC key translation table (Scan Code / Character Code)

**Issues:**
- Large static table of key translations
- Maps index to SCCC values
- Comments reference Windows VK_* codes

#### **sdl2_SDLK.h**
**Purpose:** SDLK key translation table

**Issues:**
- Declares extern `SDLK_to_SCCS[256]` - note typo "SCCS" instead of "SCCC"
- Table defined in sdl3_SDLK.c with same typo

---

### Critical Issues Summary

1. **Typo in Function Name**: `Shudown_Platform()` should be `Shutdown_Platform()` in sdl3_Init.c

2. **Naming Convention Violations** (should use Proper_Snake_Case):
   - `build_key_xlat()` → `Build_Key_Xlat()`
   - `mod_xlat()` → `Mod_Xlat()` or similar
   - `kbd_set_pressed()` → `Kbd_Set_Pressed()`
   - `kbd_is_pressed()` → `Kbd_Is_Pressed()`
   - `sdl2_get_key_code_name()` → `Sdl2_Get_Key_Code_Name()` or `Get_Key_Code_Name()`
   - `hw_textinput_start()` → `Hw_Textinput_Start()`
   - `hw_textinput_stop()` → `Hw_Textinput_Stop()`
   - Various `kilgore_*` functions should use Proper_Snake_Case

3. **SDL2-era Naming Still Present**: Many files/headers still use "sdl2_" prefix despite being SDL3 code:
   - sdl2_Audio.h, sdl2_KD.h, sdl2_MOM.h, sdl2_PFL.h, sdl2_SCCC.h, sdl2_SDLK.h
   - Variables: sdl2_window, sdl2_renderer, sdl2_surface_*, sdl2_texture, sdl2_*

4. **Engine Header Includes**: Some files directly include MoX headers (appropriate for implementation files, but may need isolation review):
   - sdl3_PFL.c includes: MOX_BASE.h, MOX_DEF.h, MOX_KEYS.h, MOX_TYPE.h, Mouse.h, Video.h
   - sdl3_KD.c includes: Keyboard.h, MOX_DEF.h, MOX_KEYS.h, MOX_TYPE.h
   - sdl3_Video.c includes: Fonts.h, MOX_BASE.h, MOX_TYPE.h, Video.h, Video2.h
   - sdl3_Timer.c includes: Mouse.h, MOX_DEF.h, MOX_TYPE.h, Timer.h, Video.h

5. **Exposed Globals Not in Platform.h**:
   - `kilgore_mouse_grabbed`, `kilgore_option_relative_mouse`, `kilgore_option_mouse_speed`
   - Multiple static debug counters in sdl3_PFL.c
   - SDL object externs (sdl2_window, sdl2_renderer, etc.) declared in sdl2_PFL.h but not formalized in Platform.h

6. **Duplicate/Inconsistent Naming**:
   - `SDLK_to_SCCS` in sdl2_SDLK.h and sdl3_SDLK.c (should be `SDLK_to_SCCC` - inconsistent with SCCC naming)
   - `SDLK_To_SCCC` exists as separate table (naming inconsistency)

7. **Boundary Violations**: Several game-level functions implemented in platform code:
   - `Get_Input()`, `Keyboard_Status()`, `Read_Key()` in sdl3_KD.c/sdl3_Input.c
   - `Mouse_Button()`, `Mouse_Movement_Handler()`, `User_Mouse_Handler()` in sdl3_MD.c
   - `Mark_Time()`, `Release_Time()`, `Release_Time_Seconds()` in sdl3_Timer.c

8. **Public Debug Function**: `DBG_Frame_Reset()` exported from sdl3_PFL.c (should be static or in a separate debug interface)

9. **Configuration State Mixed in**: `opt_music_enabled`, `opt_music_volume` defined in sdl3_Audio.c (should these be in a configuration module?)

---

### Summary of SDL3 API Coverage

The SDL3 migration appears reasonably complete:
- Window/renderer creation using SDL3 APIs
- Event handling using SDL_EVENT_* types
- Key handling using SDL_SCANCODE_ for key translation
- Mouse using SDL_GetMouseState and SDL_WarpMouseInWindow
- Palette and texture management using SDL3 surface/palette API
- Audio using SDL3_mixer

Main remaining SDL2-era artifacts are naming (variable/file names with "sdl2_" prefix) and inconsistent header naming conventions.
</output>
TaskOutput task: "af1b23bf1872de161"
OUT
<retrieval_status>success</retrieval_status>

<task_id>af1b23bf1872de161</task_id>

<task_type>local_agent</task_type>

<status>completed</status>

<output>
Perfect! Now I have all the information I need. Let me compile my comprehensive report.

## SDL2 Legacy Files Analysis Report

Based on my thorough review of all SDL2 files in c:\STU\devel\ReMoM\MoX\src/, here's the detailed analysis:

### File: sdl2_Audio.h / sdl2_Audio.c

**Functions Defined:**
- `sdl2_Audio_Init()` - Initialization (header only)
- `sdl2_Audio_Deinit()` - Cleanup (header only)
- Large implementation file with music and audio handling (SDL_mixer integration)

**Platform.h Overlap:**
- **NO direct overlap** - Platform.h has no audio functions declared. Audio is not part of the formal platform interface.

**SDL Headers:**
- Yes, directly includes `<SDL_mixer.h>` (conditional `NO_SOUND_LIBRARY`)

**Still Referenced:**
- Referenced in: SOUND.c, SOUND.h, sdl2_MOM.c, sdl2_MD.c
- Used for audio initialization

**Actively Compiled:**
- **NO** - Not listed in MoX/src/CMakeLists.txt

---

### File: sdl2_boolvec.h

**Functions Defined:**
- Macro-only header, no functions
- Defines: BOOLVEC_DECLARE, BOOLVEC_CLEAR, BOOLVEC_COPY, BOOLVEC_SET, BOOLVEC_IS1, etc.

**Platform.h Overlap:**
- **NO** - Utility macros, no platform declarations

**SDL Headers:**
- **NO** - Only includes `<string.h>` and sdl2_types.h

**Still Referenced:**
- Referenced in: sdl2_PFL.c, sdl2_MOM.c
- Used for keyboard buffer and pressed key tracking

**Actively Compiled:**
- **NO** - Header only, indirectly included through other sdl2_* files

---

### File: sdl2_EMM.c

**Functions Defined:**
- `EMS_Startup()` - Memory buffer allocation (historical: IBM-PC EMS compatibility)

**Platform.h Overlap:**
- **YES** - `void EMS_Startup(void);` declared in Platform.h (line 206)

**SDL Headers:**
- **NO** - Includes standard C headers only

**Still Referenced:**
- Referenced in: MOM_PFL.h, Platform.h
- Also has parallel: win_EMS.c for Windows platform

**Actively Compiled:**
- **NO** - Not listed in MoX/src/CMakeLists.txt

---

### File: sdl2_Input.c

**Functions Defined:**
- `Get_Input()` - Returns field index/input state (WZD s36p66)

**Platform.h Overlap:**
- **NO** - Platform.h doesn't declare Get_Input(). Input is handled through Platform_Event_Handler() and Platform_Keyboard_Buffer_*

**SDL Headers:**
- **NO** - No direct SDL includes

**Still Referenced:**
- Referenced in: Input.c, Input.h, Fields.c, Fields.h, GENDRAW.c, TEXTBOX.h, Util.c
- Calls: Platform_Event_Handler(), Interpret_Mouse_Input()

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_KD.h / sdl2_KD.c

**Functions Defined:**
- `Keyboard_Status()` - Query if key is available
- `Read_Key()` - Read next key from buffer (returns kilgore_key value with mods)
- `Platform_Keyboard_Event()` - Handle SDL keyboard events
- `Platform_Keyboard_Buffer_Clear()` - Clear buffer
- `Platform_Keyboard_Buffer_Add_Key_Press()` - Add key to buffer
- `Platform_Translate_Virtual_Key_Code_To_MOX_Key_Num()` - Key translation
- `build_key_xlat()` - Build translation tables

**Platform.h Overlap:**
- **PARTIAL** - Platform.h declares:
  - `Platform_Keyboard_Buffer_Clear()` (line 170)
  - `Platform_Keyboard_Buffer_Add_Key_Press()` (line 178)
  - But NOT `Read_Key()` or `Keyboard_Status()` (these are MoX engine functions, not platform interface)

**SDL Headers:**
- Yes, includes `<SDL.h>` directly

**Still Referenced:**
- Referenced in: Input.c, Keyboard.c, sdl2_PFL.h, sdl2_PFL.c, sdl2_MOM.c, MOM_PFL.c, MOM_PFL.h, sdl2_MOM.h
- Extensive keyboard buffer management and key translation

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_MD.c

**Functions Defined:**
- `Mouse_Button()` - Get current mouse button state (WZD s35p05)
- `Mouse_Movement_Handler()` - Lock button status flag
- `Mouse_Button_Handler()` - Release button status flag
- `User_Mouse_Handler()` - Handle mouse input and position updates

**Platform.h Overlap:**
- **PARTIAL** - Platform.h declares:
  - `Platform_Get_Mouse_Position_XY()` (line 213)
  - `Platform_Maybe_Move_Mouse()` (line 219)
  - `Update_Mouse_Position()` (line 226)
  - `Update_Mouse_Button_Status()` (line 234)
  - But NOT `Mouse_Button()` or user handlers (these are game-layer functions)

**SDL Headers:**
- Yes, includes `<SDL.h>` and sdl2_Audio.h

**Still Referenced:**
- Referenced in: Input.c, Mouse.c, Mouse.h, Fields.c, sdl2_PFL.c, sdl2_Timer.c, win_MD.c, PoC_PFL.h, MOM_PFL.h

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_MOM.h / sdl2_MOM.c

**Functions Defined (sdl2_MOM.c):**
- `Startup_Platform()` - Initialize platform (window, renderer, surfaces, audio)
- Many internal implementation details

**Functions Declared (sdl2_MOM.h):**
- `Platform_Mouse_Input_Enable()`
- `Platform_Mouse_Input_Disable()`
- `Platform_Keyboard_Event()`
- `Platform_Keyboard_Buffer_Clear()`

**Platform.h Overlap:**
- **YES** - Platform.h declares:
  - `Startup_Platform()` (line 74) - matches MoM_PFL.c definition
  - `Platform_Mouse_Input_Enable()` (line 164)
  - `Platform_Mouse_Input_Disable()` (line 167)
  - `Platform_Event_Handler()` (line 161) - called from sdl2_MOM.c

**SDL Headers:**
- Yes, includes `<SDL.h>` and `<SDL_mixer.h>`

**Still Referenced:**
- sdl2_MOM.h: Referenced in sdl2_PFL.c, sdl2_PFL.h
- sdl2_MOM.c: References sdl2_Audio.h, sdl2_KD.h, sdl2_SDLK.h, sdl2_SCCC.h

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_PFL.h / sdl2_PFL.c

**Functions Defined (sdl2_PFL.c):**
- `Platform_Get_Millies()` - Get milliseconds since startup
- `Platform_Sleep_Millies()` - Sleep for N milliseconds
- `Platform_Get_Mouse_Position_XY()` - Get current mouse coordinates
- `Platform_Maybe_Move_Mouse()` - Update cursor position and display
- `Platform_Window_Event()` - Handle window events
- `hw_textinput_start()` / `hw_textinput_stop()` - Text input control

**Platform.h Overlap:**
- **YES** - Almost complete overlap:
  - `Platform_Get_Millies()` (line 245)
  - `Platform_Sleep_Millies()` (line 250)
  - `Platform_Get_Mouse_Position_XY()` (line 213)
  - `Platform_Maybe_Move_Mouse()` (line 219)
  - `Platform_Event_Handler()` (line 161)
  - `hw_textinput_start()` / `hw_textinput_stop()` (lines 192-195)
  - Platform.h declares these; sdl2_PFL.c implements them

**SDL Headers:**
- **ERROR**: Line 11 includes `<SDL3/SDL.h>` (note: SDL3, not SDL2!) - file is misnamed or version-mismatched

**Still Referenced:**
- Heavily referenced: sdl2_PFL.h is included in sdl2_KD.c, sdl2_Input.c, sdl2_MD.c, sdl2_MOM.c, sdl2_MOM.h, sdl2_Timer.c, MOM_PFL.c

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_SCCC.h

**Functions Defined:**
- This file is **actually implementation code**, not a header
- Defines: `SDLK_To_SCCC[256]` - lookup table for SDL key to Scan Code / Character Code translation

**Platform.h Overlap:**
- **NO** - This is a translation table utility, not a platform interface function

**SDL Headers:**
- **NO** - Only includes input.h and MOX_TYPE.h

**Still Referenced:**
- Referenced in: sdl2_PFL.c (includes as header)
- Used for keyboard key code translation

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt; included as header-only

---

### File: sdl2_SDLK.h / sdl2_SDLK.c

**Functions Defined:**
- `sdl2_SDLK.c` defines: `SDLK_to_SCCS[256]` - Another SDL-to-SCCS translation table
- No actual functions, just data

**Platform.h Overlap:**
- **NO** - Utility data table

**SDL Headers:**
- **NO** - sdl2_SDLK.c only includes Input.h and MOX_TYPE.h

**Still Referenced:**
- Referenced in: sdl2_MOM.c (includes sdl2_SDLK.h)

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_Timer.c

**Functions Defined:**
- `Read_System_Clock_Timer()` - Get system ticks in microseconds
- `Mark_Time()` - Mark reference time point
- `Release_Time()` - Wait N ticks with event processing
- `Release_Time_Seconds()` - Wait N microseconds with event processing

**Platform.h Overlap:**
- **YES** - Platform.h declares:
  - `Read_System_Clock_Timer()` (line 256)
  - But NOT `Mark_Time()`, `Release_Time()`, `Release_Time_Seconds()` (these are game-layer timing functions from original assembly)

**SDL Headers:**
- Yes, includes `<SDL.h>`

**Still Referenced:**
- Referenced in: Input.c, Util.c, random.c, MOX_T4.c, Fields.c, Timer.h, random.h, win_CLK.c

**Actively Compiled:**
- **NO** - Not in CMakeLists.txt

---

### File: sdl2_types.h

**Functions Defined:**
- None - Minimal utility header
- Just includes `<inttypes.h>` and `<stdbool.h>`

**Platform.h Overlap:**
- **NO** - Utility types only

**SDL Headers:**
- **NO** - Standard C headers only

**Still Referenced:**
- Referenced in: sdl2_boolvec.h

**Actively Compiled:**
- **NO** - Header only

---

## Summary Table

| File | Functions | Platform.h Overlap | SDL Headers | Still Referenced | Compiled |
|------|-----------|-------------------|-------------|------------------|----------|
| sdl2_Audio.h/c | Audio init/deinit + implementation | No | Yes (SDL_mixer) | Yes (SOUND.c, sdl2_MOM.c) | No |
| sdl2_boolvec.h | Macros only | No | No | Yes (sdl2_PFL.c) | No |
| sdl2_EMM.c | EMS_Startup() | Yes | No | Yes (MOM_PFL.h) | No |
| sdl2_Input.c | Get_Input() | No | No | Yes (Input.c) | No |
| sdl2_KD.h/c | Keyboard ops + Read_Key() | Partial | Yes | Yes (Keyboard.c) | No |
| sdl2_MD.c | Mouse ops + User_Mouse_Handler() | Partial | Yes | Yes (Mouse.c) | No |
| sdl2_MOM.h/c | Startup_Platform() | Yes | Yes | Yes (sdl2_PFL.c) | No |
| sdl2_PFL.h/c | Platform functions | Yes (almost all) | **SDL3** (error!) | Yes (everywhere) | No |
| sdl2_SCCC.h | Lookup table | No | No | Yes (sdl2_PFL.c) | No |
| sdl2_SDLK.h/c | Lookup table | No | No | Yes (sdl2_MOM.c) | No |
| sdl2_Timer.c | Timer + Mark/Release_Time | Partial | Yes | Yes (Input.c) | No |
| sdl2_types.h | Types only | No | No | Yes (sdl2_boolvec.h) | No |

**CRITICAL FINDING:** None of these SDL2 files are compiled as part of the MOX library (per CMakeLists.txt). They appear to be legacy code from an earlier platform refactor phase. The formal platform interface is now `Platform.h` in the `platform/` directory, which should have SDL3 implementations there.
</output>
TaskOutput task: "a347539a632f5ae46"
OUT
<retrieval_status>success</retrieval_status>

<task_id>a347539a632f5ae46</task_id>

<task_type>local_agent</task_type>

<status>completed</status>

<output>
Perfect! Now I have all the information I need. Let me compile a comprehensive report of the platform boundary violations found.

## Platform Boundary Violations Report

I've conducted a thorough search of the MoX/ and MoM/ directories for platform boundary violations. Here are the violations I found:

### CRITICAL VIOLATIONS - Platform Headers in Game Code

#### 1. **SDL Includes in Platform Interface Headers (Exposed to Game Code)**
- **/c/STU/devel/ReMoM/MoX/src/MOM_PFL.h** - Line 7
  - `#include <SDL.h>` (currently uncommented, exposed to all game code that includes MOM_PFL.h)
  - Line 115: Uses SDL type `SDL_Color` for `platform_palette_buffer[]`
  
- **/c/STU/devel/ReMoM/MoX/src/SOUND.h** - Line 16
  - `#include "sdl2_Audio.h"` (game code includes this, violating platform abstraction)

#### 2. **SDL Includes in Game Code Files**
- **/c/STU/devel/ReMoM/MoX/src/SOUND.c** - Line 27
  - `#include "sdl2_Audio.h"` 

- **/c/STU/devel/ReMoM/MoX/src/Video.c** - Line 284
  - Comment reference: `SDL2: manually or next SDL_WINDOWEVENT_EXPOSED...`

- **/c/STU/devel/ReMoM/MoX/src/EXIT.c** - Lines 61, 65, 69, 72
  - Comments reference `SDL_ltoa()` and `SDL_itoa()` (in TODO blocks, but still references SDL)

#### 3. **Win32 Headers in Game/Platform Interface Code**
- **/c/STU/devel/ReMoM/MoX/src/win_CLK.c** - Line 4
  - `#include "Windows.h"`
  
- **/c/STU/devel/ReMoM/MoX/src/win_EMS.c** - Line 5
  - `#include <windows.h>`
  
- **/c/STU/devel/ReMoM/MoX/src/win_Exit.c** - Line 2
  - `#include <Windows.h>`
  - Lines 38-39: Direct Win32 API usage: `MessageBoxA()`, `PostMessage()` (WM_CLOSE)

#### 4. **Platform-Specific Files Included from Game Code**
- **/c/STU/devel/ReMoM/MoX/src/Input.c** - Line 22
  - `#include "../../platform/include/Platform.h"` (correct, but mixing game and platform)

- **/c/STU/devel/ReMoM/MoX/src/win_KD.c** - Line 7-8
  - `#include "MOM_PFL.h"` and `#include "PoC_PFL.h"` (references SDL.h transitively)

- **/c/STU/devel/ReMoM/MoX/src/win_MD.c** - Line 7-8
  - `#include "MOM_PFL.h"` and `#include "PoC_PFL.h"` (references SDL.h transitively)

### VIOLATION SEVERITY BY TYPE

**Tier 1 - Most Severe (Direct SDL/Win32 in Game Code):**
1. SOUND.h/SOUND.c including sdl2_Audio.h
2. MOM_PFL.h exposing `#include <SDL.h>` to game code
3. win_Exit.c with MessageBoxA() and PostMessage() calls

**Tier 2 - Severe (Headers Exposing Platform Types):**
1. MOM_PFL.h declaring `SDL_Color platform_palette_buffer[]` (SDL type in game-facing interface)
2. sdl2_KD.h including `<SDL.h>` (used by game keyboard code)
3. sdl2_MOM.h including `<SDL.h>` (with SDL_Event* parameter exposure)

**Tier 3 - Moderate (SDL References in Platform-Abstraction Layer):**
1. sdl2_PFL.c/h includes (this is platform layer, but sdl2_Audio.h used by game)
2. sdl2_KD.c references `SDL_NUM_SCANCODES` constant
3. sdl2_MD.c, sdl2_Timer.c, sdl2_EMM.c reference SDL

### Files NOT in CMakeLists.txt Build (Potential Dead Code)
The `/c/STU/devel/ReMoM/MoX/src/CMakeLists.txt` contains ONLY these game library files - no platform layer files:
- AIL.c, Allocate.c, angle.c, capture.c, CFG.c, DOS.c, EMM.c, EXIT.c, Fields.c, file_ani.c, Fonts.c, GENDRAW.c, GFILE.c, Graphics.c, FLIC_Draw.c, Help.c, Input.c, IO_Buffer.c, Keyboard.c, LBX_Load.c, LOADSAVE.c, Mouse.c, MOM_DAT.c, MOX_DAT.c, MOX_SET.c, MOX_SND.c, MOX_T4.c, paragrph.c, random.c, SOUND.c, special.c, TEXTBOX.c, Util.c, Video.c, Video2.c

**Missing from build:** sdl2_*, win_* files (these are presumably in the platform library)

### Summary Count
- **Files with SDL includes:** 12 files
- **Files with Win32 includes:** 3 files  
- **Game-facing interfaces exposing platform types:** 2 (MOM_PFL.h, SOUND.h)
- **Direct platform function calls in game code:** 1 (win_Exit.c)
</output>
All 5 agents have returned. Let me now synthesize everything into the summary and plan.

Update Todos

Explore project structure and CMake build system

Review platform layer headers and interfaces

Review Win32 platform implementation

Review SDL2 platform implementation

Review SDL3 platform implementation

Review legacy sdl2_ files still in MoX/src/

Identify platform boundary violations in game/engine code

Summarize state and create completion plan
EnterPlanMode
OUT
Entered plan mode. You should now focus on exploring the codebase and designing an implementation approach.

In plan mode, you should:
1. Thoroughly explore the codebase to understand existing patterns
2. Identify similar features and architectural approaches
3. Consider multiple approaches and their trade-offs
4. Use AskUserQuestion if you need to clarify the approach
5. Design a concrete implementation strategy
6. When ready, use ExitPlanMode to present your plan for approval

Remember: DO NOT write or edit any files yet. This is a read-only exploration and planning phase.
I've already done thorough exploration in Phase 1 — all 5 agents completed. Let me go straight to writing the plan based on the comprehensive findings.

Write C:\Users\jbalcomb\.claude\plans\rosy-sprouting-raven.md
211 lines
# Platform Layer Separation — State Summary & Completion Plan

## Context

ReMoM recreates the MS-DOS platform layer so the original Master of Magic game code can run on modern OSes via SDL or native Win32. The project has a `platform/` directory with a clean public API (`Platform.h`, `Platform_Keys.h`, `Platform_Replay.h`) and three backends: **SDL2**, **SDL3**, and **Win32**. The SDL3 backend is the most complete. However, the separation is not finished — there are naming issues, boundary violations, dead legacy code, and incomplete backends.

---

## Current State

### Architecture (What's Working)

```
src/ReMoM.c  (entry point)
  ├── MoM/  (game logic — links MOX, STU, Platform)
  ├── MoX/  (engine — links Platform)
  ├── STU/  (utility library)
  └── platform/
       ├── include/  (Platform.h, Platform_Keys.h, Platform_Replay.h)
       ├── sdl2/     (SDL2 backend — 16 files, compiles)
       ├── sdl3/     (SDL3 backend — 18 files, compiles, most complete)
       ├── win32/    (Win32 backend — 9 files, NOT in CMake build)
       └── replay/   (Replay.c — shared, platform-agnostic)
```

- **CMake** selects SDL2 or SDL3 via `-DUSE_SDL3=ON`. Win32 is not selectable.
- `Platform` is a STATIC library. MoX and momlib link it. Only Platform links SDL.
- `Platform.h` is the formal contract — clean, no SDL/Win32 types.

### Backend Completeness

| Feature | SDL3 | SDL2 | Win32 |
|---------|------|------|-------|
| Lifecycle (init/shutdown) | Complete | Complete | Partial (Init only, no modern API) |
| Video (palette, blit, present) | Complete | Complete | Partial (update in win_PFL.cpp, no Platform_* wrappers) |
| Keyboard (buffer, translate) | Complete | Complete | Legacy only (Read_Key/Keyboard_Status, no Platform_Keyboard_Buffer_*) |
| Mouse (position, buttons, grab) | Complete | Complete | Legacy only (Mouse_Button, no Platform_Event_Handler) |
| Timer (millies, sleep, clock) | Complete | Complete | Partial (Read_System_Clock_Timer, no Platform_Get_Millies/Sleep) |
| Audio (init, play, music) | Complete | Complete | Not implemented |
| Events (Platform_Event_Handler) | Complete | Complete | Not implemented (uses Pump_Events) |
| Replay | Complete | Complete | Not implemented |
| EMS_Startup | Complete | Complete | Complete |
| **In CMake build?** | **Yes** | **Yes** | **No** |

### Issues Found

#### 1. Boundary Violations — SDL Leaking Into Game Code

| File | Violation |
|------|-----------|
| `src/ReMoM.c:78-90` | Directly `#include <SDL3/SDL.h>` / `<SDL.h>` — the entry point should not need SDL headers |
| `MoX/src/SOUND.h:16` | `#include "sdl2_Audio.h"` — game audio header depends on SDL backend |
| `MoX/src/SOUND.c:27` | `#include "sdl2_Audio.h"` — game audio impl depends on SDL backend |
| `MoX/src/MOM_PFL.h:7` | `#include <SDL.h>` and uses `SDL_Color` type — exposed to game code |
| `platform/sdl2/sdl2_KD.h` | `Platform_Keyboard_Event(SDL_Event*)` — SDL type in public interface |
| `platform/sdl2/sdl2_PFL.h` | `Platform_Window_Event(SDL_WindowEvent*)` — SDL type in public interface |
| `platform/sdl3/sdl2_KD.h` | Same SDL_Event* exposure |
| `platform/replay/Replay.c:18-19` | Includes `../../MoX/src/Fields.h` and `Mouse.h` — platform depends on engine |

#### 2. Legacy Dead Code in MoX/src/

17 `sdl2_*` files remain in `MoX/src/` — **none are compiled** (not in CMakeLists.txt). They are superseded by `platform/sdl2/` and `platform/sdl3/`. Some are still `#include`d by active MoX code (SOUND.h, MOM_PFL.h).

Key files still referenced:
- `MoX/src/sdl2_Audio.h` — included by SOUND.h and SOUND.c
- `MoX/src/MOM_PFL.h` — included by some MoX .c files (transitively)

#### 3. Naming Convention Violations

| Location | Issue |
|----------|-------|
| `Shudown_Platform()` | Typo — should be `Shutdown_Platform()` (but changing breaks everything) |
| `build_key_xlat()` | Should be `Build_Key_Xlat()` (Proper_Snake_Case) |
| `mod_xlat()` | Should be `Mod_Xlat()` |
| `kbd_set_pressed()`, `kbd_is_pressed()` | Should be `Kbd_Set_Pressed()`, `Kbd_Is_Pressed()` |
| `hw_textinput_start()`, `hw_textinput_stop()` | Should be `Hw_Textinput_Start()`, `Hw_Textinput_Stop()` |
| `kilgore_*` functions (static in PFL.c) | Inconsistent prefix, should use Proper_Snake_Case |
| `sdl2_get_key_code_name()` | Should be `Sdl2_Get_Key_Code_Name()` |
| SDL3 backend headers still named `sdl2_*.h` | Confusing — `platform/sdl3/sdl2_Audio.h`, `sdl2_KD.h`, etc. |
| SDL3 backend globals still named `sdl2_*` | `sdl2_window`, `sdl2_renderer`, `sdl2_surface_*`, `sdl2_texture` |
| `SDLK_to_SCCS` in sdl3_SDLK.c | Typo — should be `SDLK_to_SCCC` |

#### 4. Game-Level Functions in Platform Code

Several MoX-layer functions are implemented inside platform `.c` files instead of in MoX. These are NOT in Platform.h but are called by game code:

- `Get_Input()` — in sdl2_Input.c / sdl3_Input.c (should be in MoX/src/Input.c)
- `Keyboard_Status()`, `Read_Key()` — in sdl2_KD.c / sdl3_KD.c (should be in MoX/src/Keyboard.c)
- `Mouse_Button()`, `Mouse_Movement_Handler()`, `Mouse_Button_Handler()`, `User_Mouse_Handler()`, `Set_Mouse_Position()` — in sdl2_MD.c / sdl3_MD.c (should be in MoX/src/Mouse.c)
- `Mark_Time()`, `Release_Time()`, `Release_Time_Seconds()` — in sdl2_Timer.c / sdl3_Timer.c (should be in MoX/src/Timer.c?)

These are game/engine functions that *call* Platform_* functions. They belong in MoX, not in the platform library.

#### 5. Platform Code Includes Engine Headers

Many platform `.c` files include MoX headers directly:
- `sdl3_PFL.c` includes: MOX_BASE.h, MOX_DEF.h, MOX_KEYS.h, MOX_TYPE.h, Mouse.h, Video.h
- `sdl3_Video.c` includes: Fonts.h, MOX_BASE.h, MOX_TYPE.h, Video.h, Video2.h
- `sdl3_Timer.c` includes: Mouse.h, Timer.h, Video.h

The dependency should be one-way: MoX -> Platform.h. Platform code should NOT include MoX headers. The needed types/globals should be exposed via Platform.h or passed as parameters.

#### 6. Audio Not In Platform.h

Audio functions (`sdl2_Audio_Init`, `sdl2_Audio_Deinit`, `Play_Sound_SDL2_Mixer`, `hw_audio_music_*`) are not declared in Platform.h. They're called through backend-specific headers, which creates the SOUND.h -> sdl2_Audio.h boundary violation.

#### 7. Win32 Backend Not Integrated

The win32/ directory exists but:
- Not in CMake build system
- Implements legacy API (Pump_Events, Read_Key) not modern Platform.h API
- Missing: Platform_Event_Handler, Platform_Video_Update, Platform_Palette_Update, Platform_Get_Millies, Platform_Sleep_Millies, audio, replay

---

## Completion Plan

### Phase 1: Fix Boundary Violations (Critical)

**Goal**: Game/engine code (MoX/, MoM/, src/) must never include SDL or Win32 headers.

1. **Add audio to Platform.h** — Declare `Platform_Audio_Init()`, `Platform_Audio_Deinit()`, `Platform_Audio_Play_Sound()`, `Platform_Audio_Music_Play()`, `Platform_Audio_Music_Stop()`, etc. in Platform.h. Implement in each backend's audio `.c` file.

2. **Remove SDL includes from src/ReMoM.c** — The entry point includes SDL headers for `SDL_main`. Instead, have the platform provide `main()` or use `SDL_MAIN_HANDLED` properly. Alternatively, gate the include behind `#ifdef USE_SDL3` inside Platform.h or a new `Platform_Main.h`.

3. **Remove sdl2_Audio.h include from SOUND.h/SOUND.c** — Replace with `#include "Platform.h"` and call the new Platform_Audio_* functions.

4. **Retire MoX/src/MOM_PFL.h** — Replace all includes of it in MoX code with `#include "Platform.h"`. Any types it declares that aren't in Platform.h need to be added there.

5. **Make Platform_Keyboard_Event() and Platform_Window_Event() private** — These take SDL_Event* and should NOT be in any public header. They're internal to the platform event handler. Move declarations from `sdl2_KD.h`/`sdl2_PFL.h` to be static or in a private header only included within the backend.

6. **Fix Replay.c engine dependency** — Replay.c includes Fields.h and Mouse.h. Extract the needed externs (mouse_x, mouse_y, etc.) into Platform.h or pass them via the existing callback pattern.

### Phase 2: Move Game-Level Functions Out of Platform

**Goal**: Platform library only implements Platform_* functions declared in Platform.h.

1. **Move Get_Input()** from `sdl2_Input.c`/`sdl3_Input.c` to `MoX/src/Input.c` (which already exists). The platform files just call Platform_Event_Handler — this logic belongs in MoX.

2. **Move Keyboard_Status() and Read_Key()** from `sdl2_KD.c`/`sdl3_KD.c` to `MoX/src/Keyboard.c`. These read from the platform keyboard buffer (which is exposed via Platform_Keys.h) and don't need SDL.

3. **Move Mouse_Button(), User_Mouse_Handler(), Set_Mouse_Position()** from `sdl2_MD.c`/`sdl3_MD.c` to `MoX/src/Mouse.c`. Platform_Get_Mouse_Position_XY() and Platform_Maybe_Move_Mouse() stay in platform.

4. **Move Mark_Time(), Release_Time(), Release_Time_Seconds()** from `sdl2_Timer.c`/`sdl3_Timer.c` to `MoX/src/Timer.c` (or keep in MoX). These call Platform_Get_Millies() / Platform_Sleep_Millies() — they don't need SDL.

### Phase 3: Remove Engine Header Includes from Platform Code

**Goal**: Platform `.c` files only include Platform.h, Platform_Keys.h, Platform_Replay.h, and their own backend-private headers.

After Phase 2 moves game functions out, most engine includes become unnecessary. For remaining needs:
- Types like `byte_ptr`, `SAMB_ptr` → add to Platform.h or use `uint8_t*`
- Globals like `draw_page_buffer[]`, `palette_3byte_buffer[]` → expose via Platform.h externs
- Functions like `Update_Mouse_Position()`, `Update_Mouse_Button_Status()` → already in Platform.h

### Phase 4: Clean Up Naming

1. **Rename SDL3 backend headers**: `platform/sdl3/sdl2_*.h` → proper names (or a generic `pfl_*.h` prefix)
2. **Rename SDL3 globals**: `sdl2_window` → `sdl3_window` (or `pfl_window`)
3. **Fix function names** to Proper_Snake_Case: `build_key_xlat` → `Build_Key_Xlat`, etc.
4. **Fix typo**: `SDLK_to_SCCS` → `SDLK_to_SCCC`
5. **Note**: `Shudown_Platform()` typo is in Platform.h and all backends — fixing requires coordinated rename across all callers

### Phase 5: Remove Legacy Dead Code

1. **Delete MoX/src/sdl2_* files** (17 files) — they're not compiled and are superseded by `platform/sdl2/` and `platform/sdl3/`.
2. **Delete MoX/src/MOM_PFL.h** after Phase 1 migrates its consumers.
3. **Delete platform/main.cpp** — dead file (5 lines, includes SDL.h + Windows.h, not built).

### Phase 6: Win32 Backend (Optional / Future)

The Win32 backend is significantly behind. Options:
- **A) Retire it** — SDL2/SDL3 covers Windows. Mark win32/ as legacy/unsupported.
- **B) Complete it** — Implement all Platform.h functions using Win32 API. Major effort: audio (DirectSound or XAudio2), keyboard buffer, event handler, replay, video update.
- **C) Leave as-is** — Don't include in CMake. Keep as reference for the original Win32 port.

**Recommendation**: Option A or C. The SDL backends cover Windows already. The Win32 backend is a niche feature that's not needed for the project's goals.

---

## Verification

After each phase:
- Build with SDL2: `cmake -B build && cmake --build build`
- Build with SDL3: `cmake -B build -DUSE_SDL3=ON && cmake --build build`
- Run tests: `ctest --test-dir build --output-on-failure`
- Run the game and verify: window opens, keyboard input works, mouse works, palette renders correctly, audio plays
- For replay: `ReMoMber --record test.RMR`, play a few turns, quit, then `ReMoMber --replay test.RMR`

---

## Files to Modify (Phase 1-3, critical path)

| File | Change |
|------|--------|
| `platform/include/Platform.h` | Add audio API declarations, add needed externs (draw_page, palette) |
| `platform/sdl2/sdl2_Audio.c` | Implement Platform_Audio_* wrappers |
| `platform/sdl3/sdl3_Audio.c` | Implement Platform_Audio_* wrappers |
| `src/ReMoM.c` | Remove direct SDL includes |
| `MoX/src/SOUND.h` | Remove `#include "sdl2_Audio.h"`, use Platform.h |
| `MoX/src/SOUND.c` | Remove `#include "sdl2_Audio.h"`, call Platform_Audio_* |
| `MoX/src/Input.c` | Absorb Get_Input() from platform |
| `MoX/src/Keyboard.c` | Absorb Keyboard_Status(), Read_Key() from platform |
| `MoX/src/Mouse.c` | Absorb Mouse_Button(), User_Mouse_Handler(), etc. |
| `platform/sdl2/sdl2_KD.c` | Remove game-level functions, make internal funcs static |
| `platform/sdl3/sdl3_KD.c` | Same |
| `platform/sdl2/sdl2_MD.c` | Remove game-level functions |
| `platform/sdl3/sdl3_MD.c` | Same |
| `platform/sdl2/sdl2_Input.c` | Remove (or gut) — Get_Input moves to MoX |
| `platform/sdl3/sdl3_Input.c` | Same |
| `platform/replay/Replay.c` | Remove MoX header includes, use Platform.h |
Update Todos

Explore project structure and CMake build system

Review platform layer headers and interfaces

Review Win32 platform implementation

Review SDL2 platform implementation

Review SDL3 platform implementation

Review legacy sdl2_ files still in MoX/src/

Identify platform boundary violations in game/engine code

Summarize state and create completion plan
Here's a summary of what the plan covers:

The Good: Platform.h is well-designed — clean contract, no platform types, proper separation of concerns. SDL3 and SDL2 backends are functionally complete. The CMake build system correctly selects backends.

The Problems (7 categories):

SDL headers leak into game code — ReMoM.c, SOUND.h, MOM_PFL.h all include SDL directly
17 dead sdl2_* files in MoX/src/ — not compiled, but still #included by active code
Game-level functions live in platform code — Get_Input, Read_Key, Mouse_Button, Mark_Time etc. are in platform/ but belong in MoX/
Platform code includes engine headers — dependency should be one-way (MoX → Platform), not bidirectional
Audio has no Platform.h API — forces SOUND.c to include backend-specific sdl2_Audio.h
Naming violations — Shudown_Platform typo, sdl2_* names in SDL3 backend, lowercase function names
Win32 backend is incomplete and not in the CMake build
The Plan is phased:

Phase 1: Fix boundary violations (add audio to Platform.h, remove SDL includes from game code)
Phase 2: Move game-level functions out of platform/ back into MoX/
Phase 3: Remove engine header includes from platform code
Phase 4: Fix naming (sdl2→sdl3 renames, Proper_Snake_Case)
Phase 5: Delete legacy dead code
Phase 6: Decide Win32 fate (recommend retire or leave as reference)
Full details are in the plan file.

Claude’s Plan C:\Users\jbalcomb\.claude\plans\rosy-sprouting-raven.md
User approved the plan
Plan approved. This is a large multi-phase refactoring. Let me start with Phase 1 (boundary violations) and work incrementally, building after each logical change.

Update Todos

Phase 1.1: Add audio API to Platform.h

Phase 1.2: Update SDL3 audio backend to implement Platform_Audio_* wrappers

Phase 1.3: Update SDL2 audio backend to implement Platform_Audio_* wrappers

Phase 1.4: Remove sdl2_Audio.h include from SOUND.h/SOUND.c, use Platform.h

Phase 1.5: Remove SDL includes from src/ReMoM.c

Phase 1.6: Retire MoX/src/MOM_PFL.h

Phase 1.7: Make Platform_Keyboard_Event/Platform_Window_Event private

Phase 1.8: Fix Replay.c engine dependency
Let me first understand the current audio interface by reading the relevant files.

Now let me understand what audio functions the game code actually calls. Let me check the full callsites.

Now I understand the audio boundary. The key violation is Play_Sound_SDL2_Mixer() called from SOUND.c, and the sdl2_Audio.h include in SOUND.h/SOUND.c. Let me read the relevant SOUND.c sections to see exact callsites.

Good. Now I have the full picture. The boundary is simple:
SOUND.c calls Play_Sound_SDL2_Mixer() — this needs to become Platform_Audio_Play_Sound() in Platform.h
sdl2_Audio_Init/Deinit are only called within platform code (Init.c) — they stay internal
SOUND.h includes sdl2_Audio.h unnecessarily
Let me implement Phase 1.1 through 1.4 together since they're tightly coupled.

Now let me add the audio API to Platform.h and make all the coordinated changes.

Now rename Play_Sound_SDL2_Mixer to Platform_Audio_Play_Sound in both backends.





The SDL includes are only used for SDL_main.h (entry point handling) and SDL_mixer.h. The mixer include isn't needed since SOUND.c handles all audio. The SDL_main.h is needed because SDL redefines main on some platforms.

The cleanest approach: use SDL_MAIN_HANDLED to tell SDL we handle our own main(), then remove all SDL includes. The main() function at line 161 already uses the standard int main(int argc, char * argv[]) signature.

But wait — SDL3 changed how SDL_main works. In SDL3, you need #include <SDL3/SDL_main.h> and optionally #define SDL_MAIN_USE_CALLBACKS or SDL_MAIN_HANDLED. Let me check if the project's Startup_Platform() already calls SDL_Init(). Yes, sdl3_Init.c calls SDL_Init(). So SDL_MAIN_HANDLED before the include is the right approach — or we can just define it and skip the include entirely.

Actually, the safest approach for cross-platform SDL: keep the SDL_main.h include but add SDL_MAIN_HANDLED before it. Drop the mixer include entirely since it's unused here.





Phase 1 Complete — Summary of Changes
Files modified:

File	Change
Platform.h	Added Platform_Audio_Play_Sound() declaration
sdl3_Audio.c	Renamed Play_Sound_SDL2_Mixer → Platform_Audio_Play_Sound
sdl2_Audio.c	Renamed Play_Sound_SDL2_Mixer → Platform_Audio_Play_Sound
SOUND.h	Removed #include "sdl2_Audio.h"
SOUND.c	Removed sdl2_Audio.h include, added Platform.h, updated call to Platform_Audio_Play_Sound
ReMoM.c	Replaced full SDL includes with just SDL_MAIN_HANDLED + SDL_main.h
14 platform backend .c files	Replaced #include "MOM_PFL.h" → #include "Platform.h"
Build verified: SDL3 no-audio Debug builds successfully to ReMoMber.exe.

Remaining known coupling:

Replay.c includes Fields.h and Mouse.h — required for replay injection/logging
src/ReMoM.c still includes SDL_main.h — required for entry point handling on some platforms
Phases 2-6 from the plan are ready to proceed when you'd like. Phase 2 (move game-level functions out of platform) is the next major structural improvement. Want me to continue?

Good, this confirms the Phase 1 findings — the legacy MoX/src/sdl2_* files are not compiled and will be cleaned up in Phase 5.

Phase 1 is done and verified. The remaining phases from the plan are:

Phase 2: Move game-level functions (Get_Input, Read_Key, Mouse_Button, Mark_Time, etc.) out of platform/ back into MoX/
Phase 3: Remove engine header includes from platform code
Phase 4: Fix naming (sdl2→sdl3 renames, Proper_Snake_Case, typos)
Phase 5: Delete legacy dead code (17 MoX/src/sdl2_* files, MoX/src/MOM_PFL.h, platform/main.cpp)
Phase 6: Decide Win32 backend fate
Want me to continue with Phase 2?

Good — the SDL2 backend review confirms the same patterns. The MOM_PFL.h replacement we did in Phase 1 covers the SDL2 backend too, and the Play_Sound_SDL2_Mixer → Platform_Audio_Play_Sound rename is done in both backends.

Phase 1 is complete and verified. Ready to continue with Phase 2 (moving game-level functions out of platform/) whenever you want to proceed.

All 5 research agents have now completed. Their findings are fully incorporated into the plan and the Phase 1 work already done.

Summary of current state:

Phase 1 (boundary violations) is complete and build-verified
Phases 2-6 are ready to proceed per the plan
Shall I continue with Phase 2 (moving game-level functions like Get_Input, Read_Key, Mouse_Button, Mark_Time out of platform/ back into MoX/)?

Good — this boundary violations report confirms the two critical violations we already fixed in Phase 1:

SOUND.h/SOUND.c including sdl2_Audio.h — fixed (removed, now uses Platform.h)
MOM_PFL.h exposing SDL types — fixed (all platform backends now use Platform.h instead)
The remaining items in the report are either:

In uncompiled dead code (MoX/src/sdl2_, MoX/src/win_) — Phase 5 cleanup
Comments mentioning SDL (not actual code dependencies)
Correct usage (MoX/src/Input.c including Platform.h is proper)
All 5 research agents are done. Phase 1 is complete. Ready to continue with Phase 2 whenever you want.





Starting Phase 2: Moving game-level functions out of platform/ back into MoX/.

Update Todos

Phase 2.1: Move Get_Input() from platform to MoX/src/Input.c

Phase 2.2: Move Keyboard_Status()/Read_Key() from platform to MoX/src/Keyboard.c

Phase 2.3: Move Mouse_Button()/User_Mouse_Handler()/etc from platform to MoX/src/Mouse.c

Phase 2.4: Move Mark_Time()/Release_Time()/etc from platform to MoX

Phase 2.5: Remove emptied platform files from CMake or gut them

Phase 2.6: Build verification
Let me read the current implementations in parallel to understand what needs moving.

Good — Keyboard.c has comments at lines 22-26 saying // Platform: for Keyboard_Status and Read_Key, acknowledging they belong here but are currently in platform code. Let me read the full Keyboard_Status() and Read_Key() from the platform to understand what they do.

Update Todos

Move Get_Input() from platform to MoX/src/Input.c

Move Keyboard_Status() and Read_Key() from platform to MoX/src/Keyboard.c

Move mouse game-level functions (needs new Platform.h abstractions first)

Move timer game-level functions (needs new Platform.h abstractions first)

Remove emptied platform files from CMake or gut them

Build verification

Let me read the current state of the destination files and SDL2 counterparts.

Now let me check what Keyboard.h declares and what's in the CMakeLists.txt for the platform library.

Let me check how MoX and the main CMakeLists reference these source files.

Grep "Input\.c|Keyboard\.c" (glob: **/CMakeLists.txt)
4 lines of output
Good — Input.c and Keyboard.c are already in the MoX build. Now I have the full picture. Here's the plan:

Phase 2.1: Get_Input() — identical in both sdl2 and sdl3 backends, no SDL calls. Move the function body into MoX/src/Input.c and remove it from both platform files.

Phase 2.2: Keyboard_Status() and Read_Key() — identical in both backends, no SDL calls. Move to MoX/src/Keyboard.c.

Both sdl2/sdl3 _Input.c files will become empty (can be removed from CMake). The _KD.c files still have SDL-specific functions that must stay.

Let me also check what MoX/src/Input.c has near the Get_Input declaration area — I need to know where to insert.

Grep "Get_Input|Interpret_Mouse_Input|Set_Page_Off|input_delay" (in c:\STU\devel\ReMoM\MoX\src\Input.c)
9 lines of output
Read c:\STU\devel\ReMoM\MoX\src\Input.c (lines 2941-2990)
There's already a placeholder comment at line 2959. Now I'll make all the edits. Let me:

Replace the placeholder in MoX/src/Input.c with the actual Get_Input() function
Replace the placeholder comments in MoX/src/Keyboard.c with Keyboard_Status() and Read_Key()
Remove the functions from both SDL2 and SDL3 platform files
Remove the now-empty platform Input.c files from CMake





Phase 2 is complete. Here's a summary of what was done:

Phase 2 — Move Game-Level Functions Out of Platform
Functions Moved
Function	From	To
Get_Input()	sdl2_Input.c / sdl3_Input.c	MoX/src/Input.c
Keyboard_Status()	sdl2_KD.c / sdl3_KD.c	MoX/src/Keyboard.c
Read_Key()	sdl2_KD.c / sdl3_KD.c	MoX/src/Keyboard.c
Mouse_Button()	sdl2_MD.c / sdl3_MD.c	MoX/src/Mouse.c
Mouse_Movement_Handler()	sdl2_MD.c / sdl3_MD.c	MoX/src/Mouse.c
Mouse_Button_Handler()	sdl2_MD.c / sdl3_MD.c	MoX/src/Mouse.c
User_Mouse_Handler()	sdl2_MD.c / sdl3_MD.c	MoX/src/Mouse.c
Set_Mouse_Position()	sdl2_MD.c / sdl3_MD.c	MoX/src/Mouse.c
Mark_Time()	sdl2_Timer.c / sdl3_Timer.c	MoX/src/Timer.c (new file)
Release_Time()	sdl2_Timer.c / sdl3_Timer.c	MoX/src/Timer.c
Release_Time_Seconds()	sdl2_Timer.c / sdl3_Timer.c	MoX/src/Timer.c
New Platform.h Abstractions Added
Function	Purpose
Platform_Get_Mouse_Button_State()	Query mouse buttons without SDL types
Platform_Warp_Mouse(game_x, game_y)	Warp cursor with internal coordinate scaling
Platform_Get_Window_Width()	Window width for coordinate conversion
Platform_Pump_Events()	Keep event queue alive without processing
Other Changes
Added extern struct s_KEYBOARD_BUFFER platform_keyboard_buffer to Platform_Keys.h
Removed sdl2_Input.c / sdl3_Input.c from CMake (files gutted)
Created MoX/src/Timer.c and added to MoX CMakeLists
Platform timer files now only contain Read_System_Clock_Timer()
Platform MD files now only contain the 3 new Platform_* functions
The platform layer now only implements Platform_* functions. All game/engine logic lives in MoX. Ready to continue with Phase 3 if you want.





Phase 4 complete. Both SDL2 and SDL3 backends build and link clean. Here's what was done:

Typo fixes:

Shudown_Platform → Shutdown_Platform (Platform.h, sdl2_Init.c, sdl3_Init.c, ReMoM.c)
SDLK_to_SCCS → SDLK_to_SCCC (both backends)
kilgore removal:

kilgore_key struct member/local vars → packed_key (Platform_Keys.h, Platform.h, sdl2_KD.h, sdl3_KD.h, sdl2_KD.c, sdl3_KD.c, Keyboard.c, Replay.c)
kilgore_character → packed_character (Keyboard.c)
kilgore_* static functions/globals → pfl_* (sdl2_PFL.c/h, sdl3_PFL.c/h)
Proper_Snake_Case functions:

build_key_xlat → Build_Key_Xlat
mod_xlat → Mod_Xlat
kbd_set_pressed → Kbd_Set_Pressed
kbd_is_pressed → Kbd_Is_Pressed
hw_textinput_start/stop → Hw_Textinput_Start/Stop (Platform.h, both KD.c files, Input.c)
sdl2_get_key_code_name → Platform_Get_Key_Code_Name
SDL3 header renames:

platform/sdl3/sdl2_*.h → sdl3_*.h (6 files renamed, old files deleted)
All include guards updated (SDL2_*_H → SDL3_*_H)
SDL3 global renames:

All sdl2_* globals → sdl3_* across all SDL3 backend files
1oom: All references are source attribution comments — left as-is.





Phase 5 complete. Both SDL2 and SDL3 backends build and link clean.

Deleted files (21 total):

17 MoX/src/sdl2_* files — legacy platform code superseded by platform/sdl2/ and platform/sdl3/
MoX/src/MOM_PFL.h and MoX/src/MOM_PFL.c — dead, not compiled
platform/main.cpp — dead, not compiled
platform/sdl2/sdl2_SCCC.h, platform/sdl3/sdl3_SCCC.h — unused SCCC translation tables
platform/sdl2/sdl2_SDLK.c, platform/sdl2/sdl2_SDLK.h — unused SDLK_to_SCCC array
platform/sdl3/sdl3_SDLK.c, platform/sdl3/sdl3_SDLK.h — same
Also cleaned:

Removed sdl2_SDLK.h / sdl3_SDLK.h includes from Init.c files
Removed SDLK.c from both PLATFORM_SOURCES lists in CMakeLists.txt
Zero MOX_KEYS.h includes remain in platform code — the last engine header dependency is now Fields.h in Replay.c (for debug logging)
That wraps up Phases 1–5 of the plan. Phase 6 (Win32 backend fate) is marked optional/future.





Phase 6 is complete. Here's a summary of what was done:

Phase 6: Win32 Backend — Complete
New files created:
win_State.c — Platform shared state globals (same as SDL backends)
win_PFL.h — Private Win32 header (replaces win_PFL.hpp)
win_Replay_Stub.c — Stub implementations of Replay API
Files rewritten (all engine includes removed):
win_PFL.cpp — Complete rewrite implementing all Platform.h functions: Startup_Platform(), Shutdown_Platform(), Platform_Get_Scale(), Platform_Palette_Update(), Platform_Set_Palette_Color(), Platform_Video_Update(), Platform_Event_Handler(), Platform_Pump_Events(), Platform_Mouse_Input_Enable/Disable(), Platform_Get_Mouse_Button_State(), Platform_Warp_Mouse(), Platform_Get_Window_Width(), Platform_Get_Mouse_Position_XY(), Platform_Maybe_Move_Mouse(), Hw_Textinput_Start/Stop(), Platform_Audio_Play_Sound() (stub), legacy Pump_Events/Paints()
win_KD.c — VK-to-MOX_KEY translation table + all Platform_Keyboard_Buffer_* functions
win_CLK.c — Platform_Get_Millies(), Platform_Sleep_Millies(), Read_System_Clock_Timer()
win_EMS.c — Engine includes removed
win_Exit.c — Engine includes removed
win_Video.c — Reduced to placeholder comment
win_MD.c — Reduced to comment (mouse handling in win_PFL.cpp)
Files deleted:
win_Input.c — Get_Input() lives in MoX/src/Input.c
win_PFL.hpp — replaced by win_PFL.h
CMake changes:
platform/CMakeLists.txt — Added USE_WIN32 option with Win32 source list, _STU_WIN definition, winmm link lib
CMakeLists.txt — Added USE_WIN32 option, skip SDL find_package/DLL copy when Win32
src/CMakeLists.txt — Skip SDL link when USE_WIN32
src/ReMoM.c — Guard SDL_main.h include with #ifndef _STU_WIN
Platform.h — Added MWA_Exit_With_Message to Win32 legacy section
Build: cmake -B build-win32 -DUSE_WIN32=ON && cmake --build build-win32 — all three backends (SDL2, SDL3, Win32) build cleanly.
The Win32 backend needs manual testing — keyboard, mouse, and video rendering. Audio is stubbed out (returns -1). Replay is also stubbed. These can be added later.





Where are we at on the new platform layer plan?

Read C:\Users\jbalcomb\.claude\plans\rosy-sprouting-raven.md
The plan is outdated — it was written before any work started. All 6 phases are now complete. Here's the current status:

Phase	Status	Summary
1: Fix Boundary Violations	Done	Audio added to Platform.h, SDL includes removed from ReMoM.c/SOUND.h/SOUND.c, MOM_PFL.h consumers migrated
2: Move Game-Level Functions	Done	Get_Input, Keyboard_Status, Read_Key, Mouse_Button, User_Mouse_Handler, Mark_Time, Release_Time moved to MoX
3: Remove Engine Headers from Platform	Done	All MOX_TYPE.h → <stdint.h> in SDL2/SDL3 private headers. Only Fields.h remains in Replay.c (for p_fields debug logging)
4: Clean Up Naming	Done	Shudown → Shutdown, SDLK_to_SCCS → SDLK_to_SCCC, kilgore_* → packed_key/pfl_*, all functions to Proper_Snake_Case, SDL3 headers renamed sdl2_*.h → sdl3_*.h, SDL3 globals renamed sdl2_* → sdl3_*
5: Remove Legacy Dead Code	Done	26 files deleted (17 MoX/src/sdl2_*, MOM_PFL.h/c, platform/main.cpp, SCCC/SDLK files)
6: Win32 Backend	Done	Full rewrite — implements all Platform.h functions, in CMake via -DUSE_WIN32=ON, builds cleanly. Audio stubbed, Replay stubbed.
What remains (not in the original plan):
Manual testing of the Win32 backend (keyboard, mouse, video rendering)
Win32 audio — currently stubbed (Platform_Audio_Play_Sound returns -1)
Win32 replay — currently stubbed; could share replay/Replay.c if Fields.h dependency is resolved
Fields.h in Replay.c — the sole remaining engine header in platform code (Phase 3 TODO)
Legacy MOM_PFL.h files still exist in platform/sdl2/ and platform/sdl3/ (unused by the build, but not deleted)
Should I update the plan file to reflect completion, or is there something you'd like to tackle next?





Done. Here's what was completed:
Item 1 — Replay.c Fields.h dependency: Replaced inline p_fields access with a Platform_Replay_Log_Field_Hit_Fn callback. The engine registers the callback in ReMoM.c, so Replay.c no longer includes any engine headers. Zero engine includes remain in platform code.
Item 2 — Dead MOM_PFL.h files: Deleted platform/sdl2/MOM_PFL.h and platform/sdl3/MOM_PFL.h.
Item 6 — Dead declarations: Removed Update_Mouse_Position() and Update_Mouse_Button_Status() from Platform.h.
Item 7 — Legacy dead files: Deleted MoX/src/PoC_PFL.h, 7 MoX/src/win_*.c files, and MoX/src/win_PFL.cpp/.hpp (10 files total).
All three backends (SDL3, SDL2, Win32) build clean.
3 remaining items (all Win32-specific, require your input):
Win32 manual testing — needs to be run and verified
Win32 audio — leave stubbed or implement?
Win32 replay — now possible to share Replay.c since Fields.h dependency is resolved; want me to hook it up?
