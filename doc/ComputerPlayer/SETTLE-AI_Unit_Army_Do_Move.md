SETTLE-AI_Unit_Army_Do_Move.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_UNIT_Move.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_UNIT_Move.c

AI_Unit_Army_Do_Move()
    |-> Allocate_Reduced_Map()
    |-> MainScr_Create_Reduced_Map_Picture()
    |-> Stack_Move_To()        (the actual stack movement)

---

# AI_Unit_Army_Do_Move — Walkthrough

The leaf that **actually moves a unit's stack** during the AI turn. The dispatcher [`AI_Execute_Orders`](SETTLE-AI_Execute_Orders.md) walks owned units and, for the move-ish statuses (`us_GOTO`, `us_Move`, and after `us_BuildRoad`), calls this to advance the unit's whole stack toward its stored destination (`dst_wx/wy`) via `Stack_Move_To`.

> **Naming:** OG/IDA name is `AI_UNIT_Move` (o100p06) — the disassembly file keeps it. ReMoM renamed it to `AI_Unit_Army_Do_Move` (it moves the unit's **army/stack**, not just the one unit). It is **AI-specific by role**: the only callers are the AI/neutral path, and it is the counterpart to the human-side `Move_Units` (both wrap `Stack_Move_To`). Hence the `AI_` prefix is kept.

**Location:** [MoM/src/SETTLE.c:383](../../MoM/src/SETTLE.c#L383) (~50 lines, ends [line 433](../../MoM/src/SETTLE.c#L433); Doxygen header at [361-382](../../MoM/src/SETTLE.c#L361-L382)). **WZD overlay:** ovr100, p06. **GEMINI variant:** deleted at done-done.

## Pipeline position

```
human:  WIZ_NextIdleStack → Move_Units            → Stack_Move_To
AI:     AI_Execute_Orders → AI_Unit_Army_Do_Move  → Stack_Move_To   ◄── this
```

`Stack_Move_To` has exactly these two callers; `AI_Unit_Army_Do_Move` is the AI/neutral one (confirmed by the in-code note at [SETTLE.c:360-364](../../MoM/src/SETTLE.c#L360-L364)).

## Signature

```c
int16_t AI_Unit_Army_Do_Move(int16_t unit_idx);   // returns Stack_Move_To()'s result (did_move_stack)
```

Callers — all inside [`AI_Execute_Orders`](SETTLE-AI_Execute_Orders.md):
- [SETTLE.c:161](../../MoM/src/SETTLE.c#L161) — `us_BuildRoad` case (after `AI_UNIT_BuildRoad__WIP`)
- [SETTLE.c:165](../../MoM/src/SETTLE.c#L165) — `us_GOTO` case
- [SETTLE.c:181](../../MoM/src/SETTLE.c#L181) — `us_Move` case

The return value (`did_move_stack`) is **discarded** by every caller — faithful to the OG, which also dropped it.

## Code walk

```c
if(_UNITS[unit_idx].Finished == ST_TRUE) { return ST_FALSE; }   // already done this turn

Allocate_Reduced_Map();                                          // rebuild reduced-map artifacts
MainScr_Create_Reduced_Map_Picture();

unit_dst_wx = _UNITS[unit_idx].dst_wx;   unit_dst_wy = _UNITS[unit_idx].dst_wy;
unit_wp     = _UNITS[unit_idx].wp;        unit_owner_idx = _UNITS[unit_idx].owner_idx;
niu_unit_wx = _UNITS[unit_idx].wx;        niu_unit_wy    = _UNITS[unit_idx].wy;   // extracted, never used

l_map_x = _map_x;  l_map_y = _map_y;      // redundant round-trip:
_map_x  = l_map_x; _map_y  = l_map_y;     //   writes _map_x back to itself

return_value = Stack_Move_To(unit_owner_idx, unit_idx, unit_dst_wx, unit_dst_wy, &l_map_x, &l_map_y, unit_wp);
return return_value;
```

| Step | Line | asm |
|---|---|---|
| Finished gate → `ST_FALSE` | [396-399](../../MoM/src/SETTLE.c#L396-L399) | `cmp Finished, e_ST_TRUE; jnz …; xor ax,ax; jmp Done` ✓ |
| Reduced-map rebuild | [402-403](../../MoM/src/SETTLE.c#L402-L403) | `call j_Allocate_Reduced_Map__1` / `…Picture` ✓ |
| Extract dst/wp/owner (+ dead wx/wy) | [406-411](../../MoM/src/SETTLE.c#L406-L411) | six `cbw` byte-loads ✓ |
| Map-coord round-trip | [415-418](../../MoM/src/SETTLE.c#L415-L418) | same redundant write-back ✓ |
| `Stack_Move_To(...)` | [422-430](../../MoM/src/SETTLE.c#L422-L430) | push order matches [UNITSTK.h:30](../../MoM/src/UNITSTK.h#L30) prototype ✓ |
| `return return_value` | [432](../../MoM/src/SETTLE.c#L432) | `mov did_move_stack, ax` ✓ |

**IDA-confirmed faithful** to [AI_UNIT_Move.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr100/AI_UNIT_Move.asm) — exact match.

## Faithful quirks (preserve, not bugs)

- **`niu_unit_wx` / `niu_unit_wy` are extracted but never used** ([410-411](../../MoM/src/SETTLE.c#L410-L411)) — only owner/wp/dst reach `Stack_Move_To`. The asm also loads `wx`/`wy` into stack slots and never pushes them; the `niu_` prefix marks the dead reads. OG-faithful.
- **Redundant map round-trip** ([415-418](../../MoM/src/SETTLE.c#L415-L418)) — `_map_x = l_map_x;` writes `_map_x` back to itself before `&l_map_x` is passed in. In the asm; preserve. The `DEDU` at [421](../../MoM/src/SETTLE.c#L421) notes the body was probably once just `return Stack_Move_To(...)`.

No reconstruction bugs — clean.

## ASCII summary

```
AI_Unit_Army_Do_Move(unit_idx)              (AI/neutral move-order leaf; counterpart to human Move_Units)
  if Finished:                 return ST_FALSE
  Allocate_Reduced_Map(); MainScr_Create_Reduced_Map_Picture()
  read dst_wx/dst_wy/wp/owner   (+ wx/wy extracted but NIU)
  l_map_x/y <-> _map_x/y        (redundant round-trip)
  return Stack_Move_To(owner, unit_idx, dst_wx, dst_wy, &l_map_x, &l_map_y, wp)
```

## Related references

- [SETTLE-AI_Execute_Orders.md](SETTLE-AI_Execute_Orders.md) — the dispatcher that calls this for `us_GOTO` / `us_Move` / `us_BuildRoad`
- [MoM-AI-AIMOVE-Index.md](MoM-AI-AIMOVE-Index.md) — function index
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_UNIT_Move.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
