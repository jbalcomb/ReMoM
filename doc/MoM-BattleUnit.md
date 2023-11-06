


Eh? "Battle Unit"
got that from drake178
but, it gets used for the 'Unit Statistics Window', off the 'Main Screen'

MoX_Data.H
struct s_BU_REC







Main_Screen()
    BEGIN: Right-Click Unit Window Grid Field
        USW_FullDisplay()
            UNIT_Create_BURecord(unit_idx, BattleUnit)
                BU_SetBaseStats(BattleUnit)
                    UNIT_BU_LevelStats(BattleUnit)
                    UNIT_BU_HeroAbStats(BattleUnit)
                    UNIT_BU_ApplyItems(BattleUnit)
                    BU_ApplyEnchants(BattleUnit, Enchants, Unit_Mutations)







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
void UNIT_Create_BURecord(int16_t unit_idx, struct s_BU_REC * BattleUnit)

Up   J j_UNIT_Create_BURecord jmp     UNIT_Create_BURecord            ; creates a battle unit record for the specified unit,
Down p sub_9A43E+15           call    near ptr UNIT_Create_BURecord   ; creates a battle unit record for the specified unit,



code shows itr < on Regular_Dmg
drake178 has 
    00000036 Regular_Dmg db ?
    00000037 Undeath_Dmg db ?
    00000038 Irreversible_Dmg db ?

