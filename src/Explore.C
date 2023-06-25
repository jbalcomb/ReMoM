
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "Explore.H"

#include "MoX_Data.H"


// WZD o97p02
int16_t TILE_IsVisible(int16_t world_x, int16_t world_y, int16_t world_plane)
{
    int16_t Tile_Index;
    int16_t Tile_Explored;
    int16_t Tile_Visibility;
    int16_t is_visible;

    is_visible = ST_FALSE;

    Tile_Index = world_y * WORLD_WIDTH + world_x;

    if(world_plane = 0)
    {
        // TODO  Tile_Visibility = MEM_TestBit_Far(Tile_Index, Visibility_Arcanus);
    }
    else
    {
        // TODO  Tile_Visibility = MEM_TestBit_Far(Tile_Index, Visibility_Myrror);
    }

    Tile_Explored = TILE_IsExplored(world_x, world_y, world_plane);

    // if(Tile_Visibility = ST_TRUE && Tile_Explored == ST_TRUE)
    if(Tile_Explored == ST_TRUE)
    {
        is_visible = ST_TRUE;
    }

    return is_visible;
}



// WZD o97p08
int16_t TILE_IsExplored(int16_t world_x, int16_t world_y, int16_t world_plane)
{
    int16_t is_explored;

    is_explored = ST_FALSE;

    if(TBL_Scouting[( (world_plane * 2400) + (world_y * WORLD_WIDTH) + world_x )] != ST_FALSE)
    {
        is_explored = ST_TRUE;
    }

    return is_explored;
}
