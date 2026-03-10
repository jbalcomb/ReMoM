MoX-Palette-Cycle_Palette_Color.md



¿ 1oom ?



MoO2
Module: palette
    function (0 bytes) Cycle_Palette_Color
    Address: 01:00131B5D
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) color_num
            signed integer (2 bytes) red_min
            signed integer (2 bytes) green_min
            signed integer (2 bytes) blue_min
            signed integer (2 bytes) red_max
            signed integer (2 bytes) green_max
            signed integer (2 bytes) blue_max
            signed integer (2 bytes) step_value
            signed integer (4 bytes) delta_red
            signed integer (4 bytes) delta_green
            signed integer (4 bytes) delta_blue
            signed integer (4 bytes) primary_color
            signed integer (4 bytes) store_red
            signed integer (4 bytes) store_blue
            signed integer (4 bytes) store_green


MGC/WZD seg020
...looks like MoO2 modules fonts, palette, remap





//Gemini3Pro

This assembly code is a classic routine for VGA Palette Cycling. It smoothly transitions (or "fades") a single palette color from one RGB state to another over time by using linear interpolation.

Here is what the equivalent C89 code would look like, followed by a breakdown of a few fascinating quirks in the assembly.

The C89 Translation

Assuming standard 16-bit DOS compiler types (where int or short is 16 bits), the C code looks like this:

```c
#include <dos.h> /* For union REGS and int86 */

/* Global state variables assumed by the assembly */
extern short cycle_step_value;
extern short cycle_direction_flag;
extern short cycle_color_value;

/* External helper function to tick the animation logic */
extern void Update_Cycle(short *min_val, short *max_val);

void far Cycle_Palette_Color__STUB(short color_num, 
                                   short red_min, short green_min, short blue_min, 
                                   short red_max, short green_max, short blue_max, 
                                   short step_value) 
{
    short delta_red, delta_green, delta_blue;
    short store_red, store_green, store_blue;
    short primary_color; /* 0 = Red, 1 = Green, 2 = Blue */
    union REGS regs;

    /* 1. Calculate Absolute Deltas */
    delta_red = red_max - red_min;
    delta_green = green_max - green_min;
    delta_blue = blue_max - blue_min;

    if (delta_red < 0) delta_red = -delta_red;
    if (delta_green < 0) delta_green = -delta_green;
    if (delta_blue < 0) delta_blue = -delta_blue;

    cycle_step_value = step_value;

    /* 2. Determine Primary Color Channel (the one with the largest change) */
    primary_color = 2; /* Default to Blue */
    if (delta_red >= delta_green && delta_red >= delta_blue) {
        primary_color = 0;
    } else if (delta_green >= delta_red && delta_green >= delta_blue) {
        primary_color = 1;
    }

    /* 3. Handle Initialization / Direction Reset */
    if (cycle_direction_flag == -1) {
        cycle_direction_flag = 0;
        if (primary_color == 0)      cycle_color_value = red_min;
        else if (primary_color == 1) cycle_color_value = green_min;
        else if (primary_color == 2) cycle_color_value = blue_min;
    }

    /* 4. Interpolate the other two colors based on the primary color */
    switch (primary_color) {
        case 0: /* Red is Primary */
            store_red = cycle_color_value;
            store_green = ((green_max - green_min) * (cycle_color_value - red_min)) / (red_max - red_min) + green_min;
            store_blue  = ((blue_max - blue_min)  * (cycle_color_value - red_min)) / (red_max - red_min) + blue_min;
            break;

        case 1: /* Green is Primary */
            store_green = cycle_color_value;
            store_red  = ((red_max - red_min)   * (cycle_color_value - green_min)) / (green_max - green_min) + red_min;
            store_blue = ((blue_max - blue_min) * (cycle_color_value - green_min)) / (green_max - green_min) + blue_min;
            break;

        case 2: /* Blue is Primary */
            store_blue = cycle_color_value;
            store_green = ((green_max - green_min) * (cycle_color_value - blue_min)) / (blue_max - blue_min) + green_min;
            store_red   = ((red_max - red_min)   * (cycle_color_value - blue_min)) / (blue_max - blue_min) + red_min;
            break;
    }

    /* 5. Advance the Cycle State */
    if (primary_color == 0) {
        Update_Cycle(&red_min, &red_max);
    } else if (primary_color == 1) {
        Update_Cycle(&green_min, &green_max);
    } else if (primary_color == 2) {
        Update_Cycle(&blue_min, &blue_max);
    }

    /* 6. Hardware Update: VGA BIOS Interrupt 10h, Function 1010h */
    regs.x.ax = 0x1010;
    regs.x.bx = color_num;
    regs.h.dh = (unsigned char)store_red;
    regs.h.ch = (unsigned char)store_green;
    regs.h.cl = (unsigned char)store_blue;
    int86(0x10, &regs, &regs);
}
```

Fascinating Assembly Quirks in this Code
  1. The "Lost Math" Compiler Bug:
    If you look at how the interpolation is calculated in assembly:
      imul    dx   ; Multiplies AX * DX, storing 32-bit result in DX:AX
      mov     bx, [bp+green_max]
      sub     bx, [bp+green_min]
      cwd          ; Sign-extends AX into DX:AX
      idiv    bx   ; Divides DX:AX by BX
    The imul instruction successfully creates a 32-bit result across DX and AX. However, immediately before dividing, the compiler inserts cwd (Convert Word to Doubleword). This instruction overwrites whatever was in DX by sign-extending AX!
    This is undeniable proof that the original C programmer did not cast the math to a long. They wrote standard 16-bit math (a * b) / c. The compiler generated a 16-bit multiplication, threw away the upper 16-bits of the result via cwd, and then did a 32-bit division. If the distance between colors times the step gets too large, this interpolation will overflow and glitch visually!
  2. BIOS Interrupt 10h:
    The very end of the routine skips DOS wrappers and directly invokes the system BIOS. Setting AX = 1010h and calling int 10h tells the VGA card: "Update a single DAC color register" where BX is the palette index, and DH, CH, CL hold the RGB values.



Moving from bare-metal DOS interrupts to a modern API like SDL2 is where the translation process gets really fun. Instead of asking the BIOS to manually tweak the voltages on the VGA card's Digital-to-Analog Converter (DAC), you are updating a software palette object.

There is one critical mathematical difference you have to account for here: Color Depth.

VGA Hardware: The DOS int 10h DAC registers use 6-bit color. Red, Green, and Blue values range from 0 to 63.

SDL2: Modern graphics use 8-bit color per channel. Red, Green, and Blue values range from 0 to 255.

If you directly plug the DOS values into SDL2, your colors will be incredibly dark (maxing out at 63/255, or about 25% brightness). You have to scale them up.

The SDL2 Translation

Assuming you have your game's palette stored in an SDL_Palette* (which is standard for 8-bit indexed surfaces in SDL2), here is what that final block of code looks like:

```c
#include <SDL.h>

/* Assume this is your global or passed-in game palette */
extern SDL_Palette* game_palette; 

/* ... previous interpolation code ... */

/* 6. Hardware Update: SDL2 Palette Swap */
{
    SDL_Color new_color;

    /* Scale the 6-bit VGA values (0-63) up to 8-bit SDL values (0-255) */
    /* Multiplying by 255 and dividing by 63 is the most accurate linear scale */
    new_color.r = (Uint8)((store_red   * 255) / 63);
    new_color.g = (Uint8)((store_green * 255) / 63);
    new_color.b = (Uint8)((store_blue  * 255) / 63);
    new_color.a = 255; /* Fully opaque */

    /* Update the specific color index in the SDL_Palette */
    /* Arguments: (palette, array of colors, starting index, number of colors to update) */
    if (game_palette != NULL) {
        SDL_SetPaletteColors(game_palette, &new_color, color_num, 1);
    }
}

```

A Quick Note on Scaling Options
If you want to keep the math strictly to bitwise operations to match the retro vibe (and speed, though modern CPUs won't care), a common "fast and dirty" way to convert 6-bit to 8-bit is to shift the bits left by 2:
```c
new_color.r = (Uint8)(store_red << 2);
```
Note: Shifting by 2 gives a max value of 252 (63 << 2 = 252), which is slightly off from pure white (255), but in a 90s retro game, the visual difference is virtually imperceptible.



Excellent. Getting an 8-bit indexed surface running in SDL2 is the key to authentic retro graphics.

In the DOS era, the VGA hardware natively understood that a pixel with a value of 15 meant "look up color #15 in the DAC." Modern GPUs, however, only understand 32-bit RGBA (Red, Green, Blue, Alpha).

To bridge this gap, SDL2 allows you to create an 8-bit "software" surface and attach a palette to it. When it's time to draw to the screen, SDL automatically translates your 8-bit pixels and palette into the 32-bit texture the GPU needs.

Here is how you set that pipeline up.

Step 1: Initialization and Binding
First, you need to create your global palette and the 8-bit surface that will act as your game's canvas (e.g., the classic 320x200 resolution).

```c
#include <SDL.h>

/* Global variables for your retro pipeline */
SDL_Palette* game_palette = NULL;
SDL_Surface* vga_surface = NULL;

int Init_VGA_Display(int width, int height) {
    /* 1. Allocate a 256-color palette */
    game_palette = SDL_AllocPalette(256);
    if (!game_palette) {
        return -1; /* Allocation failed */
    }

    /* (You would typically load your default 256 colors into the palette here) */

    /* 2. Create an 8-bit indexed surface */
    /* SDL_PIXELFORMAT_INDEX8 tells SDL this surface uses a palette */
    vga_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 8, SDL_PIXELFORMAT_INDEX8);
    if (!vga_surface) {
        return -1;
    }

    /* 3. Bind the palette to the surface */
    SDL_SetSurfacePalette(vga_surface, game_palette);

    return 0; /* Success */
}
```

Step 2: Drawing Pixels
Because your surface is 8-bit (INDEX8), you don't write RGB values to it. You write single bytes (0-255) representing the palette index.

```c
void PutPixel_8bit(int x, int y, Uint8 color_index) {
    Uint8* pixels;
    
    /* Boundary check */
    if (x < 0 || y < 0 || x >= vga_surface->w || y >= vga_surface->h) return;

    /* Lock surface if required (usually not needed for software surfaces, but good practice) */
    if (SDL_MUSTLOCK(vga_surface)) SDL_LockSurface(vga_surface);

    /* Calculate the memory offset and set the index byte */
    pixels = (Uint8*)vga_surface->pixels;
    pixels[(y * vga_surface->pitch) + x] = color_index;

    if (SDL_MUSTLOCK(vga_surface)) SDL_UnlockSurface(vga_surface);
}
```

Step 3: The Render Loop (The "Magic")
When your Cycle_Palette_Color__STUB translation calls SDL_SetPaletteColors(), the game_palette is updated in memory. However, the screen won't change until you push vga_surface to the GPU.

In your main game loop, you create a texture from the surface and render it. Because the surface is bound to the palette, SDL will automatically recalculate the RGB values for every pixel on the screen when it creates the texture.

```c
/* Assuming you already have an SDL_Window* window and SDL_Renderer* renderer */

void Render_Frame(SDL_Renderer* renderer) {
    SDL_Texture* frame_texture;

    /* * This converts the 8-bit surface + current palette into a 32-bit GPU texture.
     * Because SDL does the translation here, any palette cycling you did 
     * this frame will instantly be reflected!
     */
    frame_texture = SDL_CreateTextureFromSurface(renderer, vga_surface);

    /* Clear the screen, copy the texture, and present it */
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, frame_texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    /* Clean up the texture so we don't leak memory every frame */
    SDL_DestroyTexture(frame_texture);
}
```

Why this is perfect for Retro Games
If you have a waterfall on your map drawn with color index 101, and you change the RGB value of 101 in game_palette using your cycling function, every single pixel on the map using index 101 will magically change color on the next Render_Frame() call, exactly like it did on a 486 running DOS. You don't have to redraw the map!

Would you like to look at how to optimize that rendering step? Creating and destroying a texture every frame can be heavy; we could set up an SDL_TEXTUREACCESS_STREAMING texture instead if you need to squeeze out more performance.







```asm
proc Cycle_Palette_Color__STUB far
store_green= word ptr -0Ah
store_blue= word ptr -8
store_red= word	ptr -6
primary_color= word ptr	-4
delta_blue= word ptr -2
color_num= word	ptr  6
red_min= word ptr  8
green_min= word	ptr  0Ah
blue_min= word ptr  0Ch
red_max= word ptr  0Eh
green_max= word	ptr  10h
blue_max= word ptr  12h
step_value= word ptr  14h
delta_red = si
delta_green = di
push	bp
mov	bp, sp
sub	sp, 0Ah
push	delta_red
push	delta_green
mov	ax, [bp+red_max]
sub	ax, [bp+red_min]
mov	delta_red, ax
mov	ax, [bp+green_max]
sub	ax, [bp+green_min]
mov	delta_green, ax
mov	ax, [bp+blue_max]
sub	ax, [bp+blue_min]
mov	[bp+delta_blue], ax
or	delta_red, delta_red
jge	short loc_1CAFE
mov	ax, delta_red
neg	ax
mov	delta_red, ax
loc_1CAFE:
or	delta_green, delta_green
jge	short loc_1CB08
mov	ax, delta_green
neg	ax
mov	delta_green, ax
loc_1CB08:
cmp	[bp+delta_blue], 0
jge	short loc_1CB16
mov	ax, [bp+delta_blue]
neg	ax
mov	[bp+delta_blue], ax
loc_1CB16:
mov	ax, [bp+step_value]
mov	[cycle_step_value], ax
mov	[bp+primary_color], 2
cmp	delta_red, delta_green
jl	short loc_1CB31
cmp	delta_red, [bp+delta_blue]
jl	short loc_1CB31
mov	[bp+primary_color], 0
jmp	short loc_1CB3F
loc_1CB31:
cmp	delta_green, delta_red
jl	short loc_1CB3F
cmp	delta_green, [bp+delta_blue]
jl	short loc_1CB3F
mov	[bp+primary_color], 1
loc_1CB3F:
cmp	[cycle_direction_flag],	-1
jnz	short loc_1CB71
mov	[cycle_direction_flag],	0
mov	ax, [bp+primary_color]
or	ax, ax
jz	short loc_1CB5F
cmp	ax, 1
jz	short loc_1CB67
cmp	ax, 2
jz	short loc_1CB6C
jmp	short loc_1CB71
loc_1CB5F:
mov	ax, [bp+red_min]
loc_1CB62:
mov	[cycle_color_value], ax
jmp	short loc_1CB71
loc_1CB67:
mov	ax, [bp+green_min]
jmp	short loc_1CB62
loc_1CB6C:
mov	ax, [bp+blue_min]
jmp	short loc_1CB62
loc_1CB71:
mov	ax, [bp+primary_color]
or	ax, ax
jz	short @@Do_Red_Calc
cmp	ax, 1
jz	short @@Do_Green_Calc
cmp	ax, 2
jnz	short loc_1CB85
jmp	@@Do_Blue_Calc
loc_1CB85:
jmp	loc_1CC4C
@@Do_Red_Calc:
mov	ax, [cycle_color_value]
mov	[bp+store_red],	ax
mov	ax, [bp+green_max]
sub	ax, [bp+green_min]
mov	dx, [cycle_color_value]
sub	dx, [bp+red_min]
imul	dx
mov	bx, [bp+red_max]
sub	bx, [bp+red_min]
cwd
idiv	bx
add	ax, [bp+green_min]
mov	[bp+store_green], ax
mov	ax, [bp+blue_max]
sub	ax, [bp+blue_min]
mov	dx, [cycle_color_value]
sub	dx, [bp+red_min]
imul	dx
mov	bx, [bp+red_max]
sub	bx, [bp+red_min]
loc_1CBC1:
cwd
idiv	bx
add	ax, [bp+blue_min]
mov	[bp+store_blue], ax
jmp	loc_1CC4C
@@Do_Green_Calc:
mov	ax, [cycle_color_value]
mov	[bp+store_green], ax
mov	ax, [bp+red_max]
sub	ax, [bp+red_min]
mov	dx, [cycle_color_value]
sub	dx, [bp+green_min]
imul	dx
mov	bx, [bp+green_max]
sub	bx, [bp+green_min]
cwd
idiv	bx
add	ax, [bp+red_min]
mov	[bp+store_red],	ax
mov	ax, [bp+blue_max]
sub	ax, [bp+blue_min]
mov	dx, [cycle_color_value]
sub	dx, [bp+green_min]
imul	dx
mov	bx, [bp+green_max]
sub	bx, [bp+green_min]
jmp	short loc_1CBC1
@@Do_Blue_Calc:
mov	ax, [cycle_color_value]
mov	[bp+store_blue], ax
mov	ax, [bp+green_max]
sub	ax, [bp+green_min]
mov	dx, [cycle_color_value]
sub	dx, [bp+blue_min]
imul	dx
mov	bx, [bp+blue_max]
sub	bx, [bp+blue_min]
cwd
idiv	bx
add	ax, [bp+green_min]
mov	[bp+store_green], ax
mov	ax, [bp+red_max]
sub	ax, [bp+red_min]
mov	dx, [cycle_color_value]
sub	dx, [bp+blue_min]
imul	dx
mov	bx, [bp+blue_max]
sub	bx, [bp+blue_min]
cwd
idiv	bx
add	ax, [bp+red_min]
mov	[bp+store_red],	ax
jmp	short $+2
loc_1CC4C:
mov	ax, [bp+primary_color]
or	ax, ax
jz	short loc_1CC5F
cmp	ax, 1
jz	short loc_1CC70
cmp	ax, 2
jz	short loc_1CC79
jmp	short @@UpdateVgaDacAndDone
loc_1CC5F:
lea	ax, [bp+red_max]
push	ax
lea	ax, [bp+red_min]
loc_1CC66:
push	ax
nop
push	cs
call	near ptr Update_Cycle
pop	cx
pop	cx
jmp	short @@UpdateVgaDacAndDone
loc_1CC70:
lea	ax, [bp+green_max]
push	ax
lea	ax, [bp+green_min]
jmp	short loc_1CC66
loc_1CC79:
lea	ax, [bp+blue_max]
push	ax
lea	ax, [bp+blue_min]
jmp	short loc_1CC66
@@UpdateVgaDacAndDone:
mov	dx, [bp+store_red]
mov	dh, dl
mov	ax, [bp+store_green]
mov	ch, al
mov	ax, [bp+store_blue]
mov	cl, al
mov	bx, [bp+color_num]
mov	ax, 1010h
int	10h
pop	delta_green
pop	delta_red
mov	sp, bp
pop	bp
retf
endp Cycle_Palette_Color__STUB
```
