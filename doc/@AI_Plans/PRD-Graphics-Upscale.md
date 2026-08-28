# PRD — Graphics Upscaling

**Status:** Draft
**Owner:** TBD
**Date:** 2026-07-24
**Companion:** `BRA-Graphics-Upscale.md` (decision framing), `PLAN-Graphics-Upscale.md` (survey + build-out order)

---

## Problem Statement

ReMoM renders at 320×200, 8-bit palette-indexed, and the only thing standing between that and the window is a single GPU stretch (`SDL_RenderCopy(renderer, texture, NULL, NULL)`, `platform/sdl2/sdl2_Video.c:131`). On a modern display the game is either a soft bilinear blur or blocky nearest-neighbour. We want it to look substantially sharper **without** ceasing to look like Master of Magic.

The obstacle is that we cannot currently choose an upscaling strategy responsibly: the art lives in 103 `.LBX` archives as palette-indexed sprites that **no tool on this project can render to a viewable image** — `tools/lbx_extract.c` dumps every graphic entry as opaque `.BIN`. And the assets are recoloured at runtime (player colours, spell tints, shading are palette swaps), so the naïve "export to RGB and upscale" path silently discards a core property. See `BRA-Graphics-Upscale.md` for the full framing; this PRD states what to build.

## Solution

Two things, in order, with the architecture decision sandwiched between them:

1. **Phase 1 — the asset library (this PRD's committed scope).** A graphic-LBX decoder/exporter that turns every graphic entry into viewable, index-preserving images plus a machine-readable catalogue, so the actual art can be examined at real scale and the upscaling architecture can be chosen from evidence.
2. **Architecture decision** (recorded in the BRA once Phase 1 exists): output-frame upscaling vs. hi-res asset replacement vs. hybrid; and, if asset-level, which upscaler class.
3. **Later phases — the chosen upscaling itself.** Kept out of committed scope here because the responsible choice depends on Phase 1's output. Path-neutral forward requirements are recorded below so Phase 1 does not paint later phases into a corner.

## User Stories

### Phase 1 — asset library (committed)

1. As a ReMoM developer, I want every graphic LBX entry exported to a PNG, so that I can *see* the art the game draws without running the game.
2. As a ReMoM developer, I want multi-frame entries exported both as individual frames and as a sprite sheet, so that animations are inspectable as a set and their registration is visible.
3. As a ReMoM developer, I want an **index-preserving** export (indexed PNG or index-map + palette), so that runtime recolouring is not silently baked away and a later asset-level path can keep the index space.
4. As a ReMoM developer, I want each entry rendered under its real palette(s) too, so that I can see what the player actually sees, not just an index map.
5. As a ReMoM developer, I want a machine-readable catalogue (one row per entry: archive, index, width, height, frame count, class tag, recoloured-at-runtime flag, tessellates flag), so that the size and shape of the problem are quantified before any tool is chosen.
6. As a ReMoM developer, I want the exporter to reuse the engine's own sprite decode (`Draw_Picture_ASM`, `MoX/src/FLIC_Draw.c`) rather than re-derive the format, so that there is one authoritative format reader, not two that can diverge.
7. As a ReMoM developer, I want the exporter runnable over all of `assets/` in one command, so that regenerating the whole library after a decoder fix is one step.
8. As a ReMoM developer, I want an HTML or gallery index of the library, so that the whole set can be eyeballed at 1×/2×/4× and worst-case content (dithered gradients, tiny UI text, seamed terrain) can be found for A/B testing.

### Later phases — upscaling (forward-looking, not committed here)

9. As a player, I want the game to present at a modern resolution that is crisper than a bilinear stretch, so that it looks good on my display.
10. As a ReMoM developer, I want the near-term win (an output-frame pixel-art shader replacing the raw stretch) to be shippable independently of any asset work, so that value lands without waiting on a full asset pipeline.
11. As a ReMoM developer, I want any asset-level upscaling to preserve runtime recolouring (or to make its abandonment an explicit, reviewed decision), so that player colours and tints are not lost by accident.
12. As a ReMoM developer, I want terrain-tile upscaling to remain seamless and animation-frame upscaling to remain registered, so that the world map does not grid and units do not shimmer.
13. As a maintainer, I want to A/B candidate upscalers (pixel-art scaler vs. ML super-resolution vs. bespoke generative art) on real worst-case content before committing, so that the fidelity/sharpness trade-off is chosen with eyes open.
14. As a maintainer, I want any shipped upscaler dependency (runtime shader/model, or a redistributed weight file) vetted per `doc/@Devel/Dependency-Vetting.md` at the correct blast-radius tier, so that a Tier-A dependency is not adopted with Tier-C scrutiny.

## Implementation Decisions

### Phase 1

- **Reuse the engine decoder, do not re-derive the format.** The sprite format authority is `Draw_Picture` / `Draw_Picture_ASM` (`MoX/src/FLIC_Draw.c:1646`); `Draw_Picture_Windowed` shows the width/height header and clipping. The exporter links or lifts that decode. This avoids `lbx_extract`'s situation where a second, partial reader exists.
- **The LBX container reader already exists** in `tools/lbx_extract.c` (offset table, name table, `0xFEAD` magic) and is standalone C. The graphic exporter extends that container reader with a *pixel* decoder; it does not reinvent container parsing.
- **Index-preserving is the default export; RGB-under-palette is an additional output, never the only one.** Per `BRA-Graphics-Upscale.md §2.3`, an RGB-only export throws away the property that makes MoM recolour at runtime.
- **The catalogue is the deliverable, not a side effect.** Its columns are chosen to answer the BRA's open questions (§9): distinct-sprite count, recolour prevalence, tessellation prevalence, scale-factor feasibility.
- **New tool, standalone like `lbx_extract`** — no engine-runtime dependency in the tool if the decode can be lifted cleanly; if reuse requires linking `momlib`/`MOX`, that is acceptable and preferable to a divergent reimplementation. Resolve which during the tracer bullet.
- **Both build systems.** Any new source is added to both the CMake tree and `Makefile.am` (project rule).
- **No changes to game code or the render path in Phase 1.** Phase 1 only *reads* assets. The engine is untouched until the architecture decision is made.

### Later phases (recorded so Phase 1 does not foreclose them)

- **The output-frame path (Family A) touches exactly one seam:** the present step in each backend (`sdl2_Video.c`, `sdl3_*`, `win_PFL.cpp`). It requires no asset library changes and no game-coordinate changes. It is the reversible near-term option and should be evaluated first.
- **The asset-replacement path (Family B) requires a rendering-model change** (internal resolution, coordinates, clip windows, hit-testing) or a compositing hybrid. It is out of Phase 1 scope and is not committed until the BRA decision.
- **Original-game-code edit rules apply** to any later engine change: comment-mark modifications (`/* CLAUDE */`), never delete faithful reconstruction.

## Testing Decisions

- **Phase 1 correctness is checked against the running game, not a golden.** A sprite is decoded correctly iff the exported image matches what `Draw_Picture` puts on screen for the same entry. The tracer-bullet validation is: pick a handful of known entries (a wizard portrait, a unit frame, a terrain tile, a UI panel), export them, and compare against a screenshot of the same asset in-game.
- **Dimensions are a cheap invariant.** Exported width/height must equal the entry's `int16 w@0 / h@2` header for every entry; a mismatch is a decoder bug. This is assertable across the whole set without human review.
- **The catalogue is diffable.** Regenerating it after a decoder change should produce a reviewable diff, so a decoder regression that changes frame counts or dimensions is caught.
- **No characterization test of upscaler *output* in Phase 1** — there is no upscaler yet. Later-phase upscaling gets its own test approach (likely perceptual/hand-review plus tessellation-seam and frame-registration checks), specified when that phase is committed.

## Out of Scope (this PRD)

- Choosing the upscaler tool or the rendering architecture — that is the BRA decision, deferred until Phase 1 exists.
- Fonts (`FONTS.LBX`) — may want vector replacement rather than raster upscaling; separate effort.
- FLIC intro movies and sound archives — not "graphics" in the sprite/tile/UI sense.
- Any engine resolution or coordinate-system change.

## Success Criteria (Phase 1)

- Every graphic entry in all 103 `assets/*.LBX` archives either exports to an image or is explicitly catalogued as non-graphic / undecodable-with-reason.
- Exported dimensions match entry headers across the whole set (automated check passes).
- Spot-checked entries match in-game rendering (tracer-bullet manual comparison).
- The catalogue answers BRA §9 questions Q3 (distinct-sprite count), Q2 (recolour prevalence), and Q5 (tessellation prevalence) with numbers.
- A gallery view exists that lets the maintainer eyeball the whole set and identify worst-case content for later A/B testing.
