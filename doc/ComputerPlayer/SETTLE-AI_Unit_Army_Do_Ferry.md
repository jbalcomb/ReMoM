SETTLE-AI_Unit_Army_Do_Ferry.md

Next_Turn_Proc()
|-> Next_Turn_Calc()
    |-> AI_Next_Turn()
         |-> AI_Execute_Orders()
            |-> AI_Unit_Army_Do_Ferry()

---

# AI_Unit_Army_Do_Ferry — Walkthrough

**Counts down a unit's ferry-wait timer.** When a unit is parked waiting for a transport (`us_Ferry`), this routine ticks its wait counter down each turn and returns it to `us_Ready` once the wait expires **or** the unit has regrouped with other units on its square. It's the `us_Ferry` leaf of [`AI_Execute_Orders`](SETTLE-AI_Execute_Orders.md).

> **Naming:** OG/IDA name is `AI_UNIT_SeekTransprt` (`o100p11`, `ovr100\AI_UNIT_SeekTransprt.asm`). ReMoM renamed it to `AI_Unit_Army_Do_Ferry` (matching the `AI_Unit_Army_Do_Move`/`_Settle`/`_Road`/`_Meld` siblings) and dropped the `__WIP` tag now that the body matches the asm.

**Location:** [MoM/src/SETTLE.c:685](../../MoM/src/SETTLE.c#L685) (ends [line 726](../../MoM/src/SETTLE.c#L726)). **WZD overlay:** ovr100, p11. **GEMINI variant:** none.

## The `dst_wx`-as-timer trick

`dst_wx` (normally a destination x-coordinate) is **repurposed as the ferry wait counter**. It is set to `10` by [`AI_Stacks_Order_Ferry`](AIMOVE-AI_Stacks_Order_Ferry.md) when the unit is first told to wait for transport (`us_Ferry`); this routine decrements it each turn. (Same field-reuse trick as `AI_Stacks_Order_Purify` stuffing `20` into `dst_wx`.)

## Signature & caller

```c
void AI_Unit_Army_Do_Ferry(uint16_t unit_idx);
```

- Called from [`AI_Execute_Orders`](SETTLE-AI_Execute_Orders.md) — the `us_Ferry` case ([SETTLE.c:178](../../MoM/src/SETTLE.c#L178)).

## Code walk

```c
if(_UNITS[unit_idx].Finished == ST_TRUE) { return; }                      // already done this turn  [695]
/* extract wx/wy/wp/owner */                                              // [701-704]
Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner_idx, &troop_count, troops);   // who else is here  [707]

_UNITS[unit_idx].dst_wx -= 1;                                             // tick the wait timer down  [713]

if(_UNITS[unit_idx].dst_wx < 1 || troop_count > 1)                       // expired OR regrouped  [720]
{
    _UNITS[unit_idx].Status = us_Ready;                                   // stop waiting
    _UNITS[unit_idx].dst_wx = 0;
}
```

| Step | Line | asm (`AI_UNIT_SeekTransprt.asm`) |
|---|---|---|
| `Finished == ST_TRUE` → return | [695](../../MoM/src/SETTLE.c#L695) | `cmp Finished,1; jnz …; xor ax,ax` ✓ |
| extract wx/wy/wp/owner | [701-704](../../MoM/src/SETTLE.c#L701-L704) | four `cbw` loads ✓ |
| `Player_Army_At_Square` | [707](../../MoM/src/SETTLE.c#L707) | `call j_Player_Army_At_Square` ✓ |
| `dst_wx -= 1` | [713](../../MoM/src/SETTLE.c#L713) | `mov al, dst_wx; dec al; mov dst_wx, al` ✓ |
| reset if `dst_wx < 1 \|\| troop_count > 1` | [720-724](../../MoM/src/SETTLE.c#L720-L724) | `cmp dst_wx,1; jl set` / `cmp Unit_Count,1; jle done`; set: `Status=us_Ready; dst_wx=0` ✓ |

**IDA-confirmed faithful** to [ovr100/AI_UNIT_SeekTransprt.asm](C:/STU/devel/STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr100/AI_UNIT_SeekTransprt.asm) — exact match. No reconstruction bugs.

## Behaviour notes

- The reset fires on **either** condition: the timer hitting `< 1` (the unit waited long enough — give up / re-evaluate next turn) **or** `troop_count > 1` (other own units arrived on the square, so it's no longer a lone unit needing ferry pickup). Matches drake178's note: *"change its status back to ready if it either ran out, or there are also other units on its square."*
- This routine only manages the **waiting**; the actual ferry pickup/movement is handled elsewhere (the unit sits `us_Ferry` until reset to `us_Ready`, then normal dispatch reconsiders it).

## Related references

- [SETTLE-AI_Execute_Orders.md](SETTLE-AI_Execute_Orders.md) — dispatcher; the `us_Ferry` case calls this
- [AIMOVE-AI_Stacks_Order_Ferry.md](AIMOVE-AI_Stacks_Order_Ferry.md) — the order-*setter* that assigns `us_Ferry` and seeds `dst_wx = 10`
- `C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr100\AI_UNIT_SeekTransprt.asm` — IDA Pro 5.5 disassembly (ground truth, verified)
