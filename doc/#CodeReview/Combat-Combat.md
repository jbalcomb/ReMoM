Combat-Combat.md

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
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Lair_Make_Guardians.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Update_Defender_Hostility.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr096\Lair_Combat_Do.asm

Move_Units()
    |-> j_Combat()
        |-> Combat()

Lair_Combat_Do()
    |-> Combat()

UU_IDK_CMB_s7DE08()
    |-> Combat()

Combat()
    |-> Lair_Make_Guardians()
    |-> Update_Defender_Hostility()
    |-> Lair_Combat_Do()

---

# 1:1 Fidelity Review

**Status: DONE-DONE (2026-08-13).** No findings open. Four functions, 1,153 asm lines, every one walked against its listing.

`WIZARDS.EXE ovr096` — `Combat` is the entry point for every battle: it normalises the troop positions, decides from `_combat_environ` whether this is stack-versus-stack, a city siege or a lair, runs the treaty check, dispatches to either `Combat_Screen` or `Strategic_Combat`, and then does all the post-combat bookkeeping — item pool, city ownership, diplomacy, razing, map redraw.

One finding was raised — the same inverted `UA_FANTASTIC` test at both city-capture sites — plus five structural deviations. All six are applied.

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
| `Combat` | [Combat.c:3101](../../MoM/src/Combat.c#L3101) | `ovr096/Combat__WIP.asm` | 825 | **full body walk — R1** |
| `Lair_Make_Guardians` | [Combat.c:3447](../../MoM/src/Combat.c#L3447) | `ovr096/Lair_Make_Guardians.asm` | 191 | **full body walk — faithful** |
| `Update_Defender_Hostility` | [Combat.c:3503](../../MoM/src/Combat.c#L3503) | `ovr096/Update_Defender_Hostility.asm` | 69 | **full body walk — faithful** |
| `Lair_Combat_Do` | [Combat.c:3487](../../MoM/src/Combat.c#L3487) | `ovr096/Lair_Combat_Do.asm` | 68 | **full body walk — faithful** |

`Combat` calls only two of its same-overlay siblings — `Lair_Make_Guardians` at `asm:293` and `Update_Defender_Hostility` at `asm:772`. `Lair_Combat_Do` is included because it is the lair entry point that calls back into `Combat`, from [Lair.c:340](../../MoM/src/Lair.c#L340).

Two listings in `ovr096` have no production counterpart and are out of scope: `UU_IDK_Main_Screen_Draw.asm` (60) and `UU_IDK_CMB_s7DE08.asm` (72). Both are recorded as comment-only placeholders at [Combat.c:3519](../../MoM/src/Combat.c#L3519) and [Combat.h:1691](../../MoM/src/Combat.h#L1691).

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

- **`item_pool_in_process` is cleared without ever being set on the losing path.** The winning path brackets `Player_Process_Item_Pool` with `ST_TRUE` … `ST_FALSE` (`asm:565`, `asm:578`); the losing path at `asm:686-692` calls it and then writes `ST_FALSE` with no matching `ST_TRUE`. The flag's only reader is [ItemScrn.c:239](../../MoM/src/ItemScrn.c#L239), which selects `AWARD_HERO_SLOT_IDX` over `VAULT_HERO_SLOT_IDX`, so a defender who wins collects the loot with the item screen in the wrong mode. Recorded in-code at [Combat.c:3364](../../MoM/src/Combat.c#L3364).
- **Attacking a treaty partner never declares war.** The open-field path applies the relations penalty and breaks the treaty (`asm:392-407`) but nothing on it writes `Dipl_Status` — `Change_Relations` only ever compares that field ([DIPLOMAC.c:3493](../../MoM/src/DIPLOMAC.c#L3493) onward), `Break_Treaties` adjusts `Hidden_Rel`, `Default_Rel`, `Visible_Rel` and `Broken_Treaty` but not `Dipl_Status`, and there is no `Declare_War` call. So a wizard can fight a former ally with the alliance formally intact. The city-capture path does call `Declare_War`. Recorded in-code at [Combat.c:3228](../../MoM/src/Combat.c#L3228).
- **`Break_Treaties` receives the two players in the opposite order to `Change_Relations`.** `asm:404-406` pushes the attacker then the defender, so the call is `Break_Treaties(defender, attacker)` — faithful. Inside that function the first parameter is named `attacker_idx` and is the one stamped as the oath-breaker, which puts the combat *defender* in that slot. Whether the call is wrong or the parameter names are cannot be settled from this overlay; `Break_Treaties` lives in `DIPLOMAC.c` and is out of scope here.
- **The razing cascade runs twice.** `asm:781-800` has two consecutive blocks with identical bodies — `Change_City_Ownership`, `Destroy_City`, `destroy_combat_city = ST_FALSE` — the first gated on `City_Destroyed == ST_TRUE` and the second on `destroy_combat_city == ST_TRUE`. Since the first clears `destroy_combat_city`, the second can only fire when the first did not. Reproduced verbatim at [Combat.c:3407-3413](../../MoM/src/Combat.c#L3407-L3413).
- **`defender_idx` changes meaning mid-function.** It arrives as a unit index for stack-versus-stack and is overwritten with `_UNITS[defender_idx].owner_idx` in case 0 ([Combat.c:3166](../../MoM/src/Combat.c#L3166)), after which every later use treats it as a player index. Case 1 and case 5 never rewrite it, so it must already be a player index on those paths.
