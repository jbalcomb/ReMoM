AISPELL-AITP_Node.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_AI_Node_Target_For_Spell.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\IDK_AI_Node_Target_For_Spell.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Cast_Warp_Node()                 // Spells129.c — AI branch (player_idx != HUMAN_PLAYER_IDX)
                    |-> AITP_Node(&node_idx, spl_Warp_Node)

---

# `AITP_Node` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Node` | [AISPELL.c:4703-4749](../../MoM/src/AISPELL.c#L4703-L4749) | AI target picker for **Warp Node**. Picks a random target owner (the human, or a random hostile opponent), then returns the **highest-`power`** node that owner holds **whose `NF_WARPED` flag is set**. Returns `ST_TRUE` with the chosen node index in `*targeted_node_idx`, `ST_FALSE` if none qualifies (or the spell isn't Warp Node / no hostile opponents). |

> **Status: BUILDS CLEAN — faithful to the asm (1:1), carrying one preserved OGBUG ([B1](#b1-ogbug--random-owner-index-ignores-the-hostile-opponents-list-can-self-target)).** The body ([AISPELL.c:4703](../../MoM/src/AISPELL.c#L4703)) is a faithful reconstruction of `IDK_AI_Node_Target_For_Spell.asm` (production renamed `IDK_AI_Node_Target_For_Spell` → `AITP_Node`; the asm/`.c` filename keeps the OG IDA name — the `IDK_` prefix marks the original reconstructor's uncertainty about the function). It compiles and links as part of AISPELL.c / momlib (the `Cast_Warp_Node` call site now correctly passes `&scsv1`). The asm is the authority.

## Purpose

`Cast_Warp_Node` routes the AI's **Warp Node** cast here to choose which node to target. The picker selects a target owner at random — half the time the human player (index 0), otherwise a pseudo-random owner index `1..count` that is *intended* to be one of the caster's hostile opponents but isn't (see [B1](#b1-ogbug--random-owner-index-ignores-the-hostile-opponents-list-can-self-target)) — then, among that owner's nodes, returns the highest-power one that already carries the `NF_WARPED` flag. The result is a **node index** (into `_NODES[]`), not a map square.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Cast_Warp_Node` | [Spells129.c:1033](../../MoM/src/Spells129.c#L1033) | AI branch only (`player_idx != HUMAN_PLAYER_IDX`); the human's Warp Node is targeted interactively. Calls `AITP_Node(&scsv1, spl_Warp_Node)`. |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `IDK_AI_Node_Target_For_Spell.asm`.

```c
if(spell_idx != spl_Warp_Node || _cp_hostile_opponent_count == 0)   // asm cmp arg_2,spl_Warp_Node / cmp _cp_hostile_opponent_count,0
    return ST_FALSE;                                                // asm  xor ax,ax

if(Random(2) == 1)  target_owner_idx = HUMAN_PLAYER_IDX;            // asm  Random(2)==1 → var_2 = 0
else                target_owner_idx = Random(_cp_hostile_opponent_count);   // asm  else var_2 = Random(count) — RAW ordinal 1..count, NOT _cp_hostile_opponents[…]; see B1

best_node_idx = ST_UNDEFINED;  max_power = 0;                       // asm  di = -1, var_4 = 0
for(node_idx = 0; node_idx < NUM_NODES; node_idx++)                // asm  si loop, cmp si,1Eh (30)
    if(_NODES[node_idx].owner_idx == target_owner_idx)             // asm  [+3] (cbw) == var_2
        if(_NODES[node_idx].power > max_power)                     // asm  [+4] (cbw) > var_4 (jle skip)
            if((_NODES[node_idx].flags & NF_WARPED) == NF_WARPED)  // asm  [+2Eh] & 1 == 1 (jnz skip)
            { max_power = _NODES[node_idx].power; best_node_idx = node_idx; }   // asm  var_4 = power, di = si

if(best_node_idx == ST_UNDEFINED) return ST_FALSE;                 // asm  cmp di,-1 / xor ax,ax
*targeted_node_idx = best_node_idx; return ST_TRUE;                // asm  [arg_0] = di, ax = 1
```

**Faithful 1:1** — the spell/opponent-count gate, the `Random(2)`-driven owner choice, the 30-node scan, the three nested gates (owner / strict-greater power / `NF_WARPED` set), and both returns all match the asm.

### Symbol / layout cross-checks (verified)

| Production | asm | Value / note |
|---|---|---|
| `_NODES[i]` stride | `imul 30h` | `sizeof(s_NODE) = 0x30 = 48`. |
| `_NODES[i].owner_idx` | `[es:bx+3]` (cbw) | `int8_t` at `+3`. |
| `_NODES[i].power` | `[es:bx+4]` (cbw) | `int8_t` at `+4`. |
| `_NODES[i].flags` | `[es:bx+2Eh]` | `int8_t` at `+2Eh` ([MOM_DAT.h:1803](../../MoX/src/MOM_DAT.h#L1803)). |
| `NF_WARPED` | `and ax,1 / cmp ax,1` | `1` ([MOM_DAT.h:452](../../MoX/src/MOM_DAT.h#L452)). |
| `NUM_NODES` | `cmp si,1Eh` | `30` ([MOM_DEF.h:111](../../MoX/src/MOM_DEF.h#L111)). |
| `HUMAN_PLAYER_IDX` | `mov var_2,0` | `0` ([MOM_DEF.h:392](../../MoX/src/MOM_DEF.h#L392)) — production names the literal `0` owner. |
| `Random(n)` / `_cp_hostile_opponent_count` | `call Random` | hostile-opponent count drives the random owner pick. |

## B1 (OGBUG) — random owner index ignores the hostile-opponents list (can self-target)

The else-branch sets `target_owner_idx = Random(_cp_hostile_opponent_count)` ([AISPELL.c:4721](../../MoM/src/AISPELL.c#L4721)) and then compares it **directly** to `_NODES[i].owner_idx`. But `Random(n)` returns `1..n`, so this is a raw **ordinal** in `1..count`, not an actual player index.

`Player_Hostile_Opponents` ([AIDUDES.c:1678-1704](../../MoM/src/AIDUDES.c#L1678-L1704)) builds a companion array `_cp_hostile_opponents[]` holding the **real player indices** of the caster's hostile opponents — explicitly **excluding the caster** (`player_idx != itr_players`, [1692](../../MoM/src/AIDUDES.c#L1692); doc note "The caller itself is always excluded", [1673](../../MoM/src/AIDUDES.c#L1673)). The intended code was almost certainly `target_owner_idx = _cp_hostile_opponents[Random(count) - 1]`. `AITP_Node` never reads that array — confirmed in the asm (`loc_E885C`: `push count / call Random / mov var_2, ax`, no array access anywhere in the proc), and production matches.

Consequences:
- **Self-target.** If the caster's player index `N` is within `1..count`, `Random(count)` can equal `N`, so the picker selects the **caster's own** highest-power warped node. The list's caster-exclusion is defeated because the list is never consulted. (Concrete: caster = player 1 with ≥1 hostile opponent → `Random(count)` can roll `1` → targets itself.)
- **Mis-targets.** `1..count` is a contiguous ordinal range, so it can hit non-hostile players and will miss hostile players whose index exceeds `count`; it bears no relation to the actual `_cp_hostile_opponents[]` membership.

The 50% branch hard-codes owner `0` (`HUMAN_PLAYER_IDX`) — same "ignore the list" class, but the caster is never the human (the `Cast_Warp_Node` guard), so that branch can't self-target. Faithfully preserved (B-bug); do not "fix" in the baseline. The OG `IDK_` name fits — the original reconstructor flagged the function as not-understood.

## OG quirks (preserve when reconstructing)

- **[B1](#b1-ogbug--random-owner-index-ignores-the-hostile-opponents-list-can-self-target)** — the random owner index is a raw `1..count` ordinal, not a lookup into `_cp_hostile_opponents[]`; can mis-target and self-target.
- **Targets only nodes that already have `NF_WARPED` set** — the scan requires `(flags & NF_WARPED) == NF_WARPED` ([4733](../../MoM/src/AISPELL.c#L4733)), i.e. the node must *already* be warped. That is unexpected for a spell that *warps* a node (one might expect it to seek un-warped enemy nodes). The asm tests the bit set unambiguously (`and ax,1 / cmp ax,1 / jnz skip`), and the OG name carries the `IDK_` ("I don't know") prefix — so this is preserved as-is, whether an original design quirk or a meaning of `NF_WARPED` that isn't obvious. Do not "correct" it.

## Sub-functions / external calls

- **`Random(2)`**, **`Random(_cp_hostile_opponent_count)`** — `Random(n)` returns `1..n` ([random.c:296](../../MoX/src/random.c#L296)).
- Reads `_NODES[]`, `_cp_hostile_opponent_count`; writes through `*targeted_node_idx`. Note it does **not** read the companion `_cp_hostile_opponents[]` list (built by `Player_Hostile_Opponents`, [AIDUDES.c:1678](../../MoM/src/AIDUDES.c#L1678)) — see [B1](#b1-ogbug--random-owner-index-ignores-the-hostile-opponents-list-can-self-target).

## Related references

- `…\ovr156\IDK_AI_Node_Target_For_Spell.asm` — IDA Pro 5.5 disassembly (the authority; OG name `IDK_AI_Node_Target_For_Spell`).
- `…\ovr156\IDK_AI_Node_Target_For_Spell.c` — Piethawn IDA-C.
- `Cast_Warp_Node` ([Spells129.c:1020](../../MoM/src/Spells129.c#L1020)) — the sole caller; AI branch picks the node, human branch targets interactively.
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent overland spell-AI dispatcher.
