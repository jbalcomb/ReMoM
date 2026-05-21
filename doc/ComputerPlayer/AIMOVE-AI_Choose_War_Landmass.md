AIMOVE-AI_Choose_War_Landmass.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr162\AI_Pick_Action_Conts__WIP.c



I don't think I follow. How does it make sense to have a main war landmass?
(Am I just distracted by/fixated on the weirdness that is the 'First_Hostile_Player'?)



AI_Next_Turn()
    |-> AI_Choose_War_Landmass(player_idx);  /* populates _ai_landmass_war_targets[]; uses the arrays just populated in AI_Landmass_Values_And_Strengths() */


AI_Choose_War_Landmass() populates _ai_landmass_war_targets[]


So a tighter description: "head of the per-landmass linked-list of land squares that qualify as transport embarkation points."
There's a family of 4 related globals that should rename coherently:
Current	Role
_ai_landmass_dock_squares_heads[plane][landmass]	head index of chain (this one)
_ai_landmass_dock_squares_lists[plane][idx]	next-pointer in chain
_ai_landmass_dock_squares_wx_array[plane][idx]	X coord of chain entry
_ai_landmass_dock_squares_wy_array[plane][idx]	Y coord of chain entry
Per project conventions (g_ prefix for new globals, use "square" not "tile"), let me ask which direction you want:
AskUserQuestion
    User has answered your questions: "Which naming style do you prefer for the CONTX_*LoadT* family?"="Shore-adjacent land". You can now continue with the user's answers in mind.
    Good choice. Let me first scope the change — grep for all uses of each of the 4 names.

CONTX_1stLoadTs     _ai_landmass_dock_squares_heads
CONTX_LoadTChain    _ai_landmass_dock_squares_lists
_ai_landmass_dock_squares_wx_array    _ai_landmass_dock_squares_wx_array
_ai_landmass_dock_squares_wy_array    _ai_landmass_dock_squares_wy_array
The trade-off:
    Naming	                                What it says
    g_world_landmass_first_shore_adj_square	"land square within 1 of shore" (how it's defined)
    g_world_landmass_first_embark_square	"where units board/leave transports" (what it's used for)



## _ai_landmass_war_targets[]
XREF:
    Allocate_Data_Space+3B4         mov     [_ai_landmass_war_targets], (offset _players.spells_list+17E8h)  
    AI_ProcessOcean__WIP+D          mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_ProcessOcean__WIP+3B         mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_ProcessOcean__WIP+6B         mov     bx, [_ai_landmass_war_targets+bx]                                
    G_AI_ProcessTransports__WIP+488 mov     bx, [_ai_landmass_war_targets+bx]                                
    G_AI_ProcessTransports__WIP+774 mov     bx, [_ai_landmass_war_targets+bx]                                
    G_AI_ProcessTransports__WIP+798 mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_ProcessRoamers__WIP+148      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_ProcessRoamers__WIP+15F      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_ProcessRoamers__WIP+187      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_PullForMainWar__WIP+F        mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_PullForMainWar__WIP+8C       mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_PullForMainWar__WIP+119      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_PullForMainWar__WIP+145      mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_SingleCont_Reeval__WIP+8DF   mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Reevaluate_All_Continents+D94    mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Reevaluate_All_Continents+DE2    mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Choose_War_Landmass    mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Choose_War_LandmassC   mov     bx, [_ai_landmass_war_targets+bx]                                
    AI_Choose_War_Landmass2   mov     bx, [_ai_landmass_war_targets+bx]                                
    Allocate_Data_Space+3BA         mov     [_ai_landmass_war_targets+2], (offset _players.spells_list+17F4h)

---

# AI_Choose_War_Landmass() Walkthrough (re-review with GEMINI second opinion)

**Location:** [MoM/src/AIMOVE.c:7918](../../MoM/src/AIMOVE.c#L7918) (~361 lines, ends line 8278). GEMINI variant at [line 8280-8501](../../MoM/src/AIMOVE.c#L8280).
**WZD overlay:** ovr162, p37
**drake178 name:** (none — new helper / not directly named in disassembly)

## Re-review headline

The function has been improved meaningfully since the previous review:

1. **`Continent_Values_2` renamed to `final_landmass_weights`** — much clearer name for the final weighted-pick input.
2. **`First_Hostile_Player` renamed to `first_hostile_player_idx`** — standard `_idx` convention.
3. **`Own_City_Count` renamed to `own_city_count`** — lowercase consistency.
4. **OGBUG annotations added at the source** ([line 7945-7946](../../MoM/src/AIMOVE.c#L7945)): explicit `/* OGBUG  should check spl_Spell_Of_Mastery, not spl_Fire_Elemental */` and `/* OGBUG  should exclude self - 'itr_players != player_idx' */`.
5. **CONTX_* rename propagated** — uses `g_world_*__load_init` correctly at lines 8151, 8158, 8159, 8161, 8168.

Most of my previous OGBUG flags survive the re-review. The Value_Sum bug I flagged in earlier sessions remains fixed. But two divergences from GEMINI deserve attention.

## Phases summary (re-traced from current source)

| # | Lines | Phase | State |
|---|---|---|---|
| 0 | 7941-7943 | Setup: clear first_hostile_player_idx + EMM page in | OK |
| 1 | 7947-7957 | Find first hostile player (war or SoM-caster) | OGBUG (Fire Elemental, no self-exclude) — flagged in source |
| 2 | 7960-8065 | Per-plane: load current `_ai_landmass_war_targets[wp][p]`; switch on its `lmt_*` type to decide `Reevaluate` | OK; switch is complete except `lmt_NoTargets` falls to `default: DNE` (see D-1 below) |
| 3 | 8073-8077 | If reeval: zero `final_landmass_weights[]` and `Continent_Values[]` | OK |
| 4 | 8083-8129 | Per-city pass: accumulate empire centroid (own cities) + raw `Continent_Values[]` (non-own cities, weighted by wartime state) | OGBUG flagged in source for cross-plane city match |
| 5 | 8131-8140 | Compute empire centroid (or fortress fallback) | OK |
| 6 | 8149-8214 | Per-landmass: find closest-load-tile distance from empire centroid, compute weighted `final_landmass_weights[itr_landmasses]` (peace or war formula), apply Jihad +1000 fortress bonus | OK; uses correctly-renamed g_world_*__load_init arrays |
| 7 | 8220-8229 | "First Double-Check" loop summing weights into `Value_Sum` | **FIXED** (now has explicit `Value_Sum = 0;` at line 8223) |
| 8 | 8231-8256 | Tile-count fallback when Value_Sum == 0 (replaces empty weights with tile counts for eligible landmasses) | **DIVERGES from GEMINI on fortress polarity (D-2)** |
| 9 | 8260-8272 | Final pick: re-sum (this one properly resets Value_Sum), set `_ai_landmass_war_targets[wp][p]` to 0 or `Get_Weighted_Choice(final_landmass_weights, NUM_LANDMASSES)` | OK |

## Bugs

### ✅ ~~BUG-B~~ — Phase 8 fortress-continent gate (verified against disassembly: production matches OG)

```c
// AIMOVE.c:8241-8253 (current production — matches OG)
if(
    (_FORTRESSES[player_idx].active == ST_FALSE)
    ||
    (itr_landmasses == _landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[player_idx].wy * WORLD_WIDTH) + _FORTRESSES[player_idx].wx)])
)
{
    Value_Sum = CONTX_FirstTiles[wp][itr_landmasses];
    while(Value_Sum != ST_UNDEFINED)
    {
        final_landmass_weights[itr_landmasses] += 1;
        Value_Sum = CONTX_TileChain[wp][Value_Sum];
    }
}
```

**The OG comment at lines 8226-8229 is wrong**, and the GEMINI second opinion at [line 8468-8474](../../MoM/src/AIMOVE.c#L8468) followed the comment to the wrong conclusion. Disassembly confirms the OG bytes match production's gate: **count tiles if this player's fortress is not active, OR if this landmass IS this player's home continent.**

**Semantic interpretation:**
- **Fortress active:** the only fallback target is the player's own home continent. The AI focuses defensive fallback weight on protecting the capital.
- **Fortress inactive (banished):** count tiles for ALL non-Own non-NoTargets landmasses — without a home, any landmass is a candidate.

This makes sense as a defensive-priority heuristic: an active wizard fallbacks toward home; a banished wizard fallbacks anywhere. The OG comment's "with the exception of the fortress continent" was a misreading of the code's intent. My earlier BUG-B flag was wrong — I trusted the comment and GEMINI rather than verifying against the disassembly. **Resolved as not-a-bug.**

### Preserved OG bugs (faithful-to-disassembly, flagged in source)

| # | Site | OG bug | Source flag |
|---|---|---|---|
| OGBUG-C | [Line 7945](../../MoM/src/AIMOVE.c#L7945) | Fire Elemental check should be Spell of Mastery | `/* OGBUG */` comment |
| OGBUG-D | [Line 7946](../../MoM/src/AIMOVE.c#L7946) | `first_hostile_player_idx` loop doesn't exclude self | `/* OGBUG */` comment |
| OGBUG-E | [Line 8006-8007](../../MoM/src/AIMOVE.c#L8006) | Cross-plane city match possible (uses `wp` for landmass lookup but city's `wp` might differ) | `// BUG:` comment |
| OGBUG-F | [Line 8147](../../MoM/src/AIMOVE.c#L8147) | "BUG? uses an array that holds previous data at this point in time" | comment (rebutted in earlier session: `g_world_*__load_init` arrays are immutable per game, comment is overcautious) |
| OGBUG-G | [Line 8160](../../MoM/src/AIMOVE.c#L8160) | "isn't this the previous data? the table is not recalculated before this point" | comment (same as OGBUG-F) |
| OGBUG-H | [Line 8148](../../MoM/src/AIMOVE.c#L8148) | "scaling everything by the same value does not change the weights" | comment — observational, not actionable |
| OGBUG-I | [Line 8195](../../MoM/src/AIMOVE.c#L8195) | Wartime formula `(2000 - enemy×10)` decreases with enemy density — counter-intuitive | No source comment; may be intentional "remaining-to-conquer" value, see below |
| OGBUG-J | [Line 8230](../../MoM/src/AIMOVE.c#L8230) | "fortress exclusion also counts the other plane" | `// BUG:` comment (the lookup uses `wp` directly so it's plane-correct here — but if `wp` is stale from outer loop, it could mismatch the fortress's actual plane) |

### Note on OGBUG-I (wartime formula counter-intuition)

The formula `(2000 - Continent_Values[itr_landmasses] * 10) / Closest_LoadTile_Dist` decreases with enemy city count:
- 1 enemy city: `(2000 - 10) / dist` = 1990/dist
- 5 enemy cities: `(2000 - 50) / dist` = 1950/dist
- 20 enemy cities: `(2000 - 200) / dist` = 1800/dist
- 200 enemy cities: `(2000 - 2000) / dist` = 0

Possible interpretation: this represents **"remaining unconquered value"** — the AI prefers landmasses where there's less enemy strength to deal with, treating already-heavily-enemy-occupied landmasses as harder targets. Under this reading, the formula isn't a bug but an intentional risk-aversion heuristic.

Alternative interpretation: it's just wrong, should be `Continent_Values × 10 / Closest_LoadTile_Dist` (proportional to enemy density), matching the peacetime formula's pattern.

GEMINI uses the same `(2000 - enemy × 10) / dist` formula at [line 8433](../../MoM/src/AIMOVE.c#L8433) — both readings preserve it. Whether the OG bytes really do this or it's a shared translation error needs disassembly check.

## Production vs GEMINI — divergences

| # | Site | Production | GEMINI | Likely OG truth |
|---|---|---|---|---|
| **D-1** | Phase 2 switch — `lmt_NoTargets` handling | Falls through `default: { /* DNE */ }` → Reevaluate stays FALSE | Handled with `lmt_NoOwnCity` and `lmt_NoOwnCityAndAllyHasCity` in same branch ([line 8327](../../MoM/src/AIMOVE.c#L8327): `type == 3 \|\| type == 4 \|\| type == 6`) | Probably GEMINI — the comment at [line 7977](../../MoM/src/AIMOVE.c#L7977) "(type - 1), so {1,2,3,4,5}" suggests the OG switch only dispatches values 1-5, but the runtime can produce 6 (`lmt_NoTargets`). Production's `default: DNE` is correct if the OG truly never sees value 6 here — but `lmt_NoTargets` IS a valid runtime classification ([AIMOVE.c:2757](../../MoM/src/AIMOVE.c#L2757) sets it). |
| ~~D-2~~ | Phase 8 fortress-continent polarity | Includes fortress landmass only | Excludes fortress landmass | **PRODUCTION** — verified against disassembly. The OG bytes match production; OG comment and GEMINI are both wrong. |
| ~~D-3~~ | ~~Phase 7 Value_Sum reset~~ | ~~Missing (BUG-A)~~ | ~~Has explicit reset~~ | **RESOLVED** — production now has `Value_Sum = 0;` at line 8223 |
| **D-4** | Phase 8 below-threshold filter | None | GEMINI adds `if (final_landmass_weights[itr] < 10) { final_landmass_weights[itr] = 0; }` ([line 8482](../../MoM/src/AIMOVE.c#L8482)) | Unclear — could be GEMINI-only refinement |
| **D-5** | GEMINI tile-count fallback index bug | Uses `itr` correctly | Uses `landmas_idx` (the OUTER variable, frozen) for the type check at [line 8463](../../MoM/src/AIMOVE.c#L8463) | **PRODUCTION** — GEMINI has its own bug here |
| **D-6** | Peace formula location | Inside `if (first_hostile_player_idx <= ST_UNDEFINED)` branch at line 8181 | Inside the matching `else` branch at line 8447 — same logical location, different code structure | Equivalent; cosmetic difference |
| **D-7** | `first_hostile_player_idx` search loop break | Uses `& (first_hostile_player_idx == ST_UNDEFINED)` in for-condition | Uses `if (first_hostile_player_idx != ST_UNDEFINED) break;` after each iteration | Equivalent |

**Tally:**
- D-1, D-4: uncertain
- D-5: GEMINI has its own bug; production is correct here
- D-6, D-7: equivalent
- ~~D-2 (fortress polarity)~~: **resolved** — disassembly confirms production matches OG; comment and GEMINI were both wrong
- ~~D-3 (Value_Sum reset)~~: **resolved** — production now correctly resets

## Comparison with sibling functions

| Pattern | `AI_Choose_War_Landmass` | `AI_Reevaluate_All_Continents` | `AI_SingleCont_Reeval__WIP` |
|---|---|---|---|
| Value_Sum-pattern bugs | Has BUG-A (first sum doesn't reset) | Earlier had similar bug, now fixed | N/A |
| Fortress-continent polarity | ~~Was flagged as BUG-B~~ — verified production matches OG; comment was misleading | Has stale-`wp` bug at fortress check | N/A |
| OGBUG (Fire Elemental vs SoM) | Yes (flagged) | Yes (flagged) | N/A |
| OGBUG (no self-exclude) | Yes (flagged) | N/A | N/A |
| Cross-plane city match | Yes (flagged) | Yes (D-2, D-3 in that doc) | Yes (OGBUG-A/B/C in that doc) |
| Switch on `lmt_*` type | Yes (Phase 2) | No (uses if-chains) | No |
| Renames since last review | Continent_Values_2 → final_landmass_weights, First_Hostile_Player → first_hostile_player_idx | Same naming patterns | square_weight, stage_wx/wy, square_occupation_value |

## Recommendation

**No outstanding production bugs in this function.** Both previously-flagged candidates are resolved:

- **BUG-A** (Value_Sum reset missing) was a transcription error and has been fixed — production now correctly resets `Value_Sum` at line 8223.
- **BUG-B** (Phase 8 fortress polarity) was a misdiagnosis on my part — I trusted the OG comment and GEMINI second opinion without verifying against the disassembly. Disassembly confirms production matches the OG bytes; the comment is the thing that's wrong, not the code.

The preserved OGBUGs (Fire Elemental, no self-exclude, cross-plane) are already correctly flagged in source as faithful-to-disassembly. OGBUG-F and OGBUG-G (the "previous data" concerns) were rebutted in an earlier session: the `g_world_*__load_init` arrays are immutable per game, so the comments are overcautious. Worth removing those `BUG?` markers since we've established the underlying inputs are static.

The OG comment at lines 8226-8229 saying "with the exception of the fortress continent" is misleading and should be reworded to match the actual code behavior — "include only the fortress landmass when active, or all landmasses when banished."

## Related references

- [AIMOVE-AI_SingleCont_Reeval__WIP.md](AIMOVE-AI_SingleCont_Reeval__WIP.md) — per-landmass classifier; reads _ai_landmass_war_targets in its Phase 7
- [AIMOVE-AI_Reevaluate_All_Continents.md](AIMOVE-AI_Reevaluate_All_Continents.md) — full-rebuild classifier; reads _ai_landmass_war_targets in its Phase 14
- [MoM-AI-Landmass-Types.md](MoM-AI-Landmass-Types.md) — `lmt_*` enum that gates the Phase 2 switch
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
