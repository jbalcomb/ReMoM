/*
    City Screen

    WIZARDS.EXE
        ovr054

*/
/*
    ¿ ~== City (Screen) Util / Misc ?

    WIZARDS.EXE
        ovr055

*/
/*
    Cityscape  (City Screen)

    WIZARDS.EXE
        ovr144
*/



MoM.C
Screen_Control()
    Clear_Fields();
    Set_Mouse_List(1, mouse_list_default);
    switch (current_screen)
    {
        case scr_City_Screen: /* WZD 0x00 */
        {
            City_Screen__WIP();
        } break;
    }

CityScr.C
City_Screen__WIP()





IDA Groups
...
...
...
    [SCREEN LOOP]
    Enter Screen-Loop
        [INPUT]
            Left-Click Resource Row
            Hot-Key X - DEBUG
            Left-Click Up Button
            Left-Click Down Button
            Left-Click Change Button
            Left-Click OK Button  ||  Hot-Key ESCAPE
            Left-Click Buy Button
            Left-Click City Enchantment
            Left-Click Cityscape - Sell Building
            Left-Click Unit Window
            Right-Click Unit Window
            Left-Click Population Row
    Screen-Loop Wrap-Up
Leave Screen




## zz_active_stack_flag
    ¿ deprecated and/or debug ?
    ¿ `jmp   short $+2` suggests else or empty macro or null function pointer
    zz_active_stack_flag = ST_FALSE;
    if(_unit_stack_count != 0)
    {
        zz_active_stack_flag = ST_TRUE;
    }
    ...
    if((input_field_idx == city_screen_ok_button) || (input_field_idx == city_screen_esc_hotkey))
    {
        _unit_stack_count = 0;
        ...
        if(zz_active_stack_flag == ST_TRUE)
        {
            _unit_stack_count = 0;  // BUGBUG  does this always above - what is controlled by _unit_stack_count?
            // jmp     short $+2  // jump 2 bytes; from ovr054:0788, to ovr054:078A; loc_4834A:
        }
        ...
    }


m_city_no_buy
    controls whether to add a field for and draw the "Buy" button
    City_Can_Buy_Product() sets m_city_no_buy
        m_city_n_turns_to_produce
        m_city_production_cost
        City_Cost_To_Buy_Product()
        _city_idx







where is the load for ... ?
    // BACKGRND.LBX, 31  OUTPOST  outpost background
    // BACKGRND.LBX, 32  OUTNAME  outpost name backg
    




# City Screen

- City Screen  
    - Population Row  
    - Resource Window  
        - Food Row  
        - Production Row  
        - Gold Row  
        - Magic Power Row  
        - Research Points Row  
    - Cityscape Window  
    - City Enchantment Window  
    - City Map Window  
    - Garrison Window  
    - Production Window  
        - Buy Button  
        - Change Button  
        - OK Button  


Phase 2+
reassign townsfolk
buy product
draw cityscape
draw enchantments
cancel enchantment
sell building
resource breakdowns
help



¿ _city_idx ?  

shares with UV/USW  

shares with Production Screen, Enemy City Screen, ...  

¿ whole other business with Cityscape code ?  


naming convention for functions that use module-scoped `_city_idx`?  


ridiculous amount of pre-work  



## City Screen Road-Map / To-Do-List
    [ ] City_Screen()
    [ ] City_Screen_Draw()
    [ ] City_Screen_Draw_Map()
    [ ] City_Screen_Draw_Buttons()
    [ ] IDK_CityScrn_Draw()
    [ ] City_Screen_Draw_Population_Row()
    [ ] City_Screen_Draw_Resource_Icons()
    [ ] City_Screen_Draw_City_Enchantments()
    [ ] IDK_CityScreen_DrawUnitPictures()
    [ ] IDK_ProductionWindow_ProdDoneLeft()
    [ ] IDK_City_Screen_Draw_Cityscape()  
    [ ] IDK_CityScreen_Cityscape_Draw_MouseOver()  
    [ ] IDK_CityScreen_CityEnchantment_HelpIdx()

City_Screen_Add_Fields__WIP()



Round 1:  
    NOTHING for 'Cityscape'  
        none of the cityscape draw functions  
        no cityscape fields  
        no cityscape input  
    NOTHING for city enchantments  

¿ road-blocks, hurdles, hang-ups ?  
***Map_Square_Is_Shared()***  
City vs. Outpost  
Own City vs. Enemy City  
Main Screen Unit Window vs. City Screen Garrison Window  
City Screen vs. Production Screen  

City vs. Outpost  
    e.g.,
        could be named
            City_Screen_Add_Fields_City_Enchantments()
        but, also used for 'Outpost /PopUp/'
        so, just named
            City_Add_Fields_City_Enchantments()
        presumes some understanding that "City" includes "City" or "Outpost"



Elsewhere, ...  
    Cityscape  
        row & col?  
            4x4?  
                = 16?  



## Buy / Buy Button / Left-Click Buy Button
...input #7
@@Input_CityScreenBuyButton
ovr054:0795
0003B095
00048355
input_field_idx == city_screen_buy_button



## Left-Click City Enchantment
...input #8



## Left-Click Cityscape Building
...input #9



## Left-Click Unit Window
...input #10
mov     [bp+IDK_Col__unit_stack_idx], 0
mov     ax, [bp+IDK_Col__unit_stack_idx]
cmp     ax, [_unit_stack_count]
jl      short loc_48A58
loc_48A58:
mov     bx, [bp+IDK_Col__unit_stack_idx]
shl     bx, 1
cmp     [g_unit_window_fields+bx], _DI_input_field_idx
jnz     short loc_48A8E
loc_48A8E:
inc     [bp+IDK_Col__unit_stack_idx]



## Right-Click Unit Window
...input #11



## Resources Window
resource icons:  
    sack of flour  
    loaf of bread  
    anvil  
    hammer  
    stack of gold  
    gold coin  
    ¿ magic power 10 ?  
    ¿ magic power 1 ?  
    opened spell-book  
    closed spell-book  
resource icon dimensions:  
    big:  
    lil:  
resource icon spacing:
    big:  14  
    lil:   8  



## Cityscape Window




City_Screen()
    |-> City_Screen_Draw()

City_Screen_Draw()
    |-> Cityscape_Draw()
    |-> j_IDK_City_Cityscape_Draw_MouseOver()


// WZD o144p02
void Cityscape_Draw(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2);
Cityscape_Draw(_city_idx, 4, 101, IDK_completed_bldg_idx, IDK_completed_bldg_idx)

    IDK_animate_new_building_idx = IDK_bld_idx_2

    set value based on city enchantments  {1: Flying Fortress, 0: Cursed Lands, 2: Famine, 3: Gaia's Blessing, 4: NONE/Default}

    sanitize bldg_idx_1 {3, ..., 34} ¿ OR >= 100 ?



So, ...
    IDK_completed_bldg_idx is module-scoped to 'City Screen'
    IDK_animate_new_building_idx is module-scoped to 'Cityscape'


IDK_animate_new_building_idx
WZD dseg:CA78
XREF:
    Cityscape_Draw()
    IDK_CityScreen_Cityscape_sCF7F1()



City_Screen()
    |-> IDK_CityScreen_s49DF0()
        |-> j_IDK_Bld_PreqChk_s4BADF()
        IDK_CityScreen_MaybeFields[itr] = F/T

City_Screen()
    |-> City_Screen_Load()
        IDK_CityScreen_MaybeFields = Near_Allocate_Next(72)
¿ set but never used ?

IDK_CityScreen_s49DF0()
    defaults all to true
    sets to false if IDK_Bld_PreqChk_s4BADF() returns true

IDK_Bld_PreqChk_s4BADF()



City_Remove_Building()
called for selling building or damaging city
sets s_CITY.buildings[] to 2  ¿ "B_Destroyed" ?
so, does *unbuild* it, but *done & gone* ~ sold/destroyed
¿ build building list specifically checks for 0 or 1 ?
¿ anything tests for `> -1` || `>= 0` ?
    Yup.
        City_Screen_Can_Build_List()  `>= 1`


what does it mean to return ST_FALSE?

e.g., cannot sell Sages Guild - Sages Guild required by Alchemists Guild

Module: ERICNET
    Remove_Building_()
        Address: 01:000145EA






Cityscape_Draw_Background()

    Draw 'ground background'
    if River, add 'river'
    If Ocean, add 'ocean'


NOTE: uses 3rd Terrain function for *is ocean*


GFX_Swap_Cities()
    GFX_Swap_AppndCtScap__WIP()


city_scape ... background
    ground
    river, ocean

Arcanus
  0  BACKS       ground background
  1  FARBACKS    hills background
  2  FARBACKS    mountain backgrnd
  7  FARBACKS    plain background

  3  CSWATER     river 
  4  CSWATER     ocean

Myrror
  8  KAOSCAPE    chaos ground
  9  KAOBACKS    choas hills
 10  KAOBACKS    chaos mountain
 11  KAOBACKS    choas plains

115  CHWATER     chaos river
116  CHWATER     chaos ocean


Arcanus
 91  FARBACKS    cloud of dark
 92  FARBACKS    chaos rift
 93  FARBACKS    alkar
Myrror
111  KAOBACKS    choas dark cloud
112  KAOBACKS    choas chaos rift
113  KAOBACKS    chaos alkar 






## Draw, Load, Allocate

City Screen  
    Population Row  
    Resource Window  
        Food Row  
        Production Row  
        Gold Row  
        Magic Power Row  
        Research Points Row  
    Cityscape Window  
    City Enchantment Window  
    City Map Window  
    Garrison Window  
    Production Window  
        Buy Button  
        Change Button  
        OK Button  


IDK_CityScrn_Draw()  
    Print_Centered()    "[city size string] of [city name string]"  
    Print()             "[race name string]" && " (Plague)" || " (Pop. Boom)"  
    Print_Right()       "Population: "  
    City_Screen_Draw_Population_Row()  
    City_Screen_Draw_Resource_Icons()  
    City_Screen_Draw_City_Enchantments()  
    IDK_CityScreen_DrawUnitPictures()  
    Print()             "Turn" / "Turns"  
    Draw_Unit_Figure()  || Draw_Picture([building pict])  
    Print() || Print_Paragraph()  IDK_production_title  
So, ...  
    does not draw/print  
        "Resources"  
        "Enchantments"  
        "Buildings"  
        "Units"  
        "Producing"  
        *Cityscape Window*  

¿¿¿  
    "Resources", "Enchantments", "Buildings", "Units", "Producing"  
    are in their own respective "window" draw functions  
???  
Also, missing here...
    city screen background
    city map



City_Screen_Draw()  
    sets up some helps  
    IDK_CityScreen_CityEnchantment_HelpIdx()  
    and draws/prints the coins/text in the Production Window  
    Draw_Maps()  
    City_Screen_Draw_Map()  
    IDK_CityScrn_Draw()  
    City_Screen_Draw_Buttons()  buy, change, ok, up, dn
    IDK_City_Screen_Draw_Cityscape()  
    IDK_CityScreen_Cityscape_Draw_MouseOver()  
        |-> Print_Paragraph()  Housing or Trade Goods ProdWind text  
    ||  
        |-> FLIC_Draw()  BACKGRND.LBX, 13  "CITYBITS"  "city block out"  
        |-> IDK_ProductionWindow_ProdDoneLeft()  

IDK_ProductionWindow_ProdDoneLeft()  
    only draws
        gold coins      production invested
        darkened coins  production remaining



##### Naming Things Is Hard
small figures  
inhabitants  
people  
farmers  
workers  
townsfolk  
surplus food  
civil unrest  
rebels  

population row  
left side  
right side  
group  

five different resources: food, production points, gold, magic power or “mana,” and spell research points  

resource window  

lower left section of the city screen  
cityscape window  
This window contains a graphic representation of the city and its  
buildings.  
As you complete buildings or other structures, they appear in  
this area.  
To examine the contents of a city, run the mouse cursor over  
this window; the name of the structure appears.  

city enchantment window  
resources window  

CITY VIEW WINDOW  
upper right section of the city screen  
city map window  
the map squares that contribute to the city’s production  
GARRISON WINDOW  
underneath the city view window  
garrison  
troops  
currently stationed in the city or on the city’s map square  
activate a unit  
unit statistics window  

lower right section of the city screen  
production window  
building, unit or other item you are currently producing  
how many turns remain before the project is completed  
amount of production (in terms of production points) already invested in the   project
graphically shown by gold coins  
amount of production remaining before the project is completed  
shown as darkened coins  
three buttons below the production window  
Buy:  
buy button  
Change:  
change button  
production screen  
OK:  
this button  
returns you to the main movement screen  


You may also enter the city production screen  
by clicking on the change button  
below the city’s current building project.


BACKGROUND.LBX  
CITYICON    food icon  
CITYICON    production icon  
CITYICON    gold icon  
CITYICON    power icon  
CITYICON    research icon  
CITYICO3    10 food
CITYICO3    10 prod
CITYICO3    10 gold
CITYICO3    10 power
CITYICO3    10 research


BASIC UPKEEP INFORMATION
Upkeep Type Notes
Gold To pay normal troops and non-noble heroes
(maintenance cost reduced by wizard’s fame);
To maintain buildings
Food To maintain normal troops (1 food per unit)
Mana To maintain the presence of summoned creatures;
To maintain enchantments and continuous spells


"requisite materials"
"previously constructed buildings"
"requires prior construction"
"replacement buildings"

To sell a building, you can click on it  
  (you only receive half of its construction cost in gold).
The only structures that you cannot sell are fortresses and summoning circles.

Cities can only produce those buildings  
  for which they have the requisite materials (terrain types),  
  previously constructed buildings  
    (for example, construction of a temple requires prior construction of a shrine)  
    and knowledge (race-dependent; see Select Race).  

Cities with shrines, sage’s guilds or animist’s guilds  
(or their replacement buildings)  
use nightshade to automatically cast dispel magic against all city enchantments and global spells directed against the city every turn;  
