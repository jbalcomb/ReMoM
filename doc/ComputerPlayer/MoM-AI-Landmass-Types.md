# `lmt_*` Landmass-Type State Machine

Every write site, every read site, and the structural meaning of the `e_LANDMASS_TYPE` enum used in `_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx]`.

Built by exhaustive grep of `AIMOVE.c`. Verify against source before acting on any single claim.

## The enum

```c
enum e_LANDMASS_TYPE  // [MOM_DAT.h:119](../../MoX/src/MOM_DAT.h#L119)
{
    lmt_Unevaluated              = 0,  // transient (initial state during city-counting prep)
    lmt_Own                      = 1,  // dominant: own unit-cost > 10 × enemy unit-cost
    lmt_Contested                = 2,  // has own city + non-trivial enemy presence
    lmt_NoOwnCity                = 3,  // no owned city (units may still be present)
    lmt_NoOwnCityAndAllyHasCity  = 4,  // overloaded: ally's land OR no stage tile OR slot-0 sentinel
    lmt_Leaveable                  = 5,  // dock square reachable from here (departure-staging point)
    lmt_NoTargets                = 6   // no attackable enemies/lairs/nodes found
};
```

## Two-tier architecture

The enum is really **two different categories** of value stuffed into one byte:

| Tier | Members | Origin | Lifetime |
|---|---|---|---|
| **Classifier outputs** | `Own`, `Contested`, `NoOwnCity`, `NoOwnCityAndAllyHasCity` | Produced by reeval functions ([`AI_Reevaluate_Continent`](../../MoM/src/AIMOVE.c#L5933), [`AI_Evaluate_Continents`](../../MoM/src/AIMOVE.c#L6902)) | Regenerated each classification pass |
| **Phase overlays** | `Abandon`, `NoTargets` | Produced by other AI phases ([`AI_Stacks_Roamers_Target_Or_Deploy`](../../MoM/src/AIMOVE.c#L1635) at line 1806, [`AI_Build_Target_List`](../../MoM/src/AIMOVE.c#L2327) at line 2757) | `Abandon` is clobbered by the next classifier run. `NoTargets` is **preserved across turns** by `AI_Evaluate_Continents`'s preservation clause at [line 6848](../../MoM/src/AIMOVE.c#L6848), subject to a city-count collision OGBUG (see `lmt_NoTargets` notes below). |
| **Initial / sentinel** | `Unevaluated` | Set by [`AI_Evaluate_Continents`](../../MoM/src/AIMOVE.c#L7013) as counter base | Never appears as a final state |

This is the source of the design's awkwardness: a classifier-output and a phase-overlay can never coexist on the same slot, because each clobbers the other.

## Write sites — what triggers each value

### `lmt_Unevaluated` (0)

| Line | Function | Triggering condition |
|---|---|---|
| [7013](../../MoM/src/AIMOVE.c#L7013) | `AI_Evaluate_Continents` | Initialization before using slot as city counter (comment: "actually just 0, cause its about to just be a city count") |

**Note:** [`AI_Reevaluate_Continent`](../../MoM/src/AIMOVE.c#L5933) does NOT zero the slot before its own city-counter use at [line 6005](../../MoM/src/AIMOVE.c#L6005). It does `type_array[idx] += 1` per owned city, starting from whatever value was there. If the prior value was non-zero (e.g., a stale `lmt_Leaveable` = 5), the "city count > 0" test at [line 6024](../../MoM/src/AIMOVE.c#L6024) is spuriously true. **Possible bug — verify against disassembly.**

### `lmt_Own` (1)

| Line | Function | Triggering condition |
|---|---|---|
| [6027](../../MoM/src/AIMOVE.c#L6027) | `AI_Reevaluate_Continent` | `(Sum_NonOwn_UnitCost × 10) < Sum_Own_UnitCost` after city-counting found ≥1 owned city on landmass |
| [7161](../../MoM/src/AIMOVE.c#L7161) | `AI_Evaluate_Continents` | Same comparison in the full-rebuild path |

Semantic: "Decisively held — own unit-cost is more than 10× enemy unit-cost on this landmass."

### `lmt_Contested` (2)

| Line | Function | Triggering condition |
|---|---|---|
| [6031](../../MoM/src/AIMOVE.c#L6031) | `AI_Reevaluate_Continent` | Else-branch of the 10× check (has ≥1 own city, but not dominant) |
| [7167](../../MoM/src/AIMOVE.c#L7167) | `AI_Evaluate_Continents` | Same |

Semantic: "Has own city + non-trivial enemy unit-cost present."

### `lmt_NoOwnCity` (3)

| Line | Function | Triggering condition |
|---|---|---|
| [6036](../../MoM/src/AIMOVE.c#L6036) | `AI_Reevaluate_Continent` | Else-branch of outer "city count > 0" check |
| [7175](../../MoM/src/AIMOVE.c#L7175) | `AI_Evaluate_Continents` | Same |

Semantic: "No owned CITY on this landmass." Units may roam freely — they don't count.

### `lmt_NoOwnCityAndAllyHasCity` (4)

The most overloaded value — five different write sites with three distinct semantics.

| Line | Function | Triggering condition | Semantic |
|---|---|---|---|
| [6056](../../MoM/src/AIMOVE.c#L6056) | `AI_Reevaluate_Continent` | Prior state was `lmt_NoOwnCity` AND a non-neutral, allied wizard has a city here | Ally's land |
| [6061](../../MoM/src/AIMOVE.c#L6061) | `AI_Reevaluate_Continent` | Force-set: `plane[0]...type_array[0]` | Slot-0 sentinel (ocean) |
| [6063](../../MoM/src/AIMOVE.c#L6063) | `AI_Reevaluate_Continent` | Force-set: `plane[1]...type_array[0]` | Slot-0 sentinel (ocean) |
| [6134](../../MoM/src/AIMOVE.c#L6134) | `AI_Reevaluate_Continent` | Continent was Own/Contested/(ally-land) but stage-square search found no valid embarking tile within `Best_Tile_Weight < 1000` | No stage tile (downgrade) |
| [7206](../../MoM/src/AIMOVE.c#L7206) | `AI_Evaluate_Continents` | Same allied-city promotion as 6056 | Ally's land |
| [7215](../../MoM/src/AIMOVE.c#L7215) | `AI_Evaluate_Continents` | Slot-0 force, plane 0 | Slot-0 sentinel |
| [7217](../../MoM/src/AIMOVE.c#L7217) | `AI_Evaluate_Continents` | Slot-0 force, plane 1 | Slot-0 sentinel |
| [7387](../../MoM/src/AIMOVE.c#L7387) | `AI_Evaluate_Continents` | Rally-tile search failed downgrade (continent-pick path) | No stage tile (downgrade) |
| [7470](../../MoM/src/AIMOVE.c#L7470) | `AI_Evaluate_Continents` | Same — different sub-path | No stage tile (downgrade) |

**Three distinct meanings folded into one byte:** "ally's land" (diplomatic), "no stage tile" (physical inability), "ocean sentinel" (geometric). The name only fits the first.

### `lmt_Leaveable` (5)

| Line | Function | Triggering condition |
|---|---|---|
| [1890](../../MoM/src/AIMOVE.c#L1890) | `AI_Stacks_Roamers_Target_Or_Deploy` | Phase 3 fires only when all five hold: Phase 1 found a roamer stack of ≥8 units on this landmass AND no roamer stack found a target AND `_ai_landmass_war_targets[wp][player_idx] != 0` AND we're not on the war landmass AND Phase 2 found an empty dock square. Co-writes `wx_array`/`wy_array` with the chosen dock square as the embark point. |

**Semantic at write site:** "We have a sizable expeditionary force on this landmass, this landmass has nothing left to attack, there's a war elsewhere, and we found an embark point — stage the force at that dock square for ferrying."

**At read sites:** the tag is a **single-turn ferry-staging marker**, NOT a long-term "we are leaving this place" classification. It is written during dispatch slot 9 ([`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) Phase 4); consumed THIS turn by slots 10/11/14 (`AI_Stacks_Order_To_War_Landmass`, `AI_Stacks_Relocate_Roamers`, `AI_FillGarrisons__WIP`) using the embark point as a staging target; survives ONE turn boundary to be read by next turn's `AI_Choose_War_Landmass` ([line 7786](../../MoM/src/AIMOVE.c#L7786) switch sets `Reevaluate = ST_TRUE` for this case); then **clobbered** by `AI_Evaluate_Continents` ([line 6874](../../MoM/src/AIMOVE.c#L6874)) before the next dispatch — no preservation clause exists for `lmt_Leaveable` (unlike `lmt_NoTargets`).

To stay tagged across turns, the Phase 3 write must re-fire — which requires ALL five trigger conditions to still hold. See [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) for the full timing analysis, comparison with `lmt_NoTargets`, and the "why 'Abandon' is a misleading name" notes.

### `lmt_NoTargets` (6)

| Line | Function | Triggering condition |
|---|---|---|
| [2757](../../MoM/src/AIMOVE.c#L2757) | `AI_Build_Target_List` | `_ai_targets_count == 0` after target-gathering phase |

**Semantic at write site:** "Walked the landmass, found zero hostile assets to attack" — no hostile fortress, no non-own city, no lair/node/tower with `Hostility >= 3` or `DIPL_War` against the asset's owner. Per-landmass, hostility-gated.

**At read sites** (notably the [`AI_Choose_War_Landmass`](../../MoM/src/AIMOVE.c#L7733) `!= lmt_NoTargets` exclusion): the tag is **at least one turn old**. Written only by `AI_Build_Target_List`, which runs during `AI_Set_Unit_Orders` AFTER `AI_Choose_War_Landmass` in the per-turn driver ([`AIDUDES.c:241,284,285`](../../MoM/src/AIDUDES.c#L241)). Preserved across turns by `AI_Evaluate_Continents`'s preservation clause at [AIMOVE.c:6848](../../MoM/src/AIMOVE.c#L6848). See [AIMOVE-AI_Choose_War_Landmass.md](AIMOVE-AI_Choose_War_Landmass.md) for the full timing analysis and two failure modes (stale-positive false exclusion; city-count collision OGBUG that preserves 6-city heartland landmasses as `lmt_NoTargets`).

A more accurate name would be something like `lmt_NoHostileAssetsLastSeen` — the read-site interpretation is "last time we looked, nothing to attack on this landmass," not current state.

## Read sites — what each value triggers downstream

### `AI_Set_Unit_Orders` ([line 133](../../MoM/src/AIMOVE.c#L133))

| Line | Group tested | Action |
|---|---|---|
| [186-193](../../MoM/src/AIMOVE.c#L186) | `≥ lmt_Leaveable` OR `Own` OR `NoOwnCityAndAllyHasCity` OR `NoOwnCity` ("almost just NOT Contested") | (gates an `if` block — see surrounding code) |
| [203-206](../../MoM/src/AIMOVE.c#L203) | `≥ lmt_Leaveable` OR `Own` | Skip `AI_Stacks_Order_To_War_Landmass` |
| [212-217](../../MoM/src/AIMOVE.c#L212) | `Own` OR `Contested` OR `≥ lmt_Leaveable` | (gates an `if` block) |

### `G_AI_RallyFill__WIP` ([line 240](../../MoM/src/AIMOVE.c#L240))

| Line | Condition | Action |
|---|---|---|
| [259-263](../../MoM/src/AIMOVE.c#L259) | `≥ lmt_Leaveable` OR `NoOwnCity` OR (drafted unit threshold) | (gates an `if` block — staging-tile check) |

### `AI_FillGarrisons__WIP` ([line 298](../../MoM/src/AIMOVE.c#L298))

| Line | Condition | Action |
|---|---|---|
| [331-334](../../MoM/src/AIMOVE.c#L331) | `< lmt_Leaveable` AND `!= lmt_Own` | `G_Low_Threat = ST_FALSE` (i.e., we're not safe — Unevaluated, Contested, NoOwnCity, NoOwnCityAndAllyHasCity all mean threat) |
| [343](../../MoM/src/AIMOVE.c#L343) | Else (Own, Abandon, NoTargets) | `G_Low_Threat = ST_TRUE` |

Comment at [337](../../MoM/src/AIMOVE.c#L337) explicitly lists the "threat" group: `lmt_Unevaluated, lmt_Contested, lmt_NoOwnCity, lmt_NoOwnCityAndAllyHasCity`.

### `AI_ProcessOcean__WIP` ([line 742](../../MoM/src/AIMOVE.c#L742))

| Line | Condition | Action |
|---|---|---|
| [787-791](../../MoM/src/AIMOVE.c#L787) | `Contested` OR `NoOwnCity` | (gates a per-landmass action — read context for details) |

### `G_AI_ProcessTransports__WIP` ([line 959](../../MoM/src/AIMOVE.c#L959))

| Line | Condition | Action |
|---|---|---|
| [1394](../../MoM/src/AIMOVE.c#L1394) | Adjacent landmass `!= lmt_NoOwnCity` | (gates pathing logic) |

### `AI_Do_Meld` ([line 4442](../../MoM/src/AIMOVE.c#L4442))

| Line | Condition | Action |
|---|---|---|
| [4538-4540](../../MoM/src/AIMOVE.c#L4538) | Node landmass = `Own` OR `≥ lmt_Leaveable` OR garrisoned | (gates meld-target eligibility) |

### `AI_CheckOtherPlane` (around [line 5860](../../MoM/src/AIMOVE.c#L5860))

| Line | Condition | Action |
|---|---|---|
| [5860-5863](../../MoM/src/AIMOVE.c#L5860) | Opposite landmass `!= Own` AND `!= NoOwnCityAndAllyHasCity` | (gates plane-flip logic) |
| [5891](../../MoM/src/AIMOVE.c#L5891) | Opposite landmass `!= NoOwnCity` | (further gating) |

### `AI_Reevaluate_Continent` self-reads (within its own classification)

| Line | Condition | Action |
|---|---|---|
| [6053](../../MoM/src/AIMOVE.c#L6053) | `== lmt_NoOwnCity` | Promote to `NoOwnCityAndAllyHasCity` if allied city present |
| [6066-6071](../../MoM/src/AIMOVE.c#L6066) | `Contested` OR `Own` OR `NoOwnCityAndAllyHasCity` | Enter stage-square search block |
| [6146](../../MoM/src/AIMOVE.c#L6146) | `== Own` | Enter embark-tile validation |

### `AI_Evaluate_Continents` self-reads (analogous to above)

| Line | Condition | Action |
|---|---|---|
| [6950](../../MoM/src/AIMOVE.c#L6950) | `== lmt_NoOwnCity` | (gates target-search) |
| [7144](../../MoM/src/AIMOVE.c#L7144) | `!= lmt_NoTargets` | (gates classification — "still just city count" per comment) |
| [7150](../../MoM/src/AIMOVE.c#L7150) | `> lmt_Unevaluated` | (city-count check, "still just city count") |
| [7203](../../MoM/src/AIMOVE.c#L7203) | `== lmt_NoOwnCity` | Promote to `NoOwnCityAndAllyHasCity` (allied city) |
| [7235](../../MoM/src/AIMOVE.c#L7235) | `== lmt_NoOwnCity` | (continent-pick path) |
| [7313-7318](../../MoM/src/AIMOVE.c#L7313) | `Contested` OR `Own` OR `NoOwnCityAndAllyHasCity` | (stage-square search gate) |
| [7412](../../MoM/src/AIMOVE.c#L7412) | `== lmt_NoOwnCity` | (further branch) |
| [7494](../../MoM/src/AIMOVE.c#L7494) | `== lmt_Own` | (final classification adjustment) |

### `AI_Choose_War_Landmass[line 7834](../../MoM/src/AIMOVE.c#L7834))

The main consumer — uses the type to decide whether to keep current "main war continent" assignment or reevaluate.

| Line | Case | `Reevaluate` |
|---|---|---|
| [7899](../../MoM/src/AIMOVE.c#L7899) | `lmt_Unevaluated` | (DNE — dead case; disassembly never enters with value 0 per `(type-1)` jump table) |
| [7903](../../MoM/src/AIMOVE.c#L7903) | `lmt_Own` | `ST_FALSE` (don't revisit) |
| [7907](../../MoM/src/AIMOVE.c#L7907) | `lmt_Contested` | `ST_FALSE` (don't revisit — questionable) |
| [7911-7912](../../MoM/src/AIMOVE.c#L7911) | `lmt_NoOwnCity` OR `lmt_NoOwnCityAndAllyHasCity` | Complex hostility/target-presence check |
| [7966](../../MoM/src/AIMOVE.c#L7966) | `lmt_Leaveable` | `ST_TRUE` (revisit — we said we could depart) |
| (missing) | `lmt_NoTargets` | **Falls to default (DNE)** — Gemini's translation groups this with the NoOwnCity case. May be a missing case. |

### Other late-stage consumers

| Line | Function | Condition | Action |
|---|---|---|---|
| [8199-8202](../../MoM/src/AIMOVE.c#L8199) | `AI_Choose_War_Landmass `!= lmt_Own` AND `!= lmt_NoTargets` | (continent-value scoring exclusion) |

## Transition diagrams

### Within `AI_Reevaluate_Continent` (one landmass, one call)

```
                       (entry — slot may be any prior value)
                                       │
                                       ▼
                  Unconditional accumulate: type_array[idx] += 1
                  per owned city found
                                       │
                                       ▼
                       ┌── (type_array[idx] > 0) ? ──┐
                       │                              │
                      YES                            NO
                       │                              │
                       ▼                              ▼
              (Sum_Own > 10× Sum_NonOwn)?       lmt_NoOwnCity
                       │                              │
                 ┌─────┴─────┐                        │
                YES         NO                        │
                 │           │                        │
                 ▼           ▼                        ▼
              lmt_Own  lmt_Contested          (ally has city?)
                                                       │
                                                  ┌────┴────┐
                                                 YES        NO
                                                  │         │
                                                  ▼         ▼
                                      lmt_NoOwnCity         (unchanged)
                                      AndAllyHasCity
                                                                  
              (then stage-square search if classification is in {Own, Contested, NoOwnCityAndAllyHasCity})
                                                  │
                                            ┌─────┴─────┐
                                       found tile     no tile
                                            │             │
                                            ▼             ▼
                                       (preserved)   lmt_NoOwnCityAndAllyHasCity
                                                     (overrides original)
```

### Cross-function clobbering

```
AI_Stacks_Setup_Ferry        AI_Build_Target_List          AI_Reevaluate_Continent
  writes lmt_Leaveable            writes lmt_NoTargets         OVERWRITES with classifier output
   (dock square found)             (no targets found)           (Own / Contested / NoOwnCity / etc.)
        │                              │                              │
        └──────────────────────────────┴──────────────────────────────┘
                                       ▼
                            Any phase overlay (Abandon, NoTargets)
                            is destroyed whenever the classifier runs.
                            The two tiers cannot coexist.
```

## Key facts to remember

1. **`lmt_NoOwnCity` does not mean "no presence."** Units may be roaming. Only cities count.
2. **`lmt_Own` requires a 10:1 unit-cost dominance.** A single own city with comparable enemy strength → `lmt_Contested`.
3. **`lmt_NoOwnCityAndAllyHasCity` is overloaded.** Three semantic situations share this value: ally's territory, no stage tile, slot-0 sentinel. The name only fits the first.
4. **`lmt_Leaveable` means "we are leaving this continent."** Set by `AI_Stacks_Roamers_Target_Or_Deploy` when the landmass has no targets AND has at least one roaming stack of 7+ units. The dock square is the embarkation point. The next-turn re-evaluation ([`AI_Choose_War_Landmass66`](../../MoM/src/AIMOVE.c#L7966)) may revise the decision if conditions changed.
5. **`AI_Reevaluate_Continent` does not zero the slot before counting.** [`AI_Evaluate_Continents`](../../MoM/src/AIMOVE.c#L7013) does. Possible bug in `AI_Reevaluate_Continent` — verify against disassembly.
6. **`lmt_NoTargets` is unhandled in `AI_Choose_War_Landmass switch.** Falls to default. Gemini's translation includes it with the NoOwnCity group — may be a missing case in production.
7. **The disassembly's switch is `(type - 1)`**, jumping into a 5-entry table for values 1-5. Value 0 (`lmt_Unevaluated`) cannot enter that switch — the `case lmt_Unevaluated` in the C code is dead. Value 6 (`lmt_NoTargets`) would index past the table → also dead in the bytes, unless a separate default branch handles it.

## Related references

- [MoM-NextTurn-AI.md](../MoM-NextTurn-AI.md) — overall AI turn structure
- [MoM-NextTurn-AI-Landmasses.md](MoM-NextTurn-AI-Landmasses.md) — broader landmass-evaluation context
- [MoM-AI-Move.md](MoM-AI-Move.md) — AIMOVE.c overview
- [MoM-AI-Continents.md](MoM-AI-Continents.md) — continent data structures
