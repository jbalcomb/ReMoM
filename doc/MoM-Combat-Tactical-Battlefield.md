
MoM / MoX
'Tactical Combat' / 'Combat Screen'
Battlefield / Combat Grid
Generate Battlefield
Draw Battlefield



## Generate / Build / Make



struct s_BATTLEFIELD * battlefield;
s_BATTLEFIELD.House_TileXs
s_BATTLEFIELD.House_TileYs

battlefield->House_IMG_Segs[battlefield->House_Count] = IMG_CMB_Houses[(((house_type * 5) + Random(5)) - 1)];




¿ House ?
inside city
x & y ... screen, combat grid, city
city is 4x4

starts from x = 5, y = 10
adds Random(4) - 1
x: { 5,  6,  7,  8}
y: {10, 11, 12, 13}

if(city_walls == ST_TRUE)
    skips 0,0, 0,3, 3,0, 3,3
    {5,10}, {5,13}, {8,10}, {8,13}
    the corners?
    because the city wall towers jut into inside corners?



## magic_walls
¿ bit-field ?









1oom
game_battle_human.c
game_battle_with_human_init()
game_battle_with_human_init_sub1()

game_battle_with_human_init()
    |-> game_battle_with_human_init_sub1()
    |-> game_battle_place_items()

game_battle.h
struct battle_s


    uint8_t num_rocks;
    struct battle_rock_s rock[BATTLE_ROCK_MAX];

#define BATTLE_ROCK_MAX 7

struct battle_rock_s {
    int8_t sx;  /* -1, 0..9 */
    int8_t sy;  /* -1, 0..9 */
    uint8_t *gfx;
};

battle_side_i_t side;

typedef enum { SIDE_L = 0, SIDE_R = 1, SIDE_NONE = 2 } battle_side_i_t;

#define BATTLE_AREA_W 10
#define BATTLE_AREA_H 8





MoO2

_combat_grid
XREF:
    Auto_Move_Ship_()
    Draw_Main_Combat_Screen_()
    Display_Combat_View_Ship_()
    Draw_Legal_Boxes_()
    Ship_In_Legal_Square_()
    Move_Ship_()
    Do_Combat_Turn_()
    Assign_Combat_Grids_()
    Set_Legal_Moves_()
    Teleport_Enemy_Ship_()

¿ 81 * 68 = 5508 ?


Do_Combat_Turn_()
    ...
    input_field_idx == _combat_grid[]
    input_field_idx == _map_field

Assign_Combat_Grids_()
Set_Legal_Moves_()



_map_field
XREF:
    Do_Combat_Turn_()
    Add_Tactical_Combat_Fields_()

