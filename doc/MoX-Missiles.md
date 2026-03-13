

But, also, the drawing...

Must be, the module was named something for MoM and probably MoO1.
Seems the modules with lowercase names are remakes of MoO1/MoM modules, with changes.
e.g.,  MoO2  Module: angle has Get_Angle() Sin() Cos() but not the drawing functions and also the new function Rotate_Coordinates and the tables are different



MoO2
Module: angle
    array (180 bytes) angle_to_sin
    function (0 bytes) Get_Angle
    function (0 bytes) Sin
    function (0 bytes) Cos
    function (0 bytes) Rotate_Coordinates








MoO2
Module: angle
    array (180 bytes) angle_to_sin
    Address: 01:00138405
    Num elements:   90, Type:        unsigned integer (4 bytes) 

    function (0 bytes) Get_Angle
    Address: 01:001384B9
        Num params: 2
        Return type: signed integer (2 bytes) 
        signed integer (4 bytes) 
        signed integer (4 bytes) 
        Locals:
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (2 bytes) result

    function (0 bytes) Sin
    Address: 01:00138615
        Num params: 2
        Return type: signed integer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (4 bytes) 
        Locals:
            signed integer (2 bytes) angle
            signed integer (4 bytes) length
            signed integer (4 bytes) result

    function (0 bytes) Cos
    Address: 01:00138776
        Num params: 2
        Return type: signed integer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (4 bytes) 
        Locals:
            signed integer (2 bytes) angle
            signed integer (4 bytes) length

    function (0 bytes) Rotate_Coordinates
    Address: 01:001387B2
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) angle
            pointer (4 bytes) x
            pointer (4 bytes) y
            signed integer (4 bytes) sin_angle_x
            signed integer (4 bytes) sin_angle_y
            signed integer (4 bytes) cos_angle_x
            signed integer (4 bytes) cos_angle_y
            signed integer (4 bytes) trivial





//Gemini3Pro

This is a spectacular piece of retro graphics programming! You are looking at the core rendering engine for scaled and rotated sprites in VGA Mode X.

Because older CPUs (like the 386/486) were terrible at floating-point math, game developers had to use clever integer tricks to rotate and scale objects like missiles flying across the screen.

This assembly file is broken into two distinct parts. Here is what they are doing and what they look like in C89.

Part 1: Fixed-Point Trigonometry

To calculate the missile's trajectory and rotation, the code uses Look-Up Tables (LUTs) for Sine, Cosine, and Tangent instead of calling standard sin() or cos() functions.The Cos and Sin functions take an angle (from 0 to 359) and a radius. They "fold" the angle into the first quadrant (0-90 degrees) to save memory, look up a multiplier (from $0$ to $65535$, representing $0.0$ to $1.0$), multiply it by the radius, and take the upper 16 bits of the 32-bit result. This is mathematically identical to dividing by 65536!Here is the C89 equivalent for the Cosine function:

```c
/* Assuming angle_to_cos is a global array of 91 unsigned shorts */
extern unsigned short angle_to_cos[91];

int Cos(int angle, int radius) {
    int sign = 1;
    unsigned short multiplier;
    long result;

    /* Fold the 360-degree angle into the 0-90 degree quadrant */
    if (angle > 90 && angle <= 180) {
        angle = 180 - angle;
        sign = -1;
    } else if (angle > 180 && angle <= 270) {
        angle = angle - 180;
        sign = -1;
    } else if (angle > 270 && angle <= 360) {
        angle = 360 - angle;
        /* sign remains positive */
    }

    /* Look up the fixed-point fraction (0 to 65535) */
    multiplier = angle_to_cos[angle];

    /* Multiply and shift right by 16 (equivalent to assembly's "mov ax, dx") */
    result = ((long)radius * multiplier) >> 16;

    if (sign == -1) {
        return (int)(-result);
    }
    return (int)result;
}
```

Part 2: The Mode X Affine Texture Mapper

The VGA_DrawTexture function is an affine texture mapper. It draws the missile column-by-column, stretching or squishing the texture (scaling) and stepping diagonally through the source image data (rotation).

Because it writes to the VGA hardware directly (specifically Mode X or "Unchained" 256-color mode), memory isn't linear. The screen is split into 4 "planes", and you have to tell the VGA card which plane you are writing to using port 3C4h (the Sequence Controller).

Here is the conceptual C89 translation of that highly optimized assembly loop:

```c
#include <dos.h>

/* Global DDA (Digital Differential Analyzer) Error Accumulators */
int Up_Error, Col_Height_Frac, Horz_Left_Error, Horz_Down_Error;

void far VGA_DrawTexture(
    int Start_X, int Start_Y, int Width, int Col1Hgt, int CH_Slope, int CH_Incr, 
    int UpSlope, int NL, int ReadOff, unsigned char far *IMG_Data, 
    int H_RSkip, int X_VSlope, int X_Down, int X_HSlope, int X_Left, 
    int V_RSkip, int Y_VSlope, int Y_Down, int Y_HSlope, int Y_Left, 
    int Skip_Width, int MinOff, int MaxOff) 
{
    unsigned char far *screen_ptr;
    unsigned char far *tex_ptr;
    int col, row;
    unsigned char map_mask;
    
    /* Set up VGA Map Mask register for planar memory */
    outp(0x3C4, 0x02); 
    map_mask = 1 << (Start_X & 3); /* 0, 1, 2, or 3 based on X coordinate */

    /* Calculate screen memory offset: (Y * 80) + (X / 4) */
    /* 80 bytes per row * 4 planes = 320 pixel screen width */
    screen_ptr = MK_FP(current_video_page, (Start_Y * 80) + (Start_X >> 2));
    tex_ptr = IMG_Data + ReadOff;

    for (col = 0; col < Width; col++) {
        /* Set active VGA plane */
        outp(0x3C5, map_mask);

        /* Initialize column error accumulators */
        Up_Error = 0x80;
        Horz_Left_Error = 0x80;
        Horz_Down_Error = 0x80;
        Col_Height_Frac = Col1Hgt;

        if (Skip_Width <= 0) {
            unsigned char far *draw_di = screen_ptr;
            unsigned char far *read_si = tex_ptr;
            int height = (Col_Height_Frac >> 8); /* High byte is integer height */

            /* Inner Drawing Loop (Vertical Slice) */
            for (row = 0; row < height; row++) {
                unsigned char pixel = *read_si;
                
                /* Transparent pixel check & clipping checks */
                if (pixel != 0 && (unsigned int)draw_di >= MinOff && (unsigned int)draw_di <= MaxOff) {
                    *draw_di = pixel;
                }
                
                /* Advance screen pointer by 1 row (80 bytes) */
                draw_di += 80; 

                /* DDA: Advance texture read pointer based on slopes */
                read_si += V_RSkip;
                
                Y_Hrz_Slope += Y_Left;
                if (Y_Hrz_Slope >> 8) { Y_Hrz_Slope &= 0xFF; read_si += Y_Left; }
                
                Y_Vrt_Slope += Y_Down;
                if (Y_Vrt_Slope >> 8) { Y_Vrt_Slope &= 0xFF; read_si += Y_Down; }
            }
        } else {
            Skip_Width--;
        }

        /* Setup for the next column */
        screen_ptr += CH_Incr; /* Vertical step for rotation */
        
        /* DDA for column height scaling */
        Up_Error += UpSlope;
        if (Up_Error >> 8) { Up_Error &= 0xFF; screen_ptr += NL; }
        Col_Height_Frac += CH_Slope;

        /* DDA for horizontal texture stepping */
        tex_ptr += H_RSkip;
        Horz_Left_Error += X_HSlope;
        if (Horz_Left_Error >> 8) { Horz_Left_Error &= 0xFF; tex_ptr += X_Left; }
        
        Horz_Down_Error += X_VSlope;
        if (Horz_Down_Error >> 8) { Horz_Down_Error &= 0xFF; tex_ptr += X_Down; }

        /* Rotate VGA plane map mask (0001 -> 0010 -> 0100 -> 1000) */
        map_mask <<= 1;
        if (map_mask > 8) {
            map_mask = 1;
            screen_ptr++; /* Move to the next physical byte in memory */
        }
    }
}
```

Part 3: The Secret Remap Feature

You included a second function: VGA_DrawTexture_R. It is identical to the first, except for this fascinating block in the inner loop:

```asm
cmp al, 0E8h
jnb short loc_256DD
stosb
...
loc_256DD:
sub al, 0E8h
...
add si, [remap_color_palettes]
...
```

If the texture pixel color is >= 0xE8 (232), it doesn't draw that color. Instead, it subtracts 232 to get an index, reads the color currently on the screen (mov al, [es:di]), and uses both to look up a new color in a remap_color_palettes array.

This is a 90s trick for real-time translucency and shadows! It allows the smoke trail of the missile or the shadow underneath it to blend with the terrain currently drawn on the map.



Just like the Cos function, Sin relies on folding the 360-degree circle into a single 90-degree quadrant to save memory in the look-up table.

However, Sine behaves differently than Cosine across the four quadrants. Sine is positive in Quadrants 1 and 2 (0° to 180°) and negative in Quadrants 3 and 4 (181° to 360°).

There is also one really interesting optimization in this specific function that wasn't in the Cosine one: The 90-Degree Shortcut. If the folded angle is exactly 90, the assembly completely skips the multiplier array and the multiplication logic, and simply returns the Radius directly (applying the appropriate sign).

The C89 Translation

Here is the faithful C89 translation of that Sine logic:

```c
/* Assuming angle_to_sin is a global array of unsigned shorts */
extern unsigned short angle_to_sin[90]; 

int Sin(int angle, int radius) {
    int sign = 1; /* 1 for positive, -1 for negative */
    unsigned short multiplier;
    long result;

    /* 1. Fold the 360-degree angle into the 0-90 degree quadrant */
    if (angle > 90 && angle <= 180) {
        /* Quadrant 2: Sine is positive, angle counts backward from 180 */
        angle = 180 - angle;
    } else if (angle > 180 && angle <= 270) {
        /* Quadrant 3: Sine is negative, angle counts forward from 180 */
        angle = angle - 180;
        sign = -1;
    } else if (angle > 270 && angle <= 360) {
        /* Quadrant 4: Sine is negative, angle counts backward from 360 */
        angle = 360 - angle;
        sign = -1;
    }

    /* 2. The 90-Degree Shortcut */
    /* If the angle is exactly 90 (or 270, which folds to 90), sin(90) = 1.0. 
       1.0 * radius is just radius. The assembly skips the math here. */
    if (angle == 90) {
        if (sign == -1) {
            return -radius;
        }
        return radius;
    }

    /* 3. Look up the fixed-point fraction (0 to 65535) */
    multiplier = angle_to_sin[angle];

    /* 4. Multiply and take the upper 16 bits (divide by 65536) */
    /* Note: In C, standard signed long math handles negative radii natively, 
       whereas the assembly had to manually do abs(radius) to use the 'mul' instruction. */
    result = ((long)radius * multiplier) >> 16;

    /* 5. Apply the quadrant sign */
    if (sign == -1) {
        return (int)(-result);
    }
    
    return (int)result;
}
```

A Quick Note on the Assembly's mul Instruction
If you look closely at the assembly under loc_253F2, you'll notice it goes out of its way to check if the Radius is negative. If it is, it negates it to make it positive, does the multiplication, and then negates the result back to negative.

This is because the programmer used the mul instruction (unsigned multiplication) instead of imul (signed multiplication). If they had multiplied a negative 16-bit number with mul, it would have overflowed and ruined the upper 16 bits they were trying to shift into the AX register. In C89, we don't have to write that specific absolute-value workaround because casting to a long before multiplying natively handles the signed bit math correctly!



This assembly code is an implementation of atan2() (arctangent with two arguments). It calculates the angle of a 2D vector (X, Y) from $0^\circ$ to $359^\circ$.Because the target CPUs lacked floating-point units (FPUs), it achieves this by calculating the tangent ratio as a fixed-point integer—specifically (Y * 256) / X—and looking up the closest angle in pre-calculated tangent arrays.Here is the complete C89 translation, including the full lookup tables perfectly extracted from the assembly's data segment.The C89 Translation

```c
/* Tangent values for angles 0 to 44 degrees.
 * Represents tan(angle) * 256. 
 * Because tan(45) = 1, all values below 45 degrees fit in a single byte (< 256).
 */
const unsigned char Tangents_0_44[45] = {
      0,   4,   8,  13,  17,  22,  26,  31,  35,
     40,  45,  49,  54,  59,  63,  68,  73,  78,
     83,  88,  93,  98, 103, 108, 113, 119, 124,
    130, 136, 141, 147, 153, 159, 166, 172, 179,
    185, 192, 200, 207, 214, 222, 230, 238, 247
};

/* Tangent values for angles 45 to 89 degrees.
 * Represents tan(angle) * 256.
 * Values quickly exceed 255, requiring 16-bit words.
 */
const unsigned short Tangents_45_89[45] = {
      256,   265,   274,   284,   294,   305,
      316,   327,   339,   352,   365,   379,
      394,   409,   426,   443,   461,   481,
      502,   524,   548,   574,   603,   633,
      666,   703,   743,   787,   837,   892,
      955,  1026,  1108,  1204,  1317,  1451,
     1616,  1821,  2085,  2435,  2926,  3661,
     4885,  7331, 14669
};

/* Helper Function: Calculates the angle in Quadrant 1 (0 to 90 degrees) */
int Get_Base_Angle(unsigned int y, unsigned int x) {
    unsigned int ratio;
    int i;

    /* Prevent divide-by-zero: perfectly vertical means 90 degrees */
    if (x == 0) {
        return 90;
    }

    /* Fixed-point math: (Y * 256) / X
     * The assembly achieves this by shifting the AX register left by 8 bits 
     * before dividing by BX.
     */
    ratio = (y << 8) / x;

    /* The assembly optimizes the lookup by splitting the arrays in half 
     * so it never has to scan more than 23 values to find the angle. 
     */
    if (ratio < 256) {
        /* Angle is between 0 and 44 degrees */
        if (ratio < 103) {
            /* Scan 0 to 21 */
            for (i = 0; i <= 21; i++) {
                if (ratio < Tangents_0_44[i]) return i - 1 < 0 ? 0 : i - 1;
            }
            return 21;
        } else {
            /* Scan 22 to 44 */
            for (i = 22; i <= 44; i++) {
                if (ratio < Tangents_0_44[i]) return i - 1;
            }
            return 44;
        }
    } else {
        /* Angle is between 45 and 89 degrees */
        if (ratio < 603) {
            /* Scan 45 to 66 */
            for (i = 0; i <= 21; i++) {
                if (ratio < Tangents_45_89[i]) return i + 45 - 1;
            }
            return 66; /* 45 + 21 */
        } else {
            /* Scan 67 to 89 */
            for (i = 22; i <= 44; i++) {
                if (ratio < Tangents_45_89[i]) return i + 45 - 1;
            }
            return 89;
        }
    }
}

/* Main Function: Returns a full 0-359 degree angle based on X and Y distances */
int Get_Angle(int x_dist, int y_dist) {
    int angle;

    if (x_dist >= 0) {
        if (y_dist >= 0) {
            /* Quadrant 1 (Bottom-Right, assuming Y goes down on screen) */
            angle = Get_Base_Angle(y_dist, x_dist);
        } else {
            /* Quadrant 4 (Top-Right) */
            angle = Get_Base_Angle(-y_dist, x_dist);
            angle = 360 - angle;
        }
    } else {
        if (y_dist >= 0) {
            /* Quadrant 2 (Bottom-Left) */
            angle = Get_Base_Angle(y_dist, -x_dist);
            angle = 180 - angle;
        } else {
            /* Quadrant 3 (Top-Left) */
            angle = Get_Base_Angle(-y_dist, -x_dist);
            angle += 180;
        }
    }

    return angle;
}
```

A Brilliant Fixed-Point Optimization
The most elegant part of this code is how it splits the Tangents array.
By definition, the tangent of $45^\circ$ is 1. Because they scale the ratio by 256 (ratio = y * 256 / x), any angle less than $45^\circ$ is guaranteed to produce a ratio less than 256.
Instead of making one massive 16-bit array for all 90 degrees, the original programmer realized:
  1. Angles $0^\circ$ to $44^\circ$ fit perfectly into an 8-bit char array (max value is 247).
  2. Only angles $45^\circ$ to $89^\circ$ need a 16-bit short array.
This saved 45 bytes of memory—which doesn't sound like much today, but in an era of strict 640KB DOS limits, every byte counted. Furthermore, in Get_Base_Angle, the assembly checks if bh == 0 (meaning the ratio is $< 256$) to instantly decide which array to check, effectively acting as a blazing-fast, branch-predicted binary search!



In the era of DOS and 16-bit game development, these tables were almost never calculated by the game itself while it was running. Because the whole point was to avoid slow floating-point math on the player's CPU, making the player's computer calculate sines and cosines at startup would defeat the purpose (and take a noticeable amount of time on a 386 processor).

Instead, developers used pre-computation. They wrote a separate, tiny "tool" program on their powerful development workstations. This tool used standard, heavy floating-point math to calculate all the values, and then literally generated a .c or .asm text file containing the arrays. The developer would then just include that generated file into the main game's source code and compile it.

Here is exactly how that generator program would look in C.

The Lookup Table Generator
To generate these tables, the tool uses standard <math.h> functions, converts the degrees to radians, and scales the floating-point results up into integers.

#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

int main() {
    int i;
    double rad, val;

    printf("/* --- GENERATED SINE MULTIPLIERS --- */\n");
    printf("const unsigned short Sin_Multipliers[91] = {\n    ");
    for (i = 0; i <= 90; i++) {
        rad = i * (PI / 180.0);
        
        /* Scale by 65536 (Shift left 16 bits) */
        /* Note: sin(30) is 0.5. 0.5 * 65536 = 32768 (0x8000), which matches the asm! */
        val = sin(rad) * 65536.0; 
        
        /* Cap at 65535 to fit in an unsigned short to prevent overflow at 90 degrees */
        if (val > 65535.0) val = 65535.0; 
        
        printf("0x%04X, ", (unsigned short)val);
        if ((i + 1) % 8 == 0) printf("\n    ");
    }

    printf("\n\n/* --- GENERATED TANGENTS (0 to 44) --- */\n");
    printf("const unsigned char Tangents_0_44[45] = {\n    ");
    for (i = 0; i <= 44; i++) {
        rad = i * (PI / 180.0);
        
        /* Scale by 256 (Shift left 8 bits) */
        /* tan(45) = 1.0, so everything below 45 fits under 256 */
        val = tan(rad) * 256.0;
        
        printf("%3d, ", (unsigned char)val);
        if ((i + 1) % 9 == 0) printf("\n    ");
    }

    printf("\n\n/* --- GENERATED TANGENTS (45 to 89) --- */\n");
    printf("const unsigned short Tangents_45_89[45] = {\n    ");
    for (i = 45; i <= 89; i++) {
        rad = i * (PI / 180.0);
        
        /* Scale by 256 (Shift left 8 bits) */
        val = tan(rad) * 256.0;
        
        printf("%5d, ", (unsigned short)val);
        if ((i - 44 + 1) % 6 == 0) printf("\n    ");
    }
    printf("\n};\n");

    return 0;
}

The Math Behind the Magic
By looking at the scale factors in this generator, we can see the exact fixed-point formats the developers chose:

Sine / Cosine (Scale factor of 65536): This represents a 0.16 fixed-point number. There are 0 integer bits and 16 fractional bits. When the game multiplies a radius by this 16-bit number, the result becomes a 32-bit number. The game then takes the top 16 bits (by shifting right by 16, or just grabbing the DX register in assembly), which seamlessly drops the fractional part and leaves the perfectly scaled integer!

Tangents (Scale factor of 256): This represents an 8.8 fixed-point number (8 integer bits, 8 fractional bits). They used this because checking the slope of a line is just (Y * 256) / X. By pre-multiplying the tangent table by 256, the engine can compare the game's division result directly against the table without any further conversion.

In modern game engines, you don't need to do this because a modern CPU's FPU can calculate a true sinf() in a few clock cycles. But back then, doing this math ahead of time and pasting the results into the code was the only way to get a game to run at 30 or 60 frames per second.
