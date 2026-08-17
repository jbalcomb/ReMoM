__TODO-Combat.md

---

Combat-Miscellaneous.md

## Fixing the splits

1.  four NOT: entries point at reviews that are DONE-DONE and don't contain them
[x] done-done Turn_Off_Auto_Combat() in Combat-Combat_Screen.md
[x] done-done Combat_Screen_Draw() in Combat-Combat_Screen.md
[x] done-done Update_Combat_Enchantments_Icon_And_Help() in Combat-Combat_Screen.md
[x] done-done Combat_Compose_Spellbook_Background() in Combat-Combat_Spellbook_Screen.md
[x] done-done Combat_Spellbook_Mana_Adder_Screen() in Combat-Combat_Spellbook_Screen.md

2.  Four moves
[ ] inlude Spell_Resistance_Modifier() (o112p06) in Combat-Cast_Spell_On_Battle_Unit.md
[x] include Combat_Spell_Dispel_Attempt() (o113p13) in Combat-Combat_Spell_Dispel.md
[ ] include Combat_Cast_Spell_Message (o113p14) in Combat-Spell_Cast.md
[ ] move Combat_Cast_Spell (s103p14) to Combat-Spell_Cast.md
[ ] move Combat_Cast_Spell_With_Caster (s103p14) to Combat-Spell_Cast.md

3.  AITP_Combat_Spell
...stays in its own doc, for now
...that may change when we do Combat-Spell_Cast_AI_Target.md
...Claude says - Combat-Spell_Cast_AI_Target.md should carry it as the already-adjudicated dispatcher with a pointer to that doc. The eleven callees under it are the real work — that's the session.

---

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
| Setup / alloc / resources | `Allocate_Combat_Base_Blocks`, `Combat_Screen_Load_Resources`, `Item_Powers_To_Attack_Attributes`, `CMB_Units_Init__WIP` | **WIP** |
| Map / terrain / spawn | `Generate_Combat_Map__WIP`, `Set_Terrain_Tile_Types`, `Build_Battlefield`, `CMB_SpawnStructures/Figure/DarkWall/FireWall/StoneWall__WIP` | **WIP** |
| Drawing / rendering | `Combat_Compose_Background`, `CMB_ComposeBookBG__WIP`, `Combat_Screen_Map_Draw(_Entities)__WIP`, `Combat_Grid_Entities/Entity_Create__WIP`, `Combat_Figure_Effect`, `CMB_CE_Refresh__WIP` | **WIP** |
| Unit action / attack / move | `Battle_Unit_Action/Attack__WIP`, `BU_AttackTarget/ProcessAttack__WIP`, `BU_Apply_Battlefield_Effects__WIP`, `BU_CheckFlight/GetCombatEffect/HasSpellAbility/MeleeWallCheck/MoveConfused/SetBaseAnims/SetCityMovement/SetVisibility__WIP`, `Move_Battle_Unit__WIP`; done: `BU_ApplyDamage`, `BU_IsVisible`, `BU_SortSlowestFirst` | mostly **WIP** |
| Spellcasting (shared/human) | `Combat_Cast_Spell` (**done**), `AITP_Combat_Spell` (**done**), `Combat_Spell_Dispel_Attempt`/`Combat_Spell_Counter_Message` (impl), `Combat_Spell_Target_Screen__WIP`, `Do_Legal_Spell_Check__WIP` | **done** for the cast core; targeting screen **WIP** |
| Combat spellbook | `Combat_Spellbook_Build`, `CmbBook_Compose/Draw__WIP` (`Spellbook.c`) | **WIP** |
| **AI Combat** | driver + per-unit action chain — see [__TODO-Combat-AI.md](__TODO-Combat-AI.md) | mostly **impl**, review-pending |
| **AI Combat Spell** | selection/scoring + `AITP_*` pickers — see [__TODO-Combat-AI-Spell.md](__TODO-Combat-AI-Spell.md) | **done / impl** (review-only) |
| End / capture / strategic | `End_Of_Combat__WIP`, `STK_CaptureCity__WIP`, `STK_ComposeFleeLost__STUB`, `Strategic_Combat__WIP`, `Lair_Combat__WIP` (`Lair.c`) | **WIP** |

## Test coverage

Combat now has deterministic characterization tests (seed 12345); reconstruction work can be regression-checked against pinned baselines. See [BRA-Combat-Testing.md](#AI_Plans/BRA-Combat-Testing.md) / [PRD-Combat-Testing.md](#AI_Plans/PRD-Combat-Testing.md) for the channel model.

| Channel | Path | Runner | CTest |
|---|---|---|---|
| Strategic (auto-resolve) | `Strategic_Combat__WIP`, headless | `HeMoM --combat` | `HeMoM_Combat_Strategic_*` (automated) |
| Tactical (real battle screen) | `Combat_Screen__WIP`, windowed, scripted `.hms` | `ReMoM --continue --scenario` | `Combat_S1_*`, `Combat_S2_*` — automated check vs committed baseline; windowed replay gated by `ENABLE_WINDOWED_COMBAT_TESTS` |

- Scenario 1: open-field stack fight (Halberdiers vs Stag Beetles). Scenario 2: multi-turn city assault (razes Fa-rul). Assertions in `tests/assert_combat_s{1,2}.txt`; fixtures/baselines in `assets/combat_s{1,2}.*`.
- **Tactical combat does not run headless** — the battle screen is entangled with the rendering/animation layer; the windowed path is authoritative. See the BRA doc for the substrate rationale.

# To-Do List

- [ ] **Battle host & turn flow** — reconstruct `Combat__WIP` / `Combat_Screen__WIP`; done-done `Combat_Next_Turn` (impl).
- [ ] **Map build & spawn** — `Generate_Combat_Map__WIP`, `CMB_TileGen/Terrain_Init/Units_Init__WIP`, the `CMB_Spawn*__WIP` set.
- [ ] **Unit action / attack / movement** — the `Battle_Unit_*` / `BU_*__WIP` set and `Move_Battle_Unit__WIP` (the largest remaining cluster).
- [ ] **Draw layer** — `CMB_Compose*__WIP`, `Combat_Screen_Map_Draw*__WIP`, grid-entity + figure-effect helpers.
- [ ] **Water / shore combat rendering** — combat on a coastal/ocean square (`cts_Water`) is unreconstructed: `Load_Combat_Terrain_Pictures` early-returns for `cts_Water` (no tileset loaded), and `Combat_Compose_Background` then `assert`s on ocean battlefield tiles (`btt_Ocean` 48–51 vs its `< 48` guard). Surfaced when a tactical combat lands on a shore tile. OG supports coastal combat, so this is a reconstruction gap, not "unsupported."
- [ ] **Human spellcasting UI** — `Combat_Spell_Target_Screen__WIP`, `Do_Legal_Spell_Check__WIP`, `Combat_Spellbook_Build`.
- [ ] **Resolution** — `End_Of_Combat__WIP`, `STK_CaptureCity__WIP`, `Strategic_Combat__WIP`, `Lair_Combat__WIP`.
- [ ] **AI Combat** — finish the review/reconstruction tracked in [__TODO-Combat-AI.md](__TODO-Combat-AI.md).
- [ ] **AI Combat Spell** — finish the review tracked in [__TODO-Combat-AI-Spell.md](__TODO-Combat-AI-Spell.md).

## Notes

- **Rename applied:** `CMB_ProgressTurnFlow__WIP` → `Combat_Next_Turn` ([Combat.c:4528](../MoM/src/Combat.c#L4528)). (AI-layer renames — `AI_CMB_PlayTurn__WIP` → `Auto_Cast_Spell_And_Do_Combat_Turn`, `AI_BU_SelectAction__WIP` → `Choose_Target_And_Action` — are recorded in [__TODO-Combat-AI.md](__TODO-Combat-AI.md).)
- Authoritative per-function checkboxes remain in [stub_wip_todo.md](#TODO/stub_wip_todo.md); these three `__TODO-Combat*` docs are the navigable overview layered on top of it. When a function is done-done, check it there **and** update the relevant subsystem/table row here.
- The AI spell path (bottom of the tree) is intentionally the most complete — combat spellcasting was reconstructed first so the CP can act; the surrounding battlefield mechanics are the larger remaining body of work.
