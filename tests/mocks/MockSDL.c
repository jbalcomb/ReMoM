#include "MockSDL.h"

int SDL_Init(unsigned int flags) { (void)flags; return 0; }
void SDL_Quit(void) { }
unsigned int SDL_GetTicks(void) { return 0; }
