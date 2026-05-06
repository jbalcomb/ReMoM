
# ReMoM Init Sequence — Data Dependencies

Traced from [ReMoM_Init_Engine()](../src/ReMoM_Init.c) (the shared init for ReMoM and HeMoM).

## Call Order

### 1. CONFIG.MOM — Sound Driver Config
- `fread(&config_mom, ...)` at [ReMoM_Init.c:82](../src/ReMoM_Init.c#L82)
- Binary struct `s_CONFIG_MOM_18`: MIDI/DIGI driver IDs, IO, IRQ, DMA

### 2. MAGIC.SET — Game Settings
- `Load_MAGIC_SET()` at [ReMoM_Init.c:86](../src/ReMoM_Init.c#L86)
- Struct `s_MAGIC_SET`: input type, sound channels, save-game slots

### 3. Init_Drivers() — [Init.c:44](../MoM/src/Init.c#L44)
Called at [ReMoM_Init.c:286](../src/ReMoM_Init.c#L286).
Internally calls:
- `EMS_Startup()`
- `Init_Video_Drivers()`
- **`Load_Font_File(FONTS.LBX)`** — [Init.c:60](../MoM/src/Init.c#L60)
  - **FONTS.LBX [0]** — font glyph / style data → `font_style_data`, cast to `font_header` ([Fonts.c:201](../MoX/src/Fonts.c#L201))
  - **FONTS.LBX [1]** — border / outline data → `border_style_data`
  - Allocates: `palette_block`, `current_palette`, `palette_flags`, `remap_color_palettes`, `Intensity_Scale_Tbl`
- `Init_Mouse_Keyboard()`
- `Randomize()`

### 4. Allocate_Data_Space(6100) — [ReMoM_Init.c:291](../src/ReMoM_Init.c#L291)
No file I/O.  Bulk memory allocation for:
- `EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`
- `_screen_seg`, `GfxBuf_2400B`, `g_graphics_cache_seg`
- `World_Data` (cities, world maps, terrain specials, explored/scouted maps)

### 5. Load_Palette(0, ST_UNDEFINED, 0) — [ReMoM_Init.c:293](../src/ReMoM_Init.c#L293)
Calls `LBX_Reload()` in [Fonts.c:2908](../MoX/src/Fonts.c) (approx):
- **FONTS.LBX [2]** (entry+2) — palette + font_colors + mouse_palette + remap_colors
  - Palette data layout:
    - `0x000`: 768 bytes RGB palette (256 * 3)
    - `0x300`: `font_colors` (16 blocks of 16 colour indices)
    - `0x500`: `mouse_palette` (16 cursor images)
    - `0x1500`: `remap_colors` (shading colour parameters)
- Then calls `Set_Font(0, 0, 0, 0)`

### 6. Load_MGC_Resources() — [LOADER.c:132](../MoM/src/LOADER.c#L132)
Called at [ReMoM_Init.c:296](../src/ReMoM_Init.c#L296).
- **MAINSCRN.LBX [0]** → `mainmenu_top`
- **MAINSCRN.LBX [5]** → `mainmenu_bot`
- **VORTEX.LBX [1]** → `mainmenu_c`
- **VORTEX.LBX [2]** → `mainmenu_h`
- **VORTEX.LBX [3]** → `mainmenu_q`
- **VORTEX.LBX [4]** → `mainmenu_n`
- **VORTEX.LBX [5]** → `mainmenu_l`
- **WIZARDS.LBX [0-13]** → `wizard_portrait_segs[0..13]`

### 7. Load_TERRSTAT() — [LOADER.c:771](../MoM/src/LOADER.c#L771)
Called at [ReMoM_Init.c:298](../src/ReMoM_Init.c#L298).
- **TERRSTAT.LBX [0]** → `terrain_stats_table` (770 records x 6 bytes)

### 8. Load_SPELLDAT() — [LOADER.c:778](../MoM/src/LOADER.c#L778)
Called at [ReMoM_Init.c:299](../src/ReMoM_Init.c#L299).
- **SPELLDAT.LBX [0]** → `spell_data_table` (215 records x 36 bytes)

### 9. Load_WZD_Resources() — [LOADER.c:174](../MoM/src/LOADER.c#L174)
Called at [ReMoM_Init.c:301](../src/ReMoM_Init.c#L301).
Calls the following in order:

#### 9a. Terrain_Init()
- **TERRAIN.LBX [0]** → `m_terrain_lbx_000`
- **TERRAIN.LBX [1]** → `m_terrain_lbx_001`
- **TERRAIN.LBX [2]** → `m_terrain_lbx_002`
- **TERRSTAT.LBX [1]** → `TBL_Unrest` (1 record, 196 bytes)
- **MAPBACK.LBX [0-93]** → unexplored masks, unit backgrounds, city overlays, roads, nodes, mineral sites, etc.

#### 9b. Main_Screen_Load_Pictures()
- **MAIN.LBX [0-64]** → main game screen UI: background, buttons, unit frames, medals, movement icons, event icons
- **BACKGRND.LBX [4]** → `mirror_screen_background_seg`

#### 9c. Load_Combat_Background_Bottom() — [LOADER.c:798](../MoM/src/LOADER.c#L798)
- **BACKGRND.LBX [3]** → `combat_background_bottom`

#### 9d. Load_SPELLDAT() — [LOADER.c:216](../MoM/src/LOADER.c#L216)
Called again inside Load_WZD_Resources.  Overwrites the pointer from step 8.
Noted in source as possible bug (`¿ BUGBUG  overwriting the pointer to a previous allocation (8k wasted) ?`).

#### 9e. Load_BUILDDAT() — [LOADER.c:785](../MoM/src/LOADER.c#L785)
- **BUILDDAT.LBX [0]** → `bldg_data_table` (36 records x 52 bytes)

#### 9f. Load_Button_Sounds() — [LOADER.c](../MoM/src/LOADER.c)
- **SOUNDFX.LBX [0]** → `snd_standard_button_click`
- **SOUNDFX.LBX [2]** → `snd_left_button_click`

#### 9g. Units_Upkeeps() — [LOADER.c:242](../MoM/src/LOADER.c#L242)
No file I/O.  Computes upkeep values from `_unit_type_table` and `spell_data_table`.

---

## Summary: LBX Files Loaded at Init

| File | Entries | Loaded By |
|------|---------|-----------|
| FONTS.LBX | 0, 1, 2 | Load_Font_File, Load_Palette |
| MAINSCRN.LBX | 0, 5 | Load_MGC_Resources |
| VORTEX.LBX | 1-5 | Load_MGC_Resources |
| WIZARDS.LBX | 0-13 | Load_MGC_Resources |
| TERRSTAT.LBX | 0, 1 | Load_TERRSTAT, Terrain_Init |
| SPELLDAT.LBX | 0 | Load_SPELLDAT |
| TERRAIN.LBX | 0-2 | Terrain_Init |
| MAPBACK.LBX | 0-93 | Terrain_Init |
| MAIN.LBX | 0-64 | Main_Screen_Load_Pictures |
| BACKGRND.LBX | 3, 4 | Load_Combat_Background_Bottom, Main_Screen_Load_Pictures |
| BUILDDAT.LBX | 0 | Load_BUILDDAT |
| SOUNDFX.LBX | 0, 2 | Load_Button_Sounds |

## Non-LBX Files

| File | Format | Loaded By |
|------|--------|-----------|
| CONFIG.MOM | binary struct | ReMoM_Init_Engine (fread) |
| MAGIC.SET | binary struct | Load_MAGIC_SET |

## Key Cross-References

- Font header struct: [Fonts.h:83](../MoX/src/Fonts.h#L83) (`s_FONT_HEADER`)
- Font/palette notes: [STU-MoM-FON_PAL.md](STU-MoM-FON_PAL.md)
- Font rendering notes: [MoX-Fonts.md](MoX-Fonts.md)
