

The Dasm shows various approaches to getting words or bytes from the near and far pointers.

LEA and then word/byte ptr [ES:BX]

wp * WORLD_SIZE
wp * WORLD_SIZE + wy * WORLD_WIDTH
wp * WORLD_SIZE + wy * WORLD_WIDTH + wx


wp * WORLD_SIZE_DB
wp * WORLD_SIZE_DB + wy * WORLD_WIDTH_DB
wp * WORLD_SIZE_DB + wy * WORLD_WIDTH_DB + wx * 2


...like I did with CITYX(), CITY(), CITYP()
```c
#define CITYX() ( _CITIES[city_idx].world_x     )
#define CITYY() ( _CITIES[city_idx].world_y     )
#define CITYP() ( _CITIES[city_idx].world_plane )
```
requires having/naming `city_idx`
assumes `_CITIES` and `s_CITY`
Then, ...
    same for city_wx, city_wy, city_wp?
    option for passing some/all parameters?

Kinda wanna just use wx,wy,wp...
But, ...
    some functions have
        x,y,p for units and cities
        iters for them
        intermediate/current values




So, ...
    ~ 'Get Word Of World Map' (wx, wy, wp)
    ~ 'Get Byte Of World Map' (wx, wy, wp)
    ¿ the DB or DW access seen in the Dasm is due to the width of the data-type of the pointer ?
        (uint8_t  *)(_world_maps + ( (wp * 4800) + (wy * 120) + (wx * 2) ) );
        (uint16_t *)(_world_maps + ( (wp * 2400) + (wy *  60) + (wx    ) ) );


### _world_maps

if the value matches what is in the Terrain Type enum, then the value is a Terrain Type
¿ that just happens to also be the index into TERRAIN.LBX,0 and TERRAIN.LBX,1 ?

    uint8_t * world_map_ptr;
    int16_t terrain_type_idx;
    int16_t terrain_type;
    world_map_ptr = (_world_maps + (wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2));
    terrain_type_idx = GET_2B_OFS(world_map_ptr, 0);
    terrain_type = terrain_type_idx % TERRAIN_COUNT;

Draw_Map_Unexplored_Area()
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
    terrain_001_0 = GET_1B_OFS(terrain_lbx_001, (terrain_001_index * 2) + 0);

Redraw_Map_Unexplored_Area()
    uint16_t terrain_001_index;
    terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));

Draw_Map_Terrain()
    int16_t world_maps_offset;
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    uint8_t terrain_001_1;
    world_maps_offset = ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2));
    terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
    terrain_001_index += terrain_tile_base;
    terrain_001_index *= 2;  // because, sizeof(int16_t)
    terrain_001_0 = 0;
    terrain_001_0 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 0);
    terrain_001_1 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 1);

Draw_Reduced_Map()
    int16_t terrain_type_idx_base;
    int16_t terrain_type_idx;
    uint8_t * explore_data_ptr;
    uint8_t * world_data_ptr;
    uint8_t * minimap_pict_data_ptr;
    terrain_type_idx_base = world_plane * TERRAIN_COUNT;
    explore_data_ptr = (TBL_Scouting + (world_plane * WORLD_SIZE));
    world_data_ptr = (_world_maps + (world_plane * 4800));
    minimap_pict_data_ptr = minimap_pict_seg + 16;  // +1 segment (paragraph), for the animation/picture/bitmap header
    terrain_type_idx = GET_2B_OFS(world_data_ptr, ((minimap_square_y * 120) + (minimap_square_x * 2)));
    terrain_type_idx += terrain_type_idx_base;
    minimap_terrain_color = terrain_lbx_002[terrain_type_idx];

Terrain_Is_Sailable()
    uint8_t * src_sgmt;
    uint16_t src_ofst;
    uint16_t world_map_value;
    uint16_t terrain_type;
    // les  bx, [_world_maps]
    // ~== ES = (&_world_maps[0] / 16); BX = (&_world_maps[0] % 16);
    src_sgmt = _world_maps;
    src_ofst = 0;
    src_ofst = (map_plane * 4800) + (world_y * 120) + (world_x * 2);
    world_map_value = GET_2B_OFS(src_sgmt, src_ofst);
    terrain_type = world_map_value % TERRAIN_TYPE_COUNT;


Map_Square_Production_Bonus()
    ¿ no word ptr or byte ptr ?
    ¿ all 16-bit registers ?

```x86asm
    mov     ax, [bp+wp]
    mov     dx, e_WORLD_SIZE_DB
    imul    dx
    les     bx, [_world_maps]               ; 25Ah paragraphs in World_Data@
    add     bx, ax
    mov     ax, [bp+wy]
    mov     dx, e_WORLD_WIDTH_DB
    imul    dx
    add     bx, ax
    mov     ax, [bp+wx]
    shl     ax, 1
    add     bx, ax
    mov     ax, [es:bx]

    mov     bx, TerType_Count
    xor     dx, dx
    div     bx
    mov     _SI_terrain_type, dx

    cmp     _SI_terrain_type, TT_Desert_end
```
