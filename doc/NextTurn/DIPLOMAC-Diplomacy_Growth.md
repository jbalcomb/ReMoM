DIPLOMAC-Diplomacy_Growth.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr085\Diplomacy_Growth.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr085\Diplomacy_Growth.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Diplomacy_Growth()

---

# `Diplomacy_Growth` — Walkthrough

**Location:** [DIPLOMAC.c:2652-2857](../../MoM/src/DIPLOMAC.c#L2652-L2857) (ends [line 2857](../../MoM/src/DIPLOMAC.c#L2857); Doxygen block from 2652, body from [2690](../../MoM/src/DIPLOMAC.c#L2690)).
**WZD overlay:** ovr085, p02 (`MoO2` module `DIPLOMAC :: Diplomacy_Growth_`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c) — normal (non-time-stop) end-of-turn (Wave 10C).

> Verified against the **`.asm`** (542 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.**

## Purpose

The per-turn natural drift of wizard-to-wizard diplomatic relations. Five sequential passes:

1. **Pact / alliance goodwill** — 50% per wizard-pair to gain `+Random(3)` relations if pacted, `+Random(6)` if allied.
2. **Human military-threat penalty** — a `d20` / strength-ratio roll that can hit the human with `-10` relations from an AI whose army is weaker (the OSG p.374 "military build-up" mechanic).
3. **Human overextension** — every other turn, if the human owns too many cities for the map size, all contacted non-allied wizards react negatively.
4. **Gravitate to default** — each AI's *visible* relations drift back toward their `default_relations` value over time.
5. **Symmetrize** — mirror visible relations across the diagonal and zero the self-relations.

All relation changes go through `Change_Relations`.

## Signature

```c
void Diplomacy_Growth(void)
```

## Code walk

Line refs are production [DIPLOMAC.c](../../MoM/src/DIPLOMAC.c); cross-checked against `Diplomacy_Growth.asm`.

1. **Pact/alliance goodwill** ([2709-2725](../../MoM/src/DIPLOMAC.c#L2709-L2725), asm:17-91) — for each pair `(itr1, itr2>itr1)`, if `Random(2) == 1`: pacted → `Change_Relations(Random(3), …)`, allied → `Change_Relations(Random(6), …)` (both `if`s run; mutually exclusive statuses).
2. **Human military threat** ([2737-2759](../../MoM/src/DIPLOMAC.c#L2737-L2759), asm:94-172) — `human_army_strength = _players[HUMAN].astrologer.army_strength`; if `> 0`, for each weaker-but-nonzero AI: `too_strong_treshold = (int32_t)human × 50 / ai_strength`; if `Random(100) >= too_strong_treshold` **and** `Random(20) == 1` → `Change_Relations(-10, HUMAN, itr1, 7, …)`.
3. **Human overextension** ([2760-2798](../../MoM/src/DIPLOMAC.c#L2760-L2798), asm:174-307) — every even turn, for each `itr1`: count human cities; if `(_landsize+1)×3 < city_count` and `Random(4) == 1`, compute `reaction_value` (city surplus / map factor × `Random(4)` / 3), **`× 4`** ([2779](../../MoM/src/DIPLOMAC.c#L2779)), halve for non-human `itr1`, floor at `-15` (`SETMIN`), then apply to all contacted non-allied wizards via `Change_Relations(…, 14, …)`.
4. **Gravitate to default** ([2799-2835](../../MoM/src/DIPLOMAC.c#L2799-L2835), asm:310-492) — outer `itr1` from `1`, inner `itr2` from `0` (each AI wizard toward every player including the human): `gravitation = (Random(105) > abs(Visible_Rel)) ? Random(2) : 0`; then for `itr2 != itr1` with `DA_Strength == 0`, `default_relations = Default_Rel[itr2]`; nudge `Visible_Rel` toward it (up always; down only every 10th turn), clamp to `default_relations`, then to `[-100, 100]`.
5. **Symmetrize** ([2836-2850](../../MoM/src/DIPLOMAC.c#L2836-L2850), asm:495-536) — full `0…_num_players` square: `Visible_Rel[i][i] = 0`, else `Visible_Rel[itr1][itr2] = Visible_Rel[itr2][itr1]` (store target `[itr1][itr2]`, matching asm:527; source `[itr2][itr1]`, asm:519).

## Faithful (do not "fix")

- **Overextension `reaction_value *= 4`** ([2779](../../MoM/src/DIPLOMAC.c#L2779)) — matches the asm's `shl 2` (asm:250-253) and the Gemini `.c`'s `× 4`.
- **`too_strong_treshold` in 32-bit** ([2748](../../MoM/src/DIPLOMAC.c#L2748)) — the `(int32_t)` cast reproduces the asm's `LXMUL@` / `LDIV@` 32-bit multiply/divide (asm:122-138).
- **`SETMIN`/`SETMAX` clamps** ([2784](../../MoM/src/DIPLOMAC.c#L2784), [2821](../../MoM/src/DIPLOMAC.c#L2821), [2828](../../MoM/src/DIPLOMAC.c#L2828), [2831-2832](../../MoM/src/DIPLOMAC.c#L2831-L2832)) — `SETMIN` floors, `SETMAX` caps; each matches the asm's clamp branch.
- **`Random` call sequence** — every `Random(n)` matches the asm in order and modulus (`(2)`,`(3)`,`(6)`; `(100)`,`(20)`; `(4)`,`(4)`; `(105)`,`(2)`). Preserved — important for PRNG parity.
- **`human_player_city_count` recomputed per `itr1`** ([2764-2771](../../MoM/src/DIPLOMAC.c#L2764-L2771), asm:187-205) — redone for every outer player though player-independent. Redundant but faithful.
- **OSG p.374 mechanic comment** ([2726-2736](../../MoM/src/DIPLOMAC.c#L2726-L2736)) — preserved documentation of the military-threat pass.

## ReMoM additions (not OG)

- **`LOG_TRACE` FN-ENTER** ([2701](../../MoM/src/DIPLOMAC.c#L2701)) — logs the RNG call count on entry; not in the asm.
- **`STU_GD_Dump_Players` capture points 628 / 629** ([2979](../../MoM/src/DIPLOMAC.c#L2979) entry, [3128](../../MoM/src/DIPLOMAC.c#L3128) return) — one-shot `_players` snapshots bracketing the pass, added to diagnose the treaty/peace/exchange-modifier divergence. Debug scaffolding, not in the asm; transient.

## Related references

- `…\ovr085\Diplomacy_Growth.asm` — IDA Pro 5.5 disassembly (ground truth, 542 lines).
- `…\ovr085\Diplomacy_Growth.c` — Gemini translation of the asm (matches production).
- Not to be confused with `Diplomacy_Growth_For_Enchantments__WIP` (ovr140, Wave 10A) — a separate function.
- `Change_Relations` — the relation-delta applier ([`DIPLOMAC-Change_Relations.md`](DIPLOMAC-Change_Relations.md)). Fields: `_players[].Dipl.{Dipl_Status, Contacted, Visible_Rel, Default_Rel, DA_Strength}`, `_players[].astrologer.army_strength`.
- Constants: `HUMAN_PLAYER_IDX = 0`, `DIPL_WizardPact`, `DIPL_Alliance`, `_landsize`, `_difficulty`.
- Tracked as **Wave 10C** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
