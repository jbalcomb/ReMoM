NEXTTURN-City_Apply_Production.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\City_Apply_Production.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\City_Apply_Production.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()
            |-> City_Apply_Production()

---

# `City_Apply_Production` — Walkthrough

**Location:** [NEXTTURN.c:2159-2304](../../MoM/src/NEXTTURN.c#L2159-L2304) (~145 lines, ends [line 2304](../../MoM/src/NEXTTURN.c#L2304)).
**WZD overlay:** ovr140, p03 (IDA address `ovr140:3F94`).
**Called from:** [`Apply_City_Changes`](../../MoM/src/NEXTTURN.c#L2901) — once per city in the end-of-turn city loop.

## Purpose

Advances one city's construction project by a turn. Accumulates production points into `Prod_Accu`, and when the accumulated total reaches the current product cost, **completes** the project:

- **Unit** (`construction >= 100`, the value is `100 + unit_type`): spawn the unit via `Create_Unit`, trim excess, and reset the build queue (or emit a "too many units" warning if the global unit cap is hit).
- **Building** (`construction < 100`): mark the building Built (or bump its status), fire building-specific side effects (Oracle map reveal), queue the human "building complete" popup, and reset the build queue.

After completion it re-points `construction` to the next default project — `bt_AUTOBUILD` for AI / Grand-Vizier cities, or `bt_Housing` / `bt_TradeGoods` for a human without the Vizier. Finally, if this is a human city running the Grand Vizier and now sitting on `bt_AUTOBUILD`, it calls `Player_Colony_Autobuild_HP` to pick the next thing to build.

`bt_AUTOBUILD` is production's name for the constant the OG disassembly labels `bt_GRANDVIZIER` — both are `-4` (`0xFFFC`). Faithful rename.

## Signature

```c
void City_Apply_Production(int16_t city_idx)
```

## Globals read / written

| Symbol | Access | Effect |
|---|---|---|
| `_CITIES[city_idx]` (`s_CITY`) | read + mutated | `population`, `construction`, `Prod_Accu`, `production_units`, `owner_idx`, `wx/wy/wp`, `bldg_status[]`, `bldg_cnt`, `name`. |
| `_units` (`int16_t`) | read | unit-cap check `(_units + 1) < MAX_UNIT_COUNT`. |
| `grand_vizier` (`uint16_t`) | read | Grand-Vizier auto-build toggle. |
| `g_bldg_msg_ctr` / `MSG_Building_Complete[]` | read + mutated | human building-complete popup queue (cap 20). |
| `bldg_data_table[]` (`s_BLDG_DATA`) | read | `replace_bldg` lookup for upgrade chains. |
| `GUI_NearMsgString`, `cnst_TooManyUnits`, `message_lbx_file` | read/written | "too many units" warning text assembly. |

External calls: `City_Current_Product_Cost`, `Create_Unit`, `Evict_Weakest_From_Full_Stack`, `Army_At_City`, `Set_Map_Square_Explored_Flags_XYP_Range`, `Player_Colony_Autobuild_HP`, `LBX_Load_Data_Static`, `Warn0`, `stu_strcpy`, `stu_strcat`.

## Locals

```c
char    city_name[LEN_NAME];      /* scratch copy of the city name for the too-many-units message */
int16_t uu_troops[MAX_STACK];     /* Army_At_City output buffer; ReMoM inits to 0xBB sentinels (HACK, not OG) */
int16_t uu_troop_count;           /* Army_At_City output count */
int16_t product_cost;             /* si — City_Current_Product_Cost(city_idx) */
```

`uu_troops` / `uu_troop_count` are pure out-parameters filled by `Army_At_City` after a unit is created; their values are never read here. The `{ 0xBB, ... }` initializer is a ReMoM debug sentinel — OG leaves the buffer uninitialized (asm just passes `lea [bp+uu_troops]`).

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `City_Apply_Production.asm` (597 lines).

### Guards ([2166-2171](../../MoM/src/NEXTTURN.c#L2166-L2171))

`product_cost = City_Current_Product_Cost(city_idx)` is fetched first (asm:12-16, even though it's only used on the completion paths). Then `population <= 0` returns early (asm:22-24) — outposts and dead cities produce nothing.

> Branch order note: production tests `construction >= 100` (Unit) first with the Building work in the `else`; OG source tests `< 100` (Building) first (asm:32-34). This is a codegen-neutral source ordering — the compiler emits one `cmp`/conditional-jump either way — not a behavioral difference.

### Unit branch — `construction >= 100` ([2173-2221](../../MoM/src/NEXTTURN.c#L2173-L2221), asm:359-571)

`Prod_Accu += production_units`; if `Prod_Accu >= product_cost`, complete:

- **Cap OK** (`(_units + 1) < MAX_UNIT_COUNT`, asm:394-398): `Create_Unit(construction - 100, owner, wx, wy, wp, city_idx)` ([2180](../../MoM/src/NEXTTURN.c#L2180)) — the `city_idx` as the 6th arg drives Create_Unit's city-block XP/weapon logic. Then `Evict_Weakest_From_Full_Stack(_units - 1)` and `Army_At_City(...)`. If AI **or** Grand Vizier → `construction = bt_AUTOBUILD` (asm:461-471).
- **Cap hit** ([2192-2218](../../MoM/src/NEXTTURN.c#L2192-L2218), asm:475-564): AI → `bt_AUTOBUILD`. Human → load + assemble the "Maximum number of units exceeded … `<city>` … disband some units" string and `Warn0`, then `bt_AUTOBUILD` if Grand Vizier else `bt_TradeGoods`. Production re-checks `owner == HUMAN` inside the already-human branch ([2205](../../MoM/src/NEXTTURN.c#L2205)) — this mirrors the redundant `cmp owner, HUMAN` at asm:536, faithful.
- `Prod_Accu = 0` after either sub-case ([2219](../../MoM/src/NEXTTURN.c#L2219), asm:565-571) — surplus production dropped.

### Building branch — `construction < 100` ([2222-2291](../../MoM/src/NEXTTURN.c#L2222-L2291), asm:36-357)

- **Not a real building** (`construction < bt_Barracks (3)`, [2224-2230](../../MoM/src/NEXTTURN.c#L2224-L2230), asm:43-59): only AI cities get nudged to `bt_AUTOBUILD`; a human sitting on `bt_NONE`/`bt_TradeGoods`/`bt_Housing` is left alone (these have no completion event).
- **Real building** (`>= bt_Barracks`): accumulate `Prod_Accu += production_units`, halved for **neutral** owners (`production_units / 2`, [2235](../../MoM/src/NEXTTURN.c#L2235), asm:70-81). On `Prod_Accu >= product_cost`, complete (see below).

**Building completion** ([2242-2289](../../MoM/src/NEXTTURN.c#L2242-L2289), asm:119-355):

1. **Set-vs-increment of `bldg_status`** ([2245-2257](../../MoM/src/NEXTTURN.c#L2245-L2257)): `bldg_status[construction] >= 0` → increment; `< 0` (`bs_NotBuilt = -1`) → set `bs_Built` and, if `bldg_data_table[construction].replace_bldg != ST_UNDEFINED`, mark the replaced building `bs_Replaced`. `bldg_status` is `int8_t`, so the signed `>= 0` test correctly matches OG's `cmp … bs_Replaced / jl` (asm:137-139).
2. **Bookkeeping** — runs unconditionally after the set/increment, matching OG's convergence at `loc_C41BB` (asm:240): Oracle map reveal for a human Oracle ([2258-2265](../../MoM/src/NEXTTURN.c#L2258-L2265)); `bldg_cnt++` ([2266](../../MoM/src/NEXTTURN.c#L2266)); `Prod_Accu = 0` ([2267](../../MoM/src/NEXTTURN.c#L2267)); then re-point `construction` — AI → `bt_AUTOBUILD`, human → queue the building-complete popup (cap 20) and set `bt_AUTOBUILD` (Vizier) or `bt_Housing` ([2268-2288](../../MoM/src/NEXTTURN.c#L2268-L2288)).

The `bldg_status[construction]` index draws MSVC `/analyze` warnings **C6385** (read, [2244](../../MoM/src/NEXTTURN.c#L2244)) and **C6386** (write, [2251](../../MoM/src/NEXTTURN.c#L2251)), both marked with `// TODO pragma ignore`. These are **false positives**: the branches only bound `construction` to `[3, 99]` (`>= bt_Barracks`, `< 100`), so the analyzer can't prove the index stays under the 36-entry `bldg_status` array — but a real buildable building type is always `< NUM_BUILDINGS (36)`, an invariant SAL doesn't model. Matches OG (asm indexes `bldg_status[construction]` with no clamp); safe by the same invariant.

### Autobuild tail ([2293-2302](../../MoM/src/NEXTTURN.c#L2293-L2302), asm:572-591)

`owner == HUMAN AND grand_vizier == TRUE AND construction == bt_AUTOBUILD` → `Player_Colony_Autobuild_HP(city_idx)`. Runs for every path that left a Vizier-run human city pointed at `bt_AUTOBUILD`.

## OG quirks preserved (faithful — do not "fix")

- **Surplus production discarded on completion** ([2219](../../MoM/src/NEXTTURN.c#L2219), [2267](../../MoM/src/NEXTTURN.c#L2267)) — `Prod_Accu = 0` throws away any overflow past `product_cost`. OG asm:297,571 store literal 0. Production comment ("surplus production units will be lost") preserves the prescribed behavior; not a bug.
- **Neutral cities produce buildings at half rate** ([2235](../../MoM/src/NEXTTURN.c#L2235), asm:70-81) — `production_units / 2`. Unit production is **not** halved (asm:359-366). Faithful.
- **Redundant `owner == HUMAN` re-check in the too-many-units branch** ([2205](../../MoM/src/NEXTTURN.c#L2205)) — already inside the human branch; mirrors the OG's redundant `cmp owner, HUMAN` at asm:536. Faithful, harmless.
- **Non-real-building (`< bt_Barracks`) only redirects AI** ([2224-2230](../../MoM/src/NEXTTURN.c#L2224-L2230)) — human cities on `bt_NONE`/`bt_TradeGoods`/`bt_Housing` are left untouched (no completion event for those). Faithful.
- **Unchecked `bldg_status[construction]` index** ([2245](../../MoM/src/NEXTTURN.c#L2245), [2252](../../MoM/src/NEXTTURN.c#L2252)) — no bounds clamp, matching OG. Safe by the "buildable type `< 36`" invariant; C6385/C6386 are false positives suppressed via pragma-ignore TODOs.
- **`bt_AUTOBUILD` == OG `bt_GRANDVIZIER` (`-4` / `0xFFFC`)** — production rename of the same sentinel. Not a divergence.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\City_Apply_Production.asm` — IDA Pro 5.5 disassembly (the authority, 597 lines).
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\City_Apply_Production.c` — drake178 decompilation (matches the asm structure).
- [`NEXTTURN-Apply_City_Changes.md`](NEXTTURN-Apply_City_Changes.md) — the caller; `City_Apply_Production` runs at the tail of each non-outpost city's per-turn pass.
- [`NEXTTURN-Create_Unit.md`](NEXTTURN-Create_Unit.md) — the unit-completion spawn (called with `R_Param = city_idx`, driving the city-block XP/weapon path).
- Constants: `MAX_UNIT_COUNT = 1000`, `NUM_BUILDINGS = 36`, `bt_Barracks = 3`, `bt_Oracle = 18`, `bt_Housing = 2`, `bt_TradeGoods = 1`, `bt_AUTOBUILD = -4`, `bs_NotBuilt = -1`, `bs_Replaced = 0`, `bs_Built = 1`, `bs_Removed = 2`, `HUMAN_PLAYER_IDX = 0`, `NEUTRAL_PLAYER_IDX = 5`.
- Tracked as a downstream helper of Wave 9B in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
