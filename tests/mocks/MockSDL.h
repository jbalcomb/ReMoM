#pragma once

// Minimal SDL stubs used by MoX sdl2 adapter when tests are run headless.
// Only provide the symbols the code expects at link time; implementations may be empty.

typedef unsigned int Uint32;
typedef unsigned short Uint16;

int SDL_Init(unsigned int flags);
void SDL_Quit(void);

// Timing stub
unsigned int SDL_GetTicks(void);

