



Blerg. Why are notes so much garbage?
Past-Me does not show enough love for Future-Me.






Combat Grid Coordinates
...indexing data structures...
CMB_TargetRows[]
CMB_ActiveMoveMap[]
CMB_Vortex_Array[]

BU_Move__WIP indexes CMB_ActiveMoveMap[] using ((cgy * 21) + cgx)

battle_units[itr].position_cgc1,cgc2
is used the same as 
CMB_Vortex_Array[itr].Y_Pos,X_Pos


CMB_GetPath__WIP()
    if(CMB_ActiveMoveMap[((target_cgy * 21) + target_cgx)] == -1)  /* impassible */
        return;








¿ CMB_TargetRows[] ?
// DELETE  #define DEBUG_UNIT_IDX          825
// DELETE  #define DEBUG_FIGURE_SET_IDX    7  // DBG_figure_set_idx: 7
// DELETE  #define DEBUG_UNIT_TYPE         ut_HMenPikemen  // ut_HMenPikemen  = 112,  /* FIGURES8.LBX, 056    HMPIKE*/
// DELETE  #define DEBUG_FIGURE_COUNT      8
...
screen_x, screen_y
grid_x, grid_y
Get_Combat_Grid_Cell_X()
Get_Combat_Grid_Cell_Y()

Q: Where does CMB_TargetRows[] get populated?
A: ¿ Assign_Combat_Grids() ?




        if(-(MapGrid_Control_Index) == input_field_idx)
            RightClick_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
            RightClick_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
            battle_unit_idx = CMB_TargetRows[RightClick_Y][RightClick_X];


Q: What does CMB_TargetRows[] get used for?
A: 







What do I mean by "Battlefield" vs. "Grid"?







ELSEWHERE
draw combat entities


Tactical_Combat__WIP()
    // ...
    |-> CMB_ComposeBackgrnd__WIP()
    // ...
    CMB_DrawFullScreen__WIP()
        CMB_CreateEntities__WIP()
        CMB_DrawMap__WIP()
            Combat_Figure_Compose_USEFULL()
            CMB_DrawEntities__WIP()
                ...
                Draw_Picture_Windowed(CMB_BU_Figure_GFX[])
                ...

        GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];


















CMB_TargetRows[][]
...indexing...
...22 pointers, to 21 bytes each
m x n matrix
y index is m
x index is n
...has to be in half-rows and half-columns...

at sx,sy {  0,  0}, cgc,cgr should be { 0, 0}  CMB_TargetRows[ 0][ 0]
at sx,sy { 15,  0}, cgc,cgr should be { 0, 0}  CMB_TargetRows[ 0][ 0]
at sx,sy {  0, 32}, cgc,cgr should be { 0, 1}  CMB_TargetRows[ 1][ 0]
at sx,sy {  0,164}, cgc,cgr should be { 1,21}  CMB_TargetRows[21][ 1]

first row is unavailable
first col is unavailable
last row is unavailable
last col is unavailable
10 selectable columns
10 selectable rows

in the screenshot,
    the four defending units are at
    {8,12}, {8,11}, {8,13}, {8,10}
    the five attacking units are at
    {14,12}, {14,11}, {14,13}, {14,10}, {15,12}



CMB_SpawnFigure__WIP()
    sets draw type to 1

CMB_CreateEntity__WIP() is what sets draw_x and draw_x_shift, based on what is passed in from CMB_SpawnFigure__WIP()
CMB_SpawnFigure__WIP() uses Combat_Grid_Screen_Coordinates()
    battle_units[itr].position_cgc2, battle_units[itr].position_cgc1



    // void CMB_CreateEntity__WIP(int16_t draw_x, int16_t draw_y, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t DrawType, int16_t controller_idx, int16_t UU_10h, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU_14h, int16_t BldFrm)
    CMB_CreateEntity__WIP(                 Left,           Top,         seg_or_idx,           13,           23,            UU,                1,         controller_idx, figure_set_idx,            outline_magic_realm,         BldAmt,             UU,    Blood_Frame);

combat_grid_entities[combat_grid_entity_count].outline_magic_realm = outline_magic_realm;
    <- CMB_CreateEntity__WIP(..., outline_magic_realm, ...)
        <- CMB_SpawnFigure__WIP(..., battle_units[itr].outline_magic_realm, ...)


CMB_SpawnFigure__WIP() hard-codes combat_grid_entities[].draw_x_shift,draw_y_shift to 13,23





Combat_Grid_Screen_Coordinates()
    screen_x = (((cgc2 - cgc1) * 16) + 158);  /* ¿ + mid x ? */
    screen_y = (((cgc2 + cgc1) *  8) -  80);  /* ¿ + mid y ? */



CMB_DrawEntities__WIP()
combat_grid_entities
combat_grid_entities[IDK_combat_entity_idx].draw_x
combat_grid_entities[IDK_combat_entity_idx].draw_y
combat_grid_entities[IDK_combat_entity_idx].draw_x_shift
combat_grid_entities[IDK_combat_entity_idx].draw_y_shift
CMB_CreateEntity__WIP()


        Starting_Xs[ 0] = 8;
        Starting_Xs[ 1] = 8;
        Starting_Xs[ 2] = 8;
        Starting_Xs[ 3] = 8;

        Starting_Xs[ 0] = 14;
        Starting_Xs[ 1] = 14;
        Starting_Xs[ 2] = 14;
        Starting_Xs[ 3] = 14;
        Starting_Xs[ 4] = 15;

        Starting_Ys[ 0] = 12;
        Starting_Ys[ 1] = 11;
        Starting_Ys[ 2] = 13;
        Starting_Ys[ 3] = 10;
        Starting_Ys[ 4] = 12;


Deploy_Battle_Units()
        battle_units[Melee_Units[itr]].position_cgc2 = Starting_Xs[ctr];
        battle_units[Melee_Units[itr]].position_cgc1 = Starting_Ys[ctr];

CMB_CreateEntities__WIP()
    CMB_SpawnFigure__WIP(battle_units[itr].battle_unit_figure_idx, battle_units[itr].position_cgc2, battle_units[itr].position_cgc1, battle_units[itr].Target_X, battle_units[itr].Target_Y, battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);

CMB_SpawnFigure__WIP()
    Combat_Grid_Screen_Coordinates(PosX, PosY, 0, 0, &Tile_Left, &Tile_Top);
    Combat_Grid_Screen_Coordinates(TarX, TarY, 0, 0, &Target_Left, &Target_Top);
    Left = (((((Target_Left - Tile_Left) * MStage) / 8) + Tile_Left) + fig_x);
    Top = (((((Target_Top - Tile_Top) * MStage) / 8) + Tile_Top) + fig_y);
    CMB_CreateEntity__WIP(Left, Top, seg_or_idx, 13, 23, UU, 1, Owner, figure_set_idx, outline_magic_realm, BldAmt, UU, Blood_Frame);


CMB_CreateEntity__WIP()
    combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;
    combat_grid_entities[combat_grid_entity_count].draw_y = draw_y;
    combat_grid_entities[combat_grid_entity_count].draw_x_shift = draw_x_shift;
    combat_grid_entities[combat_grid_entity_count].draw_y_shift = draw_y_shift;



    Screen_X = (Pointer_X() + 4);  // ¿ why the (+ 4) ?
    Screen_Y = (Pointer_Y() + 4);  // ¿ why the (+ 4) ?
    if(Screen_Y < 168)
    {
        Tile_X = Get_Combat_Grid_Cell_X(Screen_X, Screen_Y);
        Tile_Y = Get_Combat_Grid_Cell_Y(Screen_X, Screen_Y);
        // ...
        // ...
        // ...
    }



        if(-(MapGrid_Control_Index) == input_field_idx)
        {
            RightClick_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
            RightClick_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
            battle_unit_idx = *CMB_TargetRows[((RightClick_Y * 2) + RightClick_X)];
            // ...
            // ...
            // ...
        }



Dratinimaster — 11/07/2024 11:39 AM
(1/2) * (screen_x - 158) + screen_y + 80
----------------------------------------
                               16
= (1/32) * (screen_x - 158) + (1/16) * (screen_y + 80)
= screen_x/32 - 4.9375 + screen_y/16 +5
= screen_x/32 + screen_y/16 +0.0625
= screen_x/32 + screen_y/16 + 1/16









change grid x, based on screen y

grid x = screen x / grid square width
      0 / 32 = 0
     32 / 32 = 1
    319 / 32 = 9.96875

...assuming 32 x 16...
...at screen y ==  0, grid x should be 0
...at screen y == 16, grid x should be 1




`((((screen_x - 158) / 2) + (screen_y + 80)) / 16)`
`(((screen_x - 158) / 2) / 16) + ((screen_y + 80) / 16)`
`(((screen_x - 158) / 32) + ((screen_y + 80) / 16))`
`(((screen_x - 158) / 32) + ((screen_y / 16) + (80 / 16)))`
`(((screen_x - 158) / 32) + ((screen_y / 16) + 5))`
screen_y: {0,...,168} / 16 = {0,...,10.5} + 5 = {5,...,15.5}

((screen_x - 158) / 2)
((screen_x - (SCREEN_XMAX / 2)) / 2)
((screen_x / 2) - ((SCREEN_XMAX / 2) / 2))

{0,...167} / 16 = {0,...,10.4375}
{0,...167} + 80 = {80,...,247} / 16 = {0,...,15.4375}

`319 - 158 = 161 / 2 = 80.5`

`319 / 2 = 159.5; 316 / 2 = 158 / 2 = 79; 159.5 - 79 = 80.5`


```
((screen_x - 158) / 2) + (screen_y + 80)
----------------------------------------
                   16
```

```
((screen_x - 158) / 2)   (screen_y + 80)
---------------------- + ---------------
          16                   16
```

```
(screen_x / 2)   (158 / 2)   screen_y   80
-------------- - --------- + -------- + --
      16             16         16      16
```


Dratinimaster — 11/7/2024 at 11:39 AM
(1/2) * (screen_x - 158) + screen_y + 80
----------------------------------------
                               16
= (1/32) * (screen_x - 158) + (1/16) * (screen_y + 80)
= screen_x/32 - 4.9375 + screen_y/16 +5
= screen_x/32 + screen_y/16 +0.0625
= screen_x/32 + screen_y/16 + 1/16
= (1/32) * (screen_x + 2*screen_y + 2)




// Basic isometric map to screen is:
screen.x = (map.x - map.y) * TILE_WIDTH_HALF;
screen.y = (map.x + map.y) * TILE_HEIGHT_HALF;

screen.x = (map.x - map.y) * TILE_WIDTH_HALF;





screen x {0,...,319} / 32 = {0,...,9.96875}








the first drawn combat grid terrain is a half-height, half-width tile
the screen x,y have to be -(WIDTH / 2) and -(HEIGHT / 2)



WHAT THE FUCK with the -158 and +80?!?!?
...*feels* like it makes some kind of sense with Dratinimaster's rearrangement?
...`= screen_x/32 + screen_y/16 + 1/16`

¿ (360 - 4) / 2 ? ...where the -4 is like the +4 seen elsewhere to make the left and top squares unavailable?






Is the (screen_x - 158) equivalent to (screen_x - (SCREEN_XMAX / 2))?
...makes negative values
So, moving down - no change in screen x - x alternates based on screen y.


0 - 158 = -158
/ 2 = -79
+ 0 = -79
+ 80 = 1
/ 16 = 0

32 - 158 = -126
/ 2 = -63
+ 0 = -63
+ 80 = 17
/ 16 = 1

64 - 158 = -94
/ 2 = -47
+ 0 = -47
+ 80 = 33
/ 16 = 2.0625

319 - 158 = 161
/ 2 = 80.5
+ 0 = 80
+ 80 = 160
/ 16 = 10








change grid y, based on screen x



CMB_TargetRows
is 22 pointers to 21 values
the combat map window is 168 in height
168 / 16 = 10.5
168 / 21 = 8

202412041432:  the check on 168 includes a `+ 4`


e.g., ...
screen x,y {0,0}
combat grid 



width   32
height  16

x_pos = x_screen / 32
y_pos = y_screen / 16

x_grid = x_pos - y_pos
y_grid = x_pos + y_pos

0 / 32 = 0
0 / 16 = 0
0 - 0 = 0
0 + 0 = 0

319 / 32 = 9.96875
167 / 16 = 10.4375
9 - 10 = -1
9 + 10 = 19




((((screen_x - 158) / 2) + (screen_y + 80)) / 16)

0 - 158 = -158
/ 2 = -79
+ 0 = -79
+ 80 = 1
/ 16 = 0

32 - 158 = -126
/ 2 = -63
+ 0 = -63
+ 80 = 17
/ 16 = 1

64 - 158 = -94
/ 2 = -47
+ 0 = -47
+ 80 = 33
/ 16 = 2.0625

319 - 158 = 161
/ 2 = 80.5
+ 0 = 80
+ 80 = 160
/ 16 = 10



`(((screen_y + 80) - ((screen_x - 158) / 2)) / 16)`

(
    ((screen_y + 80) / 16)
    -
    (((screen_x - 158) / 2) / 16)
)

(
    ((screen_y + 80) / 16)
    -
    (
        (
            (screen_x / 2)
            -
            (158 / 2)
        )
    / 16)
)


(
    (screen_y + 80) / 16
    -
    (screen_x - 158) / 32
)

`(((screen_y + 80) / 16) - ((screen_x - 158) / 32))`

(
    ((screen_y + 80) / 16)
    -
    ((screen_x - 158) / 32)
)

`(((screen_y + 80) / GRID_SQUARE_HEIGHT) - ((screen_x - 158) / GRID_SQUARE_WIDTH))`

(
    (
        (screen_y / 16)
        +
        (80 / 16)
    )
    -
    (
        (screen_x / 32)
        -
        (158 / 32)
    )
)

(
    (
        (screen_y / 16)
        +
        (80 / 16)
    )
    -
    (
        (screen_x / 32)
        -
        (158 / 32)
    )
)

`(((screen_y / 16) + (80 / 16)) - ((screen_x / 32) - (158 / 32)))`





0 + 80 = 80
0 - 158 = -158 / 2 = -79
80 - -79 = 159
/ 16 = 9.9375

167 + 80 = 247
0 - 158 = -158 / 2 = -79
247 - -79 = 326
/ 16 = 20.375


(((screen_y + 80) - ((screen_x - 158) / 2)) / 16)
~==
((screen_y + 80) / 16) - (((screen_x - 158) / 2) / 16)
~==

...
((screen_x / 2) - (320 / 4))
...
