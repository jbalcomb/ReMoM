MAPGEN-Init_Square_Explored.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Init_Square_Explored.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Init_Square_Explored.c

Init_New_Game()
    |-> Init_Square_Explored();

---

# `Init_Square_Explored` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Init_Square_Explored` | [MAPGEN.c:6687-6755](../../MoM/src/MAPGEN.c#L6687-L6755) | Sets the starting fog-of-war: clears the whole explored map (both planes), then reveals a rounded vision area around the **human** player's capital. |
| `Set_Square_Explored_Bits` | [MAPGEN.c:6771](../../MoM/src/MAPGEN.c#L6771) | Bounds-checked helper: `SET_SQUARE_EXPLORED(wx,wy,wp,bits)` — **overwrites** one tile's 4-corner explored byte. |

## Purpose

Each map tile carries a 4-bit "explored" byte — one bit per **sub-tile corner** (`SCT_BottomLeft / SCT_TopLeft / SCT_TopRight / SCT_BottomRight`), so fog-of-war can be revealed at corner granularity and rendered as a smooth circle. `Init_Square_Explored` blanks that byte for every tile on both planes, then lights up a roughly circular patch around the human capital so the player starts seeing their immediate surroundings.

**RNG-neutral: zero `Random()` calls** ([6687-6755](../../MoM/src/MAPGEN.c#L6687-L6755)) — it does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:643](../../MoM/src/MAPGEN.c#L643) | Late in new-game setup; followed by `gd_dump_square_explored("28_Init_Square_Explored_E")` ([644](../../MoM/src/MAPGEN.c#L644)). |

## How it works

1. **Clear** ([6695-6707](../../MoM/src/MAPGEN.c#L6695-L6707)) — triple loop over both planes × 40 × 60, `SET_SQUARE_EXPLORED(wx,wy,wp,ST_FALSE)`.
2. **Anchor on the human capital** ([6712-6714](../../MoM/src/MAPGEN.c#L6712-L6714)) — `wp/wx/wy = _FORTRESSES[HUMAN_PLAYER_IDX]`.
3. **3×3 full reveal** ([6718-6728](../../MoM/src/MAPGEN.c#L6718-L6728)) — the capital and its 8 neighbors get all four corners set (three `wx_offset` loops over rows `wy-1`, `wy`, `wy+1`).
4. **Distance-2 fringe + rounded diagonals** ([6731-6751](../../MoM/src/MAPGEN.c#L6731-L6751)) — the outer ring of the 5×5 area (the four edge runs at `±2`, omitting the far `±2,±2` corners) gets only the corners facing the capital, and the four `±1,±1` diagonal tiles are **re-written** with three corners (dropping the far corner) to round the circle.

`Set_Square_Explored_Bits` **assigns** (`SET_SQUARE_EXPLORED` is `= bits`, [MOX_DEF.h:473](../../MoX/src/MOX_DEF.h#L473)), so step 4's writes *overwrite* the full corners step 3 placed on the diagonal tiles — the OG comment ("the corners will be overwritten below too", [6717](../../MoM/src/MAPGEN.c#L6717)) refers to exactly this. The helper silently drops out-of-bounds tiles (no wrap), so a capital near an edge just reveals less.

The net reveal is a radius-2 vision circle: the 3×3 core fully lit, the four cardinal `±2` edge tiles lit on their near corners, the eight "knight" tiles lit on one corner, and the four diagonal tiles rounded to three corners each.

## Verification against the asm

- **No `Random()`** anywhere in the function.
- The OG asm makes **16** straight-line `Set_Square_Explored_Bits` calls (fringe + diagonals); production matches **all 16** operand-level — each call's corner bits and `wx`/`wy` offsets decoded from `Init_Square_Explored.asm` (file 121-320) and confirmed against production line-by-line.
- The clear loop and the 3×3 reveal loops match the asm structure.
- `Set_Square_Explored_Bits` bounds-check + `SET_SQUARE_EXPLORED` assign — matches `Set_Square_Explored_Bits.asm`.

## Sub-functions / external calls

- **`Set_Square_Explored_Bits`** ([MAPGEN.c:6771](../../MoM/src/MAPGEN.c#L6771)) — bounds-checked single-tile corner-bit write (assign).
- **`SET_SQUARE_EXPLORED`** ([MOX_DEF.h:473](../../MoX/src/MOX_DEF.h#L473)) — `_square_explored[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx] = bits`.
- **`_FORTRESSES[HUMAN_PLAYER_IDX]`** — the human capital location (anchor for the reveal).
- **`SCT_*`** corner-bit constants; `_square_explored` fog-of-war byte array.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Init_Square_Explored.asm` — IDA Pro 5.5 disassembly (authority); fringe/diagonal calls at file 121-320.
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Set_Square_Explored_Bits.asm` — the helper.
- [MAPGEN.c:643-644](../../MoM/src/MAPGEN.c#L643-L644) — call site + explored-map dump.
