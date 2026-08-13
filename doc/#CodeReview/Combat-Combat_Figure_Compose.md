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
| `Draw_Active_Unit_Window` | [Combat.c:5610](../../MoM/src/Combat.c#L5610) | `ovr099/Draw_Active_Unit_Window.asm` | 347 |
| `Battle_Unit_Compose_Bitmap` | [Combat.c:6839](../../MoM/src/Combat.c#L6839) | `ovr103/BU_CreateImage__SEGRAX.asm` | 325 |
| `Combat_Screen_Map_Compose_Figures` | [Combat.c:21007](../../MoM/src/Combat.c#L21007) | `ovr153/Combat_Figure_Compose_USEFULL.asm` | 284 |
| `Combat_Figure_Load` | [Combat.c:23942](../../MoM/src/Combat.c#L23942) | `ovr163/Combat_Figure_Load.asm` | 142 |
| `Claim_EMS_Page_For_Figure_Set` | [Combat.c:20971](../../MoM/src/Combat.c#L20971) | `ovr153/EMM_FIGUREX_Init__HACK.asm` | 77 |
| `Combat_Figure_Compose` | [Combat.c:20639](../../MoM/src/Combat.c#L20639) | `ovr153/USELESS_Combat_Figure_Load_Compose.asm` | 77 |
| `NIU_Battle_Unit_Load_Figure_Pictures` | [Combat.c:2257](../../MoM/src/Combat.c#L2257) | `ovr091/UU_BU_LoadFigureGFX.asm` | 46 |

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
