# Plan: Static-Pool-Backed SimTex Space Alloc

> Source PRD: `doc/#AI_Plans/PRD-Static-Pool-Allocator.md` — GitHub issue #17

## Architectural decisions

Durable decisions that apply across all phases:

- **Public allocator API surface (unchanged)**: `Allocate_Space`,
  `Allocate_Space_No_Header`, `Allocate_First_Block`, `Allocate_Next_Block`,
  `Check_Allocation`, `Get_Free_Blocks`, `Mark_Block`, `Release_Block`,
  `Reset_First_Block`. Their signatures and return semantics do not change.
- **New public functions**: the `Allocate_Pool` module's API
  (`Pool_Init`, `Pool_Carve`, `Pool_Bytes_Used`, `Pool_Bytes_Free`). No
  new `Allocate_*` wrapper — excluded arenas call `malloc()` directly.
- **Pool storage shape**: one `static uint8_t` array in BSS, owned by the
  new `Allocate_Pool` module. Size is a compile-time constant
  (`POOL_SIZE`) equal to the sum of in-scope `Allocate_Space` sizes plus
  safety margins at both ends: `LEADING_GUARD` (256 B - 4 KB) reserved
  ahead of the first carve, so `-1`-style underruns off the first arena
  stay inside the pool, and a trailing `FIXED_MARGIN` (4-16 KB) for
  overruns off the last arena. `Pool_Carve` never hands out guard bytes;
  the first carve returns `pool_base + LEADING_GUARD`. `static_assert` at
  build time enforces sufficiency. Arenas between neighbors get incidental
  OOB coverage in both directions from tight packing.
- **Sentinel pattern**: `0xCC` byte fill performed exactly once at
  `Pool_Init`. Sub-block writes overwrite their own bytes; slack stays
  at sentinel until explicitly written (or until `gd_ci_inject_*` stamps
  OG-captured bytes into a known OOB region).
- **SAMB header layout (unchanged)**: 16-byte per-sub-block header with
  `_AAAA` / `_CCCC` sentinels, `MEMSIG1`, `MEMSIG2`, `SIZE`, `USED`,
  `MARK` fields. Same field offsets, same sentinel values.
- **Pool/heap boundary**: video, audio, and EMS/graphics-bank arenas are
  not System RAM and call `malloc()` directly (no SAMB header, sized in
  bytes). Specifically: Video2.c (`video_memory` + per-page buffers),
  SOUND.c (`state_table`, `timbre_cache`, `timb_seg`), ALLOC.c
  (`EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`).
  `g_graphics_cache_seg` is *not* on this list — it is a SAMB arena
  (`Allocate_First_Block` in `Graphics_Cache_Reset()`) and stays
  pool-backed. Every other in-scope `Allocate_Space` call site moves to
  pool transparently.
- **Pool exhaustion**: invokes `Allocation_Error` (matching existing
  fatal-on-allocator-failure pattern). Caller never sees a NULL return.
- **CI byte injection invariant**: `gd_ci_inject_world_overrun`, its
  existing companion `gd_ci_inject_flags_overrun`, and any future
  OOB-region inject hooks compute slack addresses from base pointers.
  The base pointers now point into the static pool instead of into a
  malloc block; the inject API and the OG-captured byte payloads are
  unchanged.
- **HeMoM ↔ ReMoMber parity**: both targets consume MoX/src/Allocate.c
  and the new `Allocate_Pool` module identically. No per-target
  conditional behavior.

---

## Phase 1: Pool module standalone

**User stories**: 12, 13, 14, 15

### What to build

A new `Allocate_Pool` module that owns the static pool storage and the
bump-pointer state. Public API: `Pool_Init` (one-time sentinel-fill +
reset), `Pool_Carve(bytes)` (bump-pointer carve; fatal on exhaustion),
`Pool_Bytes_Used`, `Pool_Bytes_Free`. The module is independent of any
MoM concept and has no knowledge of SAMB headers; it just hands out
contiguous byte regions from a fixed-size array. Pool size is a
compile-time constant defined in the module's header; for this phase a
generous placeholder size is acceptable since the real sum-of-in-scope
constant is finalized in Phase 2b. Unit tests verify external behavior
end-to-end against the module's public API alone — no integration with
`Allocate.c` yet.

### Acceptance criteria

- [ ] `Pool_Init` fills the entire pool with sentinel byte `0xCC`,
  observable via byte reads at any offset — including the leading guard
  and trailing margin.
- [ ] The first `Pool_Carve` after `Pool_Init` returns
  `pool_base + LEADING_GUARD`; guard bytes are never handed out and stay
  at the sentinel until explicitly written.
- [ ] `Pool_Carve(N)` returns a pointer that advances the next-free
  cursor by exactly N bytes; back-to-back carves return adjacent
  addresses.
- [ ] `Pool_Carve` that would exceed remaining capacity invokes
  `Allocation_Error` (fatal) and does not return.
- [ ] `Pool_Bytes_Used` and `Pool_Bytes_Free` reflect cumulative carve
  state; their sum equals total pool size.
- [ ] Determinism: same sequence of `Pool_Carve` calls in two
  back-to-back process runs returns identical offsets relative to the
  pool base.
- [ ] Unit tests for the above live in a new CTest target that builds
  and runs in isolation (no MoM library dependency).
- [ ] Module compiles and links into both HeMoM and ReMoMber without
  affecting either binary's runtime behavior (it's wired in but
  nothing else calls it yet).

---

## Phase 2a: Route excluded arenas to malloc (no behavioral change)

**User stories**: 4, 5, 9, 23

### What to build

Edit the excluded (non-System-RAM) call sites to allocate directly via
`malloc()` instead of `Allocate_Space` — Video2.c (`video_memory` plus
the three 1x and three XBGR-2x page buffers, indices 1-3), SOUND.c
(`state_table`, `timbre_cache`, `timb_seg`), ALLOC.c (`EmmHndl_FIGUREX`,
`EmmHndl_TILEXXX`, `GfxBuf_2400B`). Each of these arenas is a flat
buffer that is never sub-divided and never reads its SAMB header, so the
`malloc()` is sized in bytes to the buffer's real requirement and drops
both the SAMB header and the paragraph `+1` rounding that `Allocate_Space`
applied. Each edited call site gets a brief comment explaining why it
bypasses the pool (VGA hardware / sound driver / EMS bank).
`g_graphics_cache_seg` is deliberately left on `Allocate_Space` — it is a
SAMB arena (`Allocate_First_Block` in `Graphics_Cache_Reset()`) and rides
the pool. At the end of this phase, `Allocate_Space` is still malloc-backed
for the remaining call sites; no game behavior changes. The phase exists to
make the pool/heap boundary explicit and auditable before the
implementation swap.

### Acceptance criteria

- [ ] All excluded call sites in Video2.c, SOUND.c, and ALLOC.c allocate
  via `malloc()` directly, sized in bytes to the buffer's requirement.
- [ ] `g_graphics_cache_seg` still uses `Allocate_Space` (unchanged).
- [ ] Every edited call site has an inline comment explaining why it
  bypasses the pool.
- [ ] ReMoMber boots to the Main Screen with no behavior change.
- [ ] Matchup pipeline runs to completion with all `gd_dump_*`
  comparison points still green.
- [ ] HeMoM headless smoke run completes with no regression.
- [ ] No new ASan crashes; no fewer either.

---

## Phase 2b: Pool-backed swap

**User stories**: 1, 2, 3, 6, 8, 10, 11, 22, 25

### What to build

Wire `Allocate_Space(size)` and `Allocate_Space_No_Header(size)`
through `Pool_Carve` instead of `malloc`. The SAMB header writes
(`_AAAA` sentinel, `MEMSIG1`, `MEMSIG2`, `SIZE`, `USED=1`) are
unchanged; only the underlying memory source changes. `Pool_Init` is
called once at startup, before any `Allocate_Space` call. `POOL_SIZE`
is finalized as the compile-time sum of every in-scope
`Allocate_Space` call's `size` argument plus the leading guard
(256 B - 4 KB) and the trailing safety margin (4-16 KB); a
`static_assert` guards the sufficiency (`POOL_SIZE` includes
`g_graphics_cache_seg`'s ~1 MB). Excluded arenas are unaffected — they
were moved to direct `malloc()` in Phase 2a. After this phase, every
formerly-pool-bound OOB access in
either direction lands inside the static pool's bounds — overruns in
the next arena's head or the trailing margin, `-1`-style underruns in
the previous arena's tail or the leading guard — ASan-detectable
parent-malloc violations from in-scope arenas stop, and the matchup
byte-compare pipeline continues to pass.

### Acceptance criteria

- [ ] `Allocate_Space` and `Allocate_Space_No_Header` allocate from the
  pool; SAMB header bytes match what the pre-refactor implementation
  wrote.
- [ ] `Pool_Init` runs exactly once at startup, before the first
  `Allocate_Space` call.
- [ ] `POOL_SIZE` is the compile-time sum of every in-scope
  `Allocate_Space` call's `size` argument plus `LEADING_GUARD` and the
  trailing safety margin; `static_assert` enforces sufficiency.
- [ ] The first pool-backed arena sits at `pool_base + LEADING_GUARD`,
  so a `-1`-style underrun off it lands in sentinel-filled pool memory
  rather than before the static array.
- [ ] ReMoMber starts, reaches the Main Screen, plays a turn without
  crashing.
- [ ] HeMoM matchup-seed run completes through Simtex_Autotiling and
  reaches the post-CRP_NEWG_CreatePathGrids dump point without
  crashing.
- [ ] All `gd_dump_*` comparison points that were green before this
  phase remain green after.
- [ ] No new ASan or sanitizer crashes inside in-scope arenas.
- [ ] Integration tests verify: `Allocate_Space` returns a
  pool-resident pointer with a valid SAMB header;
  `Allocate_First_Block` and `Allocate_Next_Block` carve correctly
  inside a pool-backed arena.
- [ ] Sentinel pattern in slack between sub-blocks is preserved after
  normal sub-block writes (integration test).

---

## Phase 3: OOB end-to-end harness

**User stories**: 7, 19

### What to build

A HeMoM scenario script that drives a new game with the
`matchup_hemom` seed through Simtex_Autotiling's wp=1/wy=39 OOB read
site. The harness exists to empirically prove the refactor solved the
original problem: the OOB read no longer faults, and the byte at the
OOB address equals the value that `gd_ci_inject_world_overrun` stamped
into the slack past `_world_maps`. The CI inject infrastructure
itself is unchanged — this phase verifies it still works over the
pool-backed arena.

### Acceptance criteria

- [ ] HeMoM scenario script exists that boots a new game with the
  matchup seed and runs through the
  `19_Simtex_Autotiling_W` dump point.
- [ ] Scenario runs to completion without crashing, including under
  ASan.
- [ ] The byte at the OG OOB read address (wp=1, wy=40, wx=any matching
  the captured baseline) equals the value injected by
  `gd_ci_inject_world_overrun`, verified via the existing `gd_dump_*`
  channel.
- [ ] The harness is runnable from the matchup pipeline as a named
  scenario.
- [ ] Documentation: a one-paragraph note in `doc/` describing the
  harness, what it asserts, and how to run it standalone.

---

## Phase 4: Test infrastructure consolidation

**User stories**: 16, 17, 18, 20, 21, 24

### What to build

Consolidate the new test artifacts introduced in Phases 1-3 into a
discoverable, single-command-runnable test suite, and add the
cross-phase tests that don't fit naturally in earlier phases.
Specifically: wire `test_Allocate_Pool` and `test_Allocate_Space` (the
unit and integration test files from Phases 1 and 2b) into the
project's CTest target so `ctest` runs them alongside existing tests;
make the Phase 3 OOB harness invokable from the matchup pipeline as a
gate. Add new tests that span phases: pool-layout determinism (same
`Allocate_Data_Space` call order across two runs produces identical
pool offsets and identical sentinel content in slack), HeMoM ↔
ReMoMber parity (both targets observe identical pool layout
post-init), and a `gd_dump_*` regression check that fails if any
previously-green comparison point goes red. Verify Phase-2 readiness
properties: deterministic offsets, sentinel slack between sub-blocks,
no compile-time obstacles to future ASan annotation.

### Acceptance criteria

- [ ] `ctest` discovers and runs `test_Allocate_Pool`,
  `test_Allocate_Space`, and the matchup-pipeline OOB harness as a
  single combined suite.
- [ ] Layout-determinism test: two back-to-back runs of
  `Allocate_Data_Space` produce identical pool offsets for every arena
  and identical sentinel content in every slack region.
- [ ] HeMoM ↔ ReMoMber parity test: both binaries report identical
  pool offsets for each in-scope `Allocate_Space` call after
  startup.
- [ ] `gd_dump_*` regression check: runs the matchup pipeline and
  fails the test suite if any previously-green comparison point
  diverges.
- [ ] `static_assert` for POOL_SIZE sufficiency is reachable from the
  test build and verified to fire on a deliberately-undersized pool.
- [ ] Phase-2 readiness verified: sub-block offsets are stable and
  exposable in a form usable by future `__asan_poison_memory_region`
  calls; sentinel slack between sub-blocks is intact post-normal-run.
- [ ] CI workflow (or equivalent project task) runs the consolidated
  suite on every push; gate fails block merging.
- [ ] Documentation update: brief section in the project README (or
  test directory README) describing what each test target covers.
