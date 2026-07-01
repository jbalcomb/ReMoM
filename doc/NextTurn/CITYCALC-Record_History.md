CITYCALC-Record_History.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\Record_History.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\Record_History.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Astr_Power.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Astr_Power.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Astr_ArmyStr.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Astr_ArmyStr.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Astr_Resrch.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Astr_Resrch.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Nation_Size.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr142\WIZ_Get_Nation_Size.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Record_History()

---

# `Record_History` — Walkthrough

**Location:** [CITYCALC.c:3346-3478](../../MoM/src/CITYCALC.c#L3346-L3478) (~133 lines, ends [line 3478](../../MoM/src/CITYCALC.c#L3478)).
**WZD overlay:** ovr142, p22 (IDA address `ovr142:22A2`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L829) — once per turn, end-of-turn.

## Purpose

Computes each wizard's "power" for the **History of Wizards' Power** graph (the astrologer screen) and appends this turn's value to every wizard's 288-entry `history[]` ring.

Per wizard it gathers four astrologer sub-scores — magic power, army strength, spell research, and empire size — sums them into a per-wizard total (`power_of_a_wizard`), scales each category and the total into fixed display ranges (categories ≤ 200, total ≤ 160), then records the total into `history[_turn]` (or scrolls the ring and appends when the buffer is full at turn ≥ 288).

The four input sub-scores come from [`Astrologer_Power`](../../MoM/src/CITYCALC.c#L3482), [`Astrologer_Strength`](../../MoM/src/CITYCALC.c#L3491), [`Astrologer_Research`](../../MoM/src/CITYCALC.c#L3507), and [`Empire_Population_Points`](../../MoM/src/CITYCALC.c#L3528) — all reconstructed and verified 1:1 against their `ovr142` disassembly (they were previously `__STUB`s returning `Random(100)`; that placeholder era is over).

## Signature

```c
void Record_History(void)
```

## Globals read / written

| Symbol | Access | Effect |
|---|---|---|
| `_players[].astrologer` (`magic_power`, `army_strength`, `spell_research`) | written | filled from the astrologer getters, then scaled to ≤ 200. |
| `_players[].empire_mini_pops` | written | filled from `Empire_Population_Points`. |
| `_players[].history[288]` | written | this turn's scaled total appended (with scroll when full). |
| `_num_players` | read | loop bound. |
| `_turn` | read | index into `history[]` / full-buffer test (`< 288`). |

External calls: `Astrologer_Power`, `Astrologer_Strength`, `Astrologer_Research`, `Empire_Population_Points`.

## Locals

```c
int16_t power_of_a_wizard[NUM_PLAYERS];  /* per-wizard summed total */
int16_t itr_history;                     /* scroll inner index */
int16_t max_curr_population/research/military/magic;  /* per-category running maxima */
int16_t max_curr_wiz_pwr;                /* running max of the totals */
int16_t itr, itr_players;                /* loop indices */
```

## Code walk

Line refs are production [CITYCALC.c](../../MoM/src/CITYCALC.c); cross-checked against `Record_History.asm` (451 lines).

### Gather + sum ([3359-3372](../../MoM/src/CITYCALC.c#L3359-L3372), asm:17-104)

Per wizard: fill `astrologer.magic_power / army_strength / spell_research` and `empire_mini_pops` from the getters, then `power_of_a_wizard[p] = magic_power + army_strength + spell_research + empire_mini_pops`.

### Find category / total maxima ([3381-3409](../../MoM/src/CITYCALC.c#L3381-L3409), asm:106-188)

Running `max_curr_*` per category and `max_curr_wiz_pwr` for the total (all strict `>`; `jle` skips in the asm).

### Scale categories to ≤ 200 ([3412-3436](../../MoM/src/CITYCALC.c#L3412-L3436), asm:190-303)

For each of magic / military / research, `if (max > 200)` rescale every wizard's value **down** so the peak lands at 200: `value = ((int32_t)value * 200) / max`. The `int32_t` cast promotes the multiply to 32-bit, matching OG's `LXMUL@`/`LDIV@` long arithmetic (asm:210 `mov ax, 200`; divisor is the per-category max).

### Divide totals by 7, scale to ≤ 160 ([3439-3453](../../MoM/src/CITYCALC.c#L3439-L3453), asm:305-375)

`power_of_a_wizard[p] /= 7`, `max_curr_wiz_pwr /= 7`, then `if (max > 160)` rescale `((int32_t)value * 160) / max_curr_wiz_pwr` (asm:346-371).

### Record into the history ring ([3456-3475](../../MoM/src/CITYCALC.c#L3456-L3475), asm:376-444)

`_turn < 288` → `history[_turn] = total` (asm:382-399). Else scroll: for `itr_history` 0→286, `history[itr_history] = history[itr_history + 1]` (asm:416/424 — slide each entry down from its successor, discarding the oldest at `[0]`), then append this turn's value at `history[287]` (asm:429-440).

## Faithful (do not "fix")

- **Category scaling scales down via `value * cap / max` in 32-bit** ([3416](../../MoM/src/CITYCALC.c#L3416)/[3425](../../MoM/src/CITYCALC.c#L3425)/[3434](../../MoM/src/CITYCALC.c#L3434)/[3451](../../MoM/src/CITYCALC.c#L3451)) — matches OG's operand order and long arithmetic (`LXMUL@`/`LDIV@`).
- **History scroll copies `[i] = [i+1]`** ([3471](../../MoM/src/CITYCALC.c#L3471)) — slides down, discards oldest, frees `[287]`; matches asm:416/424.
- **`_turn < 288` full-buffer split** ([3456](../../MoM/src/CITYCALC.c#L3456), asm:377) — `history[]` is 288 bytes; before turn 288 write directly at `[_turn]`, after scroll-and-append at `[287]`. Faithful.
- **Strict `>` maxima and `/7` integer divide** ([3381-3444](../../MoM/src/CITYCALC.c#L3381-L3444)) — match asm (`jle` skips; `LDIV@` by 7). Faithful.

## Astrologer input getters (reconstructed, verified against asm)

| Function | Location | Formula | asm |
|---|---|---|---|
| `Astrologer_Power` | [3482](../../MoM/src/CITYCALC.c#L3482) | `max(Power_Base - 10, 0)` (`SETMIN` floors at 0) | `WIZ_Get_Astr_Power.asm:11-16` |
| `Astrologer_Strength` | [3491](../../MoM/src/CITYCALC.c#L3491) | Σ owned units `(gold_upkeep + 2·mana_upkeep)` (32-bit), `/5` | `WIZ_Get_Astr_ArmyStr.asm:32/36/45` |
| `Astrologer_Research` | [3507](../../MoM/src/CITYCALC.c#L3507) | Σ over 6×40 of `(spell_idx+1)` where `spells_list[...]==sls_Known (2)`, `/8` | `WIZ_Get_Astr_Resrch.asm:28/43-45` |
| `Empire_Population_Points` | [3528](../../MoM/src/CITYCALC.c#L3528) | Σ population of owned cities, `/10` | `WIZ_Get_Nation_Size.asm:31/38-40` |

> Note: `Astrologer_Power` uses `SETMIN(power, 0)`, which floors at 0 via the active `#define SETMIN(a,b) if((b)>(a)) (a)=(b)` (MOX_DEF.h:95). A commented-out alternate `SETMIN` with different semantics sits at MOX_DEF.h:96 — if that were ever swapped in, this clamp would silently invert.

## Related references

- `…\ovr142\Record_History.asm` — IDA Pro 5.5 disassembly (the authority, 451 lines).
- `…\ovr142\Record_History.c` — drake178 decompilation (matches production).
- OG astrologer getters (now reconstructed as the four functions above): `WIZ_Get_Astr_Power`, `WIZ_Get_Astr_ArmyStr`, `WIZ_Get_Astr_Resrch`, `WIZ_Get_Nation_Size` (ovr142).
- `_players[].history[288]` — per-wizard power-graph ring; `_players[].astrologer` — astrologer sub-scores; `_players[].empire_mini_pops` — empire-size score.
- Tracked as **Wave 12C** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
