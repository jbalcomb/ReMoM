
# PRD: Embedded Game Data — Eliminate Original-File Dependencies

## Problem

ReMoM cannot start without the original MoM game files on disk.  The init sequence
(`ReMoM_Init_Engine` in [ReMoM_Init.c](../src/ReMoM_Init.c)) opens CONFIG.MOM, MAGIC.SET,
and 12 LBX archives before anything useful happens.  If any file is missing, the game
crashes or exits.

This is a 1993 DOS game.  The entire asset set — every sprite, every table, every sound
effect loaded at start-up — is trivially small by modern standards.  There is no technical
reason the data can't live directly in the binary.

## Goal

Embed all start-up game data into the compiled binary so that ReMoM can initialise and run
with **zero original game files** present.

This is not a headless-testing concern (that's a separate problem about having no
video/input/sound *hardware*).  This is purely about not needing the *files*.

## What's Not In Scope

- Font interpretation, glyph extraction, or TTF conversion (separate fonts PRD).  The raw
  FONTS.LBX bytes are in scope here because init needs them.
- Data loaded after the init sequence (save files, combat-time assets, etc.).
- Changes to the LBX loading infrastructure itself.
- HeMoM headless concerns (no-video, no-input, no-sound stubs).

## Init Sequence Data Inventory

Everything loaded by `ReMoM_Init_Engine()`, in call order.  See
[STU-Init-Sequence.md](../doc/STU-Init-Sequence.md) for the full trace.

### Config Files

| File | Struct | Size | Current Fallback |
|------|--------|------|------------------|
| CONFIG.MOM | `s_CONFIG_MOM_18` (20 B) | 20 B | All-zeros if file missing ([ReMoM_Init.c:66-78](../src/ReMoM_Init.c#L66)) |
| MAGIC.SET | `s_MAGIC_SET` (466 B) | 466 B | `Set_Default_Game_Settings()` if file missing/wrong-sized ([Settings.c:575](../MoM/src/Settings.c#L575)) |

Both already have working defaults.

### Tabular Data (LBX)

| File | Entry | Struct/Format | Records | Record Size | Total |
|------|-------|---------------|---------|-------------|-------|
| TERRSTAT.LBX | [0] | raw bytes (terrain stats) | 770 | 6 B | 4,620 B |
| TERRSTAT.LBX | [1] | raw bytes (unrest table) | 1 | 196 B | 196 B |
| SPELLDAT.LBX | [0] | `s_SPELL_DATA` | 215 | 36 B | 7,740 B |
| BUILDDAT.LBX | [0] | `s_BLDG_DATA` | 36 | 52 B | 1,872 B |

### Font / Palette Data (FONTS.LBX)

| Entry | Contents | Size |
|-------|----------|------|
| [0] | Font glyph/style data (`s_FONT_HEADER` + glyph bitmaps) | ~2 KB header + glyph data |
| [1] | Border/outline data | TBD (measure during extraction) |
| [2] | Palette (768 B) + font_colors (256 B) + mouse_palette + remap_colors | ~5,568 B |

### Graphics (LBX)

| File | Entries | Contents |
|------|---------|----------|
| MAINSCRN.LBX | [0, 5] | Main menu top/bottom |
| VORTEX.LBX | [1-5] | Menu option icons |
| WIZARDS.LBX | [0-13] | Wizard portraits (14 entries) |
| TERRAIN.LBX | [0-2] | Terrain tileset |
| MAPBACK.LBX | [0-93] | Overlays, roads, nodes, mineral sites |
| MAIN.LBX | [0-64] | Main game screen UI, buttons, icons |
| BACKGRND.LBX | [3, 4] | Combat bottom, mirror background |

### Audio (LBX)

| File | Entries | Contents |
|------|---------|----------|
| SOUNDFX.LBX | [0, 2] | Button click sounds |

## Design

### Approach: Extraction Tool Generates C Source, Init Code Uses It Directly

**Step 1 — Build an extraction tool** that reads the original game files and emits C source
files containing the raw data as byte arrays.

**Step 2 — Commit the generated files** alongside the rest of the source.  They become the
canonical copy of the data.

**Step 3 — Replace the `LBX_Load` / `fread` calls** in the init sequence with direct
references to the embedded arrays.  The file-loading code goes away; it doesn't become
a fallback or an alternative — it is replaced.

The extraction tool remains available to regenerate the files from the original LBX archives
for verification purposes, but it is not part of the normal build.

### File Organisation

```
MoX/src/
    Init_Data/                <-- new directory for embedded data
        INIT_DATA.h           <-- master header, all extern declarations
        TERRSTAT_LBX.c        <-- terrain stats [0] + unrest table [1]
        SPELLDAT_LBX.c        <-- spell data [0]
        BUILDDAT_LBX.c        <-- building data [0]
        FONTS_LBX.c           <-- FONTS.LBX sub-files [0], [1], [2]
        MAINSCRN_LBX.c        <-- MAINSCRN.LBX [0, 5]
        VORTEX_LBX.c          <-- VORTEX.LBX [1-5]
        WIZARDS_LBX.c         <-- WIZARDS.LBX [0-13]
        TERRAIN_LBX.c         <-- TERRAIN.LBX [0-2]
        MAPBACK_LBX.c         <-- MAPBACK.LBX [0-93]
        MAIN_LBX.c            <-- MAIN.LBX [0-64]
        BACKGRND_LBX.c        <-- BACKGRND.LBX [3, 4]
        SOUNDFX_LBX.c         <-- SOUNDFX.LBX [0, 2]
```

One file per LBX archive keeps things navigable.  Each file is generated, but once
committed it's just source code like anything else.

### CONFIG.MOM

Do nothing, already optional.

### MAGIC.SET

Do nothing, already optional.

### Tabular Data (TERRSTAT, SPELLDAT, BUILDDAT)

The extraction tool reads each LBX sub-file via `LBX_Load_Data` and emits a C file like:

```c
const uint8_t INIT_DATA_TERRSTAT_0[] = {
    0x01, 0x00, 0x03, 0x00, 0x00, 0x00,  /* record 0 */
    ...
};
const int INIT_DATA_TERRSTAT_0_SIZE = 4620;
```

In the init code, replace `LBX_Load_Data(terrstat_lbx_file, 0, 0, 770, 6)` with a
`memcpy` from `INIT_DATA_TERRSTAT_0` into the allocated buffer (or point directly if the
data is read-only after init).

**Mutability question:** Do any code paths write through `spell_data_table`,
`bldg_data_table`, or `terrain_stats_table` after init?  If yes, `memcpy` into a mutable
allocation.  If no, point directly at the `const` array.

**Double-load question:** `Load_SPELLDAT()` is called twice — once at
[ReMoM_Init.c:299](../src/ReMoM_Init.c#L299) and again inside `Load_WZD_Resources()` at
[LOADER.c:216](../MoM/src/LOADER.c#L216).  The source notes this as a possible OGBUG
(`¿ BUGBUG  overwriting the pointer to a previous allocation (8k wasted) ?`).
Investigate and resolve during implementation.

### FONTS.LBX

Sub-files [0], [1], [2] are dumped as raw byte arrays.  The init code currently does:

```c
font_style_data = LBX_Load(font_file, 0);      // sub-file 0
border_style_data = LBX_Load(font_file, 1);     // sub-file 1
palette_data = LBX_Reload(font_name, entry+2, palette_block);  // sub-file 2
```

Replace with `memcpy` from the embedded arrays into the allocated buffers, then run the
existing pointer-setup code unchanged (`font_header = (struct s_FONT_HEADER *)font_style_data`,
`font_colors = &palette_data[768]`, etc.).

### Graphics and Audio

Same approach as tabular data: extraction tool dumps each sub-file as a byte array,
init code `memcpy`s into the allocated segment instead of calling `LBX_Load`.

The segment allocation code (`Allocate_Space`, `Near_Allocate_First`, etc.) still runs —
it provides the mutable buffers the rendering code expects.  The embedded data just
replaces the file read as the source of the bytes.

## Implementation To-Do List

### Phase 1: Extraction Tool

- [ ] **1.1** Add `Emit_C_Byte_Array()` to [STU_XTRC.c](../STU/src/STU_XTRC.c) — takes a
      byte buffer, size, symbol name, and output file path; writes a `.c` file with
      `const uint8_t name[] = { 0xNN, ... };` and `const int name_SIZE = N;`
- [ ] **1.2** Add `Extract_LBX_Subfile_To_C()` — loads one LBX sub-file via `LBX_Load`,
      calls `Emit_C_Byte_Array()`
- [ ] **1.3** Add `Extract_All_Init_Data()` — calls `Extract_LBX_Subfile_To_C()` for every
      entry in the init inventory (all files/entries from the table above)
- [ ] **1.4** Wire `Extract_All_Init_Data()` into a callable path (HeMoM command or
      standalone tool in `tools/`)
- [ ] **1.5** Run the extraction tool against the original game files
- [ ] **1.6** Create `MoM/src/Init_Data/` directory
- [ ] **1.7** Move generated files into `Init_Data/`, create `INIT_DATA.h` with all externs
- [ ] **1.8** Commit the generated files

### Phase 2: Config Defaults (CONFIG.MOM + MAGIC.SET)

- [ ] **2.1** Build and verify init completes without CONFIG.MOM and MAGIC.SET

### Phase 3: Tabular Data (TERRSTAT, SPELLDAT, BUILDDAT)

- [ ] **3.1** Audit mutability: grep for writes through `terrain_stats_table`,
      `spell_data_table`, `bldg_data_table` after init — determine if `memcpy` or
      direct pointer is needed
- [ ] **3.2** Replace `Load_TERRSTAT()` — `memcpy` from `INIT_DATA_TERRSTAT_0`
- [ ] **3.3** Replace `Load_SPELLDAT()` — `memcpy` from `INIT_DATA_SPELLDAT_0`
- [ ] **3.4** Resolve the double `Load_SPELLDAT()` call — collapse to one if safe
- [ ] **3.5** Replace `Load_BUILDDAT()` — `memcpy` from `INIT_DATA_BUILDDAT_0`
- [ ] **3.6** Replace TERRSTAT.LBX [1] load in `Terrain_Init()` (unrest table)
- [ ] **3.7** Build and verify: data matches LBX-loaded values byte-for-byte

### Phase 4: FONTS.LBX (raw data for init)

- [ ] **4.1** Replace `LBX_Load(font_file, 0)` in `Load_Font_File()` — `memcpy` from
      `INIT_DATA_FONTS_0` into allocated `font_style_data` buffer
- [ ] **4.2** Replace `LBX_Load(font_file, 1)` — same for `border_style_data`
- [ ] **4.3** Replace `LBX_Reload(font_name, entry+2, palette_block)` in `Load_Palette()` —
      `memcpy` from `INIT_DATA_FONTS_2` into `palette_block`
- [ ] **4.4** Verify pointer setup code still works unchanged (`font_header`, `font_colors`,
      `mouse_palette`, `remap_colors`)
- [ ] **4.5** Build and verify: text renders identically

### Phase 5: Graphics

- [ ] **5.1** Replace `Load_MGC_Resources()` — `memcpy` from embedded arrays for
      MAINSCRN.LBX [0, 5], VORTEX.LBX [1-5], WIZARDS.LBX [0-13]
- [ ] **5.2** Replace graphics loads in `Terrain_Init()` — TERRAIN.LBX [0-2],
      MAPBACK.LBX [0-93]
- [ ] **5.3** Replace `Main_Screen_Load_Pictures()` — MAIN.LBX [0-64], BACKGRND.LBX [4]
- [ ] **5.4** Replace `Load_Combat_Background_Bottom()` — BACKGRND.LBX [3]
- [ ] **5.5** Build and verify: graphics display correctly

### Phase 6: Audio

- [ ] **6.1** Replace `Load_Button_Sounds()` — SOUNDFX.LBX [0, 2]
- [ ] **6.2** Build and verify: button sounds play correctly

### Phase 7: Cleanup

- [ ] **7.1** Wire `Init_Data/` files into `MoM/src/CMakeLists.txt`
- [ ] **7.2** Remove dead file-loading code paths (the `LBX_Load` calls that were replaced)
- [ ] **7.3** Update [STU-Init-Sequence.md](../doc/STU-Init-Sequence.md) to reflect the new
      embedded-data init flow
- [ ] **7.4** Full build and regression test
- [ ] **7.5** Verify the extraction tool can regenerate all `INIT_DATA_*.c` files from the
      original LBX files and produce byte-identical output

## Risks and Open Questions

1. **Mutability:** If any code writes through the data table pointers after init, we need
   `memcpy` into mutable allocations rather than pointing at `const` arrays.  Phase 3.1
   audits this.

2. **Pointer semantics:** The existing globals use `SAMB_ptr` (segment-aware pointers from
   the DOS era).  Pointing these at `const uint8_t[]` arrays requires clean casts on all
   target platforms.

3. **LBX_Load side effects:** `LBX_Load` may do allocation, header parsing, or
   decompression beyond just reading bytes.  The extraction tool must capture the
   *post-processing* output, not the raw LBX file bytes.  If `LBX_Load` decompresses,
   the embedded data is the decompressed form.

4. **Binary size:** All of this data combined is well under 1 MB.  Not a concern.

5. **Copyright:** The embedded data is from the original MoM game files.  Same legal
   posture as the existing reverse-engineered structs and constants already in the
   codebase.

## Success Criteria

- ReMoM builds and runs through init with no original game files on disk.
- All screens, text, sprites, and sounds display/play identically to the file-loaded path.
- The extraction tool can regenerate the embedded data from the original LBX files and
  produce byte-identical output.

## Cross-References

- Init sequence trace: [STU-Init-Sequence.md](../doc/STU-Init-Sequence.md)
- STU_INIT module: [STU_INIT.c](../STU/src/STU_INIT.c), [STU_INIT.h](../STU/src/STU_INIT.h)
- STU_XTRC module: [STU_XTRC.c](../STU/src/STU_XTRC.c)
- Font header struct: [Fonts.h:83](../MoX/src/Fonts.h#L83)
- Spell data struct: [MOM_DAT.h:944](../MoX/src/MOM_DAT.h#L944)
- Building data struct: [MOM_DAT.h:1410](../MoX/src/MOM_DAT.h#L1410)
- Config struct: [CFG.h:26](../MoX/src/CFG.h#L26)
- Settings struct: [MOX_SET.h:17](../MoX/src/MOX_SET.h#L17)
