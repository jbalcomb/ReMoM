
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
        OVL_StackHasPath = ST_FALSE;
        _UNITS[stack_unit_idx].Status = US_Ready;
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
        (_UNITS[stack_unit_idx].Status != US_ReachedDest)  /* "DONE" */ &&
        (_UNITS[stack_unit_idx].HMoves >= 1)
    )
    {

        if(
            (reset_active_stack == ST_TRUE) &&
            (_unit_stack_count > 1) &&
            (_UNITS[stack_unit_idx].Status != US_Casting)
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
            if(_UNITS[stack_unit_idx].Status > 0 /* US_Ready */)
            {
                if(_UNITS[stack_unit_idx].HMoves > 0)
                {
                    DEPR_Local_Flag = ST_TRUE;

                    if(DEPR_Local_Flag == ST_TRUE)
                    {
                        _unit_stack[stack_idx].active = ST_TRUE;

                        if(_UNITS[stack_unit_idx].Status == US_GoingTo)
                        {
                            OVL_StackHasPath = ST_FALSE;
                        }

                        _UNITS[stack_unit_idx].Status = US_Ready;
                        _UNITS[stack_unit_idx].Finished = ST_FALSE;
                        _UNITS[stack_unit_idx].Rd_Constr_Left = ST_UNDEFINED;

                        if(_UNITS[stack_unit_idx].HMoves >= 1)
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
                if(_UNITS[stack_unit_idx].HMoves <= 0)
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
        STK_GetExtraActions__WIP();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Stack_Active()\n", __FILE__, __LINE__);
#endif
}
