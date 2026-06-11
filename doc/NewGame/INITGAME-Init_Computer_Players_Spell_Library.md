INITGAME-Init_Computer_Players_Spell_Library.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Computer_Players_Spell_Library.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Computer_Players_Spell_Library.c

New_Game / Map setup
|-> Init_Computer_Players()            [INITGAME.c:73]
    |-> Init_Computer_Players_Wizard_Profile()
    |-> Init_Computer_Players_Spell_Library()   [INITGAME.c:1275]

---

# `Init_Computer_Players_Spell_Library` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Init_Computer_Players_Spell_Library` | [INITGAME.c:1275-1661](../../MoM/src/INITGAME.c#L1275-L1661) | For every player, builds the spell library: marks each realm's spells `Known` / `Knowable` / `Unknown` from the wizard's book count, copies the predefined starting spells, and grants the Arcane + artificer fixed spells. |
| `Init_Computer_Players_Spell_Library__GEMINI` | [INITGAME.c:1664-1929](../../MoM/src/INITGAME.c#L1664-L1929) (inside `#if 0`) | Reference IDA→C translation (= Piethawn `*.c`), kept for comparison. Correct on the index math, **wrong on the rank-11 realm map** — see [Notes vs `__GEMINI`](#notes-vs-__gemini). Not OG-truth. |

This reconstruction is verified faithful to the disassembly `Init_Computer_Players_Spell_Library.asm` throughout.

## Purpose

Runs once at new-game setup, after `Init_Computer_Players_Wizard_Profile` has set each wizard's `spellranks[]`. It fills `_players[itr].spells_list[]` — 6 realms × 40 spells (10 common / 10 uncommon / 10 rare / 10 very-rare), each byte `sls_Unknown (0)` / `sls_Knowable (1)` / `sls_Known (2)`. Per realm with books, it grants starting `Known` spells from `_player_start_spells` and marks a book-count-scaled number of the rest `Knowable`. Arcane is always partly researchable; Spell of Mastery, Demon Lord, and the artificer spells are fixed grants.

`spells_list` index for realm `sbr`, rarity slot position `p`: **`sbr*40 + rarity_base + p`** (rarity_base 0/10/20/30).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Init_Computer_Players` | [INITGAME.c:73](../../MoM/src/INITGAME.c#L73) | Second AI-setup step, after the wizard-profile pass. |
| `Init_Computer_Players` ← `Map`/new-game | [MAPGEN.c:288](../../MoM/src/MAPGEN.c#L288) | The only call site of `Init_Computer_Players`. |

## The `_player_start_spells` indexing convention

`_player_start_spells[itr]` is one `s_Init_Base_Realms` per player (`0x82` = 130 bytes = 65 `int16_t`): five 13-word realm blocks. **Those blocks are stored in their own order — `[Nature, Death, Life, Sorcery, Chaos]` — not `sbr` order.** So `realms[]` is indexed by `e_PLAYER_START_SPELL_REALM` ([MOM_DAT.h:715](../../MoX/src/MOM_DAT.h#L715)), *not* by `sbr`:

| Realm | `sbr_*` | `pssr_*` = `realms[]` index | word base |
|---|---|---|---|
| Nature  | 0 | `pssr_Nature` = 0 | 0 |
| Sorcery | 1 | `pssr_Sorcery` = 3 | 39 |
| Chaos   | 2 | `pssr_Chaos` = 4 | 52 |
| Life    | 3 | `pssr_Life` = 2 | 26 |
| Death   | 4 | `pssr_Death` = 1 | 13 |

Each stored value is a 1-based global spell id; the realm-local slot is `(value - 1) % 40`, written at `spells_list[sbr*40 + (value-1) % 40]`. The key asymmetry: **the `_player_start_spells` read indexes by `pssr_*`, the `spells_list` write indexes by `sbr`.** Both `switch(sbr)` blocks below use the matching `pssr_*` constant per case, so the permutation is self-documenting and can't drift.

## Structure

```mermaid
flowchart TD
    Entry(["Init_Computer_Players_Spell_Library()"])
    PlayerLoop["for itr in players"]
    Clear["clear spells_list[] (6×40)"]
    RealmLoop["for sbr in 6 realms"]
    Arcane{"sbr == Arcane?"}
    ArcBlock["Arcane 0-11 Knowable, 13 Known; continue"]
    Zero{"spellranks[sbr] == 0?"}
    Skip["continue"]
    Max{"spellranks[sbr] == 11?"}
    MaxBlock["all 40 Knowable + all commons Known<br/>+ 3 bonus starting spells (pssr_X.spells[10/11/12])"]
    Normal["commons: copy starting spells + fill to limit;<br/>uncommon/rare/very-rare: all-Knowable if high rank,<br/>else random fill to limit"]
    Fixed["spells_list[Demon_Lord]=Known;<br/>artificer -> Detect_Magic, Disjunction Known"]
    Done(["return"])

    Entry --> PlayerLoop --> Clear --> RealmLoop --> Arcane
    Arcane -->|yes| ArcBlock --> RealmLoop
    Arcane -->|no| Zero
    Zero -->|yes| Skip --> RealmLoop
    Zero -->|no| Max
    Max -->|yes| MaxBlock --> RealmLoop
    Max -->|no| Normal --> RealmLoop
    RealmLoop -->|done| Fixed --> Done
```

## Code walk

Line refs are production [INITGAME.c](../../MoM/src/INITGAME.c); cross-checked against `Init_Computer_Players_Spell_Library.asm` (the authority). `Random(n)` returns `1..n` ([random.c:263](../../MoX/src/random.c#L263)).

### Clear ([1293-1300](../../MoM/src/INITGAME.c#L1293-L1300))

Zeroes all 6×40 `spells_list` entries for the player.

### Realm dispatch ([1302-1330](../../MoM/src/INITGAME.c#L1302-L1330))

`for sbr in 0..5`. Arcane ([1305-1322](../../MoM/src/INITGAME.c#L1305-L1322)) marks slots 0-11 `Knowable` and slot 13 (`spl_Spell_Of_Mastery`) `Known`, skips slot 12 (the `// WTF Why not 0x0C?` note, faithful to the asm), then `continue`. `spellranks==0` realms `continue` ([1324](../../MoM/src/INITGAME.c#L1324)). `spellranks==11` takes the max-book branch ([1585](../../MoM/src/INITGAME.c#L1585)); else the normal rank-1-10 branch.

### Normal branch — commons ([1332-1423](../../MoM/src/INITGAME.c#L1332-L1423))

1. Clear `Availability_Array`; common availability limit by book count (`1→3 … 6→9, 7-10→10`).
2. **Copy starting commons** ([1354-1402](../../MoM/src/INITGAME.c#L1354-L1402)): per `switch(sbr)` case, for the first `spellranks-1` entries, `spells_list[sbr*40 + (start_spell - 1) % 40] = Known`, where `start_spell = _player_start_spells[itr].realms[pssr_X].spells[idk]` (read via `pssr_*`, written via `sbr` — see the convention above).
3. **Fill commons to the limit** ([1406-1423](../../MoM/src/INITGAME.c#L1406-L1423)): `Available_Spells = spellranks-1`; while under the limit, roll `Random(NUM_SPELLS_PER_MAGIC_RARITY)`, mark an unknown common `Knowable`, and recount.

### Normal branch — uncommon / rare / very-rare ([1426-1583](../../MoM/src/INITGAME.c#L1426-L1583))

Each rarity: if the book count is high enough (`>7` uncommon, `>9` rare/very-rare) mark all 10 `Knowable`; otherwise set a book-count-scaled limit and randomly fill that many **distinct** slots —

```c
while(IDK_itr_10 < Availability_Limit) {
    InRarity_Index = (Random(NUM_SPELLS_PER_MAGIC_RARITY) - 1);
    if(Availability_Array[InRarity_Index] != 1) { Availability_Array[InRarity_Index] = 1; IDK_itr_10++; }
}
```

— then copy the marked slots into `spells_list[sbr*40 + rarity_base + p]`. The `while` (advance only on a newly-filled slot) re-rolls collisions, matching the asm's `do { roll } while(slot set); set; idx++` — same distinct-fill count and same RNG consumption.

### Max-book branch (`spellranks == 11`) ([1585-1646](../../MoM/src/INITGAME.c#L1585-L1646))

All 40 realm spells `Knowable`, then per realm all 10 commons `Known`, then 3 bonus starting spells `Known` at `realms[pssr_X].spells[10/11/12]` (written `spells_list[sbr*40 + (value-1)%40]`).

### Fixed grants ([1650-1655](../../MoM/src/INITGAME.c#L1650-L1655))

`spells_list[spl_Demon_Lord] = Known`; if `artificer`, `spl_Detect_Magic` and `spl_Disjunction` `Known` (asm `[200]=2`, artificer `[206]=2`, `[210]=2`).

## Notes vs `__GEMINI`

The `#if 0` `__GEMINI` block (= Piethawn `*.c`) is a raw IDA→C translation — a useful cross-check, not truth:

- **Index math.** GEMINI uses the flat `_player_start_spells.realms[itr].spells[base+idk]` addressing with the correct realm bases `{0,39,52,26,13}`, the `% 40`, and the `sbr*40 +` destination. This reconstruction expresses the same thing through `realms[pssr_X]` + the `e_PLAYER_START_SPELL_REALM` enum.
- **Rank-11 realm map — GEMINI is wrong here.** Its `switch` labels don't match the offsets it transcribed (it lists Sorcery `62-64`, Chaos `36-38`, Life `49-51`). The asm order is Sorcery `49-51`, Chaos `62-64`, Life `36-38` — i.e. `realms[pssr_Sorcery/pssr_Chaos/pssr_Life]`. GEMINI transcribed the asm case-blocks in memory order without resolving the jump table; **don't trust its rank-11 cases.**
- **Rarity fills / commons die.** GEMINI rolls `Random(10)` and uses the collision-retry `idx++`-inside-`if`, both matching the asm.

## Sub-functions / external calls

- **`Random`** ([random.c:263](../../MoX/src/random.c#L263)) — returns `1..n`.
- **`_players[]`**, **`_num_players`**, **`_player_start_spells`** — globals read/written.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr056\Init_Computer_Players_Spell_Library.asm` — IDA Pro 5.5 disassembly (the authority).
- [INITGAME.c:1664-1929](../../MoM/src/INITGAME.c#L1664-L1929) — `__GEMINI` reference translation (`#if 0`).
- [INITGAME.c:73 — `Init_Computer_Players`](../../MoM/src/INITGAME.c#L73) — caller.
- [INITGAME-Init_Computer_Players_Wizard_Profile.md](INITGAME-Init_Computer_Players_Wizard_Profile.md) — the preceding AI-setup step; populates the `spellranks[]` this function reads.
- `NewGame.h` — `s_Init_Base_Realms` / `s_Init_Base_Spells` (`_player_start_spells` layout); `MOM_DAT.h` — `e_PLAYER_START_SPELL_REALM` (`pssr_*`).
