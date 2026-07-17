__TODO-Combat-AI-Spell.md

# 'AI Combat Spell' - Overview

Scope: the computer player's **combat spellcasting** — choosing a spell, scoring it, picking a target, paying, and resolving. Sits under [AI Combat](__TODO-Combat-AI.md) → [Tactical Combat](__TODO-Combat.md).

**Bottom line: functionally complete and live.** Every node on this chain has a substantive body, and the path is reachable in a real AI combat turn (no disabled/TODO guard). What remains is **review/finalize (done-done)**, not implementation — plus one missing helper to resolve and tracker hygiene.

Call chain:

```
AI_BU_ProcessAction (CMBTAI.c:268)                       ← BUA_UseItem / BUA_CastSpell arm
  └ Combat_Cast_Spell(20+player, …)   CMBTAI.c:521       done-done
      ├ AI_SetCombatRealms()                             → AI_UnitThreatRealms()
      ├ AI_SelectCmbtSpell()                             → AI_CombatSpellList(), AI_EvaluateCmbtSpell()
      └ AITP_Combat_Spell()                              done-done
          └ 10 AITP_* sub-pickers
```

Status legend: **[done-done]** reviewed 1:1 + doc + builds · **[impl]** substantive body, not yet reviewed 1:1 · **[missing]** no body.

| Function | Loc | Status | What it does |
|---|---|---|---|
| `Combat_Cast_Spell` | [Combat.c:13176](../MoM/src/Combat.c#L13176) | **done-done** | AI-branch cast dispatcher; [walkthrough](#CodeReview/Combat-Combat_Cast_Spell.md) |
| `AITP_Combat_Spell` | [Combat.c:11650](../MoM/src/Combat.c#L11650) | **done-done** | combined combat target picker; [walkthrough](ComputerPlayer/Combat-AITP_Combat_Spell.md) |
| `Combat_Spell_Dispel_Attempt` | [Spells133.c:653](../MoM/src/Spells133.c#L653) | **impl** (builds clean) | counter/node dispel roll; was `WIZ_DispelAttempt__STUB`; arg order verified vs asm |
| `Combat_Spell_Counter_Message` | [Spells133.c:532](../MoM/src/Spells133.c#L532) | **impl** (builds clean) | "Counter Magic" popup; was `CMB_CounterMessage__STUB`; arg order verified vs asm |
| `AI_SelectCmbtSpell` | [Combat.c:24565](../MoM/src/Combat.c#L24565) | **impl** | builds castable list, scores via `AI_EvaluateCmbtSpell`, returns best |
| `AI_EvaluateCmbtSpell` | [Combat.c:24632](../MoM/src/Combat.c#L24632) | **impl** | 5 situational tables by threat mode + per-spell adjustments; score / -1 / -100 |
| `AI_CombatSpellList` | [Combat.c:25959](../MoM/src/Combat.c#L25959) | **impl** | affordable-spell list (wizard vs unit vs creature realm sets); count → `GUI_Multipurpose_Int` |
| `AI_SetCombatRealms` | [Combat.c:25792](../MoM/src/Combat.c#L25792) | **impl** | sets attacker/defender realm bitmasks; calls `AI_UnitThreatRealms` ×2 |
| `AI_UnitThreatRealms` | [Combat.c:25860](../MoM/src/Combat.c#L25860) | **impl** | per-realm attack tally → percentages |
| `AITP_DarknessLight` | [Combat.c:12065](../MoM/src/Combat.c#L12065) | **impl** | active Life/Death-race unit → 99 |
| `AITP_Healing` | [Combat.c:12099](../MoM/src/Combat.c#L12099) | **impl** | own unit missing most HP, weighted by attack |
| `AITP_WarpWood` | [Combat.c:12144](../MoM/src/Combat.c#L12144) | **impl** | enemy missile unit by ammo value |
| `AITP_WarpCreature` | [Combat.c:12196](../MoM/src/Combat.c#L12196) | **impl** | enemy normal unit by attack × resist-fail |
| `AITP_DispelMagic` | [Combat.c:12260](../MoM/src/Combat.c#L12260) | **impl** | two-phase: own debuffs vs enemy enchantments |
| `AITP_HolyWord` | [Combat.c:13068](../MoM/src/Combat.c#L13068) | **impl** | eligible enemy fantastic unit → 99 |
| `AITP_EarthToMud` | [Combat.c:14588](../MoM/src/Combat.c#L14588) | **impl** | grid scan, mud near enemies vs own units |
| `AITP_Disrupt` | [Combat.c:14708](../MoM/src/Combat.c#L14708) | **impl** | five wall-square probes (attacker vs walled city) |
| `AITP_CracksCall` | [Combat.c:14763](../MoM/src/Combat.c#L14763) | **impl** | strongest visible enemy ground unit + wall fallback |
| `AITP_RecallHero` | [Combat.c:14876](../MoM/src/Combat.c#L14876) | **impl** | own hero in most danger (excludes own-fortress battle) |
| `UU15_AITP_Disintegrate` | [Combat.c:12245](../MoM/src/Combat.c#L12245) | **missing** | comment-only; likely not needed (see notes) |

# To-Do List

- [ ] **Done-done the 5 AI evaluators** — `AI_SelectCmbtSpell`, `AI_EvaluateCmbtSpell`, `AI_CombatSpellList`, `AI_SetCombatRealms`, `AI_UnitThreatRealms`: 1:1 walkthrough vs their `ovr139` disassembly, `B`/`R` bug pass, walkthrough doc, checkbox.
- [ ] **Done-done the 10 `AITP_*` sub-pickers** (`DarknessLight`, `HolyWord`, `Healing`, `WarpWood`, `WarpCreature`, `EarthToMud`, `Disrupt`, `RecallHero`, `CracksCall`, `DispelMagic`) — same treatment vs `ovr111`.
- [ ] **Resolve `UU15_AITP_Disintegrate`** — confirm nothing calls a standalone picker (Disintegrate targeting is inline in `AITP_Combat_Spell`'s `scc_Combat_Destroy_Unit`/`Banish` case) and delete the comment stub; else reconstruct it.
- [ ] **Tracker hygiene** — add the 15 `impl` functions above to [stub_wip_todo.md](#TODO/stub_wip_todo.md); they are currently untracked.

## Notes

- **Reachable & live:** the AI cast at [CMBTAI.c:521](../MoM/src/CMBTAI.c#L521) is inside `AI_BU_ProcessAction`'s live dispatch switch — no disable/TODO guard. The full selection → scoring → targeting chain runs in a real AI turn.
- The cast entry has **no `__WIP` symbol** — the real `Combat_Cast_Spell` is what's called.
- `impl` ≠ done-done: those bodies are real code that builds, but have **not** had the 1:1-against-asm fidelity pass. Elevating them is the remaining work.
- **Exercised by tests:** the AI cast/summon path runs live in the combat characterization tests — combat scenario 1 has the AI defender summon a Fire Elemental mid-battle (pinned in `tests/assert_combat_s1.txt`), so `Combat_Cast_Spell` → selection/scoring → `AITP_Combat_Spell` and the summon resolution are covered by a deterministic baseline. See [__TODO-Combat.md § Test coverage](__TODO-Combat.md).
