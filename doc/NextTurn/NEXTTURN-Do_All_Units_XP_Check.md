NEXTTURN-Do_All_Units_XP_Check.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Do_All_Units_XP_Check.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr140\Do_All_Units_XP_Check.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Do_All_Units_XP_Check()       NEXTTURN

---

# `Do_All_Units_XP_Check` — Walkthrough

**Location:** [NEXTTURN.c:4370-4521](../../MoM/src/NEXTTURN.c#L4370-L4521) (~152 lines, ends [line 4521](../../MoM/src/NEXTTURN.c#L4521)).
**WZD overlay:** ovr140.
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c#L821) — normal end-of-turn (Wave 12A).

> Verified against the **`.asm`** (527 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** One deliberate platform-safety deviation (the offset-backed `processed_hero_list`, below). Builds clean (Debug, MSVC).

## Purpose

Per-turn unit maintenance in two passes. **Pass 1** (all units): Herb Mastery full-heal, Stasis countdown, and the `+1 XP` / level-recalc (with a human hero level-up popup). **Pass 2** (all players × their heroes): an Armsmaster hero grants bonus XP to the non-hero units sharing its square. Both passes honour Time Stop (only the time-stopping player's units tick).

## Signature

```c
void Do_All_Units_XP_Check(void)
```

Locals: `battle_unit` (static, for the Stasis resistance check), `processed_hero_list_backing[NUM_HEROES + 1]` + `processed_hero_list` (offset pointer, see deviation), `troop_list[MAX_STACK]`, `hero_level`, `hero_unit_idx`, `troop_count`, `highest_armsmaster_xp`, `xp_gain`, `hero_slot` (`int8_t`, DNE in Dasm — hoisted temp), `itr_units`/`itr_players` (`_SI_`), `itr_troops` (`_DI_`), `itr_heroes`.

## Code walk

Line refs are production [NEXTTURN.c](../../MoM/src/NEXTTURN.c); cross-checked against `Do_All_Units_XP_Check.asm`.

### Pass 1 — per unit ([4388-4454](../../MoM/src/NEXTTURN.c#L4388-L4454), asm:20-225)

1. **Herb Mastery heal** ([4392-4401](../../MoM/src/NEXTTURN.c#L4392-L4401), asm:23-60) — `Globals[HERB_MASTERY] > 0 && race_type != rt_Death && !(mutations & UM_UNDEAD)` → `Damage = 0`.
2. **Stasis** ([4405-4417](../../MoM/src/NEXTTURN.c#L4405-L4417), asm:61-133) — `C_STASISLINGER` set: `Load_Battle_Unit`, and if `Combat_Resistance_Check(battle_unit, -5, sbr_Sorcery) == 0` clear the linger bit (`^=`). `C_STASISINIT` set: promote to `C_STASISLINGER` and clear the init bit.
3. **XP / Level** ([4424-4449](../../MoM/src/NEXTTURN.c#L4424-L4449), asm:134-219) — for non-`UA_FANTASTIC`, non-undead units **belonging to the time-stopping player (or always, if no Time Stop)**: `XP += 1`; `hero_level = Calc_Unit_Level`; a human hero (`hero_slot > ST_UNDEFINED`) whose stored `Level < hero_level` shows `Hero_LevelUp_Popup`; then `Level = Calc_Unit_Level`.

### Pass 2 — per player, per hero ([4455-4520](../../MoM/src/NEXTTURN.c#L4455-L4520), asm:228-519)

For each player (gated by the same Time-Stop test), reset `processed_hero_list`, then for each owned hero not yet processed: gather the hero's square garrison (`Army_At_Square_1`), mark heroes in that stack processed, and

- **Find best Armsmaster** ([4479-4504](../../MoM/src/NEXTTURN.c#L4479-L4504), asm:328-454) — over the stack: mark each unit's `Hero_Slot` in `processed_hero_list` (unconditional; the `-1` case lands in the backing slot, see deviation), then for real hero slots (`hero_slot > ST_UNDEFINED`, asm:358-360) `HSA_ARMSMASTER` → `xp_gain = (Level+1)*2`, else `HSA_ARMSMASTER2` → `(Level+1)*3`; keep the max in `highest_armsmaster_xp`.
- **Award to non-heroes** ([4505-4516](../../MoM/src/NEXTTURN.c#L4505-L4516), asm:456-506) — if `highest_armsmaster_xp > 0`, every `Hero_Slot == -1` unit in the stack gets `XP += highest_armsmaster_xp` and a `Level` recalc.

## ReMoM deviations (deliberate — not OG-faithful, not bugs)

- **`processed_hero_list` is an offset-backed array** ([4374-4375](../../MoM/src/NEXTTURN.c#L4374-L4375)) — the asm marks each stack unit processed with an **unconditional** `Processed_Hero_List[Hero_Slot] = ST_TRUE` (asm:333-348), and for a non-hero unit (`Hero_Slot == -1`) that becomes `Processed_Hero_List[-1]` — the drake178-flagged `// BUG: this can also be -1!`. In the OG (Borland C++ 3.0, no runtime checks) that `[-1]` store harmlessly scribbles an adjacent stack local; under MSVC `/RTCs` it clobbers a stack guard byte and trips *"Run-Time Check Failure #2 - Stack around the variable 'processed_hero_list' was corrupted."* ReMoM keeps the write **unconditional (1:1 with asm:333-348)** but declares a `processed_hero_list_backing[NUM_HEROES + 1]` and points `processed_hero_list = &backing[1]`, so the `-1` write resolves to the reserved in-bounds `backing[0]` (never read) instead of an out-of-bounds guard byte. Faithful in effect — the mark was only ever meaningful for real hero slots — and legal C with no RTC trip. The `Hero_Slot` field is read once into the `hero_slot` temp and reused for the write index and the `-1` compare (the asm re-reads the field at asm:342 and asm:358); same value, minor restructure.
- **`static` `battle_unit` + `memset`** ([4372](../../MoM/src/NEXTTURN.c#L4372), [4387](../../MoM/src/NEXTTURN.c#L4387)) — the asm keeps `battle_unit` as a stack local; ReMoM makes it a zero-`memset` static (the large struct is only read after `Load_Battle_Unit` fills it, so the zeroing is defensive, not behavioral).

## Related references

- `…\ovr140\Do_All_Units_XP_Check.asm` — IDA Pro 5.5 disassembly (ground truth, 527 lines).
- `…\ovr140\Do_All_Units_XP_Check.c` — Gemini translation of the asm.
- Callees: `Load_Battle_Unit`, `Combat_Resistance_Check`, `Calc_Unit_Level`, `Hero_LevelUp_Popup`, `Army_At_Square_1`.
- Constants/fields: `HERB_MASTERY`, `rt_Death`, `UM_UNDEAD`, `C_STASISLINGER`/`C_STASISINIT` (`0x40`), `sbr_Sorcery`, `UA_FANTASTIC`, `HSA_ARMSMASTER`/`HSA_ARMSMASTER2`, `g_timestop_player_num` (1-based), `_HEROES2[]` (per-player), `NUM_HEROES`, `MAX_STACK`, `HUMAN_PLAYER_IDX = 0`, `ST_UNDEFINED = -1`.
- Tracked as **Wave 12A** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
