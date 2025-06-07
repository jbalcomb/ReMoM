


Cityscape_Roads_1__WIP()
Cityscape_Roads_2__WIP()
Cityscape_Roads_3__WIP()
Cityscape_Roads_4__WIP()
Cityscape_Roads_5__WIP()

cityscape_roads_cr[]



Cityscape_Draw__WIP()
    Cityscape_Draw_Background()
    Cityscape_Make_Buildings_Array()
    Cityscape_Roads_1__WIP()
    Cityscape_Draw_Buildings()
    Cityscape_Draw_Foreground()



MoO2
    Module: COLDRAW
        code (0 bytes) N_Roads_Around_CR_
        code (0 bytes) Pick_Random_Road_At_CR_
        code (0 bytes) Pick_Random_Road_Around_CR_
        code (0 bytes) Pick_Random_Road_At_And_Around_CR_




row  0, 5,  9, 14  road A, B, C, D
col  4, 9, 13, 18  road 1, 2, 3, 4

[ 0][ 4] intersection A1
[ 5][ 4] intersection B1
[ 9][ 4] intersection C1
[14][ 4] intersection D1

[ 0][ 9] intersection A2
[ 5][ 9] intersection B2
[ 9][ 9] intersection C2
[14][ 9] intersection D2

[ 0][13] intersection A3
[ 5][13] intersection B3
[ 9][13] intersection C3
[14][13] intersection D3

[ 0][18] intersection A4
[ 5][18] intersection B4
[ 9][18] intersection C4
[14][18] intersection D4

DNE [0][19] road #A nub right
[ 5][19] road #B nub right
[ 9][19] road #C nub right
[14][19] road #D nub right


    // #25 CR3
    for(itr1 = 19; itr1 < 22; itr1++)
    {
        if(cityscape_bldg_array->RC[9][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-21 + (itr1 * 10)), 45, cityscape_pict_seg);
        }
    }
    // #33 CR3
    for(itr1 = 19; itr1 < 23; itr1++)
    {
        if(cityscape_bldg_array->RC[14][itr1] == -2)
        {
            Cityscape_Roads_3__WIP((-44 + (itr1 * 10)), 68, cityscape_pict_seg);
        }
    }




rows
cityscape_bldg_array->cr[0][itr2].bldg_idx = -2;
cityscape_bldg_array->cr[5][itr2].bldg_idx = -2;
cityscape_bldg_array->cr[9][itr2].bldg_idx = -2;
cityscape_bldg_array->cr[14][itr2].bldg_idx = -2;

cols
cityscape_bldg_array->cr[itr1][4].bldg_idx = -2;
cityscape_bldg_array->cr[itr1][9].bldg_idx = -2;
cityscape_bldg_array->cr[itr1][13].bldg_idx = -2;
cityscape_bldg_array->cr[itr1][18].bldg_idx = -2;



## Cityscape_Roads_1__WIP()

...block...block...block...block...block...

block
    check intersections

block
    set background, draw roads pict to cityscape bitm
    draw roads mask to global bitm
    whatever it does - Cityscape_Roads_2__WIP()
    draw cityscape bitm to screen buffer

## Cityscape_Roads_2__WIP()

## Cityscape_Roads_3__WIP()
## Cityscape_Roads_4__WIP()
## Cityscape_Roads_5__WIP()
...just touch cityscape_pict_seg
...seem to be manually editing pixels
