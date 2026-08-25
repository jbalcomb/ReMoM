Combat-Spell_Cast.md

SEEALSO:  Combat-Combat_Spellbook_Screen.md
SEEALSO:  Combat-Combat_Spell_Target_Screen.md
SEEALSO:  Combat-Spell_Cast_AI_Select.md
SEEALSO:  Combat-Spell_Cast_AI_Target.md
SEEALSO:  Combat-Combat_Cast_Apply_Spell_Effect.md
SEEALSO:  Combat-Combat_Spell_Dispel.md
SEEALSO:  Combat-Combat_Summon.md

Do_Legal_Spell_Check__WIP()  ==>  Do_Legal_Spell_Check()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Cast_Spell__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr103\xCombat_Cast_Spell_With_Caster.asm
Combat_Cast_Spell_Error()
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Do_Legal_Spell_Check__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr112\Combat_Casting_Cost_Multiplier.asm
Combat_Cast_Spell_Message()

### Indistinctly connected to 'Combat Spell Cast'
Combat_Cast_Spell_Message() (o113p14)
...nine callers are all spell animation functions in Spells131.c and Spells133.c — Animate_Lightning_Bolt, Wall_Rise, Battle_Unit_Summon_Animation, etc.
...probably not worth a 'Combat Spell Animation' session

AI_Execute_Unit_Action()                   CMBTAI
    |-> Combat_Cast_Spell()             Combat
Combat_Screen()                         Combat
    |-> Combat_Cast_Spell()             Combat
Auto_Cast_Spell_And_Do_Combat_Turn()    Combat  // WZD o98p04
    |-> Combat_Cast_Spell()             Combat
Combat_Cast_Spell_With_Caster()         Combat  // WZD s103p14
    |-> Combat_Cast_Spell()

Combat_Cast_Spell()
    |-> Summon_Demon()
    |-> Combat_Spellbook_Build()
    |-> Combat_Compose_Spellbook_Background()
    |-> Combat_Spellbook_Screen()
    |-> Do_Legal_Spell_Check()
    |-> Combat_Compose_Background()
    |-> Combat_Screen_Draw()
    |-> AI_Prepare_Combat_Realm_Threats()
    |-> AI_Select_Combat_Spell()
    |-> Combat_Spellbook_Mana_Adder_Screen()
    |-> Combat_Spell_Dispel_Attempt()
    |-> Combat_Spell_Counter_Message()
    |-> Combat_Casting_Cost_Multiplier()
    |-> Update_Combat_Enchantments_Icon_And_Help()
    |-> Combat_Spell_Target_Screen()
    |-> AITP_Combat_Spell()
    |-> Combat_Cast_Apply_Spell_Effect()

Combat_Battlefield_Instant()
Combat_Spell_Animation()
Wall_Rise()
Animate_Cracks_Call()
Combat_Spell_Animation_Bolt()
Combat_Spell_Animation_Default()
Battle_Unit_Summon_Animation()
Animate_Lightning_Bolt()
Cast_Call_Chaos__WIP()
    |-> Combat_Cast_Spell_Message()

NOT:
    'Combat Screen'
        Combat_Compose_Background()
        Combat_Screen_Draw()
        Update_Combat_Enchantments_Icon_And_Help()
    'Combat Spellbook'
        Combat_Spellbook_Build()
    'Combat Spellbook Screen'
        Combat_Spellbook_Screen()
        Combat_Compose_Spellbook_Background()
        Combat_Spellbook_Mana_Adder_Screen()
    'Combat Spell Target Screen'                                    Combat-Combat_Spell_Target_Screen.md
        Combat_Spell_Target_Screen()
    'Combat Spell Cast - AI - Select'                               Combat-Spell_Cast_AI_Select.md
        AI_Prepare_Combat_Realm_Threats()
        AI_Select_Combat_Spell()
    'Combat Spell Cast - AI - Target'                               Combat-Spell_Cast_AI_Target.md
        AITP_Combat_Spell()
    'Cast Spell On Unit'                                            Combat-Combat_Cast_Apply_Spell_Effect.md
        Combat_Cast_Apply_Spell_Effect()
    'Combat Spell Dispell'                                          Combat-Combat_Spell_Dispel.md
        Combat_Spell_Dispel_Attempt()
        Combat_Spell_Counter_Message()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-18).** Six functions, 2,668 asm lines, all walked against their listings and faithful. Nine reconstruction errors were raised and fixed across the pass; the tree builds clean. D1 records a project-wide initialiser divergence that is deliberately not being chased function by function.

## Scope

| function | production | listing | asm | state |
| --- | --- | --- | --- | --- |
| `Combat_Cast_Spell` | [CMBMAGIC.c:218](../../MoM/src/CMBMAGIC.c#L218) | `ovr112/Combat_Cast_Spell__WIP.asm` | 1379 | **phase-for-phase walk — faithful** (2026-07-07) |
| `Do_Legal_Spell_Check` | [CMBMAGIC.c:1072](../../MoM/src/CMBMAGIC.c#L1072) | `ovr112/Do_Legal_Spell_Check__WIP.asm` | 767 | **full body walk — faithful** |
| `Combat_Cast_Spell_Message` | [Combat.c:12372](../../MoM/src/Combat.c#L12372) | `ovr113/Combat_Cast_Spell_Message.asm` | 276 | **full body walk — faithful** |
| `Combat_Cast_Spell_With_Caster` | [Combat.c:7206](../../MoM/src/Combat.c#L7206) | `ovr103/zCombat_Cast_Spell_With_Caster.asm` | 111 | **full body walk — faithful** |
| `Combat_Casting_Cost_Multiplier` | [CMBMAGIC.c:1448](../../MoM/src/CMBMAGIC.c#L1448) | `ovr112/Combat_Casting_Cost_Multiplier.asm` | 109 | **full body walk — faithful** |
| `Combat_Cast_Spell_Error` | [Combat.c:6992](../../MoM/src/Combat.c#L6992) | `ovr103/zCombat_Cast_Spell_Error.asm` | 26 | **full body walk — faithful** |

**No open work.** All six functions have been walked and all findings are resolved. `Combat-Combat_Cast_Spell.md` has been retired into this doc — its walkthrough is the [phase-for-phase section](#combat_cast_spell--phase-for-phase-walkthrough) below. The tree builds clean and the tracker rows are in.

## Where `Combat_Cast_Spell_With_Caster` came from

It carried a `faithful` row in [Combat-Combat_Spellbook_Screen.md](Combat-Combat_Spellbook_Screen.md), where it was picked up as a callee of the spellbook pass rather than as spellbook code. That row has been removed from that doc, and the function has since been walked properly here — the carried verdict did not survive the walk. It is the cast-path entry wrapper: its sole caller is `Combat_Screen` at [Combat.c:1960](../../MoM/src/Combat.c#L1960), it calls `Combat_Cast_Spell`, and it is one of the two callers of `Combat_Cast_Spell_Error`.

## `Combat-Combat_Cast_Spell.md` is retired into this doc

`Combat_Cast_Spell` was reviewed on its own and taken to done-done on 2026-07-07. Two review docs whose names differed only by word order — `Combat-Combat_Cast_Spell.md` and `Combat-Spell_Cast.md` — was a trap, so that doc has been deleted and its walkthrough moved here verbatim below.

**Its anchors were all stale and have been re-pointed.** The function moved out of `Combat.c` and into `CMBMAGIC.c` in commit `e79026eb` ("extract CMBMAGIC", 2026-08-01), after the review was written. Every `Combat.c#L13xxx` link in it still resolved — `Combat.c` is long enough that the line numbers existed — but landed on unrelated combat-damage code, which is the silent-failure case the anchor checker cannot catch. The body relocated as a block at a constant offset of **-12946**, so the mapping is mechanical; three cross-references outside the function were re-verified by name instead (`Combat_Cast_Apply_Spell_Effect`, `AI_Select_Combat_Spell`, `Combat_Spell_Dispel_Attempt`), and `B8` was retargeted from a section marker to the `; BUG:` comment it describes.

The **verdict is carried, not re-earned**: those 1,379 asm lines were walked on 2026-07-07 and were not re-walked in this pass. The local-variable table below has been refreshed to the names production uses today; the asm slot assignments in it are as recorded by that pass.

## `Combat_Cast_Spell_Message` is here by decision, not by call graph

Its nine callers are all spell-animation functions in `Spells131.c` and `Spells133.c` — `Animate_Lightning_Bolt`, `Wall_Rise`, `Battle_Unit_Summon_Animation` and friends — so a caller-based attribution would send it to a combat spell-animation review that does not exist and, per the note in the header block above, is not judged worth creating. It sits here because this is the nearest live home, not because the call graph puts it here. Worth restating if a spell-animation session is ever opened.

## `Combat_Cast_Spell` — phase-for-phase walkthrough

| Function | Location | Role |
|---|---|---|
| `Combat_Cast_Spell` | [CMBMAGIC.c:218-810](../../MoM/src/CMBMAGIC.c#L218-L810) | Resolves combat spellcasting by one entity (a battle unit's spell-like ability, a human at the combat spellbook, or an AI/CP wizard): selects the spell, computes the effective mana cost, resolves opposing Counter Magic / node-realm counters, targets, casts, and pays. Returns `0` = nothing cast, `1` = cancelled during targeting, `2` = resolved (or countered). |

> **Walked phase-for-phase against `Combat_Cast_Spell__WIP.asm` on 2026-07-07 and found faithful.** The human-interactive path, the AI/CP selection branch (`loc_8D138`), all three cast-payment blocks, both counter-magic paths, and the shared target/cast/cleanup all match the disassembly. The reconstruction preserves a long list of OG casting-cost bugs as inline `; BUG:` comments (kept faithfully — see [Preserved OG bugs](#preserved-og-bugs-b--faithful-to-the-asm)). The `.asm` is the authority; the Gemini `.c` is a second opinion. The function's own header comments flag "not sure about the multiple returns", "handful of variable reuses (don't make distinct…)", and "cast_status seems mixed up" — those are design notes, not defects, and are left untouched. Every helper this function calls is now implemented; the `__WIP` names that pass listed as outstanding (`Combat_Spellbook_Build`, the `CMB_*__WIP` compose calls, `Combat_Spell_Target_Screen__WIP`) are all gone from the body.

### Signature & frame

```c
int16_t Combat_Cast_Spell(int16_t caster_idx, int16_t wx, int16_t wy, int16_t wp)
```

The asm frame declares **only** `caster_idx` (bp+6); `wx`/`wy`/`wp` are pushed by callers (`…(caster, _combat_wx, _combat_wy, _combat_wp)`) but never read in the body. Production faithfully keeps them in the signature and likewise never uses them. Locals map to the asm's bp-relative slots:

| Production local | asm slot | Notes |
|---|---|---|
| `cast_status` | `return_value` (-18h) | the `0/1/2` return |
| `is_spell_like_ability` | (-1Ah) | set when a unit casts via a granted ability |
| `spell_idx` | `si` | selected spell |
| `available_mana_pool` | `di` | working mana / cost accumulator |
| `battle_unit_mana`, `counter_player_idx`, `base_mana_cost`, `extra_mana_cost`, `player_idx`, `target_cgy`, `counter_magic_idx__target_cgx`, `Target`, `selected_page_slot`, `effective_cost`, `overland_spell_idx_saved`, `has_castable_spell` | -1Ch … -2 | same order as the asm frame |

Declared at [CMBMAGIC.c:219-238](../../MoM/src/CMBMAGIC.c#L219-L238). The slot assignments are as recorded by the 2026-07-07 pass; the names have since been brought into `lower_snake_case`, except `Target` ([CMBMAGIC.c:230](../../MoM/src/CMBMAGIC.c#L230)), which is still the IDA label and is the one local in this function that does not follow the convention.

`caster_idx` encoding: `< CASTER_IDX_BASE` (20) → a battle-unit index; `>= CASTER_IDX_BASE` → wizard `caster_idx - CASTER_IDX_BASE`; `== CASTER_IDX_BASE + NEUTRAL_PLAYER_IDX` → early `return cast_status`.

### Phase map (asm `loc_` ↔ production)

| Phase | asm | production | fidelity |
|---|---|---|---|
| Setup: `_combat_caster_idx`, derive `player_idx` / `battle_unit_mana` | 26-54 | [240-251](../../MoM/src/CMBMAGIC.c#L240-L251) | faithful |
| Neutral caster early-out | `loc_8CFC7` | [252-255](../../MoM/src/CMBMAGIC.c#L252-L255) | faithful |
| Human-interactive: unit ability spells (Doom Bolt / Fireball / Web / Healing) + Summon Demon | `loc_8CFE0`-`loc_8D0A6` | [267-311](../../MoM/src/CMBMAGIC.c#L267-L311) | faithful |
| Human spellbook select loop (`Combat_Spellbook_Build/Screen`, `Do_Legal_Spell_Check`) — nested inside the human branch | `loc_8D0AF`-`loc_8D135` | [317-355](../../MoM/src/CMBMAGIC.c#L317-L355) | faithful |
| AI/CP select: `AI_Prepare_Combat_Realm_Threats`, hero-item embed spell, `AI_Select_Combat_Spell`, target/immobile-counter | `loc_8D138`-`loc_8D1C1` | [357-382](../../MoM/src/CMBMAGIC.c#L357-L382) | faithful |
| Effective-cost computation (human popup vs AI skill/reserve budget) | `loc_8D204`-`loc_8D49E` | [386-505](../../MoM/src/CMBMAGIC.c#L386-L505) | faithful (carries the OG cost bugs) |
| Counter Magic dispel (opponent's Counter Magic enchantment; `Combat_Spell_Dispel_Attempt` / `Combat_Spell_Counter_Message`) | `loc_8D4CE`-`loc_8D6EC` | [512-600](../../MoM/src/CMBMAGIC.c#L512-L600) | faithful |
| Node-realm counter (Sorcery/Chaos/Nature node vs spell realm) | `loc_8D6F4`-`loc_8D986` | [601-692](../../MoM/src/CMBMAGIC.c#L601-L692) | faithful |
| Target + effect + pay (`Combat_Spell_Target_Screen` / `AITP_Combat_Spell` → `Combat_Cast_Apply_Spell_Effect`) | `@@Target_And_Effect`-`loc_8DC4D` | [703-798](../../MoM/src/CMBMAGIC.c#L703-L798) | faithful |
| Post-cast unit cleanup (write back `mana`, zero `ranged` if depleted) | `loc_8DC4D`-`loc_8DCA9` | [798-810](../../MoM/src/CMBMAGIC.c#L798-L810) | faithful |

### Selection dispatch (the part most recently reconstructed)

The human/AI split matches `loc_8CFCD`/`loc_8CFD6`: the interactive-human path (`player_idx == HUMAN_PLAYER_IDX && _auto_combat_flag == ST_FALSE`, [256-260](../../MoM/src/CMBMAGIC.c#L256-L260)) runs the unit-ability checks and, if still `spl_NONE`, the interactive spellbook — the spellbook is **nested inside the human branch** ([317](../../MoM/src/CMBMAGIC.c#L317)) so an AI caster never reaches it. The `else` branch ([357](../../MoM/src/CMBMAGIC.c#L357)) is the AI/CP path: `AI_Prepare_Combat_Realm_Threats()`, then the hero-item `embed_spell_idx` when a charged item is in use ([361-366](../../MoM/src/CMBMAGIC.c#L361-L366)) else `AI_Select_Combat_Spell(caster_idx)` ([369](../../MoM/src/CMBMAGIC.c#L369)), then `target_battle_unit_idx = caster` + non-human `_ai_immobile_counter = ST_UNDEFINED` ([371-381](../../MoM/src/CMBMAGIC.c#L371-L381)). The `spell_idx == 0` convergence the asm reaches via `loc_8D1FD → @@Target_And_Effect` is modelled structurally by the `if(spell_idx != spl_NONE)` cost/counter guard ([386](../../MoM/src/CMBMAGIC.c#L386)) and the `if(spell_idx > spl_NONE)` target guard ([703](../../MoM/src/CMBMAGIC.c#L703)) — no `goto` needed.

All three cost-payment blocks correctly index `_players[(caster_idx - CASTER_IDX_BASE)]` — counter-magic [545](../../MoM/src/CMBMAGIC.c#L545)/[546](../../MoM/src/CMBMAGIC.c#L546), node-counter [643](../../MoM/src/CMBMAGIC.c#L643)/[644](../../MoM/src/CMBMAGIC.c#L644), actual cast [752](../../MoM/src/CMBMAGIC.c#L752)/[753](../../MoM/src/CMBMAGIC.c#L753) — matching the asm's `_players.X-(14h*4c8h)[caster*size]` addressing.

### Faithful oddity — `if(Target != 999)`

The cast is guarded by `if(Target != 999)` ([741](../../MoM/src/CMBMAGIC.c#L741)), matching the asm `cmp [bp+Target], 999 / jnz loc_8DA78`. `Target` is only ever set to `99`, a battle-unit index, or the return of `Combat_Spell_Target_Screen__WIP` — so `999` is presumably that screen's cancel sentinel (the `1` = "cancelled during targeting" return). Reproduced verbatim; no change. (The header comment's "atleast one BUGBUG with Target" refers here; left as-is per the faithful-to-dasm rule.)

### Preserved OG bugs (B — faithful to the asm)

The reconstruction keeps the OG casting-cost / ability defects and annotates each inline; all match the asm and must **not** be "fixed":

| # | Site | Preserved defect |
|---|---|---|
| B1 | [399](../../MoM/src/CMBMAGIC.c#L399) | counters use the effective (reduced) cost |
| B2 | [404](../../MoM/src/CMBMAGIC.c#L404), [426](../../MoM/src/CMBMAGIC.c#L426), [466](../../MoM/src/CMBMAGIC.c#L466), [497](../../MoM/src/CMBMAGIC.c#L497) | ignores Evil Omens (unit / human / AI / Ice Bolt paths) |
| B3 | [449](../../MoM/src/CMBMAGIC.c#L449), [451](../../MoM/src/CMBMAGIC.c#L451), [457](../../MoM/src/CMBMAGIC.c#L457), [460](../../MoM/src/CMBMAGIC.c#L460) | AI cost path ignores base cost and casting-cost modifiers |
| B4 | [467](../../MoM/src/CMBMAGIC.c#L467) | an infusable item's charge is auto-countered when the user has no mana |
| B5 | [477](../../MoM/src/CMBMAGIC.c#L477) | Banish's random extra mana yields an effective gain of 1/15, not 5 |
| B6 | [541](../../MoM/src/CMBMAGIC.c#L541) | `Target_X` reused as scratch while still needed below (variable-reuse bug) |
| B7 | [552](../../MoM/src/CMBMAGIC.c#L552), [650](../../MoM/src/CMBMAGIC.c#L650), [759](../../MoM/src/CMBMAGIC.c#L759) | item-charge decrement may not be the hero's original owner |
| B8 | [804](../../MoM/src/CMBMAGIC.c#L804) | zeroing `ranged` on mana depletion also removes short-range attacks |

The three near-identical XOR-toggle / item-charge / mana-debit payment blocks (`loc_8D5A9` counter-magic, `loc_8D843` node-counter, `loc_8DADD` actual cast) are the asm's own copy-paste — reproduced faithfully, not refactored.

### Related references

- `…\ovr112\Combat_Cast_Spell__WIP.asm` — IDA Pro 5.5 disassembly (**the authority**).
- `…\ovr112\Combat_Cast_Spell__WIP__GEMINI.c` — Gemini translation (second opinion).
- [`AITP_Combat_Spell`](../ComputerPlayer/Combat-AITP_Combat_Spell.md) — the AI target picker invoked at [733](../../MoM/src/CMBMAGIC.c#L733) for the non-human targeting path.
- [`Combat_Cast_Apply_Spell_Effect`](../../MoM/src/Combat.c#L9964) — applies the chosen spell ([747](../../MoM/src/CMBMAGIC.c#L747)).
- `AI_Select_Combat_Spell` ([Combat.c:17398](../../MoM/src/Combat.c#L17398)), `AI_Prepare_Combat_Realm_Threats` — the AI-branch pickers, now wired into the `else` selection path.
- `Combat_Spell_Dispel_Attempt` ([Spells133.c:651](../../MoM/src/Spells133.c#L651), decl `Spells133.h:42`) — Counter-Magic / node dispel roll; **implemented** (was `Combat_Spell_Dispel_Attempt`); called at [532](../../MoM/src/CMBMAGIC.c#L532) and [632](../../MoM/src/CMBMAGIC.c#L632).
- `Combat_Spell_Counter_Message` ([Spells133.c:532](../../MoM/src/Spells133.c#L532), decl `Spells133.h:39`) — "Counter Magic" popup; **implemented** (was `Combat_Spell_Counter_Message`); called at [538](../../MoM/src/CMBMAGIC.c#L538) and [639](../../MoM/src/CMBMAGIC.c#L639).

## Deviations

### D1 - zero-initialisers with no asm counterpart (project-wide, recorded once)

Production initialises every local at its declaration. For scalars kept in registers that costs nothing observable, but **aggregates are different**: `char * selection_list_text[4] = { 0, 0, 0, 0 };` at [Combat.c:7208](../../MoM/src/Combat.c#L7208) is eight bytes of stack that Borland 3.0 would have zeroed with visible stores, and `char spell_name[LEN_SPELL_NAME] = { 0, ... };` at [Combat.c:12374](../../MoM/src/Combat.c#L12374) is twenty. Neither prologue does it -- `zCombat_Cast_Spell_With_Caster` is `push bp` / `mov bp, sp` / `sub sp, 0Ah` / two register pushes and straight into the body, and `Combat_Cast_Spell_Message` is the same with `sub sp, 1Eh`. Borland 3.0 does not eliminate dead stores to stack aggregates, so the source had bare declarations.

This is **not specific to these two functions** -- it is how the whole reconstruction is written, and every element is assigned before it is read at both sites, so nothing behaves differently. Recorded once so the divergence is on the books; chasing it function by function is not proposed.

## Verified faithful

### `Combat_Cast_Spell_With_Caster` ([Combat.c:7206](../../MoM/src/Combat.c#L7206), asm 111)

Faithful apart from the D1 initialiser.

**The top-level branch matches the asm.** asm:14-16 is `cmp [CMB_WizCastAvailable], e_ST_TRUE` / `jz short loc_85373` / `jmp loc_8542C`, where `loc_85373` is the instruction immediately after the `jmp` -- the Borland long-branch fixup, so the surviving `jz` carries the source condition un-inverted. The selection-box body follows the test and the direct cast sits far below at `loc_8542C` as the `else`, which is what [Combat.c:7211](../../MoM/src/Combat.c#L7211) now has.

**Frame - exact match.** `selection_list_text` at `bp-0Ah`, `selection_list_idx` at `bp-2`, `sub sp, 0Ah`, with `caster_id` in DI and `cast_status` in SI. The array spans `bp-0Ah` to `bp-3`, **8 bytes** = four near `char *`, and [Combat.c:7208-7210](../../MoM/src/Combat.c#L7208-L7210) declares the two stack locals in that order.

**The selection list.** `selection_list_text[0]` takes `offset _players.name` -- an address constant with no index, which is `_players[HUMAN_PLAYER_IDX].name` because `HUMAN_PLAYER_IDX` is 0 ([MOX_DEF.h:611](../../MoX/src/MOX_DEF.h#L611)). Entry [1] is a single load, `mov ax, [word ptr _unit_type_table.name+bx]`, with `bx` built from `unit_idx << 5` then `type * sizeof(s_UNIT_TYPE)`. That single-load form is exactly what every other `.name` site in the disassembly does -- `ovr065/CityList_Screen_Draw.asm:662`, `ovr105/Draw_Combat_Unit_Display.asm:158`, `ovr113/Combat_Cast_Spell_Message.asm:118` -- and production spells all of those `*_unit_type_table[x].name`, so this site is consistent with its siblings. Entries [2] and [3] are `offset str_cancel__ovr103` and `offset str_empty_string__ovr103`, defined at [Combat.c:304](../../MoM/src/Combat.c#L304) and [Combat.c:258](../../MoM/src/Combat.c#L258).

**`Selection_Box`.** asm:39-48 pushes `cnst_CasterSelectMsg`, `0`, `&selection_list_text`, `3` and cleans `add sp, 8` -- right to left, so the source order is `Selection_Box(3, &selection_list_text[0], 0, cnst_CasterSelectMsg)`, matching [Combat.c:7218](../../MoM/src/Combat.c#L7218) and the four-parameter declaration at [GENDRAW.h:106](../../MoX/src/GENDRAW.h#L106). Four words pushed, four parameters -- no argument-count mismatch here.

**All three `Combat_Cast_Spell` call sites.** Each pushes `_combat_wp`, `_combat_wy`, `_combat_wx`, then the caster, and cleans `add sp, 8`. The player case builds its caster as `mov ax, [_current_player_idx]` / `add ax, 20` -- a raw `20`, which is what [Combat.c:7223](../../MoM/src/Combat.c#L7223) writes rather than `CASTER_IDX_BASE`, and the literal is the faithful reading. The unit case and the no-selection case both pass `caster_id` straight through.

**Both `switch` statements match, and the contrast with the one real `if` is what proves them.** asm:49-55 loads `selection_list_idx` into AX and compares the register repeatedly, jumping *to* each body with a final unconditional jump to the default - Borland's sparse-`switch` dispatch, the same shape `Combat_Cast_Spell_Error` has at asm:7-12. asm:65-72 is the same one level in for a single case, closed by a degenerate `jmp short $+2` - a jump to the very next address, which is what a trailing `break` compiles to when the switch end is already there. Against that, asm:97-98 is `or cast_status, cast_status` / `jnz short @@Done`: tested in place, jumping *past* the body, which is a genuine `if`. Production now has all three constructs the way the listing has them.

**The tail.** `or cast_status, cast_status` / `jnz` gives `if(cast_status == ST_FALSE)`, then `Combat_Cast_Spell_Error(2)` -- pushed as `mov ax, 2` / `push ax` / `push cs` / `call near ptr`, the same-segment far-call idiom, cleaned with one `pop cx`. `Combat_Compose_Background()` runs unconditionally after the join at asm:105, and nothing is returned in AX.

### `Combat_Cast_Spell_Error` ([Combat.c:6992](../../MoM/src/Combat.c#L6992), asm 26)

Faithful. No findings. Twenty-six lines including the frame, and every one of them accounted for.

**No frame.** `push bp` / `mov bp, sp` with no `sub sp` and no register pushes — no locals of any kind, and `type` is the lone `word ptr 6` parameter. The exit is `pop bp` / `retf` with nothing written to AX, so the function is `void`.

**It really is a `switch`.** asm:7-12 loads the selector once into AX and then compares against it repeatedly — `mov ax, [bp+type]` / `cmp ax, 1` / `jz` / `cmp ax, 2` / `jz` / `jmp short loc_84E46`. An `if` / `else if` chain over an `int16_t` parameter would have compared the memory operand in place; loading to a register first and reusing it is Borland's signature for a sparse `switch`, so [Combat.c:6995](../../MoM/src/Combat.c#L6995) has the right construct. Selector order matches too, 1 before 2, and the final `jmp` is the absent `default` falling straight to the exit.

**The two arms are tail-merged in the asm, not in the source.** asm:13-22 lays case 2's body first (`mov ax, offset cnst_CmbCastError_2`), lets it fall into a shared `push ax` / `call j_Warn1` / `pop cx` / `jmp` at `loc_84E38`, and has case 1 load its own string and `jmp short loc_84E38` into that same tail. Two identical call sequences collapsed into one is Borland's cross-jump optimization; the two separate `Warn1` calls at [Combat.c:6999](../../MoM/src/Combat.c#L6999) and [7003](../../MoM/src/Combat.c#L7003) are the correct reconstruction, and the reversed body layout is placement, not source order. The shared `jmp short loc_84E46` is both cases' `break`.

**Everything else lines up.** `Clear_Fields()` before the switch at asm:6, one word pushed and `pop cx` for each `Warn1`, `j_Warn1` being the far-call thunk this codebase routes `Warn1` through everywhere. Both string constants exist with the text the inline comments quote — `cnst_CmbCastError_2` at [Combat.c:300](../../MoM/src/Combat.c#L300) and `cnst_CmbCastError_1` at [302](../../MoM/src/Combat.c#L302).

**Callers.** Two functions, three sites: `Combat_Screen` at [Combat.c:1971](../../MoM/src/Combat.c#L1971) and [1990](../../MoM/src/Combat.c#L1990), and `Combat_Cast_Spell_With_Caster` at [Combat.c:7248](../../MoM/src/Combat.c#L7248).

### `Combat_Casting_Cost_Multiplier` ([CMBMAGIC.c:1448](../../MoM/src/CMBMAGIC.c#L1448), asm 109)

Faithful.

**The distance block is Chebyshev, corrected once.** asm:55-60 is the single wrap correction — one `cmp _SI_modifier, 30`, one `sub ax, _SI_modifier`, and nothing jumping back to the orphan `loc_8ECBC`, so it is an `if` and not a `while` ([CMBMAGIC.c:1468](../../MoM/src/CMBMAGIC.c#L1468)). asm:61-68 then selects with `cmp _SI_modifier, [bp+Y_Distance]` / `jge`: the guarded body assigns the **y** delta, so both arms keep the larger of the two and the result is the Chebyshev distance the overland code uses everywhere — the same rule the `AITP_EarthToMud` header at [CMBMAGIC.c:1520](../../MoM/src/CMBMAGIC.c#L1520) states.

**Three return paths.** `mov ax, 1` at asm:14-17, `mov ax, 5` at asm:71-73, and `mov ax, _SI_modifier` at asm:100-102. `modifier` lives in SI, so both constant exits bypassing SI entirely means the source returned those constants outright rather than assigning `modifier` and falling through to a common tail — Borland 3.0 does not forward a register variable into AX across a jump. Note the surrounding `else` levels are **not** adjudicable from the listing: `if(c) { return 1; } else { B }` and `if(c) { return 1; } B` generate byte-identical code, so their presence is a readability question and not a fidelity one.

**Frame.** One stack local — `Y_Distance` at `bp-2`, `sub sp, 2` — plus SI and DI. [CMBMAGIC.c:1450-1453](../../MoM/src/CMBMAGIC.c#L1450-L1453) declares `delta_y` first, which is the one that has to be the stack slot; the other three carry no ordering constraint. SI holds `delta_x` up to asm:67 and then `modifier` from asm:84 — a single register serving both, disjoint live ranges, no evidence either way about how many locals the source declared.

**The neutral-player gate.** asm:12-13 is `cmp [bp+player_idx], e_NEUTRAL_PLAYER_IDX` / `jnz`, the jump-skips-the-body idiom, so the source tests `==`. `NEUTRAL_PLAYER_IDX` is 5 ([MOX_DEF.h:612](../../MoX/src/MOX_DEF.h#L612)).

**The off-plane short circuit.** asm:19-29 indexes `_FORTRESSES` by `player_idx` shifted left 2 — `sizeof(s_FORTRESS)` is 4 — reads `.wp` through `cbw`, compares against `_combat_wp`, and `jz` to the same-plane case, so the source condition is `!=`. The literal loaded is `30` where asm:58 loads the symbolized `e_WORLD_WIDTH`; `WORLD_WIDTH` is 60 ([MOM_DEF.h:272](../../MoX/src/MOM_DEF.h#L272)), so production's `(WORLD_WIDTH / 2)` folds to the same byte and IDA's choice to symbolize one and not the other carries no weight.

**Both deltas.** asm:30-54 computes `abs(_FORTRESSES[player_idx].wy - _combat_wy)` into the stack slot first, then `abs(_FORTRESSES[player_idx].wx - _combat_wx)` into SI — y before x, matching [CMBMAGIC.c:1466-1467](../../MoM/src/CMBMAGIC.c#L1466-L1467), each a near `call abs` with a one-word push and `pop cx`. IDA's `_combat_wx__som_started_anim_ctr` is a label recording a second consumer of that address elsewhere; there is no second global in production and none is wanted.

**The channeler override and the four steps.** asm:76-81 is `cmp [_players.channeler+bx], 0` / `jle` to skip, i.e. `> 0`, forcing `distance = 2`. It runs **after** the `distance == 0` test, so a channeler always lands on a bare `modifier = 10` — 2 is not greater than 5 — which is faithful. Then `mov _SI_modifier, 10` and four `cmp _DI_distance, N` / `jle` / `add _SI_modifier, 5` pairs at asm:85-99 against 5, 10, 15 and 20, reproduced at [CMBMAGIC.c:1491-1506](../../MoM/src/CMBMAGIC.c#L1491-L1506).

**One artifact, no counterpart wanted.** asm:74 is `jmp short @@JmpDone__Return_SI` sitting immediately after the unconditional `jmp short @@JmpDone` on the line above — unreachable, stranded by the return-value pattern, the same kind of leftover as the raw `db 0EBh` / `db 4` in `Combat_Spell_Dispel_Attempt`.

### `Combat_Cast_Spell_Message` ([Combat.c:12372](../../MoM/src/Combat.c#L12372), asm 276)

Faithful. No findings.

**Frame — exact match.** `spell_name` at `bp-1Eh`, `string_width` at `bp-0Ah`, `colors[2]` at `bp-8`, `y2` at `bp-6`, `x2` at `bp-4`, `y1` at `bp-2`, `sub sp, 1Eh`. [Combat.c:12374-12380](../../MoM/src/Combat.c#L12374-L12380) declares them in that order, with `caster_idx` in SI and `x1` in DI as register locals. `spell_name` spans `bp-1Eh` to `bp-0Bh` — **20 bytes** = `LEN_SPELL_NAME` ([MOX_BASE.h:41](../../MoX/src/MOX_BASE.h#L41)).

**The caster chain.** asm:19-20 is `cmp caster_idx, e_CASTER_IDX_BASE` / `jl`, so the wizard case is the fall-through and production's `if(caster_idx >= CASTER_IDX_BASE)` is the source form. The wizard-name address folds the rebase into the base constant — `offset _players.name-(14h*4c8h)-1`, then `add dx, ax` and `inc dx` — resolving to `_players[caster_idx - CASTER_IDX_BASE].name`, the same folding seen in `Combat_Spell_Counter_Message`.

**The unit branch.** asm:52 is `cmp [es:bx+s_UNIT.Hero_Slot], e_ST_UNDEFINED_DB` / `jle`, i.e. `> ST_UNDEFINED`. The hero path reads the name through `battle_units[caster_idx].controller_idx` — which is what the preserved `/* OGBUG: this may not be the hero's original owner, should use _UNITS[].owner_idx */` note at [Combat.c:12390](../../MoM/src/Combat.c#L12390) is about, and it is faithful. The non-hero path builds "The " + unit type name, then picks the verb on `cmp [es:bx+s_BATTLE_UNIT.Max_Figures], 1` / `jle` — `> 1` takes " have cast ", otherwise " has cast ". IDA's `Max_Figures` is production's `figure_max`, `int8_t` at [Combat.h:1250](../../MoM/src/Combat.h#L1250).

**The spell name.** asm:143-144 is `cmp [bp+spell_idx], spl_NONE` / `jz`, the jump-skips-the-body idiom, so the source reads `spell_idx != spl_NONE`; `spl_NONE` is 0. The copy is a far call — the segment word of `spell_data_table` is pushed — matching `_fstrcpy` at [Combat.c:12410](../../MoM/src/Combat.c#L12410). The else arm appends "summon demon".

**Geometry.** `Get_String_Width` rather than the paragraph measurement its `o113p13` sibling uses, because this message is a single line: `y1 = 5` and `y2 = 15` are both literals, where the sibling computes `y2` from a measured height. `x1` is built in two steps — `mov dx, 160` / `sub dx, ax` / `add dx, -5` — and [Combat.c:12424](../../MoM/src/Combat.c#L12424) already carries that form. `x2` is `(string_width / 2) + 165`; production writes `165 + (string_width / 2)`, an operand-order difference of the kind catalogued as D3 in [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md).

**The box and the text.** Four `Line` calls with the same corners and colours as the sibling — `(x1, y1, x1, y2, 230)`, `(x1, y1, x2 - 1, y1, 230)`, `(x2, y1, x2, y2, 237)`, `(x1 + 1, y2, x2, y2, 237)` — then `Print_Centered(160, 8, GUI_NearMsgString)`, three words with `add sp, 6`. Note this one centres on a hardcoded 160 rather than deriving the x from the box, unlike the paragraph sibling which prints at `x1 + 6`.

**`Gradient_Fill` is under-supplied here too.** asm:217-226 pushes six words -- `8`, `15`, `y2`, `x2`, `y1`, `x1` -- and cleans `add sp, 0Ch` against a nine-parameter callee. [Combat.c:12428](../../MoM/src/Combat.c#L12428) pads the missing three with `ST_NULL`, which is the same K&R short-call situation and the same reconstruction already catalogued as D8 in [Combat-Combat_Screen.md](Combat-Combat_Screen.md); no separate finding is raised for it.

### `Do_Legal_Spell_Check` ([CMBMAGIC.c:1072](../../MoM/src/CMBMAGIC.c#L1072), asm 767)

Faithful. No findings.

**The nine scans use both `controller_idx` polarities, and production now matches all nine.** Seven skip the body with `jnz` and are `==` — asm:139, 266, 307, 409, 425, 584 and 722. Two skip with `jz` and are `!=`: `spl_Star_Fires` at asm:197-198 jumping to `loc_8E65B` ([CMBMAGIC.c:1154](../../MoM/src/CMBMAGIC.c#L1154)) and `scc_Mundane_Curse` at asm:641-642 jumping to `loc_8EA90` ([CMBMAGIC.c:1328](../../MoM/src/CMBMAGIC.c#L1328)), both labels being the loop increment. The pairing to check against is `scc_Unit_Enchantment_Normal_Only` at [CMBMAGIC.c:1307](../../MoM/src/CMBMAGIC.c#L1307), directly above the curse block and `jnz` in the asm: the two adjacent scans differ by exactly this operator.


**Frame.** `controlled_unit_count` at `bp-4`, `illegal` at `bp-2`, `sub sp, 4`, with `itr` in SI and the flag in DI. [CMBMAGIC.c:1074-1078](../../MoM/src/CMBMAGIC.c#L1074-L1078) declares the two stack locals in that order. The IDA header names them `Controlled_Units`, `illegal`, `_SI_itr` and `_DI_IDK`; all four are database labels, not OG source names.

**DI carries both `legal_target_found` and `side`.** The scans use it as the found-flag; the battlefield-enchantment block at [CMBMAGIC.c:1287-1293](../../MoM/src/CMBMAGIC.c#L1287-L1293) uses it as the attacker/defender offset into `combat_enchantments` — `xor di, di` / `mov di, e_ST_TRUE` at asm:535-538, then `add bx, _DI_IDK` at asm:545. Two source locals with disjoint live ranges sharing one register is exactly as consistent with the listing as one local reused, so the split into two names is not a fidelity question either way.

**The two early exits.** asm:14-18 is `cmp [bp+spell_idx], 666` / `jnz`, returning `ST_TRUE`; asm:20-23 is `cmp [bp+spell_idx], spl_NONE` / `jg`, returning `ST_FALSE` — i.e. `<= spl_NONE`, with `spl_NONE` = 0.

**Every scan loop carries a compound condition intact.** The shape is `cmp si, [_combat_total_unit_count]` / `jge` to exit, then `or di, di` / `jz` to the body, which is `(itr < _combat_total_unit_count) && (legal_target_found == ST_FALSE)` — no `break`, and production reproduces it at all nine sites. The message block that follows each is guarded by `or di, di` / `jnz` to skip, i.e. `if(legal_target_found == ST_FALSE)`.

**The `spl_Star_Fires` race test is a genuine `||`.** asm:204-212 short-circuits `rt_Death` to the set and falls through to `rt_Chaos`, matching [CMBMAGIC.c:1157-1159](../../MoM/src/CMBMAGIC.c#L1157-L1159). Elsewhere the race test is `>= rt_Arcane` to skip, i.e. `< rt_Arcane` in production.

**`LBX_Load_Data_Static(message_lbx_file__ovr112, 0, &GUI_NearMsgString[0], N, 1, 150)`** at every message site, with the record number the only thing that varies — 76, 85, 77, 79, 83 and the rest — pushed right to left and cleaned with the same count throughout, each followed by `Warn1(GUI_NearMsgString)` and `illegal = ST_TRUE`.

**The counting guards.** `cmp [bp+Controlled_Units], e_MAX_STACK` / `jnz` at asm:366, 484 and 737 gives `else if(controlled_unit_count == MAX_STACK)` — [1213](../../MoM/src/CMBMAGIC.c#L1213), [1263](../../MoM/src/CMBMAGIC.c#L1263) and [1366](../../MoM/src/CMBMAGIC.c#L1366), and asm:690 is `cmp [_units], e_MAX_UNIT_COUNT` / `jnz` for the summoning cap.

**The `scc_Counter_Spell` exclusion chain.** asm:518-531 tests the type, then excludes `spl_Holy_Word`, `spl_Death_Spell`, `spl_Flame_Strike` and `spl_Call_Chaos` in that order before testing `_combat_attacker_player == HUMAN_PLAYER_IDX` — four `jz` exits to the same label, which is the `||` chain production has.

**The `wp == 9` tests** at asm:332 and asm:451 are reproduced as `wp != 9` guards at [CMBMAGIC.c:1201](../../MoM/src/CMBMAGIC.c#L1201) and [1251](../../MoM/src/CMBMAGIC.c#L1251) — `jz` to skip, so the surviving condition is the inequality.
