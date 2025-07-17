

1oom
chocolate-doom
OpenXcom
SDLPoP



¿ OS-Specific File I/O ?



https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170
_WIN32 Defined as 1 when the compilation target is 32-bit ARM, 64-bit ARM, x86, x64, or ARM64EC. Otherwise, undefined.
_WIN64 Defined as 1 when the compilation target is 64-bit ARM, x64, or ARM64EC. Otherwise, undefined.

https://github.com/Unidata/netcdf-c/issues/1105
https://github.com/Unidata/netcdf-c/issues/1108
https://github.com/Unidata/netcdf-c/blob/c844e5bab912f42553f0a60c39f5d22ef4d48123/libdispatch/dfile.c#L2522-L2526
https://github.com/Unidata/netcdf-c/blob/c844e5bab912f42553f0a60c39f5d22ef4d48123/libdispatch/dutil.c#L171-L175

https://www.vitoshacademy.com/file-finder-tiny-c-projects/

https://www.reddit.com/r/cpp/comments/10tt5nw/is_there_a_good_crossplatform_windows_linux_c_or/

https://github.com/VioletGiraffe/thin_io

https://www.google.com/search?q=CMake+use+different+source+file+for+Linux+and+Windows&oq=CMake+use+different+source+file+for+Linux+and+Windows&gs_lcrp=EgZjaHJvbWUyBggAEEUYQDIHCAEQIRigATIHCAIQIRigATIHCAMQIRigATIHCAQQIRigATIHCAUQIRigATIHCAYQIRirAtIBCTEzMzQzajBqN6gCCLACAfEFLNZAxKrjNgk&sourceid=chrome&ie=UTF-8
 CMakeLists.txt
    set(COMMON_SOURCES
        common_file1.cpp
        common_file2.cpp
    )
    if(WIN32)
        set(PLATFORM_SPECIFIC_SOURCES
            windows_only_file.cpp
        )
    else() # Assuming Linux for non-Windows
        set(PLATFORM_SPECIFIC_SOURCES
            linux_only_file.cpp
        )
    endif()
    add_executable(my_application ${COMMON_SOURCES} ${PLATFORM_SPECIFIC_SOURCES})

https://github.com/open-watcom/open-watcom-v2/discussions/528



*Platform-Layer* for IBM-PC/MS-DOS
VGA (Video) Card
    Video Memory (VGA VRAM)
    Palette/Color-Map (VGA DAC)

e.g.,
    Apply_Palette()
        copies `current_palette` to `VGA DAC`
    ...no equivalent - must add and expose platform palette  `SDL_Color sdl2_palette[256];`

video_back_buffer
PFL_Palette
VGA Memory
VGA Palette
video buffer
palette buffer
...
MoO2
    __back_page_buffer
    __off_page_buffer
    N/A / DNE
...
video_page_buffer
video_palette_buffer
platform_video_buffer
platform_palette_buffer

https://github.com/lab313ru/dsbxida/blob/master/README.source-code-description
src/hardware/vga_dac.cpp    VGA DAC (palette) emulation

https://github.com/joncampbell123/dosbox-x/blob/master/include/vga.h#L704
void VGA_DAC_SetEntry(Bitu entry,uint8_t red,uint8_t green,uint8_t blue);
https://github.com/joncampbell123/dosbox-x/blob/master/src/hardware/vga_dac.cpp#L411
void VGA_DAC_SetEntry(Bitu entry,uint8_t red,uint8_t green,uint8_t blue) {
    //Should only be called in machine != vga
    vga.dac.rgb[entry].red=red;
    vga.dac.rgb[entry].green=green;
    vga.dac.rgb[entry].blue=blue;
    for (Bitu i=0;i<16;i++) 
        if (vga.dac.combine[i]==entry)
            VGA_DAC_SendColor( i, i );
}
https://github.com/joncampbell123/dosbox-x/blob/master/include/video.h#L33
https://github.com/joncampbell123/dosbox-x/blob/master/include/video.h#L59
https://github.com/joncampbell123/dosbox-x/blob/master/src/hardware/vga_dac.cpp#L199
https://github.com/joncampbell123/dosbox-x/blob/master/src/hardware/vga_dac.cpp#L143







OS
Window
canvas/surface/texture
...just some bytes of memory...
...represents what to *display* on the *output device*...
For MS-DOS (IBM-PC, VGA), ...
    ...those bytes are the video memory on the video card...
    ...this comes with a hardware-based specification of the structure of the representation...
    ...the byte is an *index* into a *color-map* of 256 colors...
    ...the structure of the *color-map* is 3 6-bit values representing Red, Green, and Blue...
    ...what is *displayed* on the *output device* changes whenever you change any of the color values or any of the color indecies...
Moving to MS-Windows (MWA+GDI), ...
    ...those bytes are yours to allocate and make use of...
    ...the structure of the representation is dictated by the requirements of the *drawing* functions e.g., StretchDIBits()
    ...the structure is that the bytes represent color values directly, in sets of 3 or 4, with 1 byte each, as {Red, Green, Blue} or {Red, Green, Blue, Alpha}

Carrying forward to SDL2, ...

SDL_Window * window = SDL_CreateWindow()
SDL_Surface * window_surface = SDL_GetWindowSurface();
SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
SDL_UpdateWindowSurface( window );


    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );

    optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );


So, ...
    How do I move beyond thinking about the platform-layer as being specific to MS-Windows?
    The game code just wants some memory to write to for *drawing*
    elsewhere, something needs to care about the *structure* of that memory,
        the structure required by the platform
        and the relationship between the *screen* and the *window*
    surely, that'd be in the platform-layer
        meaning, the MS-Windows specific code just needs to meet the expectations set by the Platform-Layer Specification
    MoX sets the width, height, size, *pixel format*, etc. and shares that with the game as well as the platform
    the platform is responsible for the *window*
    MoX-PFL must handle dealing with *window* vs. *screen*

Circling back,
    How to abstract away the concept of a *window* in the MS-DOS code?
    Page_Flip() and Apply_Palette() both qualify as some platform equivalent of Update_Display/Window/Surface()
        as do the mouse "..._On_Page()" functions used by the mouse movement handler
    ¿ factoring in the v-sync changes the conceptualization here ?


Elsewhere,
    quit from the game vs. close the the platform window
    use the quit flag in the game, expose it to the platform
    what should the game code do when a user closes the window?
        auto-save?
        same as Game > Quit > Quit To DOS?
        


    







Platform-Layer
API Specification
MoM-Platform.H
-includes MoM Headers

Platform-Side:
    FS
        Find, Open, Read, Write, Close
    MM
        Allocate, Deallocate
    KD
        Key Press
    MD
        Mouse Move
        Mouse Button
    VD
        Translate From Game Video Buffer Format to Platform Video Buffer Format
        *Paint* *Canvas* to *Display*
        IBM-PC VGA VRAM (platform_video_memory)
        IBM-PC VGA DAC  (platform_palette)
    CLK
        Timer Ticks

Game-Side:
    API Specification
        Set Expectations for the Platform-Layer
    Pump Events
        Trigger the Processing of Awaiting Events on the Platform-Layer
    Video Back-Buffer
        ...provide bytes to be rendered on the display device...
    

Win Main
    Init
    Evnt

MoM Main

MoM Platform








# MoM - Platform-Layer

ya see, what it is is...
let's say you want the game to compile for and run on MS-DOS and MS-Windows...
you gotta extract the code that is platform-specific...
say like, for MS-DOS, you're call INT 33,3 to get the mouse button status...
well, on Windows, there ain't no such thing
so, you gotta move that MD_GetButtonStatus() function out to a platform-layer code mode
i.e., dos_MD.C and win_MD.C
but, you don't want the rest of the code to care about that or have things in multiple places
so, you put the declaration in one header file and the implementations in separate platform-specific source files
then, you just include that *shared* header file everywhere that anything else needs to know what is where
later, when you get to compiling and linking, just offer up the source files for the target platform

So, just leave Input, Keyboard, and Mouse as they are?
? still need to finish making decision about IN vs. KD vs. MD, etc. ?
? what happens when you get to JD, GD, HD, etc. ?


Mouse.C includes Mouse.H
dos_MD.C includes Mouse.H
win_MD.C includes Mouse.H
anything else that needs to know about anything specific to the mouse includes Mouse.H


20230629:
I am feeling like Pointer_X() and Pointer_Y() should be in the platform layer, along with MD_GetButtonStatus()
Then, the platform can set mouse x,y,buttons of its own, its own way, with the getters translating to the MoX API specification.
Caveat? MoX Mouse expects mouse_x,mouse_y to be available internally/privately?
    OON XREF: Init_Mouse_Driver() sets it directly






Keyboard_Status()
Read_Key()
MD_GetButtonStatus()  // MoO2: Mouse_Button() INT 33,3
...
The existance of Pointer_X() and Pointer_Y() means that mouse_x and mouse_y were module-scoped/private.
Why? I dunno.
? with the platform-layer shift, they can no londer be so ?
...




? "Fields" ~== "Input Controls" ?
//MoO2
WIZARDS.EXE
seg035  Fields - Asm
seg036  Fields - C
multiple source files, one translation unit AKA A Library

Fields.C
Mouse.C
Keyboard.C
Fields.ASM
Keyboard.ASM
Mouse.ASM
