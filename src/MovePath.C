/*
    WIZARDS.EXE
        ovr147
*/

#include "MoX.H"



// WZD ovr147:0000 01                                              
int8_t CS_Value_Change = ST_TRUE;
// WZD ovr147:0001                                                 algn_D5111:
// WZD ovr147:0001 00                                              align 2
// WZD ovr147:0002 00 00                                           
int16_t CS_Row_Start = 0;



// WZD o147p1  [1of1]  (OON)
// drake178: Overland_Pathfinder()
void Move_Path_Find_C(int16_t wx, int16_t wy, int8_t * movepath_cost_map)
{
    int16_t _AX_;
    int16_t _BX_;
    int16_t _CX_;
    int16_t _DX_;
    int16_t _DS_;
    int16_t _ES_;
    int16_t _SI_;
    int16_t _DI_;
    int16_t * _DS_SI_;
    int16_t * _ES_DI_;

    // mov     ax, [bp+wy]
    // mov     bx, e_WORLD_WIDTH
    // mul     bx
    // mov     [cs:CS_Row_Start], ax
    CS_Row_Start = (wy * WORLD_WIDTH);

    // add     ax, [bp+wx]
    // mov     dx, ax                          ; dx = tile index
    _DX_ = wx;

    // mov     ax, [word ptr bp+MoveMap@+2]
    // mov     ds, ax
    // mov     es, ax
    _DS_ = &movepath_cost_map;
    _ES_ = &movepath_cost_map;

    // mov     di, OVL_Path_Results.Reach_Costs
    _DI_ = 2400;

    // mov     cx, e_WORLD_SIZE_DD
    // mov     ax, -1
    // rep stosw
    _CX_ = 1200;  // WORLD_SIZE_DD
    _AX_ = -1;
    _ES_DI_ = (int16_t *)(_ES_ + _DI_);
    while(_CX_--)
    {
        *_ES_DI_++ = _AX_;
    }

    // mov     _DI_OVL_Path_Results_Reach_Costs, OVL_Path_Results.Reach_From
    // mov     cx, e_WORLD_SIZE_DW
    // mov     ax, 0
    // stosw
    // inc     ax
    // loop    loc_D5147
    _DI_    = 4800;
    _CX_    = 2400;  // WORLD_SIZE_DW
    _AX_    = 0;
    _ES_DI_ = (int16_t *)(_ES_ + _DI_);
    while(_CX_--)
    {
        *_ES_DI_++ = _AX_++;
    }

    // mov     _DI_OVL_Path_Results_Reach_Costs, _DX_wx
    // add     _DI_OVL_Path_Results_Reach_Costs, OVL_Path_Results.Reach_Costs
    // mov     ax, 0
    // stosb
    _DI_     =  _DX_;
    _DI_     += 2400;
    _AX_     =  0;
    _ES_DI_  =  (int16_t *)(_ES_ + _DI_);
    *_ES_DI_ =  _AX_;

    // loc_D5155:
    // mov     [cs:Value_Change], e_ST_FALSE
    // mov     si, 0
    // mov     bx, OVL_Path_Results.Reach_Costs
    // mov     _DI_OVL_Path_Results_Reach_Costs, OVL_Path_Results.Reach_From
    // add     si, [cs:CS_Row_Start]
    // add     bx, [cs:CS_Row_Start]
    // add     _DI_OVL_Path_Results_Reach_Costs, [cs:CS_Row_Start]
    // add     _DI_OVL_Path_Results_Reach_Costs, [cs:CS_Row_Start]
loc_D5155:
    CS_Value_Change = ST_FALSE;
    _SI_ = 0;
    _BX_ = 2400;
    _DI_ = 4800;
    _SI_ += CS_Row_Start;
    _BX_ += CS_Row_Start;
    _DI_ += CS_Row_Start;
    _DI_ += CS_Row_Start;

    // ...
    // ...
    // ...

loc_D554A:

    // ...
    // ...
    // ...

    // loc_D58EE:
    // inc     bx
    // inc     _DI_OVL_Path_Results_Reach_Costs
    // inc     _DI_OVL_Path_Results_Reach_Costs
    // sub     si, 120                         ; next row up
    // sub     bx, 120
    // sub     _DI_OVL_Path_Results_Reach_Costs, 240
    // cmp     ch, 0
    // jz      short loc_D5908

    // cmp     si, e_WORLD_WIDTH
    // js      short loc_D5908

    // jmp     loc_D554A

    // loc_D5908:
    // cmp     [cs:CS_Value_Change], e_ST_TRUE
    // jnz     short @@Done
loc_D5908:
    if(CS_Value_Change == ST_TRUE)
    {
        goto loc_D5155;
    }

}
