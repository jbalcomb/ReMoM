Combat-Move_Battle_Unit.md

BU_Teleport()             ==>  Battle_Unit_Teleport()
BU_TunnelTo()             ==>  Battle_Unit_Tunnel()
BU_CreateImage__SEGRAX()  ==>  Battle_Unit_Compose_Bitmap()
BU_SetBaseAnims__WIP()    ==>  Battle_Unit_Set_Animation_Flags()
BU_CheckFlight__WIP()     ==>  Battle_Unit_Is_Airborne()
BU_SetVisibility__WIP()   ==>  Battle_Unit_Set_Invisibility_Effect()

battle_units[].Image_Effect  ==>  battle_units[].figure_effect
battle_units[].Always_Animate ==> battle_units[].animate_idle
battle_units[].Move_Bob      ==>  battle_units[].animate_move_as_idle
battle_units[].Moving        ==>  battle_units[].
battle_units[].Cur_Figures   ==>  battle_units[].figure_cnt
battle_units[].Max_Figures   ==>  battle_units[].figure_max

IMG_CMB_FX_Figure     ==>  battle_unit_scratch_seg
CMB_BaseAnimFrame     ==>  combat_idle_anim_frame
CMB_MoveAnimFrame     ==>  combat_walk_anim_frame
CMB_MoveAnimDir       ==>  combat_walk_anim_dir
SND_CMB_Silence       ==>  sound_silent_seg
SND_CMB_Silence_size  ==>  sound_silent_seg_size
GfxBuf_2400B          ==>  scratch_bitmap_seg

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Move_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Get_Battle_Unit_Move_Sound_Buffer.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Check_Wall_Of_Fire_Attack.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\BU_Teleport.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\BU_TunnelTo.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\BU_CreateImage__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\BU_SetBaseAnims__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\BU_CheckFlight__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\BU_SetVisibility__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\Copy_Bitmap_To_Bitmap.asm

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Move_Battle_Unit()
            |-> Set_Movement_Cost_Map()
            |-> Update_Move_Map_City_Area_Restrictions()
            |-> Battle_Unit_Instant_Movement_Mode()
            |-> Combat_Move_Path_Find()
            |-> Get_Battle_Unit_Move_Sound_Buffer()
            |-> Battle_Unit_Teleport()
            |-> Battle_Unit_Tunnel()
            |-> Play_Sound()
            |-> Mark_Time()
            |-> Combat_Screen_Draw()
            |-> PageFlip_FX()
            |-> Release_Time()
            |-> Combat_Screen_Draw()
            |-> Check_Wall_Of_Fire_Attack()

Move_Battle_Unit()
    |-> Battle_Unit_Teleport()
        |-> Battle_Unit_Compose_Bitmap()
            |-> Battle_Unit_Set_Animation_Flags()
                |-> Battle_Unit_Is_Airborne()
            |-> Battle_Unit_Set_Invisibility_Effect()
Move_Battle_Unit()
    |-> Battle_Unit_Tunnel()
        |-> Battle_Unit_Compose_Bitmap()
            |-> Battle_Unit_Set_Animation_Flags()
                |-> Battle_Unit_Is_Airborne()
            |-> Battle_Unit_Set_Invisibility_Effect()

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open; two marked divergences (D1, D2). All nine in-scope functions compared against their listings (2026-08-03).**

## Scope

| Function | Production | ASM (ground truth) |
| --- | --- | --- |
| `Move_Battle_Unit` | [Combat.c:3067-3226](../../MoM/src/Combat.c#L3067-L3226) | `ovr091/Move_Battle_Unit.asm` |
| `Get_Battle_Unit_Move_Sound_Buffer` | [Combat.c:6098-6114](../../MoM/src/Combat.c#L6098-L6114) | `ovr098/Get_Battle_Unit_Move_Sound_Buffer.asm` |
| `Check_Wall_Of_Fire_Attack` | [Combat.c:21339-21365](../../MoM/src/Combat.c#L21339-L21365) | `ovr124/Check_Wall_Of_Fire_Attack.asm` |
| `Battle_Unit_Teleport` | [Spells133.c:959-1045](../../MoM/src/Spells133.c#L959-L1045) | `ovr133/BU_Teleport.asm` |
| `Battle_Unit_Tunnel` | [Spells133.c:1049-1141](../../MoM/src/Spells133.c#L1049-L1141) | `ovr133/BU_TunnelTo.asm` |
| `Battle_Unit_Compose_Bitmap` | [Combat.c:8077-8158](../../MoM/src/Combat.c#L8077-L8158) | `ovr103/BU_CreateImage__SEGRAX.asm` |
| `Battle_Unit_Set_Animation_Flags` | [Combat.c:9411-9432](../../MoM/src/Combat.c#L9411-L9432) | `ovr105/BU_SetBaseAnims__WIP.asm` |
| `Battle_Unit_Is_Airborne` | [Combat.c:5938-5955](../../MoM/src/Combat.c#L5938-L5955) | `ovr098/BU_CheckFlight__WIP.asm` |
| `Battle_Unit_Set_Invisibility_Effect` | [Combat.c:9286-9355](../../MoM/src/Combat.c#L9286-L9355) | `ovr105/BU_SetVisibility__WIP.asm` |

**Deliberately out of scope:** `Apply_Battle_Unit_Damage_From_Spell` and `BU_ApplyDamage`, called from `Check_Wall_Of_Fire_Attack` — held for the attack-functions review.

**Already reviewed elsewhere:** `Set_Movement_Cost_Map`, `Update_Move_Map_City_Area_Restrictions`, `Battle_Unit_Instant_Movement_Mode` in [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md); `Combat_Move_Path_Find` in [Combat-Combat_Move_Path_Find.md](Combat-Combat_Move_Path_Find.md).

**Engine/platform, not compared:** `Play_Sound`, `Mark_Time`, `Release_Time`, `Combat_Screen_Draw`, `PageFlip_FX`, `Create_Picture`, `Vanish_Bitmap`, `Set_Window`, `Draw_Picture_Windowed`. `Copy_Bitmap_To_Bitmap` is engine code, but `seg030/Copy_Bitmap_To_Bitmap.asm` was read to establish its parameter order, since the FX callers depend on it.

**Macro-hidden, not compared:** `BATTLE_UNIT_FACING_DRECTION`, `FIGUREX_MAP`, `FIGUREX_OFFSET`, `FIGUREX_POINTER` in `Battle_Unit_Compose_Bitmap` ([Combat.c:8142-8145](../../MoM/src/Combat.c#L8142-L8145)) stand in for `asm:187-266` — the facing decision tree and the EMS figure-set pointer setup. Their expansions were not read against the listing.

`asm:N` refers to the listing named in the row above.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared, and an emitted comparison is not treated as evidence of the expression that produced it.

---

# Divergences

## D1 — reconstruction conventions: pacing calls and size parameters

Deliberate additions, tagged in the source, recorded so they aren't mistaken for reconstruction errors later.

**Frame pacing.** The listing's animation body is two calls:

```
asm:248  call    j_Combat_Screen_Draw
asm:249  call    PageFlip_FX
```

Production ([3159](../../MoM/src/Combat.c#L3159), [3163](../../MoM/src/Combat.c#L3163)) wraps them in `Mark_Time()` / `Release_Time(1)`, both tagged `/* HACK  pacing */`. The OG ran at VGA retrace speed with no explicit delay.

**Sound-size parameters.** Every `Play_Sound` in these listings takes one argument and pops one `cx` (`Move_Battle_Unit.asm:224-225`, `asm:267-268`; `BU_Teleport.asm:84-86`, `asm:95-97`; `BU_TunnelTo.asm:76-78`, `asm:87-89`). Production passes a size argument at all of them — [3153](../../MoM/src/Combat.c#L3153), [3174](../../MoM/src/Combat.c#L3174), [6104](../../MoM/src/Combat.c#L6104), [Spells133.c:996](../../MoM/src/Spells133.c#L996), [1000](../../MoM/src/Spells133.c#L1000), [1075](../../MoM/src/Spells133.c#L1075), [1079](../../MoM/src/Spells133.c#L1079). `Get_Battle_Unit_Move_Sound_Buffer` likewise carries an extra `/* HACK */ uint32_t * sound_buffer_size` out-parameter ([6098](../../MoM/src/Combat.c#L6098)) against the listing's one-argument form. The supporting locals and globals are tagged `// DNE in Dasm` — [3081](../../MoM/src/Combat.c#L3081), [Combat.c:680](../../MoM/src/Combat.c#L680).

**Debug asserts.** The four `assert()` calls at [3184-3187](../../MoM/src/Combat.c#L3184-L3187) have no counterpart.

**Void vs. int return.** The early bail sets a return value (`xor ax, ax`, `Move_Battle_Unit.asm:147`) where production is `void` — the same pattern recorded for `Combat_Move_Path_Find`.

## D2 — a dead argument the reconstruction cannot express

`BU_SetBaseAnims__WIP.asm:23-28` pushes two arguments and pops two:

```
xor     ax, ax
push    ax
push    _SI_battle_unit_idx
call    j_BU_CheckFlight__WIP
pop     cx
pop     cx
```

The callee's frame declares only `battle_unit_idx = word ptr 6` and never reads `bp+8` — the second argument is dead on arrival, the signature of a call made without a prototype in scope. Production calls with one argument ([9420](../../MoM/src/Combat.c#L9420)).

Reproducing the listing would mean giving `Battle_Unit_Is_Airborne` a parameter it must not read, which is worse than the divergence. Left as-is; this closes the `// TODO  is that 0 really there?` question that stood at that call site.

---

# Move_Battle_Unit

## Frame map

`sub sp, 16h` (asm:21) = 22 bytes, plus two register locals.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-16h` | `sound_buffer` | `move_sound_seg` |
| `bp-14h` | `has_teleport` | `has_instant_movement_mode` |
| `bp-12h` | `Teleport_Type` | `instant_movement_mode` |
| `bp-10h` | `Last_Facing_Y` | `Last_Facing_Y` |
| `bp-0Eh` | `Last_Facing_X` | `Last_Facing_X` |
| `bp-0Ch` | `Origin_Y` | `Origin_Y` |
| `bp-0Ah` | `Origin_X` | `Origin_X` |
| `bp-7` (**byte**) | `battle_unit_owner_idx` | `battle_unit_owner_idx` (`int16_t`) |
| `bp-6` | `Facing_Diff_Y` | `Facing_Diff_Y` |
| `bp-4` | `Facing_Diff_X` | `Facing_Diff_X` |
| `bp-2` | `Move_Step_Index` | `Move_Step_Index` |
| `di` | `_DI_itr` | `itr` |
| `si` | `_SI_battle_unit_idx` | (the parameter, used directly) |

`battle_unit_owner_idx` is written once (asm:30-31, production [3082](../../MoM/src/Combat.c#L3082)) and never read — a faithful write-only local.

## Faithful — verified, leave alone

- **Cost-map seeding is the same sequence as `Assign_Combat_Grids`** — `Set_Movement_Cost_Map` (asm:32-35) -> active-unit `INF` loop (asm:36-69) -> vortex `INF` loop (asm:70-95) -> central structure (asm:96-100) -> walled/flying-fortress gate (asm:101-111). Production [3083](../../MoM/src/Combat.c#L3083), [3084](../../MoM/src/Combat.c#L3084), [3091](../../MoM/src/Combat.c#L3091), [3096](../../MoM/src/Combat.c#L3096), [3100](../../MoM/src/Combat.c#L3100). The gate uses `jle` on `FLYING_FORTRESS` (asm:107), matching `> 0`.
- **Instant-move detection short-circuits the pathfinder** — mode 6 or 7 sets `movement_path_grid_cell_count = 0` and the flag, and skips `Combat_Move_Path_Find` entirely (asm:113-127). Otherwise the path is found and a zero-length result returns immediately (asm:128-148). Production [3108-3126](../../MoM/src/Combat.c#L3108-L3126).
- **`Combat_Move_Path_Find(cgx, cgy, target_cgx, target_cgy)` argument order** — asm:129-143 pushes target_cgy, target_cgx, cgy, cgx, so cdecl right-to-left gives source first. Production [3121](../../MoM/src/Combat.c#L3121).
- **Pre-move state** — `Origin_X`/`Origin_Y` from the unit's current cell (asm:149-163), `uu_combat_movement_variable = 0` (asm:164), `Moving = ST_TRUE` (asm:165-170), then the sound buffer (asm:171-174). Production [3127-3131](../../MoM/src/Combat.c#L3127-L3131).
- **Teleport dispatch** — mode 6 -> `Battle_Unit_Teleport`, otherwise `Battle_Unit_Tunnel`, both `(battle_unit_idx, target_cgx, target_cgy)` (asm:323-337). Production [3132-3142](../../MoM/src/Combat.c#L3132-L3142).
- **Walk loop** — `target_cgx`/`target_cgy` seeded from the parameters before the loop (asm:178-192), then per step from `_cmbt_mvpth_x`/`_y` with `shl ax, 1` word indexing (asm:195-221). Production [3145-3150](../../MoM/src/Combat.c#L3145-L3150). The word scaling matches the `int16_t` element type established in [Combat-Combat_Move_Path_Find.md](Combat-Combat_Move_Path_Find.md).
- **Sound gating** — `magic_set.sound_effects == ST_TRUE` before the move sound (asm:222-226) and again before the silence buffer after the animation (asm:265-269). Production [3151](../../MoM/src/Combat.c#L3151), [3172](../../MoM/src/Combat.c#L3172).
- **Wall-of-fire check runs after the animation, before the cell commit** (asm:271-273). Production [3176](../../MoM/src/Combat.c#L3176). The listing reaches it through a thunk labelled `j_BU_WallofFire__NOOP`; the `proc` it lands on is `Check_Wall_Of_Fire_Attack` and it does real work, so the `__NOOP` in the thunk label is misleading.
- **Death check breaks the walk** (asm:279-281). The listing tests `cmp [status], bus_Active` / `jle` — continue while `status <= bus_Active`. Production [3177](../../MoM/src/Combat.c#L3177) tests `!= bus_Active`. `bus_Active` is 0 ([Combat.h:803](../../MoM/src/Combat.h#L803)) and the enum has no negative members, so the two select identically over every defined value.
- **Cell commit after the death check** — `move_anim_ctr = 0`, then `cgx`/`cgy` from the path arrays (asm:282-314). Production [3181-3183](../../MoM/src/Combat.c#L3181-L3183).
- **Post-move teardown** — `move_anim_ctr = 0`, `Moving = ST_FALSE` (asm:338-350). Production [3190-3191](../../MoM/src/Combat.c#L3190-L3191).
- **Facing derivation** — `movement_path_grid_cell_count > 1` takes `Origin` from step `count-2` and `Last_Facing` from `count-1`; otherwise `Last_Facing` comes from the target parameters (asm:351-386). `Facing_Diff` is then `Last_Facing - Origin` on both axes (asm:387-392). Production [3192-3205](../../MoM/src/Combat.c#L3192-L3205).
- **Instant-move deduction is a flat 2 points, not a cost-map lookup** — `loc_77D22`, asm:476-506: `movement_points > 2` subtracts 2, else clamps to 0. Production [3206-3216](../../MoM/src/Combat.c#L3206-L3216).
- **Normal-move deduction** — `cgx`/`cgy` copied from `target_cgx`/`target_cgy`, `target_*` advanced by `Last_Facing + Facing_Diff`, then `movement_points -= _cmbt_mvpth_c[cgy * 21 + cgx]` as byte arithmetic (asm:396-474). Production [3219-3223](../../MoM/src/Combat.c#L3219-L3223).
- **Final clamp** — `if (movement_points < 0) movement_points = 0` (asm:507-520). Production's `SETMIN(..., 0)` at [3225](../../MoM/src/Combat.c#L3225) has that sense ([MOX_DEF.h:95](../../MoX/src/MOX_DEF.h#L95)).

---

# Get_Battle_Unit_Move_Sound_Buffer

Faithful. `if (magic_set.sound_effects == ST_TRUE) Play_Sound(silence)`, then `Mark_Block(World_Data)`, `Reload_Battle_Unit_Move_Sound(battle_unit_idx)` into the local, `Release_Block(World_Data)`, return the segment. One stack local (`sound_seg`, `word ptr -2`, `sub sp, 2`).

Production [6098-6114](../../MoM/src/Combat.c#L6098-L6114), plus the size out-parameter under D1.

---

# Check_Wall_Of_Fire_Attack

Faithful. The listing is a chain of early exits to `@@Done`, in this order:

1. `wall_of_fire > ST_FALSE` (`jg`)
2. not `MV_FLYING`
3. not `MV_TELEPORT`
4. not `MV_MERGING`
5. `Battle_Unit_Is_Within_City` returns zero
6. `target_cgx >= 5`
7. `target_cgx <= 8`
8. `target_cgy >= 10`
9. `target_cgy <= 13`

Production [21342-21360](../../MoM/src/Combat.c#L21342-L21360) expresses the same nine tests as one `&&` chain in the same order, with `MIN`/`MAX_CGX_CITY` and `MIN`/`MAX_CGY_CITY` evaluating to the listing's literals.

The damage call takes `(spl_Fireball, battle_unit_idx, &damage_array[0], 0)` — the listing pushes `0`, the array address, the index, then `spl_Fireball`, which is that order under cdecl. `damage_array` is `word ptr -6` with `sub sp, 6`, i.e. three words, matching production's `int16_t damage_array[3]` at [21341](../../MoM/src/Combat.c#L21341).

Note this function tests the unit's **`target_cgx`/`target_cgy`**, not its current cell — which is why `Move_Battle_Unit` calls it after setting the step target but before committing `cgx`/`cgy`. The ordering in the caller is load-bearing.

---

# Battle_Unit_Teleport / Battle_Unit_Tunnel

Both faithful apart from D1.

## Frame maps

`BU_Teleport.asm`, `sub sp, 10h` = 8 words; `BU_TunnelTo.asm`, `sub sp, 0Eh` = 7 words. `di` carries the loop counter in both, `si` the `battle_unit_idx` parameter.

| asm name | production name | in Tunnel |
| --- | --- | --- |
| `Frame_Speed` | `fade_step_per_frame` | — (Tunnel's loops are fixed-length) |
| `Sound_Data_Seg` | `sound_seg` | same |
| `Origin_Y` / `Origin_X` | `origin_cgy` / `origin_cgx` | same |
| `Target_Draw_Y` / `Target_Draw_X` | `dst_screen_y` / `dst_screen_x` | same |
| `Origin_Draw_Y` / `Origin_Draw_X` | `src_screen_y` / `src_screen_x` | same |
| `di` | `i` | same |

Production adds `sound_seg_size` (D1) in both, and `temp_y` in Tunnel. The listing computes Tunnel's Y in registers and pushes `dx` directly (`BU_TunnelTo.asm:144-150`, `asm:211-217`) with no store to a named slot — that says nothing either way about whether the source declared a temp, so it is not recorded as a divergence.

## Faithful — verified, leave alone

- **Screen projection** — `Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &x, &y)` for origin then destination, argument order confirmed by push order (`BU_Teleport.asm:19-53`). Then `-13` on both X and `-27` on both Y, in that sequence. Production [Spells133.c:972-979](../../MoM/src/Spells133.c#L972-L979), [1062-1068](../../MoM/src/Spells133.c#L1062-L1068). The `(4, 4)` sub-cell offset resolves to `+0` X, `+8` Y ([Combat.c:27746-27750](../../MoM/src/Combat.c#L27746-L27750)) — the cell centre; the `-13`/`-27` then move to the sprite's top-left anchor.
- **`Frame_Speed` selection** — `cmp [magic_set.movement_animations], 0`, 20 or 50 (`BU_Teleport.asm:58-64`). Production [981-988](../../MoM/src/Spells133.c#L981-L988). The listing's branch layout does not determine which sense the source tested, so the inverted-looking condition is not a divergence.
- **Origin capture point** — `Origin_X`/`Origin_Y` are read after the screen-coordinate adjustments and before `Mark_Block` (`BU_Teleport.asm:65-81`). Production [989-992](../../MoM/src/Spells133.c#L989-L992), [1069-1071](../../MoM/src/Spells133.c#L1069-L1071).
- **Sound block** — silence, then `LBX_Reload_Next(soundfx_lbx_file__ovr133__1of2, SFX_*, World_Data)`, then play, all under `sound_effects == 1` (`BU_Teleport.asm:82-97`). Production [994-1001](../../MoM/src/Spells133.c#L994-L1001), [1072-1080](../../MoM/src/Spells133.c#L1072-L1080).
- **Compose / hide / release ordering differs between the two functions and both are faithful.** Teleport is compose, `bus_Dead`, `Release_Block` (`BU_Teleport.asm:99-110`, production [1002-1005](../../MoM/src/Spells133.c#L1002-L1005)); Tunnel is compose, `Release_Block`, `bus_Dead` (`BU_TunnelTo.asm:91-102`, production [1081-1085](../../MoM/src/Spells133.c#L1081-L1085)). Do not normalise these to each other.
- **`Copy_Bitmap_To_Bitmap(scratch_bitmap_seg, battle_unit_scratch_seg)`** — the callee's frame is `target_bitmap = word ptr 6`, `source_bitmap = word ptr 8`, and it sizes the copy from the source header before `_fmemcpy` (`seg030/Copy_Bitmap_To_Bitmap.asm:3-48`). Both `ovr133` listings push `IMG_CMB_FX_Figure@` then `GfxBuf_2400B`, so target is the scratch buffer and source is the composed figure. Production [1017](../../MoM/src/Spells133.c#L1017), [1027](../../MoM/src/Spells133.c#L1027), [1094](../../MoM/src/Spells133.c#L1094), [1118](../../MoM/src/Spells133.c#L1118).
- **Teleport cross-fade** — bottom-tested `for (i = 150; i > 0; i -= Frame_Speed)`; `cmp di, 50 / jle` skips the fade-out, so the guard is `i > 50`; `cmp di, 100 / jg` skips the fade-in, so the guard is `i <= 100`; `Vanish_Bitmap` takes `i - 50` and `100 - i` respectively (`BU_Teleport.asm:111-186`). Production [1008-1034](../../MoM/src/Spells133.c#L1008-L1034).
- **Tunnel sink and rise** — `di = 4` counting down, then `di = 0` counting up to 5, with the static-frame redraw between them (`BU_TunnelTo.asm:103-170`). `Vanish_Bitmap` takes `i * 20` in both. `Set_Window(0, 0, 319, y + 30)` clips each; the Y ramps are `Origin_Draw_Y + (5 - i) * 5` and `Target_Draw_Y + 20 - i * 5`. Production [1087-1130](../../MoM/src/Spells133.c#L1087-L1130).
- **Commit tail, identical in both** — `bus_Active`, `cgx`, `cgy`, then `target_cgx -= (Origin_X - cgx)` and `target_cgy -= (Origin_Y - cgy)`, then `Set_Page_Off` / `Combat_Screen_Draw` / `PageFlip_FX` (`BU_Teleport.asm:188-254`, `BU_TunnelTo.asm:232-289`). Production [1035-1044](../../MoM/src/Spells133.c#L1035-L1044), [1131-1140](../../MoM/src/Spells133.c#L1131-L1140).
- **No cached battle-unit pointer.** Both listings re-emit `imul 6Eh` / `les bx, [battle_units]` / `add bx, ax` at every field access rather than loading a stored far pointer, so every access in production indexes `battle_units[battle_unit_idx]` directly.

Cosmetic only: [994](../../MoM/src/Spells133.c#L994) tests `sound_effects == ST_TRUE` while [1072](../../MoM/src/Spells133.c#L1072) tests `== 1`. Both match `cmp [magic_set.sound_effects], 1`.

---

# Battle_Unit_Compose_Bitmap

Faithful.

- **Allocate before the status check.** `Allocate_Next_Block(World_Data, 124)` runs first, then `cmp [status], bus_Active` bails to `@@Done` (asm:29-43). An inactive unit still leaves an allocated block for the caller to release. Production [8097-8101](../../MoM/src/Combat.c#L8097-L8101).
- **`Create_Picture(47, 42, …)`** (asm:45-51) — note the 47 here against the 45 used by the two FX callers. Production [8102](../../MoM/src/Combat.c#L8102).
- **Figure counts read as bytes** with `cbw` (asm:57-67), then the Hydra collapse to a single figure (asm:81-84). Production [8104-8110](../../MoM/src/Combat.c#L8104-L8110). The comparison is `cmp ax, es:[bx+(spl_Hydra*24h)+…]` — indexed by **spell**, `spl_Hydra` = 111 ([Spellbook.h:137](../../MoM/src/Spellbook.h#L137)), not by unit type. The operand is a 16-bit read of 0x20-0x21 regardless of the `Param0` label IDA attached to the offset, and Hydra is a summoning spell, so `int16_t unit_type` is the live union arm ([MOM_DAT.h:1072](../../MoX/src/MOM_DAT.h#L1072)).
- **State refresh in order** — outline, animation flags, invisibility effect (asm:86-94). Production [8111-8113](../../MoM/src/Combat.c#L8111-L8113).
- **`frame_num` selection** — `Always_Animate` first, else `Moving` with `Move_Bob` choosing between the two frame counters, else literal 1 (asm:129-158). Production [8119-8137](../../MoM/src/Combat.c#L8119-L8137).
- **The computed facing index is discarded.** The listing builds `_DI_figure_set_idx` through the full nine-way decision tree (asm:187-238) and then overwrites it with `mov _DI_figure_set_idx, 7` immediately before use (asm:267). Production's `figure_set_idx = 7;` at [8146](../../MoM/src/Combat.c#L8146) is faithful, and the tree above it is what the `FIGUREX_*` macros stand in for. **OGBUG** — the facing computation is dead in the original.
- **Compose then stamp** — `Set_Animation_Frame`, `Draw_Picture_To_Bitmap`, banner colour, enchantment outline, figure effect (asm:268-294), then the per-figure loop calling `Battle_Unit_Figure_Position(max_figs, itr, &fig_x, &fig_y)` and `Clipped_Copy_Bitmap(fig_x, fig_y - 4, IMG_CMB_FX_Figure, GfxBuf_2400B)` (asm:295-318). Both argument orders confirmed by push order. Production [8147-8157](../../MoM/src/Combat.c#L8147-L8157).

---

# Battle_Unit_Set_Animation_Flags

Faithful apart from D2.

Initialise both flags false; `Attribs_1 & USA_FLYING` sets the move flag true and calls `Battle_Unit_Is_Airborne` for the idle flag; the Great Wyrm / Fire Elemental comparison forces the idle flag true; store idle then move (asm:13-76). Production [9411-9432](../../MoM/src/Combat.c#L9411-L9432).

Both spell comparisons read the 16-bit `unit_type` arm — `cmp ax, es:[bx+(spl_*×24h)+s_SPELL_DATA.Params0_1]` (asm:44, asm:59) — matching production's `.unit_type` at [9423](../../MoM/src/Combat.c#L9423) and [9425](../../MoM/src/Combat.c#L9425).

Note the flags are gated on `USA_FLYING` in `Attribs_1`, which arrives only via the wholesale unit-type `memcpy` in `Load_Battle_Unit` ([15414](../../MoM/src/Combat.c#L15414)) and is never set afterwards. Spell-granted flight sets `Move_Flags |= MV_FLYING` instead ([15779](../../MoM/src/Combat.c#L15779), [15909](../../MoM/src/Combat.c#L15909)), so an enchanted flier keeps its walk cycle. **OGBUG** — faithful, do not fix.

---

# Battle_Unit_Is_Airborne

Faithful. Three `test`/`jz` blocks in listing order — `Move_Flags & MV_FLYING` sets true, `Combat_Effects & bue_Web` clears it, `Combat_Effects & bue_Black_Sleep` clears it — with the result accumulated in `cx` from an initial `xor` (asm:10-38). Production [5938-5955](../../MoM/src/Combat.c#L5938-L5955).

Distinct from `Battle_Unit_Has_Flight` ([5974](../../MoM/src/Combat.c#L5974)), which additionally ORs in `UE_WIND_WALKING`, `UE_FLIGHT`, and the unit-type table's `MV_FLYING`. The two are not interchangeable.

---

# Battle_Unit_Set_Invisibility_Effect

Faithful.

- Combined enchantments `battle_units[].enchantments | _UNITS[].enchantments` (asm:29-48), then the gate: `UE_INVISIBILITY` in the low word — the listing masks the high word with `and ax, 0` — or `Abilities & UA_INVISIBILITY`; neither means an immediate `jmp @@Done` (asm:49-63). Production [9295-9303](../../MoM/src/Combat.c#L9295-L9303).
- Three reveals, each writing `Image_Effect = 4`: defender's unit against `_attacker_sees_illusions`, attacker's unit against `_defender_sees_illusions`, then any unit owned by `_combat_human_player` (asm:64-101). Production [9307-9329](../../MoM/src/Combat.c#L9307-L9329).
- If still hidden, `Image_Effect = 5` and the proximity scan: `status == bus_Active`, controller differs, `Range_To_Battle_Unit(battle_unit_idx, itr) < 2` (`cmp [bp+distance], 2 / jge` to skip), setting `Image_Effect = 4` (asm:103-149). Production [9331-9351](../../MoM/src/Combat.c#L9331-L9351).
- The loop does not break early once revealed — it runs to `_combat_total_unit_count`. Faithful; production carries a comment saying so.

---

## Build state

**Not verified — build could not be run.** `cmake` is not on the PATH in the agent shell (`cmake: command not found`, 2026-08-03), so `cmake --build --preset MSVC-debug` has not been run since the rename pass or the `spell_data_table[spl_Hydra]` fix. The review conclusions are byte-level comparisons and do not depend on a build, but the renames touched many call sites and warrant one:

```
cd /c/STU/devel/ReMoM && cmake --build --preset MSVC-debug 2>&1 | tail -200
```

Production anchors in this document were re-read against `Combat.c` and `Spells133.c` on 2026-08-03 and are current as of that pass.

## Related references

- [Combat-Assign_Combat_Grids.md](Combat-Assign_Combat_Grids.md) — `Set_Movement_Cost_Map`, `Update_Move_Map_City_Area_Restrictions`, `Battle_Unit_Instant_Movement_Mode`, and the identical cost-map seeding sequence; DONE-DONE.
- [Combat-Combat_Move_Path_Find.md](Combat-Combat_Move_Path_Find.md) — the path solver, and the `int16_t` element type of `_cmbt_mvpth_x`/`_y` this function indexes; DONE-DONE.
- [Combat-Combat_Move_Path_Valid.md](Combat-Combat_Move_Path_Valid.md) — the reachability overlay that gates which destinations reach this function; DONE-DONE.
- `Apply_Battle_Unit_Damage_From_Spell` and `BU_ApplyDamage` — held for the attack-functions review.
