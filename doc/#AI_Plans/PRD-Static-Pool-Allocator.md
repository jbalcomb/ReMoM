# PRD: Static-Pool-Backed SimTex Space Alloc

## Problem Statement

OG-MoM contains a small but documented set of out-of-bounds memory accesses
(AVRL = Access Violation Reading Location; AVWL = Access Violation Writing Location)
that are non-critical in DOS real-mode but trigger crashes when faithfully
preserved in ReMoM under modern memory protections (ASan, page-based segfaults,
fortified malloc, etc.). The current Allocate.c implementation calls `malloc`
once per arena (`Allocate_Space`), and sub-blocks are bump-pointer carved out of
that arena via `Allocate_First_Block` / `Allocate_Next_Block`. When an OG OOB
access spills outside the parent malloc's boundary — past its end (overrun) or,
via `-1`-style indexing, before its start (underrun) — ASan or the OS faults
ReMoM even though the OG byte is logically harmless. A documented underrun
example is AISPELL.c's `_ai_all_own_stacks[best_stack_idx]` read when
`best_stack_idx` is `ST_UNDEFINED` (-1); documented overrun examples are
Simtex_Autotiling's `wy+1` row read and Generate_Roads' writes past
`_world_maps`.

Reproducing OG's full bug-for-bug behavior requires those OOB accesses — in
both directions — to land in addressable, deterministic memory, and not crash
the process.

## Solution

Replace the per-arena `malloc` inside `Allocate_Space` with a single large
static pool. The pool is sized at compile time as the sum of all in-scope
`Allocate_Space` sizes plus fixed safety margins at both ends: a small leading
guard (`LEADING_GUARD`, 256 B - 4 KB) ahead of the first arena so `-1`-style
underruns off the first carve stay inside the pool, and a trailing margin
(`FIXED_MARGIN`, 4-16 KB) so overruns off the last arena do too. Arenas are
carved out of the pool by a new internal `Pool_Carve` bump-pointer allocator;
sub-blocks within each arena continue to use the existing
`Allocate_First_Block` / `Allocate_Next_Block` headers unchanged. Arenas
between neighbors get incidental OOB coverage in both directions from tight
packing: an underrun lands in the previous arena's tail, an overrun in the
next arena's head.

The static pool is initialized to a sentinel byte pattern (`0xCC`) so accidental
OOB reads in non-OG-faithful code produce visible garbage rather than silent
zero. The existing `gd_ci_inject_*` infrastructure for stamping OG-captured byte
patterns into known OOB regions (e.g., the slack past `_world_maps`) continues
to work unchanged — the slack address is now inside the pool instead of inside
a malloc block, but the inject API is unchanged.

Video and hardware/expanded-memory buffers are not System RAM: they model
VGA hardware (the framebuffer and page buffers) and EMS/expanded-memory banks.
Those buffers bypass the Space Alloc subsystem entirely and call `malloc()`
directly to reach the OS allocator. None of them are sub-divided, so they
carry no SAMB header — each site is edited to allocate a raw byte buffer via
`malloc()`. Everything that is System RAM stays inside the pool via a drop-in
internal swap — including `g_graphics_cache_seg` (a SAMB arena sub-divided via
`Allocate_First_Block`) and the AIL/XMIDI audio buffers in `Audio_Init__WIP`
(`state_table`, `timbre_cache`, `timb_seg`), which are conventional-RAM driver
tables and stay on `Allocate_Space`.

ASan sub-block tracking (poisoning the slack between arena sub-blocks so OOBs
into the slack still flag) is explicitly out of scope for this MVP and is
deferred to Phase 2.

## User Stories

1. As a ReMoM developer, I want Simtex_Autotiling's OG-faithful OOB read at
   `wp=1 wy=39 → wy+1=40` to not crash ReMoM under ASan or page-protection, so
   that I can run ReMoM with sanitizers enabled and still preserve OG behavior.
2. As a ReMoM developer, I want OG's OOB writes from Generate_Roads to land in
   addressable memory inside the static pool, so that they don't corrupt random
   heap data or fault the process.
3. As a ReMoM developer, I want the existing public allocator API
   (`Allocate_Space`, `Allocate_Space_No_Header`, `Allocate_First_Block`,
   `Allocate_Next_Block`, `Check_Allocation`, `Get_Free_Blocks`, `Mark_Block`,
   `Release_Block`, `Reset_First_Block`) to keep unchanged signatures, so that
   the ~30 in-scope call sites need no edits.
4. As a ReMoM developer, I want video buffers (Video2.c) and the EMS/graphics
   banks (`EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`) to be
   allocated via `malloc()` directly, so that they don't bloat the static
   pool and so that their existing ASan tracking stays as it is.
5. As a ReMoM developer, I want the excluded (non-System-RAM) call sites to
   call `malloc()` directly instead of going through the Space Alloc API, so
   that the pool/heap boundary is explicit and auditable and those buffers
   plainly model hardware / expanded memory rather than System RAM.
6. As a ReMoM developer, I want the static pool filled with sentinel byte
   `0xCC` at startup, so that accidental OOB reads in non-OG-faithful code
   produce visible garbage rather than silent zero.
7. As a ReMoM developer, I want the existing `gd_ci_inject_world_overrun` and
   future companion injection hooks to work unchanged over the pool, so that
   OG-captured byte patterns at known OOB sites continue to be observable in
   ReMoM runs.
8. As a ReMoM developer, I want the pool's bump-pointer carving to be
   deterministic across runs (same `Allocate_Space` call order → same per-arena
   offsets), so that the layout matches what the CI byte-capture pipeline
   expects.
9. As a ReMoM developer, I want the pool size set as
   `LEADING_GUARD + sum_of_in_scope_Allocate_Space_sizes + FIXED_MARGIN`
   (LEADING_GUARD in the 256 B - 4 KB range, FIXED_MARGIN in the 4-16 KB
   range), so that the pool is auditable, tight, and forces new OOB
   accommodations — before or after the arenas — to be explicit.
10. As a ReMoM developer, I want `Allocate_Space_No_Header` to be pool-backed
    too, so that all SAMB-style arenas behave uniformly with the
    `malloc()`-direct excluded buffers as the only exception.
11. As a ReMoM developer, I want pool exhaustion to be a fatal error via
    `Allocation_Error`, so that pool-sizing bugs surface loudly at startup
    rather than via silent corruption.
12. As a ReMoM developer, I want the new `Allocate_Pool` module testable in
    isolation, so that I can verify bump-pointer correctness without dragging
    in the SAMB header machinery.
13. As a ReMoM developer, I want unit tests asserting `Pool_Init` fills the
    entire pool with `0xCC`, so that subsequent OOB reads produce predictable
    bytes.
14. As a ReMoM developer, I want unit tests asserting consecutive `Pool_Carve`
    calls return adjacent addresses, so that the determinism invariant is
    enforced by tests.
15. As a ReMoM developer, I want unit tests asserting a `Pool_Carve` request
    that exceeds remaining capacity triggers a fatal `Allocation_Error`, so
    that pool-overflow bugs surface immediately.
16. As a ReMoM developer, I want an integration test asserting `Allocate_Space`
    returns a pool-resident pointer with a valid SAMB header (correct
    `_AAAA` sentinels, `MEMSIG1`, `MEMSIG2`, `SIZE`, `USED=1`), so that
    downstream sub-block code can rely on header correctness.
17. As a ReMoM developer, I want an integration test asserting
    `Allocate_First_Block` and `Allocate_Next_Block` carve correctly within a
    pool-backed arena, so that sub-block allocation is verified to continue to
    work post-refactor.
18. As a ReMoM developer, I want an integration test asserting the sentinel
    pattern in the slack between sub-blocks is preserved across normal
    sub-block writes, so that accidental sub-block-boundary corruption can be
    detected.
19. As a matchup-pipeline runner, I want an end-to-end HeMoM-scripted harness
    that starts a new game with the `matchup_hemom` seed, drives it through
    Simtex_Autotiling at the wp=1/wy=39 OOB read site, asserts the process
    doesn't crash, and asserts that the byte read at the OOB address equals
    the CI-injected OG value, so that the refactor's success criterion is
    empirically validated.
20. As a ReMoM developer, I want existing `gd_dump_*` byte-compare points
    (e.g., `19_Simtex_Autotiling_W`, `20_River_Path_W`, `27_CRP_NEWG_CreatePathGrids_U`,
    save-game compares) to remain green after the refactor, so that no new
    byte divergences are introduced.
21. As a ReMoM developer, I want a compile-time check that
    `POOL_ARENA_CAPACITY >= POOL_MIN_ARENA_BYTES` (the measured floor), so that
    shrinking the pool below the known requirement cannot silently ship;
    genuine run-time over-capacity is caught loudly by `Pool_Carve`'s fatal
    `Allocation_Error`.
22. As a HeMoM headless test runner, I want the same allocator behavior in
    HeMoM as in ReMoMber, so that headless and interactive runs do not
    diverge due to allocator differences.
23. As a future-maintainer, I want each excluded `malloc()` call site to
    carry a brief comment explaining why that arena bypasses the pool (VGA
    hardware / sound driver / EMS bank), so that the pool/heap boundary
    remains auditable and intentional over time.
24. As a Phase-2 implementer, I want the MVP's pool layout to be compatible
    with future ASan sub-block annotation (deterministic offsets, sentinel
    slack, no compile-time obstacles), so that the suballocation/shadow-memory
    tracking work can layer on without a second refactor.
25. As a ReMoM developer, I want OG-faithful underruns (`-1`-style reads and
    writes before an arena's start) to land inside the pool just like
    overruns do — via the leading guard for the first arena and via tight
    packing for every arena after it — so that both OOB directions are
    crash-free and deterministic.

## Implementation Decisions

- **New module `Allocate_Pool`** (in MoX/src/). Deep module; thin public
  interface. Owns the static byte array and the bump-pointer state. Public
  API: `Pool_Init`, `Pool_Carve`, `Pool_Bytes_Used`, `Pool_Bytes_Free`,
  `Pool_Bytes_Peak`. No knowledge of SAMB headers or any MoM concept.
- **Pool storage**: single `static uint8_t` array in BSS, size
  `POOL_SIZE = POOL_LEADING_GUARD + POOL_ARENA_CAPACITY + POOL_FIXED_MARGIN`,
  with `POOL_LEADING_GUARD` (1 KB) reserved ahead of the first carve and a
  trailing `POOL_FIXED_MARGIN` (8 KB). `Pool_Carve` never hands out guard
  bytes; the first carve returns `pool_base + POOL_LEADING_GUARD`.
  `POOL_ARENA_CAPACITY` is sized from the **measured** peak (see Pool sizing
  below), not a static enumerated sum — the in-scope footprint includes
  run-time-sized one-time `sa_Single` LBX loads a compile-time sum cannot
  capture.
- **Pool sizing (measured)**: the carvable capacity was set from the observed
  high-water mark, not a paper estimate. `Pool_Bytes_Peak` reports the peak
  `Pool_Bytes_Used` across a process; the HeMoM Continue scenario (full-game
  entry — loads a save, enters the game, pulling in many one-time `sa_Single`
  LBX graphic buffers on top of the ~1.5 MB enumerated arenas) peaks at
  **~4.58 MB**. `POOL_ARENA_CAPACITY` is set to **16 MB** (~3.5× that peak) to
  cover heavier game paths, with `POOL_MIN_ARENA_BYTES` (5 MB) as the asserted
  floor. Because `Pool_Carve` is fatal-on-exhaustion, a path that ever exceeds
  the capacity fails loudly rather than corrupting — the fix is to raise the
  constant and re-measure.
- **Sentinel pattern & init**: `Pool_Init` fills the entire pool with `0xCC`
  and resets the cursor. `Pool_Carve` **lazily** calls `Pool_Init` on first
  use (one-time, guarded by a static flag), so production code needs no
  explicit startup call and no call-ordering assumption; tests that call
  `Pool_Init` directly still reset it. This is safe because every top-level
  allocation sequence (`Allocate_Data_Space`, STU world-gen's
  `Allocate_Game_Data`) runs once per process and the OG allocator never
  frees, so the never-freeing pool only ever accumulates the persistent
  footprint. Sub-block writes overwrite their own bytes; slack stays at the
  sentinel.
- **`Allocate.c` public API unchanged**:
  - `Allocate_Space(size)` becomes pool-backed via `Pool_Carve((size+1)*16)`,
    then writes the same SAMB header (`_AAAA` sentinel, `MEMSIG1`, `MEMSIG2`,
    `SIZE=size`, `USED=1`) it does today.
  - `Allocate_Space_No_Header(size)` becomes pool-backed (no header writes).
  - `Allocate_First_Block`, `Allocate_Next_Block`, `Check_Allocation`,
    `Get_Free_Blocks`, `Mark_Block`, `Release_Block`, `Reset_First_Block`
    are unchanged — they only operate inside an already-allocated arena
    header.
- **Excluded call sites call `malloc()` directly.** These arenas are not
  System RAM, so they bypass the Space Alloc subsystem and allocate a raw
  byte buffer straight from the OS allocator. None are sub-divided, so none
  carry a SAMB header; each site is sized in bytes (not paragraphs) to the
  buffer's real requirement, and the paragraph `+1` / 16-byte-header rounding
  that `Allocate_Space` applied is dropped. Sites:
  - Video2.c: `video_memory` and the 4×16000 + 4×64000 PR page buffers.
  - ALLOC.c: `EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`.
- **`g_graphics_cache_seg` stays pool-backed.** Unlike the buffers above it
  is a SAMB arena — `Graphics_Cache_Reset()` calls
  `Allocate_First_Block(g_graphics_cache_seg, 1)` (LOADER.c), which relies on
  the arena header `Allocate_Space` writes. It therefore rides the pool with
  every other in-scope `Allocate_Space` arena (adds ~1 MB to `POOL_SIZE`).
- **The AIL/XMIDI audio buffers stay pool-backed.** `state_table`,
  `timbre_cache`, and `timb_seg` in `Audio_Init__WIP` (SOUND.c) are
  conventional-RAM driver tables, not sound-hardware memory, and remain on
  `Allocate_Space` — so they ride the pool like every other in-scope arena.
  (`Audio_Init__WIP` is work-in-progress; leaving it on the standard allocator
  keeps that path untouched by this refactor.)
- **`_screen_seg` moves to pool**. Its sub-blocks (`near_buffer_save`,
  `help_pict_seg`, `IMG_SBK_PageText`) automatically come with it via the
  existing `Allocate_First_Block` / `Allocate_Next_Block` calls.
- **CI byte injection unchanged**. `gd_ci_inject_world_overrun` and any
  future companion injection hooks compute slack addresses from base
  pointers; those pointers now point into the pool instead of into malloc,
  but the API and the injected byte patterns are unchanged.
- **Pool exhaustion** invokes `Allocation_Error` (matching the existing
  fatal-on-allocator-failure pattern). Caller never sees a NULL return.
- **Build-time sanity**: a compile-time assertion that
  `POOL_ARENA_CAPACITY >= POOL_MIN_ARENA_BYTES` (the measured floor). These
  `.c` files build as C90, so this uses the portable negative-array-size idiom
  (`typedef char check[cond ? 1 : -1]`), not C11 `_Static_assert`. It guards
  against anyone shrinking the capacity back below the known requirement; true
  over-capacity at run time is caught by `Pool_Carve`'s fatal
  `Allocation_Error`.
- **No public header changes** beyond adding the `Allocate_Pool` module's
  header. The SAMB struct, the paragraph constants, the sentinel values, and
  every existing sub-block layout stay identical.
- **HeMoM and ReMoMber both consume MoX/src/Allocate.c** and the new
  Allocate_Pool module identically; no per-target conditional behavior.

## Testing Decisions

Good tests verify external behavior — pointer correctness, sentinel
contents, fatal-on-exhaustion, byte equality with OG capture. Tests do not
inspect internal bump-pointer state directly; they observe externally
visible effects.

- **`Pool_*` unit tests** (around `Allocate_Pool` module): init produces
  `0xCC` throughout; carve advances the next-pointer by exactly the
  requested byte count; consecutive carves return adjacent addresses; a
  request exceeding remaining capacity is fatal; carve is deterministic
  across runs (same sequence → same addresses).
- **Allocator integration tests** (around `Allocate.c`):
  `Allocate_Space` returns a pool-resident pointer with a correct SAMB
  header; `Allocate_First_Block` and `Allocate_Next_Block` carve correctly
  inside a pool-backed arena; slack between sub-blocks remains at the
  sentinel pattern after normal sub-block writes.
- **End-to-end OOB harness** (driven via HeMoM scenario script): start a
  new game with the matchup seed; let Simtex_Autotiling run; assert the
  process doesn't crash through the wp=1/wy=39 dump point; extract the
  byte at the OOB read address from the existing `gd_dump_*` channel;
  assert it equals the CI-injected OG value.
- **Existing dump regression**: re-run the matchup pipeline; assert all
  currently-green `gd_dump_*` comparison points stay green.

Prior art:
- `MoM/tests/test_Map_Square_Is_Embarkable.cpp` for the unit-test pattern
  (CTest + Google Test inside MoM/tests/).
- `tools/parity_check.py` for the end-to-end pipeline that compares dump
  outputs against an OG-captured baseline.
- Existing `gd_dump_*` infrastructure in MAPGEN.c and the
  `gd_ci_inject_world_overrun` hook in ALLOC.c for the byte-capture and
  injection mechanism the OOB harness relies on.

## Out of Scope

- **Near_Allocate_* family** (the 4400 B near buffer and its First / Next /
  Mark / Undo / Reset siblings). Stays on its current backing. Deferred.
  NOTE: the documented `_ai_all_own_stacks[-1]` underrun (AISPELL.c, read of
  `_ai_all_own_stacks[best_stack_idx]` with `best_stack_idx == ST_UNDEFINED`)
  is backed by `Near_Allocate_Next` and therefore rides with this deferral —
  the static pool does not cover it.
- **Video2.c arenas**: `video_memory` and the per-page buffers call
  `malloc()` directly (VGA hardware).
- **SOUND.c audio buffers are NOT excluded**: `state_table`, `timbre_cache`,
  `timb_seg` in `Audio_Init__WIP` stay on `Allocate_Space` and ride the pool
  (conventional-RAM AIL/XMIDI driver tables, and a WIP path left untouched).
- **EMS/graphics banks in ALLOC.c**: `EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`,
  `GfxBuf_2400B` call `malloc()` directly. (`g_graphics_cache_seg` is *not*
  excluded — it is a SAMB arena and stays pool-backed.)
- **ASan sub-block tracking** via `__asan_poison_memory_region` /
  `__sanitizer_annotate_contiguous_container`: Phase 2.
- **Build-flag-gated ASan sub-block detection**: Phase 2.
- **Dynamic pool resizing**: pool size is compile-time fixed.
- **Per-arena slack inside the pool**: arenas pack tightly; an OOB access
  from one arena spills into a neighboring arena's bytes (next arena's head
  on overrun, previous arena's tail on underrun) or into the leading /
  trailing guard regions at the pool's ends. Per-arena padding is Phase 2
  if needed.
- **`Allocate_Dos_Space` / `Allocate_Dos_Data_Space`**: DOS-only wrappers
  with minimal use in ReMoM; left as-is.

## Further Notes

- **BSS bloat**: the enumerated in-scope arenas sum to ~1.5 MB
  (`g_graphics_cache_seg` alone is ~1 MB), but the measured full-game-entry
  peak is ~4.58 MB once one-time `sa_Single` LBX graphic buffers are counted.
  `POOL_ARENA_CAPACITY` is set to 16 MB for margin, so `POOL_SIZE` (guard +
  capacity + margin) is ~16 MB of BSS. Still trivial for a modern build (BSS
  is zero-fill, not on-disk).
- **Determinism**: the pool layout is bump-pointer per `Allocate_Space` call
  order. As long as the startup sequence (`Allocate_Data_Space` and the
  individual `Allocate_Space` calls in Fonts.c / Input.c / MOM_DAT.c /
  ALLOC.c) preserves call order, arena offsets are stable across runs.
- **Phase 2 readiness**: the pool is laid out so that ASan poisoning APIs
  (`__asan_poison_memory_region`) can be applied around each sub-block
  boundary without further restructuring. Sentinel slack, deterministic
  offsets, and a known pool base make instrumentation straightforward.
- **HeMoM ↔ ReMoMber parity**: both targets share MoX/src/Allocate.c and the
  new Allocate_Pool module, so the refactor is uniform across them. No
  per-target conditionals.
- **CI baseline capture**: the existing capture-and-inject pipeline for OG
  OOB bytes (currently exercised for `_world_maps` overrun) is the model
  for any future OOB site discovered post-MVP. Adding a new captured-OOB
  region is a small additive change to the inject hook, not a restructure.
