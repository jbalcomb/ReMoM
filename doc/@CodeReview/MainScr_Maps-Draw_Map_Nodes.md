MainScr_Maps-Draw_Map_Nodes.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr150\Draw_Map_Nodes.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr150\Draw_Map_Nodes.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\LBX_IMG_HorzWarp.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\LBX_IMG_HorzWarp.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\LBX_IMG_VertWarp.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\LBX_IMG_VertWarp.c

Draw_Map_Nodes()
    |-> Shear_Array_Bitmap_X()   (was LBX_IMG_HorzWarp)
    |-> Shear_Array_Bitmap_Y()   (was LBX_IMG_VertWarp)


Warp Mechanics: The functions take an array of signed shift values (Col_Buffer@ and Line_Buffer@).
Pixels shifted out of bounds are zero-filled (cleared to index 0, which typically represents a transparent mask in indexed color palettes).

Shear_Array_Bitmap_Y
Shifts individual columns vertically based on the modifiers in col_buffer.
Out-of-bounds shifted indices are filled with 0 (transparent index).

...a Procedural Displacement Filter—specifically used here to create a shimmering, bubbling "Heat Distortion" or "Chaos" effect.

Shear_Array_Bitmap_X(&TBL_Warp_GFX_Lines[0], Warp_Node_WorkArea);  (Row-wise Horizontal Displacement)
Shear_Array_Bitmap_Y(&TBL_Warp_GFX_Lines[0], Warp_Node_WorkArea);  (Column-wise Vertical Displacement)

---

> **Status: DONE-DONE — behaviourally faithful 1:1, builds clean.** `MSVC-debug` compiles `FLIC_Draw.c` (`MOX.lib`) and `MainScr_Maps.c` (`momlib.lib`) with zero `/WX` warnings, and `HeMoM.exe` links them (2026-07-23). Three OG bugs (**B1**-**B3**) are preserved in `Shear_Array_Bitmap_X` and must not be "fixed". One structural-only item (**S1**) remains open by choice — see below; it does not change any pixel and is not a blocker.

## Source

| Function | Production | Disassembly |
|---|---|---|
| `Draw_Map_Nodes` | [MainScr_Maps.c:2438](MoM/src/MainScr_Maps.c#L2438) | `ovr150` WZD o150p11 |
| `Shear_Array_Bitmap_X` (was `LBX_IMG_HorzWarp`) | [FLIC_Draw.c:813](MoX/src/FLIC_Draw.c#L813) | `seg030` WZD s30p05 |
| `Shear_Array_Bitmap_Y` (was `LBX_IMG_VertWarp`) | [FLIC_Draw.c:696](MoX/src/FLIC_Draw.c#L696) | `seg030` WZD s30p04 |

Sole caller: [MainScr_Maps.c:1957](MoM/src/MainScr_Maps.c#L1957) (`Draw_Map`). The two shear helpers are called only from [MainScr_Maps.c:2545-2546](MoM/src/MainScr_Maps.c#L2545-L2546).

## What it does

`Draw_Map_Nodes` walks all 30 magic nodes. For each node on the requested plane it does two independent passes:

1. **Aura pass** — for each of the node's `power` aura squares, if the square is explored and falls inside the visible map window, draw the owner's coloured aura overlay (`node_auras_seg[banner_id]`) at frame `(node_anim_ctr + tile) % 6`.
2. **Warp pass** — if the node carries `NF_WARPED`, grab the 20x18 square off-screen, run it through the *vanish* dissolve plus the two shear passes, and blit the distorted result back.

The distortion is driven by `TBL_Warp_GFX_Lines` ([MainScr_Maps.c:222](MoM/src/MainScr_Maps.c#L222)) — 20 signed bytes, each clamped to `-1`, `0` or `+1`, random-walked one step per frame. `Shear_Array_Bitmap_X` reads it as one entry **per scanline** and shifts that row left/right; `Shear_Array_Bitmap_Y` reads it as one entry **per column** and shifts that column up/down. Same table, both axes — which is why the warp node shimmers diagonally.

The table is signed (`int8_t[20]`), the shear params are `int8_t *`, and the asm reads each entry with `cbw` — so a `-1` (`0xFF`) sign-extends to `-1`, not `255`. The signed pointer type is load-bearing, not cosmetic.

---

## B1 — right-shift width computed from `height` (preserved OG bug)

[FLIC_Draw.c:875-876](MoX/src/FLIC_Draw.c#L875-L876):

```c
/* OGBUG  uses height instead of width for warped_width calculation */
warped_width = (height - current_shear);
```

Asm 122-125 (`LBX_IMG_HorzWarp`):

```
loc_20C32:
mov     ax, height                  ; di == Image_Height
sub     ax, [bp+Buffer_Shift_Value]
mov     [bp+Warped_Width], ax
```

The negative-shift branch correctly uses `width` (asm 65-67, `mov ax,[bp+width]`); the positive-shift branch uses the `height` register. Original-game behaviour — stays.

## B2 — right-shift loop reads before the buffer (preserved OG bug)

[FLIC_Draw.c:881-888](MoX/src/FLIC_Draw.c#L881-L888):

```c
/* OGBUG  loop condition 'i >= 0' incorrect; should stop at (i >= shear_add); reads up to -18 on bitmap_array */
...
for(i = (total_pixels - height + y); i >= 0; i -= height)
{
    bitmap_array[i] = bitmap_array[i - shear_add];
}
```

Asm 149-151:

```
loc_20C6F:
or      si, si
jge     short loc_20C58
```

The guard really is `si >= 0`, not `si >= LineShift_Size`, so on the final pass `i == y` and the source read is `bitmap_array[y - shear_add]` = `bitmap_array[y - height]`, i.e. up to `height` bytes before `bitmap_array`. Since `bitmap_array = bitmap + SZ_FLIC_HDR` and `SZ_FLIC_HDR` is 16, `y >= 2` reads inside the FLIC header and `y` of 0/1 reads two bytes before the segment base. The byte it *writes* (column 0 of the row) is immediately overwritten by the transparent-fill loop at [FLIC_Draw.c:891-894](MoX/src/FLIC_Draw.c#L891-L894), so there is no visible artefact. Preserved.

Note for flat-memory builds: under DOS this was harmless garbage; here it is a genuine underflow past the allocation base for `y` 0/1 and will trip ASAN/valgrind if the warp-node path is exercised under them. It stays (it is OG) — this is a heads-up, not a fix.

## B3 — `bitmap_array[i]` read with `i` uninitialised (preserved OG bug)

[FLIC_Draw.c:879-880](MoX/src/FLIC_Draw.c#L879-L880):

```c
/* OGBUG  i is undefined here; it contains the last value from the previous block */
niu_var1 = bitmap_array[i];
```

Asm 128-132, entered at `loc_20C32` before `si` is assigned in this branch:

```
les     bx, [bp+Image_Data@]
add     bx, si
mov     al, [es:bx]
cbw
mov     [bp+Unused_Local], ax
```

A dead load into a local (`niu_var1`, the asm's `Unused_Local`) that is never read. Preserved. Production initialises `int16_t i = 0;` at [FLIC_Draw.c:825](MoX/src/FLIC_Draw.c#L825), so the first line taking this branch reads `bitmap_array[0]` rather than the asm's genuine garbage; the value is discarded either way, so nothing observable changes.

---

## S1 — `Draw_Map_Nodes` caches `power`; the asm re-reads it each iteration (open, structural-only)

[MainScr_Maps.c:2476-2477](MoM/src/MainScr_Maps.c#L2476-L2477):

```c
node_power = _NODES[itr_nodes].power;
for(itr = 0; itr < node_power; itr++)
```

Asm 77-79 loads `power` into `UU_node_power` and **never reads it again**; the loop test at 160-168 re-loads it from `_NODES` on every pass:

```
loc_DA038:
mov     ax, [bp+itr_nodes]
mov     dx, 30h
imul    dx
les     bx, [_NODES]
add     bx, ax
mov     al, [es:bx+s_NODE.power]
cbw
cmp     ax, [bp+Tile_Index]
jle     short loc_DA053
jmp     loc_D9F7D
```

Two artefacts against the asm frame:

- `niu_node_power` at [MainScr_Maps.c:2446](MoM/src/MainScr_Maps.c#L2446) is the asm's dead `UU_node_power` slot — declared but never assigned.
- `node_power` at [MainScr_Maps.c:2454](MoM/src/MainScr_Maps.c#L2454) has **no** asm counterpart; it is a phantom cache.

Behaviour is identical — nothing in the loop body writes `_NODES[itr_nodes].power` — so this is structural only, no pixel changes. Left open by choice. The 1:1 form, if it is ever taken:

```c
niu_node_power = _NODES[itr_nodes].power;
for(itr = 0; itr < _NODES[itr_nodes].power; itr++)
```

and drop the phantom `node_power` local.

---

## Verified matching

Checked line-for-line against the asm and found faithful:

- Owner filter: skip when `owner_idx == ST_UNDEFINED` ([MainScr_Maps.c:2466](MoM/src/MainScr_Maps.c#L2466), asm 52-54 `jnz`-to-draw).
- `show_node_owners` filter: skip when the option is off ([MainScr_Maps.c:2470](MoM/src/MainScr_Maps.c#L2470), asm 57-59 `jz`-to-draw).
- Both aura-pass and warp-pass window clips discard `y < 0`, `y >= height`, `x < 0`, `x >= width` ([MainScr_Maps.c:2491-2494](MoM/src/MainScr_Maps.c#L2491-L2494) and [2517-2520](MoM/src/MainScr_Maps.c#L2517-L2520), asm 124-131 / 226-243).
- Warp pass explored-tests the freshly loaded `Aura_Xs[0]`/`Aura_Ys[0]` ([MainScr_Maps.c:2505-2507](MoM/src/MainScr_Maps.c#L2505-L2507), asm 189-211).
- Node loop bounds `itr_nodes < 30` (asm 377); plane filter (asm 37-41).
- Banner lookup `node_auras_seg[_players[owner].banner_id]` — `imul 4C8h` then `shl ax,1` for the word array (asm 62-71).
- Aura frame `(node_anim_ctr + itr) % 6` — `cwd`/`idiv bx` remainder in `dx` (asm 142-147).
- `x -= world_grid_x; if(x < 0) x += WORLD_WIDTH;` wrap in both passes (asm 114-119, 216-221).
- `NF_WARPED` gate. Asm 179-180 is `test ax, NF_WARPED / jg`, equivalent to `!= 0` for a byte flag reached through `cbw`.
- `Screen_Picture_Capture(start_x, start_y, start_x + 19, start_y + 17, Warp_Node_WorkArea)` — push order reversed in asm 273-282.
- `terrain_anim_ctr >= 0` gate on the random walk (asm 284-285, `jl` to skip).
- The `-1`/`+1` clamp pair via `SETMIN`/`SETMAX` and `Random(4) == 1` twice ([MainScr_Maps.c:2533-2536](MoM/src/MainScr_Maps.c#L2533-L2536), asm 292-328). Active macros `SETMIN(a,b): if(b>a) a=b` / `SETMAX(a,b): if(b<a) a=b` ([MOX_DEF.h:95,99](MoX/src/MOX_DEF.h#L95)) expand to `if(v < -1) v=-1` / `if(v > 1) v=1`, matching `loc_DA19B`.
- Seed save/restore around `Vanish_Bitmap` including the `/* OGBUG */` note at [MainScr_Maps.c:2540](MoM/src/MainScr_Maps.c#L2540) (asm 330-348).
- `Shear_Array_Bitmap_X` `niu_var2` — the asm's `Must_Be_Zero`, set to 0 at asm 42, never changed, still added to the index at asm 95 ([FLIC_Draw.c:856](MoX/src/FLIC_Draw.c#L856)).
- `Shear_Array_Bitmap_Y` signed 16-bit locals throughout, matching the asm's signed `jge`/`jl`/`jle`/`jg` conditionals; both `else` clear-column branches guarded by `if(warped_height < 0)` so `warped_height == 0` leaves the column untouched (asm `loc_20AA9`/`loc_20B1A` `jge`-to-do-nothing).
- Both helpers' header reads: `width` at offset 0, `height` at `s_FLIC_HDR.height`, data at `seg + 1` paragraph (`SA_MK_FP0`), matching `SZ_FLIC_HDR`.
- Shear-array params typed `int8_t *` so the `cbw`-signed table read (`-1`/`0`/`+1`) is preserved ([FLIC_Draw.h:213,216](MoX/src/FLIC_Draw.h#L213), [FLIC_Draw.c:696,813](MoX/src/FLIC_Draw.c#L696)).

## Tracker

`Shear_Array_Bitmap_Y` (was `LBX_IMG_VertWarp`) and `Shear_Array_Bitmap_X` (was `LBX_IMG_HorzWarp`) at [stub_wip_todo.md:209-210](doc/@TODO/stub_wip_todo.md#L209-L210); add a `Draw_Map_Nodes` entry.
