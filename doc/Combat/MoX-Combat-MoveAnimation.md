

# Combat Figure Movement Animation


## Overview

When a unit moves from one combat grid square to another, each figure in the unit slides smoothly between the two squares over a series of animation frames.  This document covers how the animation works, why it works that way, and how the original MS-DOS timing was adapted for SDL2.


## The Animation Cycle

### Step 1: Set Up the Move

The movement path is pre-computed as a list of grid coordinates stored in `_cmbt_mvpth_x[]` / `_cmbt_mvpth_y[]`.  The outer loop walks through each step in the path:

```c
for(Move_Step_Index = 0; Move_Step_Index < movement_path_grid_cell_count; Move_Step_Index++)
{
    battle_units[battle_unit_idx].target_cgx = _cmbt_mvpth_x[Move_Step_Index];
    battle_units[battle_unit_idx].target_cgy = _cmbt_mvpth_y[Move_Step_Index];
    ...
```

For each path step the unit's current grid position (`cgx`, `cgy`) stays at the square it is leaving.  The target position (`target_cgx`, `target_cgy`) is the square it is entering.

### Step 2: Animate the Slide (the Inner Loop)

The inner loop runs `MOVE_ANIM_CNT` (8) iterations.  Each iteration increments `move_anim_ctr`, redraws the entire combat screen, and flips the video page:

```c
// Original (MS-DOS)
for(itr = 0; itr < MOVE_ANIM_CNT; itr++)
{
    battle_units[battle_unit_idx].move_anim_ctr += 1;
    Combat_Screen_Draw();
    PageFlip_FX();
}
```

`move_anim_ctr` counts from 0 to 7.  When it reaches `MOVE_ANIM_MAX` (7) the figure is fully at the target square.

### Step 3: Draw with Interpolation

`Combat_Screen_Draw()` iterates every battle unit and calls `CMB_SpawnFigure__WIP()` for each figure in the unit.  The `move_anim_ctr` value is passed as the `MoveStage` parameter.

Inside `CMB_SpawnFigure__WIP()`, the pixel position is linearly interpolated:

```c
draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / MOVE_ANIM_CNT) + position_screen_x) + fig_x);
draw_y = (((((target_screen_y - position_screen_y) * MoveStage) / MOVE_ANIM_CNT) + position_screen_y) + fig_y);
```

When `MoveStage` is 0 the figure draws at the origin square.  When `MoveStage` is 7 (i.e., `MOVE_ANIM_CNT - 1`) it draws 7/8 of the way to the target.  After the loop finishes, `move_anim_ctr` is reset to 0 and the unit's `cgx`/`cgy` is snapped to the target position.

### Step 4: Finalize the Step

After the inner animation loop completes:
- `move_anim_ctr` is reset to 0
- `cgx`/`cgy` is updated to the target position
- Wall of Fire damage is checked
- If the unit died, the path loop breaks early

### AI Movement

The AI path (Auto_Move_Unit / o114p09) uses the same pattern but the loop increment uses `move_anim_speed` instead of 1:

```c
for(itr_battle_units = 0; itr_battle_units < MOVE_ANIM_CNT; itr_battle_units += move_anim_speed)
```

This makes AI units move faster (fewer visible frames) because `move_anim_speed` can be 2 or more, causing the interpolation to skip intermediate positions.

### Animations Disabled

If `magic_set.movement_animations == ST_FALSE`, the animation loop is skipped entirely.  `move_anim_ctr` is set to `MOVE_ANIM_MAX`, a single draw+flip is done, and the unit teleports to the next square.


## Data Flow Summary

```
battle_units[idx].move_anim_ctr   (0..7, set by the movement loop)
        |
        v
Combat_Screen_Draw()
        |
        v
CMB_SpawnFigure__WIP(..., move_anim_ctr as MoveStage, ...)
        |
        v
draw_x = (((target_screen_x - position_screen_x) * MoveStage) / MOVE_ANIM_CNT) + position_screen_x + fig_x
draw_y = (((target_screen_y - position_screen_y) * MoveStage) / MOVE_ANIM_CNT) + position_screen_y + fig_y
```

`MOVE_ANIM_CNT = 8`  (CMBTDEF.h)
`MOVE_ANIM_MAX = 7`  (CMBTDEF.h)


## Timing: MS-DOS vs. SDL2


### How It Worked on MS-DOS (IBM-PC, VGA)

On the original hardware there was no explicit timing code in the movement animation loop.  The loop just drew and flipped as fast as it could:

```c
for(itr = 0; itr < MOVE_ANIM_CNT; itr++)
{
    battle_units[battle_unit_idx].move_anim_ctr += 1;
    Combat_Screen_Draw();
    PageFlip_FX();
}
```

The pacing came for free from the hardware:

1. **VGA vertical retrace sync** -- `Page_Flip()` waited for the VGA vertical retrace before swapping the displayed page.  On a standard VGA monitor this happened at ~70 Hz, meaning each flip took ~14 ms.  This was the natural frame limiter.

2. **CPU speed** -- A 386/486-class CPU running at 25-66 MHz took real time to redraw the combat screen (software rendering into a 320x200 framebuffer, blitting sprites, etc.).  The draw itself consumed a significant fraction of the frame budget.

3. **BIOS timer tick** -- The IBM-PC BIOS timer (INT 1Ah) ticked at ~18.2 Hz (~55 ms per tick).  This timer was used elsewhere in the game (fade effects, delays via `Mark_Time`/`Release_Time`) but was NOT used inside the movement animation loop.  The VGA retrace was the implicit clock.

The result: 8 frames x ~14 ms = ~112 ms per grid square.  Fast, smooth, and no timing code needed because the hardware was the bottleneck.


### The Problem with SDL2 (Modern Hardware)

On modern hardware, none of those natural limiters exist:

1. **No VGA retrace wait** -- `Page_Flip()` in the SDL2 port calls `Platform_Video_Update()` which calls `SDL_RenderPresent()`.  Without vsync, this returns almost instantly.  Even with vsync enabled, modern displays run at 60-360+ Hz.

2. **CPU is thousands of times faster** -- `Combat_Screen_Draw()` completes in microseconds instead of milliseconds.  The entire 8-iteration loop can finish in well under 1 ms.

3. **Result** -- The unit teleports instantly from origin to destination.  The player sees no animation at all, just a unit jumping to its new position.


### The Naive Fix (And Why It Failed)

An early attempt tried to stretch the loop from 8 iterations to 80, using integer division to map the loop counter back to the 0-7 range:

```c
for(itr = 0; itr < (MOVE_ANIM_CNT * 10); itr++)
{
    battle_units[battle_unit_idx].move_anim_ctr = ((itr + (10 - 1)) / 10);
    Combat_Screen_Draw();
    PageFlip_FX();
}
```

This failed in two ways:

1. **Still too fast** -- 80 iterations of draw+flip still completes in a few milliseconds on modern hardware.  Adding more iterations does not solve the problem because the CPU is simply too fast.

2. **Staircase effect** -- The integer division `((itr + 9) / 10)` produces a staircase pattern where `move_anim_ctr` holds the same value for 10 frames, then jumps.  The unit appears to stutter: move-pause-move-pause.  This is because the draw interpolation only has 8 discrete pixel positions (0/8 through 7/8 of the distance), and showing the same position for 10 rapid frames followed by a jump to the next position creates visible hitching.

An even earlier bug had `move_anim_ctr = (((itr + 9) / 10) * itr)` which caused the counter to grow quadratically to values like 632, sending the draw position wildly off-screen.


### The SDL2 Fix: Mark_Time / Release_Time

The working solution uses the game's existing frame-pacing system, the same one used by `Fade_Out()` and `Fade_In()` in MOX_T4.c.  The loop is kept at the original 8 iterations, and each frame is paced with `Mark_Time()` / `Release_Time()`:

```c
for(itr = 0; itr < MOVE_ANIM_CNT; itr++)
{
    Mark_Time();
    battle_units[battle_unit_idx].move_anim_ctr = itr;
    Combat_Screen_Draw();
    PageFlip_FX();
    Release_Time(1);
}
```

**How it works:**

- `Mark_Time()` captures the current time via `SDL_GetTicks64()`.
- The draw and flip happen as normal.
- `Release_Time(1)` busy-waits (while pumping SDL events and updating the mouse cursor) until `MILLISECONDS_PER_FRAME` (55 ms) has elapsed since `Mark_Time()` was called.

```
Mark_Time / Release_Time internals (sdl2_Timer.c):

    Mark_Time()
        sdl2_ticks_mark_time = SDL_GetTicks64();

    Release_Time(ticks)
        sdl2_tick_end = sdl2_ticks_mark_time + (ticks * MILLISECONDS_PER_FRAME);
        while(Platform_Get_Millies() < sdl2_tick_end)
        {
            SDL_PumpEvents();
            Platform_Maybe_Move_Mouse();
            Platform_Sleep_Millies(1);
        }

    MILLISECONDS_PER_FRAME = 55   (sdl2_PFL.h)
```

This produces 8 frames x 55 ms = 440 ms per grid square.  It is slower than the original ~112 ms but looks deliberate and smooth.  The timing constant can be tuned later if needed.


### Why This Approach

Several alternatives were considered:

| Approach | Problem |
|---|---|
| More loop iterations | CPU is too fast; doesn't add real time, just burns cycles |
| SDL_Delay() in the loop | Blocks the thread; mouse cursor freezes; SDL event queue stalls |
| Vsync-only | Varies by monitor (60-360 Hz); not portable; would need different loop counts for different refresh rates |
| Sub-pixel interpolation | Would require float math and changes to the draw pipeline; large scope change for a reconstruction project |
| Mark_Time / Release_Time | Already exists in the codebase; processes events during the wait; mouse stays responsive; minimal code change |

The `Mark_Time` / `Release_Time` approach wins because:
1. **It already exists** -- The pattern is used by `Fade_Out()`, `Fade_In()`, and ~110 other call sites.
2. **It keeps the original loop structure** -- 8 iterations, `move_anim_ctr` increments by 1, same interpolation math.  The original game code is preserved.
3. **It processes events during the wait** -- `Release_Time()` calls `SDL_PumpEvents()` and `Platform_Maybe_Move_Mouse()` in its wait loop, so the mouse cursor stays responsive and the window doesn't become unresponsive.
4. **Minimal change** -- Two lines added (`Mark_Time()` and `Release_Time(1)`), nothing else changes.


## Constants

| Name | Value | Defined In | Purpose |
|---|---|---|---|
| `MOVE_ANIM_CNT` | 8 | CMBTDEF.h | Number of animation frames per grid-square move |
| `MOVE_ANIM_MAX` | 7 | CMBTDEF.h | Maximum value of `move_anim_ctr` (CNT - 1) |
| `MILLISECONDS_PER_FRAME` | 55 | sdl2_PFL.h | IBM-PC BIOS timer tick period (~18.2 Hz) |
| `FRAMES_PER_SECOND` | ~18 | sdl2_PFL.h | 1000 / 55, matches BIOS timer tick rate |


## Key Files

| File | What |
|---|---|
| MoM/src/Combat.c ~3362 | Human player movement animation loop (BU_Move / s91p04) |
| MoM/src/Combat.c ~18603 | AI movement animation loop (Auto_Move_Unit / o114p09) |
| MoM/src/Combat.c ~27268 | Draw interpolation in CMB_SpawnFigure__WIP() |
| MoM/src/CMBTDEF.h | MOVE_ANIM_CNT, MOVE_ANIM_MAX defines |
| MoX/src/MOX_T4.c ~267 | Fade_Out() -- reference for Mark_Time/Release_Time pattern |
| MoX/src/sdl2_Timer.c | Mark_Time(), Release_Time() implementations |
| MoX/src/sdl2_PFL.h | MILLISECONDS_PER_FRAME constant |
| MoX/src/Video.c ~285 | Page_Flip() -- the VGA page swap |


## Future Considerations

- **Timing constant tuning** -- 55 ms per frame (440 ms per square) may feel sluggish.  A dedicated combat animation timing constant could be defined separately from the BIOS timer tick period.  The original was closer to ~14 ms per frame (~112 ms per square).
- **Sub-frame interpolation** -- If smoother animation is desired at high refresh rates, the draw code could interpolate between integer `move_anim_ctr` values using a fractional time parameter.  This would require changes to `CMB_SpawnFigure__WIP()` and is a larger scope change.
- **AI move_anim_speed** -- The AI loop steps by `move_anim_speed` (typically 2), so it completes in 4 visible frames (220 ms at 55ms/frame).  This preserves the original behavior where AI units visibly move faster than human units.
