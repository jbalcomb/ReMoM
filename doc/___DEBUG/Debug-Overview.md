


~ STU_DBG


STU_DEBUG

// e.g.,  #ifdef STU_DEBUG
// e.g.,  #include "STU/STU_DBG.h"
// e.g.,  #endif

CMake vs. VSC vs. VS  ...(VSC w/wo CMAKE), (VSC w/wo clangd), (VSC w/wo CMAKE)
CMake:
    CMakeLists.txt
        add_compile_definitions(STU_DEBUG)
        target_compile_definitions(MOX PRIVATE STU_DEBUG)
VSC:
    c_cpp_properties.json

    VSCode: File->Preferences->Settings->C/C++->IntelliSense->C_Cpp > Default: Defines
        The value to use in a configuration if defines is not specified, or the values to insert if ${default} is present in defines.
    ...didn't do anything



¿ Process ?



e.g.,
Why are Units disappearing from the min movement map?
Are they just not being draw or are they being killed, deleted, or corrupted?
How to know which data changes are valid?
¿ snap-shot before and after, then step through a whole 'Next Turn', deciding what should or should not change ?
output a diff?
