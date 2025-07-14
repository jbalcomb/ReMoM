/*
    CMB_BaseAllocs__WIP();
    CMB_LoadResources__WIP();
        |-> CMB_SetNearAllocs__WIP();

    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_attacker_player = combat_attacker_player_idx;
    _combat_defender_player = combat_defender_player_idx;

    Cache_Graphics_Combat();

    CMB_Terrain_Init__WIP(wx, wy, wp);


    Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);
    if(_combat_attacker_player == _human_player_idx)
        Switch_Active_Battle_Unit(0);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));

    CMB_PrepareTurn__WIP();

    Assign_Combat_Grids();

BU_UnitLoadToBattle__SEGRAX()
*/

#include "CMBTTSTU.h"

#include "MOX/Allocate.h"
#include "MOX/EMM.h"
#include "MOX/FLIC_Draw.h"
#include "MOX/Graphics.h"
#include "MOX/MOX_TYPE.h"

#include "CMBTDEF.h"
#include "Combat.h"
#include "MOX/Video.h"



// WZD dseg:C550
extern SAMB_ptr battle_unit_picts_seg[(2 * MAX_STACK)];
// WZD dseg:D15E
extern int16_t combat_grid_entity_count;
// WZD dseg:D280
extern uint8_t combat_grid_entities_draw_order[MAX_ENTITIES];


/*
    Goal(s):
        battle units can be drawn

Depends on:
    _combat_wx
    _combat_wy
    _combat_wp
    _combat_defender_player

~ CMB_Units_Init__WIP()

start from attacker unit stack
find defender units

*/
int16_t Prepare_All_Battle_Units(int16_t attacker_troop_count, int16_t attacker_troops[])
{
    int16_t itr = 0;
    int16_t defender_troop_count = 0;

    _combat_total_unit_count = 0;
    for(itr = 0; itr < attacker_troop_count; itr++)
    {
        Load_Battle_Unit(attacker_troops[itr], &battle_units[_combat_total_unit_count]);
        battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[attacker_troops[itr]].type, itr);
        _combat_total_unit_count++;
    }

    for(itr = 0; itr < _units; itr++)
    {
        if(
            (_UNITS[itr].wx == _combat_wx)
            &&
            (_UNITS[itr].wy == _combat_wy)
            &&
            (_UNITS[itr].wp == _combat_wp)
            &&
            (_UNITS[itr].owner_idx == _combat_defender_player)
        )
        {
            Load_Battle_Unit(itr, &battle_units[_combat_total_unit_count]);
            battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[itr].type, _combat_total_unit_count);
            defender_troop_count++;
            _combat_total_unit_count++;
        }
    }

    Deploy_Battle_Units(_combat_attacker_player);
    Deploy_Battle_Units(_combat_defender_player);

    return defender_troop_count;
}

/*
~ CMB_CreateEntity__WIP()
*/
void Create_Entity(int16_t draw_x, int16_t draw_y, int64_t pict, int16_t draw_x_shift, int16_t draw_y_shift, int16_t frame_num, int16_t entity_type, int16_t owner_idx)
{
    int16_t cgy_ofst = 0;
    int16_t cgx_ofst = 0;
    int16_t cgy = 0;
    int16_t cgx = 0;
    struct s_COMBAT_ENTITY * entt;

    entt = &combat_grid_entities[combat_grid_entity_count];

    entt->draw_x = draw_x;
    entt->draw_y = draw_y;
    entt->seg_or_idx = pict;
    entt->frame_num = frame_num;
    entt->draw_x_shift = draw_x_shift;
    entt->draw_y_shift = draw_y_shift;
    entt->entity_type = entity_type;
    entt->owner_idx = owner_idx;

    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &cgx, &cgx_ofst);
    Screen_To_Combat_Grid_Cell_Y_And_Offset(draw_x, draw_y, &cgy, &cgy_ofst);
    entt->draw_order_value = ((cgy * 8000) + (cgx * 320) + (cgy_ofst * 16) + cgx_ofst);
    Set_Entity_Draw_Order();

    combat_grid_entity_count++;

}

/*
    battle unit specific wrapper for Create_Entity()
~ CMB_SpawnFigure__WIP()
CMB_SpawnFigure__WIP(battle_units[itr].bufpi, battle_units[itr].cgx, battle_units[itr].cgy, battle_units[itr].target_cgx, battle_units[itr].target_cgy, battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);
*/
void Create_Battle_Unit_Entity(int64_t bufpi, int16_t cgx, int16_t cgy, int16_t target_cgx, int16_t target_cgy, int16_t MoveStage, int16_t current_figure, int16_t figure_count, int16_t controller_idx)
{
    int16_t position_screen_x = 0;
    int16_t position_screen_y = 0;
    int16_t target_screen_x = 0;
    int16_t target_screen_y = 0;
    int16_t fig_x = 0;
    int16_t fig_y = 0;
    int16_t draw_x = 0;
    int16_t draw_y = 0;

    Combat_Grid_Screen_Coordinates(cgx, cgy, 0, 0, &position_screen_x, &position_screen_y);
    Combat_Grid_Screen_Coordinates(target_cgx, target_cgy, 0, 0, &target_screen_x, &target_screen_y);

    BATTLE_UNIT_FIGURE_POSITION

    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);
    draw_y = (((((target_screen_y - position_screen_y) * MoveStage) / 8) + position_screen_y) + fig_y);

    Create_Entity(
        draw_x, 
        draw_y, 
        bufpi, 
        13, /* draw_x_shift */
        23, /* draw_y_shift */
        MoveStage,
        1,  /* entity_type: battle unit figure */
        controller_idx
    );

}

/*
    create combat grid entity, per battle unit figure
~ CMB_CreateEntities__WIP()
*/
void Make_Combat_Grid_Battle_Unit_Entities(void)
{
    int16_t itr = 0;
    int16_t fig_cnt = 0;
    int16_t fig_max = 0;
    int16_t cur_fig = 0;
    Clear_Combat_Grid_Entities();
    /*
        BEGIN:  Battle Unit Figure Entities
    */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        fig_cnt = battle_units[itr].Cur_Figures;
        fig_max = battle_units[itr].Max_Figures;
        for(cur_fig = 0; cur_fig < fig_cnt; cur_fig++)
        {
            Create_Battle_Unit_Entity(
                battle_units[itr].bufpi,
                battle_units[itr].cgx,
                battle_units[itr].cgy,
                battle_units[itr].target_cgx,
                battle_units[itr].target_cgy,
                battle_units[itr].MoveStage,
                cur_fig,
                fig_max,
                battle_units[itr].controller_idx
            );
        }
    }
    /*
        END:  Battle Unit Figure Entities
    */
}

/*
~ Combat_Figure_Compose_USEFULL()
*/
void Compose_Battle_Unit_Pictures(void)
{
    SAMB_ptr temp_GfxBuf_2400B = 0;
    int16_t battle_unit_idx = 0;
    int16_t cgx = 0;
    int16_t cgy = 0;
    int16_t target_cgx = 0;
    int16_t target_cgy = 0;
    int16_t bufpi = 0;
    int16_t offset = 0;
    SAMB_ptr * figure_pict_set_seg = 0;
    int16_t figure_set_idx = 0;
    int16_t frame_num = 0;

    temp_GfxBuf_2400B = GfxBuf_2400B;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        bufpi = battle_units[battle_unit_idx].bufpi;

        frame_num = 1;

        cgx = battle_units[battle_unit_idx].cgx;
        cgy = battle_units[battle_unit_idx].cgy;
        target_cgx = battle_units[battle_unit_idx].target_cgx;
        target_cgy = battle_units[battle_unit_idx].target_cgy;
        BATTLE_UNIT_FACING_DRECTION

        FIGUREX_MAP
        FIGUREX_OFFSET
        FIGUREX_POINTER

        Set_Animation_Frame(figure_pict_set_seg[figure_set_idx], frame_num);
        GfxBuf_2400B = battle_unit_picts_seg[battle_units[battle_unit_idx].bufpi];
        Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], GfxBuf_2400B);

    }

    GfxBuf_2400B = temp_GfxBuf_2400B;

}

/*
~ CMB_DrawMap__WIP() |->
*/
void Draw_Combat_Grid_Battle_Unit_Entities(void)
{
    int16_t itr = 0;
    int16_t combat_grid_entity_idx = 0;
    struct s_COMBAT_ENTITY * entt;

    // BEGIN:  CMB_DrawMap__WIP()
    Copy_Back_To_Off();  // 'combat background' from Combat_Screen_Compose_Background()
    Compose_Battle_Unit_Pictures();
    Set_Page_Off();
    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, 163);
    // END:  CMB_DrawMap__WIP()

    // BEGIN:  CMB_DrawEntities__WIP()
    for(itr = 0; itr < combat_grid_entity_count; itr++)
    {
        combat_grid_entity_idx = combat_grid_entities_draw_order[itr];
        entt = &combat_grid_entities[combat_grid_entity_idx];
        Draw_Picture_Windowed((entt->draw_x - entt->draw_x_shift), (entt->draw_y - entt->draw_y_shift), battle_unit_picts_seg[entt->index]);
    }
    // END:  CMB_DrawEntities__WIP()

    // BEGIN:  CMB_DrawMap__WIP()
    Reset_Window();
    // END:  CMB_DrawMap__WIP()
}