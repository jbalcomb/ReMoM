

ReMoM.C
SDL_main()
    Startup_Platform()
    MOM_main()
    Shudown_Platform()

sdl2_MOM.C
void Startup_Platform(void)
    SDL_Init()
    SDL_CreateWindow()
    SDL_CreateRenderer()
    SDL_CreateRGBSurface()
    SDL_FillRect()
    SDL_CreateRGBSurfaceWithFormatFrom()
    SDL_CreateTexture()
    SDL_ShowCursor()
void Shudown_Platform(void)
    SDL_ShowCursor()
    SDL_DestroyRenderer()
    SDL_DestroyWindow()
    SDL_Quit()

keyboard
mouse
timer
...LOF(),DIR()...
EMS/EMM
