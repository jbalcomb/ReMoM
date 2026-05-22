# AI Turn Diagrams

Mermaid PoC. Renders inline in GitHub-flavored markdown, VS Code's preview (with the **Markdown Preview Mermaid Support** extension), and most other markdown renderers. Falls back to a code-fenced block in raw / unrendered views.

If a diagram doesn't render in your viewer, look at the inline source — the relationships are intended to be readable as text too.

---

## Per-turn driver (per AI player)

Where `AI_Set_Unit_Orders` and `AI_MoveUnits` fit in `AI_Next_Turn`:

```mermaid
sequenceDiagram
    autonumber
    participant Driver as AI_Next_Turn (AIDUDES.c)
    participant Choose as AI_Choose_War_Landmass
    participant Eval as AI_Evaluate_Continents
    participant Orders as AI_Set_Unit_Orders
    participant Move as AI_MoveUnits

    Note over Driver: per AI player_idx
    Driver->>Choose: AI_Choose_War_Landmass(player_idx)
    Note over Choose: reads _ai_continents.type_array (stale from last turn)<br/>writes _ai_landmass_war_targets[wp][player_idx]
    Driver->>Eval: AI_Evaluate_Continents(player_idx)
    Note over Eval: rewrites _ai_continents.type_array<br/>(preserves lmt_NoTargets, clobbers lmt_Abandon)
    Driver->>Orders: AI_Set_Unit_Orders(player_idx)
    Note over Orders: per (plane, landmass) dispatch<br/>writes _UNITS.Status + dst_wx/wy
    Driver->>Move: AI_MoveUnits(player_idx)
    Note over Move: reads _UNITS.Status<br/>dispatches to per-status handlers
```

The order is strict: `Orders` writes Status + dst, then `Move` reads Status + dst. No interleaving. (See `AIDUDES.c:241/284/285/327` for the call sites.)

---

## Order phase → Movement phase: how unit assignments reach `AI_UNIT_Move`

```mermaid
flowchart TD
    classDef phase fill:#f5f5f5,stroke:#666,stroke-width:2px,color:#000
    classDef func fill:#e1f5ff,stroke:#0066cc,color:#000
    classDef state fill:#fff3cd,stroke:#856404,color:#000
    classDef terminal fill:#d4edda,stroke:#28a745,color:#000
    classDef gate fill:#f8d7da,stroke:#721c24,color:#000

    subgraph PhaseOrders["ORDER PHASE — AI_Set_Unit_Orders(player_idx)"]
        direction TB
        Dispatch["per plane × per landmass dispatch loop"]:::func

        Dispatch --> Slot1["slot 1<br/>AI_Stacks_Init_Build_Target_Order>rebuilds _ai_own_stack_*</i>"]:::func
        Dispatch --> Slot2["slot 2<br/>AI_GarrBuilderPush__WIP<br/><i>push settlers/engineers/melders out of garrisons</i>"]:::func
        Dispatch --> Slots4_7["slots 4-7<br/>AI_Do_Meld<br/>AI_Do_Settle<br/>AI_Do_Purify<br/>AI_Do_RoadBuild"]:::func
        Dispatch --> Slot8["slot 8<br/>AI_Build_Target_List<br/>builds _ai_targets_*"]:::func
        Dispatch --> Slot9["slot 9<br/>AI_Stacks_Roamers_Target_Or_Deploy"]:::func
        Dispatch --> SlotsRally["slots 10/11/13/14<br/>AI_PullForMainWar<br/>G_AI_HomeRallyFill<br/>G_AI_RallyFill<br/>AI_FillGarrisons"]:::func

        Slot1 -. "Phase 3 only<br/>(first unit in us_Move)" .-> FindNearest["AI_Stacks_Target_Nearest_Hostile_Stack<br/>nearest hostile free-roaming stack on landmass"]:::func
        FindNearest --> SetTgt

        Slot2 --> SetTgt
        Slot9 --> AssignTgt["AI_Stacks_Assign_Target<br/>scores _ai_targets_* by value/distance<br/>filters by strength + reachability"]:::func
        AssignTgt --> SetTgt
        SlotsRally --> SetTgt

        SetTgt["AI_Stacks_Order_Attack_Target_Or_Goto_Destination<br/><br/>reads g_ai_evaluation_map at target<br/>SITE or STRENGTH_MASK bits set → Status = us_Move<br/>else → Status = us_GOTO<br/>writes dst_wx/wy<br/>consumes _ai_own_stack_unit_list slot"]:::func

        Slots4_7 --> OrderXxx["AI_Order_Meld<br/>AI_Order_Settle<br/>AI_Order_Purify<br/>AI_Order_RoadBuild<br/>AI_Order_SeekTransport<br/><br/>sets Status to the matching us_* value<br/>consumes _ai_own_stack_unit_list slot"]:::func
    end

    SetTgt -. "writes" .-> UnitFields
    OrderXxx -. "writes" .-> UnitFields

    UnitFields["_UNITS[u].Status<br/>_UNITS[u].dst_wx / dst_wy"]:::state

    subgraph PhaseMove["MOVEMENT PHASE — AI_MoveUnits(player_idx)"]
        direction TB
        MoveLoop["for unit_idx in 0.._units:<br/>switch(_UNITS[unit_idx].Status)"]:::func
        MoveLoop --> StatusGate{Status}:::gate
        StatusGate -- "us_GOTO" --> UnitMove
        StatusGate -- "us_Move" --> UnitMove
        StatusGate -- "us_BuildRoad" --> BuildRoad["AI_UNIT_BuildRoad__WIP"]:::func
        BuildRoad --> UnitMove
        StatusGate -- "us_Meld" --> UnitMeld["AI_UNIT_Meld__WIP"]:::func
        StatusGate -- "us_Settle" --> UnitSettle["AI_UNIT_Settle__WIP"]:::func
        StatusGate -- "us_SeekTransport" --> UnitSeek["AI_UNIT_SeekTransprt__WIP"]:::func

        UnitMove["AI_UNIT_Move<br/>reads _UNITS[u].dst_wx/wy"]:::func --> MoveUnits["Move_Units<br/>actual movement resolution"]:::terminal

        MoveUnits -. "post-move: if Status was us_GOTO/us_Move and unit didn't budge" .-> MoveFailed["_UNITS[u].Move_Failed = ST_TRUE<br/>(UNITSTK.c:385)"]:::state
    end

    UnitFields -. "reads" .-> MoveLoop
```

### How to read this diagram

- **Boxes in blue (`func`)** are functions. Boxes in yellow (`state`) are persistent state fields on `_UNITS`. The green box (`terminal`) is the final movement resolver.
- **Solid arrows** are direct calls. **Dotted arrows** are data flow (reads/writes to module-scope state).
- **The two phases are strictly sequential** within a single AI player's turn. Everything in the Order phase completes before the Movement phase begins.
- **The convergence point** is `_UNITS[u].Status` and `_UNITS[u].dst_wx/wy`. Six different order-issuing paths in the Order phase all write these two fields; the Movement phase reads them and dispatches.

### Where the Status-classification happens

Two of the order-setters classify the target via `g_ai_evaluation_map`:

- **`AI_Stacks_Order_Attack_Target_Or_Goto_Destination`** — reads the target square's eval value. If SITE flag set OR STRENGTH_MASK bits non-zero → `us_Move` (target has a site or units). Else → `us_GOTO` (empty travel destination). This is why opportunistic ambush retargeting in slot 1 keeps the unit in `us_Move` — the new target (free-roaming enemy stack) has STRENGTH_MASK bits set.
- **`AI_Order_*` family** — each sets its own dedicated status (`us_Settle`, `us_BuildRoad`, etc.) directly without consulting `g_ai_evaluation_map`. The Status determines which `AI_UNIT_*__WIP` handler `AI_MoveUnits` dispatches to.

### `us_Move` vs `us_GOTO` at the execution level

Both dispatch to the same `AI_UNIT_Move` handler. The distinction matters only for downstream consumers that gate on Status:

- **Next turn's slot 1 (`AI_Stacks_Init_Build_Target_Order 3)** uses `Status == us_Move` to detect mid-attack-run stacks for the opportunistic ambush re-check.
- **Post-move bookkeeping in `UNITSTK.c:385`** sets `Move_Failed = TRUE` if Status was `us_GOTO` or `us_Move` and the unit didn't actually move. (Cleared at the start of each AI turn per `AIDUDES.c:297-303`.)

### Cross-turn loop: how `us_Move` re-renews

The mid-attack-run state persists across turns by a re-checking loop:

```mermaid
flowchart LR
    classDef func fill:#e1f5ff,stroke:#0066cc,color:#000
    classDef state fill:#fff3cd,stroke:#856404,color:#000

    Move["Status = us_Move<br/>(from prior order)"]:::state
    Move --> NextSlot1["next turn slot 1 Phase 3:<br/>AI_Stacks_Target_Nearest_Hostile_Stack"]:::func
    NextSlot1 -- "enemy stack found" --> SetMove["AI_Stacks_Order_Attack_Target_Or_Goto_Destination<br/>target has STRENGTH_MASK bits<br/>→ Status = us_Move (renewed)"]:::func
    SetMove --> Move
    NextSlot1 -- "no enemy on landmass" --> Demote["AISTK_InTransit<br/>Status = us_Ready"]:::state
```

The loop terminates only when no hostile free-roaming stack remains on the landmass, OR when between-turn movement resolution actually engages the target.

---

## Notes on the diagram approach

What worked well here:
- **Subgraph framing** for the two-phase structure — phases are visually distinct without needing prose to spell it out.
- **Convergence node** (`UnitFields`) made the order-issuer fan-in / status-reader fan-out immediately visible. Hard to do in ASCII without losing alignment.
- **Color-coding via classDef** distinguished functions from state from gates from terminals at a glance.

What didn't translate:
- Mermaid auto-layout sometimes places nodes in surprising spots. For the cross-turn loop diagram, the LR (left-right) layout reads better than TD (top-down).
- Embedded markdown (`<i>`, `<b>`) renders in most viewers but not all. The fallback is plain text.

Suggested use for the project: mermaid for **branching dispatch graphs and state machines**; keep ASCII for **short linear call chains** that need to render in any context (raw file, grep output, terminal).
