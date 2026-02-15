#ifndef MOCKSDL_H
#define MOCKSDL_H

#ifdef __cplusplus
extern "C" {
#endif

// Minimal SDL stubs used by MoX sdl2 adapter when tests are run headless.
// Only provide the symbols the code expects at link time; implementations may be empty.

int SDL_Init(unsigned int flags);
void SDL_Quit(void);

// Timing stub
unsigned int SDL_GetTicks(void);


#ifdef __cplusplus
}
#endif

#endif  /* MOCKSDL_H */
