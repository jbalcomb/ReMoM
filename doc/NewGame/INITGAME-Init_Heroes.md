INITGAME-Init_Heroes.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Heroes.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Heroes.c

Init_Runtime()
    |-> Init_Heroes();

---

# `Init_Heroes` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Init_Heroes` | [INITGAME.c:2270-2435](../../MoM/src/INITGAME.c#L2270-L2435) | Builds the per-player premade hero roster: for every player × hero-type, seeds the hero from `_hero_premade_table` and rolls its random ability picks (warrior/mage/all) via a `Random(14)` loop, then writes abilities, casting skill, and starting spells. |

`MGC` overlay 56 (asm `proc Init_Heroes far`).

`Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

## Purpose

Each of the 5 player slots gets its own copy of all `NUM_HERO_TYPES` (35) heroes.
For each hero, the premade template supplies a number of ability "picks"
(`warrior`, `mage`, or `all` = both), an initial ability bitset, and a casting
skill. The pick loop randomly distributes those picks across the hero-ability
table (Leadership, Might, Arcane Power, …), upgrading level-1 abilities to
level-2 on a repeat, then the result is stored into `_HEROES2[player]->heroes[ht]`.

**RNG-consuming:** one `Random(14)` per pick-loop iteration. The total draw count
depends on how many iterations each hero takes to exhaust its picks, so this
function is part of the new-game `Random()` alignment chain.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_Runtime` / INITGAME | [INITGAME.c:597](../../MoM/src/INITGAME.c#L597) | Followed by `gd_dump_players("100_Init_Heroes_P")` ([598](../../MoM/src/INITGAME.c#L598)); `Init_Players` runs next. |

## How it works

Two nested loops: `itr_players = 0..4` ([2282](../../MoM/src/INITGAME.c#L2282)) × `itr_hero_types = 0..NUM_HERO_TYPES-1` ([2285](../../MoM/src/INITGAME.c#L2285)). Per hero:

1. `Level = 0`; zero `warrior_picks` / `mage_picks` / `all_picks` ([2288-2292](../../MoM/src/INITGAME.c#L2288-L2292)).
2. `switch(pick_type)` → route `pick_count` into warrior (0), mage (1), or all (2) ([2294-2308](../../MoM/src/INITGAME.c#L2294-L2308)).
3. Load `abilities` (`int32_t`) and `casting_skill` from the template; if `all_picks > 0` set both `warrior_picks` and `mage_picks` to it ([2310-2318](../../MoM/src/INITGAME.c#L2310-L2318)).
4. **Pick loop** `while(warrior_picks > 0 || mage_picks > 0)` ([2320-2408](../../MoM/src/INITGAME.c#L2320-L2408)): `switch(Random(14) - 1)` selects an ability (0..13). Each case checks its gate (level-2 not already set, the right pick pool > 0, sometimes a hero/unit restriction), sets the level-1 bit or upgrades level-1→level-2 (clearing level-1 via `^=`), and consumes a pick from **both** pools.
5. Store `abilities`, `Casting_Skill = (int8_t)casting_skill`, and `Spells[0..3]` from the template ([2410-2415](../../MoM/src/INITGAME.c#L2410-L2415)).

The 32-bit `abilities` value uses single `HSA_*` constants in production; the asm
splits it into `HSA_LO_*` / `HSA_HI_*` 16-bit halves (representational).

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| Player loop | `cmp itr_players, 5 / jge` | `itr_players < 5` ✓ |
| Hero-type loop | `cmp itr_hero_types, e_NUM_HERO_TYPES` (35) `/ jge` | `itr_hero_types < NUM_HERO_TYPES` (35) ✓ |
| `_HEROES2` indexing | `[itr_players*4]` ptr-array, deref, `+ itr_hero_types*size s_HERO` | `_HEROES2[itr_players]->heroes[itr_hero_types]` ✓ |
| `pick_type` switch | `0→warrior, 1→mage, 2→all` | same ✓ |
| `all_picks` doubling | `cmp all_picks,0 / jle` skip, else `warrior=all; mage=all` | `if(all_picks > 0){…}` ✓ |
| Loop condition | `loc_54CA4`: `warrior>0` OR `mage>0` continue | `while(warrior_picks > 0 \|\| mage_picks > 0)` ✓ |
| Pick selector | `Random(14)` `/ dec / cmp 13 / jbe` jump table | `switch(Random(14) - 1)` ✓ |
| Cases 0–5 (warrior L1→L2) | gate `L2==0 && warrior>0`; `xor L1 / or L2`; dec both | same ✓ |
| Case 6 (Arcane Power) | gate `ARCANE2==0 && mage>0`; `!= ut_Knight`; `Ranged_Type ∈ [Ranged_Fireball, rat_Nat_Bolt]`; `xor/or`; **dec both inside the non-Knight path** (`jt_htt_06`: Knight `jz`-skips dec) | `!= ut_Knight` guard with `warrior_picks--; mage_picks--;` **inside** it ✓ |
| Case 7 (Prayermaster) | gate `mage>0`; `xor/or`; dec both | same ✓ |
| Case 8 (Casting Skill) | gate `mage>0`; `inc casting_skill`; dec both | same ✓ |
| Case 9 (Noble) | `cmp itr_hero_types, _Chosen / jz` skip → set when `!= _Chosen`; dec both | `!= ut_Chosen` (`_Chosen == ut_Chosen == 34`) ✓ |
| Case 10 (Charm) | gate `CHARMED==0 && !(Attribs_1 & Imm_Magic)`; set; dec both | same ✓ |
| Case 11 (Lucky) → 12 fall-through | `jt_htt_11` sets LUCKY then falls into `jt_htt_12`; no `break` | missing-`break` fall-through, commented ✓ |
| Case 12 (Agility) | `xor HSA_HI_AGILITY / or HSA_HI_AGILITY2`; dec both | `^= HSA_AGILITY; \|= HSA_AGILITY2;` ✓ |
| Case 13 (Sage) | `xor HSA_HI_SAGE / or HSA_HI_SAGE2`; dec both | `^= HSA_SAGE; \|= HSA_SAGE2;` ✓ |
| Final stores | `abilities` (dword), `Casting_Skill` (byte), `Spells[0..3]` from template | same ✓ |

## OG bugs preserved (do **not** "fix" in baseline ReMoM)

- **Case 11 (Lucky) missing `break`** — falls through into case 12 (Agility);
  commented at [INITGAME.c:2396](../../MoM/src/INITGAME.c#L2396). Matches the asm
  (`jt_htt_11` falls into `jt_htt_12`).
- **Case 6 Knight exclusion / Ranged range** — the asm excludes `ut_Knight` and
  gates on `Ranged_Type ∈ [Ranged_Fireball, rat_Nat_Bolt]`; the in-code comments
  ([2353](../../MoM/src/INITGAME.c#L2353), [2356](../../MoM/src/INITGAME.c#L2356))
  speculate these are OG off-by-ones (ElvenArcher / Lightning), but production
  matches the disassembly. Faithful; preserve.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — `1..14` per pick-loop iteration.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Heroes.asm` — IDA Pro 5.5 disassembly (authority); `jt_htt` 14-entry jump table.
- `MAGIC.inc` — `_Chosen = 34`, `ut_Knight = 31`, `e_NUM_HERO_TYPES = 35`.
- [INITGAME.c:597-598](../../MoM/src/INITGAME.c#L597-L598) — call site + player dump.
- [INITGAME-Init_Players.md](INITGAME-Init_Players.md) — sibling `Init_Runtime` step that runs next.
