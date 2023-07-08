
gsa_OVL_Tile_WorkArea

used for Draw_Map_Cities, Towers, Nodes
¿ just because/for the wizard banner color remap ?


// WZD dseg:CB60
SAMB_ptr gsa_OVL_Tile_WorkArea;              // alloc in Terrain_Init  // ; 70 * 16 = 1120 bytes

Terrain_Init()
    gsa_OVL_Tile_WorkArea = Allocate_Space(70);  // ; 70 * 16 = 1120 bytes

Draw_Map_Cities()
    city_pict_seg = IMG_OVL_NoWall_City;  // CITYNOWA
    city_pict_seg = IMG_OVL_Walled_City;  // MAPCITY
    FLIC_Set_CurrentFrame(city_pict_seg, city_size);
    Draw_Picture_To_Bitmap(city_pict_seg, gsa_OVL_Tile_WorkArea);
    FLIC_Remap_Color(gsa_OVL_Tile_WorkArea, 214 + itr_color_replacement, (COL_City_Banner[((_players[city_owner].Banner * 5) + itr_color_replacement)] - 1));
    Draw_Picture_Windowed(screen_start_x, screen_start_y, gsa_OVL_Tile_WorkArea);



XREFs
Terrain_Init+2E              mov     [gsa_OVL_Tile_WorkArea@], ax    ; 46h LBX_Alloc_Space paragraphs

RP_OVL_DrawCities2+165       push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
RP_OVL_DrawCities2+183       push    [gsa_OVL_Tile_WorkArea@]        ; IMG_Seg                       
RP_OVL_DrawCities2+1EB       push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
RP_OVL_DrawCities2+223       push    [gsa_OVL_Tile_WorkArea@]        ; IMG_Seg                       
RP_OVL_DrawCities2:loc_5AD8E push    [gsa_OVL_Tile_WorkArea@]        ; pict_seg                      

Draw_Map_Cities+17D          push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
Draw_Map_Cities+19B          push    [gsa_OVL_Tile_WorkArea@]        ; IMG_Seg                       
Draw_Map_Cities+203          push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
Draw_Map_Cities+23B          push    [gsa_OVL_Tile_WorkArea@]        ; IMG_Seg                       
Draw_Map_Cities:loc_D9C71    push    [gsa_OVL_Tile_WorkArea@]        ; pict_seg                      

Draw_Map_Towers+C5           push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
Draw_Map_Towers:loc_D9D69    push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
Draw_Map_Towers+10B          push    [gsa_OVL_Tile_WorkArea@]        ; IMG_Seg                       
Draw_Map_Towers:loc_D9DB3    push    [gsa_OVL_Tile_WorkArea@]        ; pict_seg                      

Draw_Map_Nodes+224           push    [gsa_OVL_Tile_WorkArea@]        ; dst_pict_seg                  
Draw_Map_Nodes+327           push    [gsa_OVL_Tile_WorkArea@]        ; Source_Seg                    
