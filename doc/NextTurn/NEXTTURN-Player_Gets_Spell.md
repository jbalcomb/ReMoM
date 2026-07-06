NEXTTURN-Player_Gets_Spell.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Player_Gets_Spell.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Player_Gets_Spell.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Check_Spell_Research()
            |-> Player_Gets_Spell()       NEXTTURN

---

# `Player_Gets_Spell` — Walkthrough

**Location:** [NEXTTURN.c:3778-3862](../../MoM/src/NEXTTURN.c#L3778-L3862) (~85 lines, ends [line 3862](../../MoM/src/NEXTTURN.c#L3862)).
**WZD overlay:** ovr140 (o140p21; drake178/MoO2: `COLCALC` module `Player_Gets_Tech_App_()`).
**Called from:** spell-acquisition sites — research completion ([`Players_Check_Spell_Research`](../../MoM/src/NEXTTURN.c#L3615), Wave 7F) plus traded/gifted/treasure/conquest spell grants (per the source header).

> Verified against the **`.asm`** (272 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** Builds clean (Debug, MSVC).

## Purpose

Record that `player_idx` now knows `spell_idx`: mark it in `spells_list`, discount the Spell-of-Mastery research cost, and — if the gained spell was the player's current research target — clear it from the research candidate list and pick the next target (human: play the learn animation; AI: re-select and set the new remaining cost).

## Signature

```c
void Player_Gets_Spell(int16_t player_idx, int16_t spell_idx, int16_t New_Research)
```

Locals: `spells_cnt`, `was_research_target`, `spell_realm_idx`, `spell_realm`, `itr` (`_DI_`).

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Player_Gets_Spell.asm`.

1. **SoM cost discount** ([3786-3792](../../MoM/src/NEXTTURN.c#L3786-L3792), asm:19-55) — `if(som_research_cost > research_cost/2) som_research_cost -= research_cost/2; else som_research_cost = 0;` — a floor-subtract of half the gained spell's research cost.
2. **Mark known** ([3794-3796](../../MoM/src/NEXTTURN.c#L3794-L3796), asm:56-79) — `spell_realm = (spell_idx-1) / NUM_SPELLS_PER_MAGIC_REALM`, `spell_realm_idx = (spell_idx-1) % …`; `spells_list[realm*N + idx] = sls_Known`.
3. **Was this the research target?** `if(researching_spell_idx == spell_idx)` ([3798](../../MoM/src/NEXTTURN.c#L3798), asm:84-87):
   - **Yes** ([3800-3818](../../MoM/src/NEXTTURN.c#L3800-L3818), asm:89-166) — `was_research_target = ST_TRUE`; loop the candidate list removing entries equal to `researching_spell_idx` (`Clear_Structure` + zero the last slot, asm:91-132); `spells_cnt = Player_Research_Spells`; `research_cost_remaining = 0`; if no candidates left → `researching_spell_idx = spl_NONE (0)` ([3813](../../MoM/src/NEXTTURN.c#L3813), asm:148), else negate it ([3817](../../MoM/src/NEXTTURN.c#L3817), asm:151-164).
   - **No** ([3820-3832](../../MoM/src/NEXTTURN.c#L3820-L3832), asm:168-207) — loop the candidate list removing entries equal to `spell_idx` ([3825](../../MoM/src/NEXTTURN.c#L3825), asm:181), `Clear_Structure` + zero last slot + `Player_Research_Spells`.
4. **Human learn / AI re-select**:
   - Human + `New_Research` ([3833-3845](../../MoM/src/NEXTTURN.c#L3833-L3845), asm:208-223) — `Learn_Spell_Animation(spell_idx, was_research_target)`; if not the research target, `Stop_All_Sounds__STUB` + `Play_Background_Music`.
   - AI + was-research-target ([3846-3860](../../MoM/src/NEXTTURN.c#L3846-L3860), asm:225-264) — `AI_Spell_Research_Select`; `research_cost_remaining = (researching_spell_idx == spl_Spell_Of_Mastery) ? som_research_cost : spell_data_table[researching_spell_idx].research_cost` ([3859](../../MoM/src/NEXTTURN.c#L3859), asm:251 reads `researching_spell_idx`).

## Faithful (do not "fix")

- **Redundant last-slot zero after `Clear_Structure`** ([3806](../../MoM/src/NEXTTURN.c#L3806), [3828](../../MoM/src/NEXTTURN.c#L3828)) — `Clear_Structure` ([Util.c:299](../../MoX/src/Util.c#L299)) "shifts what follows downwards and sets the last one to all zeroes," so the explicit `research_spells[NUM_RESEARCH_SPELLS-1] = 0` re-zeroes an already-zero slot — harmless redundancy, **not a functional bug**. It's in the asm (asm:127/199), so preserve the store. (drake178's now-removed `¿ already done above ?` comments flagged this; confirmed redundant.)
- **Negate-in-place** ([3817](../../MoM/src/NEXTTURN.c#L3817), asm:151-164) — `researching_spell_idx = -(researching_spell_idx)` when candidates remain; faithful `neg`.
- **`spell_realm`/`spell_realm_idx` computed the long way** — `realm*N + idx` reconstructs `(spell_idx-1)`; matches the asm's separate `idiv` (asm:57-68).

## Related references

- `…\ovr140\Player_Gets_Spell.asm` — IDA Pro 5.5 disassembly (ground truth, 272 lines).
- `…\ovr140\Player_Gets_Spell.c` — Gemini translation of the asm.
- Callees: `Clear_Structure` ([MoX/src/Util.c:299](../../MoX/src/Util.c#L299)), `Player_Research_Spells`, `AI_Spell_Research_Select`, `Learn_Spell_Animation`, `Stop_All_Sounds__STUB`, `Play_Background_Music`.
- Constants/fields: `_players[].{som_research_cost, research_cost_remaining, researching_spell_idx, spells_list, research_spells}`, `spell_data_table[].research_cost`, `NUM_SPELLS_PER_MAGIC_REALM`, `NUM_RESEARCH_SPELLS`, `sls_Known`, `spl_NONE = 0`, `spl_UNDEFINED = -1`, `spl_Spell_Of_Mastery`, `HUMAN_PLAYER_IDX`.
- Tracked in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md) (sub-item under Wave 7F).
