EVENTS-Get_Event_Victim.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr081\Get_Event_Victim.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr081\Get_Event_Victim.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Determine_Event()       EVENTS
            |-> Get_Event_Victim()  EVENTS

---

# `Get_Event_Victim` — Walkthrough

**Location:** [EVENTS.c:1671-1737](../../MoM/src/EVENTS.c#L1671-L1737) (~67 lines, ends [line 1737](../../MoM/src/EVENTS.c#L1737); Doxygen block precedes).
**WZD overlay:** ovr081, s081p03 (drake178 IDA label: `EVNT_SelectTarget`; `MoO2` module `EVENTS :: Get_Event_Victim_`).
**Called from:** [`Determine_Event`](../../MoM/src/EVENTS.c#L311) (Wave 8A) — the only caller.

> Verified against the **`.asm`** (187 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.**

## Purpose

Weighted-random selection of the wizard an event lands on. Each wizard's "power" is `magic_power + spell_research + army_strength`. For a **bad** event the pick is weighted *toward* stronger wizards; for a **good** event the weights are inverted (`600 − power`) so *weaker* wizards are favored. Weights are halved as many times as needed to bring the total to ≤ 500, then a `Random(total_weight)`-driven roulette walk chooses the target. Returns the wizard index, or `ST_UNDEFINED (-1)` if no one qualifies.

## Signature

```c
int16_t Get_Event_Victim(int16_t event_type)
```

**`event_type` is a misnomer — it's a good/bad polarity flag, not an event id.** The caller passes `m_event_good_array[event_type]` (a `0`/`1` value); only `== ST_TRUE (1)` (a *good* event) triggers the weight inversion. Locals (renamed this session): `wizard_power[NUM_PLAYERS]` (per-wizard weights), `total_weight` (`_DI_`, running weight sum; was `IDK`), `weight_total_in_range` (scaling-loop exit flag; was `In_RNG_Range`), `roll_remaining` (roulette position; was `Weights_Remainder`), `victim_idx`, `itr` (`_SI_`).

## Code walk

Line refs are production [EVENTS.c](../../MoM/src/EVENTS.c); cross-checked against `Get_Event_Victim.asm`.

1. **Zero the array** ([1679-1682](../../MoM/src/EVENTS.c#L1679-L1682), asm:16-28) — `wizard_power[0..NUM_PLAYERS) = 0` (full array, not just active players).
2. **Build weights** ([1683-1686](../../MoM/src/EVENTS.c#L1683-L1686), asm:29-60) — for `itr < _num_players`: `wizard_power[itr] = magic_power + spell_research + army_strength`.
3. **Good-event inversion** ([1687-1697](../../MoM/src/EVENTS.c#L1687-L1697), asm:61-93) — if `event_type == ST_TRUE`: `wizard_power[itr] = 600 - wizard_power[itr]`, floored at 0 (`< 1 → 0`). Weaker wizards now carry more weight.
4. **Scale into range** ([1698-1717](../../MoM/src/EVENTS.c#L1698-L1717), asm:94-144) — `while(weight_total_in_range == ST_FALSE)`: sum all weights into `total_weight`; if `> 500`, halve every weight (signed `/ 2`, the asm's `cwd; sub ax,dx; sar ax,1`) and repeat; else set in-range. Keeps the eventual `Random(total_weight)` argument ≤ ~500.
5. **Empty bail** ([1718-1721](../../MoM/src/EVENTS.c#L1718-L1721), asm:145-149) — `total_weight < 1` → `return ST_UNDEFINED`.
6. **Roulette walk** ([1722-1736](../../MoM/src/EVENTS.c#L1722-L1736), asm:151-179) — `roll_remaining = Random(total_weight) - 1`; from `victim_idx = HUMAN_PLAYER_IDX (0)`, subtract each wizard's weight and advance while the remainder stays `>= 0`, stopping at the last player; `return victim_idx`.

## Notes (faithful — do not "fix")

- **`Random` is 1-based, so `roll_remaining = Random(total_weight) - 1`** ([1722](../../MoM/src/EVENTS.c#L1722), asm:151-156 `Random; dec ax`) — `Random(n)` returns `1..n` ([random.c:294](../../MoX/src/random.c#L294)); the `- 1` converts it to a 0-based position `0..total_weight-1` so the weight-subtraction walk lands in exactly one bucket. Faithful.
- **Selection-loop rendering** ([1724-1735](../../MoM/src/EVENTS.c#L1724-L1735)) — the asm's single loop guard at `loc_6CACA` is the compound `roll_remaining >= 0 && (_num_players - 1) > victim_idx`. Production renders it as `while(roll_remaining >= 0)` plus an early `break` on `(_num_players - 1) <= victim_idx` (the negation of the second half). Equivalent rendering of the asm's two-part guard — neither the `break` nor a compound `while` is literally "in the asm."
- **`600` good-event ceiling and `500` roll cap** ([1691](../../MoM/src/EVENTS.c#L1691), [1706](../../MoM/src/EVENTS.c#L1706)) — magic constants straight from the asm (`mov ax, 600`; `cmp total_weight, 500`). Faithful.
- **Signed `/ 2` halving** ([1710](../../MoM/src/EVENTS.c#L1710), asm:120-133) — C `/ 2` truncating toward zero matches the `cwd/sub/sar` idiom.

## Related references

- `…\ovr081\Get_Event_Victim.asm` — IDA Pro 5.5 disassembly (ground truth, 187 lines).
- `…\ovr081\Get_Event_Victim.c` — Gemini translation of the asm (matches production).
- Caller: [`Determine_Event`](../../MoM/src/EVENTS.c#L221) ([`EVENTS-Determine_Event.md`](EVENTS-Determine_Event.md)) — passes `m_event_good_array[event_type]` as the good/bad flag.
- Callee: [`Random`](../../MoX/src/random.c#L296) (1-based `1..n`).
- Constants/fields: `NUM_PLAYERS`, `_num_players`, `_players[].astrologer.{magic_power, spell_research, army_strength}`, `m_event_good_array[]`, `HUMAN_PLAYER_IDX = 0`, `ST_TRUE = 1`, `ST_FALSE = 0`, `ST_UNDEFINED = -1`.
- Tracked as a downstream helper of **Wave 8A** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
