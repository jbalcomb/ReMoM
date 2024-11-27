
MoM / MoX
'Tactical Combat' / 'Combat Screen'
Battlefield / Combat Grid
Generate Battlefield
Draw Battlefield





CMB_DrawFullScreen__WIP()
    |-> CMB_CreateEntities__WIP();
    |-> CMB_DrawMap__WIP();
    draw combat enchantments
    draw player name, city name, or lair type
    |-> CMB_DrawSpellInfoW()
    |-> CMB_DrawUnitDisplay()
    |-> CMB_DrawActiveUnitW()



CMB_CreateEntities__WIP()

CMB_ZSort_Array
CMB_SpriteCount

dseg:D15A 00 00 00 00                                     CMB_EntitySprites@ dd 0                 ; DATA XREF: CMB_DrawEntities__WIP+1Cr ...
dseg:D15A                                                                                         ; 1E2h LBX_Alloc_Next paragraphs, sandbox segment
dseg:D15A                                                                                         ; (up to 101h records of 1Eh bytes each)

dseg:D15E 00 00                                           CMB_SpriteCount dw 0                    ; DATA XREF: CMB_DrawEntities__WIP:loc_DD3A3r ...

dseg:D280
; 1-byte, unsigned
CMB_ZSort_Array db 100h dup(0)





## MoveStage





## pict_or_idx
    CMB_CreateEntity__WIP()
        NX_j_CMB_CreateEntity__WIP()
        CMB_SpawnTrees()
        CMB_SpawnRocks()
        CMB_SpawnStructures()
        CMB_SpawnStoneWall()
        CMB_SpawnDarkWall()
        CMB_SpawnFireWall()
        CMB_SpawnVortices()
        CMB_SpawnFigure__WIP() passes battle_units[itr].battle_unit_figure_idx
        CMB_SpawnUnitCurse()
        CMB_SpawnProjectiles()

Trees, Rocks, Structures, StoneWall, DarkWall, FireWall, Vortices, UnitCurse, Projectiles





// WZD ovr153p02
// drake178: CMB_DrawMap()

; draws the combat map into the current draw frame,
; starting with the background saved in VGA frame 3,
; and drawing all tile animations, roads, and combat
; entities on top of that; includes the bottom UI
; background, but not the actual controls



// WZD ovr154p01
// drake178: CMB_Terrain_Init()
/*
; sets the terrain and map-related combat variables
; into the battlefield structure, loads the combat tile
; images, generates the combat map, and then draws the
; background, saving it into VGA frame 3
;
; has multiple BUGs related to outposts, roads across
; the Y-axis, rivers, and walls
*/
void CMB_Terrain_Init__WIP(int16_t wx, int16_t wy, int16_t wp)


iters cities
    checks Outpost vs. City
    if City, checks Fortress City
iters Towers
iters Nodes
iters Lairs



## Draw

...
...
...

CMB_TargetFrame
CMB_TargetFrame_X
CMB_TargetFrame_Y

CMB_ActiveUnitFrame
CMB_ActiveUnitFrameX
CMB_ActiveUnitFrameY


CMB_DrawEntities__WIP()

for CMB_EntitySprites[IDK_combat_entity_idx].entity_type == 1
uses CMB_BU_Figure_GFX[]

0, 2, 3, 4
use CMB_EntitySprites[IDK_combat_entity_idx].pict_seg


...draw battle unit figure...

Deploy_Battle_Units()
CMB_CreateEntities__WIP()
CMB_SpawnFigure__WIP()
CMB_CreateEntity__WIP()
CMB_DrawEntities__WIP()

...`Deploy_Battle_Units() |-> CMB_CreateEntities__WIP() |-> CMB_SpawnFigure__WIP() |-> CMB_CreateEntity__WIP() |-> CMB_DrawEntities__WIP()`...
...
...
...



Deploy_Battle_Units()
    battle_units[Melee_Units[itr]].position_cgc2 = starting_cgc2[ctr];
    battle_units[Melee_Units[itr]].position_cgc1 = starting_cgc1[ctr];
    ...
    battle_units[itr].target_cgc2 = (battle_units[itr].position_cgc2 - 1);
    OR
    battle_units[itr].target_cgc2 = (battle_units[itr].position_cgc2 + 1);
    ...
    battle_units[itr].target_cgc1 = battle_units[itr].position_cgc1;

Combat_Figure_Compose_USEFULL()
    position_cgc2 = battle_units[itr].position_cgc2;





CMB_SpawnFigure__WIP(battle_units[itr].battle_unit_figure_idx, battle_units[itr].position_cgc2, battle_units[itr].position_cgc1, battle_units[itr].target_cgc2, battle_units[itr].target_cgc1, battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);


void CMB_SpawnFigure__WIP(  int64_t seg_or_idx, 
                            int16_t position_cgc2, int16_t position_cgc1, 
                            int16_t target_cgc2,   int16_t target_cgc1, 
                            int16_t MoveStage,
                            int16_t current_figure, int16_t figure_count,
                            int16_t controller_idx, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU, int16_t LostFigs, int16_t SrcBld)

    Combat_Grid_Screen_Coordinates(position_cgc2, position_cgc1, 0, 0, &position_screen_x, &position_screen_y);

    Combat_Grid_Screen_Coordinates(target_cgc2, target_cgc1, 0, 0, &target_screen_x, &target_screen_y);

    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);  // used for combat_grid_entities[].draw_x

    draw_y = (((((target_screen_y - position_screen_y) * MoveStage) / 8) + position_screen_y) + fig_y);  // used for combat_grid_entities[].draw_y

    |-> CMB_CreateEntity__WIP(draw_x, draw_y, seg_or_idx, 13, 23, UU, 1, controller_idx, figure_set_idx, outline_magic_realm, BldAmt, UU, Blood_Frame);


void CMB_CreateEntity__WIP( int16_t draw_x, int16_t draw_y, 
                            int64_t seg_or_idx,
                            int16_t draw_x_shift, int16_t draw_y_shift,
                            int16_t Frame, int16_t DrawType, int16_t controller_idx, int16_t UU_10h, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU_14h, int16_t BldFrm)

    combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;
    combat_grid_entities[combat_grid_entity_count].draw_y = draw_y;
    combat_grid_entities[combat_grid_entity_count].draw_x_shift = draw_x_shift;
    combat_grid_entities[combat_grid_entity_count].draw_y_shift = draw_y_shift;


CMB_DrawEntities__WIP()
    Draw_Picture_Windowed(
        (combat_grid_entities[IDK_combat_entity_idx].draw_x - combat_grid_entities[IDK_combat_entity_idx].draw_x_shift),
        (combat_grid_entities[IDK_combat_entity_idx].draw_y - combat_grid_entities[IDK_combat_entity_idx].draw_y_shift),
        CMB_BU_Figure_GFX[combat_grid_entities[IDK_combat_entity_idx].index]
    );


combat_grid_entities[].draw_x_shift
...per common usage, this is not an "offset" by the very nature of it being subtracted...





Deploy_Battle_Units()
    battle_units[Melee_Units[itr]].position_cgc2 = starting_cgc2[ctr];
    battle_units[Melee_Units[itr]].position_cgc1 = starting_cgc1[ctr];

CMB_SpawnFigure__WIP( battle_units[itr].position_cgc2, battle_units[itr].position_cgc1 )
    Combat_Grid_Screen_Coordinates(position_cgc2, position_cgc1, 0, 0, &position_screen_x, &position_screen_y);
    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);
    |-> CMB_CreateEntity__WIP( draw_x )
        CMB_CreateEntity__WIP( draw_x )
            combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;
    CMB_DrawEntities__WIP()
        (combat_grid_entities[IDK_combat_entity_idx].draw_x - combat_grid_entities[IDK_combat_entity_idx].draw_x_shift)

¿ first defender unit ?
¿ wounded attacker unit ?

_combat_total_unit_count is 8
itr is 0
Max_Figures is 8
Cur_Figures is 4
unit_idx is 665
...
itr is 0
ctr is 0
position_cgc2 is 14
position_cgc1 is 12
...
    Combat_Grid_Screen_Coordinates(position_cgc2, position_cgc1, 0, 0, &position_screen_x, &position_screen_y);
        l_screen_x  = (((cgc2 - cgc1) * 16) + 158);
        l_screen_y  = (((cgc2 + cgc1) *  8) -  80);
        l_screen_x += ((something_x - something_y) * 2);
        l_screen_y += ( something_x + something_y     );
        *screen_x   = l_screen_x;
        *screen_y   = l_screen_y;
screen_x is 190
screen_y is 128
...
    Combat_Grid_Screen_Coordinates(target_cgc2, target_cgc1, 0, 0, &target_screen_x, &target_screen_y);
        target_cgc2, target_cgc1
         13,  12
        174, 120
...
    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);
...
    (figure_count - 1) is 7
    current_figure is 0
    fig_x is 1
    fig_y is 3
    figure_set_idx is 7
    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);
    ((((174 - 190) * 0 / 8) + 190) + 1)
    draw_x is 191
    draw_y = (((((target_screen_y - position_screen_y) * MoveStage) / 8) + position_screen_y) + fig_y);
    (((((120 - 128) * 0) / 8) + 128) + 3)
    draw_y is 131
The calc on target_screen_x,y, position_screen_x,y, MoveStage, and fig_x,y gets turned into 191, 131.
...
first figure (top-most) looks to be at 189, 121, in the OG-MoM screen-shot

...
CMB_CreateEntity__WIP()
    combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;
    combat_grid_entities[combat_grid_entity_count].draw_y = draw_y;
    combat_grid_entities[combat_grid_entity_count].draw_x_shift = draw_x_shift;
    combat_grid_entities[combat_grid_entity_count].draw_y_shift = draw_y_shift;
draw_x is 191
draw_y is 131
draw_x_shift is 13
draw_y_shift is 23
...

CMB_DrawEntities__WIP()
        (combat_grid_entities[IDK_combat_entity_idx].draw_x - combat_grid_entities[IDK_combat_entity_idx].draw_x_shift)

combat_grid_entity_count is 31
    ... defender is 4 units ... 1 + 1 + 1 + 1 =  4 figures
    ... attacker is 4 units ... 8 + 4 + 7 + 8 = 27 figures
IDK_itr_sprites is 18
IDK_combat_entity_idx is 0
...
    (combat_grid_entities[IDK_combat_entity_idx].draw_x - combat_grid_entities[IDK_combat_entity_idx].draw_x_shift)
    (combat_grid_entities[IDK_combat_entity_idx].draw_y - combat_grid_entities[IDK_combat_entity_idx].draw_y_shift)
(191 - 13) = 178
(131 - 23) = 108

combat_grid_entities[].draw_x,draw_y - combat_grid_entities[].draw_x_shift,draw_y_shift gets us 178, 108

...{178, 108} does not *feel* like {189, 121}...

... * 2 for ReMoM is 356, 216 ...
ReMoM is 356, 216





## combat_grid_entities

XREF:  (65)


struc s_CMBT_ENTITY ; (sizeof=0x1E)
00000000 draw_x dw ?
00000002 draw_y dw ?
00000008 draw_y_shift dw ?
0000000A draw_x_shift dw ?

ovr153:08AC 26 8B 47 02                                     mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:08BF 26 2B 47 08                                     sub     ax, [es:bx+s_CMBT_ENTITY.draw_y_shift]

ovr153:08D1 26 8B 07                                        mov     ax, [es:bx+s_CMBT_ENTITY.draw_x]
ovr153:08E3 26 2B 47 0A                                     sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]

move memory to register AX

26 8B 47 02                                     mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
      ^ why the 4?
26 8B 07                                        mov     ax, [es:bx+s_CMBT_ENTITY.draw_x]
         ^^ ¿ assumes offset 00 ?


search: 26 8B 47 02

ovr153:07AA CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:0862 CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:08AC CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:0951 CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:09CE CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:0A7A CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]
ovr153:0B1A CMB_DrawEntities__WIP       mov     ax, [es:bx+s_CMBT_ENTITY.draw_y]

search: 26 2B 47 0A
ovr153:07E1 CMB_DrawEntities__WIP sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]
ovr153:08E3 CMB_DrawEntities__WIP sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]
ovr153:0988 CMB_DrawEntities__WIP sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]
ovr153:0A05 CMB_DrawEntities__WIP sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]
ovr153:0AB1 CMB_DrawEntities__WIP sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]
ovr153:0B51 CMB_DrawEntities__WIP sub     ax, [es:bx+s_CMBT_ENTITY.draw_x_shift]

search: 26 8B 47 0A
none







Deploy_Battle_Units() uses _combat_total_unit_count...

...set in CMB_Units_Init__WIP()
...uses troop_count
Tactical_Combat__WIP()
    Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);
...passed in int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
...passed in int16_t Combat__WIP(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[])
...passed in combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
...from Move_Units()





STEP-BY-STEP
Deploy_Battle_Units()
        battle_units[Melee_Units[itr]].position_cgc2 = starting_cgc2[ctr];
        battle_units[Melee_Units[itr]].position_cgc1 = starting_cgc1[ctr];

_combat_total_unit_count is 8
_combat_attacker_player is 0
Melee_Count is 4
itr is 0
ctr is 0
battle_units[Melee_Units[itr]].position_cgc2 is 14
battle_units[Melee_Units[itr]].position_cgc1 is 12
itr is 1
ctr is 1
battle_units[Melee_Units[itr]].position_cgc2 is 14
battle_units[Melee_Units[itr]].position_cgc1 is 11
itr is 2
ctr is 2
battle_units[Melee_Units[itr]].position_cgc2 is 14
battle_units[Melee_Units[itr]].position_cgc1 is 13
itr is 3
ctr is 3
battle_units[Melee_Units[itr]].position_cgc2 is 14
battle_units[Melee_Units[itr]].position_cgc1 is 10

cgc2, cgc1
14, 12
14, 11
14, 13
14, 10

player_idx is 2
Melee_Count is 4
itr is 0
ctr is 0
battle_units[Melee_Units[itr]].position_cgc2 is  8
battle_units[Melee_Units[itr]].position_cgc1 is 12
itr is 1
ctr is 1
battle_units[Melee_Units[itr]].position_cgc2 is  8
battle_units[Melee_Units[itr]].position_cgc1 is 11
itr is 2
ctr is 2
battle_units[Melee_Units[itr]].position_cgc2 is  8
battle_units[Melee_Units[itr]].position_cgc1 is 13
itr is 3
ctr is 3
battle_units[Melee_Units[itr]].position_cgc2 is  8
battle_units[Melee_Units[itr]].position_cgc1 is 10

cgc2, cgc1
 8, 12
 8, 11
 8, 13
 8, 10

...`defender {cgc2, cgc1}    { 8, 12}, { 8, 11}, { 8, 13}, { 8, 10}`...
...`attacker {cgc2, cgc1}    {14, 12}, {14, 11}, {14, 13}, {14, 10}`...



x,y from screenshot; defender, attacker; bottom-left to top-right; ~ center of figure picture
...
 80, 95
 96, 85
112, 77
128, 69
...
173, 139
191, 129
207, 121
225, 111




CMB_DrawEntities__WIP() calls Draw_Picture_Windowed() with (draw_x - draw_x_shift), (draw_y - draw_y_shift)
draw_x_shift, draw_y_shift are hard-coded to 13, 23
















### Location_Type
    0   ¿ default ?
    1   Outpost
    2   City
    3   Fortress City
    4   Tower



### City_House_Type



### Magic_Walls
    0   ¿ default ?
    1   Wall of Fire





Tile_Terrain[]

Tile_TerrGroup[]

Generate_Combat_Map__WIP()
sets Tile_TerrGroup[]
CMB_TileGen__WIP
sets Tile_Terrain[], using Tile_TerrGroup[]

Generate_Combat_Map__WIP()
if nay water, sets all of Tile_TerrGroup[] to CTG_Grass
if yay water, sets all of Tile_TerrGroup[] to CTG_DeepWater



## CMB_TileGen__WIP()

iters over 21 rows and 20 columns
sets 9 values based on Tile_TerrGroup
sets the 4 corners to CTG_Grass

`(itr_y * 21)` means that the combat grid column count / row length is 21



battlefield->Tile_Terrain[]
maps first 48 (of 58) entries from combat terrain LBX files





## CMB_ComposeBackgrnd__WIP()

iters 21
iters 10
calcs
accesses battlefield->Tile_Terrain[]

        IDKL_base_cgc2 = (itr_y / 2);  // ¿ row, in half-rows ?

        Base_Tile_Y = (((itr_y + 1) / 2) + 9);  // {9, }

Why the `+ 9`?


IDKL_base_cgc2 = (itr_y / 2);
cgc2 = (IDKL_base_cgc2 + itr_x);
combat_terrain_type = battlefield->Tile_Terrain[((cgc1 * 21) + cgc2)];

cgc2 = ((itr_y / 2) + itr_x);


Base_Tile_Y = (((itr_y + 1) / 2) + 9);
cgc1 = (Base_Tile_Y - itr_x);
combat_terrain_type = battlefield->Tile_Terrain[((cgc1 * 21) + cgc2)];






CMB_ComposeBackgrnd__WIP()

itr_y
itr_x


        IDKL_base_cgc2 = (itr_y / 2);  

        Base_Tile_Y = (((itr_y + 1) / 2) + 9);

            cgc2 = (IDKL_base_cgc2 + itr_x);

            cgc1 = (Base_Tile_Y - itr_x);

            screen_x = (itr_x * 32) - ((itr_y & 0x1) * 16);

            screen_y = ((itr_y * 8) - 8);


IDKL_base_cgc2 = (itr_y / 2);
    {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10}
cgc2 = (IDKL_base_cgc2 + itr_x);
    {0, 1, 2, 3, 4, 5, 6, 7, 8,  9, 10}
    {0, 1, 2, 3, 4, 5, 6, 7, 8,  9, 10}
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
    ...
    ...
    ...


Base_Tile_Y = (((itr_y + 1) / 2) + 9);
itr_y:                   {0, ..., 21}
(itr_y + 1):             {1, ..., 22}
((itr_y + 1) / 2):       {0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11}
(((itr_y + 1) / 2) + 9): {9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,17,17,18,18,19,19,20}
itr_x:                   {0, ..., 10}


draws 10 or 11 cells on the x-axis

combat terrain picture is 30 x 16

iter over 21 rows
iter over 10 columns
what does (row / 2) get you?



Tile_Type = battlefield->Tile_Terrain[(2 + (cgc1 + cgc2))];
if(Tile_Type >= 56)
if(Tile_Type < 48)
What about 48 through 55?

x,y for the firs/top-left corner?
shift left a half-square  ... x - 16
shifted up a half-square  ... y - 8
¿ -16,-8 ?


screen_x,screen_y
{-16,-8}, {16, 0}, {48,-8}, ...
{-16, 8}, {16,16}, {48, 8}, ...

¿ {((0 * 32) - 16), ((0 * 8) - 8)} ?



## CMB_SpawnFigure__WIP()

    Combat_Grid_Screen_Coordinates(position_cgc2, position_cgc1, 0, 0, &position_screen_x, &position_screen_y);

    Combat_Grid_Screen_Coordinates(target_cgc2, target_cgc1, 0, 0, &target_screen_x, &target_screen_y);

    switch(figure_count - 1)

e.g., ...
...`Deploy_Battle_Units() |-> CMB_CreateEntities__WIP() |-> CMB_SpawnFigure__WIP() |-> CMB_CreateEntity__WIP() |-> CMB_DrawEntities__WIP()`...
Deploy_Battle_Units()
    Melee_Count is 1
    Cur_Figures is 8
    Max_Figures is 8
    position_cgc2 is 14
    position_cgc1 is 12
            if(battle_units[itr].controller_idx == player_idx)
                if(_combat_attacker_player == player_idx)
                    battle_units[itr].target_cgc2 = (battle_units[itr].position_cgc2 - 1);
    target_cgc2 is 13
    target_cgc1 is 12

CMB_DrawFullScreen__WIP()
    |-> CMB_CreateEntities__WIP()
        unit_figure_count = battle_units[itr].Cur_Figures;
        unit_figure_maximum = battle_units[itr].Max_Figures;
        |-> CMB_SpawnFigure__WIP(battle_units[itr].battle_unit_figure_idx, battle_units[itr].position_cgc2, battle_units[itr].position_cgc1, battle_units[itr].target_cgc2, battle_units[itr].target_cgc1, battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);

CMB_SpawnFigure__WIP()
    Combat_Grid_Screen_Coordinates(position_cgc2, position_cgc1, 0, 0, &position_screen_x, &position_screen_y);
    Combat_Grid_Screen_Coordinates(target_cgc2, target_cgc1, 0, 0, &target_screen_x, &target_screen_y);
position_screen_x is 190
position_screen_y is 128
target_screen_x is 174
target_screen_y is 120
figure_count is 8
current_figure is 0
switch(figure_count - 1)
    case 7:
        switch(current_figure)
            case 0: { fig_x =   1; fig_y =  3; } break;
fig_x is 1
fig_y is 3
if(position_cgc2 > target_cgc2)
    NOT if(position_cgc1 > target_cgc1)
    NOT else if(position_cgc1 < target_cgc1)
    figure_set_idx = 7;
    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);  // used for combat_grid_entities[].draw_x
    draw_y = (((((target_screen_y - position_screen_y) * MoveStage) / 8) + position_screen_y) + fig_y);  // used for combat_grid_entities[].draw_y
    (((((174 - 190) * 0) / 8) + 190) + 1) = ((((-16 * 0) / 8) + 190) + 1) = (((0 / 8) + 190) + 1) = ((0 + 190) + 1) = (190 + 1) = 191
    (((((120 - 128) * 0) / 8) + 128) + 3) = (((( -8 * 0) / 8) + 128) + 3) = (((0 / 8) + 128) + 3) = ((0 + 128) + 3) = (128 + 3) = 131
draw_x is 191
draw_y is 131
    |-> CMB_CreateEntity__WIP(draw_x,draw_y,seg_or_idx,13,23,UU,1,controller_idx,figure_set_idx,outline_magic_realm,BldAmt,UU,Blood_Frame);

CMB_CreateEntity__WIP()
    combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;
    combat_grid_entities[combat_grid_entity_count].draw_y = draw_y;
    combat_grid_entities[combat_grid_entity_count].seg_or_idx = seg_or_idx;
    combat_grid_entities[combat_grid_entity_count].Draw_Frame = Frame;
    combat_grid_entities[combat_grid_entity_count].draw_x_shift = draw_x_shift;
    combat_grid_entities[combat_grid_entity_count].draw_y_shift = draw_y_shift;
    combat_grid_entities[combat_grid_entity_count].entity_type = DrawType;
    combat_grid_entities[combat_grid_entity_count].Owner = controller_idx;
    combat_grid_entities[combat_grid_entity_count].outline_magic_realm = outline_magic_realm;
    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &combat_grid_cell_x, &combat_grid_cell_x_offset);
    Screen_To_Combat_Grid_Cell_Y_And_Offset(draw_x, draw_y, &combat_grid_cell_y, &combat_grid_cell_y_offset);
combat_grid_cell_x is 14
combat_grid_cell_x_offset is 3
combat_grid_cell_y is 12
combat_grid_cell_y_offset is 3

    ...
    ...  END: CMB_DrawFullScreen__WIP() |-> CMB_CreateEntities__WIP()
    ...

CMB_DrawFullScreen__WIP()
    |-> CMB_DrawMap__WIP()

CMB_DrawMap__WIP()
    |-> Combat_Figure_Compose_USEFULL()

BEGIN:  Combat_Figure_Compose_USEFULL()
            position_cgc2 = battle_units[itr].position_cgc2;
            position_cgc1 = battle_units[itr].position_cgc1;
            target_cgc2 = battle_units[itr].target_cgc2;
            target_cgc1 = battle_units[itr].target_cgc1;
            frame_num is 1
            14,12  13,12
            figure_set_idx is 7
            itr is 0
            battle_unit_figure_idx is 0
            unit_idx is 825
END:  Combat_Figure_Compose_USEFULL()

for(itr_y = 0; itr_y < 22; itr_y++)
    for(itr_x = 0; itr_x < 11; itr_x++)

cgc2 is 0
cgc1 is 9
screen_x is 0
screen_y is -8
battlefield_terrain is 0

|-> CMB_DrawEntities__WIP()

CMB_DrawEntities__WIP()
combat_grid_entity_count is 12  ... 1 + 1 + 1 + 1 + 8 figures

...
combat_entity_draw_order_array[] {11, 9, 8, 0, 1, 4, 2, 5, 3, 6, 7, 10 }
...
combat_grid_entities[] {  }
...



CMB_BU_Figure_GFX[]


CMB_Units_Init__WIP
    for(itr = 0; itr < troop_count; itr++)
        battle_units[_combat_total_unit_count].battle_unit_figure_idx = Combat_Figure_Load(_UNITS[troops[itr]].type, itr);

Combat_Figure_Load()

    ptr_figure_pointer_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMM_PageFrame + offset));


EMM_FIGUREX_Init__HACK()
    logical_page = ((battle_unit_figure_idx * 3) / 2);
    EMM_PageFrame = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));

Combat_Figure_Compose_USEFULL()
    battle_unit_figure_idx = battle_units[itr].battle_unit_figure_idx;
    EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);
    ...
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
    GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);




Draw_Picture_Windowed()
 x1,  y1
175, 112
*2
350,224
