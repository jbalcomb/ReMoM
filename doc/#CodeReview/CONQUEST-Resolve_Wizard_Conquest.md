CONQUEST-Resolve_Wizard_Conquest.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\WIZ_Conquer.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr093\WIZ_Conquer.c     (Gemini translation of the .asm — second opinion, NOT ground truth)

Change_City_Ownership()
    |-> j_Resolve_Wizard_Conquest()
        |-> Resolve_Wizard_Conquest()

---

# `Resolve_Wizard_Conquest` — Review / Walkthrough

Handles one wizard conquering another: the banishment-vs-defeat gate, transfer of the conquered city, fortress deactivation and fame, removal of the loser's city enchantments / garrisoned units / nodes / overland enchantments, gold+mana looting, diplomacy reset, the conquest animation, and the human-defeat / last-AI-defeat endgame.

| Function | Location | asm | Role |
|---|---|---|---|
| `Resolve_Wizard_Conquest` | [CONQUEST.c:191-347](../../MoM/src/CONQUEST.c#L191-L347) | `WIZ_Conquer.asm` (WZD 093p01, `ovr093:78BA6`) | The full wizard-conquest resolution; both terminal branches (human loss, last-AI win) now route through the flag-based endgame instead of the DOS `GAME_EXE_Swap`. |

> **Status: faithful to `WIZ_Conquer.asm` 1:1, with two accepted `EOG_HACK` deviations (D1, D2).** Every game-logic phase — city-count/banishment gate, early return, ownership transfer, fortress/fame, per-city enchant clear + garrison handling + neutralization, node reset, gold+mana loot, unit dismissal, overland-enchant clear, the `resign`-guarded conquest animation, diplomacy zeroing + `Change_Relations`, `Conquest_Spells`, and both endgame branches — matches the disassembly arg-for-arg (cross-checked against the Gemini `.c`). The only departures from the asm are the two deliberate endgame-return substitutions for the DOS `execl`, documented in [PRD-Endgame-Return-To-Menu-Screen-State.md](../#AI_Plans/PRD-Endgame-Return-To-Menu-Screen-State.md).

## Signature & frame

```c
void Resolve_Wizard_Conquest(int16_t city_owner_idx, int16_t player_idx, int16_t city_idx)
```

| Production | asm slot | Notes |
|---|---|---|
| `city_owner_idx` | `Target` (bp+6) → `di` | the defeated wizard |
| `player_idx` | `Conqueror` (bp+8) | the conqueror |
| `city_idx` | `City` (bp+0Ah) | conquered city, or `ST_UNDEFINED` |
| `troops[MAX_STACK]` | `City_Garrison` (-20h) | 9-word garrison scratch |
| `resign` | `Forfeited` (-0Eh) | `Banish_Wizard` result (asm `WIZ_Banishment__STUB`) |
| `city_count` | `City_Count` (-0Ch) | capped at 2 |
| `city_enchantments` | `City_Enchants@` (dword -0Ah) | far ptr into `_CITIES[i].enchantments` |
| `overland_enchantments` | `Globals@` (-6) | ptr into `_players[di].Globals` |
| `troop_count` | `Count` (-4) | `Army_At_City` out-param |
| `itr_*` | `Loop_Var` (-2) / `si` | asm reuses `Loop_Var` and `si` across the loops |

## Accepted deviations (`EOG_HACK` — platform substitution for DOS `execl`)

The original ends each terminal branch with `GAME_EXE_Swap(cnst_MAGIC_EXE_File, cnst_MAGICEXE_arg0, cnst_ZeroString_7, cnst_ZeroString_7)` (asm lines 457 and 486) — an `execl` to MAGIC.EXE that never returns, discarding the stack. ReMoM can't relaunch an EXE, so the endgame is state-based (`magic_master_idx = Get_Winner()` set at the capture sites, reacted-to at the `Main_Screen` / `Next_Turn_Proc` boundaries). See the PRD.

- **D1 — re-entry GUARD** ([CONQUEST.c:206](../../MoM/src/CONQUEST.c#L206)) — `if(magic_master_idx != ST_UNDEFINED) { return; }`. **No asm counterpart** (the asm falls straight into the city-count loop at `loc_78BAB`). This is the endgame "no second presentation" guard: once a winner exists, a later capture the same turn (e.g. an `Event_Twiddle` rebellion after an AI already won) must not replay `Lose_/Win_Animation`. Required — the AI-conquest-then-rebellion path double-presents without it.
- **D2 — `Respawn` commented out** ([CONQUEST.c:335](../../MoM/src/CONQUEST.c#L335) human loss, [CONQUEST.c:344](../../MoM/src/CONQUEST.c#L344) last-AI win) — the asm `GAME_EXE_Swap` is preserved in source as `/* EOG_HACK */ // DONT Respawn(magic_exe_file__ovr093, str_JENNY__ovr093, str_empty_string__ovr093, str_empty_string__ovr093);`. The presentation calls above each (`Lose_/Win_Animation`, `End_Of_Game_Score`, `s01p16_empty_function`) stay and remain faithful; only the process-swap is replaced by returning and letting the boundary route to the menu.

## Phase map (asm ↔ production)

| Phase | asm | production | fidelity |
|---|---|---|---|
| **D1 endgame re-entry guard** | *(none)* | [206](../../MoM/src/CONQUEST.c#L206) | `EOG_HACK` (D1) |
| Count `city_owner_idx`'s cities (cap 2) | `loc_78BAB`-`loc_78BDA` | [207-213](../../MoM/src/CONQUEST.c#L207-L213) | faithful |
| `resign = ST_FALSE`; if `> 1` city → `Banish_Wizard(city_owner_idx, player_idx)` | `loc_78BDA`-`loc_78BF3` | [214-218](../../MoM/src/CONQUEST.c#L214-L218) | faithful |
| Early return: `city_count >= 2 && resign < ST_TRUE` | `loc_78BF3`-`@@Done` | [219-226](../../MoM/src/CONQUEST.c#L219-L226) | faithful |
| Transfer conquered city (`city_idx != -1`) | `loc_78C02`-`loc_78C1D` | [227-230](../../MoM/src/CONQUEST.c#L227-L230) | faithful |
| Fortress inactive + `+5` fame | `loc_78C1D`-`loc_78C38` | [231-232](../../MoM/src/CONQUEST.c#L231-L232) | faithful |
| Per-city: clear loser's city enchantments; if loser's city → `Army_At_City`, dismiss fantastic/heroes, normals → neutral; then turn the city neutral (unless human) | `loc_78C42`-`loc_78DB3` | [238-280](../../MoM/src/CONQUEST.c#L238-L280) | faithful |
| Reset loser's nodes (owner `ST_UNDEFINED`, flags `NF_NONE`) | `loc_78DC0`-`loc_78DFB` | [282-289](../../MoM/src/CONQUEST.c#L282-L289) | faithful |
| Loot: gold/2, mana/2 to conqueror | `loc_78DFB`-`loc_78E3E` | [291-293](../../MoM/src/CONQUEST.c#L291-L293) | faithful |
| Dismiss loser's non-garrison units | `loc_78E3E`-`loc_78E60` | [295-301](../../MoM/src/CONQUEST.c#L295-L301) | faithful |
| Clear loser's overland enchantments | `loc_78E60`-`loc_78E80` | [303-307](../../MoM/src/CONQUEST.c#L303-L307) | faithful |
| Conquest animation, guarded by `resign != ST_TRUE` (`Conquest_Animation`, asm `WIZ_Conquest__WIP`) | `loc_78E80`-`loc_78E96` | [308-311](../../MoM/src/CONQUEST.c#L308-L311) | faithful |
| Diplomacy: zero both directions, `Change_Relations(-40, player_idx, itr_players, 7, 0, 0)` | `loc_78E9A`-`loc_78EFE` | [313-328](../../MoM/src/CONQUEST.c#L313-L328) | faithful |
| `Conquest_Spells(player_idx, city_owner_idx)` (asm `WIZ_ConquestSpells`) | `loc_78EFE` tail | [329](../../MoM/src/CONQUEST.c#L329) | faithful |
| Human defeated → `Lose_Animation` + `End_Of_Game_Score` + `s01p16_empty_function` + ~~`Respawn`~~ | `cmp di,_human_player_idx` / `GAME_LimboFallAnim` / `GAME_OVER` / `GAME_EXE_Swap` | [330-336](../../MoM/src/CONQUEST.c#L330-L336) | faithful + **D2** |
| Else `CP_Is_Dead()` (asm `GAME_IsWon__STUB`) → `Win_Animation` + `End_Of_Game_Score` + `s01p16_empty_function` + ~~`Respawn`~~ | `loc_78F40`-`@@Done` / `GAME_PlayVictoryAnim` / `GAME_OVER` / `GAME_EXE_Swap` | [337-346](../../MoM/src/CONQUEST.c#L337-L346) | faithful + **D2** |

## Verified faithful (spot notes)

- **`city_count < 2` cap** ([207](../../MoM/src/CONQUEST.c#L207)) matches asm `cmp City_Count, 2 / jl` — the OG's own early-out, same idiom as `CP_Is_Dead`.
- **Push order** for `Banish_Wizard`, `Army_At_City`, `Kill_Unit(…, kt_Dismissed)`, `Player_Add_Gold`/`_Mana`, `Change_Relations`, and `Conquest_Spells` each checked against the asm (cdecl, right-to-left) and match. Gold/mana use the signed `cwd / sub / sar 1` halve (`/ 2`).
- **Immediates:** `kt_Dismissed` = 1, `NEUTRAL_PLAYER_IDX` = 5, `ST_UNDEFINED` = 0FFh/−1, `NUM_CITY_ENCHANTMENTS` = 0x1A (26), `NUM_NODES` = 0x1E (30), `NUM_OVERLAND_ENCHANTMENTS` = 0x18 (24) all match the asm.
- **Renames** (asm → production, all verified against `WIZ_Conquer.asm`): `WIZ_Banishment__STUB`→`Banish_Wizard`, `WIZ_Conquest__WIP`→`Conquest_Animation`, `GAME_LimboFallAnim`→`Lose_Animation`, `GAME_PlayVictoryAnim`→`Win_Animation`, `GAME_OVER`→`End_Of_Game_Score`, `GAME_IsWon__STUB`→`CP_Is_Dead`, `WIZ_ConquestSpells`→`Conquest_Spells`, `GAME_EXE_Swap`→`Respawn` (commented, D2). The local `resign` reconstructs asm `Forfeited`.
- **Preserved OG observation:** the garrison inner test's "conflicting condition … will always jump" comment ([258](../../MoM/src/CONQUEST.c#L258)) is a faithful note about the OG; the branch structure matches `loc_78CC3`-`loc_78D7A`.

## Related references

- `…\ovr093\WIZ_Conquer.asm` — IDA Pro 5.5 disassembly (**the authority**); Gemini `.c` is a second opinion (agrees phase-for-phase).
- [PRD-Endgame-Return-To-Menu-Screen-State.md](../#AI_Plans/PRD-Endgame-Return-To-Menu-Screen-State.md) — the flag-based endgame that D1/D2 are part of (this function is one of the four SET/GUARD sites).
- [`CP_Is_Dead`](CONQUEST-CP_Is_Dead.md) — last-AI-standing check at [339](../../MoM/src/CONQUEST.c#L339) (asm `GAME_IsWon__STUB`).
- [`Banish_Wizard`](CONQUEST-Banish_Wizard.md) — banish-vs-defeat decision at [217](../../MoM/src/CONQUEST.c#L217).
- [`Conquest_Animation`](CONQUEST-Conquest_Animation.md) — the guarded conquest scene at [310](../../MoM/src/CONQUEST.c#L310).
- [`Lose_Animation`](CONQUEST-Lose_Animation.md) / [`Win_Animation`](CONQUEST-Win_Animation.md) / [`End_Of_Game_Score`](CONQUEST-End_Of_Game_Score.md) — the endgame presentations at [332-342](../../MoM/src/CONQUEST.c#L332-L342).
