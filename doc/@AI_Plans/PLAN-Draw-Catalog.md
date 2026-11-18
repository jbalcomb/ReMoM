# PLAN — Draw / graphics-asset catalog

A graphics-side companion to the input-field catalog ([tools/field_catalog/](../../tools/field_catalog/)),
in two databases:

- **DB A — asset inventory** (parse the LBX archives): every sprite + its dimensions. **DONE.**
- **DB B — runtime draw-trace** (instrument the blit path): *where* each asset is drawn, so a recorded
  click resolves to the specific drawn sprite (finer than the field it sits in). **Designed here; not built.**

Downstream consumers: graphics **upscaling** (DB A alone — every sprite + dims + archive offset), screen
**rearrange / authoring** (A + B — what exists and where it's placed), and `rmr2hms` **click-centering**
(A + B — resolve a click to the sprite under it and name it by its asset).

---

## DB A — asset inventory — **DONE**

[tools/asset_catalog/](../../tools/asset_catalog/) parses `assets/*.LBX` directly (no game run) into
[tools/fields/assets.fwv](../../tools/fields/assets.fwv): `lbx, entry, name, kind, width, height, frames,
offset, length`. 103 archives → 5271 entries → **4800 sprites** (dims from the FLIC header,
`s_FLIC_HDR` 0x00/0x02), 471 `other` (sounds/music/data, gated out by an implausible-dims check).
Parser ported from `tools/lbx_extract.c` + `MoX/src/FLIC_Draw.h`; hermetic test in
[tests/test_lbx.py](../../tools/asset_catalog/tests/test_lbx.py). See the package README for the format.

---

## DB B — runtime draw-trace — **B1 built; B4 palette generation built, effect flags not yet**

The blit call gives a **position**; the sprite's **dimensions** live in the FLIC asset, and the asset is
named by a **loaded segment pointer**, not a literal. So — like the field runtime-geometry merge — the
robust source of truth is a runtime trace. Two `#if STU_CLOG` instrumentation points ([STU_CLOG.h](../../STU/src/STU_CLOG.h)) feed one
per-frame draw list that a click queries.

### 1. Asset identity — instrument `LBX_Reload` / `LBX_Reload_Next`

Resolve identity **up front** (chosen): maintain a debug-only map `pointer -> (lbx_name, entry_num)`.

- **Instrument only the four sprite-returning loaders** — the ones that return a drawable `SAMB_ptr`
  (= `unsigned char *`, the same pointer type the draw calls receive), by call frequency:
  `LBX_Reload_Next` (**608 calls**), `LBX_Load` (118), `LBX_Reload` (89), `LBX_Load_Entry` (5)
  ([LBX_Load.h:78-99](../../MoX/src/LBX_Load.h#L78-L99)). On each, record
  `map[returned_ptr] = (basename(lbx_name), entry_num)`; latest load wins (buffers are reused).
- **Skip the data loaders** — `LBX_Load_Data_Static` (160), `LBX_Load_Data` (8),
  `LBX_Load_Library_Data` (7) load game tables/records, **not** FLIC sprites; they never feed
  `FLIC_Draw`.
- `LBX_Reload_Next` is dominant because the SAMB allocator grows a contiguous region: `LBX_Load`/
  `LBX_Reload` reset/allocate a block (First-Block style), then `_Next` **appends** each following
  entry (Next-Block style), returning a distinct pointer per entry. So each `_Next` call has a
  well-defined `pointer → (lbx, entry)` — but instrument it to record the **actual** entry it
  resolved, since it advances past the passed `entry_num`.
- The general form `LBX_Load_Entry(lbx, entry, head, allocation_type)` takes the allocation kind
  explicitly (`Allocate_First_Block` vs `Allocate_Next_Block`, etc.); the others are wrappers over it.
- This ties every drawn sprite straight back to DB A (`assets.fwv`), so the trace is self-describing
  (`ARMYLIST.LBX:1` → name `ARMYLBUT`, 9×10).

### 2. The per-frame draw list — instrument `FLIC_Draw` (and `Draw_Picture`)

- `FLIC_Draw(x, y, p_FLIC_File)` ([FLIC_Draw.h:236](../../MoX/src/FLIC_Draw.h#L236)): append
  `{ x1=x, y1=y, x2=x+FLIC_Get_Width(p)-1, y2=y+FLIC_Get_Height(p)-1, asset=map[p] }` to a debug-only
  `g_draw_list[]`. Width/height via `FLIC_Get_Width/Height`
  ([:257-260](../../MoX/src/FLIC_Draw.h#L257-L260)).
- `Draw_Picture(x, y, pict_seg)` ([:275](../../MoX/src/FLIC_Draw.h#L275)): pictures also carry dims
  (`Create_Picture(width, height, pict_seg)`, [:207](../../MoX/src/FLIC_Draw.h#L207)) — **open item:**
  confirm the picture dim offset / accessor before including pictures; FLIC sprites cover the common case.
- **`s_FLIC_HDR` and picture buffers are byte-offset structures — do not modify them.** The bbox +
  asset live only in `g_draw_list[]`, all under `#if STU_CLOG`.

### 3. Draw-list lifecycle

**As built:** no per-frame clear. MoM screens often draw once and then copy pages, so a sprite can stay on screen for many frames without another
`FLIC_Draw`; a list cleared each frame would lose it. STU_CLOG keeps a ring of the last 4096 draws ([STU_CLOG.c](../../STU/src/STU_CLOG.c)), and a
hit test scans it newest-first, so the most recently drawn sprite under the point wins. This settles open item 1. The original design follows.

- Cleared at the start of each screen render pass; repopulated as the screen draws.
- A click queries the **last completed** render's list — the screen the user sees and clicks. Because
  the click is injected in `Automation_Driver_Frame` *before* this frame renders, the list at query time
  holds the previous frame's draws = the visible screen. (Nail the exact clear seam against the render
  entry point — `Draw_Fields` ([Fields.h:757](../../MoX/src/Fields.h#L757)) is the field-render marker;
  the screen background/sprites draw around it.)

### 4. Draw-hit query + RECORD.log format

- At click time, in the existing field-hit callback (`Driver_Log_Field_Hit` / `Replay_Log_Field_Hit`),
  also scan `g_draw_list[]` **in reverse** (topmost = last drawn) for the first bbox containing the
  click, and append a `draw=` token:

  ```
  field[..]=Main_Screen@MainMenu.c:416  draw=ARMYLIST.LBX:1@(60,139)-(68,148)
  ```

- Reverse scan gives the correct topmost sprite when sprites overlap.

### 5. `rmr2hms` consumption

- `runtime.py`-style harvest: parse `draw=LBX:entry@(x1,y1)-(x2,y2)` from RECORD.logs → per-asset
  bbox → center; join to `assets.fwv` for the sprite name.
- For a click, prefer the **draw-hit** (fine: the specific sprite) when present, else fall back to the
  **field** name (coarse). Emit `click Screen.Sprite_<alias>` where the sprite alias is curated from the
  LBX entry (names aren't unique — `ARMYLBUT` appears twice — so key on `LBX:entry`).

### Open items to resolve before coding

1. **Render clear seam** — settled by the ring in §3 (no clear).
2. **`Draw_Picture` dims** — confirm the picture header's width/height offset or accessor; until then,
   trace `FLIC_Draw` only.
3. **Draw-list capacity** — measure sprites-per-frame (could be hundreds on the main map); size the
   array and `log()` if it saturates (no silent truncation).
4. **Sprite naming/alias scheme** — a curated alias table keyed on `LBX:entry` (mirrors the field alias
   table), since entry names collide.
5. **Field vs draw precedence** in the emitted `.hms` — default to draw-when-present; confirm.

### 6. Palettes and color effects

The question this answers is "which palette was loaded, and which color effects were in play, when this picture was drawn". It is not "what
exact colors did it show": no RGB values are recorded.

MoM runs in VGA's 256-color mode, so there is **one palette for the whole screen**. A picture with its own palette (`has_palette` in
`assets.fwv`) overwrites a range of that shared palette, and every picture is shown through whatever the palette holds. So palettes are logged
as **events with a generation number**, and each draw record carries the generation in effect:

- **Base palette load** — `Load_Palette(entry, start_color, end_color)` ([Fonts.c:2949](../../MoX/src/Fonts.c#L2949)): log
  `FONTS.LBX:<entry + 2>` (entry 0 uses the embedded copy; log it as such) and the color range.
- **Picture palette load** — `FLIC_Load_Palette(p_FLIC_Header, frame_index)` ([FLIC_Draw.c:74](../../MoX/src/FLIC_Draw.c#L74)): log the
  picture's `LBX:entry` from the §1 identity map, the frame, and the color range it wrote (`start`, `count`; header palette or per-frame palette).
  It is called from `Load_Palette_From_Animation` and the frame-draw paths in FLIC_Draw.c.
- Each load bumps `palette_generation`; each draw record (§2) gets `pal=<generation>`.

**Effect flags.** Each draw record also carries a set of flags saying which color effects applied. Two kinds:

| Kind | Applies to | Functions (where found) | Flag |
|---|---|---|---|
| Palette effects | the whole screen, from when they run until the palette is reloaded or reset | `Cycle_Palette_Color` / `Update_Cycle` ([Fonts.c:3174](../../MoX/src/Fonts.c#L3174)) | `cycle` |
| | | `Cycle_Palette(percent)` — the MoO2 `Darken_Palette_` equivalent ([Fonts.c:3567](../../MoX/src/Fonts.c#L3567)) | `darken` |
| | | `Tint_Palette` ([Fonts.c:3664](../../MoX/src/Fonts.c#L3664)) | `tint` |
| | | `Fade_Out` / `Fade_In` ([MOX_T4.c:271](../../MoX/src/MOX_T4.c#L271)) | `fade` |
| Picture effects | one picture's pixels, before or while it is drawn | `FLIC_Remap_Draw_Frame` ([FLIC_Draw.c:280](../../MoX/src/FLIC_Draw.c#L280)), `Remap_Draw_Picture_ASM` ([:4014](../../MoX/src/FLIC_Draw.c#L4014)), the `Create_Remap_Palette*` blocks they use ([Fonts.c:4125](../../MoX/src/Fonts.c#L4125)) | `remap` |
| | | `Gray_Scale_Bitmap` / `Inverse_Gray_Scale_Bitmap` ([FLIC_Draw.c:4415](../../MoX/src/FLIC_Draw.c#L4415)) | `gray` |
| | | `Vanish_Bitmap` ([FLIC_Draw.c:4389](../../MoX/src/FLIC_Draw.c#L4389)) | `vanish` |
| | | `Shear_Array_Bitmap_X` / `_Y` ([FLIC_Draw.c:766](../../MoX/src/FLIC_Draw.c#L766)) | `shear` |
| | | `Outline_Bitmap_Pixels` ([FLIC_Draw.c:2949](../../MoX/src/FLIC_Draw.c#L2949)) | `outline` |
| | | `Replace_Color` / `Replace_Color_All` ([FLIC_Draw.c:4132](../../MoX/src/FLIC_Draw.c#L4132)) | `recolor` |
| Region effects | a screen rectangle, after drawing | `Tint_Fill` — the MoO2 `Darken_Fill` equivalent ([Graphics.c:1153](../../MoX/src/Graphics.c#L1153)) | `darken_fill`, on every draw record the rectangle overlaps |

- Palette-effect flags are kept as state in STU_CLOG (set when the effect runs, cleared when the palette is loaded or reset) and copied onto each
  draw record.
- Picture-effect flags are recorded against the picture pointer when the effect runs and copied onto the draw record when that pointer is drawn.
- **Copies lose identity.** Many effects work on a scratch copy: e.g. the node-warp draw copies a square into a scratchpad, vanishes and shears it,
  then draws the scratchpad. To keep the source `LBX:entry`, the copy functions have to pass identity and flags from source to destination.
  Which copy functions need this is an open item.
- No function named "blur" was found. The nearest are `Vanish_Bitmap`'s dither and the shears. If "blur" means something else, add it to the
  table.

### 7. Output files — one per purpose

Catalog output does not go into the general log (`remom_log_*.txt`). STU_CLOG writes its own files, in the log directory, one per catalog, so
each tool reads only its own data and a catalog run doesn't have to dig through general logging:

| File | Contents | Reader |
|---|---|---|
| `remom_clog_draw.tsv` | one row per draw: frame, `LBX:entry`, bbox, `pal`, effect flags | B2 harvest, rearrange export |
| `remom_clog_palette.tsv` | one row per palette event: generation, source (`FONTS.LBX:n` or `LBX:entry` + frame), color range, effect on/off | joined to the draw file on `pal` |
| `remom_clog_fields.tsv` | the `FIELDADD` / `FIELDSNAPSHOT` rows, if those are re-enabled | `fields_merge.py` |
| `<name>-RECORD.log` | unchanged: the per-click `field[...]@file:line` stamp, plus the `draw=` hit stamp (§4) | `runtime.py`, `rmr2hms` |

- They're written as `.tsv` because rows stream out during a run; the tools convert to `.fwv`, as `runtime.py` already does.
- File names follow the one-period rule. Each run overwrites them unless the log rotation scheme is extended to cover them; decide when building.

---

## Phasing

- **B1 — built.** `LBX_Load_Entry` names each loaded buffer (`STU_CLOG_Asset_Loaded`), `FLIC_Draw` adds to the draw ring (`STU_CLOG_Draw`), and the
  record-log field-hit callbacks in [ReMoM.c](../../src/ReMoM.c) / [DeMoM.c](../../src/DeMoM.c) stamp `draw=NAME.LBX:entry@(x1,y1)-(x2,y2)`. With
  `REMOM_CLOG_DRAW=1`, every draw is a row in `remom_clog_draw.tsv`.
  Verify live: a recorded click on a known sprite stamps the right `LBX:entry` and bbox.
- **B2** — `runtime.py`-style harvest of `draw=` into a `draw_geometry.fwv`; resolver/`rmr2hms` prefer
  the sprite center; sprite alias table.
- **B3** (optional) — `Draw_Picture` coverage; the rearrange project's placement export
  (asset → all draw sites), which the same trace already collects.
- **B4 — palette part built.** `Load_Palette` and `FLIC_Load_Palette` call `STU_CLOG_Palette_Loaded`, which bumps the generation and (with
  `REMOM_CLOG_DRAW=1`) writes a row to `remom_clog_palette.tsv`; each draw row carries `pal`. **Not built:** the effect flags (§6), which first need
  the copy functions to pass identity from source to destination.
