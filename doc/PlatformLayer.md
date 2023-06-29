

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
