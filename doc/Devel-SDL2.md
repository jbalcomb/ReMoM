
SDL2





## Page_Flip()
On Windows, we force a paint event in Pump_Paints(), called from MoM's Page_Flip().
Shoulda notta named it Pump_Paints()...
...it updates the screen
...which means we have to called Update_Window_Display()

Copy palette
set surface1-format->palette
copy MoM VBB to surface1->pixels
blit surface1 to surface2
lock texture
copy surface2 to texture


Copy palette
set palette for 8-bit surface
Lock the texture, pointing it at the 32-bit surface's pixels  (requires SDL_TEXTUREACCESS_STREAMING)
Blit 8-bit surface to 32-bit surface  (RGB666 ==> ARGB8888)
...thereby *copying* it to the texture




SDL_Init()
SDL_CreateWindow()
SDL_CreateRenderer()

SDL_DestroyRenderer()
SDL_DestroyWindow()
SDL_Quit()



SDL_Window *SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
x,y window position
w,h window size, in screen coordinates  (max 16384 x 16384)

const char *	title	the title of the window, in UTF-8 encoding.
Uint32	flags	0, or one or more SDL_WindowFlags OR'd together.
fullscreen, support opengl, vulkan, metal, hidden, borderless, resizabe, minimized, maximized, input grabbed, allow highdpi


TODO
get the highdpi, fullscreen and center window code from chocolate-doom  (i_video.c)


## chocolate-doom

main()
    |-> D_DoomMain()
        |-> D_DoomLoop()
            |-> I_InitGraphics()

D_DoomLoop()
    |-> D_RunFrame()
        |-> I_FinishUpdate()

WHATSIT
    // The screen buffer that the v_video.c code draws to.
    static pixel_t *dest_screen = NULL;

    // The screen buffer; this is modified to draw things to the screen
    pixel_t *I_VideoBuffer = NULL;



Unroll it?



¿ I_FinishUpdate() ~== Update_MoX_Texture() ?


//
// I_SetPalette
//
void I_SetPalette (byte *doompalette)
{
    int i;

    for (i=0; i<256; ++i)
    {
        // Zero out the bottom two bits of each channel - the PC VGA
        // controller only supports 6 bits of accuracy.

        palette[i].a = 0xFFU;
        palette[i].r = gammatable[usegamma][*doompalette++] & ~3;
        palette[i].g = gammatable[usegamma][*doompalette++] & ~3;
        palette[i].b = gammatable[usegamma][*doompalette++] & ~3;
    }

    palette_to_set = true;
}



## MoM Match-Up

¿ chocolate-doom's per frame structure is something like MoM's screen-loop ?

MoM draws to video_page_buffer[4], which emulates the 256K of IBM-PC VRAM



FLIC_Load_Palette()
    sets values in p_Palette from flic_palette_data

FLIC_Draw() |-> FLIC_Draw_Frame() *draws* to `current_video_page + ((y_start * SCREEN_WIDTH) + x_start)`

draw_page_num as in the page that is *drawn* to the *screen*

int16_t draw_page_num = 0;
uint8_t * current_video_page;
uint8_t * video_memory;
uint8_t * video_page_buffer[4];  // On, Off, Back, ¿ Back2 ?
uint8_t * draw_page;

void Set_Page_On(void)
    /*
        ~== VRAM + ((draw_page_num) * 4)  {0xA000, 0xA400}
    */
    current_video_page = video_page_buffer[(draw_page_num)];

Set_Page_Off()
    /*
        ~== VRAM + ((1 - draw_page_num) * 4)  {0xA000, 0xA400}
    */
    current_video_page = video_page_buffer[(1 - draw_page_num)];







C:\STU\devel\STU-Extras\STU-SDL\STU-SDL.cpp
    SDL_SetPaletteColors(g_mox_vbb_surface1->format->palette, mox_sdl_surface1_palette, 0, 256);
C:\STU\developp\chocolate-doom\src\i_video.c
    SDL_SetPaletteColors(screenbuffer->format->palette, palette, 0, 256);





## Chocolate Doom


C:\STU\developp\chocolate-doom\src\i_video.c

// These are (1) the window (or the full screen) that our game is rendered to
// and (2) the renderer that scales the texture (see below) into this window.

static SDL_Window *screen;
static SDL_Renderer *renderer;

// Window title

static const char *window_title = "";

// These are (1) the 320x200x8 paletted buffer that we draw to (i.e. the one
// that holds I_VideoBuffer), (2) the 320x200x32 RGBA intermediate buffer that
// we blit the former buffer to, (3) the intermediate 320x200 texture that we
// load the RGBA buffer to and that we render into another texture (4) which
// is upscaled by an integer factor UPSCALE using "nearest" scaling and which
// in turn is finally rendered to screen using "linear" scaling.

static SDL_Surface *screenbuffer = NULL;
static SDL_Surface *argbbuffer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Texture *texture_upscaled = NULL;

static SDL_Rect blit_rect = {
    0,
    0,
    SCREENWIDTH,
    SCREENHEIGHT
};

// palette

static SDL_Color palette[256];
static boolean palette_to_set;






## OpenXcom
Game -> Screen -> Surface
default: 640 x 480, 8bpp

Screen() uses resetDisplay() to initialize

no calls to
    SDL_CreateWindow()
    SDL_CreateRenderer()











#include <SDL.h>

¿ SDLmain ?

SDL_Init()


## PoC-SDL2

```c
#include <SDL.h>
SDL_Window * g_window;
SDL_Renderer * g_renderer;
SDL_Texture * g_texture;
int main(int, char**)
{
    // Startup
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags sdl_window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    g_window = SDL_CreateWindow(sdl_window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sdl_window_width, sdl_window_height, sdl_window_flags);

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Operate
    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
        }
    }
    // Shutdown
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
    return 0;
}
```



SDL_GetError()


## RefMat

Lazy Foo' Productions
