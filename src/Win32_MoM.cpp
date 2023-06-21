
#include "MoX_TYPE.H"

#include "MoM.H"

#include "windows.h"

// ? #include "Win32.hpp"

#include "MoM_main.H"  /* ONN: g_State_Run */

// #define STU_DEBUG 1
#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


void Pump_Events(void)
{
        MSG Message;
        while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT)
            {
                g_State_Run = false;
            }
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
}
