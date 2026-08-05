Combat-Summon_Demon.md

BU_SummonDemon__SEGRAX()       ==>  Summon_Demon()
BU_CombatSummon__SEGRAX()      ==>  Battle_Unit_Summon_Animation()
UNIT_SummonToBattle__SEGRAX()  ==>  Prepare_Battle_Unit_Summons()
BU_UnitLoadToBattle__SEGRAX()  ==>  Prepare_Battle_Unit()

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr113\BU_SummonDemon__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\UNIT_SummonToBattle__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\BU_UnitLoadToBattle__SEGRAX.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr133\BU_CombatSummon__SEGRAX.asm

CMB_Chasm_Anim_X    ==>  cmbt_cell_effect_cgx
CMB_Chasm_Anim_Y    ==>  cmbt_cell_effect_cgy
CMB_Chasm_Anim	    ==>  cmbt_cell_effect_active
CMB_ChasmAnimStage	==>  cmbt_cell_effect_frame
IMG_GUI_Chasm	    ==>  cmbt_cell_effect_seg

AI_BU_ProcessAction()
    case BUA_SummonDemon:
    |-> Summon_Demon()
        |-> Create_Unit()
        |-> Prepare_Battle_Unit_Summons()
            |-> Prepare_Battle_Unit()
                |-> Load_Battle_Unit()
                |-> Battle_Unit_Pict_Open()
                |-> Combat_Figure_Load()
        |-> Battle_Unit_Summon_Animation()
            |-> Combat_Grid_Screen_Coordinates()
            |-> Mark_Block()
            |-> Spell_Animation_Load_Graphics()
            |-> Mark_Block()
            |-> Play_Sound()
            |-> LBX_Reload_Next()
            |-> Battle_Unit_Compose_Bitmap()
            |-> Mark_Time()
            |-> Set_Page_Off()
            |-> Combat_Screen_Draw()
            |-> Combat_Cast_Spell_Message()
            |-> Create_Picture()
            |-> Copy_Bitmap_To_Bitmap()
            |-> Vanish_Bitmap()
            |-> Set_Window()
            |-> FLIC_Set_LoopFrame_1()
            |-> Draw_Picture_Windowed()
            |-> Reset_Window()
            |-> PageFlip_FX()
            |-> Release_Time()
            |-> Set_Page_Off()
            |-> Combat_Screen_Draw()
            |-> PageFlip_FX()
            |-> Release_Block()
            |-> Release_Block()

~ CMB_Units_Init__WIP()  AKA Prepare_All_Battle_Units()
OON XREF:  Prepare_Battle_Unit_Summons() |-> Prepare_Battle_Unit
...which is only used for scc_Summoning and USA 'Summon Demon'
OON XREF:  Combat_Cast_Spell() |-> Summon_Demon()

Combat_Cast_Spell()
    |-> Summon_Demon()
        |-> Create_Unit()
        |-> Prepare_Battle_Unit_Summons()
            |-> Prepare_Battle_Unit()
                |-> Load_Battle_Unit()
                |-> Battle_Unit_Pict_Open()
                |-> Combat_Figure_Load()
        |-> Battle_Unit_Summon_Animation()

Cast_Spell_On_Battle_Unit()
    switch(spell_data_table[spell_idx].type)
    case scc_Summoning:
    |-> Prepare_Battle_Unit_Summons(player_idx, (_units - 1), target_cgx, target_cgy);

Cast_Animate_Dead()
    |-> Battle_Unit_Pict_Open()
    |-> Combat_Figure_Load()
    |-> BU_Init_Battle_Unit()
    |-> BU_Apply_Battlefield_Effects__WIP()
    |-> Battle_Unit_Summon_Animation()

Cast_Raise_Dead()
    |-> Battle_Unit_Pict_Open()
    |-> Combat_Figure_Load()
    |-> BU_Init_Battle_Unit()
    |-> BU_Apply_Battlefield_Effects__WIP()
    |-> Battle_Unit_Summon_Animation()

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open. Four functions compared (2026-08-04).**

The summon chain is faithful end to end. One finding was raised and closed: `Battle_Unit_Summon_Animation` cached a battle-unit pointer the original never had — a phantom local, not a logic error. The frame settled it: `sub sp, 8` is fully accounted for by four named screen locals, leaving no room for a far pointer, and the listing recomputes the unit address ten times in 229 lines.

Two OGBUGs, both checked against sibling code rather than taken on trust. Both hold up.

## Scope

Anchors are function **start** lines.

| Function | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Summon_Demon` | [Combat.c:13998](../../MoM/src/Combat.c#L13998) | [ovr113/BU_SummonDemon__SEGRAX.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr113/BU_SummonDemon__SEGRAX.asm) (172 lines) | faithful |
| `Prepare_Battle_Unit_Summons` | [Combat.c:5548](../../MoM/src/Combat.c#L5548) | [ovr098/UNIT_SummonToBattle__SEGRAX.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/UNIT_SummonToBattle__SEGRAX.asm) (70 lines) | faithful |
| `Prepare_Battle_Unit` | [Combat.c:5575](../../MoM/src/Combat.c#L5575) | [ovr098/BU_UnitLoadToBattle__SEGRAX.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/BU_UnitLoadToBattle__SEGRAX.asm) (121 lines) | faithful |
| `Battle_Unit_Summon_Animation` | [Spells133.c:1140](../../MoM/src/Spells133.c#L1140) | [ovr133/BU_CombatSummon__SEGRAX.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr133/BU_CombatSummon__SEGRAX.asm) (229 lines) | faithful (R1 fixed) |

**Already reviewed elsewhere:** `Battle_Unit_Compose_Bitmap` (`BU_CreateImage__SEGRAX`) in [Combat-Move_Battle_Unit.md](Combat-Move_Battle_Unit.md).

**Not reviewed — next tier.** `Create_Unit` ([NEXTTURN.c:1023](../../MoM/src/NEXTTURN.c#L1023), 493 asm lines) is deliberately excluded: it has **12 callers** across lair guardians, hero hiring, mercenaries, Floating Island and overland casting, so it is a general unit factory rather than part of this chain and deserves its own bundle. Also out of scope: `Load_Battle_Unit` (286), `Combat_Figure_Load` (142), `Battle_Unit_Pict_Open` (72), `Combat_Cast_Spell_Message` (276), `Spell_Animation_Load_Graphics`, `Combat_Grid_Screen_Coordinates`, and the whole platform/screen layer in the tree above. Stated so the "faithful" verdicts are not read as covering them.

`asm:N` refers to the listing named in the row above.

**A global appears under two names in the listings.** `Summon_Demon` reads the placement map as `g_combat_grid_action_map` (asm:138); `Battle_Unit_Action` and `Assign_Combat_Grids` read the same array as `g_combat_grid_action_map`. There is only one dseg declaration — `g_combat_grid_action_map dw 16h dup(0)` at `dseg/_misc.asm:4796`, 22 words — and `COMBAT_GRID_HEIGHT` is `22` ([Combat.h:190](../../MoM/src/Combat.h#L190)). `g_combat_grid_action_map` is drake178's rename, applied only inside `ovr091`. Production uses the one name throughout, which is correct; do not read the two labels as two arrays.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared — with one exception applied below: where the frame is *fully* accounted for by named locals, the absence of room for another variable is byte evidence.

---

# Findings

None open.

---

# Divergences

## D1 — two `assert`s with no asm counterpart

`assert(bufpi >= 0)` and `assert(bufpi < MAX_BATTLE_UNIT_COUNT)` at [Combat.c:5581-5582](../../MoM/src/Combat.c#L5581-L5582), guarding exactly the failure the original's own header warned about — see the OGBUGs under `Prepare_Battle_Unit`. Debug-only; Release behaviour is unchanged.

## D2 — `SND_SpellCast_size` is the two-value-return HACK

[Spells133.c:1164](../../MoM/src/Spells133.c#L1164). The original returns a segment from `LBX_Reload_Next` and leaves the length in `lbxload_entry_length`; production threads it explicitly. Same class as the HACKs recorded in [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md). Not a fix.

## D3 — `Prepare_Battle_Unit_Summons` is `static` in production

The original is a `far proc` reached through a cross-overlay thunk — `j_UNIT_SummonToBattle__SEGRAX` from `ovr113` and `ovr111`. Production declares it `static` ([Combat.c:5548](../../MoM/src/Combat.c#L5548)), which links only because both callers now live in `Combat.c` ([11923](../../MoM/src/Combat.c#L11923) and [14035](../../MoM/src/Combat.c#L14035)). A linkage narrowing with no asm counterpart; harmless today, but it comes off if either caller moves to its own translation unit.

---

# Summon_Demon

## Frame map

`sub sp, 4` (asm:10) = 4 bytes, plus two register locals. One parameter at `bp+6`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-4` | `cgy` | `cgy` |
| `bp-2` | `caster_attribs_1` | `caster_attribs_1` |
| `bp+6` | `caster_idx` | `caster_idx` |
| `si` | `_SI_caster_idx` | (the parameter, used directly) |
| `di` | `_DI_cgx` | `cgx` |

## Faithful — verified, leave alone

- **The charge counter is two USA bits used as a 2-bit count**, decremented by an unusual three-armed dance (asm:22-82). `USA_SUMMON_DEMON_1` alone → clear it. `USA_SUMMON_DEMON_2` alone → clear it *and* set `_1`. Anything else — including both bits set, and including neither — → clear `_1`. Production [14003-14016](../../MoM/src/Combat.c#L14003-L14016) reproduces all three arms including the `else`, which fires on a state the caller should never produce.
- **`Create_Unit` argument order** — pushes are `2000`, `9`, `0`, `0`, `controller_idx`, `ut_Demon`; right-to-left that is `(ut_Demon, controller_idx, 0, 0, 9, 2000)` (asm:84-103), matching the six-parameter signature at [NEXTTURN.c:1023](../../MoM/src/NEXTTURN.c#L1023). Production [14023](../../MoM/src/Combat.c#L14023). `wp = 9` is what marks the unit as combat-summoned, and `Prepare_Battle_Unit_Summons` looks for exactly that value when recycling a slot (asm:48).
- **The placement retry is a `do`/`while`, not a `while`** — `loc_918F4` (asm:104) is reached by falling out of the `Create_Unit` call, so the body always runs once before the test at asm:140-141. Production [14024-14034](../../MoM/src/Combat.c#L14024-L14034).
- **The two sides get disjoint 3×3 boxes.** Attacker `cgx = 14 - Random(3)` (asm:114-120); defender `cgx = 7 + Random(3)` (asm:123-128); both `cgy = 8 + Random(3)` (asm:130-135). `Random(n)` is 1-based, so the attacker draws x ∈ {11,12,13} and the defender x ∈ {8,9,10}, both over y ∈ {9,10,11} — nine cells each, exactly the count in the original's own infinite-loop warning.
- **The occupancy test is a signed byte compare** — `cmp [byte ptr bx], 0` / `jge` retries while the cell is `>= 0`, i.e. holds a unit index (asm:140-141). Only the negative "empty" marker ends the loop.
- **The two calls are handed different index bases** — `_units - 1` (the new *unit* record) and `_combat_total_unit_count - 1` (the new *battle unit* slot), asm:144-146 and asm:162-164. Production [14035-14051](../../MoM/src/Combat.c#L14035-L14036). The second reads the counter *after* `Prepare_Battle_Unit_Summons` has incremented it.
- **`spell_idx` is passed as literal `0`** to `Battle_Unit_Summon_Animation` (asm:158-159), not a real spell id — this is a unit ability, not a spell. Production [14036](../../MoM/src/Combat.c#L14036).

## OGBUGs — faithful, do not fix

- **Infinite loop when all nine target cells are occupied.** The retry loop at asm:104-141 has no bail-out and no attempt counter; if every cell in the caster's 3×3 box holds a unit it spins forever drawing new coordinates. Annotated at [13992](../../MoM/src/Combat.c#L13992).

# Prepare_Battle_Unit_Summons

Faithful.

Two paths, and the distinction matters:

- **Room available** — `_combat_total_unit_count < MAX_BATTLE_UNIT_SLOT_COUNT` (36, matching `e_MAX_BATTLE_UNIT_SLOTS` at `WIZARDS.inc:9224`): load into the next free slot, then increment the counter (asm:14-26). Production [5551-5554](../../MoM/src/Combat.c#L5551-L5554).
- **Slots exhausted** — scan for a battle unit with `status > bus_Active` *and* `_UNITS[].wp == 9`, i.e. a dead unit that was itself combat-summoned, and reuse its slot (asm:27-64). The counter is **not** incremented on this path, correctly, since no new slot is consumed. Production [5551-5579](../../MoM/src/Combat.c#L5551-L5579).

If the scan finds nothing the function returns having done nothing, and `Summon_Demon` then calls `Battle_Unit_Summon_Animation(_combat_total_unit_count - 1, …)` against a stale slot. The original has no guard; production adds only the `STU_DEBUG_BREAK()` of D1, which does not change behaviour.

# Prepare_Battle_Unit

Faithful.

`Load_Battle_Unit` is called with `&battle_units[battle_unit_idx]` built explicitly as a far pointer — the listing pushes the segment then the offset — then `Battle_Unit_Pict_Open` supplies a figure-buffer index that is passed to `Combat_Figure_Load` alongside `_UNITS[unit_idx].type` and finally stored into `bufpi`. Production [5578-5602](../../MoM/src/Combat.c#L5578-L5602).

The remaining stores — `controller_idx`, `cgx`, `cgy`, `target_cgx`, `target_cgy`, `move_anim_ctr`, `outline_magic_realm`, `Atk_FigLoss`, `Moving`, `action` — are written one at a time in listing order, each recomputing the base address. Production [5585-5596](../../MoM/src/Combat.c#L5585-L5596) keeps the order. `controller_idx` is stored as a **byte** from `[bp+player_idx]`, which the listing declares `byte ptr 8` even though the caller pushes a word; production's `(int8_t)` cast at [5585](../../MoM/src/Combat.c#L5585) is right.

## OGBUGs — faithful, do not fix

- **Battlefield effects are never applied to the summoned unit — but the "bug" is unproven.** The behaviour is real: nothing in this function's 121 lines calls `BU_Apply_Battlefield_Effects__WIP`, so a demon arrives without Prayer, Black Prayer, Terror or a node aura.

  What the listings do **not** show is that this is an oversight. The other path that creates a new battle unit behaves identically — `ovr111/Cast_Spell_On_Battle_Unit.asm` runs `Create_Unit` (1773) → `Prepare_Battle_Unit_Summons` (1784) → `Battle_Unit_Summon_Animation` (1793) with no effects call anywhere in that block; its last one is back at 1663. Two independent new-unit paths agreeing is consistency, not omission.

  `ovr131/Cast_Animate_Dead.asm:367` and `ovr131/Cast_Raise_Dead.asm:398` *do* apply effects before their summon, and an earlier draft of this review cited them as proof. That was the wrong comparison: neither calls `Create_Unit` or `Prepare_Battle_Unit_Summons` — they reanimate a battle unit that already exists on the field, which is a different situation.

  So the `¿ … ?` at [Combat.c:5578](../../MoM/src/Combat.c#L5578) is correctly hedged and should stay hedged. Settling it needs MoM's intended rule for whether battlefield enchantments reach newly summoned units, which is not in the disassembly. Faithful either way — do not add the call.

- **No guard on an invalid `bufpi`.** `Battle_Unit_Pict_Open` is called without checking its result. That function returns the first free figure-buffer index, but when all 18 are held by `bus_Active` units its final loop falls through without setting `AX` — so the caller receives whatever was last in the register, a stack offset, and hands it straight to `Combat_Figure_Load`. Annotated at [5580](../../MoM/src/Combat.c#L5580); production's two `assert`s (D1) surface it in Debug, and `Battle_Unit_Pict_Open` itself carries a `/* HACK */ return ST_UNDEFINED;` so the bad value is at least detectable.

# Battle_Unit_Summon_Animation

Faithful apart from R1.

A 16-frame summon animation wrapped around the moment the unit becomes real.

- **The unit is `bus_Dead` for the first fourteen frames** and flips to `bus_Active` exactly at `anim_ctr == 14` (asm:134-141), two frames before the loop ends. Production [1180](../../MoM/src/Spells133.c#L1180) and [1189-1191](../../MoM/src/Spells133.c#L1189-L1191). That is what keeps it from being drawn or targeted mid-animation.
- **The graphics are Fire Elemental's, whatever is being summoned** — `Spell_Animation_Load_Graphics(spl_Fire_Elemental)` is hard-coded (asm:57-59). Production [1155](../../MoM/src/Spells133.c#L1155).
- **`target_cgx` is set to a fixed off-grid value by side** — 8 for the attacker, 14 otherwise, with `target_cgy` always 12 (asm:84-113). Production [1170-1178](../../MoM/src/Spells133.c#L1170-L1178). These are animation targets, not placement.
- **`uu_screen_x` / `uu_screen_y` are computed and never read** (asm:46-51). Faithful dead stores; production [1150-1151](../../MoM/src/Spells133.c#L1150-L1151) keeps them.
- **The rise animation runs only for `anim_ctr` in 7..13** — `jle 6` and `jge 14` both skip (asm:162-165). Within it `Vanish_Bitmap` gets `(anim_ctr - 6) * 14` and the Y offset shrinks by `(anim_ctr - 6) * 3`, so the sprite rises three pixels per frame while dissolving. Production [1199-1207](../../MoM/src/Spells133.c#L1199-L1207).
- **Argument orders** — `Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y)` (asm:34-44); `Create_Picture(45, 42, scratch_bitmap_seg)` (asm:150-155); `Copy_Bitmap_To_Bitmap(scratch_bitmap_seg, battle_unit_scratch_seg)` (asm:157-159); `Set_Window(0, 0, SCREEN_XMAX, screen_y + 30)` (asm:175-184, `e_SCREEN_XMAX` = 319); `Combat_Cast_Spell_Message(player_idx, spell_idx)` (asm:145-147). All match production.
- **Both `Mark_Block` calls are paired with `Release_Block` at the tail**, `_screen_seg` and `World_Data` in that order (asm:54-56, 64-66, 218-223). Production [1154](../../MoM/src/Spells133.c#L1154), [1158](../../MoM/src/Spells133.c#L1158), [1217-1218](../../MoM/src/Spells133.c#L1217-L1218).

---

## Build state

**Clean.** `cmake --build --preset MSVC-debug` run 2026-08-05, after the R1 fix and the seven renames in the ledger at the top of this file. No warnings, no errors.

**Header prototypes checked.** [Combat.h:1760](../../MoM/src/Combat.h#L1760) (`Summon_Demon`), [Combat.h:1525](../../MoM/src/Combat.h#L1525) (`Prepare_Battle_Unit`) and [Spells133.h:60](../../MoM/src/Spells133.h#L60) (`Battle_Unit_Summon_Animation`) all match their definitions. `Prepare_Battle_Unit_Summons` is `static` and correctly has no prototype — see D3.

**Anchors.** Scope-table anchors and every `Combat.c#L…` / `Spells133.c#L…` reference were read on 2026-08-04. Both files shift under every edit, so re-verify before acting on a line number; the `asm:N` references are stable.

## Related references

- [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) — the HACK two-value-return pattern of D2; DONE-DONE.
- [Combat-Battle_Unit_Action.md](Combat-Battle_Unit_Action.md) — where `g_combat_grid_action_map` was reviewed under its other name; DONE-DONE.
- `Cast_Spell_On_Battle_Unit` (`ovr111`, 1806 lines) — the other caller of both `Prepare_Battle_Unit_Summons` and `Battle_Unit_Summon_Animation`, and the path that *does* apply battlefield effects. Not reviewed; the natural next bundle if the summon OGBUG is ever revisited.
- `Create_Unit` ([NEXTTURN.c:1023](../../MoM/src/NEXTTURN.c#L1023)) — 12 callers, own bundle.
