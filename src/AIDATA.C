/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
        ¿ ovr164 ?
*/

#include "MoM.H"



// WZD dseg:8890                                                 ¿ BEGIN: ?
// WZD dseg:8890                                                 IDK
// WZD dseg:8890                                                 ...*feels like* ~ AIDATA

// WZD dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Arcanus CONT_TBL <0>
// WZD dseg:8C14 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Myrror CONT_TBL <0>

/*
; 80 + rnd(40) is stored here for each AI wizard (word
; array pointer, human excluded) when setting initial
; gold during game creation in MAGIC.EXE
; decreased during the AI's turn, likely some relation
; or interest value
*/
// WZD dseg:8F98 
int16_t AI_SCircle_Reevalsl;

// WZD dseg:8F9A AI_Arc_NewColTgtYs@ dw 0                ; DATA XREF: Allocate_Data_Space+3F6w ...
// WZD dseg:8F9C AI_Myr_NewColTgtYs@ dw 0                ; DATA XREF: Allocate_Data_Space+3FCw
// WZD dseg:8F9E AI_Arc_NewColTgtXs@ dw 0                ; DATA XREF: Allocate_Data_Space+3EAw ...
// WZD dseg:8FA0 AI_Myr_NewColTgtXs@ dw 0                ; DATA XREF: Allocate_Data_Space+3F0w
// WZD dseg:8FA2 Wiz5_Spell_50h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D2w
// WZD dseg:8FA4 Wiz5_Spell_64h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D8w
// WZD dseg:8FA6 Wiz5_Spell_28h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C6w
// WZD dseg:8FA8 Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3CCw
// WZD dseg:8FAA AI_CONTX_Reevals@ dw 0                  ; DATA XREF: Loaded_Game_Update_WZD+B4r ...
// WZD dseg:8FAC AI_Arc_NewColConts@ dw 0                ; DATA XREF: Allocate_Data_Space+3DEw ...
// WZD dseg:8FAE AI_Myr_NewColConts@ dw 0                ; DATA XREF: Allocate_Data_Space+3E4w
// WZD dseg:8FB0 AI_Arc_MainWarConts@ dw 0               ; DATA XREF: Allocate_Data_Space+3B4w ...
// WZD dseg:8FB2 AI_Myr_MainWarConts@ dw 0               ; DATA XREF: Allocate_Data_Space+3BAw
// WZD dseg:8FB4 unk_3FA54 db    0                       ; DATA XREF: sub_F6CAB+103o
// WZD dseg:8FB5 db    0
// WZD dseg:8FB6 db    0
// WZD dseg:8FB7 db    0
// WZD dseg:8FB8 db    0
// WZD dseg:8FB9 db    0
// WZD dseg:8FBA db    0
// WZD dseg:8FBB db    0
// WZD dseg:8FBC db    0
// WZD dseg:8FBD db    0
// WZD dseg:8FBE db    0
// WZD dseg:8FBF db    0
// WZD dseg:8FC0 db    0
// WZD dseg:8FC1 db    0
// WZD dseg:8FC2 db    0
// WZD dseg:8FC3 db    0

// WZD dseg:8FC4
int16_t Some_AI_Turn_Var_2;

// WZD dseg:8FC6
int16_t Some_AI_Turn_Var_3;

// WZD dseg:8FC8 AI_Target_Values dw 19h dup(     0)     ; DATA XREF: AI_AddTarget+3Aw ...
// WZD dseg:8FFA AI_Target_Strengths dw 19h dup(     0)  ; DATA XREF: AI_AddTarget+2Dw ...
// WZD dseg:902C AI_Target_Ys dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+20w ...
// WZD dseg:905E AI_Target_Xs dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+13w ...
// WZD dseg:9090 db    0
// WZD dseg:9091 db    0
// WZD dseg:9092 AI_Target_Count dw 0                    ; DATA XREF: AI_CreateTargetList+8w ...

// WZD dseg:9094
int16_t AI_Transport_Count;

// WZD dseg:9096 AI_SeekTransport_Cnt dw 0               ; DATA XREF: AI_SetUnitOrders:loc_EBDECw ...
// WZD dseg:9098 AI_SeekTransport_Ps dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+B4r ...
// WZD dseg:90B6 AI_SeekTransport_Ys dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+AAr ...
// WZD dseg:90D4 AI_SeekTransport_Xs dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+8Er ...

// ; own str - enemy str - own value
// WZD dseg:90F2
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Arc_ContBalances;

// ; own str - enemy str - own value
// WZD dseg:90F4
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Myr_ContBalances;

// WZD dseg:90F6
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Arc_Cont_Nme_Val;

// WZD dseg:90F8
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Myr_Cont_Nme_Val;

// WZD dseg:90FA
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Arc_Cont_Own_Val;

// WZD dseg:90FC
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Myr_Cont_Own_Val;

// ; redundant, should not be the primary pointer
// WZD dseg:90FE
// allocated in Allocate_AI_Data()
SAMB_ptr CRP_AI_Arc_Cont_Nme_Str;

// ; redundant, should not be the primary pointer
// WZD dseg:9100
// allocated in Allocate_AI_Data()
SAMB_ptr CRP_AI_Myr_Cont_Nme_Str;

// WZD dseg:9102
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Arc_Cont_Own_Str;

// WZD dseg:9104
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Myr_Cont_Own_Str;

// WZD dseg:9106
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Enemy_Stacks;

// WZD dseg:9108
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Own_Stacks;

// WZD dseg:910A AI_Enemy_Stack_Count dw 0               ; DATA XREF: AI_Continent_Eval:loc_D2593r ...
// WZD dseg:910C AI_Own_Stack_Count dw 0                 ; DATA XREF: AI_Continent_Eval:loc_D24FFr ...
// WZD dseg:910E AI_AtWarWith_Players dw 6 dup(0)        ; DATA XREF: AI_Count_Active_Wars+43w ...
// WZD dseg:911A AI_Active_War_Count dw 0                ; DATA XREF: AI_Count_Active_Wars+7w ...

// ; redundant - filled out but never used
// WZD dseg:911C
// allocated in Allocate_AI_Data()
SAMB_ptr CRP_AI_OVL_SpellList;

// WZD dseg:911E CRP_AI_OVL_SpellCount dw 0              ; DATA XREF: AI_OVL_GetSpellList+8w ...

// WZD dseg:9120
// allocated in Allocate_AI_Data()
SAMB_ptr AI_OVL_Spell_Cats;

// WZD dseg:9122
// allocated in Allocate_AI_Data()
SAMB_ptr AI_NME_Garrison_Strs;

// WZD dseg:9124
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Own_Garr_Strs;

// WZD dseg:9126
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Enemy_City_Values;

// WZD dseg:9128
// allocated in Allocate_AI_Data()
SAMB_ptr AI_Own_City_Values;

// WZD dseg:912A ¿ BEGIN: ?
// WZD dseg:912A
// WZD dseg:912A 00 00                                           g_TimeStop_PlayerNum dw 0               ; DATA XREF: Loaded_Game_Update_WZD+C5w ...
// WZD dseg:912A                                                                                         ; index of the Time Stop wizard plus 1 (0 if none)



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0




/*
    WIZARDS.EXE  ovr64
*/

// WZD o164p01
// drake178: AI_ExcessNeutrals()
// AI_ExcessNeutrals()

// WZD o164p02
// drake178: EVNT_RampageMonsters()
// EVNT_RampageMonsters()

// WZD o164p03
// drake178: EVNT_GenerateRaiders()
// EVNT_GenerateRaiders()

// WZD o164p04
// drake178: AI_SetNeutralTargets()
// AI_SetNeutralTargets()

// WZD o164p05
// drake178: AI_Turn_NearAllocs()
// ¿ MoO2  Module: AIDATA  Compute_AI_Data_() ? ¿ ... Deallocate_AI_Data_() ?
/*
; allocates a total of 3,942 bytes of the LBX near
; buffer to various AI arrays
;
; RE-EXPLORE and MOVE
*/
/*

*/
void Allocate_AI_Data(void)
{

    AI_Own_City_Values = Near_Allocate_First(200);

    AI_Enemy_City_Values = Near_Allocate_Next(200);

    AI_Own_Garr_Strs = Near_Allocate_Next(200);

    AI_NME_Garrison_Strs = Near_Allocate_Next(200);

    AI_Own_Stacks = Near_Allocate_Next(800);

    AI_Enemy_Stacks = Near_Allocate_Next(1440);

    AI_OVL_Spell_Cats = Near_Allocate_Next(92);

    CRP_AI_OVL_SpellList = Near_Allocate_Next(50);

    AI_Arc_Cont_Own_Str = Near_Allocate_Next(120);

    AI_Myr_Cont_Own_Str = Near_Allocate_Next(120);

    CRP_AI_Arc_Cont_Nme_Str = Near_Allocate_Next(120);

    CRP_AI_Myr_Cont_Nme_Str = Near_Allocate_Next(120);

    AI_Arc_Cont_Own_Val = Near_Allocate_Next(120);

    AI_Myr_Cont_Own_Val = Near_Allocate_Next(120);

    AI_Arc_Cont_Nme_Val = Near_Allocate_Next(120);

    AI_Myr_Cont_Nme_Val = Near_Allocate_Next(120);

    AI_Arc_ContBalances = CRP_AI_Arc_Cont_Nme_Str;

    AI_Myr_ContBalances = CRP_AI_Myr_Cont_Nme_Str;

}


// WZD o164p06
// drake178: AI_Magic_Strategy()
// AI_Magic_Strategy()

// WZD o164p07
// drake178: AI_Evaluate_War()
/*
*/
/*



*/
void AI_Evaluate_War__WIP(int16_t player_idx)
{



}


// WZD o164p08
// drake178: AI_GetNeutralStacks()
// AI_GetNeutralStacks()
