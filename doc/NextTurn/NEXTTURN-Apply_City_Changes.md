NEXTTURN-Apply_City_Changes.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Apply_City_Changes.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Apply_City_Changes.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Apply_City_Changes()

---

# `Apply_City_Changes` — Walkthrough

**Location:** [NEXTTURN.c:2917-3146](../../MoM/src/NEXTTURN.c#L2917-L3146) (~230 lines, ends [line 3146](../../MoM/src/NEXTTURN.c#L3146)).
**WZD overlay:** ovr140, p16 (IDA address `ovr140:14ED`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L609), normal (non-time-stop) end-of-turn path only.

> **Note on the OG file path:** the stub header for this doc pointed at `ovr145`. The disassembly actually lives in `ovr140` (`Apply_City_Changes.asm` / `.c`) — path corrected above.

## Purpose

Per-turn city/outpost population maintenance. Iterates every city in `_CITIES[0.._cities)` and, for each, does one of two mutually-exclusive things depending on whether the entry is an **outpost** (`population == 0`) or a **real city** (`population != 0`):

- **Outpost** — resolve its fate: if its fractional-population counter `Pop_10s` has dropped to `<= 0` the outpost is lost (`Destroy_City`); if it has reached `>= 10` it graduates to a size-1 Hamlet.
- **City** — apply growth: add this turn's `City_Growth_Rate` into `Pop_10s`, then promote population up a step when `Pop_10s >= 100` (with farmer re-balancing, size recompute, neutral-owner cap), demote it when `Pop_10s < 0`, apply a Pestilence random death roll, and run `City_Apply_Production`.

After the per-branch work, every city (outpost or not) runs its overland city-enchantment hooks (Consecration, Stream of Life, Chaos Rift, Gaia's Blessing, Nightshade) and the loop advances. When all cities are processed, `Volcano_Counts()` runs once.

Human-owned events are logged into the end-of-turn message queues (`MSG_CityLost_*`, `MSG_CityGained_*`, `MSG_CityGrowth_*`, `MSG_CityDeath_*`) for the Chancellor / popup summaries, each capped at 20 entries.

## Signature

```c
void Apply_City_Changes(void)
```

## Globals read / written

| Symbol | Access | Fields / effect |
|---|---|---|
| `_cities` (`int16_t`) | read | loop bound. |
| `_CITIES[]` (`s_CITY[]`) | read + mutated | `population`, `Pop_10s`, `size`, `farmer_count`, `owner_idx`, `name`, `enchantments[]`, `NIGHTSHADE`. |
| `MSG_CityLost_Count` / `MSG_CityLost_Names[]` | read + mutated | lost-outpost name buffer (stride 20 — see quirks). |
| `MSG_CityGained_Count` / `MSG_CityGained_Array[]` | read + mutated | outpost→city graduation list. |
| `MSG_CityGrowth_Count` / `MSG_CityGrowth_Array[]` | read + mutated | population-up list. |
| `MSG_CityDeath_Count` / `MSG_CityDeath_Array[]` | read + mutated | population-down list (shared by the decrease branch and Pestilence). |

External calls: `Destroy_City`, `City_Minimum_Farmers`, `Do_City_Calculations`, `City_Growth_Rate`, `Random`, `City_Apply_Production`, `Apply_Consecration`, `Volcano_Counts`, `stu_strcpy`. Stubbed (see below): `CTY_StreamOfLife`, `CTY_ChaosRift`, `CTY_GaiasBlessing`, `CTY_NightshadeDispel`.

## Locals

```c
int16_t excess_farmer_count;   /* [bp-08h] — farmer_count minus this-size minimum, carried across the size bump so the surplus/deficit survives */
int16_t New_Min_Farmers;       /* [bp-06h] — City_Minimum_Farmers recomputed after population++ */
int16_t Population_Growth;      /* [bp-04h] — City_Growth_Rate(itr_cities) return for this turn */
int16_t itr_cities;            /* [bp-02h] — city loop index */
```

Stack-frame order matches OG (`[bp-2]` itr_cities down to `[bp-8]` excess_farmer_count).

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Apply_City_Changes.asm` (520 lines).

### Phase 1 — Outpost branch (`population == 0`) ([2928-2973](../../MoM/src/NEXTTURN.c#L2928-L2973))

Two independent checks on `Pop_10s`:

- **Outpost lost** (`Pop_10s <= 0`, asm:30-31 `jg` skips): if HUMAN-owned and the lost-list has room, copy the name into `MSG_CityLost_Names` and bump the count; then `Destroy_City(itr_cities)` **unconditionally** (asm:61-64 — the destroy call sits after the message block, not inside the HUMAN gate).
- **Outpost graduated** (`Pop_10s >= 10`, asm:71-72): set `population = 1`, `size = 1 (CTY_Hamlet)`, `farmer_count = City_Minimum_Farmers`, run `Do_City_Calculations`, and (HUMAN, list room) record into `MSG_CityGained_Array`.

Both checks are evaluated in the same pass — a freshly-destroyed outpost with `Pop_10s <= 0` cannot also have `Pop_10s >= 10`, so they are effectively exclusive.

### Phase 2 — City growth (`population != 0`) ([2976-3115](../../MoM/src/NEXTTURN.c#L2976-L3115))

`Population_Growth = City_Growth_Rate(itr_cities)`, then accumulated into `Pop_10s` — `Pop_10s += Population_Growth` ([2982](../../MoM/src/NEXTTURN.c#L2982), asm:129-130 `add [Pop_10s], ax`). `City_Growth_Rate` returns a per-turn delta, so the counter accrues toward the `>= 100` promotion / `< 0` demotion thresholds.

**Population up** — `Pop_10s >= 100 AND population < 25` ([2985-3042](../../MoM/src/NEXTTURN.c#L2985-L3042)):
1. `excess_farmer_count = farmer_count - City_Minimum_Farmers` (surplus/deficit vs the *current* size).
2. `population++`, `Pop_10s = 0` (preserved OGBUG — the leftover `Pop_10s - 100` is dropped, faithful).
3. `farmer_count = City_Minimum_Farmers(new size) + excess_farmer_count`, then clamp `farmer_count <= population`.
4. `size = (population + 3) / 4`, clamped to `MAX_CITY_SIZE (5)`.
5. Neutral owner: clamp `population <= MAX_CITY_POPULATION_NEUTRAL_PLAYER (8)`. Production carries an added `// BUGBUG` note that `City_Growth_Rate` gates growth on `((_difficulty + 1) * 2)` while the clamp here is a hard 8 — annotation only, the value 8 is OG-faithful (asm:272,279).
6. HUMAN + list room → `MSG_CityGrowth_Array`.

**Population down** — `Pop_10s < 0` ([3045-3078](../../MoM/src/NEXTTURN.c#L3045-L3078)): if `population <= 1` set `Pop_10s = 5`; else `population--`, recompute `size`, log `MSG_CityDeath` (HUMAN), and `Pop_10s += 100`.

**Pestilence** — `enchantments[PESTILENCE] > 0` ([3081-3104](../../MoM/src/NEXTTURN.c#L3081-L3104)): if `population > Random(10)` then `population--` and log `MSG_CityDeath` (HUMAN). Note `Random(10)` is rolled only when the enchantment is present, keeping the RNG stream aligned with OG.

Then `City_Apply_Production(itr_cities)` ([3113](../../MoM/src/NEXTTURN.c#L3113), asm:448-451).

### Phase 3 — Per-city enchantment hooks ([3118-3141](../../MoM/src/NEXTTURN.c#L3118-L3141))

Runs for **every** city, outpost or not (after the if/else rejoins):

| Condition | OG call | Production status |
|---|---|---|
| `enchantments[CONSECRATION] > 0` | `Apply_Consecration` | **active** ✓ |
| `enchantments[STREAM_OF_LIFE] > 0` | `CTY_StreamOfLife` | `// SPELLY` stub |
| `enchantments[CHAOS_RIFT] > 0` | `CTY_ChaosRift` | `// SPELLY` stub |
| `enchantments[GAIAS_BLESSING] > 0` | `CTY_GaiasBlessing` | `// SPELLY` stub |
| `enchantments[NIGHTSHADE] > 0` | `CTY_NightshadeDispel` | `// SPELLY` stub |

The last hook's OG guard reads the standalone `s_CITY.NIGHTSHADE` field (asm:502), not an entry inside the `enchantments` struct. In production, `NIGHTSHADE == 0x19 (25)` indexes `enchantments[]` at the byte immediately after `ALTAR_OF_BATTLE (0x18 / 24)` — the same physical byte. Faithful.

### Phase 4 — Tail ([3144](../../MoM/src/NEXTTURN.c#L3144))

`Volcano_Counts()` once after the loop (asm:516).

## OG quirks preserved (faithful — do not "fix")

- **`Pop_10s = 0` discards excess population on size-up** ([2994](../../MoM/src/NEXTTURN.c#L2994)) — after `population++`, the leftover `Pop_10s - 100` is thrown away rather than carried. OG asm:184 stores literal 0. Preserved `/* OGBUG  discards excess population */` comment.
- **Neutral population hard-capped at 8** ([3020-3023](../../MoM/src/NEXTTURN.c#L3020-L3023)) — `MAX_CITY_POPULATION_NEUTRAL_PLAYER = 8`, a hard constant, while `City_Growth_Rate` gates neutral growth on `((_difficulty + 1) * 2)`. Mismatch is OG-faithful (asm:272/279); the `// BUGBUG` note is a ReMoM annotation only.
- **`Destroy_City` runs unconditionally on outpost loss** ([2944](../../MoM/src/NEXTTURN.c#L2944)) — the HUMAN message-queue push is gated, but the destroy itself is not (asm:61-64).
- **`MSG_CityLost_Names` stride is 20, not the name length (14)** ([2938](../../MoM/src/NEXTTURN.c#L2938)) — production indexes `MSG_CityLost_Names[MSG_CityLost_Count * 20]`, matching asm:52-54 (`mov dx, 20; imul`). The decompiled `Apply_City_Changes.c` models this buffer as `char[20][14]` (stride 14) — that decompilation is **wrong**; the asm bytes use stride 20 and production is correct. Do not "fix" this to 14.
- **Pestilence `Random(10)` rolled only when the enchantment is present** ([3081-3084](../../MoM/src/NEXTTURN.c#L3081-L3084)) — keeps the RNG stream OG-aligned. Faithful.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Apply_City_Changes.asm` — IDA Pro 5.5 disassembly (the authority, 520 lines).
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Apply_City_Changes.c` — drake178 decompilation (second opinion; note its `MSG_CityLost_Names[20][14]` stride is wrong — asm decides).
- Tracked as Wave 9B in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
- Downstream helpers with their own review tracks: `Destroy_City`, `City_Growth_Rate`, `City_Minimum_Farmers`, `Do_City_Calculations`, `City_Apply_Production`, `Apply_Consecration`, `Volcano_Counts`.
- Stubbed city-enchantment spell hooks (`CTY_StreamOfLife`, `CTY_ChaosRift`, `CTY_GaiasBlessing`, `CTY_NightshadeDispel`) — not yet reconstructed (`// SPELLY`).
