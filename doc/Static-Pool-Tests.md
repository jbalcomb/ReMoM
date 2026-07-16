# Static Pool — Test Catalog

Tests covering the static-pool-backed SimTex Space Alloc
(`doc/#AI_Plans/PLAN-Static-Pool-Allocator.md`). All are CTest-discovered and
run under a plain `ctest`.

| Target / test | Layer | What it covers |
|---|---|---|
| `test_Allocate_Pool` (`AllocatePoolTest.*`, 11 tests) | Pool module, isolated | `Pool_Init` sentinel fill; `Pool_Carve` adjacency, exact advance, determinism (`Reinit_Produces_Identical_Carve_Sequence`); leading-guard offset; fatal-on-exhaustion; trailing margin never handed out. Compiles `Allocate_Pool.c` standalone with a stubbed `Allocation_Error` — no MOX/MoM dependency. |
| `MOX_tests` → `AllocateTest.*` (26 tests) | `Allocate.c` over the pool | Pool-backed `Allocate_Space` writes a valid SAMB header (`_AAAA`, `MEMSIG1/2`, `SIZE`, `USED`); `Allocate_First_Block` / `Allocate_Next_Block` carve correctly inside a pool arena; `Mark`/`Release`/`Reset`/`Get_Free_Blocks`; **`Allocate_Space_Is_Pool_Backed`** (cursor advances by `(size+1)*16`); **`Sentinel_Slack_In_Sub_Block_Data_Stays_0xCC`** (unwritten sub-block data keeps the sentinel). Near-buffer allocator too. |
| `HeMoM_OOB_Autotiling` | End-to-end (HeMoM) | The OG-faithful OOB read at `p_world_map[1][40][x]` is crash-free and deterministic. Pins the plane-1 last-row tiles. See `doc/HeMoM-OOB-Autotiling-Harness.md`. |
| `HeMoM_WorldGen_Run` / `_Validate` / `_Fields` | End-to-end (HeMoM) | Pre-existing world-gen regression gate (byte-parity of curated save fields) — exercises the pool-backed allocator through full world generation. |

## Running

```
# Everything pool-related, from the CTest build dir:
ctest -R "AllocatePoolTest|AllocateTest|HeMoM_(WorldGen|OOB)" -C <Debug|Release> --output-on-failure

# Or the module + integration units on their own binaries:
./bin/<cfg>/test_Allocate_Pool
./bin/<cfg>/MOX_tests --gtest_filter="AllocateTest.*:AllocatePoolTest.*"
```

## Notes on what these do and don't prove

- **Determinism** is covered at two layers: `Reinit_Produces_Identical_Carve_Sequence`
  (pool cursor) and the verified byte-identical `HeMoM_WorldGen`/`OOB` output
  (full `Allocate_Data_Space` call order).
- **Sizing** — `POOL_ARENA_CAPACITY` is a measured value (~4.58 MiB peak → 16 MiB);
  a C90 negative-array assert enforces the floor at compile time. `Pool_Bytes_Peak()`
  is logged at HeMoM shutdown for re-measurement.
- **Deferred** (tracked in the plan, not yet built): running the suite under an
  ASan build; a HeMoM ↔ ReMoMber pool-layout parity test (needs a ReMoMber
  harness); a compile-failure test that the capacity assert fires when
  deliberately undersized.
