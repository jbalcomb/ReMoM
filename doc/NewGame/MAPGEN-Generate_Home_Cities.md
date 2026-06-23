MAPGEN-Generate_Home_Cities.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Home_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Home_City.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Random_City_Name_By_Race_NewGame.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Random_City_Name_By_Race_NewGame.c

Init_New_Game()
    |-> ... Generate_Towers / Generate_Lairs ...
    |-> Generate_Home_Cities();           [MAPGEN.c:420]
        |-> Random_City_Name_By_Race_NewGame()

---

# `Generate_Home_Cities` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Generate_Home_Cities` | [MAPGEN.c:878-1315](../../MoM/src/MAPGEN.c#L878-L1315) | Places each wizard's starting fortress + capital city: random-picks fortress sites under distance/terrain/population constraints (relaxing the distances on repeated failure), seeds each capital's race / fields / starting buildings, then spawns the starting spearmen + swordsmen garrison. |
| `Generate_Home_City__GEMINI` | [MAPGEN.c:1316+](../../MoM/src/MAPGEN.c#L1316) (inside `#if 0`) | Reference IDA→C translation (= Piethawn `*.c`); kept for cross-reference. **Diverges from the asm on the High-Elf forest test** — see [B4](#og-bugs--quirks-preserved). |

Verified faithful to the disassembly `Generate_Home_City.asm` — re-checked operand-by-operand against the live source: the five `Random()` sites, the location/validity gates (incl. the **halved** site-distance thresholds), the population gate, the race rolls (all three jump tables read from the table data), the High-Elf re-roll, and unit creation all match 1:1. The OG name is singular (`Generate_Home_City`) though it does every player. Carries debug scaffolding (`DBG_*` + `[LOC_*]` `LOG_INFO` + `STU_DEBUG_BREAK`) — RNG-neutral; see [below](#debug-scaffolding-transitory).

## Purpose

Runs once during map generation (after nodes/towers/lairs exist). For each of up to `NUM_PLAYERS` fortress slots it random-walks a site that is land, far enough from other fortresses / nodes / towers / lairs, and on a square whose max city size can support the start population; the distance thresholds shrink if a full pass fails. It then builds one capital per actual player (`_num_players`), assigns races (human = chosen; AI = rolled by plane + difficulty), sets fixed fields/buildings, names the city, and creates the garrison.

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)). **Five** call sites: `Random(54)` + `Random(34)` per location attempt, and one race roll per non-human capital (`Random(5)` Myrror, `Random(9)` Arcanus ≤Normal, `Random(5)` Arcanus >Normal).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_New_Game` / MAPGEN | [MAPGEN.c:420](../../MoM/src/MAPGEN.c#L420) | After the encounter tables are built; followed by `Validate_All_Cities` then `Generate_Neutral_Cities`. |

## Code walk

Line refs are production [MAPGEN.c](../../MoM/src/MAPGEN.c); cross-checked against `Generate_Home_City.asm` (the authority). Stack-var note: OG `UU_Fortresses[2]` → production `niu_fortresses[NUM_PLANES]`.

### Distance loop ([908-928](../../MoM/src/MAPGEN.c#L908-L928))

`minimum_fortress_distance = 16`, `minimum_site_distance = 8`; each `Loop_Distances` pass decrements both then floors them via `SETMIN(site,1)` / `SETMIN(fortress,10)` (`SETMIN(a,b)` raises `a` to `b`). Resets `Tries_Per_Distance`, `niu_fortresses[]`, `max_pop_failures`. Matches asm `@@Loop_Distances` (init 16/8, `dec`/`dec`, floor 1/10). **Faithful.**

### Location pick + validity ([935-1046](../../MoM/src/MAPGEN.c#L935-L1046))

`for player_idx in 0..NUM_PLAYERS-1` ([935](../../MoM/src/MAPGEN.c#L935) — [B1](#og-bugs--quirks-preserved)), body gated by `Tries_Per_Distance < 1000`. `Loop_Location_1` ([942](../../MoM/src/MAPGEN.c#L942)): `tries++`; `wp = myrran ? MYRROR : ARCANUS`; `wx = 2+Random(54)`, `wy = 2+Random(34)` ([949-950](../../MoM/src/MAPGEN.c#L949-L950)). Five reject tests set `Invalid`:
1. square is `tt_Ocean` ([955](../../MoM/src/MAPGEN.c#L955));
2. fortress: `Delta_XY_With_Wrap < minimum_fortress_distance` (on-plane) ([966](../../MoM/src/MAPGEN.c#L966)) — **full** distance, asm has no `sar` here;
3. node: `Delta_XY_With_Wrap < (minimum_site_distance / 2)` (on-plane) ([980](../../MoM/src/MAPGEN.c#L980));
4. tower: `Range < (minimum_site_distance / 2)` ([993](../../MoM/src/MAPGEN.c#L993) — `Range`, un-wrapped: [B2](#og-bugs--quirks-preserved));
5. lair: `Delta_XY_With_Wrap < (minimum_site_distance / 2)` (on-plane) ([1009](../../MoM/src/MAPGEN.c#L1009)).

The three **site** checks (node/tower/lair) compare against `minimum_site_distance / 2` — 1:1 with the asm, which halves the threshold via the signed `cwd; sub ax,dx; sar ax,1` idiom (asm lines 162-167 / 200-205 / 249-254) before each `cmp` ([B3](#og-bugs--quirks-preserved)). The fortress check uses the full distance. If `tries < 1000 && Invalid` → `goto Loop_Location_1` ([1045](../../MoM/src/MAPGEN.c#L1045)). **Faithful.**

### Population gate ([1048-1093](../../MoM/src/MAPGEN.c#L1048-L1093))

`pop_min = 8 - player_idx/3` ([1050](../../MoM/src/MAPGEN.c#L1050)); `pop_max = City_Maximum_Size_NewGame(wx,wy,wp)` ([1051](../../MoM/src/MAPGEN.c#L1051)). **Reject if `pop_min > pop_max`** ([1054](../../MoM/src/MAPGEN.c#L1054)) — accept iff `pop_max >= pop_min`, 1:1 with asm `cmp ax,dx; jge @@AddFortress`. On reject: `max_pop_failures++`; `>500` → `goto Loop_MaxPopTries`, else `goto Loop_Location_1` ([1068](../../MoM/src/MAPGEN.c#L1068)). On accept: write `_FORTRESSES[player_idx]` (`wx/wy/wp/active`) + `niu_fortresses[wp]++` ([1073-1078](../../MoM/src/MAPGEN.c#L1073-L1078)). After the pass, `if(Tries_Per_Distance >= 1000) goto Loop_Distances` ([1086](../../MoM/src/MAPGEN.c#L1086)). **Faithful.**

### Capitals: race + fields ([1100-1224](../../MoM/src/MAPGEN.c#L1100-L1224))

`_cities = 0` ([1100](../../MoM/src/MAPGEN.c#L1100)); `for itr in 0.._num_players-1`. Race: player 0 = `NEWG_Clicked_Race` ([1110](../../MoM/src/MAPGEN.c#L1110)); else Myrror `switch(Random(5)-1)` ([1116](../../MoM/src/MAPGEN.c#L1116)); Arcanus `_difficulty <= god_Normal` `switch(Random(9)-1)` ([1129](../../MoM/src/MAPGEN.c#L1129)) else `switch(Random(5)-1)` ([1144](../../MoM/src/MAPGEN.c#L1144)). Branch order, the `<=Normal`→9-way test, and the case→race tables all match the asm jump tables (verified against the table data: `off_449C6` Myrror, `off_449BC` Arc>Normal, `off_449AA` Arc≤Normal — each identity-ordered). Written to `_CITIES[_cities]` (owner = `itr`); `_cities` advances in lockstep with `itr`.

**High-Elf re-roll** ([1154-1162](../../MoM/src/MAPGEN.c#L1154-L1162)): for non-human cities, `if(race==rt_High_Elf && !Square_Is_Forest_NewGame(wx,wy,wp)) goto Loop_Location_1` — 1:1 with asm `loc_44375` (`jmp @@Loop_Location`). The human (`itr==0`) is **not** subject to this check (asm jmps past it). Two preserved OG defects live here — the forest test reads leftover `wx/wy/wp` ([B4](#og-bugs--quirks-preserved)) and the jump lands in a bad state ([B5](#og-bugs--quirks-preserved)).

Then fixed fields: `size=1`/Hamlet ([1173](../../MoM/src/MAPGEN.c#L1173)), `population=4` ([1174](../../MoM/src/MAPGEN.c#L1174)), construction human=`bt_Housing`/else `bt_AUTOBUILD`, all enchantments cleared ([1186-1211](../../MoM/src/MAPGEN.c#L1186-L1211)), `farmer_count = 3` ([1212](../../MoM/src/MAPGEN.c#L1212)), Smithy/Barracks/BuildersHall built ([1218-1220](../../MoM/src/MAPGEN.c#L1218-L1220)), `Random_City_Name_By_Race_NewGame` ([1222](../../MoM/src/MAPGEN.c#L1222)), `_cities++`. **Faithful.**

### Garrison ([1237-1308](../../MoM/src/MAPGEN.c#L1237-L1308))

`Done_Done:` — spearman loop: every non-Dwarf capital gets its race spearman ([1266-1269](../../MoM/src/MAPGEN.c#L1266-L1269)). Swordsman loop: Dwarf gets an **extra** swordsman first ([1300-1303](../../MoM/src/MAPGEN.c#L1300-L1303)), then **all** capitals get a swordsman ([1307](../../MoM/src/MAPGEN.c#L1307)). Net: Dwarves = 2 swordsmen, others = spearman + swordsman. The race→unit switches match the asm (`off_4498E` spearman table read; identity-ordered, incl. `Dwarf→Swordsmen`); the three `Create_Unit_NewGame` sites map to asm 1037 / 1156 / 1188. **Faithful.**

## OG bugs / quirks preserved

| # | Line | What |
|---|---|---|
| B1 | [935](../../MoM/src/MAPGEN.c#L935) | The fortress-placement loop runs `player_idx < NUM_PLAYERS` (6) though at most 5 wizards exist (asm `cmp player_idx, 6`). The capital/garrison loops correctly use `_num_players`. |
| B2 | [993](../../MoM/src/MAPGEN.c#L993) | The **tower** distance test uses `Range()` (un-wrapped) while fortress/node/lair use `Delta_XY_With_Wrap()`. Inconsistent, but faithful (asm `call Range`). |
| B3 | [975](../../MoM/src/MAPGEN.c#L975), [989](../../MoM/src/MAPGEN.c#L989), [1004](../../MoM/src/MAPGEN.c#L1004) | The site-distance threshold is **halved** (`minimum_site_distance / 2`, matching the asm's `sar`). Genuine OG consequence, preserved: once the distance loop shrinks `minimum_site_distance` to `1`, `1 / 2 == 0`, so `< 0` never trips and a node/tower/lair on the **same square** isn't excluded. Implemented faithfully — not a reconstruction gap. |
| B4 | [1154](../../MoM/src/MAPGEN.c#L1154), [1156](../../MoM/src/MAPGEN.c#L1156) | The High-Elf forest test reads the leftover `wx/wy/wp` (the *last* fortress placed in the location loop), **not** this city's own square `_FORTRESSES[itr]` — so for every city except the last-placed one it tests the wrong tile. Faithful to the asm (`loc_44375` pushes `[bp+wx]/[bp+wy]/[bp+wp]`). **`__GEMINI` diverges** here — it uses `_FORTRESSES[itr].wx/.wy/.wp`, silently "fixing" the OG bug; do **not** match GEMINI. |
| B5 | [1161](../../MoM/src/MAPGEN.c#L1161), [1162](../../MoM/src/MAPGEN.c#L1162) | Taking the High-Elf reject `goto Loop_Location_1` lands in a bad state: the location loop has already completed, so `player_idx == NUM_PLAYERS` (6) here (this loop iterates `itr`). Re-entering does a junk location pick for the nonexistent 6th slot (`_players[6]`/`_FORTRESSES[6]` — the B1 over-iteration) before the city loop restarts at `_cities=0`. Also unconditional — unlike the validity rejects it isn't gated by `tries < 1000`. Faithful to the asm (`jmp @@Loop_Location`, `player_idx` left at 6, no `tries` gate). |

## Debug scaffolding (transitory)

Instrumented for the new-game RNG-divergence hunt — none OG, all RNG-neutral:
- `DBG_Invalid_Reason*` counters + `DBG_Loop_Location[]` + `DBG_pop_min/max` ([893-903](../../MoM/src/MAPGEN.c#L893-L903), reset at [923-928](../../MoM/src/MAPGEN.c#L923-L928)).
- `[LOC_PICK]` per-attempt trace ([1036](../../MoM/src/MAPGEN.c#L1036)); `[LOC_GIVEUP_POP]` / `[LOC_GIVEUP_DIST]` give-up dumps ([1061](../../MoM/src/MAPGEN.c#L1061) / [1090](../../MoM/src/MAPGEN.c#L1090)); `Done_Done:` marker ([1239](../../MoM/src/MAPGEN.c#L1239)).
- `STU_DEBUG_BREAK()` on the `max_pop_failures > 500` path ([1062](../../MoM/src/MAPGEN.c#L1062)).

Strip or guard once the new-game path is confirmed RNG-aligned.

## Notes vs `__GEMINI`

GEMINI matches the asm on the structure, the five `Random` sites, the race tables, and the goto targets — **except the High-Elf forest test ([B4](#og-bugs--quirks-preserved))**, where it passes `_FORTRESSES[itr].wx/.wy/.wp` instead of the OG's leftover `wx/wy/wp`. The asm is the authority; production is correct and GEMINI is the unfaithful side here. Treat GEMINI as a second opinion, not OG-truth.

## Note on RNG consumption

The function is 1:1 with the asm, but its actual `Random()` *count* is input-driven: every reject (Ocean / fortress / node / tower / lair / low-pop / High-Elf-not-Forest) burns another `Random(54)`+`Random(34)`. A divergence here is usually a symptom of wrong upstream state (`p_world_map`, `_NODES`/`_TOWERS`/`_LAIRS` positions) rather than this code.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — returns `1..n`.
- **`City_Maximum_Size_NewGame(wx,wy,wp)`** — estimated max city size (the population gate).
- **`Square_Is_Forest_NewGame` / `GET_TERRAIN_TYPE`** — terrain tests (High-Elf forest, Ocean).
- **`Delta_XY_With_Wrap` / `Range`** — distance tests (wrapped vs un-wrapped — B2).
- **`Random_City_Name_By_Race_NewGame`**, **`Create_Unit_NewGame`** — name + garrison (own `Random` use counted in those functions).
- **`SETMIN(a,b)`** (raise `a` to `b`) — `MOX_DEF.h`.
- Globals: **`_FORTRESSES[]`**, **`_CITIES[]`**, **`_cities`**, **`_NODES[]`**, **`_TOWERS[]`**, **`_LAIRS[]`**, **`_players[]`**, **`_num_players`**, **`_difficulty`**, **`NEWG_Clicked_Race`**, **`p_world_map`**.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Home_City.asm` — IDA Pro 5.5 disassembly (the authority; 5 `call Random`; the three site checks halve via `sar ax,1`; High-Elf check `loc_44375` uses `[bp+wx]/[bp+wy]/[bp+wp]` then `jmp @@Loop_Location`; race tables `off_449C6`/`off_449BC`/`off_449AA`; spearman table `off_4498E`).
- [MAPGEN.c:1316+](../../MoM/src/MAPGEN.c#L1316) — `__GEMINI` reference translation (`#if 0`); note the [B4](#og-bugs--quirks-preserved) divergence.
- [MAPGEN.c:420](../../MoM/src/MAPGEN.c#L420) — call site.
- [MAPGEN-Generate_Towers.md](MAPGEN-Generate_Towers.md) / [MAPGEN-Generate_Lairs.md](MAPGEN-Generate_Lairs.md) — produce the towers/lairs this places fortresses around.
- `MOM_DEF.h` — `NUM_PLAYERS` (6), `NUM_NODES` (30), `NUM_TOWERS` (6), `NUM_LAIRS`, `WORLD_WIDTH`/`WORLD_HEIGHT` (60/40), `god_Normal`; `MOM_DAT.h` — `rt_*` races, `ut_*` unit types, `s_FORTRESS`/`s_CITY`.
