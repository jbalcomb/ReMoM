Spells128-Player_Research_Spells.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr128\Player_Research_Spells.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr128\Player_Research_Spells.c

~ 'New Game' / 'Load Game'  -> PreInit_Overland() |-> Player_Research_Spells()  (per player)

Loaded_Game_Update()
    |-> Init_Overland()
        |-> PreInit_Overland()
            |-> Player_Research_Spells()

---

# `Player_Research_Spells` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Player_Research_Spells` | [Spells128.c:50-207](../../MoM/src/Spells128.c#L50-L207) (WZD `o128p01`; drake178 `WIZ_RefreshResearch`) | Fills any empty slots in a wizard's research-candidate list (`research_spells[8]`) from the pool of researchable spells, makes sure a wizard with candidates is researching *something* (prompting the human or the AI picker), and returns the candidate count. |

> **Disassembly fidelity:** verified against `ovr128` [Player_Research_Spells.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr128/Player_Research_Spells.asm) — control flow matches 1:1.

## Purpose

A wizard tracks up to `NUM_RESEARCH_SPELLS` (= 8) **research candidates** in `research_spells[]`. This routine:

1. Rebuilds the eligible pool via `Build_Research_List` (lowest-rarity spell from each realm; count returned in the global `m_spell_list_count`, the OG's `GUI_Multipurpose_Int`).
2. Counts existing candidates and notes whether Spell of Mastery is already known/queued (`skip_som`).
3. Fills empty candidate slots by drawing random pool entries until full or the pool is exhausted.
4. If the list still isn't full, the pool is empty, and SoM isn't excluded, grants **Spell of Mastery** as a candidate.
5. Sorts the list, and — from turn 2 on — prompts research selection (human dialog or AI picker) if nothing is being researched yet, then sets `research_cost_remaining` for the chosen spell.

Returns `spells_cnt` (final candidate count).

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `PreInit_Overland` (New/Load Game) | [LoadScr.c:1030](../../MoM/src/LoadScr.c#L1030) | Per-player loop during overland init — first population of `research_spells[]`. |
| `Next_Turn` (per-turn) | [NEXTTURN.c:3751](../../MoM/src/NEXTTURN.c#L3751) | Refreshes candidates each turn; return value used. |
| `Next_Turn` (post-learn) | [NEXTTURN.c:3773](../../MoM/src/NEXTTURN.c#L3773) | After a spell is learned and `research_spells[]` is cleared. |
| `WIZ_AddSpellRank__WIP` | [Spells128.c:1098](../../MoM/src/Spells128.c#L1098) | After granting a spell rank. |

On turn 1 the auto-select branch is skipped (`_turn > 1` guard), so the New-Game pass only populates and sorts the list.

## Structure

```mermaid
flowchart TD
    Entry(["Player_Research_Spells(player_idx)"])
    Setup["Build_Research_List → research_list[], m_spell_list_count"]
    SomKnown{"spells_list[SoM-1] == Known?"}
    SkipT["skip_som = TRUE"]
    Count["count candidates → spells_cnt;<br/>if any == SoM → skip_som = TRUE"]
    Fill["while(spells_cnt < 8 && pool > 0):<br/>idx = Random(pool)-1; fill first empty slot;<br/>mark Researchable; rebuild pool; break"]
    Recount["recount spells_cnt"]
    SomGrant{"spells_cnt<8 && pool==0 && !skip_som?"}
    Grant["grant Spell of Mastery to first empty slot"]
    Sort["Sort_Research_List"]
    AutoSel{"researching==NONE && spells_cnt>0 && turn>1?"}
    Human["Spell_Research_Select()"]
    AI["AI_Spell_Research_Select()"]
    Cost["set research_cost_remaining (SoM → som_research_cost,<br/>else spell_data_table[idx].research_cost)"]
    Ret(["return spells_cnt"])

    Entry --> Setup --> SomKnown
    SomKnown -->|yes| SkipT --> Fill
    SomKnown -->|no| Count --> Fill
    Fill --> Recount --> SomGrant
    SomGrant -->|yes| Grant --> Sort
    SomGrant -->|no| Sort
    Sort --> AutoSel
    AutoSel -->|yes, human| Human --> Cost --> Ret
    AutoSel -->|yes, AI| AI --> Cost --> Ret
    AutoSel -->|no| Ret
```

## Code walk

### Phase 1 — Setup + pool build ([64-71](../../MoM/src/Spells128.c#L64-L71))

`spells_max = NUM_RESEARCH_SPELLS` (8); `m_spell_list_count = 0`; allocate the 200-byte `research_list` scratch (`Near_Allocate_First`); `Build_Research_List` fills it and sets `m_spell_list_count`. (A `/* CLAUDE */` `RESEARCH_SETUP` diagnostic block follows at [73-86](../../MoM/src/Spells128.c#L73-L86) — debug-only.)

### Phase 2 — SoM-known check + candidate count ([89-106](../../MoM/src/Spells128.c#L89-L106))

```c
if(spells_list[spl_Spell_Of_Mastery - 1] == sls_Known) { skip_som = ST_TRUE; }   // SoM already learned
else {
    for(itr = 0; itr < spells_max; itr++)
        if(research_spells[itr] > spl_NONE) { spells_cnt++; if(== spl_Spell_Of_Mastery) skip_som = ST_TRUE; }
}
```

If SoM is already known, the count loop is skipped entirely (matches asm `jmp loc_ABACE`), leaving `spells_cnt == 0`. Otherwise existing candidates are counted and a queued SoM sets `skip_som`.

### Phase 3 — Fill loop ([109-135](../../MoM/src/Spells128.c#L109-L135))

```c
while((spells_cnt < spells_max) && (m_spell_list_count > 0)) {
    research_list_idx = (Random(m_spell_list_count) - 1);
    spells_cnt++;
    for(itr = 0; itr < spells_max; itr++) {
        if(research_spells[itr] == spl_NONE) {
            research_spells[itr] = research_list[research_list_idx];
            spell_realm_idx = ((research_list[research_list_idx] - 1) % NUM_SPELLS_PER_MAGIC_REALM);
            spell_realm     = ((research_list[research_list_idx] - 1) / NUM_SPELLS_PER_MAGIC_REALM);
            spells_list[(spell_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx] = sls_Researchable;
            Build_Research_List(player_idx, &research_list[0]);   // rebuild pool (chosen spell now Researchable)
            break;
        }
    }
}
```

Draws a random pool entry, drops it into the **first** empty candidate slot, marks that spell `sls_Researchable`, and rebuilds the pool. The `break` ([132](../../MoM/src/Spells128.c#L132)) is OG-faithful (asm `jmp loc_ABB79`) — one fill per draw; without it every empty slot would take the *same* `research_list_idx` (duplicates).

### Phase 4 — Recount + Spell-of-Mastery grant ([141-169](../../MoM/src/Spells128.c#L141-L169))

```c
spells_cnt = 0;
for(itr = 0; itr < spells_max; itr++)
    if(research_spells[itr] > spl_NONE) spells_cnt++;

if((spells_cnt < spells_max) && (m_spell_list_count == 0) && (skip_som == ST_FALSE)) {
    for(itr = 0; itr < spells_max; itr++) {
        if(research_spells[itr] == spl_NONE) { research_spells[itr] = spl_Spell_Of_Mastery; break; }
    }
}
```

Recount the live candidates. When the list can't be filled from the (now empty) pool and SoM isn't excluded, grant **Spell of Mastery** into the first empty slot. The grant loops from index 0 and `break`s after one assignment — matching the OG's `xor si,si` + loop + `jmp loc_ABBFF` ([asm:196-224](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr128/Player_Research_Spells.asm#L196-L224)).

### Phase 5 — Sort + auto-select + cost ([172-201](../../MoM/src/Spells128.c#L172-L201))

```c
Sort_Research_List(player_idx, spells_cnt);

if((researching_spell_idx == spl_NONE) && (spells_cnt > 0) && (_turn > 1)) {
    if(player_idx == HUMAN_PLAYER_IDX) Spell_Research_Select();
    else                              AI_Spell_Research_Select(player_idx);

    if(researching_spell_idx == spl_Spell_Of_Mastery)
        research_cost_remaining = som_research_cost;
    else
        research_cost_remaining = spell_data_table[researching_spell_idx].research_cost;
}
return spells_cnt;   // [205]
```

If nothing is being researched and there are candidates (turn ≥ 2), prompt the human dialog or run the AI picker, then initialize `research_cost_remaining` ([MOM_DAT.h:1503](../../MoX/src/MOM_DAT.h#L1503)) for the just-selected spell — SoM uses the wizard's `som_research_cost`, everything else the spell's `research_cost`. Matches the OG ([asm:252-284](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr128/Player_Research_Spells.asm#L252-L284)).

## Sub-functions / external calls

- **`Build_Research_List`** ([Spells128.c:218](../../MoM/src/Spells128.c#L218); WZD `o128p02`, drake178 `WIZ_GetResearchList`) — compiles the lowest-rarity available spell from each realm into `research_list[]`; sets `m_spell_list_count`.
- **`Sort_Research_List`** — orders the candidate list.
- **`Spell_Research_Select`** / **`AI_Spell_Research_Select`** — human dialog / AI picker that sets `researching_spell_idx`.
- **`Random(m_spell_list_count)`** — draws a pool index (1..n; the `- 1` makes it 0-based).
- **`Near_Allocate_First(200)`** — near-heap scratch arena for `research_list[]` (not freed; reset elsewhere — standard pattern).
- **`m_spell_list_count`** — the OG global `GUI_Multipurpose_Int`; eligible-pool size.

## Related references

- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr128\Player_Research_Spells.asm` / `.c` — IDA Pro 5.5 disassembly.
- [LoadScr.c:960 — `PreInit_Overland`](../../MoM/src/LoadScr.c#L960) — New/Load-Game caller.
- [Spells128.c:217 — `Build_Research_List`](../../MoM/src/Spells128.c#L217) — the pool builder.
