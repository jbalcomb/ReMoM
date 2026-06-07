SETTLE-AI_Execute_Orders.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_MoveUnits__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_MoveUnits__WIP.c

AI_Execute_Orders()
    |-> AI_UNIT_BuildRoad__WIP()  + AI_Unit_Army_Do_Move()   (us_BuildRoad)
    |-> AI_Unit_Army_Do_Move()                                (us_GOTO, us_Move)
    |-> AI_UNIT_Meld()                                (us_Meld)
    |-> AI_Unit_Army_Do_Settle()                         (us_Settle)
    |-> AI_UNIT_SeekTransprt__WIP()                   (us_Ferry)
    |-> AI_Metrics_Emit_Unit_Outcome()               (ReMoM instrumentation)

---

# AI_Execute_Orders — Walkthrough

**The execution half of the AI turn.** The AI turn runs in two phases:

1. **Set orders** — [`AI_Set_Unit_Orders`](AIMOVE-AI_Set_Unit_Orders.md) and its slot dispatchers (Meld / Settle / Purify / RoadBuild / combat / …) decide what each unit should do and **stamp it onto the unit**: `_UNITS[unit_idx].Status` plus the relevant destination fields (`dst_wx/wy`, `Rd_From_X/Y`, …), consuming the `_ai_own_stack_*` slots as they go. That phase is **stack-centric**.
2. **Execute orders — `AI_Execute_Orders` (this).** Walk **every owned unit** in the raw `_UNITS[]` array and run the handler for whatever `Status` it now carries.

By the time control reaches here, the "AI stack" abstraction is **gone**: `AI_Execute_Orders` never touches `_ai_own_stack_*` / `_ai_all_own_stacks` / `AICAP_*`. It is a flat, **1:1 dispatch on `_UNITS[unit_idx].Status`** — the orders were already baked into each unit, so this just carries them out.

**Location:** [MoM/src/SETTLE.c:113](../../MoM/src/SETTLE.c#L113) (~91 lines, ends [line 204](../../MoM/src/SETTLE.c#L204); Doxygen header at [90-112](../../MoM/src/SETTLE.c#L90-L112)). **WZD overlay:** ovr100, p01. **OG/IDA name:** `AI_Execute_Orders` (asm proc `AI_Execute_Orders__WIP` — the `__WIP` is the project's not-yet-fully-reconstructed marker). **GEMINI variant:** present ([line 206](../../MoM/src/SETTLE.c#L206)) — see comparison; do not trust its case numbers.

## Pipeline position

```
AI_Next_Turn ... (per AI player) AIDUDES.c
  ├─ AI_Set_Unit_Orders(player_idx)        [AIDUDES.c:301]   ◄── SET   (stack-centric; writes _UNITS[].Status)
  └─ AI_Execute_Orders(player_idx)              [AIDUDES.c:343]   ◄── EXECUTE (this; per-unit Status dispatch)
... then once for neutrals:
  AI_Execute_Orders(NEUTRAL_PLAYER_IDX)         [AIDUDES.c:362]
```

## Signature

```c
void AI_Execute_Orders(int16_t player_idx);
```

`AI_Execute_Orders` callers:
- [AIDUDES.c:343](../../MoM/src/AIDUDES.c#L343) — once per AI player, right after that player's `AI_Set_Unit_Orders`.
- [AIDUDES.c:362](../../MoM/src/AIDUDES.c#L362) — once for `NEUTRAL_PLAYER_IDX` (neutral-owned units).

## What it does

### 1. Save map view + Time-Stop gate ([lines 121-136](../../MoM/src/SETTLE.c#L121-L136))

```c
l_map_x = _map_x;  l_map_y = _map_y;  l_map_plane = _map_plane;   // restored at the end

if (g_timestop_player_num > 0)
{
    if ((player_idx + 1) != g_timestop_player_num) { return; }     // only the Time-Stop caster moves
}
```

`g_timestop_player_num` is 1-indexed, so the casting player is `player_idx + 1`. While Time Stop is up, every other player returns immediately and processes no units. **IDA-confirmed** (asm `loc_81E22`: `cmp g_timestop_player_num,0; jle skip; … cmp player_idx+1, g_timestop_player_num; jz skip; else return`).

> **`¿ OGBUG redundant ?`** ([comment at line 126](../../MoM/src/SETTLE.c#L126)) — this Time-Stop guard sits inside a per-player loop that may already gate on Time Stop upstream, so it could be redundant. The OG asm has it here regardless, so it is preserved as-is (OG-faithful).

### 2. Per-unit Status dispatch ([lines 139-195](../../MoM/src/SETTLE.c#L139-L195))

```c
for (unit_idx = 0; unit_idx < _units; unit_idx++)
{
    if (_UNITS[unit_idx].owner_idx == (int8_t)player_idx)
    {
        // snapshot for metrics (ReMoM only): pre_wx/wy, pre_status, pre_dst_wx/wy
        switch (_UNITS[unit_idx].Status) { ... }
        // emit metrics if pre_status was GOTO / Move / BuildRoad
    }
}
```

Flat scan of all `_units` filtered by `owner_idx == player_idx`, then dispatch on `Status`. The switch is the whole point — it matches the OG jump table (`bx = Status - 2; if (bx > 14) skip; jmp jt_ai_unit_status[bx]`) **exactly**:

| `Status` (value) | Handler | asm label |
|---|---|---|
| `us_BuildRoad` (2) | `AI_UNIT_BuildRoad__WIP(unit_idx)` **then** `AI_Unit_Army_Do_Move(unit_idx)` (fall-through) | `sw_aius_02` → `sw_aius_16` |
| `us_GOTO` (3) | `AI_Unit_Army_Do_Move(unit_idx)` | `sw_aius_03` → `sw_aius_16` |
| `us_Meld` (9) | `AI_UNIT_Meld(unit_idx)` | `sw_aius_09` |
| `us_Settle` (10) | `AI_Unit_Army_Do_Settle(unit_idx)` | `sw_aius_10` (OG `AI_UNIT_Settle`) |
| `us_Ferry` (11) | `AI_UNIT_SeekTransprt__WIP(unit_idx)` | `sw_aius_11` |
| `us_Move` (16) | `AI_Unit_Army_Do_Move(unit_idx)` | `sw_aius_16` |
| everything else (4-8, 12-15) | `default:` — do nothing | falls to loop tail |

Notes:
- `us_BuildRoad` does the road work **then falls through to Move** so the engineer also advances — faithful to the asm `sw_aius_02` → `jmp sw_aius_16`.
- `us_GOTO` and `us_Move` both just `AI_Unit_Army_Do_Move` (both are "auto-move to `dst_wx/wy`"; `us_Move` additionally implies a hostile target was flagged when the order was set).
- The `us_Settle` callee `AI_Unit_Army_Do_Settle` ([SETTLE.c:332](../../MoM/src/SETTLE.c#L332)) is the **ReMoM rename** of the OG `AI_UNIT_Settle`; `__WIP` callees are in-progress reconstructions of the OG names. None of these are substitutions — they are the same functions.

### 3. ReMoM metrics ([lines 144-193](../../MoM/src/SETTLE.c#L144-L193))

The pre-action snapshot and `AI_Metrics_Emit_Unit_Outcome(...)` call are **ReMoM instrumentation with no OG counterpart** — emitted only for units whose pre-status was `us_GOTO` / `us_Move` / `us_BuildRoad` (the movement orders), capturing intended destination vs. final position and `Move_Failed`.

### 4. Restore map view ([lines 197-202](../../MoM/src/SETTLE.c#L197-L202))

```c
_map_x = l_map_x;  _map_y = l_map_y;
_prev_world_x = _map_x;  _prev_world_y = _map_y;
_map_plane = l_map_plane;
```

Unit processing scrolls the map view as a side effect; this restores the pre-call view (and re-points `_prev_world_*` at it). **IDA-confirmed** (asm tail `loc_81E8C`).

## Production vs GEMINI

GEMINI ([line 206](../../MoM/src/SETTLE.c#L206)) is structurally the same flat Status switch, but its **case values are wrong** — it uses `case 4 / 5 / 11 / 12 / 13` where the real `us_*` values are `2 / 3 / 9 / 10 / 11`; only `case 16` (Move) is correct. Its "statuses that do nothing" comment (`2,3,6,7,8,10,14,15`) is likewise wrong (it lists handled statuses as skipped). Production's named-enum cases and its "4-8, 12-15 skipped" comment are the accurate ones. Another **GEMINI-is-not-ground-truth** case — verify against the `us_*` enum ([MOM_DAT.h:516-530](../../MoX/src/MOM_DAT.h#L516-L530)), not GEMINI's literals. GEMINI also calls the OG names (`AI_UNIT_BuildRoad` / `AI_UNIT_Settle` / `AI_UNIT_SeekTransprt`) rather than the current reconstruction names.

## Bug catalog

| # | Where | Issue | Severity |
|---|---|---|---|
| Note | [line 126](../../MoM/src/SETTLE.c#L126) | `¿OGBUG?` — the Time-Stop guard may duplicate an upstream per-player Time-Stop gate. OG has it inline; preserved. | OG-faithful; possibly redundant |

The function is otherwise an exact reconstruction of the asm dispatch — no reconstruction bugs.

## ASCII summary

```
AI_Execute_Orders(player_idx)            ── EXECUTE orders already stamped on _UNITS[].Status
  save _map_x/_map_y/_map_plane
  if Time Stop up and player_idx+1 != caster:  return
  for unit_idx in 0.._units:
      if _UNITS[unit_idx].owner_idx == player_idx:
          (snapshot pre-state for metrics)
          switch(_UNITS[unit_idx].Status):                 ── 1:1, no AI-stack concept here
              us_BuildRoad(2): AI_UNIT_BuildRoad__WIP; ↓ (fall through)
              us_GOTO(3) / us_Move(16): AI_Unit_Army_Do_Move
              us_Meld(9):   AI_UNIT_Meld
              us_Settle(10): AI_Unit_Army_Do_Settle     (= OG AI_UNIT_Settle)
              us_Ferry(11):  AI_UNIT_SeekTransprt__WIP
              default (4-8,12-15): nothing
          if pre_status in {GOTO,Move,BuildRoad}: AI_Metrics_Emit_Unit_Outcome   (ReMoM)
  restore _map_x/_map_y/_prev_world_x/_prev_world_y/_map_plane
```

## Related references

- [AIMOVE-AI_Set_Unit_Orders.md](AIMOVE-AI_Set_Unit_Orders.md) — the **set** phase that writes the `Status` values this function executes
- [AIMOVE-AI_Do_Meld.md](AIMOVE-AI_Do_Meld.md) — sets `us_Meld`; this dispatches it to `AI_UNIT_Meld` ([SETTLE.c:297](../../MoM/src/SETTLE.c#L297))
- [AIMOVE-AI_Stacks_Do_Purify.md](AIMOVE-AI_Stacks_Do_Purify.md), [AIMOVE-AI_Stacks_Do_RoadBuild.md](AIMOVE-AI_Stacks_Do_RoadBuild.md) — set `us_Purify`/`us_BuildRoad`/move orders
- [MoM-AI-Move-ai_own_stack.md](MoM-AI-Move-ai_own_stack.md) — the stack structures used by the **set** phase (and deliberately absent here)
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_Execute_Orders__WIP.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
