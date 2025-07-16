#ifndef SDL2_MOM_H
#define SDL2_MOM_H

#include "MOX/MOX_TYPE.h"

#include <SDL.h>



#ifdef __cplusplus
extern "C" {
#endif

void Platform_Mouse_Input_Enable(void);
void Platform_Mouse_Input_Disable(void);

void Platform_Keyboard_Event(SDL_Event * sdl2_event);
void Platform_Keyboard_Buffer_Clear(void);

#ifdef __cplusplus
}
#endif

#endif  /* #define SDL2_MOM_H
*/
