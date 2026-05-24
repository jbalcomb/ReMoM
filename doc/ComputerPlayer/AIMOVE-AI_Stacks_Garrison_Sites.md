AIMOVE-AI_Stacks_Garrison_Sites.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr158\AI_Stacks_Garrison_Sites

---

# AI_Stacks_Garrison_Sites — Walkthrough

**Location:** [MoM/src/AIMOVE.c:523](../../MoM/src/AIMOVE.c#L523) (~336 lines, ends 858).
**WZD overlay:** ovr158, p03
**drake178 name:** `AI_FillGarrisons()` — "tries to fill all garrisons on the continent to their predefined minimum unit count using any roaming or building stack units (but not the actual builders)"

## Purpose

Per-(plane, landmass) function that **surveys own cities and own-plane nodes on this landmass, computes per-asset garrison shortfalls, and drafts spare units from `AISTK_Roamer` / `AISTK_Unknown` stacks NOT at the stage point** to fill those shortfalls.

Each stack picks ONE target per call: the city with the best "distance − units-needed" score (lower is better). Nodes are a fallback — only considered when NO city target was found. Up to `units_to_send` non-builder units from the stack are ordered toward the chosen target.

Runs as dispatch slot 13 of [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md), immediately after slot 12 [`AI_Stacks_Stage_Expedition_Forces`](AIMOVE-AI_Stacks_Stage_Expedition_Forces.md).

The "Fill" framing applies literally — this fills garrisons. Contrast with the broader expedition-staging mechanism (slot 12) which assembles attack forces for overseas use.

## Signature

```c
void AI_Stacks_Garrison_Sites(int16_t player_idx, int16_t wp, int16_t landmass_idx)
```

Called from [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) at [line 310](../../MoM/src/AIMOVE.c#L310) and [line 412](../../MoM/src/AIMOVE.c#L412) (the two dispatch paths). Gated by landmass type:

```c
if(
    (cp_landmass_type_array[landmass_idx] == lmt_Own)
    ||
    (cp_landmass_type_array[landmass_idx] == lmt_Contested)
    ||
    (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)  // Leaveable, NoTargets
)
{
    AI_Stacks_Garrison_Sites(player_idx, wp, landmass_idx);
}
```

Runs on `lmt_Own`, `lmt_Contested`, `lmt_Leaveable`, and `lmt_NoTargets`. Skips `lmt_Unevaluated`, `lmt_NoOwnCity`, `lmt_NoOwnCityAndAllyHasCity` (no garrisons to fill on those).

The user's inline comment at [line 301](../../MoM/src/AIMOVE.c#L301) flags the gate's apparent oddness: *"Eh? Incongruent landmass filter? dominant, nervous, sure we can withdaraw, no reason not to withdraw?"* — the gate accepts both `lmt_Own` (secure) and `lmt_Leaveable` (evacuating), which is non-obvious. The unified rationale: in both cases, the landmass still has cities (else `lmt_NoOwnCity` would apply) that benefit from minimum garrisons even if the wider strategic posture differs.

## Inputs (globals read)

| Source | Fields |
|---|---|
| `_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx]` | Landmass type — drives `low_concern_landmass` flag |
| `_cities` | Loop bound for city scans (Phase 2 + Phase 3) |
| `_CITIES[itr].{owner_idx, wp, wx, wy, race, population}` | City filter + per-city garrison-need computation |
| `_FORTRESSES[player_idx].{wx, wy, wp}` | Coordinate match for fortress-city identification (Phase 2) |
| `NUM_NODES` (= 30) | Loop bound for node scan (Phase 4) |
| `_NODES[itr].{wp, wx, wy}` | Node filter + position |
| `g_ai_evaluation_map[wp][...]` | `AI_TARGET_SITE` bit check for nodes (only flagged-target nodes considered) |
| `_landmasses[wp*WORLD_SIZE + wy*WORLD_WIDTH + wx]` | Landmass-membership check for cities and nodes |
| `_ai_own_stack_count` | Loop bound for stack scans |
| `_ai_own_stack_wx[s]`, `_ai_own_stack_wy[s]` | Stack position (used to detect already-garrisoning stacks and to filter out stage-point stacks) |
| `_ai_own_stack_unit_count[s]` | Per-stack present-count for shortfall calculation |
| `_ai_own_stack_type[s]` | Filter — only `AISTK_Unknown` (0) or `AISTK_Roamer` (1) stacks are draft candidates |
| `_ai_own_stack_unit_list[s][u]` | Per-stack unit index for the order-issuing loop |
| `_UNITS[unit_idx].type` | Unit-type lookup for builder exclusion |
| `_unit_type_table[type].{Construction, Abilities}` | Builder exclusion via `Construction == 0` AND no `UA_CREATEOUTPOST | UA_MELD` |
| `cp_landmass_wx_array[landmass_idx]`, `cp_landmass_wy_array[landmass_idx]` | Stage point coordinates (used to filter OUT stacks already at stage — those are slot-12's responsibility) |
| `MAX_STACK` (= 9), `NUM_CITIES` (= 100) | Capacity bounds |
| `Delta_XY_With_Wrap(...)` | Distance helper for scoring |

## Outputs (side effects)

- For each draft unit, calls [`AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units)`](../../MoM/src/AIMOVE.c) with `g_ai_set_target_caller = 2` at [line 843](../../MoM/src/AIMOVE.c#L843).
  - Writes `_UNITS[unit_idx].Status` (`us_Move` or `us_GOTO`) and `dst_wx/wy`.
  - Consumes the stack slot via `ST_UNDEFINED` write to `_ai_own_stack_unit_list[itr_stacks][itr_list_units]`.
- Decrements the matching `city_garrison_shortfall[target_site_idx]` (city target) or `node_garrison_shortfall[target_site_idx]` (node target) — local state, but reflects the per-order accounting.

## Locals

```c
int8_t node_garrison_shortfall[NUM_NODES = 30];   // per-node shortfall (parallel with node_list)
int8_t node_list[NUM_NODES];                      // per-node _NODES[] index (parallel with node_garrison_shortfall)
int8_t city_garrison_shortfall[NUM_CITIES = 100]; // per-city shortfall (parallel with city_list)
int8_t city_list[NUM_CITIES];                     // per-city _CITIES[] index (parallel with city_garrison_shortfall)
int16_t extra_strong_race;                     // Phase 3 flag — TRUE for Dwarf/Troll/Draconian cities (smaller garrison ask)
int16_t fortress_city_idx;                     // _CITIES[] index of player's fortress; default = player_idx (world-gen order)
int16_t low_concern_landmass;                  // Phase 1 flag — TRUE for {lmt_Own, lmt_Leaveable, lmt_NoTargets}
int16_t site_added;                            // local "already counted this site" flag in Phase 3/4
int16_t unit_idx, unit_type;                   // Phase 5 per-iteration caches
int16_t target_type;                           // 0 = city target, 1 = node target
int16_t units_to_send;                         // Phase 5 cap on order count per stack-per-call
int16_t stack_wx, stack_wy;                    // Phase 5 stack position cache
int16_t target_wx, target_wy;                  // chosen target coords
int16_t target_value, best_target_value;       // Phase 5 scoring (lower-is-better)
int16_t node_count, city_count;                // populated in Phase 3/4, consumed in Phase 5
int16_t node_wx, node_wy;                      // Phase 4 node-position cache
int16_t city_wx, city_wy;                      // Phase 3 city-position cache
int16_t itr;                                   // outer loop iterator (reused across phases)
int16_t list_unit_count;                       // Phase 5 stack-unit-count cache
int16_t itr_list_units;                        // Phase 5 inner unit iterator
int16_t itr_stacks;                            // Phase 3/4/5 stack iterator
int16_t target_site_idx;                       // Phase 5 — index into city_list/node_list of the chosen target
```

## Code walk

### Phase 1 — Threat classification ([lines 556-572](../../MoM/src/AIMOVE.c#L556-L572))

```c
if(
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] < lmt_Leaveable)
    &&
    (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_Own)
)
{
    /* {lmt_Unevaluated, lmt_Contested, lmt_NoOwnCity, lmt_NoOwnCityAndAllyHasCity} */
    low_concern_landmass = ST_FALSE;
}
else
{
    /* {lmt_Own, lmt_Leaveable, lmt_NoTargets} */
    low_concern_landmass = ST_TRUE;
}
```

Computes `low_concern_landmass` from landmass type. The flag controls per-asset garrison-size formulas in Phases 3 and 4.

The dispatcher gate (above) already excludes `lmt_Unevaluated` and the `lmt_NoOwnCity*` types, so in practice this flag is only ever computed for landmasses where:
- `lmt_Contested` → `low_concern_landmass = FALSE` (high threat — bigger garrisons)
- `lmt_Own` / `lmt_Leaveable` / `lmt_NoTargets` → `low_concern_landmass = TRUE` (low concern — smaller garrisons)

So in practice the flag is just "is this landmass contested?"

### Phase 2 — Locate fortress city index ([lines 575-592](../../MoM/src/AIMOVE.c#L575-L592))

```c
/* player_idx, here, because fortress cities are created during world-gen in player order */
fortress_city_idx = player_idx;
for(itr = 0; itr < _cities; itr++)
{
    if(
        (_CITIES[itr].wx == _FORTRESSES[player_idx].wx)
        &&
        (_CITIES[itr].wy == _FORTRESSES[player_idx].wy)
        &&
        (_CITIES[itr].wp == _FORTRESSES[player_idx].wp)
    )
    {
        fortress_city_idx = itr;
    }
}
```

Find the `_CITIES[]` index of this player's fortress by matching coordinates against `_FORTRESSES[player_idx]`. The default `fortress_city_idx = player_idx` relies on a world-gen invariant: fortress cities are created in player order, so initially `_CITIES[player_idx]` IS this player's fortress. The coordinate-matching loop verifies and updates if cities have been reordered (razed, etc.).

No early-out — scans all cities even after finding a match. Last-write-wins semantics; in normal data only one match exists. The source comment at [line 578](../../MoM/src/AIMOVE.c#L578) flags this as `/* OGBUG  should early-exit with `break;` */`.

### Phase 3 — Survey cities for garrison shortfalls ([lines 595-672](../../MoM/src/AIMOVE.c#L595-L672))

```c
city_count = 0;
for(itr = 0; itr < _cities; itr++)
{
    if (_CITIES[itr].owner_idx != player_idx) { continue; }
    if (_CITIES[itr].wp != wp) { continue; }

    city_wx = _CITIES[itr].wx;
    city_wy = _CITIES[itr].wy;

    /* Dwarf/Troll/Draconian units are high value, influencing garrison size logic */
    extra_strong_race = ST_FALSE;
    if (_CITIES[itr].race == rt_Dwarf || _CITIES[itr].race == rt_Troll || _CITIES[itr].race == rt_Draconian)
    {
        extra_strong_race = ST_TRUE;
    }

    /* Ensure City is on the active landmass being processed */
    if(_landmasses[((wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)] != landmass_idx)
    {
        continue;
    }

    site_added = ST_FALSE;
    if(itr == fortress_city_idx)
    {
        city_garrison_shortfall[city_count] = MAX_STACK;
    }
    else
    {
        if((low_concern_landmass == ST_FALSE) && (extra_strong_race == ST_FALSE))
        {
            /* Low threat or high-quality defenders: 2 + pop/3 */
            city_garrison_shortfall[city_count] = (2 + (_CITIES[itr].population / 3));
        }
        else
        {
            /* Standard threat: 2+ pop/4 */
            city_garrison_shortfall[city_count] = (2 + (_CITIES[itr].population / 4));
        }
    }
    SETMAX(city_garrison_shortfall[city_count], MAX_STACK);

    /* Back out units already present in AI stacks at this city */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(site_added != ST_FALSE) { break; }
        if((_ai_own_stack_wx[itr_stacks] == city_wx) && (_ai_own_stack_wy[itr_stacks] == city_wy))
        {
            city_garrison_shortfall[city_count] -= _ai_own_stack_unit_count[itr_stacks];
            city_list[city_count] = (int8_t)itr;
            if(city_garrison_shortfall[city_count] > 0)
            {
                city_count++;
            }
            site_added = ST_TRUE;
        }
    }

    /* If no stack was found at city, add it as a target if it needs units */
    if(site_added == ST_FALSE)
    {
        city_list[city_count] = (int8_t)itr;
        city_count++;
    }
}
```

For each own city on this landmass: compute target garrison size, subtract present stack count, record as candidate.

**Target-size formula:**
- **Fortress city** → `MAX_STACK = 9` (full stack)
- **Other cities, low-concern landmass OR extra-strong race** → `2 + population/4`
- **Other cities, high-threat landmass with non-extra-strong race** → `2 + population/3`

Then capped at `MAX_STACK` via `SETMAX`. (Note: the `SETMAX` cap was already applied implicitly to the fortress assignment too — redundant but harmless there.)

**Present-stack subtraction:** The inner loop finds the FIRST stack co-located with the city (assumes `_ai_own_stack_*` doesn't have duplicates at a coordinate — true because slot 1's build pass coalesces by position). Subtracts that stack's unit count from `city_garrison_shortfall[city_count]`. If the result is still > 0, the slot is kept (`city_count++`); otherwise the slot is left "claimed" but not advanced (next iteration overwrites).

**No-present-stack case:** If no own stack is at the city, append the city to `city_list[]` with the full computed need.

**Notable quirks:**

- **Early continue order** — `extra_strong_race` is computed BEFORE the landmass-membership check. So for cities NOT on this landmass, we compute the race flag and then skip via `continue`. Wasted CPU but harmless.
- **`SETMAX` after fortress assignment is redundant** — `MAX_STACK` is already the assigned value; the cap is a no-op.
- **City Indices vs city_list** — the array stores `_CITIES[]` indices (int8_t), allowing `NUM_CITIES = 100` to fit in a byte.

### Phase 4 — Survey nodes for garrison shortfalls ([lines 674-734](../../MoM/src/AIMOVE.c#L674-L734))

```c
node_count = 0;
for(itr = 0; itr < NUM_NODES; itr++)
{
    if(_NODES[itr].wp != wp) { continue; }
    node_wx = _NODES[itr].wx;
    node_wy = _NODES[itr].wy;
    /* Only process nodes marked as valid AI target sites on the evaluation map */
    if(g_ai_evaluation_map[wp][((node_wy * WORLD_WIDTH) + node_wx)] != AI_TARGET_SITE)
    {
        continue;
    }
    node_wx = _NODES[itr].wx;        /* redundant re-cache */
    node_wy = _NODES[itr].wy;
    /* Ensure Node is on the active landmass being processed */
    if(_landmasses[((wp * WORLD_SIZE) + (node_wy * WORLD_WIDTH) + node_wx)] != landmass_idx)
    {
        continue;
    }
    site_added = ST_FALSE;
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(site_added != ST_FALSE) { break; }
        if((_ai_own_stack_wx[itr_stacks] == node_wx) && (_ai_own_stack_wy[itr_stacks] == node_wy))
        {
            if(low_concern_landmass == ST_TRUE)
            {
                node_garrison_shortfall[node_count] = (8 - _ai_own_stack_unit_count[itr_stacks]);
            }
            else
            {
                node_garrison_shortfall[node_count] = (4 - _ai_own_stack_unit_count[itr_stacks]);
            }
            node_list[node_count] = (int8_t)itr;
            if(node_garrison_shortfall[node_count] > 0)
            {
                node_count++;
            }
            site_added = ST_TRUE;
        }
    }
    if(site_added == ST_FALSE)
    {
        node_garrison_shortfall[node_count] = 8;  /* B2 — see Bug catalog */
        node_list[node_count] = (int8_t)itr;
        node_count++;
    }
}
```

Same structure as Phase 3 but for nodes (`_NODES[]`) instead of cities. Three filters via early-continue:
1. Wrong plane → skip
2. Not flagged as `AI_TARGET_SITE` in the evaluation map → skip
3. Not on this landmass → skip

**Target-size formula** (inverted from Phase 3's threat-level):
- **Low-concern landmass (`Own`/`Leaveable`/`NoTargets`)** → 8
- **High-concern landmass (`Contested`)** → 4

That direction is opposite of intuition. Phase 3 made bigger garrisons in HIGH-threat landmasses (more defense needed). Phase 4 makes bigger garrisons in LOW-threat landmasses. The difference probably reflects intent: nodes are SITES that need garrisoning even when calm because they produce mana for the player; cities need defense when threatened.

But the value when no stack is present (line 727) hardcodes `8` — not the threat-aware 8/4 (B2 — see Bug catalog).

The redundant re-cache of `node_wx`/`node_wy` at lines 689-690 (after just being set at 682-683) is preserved-from-OG noise — looks like leftover from a longer expression that got simplified.

### Phase 5 — Match draft stacks to targets, issue orders ([lines 736-856](../../MoM/src/AIMOVE.c#L736-L856))

Source structures this as four sub-phases:

- **Phase 5a** ([lines 739-757](../../MoM/src/AIMOVE.c#L739-L757)) — sanity-check filters (stack-type, stage-point exclusion)
- **Phase 5b** ([lines 759-764](../../MoM/src/AIMOVE.c#L759-L764)) — init per-stack locals
- **Phase 5c** ([lines 766-787](../../MoM/src/AIMOVE.c#L766-L787)) — score cities
- **Phase 5d** ([lines 789-814](../../MoM/src/AIMOVE.c#L789-L814)) — score nodes (fallback)
- followed by the order-issuing block at [lines 816-853](../../MoM/src/AIMOVE.c#L816-L853)

```c
for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
{
    if((_ai_own_stack_type[itr_stacks] != AISTK_Unknown) && (_ai_own_stack_type[itr_stacks] != AISTK_Roamer))
    {
        continue;
    }
    if((_ai_own_stack_wx[itr_stacks] == cp_landmass_wx_array[landmass_idx]) &&
       (_ai_own_stack_wy[itr_stacks] == cp_landmass_wy_array[landmass_idx]))
    {
        continue;
    }

    list_unit_count = _ai_own_stack_unit_count[itr_stacks];
    stack_wx = _ai_own_stack_wx[itr_stacks];
    stack_wy = _ai_own_stack_wy[itr_stacks];
    list_unit_count = _ai_own_stack_unit_count[itr_stacks];  /* OGBUG  duplicate */
    best_target_value = 1000;

    /* Evaluate Cities first */
    for(itr = 0; itr < city_count; itr++)
    {
        if(city_garrison_shortfall[itr] > 0)
        {
            target_value = (Delta_XY_With_Wrap(stack_wx, stack_wy, _CITIES[city_list[itr]].wx, _CITIES[city_list[itr]].wy, WORLD_WIDTH)
                            - city_garrison_shortfall[itr]);
            if(target_value < best_target_value)
            {
                best_target_value = target_value;
                target_wx = _CITIES[city_list[itr]].wx;
                target_wy = _CITIES[city_list[itr]].wy;
                units_to_send = city_garrison_shortfall[itr];
                target_type = 0;
                target_site_idx = itr;
            }
        }
    }

    /* only tries Nodes if it didn't find a City */
    if(best_target_value == 1000)
    {
        for(itr = 0; node_count > itr; itr++)
        {
            if(node_garrison_shortfall[itr] > 0)
            {
                target_value = (Delta_XY_With_Wrap(stack_wx, stack_wy, _NODES[node_list[itr]].wx, _NODES[node_list[itr]].wy, WORLD_WIDTH)
                                - node_garrison_shortfall[itr]);
                if(target_value < best_target_value)
                {
                    best_target_value = target_value;
                    target_wx = _NODES[node_list[itr]].wx;
                    target_wy = _NODES[node_list[itr]].wy;
                    units_to_send = node_garrison_shortfall[itr];
                    target_type = 1;
                    target_site_idx = itr;
                }
            }
        }
    }

    if(best_target_value < 1000)
    {
        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            /* B-quirk: itr_list_units increments per consumed-or-not slot,
               but units_to_send is the cap — so consumed slots count against the cap */
            if(itr_list_units >= units_to_send) { break; }

            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
            if(unit_idx != ST_UNDEFINED)
            {
                unit_type = _UNITS[unit_idx].type;
                if(((_unit_type_table[unit_type].Construction == 0)) &&
                   ((_unit_type_table[unit_type].Abilities & (UA_CREATEOUTPOST | UA_MELD)) == 0))
                {
                    g_ai_set_target_caller = 2;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
                    if(target_type == 0) { city_garrison_shortfall[target_site_idx] -= 1; }
                    else                 { node_garrison_shortfall[target_site_idx] -= 1; }
                }
            }
        }
    }
}
```

The main match-and-order loop. For each `AISTK_Unknown` / `AISTK_Roamer` stack NOT at the landmass stage point:

1. **Score cities** — for each city with positive `city_garrison_shortfall`, compute `target_value = distance - units_needed`. Lower is better (closer + more-needed wins). Initialize `best_target_value = 1000`.
2. **If no city target was found** (best_target_value still 1000), **score nodes** the same way.
3. **If any target was found** (best_target_value < 1000), iterate the stack's unit slots up to `units_to_send`. For each slot:
   - Skip ST_UNDEFINED (consumed by an earlier order-issuer).
   - Skip builders: `Construction != 0` OR `Abilities & (UA_CREATEOUTPOST | UA_MELD)` (this is the drake178-described "but not the actual builders" filter).
   - Issue goto-target order (`g_ai_set_target_caller = 2`).
   - Decrement the local need accounting.

**Stage-point exclusion** at lines 750-757 filters out stacks at the stage point — those are slot 12's responsibility (`AI_Stacks_Stage_Expedition_Forces`). Garrison-fill draws from elsewhere.

**Scoring heuristic** — `distance - units_needed`. Examples:
- Stack at distance 3 from city needing 5 units → score = -2
- Stack at distance 5 from city needing 2 units → score = 3
- Closer + more-needed cities win

The bias toward "more-needed" means partial-fills get deprioritized. A city that needs 1 more unit at distance 0 (score = -1) loses to a city that needs 5 more units at distance 3 (score = -2).

**City-over-node preference** is structural: the node scan only fires if `best_target_value == 1000` (no city target found). So nodes only get garrisoned by THIS function when there are no city shortfalls anywhere in the city_list, OR all cities are at distance >= 1000 + units_needed (effectively impossible). In normal data, nodes are starved by this function unless cities are all already full.

## Bug catalog

| # | Where | Issue | Severity |
|---|---|---|---|
| B2 | [line 727](../../MoM/src/AIMOVE.c#L727) | `node_garrison_shortfall[node_count] = 8` hardcoded when no stack at node — should be threat-conditional `(low_concern_landmass ? 8 : 4)` matching lines 709-715 | Real but rare — fires only for nodes with NO own units already present |
| B6 | [lines 760-763](../../MoM/src/AIMOVE.c#L760-L763) | Duplicate `list_unit_count = _ai_own_stack_unit_count[itr_stacks]` at line 760 and line 763 — flagged in source: `/* OGBUG  diplicate line, see three above */` | Harmless |
| B-Phase2-quirk | [line 578](../../MoM/src/AIMOVE.c#L578) | Fortress-coord match loop has no early-`break;` after finding the fortress — flagged in source: `/* OGBUG  should early-exit with `break;`*/` | Harmless — wasted CPU only |
| B-Phase5-quirk | [line 820](../../MoM/src/AIMOVE.c#L820) | `if(itr_list_units >= units_to_send) break;` — the iterator counts every slot examined (whether consumed or builder-filtered), not just successful orders. So consumed/builder slots reduce the effective number of orders issued. Flagged in source: `/* OGBUG  should track units sent, not processed - will exit early */` | Behavior — orders fewer units than `units_to_send` in mixed stacks |

## ASCII summary

```
AI_Stacks_Garrison_Sites(player_idx, wp, landmass_idx)
  ├─ Phase 1: classify landmass — set low_concern_landmass
  ├─ Phase 2: find fortress_city_idx via coordinate match
  ├─ Phase 3: for each own city on this landmass:
  │    ├─ compute target garrison (fortress=MAX_STACK; race/threat-aware formula)
  │    ├─ subtract present stack count
  │    └─ record (city_idx, units_needed) in city_list / city_garrison_shortfall
  ├─ Phase 4: for each node on this plane, this landmass, SITE-flagged:
  │    ├─ compute target garrison (8 low-concern, 4 high)
  │    ├─ subtract present stack count
  │    └─ record (node_idx, units_needed) in node_list / node_garrison_shortfall
  └─ Phase 5: for each Roamer/Unknown stack NOT at stage point:
       ├─ score cities (distance - units_needed); pick min
       ├─ if no city found: score nodes; pick min
       └─ if any target: for u in 0..units_to_send:
            └─ if unit not ST_UNDEFINED and not builder:
                 ├─ AI_Stacks_Order_Attack_Target_Or_Goto_Destination(...) [caller=2]
                 └─ decrement target's need
```

## Position in the dispatch chain

| Slot | Function | Gate |
|---|---|---|
| 1 | [`AI_Stacks_Init_Build_Target_Order`](AIMOVE-AI_Stacks_Init_Build_Target_Order.md) | Always |
| 2 | [`AI_Stacks_Move_Out_NonMilitary_Garrisoned`](AIMOVE-AI_Stacks_Move_Out_NonMilitary_Garrisoned.md) | Always |
| 3 | [`AI_Stacks_Survey_Expedition_Forces`](MoM-AI-Move-ai_own_stack.md) | Always — populates `G_Pushout_*` / `G_Seafaring_*` pools |
| 4-7 | `AI_Do_Meld` / `AI_Do_Settle` / `AI_Do_Purify` / `AI_Do_RoadBuild` | Always |
| 8 | [`AI_Build_Target_List`](AIMOVE-AI_Build_Target_List.md) | Always |
| 9 | [`AI_Stacks_Roamers_Target_Or_Deploy`](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) | Always |
| 10 | [`AI_Stacks_Order_To_War_Landmass`](AIMOVE-AI_Stacks_Order_To_War_Landmass.md) | NOT `lmt_Contested` |
| 11 | [`AI_Stacks_Relocate_Roamers`](AIMOVE-AI_Stacks_Relocate_Roamers.md) | `lmt_Leaveable` / `lmt_NoTargets` / `lmt_Own` |
| 12 | [`AI_Stacks_Stage_Expedition_Forces`](AIMOVE-AI_Stacks_Stage_Expedition_Forces.md) | UNGATED |
| **13** | **`AI_Stacks_Garrison_Sites`** | **`lmt_Own` / `lmt_Contested` / `>= lmt_Leaveable`** |

**Pairing with adjacent slots:**

- **Slot 12 (`AI_Stacks_Stage_Expedition_Forces`)** pulls units toward the landmass STAGE POINT for offensive use. Stacks at the stage point are slot 12's domain.
- **Slot 13 (this)** pulls units toward CITIES and NODES for defensive garrison. Explicitly excludes stacks at the stage point (line 753-760) — those are reserved for the offensive expedition.

So slots 12 and 13 partition the AISTK_Roamer/Unknown pool on this landmass into "stage attackers" vs "garrison defenders" by position. The same stacks can't serve both roles in one turn.

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — dispatcher; this function is slot 13
- [AIMOVE-AI_Stacks_Stage_Expedition_Forces.md](AIMOVE-AI_Stacks_Stage_Expedition_Forces.md) — slot 12 sibling (offensive staging vs defensive garrison)
- [AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md](AIMOVE-AI_Stacks_Roamers_Target_Or_Deploy.md) — slot 9; assigns roamer-stack targets earlier in the dispatch
- [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) — `_ai_own_stack_*` area-of-code reference
- [MoX/src/MOM_DAT.h:102-107](../../MoX/src/MOM_DAT.h#L102-L107) — `AISTK_*` enum values
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
