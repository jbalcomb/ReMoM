MAPGEN-Generate_Lairs.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Lairs.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Lairs.c

Init_New_Game()
    |-> ... Generate_Towers / Extend_Islands ...
    |-> Generate_Lairs();                [MAPGEN.c:345]
        |-> Create_Lair()

---

# `Generate_Lairs` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Generate_Lairs` | [MAPGEN.c:2540-2666](../../MoM/src/MAPGEN.c#L2540-L2666) | Builds the encounter (`_LAIRS[]`) table: tower lairs on both planes, a lair at every node, then 25 strong + 32 random "weak" encounters on land, with a budget per lair. |

Verified faithful to the disassembly `Generate_Lairs.asm` throughout — structure and `Random()` sequence 1:1 — carrying one preserved OG quirk (`Create_Lair` arg5 is unused; see below).

## Purpose

Called once during map generation (after towers and island-extension). Populates `_LAIRS[]` via `Create_Lair`:

| `_LAIRS[]` range | count | source |
|---|---|---|
| 0-5 | 6 | tower lairs, Arcanus — `Create_Lair(.., lair_type 0, 650 + 50·Random(11))` |
| 6-11 | 6 | tower lairs, Myrror — `memcpy` of 0-5 with `wp = MYRROR` |
| 12-41 | 30 | one lair per node — `lair_type = node.type + 1`, budget `(power²·(4+Random(11))·(_magic+1))/2` |
| 42-66 | 25 | strong random lairs (`NUM_STRONG_LAIRS`) on land, ≥2 from any existing lair |
| 67-98 | 32 | weak random lairs (`NUM_WEAK_LAIRS`) on land, ≥2 from any existing lair |
| 99-101 | 3 | trailing records marked empty (`intact = ST_FALSE`) |

`Create_Lair` signature: `(lair_idx, wp, wx, wy, niu_create_lair_parameter, lair_type, budget)` — note **arg5 is "NIU" (not used)** and **arg6 is `lair_type`**.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:345](../../MoM/src/MAPGEN.c#L345) | Once, after `Generate_Towers` and `Extend_Islands`. |

## Code walk

Line refs are production [MAPGEN.c](../../MoM/src/MAPGEN.c); cross-checked against `Generate_Lairs.asm` (the authority for the `Create_Lair` argument order). `Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

### Tower lairs + Myrror mirror ([2556-2565](../../MoM/src/MAPGEN.c#L2556-L2565))

For the 6 towers, `Create_Lair(itr, ARCANUS, wx, wy, 3, 0, 650 + 50·Random(11))`. The asm pushes `budget, 0, 3, wy, wx, 0, itr` (right→left), confirming **arg5 = 3 (niu), arg6 = 0 (lair_type)**. Then `_LAIRS[6..11]` are `memcpy`'d from `[0..5]` with `wp = MYRROR_PLANE` (asm `_fmemcpy`, same). **Faithful.**

### Node lairs ([2567-2574](../../MoM/src/MAPGEN.c#L2567-L2574))

Per node: compute `niu_create_lair_parameter = ((((power-1)·_magic)/2)/6)+1`, clamped to `[1,4]` via `SETMAX(.,4)` (cap) / `SETMIN(.,1)` (floor); compute `budget = (power²·(4+Random(11))·(_magic+1))/2`; then `Create_Lair(12+itr, node.wp, wx, wy, clamp, node.type+1, budget)`. The asm pushes `budget, (type+1), clamp, wy, wx, wp, idx` → **arg5 = clamp, arg6 = type+1**. **Faithful.** (The clamp lands in the unused `niu` slot — see the [quirk](#og-quirk-preserved).)

### Strong lairs ([2578-2613](../../MoM/src/MAPGEN.c#L2578-L2613))

`while(itr < NUM_STRONG_LAIRS)` (25): roll `wp = Random(2)-1` (0/1), `wx = 2+Random(54)`, `wy = 2+Random(34)`; `continue` if Ocean; check `Delta_XY_With_Wrap < 2` against all `itr + 42` existing lairs (`break` on violation); accept only if the loop ran to completion (`if((42+itr) <= itr2)`), which re-rolls otherwise. Budget: Arcanus `50 + 50·Random(29)`, Myrror `100 + 100·Random(24)`. `Create_Lair(42+itr, wp, wx, wy, budget, 4, budget)` — arg5 = budget (niu), arg6 = `4`. **Faithful** (asm pushes `budget, 4, budget, …`).

### Weak lairs ([2617-2652](../../MoM/src/MAPGEN.c#L2617-L2652))

Same shape, `while(itr < NUM_WEAK_LAIRS)` (32), spacing checked against `42 + NUM_STRONG_LAIRS + itr` existing lairs. Budget: Arcanus `10·Random(10)`, Myrror `10·Random(20)`. `Create_Lair(67+itr, wp, wx, wy, budget, 4, budget)`. **Faithful.**

### Empty-tail marking ([2654-2659](../../MoM/src/MAPGEN.c#L2654-L2659))

`tail_start = 12 + 30 + 25 + 32 = 99`; loop `99..101` (bounded by `tail_start + 3` and `NUM_LAIRS_102`) setting `_LAIRS[itr].intact = ST_FALSE`. The `_LAIRS[]` array is sized `NUM_LAIRS_102` (102), so these three trailing records exist and are marked empty, matching the asm (`strong_lair_count + 74 .. + 77`). **Faithful.**

## OG quirk preserved

`Create_Lair`'s arg5 is `niu_create_lair_parameter` — **not used** by `Create_Lair`. The node loop computes the `[1,4]` clamp into it, and the strong/weak loops pass `budget` into it, but `Create_Lair` ignores arg5 entirely. The asm pushes these values too (and ignores them), so production preserves the dead computation/argument faithfully — the clamp and the duplicated budget have no effect.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — returns `1..n`.
- **`Create_Lair(idx, wp, wx, wy, niu, lair_type, budget)`** ([MAPGEN.c:2711](../../MoM/src/MAPGEN.c#L2711)) — writes one `_LAIRS[]` record (arg5 unused).
- **`Square_Is_Ocean_NewGame(wx, wy, wp)`** — reject ocean spots for random lairs.
- **`Delta_XY_With_Wrap(...)`** — toroidal distance for the ≥2 spacing test.
- **`memcpy`** — mirrors the tower lairs to Myrror.
- **`_LAIRS[]`**, **`_TOWERS[]`**, **`_NODES[]`**, **`_magic`** — globals read/written.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Lairs.asm` — IDA Pro 5.5 disassembly (the authority; the push order pins the `Create_Lair` arg positions).
- [MAPGEN.c:2711 — `Create_Lair`](../../MoM/src/MAPGEN.c#L2711) — the per-record writer.
- [MAPGEN.c:345](../../MoM/src/MAPGEN.c#L345) — call site.
- [MAPGEN-Generate_Towers.md](MAPGEN-Generate_Towers.md) / [MAPGEN-Generate_Nodes.md](MAPGEN-Generate_Nodes.md) — produce the towers and nodes this seeds lairs onto.
- `MOM_DEF.h` — `NUM_TOWERS` (6), `NUM_NODES` (30), `NUM_STRONG_LAIRS` (25), `NUM_WEAK_LAIRS` (32), `NUM_LAIRS_102` (102), `WORLD_WIDTH`/`WORLD_HEIGHT` (60/40).
