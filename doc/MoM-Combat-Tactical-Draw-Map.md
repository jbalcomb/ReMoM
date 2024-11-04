
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

for CMB_EntitySprites[IDK_combat_entity_idx].Draw_Type == 1
uses CMB_BU_Figure_GFX[]

0, 2, 3, 4
use CMB_EntitySprites[IDK_combat_entity_idx].pict_seg





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






CMB_ComposeBackgrnd__WIP()

itr_y
itr_x


        Base_Tile_X = (itr_y / 2);  // {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10}

        Base_Tile_Y = (((itr_y + 1) / 2) + 9);

            Draw_Tile_X = (Base_Tile_X + itr_x);

            Draw_Tile_Y = (Base_Tile_Y - itr_x);

            screen_x = (itr_x * 32) - ((itr_y & 0x1) * 16);

            screen_y = ((itr_y * 8) - 8);


Base_Tile_X = (itr_y / 2);
    {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10}
Draw_Tile_X = (Base_Tile_X + itr_x);
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



Tile_Type = battlefield->Tile_Terrain[(2 + (Draw_Tile_Y + Draw_Tile_X))];
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
