MoM-CartographerScreen.md




terrain_lbx_000_offset
...getting 835008
...from
    terrain_lbx_000_offset = 0xC0 + ((l_terrain_001_1 - 2) * 384);
    OR
    terrain_lbx_000_offset = (l_terrain_001_0 * 16384) + (l_terrain_001_1 * 384) - 0xC0 - 384;
l_terrain_001_0 ==  48
l_terrain_001_1 == 128  0x80
terrain_type == 34
...feels wrong...l_terrain_001_1 == 128  0x80

terrain_001_0
MSB is {F,T} terrain is animated
other 7 bits are value of EMM Logical Page Number

terrain_001_1 is the index of the terrain picture, as loaded into the EMM Page Frame Buffer

assert for l_terrain_001_1?
max?
1024 * 16 / 384 = 42.666666666666666666666666666667

676416 / 16384 = 41.28515625
Hrrrmm...
file offset of last terrain picture
0xA50C0  676032
676032 / 16384 = 41.26171875





                l_terrain_001_0 = GET_1B_OFS(m_terrain_lbx_001, terrain_001_index + 0);
                l_terrain_001_1 = GET_1B_OFS(m_terrain_lbx_001, terrain_001_index + 1);

                assert( (l_terrain_001_0 == 0 && l_terrain_001_1 >= 2) || (l_terrain_001_0 != 0) );

                map_draw_full = ST_TRUE;
                if( ((l_terrain_001_0 & 0x80) == 0) || (map_draw_full == ST_TRUE) )
                {
                    if((l_terrain_001_0 & 0x80) != 0)
                    {
                        l_terrain_001_0 = (l_terrain_001_0 & 0x7F);  // ~== -128  ... NOTE(JimBalcomb,20240915)  clear the sign-bit
                        l_terrain_001_1 += terrain_anim_ctr;
                    }

                    // MS-DOS  // Dasm  terrain_pict_seg = EMM_PageFrame[( (((((m_terrain_lbx_001 * 2) + m_terrain_lbx_001) * 2) * 2) *2) )];
                    // MS-DOS  terrain_pict_seg = EMM_PageFrame[(m_terrain_lbx_001 * 24)];  // segments; 24 * 16 = 384, sizeof pict; 180h offset from File, not Entry
                    // MS-DOS  if(terrain_000_elpn != terrain_001_0)
                    // MS-DOS  {
                    // MS-DOS      EMM_Map4Pages(terrain_001_0, g_EmmHndl_TERRAIN); // First Logical Page, EMM Handle Name
                    // MS-DOS  }

                    // map index in m_terrain_lbx_001 to TERRAIN.LBX Entry 0, instead of TERRAIN.LBX entire file in EMM
                    // loading entry 0, instead of whole file
                    // ( EMM Page * 16K ) + (terrain pict index * 384) - (difference in header size)
                    if(l_terrain_001_0 == 0)
                    {
                        terrain_lbx_000_offset = 0xC0 + ((l_terrain_001_1 - 2) * 384);
                        // terrain_lbx_000_offset = (terrain_001_0 * 16384) + ((m_terrain_lbx_001 - 2) * 384) + 0xC0;
                    }
                    else
                    {
                        terrain_lbx_000_offset = (l_terrain_001_0 * 16384) + (l_terrain_001_1 * 384) - 0xC0 - 384;
                        // terrain_lbx_000_offset = (terrain_001_0 * 16384) + (m_terrain_lbx_001 * 384) - 0xC0 - 384;
                    }

                    terrain_pict_seg = m_terrain_lbx_000 + terrain_lbx_000_offset;
                    Draw_Picture(itr_screen_x, itr_screen_y, terrain_pict_seg);








## IDK_Prepare_TileMap_sDAB1A()

¿ ...equivalent to... ?
Draw_Map_Terrain()



Draw_Map_Terrain()

```c
    uint32_t terrain_lbx_000_offset;
    uint16_t terrain_001_index;
    uint8_t l_terrain_001_0;
    uint8_t l_terrain_001_1;
    byte_ptr terrain_pict_seg;

world_maps_offset = ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2));
l_terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
l_terrain_001_index += terrain_tile_base;
l_terrain_001_index *= 2;  // because, sizeof(int16_t)
l_terrain_001_0 = 0;
l_terrain_001_0 = GET_1B_OFS(m_terrain_lbx_001, l_terrain_001_index + 0);
l_terrain_001_1 = GET_1B_OFS(m_terrain_lbx_001, l_terrain_001_index + 1);

if(l_terrain_001_0 == 0)
{
    l_terrain_lbx_000_offset = 0xC0 + ((l_terrain_001_1 - 2) * 384);
    // terrain_lbx_000_offset = (terrain_001_0 * 16384) + ((terrain_001_1 - 2) * 384) + 0xC0;
}
else
{
    l_terrain_lbx_000_offset = (l_terrain_001_0 * 16384) + (l_terrain_001_1 * 384) - 0xC0 - 384;
    // terrain_lbx_000_offset = (terrain_001_0 * 16384) + (terrain_001_1 * 384) - 0xC0 - 384;
}

terrain_pict_seg = terrain_lbx_000 + terrain_lbx_000_offset;
Draw_Picture(itr_screen_x, itr_screen_y, terrain_pict_seg);
```

```c
mov     [bp+terrain_001_0], 0

mov     ax, [bp+terrain_type]
shl     ax, 1                           ; sizeof 2
les     bx, [terrain_lbx_001]
add     bx, ax
mov     al, [es:bx]                     ; in Draw_Map_Unexplored_Area - 'byte ptr'
                                        ; test  [byte ptr es:bx], 80h
mov     ah, 0                           ; ¿ ~ ((u)int8_t >> 8) & 0x00FF ?
mov     [bp+terrain_001_0], ax


mov     ax, [bp+terrain_type]
shl     ax, 1                           ; sizeof 2
les     bx, [terrain_lbx_001]
add     bx, ax
mov     al, [es:bx+1]
mov     ah, 0
mov     [terrain_001_1], ax


test    [bp+terrain_001_0], 80h         ; animated tile
jz      short loc_D9817

loc_D9817:                              ; determines whether non-animated terrain tiles will
cmp     [map_draw_full], e_ST_FALSE     ; be redrawn or not
jnz     short loc_D9821

mov     ax, [terrain_anim_ctr]          ; ? Animated Terrain Frame Index ?
add     [terrain_001_1], ax

mov     ax, [bp+terrain_001_0]
and     ax, 7Fh                         ; clear the sign bit
mov     [bp+terrain_001_0], ax
jmp     short loc_D9821

loc_D9821:                              ; terrain_pict_idx
mov     ax, [terrain_001_1]
mov     bx, ax
shl     ax, 1
add     ax, bx                          ; ~= terrain index * 3
shl     ax, 1                           ; * 6
shl     ax, 1                           ; * 12
shl     ax, 1                           ; * 24
add     ax, [EMM_PageFrame]             ; * 180h, from FILE start, not entry start!
mov     [bp+terrain_pict_seg], ax       ; offset into TERRAIN.LBX

mov     ax, [bp+terrain_000_elpn]       ; always -1...
cmp     ax, [bp+terrain_001_0]
jz      short loc_D984D

push    [ehn_terrain_lbx]               ; EMM_Handle
push    [bp+terrain_001_0]              ; First_Log_Page
call    EMM_Map4Pages                   ; maps in four consecutive logical pages from the
                                        ; passed handle, starting with the one specified
                                        ; uses a different EMM function than seg012:0255
                                        ; preserves all register values
pop     cx
pop     cx


loc_D984D:                              ; pict_seg
push    [bp+terrain_pict_seg]
push    _SI_itr_screen_y                ; y
push    _DI_itr_screen_x                ; x
call    Draw_Picture
add     sp, 6
jmp     short loc_D9875
```
