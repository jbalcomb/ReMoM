AISPELL-Get_Map_Square_Target_For_Spell.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\Get_Map_Square_Target_For_Spell.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr156\Get_Map_Square_Target_For_Spell.c

Cast_Spell_Overland()                       // generic sites (stack-target & disenchant spells; dynamic spell_idx)
Cast_Transmute() / Cast_Change_Terrain() / Cast_Raise_Volcano() / Cast_Corruption() /
Cast_Enchant_Road() / Cast_Floating_Island() / Cast_Plane_Shift() / Cast_Natures_Cures()   // each passes its own fixed spell_idx
    |-> Get_Map_Square_Target_For_Spell()
                    |-> AITP_Transmute(player_idx, wx, wy, wp);
                    |-> AITP_Change_Terrain(player_idx, wx, wy, wp);
                    |-> AITP_Attack_Stack(player_idx, wx, wy, wp);
                    |-> AITP_Floating_Island(player_idx, wx, wy, wp);
                    |-> AITP_Enchant_Road(player_idx, wx, wy, wp);
                    |-> AITP_Disenchant(player_idx, wx, wy, wp);
                    |-> AITP_Attack_Terrain(player_idx, wx, wy, wp, spell_idx);
                    |-> AITP_Plane_Shift(player_idx, wx, wy, wp);
                    |-> AITP_Natures_Cures(player_idx, wx, wy, wp);
                    |-> Cast_Spell_Target_Error(spell_idx);

---

# `Get_Map_Square_Target_For_Spell` — Walkthrough

| Function | Location | Role |
|---|---|---|
| `Get_Map_Square_Target_For_Spell` | [AISPELL.c:4991-5051](../../MoM/src/AISPELL.c#L4991-L5051) | The **map-square target router**: given a `spell_idx`, dispatch to the matching `AITP_*` picker, which fills `*wx/*wy/*wp` with a target square. Returns the picker's result (`ST_TRUE`/`ST_FALSE`); on an unrecognized spell it reports an error and returns the passed-in `spell_target_type`. |

> **Status: BUILDS CLEAN — faithful to the asm (1:1).** The body ([AISPELL.c:4991](../../MoM/src/AISPELL.c#L4991)) is a faithful reconstruction of `Get_Map_Square_Target_For_Spell.asm` (the IDA-C's `if/else if` chain is rendered as a `switch`; same tests, same order). It compiles and links as part of AISPELL.c / momlib. Every case dispatches to the same target the asm calls — including `spl_Plane_Shift` → `AITP_Plane_Shift` (an earlier mis-wire here has been corrected). One preserved OG quirk on the default path ([B1](#b1-ogbug--default-returns-the-passed-in-target-type)). The asm is the authority.

## Purpose

When the AI casts an overland spell whose target is a **map square** (terrain, an enemy stack, an own unit, a settler to plane-shift, etc.), the cast routine asks this router for the target. The router is a pure dispatcher: it does no scanning itself — it forwards `(player_idx, wx, wy, wp)` (plus `spell_idx` for the terrain-attack case) to the spell's `AITP_*` picker and relays the picker's return value. City-enchantment spells are routed elsewhere; this router only handles the map-square family.

## How it's reached

Two call shapes feed it. The stack-target spells (Ice/Fire/Black Wind/Stasis) and the disenchant spells reach it through `Cast_Spell_Overland`'s two **generic** sites, which pass a dynamic `spell_idx`. Every other map-square spell has its own dedicated `Cast_<Spell>` routine that calls the router with a **fixed** `spell_idx`. The first argument (`spell_target_type`) carries the spell's UI target category (`stt_Map_Square`, `stt_Friendly_Group`, or `stt_Enemy_Group`).

| Caller | Site | `spell_idx` passed | `spell_target_type` |
|---|---|---|---|
| `Cast_Spell_Overland` | [OverSpel.c:1373](../../MoM/src/OverSpel.c#L1373) | dynamic (stack-target spells) | `stt_Enemy_Group` |
| `Cast_Spell_Overland` | [OverSpel.c:1660](../../MoM/src/OverSpel.c#L1660) | dynamic (disenchant, etc.) | `stt_Map_Square` |
| `Cast_Transmute` | [Spells130.c:1070](../../MoM/src/Spells130.c#L1070) | `spl_Transmute` | `stt_Map_Square` |
| `Cast_Change_Terrain` | [Spells130.c:516](../../MoM/src/Spells130.c#L516) | `spl_Change_Terrain` | `stt_Map_Square` |
| `Cast_Raise_Volcano` | [Spells130.c:807](../../MoM/src/Spells130.c#L807) | `spl_Raise_Volcano` | `stt_Map_Square` |
| `Cast_Corruption` | [Spells130.c:1279](../../MoM/src/Spells130.c#L1279) | `spl_Corruption` | `stt_Map_Square` |
| `Cast_Enchant_Road` | [Spells130.c:1632](../../MoM/src/Spells130.c#L1632) | `spl_Enchant_Road` | `stt_Map_Square` |
| `Cast_Floating_Island` | [Spells132.c:1170](../../MoM/src/Spells132.c#L1170) | `spl_Floating_Island` | `stt_Map_Square` |
| `Cast_Plane_Shift` | [Spells132.c:808](../../MoM/src/Spells132.c#L808) | `spl_Plane_Shift` | `stt_Friendly_Group` |
| `Cast_Natures_Cures` | [Spells129.c:1362](../../MoM/src/Spells129.c#L1362) | `spl_Natures_Cures` | `stt_Friendly_Group` |

## Dispatch table (production vs. asm)

Production order matches the asm's `cmp`/`jz` chain exactly. The asm/`.c` filenames keep the OG (drake178) symbol names; production uses the renamed pickers.

| `spell_idx` case(s) | Production picker | asm call | Site |
|---|---|---|---|
| `spl_Transmute` | `AITP_Transmute` | `AITP_Transmute` | [5001](../../MoM/src/AISPELL.c#L5001) |
| `spl_Change_Terrain` | `AITP_Change_Terrain` | `AITP_ChangeTerrain__WIP` | [5005](../../MoM/src/AISPELL.c#L5005) |
| `spl_Ice_Storm` / `spl_Fire_Storm` / `spl_Black_Wind` / `spl_Stasis` | `AITP_Attack_Stack` | `AITP_OVL_HarmStack` | [5012](../../MoM/src/AISPELL.c#L5012) |
| `spl_Floating_Island` | `AITP_Floating_Island` | `Pick_Target_For_Floating_Island` | [5016](../../MoM/src/AISPELL.c#L5016) |
| `spl_Enchant_Road` | `AITP_Enchant_Road` | `AITP_EnchantRoad` | [5020](../../MoM/src/AISPELL.c#L5020) |
| `spl_Disenchant_True` / `spl_Disenchant_Area` | `AITP_Disenchant` | `AITP_Disenchant` | [5025](../../MoM/src/AISPELL.c#L5025) |
| `spl_Corruption` / `spl_Raise_Volcano` | `AITP_Attack_Terrain` | `AITP_HarmTerrain` | [5030](../../MoM/src/AISPELL.c#L5030) |
| `spl_Plane_Shift` | `AITP_Plane_Shift` | `AITP_PlaneShift` | [5034](../../MoM/src/AISPELL.c#L5034) |
| `spl_Natures_Cures` | `AITP_Natures_Cures` | `AITP_NaturesCures` | [5038](../../MoM/src/AISPELL.c#L5038) |
| *default* | `Cast_Spell_Target_Error` + return `spell_target_type` | `Cast_Spell_Target_Error` + `mov ax,[bp+RetV]` | [5040-5046](../../MoM/src/AISPELL.c#L5040-L5046) |

Every picker except the terrain-attack case is called with `(player_idx, wx, wy, wp)` (asm pushes `wp, wy, wx, player_idx`; `add sp, 8` = 4 args). The terrain-attack case additionally passes `spell_idx` (asm pushes `_SI_spell_idx` first; `add sp, 0Ah` = 5 args), so `AITP_Attack_Terrain` can apply its Corruption-only terrain filters.

## Code walk

Line refs are production [AISPELL.c](../../MoM/src/AISPELL.c); the authority is `Get_Map_Square_Target_For_Spell.asm`.

```c
int16_t return_value = 0;   // DNE in Dasm — asm relays the call's AX directly
int16_t niu_variable = 0;   // asm [bp-2] "Passed_Return"; written only in default, never read (NIU)

switch(spell_idx)                                                  // asm  cmp si,<spl_*> chain
{
    case spl_Transmute:        return_value = AITP_Transmute(player_idx, wx, wy, wp);        break;
    case spl_Change_Terrain:   return_value = AITP_Change_Terrain(player_idx, wx, wy, wp);   break;
    case spl_Ice_Storm: case spl_Fire_Storm:
    case spl_Black_Wind: case spl_Stasis:
                               return_value = AITP_Attack_Stack(player_idx, wx, wy, wp);     break;
    case spl_Floating_Island:  return_value = AITP_Floating_Island(player_idx, wx, wy, wp);  break;
    case spl_Enchant_Road:     return_value = AITP_Enchant_Road(player_idx, wx, wy, wp);     break;
    case spl_Disenchant_True:
    case spl_Disenchant_Area:  return_value = AITP_Disenchant(player_idx, wx, wy, wp);       break;
    case spl_Corruption:
    case spl_Raise_Volcano:    return_value = AITP_Attack_Terrain(player_idx, wx, wy, wp, spell_idx); break;  // extra arg
    case spl_Plane_Shift:      return_value = AITP_Plane_Shift(player_idx, wx, wy, wp);      break;
    case spl_Natures_Cures:    return_value = AITP_Natures_Cures(player_idx, wx, wy, wp);    break;
    default:
        Cast_Spell_Target_Error(spell_idx);                       // asm  call Cast_Spell_Target_Error
        return_value = spell_target_type;                         // asm  mov ax,[bp+RetV]   [B1]
        niu_variable = return_value;                              // asm  mov [bp+Passed_Return],ax  (never read)
        break;
}
return return_value;                                              // asm  jmp @@Done (AX already holds the call result)
```

**Faithful 1:1** — the spell-by-spell test order, the grouped `jz` cases (the four stack-target spells; the two disenchant spells; the two terrain-attack spells), the 4-arg vs. 5-arg call shapes, and the default error path all match the asm.

### Symbol / layout cross-checks (verified against the asm)

| Production | asm | Value / note |
|---|---|---|
| `spell_target_type` (1st param) | `RetV` (`word ptr 6`) | the passed-in value the default path returns. |
| `niu_variable` | `Passed_Return` (`word ptr -2`) | written only in the default case, never read. |
| `return_value` | — | DNE in the asm; the asm relays each call's `AX` straight to `@@Done`. Inert. |
| `Cast_Spell_Target_Error(spell_idx)` | `call Cast_Spell_Target_Error` | [AISPELL.h:222](../../MoM/src/AISPELL.h#L222). |

## B1 (OGBUG) — default returns the passed-in target type

On an unrecognized `spell_idx` the asm calls `Cast_Spell_Target_Error`, then does `mov ax, [bp+RetV]` / `mov [bp+Passed_Return], ax` — i.e. it returns the **first parameter** (`spell_target_type`/`RetV`) as the function's result, and stashes a copy in a local that is never read. Production mirrors this exactly ([5044-5045](../../MoM/src/AISPELL.c#L5044-L5045), tagged `OGBUG`). It is odd (the default arguably should return `ST_FALSE`/0), but it is original-game behavior and is faithfully preserved. The default is effectively unreachable in normal play — `Cast_Spell_Overland` only routes recognized map-square spells here.

## OG quirks (preserve when reconstructing)

- **B1** — default path returns `spell_target_type` (the passed-in `RetV`), not a fixed failure code.
- **Reconstruction-only locals**: `return_value` is DNE in the asm (the asm relays `AX` directly); `niu_variable` is the asm's write-only `Passed_Return`. Both are inert and faithful to the frame.

## Sub-functions / external calls

- The `AITP_*` pickers — one per case (see the dispatch table; each has its own walkthrough doc).
- **`Cast_Spell_Target_Error(spell_idx)`** — logs "[spell name] could not be found for CP." on the default path.
- Writes nothing itself; threads the `wx`/`wy`/`wp` pointers through to the picker.

## Related references

- `…\ovr156\Get_Map_Square_Target_For_Spell.asm` — IDA Pro 5.5 disassembly (the authority).
- `…\ovr156\Get_Map_Square_Target_For_Spell.c` — Piethawn IDA-C (`__GEMINI`).
- Per-case picker docs: [AITP_Transmute](AISPELL-AITP_Transmute.md), [AITP_Change_Terrain](AISPELL-AITP_Change_Terrain.md), [AITP_Attack_Stack](../AISPELL-AITP_Attack_Stack.md), [AITP_Floating_Island](AISPELL-AITP_Floating_Island.md), [AITP_Enchant_Road](AISPELL-AITP_Enchant_Road.md), [AITP_Disenchant](AISPELL-AITP_Disenchant.md), [AITP_Attack_Terrain](AISPELL-AITP_Attack_Terrain.md), [AITP_Plane_Shift](AISPELL-AITP_Plane_Shift.md), [AITP_Natures_Cures](AISPELL-AITP_Natures_Cures.md).
- [AISPELL-AI_Spell_Select.md](AISPELL-AI_Spell_Select.md) — the parent dispatcher.
