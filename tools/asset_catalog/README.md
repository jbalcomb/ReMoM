# asset_catalog

Inventory of every graphics asset in the game's LBX archives, built by parsing
`assets/*.LBX` directly — **no game run**. One row per archive entry: the sprite's
name and its width / height / frame count read from each FLIC header.

Serves the graphics **upscale** and **rearrange / new-screen** projects (a complete
sprite inventory with dimensions), and — paired with the runtime draw-trace — lets
`rmr2hms` resolve a click to the specific drawn sprite (finer than the field it sits in).

## Run

```sh
python -m tools.asset_catalog                       # writes tools/fields/assets.fwv
python -m tools.asset_catalog --out other.fwv        # custom path
python -m tools.asset_catalog.tests.test_lbx         # hermetic parser test
```

## Columns (`assets.fwv`, fixed-width)

`lbx, entry, name, kind, width, height, frames, loop, has_palette, store, remap, offset, length`

- **kind** — `sprite` (a valid FLIC header) or `other` (palette / sound / music / data). The sprite
  metadata columns are blank for `other`.
- **width/height** — original 320×200 sprite space, from the FLIC header (`s_FLIC_HDR` 0x00 / 0x02).
- **frames** — FLIC frame count (0x06). **loop** — frame the animation loops back to (0x08).
- **has_palette** — 1 if the sprite carries its own palette (`palette_header_offset` 0x0E ≠ 0), else 0
  (uses the shared game palette). ~413 of 4800 sprites.
- **store** — frame_type / "full store flag" (0x10): full vs delta frame encoding.
- **remap** — remap-colors flag (0x11).
- **offset/length** — where the entry lives inside the archive (for extraction / upscaling).

The FLIC header's DOS EMS paging fields (0x0A–0x0D `emm_*`) and the runtime `current_frame` (0x04) are
intentionally omitted — the EMS fields are meaningful only under DOS expanded memory and are zero in
all but ~33 sprites.

## Format

LBX and FLIC parsing (`lbx.py`) is ported from `tools/lbx_extract.c` and
`MoX/src/FLIC_Draw.h`:

- LBX header: `num_entries`@0x00, magic `0xFEAD`@0x02, offset table@0x08
  (`num_entries + 1` u32 LE), name table@0x200 (32-byte records, 9-char names).
- FLIC header (first 16 bytes of a sprite entry): `width` i16@0x00, `height` i16@0x02,
  `frame_count` i16@0x06. Non-sprite entries have no valid FLIC header; the parser gates on
  plausible dims (`0 < w ≤ 640`, `0 < h ≤ 480`, `0 < frames ≤ 4000`) and flags them `other`.

## Coverage (current)

103 archives in `assets/`, 5271 entries → **4800 sprites**, 471 `other`
(SOUNDFX/MUSIC are sounds, not sprites). Dimensions spot-checked against known assets
(`ARMYLIST` 320×200 background, `ARMYLBUT` 9×10 2-frame button, `CITYBACK` 320×200).

## Related

Sibling of [tools/field_catalog/](../field_catalog/) (input fields). Shares its `fwv`
fixed-width reader/writer. The planned **runtime draw-trace** (instrumenting `FLIC_Draw`)
will add *where* each asset is drawn — see the HMS plan
[doc/@AI_Plans/PLAN-HMS-Named-Actions-And-Waits.md](../../doc/%23AI_Plans/PLAN-HMS-Named-Actions-And-Waits.md).
