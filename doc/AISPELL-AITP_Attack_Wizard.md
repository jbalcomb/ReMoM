AISPELL-AITP_Attack_Wizard.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_AITP_Target_Wizard__STUB.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_AITP_Target_Wizard__STUB.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                case scc_Target_Wizard:   // 6  (Cruel Unminding, Drain Power, Spell Blast, Subversion)
                    |-> AITP_Attack_Wizard(&spell_target_idx, spell_idx, player_idx)

---

# `AITP_Attack_Wizard` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Attack_Wizard` | [AISPELL.c:6183-6225](../MoM/src/AISPELL.c#L6183-L6225) | AI target-picker / go-no-go for the four **wizard-disruption** spells (Drain Power, Subversion, Cruel Unminding, Spell Blast). Picks an enemy wizard into `*targeted_player_idx` and returns `ST_TRUE` if that wizard is mid-cast and the caster can afford to disrupt now, else `ST_FALSE`. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying one preserved OG quirk ([B1](#b1-ogbug--50-branch-targets-_cp_hostile_opponents0-not-the-human)).** The body ([AISPELL.c:6183](../MoM/src/AISPELL.c#L6183)) is a faithful reconstruction of `IDK_AITP_Target_Wizard__STUB.asm` (production renamed `IDK_AITP_Target_Wizard__STUB` → `AITP_Attack_Wizard`; the asm/`.c` filename keeps the OG IDA name). It compiles and links as part of AISPELL.c / momlib. The opponent-count gate, the four-spell set, the `Random(2)` target pick (written through `*targeted_player_idx`), the casting-progress test, and all returns match the asm. The asm is the authority.

## Purpose

`Cast_Spell_Overland` routes the **Target-Wizard** spell category (Cruel Unminding, Drain Power, Spell Blast, Subversion) here. The routine (a) chooses *which* enemy wizard to hit — half the time the first hostile opponent, half the time a random one — and writes it to `*targeted_player_idx`, then (b) decides whether casting **now** is worthwhile: only if the chosen wizard is currently casting a spell, and the mana it has **already** sunk into that spell is within the caster's affordable budget (`mana_reserve - 50`). The boolean return is the go/no-go.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Cast_Spell_Overland` | [OverSpel.c:1459](../MoM/src/OverSpel.c#L1459) | `case scc_Target_Wizard` (6), AI branch only (`player_idx != HUMAN_PLAYER_IDX`). Passes `&spell_target_idx`; on `ST_TRUE` it reads that back as the target wizard and applies the effect (e.g. Spell Blast drains that wizard's mana). |

## Code walk

Line refs are production [AISPELL.c](../MoM/src/AISPELL.c); the authority is `IDK_AITP_Target_Wizard__STUB.asm`.

```c
if(_cp_hostile_opponent_count == 0) return ST_FALSE;                    // 6186  asm cmp count,0 / xor ax,ax

if(spell_idx == spl_Drain_Power || spell_idx == spl_Subversion ||       // 6191  asm cmp di,<spl_*> chain
   spell_idx == spl_Cruel_Unminding || spell_idx == spl_Spell_Blast)
{
    if(Random(2) == 1) *targeted_player_idx = _cp_hostile_opponents[0];                          // 6198  asm mov ax,[_cp_hostile_opponents]  [B1]
    else               *targeted_player_idx = _cp_hostile_opponents[Random(_cp_hostile_opponent_count) - 1];   // 6202  asm [AI_Own_Stack_Count + Random(count)*2]
                                                                        // asm loc_EA493: mov [si], ax  (the write)

    if(_players[*targeted_player_idx].casting_spell_idx == spl_NONE) return ST_FALSE;   // 6205  asm casting_spell_idx == 0

    if((spell_data_table[_players[*targeted_player_idx].casting_spell_idx].casting_cost     // 6210  progress = cost - remaining
        - _players[*targeted_player_idx].casting_cost_remaining)
       <= (_players[player_idx].mana_reserve - 50))                     // limit = mana_reserve - 50;  asm cmp dx,ax / jle
        return ST_TRUE;
    else
        return ST_FALSE;
}
else { Cast_Spell_Target_Error(spell_idx); return ST_FALSE; }           // 6219  asm loc_EA4F4
```

**Faithful 1:1** — the opponent-count gate, the four-spell test, the `Random(2)` pick written to `*targeted_player_idx` (asm `loc_EA493: mov [si], ax`, then re-reads `[si]`), the proper `_cp_hostile_opponents[]` indexing, the `casting_spell_idx == 0` early-out, the `progress <= limit` test (asm `cmp dx,ax / jle`), and the error path all match. The asm carries the `progress`/`limit`/`target_spell_cost` values as anonymous `push`/`pop` stack temporaries; production inlines them into the one comparison (no named locals), which maps directly to the `sub` / `add -50` / `cmp` / `jle`.

## B1 (OGBUG) — 50% branch targets `_cp_hostile_opponents[0]`, not the human

When `Random(2) == 1`, the target is `_cp_hostile_opponents[0]` — the **first** hostile opponent in the list — not the human player. The asm is unambiguous (`mov ax, [_cp_hostile_opponents]`), so it is preserved as-is. An in-code `OGBUG` note ([6198](../MoM/src/AISPELL.c#L6198)) hypothesizes it *should* be `HUMAN_PLAYER_IDX` (by analogy to [`AITP_Node`](ComputerPlayer/AISPELL-AITP_Node.md), whose 50% branch does target the human). That's a plausible reading of original intent but unproven — the faithful reconstruction keeps `_cp_hostile_opponents[0]`. Note this function **does** index the hostile-opponents list correctly in its other branch (unlike `AITP_Node`'s B1), so it cannot self-target.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_cp_hostile_opponents[]` / `_cp_hostile_opponent_count` | `_cp_hostile_opponents` / `_cp_hostile_opponent_count` | built by `Player_Hostile_Opponents` ([AIDUDES.c:1678](../MoM/src/AIDUDES.c#L1678)). |
| `_cp_hostile_opponents[Random(n)-1]` | `[AI_Own_Stack_Count + Random(n)*2]` | faithful: `_cp_hostile_opponents` directly follows `AI_Own_Stack_Count` in DSEG, and `Random(n) ∈ 1..n` ([random.c:296](../MoX/src/random.c#L296)). |
| `spl_NONE` | `0` | "not casting" sentinel ([Spellbook.h:26](../MoM/src/Spellbook.h#L26)). |
| `_players[w].casting_spell_idx` / `.casting_cost_remaining` / `.mana_reserve` | `s_WIZARD` fields | `imul size s_WIZARD`. |
| `spell_data_table[s].casting_cost` | `s_SPELL_DATA.casting_cost` | `imul size s_SPELL_DATA`. |
| `mana_reserve - 50` | `add ax, -50` | the affordability slack. |
| four spells | `spl_Drain_Power` / `spl_Subversion` / `spl_Cruel_Unminding` / `spl_Spell_Blast` | the `scc_Target_Wizard` set. |

## Sub-functions / external calls

- **`Random(2)`**, **`Random(_cp_hostile_opponent_count)`**, **`Cast_Spell_Target_Error(spell_idx)`** ([AISPELL.h:222](../MoM/src/AISPELL.h#L222)).
- Reads `_cp_hostile_opponents[]`, `_cp_hostile_opponent_count`, `_players[]`, `spell_data_table[]`; writes the chosen wizard through `*targeted_player_idx`.

## Related references

- `…\ovr156\IDK_AITP_Target_Wizard__STUB.asm` — IDA Pro 5.5 disassembly (the authority; OG name `IDK_AITP_Target_Wizard__STUB`).
- `…\ovr156\IDK_AITP_Target_Wizard__STUB.c` — Piethawn IDA-C.
- [ComputerPlayer/AISPELL-AITP_Node.md](ComputerPlayer/AISPELL-AITP_Node.md) — sibling random-opponent picker; contrast its B1 (raw-ordinal self-target) with this function's correct `_cp_hostile_opponents[]` indexing.
- [ComputerPlayer/AISPELL-AI_Spell_Select.md](ComputerPlayer/AISPELL-AI_Spell_Select.md) — the parent overland spell-AI dispatcher.
