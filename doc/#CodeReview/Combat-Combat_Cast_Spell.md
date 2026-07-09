Combat-Combat_Cast_Spell.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Cast_Spell__WIP.asm         (ground truth — IDA Pro 5.5 disassembly; OG name keeps __WIP)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Cast_Spell__WIP__GEMINI.c   (Gemini translation of the .asm — second opinion, NOT ground truth)

Combat_Screen__WIP()                    Combat
    |-> Combat_Cast_Spell()             Combat
AI_BU_ProcessAction()                   CMBTAI
    |-> Combat_Cast_Spell()             Combat
Auto_Cast_Spell_And_Do_Combat_Turn()    Combat  // WZD o98p04
    |-> Combat_Cast_Spell()             Combat

---

# `Combat_Cast_Spell` — Review / Walkthrough

| Function | Location | Role |
|---|---|---|
| `Combat_Cast_Spell` | [Combat.c:13176-13806](../../MoM/src/Combat.c#L13176-L13806) | drake178 `G_CMB_CastSpell`. Resolves combat spellcasting by one entity (a battle unit's spell-like ability, a human at the combat spellbook, or an AI/CP wizard): selects the spell, computes the effective mana cost, resolves opposing Counter Magic / node-realm counters, targets, casts, and pays. Returns `0` = nothing cast, `1` = cancelled during targeting, `2` = resolved (or countered). |

> **Status: DONE-DONE (2026-07-07) — faithful to `Combat_Cast_Spell__WIP.asm` phase-for-phase; whole tree builds clean.** The human-interactive path, the AI/CP selection branch (`loc_8D138`), all three cast-payment blocks, both counter-magic paths, and the shared target/cast/cleanup are reconstructed and match the disassembly. The reconstruction preserves a long list of OG casting-cost bugs as inline `; BUG:` comments (kept faithfully — see [Preserved OG bugs](#preserved-og-bugs-b--faithful-to-the-asm)). The two counter-magic subfunctions `Combat_Spell_Dispel_Attempt` and `Combat_Spell_Counter_Message` (`Spells133.c`) are now **implemented**; the remaining `__WIP` spellbook/screen helpers it calls (`Combat_Spellbook_Build__WIP`, the `CMB_*__WIP` compose calls, `Combat_Spell_Target_Screen__WIP`) are still WIP and tracked separately. The `.asm` is the authority; the Gemini `.c` is a second opinion. The function's own header comments flag "not sure about the multiple returns", "handful of variable reuses (don't make distinct…)", and "cast_status seems mixed up" — those are design notes, not defects, and are left untouched.

## Signature & frame

```c
int16_t Combat_Cast_Spell(int16_t caster_idx, int16_t wx, int16_t wy, int16_t wp)
```

The asm frame declares **only** `caster_idx` (bp+6); `wx`/`wy`/`wp` are pushed by callers (`…(caster, _combat_wx, _combat_wy, _combat_wp)`) but never read in the body. Production faithfully keeps them in the signature and likewise never uses them. Locals map to the asm's bp-relative slots:

| Production local | asm slot | Notes |
|---|---|---|
| `cast_status` | `return_value` (-18h) | the `0/1/2` return |
| `Spell_Like_Ability` | (-1Ah) | set when a unit casts via a granted ability |
| `spell_idx` | `si` | selected spell |
| `IDK_mana` | `di` | working mana / cost accumulator |
| `battle_unit_mana`, `Opponent_Index`, `Base_Cost`, `Extra_Mana`, `player_idx`, `Target_Y`, `Target_X`, `Target`, `Selected_Spell`, `Effective_Cost`, `Overland_Cast_Save`, `Can_Cast` | -1Ch … -2 | same order as the asm frame |

`caster_idx` encoding: `< CASTER_IDX_BASE` (20) → a battle-unit index; `>= CASTER_IDX_BASE` → wizard `caster_idx - CASTER_IDX_BASE`; `== CASTER_IDX_BASE + NEUTRAL_PLAYER_IDX` → early `return cast_status`.

## Phase map (asm `loc_` ↔ production)

| Phase | asm | production | fidelity |
|---|---|---|---|
| Setup: `_combat_caster_idx`, derive `player_idx` / `battle_unit_mana` | 26-54 | [13178-13205](../../MoM/src/Combat.c#L13178-L13205) | faithful |
| Neutral caster early-out | `loc_8CFC7` | [13206-13209](../../MoM/src/Combat.c#L13206-L13209) | faithful |
| Human-interactive: unit ability spells (Doom Bolt / Fireball / Web / Healing) + Summon Demon | `loc_8CFE0`-`loc_8D0A6` | [13221-13257](../../MoM/src/Combat.c#L13221-L13257) | faithful |
| Human spellbook select loop (`Combat_Spellbook_Build/Screen`, `Do_Legal_Spell_Check`) — nested inside the human branch | `loc_8D0AF`-`loc_8D135` | [13263-13301](../../MoM/src/Combat.c#L13263-L13301) | faithful |
| AI/CP select: `AI_SetCombatRealms`, hero-item embed spell, `AI_SelectCmbtSpell`, target/immobile-counter | `loc_8D138`-`loc_8D1C1` | [13303-13328](../../MoM/src/Combat.c#L13303-L13328) | faithful |
| Effective-cost computation (human popup vs AI skill/reserve budget) | `loc_8D204`-`loc_8D49E` | [13332-13451](../../MoM/src/Combat.c#L13332-L13451) | faithful (carries the OG cost bugs) |
| Counter Magic dispel (opponent's Counter Magic enchantment; `Combat_Spell_Dispel_Attempt` / `Combat_Spell_Counter_Message`) | `loc_8D4CE`-`loc_8D6EC` | [13458-13546](../../MoM/src/Combat.c#L13458-L13546) | faithful |
| Node-realm counter (Sorcery/Chaos/Nature node vs spell realm) | `loc_8D6F4`-`loc_8D986` | [13547-13638](../../MoM/src/Combat.c#L13547-L13638) | faithful |
| Target + effect + pay (`Combat_Spell_Target_Screen` / `AITP_Combat_Spell` → `Cast_Spell_On_Battle_Unit`) | `@@Target_And_Effect`-`loc_8DC4D` | [13649-13744](../../MoM/src/Combat.c#L13649-L13744) | faithful |
| Post-cast unit cleanup (write back `mana`, zero `ranged` if depleted) | `loc_8DC4D`-`loc_8DCA9` | [13750-13763](../../MoM/src/Combat.c#L13750-L13763) | faithful |

## Selection dispatch (the part most recently reconstructed)

The human/AI split matches `loc_8CFCD`/`loc_8CFD6`: the interactive-human path (`player_idx == HUMAN_PLAYER_IDX && _auto_combat_flag == ST_FALSE`, [13210](../../MoM/src/Combat.c#L13210)) runs the unit-ability checks and, if still `spl_NONE`, the interactive spellbook — the spellbook is **nested inside the human branch** ([13263](../../MoM/src/Combat.c#L13263)) so an AI caster never reaches it. The `else` branch ([13303](../../MoM/src/Combat.c#L13303)) is the AI/CP path: `AI_SetCombatRealms()`, then the hero-item `embed_spell_idx` when a charged item is in use ([13307-13312](../../MoM/src/Combat.c#L13307-L13312)) else `AI_SelectCmbtSpell(caster_idx)` ([13315](../../MoM/src/Combat.c#L13315)), then `target_battle_unit_idx = caster` + non-human `_ai_immobile_counter = ST_UNDEFINED` ([13317-13327](../../MoM/src/Combat.c#L13317-L13327)). The `spell_idx == 0` convergence the asm reaches via `loc_8D1FD → @@Target_And_Effect` is modelled structurally by the `if(spell_idx != spl_NONE)` cost/counter guard ([13332](../../MoM/src/Combat.c#L13332)) and the `if(spell_idx > spl_NONE)` target guard ([13649](../../MoM/src/Combat.c#L13649)) — no `goto` needed.

All three cost-payment blocks correctly index `_players[(caster_idx - CASTER_IDX_BASE)]` — counter-magic [13491](../../MoM/src/Combat.c#L13491)/[13492](../../MoM/src/Combat.c#L13492), node-counter [13589](../../MoM/src/Combat.c#L13589)/[13590](../../MoM/src/Combat.c#L13590), actual cast [13698](../../MoM/src/Combat.c#L13698)/[13699](../../MoM/src/Combat.c#L13699) — matching the asm's `_players.X-(14h*4c8h)[caster*size]` addressing.

## Faithful oddity — `if(Target != 999)`

The cast is guarded by `if(Target != 999)` ([13687](../../MoM/src/Combat.c#L13687)), matching the asm `cmp [bp+Target], 999 / jnz loc_8DA78`. `Target` is only ever set to `99`, a battle-unit index, or the return of `Combat_Spell_Target_Screen__WIP` — so `999` is presumably that screen's cancel sentinel (the `1` = "cancelled during targeting" return). Reproduced verbatim; no change. (The header comment's "atleast one BUGBUG with Target" refers here; left as-is per the faithful-to-dasm rule.)

## Preserved OG bugs (B — faithful to the asm)

The reconstruction keeps the OG casting-cost / ability defects and annotates each inline; all match the asm and must **not** be "fixed":

| # | Site | Preserved defect |
|---|---|---|
| B1 | [13345](../../MoM/src/Combat.c#L13345) | counters use the effective (reduced) cost |
| B2 | [13350](../../MoM/src/Combat.c#L13350), [13372](../../MoM/src/Combat.c#L13372), [13412](../../MoM/src/Combat.c#L13412), [13443](../../MoM/src/Combat.c#L13443) | ignores Evil Omens (unit / human / AI / Ice Bolt paths) |
| B3 | [13395](../../MoM/src/Combat.c#L13395), [13397](../../MoM/src/Combat.c#L13397), [13403](../../MoM/src/Combat.c#L13403), [13406](../../MoM/src/Combat.c#L13406) | AI cost path ignores base cost and casting-cost modifiers |
| B4 | [13413](../../MoM/src/Combat.c#L13413) | an infusable item's charge is auto-countered when the user has no mana |
| B5 | [13423](../../MoM/src/Combat.c#L13423) | Banish's random extra mana yields an effective gain of 1/15, not 5 |
| B6 | [13487](../../MoM/src/Combat.c#L13487) | `Target_X` reused as scratch while still needed below (variable-reuse bug) |
| B7 | [13498](../../MoM/src/Combat.c#L13498), [13596](../../MoM/src/Combat.c#L13596), [13705](../../MoM/src/Combat.c#L13705) | item-charge decrement may not be the hero's original owner |
| B8 | [13759](../../MoM/src/Combat.c#L13759) | zeroing `ranged` on mana depletion also removes short-range attacks |

The three near-identical XOR-toggle / item-charge / mana-debit payment blocks (`loc_8D5A9` counter-magic, `loc_8D843` node-counter, `loc_8DADD` actual cast) are the asm's own copy-paste — reproduced faithfully, not refactored.

## Related references

- `…\ovr112\Combat_Cast_Spell__WIP.asm` — IDA Pro 5.5 disassembly (**the authority**; OG name keeps `__WIP`).
- `…\ovr112\Combat_Cast_Spell__WIP__GEMINI.c` — Gemini translation (second opinion).
- [`AITP_Combat_Spell`](../ComputerPlayer/Combat-AITP_Combat_Spell.md) — the AI target picker invoked at [13679](../../MoM/src/Combat.c#L13679) for the non-human targeting path.
- [`Cast_Spell_On_Battle_Unit`](../../MoM/src/Combat.c#L12382) — applies the chosen spell ([13693](../../MoM/src/Combat.c#L13693)).
- `AI_SelectCmbtSpell` ([24758](../../MoM/src/Combat.c#L24758)), `AI_SetCombatRealms` — the AI-branch pickers, now wired into the `else` selection path.
- `Combat_Spell_Dispel_Attempt` ([Spells133.c:653](../../MoM/src/Spells133.c#L653), decl `Spells133.h:42`) — Counter-Magic / node dispel roll; **implemented** (was `WIZ_DispelAttempt__STUB`); called at [13478](../../MoM/src/Combat.c#L13478) and [13578](../../MoM/src/Combat.c#L13578).
- `Combat_Spell_Counter_Message` ([Spells133.c:532](../../MoM/src/Spells133.c#L532), decl `Spells133.h:39`) — "Counter Magic" popup; **implemented** (was `CMB_CounterMessage__STUB`); called at [13484](../../MoM/src/Combat.c#L13484) and [13585](../../MoM/src/Combat.c#L13585).
