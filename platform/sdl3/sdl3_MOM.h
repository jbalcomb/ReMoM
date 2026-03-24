#ifndef SDL3_MOM_H
#define SDL3_MOM_H

#include <stdint.h>

#include <SDL3/SDL.h>



#ifdef __cplusplus
extern "C" {
#endif

void Platform_Mouse_Input_Enable(void);
void Platform_Mouse_Input_Disable(void);

void Platform_Keyboard_Event(SDL_Event * sdl3_event);
void Platform_Keyboard_Buffer_Clear(void);

#ifdef __cplusplus
}
#endif

#endif  /* #define SDL3_MOM_H
*/
