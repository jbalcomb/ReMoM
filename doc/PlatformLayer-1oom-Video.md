




*.* |-> SDL_RenderPresent()
video_update()
video_sw_set()


hw_video_init()
hw_video_toggle_fullscreen()
hw_video_update_aspect()






Program-Flow
MOX loads a palette
MOX draws something to the VRAM buffer
maybe, MOX updates the palette
...
copy the VRAM buffer to the platform-layer 6BPP video buffer
copy the platform-layer 6BPP video buffer to the platform-layer 8BPP video buffer
update what is displayed in the platform-layer window



Elsewhere, MOX directly updates the VGA DAC  (e.g., color-cycling)
~== write directly to the platform-layer palette and, in real-time, update the platform-layer window display


Presently, ....
...on Windows, Update_Window_Display() uses PFL_Palette to update PlatformVideoBackBuffer
...only called from WndEvnt(), on WM_PAINT
...PFL_Palette gets updated in Apply_Palette(), Cycle_Palette(), 

1oom
SDL_SetPaletteColors(video.screen->format->palette, video.color, 0, 256);
static struct sdl_video_s {
    SDL_Surface *screen;
    SDL_Color color[256];
} video = { 0 };

// copy the faux VGA DAC to the platform-layer equivalent
static void video_setpal(uint8_t *pal, int first, int num)

¿ equivalent to Load_Palette(), in the loop that copies from palette_data to current_palette ?

1oom calls video_setpal(), which updates the PFL surface/texture/window palette, sets palette_to_set, and calls video_update().
video_update() calls SDL_SetPaletteColors() which copies the PFL palette to the surface.

void hw_video_refresh_palette(void)
void hw_video_set_palette(uint8_t *pal, int first, int num)
static void video_setpal(uint8_t *pal, int first, int num)

    hw_video_init()
    hw_video_refresh_palette()
Apply_Palette()
Cycle_Palette()
    ui_palette_update()
    hw_video_refresh_palette()
        hw_video_set_palette(video.pal, 0, 256);
            video_setpal(pal, first, num);
                video_update()
                    SDL_SetPaletteColors()
                    ...
                    SDL_RenderPresent()

so, ...
    hw_video_refresh_palette() is hard-coded to update every color value in sdl2_palette from video.pal
...where video.pal is?


¿ struct sdl_video_s { uint8_t pal[256 * 3]; } ?
...really, just for screenshots?

¿ struct sdl_video_s { bool palette_to_set; } ?
...~pointless?

void lbxpal_update(void)
    memset(lbxpal_update_flag, 0, sizeof(lbxpal_update_flag));
    hw_video_set_palette(lbxpal_palette, 0, 256);
        video_setpal(pal, first, num);




## video.palette_to_set
What is the raison d'être of video.palette_to_set?

...tested and set in...
    video_update()
        if (video.need_resize)
            if (SDL_GetTicks() > (video.last_resize_time + RESIZE_DELAY))
                video.palette_to_set = true;
        ...
        if(video.palette_to_set)
        {
            SDL_SetPaletteColors(sdl2_surface_RGB666->format->palette, video.color, 0, 256);
            video.palette_to_set = false;
        }
...set in...
    video.setpal()
        video.palette_to_set = true;

So, ...
    actualy has no raison d'être?



win_PFL
treats PFL_Palette like the VGA DAC
p_Palette is current_palette; 1024 bytes, 768 colors, 256 flags
Load_Font_File()
    copies palette_data to p_Palette
Load_Palette()
    copies palette_data to p_Palette
FLIC_Load_Palette()
    copies flic_palette_data to p_Palette
Apply_Palette()
    copies p_Palette to PFL_Palette
Cycle_Palette()
    copies p_Palette to PFL_Palette


Can we just have one shadow palette, in the platform-layer proper?

Load_Palette()?
...palette_data...current_palette...
...p_PaletteFlags...







Where, in MoO1/MoM, do we need to process platform events?
How did that work for IBM-PC/MS-DOS?
...hardware interrupt


Release_Time()
    hw_event_handle()
        SDL_Event e;
        SDL_PumpEvents();
        while (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_WINDOWEVENT:
                {
                    if(e.window.windowID == hw_video_get_window_id())
                    {
                        hw_event_handle_window(&e.window);
                    }
                } break;
            }
        }



hw_event_handle_window()


video_update()
    |-> ...
    |-> SDL_RenderPresent()



wWinMain()
    |-> Init_Drivers()
    |-> Init_Platform()

WndEvnt()
    |-> Update_Window_Display(&PlatformVideoBackBuffer, hdc, WndDim.Width, WndDim.Height);



## Page_Flip()

...on Windows
    draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'
    draw_page = video_page_buffer[draw_page_num];
    Pump_Paints();
    Set_Page_Off();


## video_render()
memcpy() to video.screen->pixels from video.buf[bufi]


## video_update()
Where does this get set for Kilgore's SDL2 PFL?
hw_video_init()
...assigns video.render(), video.update(), video.setpal()

video.screen ~== sdl2_surface_RGB666



sdl2_video.c
```c
static struct sdl_video_s {
    void (*update)(void);
} video = { 0 };
int hw_video_init(int w, int h)
{
    video.update = video_update;
}
static void video_update(void)
{
    if (video.need_resize) {
        if (SDL_GetTicks() > (video.last_resize_time + RESIZE_DELAY)) {
            int flags, w, h;
            flags = SDL_GetWindowFlags(video.window);
            if ((flags & (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MAXIMIZED)) == 0) {
                SDL_GetWindowSize(video.window, &w, &h);
                video_adjust_window_size(&w, &h);
                SDL_SetWindowSize(video.window, w, h);
                hw_opt_screen_winw = w;
                hw_opt_screen_winh = h;
            }
            video_create_upscaled_texture(false);
            video.need_resize = false;
            video.palette_to_set = true;
        } else {
            return;
        }
    }
    if (video.palette_to_set) {
        SDL_SetPaletteColors(video.screen->format->palette, video.color, 0, 256);
        video.palette_to_set = false;
    }
    SDL_BlitSurface(video.screen, &video.blit_rect, video.interbuffer, &video.blit_rect);
    {
        void *pixels;
        int pitch;
        if (SDL_LockTexture(video.texture, NULL, &pixels, &pitch) == 0) {
            memcpy(pixels, video.interbuffer->pixels, pitch*video.bufh);
            SDL_UnlockTexture(video.texture);
        } else {
            log_error("SDL_LockTexture(video.texture): %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    SDL_RenderClear(video.renderer);
    SDL_SetRenderTarget(video.renderer, video.texture_upscaled);
    SDL_RenderCopy(video.renderer, video.texture, NULL, NULL);
    SDL_SetRenderTarget(video.renderer, NULL);
    SDL_RenderCopy(video.renderer, video.texture_upscaled, NULL, NULL);
    SDL_RenderPresent(video.renderer);
}
```


## hw_video_redraw_front()

hw_video_redraw_front()
    |-> hw_video_refresh(1)
vs.
Page_Flip()
    |-> hw_video_refresh(0)



ui_cursor_refresh()
ui_draw_finish_wipe_anim()
ui_news_fade()
    hw_video_redraw_front()
        hw_video_refresh(1)
            SDL_MUSTLOCK(video.screen)
            SDL_LockSurface(video.screen)
            video.render(video.bufi ^ front)
            SDL_MUSTLOCK(video.screen)
            SDL_UnlockSurface(video.screen)
            video.update()

Why am I spinning my gears on this?
What does it do and not do?
What is the intent?
~== MWA_PFL  Update_Window_Display()
update what is displayed to the user
//SDL2
copy the game VBB to the PFL VBB [+ surface] [+ texture]
...lock, blit, unlock, render, present

// ¿ ~== Update_Window_Display() ?
void hw_video_redraw_front(void)
{
    /*
        BEGIN:  hw_video_refresh(1)
    */

    if(SDL_MUSTLOCK(sdl2_surface_RGB666))
    {
        if(SDL_LockSurface(sdl2_surface_RGB666) < 0)
        {
            return;
        }
    }

    /*
        BEGIN:  video_render(draw_page_num ^ 1)
    */
    {
        int pitch = sdl2_surface_RGB666->pitch;
        Uint8 *dst = (Uint8 *)sdl2_surface_RGB666->pixels;
        uint8_t *src = video_page_buffer[(draw_page_num ^ 1)];
        for(int y = 0; y < screen_pixel_height; ++y)
        {
            memcpy(dst, src, screen_pixel_width);
            dst += pitch;
            src += screen_pixel_width;
        }
    }
    /*
        END:  video_render(draw_page_num ^ 1)
    */

    if(SDL_MUSTLOCK(sdl2_surface_RGB666))
    {
        SDL_UnlockSurface(sdl2_surface_RGB666);
    }

    /*
        BEGIN:  video_update()
    */
    {
        // if(video.need_resize)
        // {
        //     if(SDL_GetTicks() > (video.last_resize_time + RESIZE_DELAY))
        //     {
        //         int flags, w, h;
        //         flags = SDL_GetWindowFlags(sdl2_window);
        //         if((flags & (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MAXIMIZED)) == 0)
        //         {
        //             SDL_GetWindowSize(sdl2_window, &w, &h);
        //             // video_adjust_window_size(&w, &h);
        //             SDL_SetWindowSize(sdl2_window, w, h);
        //             // ¿ fullscreen ?  hw_opt_screen_winw = w;
        //             // ¿ fullscreen ?  hw_opt_screen_winh = h;
        //         }
        //         video_create_upscaled_texture(false);
        //         video.need_resize = false;
        //         video.palette_to_set = true;
        //     }
        //     else
        //     {
        //         return;
        //     }
        // }

        if(video.palette_to_set)
        {
            SDL_SetPaletteColors(sdl2_surface_RGB666->format->palette, video.color, 0, 256);
            video.palette_to_set = false;
        }

        SDL_BlitSurface(sdl2_surface_RGB666, &video.blit_rect, sdl2_surface_ARGB8888, &video.blit_rect);

        {
            void *pixels;
            int pitch;
            if(SDL_LockTexture(sdl2_texture, NULL, &pixels, &pitch) == 0)
            {
                memcpy(pixels, sdl2_surface_ARGB8888->pixels, pitch*screen_pixel_height);
                SDL_UnlockTexture(sdl2_texture);
            }
            else
            {
                // log_error("SDL_LockTexture(sdl2_texture): %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }
        }
        SDL_RenderClear(sdl2_renderer);
        SDL_SetRenderTarget(sdl2_renderer, sdl2_texture_upscaled);
        SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, NULL);
        SDL_SetRenderTarget(sdl2_renderer, NULL);
        SDL_RenderCopy(sdl2_renderer, sdl2_texture_upscaled, NULL, NULL);
        SDL_RenderPresent(sdl2_renderer);
    }

    /*
        END:  hw_video_refresh(1)
    */

}
