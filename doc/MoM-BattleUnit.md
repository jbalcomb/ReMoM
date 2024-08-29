


Eh? "Battle Unit"
got that from drake178
but, it gets used for the 'Unit Statistics Window', off the 'Main Screen'

MoX_Data.H
struct s_BU_REC
struct s_STRATEGIC_UNIT


MoO2  Module: COMBINIT  Load_Combat_Ship_()

1oom
    struct battle_item_s
    vs.
    struct shipparsed_s
¿ struct shipparsed_s is ~== MoM s_UNIT_TYPE ?

battle_item_from_parsed() ~== Load_Combat_Ship_()





Load_Battle_Unit() vs. BU_Init_Battle_Unit()
    some same fields cleared
Load_Battle_Unit()
    sets the strategic_unit->unit_idx
    ¿ the rest expect/get/use it from there ?

BU_Apply_Level()
    does not assume the BU is up-to-date - checks UNITS[] and strategic_unit

¿ if/why gets passed unit_idx ?
    Load_Battle_Unit()
    BU_Apply_Level()
        BU_Apply_Level() is only called from BU_Init_Battle_Unit()
        but, BU_Init_Battle_Unit() doesn't get passed the unit_idx
            it gets it from Load_Battle_Unit()

BU_ApplyCombatFX()



End-To-End:
    Load_Battle_Unit()
    BU_Init_Battle_Unit() ... BU_ApplyCombatFX()








MoO2
    Load_Combat_Ship_()
        |-> Best_Warp_Drive_()
        |-> Get_Ship_Structure_()
        |-> Get_Ship_Armor_Hits_()
        |-> Init_Internal_Space_()
        |-> Calc_Combat_Shield_Max_()
            |-> Does_Combat_Ship_Have_Special_()
        |-> Init_Special_Devices_()
            |-> Does_Combat_Ship_Have_Special_()



Main_Screen()
    BEGIN: Right-Click Unit Window Grid Field
        USW_FullDisplay()
            Load_Battle_Unit(unit_idx, BattleUnit)
                BU_Init_Battle_Unit(BattleUnit)
                    BU_Apply_Level(BattleUnit)
                    BU_Init_Hero_Unit(BattleUnit)
                    BU_Apply_Items(BattleUnit)
                        BU_Apply_Item_Enchantments()
                        BU_Apply_Item_Attack_Specials()
                    BU_Apply_Specials(BattleUnit, Enchants, Unit_Mutations)
                    UNIT_GetHalfMoves_WIP()
                    UNIT_GetHitsPerFig()

Combat()
    Strategic_Combat()
        Load_Battle_Unit()
            BU_Init_Battle_Unit(BattleUnit)
                BU_Apply_Level(BattleUnit)
                BU_Init_Hero_Unit(BattleUnit)
                BU_Apply_Items(BattleUnit)
                    BU_Apply_Item_Enchantments()
                    BU_Apply_Item_Attack_Specials()
                BU_Apply_Specials(BattleUnit, Enchants, Unit_Mutations)
    ...


Some used elsewhere as intermediate or just for display purposes?
e.g., Prod_Init_Battle_Unit  ~== MoO2  Load_Display_Combat_Ship()?
    ... Prod_Build_Specials_List(active_product_idx, prod_specials_list, &UV_specials_count);





MoO2  Module: COMBINIT  Init_Special_Devices_()
¿ MoO2 Leader mods ?

BU_Apply_Level()
BU_Init_Hero_Unit()
BU_Apply_Items()



BU_Apply_Items(BattleUnit)
BU_Apply_Item_Enchantments()

¿¿¿ 
_players.Heroes.Items
vs.
_HEROES
???
struct HIRED_HERO
00000000 unit_idx dw ?
00000002 Name db 14 dup(?)
00000010 Items dw 3 dup(?)
00000016 Item_Slots dw 3 dup(?)
1C
...
s_UNIT.Hero_Slot -> _HEROES[hero_owner_idx][hero_type]
_UNITS[unit_idx].owner_idx => _HEROES[player_idx]
_UNITS[unit_idx].hero_slot_idx => _players[].Heroes[]

¿ item_idx = ? ... _ITEMS[item_idx]

WHY!?!  ¿ _players[].owned_heroes[].unit_idx => ?

¿ Items[] vs. Item_Slots[] ?

Maybe something with 'Vault Items'?


BU_Apply_Items() uses s_STRATEGIC_UNIT.mana_max as if it means 'has spell caster ability'
...which means it is relying on the previously called function(s) to have set it, given the HSA








### BU_Init_Battle_Unit()

gets called directly elsewhere
so, not just a part of loading a 'Battle Unit'
    BU_Heal()
    CMB_AnimateDead()
    CMB_PrepareTurn()
    CMB_RaiseDead()
    G_CMB_SpellEffect()




BU_Apply_Items(BattleUnit)
    BU_Apply_Item_Enchantments()
    BU_Apply_Item_Attack_Specials()
BU_Apply_Specials()


### BU_Apply_Items()


### BU_Apply_Item_Enchantments()

BU_Apply_Item_Enchantments(item_idx, strategic_unit)
...applies an item to a combat unit...

### BU_Apply_Item_Attack_Specials()


### BU_Apply_Specials()
    does a bunch of different stuff
    applies 'battle unit enchantments' (unit enchants & item enchantments)
    applies 'unit mutations'
    applies special attack abilities, as were set in BU_Apply_Item_Attack_Specials()
unit mutations comes straight through
unit enchantments comes straight through, just with item enchantments added








copies the unit type structure over the battle unit structure
starts at the offset to melee, so skips name
but, still does count of 24h 36d

_fmemcpy()
dst: fp_BattleUnit
src: 
n:

void far * far _fmemcpy(void far * dest, const void far * src, size_t n)



WZD dseg:9226 00 00 00 00                                     Active_Unit@ dd 0

Allocate_Data_Space()
    Active_Unit = SA_MK_FP0(Allocate_Space(8));
8 paragraphs, 128 bytes

sizeof: 6Eh  110d
struct BU_REC


https://stackoverflow.com/questions/5305084/memcpy-of-a-part-of-a-struct
memcpy(&s1.pod1, &s2.pod1, (char*)(1 + &s1.pod4) - (char*)(&s1.pod1));

So?
    first 36 bytes are the same, but last two bytes are meaningles
    Then?
        clears, overwrites, defaults what?
            ?
        gSheet?
            time-lapse?  ...was/is?  ...then, here, now?
        
        







// WZD o116p06
void Load_Battle_Unit(int16_t unit_idx, struct s_BU_REC * BattleUnit)

Up   J j_Load_Battle_Unit jmp     Load_Battle_Unit            ; creates a battle unit record for the specified unit,
Down p sub_9A43E+15           call    near ptr Load_Battle_Unit   ; creates a battle unit record for the specified unit,



code shows itr < on Regular_Dmg
drake178 has 
    00000036 Regular_Dmg db ?
    00000037 Undeath_Dmg db ?
    00000038 Irreversible_Dmg db ?

