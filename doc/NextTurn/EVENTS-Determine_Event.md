EVENTS-Determine_Event.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr080\Determine_Event.asm        (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr080\Determine_Event__FABLE.c   (Fable 5 translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Determine_Event()        EVENTS

---

# `Determine_Event` — Walkthrough

**Location:** [EVENTS.c:221-610](../../MoM/src/EVENTS.c#L221-L610) (~390 lines, ends [line 610](../../MoM/src/EVENTS.c#L610)).
**WZD overlay:** ovr080, s080p01.
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L708) — normal (non-time-stop) end-of-turn (Wave 8A), gated on `magic_set.random_events == ST_TRUE AND DBG_Alt_A_State == ST_FALSE`.

> Verified against the **`.asm`** (910 lines). The `.c` is a **Fable 5** translation of that asm (not Gemini); the asm bytes decide. **Production is 1:1 with the asm — no divergences.**

## Purpose

Once per turn, decides whether a random event fires and which one. Builds an `event_pressure_accumulator` (difficulty-weighted turns since the last event), tests it against `Random(512)`, then makes up to **5 tries** to draw a valid `event_type` via `Random(18)` — each draw run through a long veto gauntlet (already-active/cooldown status, conjunction exclusivity, affordability, city/target availability, turn gates, rebellion garrison rules, mutually-exclusive events). The first survivor is committed into `events_table` (status `1`) with its per-event payload; `Event_Twiddle` progresses it on later turns.

## Signature

```c
void Determine_Event(void)
```

Event-status convention: `1` = newly triggered, `2` = active/ongoing, `3` = on cooldown, `0` = cleared. Event ids `et_Meteor = 1 … et_Mana_Short = 18`.

Locals of note: `event_pressure_accumulator` (**not** a percentage — see step 2), `event_type`, `player_idx`, `tries`, `conjunction_active`, `normal_units`/`summoned_units` (rebellion garrison tally), `niu_escalation_scalar` (dead — see below).

## Code walk

Line refs are production [EVENTS.c](../../MoM/src/EVENTS.c); cross-checked against `Determine_Event.asm`.

1. **Events disabled** ([240-269](../../MoM/src/EVENTS.c#L240-L269), asm:28-95) — `g_random_events_enabled == ST_FALSE` → set every event `*_Status = 3` (cooldown) and `*_Duration = 0`, `return`. Field order matches the asm exactly. (`g_random_events_enabled` is the OG master switch — see the vestigial-global note below.)
2. **`event_pressure_accumulator`** ([270-284](../../MoM/src/EVENTS.c#L270-L284), asm:97-118) — `last_event_turn > _turn` → `0`; else `_turn - last_event_turn`, floored at `0` (`SETMIN(…, 0)`, redundant here but faithful). Then the **first-5-turns suppression** ([279-284](../../MoM/src/EVENTS.c#L279-L284)) zeroes it while `_event_delay_count < 5`.
   - **This is not a "chance"/percent — it's accumulated event pressure.** It counts (difficulty-weighted) turns since the last event; the fire test in step 4 is `Random(512) <= pressure`, so the per-turn fire probability is `pressure / 512`, climbing every eventless turn until something triggers.
3. **Difficulty weight** ([285-293](../../MoM/src/EVENTS.c#L285-L293), asm:120-155, jump table) — Intro `×1/2`, Easy `×2/3`, Normal `×3/4`, Hard `×4/5`, Impossible unscaled. Each matches the asm's `idiv`/`sar` sequence. Because the factors *shrink* the pressure, higher difficulty ⇒ larger effective pressure ⇒ **more frequent events** (the factor `< 1` cases stretch the interval out).
4. **Fire test** ([297-301](../../MoM/src/EVENTS.c#L297-L301), asm:157-166) — `Random(512) > event_pressure_accumulator` → `return`. `512 = 2⁹`: a power-of-two modulus (cheap masked RNG, no modulo bias) that also fixes the probability denominator (`P = pressure / 512`) and the ~512-turn full-pressure timescale.
5. **Draw + validate (≤5 tries)** ([303-493](../../MoM/src/EVENTS.c#L303-L493), asm:168-660) — `for(tries < 5 && event_type == ST_UNDEFINED)`:
   - `event_type = Random(18)` ([306](../../MoM/src/EVENTS.c#L306), asm:175-180); `player_idx = Get_Event_Victim(m_event_good_array[event_type])` ([311](../../MoM/src/EVENTS.c#L311), asm:181-186).
   - Veto (`event_type = ST_UNDEFINED`) if: victim `ST_UNDEFINED` ([316](../../MoM/src/EVENTS.c#L316)); victim has no active fortress ([321](../../MoM/src/EVENTS.c#L321)); the drawn event's own status is `3` or `2` ([328-345](../../MoM/src/EVENTS.c#L328-L345), asm:200-396); any moon/conjunction/mana-short active and the draw is another such ([347-372](../../MoM/src/EVENTS.c#L347-L372)); Pirates with `gold_reserve < 100` ([374-381](../../MoM/src/EVENTS.c#L374-L381)); a city-based event with no eligible city (`Pick_Random_City`, [383-410](../../MoM/src/EVENTS.c#L383-L410)); Marriage/Meteor before `_turn 150` ([412-428](../../MoM/src/EVENTS.c#L412-L428)); Rebellion in the fortress city or a garrison that is more-summoned-than-normal or contains a hero ([430-461](../../MoM/src/EVENTS.c#L430-L461)); Depletion/New-Mine with no target ([463-477](../../MoM/src/EVENTS.c#L463-L477)); Disjunction with no overland enchantments ([479-485](../../MoM/src/EVENTS.c#L479-L485)); opposing moon active ([487-488](../../MoM/src/EVENTS.c#L487-L488)); Plague/Population-Boom colliding on the same city ([490-491](../../MoM/src/EVENTS.c#L490-L491)).
6. **No valid event** ([495-498](../../MoM/src/EVENTS.c#L495-L498), asm:662-665) — still `ST_UNDEFINED` → `return`.
7. **Commit** ([500-609](../../MoM/src/EVENTS.c#L500-L609), asm:668-902) — `last_event_turn = _turn`, `_event_delay_count = 0`; the `niu_escalation_scalar` difficulty switch ([502-509](../../MoM/src/EVENTS.c#L502-L509), computed then never read); then the apply `switch(event_type)` ([511-609](../../MoM/src/EVENTS.c#L511-L609)) sets the chosen event's `*_Status = 1` and payload — `Meteor`/`Earthquake`/`Plague`/`Rebellion`/`Population_Boom` store `city_idx` + player; `Gift` → `Make_Item(2, spellranks, 0)`; `Pirates` → `(gold_reserve × 100 / (29 + Random(21)))` rounded down to a multiple of 10 ([546-548](../../MoM/src/EVENTS.c#L546-L548)); `Donation` → `100 + Random(100)×5` ([566](../../MoM/src/EVENTS.c#L566)); `Marriage` stores both cities.

## OG quirks preserved (faithful — do not "fix")

- **OGBUG — no events in the first 5 turns** ([279-284](../../MoM/src/EVENTS.c#L279-L284), asm:114-118) — `_event_delay_count` starts at 0 and is bumped (zeroing the pressure) each turn until it reaches 5, so nothing can fire in the opening 5 turns. The preserved comment also notes it counts calls rather than `_turn`, so it misbehaves across a mid-game *Load Game*. Matches asm.
- **`niu_escalation_scalar` dead code** ([502-509](../../MoM/src/EVENTS.c#L502-L509), asm:672-704) — a `_turn / {20,18,15,13,10}` value computed per difficulty and never read (NIU = Not In Use). The asm computes it too (its own jump table); faithful dead code. Its structure is a turn ÷ difficulty-divisor escalation scalar, presumably an unwired event-magnitude idea — the name asserts nothing about the intent.
- **Donation/Rebellion checked out of enum order** ([335-336](../../MoM/src/EVENTS.c#L335-L336), asm:277-297) — the status gauntlet tests Donation (9) before Rebellion (8); the `/* OoO */` comments flag it, and the asm has the same inversion. Faithful.
- **Pirates / Donation payload formulas** ([546-548](../../MoM/src/EVENTS.c#L546-L548), [566](../../MoM/src/EVENTS.c#L566), asm:774-848) — the `LXMUL@`/`LDIV@` gold formula and the `% 10` round-down match the asm; `Random(21)` / `Random(100)` in order.

## Reconstruction note — vestigial OG global

- **`g_random_events_enabled`** ([EVENTS.c:55](../../MoM/src/EVENTS.c#L55), read only at [240](../../MoM/src/EVENTS.c#L240)) is the OG's master "Random Events on/off" flag (OG `dseg:0154`, drake178 `EVNT_Enabled`). In ReMoM it is initialized `ST_TRUE` and **never assigned `ST_FALSE`**, so the disable branch (step 1) is currently dead — the real events-off gate was reconstructed one level up, where `Next_Turn_Calc` only *calls* `Determine_Event` when [`magic_set.random_events == ST_TRUE`](../../MoM/src/NEXTTURN.c#L708). The two are the same concept with two storages. Reading the global is faithful to the asm; wiring `g_random_events_enabled` to `magic_set.random_events` (or leaving it OG-faithful-but-dead) is a separate decision, not a fidelity fix.

## ReMoM additions (not OG)

- **`DBG_trigger_event` hooks** ([295](../../MoM/src/EVENTS.c#L295) force `event_pressure_accumulator = 512`; [314](../../MoM/src/EVENTS.c#L314) force `player_idx = HUMAN`) — debug triggers, not in the asm.
- **`if(event_type > ST_UNDEFINED)` guard before `Get_Event_Victim`** ([309-312](../../MoM/src/EVENTS.c#L309-L312)) — a defensive guard added to silence MSVC **C6385** (invalid-read on `m_event_good_array`). Behaviorally a no-op: `event_type` was just set by `Random(18)` (always `0..17`), so the guard is always true and the asm's unconditional call is preserved in effect.

## Related references

- `…\ovr080\Determine_Event.asm` — IDA Pro 5.5 disassembly (ground truth, 910 lines).
- `…\ovr080\Determine_Event__FABLE.c` — Fable 5 translation of the asm.
- Callees: `Get_Event_Victim`, `Pick_Random_City`, `Player_Fortress_City`, `Army_At_City`, `EVNT_TargetDepletion__STUB`, `EVNT_FindNewMineral__STUB`, `Any_Overland_Enchantments`, `Make_Item`, `Random`.
- Sibling: [`Event_Twiddle`](../../MoM/src/EVENTS.c#L665) (s080p02, Wave 8B) — progresses the events this function arms.
- Constants/fields: `g_random_events_enabled`, `magic_set.random_events`, `events_table->*` (per-event `*_Status` / `*_Duration` / `*_Data` / `*_Player`), `_event_delay_count`, `m_event_good_array[]`, `et_Meteor=1 … et_Mana_Short=18`, `rt_Arcane`, `NEUTRAL_PLAYER_IDX = 5`, `HUMAN_PLAYER_IDX = 0`, `god_Intro/Easy/Normal/Hard/Impossible`, `ST_UNDEFINED = -1`, `MAX_STACK`.
- Tracked as **Wave 8A** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
