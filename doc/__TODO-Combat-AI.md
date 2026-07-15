__TODO-Combat-AI.md

# 'AI Combat' - Overview

Scope: the computer player's **combat turn** — driving each AI battle unit to pick and perform an action (move, attack, use item, cast), including reaching the spell layer. Parent: [Tactical Combat](__TODO-Combat.md). Child: [AI Combat Spell](__TODO-Combat-AI-Spell.md).

**Bottom line:** the turn driver and the per-unit action chain are **implemented (substantive)** but not yet reviewed 1:1 (done-done); a couple of movement helpers are still WIP. The spell sub-layer is complete — see [AI Combat Spell](__TODO-Combat-AI-Spell.md).

Call chain:

```
Combat_Next_Turn (Combat.c:4528)                         ← turn-flow driver (was CMB_ProgressTurnFlow__WIP)
  └ Auto_Cast_Spell_And_Do_Combat_Turn (Combat.c:4508)   ← AI turn driver (was AI_CMB_PlayTurn__WIP)
      ├ Combat_Cast_Spell(20+player, …)                  → AI Combat Spell layer (done)
      └ Auto_Do_Combat_Turn(player)                      → per-unit movement/attacks
          └ AI_BU_ProcessAction (CMBTAI.c:268)           ← action dispatch (holds the cast call @521)
              ├ Choose_Target_And_Action (CMBTAI.c:1153) ← target/action chooser (was AI_BU_SelectAction__WIP)
              ├ AI_BU_AssignAction (CMBTAI.c:968)
              ├ AI_MoveBattleUnits__WIP
              └ AI_RestrictToCity__WIP (Combat.c:23075)
```

Status legend: **[done-done]** reviewed 1:1 + doc · **[impl]** substantive body, not yet reviewed · **[WIP]** placeholder/incomplete.

| Function | Loc | Status | Note |
|---|---|---|---|
| `Combat_Next_Turn` | [Combat.c:4528](../MoM/src/Combat.c#L4528) | **impl** | turn-flow: cp/defender/human turns, `m_cp_took_turn` gate; **was `CMB_ProgressTurnFlow__WIP`** |
| `Auto_Cast_Spell_And_Do_Combat_Turn` | [Combat.c:4508](../MoM/src/Combat.c#L4508) | **impl** | AI casts a spell then does its unit turn; **was `AI_CMB_PlayTurn__WIP`** |
| `Auto_Do_Combat_Turn` | [Combat.c:4522](../MoM/src/Combat.c#L4522) (call) | (not assessed) | the unit-movement half of the AI turn |
| `AI_BU_ProcessAction` | [CMBTAI.c:268](../MoM/src/CMBTAI.c#L268) | **impl** | action-dispatch switch; contains the AI cast call at [521](../MoM/src/CMBTAI.c#L521) |
| `Choose_Target_And_Action` | [CMBTAI.c:1153](../MoM/src/CMBTAI.c#L1153) | **impl** | per-unit target/action chooser (Summon Demon, Doom Bolt, Fireball, item/spell, scoring); **was `AI_BU_SelectAction__WIP`** |
| `AI_BU_AssignAction` | [CMBTAI.c:968](../MoM/src/CMBTAI.c#L968) | **impl** | assigns the chosen action |
| `AI_BU_GetAttackValue` | Combat.c | **done-done** | tracker `[x]` |
| `AI_FightorFlight` | Combat.c | **done-done** | tracker `[x]` |
| `AI_GetCombatRallyPt` | Combat.c | **done-done** | tracker `[x]` |
| `AI_SetBasicAttacks` | Combat.c | **done-done** | tracker `[x]` |
| `AI_MoveBattleUnits__WIP` | Combat.c | **WIP** | movement driver, still stubbed |
| `AI_RestrictToCity__WIP` | [Combat.c:23075](../MoM/src/Combat.c#L23075) | **WIP** | keep-in-city restriction, still stubbed |

# To-Do List

- [ ] **Done-done the turn drivers** — `Combat_Next_Turn`, `Auto_Cast_Spell_And_Do_Combat_Turn`: 1:1 walkthrough vs their `ovr98` disassembly + docs + checkboxes.
- [ ] **Done-done the per-unit action chain** — `AI_BU_ProcessAction`, `Choose_Target_And_Action`, `AI_BU_AssignAction` (all substantive in `CMBTAI.c`).
- [ ] **Assess `Auto_Do_Combat_Turn`** — not yet classified (stub vs substantive); it is the movement half of the AI turn.
- [ ] **Reconstruct the WIP movement helpers** — `AI_MoveBattleUnits__WIP`, `AI_RestrictToCity__WIP`.
- [ ] **Tracker hygiene** — [stub_wip_todo.md](#TODO/stub_wip_todo.md) still lists `AI_CMB_PlayTurn__WIP` / `AI_BU_ProcessAction__WIP` / `AI_BU_SelectAction__WIP` / `AI_BU_AssignAction__WIP` as `[ ]`, but their renamed/real bodies exist and are substantive; reconcile.

## Notes

- **Renames applied** (production drops the `__WIP` / re-scopes the name; the OG IDA/asm names are unchanged):
  - `AI_CMB_PlayTurn__WIP` → `Auto_Cast_Spell_And_Do_Combat_Turn` ([Combat.c:4508](../MoM/src/Combat.c#L4508))
  - `CMB_ProgressTurnFlow__WIP` → `Combat_Next_Turn` ([Combat.c:4528](../MoM/src/Combat.c#L4528))
  - `AI_BU_SelectAction__WIP` → `Choose_Target_And_Action` ([CMBTAI.c:1153](../MoM/src/CMBTAI.c#L1153))
- The AI combat turn is **reachable and running** — `Combat_Next_Turn` calls `Auto_Cast_Spell_And_Do_Combat_Turn` for the cp/defender/auto-combat human ([4539](../MoM/src/Combat.c#L4539)/[4546](../MoM/src/Combat.c#L4546)/[4552](../MoM/src/Combat.c#L4552)).
- Down here the work is mostly **review (impl → done-done)**, not fresh reconstruction, except the two `__WIP` movement helpers.
