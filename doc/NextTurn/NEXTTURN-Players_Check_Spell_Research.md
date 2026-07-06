NEXTTURN-Players_Check_Spell_Research.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Players_Check_Spell_Research.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Players_Check_Spell_Research.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Check_Spell_Research()       NEXTTURN
            |-> Player_Gets_Spell()
            |-> Spell_Research_Select() / AI_Spell_Research_Select()

---

# `Players_Check_Spell_Research` — Walkthrough

**Location:** [NEXTTURN.c:3631-3687](../../MoM/src/NEXTTURN.c#L3631-L3687) (~57 lines, ends [line 3687](../../MoM/src/NEXTTURN.c#L3687)).
**WZD overlay:** ovr140 (o140p20).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L616) — normal end-of-turn, call site [NEXTTURN.c:740](../../MoM/src/NEXTTURN.c#L740) (Wave 7F).

> Verified against the **`.asm`** (134 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** One ReMoM debug-hook deviation (`DBG_trigger_complete_research_spell`, below).

## Purpose

Once per turn, for each player: if their current research finished (`research_cost_remaining == 0` with a live target), grant the spell via `Player_Gets_Spell`; and if the player needs a new research target (negated marker, or none set on the first turns), run the picker (human UI vs. AI selector) and seed the next `research_cost_remaining`.

## Signature

```c
void Players_Check_Spell_Research(void)
```

Locals: `itr_players` (`_SI_`), `did_learn_spell_anim` (`_DI_`, was `IDK` — "a spell was learned via research this pass" flag; gates the human music restore).

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Players_Check_Spell_Research.asm`. `researching_spell_idx` sign convention: `> spl_NONE (0)` = actively researching; `< spl_NONE` = the negated "needs a new pick" marker (set by `Player_Gets_Spell`); `== spl_NONE` = none.

- **Debug hook** ([3636-3640](../../MoM/src/NEXTTURN.c#L3636-L3640)) — **not in the asm; see deviation.**
- **Per-player loop** `for(itr_players = 0 …)` ([3641](../../MoM/src/NEXTTURN.c#L3641), asm:11, 124-126).
  1. **Research complete → learn** ([3643-3651](../../MoM/src/NEXTTURN.c#L3643-L3651), asm:14-39) — `if(research_cost_remaining == 0 && researching_spell_idx > spl_NONE)` → `Player_Gets_Spell(itr_players, researching_spell_idx, 1)`, `IDK = ST_TRUE`.
  2. **Need a new target?** ([3652-3660](../../MoM/src/NEXTTURN.c#L3652-L3660), asm:40-58) — `researching_spell_idx < spl_NONE || (researching_spell_idx == spl_NONE && _turn < 2)`:
     - **Human** ([3662-3672](../../MoM/src/NEXTTURN.c#L3662-L3672), asm:60-83) — `Set_Mouse_List(1, mouse_list_default)`, `Spell_Research_Select()` (the picker UI), `Set_Mouse_List(1, mouse_list_hourglass)`; if `IDK == ST_TRUE` → `Stop_All_Sounds__STUB` + `Play_Background_Music` (restore music after the research-complete fanfare).
     - **AI** ([3673-3676](../../MoM/src/NEXTTURN.c#L3673-L3676), asm:85-88) — `AI_Spell_Research_Select(itr_players)`.
     - **Seed new cost** ([3677-3684](../../MoM/src/NEXTTURN.c#L3677-L3684), asm:89-121) — `research_cost_remaining = (researching_spell_idx == spl_Spell_Of_Mastery) ? som_research_cost : spell_data_table[researching_spell_idx].research_cost`.

## ReMoM deviations (deliberate — not OG-faithful, not bugs)

- **`DBG_trigger_complete_research_spell`** ([3636-3640](../../MoM/src/NEXTTURN.c#L3636-L3640)) — a debug hook that force-zeroes the human's `research_cost_remaining` (so the next loop grants the researched spell immediately), then clears the trigger. Not in the asm; a ReMoM testing convenience. Inert unless the debug trigger is set.

## Faithful (do not "fix")

- **`did_learn_spell_anim` is not reset per iteration** (asm `_DI_IDK`) — set `ST_TRUE` on any learn and never cleared inside the loop, but the music-restore it gates only runs on the human branch (`itr_players == HUMAN_PLAYER_IDX == 0`, the first iteration), so the carry-over is harmless. Faithful.
- **`_turn < 2` initial-pick window** (asm:56) — only forces a pick for `researching_spell_idx == spl_NONE` during the first two turns; afterwards a `spl_NONE` player is left alone. Preserve.
- **Sign-convention branches** — `> spl_NONE` / `< spl_NONE` / `== spl_NONE` map to the asm's `jle`/`jl`/`jz` exactly (asm:26/46/52).

## Related references

- `…\ovr140\Players_Check_Spell_Research.asm` — IDA Pro 5.5 disassembly (ground truth, 134 lines).
- `…\ovr140\Players_Check_Spell_Research.c` — Gemini translation of the asm.
- Callees: [`Player_Gets_Spell`](../../MoM/src/NEXTTURN.c#L3778) (o140p21, reviewed — [NEXTTURN-Player_Gets_Spell.md](NEXTTURN-Player_Gets_Spell.md)), `Set_Mouse_List`, `Spell_Research_Select`, `AI_Spell_Research_Select`, `Stop_All_Sounds__STUB`, `Play_Background_Music`.
- Constants/fields: `_players[].{research_cost_remaining, researching_spell_idx, som_research_cost}`, `spell_data_table[].research_cost`, `spl_NONE = 0`, `spl_Spell_Of_Mastery`, `mouse_list_default`/`mouse_list_hourglass`, `HUMAN_PLAYER_IDX`, `_turn`, `DBG_trigger_complete_research_spell`.
- Tracked as **Wave 7F** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
