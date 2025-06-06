

Cityscape

CITYSCAP.C






Cityscape_Draw__WIP()
    Cityscape_Draw_Background()
    Cityscape_Make_Buildings_Array()
    Cityscape_Roads_1__WIP()
    Cityscape_Draw_Buildings()
    Cityscape_Draw_Wards_And_Walls__STUB()






struct s_CITYSCAPE_CR * cityscape_bldg_array;

cityscape_bldgs = (struct s_BLDG *)Allocate_Next_Block(_screen_seg, 176);
cityscape_bldg_array = (struct s_CITYSCAPE_CR *)Allocate_Next_Block(_screen_seg, (((15 * 23 * sizeof(struct s_BLDG)) / 16) + 1));


BEGIN:  City Walls



## TODO
    the balance of LOADER - GFX_Swap_AppndCtScap__WIP() AKA ~'Reload Cityscape Graphics'
    first three in Cityscape_Draw_Buildings() - oddness with trees, rocks, and something
    IMG_CTY_Scap_Wall
    IMG_CTY_AltarofBtl
    IMG_CTY_EvilPresnc
    IMG_CTY_Dark_Ritual
    IMG_CTY_Astral_Gate
    IMG_CTY_StreamofLif
    IMG_CTY_Earth_Gate
    Cityscape_Draw_Wards_And_Walls()
    IMG_CTY_Rubble
    new building, removed building, sold building  (new building ~== cast city enchantment)
    Vanish_Bitmap()



City Screen
Enemy City Screen
Outpost Screen
Spellcast - City Enchantment


// WZD o54p02
void City_Screen_Draw__WIP(void)

// WZD o55p02
void Enemy_City_Screen_Draw(void)

// WZD o55p04
void Cityscape_Draw_Scanned_Building_Name(int16_t scanned_field, int16_t x_start, int16_t owner_idx)

// WZD o144p02
void Cityscape_Draw__WIP(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2)


City_Screen_Draw__WIP()
    Cityscape_Draw__WIP(_city_idx, 4, 101, cityscreen_city_built_bldg_idx, cityscreen_city_built_bldg_idx);
    Cityscape_Draw_Scanned_Building_Name(city_screen_scanned_field, 1, _CITIES[_city_idx].owner_idx);

Enemy_City_Screen_Draw()
    |-> Cityscape_Draw__WIP(_city_idx, (x_start + 4), (y_start + 101), 0, ST_UNDEFINED);
    |-> Cityscape_Draw_Scanned_Building_Name(_CITIES[_city_idx].owner_idx, x_start, city_screen_scanned_field);

Cast_Spell_City_Enchantment_Animation_Draw()
    |-> Cityscape_Draw__WIP(_osc_city_idx, (x_start + 5),(y_start + 27), _ce_bldg_idx, _ce_bldg_idx);



 1  Cityscape_Draw__WIP()
 2      |-> Cityscape_Draw_Background()
 3      |-> Cityscape_Make_Buildings_Array(city_idx, bldg_idx_1)
 4      |-> Cityscape_Roads_1(xstart, ystart)
 5          |-> Cityscape_Roads_2()
 6              |-> Cityscape_Roads_3()
 7              |-> Cityscape_Roads_4()
 8              |-> Cityscape_Roads_5()
 9      |-> Cityscape_Draw_Buildings(city_idx, xstart, ystart, bldg_idx_1)
10          |-> Cityscape_Add_Bldg_To_Fields_Array()
11      |-> Cityscape_Draw_Wards_And_Walls()


MoO2
    Module: COLONY
        Turn_To_Rubble_CR_Bldg_()
        Draw_Bldg_CR_()

Bldg_Coords_To_Effective_Frame_()
Bldg_Coords_To_Centered_Screen_Coord_()
CR_To_XY_()
Get_Bldg_CR_()

Get_Bldg_Box_()
    |-> CR_To_XY_()

Draw_All_Buildings_()
    |-> Draw_Building_()
        |-> Draw_Bldg_CR_()

Get_Bldg_CR_()
    in-outs random values for a building's CR coordinates



Cityscape_Roads_1    IDK_CityScreen_Cityscape_sCDC32(xstart, ystart)
Cityscape_Roads_2    IDK_CityScreen_Cityscape_sCEB93()
Cityscape_Roads_3    IDK_Cityscape_BldgStruc_sCF555()
Cityscape_Roads_4    IDK_Cityscape_BldgStruc_sCF5FA()
Cityscape_Roads_5    IDK_Cityscape_BldgStruc_Init()






new_bldg_idx
cityscape_background_frame
cityscape_pict_seg
cityscape_bldgs
cityscape_bldg_array
city_wx
city_wy
city_wp
m_cityscape_fortress_city
m_cityscape_summon_city
cityscape_bldg_anim_ctr
cityscape_water_anim_ctr
cityscape_wall_anim_ctr
IDK_cityscape_vanish_percent
cityscape_cr
city_cityscape_field_count


City_Screen__WIP()
    |-> City_Screen_Add_Fields__WIP()
        city_cityscape_field_count = 0
        |-> City_Add_Fields_Buildings()
            city_cityscape_field_count = 0
            cityscape_bldgs[itr].x1,y1,x2,y2
            city_cityscape_fields[city_cityscape_field_count] = Add_Hidden_Field()
            city_cityscape_field_count++
    city_screen_scanned_field = Scan_Input();
City_Screen_Draw__WIP()
    |-> Cityscape_Draw__WIP(_city_idx, 4, 101, cityscreen_city_built_bldg_idx, cityscreen_city_built_bldg_idx);
    |-> Cityscape_Draw_Scanned_Building_Name(city_screen_scanned_field, 1, _CITIES[_city_idx].owner_idx);

CITYSCAP.C:        if(cityscape_bldgs[cityscape_bldg_count].x1 < x_min)
CITYSCAP.C:            x_min = cityscape_bldgs[cityscape_bldg_count].x1;
CITYSCAP.C:        if(cityscape_bldgs[cityscape_bldg_count].x2 > x_max)
CITYSCAP.C:            x_max = cityscape_bldgs[cityscape_bldg_count].x2;

CITYSCAP.C:        cityscape_bldgs[cityscape_bldg_count].x1 = (x + cityscape_cr[type][itr].x1);
CITYSCAP.C:        cityscape_bldgs[cityscape_bldg_count].y1 = (y + cityscape_cr[type][itr].y1);
CITYSCAP.C:        cityscape_bldgs[cityscape_bldg_count].x2 = (x + cityscape_cr[type][itr].x2);
CITYSCAP.C:        cityscape_bldgs[cityscape_bldg_count].y2 = (y + cityscape_cr[type][itr].y2);
CITYSCAP.C:        cityscape_bldgs[cityscape_bldg_count].print_sy = (y + 1);
CITYSCAP.C:        cityscape_bldgs[itr].print_sx = (((x_max + x_min) / 2) - 3);

row 8, column 10 is fortress
cityscape_cr[8][10]



## cityscape_bldgs[]
SEEALSO: MoM-Cityscape-Fields.md

## cityscape_cr[][]{}
SEEALSO: MoM-Cityscape-Fields.md

## Cityscape_Add_Bldg_To_Fields_Array()
SEEALSO: MoM-Cityscape-Fields.md




fortress
Cityscape_Add_Bldg_To_Fields_Array(col_sx, row_sy, bt_Fortress, 4)

NOTE: shape is 4!!!!! there's no 4's in the regular buildings!

¿ col_sx, row_sy are screen x,y ?
Nope. They are center x and bottom y.

Draw_Picture() gets passed col_sx and (row_sy - 47)

width  is ~28
height is ~45

Cityscape_Draw_Scanned_Building_Name()
Print_Centered((cityscape_bldgs[itr].print_sx + 1), (cityscape_bldgs[itr].print_sy - 4), string);

0x0000013030D85490  68 00 a1 68 02 70 d1 20 ee 61 d9 34 a2 00
0x0000013030D8549E  68 00 d1 74 06 73 d7 00 a1 00 d9 34 a2 00
0x0000013030D854AC  68 00 ac 61 15 76 d0 65 c1 4d d9 34 a2 00
0x0000013030D854BA  68 00 a1 68 02 70 d1 20 ee 61 d9 34 a2 00
0x0000013030D854C8  68 00 d1 74 06 73 d7 00 a1 00 d9 34 a2 00
0x0000013030D854D6  68 00 ac 61 15 76 d0 65 c1 4d d9 34 a2 00



## Cityscape_Draw_Buildings()
gets passed bldg_idx, from Cityscape_Draw__WIP()
in Cityscape_Draw__WIP(), that's bldg_idx_1, but, it's not the bldg_idx_1 that was passed into Cityscape_Draw__WIP() from City_Screen_Draw__WIP()




## Cityscape_Roads_1()

Cityscape_Roads_2()
    OON XREF:  Cityscape_Roads_1()
Cityscape_Roads_3()
    OON XREF:  Cityscape_Roads_2()
Cityscape_Roads_4()
    OON XREF:  Cityscape_Roads_2()
Cityscape_Roads_5()
    OON XREF:  Cityscape_Roads_2()

Cityscape_Roads_1()
    |-> Cityscape_Roads_2(cityscape_pict_seg)

Cityscape_Roads_2()
    |-> Cityscape_Roads_3((29 + (itr * 10)), 4, IDK_cityscape_pict_seg);

Cityscape_Roads_2()
    |-> Cityscape_Roads_4(69, 4, IDK_cityscape_pict_seg);




## Cityscape_Add_Bldg_To_Fields_Array()
~== MoO2  CR_To_XY_()

({0,1,2} * 24) + ({0,1,2,3} * 8)
72 + 24 = 96 bytes
136 - 96 = 40
// IDK_cityscape_RC_4x2
// cityscape_cr_136x1[136]
// cityscape_cr_34x4[34][4]
cityscape_cr_24x8[4]



## cityscape_bldg_array



// WZD dseg:CA7A
struct s_CITYSCAPE * cityscape_bldg_array;
allocated in Cityscape_Draw()
44 PR, 704 B
15 of 23 2-byte values

row 8, column 2 is {..., bt_ShipWrightsGuild, bt_ShipYard, bt_MaritimeGuild, ...}

row 8, column 10 is fortress



## new_bldg_idx
set in Cityscape_Draw__WIP(), from the parameter bldg_idx_2

Down P City_Screen_Draw__WIP+231   call    j_Cityscape_Draw__WIP
Down P Enemy_City_Screen_Draw+3AC  call    j_Cityscape_Draw__WIP
Down P Cast_Spell_City_Enchantment_Animation_Draw+30 call    j_Cityscape_Draw__WIP

City_Screen_Draw__WIP(..., cityscreen_city_built_bldg_idx)
Enemy_City_Screen_Draw(..., ST_UNDEFINED)
Cast_Spell_City_Enchantment_Animation_Draw(..., _ce_bldg_idx)

cityscreen_city_built_bldg_idx
    City_Built_Building_Message()
...
    ```"The [city size] of [] has completed the construction of [|a|an] ..."```
...
mov     ax, [bp+bldg_idx]
mov     [cityscreen_city_built_bldg_idx], ax

XREF:
    Cityscape_Draw__WIP+20                         mov     [new_bldg_idx], ax
    Cityscape_Draw_Buildings+251 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+2C5 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+324 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+395 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+406 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+476 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+4F0 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+560 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+5D1 cmp     ax, [new_bldg_idx]
    Cityscape_Draw_Buildings+665 cmp     ax, [new_bldg_idx]



## cityscreen_city_built_bldg_idx

XREF:
    City_Screen__WIP:@@Leave_Screen mov     [cityscreen_city_built_bldg_idx], e_ST_UNDEFINED
    City_Screen_Draw__WIP+214       cmp     [cityscreen_city_built_bldg_idx], e_ST_UNDEFINED
    City_Screen_Draw__WIP:loc_48FCC push    [cityscreen_city_built_bldg_idx]                
    City_Screen_Draw__WIP+221       push    [cityscreen_city_built_bldg_idx]                
    City_Built_Building_Message+254 mov     [cityscreen_city_built_bldg_idx], ax            

## _ce_bldg_idx

{ 0, 1, 100, 101, 102, 103, 104, 105, 106 }

XREF:
    IDK_Spell_DisjunctOrBind_Draw+25F     cmp     [_ce_bldg_idx], 1                            
    IDK_Spell_Cityscape_2:loc_BF2D6       mov     [_ce_bldg_idx], 0                            
    IDK_Spell_DisjunctOrBind+25           mov     [_ce_bldg_idx], 0                            
    IDK_Spell_Cityscape_1:loc_BF0FA       mov     [_ce_bldg_idx], 0
    IDK_Spell_DisjunctOrBind__1+210       mov     [_ce_bldg_idx], 1                            
    IDK_Spell_DisjunctOrBind__1+2EF       mov     [_ce_bldg_idx], 1                            
    IDK_Spell_DisjunctOrBind__2:loc_BE84C mov     [_ce_bldg_idx], 1                            
    IDK_Spell_Cityscape_1:loc_BF0E2       mov     [_ce_bldg_idx], 100               ; case 0xCB
    IDK_Spell_Cityscape_1:loc_BF0CA       mov     [_ce_bldg_idx], 101               ; case 0x25
    IDK_Spell_Cityscape_1:loc_BF0D2       mov     [_ce_bldg_idx], 102               ; case 0x94
    IDK_Spell_Cityscape_1:loc_BF0DA       mov     [_ce_bldg_idx], 103               ; case 0x99
    IDK_Spell_Cityscape_1:loc_BF0C2       mov     [_ce_bldg_idx], 104               ; case 0x1C
    IDK_Spell_Cityscape_1:loc_BF0EA       mov     [_ce_bldg_idx], 105               ; case 0xA3
    IDK_Spell_Cityscape_1:loc_BF0F2       mov     [_ce_bldg_idx], 106               ; case 0x92
    Cast_Spell_City_Enchantment_Animation_Draw+16           push    [_ce_bldg_idx]                               
    Cast_Spell_City_Enchantment_Animation_Draw+1A           push    [_ce_bldg_idx]                               



## IDK_Clear_Cityscape_Vanish_Percent()

XREF:
    j_IDK_Clear_Cityscape_Vanish_Percent()
        City_Built_Building_Message()
        IDK_Spell_Cityscape_1()

```IDK_cityscape_vanish_percent = 0;```

    IDK_Clear_Cityscape_Vanish_Percent+3           mov     [IDK_cityscape_vanish_percent], 0        
    Cityscape_Draw__WIP+35B                        cmp     [IDK_cityscape_vanish_percent], 100      
    Cityscape_Draw__WIP+362                        add     [IDK_cityscape_vanish_percent], 10       
    Cityscape_Roads_1+77E                          mov     ax, [IDK_cityscape_vanish_percent+bx]    
    Cityscape_Draw_Buildings+257 push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+2CB push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+32A push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+39B push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+40C push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+47C push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+4F6 push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+566 push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+5D7 push    [IDK_cityscape_vanish_percent]  ; percent
    Cityscape_Draw_Buildings+66B push    [IDK_cityscape_vanish_percent]  ; percent









## Cityscape_Draw__WIP()

// WZD o144p02
void Cityscape_Draw__WIP(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2)

    Cityscape_Draw_Background(city_idx, xstart, ystart, city_wx, city_wy, city_wp);

    Cityscape_Make_Buildings_Array(city_idx, bldg_idx_1);

    Cityscape_Roads_1(xstart, ystart);

    Cityscape_Draw_Buildings(city_idx, xstart, ystart, bldg_idx_1);

    Cityscape_Draw_Wards_And_Walls__STUB(city_idx, xstart, ystart);







## Cityscape_Draw_Background()
draws background
handles plane, river, ocean, mountain, hills
¿ fortress ?
¿ enchantments ?


## Cityscape_Make_Buildings_Array()

// WZD o144p11
void Cityscape_Make_Buildings_Array(int16_t city_idx, int16_t bldg_idx)

XREF:
    Cityscape_Draw__WIP()
    NX_j_Cityscape_Set_BldgStruc__WIP()

Cityscape_Make_Buildings_Array(city_idx, bldg_idx_1)

bldg_idx
    e.g.,
    (m_cityscape_fortress_city == ST_TRUE) || (bldg_idx == 104)

doesn't do 4 incr's for buildings 13, 12, 14
    bt_ShipWrightsGuild   =  12,
    bt_ShipYard           =  13,
    bt_MaritimeGuild      =  14,


4 sets of columns and rows

trees are -3
rocks are -4

¿ 6 arrays of 8 and 2 arrays of 14 ?


¿ bldg_data_table[].G_Animation is *type* of {2x2,2x3,3x2,3x3} ?

Alchemists Guild is 2x2
Armorers Guild is 2x3
Parthenon is 3x2
War College is 3x3
¿ water buildings are 1x3 ?




## Cityscape_Roads_1()

## Cityscape_Draw_Buildings()

iterates over 15 rows and 23 columns, but skips the first column
¿ 15 columns at 14 pixels each ?
¿ with 1 pixel pad on either side - hence the -2 at column boundaries {4,8,13} ?

* 10 in the x direction AKA columns
* 5 in the y direction AKA rows

row_sy = ((itr_row - 1) * 5);
row_sy += (y_start + 27);
...27 + (5 * row)
...looks like Sages Guild starts at 37; ...(27 + (5 * (3 - 1))) = (27 + (5 * 2)) = (27 + 10) = 37

col_sx = (itr_col * 10);
col_sx += ((x_start + 27) - (row_sy - y_start - 27));

...looks like Sages Guild starts at 16


the 27 is
    7 + 4 rows and 7 + 2 columns
or
    2 + 5 rows and 7 + 2 columns


x_start,y_start 4,101

row 8, col 2

shipyard
16,60
20,161
or
13,59

277,355
138.5, 177.5

  4 + 134
101 +  76 = 101 + 27 + (7 * 5) 

(4 + 27) - (101 - 27 - )





## Cityscape_Draw_Wards_And_Walls__STUB()







### Cityscape_Draw_Scanned_Building_Name()
XREF:
    Enemy_City_Screen_Draw()
    j_IDK_City_Cityscape_Draw_MouseOver()
        City_Screen_Draw__WIP()



#### _ce_bldg_idx
XREF:
    IDK_Spell_DisjunctOrBind_Draw+25F     cmp     [_ce_bldg_idx], 1                            
    IDK_Spell_DisjunctOrBind+25           mov     [_ce_bldg_idx], 0                            
    IDK_Spell_DisjunctOrBind__1+210       mov     [_ce_bldg_idx], 1                            
    IDK_Spell_DisjunctOrBind__1+2EF       mov     [_ce_bldg_idx], 1                            
    IDK_Spell_DisjunctOrBind__2:loc_BE84C mov     [_ce_bldg_idx], 1                            
    Cast_Spell_City_Enchantment_Animation_Draw+16           push    [_ce_bldg_idx]                               
    Cast_Spell_City_Enchantment_Animation_Draw+1A           push    [_ce_bldg_idx]                               
    IDK_Spell_Cityscape_1:loc_BF0C2       mov     [_ce_bldg_idx], 104               ; case 0x1C
    IDK_Spell_Cityscape_1:loc_BF0CA       mov     [_ce_bldg_idx], 101               ; case 0x25
    IDK_Spell_Cityscape_1:loc_BF0D2       mov     [_ce_bldg_idx], 102               ; case 0x94
    IDK_Spell_Cityscape_1:loc_BF0DA       mov     [_ce_bldg_idx], 103               ; case 0x99
    IDK_Spell_Cityscape_1:loc_BF0E2       mov     [_ce_bldg_idx], 100               ; case 0xCB
    IDK_Spell_Cityscape_1:loc_BF0EA       mov     [_ce_bldg_idx], 105               ; case 0xA3
    IDK_Spell_Cityscape_1:loc_BF0F2       mov     [_ce_bldg_idx], 106               ; case 0x92
    IDK_Spell_Cityscape_1:loc_BF0FA       mov     [_ce_bldg_idx], 0                 ; default  
    IDK_Spell_Cityscape_2:loc_BF2D6       mov     [_ce_bldg_idx], 0                            
