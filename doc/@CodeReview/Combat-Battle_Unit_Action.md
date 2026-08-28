Combat-Battle_Unit_Action.md

Battle_Unit_Action__WIP()  ==>  Battle_Unit_Action()
BU_MeleeWallCheck()       ==>  Check_Attack_Melee_City_Wall()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Battle_Unit_Action.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\BU_MeleeWallCheck.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Check_Attack_Melee.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\Battle_Unit_Has_Flight.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\Battle_Unit_Is_Within_City.asm

OON XREF:  Battle_Unit_Action() |-> Battle_Unit_Attack() |-> Battle_Unit_Attack_Target() |-> Battle_Unit_Process_Attack()

Combat_Screen__WIP()
    |-> Battle_Unit_Action()
        |-> Battle_Unit_Attack()                                    done-done
        |-> Check_Attack_Melee()
            |-> Battle_Unit_Has_Flight()
        |-> Check_Attack_Melee_City_Wall()
            |-> Battle_Unit_Is_Within_City()
            |-> Combat_Grid_Cell_Has_City_Wall()                    done-done
        |-> Move_Battle_Unit()                                      done-done

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open. Five functions compared, three findings raised and closed, one function renamed. Builds clean (2026-08-04).**

Three reconstruction errors were raised (R1-R3) and all three are closed. R1 was behavioural — `Battle_Unit_Has_Flight` dropped a third of its enchantment union, so a unit flying by an overland enchantment read as ground-bound and could be meleed. R2 and R3 were equivalent-but-restructured conditionals that the byte counts contradicted; R2's first fix landed inverted and was caught before it shipped.

`Battle_Unit_Action` itself is faithful throughout.

## Scope

Anchors are function **start** lines.

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Battle_Unit_Action` | [Combat.c:2718](../../MoM/src/Combat.c#L2718) | [ovr091/Battle_Unit_Action.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr091/Battle_Unit_Action.asm) (257 lines) | faithful |
| `Check_Attack_Melee` | [Combat.c:5023](../../MoM/src/Combat.c#L5023) | [ovr098/Check_Attack_Melee.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/Check_Attack_Melee.asm) (63 lines) | faithful (R2 fixed) |
| `Battle_Unit_Has_Flight` | [Combat.c:4985](../../MoM/src/Combat.c#L4985) | [ovr098/Battle_Unit_Has_Flight.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/Battle_Unit_Has_Flight.asm) (119 lines) | faithful (R1 fixed) |
| `Check_Attack_Melee_City_Wall` | [Combat.c:11719](../../MoM/src/Combat.c#L11719) | [ovr113/BU_MeleeWallCheck.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/BU_MeleeWallCheck.asm) (106 lines) | faithful (R3 fixed) |
| `Battle_Unit_Is_Within_City` | [Combat.c:15984](../../MoM/src/Combat.c#L15984) | [ovr124/Battle_Unit_Is_Within_City.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr124/Battle_Unit_Is_Within_City.asm) (47 lines) | faithful |

**Already reviewed elsewhere:** `Battle_Unit_Attack` in [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md); `Move_Battle_Unit` and `Combat_Grid_Cell_Has_City_Wall` in [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md). All DONE-DONE; none re-derived here.

`asm:N` refers to the listing named in the row above.

**Two enum traps confirmed in `WIZARDS.inc` for this review.**

- `rat_NONE = 0` (line 6890) and `rat_None = 255` (line 6911) are **different symbols**. `Check_Attack_Melee` uses the former, so `cmp … rat_NONE` / `jle` really is "greater than zero." Production carries both — `rat_NONE` = 0 and `rat_UNDEF` = -1 (the same byte as 255).
- **`UE_*` values in `WIZARDS.inc` are high-word masks, not full 32-bit constants.** `UE_WINDWALKING = 1` (5690) and `UE_FLIGHT = 2` (5691) are tested against the *high* word — every such test in the listings is paired with `and dx, 0`. They correspond to production's `UE_WIND_WALKING` = `0x00010000` and `UE_FLIGHT` = `0x00020000` ([MOM_DEF.h:937-938](../../MoX/src/MOM_DEF.h#L937-L938)). Reading `1` and `2` as literal masks would be wrong by 16 bits.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared, and an emitted comparison is not treated as evidence of the expression that produced it — `if (A) X else Y` and `if (!A) Y else X` compile to the same bytes, so branch layout alone is never a finding. Where the *count* of emitted stores or tests differs, that is byte evidence and is recorded.

---

# Findings

None open.

---

# Divergences

## D1 — `Check_Attack_Melee` passes a dead second argument, and C cannot reproduce it

Both call sites push **two** arguments and clean up two words:

```
mov     ax, 1
push    ax
push    _DI_attacker_battle_unit_idx
push    cs
call    near ptr Battle_Unit_Has_Flight
pop     cx
pop     cx
```

asm:17-23 and asm:25-31. But `Battle_Unit_Has_Flight` declares a single parameter — `battle_unit_idx= word ptr 6` — and there is no `[bp+8]` access anywhere in its 119 lines. The literal `1` is pushed, ignored, and discarded.

Production's one-parameter prototype at [Combat.c:5917](../../MoM/src/Combat.c#L5917) is correct for the callee, and a prototyped C call cannot pass the extra argument. Same class as the dead-argument divergence recorded for `BU_SetBaseAnims__WIP`. Nothing to fix.

## D2 — literal `1` and `-1` where the listing names a constant

Advisory, no behavioural effect. The listings use `e_ST_TRUE` and `e_ST_UNDEFINED`; production spells several of these numerically:

| site | production | listing |
| --- | --- | --- |
| [3359](../../MoM/src/Combat.c#L3359) | `combat_grid_target == -1` | `cmp … e_ST_UNDEFINED` (asm:236) |
| [14088](../../MoM/src/Combat.c#L14088) | `battlefield->walled != 1` | `cmp … e_ST_TRUE` (asm:14) |
| [14110](../../MoM/src/Combat.c#L14110) | `Battle_Unit_Is_Within_City(dst) != 1` | `cmp ax, e_ST_TRUE` (asm:55) |
| [14125](../../MoM/src/Combat.c#L14125) | `Combat_Grid_Cell_Has_City_Wall(…) != 1` | `cmp ax, e_ST_TRUE` (asm:92) |

The `src` test at [14115](../../MoM/src/Combat.c#L14115) is **correctly** `!= 0` — asm:60 uses `or ax, ax` / `jnz`, not a compare against `e_ST_TRUE`. The two `Battle_Unit_Is_Within_City` calls really are tested differently, five lines apart. Do not normalise them.

## D3 — two dead bytes in `Battle_Unit_Is_Within_City`

asm:38-39 carry `db 0EBh` / `db 4` — an unreferenced two-byte `jmp short +4` sitting between the `ST_TRUE` return path and the `FALSE` label, which IDA could not attach to either. No C construct corresponds. Noted so it is not mistaken for missing logic.

---

# Battle_Unit_Action

## Frame map

`sub sp, 0Ch` (asm:16) = 12 bytes, plus two register locals. Parameters occupy `bp+6` through `bp+0Ah`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-0Ch` | `target_cgy` | `target_cgy` |
| `bp-0Ah` | `target_cgx` | `target_cgx` |
| `bp-8` | `ranged_group` | `ranged_group` |
| `bp-6` | `Unused_Local` | `Unused_Local` |
| `bp-4` | `delta_y` | `delta_y` |
| `bp-2` | `delta_x` | `delta_x` |
| `bp+6` | `_battle_unit_idx` | `_battle_unit_idx` |
| `bp+8` | `x` | `cgx` |
| `bp+0Ah` | `y` | `cgy` |
| `si` | `battle_unit_idx` | `battle_unit_idx` |
| `di` | `combat_grid_target` | `combat_grid_target` |

## Faithful — verified, leave alone

- **The action-map lookup is a row-pointer indirection** — `[g_combat_grid_action_map + y*2]` yields a near pointer, `+ x` indexes into it, and the load is a **signed byte** (`mov al, [bx]` / `cbw`, asm:20-25). That matches `int8_t * g_combat_grid_action_map[COMBAT_GRID_HEIGHT]` at [Combat.h:1203](../../MoM/src/Combat.h#L1203), and the signedness is what makes the `< 0` empty-cell test work. Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718).
- **The `99` wall case short-circuits the enemy checks** — `cmp … 99` / `jz` jumps straight past the controller and status tests (asm:27-28). Production's `if(combat_grid_target != 99) { … }` wrapper at [Battle_Unit_Action](../../MoM/src/Combat.c#L2718) is the same shape.
- **Same-controller and non-active targets fall through to the move path**, not to a return — both jump to `loc_77FD1` (asm:48, asm:57), which is the `Move_Battle_Unit` block. Production's `goto loc_MoveCheck` at [Battle_Unit_Action](../../MoM/src/Combat.c#L2718) and [Battle_Unit_Action](../../MoM/src/Combat.c#L2718) reproduces that; a plain `return` would not.
- **The controller comparison is byte-wide** — `mov al, [target.controller_idx]` then `cmp al, [attacker.controller_idx]` (asm:38-46), no `cbw`. Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718).
- **`Unused_Local = -2`** (asm:103) is stored and never read anywhere in the function. Faithful dead weight; production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718) keeps it.
- **The movement-point gate precedes every attack branch** — `movement_points > 0` or exit (asm:109-111), *before* the wall/enemy split. Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718). A unit out of movement cannot attack at all, including at range.
- **`Battle_Unit_Attack` argument order** — pushes are `target_cgy`, `target_cgx`, `combat_grid_target`, `battle_unit_idx`; right-to-left that is `(battle_unit_idx, combat_grid_target, target_cgx, target_cgy)`. All five call sites agree (asm:119-123, 145-149, 174-178, 200-204, 227-231). Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718), [Battle_Unit_Action](../../MoM/src/Combat.c#L2718), [Battle_Unit_Action](../../MoM/src/Combat.c#L2718), [Battle_Unit_Action](../../MoM/src/Combat.c#L2718), [Battle_Unit_Action](../../MoM/src/Combat.c#L2718).
- **The ranged-group test is repeated verbatim three times** — `ranged_type / 10` compared against `rag_Boulder`, `rag_Missile`, `rag_Magic` at asm:126-143, asm:181-198 and asm:208-225. The original does not factor it out; production likewise repeats it at [Battle_Unit_Action](../../MoM/src/Combat.c#L2718), [Battle_Unit_Action](../../MoM/src/Combat.c#L2718) and [Battle_Unit_Action](../../MoM/src/Combat.c#L2718). Leave the duplication.
- **Adjacency is `delta <= 1` on both axes**, tested as two `cmp … 1` / `jg` pairs (asm:115-118, asm:163-166). Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718) and [Battle_Unit_Action](../../MoM/src/Combat.c#L2718).
- **`Move_Battle_Unit` receives the raw click coordinates**, `x`/`y` from the frame — not `target_cgx`/`target_cgy` (asm:245-249). Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718).
- **The move path re-tests movement points** (asm:243-244) even though the gate at asm:109 already passed. Both tests are in the original because the `goto` arrives from before the first gate. Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718).

## OGBUGs — faithful, do not fix

- **A melee attempt against a wall-blocked target silently becomes a ranged attack.** When `Check_Attack_Melee_City_Wall` fails, the code does not abort — it re-tests the attacker's ranged group and fires a ranged attack at the same target (asm:181-205). Production [Battle_Unit_Action](../../MoM/src/Combat.c#L2718). Clicking to melee can therefore spend ammo or mana.

---

# Check_Attack_Melee

Faithful apart from R2.

Returns `{1: melee allowed, 0: blocked}`. The rule is three-armed: an airborne attacker may always melee; against an airborne defender a ground attacker needs `ranged_type > rat_NONE`; anything else is allowed.

- Both flight queries run **before** any test (asm:17-32), so `defender_has_flight` is computed even when the attacker's own flight makes it irrelevant. Production [Check_Attack_Melee](../../MoM/src/Combat.c#L5023).
- The ranged test is `cmp … rat_NONE` / `jle` (asm:45-46) — strictly greater than zero, and `rat_NONE` is `0`, not the `rat_None` = 255 symbol. Production [5969](../../MoM/src/Combat.c#L5969).

The drake178 header notes at [Check_Attack_Melee](../../MoM/src/Combat.c#L5023) — Wind Walking counted as flight, Demon Wings only via the movement flag — describe `Battle_Unit_Has_Flight`'s behaviour and are accurate against asm:50-56 and asm:73-80.

# Battle_Unit_Has_Flight

Faithful apart from R1.

Four ways to gain flight, then two overrides that clear it:

| source | asm |
| --- | --- |
| `UE_WIND_WALKING` in the union | 50-56 |
| `UE_FLIGHT` in the union | 57-63 |
| `_unit_type_table[].Move_Flags & MV_FLYING` | 64-72 |
| `battle_units[].Move_Flags & MV_FLYING` | 73-80 |
| `bue_Web` clears it | 81-88 |
| `bue_Black_Sleep` clears it | 89-96 |

The two clears come **last** and are unconditional, so Web or Black Sleep beats every flight source. Production [Battle_Unit_Has_Flight](../../MoM/src/Combat.c#L4985) preserves that ordering.

This is a different function from `Battle_Unit_Is_Airborne` (`BU_CheckFlight__WIP`, DONE-DONE in [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md)) — different listing, different overlay, different rules.

# Check_Attack_Melee_City_Wall

Faithful apart from R3.

Returns `{1: melee allowed, 0: blocked by wall}`. Eight early exits return `ST_TRUE`; only the fall-through returns `ST_FALSE`.

- **The city-wall gate is first** — `battlefield->walled != ST_TRUE` exits immediately (asm:13-16), so the whole function is inert outside a walled city.
- **The four bypasses are a defined set, not an oversight.** `MV_FLYING` (asm:23), `MV_TELEPORT` (asm:32), `MV_MERGING` (asm:41) and `UA_NONCORPOREAL` (asm:50) — the same four, in the same order, as `ovr124/BU_SetCityMovement.asm` asm:48, 57, 66, 75. The original applies one consistent "a wall cannot stop this unit" list to both the movement-cost side and the melee side.

## OGBUGs

- **Flying Fortress is not considered.** The original's wall-present test is `walled || city_enchantments[FLYING_FORTRESS] > 0`, emitted twice in this overlay alone — [ovr091/Move_Battle_Unit.asm:102-107](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr091/Move_Battle_Unit.asm#L102-L107) and [ovr091/Assign_Combat_Grids.asm:91-96](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr091/Assign_Combat_Grids.asm#L91-L96), both gating a `BU_SetCityMovement` call. `Check_Attack_Melee_City_Wall` tests `walled` alone (asm:13-16). A Flying Fortress city therefore gets wall-based **movement** costs but no wall-based **melee** protection — ground attackers outside can melee defenders inside as though no wall existed. Faithful; do not fix.

**Not an OGBUG — the teleport/merging note.** The source carries `OGBUG? teleporting and merging units can attack through walls` in the header and a flat `OGBUG:` restatement above the flag tests. The behaviour is real, but it is the intended rule: those two flags are members of the four-way bypass set above, which the original applies identically in `BU_SetCityMovement`. Both notes should be dropped or demoted to a plain explanatory comment.
- **The two `Battle_Unit_Is_Within_City` calls are tested differently** — the defender against `e_ST_TRUE` (asm:55), the attacker with `or ax, ax` (asm:60). Faithful; see D2.
- **The gate exemption is a coordinate pair**, `CGX_GATE` (8) and `CGY_GATE` (12), both required (asm:67-75). Production [14120](../../MoM/src/Combat.c#L14120). Values at [CMBTDEF.h:72-73](../../MoM/src/CMBTDEF.h#L72-L73), matching `WIZARDS.inc:9200` and `:9207`.
- **The wall test uses the defender's own cell**, not a cell between the two units (asm:76-92). Production [14125](../../MoM/src/Combat.c#L14125).

# Battle_Unit_Is_Within_City

Faithful.

Four sequential range checks, each branching to the `FALSE` return: `cgx >= 5`, `cgy >= 10`, `cgx <= 8`, `cgy <= 13` (asm:12-35). Production's `&&` chain at [Battle_Unit_Is_Within_City](../../MoM/src/Combat.c#L15984) compiles to exactly that and keeps the same order. Bounds match [CMBTDEF.h:68-71](../../MoM/src/CMBTDEF.h#L68-L71). See D3 for the two dead bytes at asm:38-39.

---

## Build state

**Clean.** `cmake --build --preset MSVC-debug` run 2026-08-04 with all three fixes and the rename in place — `Combat.c` compiled, every target linked, no warnings and no errors.

**R2's first attempt shipped inverted.** The middle arm read `defender_has_flight != ST_TRUE` and the final `else` set `ST_FALSE`, which blocked melee between any two ground units without a ranged attack — a silent regression a build cannot catch. `Check_Attack_Melee.asm:38` uses `jnz` to *leave* on not-flying, so the fall-through arm is the defender-flies case. Recorded because the same `JZ`/`JNZ`-to-skip shape appears throughout these listings.

**R3's first attempt did not compile** — two of the three split tests were missing their opening paren. The build caught it; six C2143/C2059 errors on `Combat.c` lines 14104 and 14108.

**Header prototypes checked.** [Combat.h:1203](../../MoM/src/Combat.h#L1203) declares `g_combat_grid_action_map` as `int8_t *[COMBAT_GRID_HEIGHT]`, matching the listing's row-pointer indirection. The five function prototypes match their definitions in parameter count, order and type.


**Anchors.** Scope-table anchors and every `Combat.c#L…` reference were read against `Combat.c` on 2026-08-04. `Combat.c` shifts under every edit, so re-verify before acting on a line number; the `asm:N` references are stable.

## Related references

- [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) — the callee this function dispatches to; DONE-DONE.
- [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md) — the other callee, plus `Combat_Grid_Cell_Has_City_Wall`; DONE-DONE.
- `Assign_Mouse_Images` (`ovr098`) — the other caller of `Check_Attack_Melee_City_Wall`. Not reviewed. R3 was behaviour-preserving, so it needs no re-check there.
