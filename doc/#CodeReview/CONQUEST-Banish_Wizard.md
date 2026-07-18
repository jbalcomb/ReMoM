CONQUEST-Banish_Wizard.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\WIZ_Banishment__STUB.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\WIZ_Banishment__STUB.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()
            |-> Banish_Wizard()
                |-> Conquest_Animation()
                |-> Return_Animation()
                |-> Conquest_Spells()

---

# `Banish_Wizard` — Review / Walkthrough

| Function | Location | asm | Role |
|---|---|---|---|
| `Banish_Wizard` | [CONQUEST.c:861-918](../../MoM/src/CONQUEST.c#L861-L918) | `WIZ_Banishment__STUB.asm` (IDA `ovr093`, WZD 093p14) | Called by `Resolve_Wizard_Conquest` when the loser still holds ≥2 cities (not their last). Sets up the loser's Spell-of-Return casting state; if an AI lacks the skill/mana to return it forfeits (`99`). Otherwise it deactivates the fortress, awards `+5` fame, loots half the loser's mana, plays the banishment animation, applies `−20` relations to the other wizards, then either prompts a **human** loser to resign or plays the **AI** loser's Return animation and awards the winner spells. Returns `0` (will return), `1` (human resigned), or `99` (AI can't return). |

> **Status: DONE-DONE (2026-07-18) — faithful to `WIZ_Banishment__STUB.asm` 1:1, with one preserved OG bug (B1); builds clean.** Every phase — the Return-cost setup, the AI-can't-return `99` early-out, fortress/fame/mana-loot, `Conquest_Animation`, the `−20` diplomacy loop, the redraw, and the human-resign vs AI-Return split — matches the disassembly arg-for-arg (cross-checked against the Gemini `.c`).

## Signature & frame

`int16_t Banish_Wizard(int16_t loser_idx, int16_t winner_idx)` ↔ asm `Target` (bp+6, `si`) / `Conqueror` (bp+8, `di`). Caller: `Resolve_Wizard_Conquest` at [221](../../MoM/src/CONQUEST.c#L221) as `Banish_Wizard(city_owner_idx, player_idx)` = `(loser, winner)`.

| Production local | asm slot | Notes |
|---|---|---|
| `loser_idx` | `Target` (bp+6) → `si` | the banished wizard |
| `winner_idx` | `Conqueror` (bp+8) → `di` | the conqueror |
| `Conv_String[50]` | `Conv_String` (-3Ch) | `itoa` buffer for turns-to-return |
| `Unused_SpaceString[4]` | `Unused_SpaceString` (-0Ah) | strcpy'd but never used — faithful dead copy |
| `Est_Turns_To_Return` | `Est_Turns_To_Return` (-6) | `casting_cost_remaining / Nominal_Skill` |
| `Forfeit` | `Forfeit` (-4) | the `0`/`1` return |
| `itr_players` | `Wizard_Index` (-2) | diplomacy loop index |

**Return semantics** — the caller does `forfeited = Banish_Wizard(...); if(city_count >= 2 && forfeited < 1) return;`. So `0` = banished/survives (caller stops — the wizard keeps its other cities and may cast Return); `1` (human resigned) or `99` (AI can't return) = forfeit → caller proceeds to full defeat.

## Verified faithful (phase notes)

- **Return-cost setup** ([868-872](../../MoM/src/CONQUEST.c#L868-L872)) — dead `strcpy(Unused_SpaceString, " ")`; `casting_spell_idx = spl_Spell_Of_Return`; `casting_cost_remaining` and `casting_cost_original` each `= Casting_Cost(loser, spl_Spell_Of_Return, 0)` (asm calls it **twice**, faithfully); `Est_Turns_To_Return = casting_cost_remaining / Nominal_Skill` (asm signed `idiv`).
- **AI-can't-return `99`** ([873-883](../../MoM/src/CONQUEST.c#L873-L883)) — `if(loser != human && (Player_Base_Casting_Skill + Player_Hero_Casting_Skill < 40 || mana_reserve < 1)) return 99` — matches asm `cmp dx,40 / jl` then `cmp mana_reserve,1 / jge`.
- **Fortress + fame + mana loot** ([884-887](../../MoM/src/CONQUEST.c#L884-L887)) — `_FORTRESSES[loser].active = 0`; `_players[winner].fame += 5`; `mana_reserve /= 2`; `Player_Add_Mana(winner, mana_reserve)` (steals the halved amount).
- **Banishment animation** ([888](../../MoM/src/CONQUEST.c#L888)) — `Conquest_Animation(loser, winner)` (asm `WIZ_Conquest__WIP(Target, Conqueror)`).
- **Diplomacy** ([889-895](../../MoM/src/CONQUEST.c#L889-L895)) — for each wizard `!= loser && != winner`: `Change_Relations(-20, winner, itr_players, 7, 0, 0)`.
- **Redraw** ([896-899](../../MoM/src/CONQUEST.c#L896-L899)) — `Forfeit = ST_FALSE`; `Allocate_Reduced_Map / Full_Draw_Main_Screen / Copy_On_To_Off_Page`.
- **Human loser → resign prompt** ([900-907](../../MoM/src/CONQUEST.c#L900-L907)) — `itoa(Est_Turns_To_Return, Conv_String, 10)`; build `SoReturn_Msg1 + <turns> + SoReturn_Msg2`; `Forfeit = Confirmation_Box(GUI_NearMsgString)`.
- **AI loser → Return animation + spell award** ([908-916](../../MoM/src/CONQUEST.c#L908-L916)) — `else { if(loser != NEUTRAL) { Copy_Off_To_Back(); Return_Animation(loser); Conquest_Spells(winner, loser); } }` (asm `GAME_ReturnDialog(si)` + `WIZ_ConquestSpells(di, si)`).
- **Return** `Forfeit` ([917](../../MoM/src/CONQUEST.c#L917)).

## Preserved OG bug (B1 — faithful to the asm, do NOT "fix")

The asm's own header notes: *"BUG: fails to award spells to the AI if they banish the human player."* `Conquest_Spells` (the winner's spell award) sits **only** in the non-human-loser `else` branch ([914](../../MoM/src/CONQUEST.c#L914)). When the loser **is** the human — i.e. an AI banishes the human — control takes the resign-prompt branch ([900-907](../../MoM/src/CONQUEST.c#L900-L907)) and `Conquest_Spells` is never called, so the AI conqueror is denied the human's differential spells. Reproduced faithfully by the `if(loser == human) … else … Conquest_Spells(…)` structure.

## Related references

- `…\ovr093\WIZ_Banishment__STUB.asm` — IDA Pro 5.5 disassembly (**the authority**); the Gemini `.c` is a second opinion (agrees).
- [`Resolve_Wizard_Conquest`](CONQUEST-Resolve_Wizard_Conquest.md) — sole caller; invokes `Banish_Wizard` when `city_count > 1` and uses the return to decide banish (`< 1`) vs full defeat.
- [`Conquest_Animation`](CONQUEST-Conquest_Animation.md) (banishment cut-scene), [`Return_Animation`](CONQUEST-Return_Animation.md) (Spell-of-Return cut-scene), and [`Conquest_Spells`](Spells128-Conquest_Spells.md) (winner's spell award) — the three callees.
