

Calculate_Product_Array()

SAVETEST
York
High Men
War College
Maritime Guild
Wizards Guild
Parthenon
City Walls
Trireme
Galley
Spearmen
Swordsmen
Bowmen
Cavalry
Engineers
Settlers
Pikemen

Cant Build?
    Fantastic Stable

Requires
    War College
        Armorer’s Guild; Fighter’s Guild; Armory; Barracks, Smithy;
        University; Sage’s Guild, Library; Library; Builder’s Hall;

bt_WarCollege         =   7,
bt_ArmorersGuild      =   6,
bt_FightersGuild      =   5,
bt_Armory             =   4,
bt_Barracks           =   3,
bt_Smithy             =   8,

bt_University         =  20,
bt_SagesGuild         =  17,
bt_Library            =  16,
bt_BuildersHall       =  32,



Production_Screen()
    |-> Build_Table_List()




Build_Table_List()
    int/out
        total count
        total count
        unit count
        product idx array

probably second total count had been building count
unit type indices are +100
*embedded info* rest of code *knows* that units are first in the build table list



    used in fields to iter over build_table_list
        itr used to index build_table_list[]
        itr used to index production_screen_build_table_fields[]
    used in draw to iter over build_table_list
        cmp'd to build_table_list_item
        itr used to index build_table_list[]

depr_n_product_indexes
    used to iter for match of input to build table fields
        sets build_table_list_item to itr
        sets build_table_product_idx to build_table_list[build_table_list_item]
    used in draw
        inside of loop over n_product_indexes
            if itr >= depr_n_product_indexes
        so, ...
            somehow n_product_indexes could be greater than depr_n_product_indexes
        but, as-coded, ...
            this is not possible
    

n_military_indexes
    used in match of input to build table fields
        if garrison full and itr < count
            "...already too many units..."
    used in draw of build table items/entries/fields
        if garrison full and itr < count
            grayed-out, text: colors[0] = 4; colors[1] = 5;
garrison_full
    used in match of input to build table fields
        if garrison full and itr < count
            "...already too many units..."
    used in draw of build table items/entries/fields
        if garrison full and itr < count
            grayed-out, text: colors[0] = 4; colors[1] = 5;
So, ...
    garrison_full & n_military_indexes
    only used together
    means geyed out and warn cant select/build



dseg:BFF8 00 00                                           build_table_list dw 0                   ; DATA XREF: Production_Screen+73r ...

dseg:BFFC 00 00                                           n_military_indexes dw 0                ; DATA XREF: Production_Screen+77o ...
dseg:BFFE 00 00                                           depr_n_product_indexes dw 0             ; DATA XREF: Production_Screen+7Bo ...
dseg:C000 00 00                                           n_product_indexes dw 0            ; DATA XREF: Production_Screen+7Fo ...

dseg:C012 00 00                                           production_screen_build_table_fields dw 0
dseg:C014 00 00                                           built_table_left_ystart dw 0            ; DATA XREF: Production_Screen+66w ...
dseg:C016 00 00                                           built_table_left_xstart dw 0            ; DATA XREF: Production_Screen+5Dw ...
dseg:C018 00 00                                           build_table_item_height dw 0            ; DATA XREF: Production_Screen+2Aw ...
dseg:C01A 00 00                                           built_table_right_ystart dw 0           ; DATA XREF: Production_Screen+57w ...
dseg:C01C 00 00                                           built_table_right_xstart dw 0           ; DATA XREF: Production_Screen+4Ew ...

dseg:C026 00 00                                           build_table_item_yadd dw 0              ; DATA XREF: Production_Screen+24w ...
dseg:C028 00 00                                           build_table_item_xadd dw 0              ; DATA XREF: Production_Screen+1Ew ...

dseg:C02C 00 00                                           build_table_list_item dw 0              ; DATA XREF: Production_Screen+DFw ...



Production_Screen()

    Calculate_Product_Array(_city_idx, &n_product_indexes, &depr_n_product_indexes, &n_military_indexes, &build_table_list[0]);


    build_table_product_idx = build_table_list[build_table_list_item];

