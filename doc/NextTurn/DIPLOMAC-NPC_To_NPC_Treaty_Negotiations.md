DIPLOMAC-NPC_To_NPC_Treaty_Negotiations.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr086\NPC_To_NPC_Treaty_Negotiations__STUB.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr086\NPC_To_NPC_Treaty_Negotiations__STUB.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> NPC_To_NPC_Treaty_Negotiations()

---

# `NPC_To_NPC_Treaty_Negotiations` — Walkthrough

**Location:** [DIPLOMAC.c:4837-5007](../../MoM/src/DIPLOMAC.c#L4837-L5007) (~171 lines, ends [line 5007](../../MoM/src/DIPLOMAC.c#L5007)).
**WZD overlay:** ovr086, p02 (`MoO2` module `NPCDIPLO :: NPC_To_NPC_Treaty_Negotiations_`).
**On-disk OG name:** the disassembly/decompilation files retain the `__STUB` suffix (`NPC_To_NPC_Treaty_Negotiations__STUB`) — stale, since the body is fully reconstructed (not a stub). The production symbol otherwise matches the OG/IDA stem.
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L772) — normal end-of-turn.

> Verified against the **`.asm`** (433 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** Builds clean (Debug, MSVC).

## Purpose

The per-turn AI-vs-AI (NPC-to-NPC) diplomacy pass. Two sequential loop nests over all AI wizard pairs `(itr_players1, itr_players2)`:

1. **Treaty negotiation** — for each contacted AI pair, with a difficulty-scaled probability, score the relationship (`Base_Score` = hidden + visible relations + personality modifier, plus random and status bonuses) and act on it: form an alliance or wizard pact if the score clears 150, else attempt a spell exchange; then always run a goodwill roll, a war→peace check, three diplomat-modifier adjustments, side-picking, war-need evaluation, and the "get off my lawn" reaction.
2. **Ally-drags-you-to-war** — for each allied AI pair, any third AI at war with one ally but not the other has war declared on it by the second ally.

## Signature

```c
void NPC_To_NPC_Treaty_Negotiations(void)
```

Locals: `Total_Score`, `Base_Score`, `Other_Players_Count`, `Trade_Spell_Count`, `Trade_For_Spell`, `First_Trade_Spell`, `Player_Loop_Var` (third-party index in nest 2), `itr_players1`/`itr_players2` (the AI pair).

## Code walk

Line refs are production [DIPLOMAC.c](../../MoM/src/DIPLOMAC.c); cross-checked against `NPC_To_NPC_Treaty_Negotiations__STUB.asm`.

**Early bail** ([4850-4853](../../MoM/src/DIPLOMAC.c#L4850-L4853), asm:16-23) — if the **human** player is casting `spl_Spell_Of_Return`, return immediately. See OGBUG below.

### Nest 1 — treaty negotiation ([4855-4960](../../MoM/src/DIPLOMAC.c#L4855-L4960), asm:26-338)

Both loops start at `1` (asm:26 `mov di, 1`, asm:30 `mov si, 1`).

1. **Per-pair guards** ([4859-4882](../../MoM/src/DIPLOMAC.c#L4859-L4882), asm:33-78) — `continue` unless both fortresses active, `itr_players1 != itr_players2`, `Random(15 - _difficulty*2) == 1`, and `itr_players1` has contacted `itr_players2`.
2. **Scoring** ([4884-4899](../../MoM/src/DIPLOMAC.c#L4884-L4899), asm:80-140) — `Base_Score = Hidden_Rel + Visible_Rel + TBL_AI_PRS_IDK_Mod[Personality[itr_players2]]`; `Total_Score = Base_Score + Random(100) + (Random(5)+3)*_difficulty`; `+10` if pacted, `+20` if allied.
3. **Alliance** ([4901-4908](../../MoM/src/DIPLOMAC.c#L4901-L4908), asm:142-166) — if `Total_Score + treaty_modifier > 150` and not already allied → `Start_Treaty(…, DIPL_Alliance)`, `goto loc_72C33`.
4. **Wizard pact** ([4910-4917](../../MoM/src/DIPLOMAC.c#L4910-L4917), asm:168-193) — same `> 150` test; if not already pacted → `Start_Treaty(…, DIPL_WizardPact)`, `goto loc_72C33`.
5. **Spell exchange** ([4919-4932](../../MoM/src/DIPLOMAC.c#L4919-L4932), asm:195-244) — if `exchange_modifier + 80 < Total_Score`: pull each side's top differential spell via `Get_Differential_Spell_List`; if both non-empty, swap them with `Diplomacy_Player_Gets_Spell`.
6. **`loc_72C33` — goodwill roll** ([4934-4938](../../MoM/src/DIPLOMAC.c#L4934-L4938), asm:245-267) — `Random(20) == 1` → `Change_Relations(Random(5)+5, itr_players1, itr_players2, 1, 0, 0)`.
7. **War → peace** ([4940-4946](../../MoM/src/DIPLOMAC.c#L4940-L4946), asm:268-291) — if at war and `m_diplomsg_1_record_sub_number + peace_modifier > 100` → `Declare_Peace`.
8. **Post-passes** ([4948-4958](../../MoM/src/DIPLOMAC.c#L4948-L4958), asm:292-325) — `Adjust_Diplomat_Modifiers` ×3, `G_DIPL_PickSides()`, `G_DIPL_NeedForWar`, and if `Dipl_Action == 0` → `DIPL_GetOffMyLawn(itr_players2, itr_players1)` (note swapped arg order, matching asm:321-323).

### Nest 2 — ally-drags-you-to-war ([4962-5005](../../MoM/src/DIPLOMAC.c#L4962-L5005), asm:340-425)

All three loops start at `1` (asm:341/345/373).

- For each allied pair (both fortresses active, `Dipl_Status == DIPL_Alliance`, [4976](../../MoM/src/DIPLOMAC.c#L4976), asm:366-372), scan every third player `Player_Loop_Var` ([4978](../../MoM/src/DIPLOMAC.c#L4978)): skip if inactive, equal to either ally; if it is at war with `itr_players1` but **not** at war with `itr_players2` → `Declare_War(itr_players2, Player_Loop_Var)` ([4999](../../MoM/src/DIPLOMAC.c#L4999), asm:388-404).

## OG quirks preserved (faithful — do not "fix")

- **Human `spl_Spell_Of_Return` gates AI-vs-AI diplomacy** ([4850](../../MoM/src/DIPLOMAC.c#L4850), asm:16-23) — the entire AI-vs-AI pass is skipped based on the *human* player's casting spell. drake178: "what does this have to do with AI-AI diplomacy?" ([4849](../../MoM/src/DIPLOMAC.c#L4849)). Suspect OG behavior, faithful to the asm. Do not fix.
- **`goto loc_72C33`** ([4906](../../MoM/src/DIPLOMAC.c#L4906), [4915](../../MoM/src/DIPLOMAC.c#L4915)) — the alliance/pact branches jump past the spell-exchange block straight to the goodwill roll, faithfully replicating the asm's `jmp loc_72C33` (asm:166, 193). The label name is the asm offset.
- **Double `> 150` test** ([4901](../../MoM/src/DIPLOMAC.c#L4901) then [4910](../../MoM/src/DIPLOMAC.c#L4910)) — the same `Total_Score + treaty_modifier > 150` comparison is recomputed for the alliance and the pact branch (asm:151-152 and 178-179). Redundant but faithful.
- **`DIPL_GetOffMyLawn(itr_players2, itr_players1)` swapped args** ([4957](../../MoM/src/DIPLOMAC.c#L4957)) — the callee receives the pair reversed relative to every other call in this function; matches the asm push order (asm:321-323). Faithful.

## Related references

- `…\ovr086\NPC_To_NPC_Treaty_Negotiations__STUB.asm` — IDA Pro 5.5 disassembly (ground truth, 433 lines). On-disk name retains the `__STUB` suffix.
- `…\ovr086\NPC_To_NPC_Treaty_Negotiations__STUB.c` — Gemini translation of the asm.
- Callees: `Start_Treaty`, `Get_Differential_Spell_List`, `Diplomacy_Player_Gets_Spell`, [`Change_Relations`](../../MoM/src/DIPLOMAC.c#L3410) (see [`DIPLOMAC-Change_Relations.md`](DIPLOMAC-Change_Relations.md)), `Declare_Peace`, `Adjust_Diplomat_Modifiers`, [`G_DIPL_PickSides`](../../MoM/src/DIPLOMAC.c#L3240), [`G_DIPL_NeedForWar`](../../MoM/src/DIPLOMAC.c#L5755), [`DIPL_GetOffMyLawn`](../../MoM/src/DIPLOMAC.c#L5995), `Declare_War`. The three `G_DIPL_*`/`DIPL_*` post-pass callees were reconstructed alongside this function — quick error-scan only, full 1:1 review pending.
- Fields: `_players[].Dipl.{Contacted, Hidden_Rel, Visible_Rel, Dipl_Status, Dipl_Action, treaty_modifier, exchange_modifier, peace_modifier}`, `_players[].Personality`, `_players[].casting_spell_idx`, `_FORTRESSES[].active`.
- Constants: `spl_Spell_Of_Return`, `DIPL_WizardPact = 1`, `DIPL_Alliance`, `DIPL_War`, `TBL_AI_PRS_IDK_Mod[]`, `m_exchange_spell_list`, `m_diplomsg_1_record_sub_number`, `_human_player_idx`, `_num_players`, `_difficulty`.
- Sibling of [`DIPLOMAC-Diplomacy_Growth.md`](DIPLOMAC-Diplomacy_Growth.md) in the end-of-turn diplomacy chain.
