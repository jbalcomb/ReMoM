Combat-Init_Prep_Etc.md

¿ IDK, mish-mash of combat init code and external usage of 'battle unit', and follow-ons ?
What else belongs: the closure is exactly ovr116 — verified by extracting every call target.
Tier 1 is the eight remaining functions there (2884 lines);
    three are load-bearing for open work —
        Apply_Enchantment_And_Mutation_Effects()         (called from both reviewed functions with different masks, needed to write G1)
        Battle_Unit_Item_Stats()      (closes the ordering assumption carried from the Battle_Unit_Attack review)
        Battle_Unit_Hit_Points()
Tier 2 is Calc_Battlefield_Bonuses (ovr122, 450), which produces the three _battlefield_* arrays G1's first five blocks read.
/*
    Combat

    WIZARDS.EXE
        ovr091  CMB_PrepareTurn__WIP
        ovr098  ¿ MoO2  COMBINIT ?
        ovr116  the whole "load a unit into a battle unit" overlay
*/

CMB_PrepareTurn__WIP()               ==>  Begin_Combat_Turn()
CMB_Units_Init__WIP()                ==>  Deploy_Battle_Units()
BU_Init_Battle_Unit()                ==>  Battle_Unit_Regular_Stats()
BU_Apply_Battlefield_Effects__WIP()  ==>  Battle_Unit_Special_Stats()
BU_Apply_Level()                     ==>  Battle_Unit_Level_Stats()
BU_Apply_Specials()                  ==>  Apply_Enchantment_And_Mutation_Effects()
BU_Apply_Item_Powers()               ==>  Battle_Unit_Item_Stats()
BU_Apply_Item_Enchantments()         ==>  Item_Powers_To_Unit_Enchantments()
BU_Apply_Item_Attack_Specials()      ==>  Item_Powers_To_Attack_Attributes()
BU_Init_Hero_Unit()                  ==>  Battle_Unit_Hero_Skill_Stats()
NX_Unit_Has_Spell_As_Enchantment_Or_Item_Power()  ==>  NIU_Unit_Has_Spell_Enchantment()


CMB_WizCastAvailable  ==>  m_wizard_cast_available
CMB_AIGoesFirst       ==>  ???

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\CMB_PrepareTurn__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr091\Add_City_Damage_From_Battle_Units_Within.asm

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr098\CMB_Units_Init__WIP.asm

...all of WIZARDS.EXE ovr116 ~== MoO2 Module: COMBINIT...
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Item_Powers.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Item_Enchantments.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Item_Attack_Specials.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Unit_Hit_Points.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Battle_Unit_Hit_Points.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\Load_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Init_Battle_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Specials.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Level.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Init_Hero_Unit.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\BU_Apply_Battlefield_Effects__WIP.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr116\NX_Unit_Has_Spell_As_Enchantment_Or_Item_Power.asm

~ CMB_Units_Init__WIP()  AKA Prepare_All_Battle_Units()
OON XREF:  Prepare_Battle_Unit_Summons() |-> Prepare_Battle_Unit
...which is only used for scc_Summoning and USA 'Summon Demon'
OON XREF:  Combat_Cast_Spell() |-> Summon_Demon()

Combat_Screen__WIP()
    |-> Combat_Next_Turn()

Combat_Next_Turn()
    |-> CMB_PrepareTurn__WIP()

CMB_PrepareTurn__WIP()
    |-> Apply_Mana_Leak() / Apply_Call_Lightning() / Apply_Wrack()
    |-> Vortex_Combat_Round()
    |-> Calc_Battlefield_Bonuses()
    |-> Add_City_Damage_From_Battle_Units_Within()   [near call, same overlay]
    |-> j_BU_Init_Battle_Unit()
        |-> Battle_Unit_Regular_Stats()
    |-> j_BU_Apply_Battlefield_Effects__WIP()
    |-> j_Battle_Unit_Moves2() / j_Combat_Resistance_Check() / j_Battle_Unit_Heal() / Random()

CMB_Units_Init__WIP()
    |-> j_Load_Battle_Unit()
        |-> Load_Battle_Unit()
            |-> j_Unit_Gold_Upkeep()
            |-> Battle_Unit_Regular_Stats()
    |-> Combat_Figure_Load()
    |-> Deploy_Battle_Units()

Battle_Unit_Regular_Stats()
    |-> Battle_Unit_Level_Stats()
    |-> BU_Init_Hero_Unit()
    |-> Battle_Unit_Item_Stats()
        |-> Item_Powers_To_Attack_Attributes()
        |-> Item_Powers_To_Unit_Enchantments()
    |-> Apply_Enchantment_And_Mutation_Effects()
    |-> j_Unit_Moves2()
    |-> Unit_Hit_Points()

Battle_Unit_Special_Stats()
    |-> Apply_Enchantment_And_Mutation_Effects()
    |-> Battle_Unit_Hit_Points()

Battle_Unit_Heal
    |-> Battle_Unit_Regular_Stats
    |-> Battle_Unit_Special_Stats


// WZD o98p01
void Update_Combat_Enchantments_Icon_And_Help(void);

// WZD o98p02
int16_t BU_HasSpellAbility__WIP(int16_t battle_unit_idx);

// WZD o98p03
// NIU  int16_t NIU_Who_Has_More_Leadership(void);

// WZD o98p04
void Auto_Cast_Spell_And_Do_Combat_Turn(int16_t player_idx);

// WZD o98p05
void Combat_Next_Turn(void);

// WZD o98p06
void Retreat_From_Combat(int16_t player_idx);

// WZD o98p07
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p08
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p09
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count);

// WZD o98p10
void Assign_Mouse_Images(void);

// WZD o98p11
int16_t Ranged_Mouse_Image(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o98p12
void Add_Combat_Enchantment_Fields(void);

// WZD o98p13
int16_t Battle_Unit_Movement_Mode(int16_t battle_unit_idx);

// WZD o98p14
// static void Prepare_Battle_Unit_Summons(int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy);

// WZD o98p15
void Prepare_Battle_Unit(int16_t battle_unit_idx, int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy);

// WZD o98p16
int16_t CMB_Units_Init__WIP(int16_t troop_count, int16_t troops[]);

// WZD o98p17
int16_t Battle_Unit_Is_Airborne(int16_t battle_unit_idx);

// WZD o98p18
int16_t Battle_Unit_Has_Flight(int16_t battle_unit_idx);

// WZD o98p19
int16_t Check_Attack_Melee(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx);

// WZD o98p20
SAMB_ptr Get_Battle_Unit_Move_Sound_Buffer(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_buffer_size);

// WZD o98p21
void Turn_Off_Auto_Combat(void);

// WZD o98p22
void STK_ComposeFleeLost__STUB(int16_t troop_count, int16_t troop_list[]);


Load_Battle_Unit() calls Battle_Unit_Regular_Stats(), but not Battle_Unit_Special_Stats().
...because there are no combat/battlefield effects in-play yet?



// WZD o98p16
// MoO2  Module: COMBINIT  Deploy_Ships_()
int16_t CMB_Units_Init__WIP(int16_t troop_count, int16_t troops[])

    if(_combat_defender_player == ST_UNDEFINED)
    {
        _combat_defender_player = MOO_MONSTER_PLAYER_IDX;
    }
    if(_combat_attacker_player == ST_UNDEFINED)
    {
        _combat_attacker_player = MOO_MONSTER_PLAYER_IDX;
    }
    if(_combat_defender_player != _human_player_idx)
    {
        CMB_AI_Player = _combat_defender_player;
    }
    if(CMB_AI_Player == ST_UNDEFINED)
    {
        CMB_AI_Player = MOO_MONSTER_PLAYER_IDX;
    }
    /*
        IDGI.
            attacker / defender
            current / opponent
    */
    if(_combat_attacker_player == _human_player_idx)
    {
        combat_human_player = _combat_attacker_player;
        combat_computer_player = _combat_defender_player;
    }
    else
    {
        combat_human_player = _combat_defender_player;
        combat_computer_player = _combat_attacker_player;
    }

It's the tail of CMB_Units_Init__WIP that converts "who attacked whom" into the three globals the rest of combat actually reads. Three separate jobs.

The constants matter: HUMAN_PLAYER_IDX 0, NEUTRAL_PLAYER_IDX 5, MOO_MONSTER_PLAYER_IDX 6 (MOX_DEF.h:611-613).

1. Sentinel normalisation (5571-5578). _combat_attacker_player and _combat_defender_player are set by the caller from Combat_Screen__WIP's parameters (1536-1537). If either arrived as ST_UNDEFINED, this pins it to the monster pseudo-player so every downstream _players[...] index has some value. It's a floor, not a computation.

2. Naming the opponent for the UI (5579-5586). CMB_AI_Player is "the side that isn't the human". It's set in two places — 5540 for the attacker, 5581 for the defender — and because the defender assignment comes second it wins when both sides are AI. Nothing about combat resolution reads it; it drives presentation only: which wizard's combat theme plays (1561, via _players[CMB_AI_Player].wizard_id), the banner colours (5991, 6032, 6119) and the opponent's name on the combat bar (6004, 6045, 7144).

3. Re-labelling the two sides by seat (5587-5602) — and this is what the IDGI is about. Attacker/defender is the wrong axis for the turn loop, which cares about which seat the interactive UI drives. So the pair gets flipped into combat_human_player / combat_computer_player, and those are what the loop uses: whose turn auto-plays (4182, 4189), whether the active unit accepts input (2509, 5066, 6176, 7905), win attribution (2032), retreat checks (8542).

The names are the confusing part. The mapping is unconditional — if the attacker isn't the human, the defender gets labelled "human" whether or not a human is anywhere in the fight. In AI-vs-AI combat combat_human_player names an AI wizard. That's deliberate, and 4195 proves it: under _auto_combat_flag the game calls Auto_Cast_Spell_And_Do_Combat_Turn(combat_human_player) — machine-playing the "human" seat. So read them as seat A / seat B, named after the common case, not as a "is a person at the keyboard" flag. _human_player_idx (the variable at MOM_DAT.c:2957, always HUMAN_PLAYER_IDX) is the one that really means "the human".

One thing that falls out of reading these together — the two sentinels don't agree. The fallbacks write 6, but every reader guards with 5: 1559 CMB_AI_Player != NEUTRAL_PLAYER_IDX, and likewise 5960, 6001, 6042, and < NEUTRAL_PLAYER_IDX at 7141. A CMB_AI_Player of 6 sails through every one of those and then indexes _players[6] — same one-past-the-end read as the Cmbt_Skill_Left write we just looked at, but on wizard_id, banner_id and name.

I have not checked whether the NEUTRAL_PLAYER_IDX guards are themselves faithful — those readers are in ovr090/ovr092 territory, outside this review. So I'm reporting the mismatch as it stands in production, not asserting it's an OGBUG.

---


# 1:1 Fidelity Review

**Status: DONE-DONE — no findings open; one divergence. Fifteen functions compared against 5,870 asm lines (2026-08-05).**

Thirteen reconstruction errors were raised (R1-R13) and all thirteen are closed.

Four inverted a game rule outright. Holy Arms granted nothing to the normal units it exists to bless, because a `||` had been reconstructed as four nested `&&` with two tests flipped. The node aura buffed the wrong magic realm — traced not to the comparison code but to `e_COMBAT_NODE_TYPE` copying IDA's swapped `cnt_Chaos`/`cnt_Nature` labels. Flame Blade never set `Weapon_Plus1`, so it could not hurt weapon-immune targets. And Berserk lost its `melee > 0` guard while overwriting `Gold_Melee` instead of adding to it, discarding every earlier melee credit.

Three were memory or determinism defects rather than arithmetic. `Battle_Unit_Hit_Points` indexed `_unit_type_table` with a *unit* index instead of a unit *type*, reading far past the end of the table on almost every unit. Four phantom `Random()` guards with no asm counterpart injected RNG into a stat calculation that runs once per unit per turn, making hit points vary between turns and desynchronising the seeded sequence. And the same function read Charm of Life from `controller_idx` where the original reads `owner_idx` — a difference under Possession and Creature Binding.

`Battle_Unit_Special_Stats` was ~92% unwritten at the first pass. Its thirty effect blocks have since been written and checked against all 1137 asm lines.

## Scope

Since this review began, `ovr116` has been split out of `Combat.c` into its own translation unit, `COMBINIT.c`, and eleven functions were renamed. The Production column is the **current** name and location; the ASM column keeps the IDA names, which never change.

| Function (production) | Production | ASM (ground truth) | Result |
| --- | --- | --- | --- |
| `Begin_Combat_Turn` | [Combat.c:2635](../../MoM/src/Combat.c#L2635) | [ovr091/CMB_PrepareTurn__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr091/CMB_PrepareTurn__WIP.asm) (555) | faithful (R4 fixed) |
| `Add_City_Damage_From_Battle_Units_Within` | [Combat.c:3428](../../MoM/src/Combat.c#L3428) | [ovr091/Add_City_Damage_From_Battle_Units_Within.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr091/Add_City_Damage_From_Battle_Units_Within.asm) (46) | faithful |
| `CMB_Units_Init__WIP` | [Combat.c:5436](../../MoM/src/Combat.c#L5436) | [ovr098/CMB_Units_Init__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr098/CMB_Units_Init__WIP.asm) (527) | faithful (R3 fixed) |
| `Battle_Unit_Item_Stats` | [COMBINIT.c:62](../../MoM/src/COMBINIT.c#L62) | [ovr116/BU_Apply_Item_Powers.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Powers.asm) (467) | faithful |
| `Item_Powers_To_Unit_Enchantments` | [COMBINIT.c:161](../../MoM/src/COMBINIT.c#L161) | [ovr116/BU_Apply_Item_Enchantments.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Enchantments.asm) (469) | faithful |
| `Item_Powers_To_Attack_Attributes` | [COMBINIT.c:250](../../MoM/src/COMBINIT.c#L250) | [ovr116/BU_Apply_Item_Attack_Specials.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Item_Attack_Specials.asm) (194) | faithful |
| `Unit_Hit_Points` | [COMBINIT.c:292](../../MoM/src/COMBINIT.c#L292) | [ovr116/Unit_Hit_Points.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Unit_Hit_Points.asm) (216) | faithful (R6, R8 fixed) |
| `Battle_Unit_Hit_Points` | [COMBINIT.c:377](../../MoM/src/COMBINIT.c#L377) | [ovr116/Battle_Unit_Hit_Points.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Battle_Unit_Hit_Points.asm) (261) | faithful (R5, R7, R9 fixed) |
| `Load_Battle_Unit` | [COMBINIT.c:471](../../MoM/src/COMBINIT.c#L471) | [ovr116/Load_Battle_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/Load_Battle_Unit.asm) (286) | faithful |
| `Battle_Unit_Regular_Stats` | [COMBINIT.c:566](../../MoM/src/COMBINIT.c#L566) | [ovr116/BU_Init_Battle_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Init_Battle_Unit.asm) (478) | faithful (R1, R2 fixed) |
| `Apply_Enchantment_And_Mutation_Effects` | [COMBINIT.c:734](../../MoM/src/COMBINIT.c#L734) | [ovr116/BU_Apply_Specials.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Specials.asm) (509) | faithful (R11-R13 fixed) |
| `Battle_Unit_Level_Stats` | [COMBINIT.c:922](../../MoM/src/COMBINIT.c#L922) | [ovr116/BU_Apply_Level.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Level.asm) (275) | faithful |
| `Battle_Unit_Hero_Skill_Stats` | [COMBINIT.c:1102](../../MoM/src/COMBINIT.c#L1102) | [ovr116/BU_Init_Hero_Unit.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Init_Hero_Unit.asm) (394) | faithful (R6 fixed) |
| `Battle_Unit_Special_Stats` | [COMBINIT.c:1189](../../MoM/src/COMBINIT.c#L1189) | [ovr116/BU_Apply_Battlefield_Effects__WIP.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/BU_Apply_Battlefield_Effects__WIP.asm) (1137) | faithful |
| `NIU_Unit_Has_Spell_Enchantment` | [COMBINIT.c:1493](../../MoM/src/COMBINIT.c#L1493) | [ovr116/NX_Unit_Has_Spell_As_Enchantment_Or_Item_Power.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr116/NX_Unit_Has_Spell_As_Enchantment_Or_Item_Power.asm) (56) | faithful |

`Add_City_Damage_From_Battle_Units_Within` was pulled in because `Begin_Combat_Turn` reaches it by a *near* call (asm:53-55) — same overlay, so it is part of the same translation unit and cost nothing to cover.

**This is the whole of `ovr116` plus its two `Combat.c` entry points.** The overlay's twelve functions are the exact transitive closure of `Load_Battle_Unit` and `Battle_Unit_Special_Stats`, verified by extracting every `call` target from every listing in the overlay; nothing in it calls outward except `j_Unit_Gold_Upkeep` and `j_Unit_Moves2`.

**Rename ledger** — IDA name on the left, production name on the right:

```
CMB_PrepareTurn__WIP                            ==>  Begin_Combat_Turn()
BU_Init_Battle_Unit                             ==>  Battle_Unit_Regular_Stats()
BU_Apply_Battlefield_Effects__WIP               ==>  Battle_Unit_Special_Stats()
BU_Apply_Specials                               ==>  Apply_Enchantment_And_Mutation_Effects()
BU_Apply_Level                                  ==>  Battle_Unit_Level_Stats()
BU_Init_Hero_Unit                               ==>  Battle_Unit_Hero_Skill_Stats()
BU_Apply_Item_Powers                            ==>  Battle_Unit_Item_Stats()
BU_Apply_Item_Enchantments                      ==>  Item_Powers_To_Unit_Enchantments()
BU_Apply_Item_Attack_Specials                   ==>  Item_Powers_To_Attack_Attributes()
NX_Unit_Has_Spell_As_Enchantment_Or_Item_Power  ==>  NIU_Unit_Has_Spell_Enchantment()

CMB_WizCastAvailable                            ==>  m_wizard_cast_available
s_UNIT_TYPE.Move_Halves                         ==>  s_UNIT_TYPE.moves2_base
```

Out of scope, and the `faithful` verdicts do not extend to them: `Combat_Figure_Load` (`ovr163`, 142), `Deploy_Battle_Units` (`ovr113`, 375), `Calc_Battlefield_Bonuses` (`ovr122`, 450), `Unit_Moves2` (`ovr121`, 244), `Battle_Unit_Moves2` (`ovr124`, 300), `Unit_Gold_Upkeep` (`ovr120`, 140).
`asm:N` refers to the listing named in the row above.

**Evidence standard.** A divergence is recorded only where the bytes show a different computation, a different order, or a different operand width. Stack-slot and register counts are not used to infer how many variables the source declared. Branch layout alone is not a finding — but a `jz`/`jnz` whose target changes *which* arm executes is, because that is the computation.

**Enum values come from `WIZARDS.inc`**, not from the listings — the `ovr*/` files print only symbol names. Values used below: `bus_Active = 0` (5524), `bus_Uninvolved = 3` (5527), `bus_Gone = 6` (5530), `us_Ready = 0` (5532), `bua_Ready = 0` (5564), `bua_Finished = 4` (5565), `UE_REGENERATION = 40h` (5679), `UE_FLIGHT = 2` (5691), `UE_HOLYWEAPON = 80h` (5697), `UM_UNDEAD = 20h` (5714), `Att_Poison = 4` (5720), `UA_FANTASTIC = 1` (5735), `USA_LUCKY = 400h` (5771), `USA_CASTER_20 = 2000h` (5774), `USA_CASTER_40 = 4000h` (5775), `cnt_Sorcery = 0` (5932), `cnt_Chaos = 1` (5933), `cnt_Nature = 2` (5934), `MV_SAILING = 2` (5942), `MV_FLYING = 8` (5944), `rt_Arcane = 0Fh` (6871), `rt_Nature = 10h` (6872), `rt_Sorcery = 11h` (6873), `rt_Chaos = 12h` (6874), `rt_Life = 13h` (6875), `rt_Death = 14h` (6876), `srat_Thrown = 100` (6905), `rag_Boulder = 1` (6917), `rag_Missile = 2` (6918), `e_MOO_MONSTER_PLAYER_IDX = 6` (9192).

`UE_*` values in `WIZARDS.inc` are **high-word** masks — the listings pair them with `and dx, 0`. So `UE_HOLYWEAPON = 80h` is `0x00800000`, matching [MOM_DEF.h:919](../../MoX/src/MOM_DEF.h#L919), and `UE_FLIGHT = 2` is `0x00020000`. `UE_REGENERATION = 40h` is the exception in this bundle: `Begin_Combat_Turn` asm:491 pairs it with `and ax, 0`, so it is a **low**-word mask, `0x00000040`, matching [MOM_DEF.h:902](../../MoX/src/MOM_DEF.h#L902).

---

# Findings

None open. The thirteen raised during this review are all fixed; the roster below exists only so the `R*` citations in the sections that follow resolve to something. Each entry is one line — the defect, not its history.

| # | Function | Defect |
| --- | --- | --- |
| R1 | [`Battle_Unit_Regular_Stats`](../../MoM/src/COMBINIT.c#L566) | Holy Arms' `\|\|` was reconstructed as nested `&&` with two tests inverted, so the normal units it exists to bless got nothing |
| R2 | [`Battle_Unit_Regular_Stats`](../../MoM/src/COMBINIT.c#L566) | Chaos Surge's loop bound was the constant `NUM_PLAYERS`, not the live `_num_players` |
| R3 | [`CMB_Units_Init__WIP`](../../MoM/src/Combat.c#L5436) | the defender-side `us_Ready` reset was inverted and read `HUMAN_PLAYER_IDX` instead of `_human_player_idx` |
| R4 | [`Begin_Combat_Turn`](../../MoM/src/Combat.c#L2635) | the Web block was hoisted out of its guard and dropped `action = bua_Finished` |
| R5 | [`Battle_Unit_Hit_Points`](../../MoM/src/COMBINIT.c#L377) | `_unit_type_table` was indexed by *unit* index instead of unit *type* — an out-of-bounds read on almost every unit |
| R6 | [`Unit_Hit_Points`](../../MoM/src/COMBINIT.c#L292), [`Battle_Unit_Hero_Skill_Stats`](../../MoM/src/COMBINIT.c#L1102) | four phantom `Random()` guards with no asm counterpart, injecting RNG into a stat calculation that runs once per unit per turn |
| R7 | [`Battle_Unit_Hit_Points`](../../MoM/src/COMBINIT.c#L377) | Charm of Life read `controller_idx` where the original reads `_UNITS[unit_idx].owner_idx` |
| R8 | [`Unit_Hit_Points`](../../MoM/src/COMBINIT.c#L292) | `unit_level` was `int8_t`; the asm frame slot is `word ptr` |
| R9 | [`Battle_Unit_Hit_Points`](../../MoM/src/COMBINIT.c#L377) | the hero test was `Hero_Slot != ST_UNDEFINED` where the original is `>` |
| R10 | [`Combat.h:1255`](../../MoM/src/Combat.h#L1255) | `mana_max` was `int8_t`; its guard is an unsigned byte compare (`cmp` + `jbe`) |
| R11 | [`Apply_Enchantment_And_Mutation_Effects`](../../MoM/src/COMBINIT.c#L734) | Black Channels tested `ranged != ST_UNDEFINED` where the original tests `ranged_type != rat_UNDEF` |
| R12 | [`Apply_Enchantment_And_Mutation_Effects`](../../MoM/src/COMBINIT.c#L734) | Berserk's melee doubling lost its `melee > 0` guard |
| R13 | [`Apply_Enchantment_And_Mutation_Effects`](../../MoM/src/COMBINIT.c#L734) | Berserk wrote `Gold_Melee =` where the original adds, discarding every earlier melee credit |

`D1` — the four `MOO_MONSTER_PLAYER_IDX` fallbacks in [`CMB_Units_Init__WIP`](../../MoM/src/Combat.c#L5436) — was closed the same way, by restoring them.

# Divergences

## D2 — `combat_enchantments` is typed as the flat view only

The original uses **both** views of the same 30 bytes, and production is wired to only one of them.

`s_COMBAT_ENCHANTMENTS` is `sizeof = 0x1E`: fifteen members, each an `s_COMBAT_ENCHANTMENT_STATUS` of `{ Attkr db, Dfndr db }`. The effect code addresses it by member — `combat_enchantments[es:bx + s_COMBAT_ENCHANTMENTS.Terror.Attkr]`. But three other sites index the same block **flat, with a computed offset**:

| listing | index |
| --- | --- |
| `ovr111/AITP_Combat_Spell.asm:1443-1447` | `spell_data_table[spell_idx].Param0 + Unit_Resist` |
| `ovr112/Do_Legal_Spell_Check__WIP.asm:542-546` | same |
| `ovr131/Cast_Disenchant.asm:78-82`, `183-187` | `itr + Enchant_Offset`, a loop |

The first two matter most: **`s_SPELL_DATA.Param0` holds the flat slot number**, so the 0..29 numbering is baked into the spell table data, not an artifact of the reconstruction. Any named-members-only representation would break those call sites.

Production declares `combat_enchantments` as `int8_t *` ([MOM_DAT.h:2952](../../MoX/src/MOM_DAT.h#L2952), [MOM_DAT.c:1195](../../MoX/src/MOM_DAT.c#L1195)) and reaches it through the flat indices at [Combat.h:52-81](../../MoM/src/Combat.h#L52-L81), whose order matches the struct's member order exactly. So the *flat* half is faithful and load-bearing; what is missing is a typed path to the named half. `struct s_COMBAT_ENCHANTMENTS` is already declared at [Combat.h:959](../../MoM/src/Combat.h#L959) — it simply is not what the pointer is typed as.

No behavioural effect. The fix, when someone wants it, is a second pointer of the struct type aliasing the same block (the commented-out `(struct s_COMBAT_ENCHANTMENTS *)` casts at the two allocation sites, [Combat.c:8151](../../MoM/src/Combat.c#L8151) and [Combat.c:9466](../../MoM/src/Combat.c#L9466), show the original intent) — migrating effect-code call sites to `->Terror.Attkr` while the spell/AI sites keep the flat view. Both views are permanent; this is not an either/or.

---

# Begin_Combat_Turn

## Frame map

`sub sp, 4` (asm:8) = 4 bytes, plus one register local. No parameters.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-4` | `saved_active_battle_unit` | `saved_active_battle_unit` |
| `bp-2` | `resist_fails` | `resist_fails` |
| `si` | `_SI_itr` | `itr` |

## Faithful — verified, leave alone

- **The prologue order is exact** (asm:10-15): `_human_handle_immobile = ST_TRUE`, `_ai_immobile_counter++`, `_combat_turn++`, `CMB_WizCastAvailable = ST_TRUE`, `_scanned_battle_unit = ST_UNDEFINED`, then the flag IDA calls `CMB_AIGoesFirst` set to `ST_FALSE`. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635); the last is `m_cp_took_turn`, documented at [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635).
- **`_active_battle_unit` is saved and restored around `Vortex_Combat_Round`** (asm:45-49), because the vortex walk reassigns it. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635).
- **`Suppression = 0` happens before the `status` filter** (asm:64 then asm:70), so it clears on every slot including inactive ones. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) keeps the order.
- **`action = bus_Active`** at asm:111 — the *status* enum used for the *action* field. Both are `0`, so this is a source-level oddity with no effect, and IDA shows the `bus_` symbol. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) reproduces it.
- **Terror and Entangle share one side-selection shape** (asm:112-142, asm:188-213): attacker-side enchantment hits units controlled by the *defender*, and vice versa. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) and [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635).
- **`Combat_Resistance_Check` takes the battle unit by value.** asm:161-172 sets `cx = size s_BATTLE_UNIT` and calls `SPUSH@` to block-copy the struct onto the stack, then unwinds with `add sp, 72h`. The other two arguments are `_battlefield_resistall[controller_idx] + 1` and `spell_data_table[spl_Terror].magic_realm`, pushed in that order. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635).
- **Entangle's floor test is outside its own decrement guard.** asm:221 `jle loc_77447` and the fallthrough both land on the `movement_points < 1` test at asm:237-250, so the clamp runs whether or not the `-2` was applied. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) matches. (This is the shape R4 gets wrong two blocks later.)
- **Regeneration checks four independent sources** (asm:489-528) — `battle_unit->enchantments`, `battle_unit->item_enchantments`, `_UNITS[].enchantments`, then `Abilities & UA_REGENERATION`. Production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635). `Battle_Unit_Heal(itr, 1, 0)` from the push order at asm:537-542, production [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635).
- **Confusion result 3 swaps the controller** (asm:452-482), reading `[byte ptr _combat_defender_player]` — the low byte of a word global. Production's `(int8_t)` casts at [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) and [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) are right.

## OGBUGs — faithful, do not fix

- **The Regeneration `status == bus_Active` test can never fail** (asm:535). The loop already `continue`d on any non-active unit at asm:70. Annotated at [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635).
- **Confusion resets `Confusion_State` without returning a swapped unit to its original owner** (asm:411, and the swap at asm:482). Once result 3 has flipped `controller_idx`, nothing flips it back — the unit stays with the enemy for the rest of the battle. The `¿ NOBUG ?` at [Begin_Combat_Turn](../../MoM/src/Combat.c#L2635) reasons that `Battle_Unit_Regular_Stats` re-initialises the unit each turn, but it does not: `Battle_Unit_Regular_Stats` never writes `controller_idx` (its 478 lines contain no such store), and `Load_Battle_Unit` — which does, at asm:86 — is not called from this path. The `; BUG` above it is correct and the `¿ NOBUG ?` beneath it is not.

# Add_City_Damage_From_Battle_Units_Within

Faithful.

Early-returns unless `_combat_environ == cnv_Enemy_City` (asm:7, `cnv_Enemy_City = 1` at [Combat.h:653](../../MoM/src/Combat.h#L653)), then increments `_combat_city_damage` once per battle unit that is `bus_Active`, controlled by `_combat_attacker_player`, and inside the city walls (asm:20-36). Production [Add_City_Damage_From_Battle_Units_Within](../../MoM/src/Combat.c#L3428).

`void` is the right return type. asm:9 is `xor ax, ax` on the early-exit path, but nothing else in the 46 lines writes `AX` for return purposes — asm:32's `call j_Battle_Unit_Is_Within_City` loads it and asm:34 consumes it immediately in a `cmp` — and the one caller, `Begin_Combat_Turn` asm:53-55, discards it. Compiler artifact.

# CMB_Units_Init__WIP

Faithful apart from R3 and D1.

## Faithful — verified, leave alone

- **The clear loop runs to a literal 36** (asm:28), setting `status = bus_Gone` and `bufpi = ST_UNDEFINED` — not to `_combat_total_unit_count`, which is only zeroed afterwards at asm:30. Production [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436); `MAX_BATTLE_UNIT_SLOT_COUNT` is 36 ([MOM_DEF.h:67](../../MoX/src/MOM_DEF.h#L67)).
- **`Combat_Figure_Load`'s second argument differs between the two loops** — `itr` for the attacker (asm:71) but `_combat_total_unit_count` for the defender (asm:247). They coincide only while the attacker loop is running. Production [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) and [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) keep the asymmetry.
- **The Floating Island flags are set with different literals** — `mov [ATKR_FloatingIsland], 1` (asm:63) but `mov [DEFR_FloatingIsland], e_ST_TRUE` (asm:239). Production [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) and [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) preserve the `1` / `ST_TRUE` split rather than normalising it.
- **`controller_idx` is overwritten after `Load_Battle_Unit` already set it.** `Load_Battle_Unit` asm:84-86 stores `_UNITS[unit_idx].owner_idx`; this function then stores the side index over it (asm:100-101, asm:272-273). Redundant for most units, load-bearing for any whose owner differs from the side they are fighting on. Production [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) and [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436), the latter with the question already noted in-source.
- **The reset loop's field order is exact** (asm:322-406): `cgx`, `cgy`, `target_cgx`, `target_cgy`, `move_anim_ctr`, `outline_magic_realm`, `Atk_FigLoss`, `Moving`, `action`, `gibs`, `Unknown_5A`, `Always_Animate`, `Melee_Anim`, `Image_Effect`, `Move_Bob`. Production [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) matches, with `Always_Animate` → `animate_idle`, `Image_Effect` → `figure_effect`, `Move_Bob` → `animate_move_as_idle`.
- **`count` is incremented only in the defender loop** (asm:274) and is the return value (asm:521). Production [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436), [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436).
- **`_combat_wx` appears as `_combat_wx__som_started_anim_ctr`** in the listing (asm:159) — one storage location IDA has labelled for two uses. Production's `_combat_wx` at [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) is right.
- **`spell_data_table[spl_Floating_Island].unit_type` is `Params0_1` in the listings** (asm:61, asm:237), IDA's generic name for the word at offset `0x20`. Production names it `unit_type` ([MOM_DAT.h:1071](../../MoX/src/MOM_DAT.h#L1071)).

## OGBUGs — faithful, do not fix

- **Transports with Wraith Form are not excluded from city defence.** The city-siege filter (asm:407-445) requires `Transport > 0` **and** `MV_SAILING` **and** not `MV_FLYING`; a Wraith Form transport carries `MV_SWIMMING` rather than `MV_SAILING`, so it slips through and defends the city as a combat unit. Annotated at [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436) and [CMB_Units_Init__WIP](../../MoM/src/Combat.c#L5436).

# Load_Battle_Unit

Faithful.

A `memcpy` of the whole `s_UNIT_TYPE` row followed by ~40 individual field stores, each recomputing the far pointer from `[bp+battle_unit]`. Production [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) reproduces the sequence in listing order.

- **`memcpy` copies `sizeof(s_UNIT_TYPE)` from `&_unit_type_table[type].Melee`** — the source offset is the `Melee` member, not the row base (asm:26). Production [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471).
- **The duplicated `Unknown_5A = 0` is real** — asm:191 and asm:193 are two separate stores to the same field. Production [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) keeps both, with the note.
- **Poison strength is captured before the special-attack field is cleared** (asm:116-126), and the clear is skipped for `srat_MultiGaze`. Production [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471).
- **`front_figure_damage` and `figure_cnt` come from two separate `idiv`s of the same operands** — asm:147-163 takes the remainder in `DL`, asm:164-183 takes the quotient in `AL` and subtracts it from `Max_Figures`. The original recomputes rather than reusing. Production [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) writes it as `%` and `/`, which is the same two divisions.
- **`status = 0` is `bus_Active`** (asm:78) and **`action = 0` is `bua_Ready`** (asm:203); production's [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) and [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) name them correctly.
- **`target_battle_unit_idx` is set to `e_ST_UNDEFINED_DB`** — `0FFh`, the byte-width undefined marker (asm:107), not the word `0FFFFh` used for `bufpi` at asm:76. Production [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) and [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471).
- **`Item_Charges` is stored as the low byte of a word local** (asm:278). Production's `(int8_t)` cast at [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471) is right.

## OGBUGs — faithful, do not fix

- **Only the hero's first item is read for charges.** asm:240 and asm:263 both index `_players.Heroes.Items` with no per-item offset, so slot `[0]` is the only one consulted; a hero whose charged item sits in slot 1 or 2 enters combat with zero charges. Annotated at [Load_Battle_Unit](../../MoM/src/COMBINIT.c#L471). Contrast the Haste scan in `Battle_Unit_Special_Stats` (asm:260-311), which *does* loop over all `NUM_HERO_ITEMS` — the original is genuinely inconsistent between the two functions.

# Battle_Unit_Regular_Stats

Faithful apart from R1 and R2.

## Frame map

`sub sp, 0Ah` (asm:13) = 10 bytes, plus two register locals. One parameter at `bp+6`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-0Ah` | `weapon_quality` | `weapon_quality` |
| `bp-8` | `var_8` | `var_8` |
| `bp-7` | `unit_mutations` | `unit_mutations` |
| `bp-6` | `battle_unit_enchantments` (dword) | `battle_unit_enchantments` |
| `bp-2` | `chaos_surge` | `chaos_surge` |
| `bp+6` | `battle_unit` (dword) | `battle_unit` |
| `si` | `_SI_unit_idx` | `unit_idx` |
| `di` | `_DI_itr_players` | `itr_players` |

`var_8` occupies a byte of the frame and is never referenced in the 478 lines.

## Faithful — verified, leave alone

- **`unit_idx` is read from the battle unit, not passed in** (asm:16-17). This function's only parameter is the pointer; the unit index comes from `battle_unit->unit_idx`. Production [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566).
- **Base stats are re-read from `_unit_type_table` and the accumulators re-zeroed** (asm:18-115), which is what makes the function safe to call repeatedly — `Begin_Combat_Turn` runs it once per unit per turn.
- **`mana_max` is set by two independent tests, not an if/else** (asm:116-126). A unit with both `USA_CASTER_40` and `USA_CASTER_20` ends at 20, because the second test overwrites. Production's two separate `if`s at [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) reproduce that; an `else if` would not.
- **`weapon_quality` is a two-bit field** — `and ax, 3` (asm:207). The in-source comment at [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) says "mask first 3 bits"; the mask *value* is 3, the *width* is 2. Comment only, code is right.
- **`Weapon_Plus1 = weapon_quality + 1` sits inside the `weapon_quality > 0` block** (asm:284-287), alongside the unconditional defense bonus at asm:271-283. Production [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) is under-indented but structurally correct.
- **The ranged-weapon-quality test lists `rag_Missile`, `rag_Boulder`, `srat_Thrown`** (asm:233-252) while the Holy Weapon test lists the same three in a different order — `srat_Thrown`, `rag_Boulder`, `rag_Missile` (asm:374-392). Production [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) and [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) keep both orders as written.
- **`battle_unit_enchantments` is computed twice** — once before the weapon-quality block (asm:188-199) and again after `Apply_Enchantment_And_Mutation_Effects` (asm:443-454) — with the identical expression. The second recompute matters because `Apply_Enchantment_And_Mutation_Effects` can change `item_enchantments`. Production [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) and [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566).
- **`UA_FANTASTIC || race > rt_Troll` is a genuine `||`** (asm:407-411): `jnz` on the ability jumps *into* the apply block, `jle` on the race jumps past it. Production [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566). Worth contrasting with R1 — same instruction pair, opposite structure, and production got this one right.
- **`Unit_Moves2` and `Unit_Hit_Points` results are stored as bytes** from `AL` (asm:442, asm:472). Production's `(int8_t)` casts at [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566) and [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566).
- **`unit_mutations` is pushed as a word with only `AL` loaded** (asm:428-429), so the high byte is whatever was in `AH`. The callee reads a byte parameter, so it is harmless — but it is why production's parameter is `uint8_t` at [Battle_Unit_Regular_Stats](../../MoM/src/COMBINIT.c#L566).

## OGBUGs — faithful, do not fix

- **The Chaos Surge loop reads one fixed player, every iteration.** asm:290-302 builds a real loop — `xor DI, DI`, `inc DI`, `cmp DI, [_num_players]`, `jl` — and never uses `DI` for anything. The test inside it, asm:293, is `cmp [_players.Globals.CHAOS_SURGE], 0` with **no index register**, where every other `_players` access in these listings computes `owner * size s_WIZARD` into `BX` and addresses `[_players.Globals.X + bx]`. So it reads `_players[0]` at a fixed address. `HUMAN_PLAYER_IDX` is `0`, which is why production's `_players[HUMAN_PLAYER_IDX]` at [COMBINIT.c:652](../../MoM/src/COMBINIT.c#L652) is the correct reconstruction rather than a guess, and drake178's note above it is verifiable rather than merely plausible.

  The intent is obvious from the loop that surrounds it — "does *any* wizard have Chaos Surge up?" — and the `+bx` simply never got written. Two consequences, both faithful: an AI wizard's Chaos Surge does nothing at all, and the human's buffs **every** `rt_Chaos` unit on the field, both sides, because the effect block at [657-672](../../MoM/src/COMBINIT.c#L657-L672) applies to `battle_unit` with no owner check.

  This is also why fixing the loop bound (R2, `NUM_PLAYERS` → `_num_players`, asm:300) changed nothing observable: the body ignores the induction variable.

- **The Flight movement floor can never fire.** asm:456-465 forces `movement_points = 6` for a flying unit below 3, after `Unit_Moves2` has already applied every modifier. drake178's notes are right that the check is redundant — [NEXTTURN.c:1566-1572](../../MoM/src/NEXTTURN.c#L1566-L1572) already floors Flight at 6 — and that the threshold is in whole moves while the field holds halves.

  Beyond that, it appears to be dead code. `Unit_Moves2` floors a flying unit at 6 halves *before* Wind Mastery; everything between only adds; and the sole reducer is `moves2 / 2`, so the minimum a flying unit can reach is `6 / 2 = 3` — and `3 < 3` is false. The scenario drake178 says could trigger it lands exactly one short. Confirmed by `Item_Powers_To_Unit_Enchantments`: `ip_Flight → UE_FLIGHT` is in the table, so `item_enchantments` and the mask `Unit_Moves2` builds from `_ITEMS[].Powers` agree on Flight and both see it.

  Were it reachable, the damage would be a display discrepancy only — `movement_points` is a `s_BATTLE_UNIT` field that overland movement never reads, while the unit panels render `movement_points / 2` ([UnitList.c:454](../../MoM/src/UnitList.c#L454), [UnitStat.c:1154](../../MoM/src/UnitStat.c#L1154)).

# Battle_Unit_Special_Stats

Faithful — all 1137 asm lines have production behind them ([Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189)).

Thirty effect blocks, applied in a fixed order to one battle unit. Order is load-bearing throughout: every block reads stats the earlier ones have already modified, and the three clamps at the end are the only thing keeping a stacked debuff from going negative.

## Faithful — verified, leave alone

- **The node aura pairs value 1 with `rt_Nature` and value 2 with `rt_Chaos` — and the IDA labels say the opposite.** `WIZARDS.inc` names value 1 `cnt_Chaos` (5933) and value 2 `cnt_Nature` (5934), so asm:27-30 reads as "`cnt_Nature` ↔ `rt_Chaos`" and asm:35-38 as "`cnt_Chaos` ↔ `rt_Nature`". Those labels are wrong. The real node enum is `nt_ { Sorcery = 0, Nature = 1, Chaos = 2 }` ([MOM_DAT.h:821-826](../../MoX/src/MOM_DAT.h#L821-L826)), and `_combat_node_type` is assigned straight from `_NODES[].type` at [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189), so the bytes are correct game logic. [ovr103/Combat_Info_Effects_Base.asm](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr103/Combat_Info_Effects_Base.asm) confirms it from an unrelated function: value 0 → `HLP_SORCERY_NODE_AURA` (asm:95), value 1 → `HLP_NATURE_NODE_AURA` (asm:118), value 2 → `HLP_CHAOS_NODE_AURA` (asm:130). Production's `e_COMBAT_NODE_TYPE` ([MOM_DAT.h:834-836](../../MoX/src/MOM_DAT.h#L834-L836)) is defined to match the numbers, not the labels, which is what makes [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) come out right. **Do not "correct" the enum to agree with the IDA symbol names.** Also ignore [ovr120/Players_Update_Magic_Power.c](../../../STU-Extras/Piethawn/Piethawn/out/WIZARDS/ovr120/Players_Update_Magic_Power.c), which annotates a third numbering (`== 3 /* cnt_Nature */`) — it is a translated `.c`, not a listing.
- **Leadership melee and Leadership ranged carry different guards.** Melee needs `melee > 0 && race < rt_Arcane` (asm:92, asm:95). Ranged adds a third — `(ranged_type / 10) != rag_Magic` (asm:136) — so a magic ranged attack gets no leadership. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189).
- **Leadership applies to ranged at half value, signed.** asm:144-147 is `mov ax, [es:bx]` / `cwd` / `sub ax, dx` / `sar ax, 1` — the Borland idiom for `/2` truncating toward zero, which matters because leadership can be negative. Production's `/ 2` at [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) is the same operation, not a shift.
- **The holy bonus and the resist-all bonus have different guards from each other.** Holy-to-melee is gated on `melee > 0` with **no** race test (asm:170); resist-all (asm:198-224) and holy-to-defense (asm:225-250) are unconditional. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) keeps all three as written.
- **The hero-item Haste scan covers every slot.** asm:260-311 loops `_DI_itr_hero_items` over `e_NUM_HERO_ITEMS` (3), reading `_players[owner].Heroes[Hero_Slot].Items[itr]`. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189). Worth contrasting with the OGBUG in `Load_Battle_Unit`, which reads slot `[0]` only — the original really is inconsistent between the two functions.
- **`_ITEMS[].Powers` is IDA's `s_ITEM.enchantments`.** Byte-checked, not assumed: `WIZARDS.inc:280-297` places `enchantments dd` at offset `0x2E` in a `0x32`-byte struct, and production declares `/* 2E */ uint32_t Powers` ([MOM_DAT.h:1221](../../MoX/src/MOM_DAT.h#L1221)). Same offset, same width — a field-name rename only. asm:298-299 masks the **low** word, and `IP_HASTE` is `0x00004000` ([MOM_DEF.h:1029](../../MoX/src/MOM_DEF.h#L1029)), matching `IP_Haste = 4000h`.
- **High Prayer and Prayer test `== 1`, not `!= 0`** (asm:322, asm:331, asm:399, asm:408) — unlike True Light, Darkness, Metal Fires, Mass Invisibility, Warp Reality and Black Prayer, which all test `!= 0`. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) keep the `== 1` for exactly these two.
- **High Prayer suppresses Prayer.** asm:396 is `jmp short loc_99C51`, jumping clear over the Prayer block. Production's `else if` at [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) is the right reconstruction — the two never stack. This is the function's **only** real `else if`; the `rt_Life` / `rt_Death` pair below it looks like one and is not.
- **High Prayer raises resist twice.** `resist++` / `Gold_Resist++` at asm:352-361, then `resist += 2` / `Gold_Resist += 2` at asm:376-385 — `+3` in total, from two separately emitted store pairs. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) reproduce both rather than folding them.
- **The `rt_Life` / `rt_Death` blocks are exact mirrors** — True Light buffs Life and penalises Death, Darkness the reverse (asm:441-497, 498-545, 551-607, 608-655). The buff arms guard `melee > 0` and `ranged > 0`; the penalty arms guard nothing and decrement unconditionally. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189).
- **The two race blocks are separate `if`s, not an `else if`.** asm:436-440 tests `rt_Life`; the block it guards ends at asm:545, and asm:546 is `loc_99D9B:` — the `rt_Death` test — with no `jmp` between them. Borland 3.0 always emits one to skip the second test of an `else if`, so the source had two independent statements; the `rt_Life` Darkness block's own skip at asm:504 targets `loc_99D9B` directly for the same reason. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) are both bare `if`. It cannot matter at runtime — `race` holds one value — but the form is the one the bytes show.
- **`enchantments` is computed twice with two different formulas.** asm:663-669 is `_UNITS[].enchantments | battle_unit->enchantments`, consumed only by the Metal Fires Flame Blade test. asm:978-991 is `(_UNITS[].enchantments ^ battle_unit->enchantments) & battle_unit->enchantments` — the bits set on the battle unit but *not* on the strategic unit, i.e. combat-only enchantments — and that is what `Apply_Enchantment_And_Mutation_Effects` receives. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189). Collapsing these two into one variable would be wrong.
- **Metal Fires has three separate guards and skips if Flame Blade is already up** (asm:690-701): own side, not `UA_FANTASTIC`, and `!(enchantments & UE_FLAME_BLADE)`. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189). `UE_FLAME_BLADE` is `0x00100000` ([MOM_DEF.h:916](../../MoX/src/MOM_DEF.h#L916)), matching the **high**-word `UE_Flame_Blade = 10h` and asm:698-699's `and dx, 0`.
- **Mass Invisibility writes a low-word bit.** asm:766-767 is `or dx, UE_INVISIBILITY` / `or ax, 0`; `UE_INVISIBILITY` is `0x00008000` ([MOM_DEF.h:911](../../MoX/src/MOM_DEF.h#L911)), matching `8000h`. The opposite word from `UE_FLAME_BLADE` two blocks earlier — easy to get backwards, and production has it right.
- **Both side tests read `_combat_attacker_player` in both arms.** Black Prayer (asm:794, asm:805) and Mass Invisibility (asm:751, asm:760) never load `_combat_defender_player`; the *jump senses* are flipped instead. Black Prayer's attacker arm applies when `controller != attacker`, its defender arm when `controller == attacker`. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) reproduce the operand rather than "correcting" it to the defender global. Deliberate in the original, not an oversight.
- **Mind Twist lowers `resist` but raises `Gold_Resist`** (asm:857-866) — a penalty crediting the *gold* counter where every other penalty in the function credits `Grey_*`. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) keeps it.
- **Mind Storm's grey counters run in an odd order** — Defense, Melee, Ranged, Resist (asm:951-970), not matching the order of the four penalties above them. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) preserves it.
- **Warped Attack and Warped Defense credit the grey counter before decrementing**, and the decrement re-reads the still-unmodified stat (asm:1004-1015 then asm:1016-1027). Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) keep that order. Note the *Shatter* block twenty lines later does the opposite — clamp first, then derive — and that inversion is deliberate on the original's part, not a house style. See the OGBUG below.
- **`(x + 1) / 2` is the rounding-up half**, from `cbw` / `inc ax` / `cwd` / `sub ax,dx` / `sar ax,1` (asm:1006-1010). Production writes it literally.
- **Web strips flight with `xor`, not `and ~`** (asm:1120), which is why the guard at asm:1116 has to test `MV_FLYING` first — without it the `xor` would *grant* flight. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) keeps both the guard and the `^=`.
- **The tail calls `Battle_Unit_Hit_Points`, taking the pointer** (asm:1124-1131, production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189)) — not `Unit_Hit_Points` taking an index, which is what `Battle_Unit_Regular_Stats` calls at asm:469. Two different functions, both in `ovr116`, and the distinction is easy to lose.

## OGBUGs — faithful, do not fix

- **Shatter always records a zero loss.** The clamp is written *first* and the grey counter is then derived from the already-clamped value: asm:1074 `mov [melee], 1`, then asm:1078-1081 re-read `melee` (now `1`), `dec`, and store the result — always `0`. The ranged arm at asm:1087/1089-1092 is identical. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) keeps the order. The stat is correctly clamped to 1; the grey "lost" figure the unit-view panel shows is always zero no matter how much attack Shatter actually removed. Reversing the two statements would report the true loss and would be a fix — do not.
- **Weakness compares a group against a type.** asm:901-914 divides `ranged_type` by 10 and compares the quotient against `srat_Thrown` (100) — a value the quotient can never reach — so the `-2` ranged penalty only ever lands on `rag_Missile`. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) preserves the division. Note the Metal Fires block two hundred lines earlier tests the *same* constant **without** dividing (asm:725, production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189)); the original genuinely differs between the two, and both forms are reproduced correctly.
- **Warp Reality penalises `tohit` with no floor** (asm:783-786). Combined with Mind Twist, Vertigo and Mind Storm, `tohit` can go negative; unlike `melee`, `ranged` and `defense`, it is not clamped at asm:1093-1110. Production [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) and [Battle_Unit_Special_Stats](../../MoM/src/COMBINIT.c#L1189) match — three stats clamped, `tohit` and `resist` not.

## Frame map

`sub sp, 0Ch` (asm:12) = 12 bytes, plus two register locals. One parameter at `bp+6`.

| asm slot | asm name | production name |
| --- | --- | --- |
| `bp-0Ch` | `Node_Aura_Applies` | `Node_Aura_Applies` |
| `bp-0Ah` | `item_idx` | `item_idx` |
| `bp-7` | `Mutation_Flags` | `Mutation_Flags` |
| `bp-6` | `enchantments` (dword) | `enchantments` |
| `bp-2` | `unit_idx` | `unit_idx` |
| `bp+6` | `battle_unit` (dword) | `battle_unit` |
| `si` | (scratch, `push es` / `les si` pairs) | — |
| `di` | `_DI_itr_hero_items` | `itr_hero_items` |

`itr_hero_items` is a register local in the original, so it is not in the `sub sp, 0Ch` count; production declares it alongside the five stack locals, which is correct.

---

# Battle_Unit_Item_Stats

Faithful.

Loops the hero's three item slots and applies each item's **numeric fields** — `defense`, `resistance`, `moves2`, `spell_skill`, `attack`, `tohit` — then dispatches the two translator helpers below. Production [COMBINIT.c:62](../../MoM/src/COMBINIT.c#L62).

- **The melee gate is `type <= it_Axe || type == it_Misc`** (asm:176-188); the ranged gate is a five-way test — Bow+missile, Staff+magic, Wand+magic, Axe+thrown, or Misc (asm:280-347).
- **Item powers split three ways.** Twenty go to unit enchantments via the translator below, nine to attack attributes via the other, and exactly two — `ip_Flaming` (+3) and `ip_Giant_Strength` (+1) — are applied inline as stat bumps (asm:222-267, asm:399-433) and appear in neither table. Flaming grants `+3` here where the `UE_Flame_Blade` enchantment grants `+2`.
- **It returns a value its only caller discards.** asm:451-459 ORs the accumulated mask with `battle_unit->item_enchantments` and returns it in `DX:AX`; `Battle_Unit_Regular_Stats` asm:185-186 unwinds without reading it. Production types it `uint32_t`, which is right — the computation is real even if nothing consumes it.
- **The `mana_max += spell_skill` guard is an unsigned byte compare** (asm:117-119, `cmp` + `jbe`), which is why `mana_max` is `uint8_t` at [Combat.h:1255](../../MoM/src/Combat.h#L1255).

# Item_Powers_To_Unit_Enchantments

Faithful.

A translation table: twenty `IP_* -> UE_*` pairs ORed into a local, then ORed into `battle_unit->item_enchantments` at the tail (asm:458-465). Production [COMBINIT.c:161](../../MoM/src/COMBINIT.c#L161).

The whole 469-line listing is uniform — twenty `IP_` tests, twenty `UE_` writes, no store to any field but the local and `item_enchantments`. Production reproduces all twenty in listing order, including **`ip_Holy_Avenger` mapping to `UE_BLESS`**, the same bit `ip_Bless` already sets.

It applies nothing. It converts item powers into the unit-enchantment vocabulary so the shared enchantment table can treat "Flight from an item" and "Flight cast in combat" identically — which is what the old name `BU_Apply_Item_Enchantments` obscured.

# Item_Powers_To_Attack_Attributes

Faithful.

Nine `IP_* -> Att_*` pairs written through a caller-supplied `uint16_t *`: Vampiric→LifeSteal, Lightning→ArmorPrc, Destruction→Destruct, Chaos→DoomDmg, Death→DthTouch, Power_Drain→PwrDrain, Holy_Avenger→DsplEvil, Phantasmal→Illusion, Stoning→StnTouch. Production [COMBINIT.c:250](../../MoM/src/COMBINIT.c#L250).

It never touches a `s_BATTLE_UNIT` — `Battle_Unit_Item_Stats` calls it twice per item, once for `melee_attack_attributes` and once for `ranged_attack_attributes` (asm:269-278, asm:435-443). The listing caches the flags word in `CX` and writes back once (asm:13, asm:189-190) where production ORs through the pointer nine times; that is register allocation, not a source difference.

# Unit_Hit_Points

Faithful apart from R6 and R8, both fixed.

Production [COMBINIT.c:292](../../MoM/src/COMBINIT.c#L292).

- **The Heroism floor reads only `_UNITS[].enchantments`** (asm:25-33), unlike `Battle_Unit_Level_Stats`, which also checks the battle unit's.
- **The level ladder is interleaved, deliberately.** Heroes get `>0`, Constitution, Constitution2, `>1`, `>3` (asm:83-164); *every* unit then gets `>2`, `>4`, `>5`, `>6`, `>7` (asm:166-184). Non-heroes reach the shared block through the `jmp loc_9809A` at asm:81. So `>3` really is tested before `>2`, and production keeps that order.
- **Black Channels is a low-word mask, Lionheart a high-word one** (asm:55 `and dx`, asm:69 `and ax`).
- **Charm of Life adds `max(hit_points / 4, 1)`** (asm:198-207).

# Battle_Unit_Hit_Points

Faithful apart from R5, R7 and R9, all fixed.

Production [COMBINIT.c:377](../../MoM/src/COMBINIT.c#L377). Same level ladder as `Unit_Hit_Points`, with three real differences:

- **The enchantment mask unions three sources** — `_UNITS[].enchantments | battle_unit->enchantments | battle_unit->item_enchantments` (asm:30-39) — where `Unit_Hit_Points` reads only the first.
- **Crusade adds a level** to a non-fantastic unit (asm:50-70); `Unit_Hit_Points` has no equivalent.
- **It credits `Gold_Hits`** for Black Channels and Lionheart (asm:92-96, asm:105-109) and for `Extra_Hits` (asm:250-253).

**The two player lookups deliberately disagree.** Crusade reads `battle_unit->controller_idx` (asm:50-51); Charm of Life reads `_UNITS[unit_idx].owner_idx` (asm:228). Two different players in one function — that is what the bytes do. Do not normalise them.

# Battle_Unit_Level_Stats

Faithful.

Turns a unit's experience level into stat bonuses, with separate tables for heroes and normal units. Production [COMBINIT.c:922](../../MoM/src/COMBINIT.c#L922).

- **Heroism floors the effective level at 3**, checked on *both* `_UNITS[].enchantments` and `battle_unit->enchantments` (asm:20-41), so it works whether cast overland or in combat.
- **Hero table: eight thresholds**, `>0` through `>7` (asm:51-199). **Normal table: five**, `>0` through `>4` (asm:200-269). Nine and six tiers counting level 0.
- **Cumulative, not a switch** — each threshold stacks on the ones below it.
- **`melee` and `ranged` increment only when already `> 0`**; `resist`, `defense` and `tohit` increment unconditionally, so a unit with no ranged attack never acquires one by levelling.

IDA names the second parameter `strategic_unit` (asm:4). That label is wrong — every access through it is `s_BATTLE_UNIT.*`.

# Battle_Unit_Hero_Skill_Stats

Faithful apart from R6, fixed.

Applies `HSA_*` hero skills read from `_HEROES2[owner][type].abilities`, most scaled by hero level. Production [COMBINIT.c:1102](../../MoM/src/COMBINIT.c#L1102).

Noble — or a famous player — zeroes upkeep (asm:35-50). Agility and Agility2 raise defense by `Level+1` and `((Level+1)*3)/2`. Blademaster and Blademaster2 raise tohit by `(Level+1)/2` and `((Level+1)*3)/4`. Might and Might2 mirror Agility on melee. Arcane Power and Arcane Power2 mirror it on ranged but **only for `rag_Magic` attacks** (asm:249-256, asm:283-290). `Casting_Skill > 0` sets `mana_max = ((Casting_Skill+1) * (Level+1) * 5) / 2`, and zero otherwise (asm:318-353). Lucky adds `+1` to tohit, toblock, resist and `Gold_Resist`.

# Apply_Enchantment_And_Mutation_Effects

Faithful apart from R11, R12 and R13, all fixed.

Twenty-one enchantment and mutation effects, applied to whatever bits the caller passes. Production [COMBINIT.c:734](../../MoM/src/COMBINIT.c#L734).

**Mask-driven, and that is the point.** `Battle_Unit_Regular_Stats` passes `item_enchantments | _UNITS[].enchantments` (asm:436); `Battle_Unit_Special_Stats` passes `(_UNITS[].ench ^ bu->ench) & bu->ench` (asm:999) — the bits acquired during this combat. One table, two slices of the same bit space.

- **Iron Skin / Stone Skin is the only `if`/`else if`** (asm:161-194); they do not stack.
- **`race == rt_Death` grants `USA_IMMUNITY_DEATH`** (asm:469-476) and is not a mask bit at all. It sits *after* everything that can set `rt_Death` — the Undead mutation and Black Channels — so its position is load-bearing.
- **Berserk sets `defense = -20`**, not 0 (asm:503-504), records the whole lost defense in `Grey_Defense`, and doubles melee with `shl al, 1` under a `melee > 0` guard that the `Grey_Defense` and `defense` stores sit *outside*.
- **Each of the three Chaos Channels mutations also sets `race = rt_Chaos`** (asm:354-386).

## OGBUGs — faithful, do not fix

- **Black Channels withholds its `+1` ranged only from an exhausted attack, not from a melee-only unit.** asm:113-115 tests `ranged_type != rat_None` — that is `255`, which in a signed byte is `rat_UNDEF`, the out-of-ammo marker set by paths such as [Combat.c:11698](../../MoM/src/Combat.c#L11698) and [Combat.c:15714](../../MoM/src/Combat.c#L15714). A melee-only unit carries `ranged_type == rat_NONE` (0) and still receives the bonus.

# NIU_Unit_Has_Spell_Enchantment

Faithful.

Answers "is this spell's enchantment already on this unit?", spanning both the unit's own enchantments and those granted by its hero items. Production [COMBINIT.c:1493](../../MoM/src/COMBINIT.c#L1493).

`spell_data_table[].Param0` and `.Params2_3` are the low and high words of the spell's 32-bit effect mask ([MOM_DAT.h:1060](../../MoX/src/MOM_DAT.h#L1060)), so the spell index is really a lookup for *which* `UE_*` bit to test (asm:32-41).

**It clobbers `global_battle_unit`.** asm:14-19 calls `Load_Battle_Unit(unit_idx, global_battle_unit)` for exactly one reason — to populate `item_enchantments`, which `_UNITS[].enchantments` does not carry. That is why this is not a one-line mask test, and it destroys the buffer the unit-view and army-list panels draw from.

Currently unreferenced, hence the `NIU_` prefix. Within `ovr116` it is the one function that *calls* `Load_Battle_Unit` rather than being called by it, so its callers live outside the overlay.


# What else belongs in this review

Nothing further in `ovr116` — the bundle is complete. The overlay's twelve functions plus `Begin_Combat_Turn`, `Add_City_Damage_From_Battle_Units_Within` and `CMB_Units_Init__WIP` are all compared above.

**Tier 2 — the globals `Battle_Unit_Special_Stats` reads.** `Calc_Battlefield_Bonuses` (`ovr122`, 450 lines) produces `_battlefield_leadership`, `_battlefield_holybonus` and `_battlefield_resistall`, and `Begin_Combat_Turn` calls it at asm:50-51 immediately before the per-unit loop. All three are read every turn by the first five effect blocks, and nothing has verified how they are filled. `_battlefield_resistall` is also the second argument to the Terror `Combat_Resistance_Check` (asm:156-159), so it reaches two functions reviewed here. This is the highest-value follow-on.

**Tier 3 — the rest of `CMB_Units_Init__WIP`'s callees.** `Deploy_Battle_Units` (`ovr113`, 375) and `Combat_Figure_Load` (`ovr163`, 142). Deployment is squarely "init/prep", but both are self-contained and neither feeds a finding above.

**Not this bundle.** `Unit_Moves2` (`ovr121`, 244) and `Battle_Unit_Moves2` (`ovr124`, 300) are movement, reachable from a dozen places, and belong with the movement functions. `Prod_Init_Battle_Unit` (`ovr089`, 73) and `Prod_Load_Battle_Unit` ([UnitView.c:2505](../../MoM/src/UnitView.c#L2505)) are the production-screen analogues of `Battle_Unit_Regular_Stats` and `Load_Battle_Unit` — worth a short comparison pass someday, but not on any path reviewed here.

## One question left open

`Apply_Enchantment_And_Mutation_Effects` is called twice per unit per turn with two masks that are **not** provably disjoint. The Special mask, `(_UNITS[].ench ^ bu->ench) & bu->ench` (asm:978-991), subtracts `_UNITS[].enchantments` but **not** `item_enchantments`. So an enchantment both granted by a hero item and cast during the battle would land in both masks and apply twice — `+1` melee twice for Giant Strength, say. Whether any combat-castable spell overlaps the item-granted set is not answerable from these listings; it needs the combat spell table. Recorded rather than filed as a finding, because it may not be reachable.

---

## Build state

**`COMBINIT.c` compiles clean — 0 errors, 0 warnings** under `MSVC-debug` with `/WX`, covering all twelve `ovr116` functions and the R5-R13 fixes (2026-08-06). One include was added to get there: `MOX_BITS.h`, for the `GET_4B_OFS()` that `MOX_DEF.h`'s `ITEM_POWER()` expands to.

**The tree as a whole does not build**, for reasons outside this review — three structural moves in the working tree that predate or parallel the `ovr116` split and have never been compiled:

- `struct s_BATTLE_UNIT` moved from [MOM_DAT.h](../../MoX/src/MOM_DAT.h) to [Combat.h:1193](../../MoM/src/Combat.h#L1193). [MOM_DAT.c:1181](../../MoX/src/MOM_DAT.c#L1181) and [UnitView.c](../../MoM/src/UnitView.c) still expect it from the old home — 1 and 56 errors.
- Five globals — `_battlefield_leadership`, `_battlefield_resistall`, `_battlefield_holybonus`, `CMB_IDK_4PR`, `_combat_spell_target_type` — moved from `Combat.c` to [CMBMAGIC.c:132-139](../../MoM/src/CMBMAGIC.c#L132-L139). No header declares them, so `Combat.c` no longer sees the ones it uses — 91 errors.
- Nothing includes [COMBINIT.h](../../MoM/src/COMBINIT.h). Ten files call the moved functions with no prototype in scope; `/WX` turns each C4013 into an error — `Combat.c`, `HIRE.c`, `LVLMAKE.c`, `MainScr.c`, `NEXTTURN.c`, `Spells129.c`, `Spells131.c`, `UnitList.c`, `UnitMove.c`, `UnitStat.c`.

None of these touch the reconstruction fidelity recorded above, and none are decided by this review.
**Anchors.** Production links point at **function start lines**, not at individual statements — `Combat.c` and `COMBINIT.c` have shifted repeatedly during this review and statement-level anchors did not survive it. For statement-level precision use the `asm:N` references, which are stable ground truth. Twelve of the fifteen reviewed functions now live in `COMBINIT.c`, not `Combat.c`.

## Related references

- [Combat-Battle_Unit_Attack.md](Combat-Battle_Unit_Attack.md) — the Wind Mastery finding R2 mirrors, and the source of the `Battle_Unit_Item_Stats` ordering assumption; DONE-DONE.
- [Combat-Battle_Unit_Action.md](Combat-Battle_Unit_Action.md) — DONE-DONE.
- [Combat-Summon_Demon.md](Combat-Summon_Demon.md) — `Prepare_Battle_Unit` calls `Load_Battle_Unit`, reviewed there as out of scope and covered here; DONE-DONE.
- [MOM_DAT.h:831-837](../../MoX/src/MOM_DAT.h#L831-L837) — `e_COMBAT_NODE_TYPE`, whose values feed the node-aura test. See the note under `Battle_Unit_Special_Stats` before touching them.
