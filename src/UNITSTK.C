
/*
    WIZARDS.EXE
        ovr061

MoO2
    Module: SHIPSTK
    Module: SHIPMOVE

*/

#include "MoM.H"
#include "UNITSTK.H"
#include "MainScr.H"



/*
    WIZARDS.EXE  ovr061
*/

// WZD o61p01
// AKA IDK_ActiveUnitStack_MovesOrPath_s53150()
/*
    Handles a left-click on a Unit in the Unit Window
    
    Mostly Just:
        _unit_stack[stack_idx].active = ST_FALSE || ST_TRUE

    May
        _active_stack_has_path = ST_FALSE;
        _UNITS[stack_unit_idx].Status = us_Ready;
        _UNITS[stack_unit_idx].Finished = ST_FALSE;
        _UNITS[stack_unit_idx].Rd_Constr_Left = ST_UNDEFINED;
        _UNITS[stack_unit_idx].Finished = ST_TRUE;
        all_units_moved = ST_FALSE;
*/
void Update_Stack_Active(int16_t stack_idx)
{
    int16_t DEPR_Local_Flag;
    int16_t itr_unit_stack_count;
    int16_t active_unit_stack_count;
// stack_idx= word ptr  6

    int16_t stack_unit_idx;
    int16_t itr_stack;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Stack_Active()\n", __FILE__, __LINE__);
#endif

    stack_unit_idx = _unit_stack[stack_idx].unit_idx;

    if(
        (_UNITS[stack_unit_idx].Status != us_ReachedDest)  /* "DONE" */ &&
        (_UNITS[stack_unit_idx].moves2 >= 1)
    )
    {

        if(
            (reset_active_stack == ST_TRUE) &&
            (_unit_stack_count > 1) &&
            (_UNITS[stack_unit_idx].Status != us_Casting)
        )
        {
            active_unit_stack_count = 0;
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(_unit_stack[itr_stack].active == ST_TRUE)
                {
                    active_unit_stack_count++;
                }
            }

            if(active_unit_stack_count > 1)
            {
                for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                {
                    _unit_stack[itr_stack].active = ST_FALSE;
                }
            }


        }

        reset_active_stack = ST_FALSE;
        stack_unit_idx = _unit_stack[stack_idx].unit_idx;

        if(_unit_stack[stack_idx].active == ST_FALSE)
        {
            if(_UNITS[stack_unit_idx].Status > 0 /* us_Ready */)
            {
                if(_UNITS[stack_unit_idx].moves2 > 0)
                {
                    DEPR_Local_Flag = ST_TRUE;

                    if(DEPR_Local_Flag == ST_TRUE)
                    {
                        _unit_stack[stack_idx].active = ST_TRUE;

                        if(_UNITS[stack_unit_idx].Status == us_GOTO)
                        {
                            _active_stack_has_path = ST_FALSE;
                        }

                        _UNITS[stack_unit_idx].Status = us_Ready;
                        _UNITS[stack_unit_idx].Finished = ST_FALSE;
                        _UNITS[stack_unit_idx].Rd_Constr_Left = ST_UNDEFINED;

                        if(_UNITS[stack_unit_idx].moves2 >= 1)
                        {
                            all_units_moved = ST_FALSE;
                            Reset_Draw_Active_Stack(); 
                        }
                        else
                        {
                            _unit_stack[stack_idx].active = ST_FALSE;
                            _UNITS[stack_unit_idx].Finished = ST_TRUE;
                        }

                    }
                }
            }
            else
            {
                if(_UNITS[stack_unit_idx].moves2 <= 0)
                {
                    _unit_stack[stack_idx].active = ST_FALSE;
                }
                else
                {
                    _unit_stack[stack_idx].active = ST_TRUE;
                }
            }
        }
        else
        {
            _unit_stack[stack_idx].active = ST_FALSE;
        }



        if(_UNITS[stack_unit_idx].Finished == ST_TRUE)
        {
            _unit_stack[stack_idx].active = ST_FALSE;
        }

        Stack_Moves_Active();
        Set_Unit_Action_Special();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Stack_Active()\n", __FILE__, __LINE__);
#endif
}


// WZD o61p02
// WZD o61p03
// WZD o61p04
// WZD o61p05
// WZD o61p06
// WZD o61p07
// WZD o61p08
// WZD o61p09
// WZD o61p10
// WZD o61p11
// WZD o61p12
// WZD o61p13
// WZD o61p14
// WZD o61p15
// WZD o61p16

// WZD o61p02
// Move_Stack()

// WZD o61p03
// RdBd_UNIT_MoveStack_WIP()

// WZD o61p04
// WIZ_NextIdleStack()

// WZD o61p05
// Next_Unit_Nearest_Available()

// WZD o61p06
// Set_Unit_Action_Special()

// WZD o61p07
// Active_Unit_Stack()

// WZD o61p08
// Build_Moveable_Stack()

// WZD o61p09
// Build_RoadBuilder_Stack()

// WZD o61p10
// STK_BuildingPossible()

// WZD o61p11
// Unit_Action_Special_Purify()

// WZD o61p12
// STK_MeldingPossible()

// WZD o61p13
// STK_MeldWithNode()

// WZD o61p14
// STK_DoMeldWithNode()

// WZD o61p15
/*
; progress the purification counters of all purifying units, cleansing any tiles where the treshold has been reached
; WARNING: the Target_X field may not be the best place to hold this information?
*/
void Next_Turn_Process_Purify(void)
{
    int16_t troops[MAX_STACK];
    int16_t unit_owner;
    int16_t Purify_Progress;
    int16_t troop_count;
    int16_t unit_wp;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t itr_units;  // _SI_
    int16_t time_stop_flag;  // _DI_
    int16_t itr_troops;  // _DI_
    int16_t unit_idx;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            time_stop_flag = ST_FALSE;
            if(
                (g_TimeStop_PlayerNum > 0)
                &&
                ((_UNITS[itr_units].owner_idx + 1) != g_TimeStop_PlayerNum)
            )
            {
                time_stop_flag = ST_TRUE;
            }

            if(time_stop_flag != ST_TRUE)
            {
                if(_UNITS[itr_units].Status == us_Purify)
                {
                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_PURIFY) != 0)
                    {
                        _UNITS[itr_units].Status = us_PurifyDone;
                    }
                    else
                    {
                        _UNITS[itr_units].Status = us_Ready;
                    }
                }
            }
        }
    }


    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            if(_UNITS[itr_units].Status == us_PurifyDone)
            {
                unit_wx = _UNITS[itr_units].wx;
                unit_wy = _UNITS[itr_units].wy;
                unit_wp = _UNITS[itr_units].wp;
                unit_owner = _UNITS[itr_units].owner_idx;

                Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner, &troop_count, &troops[0]);

                Purify_Progress = 0;

                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {
                    if(_UNITS[troops[itr_troops]].Status == us_PurifyDone)
                    {
                        Purify_Progress += _UNITS[troops[itr_troops]].dst_wx;
                        _UNITS[troops[itr_troops]].dst_wx += 1;
                        _UNITS[troops[itr_troops]].Status = us_Purify;
                    }
                }

                if(Purify_Progress >= 4)
                {

                    Map_Square_Clear_Corruption(_UNITS[troops[0]].wx, _UNITS[troops[0]].wy, _UNITS[troops[0]].wp);
                    // unit_idx = troops[(itr_troops - 1)];
                    // unit_wx = _UNITS[unit_idx].wx;
                    // unit_wy = _UNITS[unit_idx].wy;
                    // unit_wp = _UNITS[unit_idx].wp;
                    // Map_Square_Clear_Corruption(unit_wx, unit_wy, unit_wp);

                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {
                        if(_UNITS[troops[itr_troops]].Status == us_Purify)
                        {
                            _UNITS[troops[itr_troops]].dst_wx = 0;
                            _UNITS[troops[itr_troops]].Status = us_Ready;
                            _UNITS[troops[itr_troops]].Finished = ST_TRUE;
                        }
                    }
                }
            }
        }
    }


    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            if(_UNITS[itr_units].Status == us_PurifyDone)
            {
                _UNITS[itr_units].Status = us_Purify;
            }
        }
    }

}


// WZD o61p16
// Any_Units_Not_Busy()
