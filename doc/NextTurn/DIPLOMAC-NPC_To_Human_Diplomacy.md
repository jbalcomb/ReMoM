DIPLOMAC-NPC_To_Human_Diplomacy.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr087\NPC_To_Human_Diplomacy__WIP.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr087\NPC_To_Human_Diplomacy__WIP.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> NPC_To_Human_Diplomacy()       DIPLOMAC

---

# `NPC_To_Human_Diplomacy` — Walkthrough

**Location:** [DIPLOMAC.c:5619-5741](../../MoM/src/DIPLOMAC.c#L5619-L5741) (~123 lines, ends [line 5741](../../MoM/src/DIPLOMAC.c#L5741)).
**WZD overlay:** ovr087 (o87p02; drake178/MoO2: `NPCDIPLO` module `NPC_To_Human_Diplomacy_()`). On-disk asm/`.c` retain the `__WIP` filename suffix.
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L616) — normal end-of-turn, call site [NEXTTURN.c:777](../../MoM/src/NEXTTURN.c#L777) (Wave 10F).

> Verified against the **`.asm`** (233 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** Fully reconstructed; `__WIP` dropped; builds clean (Debug, MSVC). **PRNG-critical** — consumes `Random(75)`, `Random(100)`, `Random(4)`; the `&&` short-circuits are load-bearing for RNG-stream parity (see Faithful).

## Purpose

Once per turn, decide what diplomatic action (if any) each AI wizard initiates toward the **human** this turn, writing it into `_players[HUMAN].Dipl.Dipl_Action[player_idx]`. Handles the introduction of newly-met wizards, a pending-bounty case, and — for everyone else — a relation-score gate that may escalate to a war/peace decision, a required-alliance nudge, or an NPC demand. Two per-wizard fallbacks (`G_DIPL_NeedForWar`, `DIPL_GetOffMyLawn`) run when no action was set.

## Signature

```c
void NPC_To_Human_Diplomacy(void)
```

Locals: `Total_Score`, `Lowest_Interest`, `player_idx` (`_SI_`), `da_strength` (asm `di`).

## Code walk

Line refs are production [DIPLOMAC.c](../../MoM/src/DIPLOMAC.c); cross-checked against `NPC_To_Human_Diplomacy__WIP.asm`.

- **Spell-of-Return gate** ([5624-5627](../../MoM/src/DIPLOMAC.c#L5624-L5627), asm:12-19) — human casting `spl_Spell_Of_Return` → return early.
- **`Limit_Treaty_Modifiers()`** ([5628](../../MoM/src/DIPLOMAC.c#L5628), asm:22).
- **Per-wizard loop** `for(player_idx = 1 …)` ([5629](../../MoM/src/DIPLOMAC.c#L5629), asm:23, 222-224). Each iteration is an if/else-if chain writing `Dipl_Action[player_idx]`:
  1. **Inactive fortress** ([5631-5635](../../MoM/src/DIPLOMAC.c#L5631-L5635), asm:26-35) — `Dipl_Action = do_None`, then `continue` straight to the next wizard (bypassing the fallbacks — asm:35 `jmp` to loop tail).
  2. **Crusade** ([5636-5639](../../MoM/src/DIPLOMAC.c#L5636-L5639), asm:37-41) — `Dipl_Action = do_None`.
  3. **New contact** ([5640-5644](../../MoM/src/DIPLOMAC.c#L5640-L5644), asm:43-54) — `contact_stage == pcs_Greeted` → advance to `pcs_Established`, `Dipl_Action = do_Greeting + Personality` (the intro event).
  4. **Pending bounty** ([5645-5648](../../MoM/src/DIPLOMAC.c#L5645-L5648), asm:56-60) — `niu_bounty_collect != 0` → `Dipl_Action = do_2` (deprecated MoO1 field; faithfully reproduced).
  5. **Else — relation score** ([5649-5731](../../MoM/src/DIPLOMAC.c#L5649-L5731), asm:62-190), only when `Dipl_Action` isn't already `do_39/do_30/do_40/do_41` ([5651-5659](../../MoM/src/DIPLOMAC.c#L5651-L5659), asm:62-72):
     - `Total_Score = Hidden_Rel[m] + Visible_Rel[m] + Find_Worst_Modifier() + TBL_AI_PRS_IDK_Mod[Personality[m]]` ([5661-5668](../../MoM/src/DIPLOMAC.c#L5661-L5668), asm:75-97).
     - If `Total_Score + Visible_Rel[m] >= -100` → `do_None` ([5669-5672](../../MoM/src/DIPLOMAC.c#L5669-L5672), asm:98-106); otherwise the `da_strength` branch, where **`da_strength = DA_Strength[player_idx] * 2`** ([5675](../../MoM/src/DIPLOMAC.c#L5675), asm:111-113 — the value-doubling `shl ax,1`):
       - `DA_Strength[player_idx] < 0`: `field_126 > 0 || Random(75) < abs(da_strength)` → `DIPL_HumanWarOrPeace`, else `do_None` ([5676-5690](../../MoM/src/DIPLOMAC.c#L5676-L5690), asm:114-140).
       - `DA_Strength[player_idx] >= 0`: if `Dipl_Status == DIPL_War && (30 + Random(100)) < peace_modifier` → `DIPL_HumanWarOrPeace` ([5693-5700](../../MoM/src/DIPLOMAC.c#L5693-L5700), asm:142-158); else `if(field_126 == 0) da_strength += 3`, then if `Random(100) < da_strength && Random(4) == 1` → (`field_126 > 0` ? clear `field_126` : `NPC_Diplo_s74420(HUMAN, player_idx)`), else `NPC_Required_Alliances(player_idx)` ([5702-5727](../../MoM/src/DIPLOMAC.c#L5702-L5727), asm:160-197).
- **Fallbacks** ([5732-5739](../../MoM/src/DIPLOMAC.c#L5732-L5739), asm:198-219) — if `Dipl_Action == do_None`: `G_DIPL_NeedForWar(HUMAN, player_idx)`; then again if `do_None`: `DIPL_GetOffMyLawn(player_idx, HUMAN)`.

## Faithful (do not "fix")

- **`player_idx = 1` start** (asm:23) — the human's own slot (0) is skipped.
- **`Spell_Of_Return` early return** (asm:16) — suppress the whole pass while the human casts Spell of Return.
- **`¿ OGBUG doesn't set a diplomatic order ?`** ([5713-5717](../../MoM/src/DIPLOMAC.c#L5713-L5717), asm:177-180) — in the deep `Random`-gated branch, `field_126 > 0` just zeroes `field_126` and sets **no** `Dipl_Action`. Faithful to the asm (asm:179 writes only `field_126 = 0`); preserve.
- **Short-circuit `&&` = asm branch skips** — `(Dipl_Status == DIPL_War) && (30 + Random(100) < …)` and `(Random(100) < da_strength) && (Random(4) == 1)` both short-circuit exactly where the asm's `jnz/jge` skip the follow-up `Random` (asm:143/169), so RNG draws match.

## Related references

- `…\ovr087\NPC_To_Human_Diplomacy__WIP.asm` — IDA Pro 5.5 disassembly (ground truth, 233 lines).
- `…\ovr087\NPC_To_Human_Diplomacy__WIP.c` — Gemini translation of the asm.
- Callees: `Limit_Treaty_Modifiers`, `Find_Worst_Modifier`, `DIPL_HumanWarOrPeace` ([DIPLOMAC.c:6507](../../MoM/src/DIPLOMAC.c#L6507)), `NPC_Diplo_s74420` ([6249](../../MoM/src/DIPLOMAC.c#L6249), asm `sub_34C10`), `NPC_Required_Alliances` ([6016](../../MoM/src/DIPLOMAC.c#L6016), asm `IDK_Dipl_s73F1C`), `G_DIPL_NeedForWar`, `DIPL_GetOffMyLawn`, `Random` (1-based `1..n`), `abs`.
- Constants/fields: `_players[].Dipl.{Dipl_Action, Dipl_Status, contact_stage, DA_Strength, Hidden_Rel, Visible_Rel, peace_modifier, field_126, niu_bounty_collect}`, `TBL_AI_PRS_IDK_Mod`, `_FORTRESSES[].active`, `spl_Spell_Of_Return`, `DIPL_Crusade`, `DIPL_War`, `pcs_Greeted`/`pcs_Established`, `do_None`/`do_Greeting`/`do_2`/`do_39`/`do_30`/`do_40`/`do_41`, `HUMAN_PLAYER_IDX`.
- Tracked as **Wave 10F** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
