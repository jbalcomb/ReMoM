# PLAN — Graphics Upscaling

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-24
**Companion:** `PRD-Graphics-Upscale.md` (requirements), `BRA-Graphics-Upscale.md` (decision framing), this file (survey + build-out order)

---

## Goal

Make ReMoM present sharper than 320×200 without ceasing to look like Master of Magic. Get there by building the asset library **first** (so the upscaling architecture is chosen from evidence, not guessed), then deciding the architecture, then implementing it. This plan details Phase 1 concretely and sketches the later phases as options gated on the Phase 1 findings and the BRA decision.

## What already exists

### The render path (the whole current "upscale")

- The engine draws into a **320×200, 8-bit indexed** surface (`platform/sdl2/sdl2_Init.c:106`), palette-expands it to ARGB8888 (`:113`), uploads to a **native-size** 320×200 texture (`:116`), and stretches that to the window with a single `SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, NULL)` (`platform/sdl2/sdl2_Video.c:131`). The stretch filter is whatever the renderer defaults to — this one call is the entire upscaling story today.
- Screen dimensions are fixed constants: `PLATFORM_SCREEN_WIDTH 320` / `PLATFORM_SCREEN_HEIGHT 200` (`platform/include/Platform.h:92-93`), with `XMAX`/`YMAX` derived. These are threaded through every backend and all of `MoM/src/`.
- SDL3 (`platform/sdl3/`) and Win32 (`platform/win32/win_PFL.cpp`) backends have their own present steps — the output-frame path (Family A) would have to change each one, not just SDL2.

### The sprite format and its decoder

- A graphic entry begins `int16 width @0; int16 height @2;` then encoded palette-indexed pixel data with transparency and, for some entries, multiple animation frames.
- LBX graphic entries are FLIC animations. The authoritative decoder is **`FLIC_Draw(x, y, picture)`**: it loads the entry's palette when it has one (`FLIC_Load_Palette()`), then decodes the current frame with **`FLIC_Draw_Frame()`** — column-major RLE, skipped pixels stay transparent — or `FLIC_Remap_Draw_Frame()` when the remap flag is set (`MoX/src/FLIC_Draw.c`). `Draw_Picture()` / `Draw_Picture_ASM()` are a separate path that draws already-decoded bitmaps. `Cache_Graphics_*` (`MoM/src/LOADER.c`) are the callers that load archives into memory for drawing.
- Runtime recolouring is applied at draw time via palette remap primitives (`Gradient_Fill`, `Tint_Fill`, `remap_block` in `MoX/src/Graphics.h:95,130`) and `colortbl`/`remap` code across ~20 `MoM/src/` files — the sprite bytes are indices, not colours.

### The container reader and extraction tooling

- **`tools/lbx_extract.c`** is a standalone C reader of the LBX container: `0xFEAD` magic, `num_entries`, the `uint32 offset[num_entries+1]` table, and the 32-byte name table. It handles the `0xDEAF` sound wrapper (XMI/VOC) and dumps everything else — **including all graphics** — as opaque `.BIN` (`:426`). It is the container half of the Phase 1 tool; the pixel decoder is what is missing.
- **`MoX/src/LBX_Load.h/.c`** is the in-engine loader (`LBX_Load`, `LBX_Load_Entry`, …) with the same container constants (`LBX_MAGSIG 0xFEAD`, `SZ_LBX_HDR_B 512`).
- **`STU/src/lbx_manifest.c`** and `STU_GRAF` already carry per-file hash/compat machinery for the v1.31 data set — useful for tying the catalogue to known-good assets, and a consumer that benefits from a catalogue.

### The assets

- 103 `.LBX` archives in `assets/` (`ls assets/*.LBX`). Graphics: `FIGURES1..16`, `UNITS1..2`, `MONSTER`, `HALOFAM`, `TERRAIN`/`DESERT`/`TUNDRA`, `PORTRAIT`/`MOODWIZ`/`LILWIZ`/`WIZARDS`, `CITYSCAP`/`CITYWALL`, the `CMB*` combat tilesets, `SPECFX`/`SPECIAL`/`SPECIAL2`, `ITEMS`/`ITEMISC`, `BOOK`/`SCROLL`/`SPELLSCR`, `MAIN`/`MAINSCRN`/`BACKGRND`/`NEWGAME` UI. Data (`SPELLDAT`, `ITEMDATA`, `TERRSTAT`, …), fonts (`FONTS`), and sound (`MUSIC`, `SOUNDFX`, `SNDDRV`, `NEWSOUND`, `INTRO*`) are **not** in scope — the catalogue must distinguish them.

## Decisions

1. **Library before tool.** The architecture (BRA §4) and upscaler (BRA §5) choices are deferred until the library exists. Phase 1 builds no upscaler and makes no change to *how* the game renders; its only engine touch is an optional read-only capture hook (decision 6).
2. **One format reader.** The exporter calls the engine's `FLIC_Draw()` rather than re-deriving the format, so the project does not carry two divergent sprite readers (the failure `lbx_extract` half-fell into for images).
3. **Index-preserving export is mandatory; RGB-under-palette is additive.** Protects runtime recolouring (BRA §2.3).
4. **The catalogue is a first-class deliverable**, structured to answer BRA §9 quantitatively.
5. **Family A (output-frame) is the first upscaling option to evaluate** once the decision phase opens — smallest risk surface, reversible, ships independently. Family B is not committed without evidence from the library that Family A is insufficient.
6. **Recolouring is preserved by mask/region separation (BRA §11, chosen direction).** Banner/team colour is a fixed reserved-index range (214–218, `MoM/src/UnitView.c:2683`) exported as a base+ramp-position mask; general tints (256-entry LUT, `MoX/src/FLIC_Draw.c:261`) become an output-stage shader pass, not a per-sprite bake. Phase 1 captures the data this needs (the mask, and the recolour classification), including an optional runtime hook on `FLIC_Draw()` and `Draw_Picture()` (and their remap variants). The mask approach composes with any Family, so it does not pre-empt decision 5.

## Phase 1 — the asset library

### 1a. Tracer bullet — decode one archive to PNG

Prove the whole pipeline on the least-ambiguous content before scaling out.

- Take the existing `lbx_extract` container reader; add a pixel decoder for **one** simple, single-frame archive (a full-screen UI/background like `MAINSCRN.LBX` or a portrait from `PORTRAIT.LBX` — fixed size, no animation, minimal recolouring).
- Decide the reuse mechanism here: lift `FLIC_Draw_Frame()`'s decode into a standalone tool, or link `momlib`/`MOX` and render into an off-screen 320×200 buffer via `Platform_Headless` (`platform/headless/`)? Pick the one that keeps a **single** authoritative decoder.
- Output: an index-map PNG (or index array + palette) **and** an RGB PNG under the entry's palette.
- **Validation:** compare the RGB PNG against a screenshot of the same asset in-game; assert exported dimensions equal the `w@0 / h@2` header. This is the go/no-go for the approach.

**1a outcome (2026-10-29).** `tools/lbx_image_export.c` (CMake-only, like every other tool) links `STU MOX momlib Platform_Headless` and calls the engine's `FLIC_Draw()` into its own 320×200 page — the reuse mechanism is "link the engine", and there is one FLIC reader. Built and run in WSL (`clang-nosound-debug`, no warnings); the MSVC presets are blocked while the Windows SDK is missing.

- **Entry:** LOAD.LBX entry 0 — 320×200, one frame, its own full 0..255 palette, no remap flag. Exported image is the Load/Save screen with correct colours.
- **Transparency** is recovered by drawing twice onto pages pre-filled `0x00` and `0xFF`: pixels that differ were never written. A write outside the declared box is counted as a decode error (none seen).
- **PNG check:** an independent stdlib-`python3` reader verified every chunk CRC and the zlib stream, and that the indexed (+PLTE/tRNS) and RGBA files agree on every pixel, for LOAD 0/1, WIZARDS 0, BACKGRND 3, MAINSCRN 0/5.
- **Refusals work:** empty entries (BACKGRND 1, 0 bytes) and remap-flagged entries exit non-zero with a reason; multi-frame entries export frame 0 and say so.
- **Not yet done — in-game oracle.** `assets/capture_load_screen.hms` drives `ReMoM --capture` to the Load screen, but no runnable binary could execute it: the MSVC Debug exe needs `ucrtbased.dll` (gone with the SDK), the 2026-08-09 Release exe never registered the `L` hotkey from its saves, and WSL has no display (no WSLg, no Xvfb). Re-run once MSVC builds again.

Findings that feed 1b/1c:

- **Some entries take their palette from a sibling entry, not from FONTS.LBX.** MAINSCRN 5 has no palette and is drawn after MAINSCRN 0 (full 0..255 palette); under the main game palette it is garbage, under `-P MAINSCRN:0` it is the main-menu castle. The catalogue needs a "palette source" column, and the right source is a property of the *screen that draws the entry*, discoverable from the `FLIC_Draw` / `Load_Palette` call order in `MoM/src/`.
- **Partial palettes are common.** WIZARDS portraits carry only 224..255 and BACKGRND 3 only 224..243; they are correct over base palette 0.
- **Full-screen backgrounds have deliberate holes.** LOAD 0 has 2,259 never-written pixels inside (24,43)–(291,185), where the Load screen draws slot fills and buttons; MAINSCRN 5 has 83. These are cut-outs for overlays, so "transparent" in a background is meaningful, not a decode artefact.
- **Index 0 was never drawn opaque** in the sampled entries, so `ST_TRANSPARENT` (0) + tRNS represents transparency losslessly in the indexed export so far. The tool reports the conflict if an entry ever breaks that.

### 1b. Handle the hard cases — frames, transparency, palettes

- Multi-frame entries (unit walk cycles in `FIGURES*`/`UNITS*`): export each frame as a numbered PNG **and** a horizontal sprite sheet; record frame count and shared origin.
- Transparency: preserve the transparent index as PNG alpha in the RGB export; keep it as its index value in the index-preserving export.
- Palettes: capture which palette an entry is drawn under (embedded vs. a shared game palette). Where an entry is recoloured at runtime, note it — the catalogue flag in 1c depends on this.
- **Banner/team-colour mask (BRA §11).** For entries that use the reserved banner indices 214–218, emit a base image with those indices neutralised plus a per-pixel ramp-position (0–4) mask. Reference the `COL_Banners` ramp table (`MoM/src/UnitView.c:2683`) for validation. Validation: base+mask re-tinted by a player ramp must reproduce the in-game banner colouring at native resolution (the go/no-go for the mask approach, before any upscaling is involved).

### 1b-hook. Optional runtime remap-capture

An optional, read-only, `/* CLAUDE */`-marked, env/debug-gated hook on `FLIC_Draw()` / `FLIC_Remap_Draw_Frame()` (LBX entries) and `Draw_Picture()` / `Remap_Draw_Picture_ASM()` (decoded bitmaps) that logs, per draw, the archive+entry and the active 256-entry `picture_remap_color_list` LUT. This turns the catalogue's recolour columns from heuristic to authoritative and separates banner-recoloured entries from globally-tinted ones. It changes nothing about what is drawn; it is the single permitted engine touch in Phase 1 (Decisions 1, 6).

### 1c. The catalogue

One row per entry across all 103 archives: `archive, index, width, height, frame_count, class (unit|terrain|portrait|ui|fullscreen|effect|item|font|data|sound|unknown), recoloured_runtime (bool), recolour_kind (none|banner|tint|both), uses_reserved_banner_indices (bool), tessellates (bool), notes`. Emit as CSV/JSON. The `class`, `recolour_*`, and `tessellates` columns are what turn BRA §9's open questions into numbers — in particular, `recolour_kind` quantifies exactly how much art needs a banner mask vs. a global tint pass (BRA §11).

### 1d. The gallery

Generate a static HTML index (self-contained, embeddable images) showing every entry at 1×/2×/4× with its catalogue row, grouped by class. This is what the maintainer actually looks at to (a) confirm the decode is right across the set and (b) pick worst-case content (dithered gradients, tiny UI text, seamed terrain) for later A/B testing.

### 1e. Wire-up

- New tool source added to **both** the CMake tree and `Makefile.am`.
- A single command (script or tool flag) regenerates the entire library + catalogue + gallery from `assets/`, so a decoder fix is one re-run.

## Decision point (between Phase 1 and Phase 2)

With the library in hand, record the architecture decision in `BRA-Graphics-Upscale.md` (flip its status from *exploring* to *decided*): Family A / B / C, and if asset-level, which upscaler class, chosen by A/B-ing candidates on the worst-case content the gallery surfaced. The open questions BRA §9 Q1–Q5 are answered here.

## Later phases (options, gated on the decision)

### Option A — Output-frame upscaling (if chosen)

1. Replace the raw `SDL_RenderCopy` stretch with integer scaling + an edge-directed pixel-art shader (xBRZ/ScaleFX class) in a fragment shader. Tracer bullet: one backend (SDL2), one shader, toggle on/off, screenshot A/B against the current stretch.
2. Port to the SDL3 and Win32 present paths. Optional scanline/CRT treatment as a user setting.
3. This ships without touching any asset or game coordinate. Reversible.

### Option B — Asset replacement at higher resolution (if chosen)

1. Bake upscaled sprites offline from the Phase 1 exports, using the chosen scaler. Recolouring is preserved by the mask/region approach (BRA §11): upscale the base RGB and its banner ramp-position mask together (mask kept crisp — nearest/vector — so its edge tracks the base), and apply general tints as an output-stage LUT pass rather than baking tint variants. Seam-aware for terrain; frame-coherent for animations.
2. Rework the rendering model to a higher internal resolution — internal surface size, coordinate math, clip windows, hit-testing, layout constants — **or** a compositing hybrid (hi-res sprites over an upscaled low-res background at aligned coordinates). This is the large, invasive body of work; scope it in its own PLAN once committed.
3. Original-game-code edit rules apply throughout (`/* CLAUDE */`, no deletion of faithful reconstruction).

### Option C — Hybrid

Option A now (shippable), plus Option B for a curated high-value subset (portraits, title/UI, wizard gems) drawn at fixed positions where compositing needs no coordinate rewrite.

## Build-out order (committed portion)

1. **Phase 1a tracer bullet** — one archive, one entry class, decode to PNG, validate against the running game. Proves the decoder-reuse approach with the least ambiguity.
2. **Phase 1b/1c** — frames/transparency/palette handling, the banner base+mask export with its re-tint validation, the optional runtime remap-capture hook, then the catalogue over all 103 archives.
3. **Phase 1d/1e** — gallery + one-command regeneration + build-system wire-up.
4. **Decision point** — architecture recorded in the BRA.
5. Later phases scoped in their own follow-up plan once the Family is chosen. Family A is the first to evaluate.

## Risks / unknowns to resolve early

- **Decoder reuse mechanism** (standalone lift vs. link-the-engine) — resolved in 1a; getting it wrong risks a second divergent format reader.
- **Compression/encoding variants across archives** — some archives (combat tilesets, effects) may use encoding the tracer-bullet entry does not exercise; the dimensions-invariant check across the full set surfaces these.
- **Recolour detection** — deciding `recoloured_runtime` / `recolour_kind` from the archive alone is heuristic; the optional runtime remap-capture hook (Phase 1b-hook) makes it authoritative by logging which entries actually pass through the banner-index or LUT paths. Resolve early whether the hook is built now or the catalogue ships with a heuristic first pass.
- **Reserved-index ambiguity** — confirm no non-banner sprite legitimately uses indices 214–218 for non-banner pixels, which would false-positive the banner mask (BRA §11.5). The runtime hook plus a static scan settles it.
- **Fonts and FLIC** — explicitly deferred; the catalogue tags them so they are visibly out of scope rather than silently skipped.
