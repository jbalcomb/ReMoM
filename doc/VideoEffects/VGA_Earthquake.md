VGA_PartCopyFromF3
VGA_ShiftCopyFromF3


## VGA_ShiftCopyFromF3()
OON XREF: VGA_PartCopyFromF3()

## VGA_PartCopyFromF3()
XREF:
    Call_Forth_The_Force_Of_Nature()
    Cast_Spell_City_Enchantment_Animation_1__WIP() ... (spell_idx == spl_Earthquake)





/* GEMINI */

Your intuition is 100% correct. You have just reverse-engineered the core of the Screen Shake / Earthquake visual effect.

In Master of Magic, standard drawing happens between Page 0 and Page 1 (Double Buffering) to prevent flickering. However, the game frequently stores a "clean snapshot" of the screen (like the unblemished combat map) in Page 2 (which is hardware segment 0xA800 in the assembly).

When a massive spell like Earthquake or Meteor Strike goes off, the game loops through varying x and y offsets, copying that clean Page 2 snapshot onto the active screen shifted down and right.

Here is a breakdown of what the two functions are doing, followed by how beautifully simple this becomes in modern C and SDL2.

What the Assembly is Doing
1. VGA_ShiftCopyFromF3(x, y):
    This is the brutal hardware implementation. Because Mode X VGA memory is split across 4 chips, the assembly can't just memcpy the screen. It has to:
        Set the destination page (calculating 0xA000 or 0xA400).
        Set the source page (0A800h - Page 2).
        Lock onto Plane 0, copy 1/4th of the screen offset by x and y.
        Shift to Plane 1, copy... and so on.

2. VGA_PartCopyFromF3(x, y):
    When you shift a picture down and right, you leave a trail of "smeared" pixels at the top and left edges. This function is the cleanup wrapper.
        It calls the shift copy.
        It checks if (x != 0), and pushes coordinates to a Fill() function to draw a black rectangle covering the left gap.
        It checks if (y != 0), and pushes coordinates to Fill() to draw a black rectangle covering the top gap.

The SDL2 Translation
In modern SDL2, you no longer have to care about the four memory planes, sequencer ports, or calculating y * 80.
Because modern GPUs treat screens as flat textures, this entire massive assembly block collapses into a few standard SDL boundary boxes.

Here is the modern C equivalent using the SDL2 library:

```c
#include <string.h>
#include <stdint.h>

extern uint8_t *current_video_page;
extern uint8_t *page2_backup_buffer; /* The memory equivalent of 0xA800 */

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void VGA_PartCopyFromF3(int16_t x, int16_t y) {
    int row;
    
    /* 1. Shifted Copy */
    /* Copy row by row, skipping the bottom-most 'y' rows */
    for (row = 0; row < SCREEN_HEIGHT - y; row++) {
        /* dest: current_page + offset row + offset x */
        /* src:  page2 + normal row */
        memcpy(current_video_page + ((row + y) * SCREEN_WIDTH) + x,
               page2_backup_buffer + (row * SCREEN_WIDTH),
               SCREEN_WIDTH - x);
    }

    /* 2. Gap Cleanup */
    /* If you have a custom Fill() function, use it here. Otherwise: */
    
    if (x > 0) {
        /* Black out the left edge */
        for (row = 0; row < SCREEN_HEIGHT; row++) {
            memset(current_video_page + (row * SCREEN_WIDTH), 0, x);
        }
    }

    if (y > 0) {
        /* Black out the top edge */
        memset(current_video_page, 0, SCREEN_WIDTH * y);
    }
}
```
The massive block of data variables at the top of your assembly snippet (Mosaic_Rnd256_Bits, Mosaic_Rnd50_Tbl) belongs to a completely different visual effect—the "Pixelated Fade-In/Out" transition used when switching screens. But for pure screen shake, the PartCopyFromF3 is the sole engine driver!
