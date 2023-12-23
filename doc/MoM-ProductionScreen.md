
# Production Screen

Pop-Up

uses current screen as its background
gets its palette from UNITVIEW.LBX, 0  UNITBACK    small unit backgrn
different set of graphics than 'City Screen'
same set of graphics as 'Main Screen'
similar list logic as armies list, cities list, city enchantments list, etc.
shares 'unit view' code for the center pane


Data Segments
Strings
    WZD dseg:2D72
    WZD dseg:2E06
Uninitialized Data
    WZD dseg:BFF2
    WZD dseg:C03C
Initialized Data



Build Table
Specials List


    if(product_indexes[current_item] < 100)
    {
        LBX_Load_Data_Static(buildesc_lbx_file, 0, (SAMB_ptr)building_description, product_indexes[current_item], 1, 200);
    }
    else
    {
        active_product_idx = (product_indexes[current_item] - 100);
        /* TODO */  UV_Create_Blank_CombatUnit(active_product_idx, Active_Unit);
        /* TODO */  UV_Build_Effect_List__WIP(active_product_idx, IDK_ProdScr_UV_Effect_List, &UV_specials_count);
    }


## Build Table
    array of product_idx of buildings and normal units
    left/bldg & right/unit
    max 20
    Calculate_Product_Array()
    

## Specials List

Production_Screen()
    |-> GFX_Swap_Overland()
    |-> GFX_Swap_AppendUView()
        145 unit specials icons
        special_seg[itr] = LBX_Reload_Next(special_lbx_file, itr, GFX_Swap_Seg);
    |-> GFX_Swap_AppendItems__WIP()







build table list contains both building and units
¿ buidling count, unit count, total count ?

if the city contains 9 units, it's *full*
    and the unit type names are greyed out
    and you get a warning when you left-click on a unit field
    so, you can't select another one
    but, if you're already buidling one it'll keep going


¿ field count vs. list item count ?



F/T flag for unit count in the city is at the max of 9
shared with other unrelated code - i.e., not ProdScr specific
maybe city specific?
like others with unit_array and unit_array_count?
~ Active_UnitGroup()  no, that'n only touch the stack
~ TILE_GetUnits()
~ TILE_GetUnits_2()

ovr059
CTY_GetGarrison()
    city wp vs. unit wp
    city wx vs. unit wx
    city wy vs. unit ww
    city owner_idx vs. unit owner_idx
NOTE: not checking for in-tower is irrrelavent, as it is inherent that there is no tower where there is a city
so, this's equivalent to either
no idea why they wouldn't just pass the wx,wy,wp of the city
    just owned units?
is there even another that takes player_idx?
checking owner_idx precludes it being -1?
    can you have dead cities?
    ghost cities have pop 0

ovr120
TILE_GetUnits()
    wx and wy
    owner_idx -1
    wp

ovr059
TILE_GetUnits_2()
    wp or in tower
    wx and wy
    owner_idx -1


wx, wy, wp, &unit_count, unit_array

base-line
    wx and wy and wp or in-tower

owner_idx -1 is just dead unit in these contexts?




## Coordinates & Dimensions

start x,y for pop-up/screen

x,y for left list
x,y for right list

w for list item

h for list item draw
h for list item field

x,y for Cancel button
x,y for OK button

x,y for product picture

x,y start for 'Thing View'
    (production_screen_xstart + build_table_item_xadd)
    production_screen_ystart

Production_Screen()
    production_screen_xstart = 0;
    production_screen_ystart = 0;

    build_table_item_xadd = 74;
    build_table_item_yadd = 11;
    build_table_item_height = 14;

    IDK_ProdScr_xadd_90 = 90;
    IDK_ProdScr_yadd_28 = 28;

    IDK_ProdScr_w42AC0 = 102;
    IDK_ProdScr_w42ABE = 55;

    built_table_right_xstart = production_screen_xstart + 240;
    built_table_right_ystart = production_screen_ystart + 4;
    built_table_left_xstart  = production_screen_xstart;
    built_table_left_ystart  = production_screen_ystart + 4;

Production_Screen_Add_Fields()
    x1 = built_table_right_xstart;
    y1 = (built_table_right_ystart + (unit_item_cnt * build_table_item_height));
    x2 = x1 + build_table_item_xadd - 1;
    y2 = y1 + build_table_item_yadd - 1;
    production_screen_build_table_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2, 0, -1);
    UV_Add_Effect_Fields((production_screen_xstart + 85), (production_screen_ystart + 108), IDK_ProdScr_UV_Effect_List, UV_Effect_Count, UV_ListItem);


Production_Screen_Draw_()
    xstart = production_screen_xstart;
    ystart = production_screen_ystart;
    UU_x2 = xstart + IDK_ProdScr_xadd_90;
    UU_y2 = ystart + IDK_ProdScr_yadd_28;
    var_8 = xstart + IDK_ProdScr_w42AC0;
    var_A = ystart + IDK_ProdScr_w42ABE;
    Thing_View_Draw__WIP((production_screen_xstart + build_table_item_xadd), production_screen_ystart


Thing_View_Draw__WIP()
    UV_x_start = x_start;
    UV_y_start = y_start;
    if(USW_TransparentBase == ST_TRUE)
        Fill(UV_x_start, UV_y_start, (UV_x_start + 281), (UV_y_start + 183), ST_TRANSPARENT);
    if(CalledFromFlag == 0)
        FLIC_Draw(UV_x_start, UV_y_start, unitview_small_background_seg);
    else
        FLIC_Draw(UV_x_start, UV_y_start, unitview_large_background_seg);
        if(CalledFromFlag != 2)
            FLIC_Draw((UV_x_start + 213), (UV_y_start + 133), unitview_button_background_seg);
    ...
    UV_x_start_offset = 0;
    if(CalledFromFlag == 0)
        UV_x_start_offset = 4;
        Print((UV_x_start + UV_x_start_offset + 51), (UV_y_start + 34), "Cost");
        if(View_Type == 1)
            ... "(" _unit_type_table[unit_type_idx].Cost ")"
            Print((UV_x_start + UV_x_start_offset + 73), (UV_y_start + 34), GUI_NearMsgString);











WIZARDS.EXE
ovr056

Data Segments
Strings
Uninitialized Data
Initialized Data


"bldtab"


##### Naming Things Is Hard

"production window"
"building project"

"production screen"
"project"
"current project"
"building project"
"new project"
"structure"
"production points"
"surplus production units"

Page 57  (PDF Page 62)

BUYING OR CHANGING PRODUCTION
In the lower right section of the city screen is the production window.  
The building, unit or other item you are currently producing is shown in this area, along with how many turns remain before the project is completed.  
The amount of production (in terms of production points) already invested in the project is graphically shown by gold coins in this window.  
The amount of production remaining before the project is completed is shown as darkened coins.  

Page 58  (PDF Page 63)

There are three buttons below the production window:  
· Buy:  
Clicking on the buy button allows you to pay to complete your current project — if you have enough gold in your treasury.  
The more you try to speed up a building project, however, the more you have to pay to get it done (all that overtime work...).  
So, you have to pay more for each remaining (i.e., needed) “production point” to complete a building that you just started than for one that is already half completed.  
· Change:  
Clicking on the change button takes you to the production screen (see next section), where you can decide which project to work on next.  
Note that you do not lose the effort you have placed into any current project; those “production points” get diverted into your new project.  
However, surplus production units (if you change production to a structure that “costs” less to build) will be lost.  
· OK:  
Clicking on this button returns you to the main movement screen.  
