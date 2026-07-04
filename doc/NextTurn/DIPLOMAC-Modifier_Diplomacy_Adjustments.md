DIPLOMAC-Modifier_Diplomacy_Adjustments.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr088\Modifier_Diplomacy_Adjustments.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr088\Modifier_Diplomacy_Adjustments.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Modifier_Diplomacy_Adjustments()       DIPLOMAC

---

# `Modifier_Diplomacy_Adjustments` — Walkthrough

**Location:** [DIPLOMAC.c:6248-6311](../../MoM/src/DIPLOMAC.c#L6248-L6311) (~64 lines, ends [line 6311](../../MoM/src/DIPLOMAC.c#L6311)).
**WZD overlay:** ovr088 (o88p03; drake178/MoO2: `DIPLOMAC` module `Modifier_Diplomacy_Adjustments_()`; 1oom `game_turn.c game_turn_update_mood_blunder()`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L616) — normal end-of-turn, call site [NEXTTURN.c:835](../../MoM/src/NEXTTURN.c#L835) (Wave 10I).

> Verified against the **`.asm`** (330 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm** on all live diplomacy logic. Two faithful OG quirks (`itr2 = 1` first-nest start; dead `IDK_Dipl_Action`). Nest 2 also touches two deprecated MoO1 fields that are NIU in MoM (see lineage note).

## Purpose

Two independent per-turn passes over the diplomacy tables:
1. **Modifier recovery** — for each ordered non-Crusade player pair, nudge each of the three diplomatic modifiers upward by `Random(5)` while they're below `0`, and again while below `50` (so a very-negative modifier can gain twice in a turn), then clamp `peace_modifier` to `[-200, 200]`.
2. **Delayed-diplomacy reset** — zero every player's per-counterpart delayed-diplomacy-action memory (`DA_Strength`, `Dipl_Action`, tribute fields, etc.).

## Signature

```c
void Modifier_Diplomacy_Adjustments(void)
```

Locals: `itr3`, `IDK_Dipl_Action` (write-only — see Faithful), `itr2` (`_SI_`), `itr1` (`_DI_`).

## Code walk

Line refs are production [DIPLOMAC.c](../../MoM/src/DIPLOMAC.c); cross-checked against `Modifier_Diplomacy_Adjustments.asm`.

### Nest 1 — modifier recovery ([6254-6292](../../MoM/src/DIPLOMAC.c#L6254-L6292), asm:13-235)

`for(itr1 = 0 …)` (asm:13, 232-234) × `for(itr2 = 1 …)` (asm:17, 225-227 — **starts at 1**, see Faithful). Skip when `itr2 == itr1` (asm:21-23). Read `IDK_Dipl_Action = Dipl_Action[itr2]` (asm:31-33, dead). Guard `if(Dipl_Status[itr2] != DIPL_Crusade)` (asm:39-41), then:

- **treaty_modifier** ([6263-6270](../../MoM/src/DIPLOMAC.c#L6263-L6270), asm:51-90) — `< 0 → += Random(5)`; `< 50 → += Random(5)`.
- **exchange_modifier** ([6271-6278](../../MoM/src/DIPLOMAC.c#L6271-L6278), asm:99-138) — same pair of tests.
- **peace_modifier** ([6279-6286](../../MoM/src/DIPLOMAC.c#L6279-L6286), asm:147-186) — same pair of tests.
- **Clamp peace** ([6287-6288](../../MoM/src/DIPLOMAC.c#L6287-L6288), asm:195-222) — `SETMIN(peace_modifier[itr2], -200)` (floor) then `SETMAX(peace_modifier[itr2], 200)` (cap). Only `peace_modifier` is clamped; `treaty`/`exchange` are not.

Up to **6 `Random(5)` calls per qualifying pair** — this pass participates in the PRNG stream.

### Nest 2 — delayed-diplomacy reset ([6294-6310](../../MoM/src/DIPLOMAC.c#L6294-L6310), asm:238-322)

`for(itr3 = 0 …)` (asm:238, 318-321) × `for(itr2 = 0 …)` (asm:242, 311-313 — note `itr2` from `0` here). For every `_players[itr2].Dipl.*[itr3]`:

- `DA_Strength[itr3] = 0` (asm:253); `Dipl_Action[itr3] = do_None` (asm:259).
- Clears the two deprecated MoO1 bounty fields `niu_bounty_collect`/`niu_attack_bounty` (asm:265-280) — NIU in MoM, see lineage note.
- `field_F0[itr3] = 0` (asm:287); `Tribute_Spell[itr3] = 0` (asm:293); `field_9C[itr3] = 0` (asm:301); `Tribute_Gold[itr3] = 0` (asm:309).

## MoO1 / MoO2 lineage

MoM runs on SimTex's *Master of Orion* engine — **MoO1** (1993) → **MoO2** → **Master of Magic** — and inherited chunks of its diplomacy code. Nest 2 resets two such inheritances, **`niu_attack_bounty`** (asm `field_6C`; 1oom `attack_bounty[]`, MoO2 `reward_attack_player`) and its paired **`niu_bounty_collect`** (asm `field_120`) — the MoO1 "pay a rival to attack a third player" mechanic, **Not In Use in MoM**. Their reset is inert, so the mechanic itself is intentionally left undocumented here beyond noting it's dead MoO1 carryover. (The `// Hereafter, DNE in MoO2` comment at the nest head marks the whole reset loop as post-MoO2.)

## Faithful (do not "fix")

- **`itr2 = 1` start in Nest 1** ([6256](../../MoM/src/DIPLOMAC.c#L6256), asm:17 `mov itr2, 1`) — the modifier-recovery pass never touches the `[0]` column, i.e. no player's modifiers *toward player 0 (human)* get the `Random(5)` drift. Matches the asm exactly (contrast `End_Of_Turn_Diplomacy_Adjustments`, whose loops both start at `0`). Preserve.
- **`IDK_Dipl_Action` is write-only** ([6260](../../MoM/src/DIPLOMAC.c#L6260), asm:31-33) — assigned from `Dipl_Action[itr2]` and never read; a dead store present in the asm. Keep it (revisit the name if a consumer ever surfaces).
- **Only `peace_modifier` is clamped** — `treaty`/`exchange` can drift above `50`+`Random(5)` without a cap, exactly as the asm (asm:75/123 test `< 50` then add, no follow-up clamp). Not a bug.

## Related references

- `…\ovr088\Modifier_Diplomacy_Adjustments.asm` — IDA Pro 5.5 disassembly (ground truth, 330 lines).
- `…\ovr088\Modifier_Diplomacy_Adjustments.c` — Gemini translation of the asm.
- Sibling: [`End_Of_Turn_Diplomacy_Adjustments`](../../MoM/src/DIPLOMAC.c#L6181) (o87, Wave 10H) — the `+= 10 toward 100` modifier drift; and [`Adjust_Diplomat_Modifiers`](../../MoM/src/DIPLOMAC.c#L3369) — the `-= 10` counterpart.
- Callees: `Random` (1-based `1..n`), `SETMIN`/`SETMAX`.
- Constants/fields: `_players[].Dipl.{treaty,exchange,peace}_modifier`, `Dipl_Action`, `Dipl_Status`, `DA_Strength`, `Tribute_Spell`, `Tribute_Gold`, `field_F0`/`field_9C`, `DIPL_Crusade`, `do_None`. (Deprecated MoO1 fields `niu_attack_bounty`/`niu_bounty_collect` are reset in Nest 2 — see lineage note.)
- Tracked as **Wave 10I** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
