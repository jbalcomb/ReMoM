# PRD: Static-Pool-Backed SimTex Space Alloc

## Problem Statement

OG-MoM contains a small but documented set of out-of-bounds memory accesses
(AVRL = Access Violation Reading Location; AVWL = Access Violation Writing Location)
that are intentional or harmless in DOS real-mode but trigger crashes when faithfully
preserved in ReMoM under modern memory protections (ASan, page-based segfaults,
fortified malloc, etc.). The current Allocate.c implementation calls `malloc`
once per arena (`Allocate_Space`), and sub-blocks are bump-pointer carved out of
that arena via `Allocate_First_Block` / `Allocate_Next_Block`. When an OG OOB
write spills past the parent malloc's boundary, ASan or the OS faults ReMoM
even though the OG byte is logically harmless.

Reproducing OG's full bug-for-bug behavior requires those OOB writes to land in
addressable, deterministic memory — and not crash the process.

## Solution

Replace the per-arena `malloc` inside `Allocate_Space` with a single large
static pool. The pool is sized at compile time as the sum of all in-scope
`Allocate_Space` sizes plus a small fixed safety margin (4-16 KB). Arenas are
carved out of the pool by a new internal `Pool_Carve` bump-pointer allocator;
sub-blocks within each arena continue to use the existing
`Allocate_First_Block` / `Allocate_Next_Block` headers unchanged.

The static pool is initialized to a sentinel byte pattern (`0xCC`) so accidental
OOB reads in non-OG-faithful code produce visible garbage rather than silent
zero. The existing `gd_ci_inject_*` infrastructure for stamping OG-captured byte
patterns into known OOB regions (e.g., the slack past `_world_maps`) continues
to work unchanged — the slack address is now inside the pool instead of inside
a malloc block, but the inject API is unchanged.

Video, audio, and large graphics buffers stay on `malloc` via a new function
`Allocate_Space_Heap` (identical behavior to today's `Allocate_Space`). Only
the carve-out call sites (~10 sites) are edited; everything else is a drop-in
internal swap.

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
4. As a ReMoM developer, I want video buffers (Video2.c), audio buffers
   (SOUND.c state_table / timbre_cache / timb_seg), and large graphics
   allocations (`EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`,
   `g_graphics_cache_seg`) to keep their current `malloc` behavior, so that
   they don't bloat the static pool and so that their existing ASan tracking
   stays as it is.
5. As a ReMoM developer, I want a new `Allocate_Space_Heap(size)` function
   exposed alongside `Allocate_Space`, so that the carve-out call sites can
   opt into the legacy `malloc` behavior explicitly and the boundary is
   auditable.
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
   `sum_of_in_scope_Allocate_Space_sizes + FIXED_MARGIN` (FIXED_MARGIN in the
   4-16 KB range), so that the pool is auditable, tight, and forces new OOB
   accommodations to be explicit.
10. As a ReMoM developer, I want `Allocate_Space_No_Header` to be pool-backed
    too, so that all SAMB-style arenas behave uniformly with the carve-out as
    the only exception.
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
21. As a ReMoM developer, I want a compile-time check (`static_assert` or
    equivalent) that `POOL_SIZE >= sum_of_in_scope_sizes + margin`, so that
    pool sizing is validated at build time and forgetting to grow it cannot
    silently ship.
22. As a HeMoM headless test runner, I want the same allocator behavior in
    HeMoM as in ReMoMber, so that headless and interactive runs do not
    diverge due to allocator differences.
23. As a future-maintainer, I want each `Allocate_Space_Heap` call site to
    carry a brief comment explaining why that arena is carve-out, so that
    the pool/heap boundary remains auditable and intentional over time.
24. As a Phase-2 implementer, I want the MVP's pool layout to be compatible
    with future ASan sub-block annotation (deterministic offsets, sentinel
    slack, no compile-time obstacles), so that the suballocation/shadow-memory
    tracking work can layer on without a second refactor.

## Implementation Decisions

- **New module `Allocate_Pool`** (in MoX/src/). Deep module; thin public
  interface. Owns the static byte array and the bump-pointer state. Public
  API: `Pool_Init`, `Pool_Carve`, `Pool_Bytes_Used`, `Pool_Bytes_Free`. No
  knowledge of SAMB headers or any MoM concept.
- **Pool storage**: single `static uint8_t` array in BSS, size
  `POOL_SIZE` set at compile time as the sum of in-scope `Allocate_Space`
  sizes plus a fixed safety margin in the 4-16 KB range.
- **Sentinel pattern**: `Pool_Init` fills the entire pool with `0xCC`. Runs
  once at startup. Sub-block writes overwrite their own bytes; slack stays at
  the sentinel.
- **`Allocate.c` public API unchanged**:
  - `Allocate_Space(size)` becomes pool-backed via `Pool_Carve((size+1)*16)`,
    then writes the same SAMB header (`_AAAA` sentinel, `MEMSIG1`, `MEMSIG2`,
    `SIZE=size`, `USED=1`) it does today.
  - `Allocate_Space_No_Header(size)` becomes pool-backed (no header writes).
  - `Allocate_First_Block`, `Allocate_Next_Block`, `Check_Allocation`,
    `Get_Free_Blocks`, `Mark_Block`, `Release_Block`, `Reset_First_Block`
    are unchanged — they only operate inside an already-allocated arena
    header.
- **New function `Allocate_Space_Heap(size)`** exposed alongside
  `Allocate_Space`. Identical implementation to today's `malloc`-based
  `Allocate_Space`. Used by the carve-out call sites.
- **Carve-out call sites** (~10 sites), edited to call
  `Allocate_Space_Heap`:
  - Video2.c: `video_memory` and the 4×16000 + 4×64000 PR buffers.
  - SOUND.c: `state_table`, `timbre_cache`, `timb_seg`.
  - ALLOC.c: `EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`,
    `g_graphics_cache_seg`.
- **`_screen_seg` moves to pool**. Its sub-blocks (`near_buffer_save`,
  `help_pict_seg`, `IMG_SBK_PageText`) automatically come with it via the
  existing `Allocate_First_Block` / `Allocate_Next_Block` calls.
- **CI byte injection unchanged**. `gd_ci_inject_world_overrun` and any
  future companion injection hooks compute slack addresses from base
  pointers; those pointers now point into the pool instead of into malloc,
  but the API and the injected byte patterns are unchanged.
- **Pool exhaustion** invokes `Allocation_Error` (matching the existing
  fatal-on-allocator-failure pattern). Caller never sees a NULL return.
- **Build-time sanity**: `static_assert` that `POOL_SIZE >= sum_of_known_sizes
  + safety_margin`. The sum is expressed as a compile-time constant in a
  central header; adding a new pool-backed `Allocate_Space` call site adds
  its size to that constant, forcing the pool to grow.
- **No public header changes** beyond adding `Allocate_Space_Heap`'s
  declaration and the `Allocate_Pool` module's header. The SAMB struct, the
  paragraph constants, the sentinel values, and every existing sub-block
  layout stay identical.
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
- **Video2.c arenas**: `video_memory` and the per-page buffers stay on
  `malloc` via `Allocate_Space_Heap`.
- **SOUND.c arenas**: `state_table`, `timbre_cache`, `timb_seg` stay on
  `malloc`.
- **Large graphics arenas in ALLOC.c**: `EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`,
  `GfxBuf_2400B`, `g_graphics_cache_seg` stay on `malloc`.
- **ASan sub-block tracking** via `__asan_poison_memory_region` /
  `__sanitizer_annotate_contiguous_container`: Phase 2.
- **Build-flag-gated ASan sub-block detection**: Phase 2.
- **Dynamic pool resizing**: pool size is compile-time fixed.
- **Per-arena slack inside the pool**: arenas pack tightly; an OOB write
  from one arena spills into the next arena's start or into the trailing
  safety margin. Per-arena padding is Phase 2 if needed.
- **`Allocate_Dos_Space` / `Allocate_Dos_Data_Space`**: DOS-only wrappers
  with minimal use in ReMoM; left as-is.

## Further Notes

- **BSS bloat**: in-scope sum is approximately ~150-170 KB. With a 4-16 KB
  margin, the static pool is well under 200 KB. Acceptable for a modern build.
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
