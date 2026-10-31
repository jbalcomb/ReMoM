# BRA — Graphics Upscaling Architecture Decision

**Status:** In progress — exploring (no decision yet; the decision is deferred until the Phase 1 asset library exists)
**Date:** 2026-07-24
**Author role:** Business Requirements Analyst (decision record)
**Related:** `PRD-Graphics-Upscale.md`, `PLAN-Graphics-Upscale.md`

This document records *how* the graphics-upscaling approach is being decided — the goal, the current-state facts that reshape the effort, the criteria, the option families, and the central tensions. It is deliberately **not** a settled decision. The chosen approach is to build a complete, viewable asset library first (Phase 1) and only then commit to an upscaling architecture, because the right choice depends on facts we do not yet have in front of us (how many distinct sprites, how much runtime recolouring, how coherent the tilesets are).

---

## 1. The ultimate goal

Make ReMoM look substantially sharper than 320×200 without betraying what Master of Magic *is*. "Upscale all of the graphics" is the one-line ask; the twofold goal underneath it has two different standards:

- **The game presents at a modern resolution.** What reaches the window is crisper than a nearest-neighbour or bilinear stretch of a 64,000-pixel frame. This is a functional bar — the pixels are either higher-resolution or they are not.
- **It still looks like Master of Magic.** The art direction, palette, silhouettes, and readability of Jeff Dee / the SimTex artists survive the process. This is a *fidelity* bar, and it is continuous, not binary. An upscaler that invents plausible-but-foreign detail can score high on "sharper" and fail this outright.

The second goal is the one that will shape every decision, exactly as timbre fidelity did in `BRA-Native-Win32-Audio.md`. Treating it as a nice-to-have is how a project ends up with a crisp game that is no longer the game.

---

## 2. Current-state findings (the facts that reshape the effort)

### 2.1 The engine renders at a fixed 320×200, 8-bit indexed, and the only "upscale" is a window stretch

| Fact | Evidence |
|---|---|
| The internal screen is 320×200 | `platform/include/Platform.h:92-93` — `PLATFORM_SCREEN_WIDTH 320`, `PLATFORM_SCREEN_HEIGHT 200` |
| It is an 8-bit palette-indexed surface, palette-expanded to ARGB8888 each frame | `platform/sdl2/sdl2_Init.c:106,113` — `CreateRGBSurface(..., 8, ...)` → `ARGB8888` |
| The frame is uploaded to a **native-size** (320×200) texture | `platform/sdl2/sdl2_Init.c:116` |
| The *entire* upscale is one GPU stretch of that texture to the window | `platform/sdl2/sdl2_Video.c:131` — `SDL_RenderCopy(renderer, texture, NULL, NULL)` |

**Consequence, and it is the crux of this whole effort:** every game coordinate, every blit, every clip window (`screen_window_x1..y2`, `MoX/src/Graphics.h:30-36`), every mouse hit-test, and every hand-tuned layout constant in `MoM/src/` lives in 320×200 space. Higher-resolution *asset files* have nowhere to go in this engine — it would sample them straight back down to 320×200. Upscaled assets only produce value if **something changes about where the pixels land** (see §4).

### 2.2 The assets are ~103 LBX archives of palette-indexed sprites, and we cannot yet see them

| Fact | Evidence |
|---|---|
| 103 `.LBX` archives ship in `assets/` | `ls assets/*.LBX` |
| Graphics live in archives like `FIGURES1..16`, `UNITS1..2`, `MONSTER`, `TERRAIN`, `PORTRAIT`, `WIZARDS`, `CITYSCAP`, `CMB*` combat tilesets, `SPECFX`, `ITEMS`, `BOOK`, `MAIN`/`MAINSCRN` UI | `assets/` listing |
| A graphic entry is a FLIC animation: a 16-byte header (`int16 width @0; int16 height @2;` frame count, loop frame, palette-header offset), a frame-type byte and remap flag at `0x10`/`0x11`, a table of 4-byte frame offsets, an optional palette, then column-major RLE frames (per column: `0xFF` skip, `0x80` decode with bytes ≥ 224 as repeat ops, `0x00` copy); pixels the RLE never writes are transparent | `s_FLIC_HDR` and the `FLIC_HDR_POS_*` offsets in `MoX/src/FLIC_Draw.h`; the RLE in `FLIC_Draw_Frame()` |
| The engine **already decodes** these to draw them | `FLIC_Draw()` loads the entry's palette (`FLIC_Load_Palette()`), then decodes the current frame with `FLIC_Draw_Frame()`, or `FLIC_Remap_Draw_Frame()` when the remap flag is set (`MoX/src/FLIC_Draw.c`). `Draw_Picture()` / `Draw_Picture_ASM()` are a different path: they draw an already-decoded in-memory *bitmap* (raw column-major indices, `ST_TRANSPARENT` = 0 skipped), not an LBX entry |
| There is **no offline image exporter** | `tools/lbx_extract.c` handles only the 0xDEAF sound wrapper (XMI/VOC); every graphic entry is dumped as opaque `.BIN` (`lbx_extract.c:426` default `extension = "BIN"`) |

**Consequence:** we are about to choose an upscaling strategy for art nobody on this project can currently look at as images. That is backwards, and it is why Phase 1 is "build the library," not "pick a tool." The in-engine decoder (`FLIC_Draw()` → `FLIC_Draw_Frame()`) is the authority for the format; the offline exporter should **reuse that decode, not re-derive it** from scratch.

### 2.3 Sprites are recoloured at runtime — flattening to RGB throws that away

Palette manipulation is pervasive, not incidental: `remap`/`colortbl`/tint code appears across ~20 `MoM/src/` files (`City_ovr55.c`, `MainScr.c`, `Combat.c`, `UnitView.c`, `SPLMASTR.c`, `INITGAME.c`, …), plus the palette-remap primitives `Gradient_Fill`, `Tint_Fill`, and the `remap_block` parameter in `MoX/src/Graphics.h:95,130`. Player/wizard banner colours, spell tints, night/day and terrain shading are all applied at draw time by swapping palette indices, not by storing recoloured copies.

**This is the palette analogue of the audio BRA's GENMIDI misstep.** The instinct is to export each sprite as a finished RGB PNG and upscale that. But a finished RGB PNG has **already collapsed the index → colour mapping**, so it can no longer be recoloured. A blue unit and a red unit are the *same indexed sprite* under two palettes; flatten them and you must upscale N recolourings of every sprite instead of one, and lose the ability to add a new player colour later. Any asset-level path must decide, up front, whether it preserves the index space or abandons runtime recolouring — this is a hard requirement to pin, not a detail. **It has now been pinned:** preserving recolouring is required, via the mask/region approach — see §11 for the mechanism and the decision.

---

## 3. Criteria considered

| Criterion | Why it matters |
|---|---|
| **Fidelity to MoM's art** | Does the result read as the original art at higher resolution, or as new art? The continuous bar of §1. |
| **Preserves runtime recolouring** | Player colours, spell tints, shading are palette swaps. Does the approach keep them working? (§2.3) |
| **Where the pixels land** | Does the approach require reworking the 320×200 rendering model, or does it leave the engine untouched? (§2, §4) |
| **Determinism / reproducibility** | Can the pipeline be re-run to the identical result, or does it depend on a hosted model that may change or vanish? |
| **Temporal & spatial coherence** | Animation frames must stay registered; terrain tiles must still tessellate seamlessly. Per-frame independent upscaling breaks both. |
| **Volume of new engine code** | Rewriting the coordinate system vs. swapping one blit vs. adding a shader — vastly different efforts. |
| **Dependency blast radius** | Anything that ships to users is Tier A per `doc/@Devel/Dependency-Vetting.md`. A build-time tool is Tier B/C. This distinction changes the scrutiny. |
| **Licence & provenance of any model/tool** | Pretrained super-resolution weights and hosted generative models carry licence and provenance questions a classical filter does not. |
| **Cost / repeatability of generation** | A one-time offline bake is free forever; a per-frame hosted call has recurring cost and latency and cannot ship. |

### Criteria explicitly excluded

- **Matching a specific fan HD mod.** Other MoM remasters are reference points, not a fidelity target.
- **Real-time arbitrary resolution.** MoM is turn-based; a fixed set of integer scales is sufficient. We do not need to rescale live to any window size with zero artifacts.
- **Upscaling non-graphics LBX.** Fonts, FLIC intro movies, and sound archives are separate problems; "all the graphics" here means the sprite/tile/UI art. Fonts in particular may want vector replacement, not raster upscaling — out of scope for this BRA.

---

## 4. The defining fork — where do the upscaled pixels land?

This is the decision the whole effort turns on, and it is **not yet made**. There are three families, and the tool choice (OpenCV, ImageMagick, xBRZ, Real-ESRGAN, Nano Banana, …) is downstream of it — picking a tool before picking a family is the §2 mistake of choosing a strategy for art we cannot see.

### Family A — Output-frame upscaling (engine stays 320×200)

Leave the engine exactly as it is. Replace the single dumb `SDL_RenderCopy` stretch with a better one: integer scaling, an edge-directed shader (xBRZ / hqx / Scale2x / ScaleFX in a fragment shader), optional scanline/CRT treatment, or a per-frame ML super-resolution pass.

- **Upscales everything uniformly** — UI, fonts, sprites, cursors, animations — with **zero asset pipeline** and no LBX work at all.
- Requires no change to game logic or coordinates. The risk surface is one render step.
- **Ceiling:** the source is still a 320×200 frame. You can make it clean and edge-preserved (xBRZ) or invent detail (ML), but you cannot exceed what a 64,000-pixel frame carries. Text and dithering are where this shows.
- Makes "build the asset library" *supporting* rather than *central* — but the library is still needed to evaluate and tune any shader/model against real content, and to A/B fidelity.

### Family B — Asset replacement at higher resolution (engine renders hi-res)

Produce higher-resolution versions of the sprites and teach the engine to blit them at a higher internal resolution.

- This is the path the "OpenCV / ImageMagick / Nano Banana on the assets" instinct implies, and it is the **only** path where asset-level upscaling actually helps.
- It is also **much larger**: it requires reworking the 320×200 rendering model (§2.1) — internal resolution, coordinate math, clip windows, hit-testing, and every layout constant — or a hybrid where hi-res sprites composite over an upscaled low-res background at aligned coordinates.
- Forks again on *how* the sprites are upscaled — see §5 — and each sub-choice has a different fidelity contract.
- Must resolve the palette-remap question (§2.3) or accept losing runtime recolouring.

### Family C — Hybrid / staged

Output-frame upscaling now (Family A) as a shippable near-term win, with a subset of high-value assets (portraits, the title/UI, wizard gems) replaced at higher resolution (Family B) where the engine can composite them at aligned positions without a full coordinate rewrite. Portraits and static full-screen art are the natural first Family-B candidates because they are drawn at fixed positions and are not recoloured or tessellated.

**No family is chosen here.** The library (Phase 1) is what makes the choice evidentiary instead of speculative.

---

## 5. The upscaler families (the tool axis, applicable mostly to Family B)

Ordered by how much they *invent*. Fidelity (§1) falls as invention rises; sharpness and "wow" rise. The right point on this axis is a judgement to make *while looking at real MoM sprites at real scale*, not in the abstract.

| Class | Examples | Character | Fidelity contract |
|---|---|---|---|
| **Classical resampling** | ImageMagick / OpenCV bicubic, Lanczos | Smooth interpolation | Faithful but blurry; wrong for hard-edged pixel art. A baseline, not an answer. |
| **Pixel-art scalers** | xBRZ, hqx, Scale2x/3x/4x, ScaleFX, EPX | Deterministic, edge-directed, integer scale | Preserves silhouettes and palette; no invented detail. The safe fidelity choice. Runs on indexed *or* RGB; can run as an offline bake **or** a live shader (ties Family A and B together). |
| **ML super-resolution** | Real-ESRGAN (+ anime/pixel-art models), waifu2x, ESRGAN variants | Learned upscaling, hallucinates plausible detail | Sharper than pixel-art scalers, but invents texture the artists never drew; can smear dithering and shift colours off-palette. Fidelity depends heavily on the model and settings. Deterministic given fixed weights; weights are a Tier-A/B dependency to vet. |
| **Generative reimagining** | Google "Nano Banana" / Gemini image generation, and similar diffusion image models | Redraws the asset from a prompt + reference | Highest visual ceiling, lowest fidelity guarantee; each output is a *new* artwork. Non-deterministic, hosted, recurring-cost, cannot ship in the pipeline. Coherence across frames/tiles is the hard problem — see below. |

**Two coherence traps that disqualify naïve per-asset generation:**

1. **Terrain must tessellate.** `TERRAIN.LBX` tiles abut seamlessly. Upscale each tile independently with an ML/generative model and the seams no longer match — the world map grids. Tileset upscaling must be seam-aware.
2. **Animation frames must stay registered.** A unit's walk cycle is N frames sharing an origin and silhouette. Upscale each frame independently with a hallucinating model and the character shimmers between frames. Frame sets must be upscaled coherently (shared conditioning, or a deterministic scaler).

Generative models (Nano Banana et al.) are best understood here as an **art-direction exploration and one-off asset-creation tool** — e.g. a bespoke high-res title screen or portrait, hand-reviewed — not as a batch pipeline over 320×200 sprites. Submitting to them is worth doing to *see the ceiling*, with eyes open that the output is new art requiring the same hand-approval any commissioned art would.

---

## 6. Why Phase 1 is "build the library," not "pick a tool"

The audio BRA's recurring lesson (`BRA-Native-Win32-Audio.md §9`, `BRA-Combat-Testing.md`) is: **when the substrate/tooling is chosen before the goal is verified against real data, the fit gets assumed instead of checked, and the gap is not noticed until something concrete is observed.** Choosing xBRZ vs. Real-ESRGAN vs. a coordinate rewrite before anyone has looked at MoM's sprites at 4× is that mistake in a new place.

So Phase 1 produces the concrete thing to look at:

1. A **graphic-LBX decoder/exporter** (calling the engine's own `FLIC_Draw()`) that turns every graphic entry into viewable PNGs — per-frame images, animation sprite sheets, and index-preserving exports (see §2.3) — plus the palette(s) used.
2. A **catalogue/manifest**: every archive, every entry, dimensions, frame count, whether it is recoloured at runtime, whether it tessellates, and a class tag (unit / terrain-tile / portrait / UI / fullscreen / effect / font).

With that in hand, the questions in §9 become answerable from evidence — how many *distinct* sprites there really are, how much is recoloured, how much tessellates — and the Family (§4) and tool (§5) choices can be made by A/B-ing candidates on the actual worst-case content (dithered gradients, tiny UI text, seamed terrain), not on a cherry-picked hero sprite.

---

## 7. Dependency posture (nothing adopted yet)

No dependency is chosen in this document. Recording the posture so the eventual choice is scrutinised correctly per `doc/@Devel/Dependency-Vetting.md`:

- **Blast-radius tier depends entirely on the Family (§4).** A **build-time** offline bake (Family B with a deterministic or ML scaler, run once, outputs checked in or generated at build) is **Tier B/C** — it does not ship. A **runtime** shader or model that ships in the binary (Family A live shader, or a bundled ML model) is **Tier A** — it ships to users and gets Tier-A scrutiny.
- **Pretrained ML weights and hosted generative models** carry licence and provenance questions a classical filter (ImageMagick/OpenCV, both permissively licensed and long-tenured) does not. Any such weight file is vetted before adoption; **no trust score is produced**, by policy — verified facts, flags, and unclosed gaps only.
- **Hosted generative APIs (Nano Banana / Gemini image) cannot be a shipping pipeline dependency** — recurring cost, non-determinism, and network dependence rule that out. They are a design-time tool whose outputs, if used, become checked-in hand-approved assets like any commissioned art.
- **ReMoM has no root `LICENCE` file** (noted in the audio BRA too). Any redistributed model weights or scaler code with copyleft or attribution terms must be reconciled with that before shipping.

---

## 8. What currently looks like it fits (provisional, pending the library)

Stated as a leaning, not a decision, and explicitly revisable once Phase 1 exists:

- **Family A (output-frame) as the near-term shippable win.** A pixel-art shader (xBRZ/ScaleFX class) in place of the raw `SDL_RenderCopy` stretch upgrades *everything* with a tiny, contained risk surface and no asset pipeline. It is the highest ratio of visible improvement to engineering risk, and it is reversible.
- **The asset library (Phase 1) regardless of Family.** Needed to evaluate, tune, and A/B any approach; a prerequisite for Family B; and independently useful (modding, documentation, the compat-hash work in `STU_GRAF`).
- **Generative models scoped to bespoke one-off art**, hand-reviewed — not a batch sprite pipeline.
- **Mask/region separation for recolouring (§11).** Keeps banner/team colours working with HD-RGB art and *any* upscaler family, so it preserves recolouring without forcing the Family decision. The preferred recolour approach.

Provisionally does **not** fit:

- **Naïve per-asset ML/generative upscaling of the whole set** — breaks tessellation and animation coherence (§5) and collapses the index space (§2.3).
- **A full 320×200-to-hi-res engine rewrite as the *opening* move** — the largest possible effort, committed before the library tells us whether Family A already gets us most of the way.
- **Bicubic/Lanczos as an end state** — a blurry baseline, useful only as a comparison floor.

---

## 9. Open questions (to be answered by the Phase 1 library, or by the maintainer)

1. **Family (§4): output-frame, asset-replacement, or hybrid?** The single decision everything else hangs on. Deferred until the library shows how far Family A alone gets.
2. **~~Is preserving runtime recolouring a hard requirement?~~ Resolved:** yes, it is required, and the approach is mask/region separation (§11) rather than index-space preservation or baked recolourings. What remains open is the *mask fidelity* detail — for the general 256-entry remap LUT (spell tints, fog, shading), whether a per-pixel mask is needed or a global output-stage LUT pass suffices (§11 leans to the latter).
3. **How many *distinct* sprites are there really?** Frame counts and cross-archive reuse are unknown until the catalogue exists. This sizes the entire effort and decides whether hand-review of generative output is even tractable.
4. **What is the target internal resolution / scale factor(s)?** 2×, 3×, 4×, or a free hi-res? Integer scales keep pixel-art scalers and coordinate math clean; arbitrary scales do not.
5. **Terrain tessellation and animation registration** — how much of the set has these constraints, and can a seam-aware / frame-coherent process handle them? (§5)
6. **Where does the offline exporter live and what is authoritative?** `FLIC_Draw()` (`MoX/src/FLIC_Draw.c`) is the format authority. The Phase 1a tracer bullet settled the mechanism: `tools/lbx_image_export.c` links the engine (`STU MOX momlib Platform_Headless`, as `Extract_FONTS_LBX` already does) and calls `FLIC_Draw()` into its own 320×200 page, so there is no second format reader.
7. **Fonts.** Excluded here (§3), but the eventual approach (raster upscale vs. vector replacement) should not be foreclosed by choices made for sprites.

---

## 10. Lesson carried from the prior BRAs

`BRA-Native-Win32-Audio.md` recorded forcing MoM's 4-operator timbres into GENMIDI, a format they were never designed for, because it was the format the tool already accepted — and the loss was capped by the *format*, not by MoM. **The same shape is latent here:** flattening MoM's palette-indexed, runtime-recoloured sprites into finished RGB PNGs (because that is what OpenCV/ImageMagick/ML upscalers accept) would cap fidelity at the format's limits and silently discard runtime recolouring — a loss attributable to the intermediate format, not to the art.

The rule this reinforces, again: **do not let the tool's preferred input format choose the fidelity ceiling.** Build the library first, look at the real art at real scale, and let MoM's data — not ImageMagick's or a model's convenient input — set the target.

---

## 11. Recoloring — mechanism and the mask approach (chosen direction)

*(Conceptually this belongs with §2.3, which states the problem; it is placed last only to avoid renumbering the sections above.)*

Recolouring is the one property that a naïve "export to RGB and upscale" pipeline silently destroys (§2.3). It is real, it is pervasive, and — importantly — it is **structured**, which is what makes it manageable at high resolution.

### 11.1 How MoM recolours (two distinct mechanisms)

| Mechanism | What it does | Evidence |
|---|---|---|
| **Reserved-index replacement — banner / team colour** | A unit's banner is **5 reserved palette indices (214–218)**, overwritten with the owning player's 5-colour ramp in a work area before the sprite is drawn | `MoM/src/UnitView.c:2683` — `Replace_Color(UnitDraw_WorkArea, 214 + i, COL_Banners[banner_id*5 + i])`, loop `i = 0..4` |
| **256-entry remap LUT — general tint** | Every source index is mapped through a 256-entry lookup at blit time; the path for spell tints, fog/night dimming, and shading | `picture_remap_color_list[256]` (`MoX/src/FLIC_Draw.c:261`) via `Remap_Draw_Picture_ASM` (`:3521`) / `FLIC_Remap_Draw_Frame` (`:295`) |

The two are different problems and want different solutions:

- **Banner/team colour is a small, fixed, *spatial* region** — 5 known indices. Which pixels are "banner" is a property of the *sprite*, so it exports cleanly as a per-sprite mask.
- **General tints are a *global*, whole-frame index→index transform** — not tied to particular pixels of one sprite, and often applied to the whole scene (fog, night). This is naturally an output-stage operation, not a per-sprite mask.

### 11.2 The core tension this resolves

HD detail wants **RGB** (what ML/generative upscalers emit); recolouring wants **indices or masks** (you cannot recolour a flattened RGB pixel). The four options considered:

1. **Keep it indexed even at HD** — upscale the index map, recolour as today. Works only with deterministic pixel-art scalers (xBRZ/hqx); forecloses ML/generative output (which can't survive a snap back to 256 indices).
2. **Mask / region separation** — export base RGB (banner pixels neutralised) + a mask of the recolourable region; tint the masked region by the player's ramp at draw time. Works with HD-RGB and *any* upscaler; standard modern team-colour technique; shader-friendly. **Chosen.**
3. **Pre-bake every recolouring** — N copies per sprite. Explodes with tints, blocks adding a colour later. Rejected except as a last-resort fallback for a tiny fixed set.
4. **Runtime palette-remap shader** — sprites as index-map textures, palette/LUT as a uniform, lookup in a fragment shader. Exact MoM semantics and free arbitrary recolour, but gives no *detail* unless paired with an upscaled index map (→ option 1's constraint).

### 11.3 Decision — Option 2, split by recolour type

- **Banner / team colour → per-sprite mask.** For each affected sprite the exporter emits (a) a base image with indices 214–218 neutralised and (b) a mask encoding *ramp position 0–4* per pixel. At draw time the masked region is tinted by looking up the owning player's 5-colour ramp (itself upscalable to a smooth gradient). Because the banner is only 5 known indices, generating the mask is nearly free and the technique composes with HD-RGB base art and any upscaler family.
- **General tints (spells / fog / night / shading) → an output-stage LUT/shader pass**, independent of how sprites are stored. These are already a 256→256 index transform; re-expressing them as a shader uniform is the modern equivalent and avoids baking tint variants into assets at all.

**Why this fits the "decide the Family after the library" stance:** the mask approach is compatible with output-frame upscaling *and* asset replacement *and* the hybrid — it does not force the Family choice (§4). It only rules *in* the HD-RGB upscaler classes while keeping banners working, and rules *out* Option 3's combinatorial baking.

### 11.4 What Phase 1 must capture for this to be possible

The mask approach needs data that only the exporter can produce, so it is folded into the Phase 1 library requirements (see the PRD/PLAN):

1. **Reserved-index roles per sprite** — which pixels use indices 214–218 (banner), emitted as a mask alongside the base image.
2. **The palette(s)** an entry is drawn under (already in scope), plus the banner ramp table `COL_Banners` for reference.
3. **Which entries actually go through the remap LUT path at runtime** — best captured by an optional hook on both draw paths — `FLIC_Draw()` (LBX entries; `FLIC_Remap_Draw_Frame()` when remapped) and `Draw_Picture()` (decoded bitmaps; `Remap_Draw_Picture_ASM()` when remapped) — that logs the entry and the LUT in use. This answers the catalogue's `recoloured_runtime` column authoritatively instead of by heuristic, and tells us which sprites are tinted globally vs. recoloured by banner.

### 11.5 Open sub-questions

- **Ramp interpolation at HD.** The banner ramp is 5 discrete colours; at 4× do we want a smooth 5-stop gradient across the masked region, or keep hard bands? A fidelity call to make while looking at the library.
- **Mask upscaling.** The base upscales as RGB; the mask must stay crisp (nearest-neighbour or a vector/SDF boundary) so the tinted region's edge matches the base. Verify no halo at the mask boundary.
- **Do any non-banner sprites use the 214–218 range for non-banner pixels?** If an unrelated sprite legitimately uses those indices, the mask heuristic would false-positive. The runtime hook (11.4.3) plus a static scan settles this.
