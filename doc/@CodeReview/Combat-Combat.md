Combat-Combat.md

Combat - general/generic; 'Tactical Combat', but not 'Combat Screen' specific  e.g., Calc_Battlefield_Bonuses(), but, also, unless it's AI specific.

SEEALSO:  Combat-Combat.md
SEEALSO:  Combat-Combat_Screen.md
SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-Generate_Combat_Map.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  Combat-Strategic_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md

Combat__WIP()  ==>  Combat()

GAME_RazeCity  ==>  destroy_combat_city

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Combat__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr124\STK_CaptureCity.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr153\EmptyFxn_o153p24.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Lair_Make_Guardians.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Update_Defender_Hostility.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Lair_Combat_Do.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr122\Calc_Battlefield_Bonuses.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr105\Check_For_Winner.asm

// MoO2  Module: COMBINIT  Tactical_Combat_()
...MoM effectively combines Combat_Screen_() and Tactical_Combat_()

Move_Units()
    |-> j_Combat()
        |-> Combat()

Lair_Combat_Do()
    |-> Combat()

NIU_Enemy_Stack_Combat()
    |-> Combat()

Combat()
    |-> Lair_Make_Guardians()
    |-> Update_Defender_Hostility()
    |-> Lair_Combat_Do()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-27).** No findings open. Eight functions, 2,153 asm lines, every one walked against its listing. Reopened 2026-08-25 to adjudicate `Calc_Battlefield_Bonuses` and `Check_For_Winner`, both faithful; see *The battlefield-bonus and winner checks* below.

`WIZARDS.EXE ovr096` — `Combat` is the entry point for every battle: it normalises the troop positions, decides from `_combat_environ` whether this is stack-versus-stack, a city siege or a lair, runs the treaty check, dispatches to either `Combat_Screen` or `Strategic_Combat`, and then does all the post-combat bookkeeping — item pool, city ownership, diplomacy, razing, map redraw.

One finding was raised — the same inverted `UA_FANTASTIC` test at both city-capture sites — plus six structural deviations. All six are applied.

## Renames

| dasm | production |
| --- | --- |
| `Combat__WIP` | `Combat` |
| `STK_CaptureCity` | `Combat_City_Capture` |
| `Combat_Screen__WIP` | `Combat_Screen` |
| `Strategic_Combat__STUB` | `Strategic_Combat` |
| `Create_Unit__WIP` | `Create_Unit` |
| `Change_Relations__WIP` | `Change_Relations` |
| `Allocate_Reduced_Map__1` | `Allocate_Reduced_Map` |
| `EmptyFxn_o153p24` | `o153p24_empty_function` |
| `Play_Background_Music__STUB` | `Play_Background_Music` |
| `magic_set.Strategic_Combat` | `magic_set.strategic_combat_only` |
| `bt_GRANDVIZIER` (65532) | `bt_AUTOBUILD` (-4) — same 16-bit value |

`Lair_Make_Guardians`, `Lair_Combat_Do` and `Update_Defender_Hostility` keep their dasm names.

## Scope

| function | production | listing | asm lines | coverage |
| --- | --- | --- | --- | --- |
| `Combat` | [Combat.c:3085](../../MoM/src/Combat.c#L3085) | `ovr096/Combat__WIP.asm` | 825 | **full body walk — R1** |
| `Battle_Unit_Is_Summoned_Creature` | [Combat.c:16048](../../MoM/src/Combat.c#L16048) | `ovr124/BU_IsCombatSummon.asm` | 59 | **full body walk — faithful** |
| `Combat_City_Capture` | [Combat.c:16205](../../MoM/src/Combat.c#L16205) | `ovr124/STK_CaptureCity.asm` | 389 | **full body walk — faithful; one deviation** |
| `o153p24_empty_function` | [Combat.c:19705](../../MoM/src/Combat.c#L19705) | `ovr153/EmptyFxn_o153p24.asm` | 7 | **full body walk — faithful** |
| `Lair_Make_Guardians` | [Combat.c:3431](../../MoM/src/Combat.c#L3431) | `ovr096/Lair_Make_Guardians.asm` | 191 | **full body walk — faithful** |
| `Update_Defender_Hostility` | [Combat.c:3515](../../MoM/src/Combat.c#L3515) | `ovr096/Update_Defender_Hostility.asm` | 69 | **full body walk — faithful** |
| `Lair_Combat_Do` | [Combat.c:3471](../../MoM/src/Combat.c#L3471) | `ovr096/Lair_Combat_Do.asm` | 68 | **full body walk — faithful** |
| `Calc_Battlefield_Bonuses` | [Combat.c:13259](../../MoM/src/Combat.c#L13259) | `ovr122/Calc_Battlefield_Bonuses.asm` | 450 | **full body walk — faithful** |
| `Check_For_Winner` | [Combat.c:6881](../../MoM/src/Combat.c#L6881) | `ovr105/Check_For_Winner.asm` | 154 | **full body walk — faithful** |

`Combat` calls only two of its same-overlay siblings — `Lair_Make_Guardians` at `asm:293` and `Update_Defender_Hostility` at `asm:772`. `Lair_Combat_Do` is included because it is the lair entry point that calls back into `Combat`, from [Lair.c:340](../../MoM/src/Lair.c#L340).

---

# Findings

None open.

---

# Verified faithful

**Frames.** All four match. `Combat` has six slotted locals and [Combat.c:3103-3108](../../MoM/src/Combat.c#L3103-L3108) declares them in frame order — `Item_List` at `bp-2Eh` (18 words) through `Battle_Outcome` at `bp-2`, with `defender_idx` in DI and `itr` in SI. `Lair_Combat_Do`, `Lair_Make_Guardians` and `Update_Defender_Hostility` likewise.

**`Combat` — the two `_combat_environ` switches.** Both are compare chains in the asm rather than jump tables, which is what Borland emits for the sparse case set `{0, 1, 5}`. The neutral-attacker switch and its three early-outs to `@@IDK_No_Combat` match, as does the environment switch that sets `_combat_wx/_wy/_wp` from the defending unit, the city or the lair, converts `defender_idx` from a unit index to a player index in case 0, counts the garrison in case 1, and calls `Lair_Make_Guardians` in case 5. The `BNR_Brown` banner write resolves to `_players[NEUTRAL_PLAYER_IDX]` — `asm:213` writes `[_players.banner_id+17E8h]`, and 0x17E8 is `NEUTRAL_PLAYER_IDX * sizeof(s_WIZARD)`.

**`Combat` — the treaty check.** The three-clause guard, the `DIPL_Alliance` / `DIPL_WizardPact` pair, the message assembly, `Confirmation_Box` tested for non-zero, the `< 6` pair, and `Change_Relations(-40, attacker, defender, 5, 0, 0)` followed by `Break_Treaties(defender, attacker)` — argument order verified against the push sequence at `asm:392-410`.

**`Combat` — dispatch and post-combat.** The `Garrison_Size < 1` shortcut to `Combat_City_Capture`; the `(attacker == human || defender == human) && !strategic_combat_only` split; both nine- and seven-argument call lists; the survivor repositioning loop; the item-pool bracket with `m_item_wx/_wy/_wp`; the city-ownership and razing cascade; the duplicated `City_Destroyed` and `destroy_combat_city` blocks; and the closing redraw gated on either side being human.

**`Lair_Make_Guardians`.** Both guard loops, `guard_count = guard*_count & 0x0F`, the compound `(itr < guard_count) && (unit_count < MAX_STACK)` condition carried across both loops so the pair shares one `MAX_STACK` budget, the six-argument `Create_Unit` list with its literal 2000, and the `< ut_Magic_Spirit` undead mutation applied to `_UNITS[_units - 1]`.

**`Update_Defender_Hostility`.** `Hostility[attacker] = 2`, raised to 3 on `>= DIPL_War`, dropped to 0 on `== DIPL_Alliance`, then `reevaluate_hostility_countdown = 15 + Random(10)`. The index arithmetic confirms the field widths: `Hostility` is word-indexed (`shl dx, 1`) and `Dipl_Status` byte-indexed, matching `uint16_t Hostility[NUM_PLAYERS]` and `int8_t Dipl_Status[NUM_PLAYERS]` ([MOM_DAT.h:1375](../../MoX/src/MOM_DAT.h#L1375), [MOM_DAT.h:1534](../../MoX/src/MOM_DAT.h#L1534)).

**`Lair_Combat_Do`.** `Player_Army_At_Square(wx, wy, wp, player_idx, &troop_count, &troops[0])` and `Combat(player_idx, NEUTRAL_PLAYER_IDX, troop_count, &troops[0])`, both argument orders verified against the push sequences.

---

# OG behaviours preserved

- **`item_pool_in_process` is cleared without ever being set on the losing path.** The winning path brackets `Player_Process_Item_Pool` with `ST_TRUE` … `ST_FALSE` (`asm:565`, `asm:578`); the losing path at `asm:686-692` calls it and then writes `ST_FALSE` with no matching `ST_TRUE`. The flag's only reader is [ItemScrn.c:239](../../MoM/src/ItemScrn.c#L239), which selects `AWARD_HERO_SLOT_IDX` over `VAULT_HERO_SLOT_IDX`, so a defender who wins collects the loot with the item screen in the wrong mode. Recorded in-code at [Combat.c:3366](../../MoM/src/Combat.c#L3366).
- **Attacking a treaty partner never declares war.** The open-field path applies the relations penalty and breaks the treaty (`asm:392-407`) but nothing on it writes `Dipl_Status` — `Change_Relations` only ever compares that field ([DIPLOMAC.c:3493](../../MoM/src/DIPLOMAC.c#L3493) onward), `Break_Treaties` adjusts `Hidden_Rel`, `Default_Rel`, `Visible_Rel` and `Broken_Treaty` but not `Dipl_Status`, and there is no `Declare_War` call. So a wizard can fight a former ally with the alliance formally intact. The city-capture path does call `Declare_War`. Recorded in-code at [Combat.c:3232](../../MoM/src/Combat.c#L3232).
- **`Break_Treaties` receives the two players in the opposite order to `Change_Relations`.** `asm:404-406` pushes the attacker then the defender, so the call is `Break_Treaties(defender, attacker)` — faithful. Inside that function the first parameter is named `attacker_idx` and is the one stamped as the oath-breaker, which puts the combat *defender* in that slot. Whether the call is wrong or the parameter names are cannot be settled from this overlay; `Break_Treaties` lives in `DIPLOMAC.c` and is out of scope here.
- **The razing cascade runs twice.** `asm:781-800` has two consecutive blocks with identical bodies — `Change_City_Ownership`, `Destroy_City`, `destroy_combat_city = ST_FALSE` — the first gated on `City_Destroyed == ST_TRUE` and the second on `destroy_combat_city == ST_TRUE`. Since the first clears `destroy_combat_city`, the second can only fire when the first did not. Reproduced verbatim at [Combat.c:3411-3417](../../MoM/src/Combat.c#L3411-L3417).
- **`defender_idx` changes meaning mid-function.** It arrives as a unit index for stack-versus-stack and is overwritten with `_UNITS[defender_idx].owner_idx` in case 0 ([Combat.c:3166](../../MoM/src/Combat.c#L3166)), after which every later use treats it as a player index. Case 1 and case 5 never rewrite it, so it must already be a player index on those paths.

---

# The battlefield-bonus and winner checks

Two functions this review's header block named from the start and never adjudicated. Both walked 2026-08-25, both faithful, 604 asm lines between them. Neither lives in `ovr096` — `Calc_Battlefield_Bonuses` is `ovr122`, `Check_For_Winner` is `ovr105` — but both are general tactical-combat services rather than screen or AI code, which is what this document collects.

## `Calc_Battlefield_Bonuses` ([Combat.c:13259](../../MoM/src/Combat.c#L13259), asm 450)

Faithful, all 450 lines. It zeroes the three per-player bonus arrays, walks every battle unit accumulating the best Holy Bonus, Resistance to All and Leadership each side has, and finally folds Holy Bonus into Resistance to All.

**Frame.** Two stack locals — `leadership_level` at `bp-4`, `prayermaster_level` at `bp-2` — so `leadership_level` is declared first, which is production's order. `_SI_itr` and `_DI_battle_unit_owner_idx` are register locals. Production declares four more counters after the two slotted ones; that carries no ordering constraint and says nothing about the source, since the compiler is free to share SI across all of them.

**The Eternal Night loop reads exactly as written.** `jle` past the body for `Globals[ETERNAL_NIGHT] > 0` (asm:32-33), then `cmp True_Light.Dfndr, 2 / jz` past for `!= 2` (asm:35-36), then the three-way attacker / defender / neither split at asm:37-52. The *neither* arm writes `DARKNESS_DFNDR` before `DARKNESS_ATTKR`, and production keeps that order.

**The zero loop is bounded at 7** — `cmp _SI_itr, 7` (asm:78) — which is `NUM_PLAYERS + 1`; [MOM_DEF.h:96](../../MoX/src/MOM_DEF.h#L96) puts `NUM_PLAYERS` at 6. The three arrays clear in the order `_battlefield_holybonus`, `_battlefield_resistall`, `_battlefield_leadership` (asm:63, 68, 73), matching production.

**All four hero-ability tests are split 32-bit masks, and each half is on the correct side.** The listing splits the `abilities` dword, masks one word and zeroes the other:

| listing | word masked | production macro | constant |
| --- | --- | --- | --- |
| asm:232-234 | high (`and dx, 0`) | `HERO_PRAYERMASTER` | `HSA_PRAYERMASTER` = `0x01000000` |
| asm:285-287 | high | `HERO_PRAYERMASTER2` | `HSA_PRAYERMASTER2` = `0x02000000` |
| asm:343-345 | low (`and ax, 0`) | `HERO_LEADERSHIP` | `HSA_LEADERSHIP` = `0x00000001` |
| asm:386-388 | low | `HERO_LEADERSHIP2` | `HSA_LEADERSHIP2` = `0x00000002` |

Macros at [MOX_DEF.h:376-377](../../MoX/src/MOX_DEF.h#L376-L377) and [MOX_DEF.h:392-393](../../MoX/src/MOX_DEF.h#L392-L393); constants at [MOM_DEF.h:788-789](../../MoX/src/MOM_DEF.h#L788-L789) and [MOM_DEF.h:812-813](../../MoX/src/MOM_DEF.h#L812-L813). Production folds the OG's `Abilities_LO`/`Abilities_HI` pair into one 32-bit field, so the word split disappears — but every mask lands in the half the listing tested.

**The four level formulas.** Three different divide idioms, all reproduced correctly:

| production | listing | idiom |
| --- | --- | --- |
| `Level + 1` | asm:248 `inc ax` | — |
| `((Level + 1) * 3) / 2` ([13533](../../MoM/src/Combat.c#L13533)) | asm:303-306 | `imul` then `cwd / sub ax,dx / sar ax,1` |
| `(Level + 1) / 3` ([13542](../../MoM/src/Combat.c#L13542)) | asm:360-362 | `mov bx, 3 / cwd / idiv bx` |
| `(Level + 1) / 2` | asm:403-405 | `cwd / sub ax,dx / sar ax,1` |

The `/ 3` uses a real `idiv` against a constant loaded into BX rather than the shift idiom, which is what Borland emits for a non-power-of-two signed divisor. The `cwd` at asm:304 also truncates the `* 3` product to 16 bits before the halve; immaterial, since a 16-bit `int` already does.

**The closing loop direction is `_battlefield_resistall[itr] += _battlefield_holybonus[itr]`** — asm:433-440 loads from holybonus and `add`s into resistall, in that direction. [Combat.c:13561](../../MoM/src/Combat.c#L13561) matches.

**The city-wall comment is a question about the original, not a reconstruction defect.** [Combat.c:13494](../../MoM/src/Combat.c#L13494) marks the `combat_structure == cs_City && controller_idx == _combat_defender_player` block `OGBUG: should be checking for City Wall c&p error from 'Strategic Combat'?`. The listing does exactly what production does — asm:93-118: `cs_City`, defender, `add al, 3` into `defense`. The reconstruction is faithful; whether the OG *should* have tested for a wall is an open design question, and the trailing `?` says as much. Left as-is.

## `Check_For_Winner` ([Combat.c:6881](../../MoM/src/Combat.c#L6881), asm 154)

Faithful, all 154 lines. Counts live units per side, applies elimination and the turn-50 rule, then gives the AI side a chance to concede.

**Frame.** One stack local, `defender_count` at `bp-2`, with `itr` in SI and `attacker_count` in DI. Production declares `defender_count` first, which is the only ordering the listing constrains.

**The confusion swap is symmetric and both halves are the right way round.** For an attacker-controlled unit, `confusion_state == 2` increments `defender_count` (asm:41-43); for a defender-controlled unit it increments `attacker_count` (asm:64-66). Reversing either would be invisible in normal play and decisive in a confused battle. Both match.

**The three exit conditions, in listing order:** `or di, di / jnz` for `attacker_count == 0` (asm:77-78), `cmp defender_count, 0 / jnz` (asm:84-85), and `cmp _combat_turn, 50 / jle short <next> / jmp short @@JmpDone_Return_DEFENDER` (asm:89-91) — a long-branch fixup whose surviving `jle` makes the source condition `> 50` un-inverted. [Combat.c:6981-6984](../../MoM/src/Combat.c#L6981-L6984).

**The flee path.** Guarded by `_combat_computer_player != NEUTRAL_PLAYER_IDX` (asm:93-95, long-branch), then `_computer_player_city_seige == ST_FALSE && _combat_turn > 1` (asm:97-100, two skips = `&&`), then `Retreat_Check(...) == ST_TRUE`. Inside, every unit of the AI side gets `action = bua_Flee` **unconditionally**, and `status = bus_Fleeing` **only** when it was `bus_Active` (asm:123-136). The nesting matters and production preserves it. The return is `_combat_human_player`, not the AI side.

**Two things to look at, neither a code defect:**

- **The `OGBUG` labels on the confusion test assert a defect with no mechanism.** [Combat.c:6953](../../MoM/src/Combat.c#L6953) and [Combat.c:6965](../../MoM/src/Combat.c#L6965) both read `OGBUG: The assembly compares confusion_state to 2`. The assembly does compare to 2 (asm:41, asm:64) and so does production, so the label is a claim about the *original*, not the reconstruction — but it never says what is wrong with 2. `confusion_state` is `int8_t` at offset `0x56` and its value set is documented as `{0: , 1: , 2: }` with all three blank ([Combat.h:1292](../../MoM/src/Combat.h#L1292)), so nothing on hand supports or refutes it. Worth noting that a *different* field carries `bue_Confusion = 0x2` as a `combat_effects` **bit** ([Combat.h:865](../../MoM/src/Combat.h#L865)); a bit-versus-state conflation would explain the label. Give it a mechanism or drop it.
- **Three `LOG_DEBUG` calls** at [Combat.c:6977-6979](../../MoM/src/Combat.c#L6977-L6979) have no asm counterpart. Port instrumentation, the same category as D4 in [Combat-End_Of_Combat.md](Combat-End_Of_Combat.md).

## Renames — the reopened pair

| dasm | production |
| --- | --- |
| `prayermaster_level` | `prayer_level` |
| `Ab_HolyBonus` / `Ab_ResistAll` | `USA_HOLYBONUS` / `USA_RESISTALL` |
| `HSA_LO_Leadership` / `HSA_HI_Prayermaster` | `HSA_LEADERSHIP` / `HSA_PRAYERMASTER`, on a single 32-bit `abilities` |
| `bus_Flee` | `bus_Fleeing` ([Combat.h:796](../../MoM/src/Combat.h#L796)) |
| `j_Retreat_Check` | `AI_Retreat_Check` |
| `combat_computer_player` / `combat_human_player` | `_combat_remote_player` / `_combat_local_player` |

---

# City capture and the second empty slot

`o124p17` and `ovr153p24`, walked 2026-08-27, 396 asm lines. Both faithful; one deviation.

## `Combat_City_Capture` ([Combat.c:16205](../../MoM/src/Combat.c#L16205), asm 389)

IDA's `STK_CaptureCity`. Runs after the attacker takes a city: adjusts both sides' fame, decides
whether the city is razed, moves the gold, and hands off to `Rampage_Combat_City` when neutral
fantastic units did the taking.

**One stack local.** `city_owner_idx` at `bp-2` is the only slot; `troop_owner_idx` is DI and `itr`
is SI. Production declares nine locals, but only the first is constrained by the frame and it is
`city_owner_idx`. The rest are register-class or reconstruction aids — `fame_penalty` is marked
`DNE in Dasm`, and `city_ptr` / `unit_ptr` / `wizard_ptr` are pointer conveniences with no slot.

**The opening four calls are in listing order** — `Unit_View_Allocate`, `Set_Page_Off`,
`Main_Screen_Draw`, `Copy_Off_To_Back` (asm:13-16) — then `troop_owner_idx` comes from
`_UNITS[troops[0]].owner_idx` (asm:17-25) and `city_owner_idx` from the city record.

**The raze decision is a three-way split and production reproduces it exactly.** asm:80-112:
population zero forces `destroy_combat_city = ST_TRUE`; otherwise it clears the flag, and then
`or _DI_troop_owner_idx, _DI_troop_owner_idx / jnz` (asm:193 uses the same idiom later) selects the
human path, which asks `magic_set.raze_city` and then `Raze_City_Prompt`, against an `else if
(troop_owner_idx < _num_players)` AI path that takes `Raze_Check`'s return directly.
`HUMAN_PLAYER_IDX` is 0 ([MOX_DEF.h:627](../../MoX/src/MOX_DEF.h#L627)), which is what makes the
`or reg, reg` test equivalent to the named comparison production writes.

**The keep-city and raze-city arms** hang off `cmp [GAME_RazeCity], e_ST_FALSE` at asm:114.
`GAME_RazeCity` is `destroy_combat_city` in production, already in the rename ledger above.

**The building-salvage loop starts at 3, not 0** — asm:243 is `mov _SI_itr, 3` and asm:267 bounds it
at 36. Each built slot adds `construction_cost / 10` via a real `idiv` (asm:262-263). Production
matches at [Combat.c:16457-16463](../../MoM/src/Combat.c#L16457-L16463), including the
`> bs_NotBuilt` test rather than a truthiness check.

**The rampage hand-off** checks `_UNITS[troops[0]].owner_idx == NEUTRAL_PLAYER_IDX` (asm:294) then
`Abilities & UA_FANTASTIC` (asm:309), fills `controller_idx` / `status` / `unit_idx` for each troop
(asm:313-343), sets `_combat_total_unit_count` and calls `Rampage_Combat_City`. All present, in
order.

### Deviation — the raze branch's fame penalty uses a local where the OG reuses the global

asm:192-205, in the raze arm:

```
mov     [CMB_Gold_Reward], ax        ; = _CITIES[...].size
or      _DI_troop_owner_idx, _DI_troop_owner_idx
jnz     short loc_A87AA
mov     ax, [CMB_Gold_Reward]        ; RE-READ
sub     [GUI_Multipurpose_Int], ax
loc_A87AA:
cmp     _DI_troop_owner_idx, [_num_players]
jge     short loc_A87E0
...
mov     dx, [CMB_Gold_Reward]        ; RE-READ
sub     [_players.fame+bx], dx
```

The global is written once and **read back twice**, so the OG genuinely uses `CMB_Gold_Reward` as
the scratch variable across those three statements. Production substitutes a local, `fame_penalty`,
at [Combat.c:16426](../../MoM/src/Combat.c#L16426), marked `DNE in Dasm`.

Behaviour is unchanged — the global is overwritten by `City_Gold`'s return at asm:221 before anything
reads it for the results scroll, so nothing downstream can observe the difference. Recorded because
the target is 1:1 with the listing, and because the substitution hides something worth seeing: on
this path the global holds a **fame penalty**, not gold. That is the clearest single piece of
evidence that IDA's `Gold_Reward` label is wrong, and the reason the rename proposed for it is
`combat_results_gold` rather than anything with "reward" in it.

## `o153p24_empty_function` ([Combat.c:19705](../../MoM/src/Combat.c#L19705), asm 7)

Faithful. The listing is a bare frame — `push bp` / `mov bp, sp` / `pop bp` / `retf` — with no body,
and production is an empty function with those four instructions preserved as comments.

Unlike its `ovr153` sibling `o153p22_empty_function`, which nothing calls, **this one is live**:
[Combat.c:3429](../../MoM/src/Combat.c#L3429) calls it from `Combat`, between clearing
`_combat_environ_idx` / `_combat_environ` and `Allocate_Reduced_Map()`. Whatever it did was removed
before ship; the call survived. Keep both the call and the empty body.

---

# `Battle_Unit_Is_Summoned_Creature`

`o124p15`, walked 2026-08-27, 59 asm lines. Faithful, no findings.

## `Battle_Unit_Is_Summoned_Creature` ([Combat.c:16048](../../MoM/src/Combat.c#L16048), asm 59)

IDA's `BU_IsCombatSummon`. Answers whether a battle unit is one of the five combat-summonable
creature types, or a Demon.

No stack locals; `unit_type` is SI.

A six-term `||` chain, each term jumping to a shared `@@JmpDone__Return_TRUE` and the last falling
through to `@@JmpDone__Return_FALSE` (asm:21-53). The order is `spl_Fire_Elemental`,
`spl_Earth_Elemental`, `spl_Phantom_Beast`, `spl_Phantom_Warriors`, `spl_Air_Elemental`, then a
direct `unit_type == ut_Demon`, and production lists them in exactly that order.

Two details preserved: the five spell terms compare `spell_data_table[...].Param0` — production's
`.unit_type` — with the table field on the **left**, matching `cmp es:[bx+...], _SI_unit_type`; and
the sixth term is a plain type comparison, not a table lookup, because Demons are summoned outside
combat.
