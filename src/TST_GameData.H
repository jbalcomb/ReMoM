#ifndef TST_GAMEDATA_H
#define TST_GAMEDATA_H

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

/*

    ¿ Config ?
    ¿ Settings ?

    ¿ New Game - Wizards, Difficulty, Magic Strength, Land Size ?
    ¿ Per Wizard - Race, Banner Color, Wizard/Wizard Portrait, Name ?
    ¿ Retorts ?
    ¿ Books ?


    World Map

    Terrain Specials

    Unexplored Area


    Units
    Cities

    Heroes

    Fortresses

    Towers

    Unit Type Table


    _unit
    _unit_stack
    _unit_stack_count

    entities_on_map


    Units
        world_x, world_y, world_p
        draw_order
    

*/
/*
    Heroes - Per Wizard - 12*35=420 Bytes
    p0_heroes
    p1_heroes
    p2_heroes
    p3_heroes
    p4_heroes
    p5_heroes

    _num_players
    _landsize
    _magic
    _difficulty
    _cities
    _units
    _turn
    _unit
    _players
    _world_maps
    UU_TBL_1
    UU_TBL_2
    TBL_Landmasses
    _NODES
    _FORTRESSES
    _TOWERS
    TBL_Encounters
    TBL_Items
    _CITIES
    _UNITS
    TBL_Terr_Specials
    TBL_Scouting
    movement_mode_cost_maps
    _events_table, 
    TBL_Terrain_Flags
    grand_vizier
    TBL_Premade_Items

    fread(p0_heroes, 12, 35, file_pointer);
    fread(p1_heroes, 12, 35, file_pointer);
    fread(p2_heroes, 12, 35, file_pointer);
    fread(p3_heroes, 12, 35, file_pointer);
    fread(p4_heroes, 12, 35, file_pointer);
    fread(p5_heroes, 12, 35, file_pointer);
    fread(&_num_players, 1, 2, file_pointer);
    fread(&_landsize, 1, 2, file_pointer);
    fread(&_magic, 1, 2, file_pointer);
    fread(&_difficulty, 1, 2, file_pointer);
    fread(&_cities, 1, 2, file_pointer);
    fread(&_units, 1, 2, file_pointer);
    fread(&_turn, 1, 2, file_pointer);
    fread(&_unit, 1, 2, file_pointer);
    fread(_players, 6, 1224, file_pointer);
    fread(_world_maps, 2, 4800, file_pointer);
    fread(UU_TBL_1, 2, 96, file_pointer);
    fread(UU_TBL_2, 2, 96, file_pointer);
    fread(TBL_Landmasses, 2, 2400, file_pointer);
    fread(_NODES, 30, 48, file_pointer);
    fread(_FORTRESSES, 6, 4, file_pointer);
    fread(_TOWERS, 6, 4, file_pointer);
    fread(TBL_Encounters, 102, 24, file_pointer);
    fread(TBL_Items, 138, 50, file_pointer);
    fread(_CITIES, 100, 114, file_pointer);
    fread(_UNITS, 1009, 32, file_pointer);
    fread(TBL_Terr_Specials, 2, 2400, file_pointer);
    fread(TBL_Scouting, 2, 2400, file_pointer);
    fread(movement_mode_cost_maps, 2, 14400, file_pointer);
    fread(_events_table, 1, 100, file_pointer);
    fread(TBL_Terrain_Flags, 2, 2400, file_pointer);
    fread(&grand_vizier, 1, 2, file_pointer);
    fread(TBL_Premade_Items, 250, 1, file_pointer);

*/

struct s_MOM_GAME_DATA {


    /*
        SAVE_GAM    
    */
   
    // p0_heroes
    SAMB_ptr p0_heroes;
    // p1_heroes
    SAMB_ptr p1_heroes;
    // p2_heroes
    SAMB_ptr p2_heroes;
    // p3_heroes
    SAMB_ptr p3_heroes;
    // p4_heroes
    SAMB_ptr p4_heroes;
    // p5_heroes
    SAMB_ptr p5_heroes;

    // _num_players
    // _landsize
    // _magic
    // _difficulty
    // _cities
    // _units
    // _turn
    // _unit
    // _players

    // _world_maps
    // UU_TBL_1
    // UU_TBL_2
    // TBL_Landmasses
    // _NODES
    // _FORTRESSES
    // _TOWERS
    // TBL_Encounters
    // TBL_Items
    // _CITIES
    // _UNITS
    // TBL_Terr_Specials
    // TBL_Scouting
    // movement_mode_cost_maps
    // _events_table, 
    // TBL_Terrain_Flags
    // grand_vizier
    // TBL_Premade_Items

};


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Export_GameData(void);


#ifdef __cplusplus
}
#endif

#endif  /* TST_GAMEDATA_H */
