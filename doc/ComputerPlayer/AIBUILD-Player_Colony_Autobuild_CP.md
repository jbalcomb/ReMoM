AIBUILD-Player_Colony_Autobuild_CP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr157\Player_Colony_Autobuild_CP.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr157\Player_Colony_Autobuild_CP.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()

Player_All_Colony_Autobuild()
    |-> Player_Colony_Autobuild_CP()

---

# `Player_Colony_Autobuild_CP` — Walkthrough

**Location:** [AIBUILD.c:92-381](../../MoM/src/AIBUILD.c#L92-L381) (~290 lines, ends [line 381](../../MoM/src/AIBUILD.c#L381)).
**WZD overlay:** ovr157, p02 (IDA address `ovr157:EABF2`).
**Called from:** [`Player_All_Colony_Autobuild`](../../MoM/src/AIBUILD.c#L33) — per AI-owned city.

> Verified against the **`.asm`** (846 lines). The `.c` is a Gemini translation of that asm (second opinion); the asm bytes decide. Production is now 1:1 with the asm — the six reconstruction divergences found during review (garbled landmass index, `need_units` else-branches, unit-weight square, inverted combat gate, `Trading_Cities` city, un-nested in-production checks) have all been repaired.

## Purpose

Chooses the next construction project for one Computer-Player city. It counts the player's trade-goods cities to derive a `Trade_Chance`, decides whether the city needs a settler / engineer / military units, applies early forced-building picks off the fortress landmass, and otherwise builds a weighted product list (buildings + units) and calls `Get_Weighted_Choice` to pick one, storing it in `_CITIES[city_idx].construction`.

One of three siblings — `_CP` (this, AI wizards 1-4), `_HP` (human, [AIBUILD.c:600](../../MoM/src/AIBUILD.c#L600)), `_NP` (neutral, [AIBUILD.c:399](../../MoM/src/AIBUILD.c#L399)). The `/* HP … */` inline comments mark where the CP tuning differs from the HP variant (transports gate on `_turn/40` and `>8` here vs `/30` and `>10` in HP).

## Signature

```c
void Player_Colony_Autobuild_CP(int16_t player_idx, int16_t city_idx)
```

## Code walk

Line refs are production [AIBUILD.c](../../MoM/src/AIBUILD.c); cross-checked against `Player_Colony_Autobuild_CP.asm`.

1. **Count trade / total cities → `Trade_Chance`** ([120-135](../../MoM/src/AIBUILD.c#L120-L135), asm:35-75) — for each player-owned city, tally `Total_Cities`, and `Trading_Cities` iff the *target* city (`city_idx`) is on TradeGoods (OGBUG, see below). `Trade_Chance = (Total/8 - Trading)*10 + 40`, zeroed for human.
2. **`fortress_landmass`** ([136-143](../../MoM/src/AIBUILD.c#L136-L143), asm:77-107) — `ST_UNDEFINED` past turn 200, else the human fortress's landmass (`[wp][wy][wx]`).
3. **`Calculate_Product_Array` + `city_landmass`** ([147-148](../../MoM/src/AIBUILD.c#L147-L148), asm:136-159).
4. **Need settler/engineer — units on the same landmass** ([152-171](../../MoM/src/AIBUILD.c#L152-L171), asm:165-258) — a player unit on `city_wp` and `city_landmass` with `UA_CREATEOUTPOST` clears `need_settler`; with `Construction > 0` clears `need_engineer`.
5. **Need settler/engineer — in production elsewhere** ([173-197](../../MoM/src/AIBUILD.c#L173-L197), asm:264-307) — for each other owned city building a **unit** (`construction >= 100`), the same two ability tests (nested inside the `>= 100` gate).
6. **`need_units` + forced buildings** ([198-266](../../MoM/src/AIBUILD.c#L198-L266), asm:308-432) — `need_units = Random(30|15) > city_unit_count`. **Only off the fortress landmass** (`city_landmass != fortress_landmass`) does the forced chain run: `>0`→Barracks, `>1`→BuildersHall, `>2`→Shrine, `>3`→Smithy, `>4`→Granary, each only if `bs_NotBuilt` (early `return`).
7. **Trade-goods roll** ([267-271](../../MoM/src/AIBUILD.c#L267-L271), asm:435-448) — `Random(100) <= Trade_Chance` → TradeGoods, return.
8. **Base weights** ([272-286](../../MoM/src/AIBUILD.c#L272-L286), asm:454-536) — buildings: `ai_build_base_weights_table[cat] + ai_build_objective_weights_table[cat][obj]`; units: `(cost/10)²` where cost = `_unit_type_table[product_array[itr] - 100].cost`.
9. **Housing ×2 for small cities** ([287-296](../../MoM/src/AIBUILD.c#L287-L296), asm:544-574) — gated on the stale-`itr` population read (OGBUG, see below).
10. **Per-product settler/engineer/transport/combat weighting** ([297-372](../../MoM/src/AIBUILD.c#L297-L372), asm:579-811):
    - Building & `need_units == TRUE` → weight 0 ([301-304](../../MoM/src/AIBUILD.c#L301-L304)).
    - Settler (`UA_CREATEOUTPOST`) & `need_settler` & `population > 2`: ×3/2 if `_turn < 50`, then × `(ai_build_objective_weights_table[9 /*Builder*/][obj]+10)/10`; else 0 ([309-327](../../MoM/src/AIBUILD.c#L309-L327)).
    - Engineer (`Construction > 0`) & `need_engineer`: same boost; else 0 ([328-342](../../MoM/src/AIBUILD.c#L328-L342)).
    - Transport: `(_turn/40 < ai_transport_count) || (count > 8)` → 0, else × `(_turn/40 - count)` ([343-359](../../MoM/src/AIBUILD.c#L343-L359)).
    - Other unit: `need_units != ST_FALSE` → × `(ai_build_objective_weights_table[8 /*Combat_Unit*/][obj]+10)/10`, else 0 ([360-370](../../MoM/src/AIBUILD.c#L360-L370)).
11. **`Get_Weighted_Choice` → `construction`** ([373-374](../../MoM/src/AIBUILD.c#L373-L374), asm:819-839).

## OG quirks preserved (faithful — do not "fix")

- **`Trading_Cities` reads `city_idx`, not the loop city** ([124](../../MoM/src/AIBUILD.c#L124)) — the loop owner-checks `itr` but reads `construction` from `city_idx` (asm:48), so `Trading_Cities` ends up `Total_Cities` if the target city is on TradeGoods, else 0. Genuine OGBUG; the `/* OGBUG should be itr … */` comment preserves it.
- **Housing gate uses the stale loop var** ([287](../../MoM/src/AIBUILD.c#L287)) — after the weight loop `itr == product_count`, so `_CITIES[itr].population` reads the wrong (or out-of-range) city. asm:544 uses the same stale `_DI_itr`. OGBUG, preserved with its comment. (Distinct from the `Trading_Cities` case: here the asm really does use `itr`.)
- **Forced buildings only off the fortress landmass** ([199-266](../../MoM/src/AIBUILD.c#L199-L266)) — when `city_landmass == fortress_landmass` the asm jumps past the whole forced chain (asm:328-329); production nests it in the `else`. Faithful.
- **Transport CP tuning** ([343-359](../../MoM/src/AIBUILD.c#L343-L359)) — `/40` and `>8` (vs HP's `/30`, `>10`). Faithful.

## ReMoM additions (not OG)

- `assert((player_idx >= 1) && (player_idx <= 4))` ([117](../../MoM/src/AIBUILD.c#L117)) and the construction-range asserts ([182-183](../../MoM/src/AIBUILD.c#L182-L183), [379-380](../../MoM/src/AIBUILD.c#L379-L380)) — debug only.
- `AI_Metrics_Emit_Build(...)` ([375-378](../../MoM/src/AIBUILD.c#L375-L378)) — instrumentation, no OG equivalent.
- `/* DNE */` commented block ([220](../../MoM/src/AIBUILD.c#L220)) — correctly notes a line the OG does **not** have.

## Related references

- `…\ovr157\Player_Colony_Autobuild_CP.asm` — IDA Pro 5.5 disassembly (ground truth, 846 lines).
- `…\ovr157\Player_Colony_Autobuild_CP.c` — Gemini translation of the asm (agrees with the asm; production now matches both).
- Siblings: [`Player_Colony_Autobuild_NP`](../../MoM/src/AIBUILD.c#L399) (neutral), [`Player_Colony_Autobuild_HP`](../../MoM/src/AIBUILD.c#L600) (human) — **not yet reviewed**; HP still carries the same-shaped patterns this review fixed in CP (e.g. the `wp*SIZE + wx*WIDTH + wp` landmass index at [651](../../MoM/src/AIBUILD.c#L651), both-branches-`ST_TRUE` at [704-711](../../MoM/src/AIBUILD.c#L704-L711), unit weight without `-100`/square at [736](../../MoM/src/AIBUILD.c#L736)). Verify against the HP asm before touching.
- Dispatcher [`Player_All_Colony_Autobuild`](../../MoM/src/AIBUILD.c#L33); `Calculate_Product_Array`, `Get_Weighted_Choice`, `Map_Square_Unit_Count` — separate reviews.
- Weight tables `ai_build_base_weights_table[Category]`, `ai_build_objective_weights_table[Category][Objective]` (Builder = idx 9, Combat_Unit = idx 8); local weight buffer `product_weights[]`.
- Constants: `WORLD_SIZE = 2400`, `WORLD_WIDTH = 60`, `bt_Barracks = 3`, `bt_Smithy = 8`, `bt_Shrine = 22`, `bt_Granary = 29`, `bt_BuildersHall = 32`, `UA_CREATEOUTPOST = 0x20`, `bs_NotBuilt = 255`.
