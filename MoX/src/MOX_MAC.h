/*

SEEALSO  ¿ docs ?

MOX_DEF.h
#define GET_MAIN_MAP_ENTITY()              ( entities_on_movement_map[ ( ( (_main_map_grid_y) * MAP_WIDTH ) + (_main_map_grid_x) ) ]                  )
#define SET_MAIN_MAP_ENTITY(_entity_idx_)  ( entities_on_movement_map[ ( ( (_main_map_grid_y) * MAP_WIDTH ) + (_main_map_grid_x) ) ] = (_entity_idx_) )
#define GET_MAP_ENTITY(_mx_,_my_)               ( entities_on_movement_map[ ( ( (_my_) * MAP_WIDTH ) + (_mx_) ) ]                  )
#define SET_MAP_ENTITY(_mx_,_my_,_entity_idx_)  ( entities_on_movement_map[ ( ( (_my_) * MAP_WIDTH ) + (_mx_) ) ] = (_entity_idx_) )


Getters & Setters

game-data


entities_on_movement_map
_landmasses
_map_square_flags
_map_square_terrain_specials
_square_explored



Eh?  MOX_UPD.h:Debugging Getters and Setters for *Dynamic* Game-Data

*/