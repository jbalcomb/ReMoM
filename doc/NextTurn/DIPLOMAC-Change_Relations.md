DIPLOMAC-Change_Relations.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr085\Change_Relations__WIP.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr085\Change_Relations__WIP.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Diplomacy_Growth()
            |-> Change_Relations()

---

# `Change_Relations` — Walkthrough

**Location:** [DIPLOMAC.c:3344-3615](../../MoM/src/DIPLOMAC.c#L3344-L3615) (~272 lines, ends [line 3615](../../MoM/src/DIPLOMAC.c#L3615)).
**WZD overlay:** ovr085, p12 (drake178 IDA label: `G_DIPL_Action`).
**On-disk OG name:** the disassembly/decompilation files retain `Change_Relations__WIP` — IDA naming decoupled from the renamed production symbol (the `__WIP` suffix was dropped this session).
**Called from:** [`Diplomacy_Growth`](../../MoM/src/DIPLOMAC.c#L2652) (Wave 10C), `End_Of_Combat`, overland spell casting (`Change_Relations_For_Enchantments`, etc.), and other diplomacy sites.

> Verified against the **`.asm`** (656 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** (R1 — a wrong symmetrize field — was fixed this session.)

## Purpose

The central applier for a change of `relation_value` diplomatic points from `attacker_idx` toward `defender_idx` (`type` = diplomatic-action id; `city_idx`/`spell_idx` = provenance). It reciprocation-scales the raw value by the current relationship, applies the defender's Personality and the attacker's Charismatic modifiers, special-cases war/overextension and late-game AI-vs-AI, then writes the result into `Visible_Rel` (clamped `[-100, 100]`) plus side modifiers (`treaty_/exchange_/peace_modifier`), the human-facing `DA_*` record, and `Hostility` / re-evaluation timers.

The `value` parameter is snapshotted into the working accumulator `relation_value` ([3348-3349](../../MoM/src/DIPLOMAC.c#L3348-L3349), `// DNE in Dasm`); `relation_value_pre_charisma` freezes it just before the Charisma step; `diminishing_returns_divisor` scales repeated same-direction actions down.

## Signature

```c
void Change_Relations(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx)
```

## Code walk (nested-guard structure)

Line refs are production [DIPLOMAC.c](../../MoM/src/DIPLOMAC.c); cross-checked against `Change_Relations__WIP.asm`.

1. **Contacted guard** ([3350](../../MoM/src/DIPLOMAC.c#L3350), asm:21-30) — bail unless `attacker` has contacted `defender`.
2. **`AI_Dipl_Unset_0 == 3` special path** ([3352-3371](../../MoM/src/DIPLOMAC.c#L3352-L3371), asm:33-62) — AI→human negative action with `Random(100) <= abs(relation_value)/10` records a `DA_Strength = 100`, `Dipl_Action = 39` action.
3. **Else — main path** ([3372-3611](../../MoM/src/DIPLOMAC.c#L3372-L3611), asm:64-649), gated on `attacker,defender < MOO_MONSTER_PLAYER_IDX` ([3374-3378](../../MoM/src/DIPLOMAC.c#L3374-L3378)) and the six-way condition ([3380-3392](../../MoM/src/DIPLOMAC.c#L3380-L3392)):
   - **`relation_value == -10000` force-record** ([3394-3407](../../MoM/src/DIPLOMAC.c#L3394-L3407), asm:94-112) — human attacker, `type != dipact_AuraOfMajesty`: writes a `DA_Strength = 10000` action.
   - **Reciprocation scaling** ([3408-3449](../../MoM/src/DIPLOMAC.c#L3408-L3449), asm:114-208) — positive: double if disliked (cap +10), else `/ (Visible_Rel/25 + 1)` (`diminishing_returns_divisor`). Negative: mirror with `Visible_Rel/-25 + 1`.
   - **Personality** ([3451-3472](../../MoM/src/DIPLOMAC.c#L3451-L3472), asm:208-252) — Lawful ×2 neg; Maniacal ×3/2 neg, ×3/4 pos.
   - **`relation_value_pre_charisma = relation_value`** ([3474](../../MoM/src/DIPLOMAC.c#L3474), asm:253-254) — snapshot before Charisma.
   - **Charismatic** ([3476-3492](../../MoM/src/DIPLOMAC.c#L3476-L3492), asm:255-280) — attacker Charismatic: ×2 pos, ÷2 neg.
   - **War + overextension** ([3493-3521](../../MoM/src/DIPLOMAC.c#L3493-L3521), asm:280-329) — at war, `type <= dipact_Overextension`, negative: (human) shift `peace_modifier` by `value/4`, then zero the value.
   - **Late-game AI-vs-AI** ([3523-3549](../../MoM/src/DIPLOMAC.c#L3523-L3549), asm:330-361) — `_turn > 200`, both non-human: ×2 pos; neg ÷ `(_difficulty/2)+1`.
   - **Apply (`relation_value != 0`)** ([3550-3592](../../MoM/src/DIPLOMAC.c#L3550-L3592), asm:362-549) — `Visible_Rel += relation_value` (clamp `[-100,100]`); bump + mirror `treaty_/exchange_modifier`; adjust `peace_modifier` from `relation_value_pre_charisma`; `spl_Earth_Lore` + `Random(100) < 11` → `Declare_War` (**OGBUG, see below**); human records the largest `DA_Strength` action.
   - **Status clamps** ([3593-3600](../../MoM/src/DIPLOMAC.c#L3593-L3600), asm:549-590) — at war → cap `-25`; not allied → cap `65`.
   - **Symmetrize** ([3601](../../MoM/src/DIPLOMAC.c#L3601), asm:591-605) — `Visible_Rel[attacker][defender] = Visible_Rel[defender][attacker]` (both sides `Visible_Rel`, matching asm:597/605).
   - **Hostility + timer** ([3602-3610](../../MoM/src/DIPLOMAC.c#L3602-L3610), asm:606-649) — at war → `Hostility = 3`, allied → `Hostility = 0`; `reevaluate_hostility_countdown = 15 + Random(10)`.

## OG quirks preserved (faithful — do not "fix")

- **Earth Lore triggers Declare_War** ([3571](../../MoM/src/DIPLOMAC.c#L3571)) — the war roll gates on `spell_idx == spl_Earth_Lore`, matching asm:505. Per the call-graph in the header comment this "should be `spl_Spell_Of_Mastery`" (casting Spell of Mastery is what should provoke war) — a genuine OGBUG, preserved with the `/* OGBUG … */` note. Do not fix.
- **`type != dipact_AuraOfMajesty`** ([3399](../../MoM/src/DIPLOMAC.c#L3399), [3603-ish](../../MoM/src/DIPLOMAC.c#L3550)) — `dipact_AuraOfMajesty = 0` equals the asm's `do_None = 0`. Descriptive constant; faithful.
- **Reciprocation / Personality / Charisma scaling** ([3408-3492](../../MoM/src/DIPLOMAC.c#L3408-L3492)) — all `shl`/`imul`/`idiv` sequences match the asm. Faithful.
- **`peace_modifier` two-sided `/4` shift in both branches** ([3507-3512](../../MoM/src/DIPLOMAC.c#L3507-L3512) war path vs [3559-3564](../../MoM/src/DIPLOMAC.c#L3559-L3564) not-at-war path) — the same both-directions `±(x/4)` adjustment appears in the at-war block (Block A, using the charisma-adjusted `relation_value`, then zeroes it) and the ordinary apply block (Block B, using `relation_value_pre_charisma`). They're mutually exclusive (A zeroes the value → B's `!= 0` guard skips), so at war only A fires, not-at-war only B. drake178's `// double shift if not at war?` ([3561](../../MoM/src/DIPLOMAC.c#L3561)) questions whether Block B should apply this war-shaped shift outside war — a suspect *OG* behavior left as an open DEDU, both blocks matching the asm (A asm:301-326, B asm:466-491). Preserved.
- **Preserved drake178 BUG/DEDU notes** — the war-path `// the result is still negative` ([3507](../../MoM/src/DIPLOMAC.c#L3507)/[3509](../../MoM/src/DIPLOMAC.c#L3509)) and the `relation_value_pre_charisma` "with reciprocation+personality, without charisma" question. OG-faithful annotations.
- **`__WIP` dropped** — the suffix was removed this session; justified now that R1 is fixed and the body is 1:1 with the asm.

## Related references

- `…\ovr085\Change_Relations__WIP.asm` — IDA Pro 5.5 disassembly (ground truth, 656 lines). On-disk name retains `__WIP`.
- `…\ovr085\Change_Relations__WIP.c` — Gemini translation of the asm.
- [`DIPLOMAC-Diplomacy_Growth.md`](DIPLOMAC-Diplomacy_Growth.md) — the Wave-10C caller. Sibling appliers `Change_Relations_For_Enchantments`, `Change_Relations_For_Bad_City_Spell` (ovr128). `Declare_War` ([DIPLOMAC.c:3625](../../MoM/src/DIPLOMAC.c#L3625)) — callee.
- Fields: `_players[].Dipl.{Contacted, Dipl_Status, Visible_Rel, DA_Strength, Dipl_Action, DA_Spell, DA_City, peace_/treaty_/exchange_modifier}`, `_players[].{Personality, charismatic, Hostility, reevaluate_hostility_countdown}`.
- Constants: `HUMAN_PLAYER_IDX = 0`, `MOO_MONSTER_PLAYER_IDX`, `DIPL_War`, `DIPL_Alliance`, `DIPL_Crusade`, `PRS_Lawful`, `PRS_Maniacal`, `dipact_AuraOfMajesty = 0`, `dipact_Overextension = 14`, `spl_Earth_Lore`, `AI_Dipl_Unset_0`.
- Tracked as a downstream helper of Wave 10C in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
