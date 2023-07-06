#ifndef TST_GAMEDATA_H
#define TST_GAMEDATA_H

#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

/*
    Entities on Movement-Map

    Game State: SAVETEST.GAM
    Map X,Y:  8,10
    Map Grid X,Y:  (0-11),(0-9)
    World Grid X,Y:  (8-19),(10-19)
    Grid Field X,y:  (0-239),(0-179)
    Screen X,Y:  (0-239),(20-199)

    uint8_t entities_on_map[(MAP_WIDTH*MAP_HEIGHT)]

    Entities: City, Unit

    _CITIES[]
    _UNITS[]

    YayNayMay: Inclusion Test ~ IsVisible, IsExplored

    world_plane == 0 ? Visibility_Arcanus : Visibility_Myrror
    Test_Bit_Field_(world_map_idx, Visibility_Arcanus)

*/
/*
    Map X,Y:    18,11

    Cities:     19,12
                18,17
                23,14
                20,19
    
    Units:      18,12
                19,15
                20,15
                19,16
                20,16
                19,17
                18,18
                19,18
                18,21
                !25,11
                !30,11
                !24,13
                !26,13
                !24,16
                !25,16
                !29,16
                !25,17
                !25,18
                !23,19
*/

struct s_TST_State_Movement_Map
{
    uint8_t entities_on_map[(MAP_WIDTH*MAP_HEIGHT)];
};

struct s_TST_State_Movement_Map Game_States[(MAP_WIDTH*MAP_HEIGHT)];

uint8_t SAVETEST_entities_on_map[(MAP_WIDTH*MAP_HEIGHT)];

// ¿ just coords for entities_on_map[] != ST_UNDEFINED ?



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Validate_Square_Explored(int16_t world_x, int16_t world_y, int16_t world_p);

void Validate_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p);

void Export_Entities_On_Movement_Map(int16_t world_x, int16_t world_y, int16_t world_p);

void Validate_Entities_On_Movement_Map(int16_t world_x, int16_t world_y, int16_t world_p);


// void Initialize_State_validation_data(void);



#ifdef __cplusplus
}
#endif

#endif  /* TST_GAMEDATA_H */
