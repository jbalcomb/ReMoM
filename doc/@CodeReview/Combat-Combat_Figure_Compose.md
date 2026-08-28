Combat-Combat_Figure_Compose.md

SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

BU_CreateImage__SEGRAX()              ==>  Battle_Unit_Compose_Bitmap()
UU_BU_LoadFigureGFX()                 ==>  NIU_Battle_Unit_Load_Figure_Pictures()
LBX_IMG_RevGrayscale()                ==>  Inverse_Gray_Scale_Bitmap()
UU_DUP_RevGrayscale()                 ==>  DUPE_Inverse_Gray_Scale_Bitmap()
Combat_Figure_Effect__WIP()           ==>  Combat_Figure_Effect()
USELESS_Combat_Figure_Load_Compose()  ==>  Combat_Figure_Compose()
Combat_Figure_Compose_USEFULL()       ==>  Combat_Screen_Map_Compose_Figures()

palette_intensity_remap_table  ==>  palette_intensity_remap_table

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\BU_CreateImage__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\USELESS_Combat_Figure_Load_Compose.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\Combat_Figure_Compose_USEFULL.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr099\Draw_Active_Unit_Window.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\UU_BU_LoadFigureGFX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\Combat_Figure_Effect__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\Transparent_Color_Range.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg033\Gray_Scale_Bitmap.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg033\LBX_IMG_RevGrayscale.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\seg030\Replace_Color_All.asm

NIU_Battle_Unit_Load_Figure_Pictures()
    |-> Combat_Figure_Load()

Combat_Screen_Map_Compose_Figures()
    BATTLE_UNIT_FACING_DRECTION
    FIGUREX_MAP
    FIGUREX_OFFSET
    FIGUREX_POINTER
    Set_Animation_Frame()
    Draw_Picture_To_Bitmap()
    Combat_Figure_Banner_Color()
    Combat_Unit_Enchantment_Outline_Draw()
    Combat_Figure_Effect()
    Combat_Figure_Active_Red_Outline()

Combat_Figure_Compose()
    FIGUREX_OFFSET
    FIGUREX_MAP
    FIGUREX_POINTER
    Set_Animation_Frame()
    Draw_Picture_To_Bitmap()
    Combat_Figure_Banner_Color()
    Combat_Unit_Enchantment_Outline_Draw()

Battle_Unit_Compose_Bitmap()
    Combat_Unit_Enchantment_Outline_Set()
    Battle_Unit_Set_Animation_Flags()
    Battle_Unit_Set_Invisibility_Effect()
    Combat_Figure_Banner_Color()
    Combat_Unit_Enchantment_Outline_Draw()
    Combat_Figure_Effect()
    Battle_Unit_Figure_Position()
    Clipped_Copy_Bitmap()

Combat.c
// WZD ovr163p06
Combat_Figure_Load()

Combat.c
// WZD s103p07
Battle_Unit_Compose_Bitmap()

Combat_Screen()
    |-> Combat_Screen_Draw()
        |-> Draw_Active_Unit_Window()
            |-> Combat_Screen_Map_Compose_Figures()

Battle_Unit_Teleport()
Battle_Unit_Tunnel()
Battle_Unit_Summon_Animation()
    |-> Battle_Unit_Compose_Bitmap()
        |-> Combat_Unit_Enchantment_Outline_Set()
        |-> Battle_Unit_Set_Animation_Flags()
        |-> Battle_Unit_Set_Invisibility_Effect()
        |-> Combat_Figure_Banner_Color()
        |-> Combat_Unit_Enchantment_Outline_Draw()
        |-> Combat_Figure_Effect()
        |-> Battle_Unit_Figure_Position()

Draw_Active_Unit_Window()
    Combat_Figure_Compose()
    Combat_Figure_Effect()
    Outline_Bitmap_Pixels()
    Draw_Picture()
    Draw_Active_Unit_Stats_And_Icons()
    Draw_Active_Unit_Damage_Bar()
 
Combat_Screen()
    |-> Allocate_Combat_Base_Blocks()
    |-> Combat_Screen_Load_Resources()
    |-> Cache_Graphics_Combat()
    |-> Build_Battlefield()
    |-> Prepare_All_Battle_Units()
        |-> Load_Battle_Unit()
        |-> Combat_Figure_Load()
            |-> Combat_Figure_Compose()
        |-> Combat_Screen_Map_Draw()
            |-> Combat_Screen_Map_Compose_Figures()
            |-> Combat_Screen_Map_Draw_Entities()



## Unit Figure Composition
Combat.c    WZD ovr153p23   Combat_Screen_Map_Compose_Figures()
vs.
Combat.c    WZD ovr153p15   Combat_Figure_Compose()
vs.
Combat.c    WZD o99p06      Combat_Grid_Entities()
vs.
Combat.c    WZD s103p07     Battle_Unit_Compose_Bitmap()
vs.
UnitView.C  WZD o89p05      Draw_Unit_Figure()

The four
Combat_Figure_Compose — faithful. It's the cut-down twin of Combat_Screen_Map_Compose_Figures: same figure-page setup and bitmap compose, but stops after the banner colour and outline — no Combat_Figure_Effect, no Combat_Figure_Active_Red_Outline.
It also settles the ¿ three different macros for a (real) reason ? question in Combat.h:575-582. The reason is ordering: Combat_Figure_Load and Combat_Screen_Map_Compose_Figures both do MAP-then-OFFSET, but this one does OFFSET-then-MAP (asm:18-33). Production preserves each function's own order. Don't normalise them.



## Unit Figure Position
!!!!! ALL DIFFERENT !!!!!
Combat.c    Combat_Grid_Entities()  (in-line)
vs.
UnitView.C  Unit_Figure_Position()
vs.
Combat.c    Battle_Unit_Figure_Position()

Unit_Figure_Position — faithful, all 36 pairs. This is the third figure-position table, and the "ALL DIFFERENT" comment is now confirmed for all three:
                                    case 4, fig 1   case 6, fig 1
BATTLE_UNIT_FIGURE_POSITION macro   −7, 8           4, 7
Battle_Unit_Figure_Position          8, 8           10, 8
Unit_Figure_Position                −9, 8           4, 7

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-08).** No findings open; three deviations. Seven functions compared against 1,298 asm lines.

None of the seven is a `__WIP` / `__STUB` function, so none has a `stub_wip_todo.md` row to check.

These seven are the whole figure-composition path. Four of them *compose* — map an EMS page, resolve an eight-entry facing table, pick a frame, blit one figure into a scratch bitmap, then recolour and outline it. One *loads* — builds an LBX filename and pulls the eight facings in. One *claims* the EMS page they all share. One is the active-unit HUD panel, the second consumer of the cut-down composer.

The three composers differ only in what drives the facing index and in what they do after the blit, and every one of those differences is faithful. See [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) for the two shared composers' first pass and for the three figure-position tables.

## Scope

The Production column carries the current name and line; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Lines |
| --- | --- | --- | --- |
| `Next_Battle_Unit_Nearest_Available` | [Combat.c:6441](../../MoM/src/Combat.c#L6441) | `ovr103/Next_Battle_Unit_Nearest_Available.asm` | 208 |
| `Draw_Active_Unit_Stats_And_Icons` | [Combat.c:5596](../../MoM/src/Combat.c#L5596) | `ovr099/Draw_Active_Unit_Stats_And_Icons.asm` | 167 |
| `Draw_Active_Unit_Damage_Bar` | [Combat.c:5714](../../MoM/src/Combat.c#L5714) | `ovr099/Draw_Active_Unit_Damage_Bar.asm` | 122 |
| `Draw_Active_Unit_Window` | [Combat.c:5406](../../MoM/src/Combat.c#L5406) | `ovr099/Draw_Active_Unit_Window.asm` | 347 |
| `Battle_Unit_Compose_Bitmap` | [Combat.c:6283](../../MoM/src/Combat.c#L6283) | `ovr103/BU_CreateImage__SEGRAX.asm` | 325 |
| `Combat_Screen_Map_Compose_Figures` | [Combat.c:19616](../../MoM/src/Combat.c#L19616) | `ovr153/Combat_Figure_Compose_USEFULL.asm` | 284 |
| `Combat_Figure_Load` | [Combat.c:22401](../../MoM/src/Combat.c#L22401) | `ovr163/Combat_Figure_Load.asm` | 142 |
| `Claim_EMS_Page_For_Figure_Set` | [Combat.c:19580](../../MoM/src/Combat.c#L19580) | `ovr153/EMM_FIGUREX_Init__HACK.asm` | 77 |
| `Combat_Figure_Compose` | [Combat.c:19268](../../MoM/src/Combat.c#L19268) | `ovr153/USELESS_Combat_Figure_Load_Compose.asm` | 77 |
| `NIU_Battle_Unit_Load_Figure_Pictures` | [Combat.c:2266](../../MoM/src/Combat.c#L2266) | `ovr091/UU_BU_LoadFigureGFX.asm` | 46 |

Everything not listed under Findings matched: declaration order in all seven frames, every field width and signedness (`cbw` versus `mov ah,0`), every argument order, every literal, and both branch tables.

---

# Findings

None open.

---

# Deviations

## D1 — `FIGUREX_POINTER` allocates 5 paragraphs where the asm allocates 2

All four asm call sites agree — `ovr153/USELESS_Combat_Figure_Load_Compose.asm:34`, `ovr153/Combat_Figure_Compose_USEFULL.asm:217`, `ovr163/Combat_Figure_Load.asm:33` and `ovr103/BU_CreateImage__SEGRAX.asm:253` are all `mov ax, 2`. [Combat.h:588-590](../../MoM/src/Combat.h#L588-L590) passes 5.

Keep the 5. The argument is a paragraph count — in [Allocate.c:424](../../MoX/src/Allocate.c#L424) `size` scales the 16-byte header offsets and the payload is `size * 16` bytes. The block holds `SAMB_ptr figure_pict_set_seg[8]`, and `SAMB_ptr` is a real `unsigned char *` in this port ([MOX_TYPE.h:26](../../MoX/src/MOX_TYPE.h#L26)), not the OG's 2-byte handle. Eight 8-byte pointers plus the 16-byte sub-block header is 80 bytes — 5 paragraphs. The OG's 2 gives 32 bytes and overflows by half the table.

The prose block above the macros matches neither value: [Combat.h:511](../../MoM/src/Combat.h#L511), [525](../../MoM/src/Combat.h#L525), [539](../../MoM/src/Combat.h#L539) and [556](../../MoM/src/Combat.h#L556) all write `33`, and the comment on line 539 then derives 5 in its own arithmetic. Reconcile those four to 5 so nobody "fixes" the macro back to 2.

## D2 — `SA_MK_FP0` and its paired word local are collapsed into the macro

Every asm composer stores `Allocate_First_Block`'s return into a word local and then converts it with `SA_MK_FP0` into the far pointer it actually indexes — `Fig_IMGSeg_Table@` / `Fig_IMGSeg_Table_Full@`, `figure_pointer_seg` / `ptr_figure_pointer_seg`, `p_FIGUREX` / `fp_FIGUREX`, `temp_figure_pict_set_seg` / `figure_pict_set_seg`. Since `SAMB_ptr` is already a real pointer in this port there is nothing to convert, and `FIGUREX_POINTER` collapses the pair into one assignment.

Correct, but recorded inconsistently. `Battle_Unit_Compose_Bitmap` preserves the dropped word local as a comment at [Combat.c:6840](../../MoM/src/Combat.c#L6840); the other three drop it silently. Add the same commented-out line to `Combat_Screen_Map_Compose_Figures`, `Combat_Figure_Compose` and `Combat_Figure_Load` so all four frames read the same way against their listings.

## D3 — `EMM_MapMulti4` is replaced by pointer arithmetic

`ovr153/EMM_FIGUREX_Init__HACK.asm:31-33` calls `EMM_MapMulti4(logical_page, EmmHndl_FIGUREX)`. [Combat.c:20976](../../MoM/src/Combat.c#L20976) computes `EMS_PFBA` directly and already carries the `TODO` naming the call it replaced. The four `farpokew` stores that follow are faithful, `1591` block size included.

---

# Notes worth keeping

## `NIU_Battle_Unit_Load_Figure_Pictures` is dead in the OG, and is a one-liner

Nothing calls it. The only other reference anywhere in the disassembly is its own unreached stub thunk, `stub091/NX_j_UU_BU_LoadFigureGFX.asm` — which is what the `UU_` prefix records.

What it does: load a battle unit's eight figure pictures into the EMS figure cache using the unit's own index as the cache slot, and write that slot back to `.bufpi`. Since `Combat_Figure_Load` returns its `bufpi` argument unchanged (`ovr163/Combat_Figure_Load.asm:135`), the round trip is the identity — the store amounts to `battle_units[i].bufpi = i`.

Renamed to `NIU_Battle_Unit_Load_Figure_Pictures`, matching the `NIU_Gibs_Frames` precedent for OG-dead code. The listing keeps its IDA name, `ovr091/UU_BU_LoadFigureGFX.asm`.

## `Battle_Unit_Compose_Bitmap` computes a facing and throws it away

[Combat.c:6901](../../MoM/src/Combat.c#L6901) expands `BATTLE_UNIT_FACING_DRECTION`, which resolves `figure_set_idx` from the unit's position versus its target. [Combat.c:6906](../../MoM/src/Combat.c#L6906) then overwrites it with `7` unconditionally. That is exactly the asm: the whole branch table at `ovr103/BU_CreateImage__SEGRAX.asm:187-238` writes `DI`, and asm:267 is `mov _DI_figure_set_idx, 7` — placed before `DI`'s only two uses, at asm:269 and asm:278. The computed facing is provably dead.

**OGBUG — faithful, do not fix.** `Combat_Screen_Map_Compose_Figures` has no such override and does use its computed facing (`ovr153/Combat_Figure_Compose_USEFULL.asm:232`), so the discard is unique to the summon / teleport / tunnel path.

## The facing table, verified against both listings

`BATTLE_UNIT_FACING_DRECTION` at [CMBTDEF.h:193](../../MoM/src/CMBTDEF.h#L193) matches `ovr103/BU_CreateImage__SEGRAX.asm:187-238` and `ovr153/Combat_Figure_Compose_USEFULL.asm:150-200` cell for cell. All nine comparisons are signed (`jle` / `jge`).

| | `cgy > target_cgy` | `cgy < target_cgy` | equal |
| --- | --- | --- | --- |
| `cgx > target_cgx` | 0 | 6 | 7 |
| `cgx < target_cgx` | 2 | 4 | 3 |
| equal | 1 | 5 | 2 |

Both 2s are real — the table is not a typo.

## What the three composers do differently

| | facing index from | after the blit |
| --- | --- | --- |
| `Combat_Screen_Map_Compose_Figures` | the facing table, per unit | banner, outline, `Combat_Figure_Effect`, `Combat_Figure_Active_Red_Outline` |
| `Battle_Unit_Compose_Bitmap` | forced to 7 | banner, outline, `Combat_Figure_Effect` |
| `Combat_Figure_Compose` | the caller's `figure_set_idx` argument | banner, outline |

`Combat_Screen_Map_Compose_Figures` is also the only one that consults `Melee_Anim` for the frame (`ovr153/Combat_Figure_Compose_USEFULL.asm:106-120`) and the only one that redirects `scratch_bitmap_seg` to the per-unit cached bitmap mid-sequence, between `Set_Animation_Frame` and `Draw_Picture_To_Bitmap` (asm:240-248). Production has that redirect in exactly that position, and re-reads `battle_units[i].bufpi` there rather than reusing the cached copy — also faithful.

`Battle_Unit_Compose_Bitmap` is the only one that allocates its own destination — `Allocate_Next_Block(World_Data, 124)` at [Combat.c:6856](../../MoM/src/Combat.c#L6856), 124 paragraphs for a 47×42 picture — and the only one that then tiles the composed figure `figure_cnt` times through `Battle_Unit_Figure_Position`.

## The LBX filename `Combat_Figure_Load` builds

`unit_type` selects both the file and the entry, by signed division by 15:

- file: `"FIGURE"`, then `"S"` if `(unit_type / 15) + 1 < 10`, then the number, then `""` — giving `FIGURES1` through `FIGURES9`, then `FIGURE10` and up. The `"S"` exists only to keep the DOS 8.3 stem full-width.
- entry: `(unit_type % 15) * 8`, then `+ 0..7` for the eight facings.

So each `FIGURE*.LBX` holds 15 units of 8 facings each. The three string globals are at [Combat.c:617](../../MoM/src/Combat.c#L617), [619](../../MoM/src/Combat.c#L619) and [637](../../MoM/src/Combat.c#L637); `LEN_FILE_NAME` is 20 ([MOX_BASE.h:33](../../MoX/src/MOX_BASE.h#L33)), matching the 20-byte `file_name` slot in the asm frame.

## `spell_data_table[spl_Hydra].unit_type` reads a word, not `Param0`

`ovr103/BU_CreateImage__SEGRAX.asm:81` symbolises the operand as `s_SPELL_DATA.Param0`, but the instruction is `cmp ax, es:[...]` — a 16-bit compare, so it reads the whole word at `0x20`. That is the `unit_type` arm of the union, which is what [Combat.c:6865](../../MoM/src/Combat.c#L6865) uses. Same situation as `spell_data_table[spl_Magic_Spirit].unit_type` in the Map_Draw review; the union is documented at [MOM_DAT.h:1071](../../MoX/src/MOM_DAT.h#L1071).

The effect: a Hydra composes as a single figure regardless of its real `figure_cnt`.

## `Set_Font_Colors_15` reads 16 bytes out of a 4-byte array

[Combat.c:5609](../../MoM/src/Combat.c#L5609) declares `uint8_t colors[4]` and sets only `[0]` and `[1]`, then both arms hand `&colors[0]` to `Set_Font_Colors_15`, which copies **16** bytes into font colour block 15 ([Fonts.c:368-372](../../MoX/src/Fonts.c#L368-L372)).

The 4 is not a reconstruction slip. `Draw_Active_Unit_Window.asm:3-4` puts `colors` at `bp-14h` and the next local, `unit_owner_idx`, at `bp-10h` — four bytes apart. The whole frame is `sub sp, 14h` = 20 bytes, and eight more word locals have to fit in it, so a 16-byte array is impossible. The OG really does over-read 12 bytes of adjacent stack into colour slots 2-15.

**OGBUG — faithful, do not fix.** It is also SimTex's habit rather than a one-off: the same call takes a `colors[2]` at [INTRO.c:113](../../MoM/src/INTRO.c#L113), `colors[4]` at [Combat.c:5750](../../MoM/src/Combat.c#L5750) and [CREDITS.c:256](../../MoM/src/CREDITS.c#L256), `colors[5]` at [AdvsrScr.c:945](../../MoM/src/AdvsrScr.c#L945), and `colors[6]` at [Combat.c:5368](../../MoM/src/Combat.c#L5368) — every size taken from its own IDA frame.

## Two redundancies in `Draw_Active_Unit_Window` that are faithful

Neither is worth tidying; both are in the listing.

- The invisibility test re-derives `_UNITS[battle_units[_active_battle_unit].unit_idx].type` from scratch (asm:57-72) and only afterwards caches `unit_idx` and `unit_type` (asm:86-95). Production does the same. Do not hoist the cache above the test.
- The six-statement font setup — `colors[0]`, `colors[1]`, `Set_Font_Colors_15`, `Set_Outline_Color`, `Set_Font_Style_Shadow_Down`, `Set_Font_Spacing_Width` — is written out twice, once per arm (asm:138-163 and asm:282-307). Do not hoist it above the branch.

The two arms also end in genuinely different calls — `Draw_Picture` (asm:133) in one, `Draw_Picture_Windowed` (asm:280) in the other. That is not a copy-paste slip.

---

# What is not covered

`Draw_Active_Unit_Stats_And_Icons` (`ovr099`, 167 lines) and `Draw_Active_Unit_Damage_Bar` (`ovr099`, 122 lines) are called from `Draw_Active_Unit_Window` but draw HUD stats and a damage bar, not figures — they belong with the active-unit panel, not here. `Combat_Screen_Draw`, the sole caller of `Draw_Active_Unit_Window`, is covered in [Combat-Combat_Screen.md](Combat-Combat_Screen.md).

The helpers every composer calls — `Combat_Figure_Banner_Color`, `Combat_Unit_Enchantment_Outline_Set` and `_Draw`, `Combat_Figure_Effect`, `Combat_Figure_Active_Red_Outline`, `Battle_Unit_Set_Animation_Flags`, `Battle_Unit_Set_Invisibility_Effect`, `Battle_Unit_Figure_Position` — were all reviewed in [Combat-Combat_Screen_Map_Draw.md](Combat-Combat_Screen_Map_Draw.md) and are not re-derived here.

## Build state

`cmake --build --preset MSVC-debug` and `cmake --build --preset MSVC-release` both clean — every target links, `ReMoM`, `HeMoM`, `MOM_tests` and `MOX_tests` included. `ctest -C Debug`: 362 of 363 pass, 9 disabled.

The single failure, `HeMoM_Combat_Strategic_Assertions`, is **pre-existing at HEAD and unrelated to this review**. It diverges on `combat.rng_calls` (30 vs 24) and the unit outcomes that cascade from it. Nothing in this review's scope can cause that: none of the seven functions calls `Random()`, `HeMoM` is headless so the figure-composition path does not render, and the working-tree diff for `Combat.c` contains only renames plus constant substitutions of equal value (`ST_REMAP_COLOR` = 232, `ST_BLACK` = 1). The golden `tests/assert_combat_strategic.txt` was last updated 2026-07-15, while combat logic has been committed as recently as `7669b2c4` (2026-08-08 18:20) — the drift is in committed code and needs its own attribution pass.

---

# The active-unit panel trio

Three slots this document named in its call tree and never adjudicated. Walked 2026-08-26, 497 asm
lines. All three faithful; one structural deviation, recorded below as D-next.

## `Draw_Active_Unit_Stats_And_Icons` ([Combat.c:5596](../../MoM/src/Combat.c#L5596), asm 167)

Draws the melee and ranged strength numbers with their weapon glyphs, the movement-point string, and
the movement-mode glyph, into the active-unit panel.

**Declaration order is exact, and the array size confirms it.** Two stack locals: `colors` at `bp-6`
and `move_type` at `bp-2`. The 4-byte gap between them is production's `uint8_t colors[4]`, declared
first, then `battle_unit_movement_mode`. `icon_idx` (SI) and `attack_strength` (DI) are register
locals and carry no ordering constraint.

**Both attack blocks are the same shape**, and production reproduces both: call the icon picker,
`!= ST_UNDEFINED`, read the strength, `Print_Integer_Right(126, y, strength)`, then
`FLIC_Draw(128, y-2, combat_weapon_icon_segs[icon_idx])` — y is 174/172 for melee (asm:50-63) and
181/179 for ranged (asm:81-94). The `shl bx, 1` before each segment fetch is the word-index scaling.

**The movement-point split** is `cmp movement_points, 0 / jz` (asm:102-103): non-zero takes
`Print_Moves_String(126, 188, movement_points, 1)` (four args, `add sp, 8`), zero takes
`Print_Integer_Right(126, 188, movement_points)`. Production keeps both arms even though the else
arm always prints a literal zero.

**The three movement-mode literals map onto the enum exactly** — asm tests 2, then 4 and 5
(asm:142-149), and [Combat.h:809-816](../../MoM/src/Combat.h#L809-L816) gives `bumm_Flight` 2,
`bumm_Sailing` 4, `bumm_Swimming` 5. The `== 4 || == 5` pair is a `jz`-to-body / `jnz`-past pair,
the standard `||`.

### D-next — the movement-mode tests are two independent `if`s, not an `if`/`else if`

asm:142-149:

```
cmp     [bp+move_type], 2
jnz     short loc_81A8E     ; not 2 -> skip the store
mov     _SI_icon_idx, 1
loc_81A8E:                  ; BOTH paths arrive here
cmp     [bp+move_type], 4
jz      short loc_81A9A
cmp     [bp+move_type], 5
jnz     short loc_81A9D
```

After the `== 2` arm stores its value it **falls through** into the `== 4 || == 5` test — there is no
jump past it. So the source is two independent `if` statements. Production at
[Combat.c:5657](../../MoM/src/Combat.c#L5657) writes `else if`. Behaviour is identical, since
`move_type` cannot be both 2 and 4, but the structure is not the same. Contrast
`Draw_Active_Unit_Damage_Bar` below, where the listing really is a chain and each arm jumps past the
rest — the two functions are a clean side-by-side of the two shapes.

**Rename:** IDA's `Battle_Unit_Movement_Icon` (`ovr098`) is production's
[Battle_Unit_Movement_Mode](../../MoM/src/Combat.c#L4715). The production name is the better one — it
returns a `bumm_` mode and the icon index is derived from it afterwards.

## `Draw_Active_Unit_Damage_Bar` ([Combat.c:5714](../../MoM/src/Combat.c#L5714), asm 122)

Faithful, all 122 lines. Four stack locals from `bp-8` up — `bar_color`, `damage_hits`,
`current_hits`, `max_hits` — declared in production in exactly that order, then `bar_length` in DI.

The arithmetic reads off the listing directly: `max_hits = hits * figure_max` (asm:18-36, IDA's
`Max_Figures`), `damage_hits = (figure_max - figure_cnt) * hits` (asm:37-66), then
`if (front_figure_damage > 0) damage_hits += front_figure_damage` (asm:72-81) and
`current_hits = max_hits - damage_hits`.

**The bar length is `((current_hits * 20) / max_hits) - 1`** — asm:88-93 is `mov dx, 20 / imul dx /
cwd / idiv [bp+max_hits] / dec ax`. The `cwd` truncates the product to 16 bits before the divide,
which a 16-bit `int` does anyway, so no cast is needed.

**This one is a genuine `if`/`else if` chain.** asm:95-104: each arm ends `jmp short loc_81DCA`,
jumping past the remaining tests. Thresholds are `>= 13` then `>= 6`, and the colours match
[MOX_DEF.h:821-823](../../MoX/src/MOX_DEF.h#L821-L823) — `DAMAGE_BAR_GREEN` 0xD9 (217),
`DAMAGE_BAR_YELLOW` 0xD3 (211), `DAMAGE_BAR_RED` 0xCA (202) — against the listing's literal 217, 211,
202. `Line(x, y, x + bar_length, y, bar_color)` closes it, five args, `add sp, 0Ah`.

## `Next_Battle_Unit_Nearest_Available` ([Combat.c:6441](../../MoM/src/Combat.c#L6441), asm 208)

Faithful, all 208 lines. Picks the next unit to activate: nearest one with moves left, preferring a
non-waiting unit, and falling back to waking the whole waiting set.

**Thirteen stack locals, and production declares all thirteen in frame order** — from `bp-1Ah`:
`selected_unit_y`, `selected_unit_x`, `processed_units`, `uu_flag`, `unit_count`,
`closest_active_unit`, `closest_active_dist`, `closest_waiting_unit`, `closest_waiting_dist`,
`battle_unit_ctr`, `delta`, `all_done_none_available`, `done` — then `itr` (SI) and
`battle_unit_idx` (DI), both annotated.

**`uu_flag` is dead in the original.** asm:68 stores `ST_UNDEFINED` into it and nothing ever reads it.
Production keeps the single write at [Combat.c:6468](../../MoM/src/Combat.c#L6468) rather than
dropping the local — correct, since the slot is in the frame.

**`processed_units++` sits where the listing puts it, which is the subtle part.** The increment is at
`loc_84D94` (asm:150). Control reaches it when `movement_points <= 0` (asm:102 jumps straight there),
after the Wait arm (asm:139), and by fall-through from the Active arm — but **not** when the status or
controller test fails, since those jump to `loc_84D97`, past it. Production has it inside the
`status && controller_idx` block and after the `movement_points > 0` block closes, which is exactly
that set of paths. Getting this wrong would make the loop miscount and either spin or exit early.

**The two `Switch_Active_Battle_Unit` calls are ordered differently and production preserves the
asymmetry.** The active branch sets `done = ST_TRUE` *before* the call (asm:192-194); the waiting
branch calls *first* and sets `done` after (asm:164-167). Production matches at
[Combat.c:6569-6571](../../MoM/src/Combat.c#L6569-L6571) and
[Combat.c:6577-6579](../../MoM/src/Combat.c#L6577-L6579). No behavioural difference, but it is what
the listing does.

The waiting branch then walks every unit resetting `bua_Wait` to `bua_Ready` (asm:170-188), the
"everyone has waited, start the round over" path. The function returns `all_done_none_available`
(asm:201), which is `ST_TRUE` only when neither an active nor a waiting candidate was found.
