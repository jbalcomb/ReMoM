
CONTX_FirstTiles[]
CONTX_TileChain[]
    Next_Tile_ChainIndex = CONTX_FirstTiles[wp][landmass_idx];
    while(Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
    {
    }



CONTX_OwnStack_Count  ==>  _ai_own_stack_count
CONTX_OwnStack_Xs  ==>  _ai_own_stack_wx
CONTX_OwnStack_Ys  ==>  _ai_own_stack_wy
CONTX_OwnStack_Ps  ==>  _ai_own_stack_wp
CONTX_OwnStack_UCnt  ==>  _ai_own_stack_unit_count
CONTX_OwnStack_ULsts  ==>  _ai_own_stack_unit_list
CONTX_OwnStack_Types  ==>  _ai_own_stack_type

Why did he name them "own"?
Is there a compliment to these for the other wizards/opponents/enemies?

AI_Target_Values[]  ==>  _ai_targets_value[]
AI_Target_Strengths[]  ==>  _ai_targets_strength[]
AI_Target_Ys[]  ==>  _ai_targets_wy[]
AI_Target_Xs[]  ==>  _ai_targets_wx[]
AI_Target_Count  ==>  _ai_targets_count



_ai_targets_value[]
_ai_targets_strength[]
_ai_targets_wy[]
_ai_targets_wx[]
_ai_targets_count



AI_Own_Stacks[]
AI_Own_Stack_Count
AI_Enemy_Stacks[]
AI_Enemy_Stack_Count






## _ai_continents


...in AI_SingleCont_Reeval__WIP(),  
...gets used as a temporary variable to hold count of cities

MoM_Data.C
struct s_AI_CONTINENTS _ai_continents;
struct s_AI_CONTINENTS
{
    struct s_AI_CONTINENTS_PLAYERS plane[NUM_PLANES];
};
struct s_AI_CONTINENTS_PLAYERS
{
    struct s_CONTS player[NUM_PLAYERS];
};
struct s_CONTS
{
    /* 00 */ uint8_t X_Coords[NUM_LANDMASSES];
    /* 3C */ uint8_t Y_Coords[NUM_LANDMASSES];
    /* 78 */ uint8_t Cont_Types[NUM_LANDMASSES];    /* ; enum CONT_Types */
    /* B4 */ 
};

¿ could just be struct s_CONTS _ai_continents[2][6]; ?














## AI_Own_Stacks[] & AI_Own_Stack_Count

CONTX_OwnStack_Count
    used to index
        CONTX_OwnStack_Xs[]
        CONTX_OwnStack_Ys[]
        CONTX_OwnStack_Ps[]
        CONTX_OwnStack_UCnt[]
        CONTX_OwnStack_ULsts[][]
        CONTX_OwnStack_Types[]


AI_CONTX_Regroup__WIP()
    CONTX_OwnStack_Count == 0
    CONTX_OwnStack_Count++


AI_Own_Stacks[].value is used to populate AI_Cont_Own_Str[]

AI_Cont_Own_Str[landmass_idx] = AI_Own_Stacks[itr].value / 10

AI_Transport_Sailoff() |-> AI_Stack_Set_Boats_Goto() sets AI_Own_Stacks[].value to ST_UNDEFINED

AI_CONTX_Regroup__WIP()
XREF:
    NX_j_AI_CONTX_Regroup__WIP jmp     AI_CONTX_Regroup__WIP         
    AI_SetUnitOrders__WIP+F2   call    near ptr AI_CONTX_Regroup__WIP
    AI_ProcessOcean+2F3        call    near ptr AI_CONTX_Regroup__WIP
    G_AI_ProcessTransports+12B call    near ptr AI_CONTX_Regroup__WIP


CONTX_OwnStack_Xs@
CONTX_OwnStack_Ys@
CONTX_OwnStack_Ps@
CONTX_OwnStack_UCnt@
CONTX_OwnStack_Types
CONTX_OwnStack_ULsts

¿ _star_stage_weights ?
Move_To_Stage_Points_()










## CONTX_OwnStack_Ps[]


XREF:
    EMM_Map_CONTXXX__WIP+E8      mov     [word ptr CONTX_OwnStack_Ps], ax  
    AI_CONTX_Regroup__WIP+32     les     bx, [CONTX_OwnStack_Ps]           
    AI_CONTX_Regroup__WIP+12B    les     bx, [CONTX_OwnStack_Ps]           
    AI_CONTX_Regroup__WIP+169    les     bx, [CONTX_OwnStack_Ps]           
    AI_GarrBuilderPush+8E        les     bx, [CONTX_OwnStack_Ps]           
    AI_GarrBuilderPush:loc_EFB7C les     bx, [CONTX_OwnStack_Ps]           
    EMM_Map_CONTXXX__WIP+E4      mov     [word ptr CONTX_OwnStack_Ps+2], dx





















## AI_RallyEnRouteCount
count of units enroute the continent stage point
    (_UNITS[itr_units].dst_wx == AI_Continent_X_Ptr[landmass_idx])
    (_UNITS[itr_units].dst_wy == AI_Continent_Y_Ptr[landmass_idx])




## CONTX_OwnStack_ULsts[80]
80 whats?

What is up with this indexing?

~ CONTX_OwnStack_ULsts[Stack_Index][CONTX_OwnStack_UCnt[Stack_Index]]

mov     cl, 2
shl     bx, cl
Stack_Index * 4?
sizeof() for Far Pointer?

like s_STACK?

// sizeof: 04h  4d
struct s_STACK
{
    /* 00 */ int16_t unit_idx;
    /* 02 */ int16_t active;  // AKA Can_Move  if us_Purify then ~busy
    /* 04 */
};

e.g.,
    CONTX_Stack_Index gets passed in
    UnitList_Count = CONTX_OwnStack_UCnt[CONTX_Stack_Index]
    ...count of units in stack
    ...now, iter over it and index stack
    Unit_Index = CONTX_OwnStack_ULsts[CONTX_Stack_Index][CX_UL_ID]

CONTX_OwnStack_ULsts[Stack_Index][CONTX_OwnStack_UCnt[Stack_Index]] = itr_units;


loc_EEE85:
les     bx, [CONTX_OwnStack_UCnt@]
add     bx, [bp+Stack_Index]
mov     al, [es:bx]
cbw
shl     ax, 1
mov     bx, [bp+Stack_Index]
mov     cl, 2
shl     bx, cl
les     bx, [CONTX_OwnStack_ULsts+bx]   ; 2 segment increments each (32 bytes data space)
                                        ; each holds a list of unit indices or -1s for units
                                        ; that have already been assigned orders
add     bx, ax
mov     [es:bx], _SI_itr_stacks1__itr_units



les     bx, [CONTX_OwnStack_UCnt@]
add     bx, [bp+CONTX_Stack_Index]
mov     al, [es:bx]
cbw
mov     [bp+UnitList_Count], ax

mov     [bp+CX_UL_ID], 0

mov     ax, [bp+CX_UL_ID]
cmp     ax, [bp+UnitList_Count]
jge     short loc_F0D56

loc_F09F8:
mov     bx, [bp+CONTX_Stack_Index]
mov     cl, 2
shl     bx, cl
les     bx, [CONTX_OwnStack_ULsts+bx]   ; 2 segment increments each (32 bytes data space)
                                        ; each holds a list of unit indices or -1s for units
                                        ; that have already been assigned orders
mov     ax, [bp+CX_UL_ID]
shl     ax, 1
add     bx, ax
mov     ax, [es:bx]
mov     [bp+Unit_Index], ax



// mov     ax, _SI_itr
// shl     ax, 1
// mov     dx, [EMM_PageFrame]             ; contains the segment address of the EMS page frame
// add     dx, ax
// add     dx, 3200
// push    dx                              ; addr
// call    SA_MK_FP0
// pop     cx
// mov     bx, _SI_itr
// mov     cl, 2
// shl     bx, cl
// mov     [word ptr (CONTX_OwnStack_ULsts+2)+bx], dx ; 2 segment increments each (32 bytes data space)
//                                         ; each holds a list of unit indices or -1s for units
//                                         ; that have already been assigned orders
// mov     [word ptr CONTX_OwnStack_ULsts+bx], ax ; 2 segment increments each (32 bytes data space)
//                                         ; each holds a list of unit indices or -1s for units
//                                         ; that have already been assigned orders
// inc     _SI_itr

    for(itr = 0; itr < 80; itr++)
    {

        CONTX_OwnStack_ULsts[itr] = (int16_t *)(&EMM_PageFrame[0] + (3200 * SZ_PARAGRAPH_B) + (itr * SZ_PARAGRAPH_B));
        
    }






// allocated in AI_Turn_NearAllocs()


AI_Own_City_Values       = Near_Allocate_First(200);
AI_Enemy_City_Values     = Near_Allocate_Next(200);
AI_Own_Garr_Strs         = Near_Allocate_Next(200);
AI_NME_Garrison_Strs     = Near_Allocate_Next(200);
AI_Own_Stacks            = Near_Allocate_Next(800);
AI_Enemy_Stacks          = Near_Allocate_Next(1440);
AI_OVL_Spell_Cats        = Near_Allocate_Next(92);
CRP_AI_OVL_SpellList     = Near_Allocate_Next(50);
AI_Arc_Cont_Own_Str      = Near_Allocate_Next(120);
AI_Myr_Cont_Own_Str      = Near_Allocate_Next(120);
CRP_AI_Arc_Cont_Nme_Str  = Near_Allocate_Next(120);
CRP_AI_Myr_Cont_Nme_Str  = Near_Allocate_Next(120);
AI_Arc_Cont_Own_Val      = Near_Allocate_Next(120);
AI_Myr_Cont_Own_Val      = Near_Allocate_Next(120);
AI_Arc_Cont_Nme_Val      = Near_Allocate_Next(120);
AI_Myr_Cont_Nme_Val      = Near_Allocate_Next(120);
AI_Arc_ContBalances = CRP_AI_Arc_Cont_Nme_Str;
AI_Myr_ContBalances = CRP_AI_Myr_Cont_Nme_Str;

// ; redundant, should not be the primary pointer
// WZD dseg:90FE CRP_AI_Arc_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+2Cr ...
// ; redundant, should not be the primary pointer
// WZD dseg:9100 CRP_AI_Myr_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+3Ar ...
// WZD dseg:9102 AI_Arc_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+10r ...
// WZD dseg:9104 AI_Myr_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+1Er ...
// WZD dseg:9106 AI_Enemy_Stacks@ dw 0                   ; DATA XREF: AI_Continent_Eval+121r ...
// WZD dseg:9108 AI_Own_Stacks@ dw 0                     ; DATA XREF: AI_Continent_Eval+8Er ...
// WZD dseg:910A AI_Enemy_Stack_Count dw 0               ; DATA XREF: AI_Continent_Eval:loc_D2593r ...
// WZD dseg:910C AI_Own_Stack_Count dw 0                 ; DATA XREF: AI_Continent_Eval:loc_D24FFr ...
// WZD dseg:910E AI_AtWarWith_Players dw 6 dup(0)        ; DATA XREF: AI_Count_Active_Wars+43w ...
// WZD dseg:911A AI_Active_War_Count dw 0                ; DATA XREF: AI_Count_Active_Wars+7w ...
// ; redundant - filled out but never used
// WZD dseg:911C CRP_AI_OVL_SpellList dw 0               ; DATA XREF: AI_OVL_GetSpellList+DBr ...
// WZD dseg:911E CRP_AI_OVL_SpellCount dw 0              ; DATA XREF: AI_OVL_GetSpellList+8w ...
// WZD dseg:9120 AI_OVL_Spell_Cats dw 0                  ; DATA XREF: AI_OVL_SplCat_Picker:loc_E4471r ...
// WZD dseg:9122 AI_NME_Garrison_Strs dw 0               ; DATA XREF: AI_OVL_Eval_Assets+15r ...
// WZD dseg:9124 AI_Own_Garr_Strs dw 0                   ; DATA XREF: AI_OVL_Eval_Assets+24r ...
// WZD dseg:9126 AI_Enemy_City_Values dw 0               ; DATA XREF: AI_Continent_Eval+36Fr ...
// WZD dseg:9128 AI_Own_City_Values dw 0                 ; DATA XREF: AI_Continent_Eval+351r ...
