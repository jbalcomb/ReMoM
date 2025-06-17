
/*






Combat Terrain

battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = CTG_Grass;



*/


#define MAX_ENTITIES  256


/*
    ~ screen coordinates
*/
// Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, 163);
#define COMBAT_MAP_YMAX 163
// Combat_Screen_Assign_Mouse_Images() does screen_y <= 168 + 4 (pointer_offset)
// ... Vortex Move Screen does screen_y <= 168 + 4 (pointer_offset)

// hard-coded in CMB_SpawnFigure__WIP() in call to CMB_CreateEntity__WIP()
// 13, /* draw_x_shift */
// 23, /* draw_y_shift */
// 1,  /* entity_type: battle unit figure */

// CMB_ComposeBackgrnd__WIP()
//     FLIC_Draw(0, 164, combat_background_bottom);



/*
    ~ combat grid coordinates

(battlefield->Central_Structure == CS_SorceryNode)
(battlefield->Central_Structure == CS_ChaosNode)
(battlefield->Central_Structure == CS_Outpost)
&&
(cgx == 6)
&&
(cgy == 11)

(battlefield->Central_Structure == CS_City)
(battlefield->Central_Structure == CS_Fortress)
&&
(cgx == 8)
&&
(cgy == 13)


Magic Vortex
    if(
        (next_cgx >= 5)
        &&
        (next_cgx <= 8)
        &&
        (next_cgy >= 10)
        &&
        (next_cgy <= 13)
    )
    {
        CMB_CityDamage += 5;
    }
*/
#define MIN_CGX_CITY  5
#define MAX_CGX_CITY  8
#define MIN_CGY_CITY 10
#define MAX_CGY_CITY 13


/*
    Combat Terrain

{ 0: CTG_Grass, 1: CTG_Rough, 2: CTG_Dirt, 3: CTG_River, 4: CTG_DeepWater, 5: CTG_IDK }
*/
#define COMBAT_TERRAIN_GROUP()          battlefield->terrain_group[((cgy * COMBAT_GRID_WIDTH) + cgx)]
#define GET_COMBAT_TERRAIN_GROUP(_cgx_,_cgy_)                                               \
    do {                                                                                    \
        battlefield->terrain_group[(((_cgy_) * COMBAT_GRID_WIDTH) + (_cgx_))];              \
    } while(0)
#define SET_COMBAT_TERRAIN_GROUP(_cgx_,_cgy_,_terrain_)                                     \
    do {                                                                                    \
        battlefield->terrain_group[(((_cgy_) * COMBAT_GRID_WIDTH) + (_cgx_))] = (_terrain); \
    } while(0)



/*
Cast_Spell_On_Battle_Unit()

    if(spell_idx == spl_Disrupt)
        battlefield->walls[(((target_cgy - 10) * 3) + (target_cgx - 5))] = 2;
*/



/*
CMB_SpawnFigure__WIP()
    no battle_unit_idx, passed in cgx, cgy, target_cgx, target_cgy

[x] Combat_Figure_Compose_USEFULL()
[x] BU_CreateImage__SEGRAX()
[x] CMB_SpawnFigure__WIP()

figure_set_idx passed to USELESS_Combat_Figure_Load_Compose()

*/

#define BATTLE_UNIT_FACING_DRECTION {                               \
    /* cgx = battle_units[battle_unit_idx].cgx; */                  \
    /* cgy = battle_units[battle_unit_idx].cgy; */                  \
    /* target_cgx = battle_units[battle_unit_idx].target_cgx; */    \
    /* target_cgy = battle_units[battle_unit_idx].target_cgy; */    \
    if(cgx > target_cgx)            \
    {                               \
        if(cgy > target_cgy)        \
        {                           \
            figure_set_idx = 0;     \
        }                           \
        else if(cgy < target_cgy)   \
        {                           \
            figure_set_idx = 6;     \
        }                           \
        else                        \
        {                           \
            figure_set_idx = 7;     \
        }                           \
    }                               \
    else if(cgx < target_cgx)       \
    {                               \
        if(cgy > target_cgy)        \
        {                           \
            figure_set_idx = 2;     \
        }                           \
        else if(cgy < target_cgy)   \
        {                           \
            figure_set_idx = 4;     \
        }                           \
        else                        \
        {                           \
            figure_set_idx = 3;     \
        }                           \
    }                               \
    else                            \
    {                               \
        if(cgy > target_cgy)        \
        {                           \
            figure_set_idx = 1;     \
        }                           \
        else if(cgy < target_cgy)   \
        {                           \
            figure_set_idx = 5;     \
        }                           \
        else                        \
        {                           \
            figure_set_idx = 2;     \
        }                           \
    }                               \
}


/*
    CMB_SpawnFigure__WIP()
    Battle_Unit_Figure_Position()
    Unit_Figure_Position()
!!!!! ALL DIFFERENT !!!!!
this is as-seen in CMB_SpawnFigure__WIP()
*/
#define BATTLE_UNIT_FIGURE_POSITION {                       \
    switch(figure_count)                                    \
    {                                                       \
        case 1:                                             \
        {                                                   \
            fig_x = 1;                                      \
            fig_y = 8;                                      \
        } break;                                            \
        case 2:                                             \
        {                                                   \
            switch(current_figure)                          \
            {                                               \
                case 0:  { fig_x = -7; fig_y =  9; } break; \
                default: { fig_x =  7; fig_y =  9; } break; \
            }   \
        } break;    \
        case 3: \
        {   \
            switch(current_figure)  \
            {   \
                case 0:  { fig_x =  0; fig_y =  4; } break; \
                case 1:  { fig_x = -6; fig_y = 10; } break; \
                default: { fig_x =  7; fig_y = 10; } break; \
            }   \
        } break;    \
        case 4: \
        {   \
            switch(current_figure)  \
            {   \
                case 0:  { fig_x =  1; fig_y =  4; } break; \
                case 1:  { fig_x = -7; fig_y =  8; } break; \
                case 2:  { fig_x =  8; fig_y =  8; } break; \
                default: { fig_x =  1; fig_y = 11; } break; \
            }   \
        } break;    \
        case 5: \
        {   \
            switch(current_figure)  \
            {   \
                case 0:  { fig_x =  1; fig_y =  4; } break; \
                case 1:  { fig_x = -7; fig_y =  8; } break; \
                case 2:  { fig_x =  1; fig_y =  8; } break; \
                case 3:  { fig_x =  8; fig_y =  8; } break; \
                default: { fig_x =  1; fig_y = 11; } break; \
            }   \
        } break;    \
        case 6: \
        {   \
            switch(current_figure)  \
            {   \
                case 0:  { fig_x =  1; fig_y =  4; } break;     \
                case 1:  { fig_x =  4; fig_y =  7; } break;     \
                case 2:  { fig_x = -8; fig_y =  8; } break;     \
                case 3:  { fig_x =  9; fig_y =  8; } break;     \
                case 4:  { fig_x = -3; fig_y =  9; } break;     \
                default: { fig_x =  1; fig_y = 11; } break;     \
            }   \
        } break;    \
        case 7: \
        {   \
            switch(current_figure)  \
            {   \
                case 0:  { fig_x =  1; fig_y =  4; } break;     \
                case 1:  { fig_x =  6; fig_y =  6; } break;     \
                case 2:  { fig_x = -8; fig_y =  8; } break;     \
                case 3:  { fig_x =  1; fig_y =  8; } break;     \
                case 4:  { fig_x = 10; fig_y =  8; } break;     \
                case 5:  { fig_x = -3; fig_y = 11; } break;     \
                default: { fig_x =  1; fig_y = 11; } break;     \
            }   \
        } break;    \
        case 8: \
        {   \
            switch(current_figure)  \
            {   \
                case 0:  { fig_x =   1; fig_y =  4; } break;    \
                case 1:  { fig_x =   6; fig_y =  6; } break;    \
                case 2:  { fig_x =  -2; fig_y =  7; } break;    \
                case 3:  { fig_x =  -8; fig_y =  8; } break;    \
                case 4:  { fig_x =  10; fig_y =  8; } break;    \
                case 5:  { fig_x =   3; fig_y =  9; } break;    \
                case 6:  { fig_x =  -3; fig_y = 11; } break;    \
                default: { fig_x =   1; fig_y = 11; } break;    \
            }   \
        } break;    \
    }   \
}



/*
Cast_Spell_On_Battle_Unit()

                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                BU_Init_Battle_Unit(&battle_units[target_idx]);
                BU_Apply_Battlefield_Effects__WIP(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = Moves_Left;
                }

*/