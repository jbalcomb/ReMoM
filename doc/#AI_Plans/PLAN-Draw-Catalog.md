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

## DB B — runtime draw-trace — **design**

The blit call gives a **position**; the sprite's **dimensions** live in the FLIC asset, and the asset is
named by a **loaded segment pointer**, not a literal. So — like the field runtime-geometry merge — the
robust source of truth is a runtime trace. Two `#ifdef STU_DEBUG` instrumentation points feed one
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
  asset live only in `g_draw_list[]`, all under `#ifdef STU_DEBUG`.

### 3. Draw-list lifecycle

- Cleared at the start of each screen render pass; repopulated as the screen draws.
- A click queries the **last completed** render's list — the screen the user sees and clicks. Because
  the click is injected in `HeMoM_Player_Frame` *before* this frame renders, the list at query time
  holds the previous frame's draws = the visible screen. (Nail the exact clear seam against the render
  entry point — `Draw_Fields` ([Fields.h:757](../../MoX/src/Fields.h#L757)) is the field-render marker;
  the screen background/sprites draw around it.)

### 4. Draw-hit query + RECORD.log format

- At click time, in the existing field-hit callback (`HeMoM_Log_Field_Hit` / `Replay_Log_Field_Hit`),
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

1. **Render clear seam** — pin exactly where a screen's draw pass begins, to reset `g_draw_list[]`.
2. **`Draw_Picture` dims** — confirm the picture header's width/height offset or accessor; until then,
   trace `FLIC_Draw` only.
3. **Draw-list capacity** — measure sprites-per-frame (could be hundreds on the main map); size the
   array and `log()` if it saturates (no silent truncation).
4. **Sprite naming/alias scheme** — a curated alias table keyed on `LBX:entry` (mirrors the field alias
   table), since entry names collide.
5. **Field vs draw precedence** in the emitted `.hms` — default to draw-when-present; confirm.

---

## Phasing

- **B1** — `LBX_Reload` identity map + `FLIC_Draw` draw list + RECORD.log `draw=` stamp (STU_DEBUG).
  Verify live: a recorded click on a known sprite stamps the right `LBX:entry` and bbox.
- **B2** — `runtime.py`-style harvest of `draw=` into a `draw_geometry.fwv`; resolver/`rmr2hms` prefer
  the sprite center; sprite alias table.
- **B3** (optional) — `Draw_Picture` coverage; the rearrange project's placement export
  (asset → all draw sites), which the same trace already collects.
