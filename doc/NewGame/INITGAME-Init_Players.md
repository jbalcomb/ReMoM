INITGAME-Init_Players.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Players.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Players.c

Init_Runtime()
    |-> Init_Players();

---

# `Init_Players` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Init_Players` | [INITGAME.c:1200-1277](../../MoM/src/INITGAME.c#L1200-L1277) | Per-player new-game reset of each wizard's derived stats: fame, starting spell-casting skill (from spell ranks), magic ratios, all overland enchantments, vault items, and hero slots. |

`MGC` overlay 56 (asm `proc Init_Players far`).

## Purpose

Once each wizard's books/retorts are chosen, `Init_Players` computes the derived
starting state for all `_num_players` wizards: fame from the Famous retort, the
initial `spell_casting_skill` grown to match the doubled spell-rank total
(+ Archmage bonus), default skill/mana/research ratios, a clean slate of global
enchantments, empty vault items, and undefined hero slots.

**RNG-neutral: zero `Random()` calls** ([1200-1277](../../MoM/src/INITGAME.c#L1200-L1277)). The only sub-call, `Player_Base_Casting_Skill__MGC` ([INITGAME.c:2659](../../MoM/src/INITGAME.c#L2659)), is a pure incremental formula — no RNG. Does not affect new-game `Random()` alignment.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_Runtime` / INITGAME | [INITGAME.c:601](../../MoM/src/INITGAME.c#L601) | Right after `Init_Heroes`; followed by `gd_dump_players("101_Init_Players_P")` ([602](../../MoM/src/INITGAME.c#L602)). |

## How it works

Single loop over `itr_players < _num_players` ([1208](../../MoM/src/INITGAME.c#L1208)). For each wizard:

1. **Fame** ([1210-1217](../../MoM/src/INITGAME.c#L1210-L1217)) — `famous == ST_TRUE` → `fame = 10`, else `0`.
2. **Casting skill** ([1218-1233](../../MoM/src/INITGAME.c#L1218-L1233)) — `spell_skill = Σ spellranks[0..4]`; `+5` if Archmage; then doubled. `Nominal_Skill` and `spell_casting_skill` are zeroed, then a grow loop increments `spell_casting_skill` and recomputes `Nominal_Skill = Player_Base_Casting_Skill__MGC(...)` until `Nominal_Skill >= spell_skill` (lands on the smallest skill whose nominal value reaches the target).
3. **Resets** ([1234-1269](../../MoM/src/INITGAME.c#L1234-L1269)) — `researching_spell_idx = spl_NONE`, `mana_reserve = 0`, `research_cost_remaining = 0`, ratios `33/33/34`, the Summon Champion spells-list slot set `sls_Researchable` (OGBUG — see below), `casting_spell_idx = spl_NONE`, `casting_cost_remaining = 0`, every overland enchantment `= 0` (in the asm's field order), `volcanoes = 0`, `tax_rate = 2`, `Vault_Items[0..3] = ST_UNDEFINED`.
4. **Hero slots** ([1270-1273](../../MoM/src/INITGAME.c#L1270-L1273)) — `for itr_heroes < NUM_HERO_SLOTS`: `Heroes[itr_heroes].unit_idx = ST_UNDEFINED`.

## Verification against the asm

| Item | OG asm | Production |
| --- | --- | --- |
| Player loop bound | `cmp player_idx, [_num_players] / jge` | `itr_players < _num_players` ✓ |
| Fame | `cmp famous, e_ST_TRUE / jnz` → 10 else 0 | `famous == ST_TRUE ? 10 : 0` ✓ |
| Spell-skill sum | `spellranks +0 +2 +4 +6 +8` (5 ranks, word-strided) | `spellranks[0]+[1]+[2]+[3]+[4]` ✓ |
| Archmage bonus | `cmp archmage, e_ST_FALSE / jz` skip, else `+5` | `if(archmage != ST_FALSE) +5` ✓ |
| Doubling | `add spell_skill, spell_skill` | `spell_skill += spell_skill` ✓ |
| Casting-skill grow loop | `loc_52DD3`: `cmp Nominal_Skill, spell_skill / jl` body; body `+1` (`add`/`adc` dword) then `Player_Base_Casting_Skill__MGC` | `while(Nominal_Skill < spell_skill){ spell_casting_skill += 1; Nominal_Skill = …MGC(); }` ✓ (`spell_casting_skill` is `int32_t`, [MOM_DAT.h:1505](../../MoX/src/MOM_DAT.h#L1505), matching the `adc`) |
| Ratios | `skill 33 / mana 33 / research 34` | `33 / 33 / 34` ✓ |
| Spells-list researchable slot | `spells_list+0xD4` (212 = `spl_Summon_Champion`) `= sls_Researchable` | `spells_list[spl_Summon_Champion] = sls_Researchable` ✓ (see OGBUG) |
| Globals reset | 22 fields incl. interleaved `volcanoes`, order Eternal_Night…Holy_Arms, volcanoes, Detect_Magic, all `= 0` | same fields, same order ✓ |
| `tax_rate` | `= 2` | `= 2` ✓ |
| Vault items | `Vault_Items[0..3] = e_ST_UNDEFINED` | `[0..3] = ST_UNDEFINED` ✓ |
| Hero loop | `itr_heroes < e_NUM_HERO_SLOTS` (=6), `Heroes[].unit_idx = e_ST_UNDEFINED` | `itr_heroes < NUM_HERO_SLOTS` (=6) ✓ |

## OG bugs preserved (do **not** "fix" in baseline ReMoM)

- **Summon Champion marked researchable instead of Spell of Mastery** —
  [INITGAME.c:1240](../../MoM/src/INITGAME.c#L1240). The asm stores to
  `spells_list + 0xD4` = index **212**, which the OG's own `MAGIC.inc` names
  `spl_Summon_Champion` (production's `Spellbook.h` agrees exactly:
  `spl_Summon_Champion = 0xD4`, `spl_Spell_Of_Mastery = 0xD5 = 213`). The
  semantically-expected start-researchable spell is the Spell of Mastery
  (`0xD5`), so the OG store to `0xD4` is almost certainly an original off-by-one
  — but it is the disassembly's behavior, so production reproduces it with
  `spells_list[spl_Summon_Champion]` and a breadcrumb comment. **Faithful;
  preserve** — do not "correct" it to `spl_Spell_Of_Mastery`.

## Sub-functions / external calls

- **`Player_Base_Casting_Skill__MGC`** ([INITGAME.c:2659](../../MoM/src/INITGAME.c#L2659)) — pure incremental formula over `spell_casting_skill` (+10 if Archmage); no RNG.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Players.asm` — IDA Pro 5.5 disassembly (authority); `spells_list+0D4h` store, dword `add`/`adc` on `spell_casting_skill`.
- `MAGIC.inc` — OG spell enum (`spl_Summon_Champion = 0D4h`, `spl_Spell_Of_Mastery = 0D5h`), `e_NUM_HERO_SLOTS = 6`.
- [INITGAME.c:601-602](../../MoM/src/INITGAME.c#L601-L602) — call site + player dump.
