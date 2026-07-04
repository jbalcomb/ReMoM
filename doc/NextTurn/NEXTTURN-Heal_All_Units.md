NEXTTURN-Heal_All_Units.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Heal_All_Units.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Heal_All_Units.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Heal_All_Units()       NEXTTURN
            |-> Army_At_City() / Army_At_Square_1()
            |-> Kill_Unit()
            |-> Heal_Unit()

---

# `Heal_All_Units` — Walkthrough

**Location:** [NEXTTURN.c:3875-3980](../../MoM/src/NEXTTURN.c#L3875-L3980) (~106 lines, ends [line 3980](../../MoM/src/NEXTTURN.c#L3980)).
**WZD overlay:** ovr140 (o140p23).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L616) — normal end-of-turn (Wave 12B).

> Verified against the **`.asm`** (417 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** One deliberate platform deviation (the dropped `SA_MK_FP0` far-pointer wrap). Builds clean (Debug, MSVC).

## Purpose

Per-turn healing. Builds a per-unit healing-*divisor* table `unit_heal_rate_divisors` (`20` = 5% base, `10` = garrisoned in a city, `6` ≈ 15% with an Animist's Guild — smaller divisor = more healing), then heals each eligible unit, and finally applies the Natural-Healer (`UA_HEALER`) flat 20% bonus to every unit sharing a healer's square. Death-race and undead units never heal.

## Signature

```c
void Heal_All_Units(void)
```

Locals: `troops[MAX_STACK]`, `var_16[MAX_STACK]` (asm `var_16` — allocated, unused here), `troops_has_human_player_units`, `troop_count`, `itr`/`itr_troops` (`_SI_`/`_DI_`), `itr_units`, `itr_cities`.

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Heal_All_Units.asm`.

### Setup + base rate ([3885-3890](../../MoM/src/NEXTTURN.c#L3885-L3890), asm:15-39)

Allocate `unit_heal_rate_divisors` (`(_units / 16) + 4` paragraphs, [3885](../../MoM/src/NEXTTURN.c#L3885)), then set every unit's entry to `20` (1/20 = 5% natural healing). **See deviation (SA_MK_FP0).**

### City pass ([3893-3915](../../MoM/src/NEXTTURN.c#L3893-L3915), asm:40-134)

For each city: `Army_At_City`, then

- **Kill mismatched-owner units** ([3896-3903](../../MoM/src/NEXTTURN.c#L3896-L3903), asm:52-89) — `if(_UNITS[troops[i]].owner_idx != _CITIES[c].owner_idx) Kill_Unit(troops[i], kt_Disappeared)`. **OGBUG (faithful):** `Army_At_City` only returns owner-matching units ([WZD_o059.c:113](../../MoM/src/WZD_o059.c#L113)), so the condition is never true and the `Kill_Unit` is unreachable — the drake178-noted botched design. Preserve.
- **Garrison rate** ([3904-3907](../../MoM/src/NEXTTURN.c#L3904-L3907), asm:90-105) — `unit_heal_rate_divisors[troops[i]] = 10`.
- **Animist's Guild rate** ([3908-3914](../../MoM/src/NEXTTURN.c#L3908-L3914), asm:106-128) — `if(bldg_status[ANIMISTS_GUILD] > bs_Replaced) unit_heal_rate_divisors[troops[i]] = 6`.

### Unit pass ([3916-3979](../../MoM/src/NEXTTURN.c#L3916-L3979), asm:136-409)

For each unit:

- **Time-Stop gate** ([3918-3924](../../MoM/src/NEXTTURN.c#L3918-L3924), asm:140-154) — when Time Stop is active (`g_timestop_player_num != 0`), `continue` unless the unit belongs to the caster: `owner_idx == (g_timestop_player_num - 1)` (`g_timestop_player_num` is **1-based**; the asm's `dec dx` supplies the `- 1`).
- **Self-heal** ([3925-3932](../../MoM/src/NEXTTURN.c#L3925-L3932), asm:156-186) — non-death/non-undead: `Heal_Unit(itr_units, unit_heal_rate_divisors[itr_units], ST_FALSE)` (the location-based regen; consumes RNG — see Faithful).
- **Natural-Healer bonus** ([3933-3977](../../MoM/src/NEXTTURN.c#L3933-L3977), asm:187-403) — if the unit has `UA_HEALER`: `Army_At_Square_1` its square, an over-`MAX_STACK` neutral/human filter (dead code — see OGBUG), then a per-troop loop that, for each non-death/non-undead stack member not already flagged `NATURAL_HEALER_ALREADY_APPLIED`, calls `Heal_Unit(troops[i], 0, ST_TRUE)` — the flat 20% pass, which stamps the `66` sentinel ([3965-3977](../../MoM/src/NEXTTURN.c#L3965-L3977), asm:346-403).

## ReMoM deviations (deliberate — not OG-faithful, not bugs)

- **`SA_MK_FP0` wrapper dropped** ([3885](../../MoM/src/NEXTTURN.c#L3885)) — the asm normalizes the `Allocate_First_Block` result through `SA_MK_FP0` (a segmented far-pointer macro, asm:24-28) before storing the pointer. ReMoM uses the result directly. Platform artifact, not behavioral.

## Faithful (do not "fix")

- **`Heal_Unit` consumes RNG on the `ST_FALSE` path** ([NEXTTURN.c:3850](../../MoM/src/NEXTTURN.c#L3850)) — the self-heal branch calls `Random(fraction)` for the fractional remainder, so this function participates in the PRNG stream; the `ST_TRUE` (Natural-Healer 20%) branch does not. Getting the two call sites' arguments exactly right (unit vs stack member, `ST_FALSE` vs `ST_TRUE`) is required for RNG-stream fidelity, not just final HP.
- **`NATURAL_HEALER_ALREADY_APPLIED = 66`** ([MOX_DEF.h:720](../../MoX/src/MOX_DEF.h#L720)) — sentinel written into the divisor slot by the `ST_TRUE` pass ([3863](../../MoM/src/NEXTTURN.c#L3863)) to mark "already natural-healed this turn"; the final loop skips slots equal to it ([3973](../../MoM/src/NEXTTURN.c#L3973)). Chosen larger than any unit's total HP so it also nulls any later divisor-based heal. Asm-verified as the byte `66`.
- **`troop_count > MAX_STACK` block is dead code** ([3937-3964](../../MoM/src/NEXTTURN.c#L3937-L3964), asm:233-345) — `Army_At_Square_1` caps `troop_count` at `MAX_STACK`, so the neutral/human filter never runs, but the asm has it. Preserve.
- **`Heal_Unit`'s "damage over 127" comments** ([NEXTTURN.c:3842](../../MoM/src/NEXTTURN.c#L3842), [3861](../../MoM/src/NEXTTURN.c#L3861)) — signed-byte `Damage` OGBUGs, preserved.

## Related references

- `…\ovr140\Heal_All_Units.asm` — IDA Pro 5.5 disassembly (ground truth, 417 lines).
- `…\ovr140\Heal_All_Units.c` — Gemini translation of the asm.
- `…\ovr140\G_UNIT_OvlHealing.asm` — the callee `Heal_Unit`'s ground truth.
- Callees: `Allocate_First_Block`, `Army_At_City` ([WZD_o059.c:101](../../MoM/src/WZD_o059.c#L101)), `Army_At_Square_1`, `Kill_Unit` ([CITYCALC.c:748](../../MoM/src/CITYCALC.c#L748)), `Heal_Unit` ([NEXTTURN.c:3834](../../MoM/src/NEXTTURN.c#L3834), asm `G_UNIT_OvlHealing`; `flag` selects regen vs Natural-Healer 20%).
- Constants/fields: `unit_heal_rate_divisors`, `_screen_seg`, `MAX_STACK`, `ANIMISTS_GUILD`, `bs_Replaced`, `kt_Disappeared`, `rt_Death`, `UM_UNDEAD`, `UA_HEALER`, `g_timestop_player_num` (1-based), `NEUTRAL_PLAYER_IDX`, `HUMAN_PLAYER_IDX`, `NATURAL_HEALER_ALREADY_APPLIED = 66`.
- Tracked as **Wave 12B** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
