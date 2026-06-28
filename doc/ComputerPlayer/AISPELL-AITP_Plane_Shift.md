AISPELL-AITP_Plane_Shift.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_PlaneShift.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_PlaneShift.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Plane_Shift:
                            |-> AITP_Plane_Shift()

---

# `AITP_Plane_Shift` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Plane_Shift` | [AISPELL.c:5622-5692](../../MoM/src/AISPELL.c#L5622-L5692) | AI target picker for **Plane Shift**: find the highest-value **own settler stack** sitting on **Arcanus** (not on a city tile) whose **Myrror** landing square is usable, and return that stack's square. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp`, `ST_FALSE` otherwise. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:5622](../../MoM/src/AISPELL.c#L5622)) is a faithful reconstruction of `AITP_PlaneShift.asm` (drake178 `AITP_PlaneShift`; production renamed to `AITP_Plane_Shift`; the asm/`.c` filename keeps the OG `AITP_PlaneShift`). It compiles and links as part of AISPELL.c / momlib, and is **wired**: `Get_Map_Square_Target_For_Spell`'s `case spl_Plane_Shift` calls it ([AISPELL.c:5045](../../MoM/src/AISPELL.c#L5045)). It carries two preserved OG bugs ([B1](#b1-ogbug--settlers-arcanusmyrror-only), [B2](#b2-ogbug--no-destination-threat-check)). The asm is the authority.

## Purpose

Plane Shift moves one of the wizard's own stacks between Arcanus and Myrror. The AI uses it to ferry a **settler** across: it scans its own-stack list (`_ai_all_own_stacks`) for the highest-value settler stack currently on **Arcanus** (`wp == 0`), not standing on a city, whose corresponding **Myrror** square is a valid landing spot, and returns that stack's coordinates.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5045](../../MoM/src/AISPELL.c#L5045) | `switch(spell_idx) case spl_Plane_Shift` → here (active). Matches the asm dispatch (`.asm loc_E93BF` / IDA-C → `AITP_PlaneShift`). |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_PlaneShift.asm`.

```c
best_stack_idx = ST_UNDEFINED;  highest_value = 0;                      // asm  di=-1, Highest_Value=0
                                                                       // asm  mov si,Player_Index then xor si,si  → dead (see notes)
for(itr_stacks = 0; itr_stacks < _ai_all_own_stack_count; itr_stacks++)
{
    stack_value = _ai_all_own_stacks[itr_stacks].value;
    if((_ai_all_own_stacks[itr_stacks].abilities & AICAP_Settler) == 0) // asm  test abilities, AICAP_Settler / jnz keep
        stack_value = 0;                                               //      not a settler → value 0

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)            // on a city tile → value 0
        if(_ai_all_own_stacks[itr_stacks].wx == _CITIES[itr_cities].wx &&
           _ai_all_own_stacks[itr_stacks].wy == _CITIES[itr_cities].wy)
            stack_value = 0;

    if(_ai_all_own_stacks[itr_stacks].wp == 0)                         // asm  wp==0 (Arcanus) only
    {
        if(stack_value > highest_value)                               // asm  jg
        {
            if((_ai_all_own_stacks[itr_stacks].abilities & AICAP_Transport) != 0)   // asm  test …,AICAP_Transport
            {   // transport: destination Myrror square must be sailable
                if(Square_Is_Sailable(stack.wx, stack.wy, 1) == 1)
                { best_stack_idx = itr_stacks; highest_value = stack_value; }
            }
            else
            {   // land stack: destination Myrror square must NOT be ocean-like
                if(Square_Is_OceanLike(stack.wx, stack.wy, 1) == 0)
                { best_stack_idx = itr_stacks; highest_value = stack_value; }
            }
        }
    }
}

if(best_stack_idx == ST_UNDEFINED) return ST_FALSE;                    // asm  cmp di,-1 / xor ax,ax
*targeted_wx = _ai_all_own_stacks[best_stack_idx].wx;  // .wy, .wp;  return ST_TRUE;   // asm  store stack[di].* / ax=1
```

**Faithful 1:1** — the settler gate, the on-city zeroing, the `wp == 0` (Arcanus) gate, the strict-greater value compare, the transport-vs-land destination test against **plane 1** (Myrror), and the return all match the asm. The returned `wp` is the stack's current plane (Arcanus); the cast itself flips it.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_ai_all_own_stacks[]` | `AI_Own_Stacks` | `struct s_AI_STACK_DATA *` ([MOM_DAT.h:3010](../../MoX/src/MOM_DAT.h#L3010)); stride `imul 0Ah` = `sizeof(s_AI_STACK_DATA) = 10`. |
| `_ai_all_own_stack_count` | `AI_Own_Stack_Count` | `int16_t` ([MOM_DAT.h:3016](../../MoX/src/MOM_DAT.h#L3016)). |
| `AICAP_Settler` / `AICAP_Transport` | same | `0x80` / `0x01` ([MOM_DAT.h:64](../../MoX/src/MOM_DAT.h#L64), [57](../../MoX/src/MOM_DAT.h#L57)). |
| `Square_Is_Sailable(wx,wy,1)` / `Square_Is_OceanLike(wx,wy,1)` | `j_Square_Is_Sailable` / `j_Square_Is_OceanLike` | [Terrain.h:155](../../MoM/src/Terrain.h#L155), [200](../../MoM/src/Terrain.h#L200); plane arg `1` = Myrror (the destination). |
| `_CITIES[i]` stride | `imul 72h` | `sizeof(s_CITY) = 114`. |

## B1 (OGBUG) — settlers, Arcanus→Myrror only

The picker only ever considers stacks with the **`AICAP_Settler`** ability (others get `stack_value = 0`, so they never beat `highest_value`) and only on **Arcanus** (`wp == 0`). So the AI can Plane-Shift *only* settler stacks, and *only* from Arcanus to Myrror — never Myrror→Arcanus, never any non-settler stack. Confirmed against the asm (the `AICAP_Settler` test + `wp == 0` gate). Faithfully preserved.

## B2 (OGBUG) — no destination-threat check

The Myrror landing square is only tested for terrain suitability (`Square_Is_Sailable` for transports, not-`Square_Is_OceanLike` for land stacks) — **never** for enemy units or encounter lairs on/near it. The AI may shift a settler straight onto a hostile square. Faithfully preserved (the asm performs no such check).

## OG quirks (preserve when reconstructing)

- **B1** — settler-only, Arcanus→Myrror only.
- **B2** — no enemy/encounter check at the destination.
- **Dead assignment**: `itr_stacks = player_idx;` ([5630](../../MoM/src/AISPELL.c#L5630), tagged `/* OGBUG pointless */`) is immediately overwritten by the loop — mirrors the asm's dead `mov si, Player_Index` before `xor si, si`. Preserved.
- Minor: the locals are declared `int` (32-bit) rather than `int16_t`; inert for these loop/value variables (the OG was 16-bit).

## Sub-functions / external calls

- **`Square_Is_Sailable(wx, wy, wp)`**, **`Square_Is_OceanLike(wx, wy, wp)`** — destination-terrain tests on Myrror (plane `1`).
- Reads `_ai_all_own_stacks[]`, `_ai_all_own_stack_count`, `_CITIES[]`, `_cities`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_PlaneShift.asm` — IDA Pro 5.5 disassembly (the authority; OG name `AITP_PlaneShift`).
- `…\ovr156\AITP_PlaneShift.c` — Piethawn IDA-C.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../../MoM/src/AISPELL.c#L5002)) — the map-square router; its `spl_Plane_Shift` case ([5045](../../MoM/src/AISPELL.c#L5045)) dispatches here.
- [AISPELL-AITP_Attack_Stack.md](AISPELL-AITP_Attack_Stack.md) — sibling map-square picker (the Ice Storm / Fire Storm / Black Wind / Stasis case).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
