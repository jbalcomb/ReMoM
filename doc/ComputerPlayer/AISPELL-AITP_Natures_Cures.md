AISPELL-AITP_Natures_Cures.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_NaturesCures.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\AITP_NaturesCures.c

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> AI_Next_Turn()
            |-> Cast_Spell_Overland()
                |-> Get_Map_Square_Target_For_Spell()
                    switch(spell_idx)
                        case spl_Natures_Cures:
                            |-> AITP_Natures_Cures()

---

# `AITP_Natures_Cures` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `AITP_Natures_Cures` | [AISPELL.c:5695-5731](../../MoM/src/AISPELL.c#L5695-L5731) | AI target picker for **Nature's Cures**: find the player's own unit with the **most Damage** and return its map square. Returns `ST_TRUE` with `*targeted_wx/_wy/_wp`, `ST_FALSE` if the player has no damaged unit. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:5695](../../MoM/src/AISPELL.c#L5695)) is a faithful reconstruction of `AITP_NaturesCures.asm` (production renamed `AITP_NaturesCures` → `AITP_Natures_Cures`; the asm/`.c` filename keeps the OG `AITP_NaturesCures`). It compiles and links as part of AISPELL.c / momlib, and the cast-time dispatch ([AISPELL.c:5049](../../MoM/src/AISPELL.c#L5049)) is live. A simple single-pass max-find with **no OG bugs**. The asm is the authority.

## Purpose

`Get_Map_Square_Target_For_Spell` routes **Nature's Cures** here. Nature's Cures heals (zeroes the `Damage` of) the units in a stack, so the AI aims it at its **own most-damaged unit** — scan owned units, pick the one with the highest `Damage`, return its square.

## How it's reached

| Caller | Site | Notes |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:5049](../../MoM/src/AISPELL.c#L5049) | `switch(spell_idx) case spl_Natures_Cures` → here (active). |

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `AITP_NaturesCures.asm`.

```c
best_unit_idx = ST_UNDEFINED;  highest_damage = 0;                      // asm  si=-1, Highest_Damage=0

for(itr_units = 0; itr_units < _units; itr_units++)                     // asm  dx loop
{
    if(_UNITS[itr_units].owner_idx == player_idx)                       // asm  owner_idx (cbw) == Player_Index
    {
        unit_damage = _UNITS[itr_units].Damage;                         // asm  Damage (cbw)
        if(unit_damage > highest_damage)                               // asm  jle skip  =>  strictly greater
        {
            best_unit_idx = itr_units;                                 // asm  si = dx
            highest_damage = unit_damage;
        }
    }
}

if(best_unit_idx == ST_UNDEFINED) return ST_FALSE;                     // asm  cmp si,-1 / xor ax,ax
*targeted_wx = _UNITS[best_unit_idx].wx;  // .wy, .wp;  return ST_TRUE; // asm  store unit[si].* / ax=1
```

**Faithful 1:1** — the owner gate, the strict-greater `Damage` compare (`jle` skip), the `si = dx` record, and the `cmp si,-1` → `xor ax,ax` / store `_UNITS[si].wx/wy/wp` / `ax=1` return all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `_UNITS[i]` stride | `shl ,5` | `sizeof(s_UNIT) = 0x20 = 32`. |
| `_UNITS[i].Damage` | `s_UNIT.Damage` (cbw) | `int8_t` at offset `0x11` ([MOM_DAT.h:1843](../../MoX/src/MOM_DAT.h#L1843)). |
| `ST_UNDEFINED` | `-1` (`0FFFFh`) | the no-target sentinel in `best_unit_idx`. |

## OG quirks

None. No dead code, no off-by-ones, no preserved original-game bugs.

- **Tie-break**: the `> highest_damage` test is strictly-greater, so among equally-damaged units the **lowest unit index** is kept (the scan runs `0 -> _units`).
- Minor: the locals are declared `int` (32-bit) rather than `int16_t`; inert for these loop/value variables (the OG was 16-bit).

## Sub-functions / external calls

- None. Reads `_UNITS[]`, `_units`; writes through `*targeted_wx`/`*targeted_wy`/`*targeted_wp`.

## Related references

- `…\ovr156\AITP_NaturesCures.asm` — IDA Pro 5.5 disassembly (the authority; OG name `AITP_NaturesCures`).
- `…\ovr156\AITP_NaturesCures.c` — Piethawn IDA-C.
- `Get_Map_Square_Target_For_Spell` ([AISPELL.c:5002](../../MoM/src/AISPELL.c#L5002)) — the map-square router (Nature's Cures case).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
