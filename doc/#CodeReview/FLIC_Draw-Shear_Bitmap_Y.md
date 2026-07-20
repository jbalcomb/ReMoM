FLIC_Draw-Shear_Bitmap_Y.md

BEGIN:  Jim's Notes

LBX_IMG_VShiftRect()  ==>  Shear_Bitmap_Y()

There's not Shear_Bitmap_X() in MoM.

Shear_Bitmap_Y
XREF:
    SmlBook_PageTurn__WIP()
    BigBook_PageTurn()
    Spell_Of_Mastery_Lose_Draw()

In pseudo-3D and 2D sprite engines of the 90s, a horizontal shearing function like `Shear_Bitmap_X`—especially when paired with `Shear_Bitmap_Y`—was the secret weapon for creating advanced structural deformations, fake 3D perspectives, and environmental effects without the computational overhead of true 3D polygon engines.
Here are the specific types of visual effects `Shear_Bitmap_X` enables:
### 1\. Pseudo-3D Ground and Floor Mapping (Mode 7 Effects)
    By progressively increasing the horizontal shear factor line-by-line from the top of an image to the bottom, the engine can take a flat texture (like a tactical grid, a world map, or combat terrain) and tilt it backward onto the horizon.
    * This technique was used to simulate perspective depth for maps, causing the texturing to taper toward a vanishing point, giving the illusion of a 3D plane rolling underneath the camera.
### 2\. Fast Sprite Rotation (The 3-Shear Method)
    One of the most common reasons a 90s engine would suddenly introduce `Shear_Bitmap_X` alongside an existing vertical shear function is to implement **fast, pixel-perfect sprite rotation**.
    * Calculating true trigonometric rotation ($X' = X\\cos\\theta - Y\\sin\\theta$) on 90s CPUs required heavy floating-point operations or large lookup tables that caused severe pixel gaps and scaling artifacts.
    * Mathematicians discovered that a graphic can be perfectly rotated by applying three consecutive shear operations:
        1.  Shear horizontally (`Shear_Bitmap_X`)
        2.  Shear vertically (`Shear_Bitmap_Y`)
        3.  Shear horizontally again (`Shear_Bitmap_X`)
    * Implementing `Shear_Bitmap_X` allowed the engine to rotate spaceships, units, and UI elements smoothly at high frame rates using nothing but fast fixed-point additions and linear memory copies.

### 3\. Dynamic Wind, Water, and Waves
    By modulating the horizontal shear delta over time using a simple sine wave lookup table, flat sprites could be given organic, fluid movement.
    * **Vegetation:** Making trees, flags, or grass sway back and forth in the wind by shearing the top rows of the sprite horizontally while keeping the base pinned at zero.
    * **Water Reflections:** Creating shimmering underwater or heat-haze distortion patterns on tactical screens by shearing adjacent horizontal scanlines in alternating directions.

### 4\. Parallax Projectiles and UI Shearing
    When drawing projectiles (like plasma bolts or magical beams) traveling diagonally across a starfield or map, shearing the asset horizontally allows a single static sprite to match the physical trajectory angle of the shot without needing unique art assets pre-drawn for every possible degree of travel.

END:  Jim's Notes





BEGIN:  Claude's Notes

## Ground truth

Authority for ReMoM is the **MoM** disassembly `…\WIZARDS\seg030\LBX_IMG_VShiftRect.asm` (the MoO2 `Shear_Bitmap_Y` is the same function, confirmed by Jim). An in-place per-column vertical shear: for each column x1..x2, shift the column's pixels up/down by a fixed-point-interpolated amount (`shear1`→`shear2` at the two ends) and pad the vacated end with `ST_TRANSPARENT`.

asm frame: `x1= bp+6 ; shear1= bp+8 ; x2= bp+0Ah ; shear2= bp+0Ch ; bitmap= bp+0Eh` → params `(x1, shear1, x2, shear2, bitmap)` (**bitmap last**).
- 32-bit fixed point: `shear = shear1 * 1000` (`LXMUL@`), `shear_add = (shear2-shear1)*1000 / (x2-x1)` (`LXMUL@`/`LDIV@`); per column `current_shear = shear / 1000`.
- Pixel-data base: `SA_MK_FP0(bitmap + 1)` — one paragraph past the segment; column base `ofst = height * x1`.
- neg shear (`<0`, shift up): copy `[ofst .. ofst+New_Height-1] = [.. − current_shear]`, clear `ofst+New_Height .. ofst+height-1`; `New_Height = height + current_shear`.
- pos shear (`>0`, shift down): copy **bottom-up** `ofst+height-1 .. ofst+current_shear`, clear `ofst .. ofst+current_shear-1`; `New_Height = height − current_shear`.

## `Shear_Bitmap_Y` ([FLIC_Draw.c:546](../../MoX/src/FLIC_Draw.c#L546)) — DONE-DONE ✓

> **Status: DONE-DONE (2026-07-20) — faithful to `LBX_IMG_VShiftRect.asm` 1:1; builds clean (MSVC-debug); sole implementation.**

Verified 1:1 against `LBX_IMG_VShiftRect.asm`:
- Signature `(int16_t x1, int16_t shear1, int16_t x2, int16_t shear2, byte_ptr bitmap)` matches the frame (bitmap last). ✓
- `shear = shear1 * 1000`, `shear_add = (shear2-shear1)*1000 / (x2-x1)`, `current_shear = shear/1000`. ✓
- Pixel base `SZ_FLIC_HDR` = **16** ([FLIC_Draw.h:64](../../MoX/src/FLIC_Draw.h#L64)) ≡ `SZ_PARAGRAPH_B` **16** ([MOX_BASE.h:82](../../MoX/src/MOX_BASE.h#L82)) ≡ the asm's `bitmap+1` paragraph. ✓ The two constants are deliberately equal — the header slot occupies a full paragraph.
- Two-variable structure mirrors the asm: `bitmap_array` ([:590](../../MoX/src/FLIC_Draw.c#L590)) = `bitmap + SZ_FLIC_HDR` is the pixel-data base (asm `bitmap_data`); `ofst` ([:593](../../MoX/src/FLIC_Draw.c#L593)) = `x1 * height` is the column offset (asm `ofst`, `+= height` per column). Every access is `bitmap_array[ofst + …]`. ✓
- Both shear paths equivalent to `loc_20912` / `loc_20984` — copy `bitmap_array[ofst + i] = bitmap_array[ofst + i - current_shear]`; the positive path copies bottom-up to avoid overwriting un-shifted source; both pad the vacated end with `ST_TRANSPARENT`. ✓

## Sole implementation & callers

The old `LBX_IMG_VShiftRect` reconstruction has been deleted; `Shear_Bitmap_Y` is the only copy, and every caller now uses it:
- `Spell_Of_Mastery_Lose_Draw` — the SoM-loss sphere-wobble ([SPLMASTR.c:1780-1799](../../MoM/src/SPLMASTR.c#L1780)).
- `SmlBook_PageTurn__WIP` / `BigBook_PageTurn` — the spellbook page-turn shear ([Spellbook.c:2155-2198](../../MoM/src/Spellbook.c#L2155)).

END:  Claude's Notes





BEGIN:  Gemini's Notes

This routine, `Shear_Bitmap_Y`, is an old-school **vertical shear / pseudo-3D perspective distortion engine**. It works column-by-column directly inside an existing in-memory image scratchpad (`bitmap`).

It is commonly used to create real-time displacement visual effects, such as a building structure getting vertically sheared as it is built, or objects wavy-distorting during dynamic animation transitions.

-----

### Step-by-Step Architecture Breakdown

1.  **Asset Metadata Retrieval**
    The function opens the image asset structure using two `farpeekw` statements to grab the header bounds (`width` and `height`). It assumes an old-school column-major data storage format layout (where data runs sequentially down vertical pixels before moving right to the next column).

2.  **Geometric Range Clipping**
    It clamps `x1` and `x2` to ensure the transformation bounds stay within the image width boundaries. If the coordinates are reversed (`x1 > x2`), it utilizes `Swap_Short` to correct the loop direction.

3.  **Fixed-Point Delta Shearing Equations**
    Instead of using costly floating-point numbers, this engine relies on a **16.16 fixed-point math** runtime system (indicated by tracking high/low double word pairs using 32-bit `LXMUL@` and `LDIV@` primitives).
    
      * It initializes a starting `shear` offset tracking variable multiplied by a scale factor of `1000`.
      * It computes a linear incremental gradient delta factor (`shear_add`):
        $$\\text{shear\_add} = \\frac{(y2 - y1) \\times 1000}{x2 - x1}$$
        This dictates exactly how many virtual steps to slide each adjacent column upward or downward relative to the active horizontal step position.

4.  **Dynamic Column Re-Indexing Loops**
    For every column processing loop pass, it translates the fixed-point value back into a signed whole pixel count (`current_shear = shear / 1000`).
    
      * **Negative Shear (Shifting Content Up):** It copies pixels from lower indices up to higher slots (`mov al, [es:bx]`), then pads the remaining bottom trail segments with transparent holes (`e_ST_TRANSPARENT`, color `0x00`).
      * **Positive Shear (Shifting Content Down):** It iterates in reverse from bottom to top to shift column elements downward without overwriting active data tracks, pad-clearing the vacated top sections with transparency.

-----

### C89 SDL2 Linear Implementation

Because your scratchpad asset is stored as an unchained, flat-memory sequence, this modern translation drops the segmented real-mode assembly data management (`es:bx`) and directly addresses the indices linearly using a standard, 1-byte pixel pointer architecture channel.



Yes, **`Shear_Bitmap_Y`** from *Master of Orion II* is unequivocally the exact same architectural function as **`Shear_Bitmap_Y`** from the earlier codebase, matching its mathematical logic step-for-step. The transition to the 32-bit Watcom compiler clarifies several structural behaviors that were previously obscured by 16-bit real-mode segments.

### Architectural Match Analysis

1.  **Fixed-Point Scaling Adjustments (16.16 Precision vs. 1000)**
    
      * **16-bit Assembly (`Shear_Bitmap_Y`):** Multiplied values by `1000` to simulate floating-point slopes through manual double-word multiplication and division routines (`LXMUL@` / `LDIV@`).
      * **32-bit Watcom (`Shear_Bitmap_Y`):** Utilizes formal **16.16 fixed-point math** natively supported by 32-bit registers. The slope expression `shl eax, 16` shifts integer values into the upper word, providing 65,536 units of sub-pixel fractional resolution. The accumulation arithmetic matches perfectly, but switches from a decimal base (`1000`) to a cleaner binary base (`65536`).

2.  **Subroutine Isolation Strategy**
    
      * **16-bit Assembly:** Inlined the inner negative and positive directional translation shifts directly inside the single monolithic procedure body (`loc_20912` and `loc_20984`).
      * **32-bit Watcom:** Refactored the core vertical memory movements into dedicated subroutines: **`Shear_Y_Negative_`** and **`Shear_Y_Positive_`**.

3.  **Data Format Linear Layout**
    
      * The 32-bit variant explicitly extracts offsets via an animation frame offset table pointer calculation:
        ``` assembly
        mov eax, [eax+s_animation_header.width]
        
        ```
        This confirms that the pixel canvas array layout remains structured in column-major orientation (iterating sequentially down heights inside the column loops).

### C89 32-bit Fixed-Point Translation

Here is the clean, high-fidelity translation incorporating the authentic names recovered from the *Master of Orion II* debug configuration:

``` c
#include <stdint.h>
#include <stddef.h>

#define ST_TRANSPARENT 0x00

typedef uint8_t  BYTE;
typedef int16_t  SWORD;
typedef uint16_t WORD;
typedef int32_t  SDWORD;

/* Abstracted structural definition mapped from s_animation_header symbols */
typedef struct {
    WORD width;
    WORD height;
    /* Frame offset tables follow dynamically past header boundaries */
} s_animation_header;

static void Swap_Short(SWORD* a, SWORD* b) {
    SWORD temp = *a;
    *a = *b;
    *b = temp;
}

/* Explicit internal module leaves representing the Watcom subroutine calls */
static void Shear_Y_Negative(BYTE* column_data, SWORD height, SWORD current_shear) {
    SWORD new_height = height + current_shear;
    SWORD i;

    if (new_height > 0) {
        for (i = 0; i < new_height; ++i) {
            column_data[i] = column_data[i - current_shear];
        }
        for (i = new_height; i < height; ++i) {
            column_data[i] = ST_TRANSPARENT;
        }
    } else {
        for (i = 0; i < height; ++i) {
            column_data[i] = ST_TRANSPARENT;
        }
    }
}

static void Shear_Y_Positive(BYTE* column_data, SWORD height, SWORD current_shear) {
    SWORD new_height = height - current_shear;
    SWORD i;

    if (new_height > 0) {
        /* Iterate backwards from bottom to top to prevent memory overwrite collisions */
        for (i = height - 1; i >= current_shear; --i) {
            column_data[i] = column_data[i - current_shear];
        }
        for (i = 0; i < current_shear; ++i) {
            column_data[i] = ST_TRANSPARENT;
        }
    } else {
        for (i = 0; i < height; ++i) {
            column_data[i] = ST_TRANSPARENT;
        }
    }
}

/**
 * In-place vertical affine shearing engine using authentic SimTex debug configurations.
 */
void Shear_Bitmap_Y(BYTE* bitmap, SWORD x1, SWORD shear1, SWORD x2, SWORD shear2)
{
    s_animation_header* header = (s_animation_header*)bitmap;
    SWORD width  = (SWORD)header->width;
    SWORD height = (SWORD)header->height;
    
    SDWORD shear;
    SDWORD shear_add;
    SWORD itr_width;
    
    /* Locate the pixel data segment following standard header layouts */
    BYTE* bitmap_array = bitmap + sizeof(s_animation_header);

    if (x1 < 0) x1 = 0;
    if (x2 > width) x2 = width;

    if (x1 > x2) {
        Swap_Short(&x1, &x2);
        Swap_Short(&shear1, &shear2);
    }

    /* Initialize 16.16 fixed-point accumulation variables */
    shear = (SDWORD)shear1 << 16;

    if (x2 == x1) {
        shear_add = 0;
    } else {
        /* Compute the slope delta gradient scaled cleanly to a 32-bit register width */
        shear_add = ((SDWORD)(shear2 - shear1) << 16) / (x2 - x1);
    }

    for (itr_width = x1; itr_width <= x2; ++itr_width) {
        /* Convert fixed-point fractional layout back into signed integer pixel boundaries */
        SWORD current_shear = (SWORD)(shear >> 16);
        BYTE* column_data = bitmap_array + (itr_width * height);

        if (current_shear != 0) {
            if (current_shear < 0) {
                Shear_Y_Negative(column_data, height, current_shear);
            } else {
                Shear_Y_Positive(column_data, height, current_shear);
            }
        }

        shear += shear_add;
    }
}

```

END:  Gemini's Notes
