__TODO-Combat.md

# 'Combat' - Overview

Scope: the whole **tactical (battlefield) combat** system — entering combat, turn flow, map/terrain build, unit actions/attacks/movement, spellcasting, drawing, and resolution. The full per-function checklist lives in [stub_wip_todo.md](#TODO/stub_wip_todo.md) under `## Combat` (plus `## Spellbook`, `## Lair`); this doc is the **subsystem map** and points to two sub-areas:

- [AI Combat](__TODO-Combat-AI.md) — the computer player's combat turn.
- [AI Combat Spell](__TODO-Combat-AI-Spell.md) — the computer player's combat spellcasting (functionally complete; review-only).

**Bottom line:** most of tactical combat is still `__WIP` — the AI spell path is the furthest along (done / impl), while the screen, map-build, unit-action, and draw subsystems are largely unreconstructed.

Top-level flow:

```
Combat__WIP / Combat_Screen__WIP          ← enter & host the battle
  └ Combat_Next_Turn (Combat.c:4528)      ← turn flow (was CMB_ProgressTurnFlow__WIP)
      └ Auto_Cast_Spell_And_Do_Combat_Turn → AI Combat  → AI Combat Spell
  └ (human input path)                    → Battle_Unit_Action / Move_Battle_Unit / spellbook
  └ End_Of_Combat__WIP                     ← resolve, capture, cleanup
```

## Subsystem status

Legend: **[done]** reconstructed/reviewed · **[impl]** substantive, not yet done-done · **[WIP]** stub/incomplete.

| Subsystem | Representative functions | Status |
|---|---|---|
| Entry & turn flow | `Combat__WIP`, `Combat_Screen__WIP`, `Combat_Next_Turn` (impl), `CMB_PrepareTurn__WIP`, `Check_For_Winner` (done), `End_Of_Combat__WIP` | mostly **WIP** |
| Setup / alloc / resources | `CMB_BaseAllocs__WIP`, `CMB_LoadResources__WIP`, `CMB_SetNearAllocs__WIP`, `CMB_Units_Init__WIP` | **WIP** |
| Map / terrain / spawn | `Generate_Combat_Map__WIP`, `CMB_TileGen__WIP`, `CMB_Terrain_Init__WIP`, `CMB_SpawnStructures/Figure/DarkWall/FireWall/StoneWall__WIP` | **WIP** |
| Drawing / rendering | `CMB_ComposeBackgrnd__WIP`, `CMB_ComposeBookBG__WIP`, `Combat_Screen_Map_Draw(_Entities)__WIP`, `Combat_Grid_Entities/Entity_Create__WIP`, `Combat_Figure_Effect__WIP`, `CMB_CE_Refresh__WIP` | **WIP** |
| Unit action / attack / move | `Battle_Unit_Action/Attack__WIP`, `BU_AttackTarget/ProcessAttack__WIP`, `BU_Apply_Battlefield_Effects__WIP`, `BU_CheckFlight/GetCombatEffect/HasSpellAbility/MeleeWallCheck/MoveConfused/SetBaseAnims/SetCityMovement/SetVisibility__WIP`, `Move_Battle_Unit__WIP`; done: `BU_ApplyDamage`, `BU_IsVisible`, `BU_SortSlowestFirst` | mostly **WIP** |
| Spellcasting (shared/human) | `Combat_Cast_Spell` (**done**), `AITP_Combat_Spell` (**done**), `Combat_Spell_Dispel_Attempt`/`Combat_Spell_Counter_Message` (impl), `Combat_Spell_Target_Screen__WIP`, `Do_Legal_Spell_Check__WIP` | **done** for the cast core; targeting screen **WIP** |
| Combat spellbook | `Combat_Spellbook_Build__WIP`, `CmbBook_Compose/Draw__WIP` (`Spellbook.c`) | **WIP** |
| **AI Combat** | driver + per-unit action chain — see [__TODO-Combat-AI.md](__TODO-Combat-AI.md) | mostly **impl**, review-pending |
| **AI Combat Spell** | selection/scoring + `AITP_*` pickers — see [__TODO-Combat-AI-Spell.md](__TODO-Combat-AI-Spell.md) | **done / impl** (review-only) |
| End / capture / strategic | `End_Of_Combat__WIP`, `STK_CaptureCity__WIP`, `STK_ComposeFleeLost__STUB`, `Strategic_Combat__WIP`, `Lair_Combat__WIP` (`Lair.c`) | **WIP** |

# To-Do List

- [ ] **Battle host & turn flow** — reconstruct `Combat__WIP` / `Combat_Screen__WIP`; done-done `Combat_Next_Turn` (impl).
- [ ] **Map build & spawn** — `Generate_Combat_Map__WIP`, `CMB_TileGen/Terrain_Init/Units_Init__WIP`, the `CMB_Spawn*__WIP` set.
- [ ] **Unit action / attack / movement** — the `Battle_Unit_*` / `BU_*__WIP` set and `Move_Battle_Unit__WIP` (the largest remaining cluster).
- [ ] **Draw layer** — `CMB_Compose*__WIP`, `Combat_Screen_Map_Draw*__WIP`, grid-entity + figure-effect helpers.
- [ ] **Human spellcasting UI** — `Combat_Spell_Target_Screen__WIP`, `Do_Legal_Spell_Check__WIP`, `Combat_Spellbook_Build__WIP`.
- [ ] **Resolution** — `End_Of_Combat__WIP`, `STK_CaptureCity__WIP`, `Strategic_Combat__WIP`, `Lair_Combat__WIP`.
- [ ] **AI Combat** — finish the review/reconstruction tracked in [__TODO-Combat-AI.md](__TODO-Combat-AI.md).
- [ ] **AI Combat Spell** — finish the review tracked in [__TODO-Combat-AI-Spell.md](__TODO-Combat-AI-Spell.md).

## Notes

- **Rename applied:** `CMB_ProgressTurnFlow__WIP` → `Combat_Next_Turn` ([Combat.c:4528](../MoM/src/Combat.c#L4528)). (AI-layer renames — `AI_CMB_PlayTurn__WIP` → `Auto_Cast_Spell_And_Do_Combat_Turn`, `AI_BU_SelectAction__WIP` → `Choose_Target_And_Action` — are recorded in [__TODO-Combat-AI.md](__TODO-Combat-AI.md).)
- Authoritative per-function checkboxes remain in [stub_wip_todo.md](#TODO/stub_wip_todo.md); these three `__TODO-Combat*` docs are the navigable overview layered on top of it. When a function is done-done, check it there **and** update the relevant subsystem/table row here.
- The AI spell path (bottom of the tree) is intentionally the most complete — combat spellcasting was reconstructed first so the CP can act; the surrounding battlefield mechanics are the larger remaining body of work.
