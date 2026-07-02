AIBUILD-Player_Colony_Autobuild_HP.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr157\Player_Colony_Autobuild_HP.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr157\Player_Colony_Autobuild_HP.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()

City_Apply_Production()
    |-> Player_Colony_Autobuild_HP()

---

# `Player_Colony_Autobuild_HP` — Walkthrough

**Location:** [AIBUILD.c:653-863](../../MoM/src/AIBUILD.c#L653-L863) (~210 lines, ends [line 863](../../MoM/src/AIBUILD.c#L863)).
**WZD overlay:** ovr157, p05.
**Called from:** [`City_Apply_Production`](../../MoM/src/NEXTTURN.c#L2340) — the Grand-Vizier autobuild tail, for a human city on `bt_AUTOBUILD`.

> Verified against the **`.asm`** (760 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. Production is now 1:1 with the asm — the five reconstruction divergences found during review (landmass index, the in-production loop's `_units`/`_UNITS`/un-nested trio, the `need_units` else-branch, the unit-weight square, the inverted combat gate) have all been repaired.

## Purpose

The Grand-Vizier autobuild for a **human** city: when a human player runs the Vizier and a city is set to auto-build, this picks its next construction project. Same shape as `_CP` — decide need settler/engineer/units, apply forced early buildings, build a weighted product list, `Get_Weighted_Choice`. Its differences from CP are intentional (see below). `player_idx` is forced to `HUMAN_PLAYER_IDX` internally ([679](../../MoM/src/AIBUILD.c#L679)).

## Signature

```c
void Player_Colony_Autobuild_HP(int16_t city_idx)
```

## Code walk

Line refs are production [AIBUILD.c](../../MoM/src/AIBUILD.c); cross-checked against `Player_Colony_Autobuild_HP.asm`.

1. **Empty-city guard** ([675-678](../../MoM/src/AIBUILD.c#L675-L678), asm:34-38) — `population == 0` → return.
2. **Snapshot city + `city_landmass`** ([679-684](../../MoM/src/AIBUILD.c#L679-L684), asm:41-89) — `player_idx = HUMAN`, `Calculate_Product_Array`, landmass `[wp][wy][wx]`.
3. **Need settler/engineer + count transports — units on landmass** ([688-713](../../MoM/src/AIBUILD.c#L688-L713), asm:93-202) — a player unit on `city_wp`/`city_landmass`: `UA_CREATEOUTPOST` clears `need_settler`, `Construction > 0` clears `need_engineer`, `Transport > 0` bumps `ai_transport_count`.
4. **Need settler/engineer — in production elsewhere** ([714-737](../../MoM/src/AIBUILD.c#L714-L737), asm:208-251) — for each other owned city building a **unit** (`construction >= 100`), the two ability tests (nested inside the `>= 100` gate).
5. **`need_units` + forced buildings** ([738-759](../../MoM/src/AIBUILD.c#L738-L759), asm:252-363) — `need_units = Random(15) > city_unit_count`, then forced to `ST_FALSE` if `_turn < 20 && city_unit_count > 1`. Forced chain (unconditional): `>0`→Barracks, `>1`→BuildersHall, `>2`→Shrine, `>3`→Smithy, `>4`→Granary, each only if `bs_NotBuilt`.
6. **Base weights** ([760-774](../../MoM/src/AIBUILD.c#L760-L774), asm:369-451) — buildings `ai_build_base_weights_table[cat] + ai_build_objective_weights_table[cat][obj]`; units `(cost/10)²`, cost = `_unit_type_table[product_array[itr] - 100].cost`.
7. **Housing ×2 for small cities** ([775-783](../../MoM/src/AIBUILD.c#L775-L783), asm:459-489) — stale-`itr` population gate (OGBUG, below).
8. **Per-product settler/engineer/transport/combat weighting** ([784-858](../../MoM/src/AIBUILD.c#L784-L858), asm:494-731) — settler & `need_settler` & `population > 2` → ×3/2 (`_turn<50`) then ×`(Builder[obj]+10)/10`; engineer likewise; transport gated on `_turn/30`/`>10`; other unit `need_units == ST_FALSE → 0`, else ×`(Combat_Unit[obj]+10)/10`.
9. **`Get_Weighted_Choice` → `construction`** ([859-860](../../MoM/src/AIBUILD.c#L859-L860), asm:733-753).

## CP-vs-HP differences (intentional — faithful, do NOT converge to CP)

- **No trade-goods logic / no `fortress_landmass`** — HP has neither the `Trading_Cities`/`Trade_Chance` pass nor the fortress-landmass gate; the forced chain runs unconditionally ([755-759](../../MoM/src/AIBUILD.c#L755-L759), asm:276-363).
- **`_turn < 20 && city_unit_count > 1` → `need_units = FALSE`** ([747-754](../../MoM/src/AIBUILD.c#L747-L754), asm:270-274) — HP genuinely has this; CP only as a commented `/* DNE */`.
- **Transport gate `/30`, `>10`** ([831-845](../../MoM/src/AIBUILD.c#L831-L845), asm:645-666) — vs CP's `/40`, `>8`; the `/* CP … */` comments mark the difference.
- **Counts `ai_transport_count` itself** ([687](../../MoM/src/AIBUILD.c#L687), [708-711](../../MoM/src/AIBUILD.c#L708-L711), asm:92/183-196) — CP reads the global set elsewhere. drake178's transport-on-landmass BUG comment preserved.
- **`population == 0` early return** ([675-678](../../MoM/src/AIBUILD.c#L675-L678), asm:34-38) — a human autobuild city may be an outpost.

## OG quirks preserved (faithful — do not "fix")

- **Housing gate uses the stale loop var** ([775](../../MoM/src/AIBUILD.c#L775)) — `_CITIES[itr].population` with `itr == product_count`; asm:459-465 uses the same stale `_DI_itr`. OGBUG, preserved with its `¿ BUGBUG … ?` comment.
- **Forced-building thresholds** ([755-759](../../MoM/src/AIBUILD.c#L755-L759)) — match asm:276-363.

## ReMoM additions (not OG)

- Construction-range asserts ([861-862](../../MoM/src/AIBUILD.c#L861-L862), now `bt_AUTOBUILD` / `bt_MAX_THINGS`) — debug only. (HP has no `AI_Metrics_Emit_Build` call.)

## Related references

- `…\ovr157\Player_Colony_Autobuild_HP.asm` — IDA Pro 5.5 disassembly (ground truth, 760 lines).
- `…\ovr157\Player_Colony_Autobuild_HP.c` — Gemini translation of the asm (agrees with the asm; production now matches both).
- [`AIBUILD-Player_Colony_Autobuild_CP.md`](AIBUILD-Player_Colony_Autobuild_CP.md) — the CP sibling. `Player_Colony_Autobuild_NP` ([AIBUILD.c:399](../../MoM/src/AIBUILD.c#L399)) — neutral, not yet reviewed.
- Weight tables `ai_build_base_weights_table[Category]`, `ai_build_objective_weights_table[Category][Objective]` (Builder = idx 9, Combat_Unit = idx 8); local weight buffer `product_weights[]`.
- Constants: `WORLD_SIZE = 2400`, `WORLD_WIDTH = 60`, `bt_Barracks = 3`, `bt_Smithy = 8`, `bt_Shrine = 22`, `bt_Granary = 29`, `bt_BuildersHall = 32`, `UA_CREATEOUTPOST = 0x20`, `bs_NotBuilt = 255`.
