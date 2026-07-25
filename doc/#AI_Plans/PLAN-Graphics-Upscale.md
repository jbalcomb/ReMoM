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
- The authoritative decoder is **`Draw_Picture(x, y, pict_seg)`** → **`Draw_Picture_ASM(...)`** (`MoX/src/FLIC_Draw.c:1646-1654`); **`Draw_Picture_Windowed`** (`:1661+`) shows the header read and the clip-window handling. `Cache_Graphics_*` (`MoM/src/LOADER.c:1657+`) are the callers that load archives into memory for drawing.
- Runtime recolouring is applied at draw time via palette remap primitives (`Gradient_Fill`, `Tint_Fill`, `remap_block` in `MoX/src/Graphics.h:95,130`) and `colortbl`/`remap` code across ~20 `MoM/src/` files — the sprite bytes are indices, not colours.

### The container reader and extraction tooling

- **`tools/lbx_extract.c`** is a standalone C reader of the LBX container: `0xFEAD` magic, `num_entries`, the `uint32 offset[num_entries+1]` table, and the 32-byte name table. It handles the `0xDEAF` sound wrapper (XMI/VOC) and dumps everything else — **including all graphics** — as opaque `.BIN` (`:426`). It is the container half of the Phase 1 tool; the pixel decoder is what is missing.
- **`MoX/src/LBX_Load.h/.c`** is the in-engine loader (`LBX_Load`, `LBX_Load_Entry`, …) with the same container constants (`LBX_MAGSIG 0xFEAD`, `SZ_LBX_HDR_B 512`).
- **`STU/src/lbx_manifest.c`** and `STU_GRAF` already carry per-file hash/compat machinery for the v1.31 data set — useful for tying the catalogue to known-good assets, and a consumer that benefits from a catalogue.

### The assets

- 103 `.LBX` archives in `assets/` (`ls assets/*.LBX`). Graphics: `FIGURES1..16`, `UNITS1..2`, `MONSTER`, `HALOFAM`, `TERRAIN`/`DESERT`/`TUNDRA`, `PORTRAIT`/`MOODWIZ`/`LILWIZ`/`WIZARDS`, `CITYSCAP`/`CITYWALL`, the `CMB*` combat tilesets, `SPECFX`/`SPECIAL`/`SPECIAL2`, `ITEMS`/`ITEMISC`, `BOOK`/`SCROLL`/`SPELLSCR`, `MAIN`/`MAINSCRN`/`BACKGRND`/`NEWGAME` UI. Data (`SPELLDAT`, `ITEMDATA`, `TERRSTAT`, …), fonts (`FONTS`), and sound (`MUSIC`, `SOUNDFX`, `SNDDRV`, `NEWSOUND`, `INTRO*`) are **not** in scope — the catalogue must distinguish them.

## Decisions

1. **Library before tool.** The architecture (BRA §4) and upscaler (BRA §5) choices are deferred until the library exists. Phase 1 builds no upscaler and changes no engine code.
2. **One format reader.** The exporter reuses `Draw_Picture_ASM`'s decode rather than re-deriving it, so the project does not carry two divergent sprite readers (the failure `lbx_extract` half-fell into for images).
3. **Index-preserving export is mandatory; RGB-under-palette is additive.** Protects runtime recolouring (BRA §2.3).
4. **The catalogue is a first-class deliverable**, structured to answer BRA §9 quantitatively.
5. **Family A (output-frame) is the first upscaling option to evaluate** once the decision phase opens — smallest risk surface, reversible, ships independently. Family B is not committed without evidence from the library that Family A is insufficient.

## Phase 1 — the asset library

### 1a. Tracer bullet — decode one archive to PNG

Prove the whole pipeline on the least-ambiguous content before scaling out.

- Take the existing `lbx_extract` container reader; add a pixel decoder for **one** simple, single-frame archive (a full-screen UI/background like `MAINSCRN.LBX` or a portrait from `PORTRAIT.LBX` — fixed size, no animation, minimal recolouring).
- Decide the reuse mechanism here: can `Draw_Picture_ASM`'s decode be lifted into a standalone tool, or must the tool link `momlib`/`MOX` and render into an off-screen 320×200 buffer via `Platform_Headless` (`platform/headless/`)? Both are viable; pick the one that keeps a **single** authoritative decoder.
- Output: an index-map PNG (or index array + palette) **and** an RGB PNG under the entry's palette.
- **Validation:** compare the RGB PNG against a screenshot of the same asset in-game; assert exported dimensions equal the `w@0 / h@2` header. This is the go/no-go for the approach.

### 1b. Handle the hard cases — frames, transparency, palettes

- Multi-frame entries (unit walk cycles in `FIGURES*`/`UNITS*`): export each frame as a numbered PNG **and** a horizontal sprite sheet; record frame count and shared origin.
- Transparency: preserve the transparent index as PNG alpha in the RGB export; keep it as its index value in the index-preserving export.
- Palettes: capture which palette an entry is drawn under (embedded vs. a shared game palette). Where an entry is recoloured at runtime, note it — the catalogue flag in 1c depends on this.

### 1c. The catalogue

One row per entry across all 103 archives: `archive, index, width, height, frame_count, class (unit|terrain|portrait|ui|fullscreen|effect|item|font|data|sound|unknown), recoloured_runtime (bool), tessellates (bool), notes`. Emit as CSV/JSON. The `class`, `recoloured_runtime`, and `tessellates` columns are what turn BRA §9's open questions into numbers.

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

1. Bake upscaled sprites offline from the Phase 1 index-preserving exports, using the chosen scaler, preserving the index space (or an explicitly-approved fixed recolour set). Seam-aware for terrain; frame-coherent for animations.
2. Rework the rendering model to a higher internal resolution — internal surface size, coordinate math, clip windows, hit-testing, layout constants — **or** a compositing hybrid (hi-res sprites over an upscaled low-res background at aligned coordinates). This is the large, invasive body of work; scope it in its own PLAN once committed.
3. Original-game-code edit rules apply throughout (`/* CLAUDE */`, no deletion of faithful reconstruction).

### Option C — Hybrid

Option A now (shippable), plus Option B for a curated high-value subset (portraits, title/UI, wizard gems) drawn at fixed positions where compositing needs no coordinate rewrite.

## Build-out order (committed portion)

1. **Phase 1a tracer bullet** — one archive, one entry class, decode to PNG, validate against the running game. Proves the decoder-reuse approach with the least ambiguity.
2. **Phase 1b/1c** — frames/transparency/palette handling, then the catalogue over all 103 archives.
3. **Phase 1d/1e** — gallery + one-command regeneration + build-system wire-up.
4. **Decision point** — architecture recorded in the BRA.
5. Later phases scoped in their own follow-up plan once the Family is chosen. Family A is the first to evaluate.

## Risks / unknowns to resolve early

- **Decoder reuse mechanism** (standalone lift vs. link-the-engine) — resolved in 1a; getting it wrong risks a second divergent format reader.
- **Compression/encoding variants across archives** — some archives (combat tilesets, effects) may use encoding the tracer-bullet entry does not exercise; the dimensions-invariant check across the full set surfaces these.
- **Recolour detection** — deciding `recoloured_runtime` may need cross-referencing the `MoM/src/` draw call sites, not just the archive; the catalogue may start with a conservative heuristic and refine.
- **Fonts and FLIC** — explicitly deferred; the catalogue tags them so they are visibly out of scope rather than silently skipped.
