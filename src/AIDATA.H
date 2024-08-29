/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
*/

#ifndef AIDATA_H
#define AIDATA_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



#define AI_OBJ_PRAGMATIST       0
#define AI_OBJ_MILITARIST       1
#define AI_OBJ_THEURGIST        2
#define AI_OBJ_PERFECTIONIST    3
#define AI_OBJ_EXPANSIONIST     4

// //  ; (sizeof=0x5, standard type)
// struct s_AI_BLD_Obj_Modifier
// {
//     /* 00 */ Pragmatist db ?                         ; base 10
//     /* 01 */ Militarist db ?                         ; base 10
//     /* 02 */ Theurgist db ?                          ; base 10
//     /* 03 */ Perfectionist db ?                      ; base 10
//     /* 04 */ Expansionist db ?                       ; base 10
//     /* 05 */
// };
// 
// //  ; (sizeof=0x32)
// struct s_AI_BLD_Obj_Priorities
// {
//     /* 00 */  Projects AI_BLD_Obj_Modifier ?
//     /* 05 */  Gold AI_BLD_Obj_Modifier ?
//     /* 0A */  Religion AI_BLD_Obj_Modifier ?
//     /* 0F */  Research AI_BLD_Obj_Modifier ?
//     /* 14 */  Military AI_BLD_Obj_Modifier ?
//     /* 19 */  Production AI_BLD_Obj_Modifier ?
//     /* 1E */  Food AI_BLD_Obj_Modifier ?
//     /* 23 */  Navy AI_BLD_Obj_Modifier ?
//     /* 28 */  Combat_Unit AI_BLD_Obj_Modifier ?
//     /* 2D */  Builder AI_BLD_Obj_Modifier ?
//     /* 32 */
// };



// WZD dseg:8F98 BEGIN:  ovr145 -
// WZD dseg:8F98
// WZD dseg:8F98 AI_SCircle_Reevals@ dw 0                ; DATA XREF: AI_Overland_Turn+136r ...
// WZD dseg:8F98                                         ; 16 bytes
// WZD dseg:8F98                                         ; 80 + rnd(40) is stored here for each AI wizard (word
// WZD dseg:8F98                                         ; array pointer, human excluded) when setting initial
// WZD dseg:8F98                                         ; gold during game creation in MAGIC.EXE
// WZD dseg:8F98                                         ; decreased during the AI's turn, likely some relation
// WZD dseg:8F98                                         ; or interest value
// WZD dseg:8F9A AI_Arc_NewColTgtYs@ dw 0                ; DATA XREF: Allocate_Data_Space+3F6w ...
// WZD dseg:8F9A                                         ; 12 bytes
// WZD dseg:8F9C AI_Myr_NewColTgtYs@ dw 0                ; DATA XREF: Allocate_Data_Space+3FCw
// WZD dseg:8F9C                                         ; 12 bytes
// WZD dseg:8F9E AI_Arc_NewColTgtXs@ dw 0                ; DATA XREF: Allocate_Data_Space+3EAw ...
// WZD dseg:8F9E                                         ; 20 bytes
// WZD dseg:8FA0 AI_Myr_NewColTgtXs@ dw 0                ; DATA XREF: Allocate_Data_Space+3F0w
// WZD dseg:8FA0                                         ; 20 bytes
// WZD dseg:8FA2 Wiz5_Spell_50h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D2w
// WZD dseg:8FA2                                         ; 20 bytes
// WZD dseg:8FA4 Wiz5_Spell_64h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D8w
// WZD dseg:8FA4                                         ; 20 bytes
// WZD dseg:8FA6 Wiz5_Spell_28h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C6w
// WZD dseg:8FA6                                         ; 20 bytes
// WZD dseg:8FA8 Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3CCw
// WZD dseg:8FA8                                         ; 20 bytes
// WZD dseg:8FAA AI_CONTX_Reevals@ dw 0                  ; DATA XREF: Loaded_Game_Update_WZD+B4r ...
// WZD dseg:8FAA                                         ; 16 bytes
// WZD dseg:8FAC AI_Arc_NewColConts@ dw 0                ; DATA XREF: Allocate_Data_Space+3DEw ...
// WZD dseg:8FAC                                         ; 20 bytes
// WZD dseg:8FAE AI_Myr_NewColConts@ dw 0                ; DATA XREF: Allocate_Data_Space+3E4w
// WZD dseg:8FAE                                         ; 20 bytes
// WZD dseg:8FB0 AI_Arc_MainWarConts@ dw 0               ; DATA XREF: Allocate_Data_Space+3B4w ...
// WZD dseg:8FB0                                         ; 12 bytes, Arcanus array
// WZD dseg:8FB2 AI_Myr_MainWarConts@ dw 0               ; DATA XREF: Allocate_Data_Space+3BAw
// WZD dseg:8FB2                                         ; 12 bytes, Myrror array
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
// WZD dseg:8FC4 Some_AI_Turn_Var_2 dw 0                 ; DATA XREF: AI_Overland_Turn+212w
// WZD dseg:8FC4                                         ; now 0
// WZD dseg:8FC6 Some_AI_Turn_Var_3 dw 0                 ; DATA XREF: AI_Overland_Turn+218w
// WZD dseg:8FC6                                         ; now 0
// WZD dseg:8FC8 AI_Target_Values dw 19h dup(     0)     ; DATA XREF: AI_AddTarget+3Aw ...
// WZD dseg:8FFA AI_Target_Strengths dw 19h dup(     0)  ; DATA XREF: AI_AddTarget+2Dw ...
// WZD dseg:902C AI_Target_Ys dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+20w ...
// WZD dseg:905E AI_Target_Xs dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+13w ...
// WZD dseg:9090 db    0
// WZD dseg:9091 db    0
// WZD dseg:9092 AI_Target_Count dw 0                    ; DATA XREF: AI_CreateTargetList+8w ...

// WZD dseg:9094
extern int16_t AI_Transport_Count;

// WZD dseg:9096 AI_SeekTransport_Cnt dw 0               ; DATA XREF: AI_SetUnitOrders:loc_EBDECw ...
// WZD dseg:9098 AI_SeekTransport_Ps dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+B4r ...
// WZD dseg:90B6 AI_SeekTransport_Ys dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+AAr ...
// WZD dseg:90D4 AI_SeekTransport_Xs dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+8Er ...
// WZD dseg:90F2 AI_Arc_ContBalances@ dw 0               ; DATA XREF: AI_Continent_Eval+4C5r ...
// WZD dseg:90F2                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F2                                         ; own str - enemy str - own value
// WZD dseg:90F4 AI_Myr_ContBalances@ dw 0               ; DATA XREF: AI_Turn_NearAllocs+DCw
// WZD dseg:90F4                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F4                                         ; own str - enemy str - own value
// WZD dseg:90F6 AI_Arc_Cont_Nme_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+64r ...
// WZD dseg:90F6                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F8 AI_Myr_Cont_Nme_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+72r ...
// WZD dseg:90F8                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FA AI_Arc_Cont_Own_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+48r ...
// WZD dseg:90FA                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FC AI_Myr_Cont_Own_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+56r ...
// WZD dseg:90FC                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FE CRP_AI_Arc_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+2Cr ...
// WZD dseg:90FE                                         ; redundant, should not be the primary pointer
// WZD dseg:9100 CRP_AI_Myr_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+3Ar ...
// WZD dseg:9100                                         ; redundant, should not be the primary pointer
// WZD dseg:9102 AI_Arc_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+10r ...
// WZD dseg:9102                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:9104 AI_Myr_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+1Er ...
// WZD dseg:9104                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:9106 AI_Enemy_Stacks@ dw 0                   ; DATA XREF: AI_Continent_Eval+121r ...
// WZD dseg:9106                                         ; 1440 LBX_NearAlloc_Next bytes
// WZD dseg:9108 AI_Own_Stacks@ dw 0                     ; DATA XREF: AI_Continent_Eval+8Er ...
// WZD dseg:9108                                         ; 800 LBX_NearAlloc_Next bytes
// WZD dseg:910A AI_Enemy_Stack_Count dw 0               ; DATA XREF: AI_Continent_Eval:loc_D2593r ...
// WZD dseg:910C AI_Own_Stack_Count dw 0                 ; DATA XREF: AI_Continent_Eval:loc_D24FFr ...
// WZD dseg:910E AI_AtWarWith_Players dw 6 dup(0)        ; DATA XREF: AI_Count_Active_Wars+43w ...
// WZD dseg:911A AI_Active_War_Count dw 0                ; DATA XREF: AI_Count_Active_Wars+7w ...
// WZD dseg:911C CRP_AI_OVL_SpellList dw 0               ; DATA XREF: AI_OVL_GetSpellList+DBr ...
// WZD dseg:911C                                         ; 50 LBX_NearAlloc_Next bytes
// WZD dseg:911C                                         ; redundant - filled out but never used
// WZD dseg:911E CRP_AI_OVL_SpellCount dw 0              ; DATA XREF: AI_OVL_GetSpellList+8w ...
// WZD dseg:911E                                         ; redundant - calculated but never used
// WZD dseg:9120 AI_OVL_Spell_Cats dw 0                  ; DATA XREF: AI_OVL_SplCat_Picker:loc_E4471r ...
// WZD dseg:9120                                         ; 92 LBX_NearAlloc_Next bytes
// WZD dseg:9122 AI_NME_Garrison_Strs dw 0               ; DATA XREF: AI_OVL_Eval_Assets+15r ...
// WZD dseg:9122                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9124 AI_Own_Garr_Strs dw 0                   ; DATA XREF: AI_OVL_Eval_Assets+24r ...
// WZD dseg:9124                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9126 AI_Enemy_City_Values dw 0               ; DATA XREF: AI_Continent_Eval+36Fr ...
// WZD dseg:9126                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9128 AI_Own_City_Values dw 0                 ; DATA XREF: AI_Continent_Eval+351r ...
// WZD dseg:9128                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:912A
// WZD dseg:912A ¿ BEGIN: ?



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0



#ifdef __cplusplus
}
#endif

#endif  /* AIDATA_H */
