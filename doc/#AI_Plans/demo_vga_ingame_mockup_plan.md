# Plan: `demo_vga` — mock up the VGA effect demos as they'd look in-game

## Goal
Replace the synthetic bar patterns in the stand-alone `demo_vga` harness with **real MoM
screens**, so each effect reads the way it does in the actual game:
- the **dissolve** transitions between two real full-screen backgrounds,
- the **earthquake** shakes a real map/overland screen.

This builds on the existing stand-alone demo harness (`demo/`), which today boots asset-free
and uses synthetic patterns. No game source is modified; the demo links the engine libraries
and calls their public API.

## Decisions (locked)
- **Boot path**: trimmed asset-boot. A new `Demo_Boot_Assets()` calls only the low-level public
  API the demo needs (memory pool, EMS, video, palette/font, LBX), NOT the game's full
  `ReMoM_Init_Engine()`. Keeps the demo lean and independent of the game's init.
- **Assets**: the original MoM LBX files are staged into `bin/Debug` (same staging ReMoMber
  uses), so `LBX_Load()` finds them at runtime. Phase 1 verifies this before relying on it.
- **Showcase screens**: dissolve between MAINSCR (main menu) and a magic/spell screen;
  earthquake shakes the main/overland screen.
- **Safety net**: if an LBX load fails, fall back to the current synthetic pattern and log it,
  so the headless smoke-run and the no-assets case keep working exactly as today.

## Anchors (verified in the codebase)
- `LBX_Load(file, entry)` → loads an LBX entry into allocated memory (returns a segment ptr).
  Reference sequences: `Main_Menu_Load_Pictures()` (MoM/src/MainMenu.c), `Draw_Logos()`
  (MoM/src/INTRO.c:108).
- `Draw_Picture(x, y, seg)` → blits an LBX image onto the current draw page (used throughout
  MainScr/CityScr/Combat).
- `Load_Palette(n, ...)` → loads a game palette: 0 = EMPERATO (main game), 2 = ARCANUS
  (magic/castle view), etc.
- `LBX_Load` needs more than today's asset-free boot: a memory pool (`Allocate_Data_Space`),
  EMS/file access (`EMS_Startup`), and the font/palette system.

## Phases (tracer-bullet)

### Phase 1 — asset pipeline tracer (no effect yet)
1. Add `Demo_Boot_Assets()` and `Demo_Load_Screen(lbx, entry, page)` to the harness.
   Determine and **verify** the minimal init sequence (the one genuine unknown, since we bypass
   `ReMoM_Init_Engine`): confirm what `Init_Drivers` / `Init_Video_Drivers` / `LBX_Load`
   actually require.
2. New effect `screen`: `LBX_Load` → `Draw_Picture(0,0,seg)` into a page → `Load_Palette` →
   `Demo_Present`, then hold.
3. Asset-missing detection → synthetic fallback + log.
4. **Gate**: windowed run shows a real MoM screen; headless smoke-run still clean. Do not
   proceed until the pipeline is proven.

### Phase 2 — real screens into the effects
- `dissolve`: MAINSCR <-> magic screen, full-screen, using the loaded backgrounds.
- `earthquake`: real main/overland background loaded into frame F3, then shaken.

### Phase 3 — authenticity polish
- Correct per-screen palette (EMPERATO `Load_Palette(0,...)` vs ARCANUS `Load_Palette(2,...)`).
- Screen choices matched to each effect's real casting context.
- CLI source-screen override: `demo_vga earthquake <LBX> <entry>`.

### Phase 4 — optional
- Frame-capture / record mode (reuse `capture.c` / replay) so demos become GIFs/screenshots
  for docs.
- `gallery` mode cycling through all effects.

## Risk being watched
The trimmed boot is the one real unknown — `LBX_Load` may transitively need more init than the
short sequence above (EMS handles, a default font, etc.). Phase 1 exists to surface that early
and cheaply. If the trimmed boot turns out to need most of `ReMoM_Init_Engine` anyway, that is
the signal to switch to reusing `ReMoM_Init_Engine()` (Option A) rather than piling on ad-hoc
init calls — flag it instead of guessing.

## RESOLVED (Phase 1) — the trimmed boot recipe
Risk resolved favorably. The trimmed asset-boot is just two calls:
  1. `Startup_Platform()`
  2. `Init_Drivers(1, 2, "FONTS.LBX", 0,-1,-1,-1, 0,-1,-1,-1)`  then `Reset_Window()`
`Init_Drivers` (MoM/src/Init.c) is the engine's own driver init: `EMS_Startup` ->
`Init_Video_Drivers` (allocates the four video pages) -> `Load_Font_File` (FONTS.LBX; allocates
`current_palette`, `palette_block`, `remap_color_palettes`, fonts) -> `Init_Mouse_Keyboard` ->
`Set_Page_Off`.  It does NOT load sound, scan saves, or load game resources.  `Allocate_Space`
is a plain `malloc` wrapper (no heap init needed).

Two gotchas found and fixed:
- Full-screen backgrounds must be drawn with **`FLIC_Draw(0,0,seg)`** (the decompressing blitter
  the game uses, e.g. MagicScr.c `FLIC_Draw(0,0,magic_background_seg)`), NOT `Draw_Picture`
  (raw-sprite copy) — `Draw_Picture` mis-decodes/overruns a compressed background and segfaults.
- Effects that never page-flip (earthquake) never triggered `Platform_Palette_Update`, so their
  palette was never pushed.  `Demo_Present()` now applies the palette every frame.

## Status
- Phase 1 COMPLETE: `Demo_Boot()` (asset-aware), `Demo_Load_Screen(lbx,entry,pal,page)`, `screen` effect.
- Phase 2 COMPLETE: real screens fed into both effects.
  - `dissolve`: dissolves between MAGIC.LBX 0 (magic screen) and MAIN.LBX 0 (overland), both
    palette 0.  Substituted MAIN for the planned MAINSCRN because MAINSCRN is composed of pieces
    (entry 0 is only the top strip), not a single full-screen background.  Both images are decoded
    once into spare pages 2/3; each cycle memcpy's the next into the off page then PageFlip_Dissolve.
  - `earthquake`: shakes MAIN.LBX 0 (overland) loaded once into F3 (page 2) -- the quake's real
    in-game context.
  - Both fall back to synthetic patterns when assets are absent.
  - Builds under /WX; all three effects headless-smoke clean.  Windowed visual not yet eyeballed.
- Verified along the way: full-screen backgrounds are drawn with FLIC_Draw(0,0,seg) (decompressing
  blitter) -- e.g. main_background_seg = LBX_Load("MAIN",0) (LOADER.c), magic_background_seg =
  LBX_Reload("MAGIC",0) (MagicScr.c).
- Next: Phase 3 — per-screen palette authenticity + CLI source-screen override.

## Phase 2/3 refinements (after windowed feedback)
- Pacing: added a guarded `g_pageflip_dissolve_pass_delay` (Video.c/.h; default 0 = original
  instant behavior).  The demo sets it to 90 ms so the 4 grain passes are perceptible.  Confirmed
  good speed in the windowed run.
- Remap colors: `Demo_Load_Screen` now calls `Calculate_Remap_Colors()` after `Load_Palette`
  (the trimmed boot allocated `remap_color_palettes` but never populated it).
- CLI override (Phase 3 brought forward): `demo_vga <effect> [LBX entry [LBX entry]]` overrides
  which screen(s) an effect loads, via Demo_Set_Args / Demo_Arg / Demo_Arg_Int in the harness.
- Screen choice finding: the magic screen (MAGIC.LBX 0) is an OVERLAY-style background — it has
  translucent/shaded remap regions that `Remap_Draw_Picture_ASM` resolves against whatever is
  already on the page.  Drawn standalone over a blank page they become dark "splotches".  Opaque
  full-screen backgrounds (MAIN overland, etc.) render correctly standalone.  Defaults changed:
  screen -> MAIN, dissolve -> MAIN <-> NEWGAME, earthquake -> MAIN.  Use the CLI override to try
  others.

## Phase 3 COMPLETE
- Per-screen palette via CLI: `screen <LBX> <entry> <palette>`, `earthquake <LBX> <entry> <palette>`,
  `dissolve <lbxA> <entryA> <lbxB> <entryB> <palette>` (one shared palette -- a cross-dissolve shows
  both images under a single active palette, so per-screen palettes there would be incoherent).
  Palettes: 0=EMPERATO(main), 1=LOADSAVE, 2=ARCANUS(castle/magic), 4=spell-mastery.
- Builds under /WX; all effects + palette overrides headless-smoke clean.  Windowed visuals still
  to be eyeballed by the user.

## Scope note
This is a manual VISUAL-TEST harness, not a shippable demo.  No capture/record/gallery
product-izing (dropped).  Grow it by adding paths that exercise real engine draw/animation code.

## Summon animation path (IMPLEMENTED — effect_summon.c; visual unverified)
Goal: watch IDK_SummonAnim's animation for a given summon type.
- IDK_SummonAnim (full) needs music + Allocate_Reduced_Map + Full_Draw_Main_Screen + input loop =
  a whole game world.  IDK_SummonAnim_Load also calls Main_Screen_Draw (world).  Too heavy.
- The actual animation is in IDK_SummonAnim_Draw; the only thing that animates is the monster
  rising (frames 0..29), gated on magic_set.spell_animations && _osc_player_idx == HUMAN.
- Approach: harness loads the summon sprites itself (LBX_Load of the SPELLSCR/MONSTER entries that
  IDK_SummonAnim_Load uses), substitutes a MAIN backdrop for Main_Screen_Draw, sets the globals
  (magic_set.spell_animations, _temp_sint_1=_osc_player_idx, _temp_sint_4=_osc_summon_unit_type,
  _osc_anim_ctr), builds the back buffer, then loops the REAL IDK_SummonAnim_Draw at the game's
  Set_Page_Off/Page_Flip cadence.
- Reliable for NEGATIVE special types (hero/item): -2 male hero, -3 female hero, -1 item -- these
  avoid the unit/spell data tables.  Positive creature types need those tables loaded AND rely on
  a MONSTER.LBX figure-index formula the reconstruction flags as uncertain -> deferred.
- CAVEAT: summon visuals are NOT self-verifiable (headless smoke only crash-checks); needs user eyeball.

## Lose animation path (DONE — effect_lose.c; visually confirmed)
`demo_vga lose <wizard 0-13>` calls the REAL Lose_Animation() directly (self-contained, no world,
self-terminating loop).  Harness only allocates _screen_seg and sets _players[0].wizard_id.  User
confirmed it runs at a good pace and looks correct.  (Depends on the FLIC_Draw.c rotate/texture
reconstruction: VGA_WndDrawRotateImg / VGA_RotateRect / VGA_DrawTexture.)

## Win animation path (DONE — effect_win.c; visually confirmed)
`demo_vga win <wizard 0-13>` calls the REAL Win_Animation() directly (like lose).  Setup beyond the
trimmed boot: Allocate_Data_Space() (World_Data + _screen_seg, which Win_Animation needs for
Combat_Cache_Write, _CITIES, and the RedSparkle/image loads) and GUI_String_1 (a char* the game
allocates during conquest-screen setup; Win_Animation_Draw strcpy's the victory text into it, else
NULL deref).
CORRECTION: an earlier note here called CONQUEST.c:877 `Allocate_First_Block(714, World_Data)` an
OGBUG that reads address 714.  That was WRONG -- it was a Gemini param-swap reconstruction error
(714 is the SIZE; signature is Allocate_First_Block(block, size)).  User fixed it to
`Allocate_First_Block(World_Data, 714)`, so the real function is callable and there is NO OGBUG /
no null-page fault.

## Remaining (optional)
- More effects: reconstruct & demo other seg026/027 transitions (CrossSlide, Interleave, Wipe,
  SliceFlip, SquaresFlip...) -- still stubs.
- Hygiene: Release build + safe-to-push gate for the MoX `g_pageflip_dissolve_pass_delay` change;
  autotools (Makefile.am) parity for demo_vga (currently CMake-only).

## Current state (starting point)
- `MoX/src/Video.c` `PageFlip_Dissolve()` — Mode-X mosaic dissolve ported to ReMoM's linear
  framebuffer; builds under `/WX`.
- `demo/` stand-alone target `demo_vga`:
  - `demo_main.c` (dispatcher), `demo_harness.{h,c}` (boot + helpers), `demo_effects.h`
    (registry), `effect_dissolve.c`, `effect_earthquake.c`.
  - Builds, links, headless smoke-runs clean for both effects. Visuals not yet eyeballed.
- Effects currently use synthetic bar patterns + a synthetic palette; no assets required.
- Not wired into autotools (`Makefile.am`) — CMake-only, by intent (optional dev harness).
