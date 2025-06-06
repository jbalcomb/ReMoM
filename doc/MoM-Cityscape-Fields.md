


Cityscape_Draw__WIP()
    Cityscape_Set_BldgStruc__WIP()
    Cityscape_Draw_Buildings_And_Features__WIP()
    Cityscape_Add_Bldg_To_Fields_Array()
Later, ...
    CityScr.C
    City_Screen__WIP()
        City_Screen_Add_Fields__WIP()
            Cityscape_Add_Fields()
        Cityscape_Draw_Scanned_Building_Name()



## Cityscape_Add_Bldg_To_Fields_Array()
takes col_sx, row_sy, bldg_idx, shape


from Cityscape_Draw_Buildings_And_Features__WIP()
iterating over rows and columns
calculated the center x and bottom y, in *final* screen coordinates
cr_bldg_idx = cityscape_bldg_array->cr[itr_row][itr_col].bldg_idx;
Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, cr_bldg_idx, bldg_data_table[cr_bldg_idx].G_Animation);
Now, ...
...add *this* building to cityscape_bldgs...




## Cityscape_Add_Bldg_To_Fields_Array()

NOTE: Fortress manually specifies shape of 4!!!!! there's no 4's in the regular buildings!

Cityscape_Draw_Scanned_Building_Name()
Print_Centered((cityscape_bldgs[itr].print_sx + 1), (cityscape_bldgs[itr].print_sy - 4), string);

0x0000013030D85490  68 00 a1 68 02 70 d1 20 ee 61 d9 34 a2 00
0x0000013030D8549E  68 00 d1 74 06 73 d7 00 a1 00 d9 34 a2 00
0x0000013030D854AC  68 00 ac 61 15 76 d0 65 c1 4d d9 34 a2 00
0x0000013030D854BA  68 00 a1 68 02 70 d1 20 ee 61 d9 34 a2 00
0x0000013030D854C8  68 00 d1 74 06 73 d7 00 a1 00 d9 34 a2 00
0x0000013030D854D6  68 00 ac 61 15 76 d0 65 c1 4d d9 34 a2 00



## cityscape_bldgs[]

Enemy_City_Screen_Draw()
    uses cityscape_bldgs[].bldg_idx to set the help
City_Add_Fields_Buildings()
    uses cityscape_bldgs[].x1,y1,x2,y2 for Add_Hidden_Field() into city_cityscape_fields[]
Cityscape_Draw_Scanned_Building_Name()
    uses cityscape_bldgs[].print_sx, print_sy to 


## cityscape_cr[][]{}

only XREF'd in Cityscape_Add_Bldg_To_Fields_Array()

indexed by shape and itr
shape is {0, 1, 2, 3}
itr is {0, 1, 2}

        cityscape_bldgs[cityscape_bldg_count].x1 = (x + cityscape_cr[type][itr].x1);
        cityscape_bldgs[cityscape_bldg_count].y1 = (y + cityscape_cr[type][itr].y1);
        cityscape_bldgs[cityscape_bldg_count].x2 = (x + cityscape_cr[type][itr].x2);
        cityscape_bldgs[cityscape_bldg_count].y2 = (y + cityscape_cr[type][itr].y2);

NOTE: city walls doesn't use this, it manually populates it's values in cityscape_bldgs

enum e_BUILDING_SHAPE
    bp_2x2 = 0,
    bp_2x3 = 1,
    bp_3x2 = 2,
    bp_3x3 = 3

e.g., Stables  3x3
84,184
        /* 3x3 */
        {  0, -14,  29,   0 },
        {  6, -19,  34,  -5 },
        { 11, -25,  39, -11 },
itr == 0:
    x1:  84 +   0 =  84
    y1: 184 + -14 = 170
    x2:  84 +  29 = 113
    y2: 184 +   0 = 184
    x1_min:  84
    x2_max: 113
itr == 1:
    x1:  84 +   6 =  90
    y1: 184 + -19 = 165
    x2:  84 +  34 = 118
    y2: 184 +  -5 = 179
    x1_min:  84
    x2_max: 118
itr == 2:
    x1:  84 +  11 =  95
    y1: 184 + -25 = 159
    x2:  84 +  39 = 123
    y2: 184 + -11 = 173
    x1_min:  84
    x2_max: 123
