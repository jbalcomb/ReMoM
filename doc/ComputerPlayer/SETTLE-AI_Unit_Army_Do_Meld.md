SETTLE-AI_Unit_Army_Do_Meld.md

Next_Turn_Proc()
|-> Next_Turn_Calc()
    |-> AI_Next_Turn()
         |-> AI_Execute_Orders()
            |-> AI_Unit_Army_Do_Meld()
                |-> Army_Do_Meld

Elsewhere, ...
    Active_Army_Do_Meld()
        Active_Unit_Stack(&troop_count, &troops[0]);
        Army_Do_Meld(troop_count, &troops[0]);
    ~==
        Active_Army_Do_Settle()
            Active_Unit_Stack(&troop_count, &troops[0]);
            return Army_Do_Settle(troop_count, &troops[0]);

---

# AI_Unit_Army_Do_Meld && Army_Do_Meld — Walkthrough

Two functions: `AI_Unit_Army_Do_Meld` is the AI-turn leaf that gathers the melder's stack and hands it to the shared node-meld routine `Army_Do_Meld`, which does the actual node-ownership resolution (and kills the spirit).

> **Naming:** the disassembly files keep the OG/IDA names (`ovr100\AI_UNIT_Meld.asm`, `ovr061\STK_DoMeldWithNode.asm`). ReMoM has renamed all three pieces: the AI leaf `AI_UNIT_Meld` → `AI_Unit_Army_Do_Meld` (`o100p02`); the shared core `STK_DoMeldWithNode` → `Army_Do_Meld(troop_count, troops)`; and the human-UI wrapper `STK_MeldWithNode` → `Active_Army_Do_Meld()` — paralleling the `Army_Do_Settle` / `Active_Army_Do_Settle` pair.

## Two callers of `Army_Do_Meld`

```
AI:     AI_Execute_Orders → AI_Unit_Army_Do_Meld(unit_idx)  → Player_Army_At_Square → Army_Do_Meld
human:  (special action)  → Active_Army_Do_Meld()           → Active_Unit_Stack     → Army_Do_Meld
```

`Army_Do_Meld` is the shared core; the AI path ([SETTLE.c:256](../../MoM/src/SETTLE.c#L256)) and the human path ([UNITSTK.c:1647](../../MoM/src/UNITSTK.c#L1647), inside `Active_Army_Do_Meld` at [1642](../../MoM/src/UNITSTK.c#L1642)) just differ in how they gather the stack.

---

## `AI_Unit_Army_Do_Meld` — [SETTLE.c:234](../../MoM/src/SETTLE.c#L234)

```c
void AI_Unit_Army_Do_Meld(int16_t unit_idx);     // o100p02
```

Called from [`AI_Execute_Orders`](SETTLE-AI_Execute_Orders.md), the `us_Meld` case ([SETTLE.c:170](../../MoM/src/SETTLE.c#L170)).

```c
if(_UNITS[unit_idx].Finished == ST_TRUE) { return; }              // [244]
/* extract wx/wy/wp/owner from _UNITS[unit_idx] */                // [249-252]
Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);   // gather stack [254]
Army_Do_Meld(troop_count, troops);                                // do the meld [256]
for(itr_troops … troop_count) _UNITS[troops[itr_troops]].Status = us_Ready;   // [258-261]
```

**IDA-confirmed faithful** to [ovr100/AI_UNIT_Meld.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr100/AI_UNIT_Meld.asm) — gate, extract, `Player_Army_At_Square`, `Army_Do_Meld`, then mark every gathered troop `us_Ready`. (Dead local `_unit_idx` at [241](../../MoM/src/SETTLE.c#L241) — not in the asm, harmless.) The final `us_Ready` sweep is the drake178-noted behavior that overwrites busy statuses on the whole square.

## `Army_Do_Meld` — [UNITSTK.c:1677](../../MoM/src/UNITSTK.c#L1677)

```c
void Army_Do_Meld(int16_t troop_count, int16_t troops[]);
```

Resolves a melder stack against the magic node under it: pick the melder, decide whether it takes the node, flip ownership, and consume the spirit.

```c
if(troop_count < 1) { return; }                                          // [1689]
unit_idx = troops[0];
node_idx = Get_Map_Square_Magic_Node(_UNITS[unit_idx].{wx,wy,wp});       // [1696]
if(_NODES[node_idx].flags & NF_WARPED) { return; }                      // warped node → bail [1698]
unit_owner = _UNITS[unit_idx].owner_idx;  node_owner = _NODES[node_idx].owner_idx;

/* pick the melder: prefer a Guardian Spirit (also flags node_has_garrison) */  // [1709-1726]
node_has_garrison = ST_FALSE;  melder_unit_idx = ST_UNDEFINED;
for each troop with UA_MELD:
    if type == Guardian Spirit: melder = troop; node_has_garrison = TRUE; break;   // [1715-1721]
    else                       melder = troop;                                     // keep scanning
if(melder_unit_idx == ST_UNDEFINED) { return; }                         // no melder [1727]

/* decide if the meld takes the node */                                  // [1733-1764]
did_take_node = ST_FALSE;
if(node_owner == ST_UNDEFINED || node_owner == unit_owner)               // unowned or already ours
    did_take_node = ST_TRUE;
else if((_NODES[node_idx].flags & NF_GUARDIAN) == 0)                     // enemy node, no guardian
    did_take_node = ST_TRUE;
else {                                                                    // enemy node WITH a guardian
    percentage = Random(100);
    if(percentage <= 25) { did_take_node = ST_TRUE; flags ^= NF_GUARDIAN; }   // 25% → take + clear guardian
    else if(unit_owner == _human_player_idx) Warn0(_msg_guardian_kills_spirit); // 75% → fail (warn humans only)
}

if(did_take_node) {                                                      // [1766-1776]
    _NODES[node_idx].owner_idx = unit_owner;
    if(node_has_garrison) _NODES[node_idx].flags |= NF_GUARDIAN;         // a Guardian Spirit re-garrisons
}
Kill_Unit(melder_unit_idx, kt_Dismissed);                               // spirit is consumed either way [1777]
```

**IDA-confirmed faithful** to [ovr061/STK_DoMeldWithNode.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr061/STK_DoMeldWithNode.asm):

| Step | Line | asm |
|---|---|---|
| `troop_count < 1` → return | [1689](../../MoM/src/UNITSTK.c#L1689) | `cmp troop_count,1; jge …` ✓ |
| magic-node lookup | [1696](../../MoM/src/UNITSTK.c#L1696) | `call j_Get_Map_Square_Magic_Node` ✓ |
| `NF_WARPED` → return | [1698](../../MoM/src/UNITSTK.c#L1698) | `test flags, NF_WARPED; jz …` ✓ |
| melder / Guardian-Spirit pick | [1709-1726](../../MoM/src/UNITSTK.c#L1709-L1726) | `loc_5495E` (`UA_MELD`, then type==`spl_Guardian_Spirit` Param0) ✓ |
| no melder → return | [1727](../../MoM/src/UNITSTK.c#L1727) | `cmp Spirit_Unit_Index, e_ST_UNDEFINED` ✓ |
| owner-match → take | [1735-1742](../../MoM/src/UNITSTK.c#L1735-L1742) | `loc_549CB`/`loc_549DE` ✓ |
| guardian roll (25%) | [1744-1763](../../MoM/src/UNITSTK.c#L1744-L1763) | `loc_549E5`: `Random(100); cmp 25; jg` — success only on `<=25`; human check gates only the `Warn0` ✓ |
| take → set owner + re-garrison | [1766-1776](../../MoM/src/UNITSTK.c#L1766-L1776) | `loc_54A53` ✓ |
| `Kill_Unit(melder, kt_Dismissed)` | [1777](../../MoM/src/UNITSTK.c#L1777) | `push 1; push Spirit_Unit_Index; call j_Kill_Unit` ✓ |

### Behaviour notes

- **Guardian-node odds are symmetric**: an enemy node defended by a Guardian Spirit is taken only on `Random(100) <= 25` (25%), for **both** AI and human melders. The `unit_owner == _human_player_idx` check decides solely whether the "Guardian Spirit kills your spirit" message is shown — it does **not** change the outcome.
- **The spirit is always consumed** (`Kill_Unit` runs regardless of `did_take_node`): a melder that loses the roll dies for nothing; one that wins flips the node and (if it was a Guardian Spirit) re-flags `NF_GUARDIAN` to garrison it.
- **Guardian Spirit is preferred as the melder** (the loop `break`s on it) and is what sets `node_has_garrison`, so a winning Guardian Spirit leaves the node guarded for the new owner.

## Related references

- [SETTLE-AI_Execute_Orders.md](SETTLE-AI_Execute_Orders.md) — dispatcher; the `us_Meld` case calls `AI_Unit_Army_Do_Meld`
- [AIMOVE-AI_Do_Meld.md](AIMOVE-AI_Do_Meld.md) — the order-*setter* that assigns `us_Meld` (and the human/AI call-paths into `Army_Do_Meld`)
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_UNIT_Meld.asm` — AI leaf disassembly (ground truth, verified)
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr061\STK_DoMeldWithNode.asm` — core meld disassembly (ground truth, verified)
