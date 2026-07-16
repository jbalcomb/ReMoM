# HeMoM OOB Autotiling Harness

*Phase 3 of the Static-Pool-Backed SimTex Space Alloc (see `doc/#AI_Plans/PLAN-Static-Pool-Allocator.md`).*

## What it proves

`Simtex_Autotiling` computes each square's autotile subtype from its eight
neighbours. For the south neighbours it reads `p_world_map[wp][wy + 1][wx]`
guarded only by the always-true `(wy < WORLD_HEIGHT)`
([MAPGEN.c:3987/3998/4011](../MoM/src/MAPGEN.c#L3987)). At the last row
`wy = 39` this reads `p_world_map[wp][40][wx]` — one row **past** the
`[2][40][60]` world-map array (`WORLD_HEIGHT = 40`). For plane 1 (Myrror) that
address is `_world_maps + 4800`, i.e. inside the `WORLD_OVERFLOW` slack past the
array: an OG-faithful out-of-bounds read.

Under the old per-arena `malloc`, that read could fault under ASan or
page-protection. With the static pool the slack is addressable, sentinel-filled,
and deterministic, and `gd_ci_inject_world_overrun`
([INITGAME.c:143](../MoM/src/INITGAME.c#L143)) stamps OG's captured bytes into
it — so the read no longer crashes and the generated world is reproducible.

## What it asserts

`tests/assert_oob_autotiling.txt` pins the 60 plane-1 last-row tiles
(`world_map[1][40*wx + 39]`, `wx = 0..59`) of the seed-pinned generated world to
their known-good values, checked by `check_save_fields` against the
`HeMoM_WorldGen` fixture's `SAVE9.txt`. These are the squares physically
adjacent to — and, during autotiling, computed from — the OOB slack. A
pool/inject regression (a crash, garbage in the slack, or a changed pool layout)
perturbs the OOB read and therefore these values, failing the test.

Scope note: the injected byte *values* come from an untracked CI capture
(`og-game-data-capture.fwv`), and the wy=40 slack itself is never written to the
save, so this harness proves the read is **crash-free and deterministic
end-to-end** rather than directly diffing the injected bytes. The direct
byte-for-byte comparison of the injected slack is a CI-only concern (it needs the
`.fwv` baseline and the Debug-only `gd_dump_world_map` log), covered transitively
by the deterministic output pinned here.

## Running it standalone

```
# Build (Release or Debug), then from the CTest build dir:
ctest -R "HeMoM_(WorldGen_Run|OOB_Autotiling)" -C Release --output-on-failure
```

`HeMoM_OOB_Autotiling` requires the `HeMoM_WorldGen` fixture (it reuses that
run's `SAVE9.txt`), so CTest runs `HeMoM_WorldGen_Run` first automatically.

To regenerate the baseline after an intentional world-gen change: run
`HeMoM_WorldGen_Run`, then extract the `world_map[1][sq]` lines with `sq % 40 == 39`
from `SAVE9.txt` into `tests/assert_oob_autotiling.txt`.
