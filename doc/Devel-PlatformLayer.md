
Platform-Layer (PFL)
AKA Platform Abstraction Layer (PAL)
AKA Hardware Abstraction Layer (HAL)

NOTE(JimBalcomb,20260323):
sdl2,sdl3,win32
...gonna get get to text from chocolate-doom
FWIW, ...
...trying to get having an AI-Prompt that gets us the right arrangement
...
Should be, CMake has presets for all four platform layers.
The autoconf/make build should target SDL2, as it comes with MSYS2 and Ubuntu/Kubuntu.





Game vs. Platform

¿ Platform ?
...hardware, operating system, file system, game engine/framework/library, etc.







MoM
keyboard
mouse
video



## When and where does input get handled?

Get_Input() |-> Interpret_Mouser_Input()
Keyboard_Status()
    Interpret_Keyboard_Input()
        Read_Key()
        hotkey
        dirkey










## RefMat

https://github.com/Pilzschaf/grounded
