# Plan: Static-Pool-Backed SimTex Space Alloc

> Source PRD: `doc/#AI_Plans/PRD-Static-Pool-Allocator.md` — GitHub issue #17

## Architectural decisions

Durable decisions that apply across all phases:

- **Public allocator API surface (unchanged)**: `Allocate_Space`,
  `Allocate_Space_No_Header`, `Allocate_First_Block`, `Allocate_Next_Block`,
  `Check_Allocation`, `Get_Free_Blocks`, `Mark_Block`, `Release_Block`,
  `Reset_First_Block`. Their signatures and return semantics do not change.
- **New public functions**: the `Allocate_Pool` module's API
  (`Pool_Init`, `Pool_Carve`, `Pool_Bytes_Used`, `Pool_Bytes_Free`,
  `Pool_Bytes_Peak`). No new `Allocate_*` wrapper — excluded arenas call
  `malloc()` directly.
- **Pool storage shape**: one `static uint8_t` array in BSS, owned by the
  new `Allocate_Pool` module. `POOL_SIZE = POOL_LEADING_GUARD (1 KB) +
  POOL_ARENA_CAPACITY + POOL_FIXED_MARGIN (8 KB)`. The leading guard absorbs
  `-1`-style underruns off the first arena; the trailing margin absorbs
  overruns off the last. `Pool_Carve` never hands out guard bytes; the first
  carve returns `pool_base + POOL_LEADING_GUARD`. `POOL_ARENA_CAPACITY` is set
  from the **measured** peak, not an enumerated sum (see Phase 2b): 16 MB, with
  `POOL_MIN_ARENA_BYTES` (5 MB) the asserted floor. A C90 negative-array
  compile-time assertion enforces `POOL_ARENA_CAPACITY >= POOL_MIN_ARENA_BYTES`
  (these `.c` files build as C90, so `_Static_assert` is unavailable). Arenas
  between neighbors get incidental OOB coverage in both directions from tight
  packing.
- **Sentinel pattern & lazy init**: `0xCC` byte fill performed exactly once, at
  `Pool_Init`, which `Pool_Carve` calls **lazily** on first use (static-flag
  guarded) — no explicit startup call or call-ordering assumption. Safe because
  each top-level allocation sequence (`Allocate_Data_Space`, STU world-gen's
  `Allocate_Game_Data`) runs once per process and the OG allocator never frees.
  Sub-block writes overwrite their own bytes; slack stays at sentinel until
  explicitly written (or until `gd_ci_inject_*` stamps OG-captured bytes into a
  known OOB region).
- **SAMB header layout (unchanged)**: 16-byte per-sub-block header with
  `_AAAA` / `_CCCC` sentinels, `MEMSIG1`, `MEMSIG2`, `SIZE`, `USED`,
  `MARK` fields. Same field offsets, same sentinel values.
- **Pool/heap boundary**: video and EMS/graphics-bank arenas are not
  System RAM and call `malloc()` directly (no SAMB header, sized in bytes).
  Specifically: Video2.c (`video_memory` + per-page buffers), ALLOC.c
  (`EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`). Two SAMB-shaped /
  conventional-RAM cases are *not* on this list and stay pool-backed:
  `g_graphics_cache_seg` (a SAMB arena — `Allocate_First_Block` in
  `Graphics_Cache_Reset()`), and the AIL/XMIDI audio buffers `state_table`
  / `timbre_cache` / `timb_seg` in `Audio_Init__WIP` (SOUND.c). Every other
  in-scope `Allocate_Space` call site moves to pool transparently.
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
the three 1x and three XBGR-2x page buffers, indices 1-3), ALLOC.c
(`EmmHndl_FIGUREX`, `EmmHndl_TILEXXX`, `GfxBuf_2400B`). Each of these
arenas is a flat buffer that is never sub-divided and never reads its SAMB
header, so the `malloc()` is sized in bytes to the buffer's real
requirement and drops both the SAMB header and the paragraph `+1` rounding
that `Allocate_Space` applied. Each edited call site gets a brief comment
explaining why it bypasses the pool (VGA hardware / EMS bank). Two cases
are deliberately left on `Allocate_Space` and ride the pool:
`g_graphics_cache_seg` (a SAMB arena — `Allocate_First_Block` in
`Graphics_Cache_Reset()`), and the AIL/XMIDI audio buffers `state_table`
/ `timbre_cache` / `timb_seg` in `Audio_Init__WIP` (SOUND.c;
conventional-RAM driver tables, WIP path). At the end of this phase,
`Allocate_Space` is still malloc-backed for the remaining call sites; no
game behavior changes. The phase exists to make the pool/heap boundary
explicit and auditable before the implementation swap.

### Acceptance criteria

- [ ] All excluded call sites in Video2.c and ALLOC.c allocate via
  `malloc()` directly, sized in bytes to the buffer's requirement.
- [ ] `g_graphics_cache_seg` and the `Audio_Init__WIP` audio buffers
  (`state_table`, `timbre_cache`, `timb_seg`) still use `Allocate_Space`
  (unchanged).
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
unchanged; only the underlying memory source changes. `Pool_Init` runs
**lazily** on the first `Pool_Carve` (no explicit startup wiring needed).
`POOL_ARENA_CAPACITY` is sized from the **measured** peak, not an enumerated
compile-time sum: the enumerated arenas are ~1.5 MB (`g_graphics_cache_seg`
alone ~1 MB), but the real full-game-entry peak — including run-time-sized
one-time `sa_Single` LBX graphic loads — is ~4.58 MB, measured via
`Pool_Bytes_Peak` (logged at HeMoM shutdown). Capacity is set to 16 MB with a
5 MB asserted floor. Excluded arenas are unaffected — they were moved to direct
`malloc()` in Phase 2a. After this phase, every formerly-pool-bound OOB access
in either direction lands inside the static pool's bounds — overruns in the
next arena's head or the trailing margin, `-1`-style underruns in the previous
arena's tail or the leading guard — ASan-detectable parent-malloc violations
from in-scope arenas stop, and the byte-compare pipeline continues to pass.

### Acceptance criteria

- [x] `Allocate_Space` and `Allocate_Space_No_Header` allocate from the
  pool; SAMB header bytes match what the pre-refactor implementation
  wrote. *(WorldGen byte-parity + HeMoM_Continue_Assertions green.)*
- [x] `Pool_Init` runs exactly once per process, **lazily** on the first
  `Pool_Carve` (before the first allocation completes).
- [x] `POOL_ARENA_CAPACITY` is set from the measured peak (~4.58 MB → 16 MB),
  with `POOL_MIN_ARENA_BYTES` (5 MB) the floor; a C90 negative-array
  compile-time assert enforces `POOL_ARENA_CAPACITY >= POOL_MIN_ARENA_BYTES`.
- [x] The first pool-backed arena sits at `pool_base + LEADING_GUARD`,
  so a `-1`-style underrun off it lands in sentinel-filled pool memory
  rather than before the static array.
- [ ] ReMoMber starts, reaches the Main Screen, plays a turn without
  crashing. *(unverified — no interactive ReMoMber run yet.)*
- [x] HeMoM run completes through Simtex_Autotiling and reaches the
  post-CRP_NEWG_CreatePathGrids dump point without crashing.
  *(HeMoM_WorldGen_Run green.)*
- [x] All `gd_dump_*` comparison points that were green before this
  phase remain green after. *(WorldGen_Validate / _Fields green.)*
- [ ] No new ASan or sanitizer crashes inside in-scope arenas.
  *(unverified — not yet run under a sanitizer build.)*
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

A named CTest harness (`HeMoM_OOB_Autotiling`) that drives a new game
through Simtex_Autotiling's `wy=39 -> wy+1=40` OOB read site and asserts the
result is crash-free and deterministic. It reuses the `HeMoM_WorldGen`
fixture's `SAVE9.txt` and `check_save_fields`, pinning the 60 plane-1 (Myrror)
last-row tiles (`world_map[1][40*wx+39]`) — the squares whose autotiling
consumes the OOB `p_world_map[1][40][x]` read — to their seed-pinned values in
`tests/assert_oob_autotiling.txt`. A pool/inject regression (crash, garbage
slack, changed layout) perturbs those values.

**Realized scope vs. original plan.** The plan assumed a direct
`byte == injected value` diff via `gd_dump_*`, but three facts make that a
CI-only concern rather than a repo-reproducible test: (a) the injected values
come from an untracked capture (`og-game-data-capture.fwv`); (b) the wy=40
slack is never written to the save dump; (c) `gd_dump_world_map` is
`LOG_DEBUG`, stripped from Release. So the harness proves the read is
**crash-free and deterministic end-to-end** (transitively covering the
inject/pool path), and the direct byte diff remains a CI/`.fwv` matter.

### Acceptance criteria

- [x] Named HeMoM test boots a new game and runs through Simtex_Autotiling
  (the `121_Simtex_Autotiling_W` checkpoint) — `HeMoM_OOB_Autotiling` reusing
  the `HeMoM_WorldGen_Run` fixture.
- [x] Runs to completion without crashing. *(Under ASan: not yet — no
  sanitizer build wired; deferred with the other ASan items.)*
- [x] The OOB-dependent output is deterministic and pinned: the plane-1
  last-row tiles match the seed-pinned baseline run-to-run (verified: two
  fresh runs byte-identical). *Direct `byte == injected value` diff is a
  CI-only concern per Realized scope above.*
- [x] The harness is runnable as a named CTest test
  (`ctest -R HeMoM_OOB_Autotiling`), auto-running its `HeMoM_WorldGen` fixture.
- [x] Documentation: `doc/HeMoM-OOB-Autotiling-Harness.md` describes the
  harness, what it asserts, the scope caveat, and how to run it standalone.

---

## Phase 4: Test infrastructure consolidation

**User stories**: 16, 17, 18, 20, 21, 24

### What to build

Consolidate the Phase 1-3 test artifacts into one CTest-discoverable suite and
fix what Phase 2b silently broke. The pool-backed swap turned
`test_Allocate.cpp`'s `Allocate_Space` returns into pool-interior pointers, so
its `free()` calls became undefined behaviour (the suite crashed on the first
case). That file **is** the pool-backed `Allocate_Space` integration test
(stories 16/17): reset the pool per test (`Pool_Init` in `SetUp`), drop the
`free()`s, and add pool-specific cases — pool-residency (`Pool_Bytes_Used`
advances by `(size+1)*16`) and sentinel slack inside a sub-block (story 18).
`test_Allocate_Pool` (Phase 1) and `HeMoM_OOB_Autotiling` (Phase 3) are already
CTest tests. Determinism and `gd_dump_*` regression are covered by existing
tests rather than new bespoke ones (see criteria). Document the catalog.

### Acceptance criteria

- [x] `ctest` discovers and runs `test_Allocate_Pool` (11 `AllocatePoolTest`),
  the pool-backed `Allocate_Space` integration tests (26 `AllocateTest` in
  `MOX_tests`), and the OOB harness (`HeMoM_OOB_Autotiling`) as one suite.
- [x] Layout determinism: covered at the pool layer by
  `AllocatePoolTest.Reinit_Produces_Identical_Carve_Sequence` and end-to-end by
  the verified byte-identical `HeMoM_WorldGen`/`OOB` output (full
  `Allocate_Data_Space` call order). *(No bespoke per-arena offset dump — the
  two existing layers cover it.)*
- [ ] HeMoM ↔ ReMoMber pool-layout parity test. *(Deferred — needs a ReMoMber
  harness to expose pool offsets; ReMoMber is the windowed target.)*
- [x] `gd_dump_*` regression check: `HeMoM_WorldGen_Fields` / `_Validate` and
  `HeMoM_OOB_Autotiling` fail the suite if their pinned save fields diverge.
- [~] Capacity assert (`POOL_ARENA_CAPACITY >= POOL_MIN_ARENA_BYTES`) is present
  and reachable from the test build (compiled into `Allocate_Pool.c`).
  *(A dedicated compile-failure test that it fires on a deliberately-undersized
  pool is deferred — needs a `try_compile`-expect-fail harness.)*
- [x] Phase-2 readiness: sub-block offsets are stable (determinism tests) and
  sentinel slack inside a sub-block is intact post-normal-run
  (`AllocateTest.Sentinel_Slack_In_Sub_Block_Data_Stays_0xCC`); no compile-time
  obstacle to future `__asan_poison_memory_region` annotation.
- [x] Project gate runs the suite: the repo `pre-push` hook / VS Code
  `check: safe-to-push` task builds Debug+Release and runs the suite on
  main-bound pushes. *(No new GitHub Actions workflow added.)*
- [x] Documentation: `doc/Static-Pool-Tests.md` (test catalog) and
  `doc/HeMoM-OOB-Autotiling-Harness.md` describe what each target covers and how
  to run it.

---

## Phase 5a: Remove the OOB-avoidance scaffolding

**User stories**: supersedes 7 (previously "gd_ci_inject works unchanged over
the pool"); realizes the pool's original purpose — OG-faithful OOB accesses no
longer need external help to be safe.

### What to build

The static pool now makes every OG-faithful OOB access land in addressable,
deterministic memory on its own. So the two mechanisms that predated it and were
kept "unchanged" through Phases 1-4 — the `WORLD_OVERFLOW` over-allocation
padding and the `gd_ci_inject_*` value injection — are now redundant scaffolding
and get removed. **Decision (locked):** the OOB reads will be backed by the
pool's own deterministic content (sentinel / neighbouring arena), not by an
injected OG capture. This trades OG-*value* fidelity in the OOB regions for a
much smaller, self-contained system; OG-*behavior* fidelity (the access happens,
against addressable memory) is preserved by the pool.

This phase is the **code change only**. It will turn the OOB-dependent goldens
red — that is expected and is repaired in Phase 5b. 5a's own gates are
no-crash + determinism + capacity, not byte-parity.

Concretely:

- **Remove the `WORLD_OVERFLOW` padding** from the pool-backed arenas —
  `_world_maps`, `_map_square_terrain_specials`, `_map_square_flags` (ALLOC.c),
  and the STU world-gen equivalents (STU_WRLD.c). Arenas are sized to their
  exact valid extent; the OOB address (a fixed offset from the arena base — the
  access site is unchanged) now lands in the next pool arena or the trailing
  margin.
- **Remove the OOB injectors** — the `gd_ci_inject_world_overrun` (ALLOC.c:111,
  MAPGEN.c:3918/6258/7040) and `gd_ci_inject_flags_overrun` (MAPGEN.c:6028) call
  sites and their definitions/declarations (INITGAME.c/.h). The pool supplies
  the bytes.
- **Remove superseded crash-only scaffolding** — e.g. the old defensive
  memset-zero of the overflow region that inject already replaced.
- **Leave the OOB accesses themselves alone.** The `OGBUG … OOB AVRL/AVWL`
  reads (MAPGEN.c:4312/4328/4349/6141/6298/7044/7046, AISPELL.c:5738-5740,
  AIMOVE.c, AIDATA.c, CITYCALC.c) are the faithful behavior the pool exists to
  support — untouched.
- **Out of scope:** the `gd_ci_*` load/get subsystem's **uninitialized-stack-auto**
  injection (`gd_ci_get` for `centroid` in AIMOVE.c and `tries` in MAPGEN.c —
  seeding locals OG left uninitialized) is a separate fidelity concern, not
  OOB-avoidance. Keep the subsystem if those consumers remain; a later phase can
  revisit it. (If removing the injectors leaves `gd_ci_get` as the only consumer,
  note that rather than silently
  deleting shared code.)

### Risks / watch-items

- **Determinism must hold after the shift.** With the padding gone the OOB lands
  in a *neighbouring arena* whose contents may be written during generation
  (e.g. `connectivity_grid_land` follows `_world_maps`). Verify the value is
  deterministic run-to-run and, ideally, that it reads sentinel (not
  gen-order-dependent live data) at the read point — if it turns out
  gen-order-dependent, decide whether that's acceptable or whether a small
  guard region is warranted.
- **Layout shift is global.** Dropping padding moves every subsequent arena's
  pool offset, so re-measure `Pool_Bytes_Peak` and expect broad re-baselining
  (done in 5b).

### Acceptance criteria

- [x] `WORLD_OVERFLOW` / `+60+1` padding removed from the pool-backed arena
  allocations (ALLOC.c + STU_WRLD.c); arenas sized to their exact valid extent
  (`_world_maps` 602 PR, `_map_square_*` 302 PR).
- [x] All `gd_ci_inject_world_overrun` / `gd_ci_inject_flags_overrun` call sites,
  definitions, and declarations removed; repo-wide `*.{c,h}` grep finds no
  references.
- [x] The OOB reads still land inside the pool (no fault) and are deterministic
  run-to-run — two fresh WorldGen runs produced byte-identical `world_map`
  output; both exit 0; no `Insufficient memory`.
- [x] `gd_ci_*` load/get subsystem retained (still used by the two
  uninitialized-stack-auto consumers: `centroid` in AIMOVE.c, `tries` in
  MAPGEN.c), with a note in INITGAME.c/.h.
- [x] `POOL_ARENA_CAPACITY` re-checked: post-shift WorldGen peak 3.56 MB, well
  under the 16 MB capacity / 5 MB floor — no change needed.
- [x] Builds clean (Debug + Release); no new crashes under a normal run.

**Outcome:** the byte-parity goldens stayed **green**, not red — the OG capture
`og-game-data-capture.fwv` is absent in this repo, so `gd_ci_inject` was already
a no-op (`[CI] load FAILED … No injection`) and the goldens already reflected
sentinel-backed OOB reads. Removing the inert inject + padding left the pinned
outputs unchanged (the OOB still lands on deterministic sentinel; the layout
shift didn't perturb them). So Phase 5b's golden re-baseline is a **no-op in this
repo** — its value is the doc/PRD reconciliation (much of which is already done)
and a note that a `.fwv`-present CI environment would re-baseline for real.

---

## Phase 5b: Re-baseline and reconcile docs

**User stories**: 20 (`gd_dump_*` comparison points stay green — re-established
against the new pool-backed baseline).

### What to build

Repair everything Phase 5a's code change perturbed: re-baseline the goldens and
field assertions against 5a's new deterministic output, review the sanitizer
suppressions, and reconcile the docs and PRD that still describe the retired
inject mechanism. Purely mechanical + documentation — no behavior change beyond
5a.

### Acceptance criteria

- [x] Goldens/assertions green — **re-baseline was a no-op** (see 5a outcome:
  `.fwv` absent, so removing the inert inject + padding didn't move any pinned
  output): `assert_worldgen.txt`, `assert_oob_autotiling.txt`,
  `assert_continue_save.txt`, `assert_continue_next_turn.txt`,
  `assert_ai_5turns.txt` all pass, and `HeMoM_WorldGen_Validate` confirms the
  save size is unchanged (123300).
- [~] `valgrind-suppressions-all.supp` — **not present in this worktree** (no
  `*.supp` anywhere), so there was nothing to review/drop. If it exists in the
  main tree, re-check there when merging.
- [x] Docs reconciled: `MAPGEN-Simtex_Autotiling.md`,
  `HeMoM-OOB-Autotiling-Harness.md` (and `Static-Pool-Tests.md`, which had no
  stale refs) describe the pool as the OOB backing. Also reconciled the three
  sibling walkthroughs that referenced the padding —
  `MAPGEN-Generate_Terrain_Specials.md`, `MAPGEN-Generate_Roads.md`,
  `MAPGEN-Animate_Oceans.md` (retired-padding framing marked *Historical*).
- [x] PRD reconciled (done in the earlier PRD/plan pass): user story 7 and the
  "CI byte injection" decision reflect the pool-backed OOB and point to Phase 5.
- [x] Full test suite green (`ctest`, 48/48 on Release; Golden disabled); OOB
  harness re-baseline confirmed
  deterministic across two fresh runs.
