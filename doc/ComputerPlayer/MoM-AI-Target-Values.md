# AI_Player_Calculate_Target_Values — Producer/Consumer Map

Single producer: [`AI_Player_Calculate_Target_Values`](../MoM/src/AIDUDES.c) ([AIDUDES.c:943](../MoM/src/AIDUDES.c#L943)).

Drake178 disassembly name: `AI_OVL_Eval_Assets`.

Once per AI turn (and again after a spell finishes casting if `AI_Eval_After_Spell == ST_TRUE`), this walks every city and every unit in the world and writes seven global arrays plus one scalar. Those outputs feed everything from continent-balance scoring to ship-build weighting.

## High-level fan-out

```
                          AI_Player_Calculate_Target_Values
                                         │
                                         │ writes
            ┌────────────────────────────┼──────────────────────────────────┐
            │                            │                                  │
            ▼                            ▼                                  ▼
    ┌────────────────┐    ┌─────────────────────────┐      ┌────────────────────────────┐
    │ CITY VALUES    │    │     STACK TABLES        │      │   GARRISON STRENGTHS       │
    └────────────────┘    └─────────────────────────┘      └────────────────────────────┘

                          AI_Player_Calculate_Target_Values
                                         │
                                         │ writes
                                         ▼
                                ai_transport_count
                                (also written by AIBUILD.c)
```

## Per-array fan-out

### `_ai_all_own_stacks[]` / `_ai_all_own_stack_count`

Most heavily consumed of the lot. Continent eval, NPC routing, move planning, and transport finding all hit this.

```
AIDUDES.c  AI_Player_Calculate_Target_Values  ──┐
AIDATA.c   AI_Build_Own_Stacks (alt writer)   ──┴── WRITERS
                              │
                              ▼
   ┌──────────────────────────┼────────────────────────────┐
   │                          │                            │
AIDUDES.c                  AIDATA.c                     AIMOVE.c
AI_Landmass_Values_And_Strengths     AI_NPC_Set_Stack_           multiple readers:
  line 427-447             Destinations
  reads .value, .wx, .wy,    line 605-609              line 3401  AI_Sail_To_Continent / unit_status
        .wp                  reads stacks for          line 3779  plane-flip for stranded stacks
  ↓                          NPC routing               line 3899  ocean-adjacent transport scan
  populates                                            line 5096  AICAP_LandOnly path filter
  AI_Cont_Own_Str[][]                                  line 5712  transport-finding for embarkers
  (per-landmass strength)                              line 6309  stack as ptr in cleanup
                                                       line 6372  ".value = ST_UNDEFINED" (done marker)
                                                       line 6414  stack-by-idx position read
                                                       line 6443  ".value = ST_UNDEFINED" (done marker)
```

**Key consumer pattern:** `AIMOVE.c` treats `.value = ST_UNDEFINED` as a "this stack has been ordered, skip it" sentinel ([lines 6372, 6443](../MoM/src/AIMOVE.c#L6372)). The producer initializes `.value` to a real strength number; the move planner overwrites it as it dispatches orders.

### `_ai_all_enemy_stacks[]` / `_ai_all_enemy_stack_count`

Single consumer.

```
AIDUDES.c  AI_Player_Calculate_Target_Values  ── WRITER
                              │
                              ▼
                       AIDUDES.c
                       AI_Landmass_Values_And_Strengths
                         line 449-462
                         reads .value, .wx, .wy, .wp
                         ↓
                         populates
                         CRP_AI_Cont_Nme_Str[][]
                         (per-landmass enemy strength)
```

No `AIMOVE.c` / `AIBUILD.c` / `AISPELL.c` consumer. The enemy stack table feeds **only** the continent-balance scoring.

### `_ai_all_own_city_values[]` / `_ai_all_enemy_city_values[]`

Strategy + spell-targeting on the enemy side; strategy only on the own side.

```
AIDUDES.c  AI_Player_Calculate_Target_Values  ── WRITER
                              │
              ┌───────────────┴────────────────┐
              ▼                                ▼
        AIDUDES.c                        AISPELL.c
        AI_Landmass_Values_And_Strengths           overland spell picker
          line 528  ← _ai_all_own_city_values     line 863  if (_ai_all_enemy_city_values[i] > best)
          line 530  ← _ai_all_enemy_city_values   line 964  best = _ai_all_enemy_city_values[i]
          ↓                                       ↓
          AI_Cont_Own_Val[][]                     picks target city for
          AI_Cont_Nme_Val[][]                     negative city-area spells
```

`AISPELL.c:863, 964` is the picker for negative city-area spells (Chaos Rift, Call the Void, etc.) — it scans `_ai_all_enemy_city_values[]` for the highest-value enemy city.

### `_ai_all_enemy_garrison_strengths[]` / `_ai_all_own_garrison_strengths[]` — WRITE-ONLY

```
AIDUDES.c  AI_Player_Calculate_Target_Values  ── WRITER
                              │
                              ▼
                     ┌─────────────────┐
                     │  NO CONSUMERS   │
                     │  (in current    │
                     │  reconstructed  │
                     │  source)        │
                     └─────────────────┘
```

Both garrison-strength arrays are populated faithfully but **never read anywhere in the codebase today**. Same pattern as `_cp_hostile_opponents` — the original-game consumers were the AI overland spell picker (garrison-target weighting for buff/curse spells on cities), which is still stubbed. The data is staged for when those modules get reconstructed.

### `ai_transport_count` — has a *second writer*

```
WRITERS:
  AIDUDES.c  AI_Player_Calculate_Target_Values   line 1687-1696  (sets to 0, then counts)
  AIBUILD.c  (some build planning function)      line 827, 853   (also resets and recounts)

READERS (both in AIBUILD.c, both in build-weight calculation):
  AIBUILD.c  line 514-529   weights ship/transport build priority
  AIBUILD.c  line 990-999   second weighting site
                            (CP / HP marker comments — likely current/historical playtesting)
```

**This is the only array in the set with a second writer.** AIDUDES.c's count and AIBUILD.c's count could disagree depending on call order. If `AI_Player_Calculate_Target_Values` runs after `AIBUILD.c`'s pass, the AIDUDES.c count wins; otherwise vice versa. The build planner reads it back at lines 514 and 990 — so whichever value is last-written before those reads is what's used. Worth confirming the OG call sequence preserves the intended write-then-read ordering.

## Summary table

| Array | Primary consumers | Stage of AI turn |
|---|---|---|
| `_ai_all_own_stacks[]` | Continent eval, NPC routing, move planning, transport finding | Strategy & dispatch |
| `_ai_all_enemy_stacks[]` | Continent eval only | Strategy |
| `_ai_all_enemy_city_values[]` | Continent eval, spell-targeting | Strategy & spell picker |
| `_ai_all_own_city_values[]` | Continent eval only | Strategy |
| `_ai_all_enemy_garrison_strengths[]` | none yet | (stubbed spell picker) |
| `_ai_all_own_garrison_strengths[]` | none yet | (stubbed spell picker) |
| `ai_transport_count` | Build planner | City production |

## Cross-cutting observations

**The strategy-vs-dispatch split:** Almost everything feeds `AI_Landmass_Values_And_Strengths` (the per-landmass balance computation). Beyond that, only `_ai_all_own_stacks` flows downstream into `AIMOVE.c` for actual unit orders — and that's where the "set `.value = ST_UNDEFINED` to mark done" pattern lives. `AI_Landmass_Values_And_Strengths` is therefore the central junction between fact-gathering and decision-making.

**The hidden write-only data** (garrison-strength arrays) is the most interesting finding for forward planning: when whoever reconstructs the area-spell picker comes through, the data they need is already being computed — the only thing missing is the reader. That's the opposite of the more common "consumer exists but producer is stubbed" pattern.

**The duplicate writer on `ai_transport_count`** is the only entry in the set where two different functions both reset-and-recount. Worth verifying that the OG call order makes the AIBUILD.c writes happen after the AIDUDES.c writes (or vice versa, consistently) so the build planner reads from a known stage.

## Related references

- [MoM-NextTurn-AI.md](MoM-NextTurn-AI.md) — overall AI turn structure, function rename map
- [MoM-AI-Data.md](MoM-AI-Data.md) — original IDA names for these arrays, plus the parallel-array `_ai_own_stack_*` family used by `AIMOVE.c` move planning
- [MoM-AI-Continents.md](MoM-AI-Continents.md) — how `AI_Cont_*_Str` / `AI_Cont_*_Val` (the downstream consumers) get used
