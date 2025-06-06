


MoO2
    code (0 bytes) Pick_Random_Veggie_Anim_For_Colony_CR_
    code (0 bytes) Pick_Random_Veggie_Anim_Entry_For_Colony_CR_
    code (0 bytes) N_Roads_Around_CR_
    code (0 bytes) Pick_Random_Road_At_CR_
    code (0 bytes) Pick_Random_Road_Around_CR_
    code (0 bytes) Pick_Random_Road_At_And_Around_CR_
    code (0 bytes) Draw_Veggie_CR_
    code (0 bytes) Turn_To_Rubble_CR_Bldg_
    code (0 bytes) CR_Building_Name_
    code (0 bytes) Draw_Bldg_CR_
    code (0 bytes) Get_Bldg_CR_
    code (0 bytes) CR_To_XY_
    code (0 bytes) Scan_Bldg_CR_
    code (0 bytes) Draw_Bldg_CR_Dark_

Make_Bldg_Array_For_Colony_()
    |-> Insert_Bldg_Into_Array_()

_colony_bldgs__COLONY





cityscape_bldg_array[][]
    stores type, but also bldg_idx





## Cityscape_Make_Buildings_Array()
    clears the array ... cityscape_bldg_array[] = 0
    reserves cells for corners, fortress, and roads

    counts of buildings by shape type
        2x2,3x2,2x3,3x3
        iters over normal buildings
            ...skips if not built and not the new building or is a boat building
        ...where does this get used?
        ...what is it used for?
    reserves cells for boat building
    does building placements - 3x3, 3x2, 2x3, 2x2, houses, trees, rocks
        sets cols,rows in arrays by type
        sets value in building array - 1, 2, -3, -4 - building, house, tree, rock

set bldg_idx in cityscape_bldg_array
    ...magic buildings, normal buildings, Fortress, boat building







### Building Placement
~ largest to smallest/hardest to easiest, in order of 3x3,2x3,3x2,2x2

What's with the random component?
    ...not the same for each group
        3x3:    if(rnd_loc < 30) { rnd_loc++; }
        3x2:    if(rnd_loc < 30) { if(Random(3) == 1) { rnd_loc++; } }
        2x3:    if(rnd_loc < 30) { if(Random(3) == 1) { rnd_loc++; } }
        2x2:    if(rnd_loc < 30) { if(Random(3) == 1) { rnd_loc++; } }
        houses: if(rnd_loc < 30) { if(Random(4) == 1) { rnd_loc++; } }
        trees:  if(rnd_loc < 30) { if(Random(4) == 1) { rnd_loc++; } }
        rocks:  if(rnd_loc < 30) { if(Random(4) == 1) { rnd_loc++; } }
rnd_loc = 3;
...
col = (10 + (Random((rnd_loc + 1))) - (rnd_loc / 2));
row = ( 6 + (Random((rnd_loc + 1))) - (rnd_loc / 2));
...
if(rnd_loc < 30) { rnd_loc++; }


BEGIN:  building_shape_2x2_count
rnd_loc = 3;
...1:3 chance of ++, up to 30

...10 + {1, ..., 31} - {3, ... 30} / 2
...{11, ..., 41} - {1, ..., 15}
...{10, ..., 26}
col = ((Random((rnd_loc + 1)) + 10) - (rnd_loc / 2));

...6 + {1, ..., 31} - {3, ... 30} / 2
...{7, ..., 37} - {1, ..., 15}
...{3, ..., 13}
row = ((Random((rnd_loc + 1)) +  6) - (rnd_loc / 2));

if((col >= 0) && (col < (NUM_CITYSCAPE_COLS - 2)) && (row >= 0) && (row < (NUM_CITYSCAPE_ROWS - 2)))

rnd_loc lets it progressively expand the width and height of the location selection?
thereby, prioritizing placement? ...looks to be along the right and bottom

col = ( (10 + Random( (3 + 1) ) ) - (3 / 2) )
col = ( (10 + Random(4)         ) - (3 / 2) )
col = ( (10 + 1                 ) - (3 / 2) )
col = ( (11                     ) - (1    ) )
col = 10
row = ( ( 6 + Random( (3 + 1) ) ) - (3 / 2) )
row = ( ( 6 + Random(4)         ) - (3 / 2) )
row = ( ( 6 + 1                 ) - (3 / 2) )
row = ( ( 7                     ) - (1    ) )
row = 6


blocks of random business
    rnd_loc = 3;
    ...
    col = ((Random((rnd_loc + 1)) + 10) - (rnd_loc / 2));
    row = ((Random((rnd_loc + 1)) +  6) - (rnd_loc / 2));
    ...
    if(rnd_loc < 30)
    {
        rnd_loc++;
    }
    So, ...
        ... 3 through 29 ...
        (Random(3 + 1) + 10) ... (Random(4) + 10) ... {11,12,13,14}
        (Random(3 + 1) +  6) ... (Random(4) +  6) ... { 7, 8, 9,10}
        (3 / 2) ... {1}
        {11,12,13,14} - {1} ... {10,11,12,13}
        { 7, 8, 9,10} - {1} ... { 6, 7, 8, 9}
        through
        (Random(29 + 1) + 10) ... (Random(30) + 10) ... {11,...,40}
        (Random(29 + 1) +  6) ... (Random(30) +  6) ... {11,...,36}
        (29 / 2) ... {14}
        {11,...,40} - {14} ... {-3,...,26}
        {11,...,36} - {14} ... {-3,...,22}
    Then,
        if((col >= 0) && (col < 20) && (row >= 0) && (row < 12))
        if((col >= 0) && (col < 20) && (row >= 0) && (row < 13))
        if((col >= 0) && (col < 21) && (row >= 0) && (row < 12))
        if((col >= 0) && (col < 21) && (row >= 0) && (row < 13))
        if((col >= 0) && (col < 23) && (row >= 0) && (row < 15))
        ...
        *((int16_t *)cityscape_bldg_array + ((row + itr1) * sizeof(struct s_CITYSCAPE)) + (col + itr2)) = 1;


4 counts, based on bldg_data_table[IDK_itr_bldg].G_Animation {0,1,2,3,4}
while(IDK_itr_bldg < incr0)
where IDK_itr_bldg gets incremented every time one gets added, given random lands and nothing is already there
maybe max 30, and incr1 and incr2 have an additional 1/3 check
















#define NUM_CITYSCAPE_ROWS         15
#define NUM_CITYSCAPE_COLS         23

Cityscape_Make_Buildings_Array()
    for(itr_row = 1; itr_row < NUM_CITYSCAPE_ROWS; itr_row++)
        for(itr_col = 0; itr_col < NUM_CITYSCAPE_COLS; itr_col++)
            cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx = 0;

    if((m_cityscape_fortress_city == ST_TRUE)||(bldg_idx == bt_Fortress))
        for(itr2 = 10; itr2 < 13; itr2++)
            for(itr1 = 6; itr1 < 9; itr1++)
                cityscape_bldg_array->cr[itr1][itr2].bldg_idx = 1;
        for(itr1 = 1; itr1 <= 2; itr1++)
            cityscape_bldg_array->cr[itr1][7].bldg_idx = -1;
        for(itr1 = 1; itr1 <= 2; itr1++)
            cityscape_bldg_array->cr[itr1][8].bldg_idx = -1;
        for(itr1 = 3; itr1 <= 4; itr1++)
            cityscape_bldg_array->cr[itr1][10].bldg_idx = -1;

if(m_cityscape_fortress_city != ST_FALSE)
    cityscape_bldg_array->cr[8][10].bldg_idx = bt_Fortress;

// {..., bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild, ...}
for(itr_bldg = bt_ShipWrightsGuild; itr_bldg <= bt_MaritimeGuild; itr_bldg++)
    cityscape_bldg_array->cr[8][2].bldg_idx = itr_bldg;

    // {..., bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild, ...}
    // sets *occuppied* for...
    // {{6,2},{6,3},{7,2},{7,3},{8,2},{8,3}}

[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][]
[][][][][][][][][][][1][1][1]
[][][][][][][][][][][1][1][1]
[][][][][][][][][][][1][1][1]

 5 pixels per row     (15 *  5 =  75)
10 pixels per column  (23 * 10 = 230)
whole window is 205 x 96?


fortress is 3x3
summoning circle is 3x2

For { bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild }
    sets { {6,2}, {6,3}, {7,2}, {7,3}, {8,2}, {8,3} } as occupied
    sets {8,2} as bldg_idx  (right, bottom)

For bt_Fortress
    sets { {10,6}, {10,7}, {10,8}, {11,6}, {11,7}, {11,8}, {12,6}, {12,7}, {12,8}, } as occupied
    sets {10,8} as bldg_idx  (left, bottom)



places buildings
    in order of 3x3,2x3,3x2,2x2


cityscape_bldg_array->cr[][].bldg_idx
{-2, -1, 0, 1, ...}
-2, like on the other grids? ~invalid/illegal
¿ nothing ever checks for -2 ?
¿ nothing ever checks for -1 ?
¿ nothing ever checks for  1 ?
