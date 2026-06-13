MAPGEN-Create_Lair.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Create_Lair.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Create_Lair.c

Init_New_Game()
    |-> ... Generate_Towers / Extend_Islands ...
    |-> Generate_Lairs();                [MAPGEN.c:345]
        |-> Create_Lair()                [MAPGEN.c:2711]

---

# `Create_Lair` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Create_Lair` | [MAPGEN.c:2711-3063](../../MoM/src/MAPGEN.c#L2711-L3063) | Fills one `_LAIRS[]` record: sets type, picks 1-2 guard stacks by race within a difficulty-scaled budget, then rolls the treasure hoard (gold/mana/items/prisoner/spell/spellbook). |

Verified faithful to the disassembly `Create_Lair.asm` throughout — structure, the treasure-switch jump-table grouping, and the `Random()` sequence all 1:1 — carrying two preserved OG bugs (B1/B2) and a dead-argument quirk.

## Purpose

Called once per lair by `Generate_Lairs` (6 towers + 30 nodes + 25 strong + 32 weak = ~93 calls). Given `(lair_idx, wp, wx, wy, niu, lair_type, budget)`:

1. Scale `budget` by difficulty, set `.wx/.wy/.wp/.intact/.type`, zero loot/guards.
2. Pick a guard **race** from `lair_type` / node-type (with rolls for tower / temple / other).
3. Pick **guard 1** then **guard 2**: the priciest unit of that race affordable within a randomly-divided budget, capped ≤8 / combined ≤9.
4. Compute a leftover **loot budget** and roll a treasure hoard category-by-category until it's spent.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Generate_Lairs` | [MAPGEN.c:345](../../MoM/src/MAPGEN.c#L345) (→ `Create_Lair`) | ~93 calls per new game. Arg5 (`niu_create_lair_parameter`) is unused. |

## Code walk

Line refs are production [MAPGEN.c](../../MoM/src/MAPGEN.c); cross-checked against `Create_Lair.asm` (the authority — jump-table targets and `LXLSH@`/`LDIV@` pin the scaling and the switch). `Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

### Budget + lair fields ([2735-2756](../../MoM/src/MAPGEN.c#L2735-L2756))

`budget = (budget * (_difficulty+1)) / 4`; write `.wx/.wy/.wp`, `.intact = TRUE`, zero loot/guard fields. The `switch(lair_type)` sets `.type`: case 0 → `lt_Tower`, **case 1 → `lt_Chaos_Node` (1)**, case 2 → `lt_Nature_Node` (2), **case 3 → `lt_Sorcery_Node` (3)** (`.type = lair_type` for node lairs — matches the asm bytes), `default` → `lt_Cave + Random(7) - 1`. Note the asm/OG deliberately *cross* `.type` vs `race` (case 1 stores a Chaos-node type but later picks a Sorcery race). **Faithful.**

### Guard race ([2757-2807](../../MoM/src/MAPGEN.c#L2757-L2807))

`lair_type` 1/2/3 → `rt_Sorcery`/`rt_Nature`/`rt_Chaos` directly. `lair_type == 0` (tower) → `switch(Random(6)-1)` and the temple types → `switch(Random(4)-1)` both have real `break`s. The final `else` → `switch(Random(5)-1)` is the preserved **[B1](#og-bugs-preserved)** fall-through.

### Guard 1 / Guard 2 ([2809-2903](../../MoM/src/MAPGEN.c#L2809-L2903))

`do { Divided_Budget = budget / Random(4); scan unit types 150..NUM_UNIT_TYPES for the priciest affordable unit of the race; } while(none && tries<200)`. Set `guard1_count = budget/cost`, a 50% `Random(2)` decrement, cap to 8 (`SETMAX(.,8)`), subtract its cost. Guard 2 is the same, gated by `guard1_count != 9 && != 0` (the **[B2](#og-bugs-preserved)** impossible-9 gate), combined count capped to 9. **Faithful.**

### Loot budget ([2905-2920](../../MoM/src/MAPGEN.c#L2905-L2920))

`loot = guard1.cost·count + (guard2.cost·count)/2`; then (when `_difficulty != god_Impossible`) `loot = (loot * 4) / (_difficulty + 1)` — the inverse of the budget scale (asm `LXLSH@ cl=2` then `LDIV@`). Plane scaling (`(49+Random(76))/100` Arcanus, `(75+Random(100))/100` Myrror) and `SETMIN(.,50)`. **Faithful.**

### Tower special + treasure hoard ([2921-3055](../../MoM/src/MAPGEN.c#L2921-L3055))

Tower (`lair_type == 0`) pre-rolls a `Spell_n_Special` discount. Then `while(loot >= 50) switch(Random(15) - 1) { … }` rolls one treasure category per pass, matching the asm's `off_47201` jump-table grouping:

| `Random(15)-1` | category | effect |
|---|---|---|
| 0, 1 | Gold | `Loot_Gold = min(Random(20)·10, loot)` rounded /10; `loot -= 200` |
| 2, 3 | Mana | as Gold, into `Loot_Mana` |
| 4, 5, 6, 7, 8 | Item | if `Item_Count<3` && `loot≥300`: `Item_Values[] = min(300+Random(27)·100, loot)`; `loot -= price` |
| 9 | prisoner | if `loot≥400` && `Misc_Flags==0`: set flag; `loot -= 1000` |
| 10, 11, 12 | spell | `Price=Random(4)`; if `Price²·50 < loot` && `Spell_n_Special<4`: add, cap 4 |
| 13, 14 | spellbook/retort | `Price=Random(4)`; tiered `Spell_n_Special = 5`/`6`; `loot -= 3000` |

The within-case `Price` clamp (`SETMAX(Price, loot)` = cap at budget) and the final "`Spell_n_Special > 4` wipes other loot" override ([3049-3055](../../MoM/src/MAPGEN.c#L3049-L3055)) are faithful. **Faithful.**

## OG bugs preserved

| # | Line | What |
|---|---|---|
| B1 | [2799-2806](../../MoM/src/MAPGEN.c#L2799-L2806) | The `else` race `switch(Random(5)-1)` **falls through** every case (no breaks), so `race_type` is always `rt_Nature` regardless of the roll. OGBUG — the asm's `jt_ng_lt_rt` cases have no inter-case `jmp`. (The tower `Random(6)-1` and temple `Random(4)-1` switches *do* break — those are faithful, not bugs.) |
| B2 | [2853](../../MoM/src/MAPGEN.c#L2853) | Guard 2 is gated on `guard1_count != 9`, but guard 1 was just capped to ≤8, so `!= 9` is always true — a dead condition. OGBUG-faithful. |

## OG quirk — dead arg5

`Create_Lair`'s arg5 is `niu_create_lair_parameter` — **not used** by the function. `Generate_Lairs` passes the node `[1,4]` clamp / the strong-weak budget into it; the asm pushes those values and ignores them. Preserved faithfully (no effect).

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — returns `1..n`.
- **`_unit_type_table[]`** — scanned (indices 150..`NUM_UNIT_TYPES`) for guard selection (race, `Transport == 0`, cost vs `Divided_Budget`).
- **`SETMAX(a,b)`** (cap a at b) / **`SETMIN(a,b)`** (floor a at b) — `MOX_DEF.h`.
- **`_LAIRS[]`**, **`_difficulty`** — globals read/written.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Create_Lair.asm` — IDA Pro 5.5 disassembly (the authority). The treasure switch's `off_47201` jump table (`sw_jt_gl_00_01 / _02_03 / _04_05_06_07_08 / _09 / _10_11_12 / _13_14`) pins the case grouping; `LXLSH@`/`LDIV@` pin the loot scale.
- [MAPGEN-Generate_Lairs.md](MAPGEN-Generate_Lairs.md) — the caller (passes `lair_type`, `budget`; arg5 unused).
- `MOM_DAT.h` — `lt_*` (`lt_Chaos_Node`=1, `lt_Nature_Node`=2, `lt_Sorcery_Node`=3, `lt_Cave`, `lt_Ancient_Temple`/`lt_Ruins`/`lt_Fallen_Temple`), `rt_*` race types; `MOX_DEF.h` — `god_Impossible`, `SETMAX`/`SETMIN`.
