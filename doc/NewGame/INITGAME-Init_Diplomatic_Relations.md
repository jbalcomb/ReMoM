INITGAME-Init_Diplomatic_Relations.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Diplomatic_Relations.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Diplomatic_Relations.c

Init_Runtime()
    |-> Init_Diplomatic_Relations();

---

# `Init_Diplomatic_Relations` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Init_Diplomatic_Relations` | [INITGAME.c:692-796](../../MoM/src/INITGAME.c#L692-L796) | Computes each wizard-pair's starting diplomatic relation from their spellbook realms (book affinities/antipathies) and seeds the symmetric `Visible_Rel` / `Default_Rel` / `peace_duration` tables. |

`MGC` overlay 56 (asm `proc Init_Diplomatic_Relations far`).

## Purpose

For every unordered pair of players, derive an initial relation value (`IDK` in
production) from the two wizards' spellbook ranks — Life/Death create antipathy,
shared realms create affinity — clamp it to a floor of `−90`, and write it
symmetrically into both wizards' diplomacy records. Also resets `grand_vizier`
and zeroes each pair's peace timer.

**RNG-neutral: zero `Random()` calls** ([692-796](../../MoM/src/INITGAME.c#L692-L796)). Does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_Runtime` / INITGAME | [INITGAME.c:664](../../MoM/src/INITGAME.c#L664) | Between `gd_dump_players("207_capital_race_P")` ([662](../../MoM/src/INITGAME.c#L662)) and `gd_dump_players("208_Init_Diplomatic_Relations_P")` ([665](../../MoM/src/INITGAME.c#L665)). |

## How it works

`grand_vizier = ST_FALSE` ([705](../../MoM/src/INITGAME.c#L705)), then nested loops
over every unordered pair `itr_players1 < itr_players2 < _num_players`
([707-710](../../MoM/src/INITGAME.c#L707-L710)). The `spellranks[]` array is ordered
`{Nature(0), Sorcery(1), Chaos(2), Life(3), Death(4)}`. Per pair, `IDK` is built up:

1. **Aggregates** ([713-728](../../MoM/src/INITGAME.c#L713-L728)) — `Most_Death_Books` = **max** of the pair's Death ranks; `Most_Chaos_Books` = **max** of the pair's Chaos ranks; `Life_Book_Total` = **sum** of the pair's Life ranks.
2. **Life/Death** ([730-743](../../MoM/src/INITGAME.c#L730-L743)): if `Life_Book_Total > 0 && Most_Death_Books > 0` → `IDK -= (Life_Book_Total + Most_Death_Books) * 5` (opposed realms both present → strong antipathy); else → `IDK += Life_Book_Total*2; IDK -= Most_Death_Books*3`.
3. **Chaos penalty** ([745](../../MoM/src/INITGAME.c#L745)): `IDK -= Most_Chaos_Books * 2`.
4. **Shared-realm affinity** ([747-777](../../MoM/src/INITGAME.c#L747-L777)): for each of four realms — Sorcery, Chaos, Nature, Life — `IDK += min(p1.rank, p2.rank) * 2`. (Mutual investment in the same realm → mutual liking.)
5. **Clamp** ([779](../../MoM/src/INITGAME.c#L779)): `SETMIN(IDK, -90)` → floor at `−90` (`if(-90 > IDK) IDK = -90`).
6. **Store** ([781-788](../../MoM/src/INITGAME.c#L781-L788)): `IDK` → `Dipl.Visible_Rel` and `Dipl.Default_Rel`, both directions; `peace_duration` both directions `= 0`.

Net: max-Death + max-Chaos drive dislike, summed-Life drives like, and min-shared
books across Sorcery/Chaos/Nature/Life drive mutual affinity.

## Variables

| Name | Role | Notes |
|---|---|---|
| `IDK` | the pair's **initial diplomatic relation** score, stored to `Visible_Rel`/`Default_Rel` | placeholder name ("I don't know"); really a `base_relation` accumulator. Higher = friendlier; floored at `−90`. |
| `Most_Death_Books` | **max** of the two wizards' Death ranks | `Most_` = maximum |
| `Most_Chaos_Books` | **max** of the two wizards' Chaos ranks | `Most_` = maximum |
| `Life_Book_Total` | **sum** of the two wizards' Life ranks | `_Total` = sum |
| `Wiz_1_Books` | wizard-1's rank in the realm of the current affinity block | realm-local scratch, reloaded per block |
| `Wiz_2_Books` | wizard-2's rank when loaded, then **overwritten with `min(Wiz_1, Wiz_2)`** | overloaded: holds the *shared* book count after the `if` (a clearer name would be `shared_books`) |

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| `grand_vizier` reset | `mov [grand_vizier_flag], e_ST_FALSE` (entry) | `grand_vizier = ST_FALSE;` ✓ |
| Outer/inner loops | `di = 0..n`, `si = di+1..n` (`jge` tails) | `itr_players1 < _num_players`, `itr_players2 = p1+1 < _num_players` ✓ |
| `Most_Death_Books` | `max(p1, p2)` of `spellranks+8` | `max(...spellranks[sbr_Death])` ✓ |
| `Life_Book_Total` | `p1[+6] + p2[+6]` (sum) | `p1[sbr_Life] + p2[sbr_Life]` ✓ |
| `Most_Chaos_Books` | `max(p1, p2)` of `spellranks+4` | `max(...spellranks[sbr_Chaos])` ✓ |
| Life/Death branch | both>0 → `−(L+D)*5` (shared `imul 5`/`sub`); else `+L*2 −D*3` | same ✓ |
| Chaos penalty | `Most_Chaos_Books * 2` then `sub` | `IDK -= Most_Chaos_Books * 2` ✓ |
| Shared-realm affinity | four realms — Sorcery `+2`, Chaos `+4`, Nature `+0`, Life `+6` — each `min` then `shl 1` (`* 2`) then `add` | four blocks `IDK += (min * 2)`, same realm order ✓ |
| Clamp | `cmp cx, 0FFA6h / jge` else `cx = -90` | `SETMIN(IDK, -90)` (floor at −90) ✓ |
| Stores | `Visible_Rel` + `Default_Rel` + `peace_duration`, both directions (`cl`) | four symmetric `(int8_t)IDK` / `0` writes ✓ |

## Sub-functions / external calls

- None. Reads `_players[].spellranks[]`; writes `grand_vizier` ([MOM_DAT.h:4121](../../MoX/src/MOM_DAT.h#L4121)) and `_players[].Dipl.Visible_Rel` / `.Default_Rel` / `.peace_duration`.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Diplomatic_Relations.asm` — IDA Pro 5.5 disassembly (authority); `grand_vizier_flag` reset (line 17), four `shl ax,1`/`add cx,ax` affinity blocks.
- [INITGAME.c:664-665](../../MoM/src/INITGAME.c#L664-L665) — call site + player dumps.
- [MOM_DAT.h:732-736](../../MoX/src/MOM_DAT.h#L732-L736) — `sbr_` realm enum (Nature 0 … Death 4), matching the `spellranks+0/+2/+4/+6/+8` offsets.
