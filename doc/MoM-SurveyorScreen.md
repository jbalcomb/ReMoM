
Surveyor
Survey

WIZARDS.EXE  
ovr094



Surveyor_Screen()
Surveyor_Window_Display()
Surveyor_Screen_Draw()
Set_Surveyor_Screen_Help_List()
Surveyor_Screen_Add_Fields()



GUI_String_1
    in Surveyor_IDK_Print_Terrain_And_Effect(), used for terrain effect #1 name or "Corruption"
GUI_String_2
    in Surveyor_IDK_Print_Terrain_And_Effect(), used for terrain effect #2 name or cleared for corruption
survey_near_alloc_1_100
survey_near_alloc_2_50
survey_near_alloc_3_50

Surveyor_IDK_Print_Terrain_And_Effect()



## Surveyor_Screen()

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    survey_near_alloc_1_100 = (char *)Near_Allocate_Next(100);
    survey_near_alloc_2_50 = (char *)Near_Allocate_Next(50);
    survey_near_alloc_3_50 = (char *)Near_Allocate_Next(50);




## Surveyor_Window_Display()

Surveyor_Screen()
    |-> Surveyor_Screen_Draw()
        |-> Surveyor_Window_Display()

    var_4 = Map_Square_Survey(l_mx, l_my, _map_plane);
    if(var_4 == 5)
        IDK = Surveyor_Cities(l_mx, l_my, _map_plane);
        if(IDK > 0)
            var_4 = 0;
HERE: var_4 being 0 means it shows all the summary information
THEN: 
    if(var_4 > 0)
        strcpy(GUI_String_2, str_CitiesCannotBe);
        strcat(GUI_String_2, GUI_String_1);
        Print_Paragraph(246, 143, 67, GUI_String_2, 0);


XREF?
    // WZD dseg:53A4
    char str_OnTowers_[] = "on towers";
    ...
    ...
    ...
    Surveyor_Window_Display()
        switch(var_4)
            case 3:
                strcpy(GUI_String_1, str_OnTowers_);

3 parts?
1)  terrain type name and effect / "Corruption"
2)  lair/node/tower and guardian units names
    OR
    city size name and city name
3)
    "Cities cannot be built " ... {"on water.", "on lairs.", "on towers.", "on magic nodes.", "less than 3 squares from any other city."}
    OR
    "City Resources"; Maximum Pop, Prod Bonus, Gold Bonus;

Anywhoodle, ...
where's the terrain typing printing incorrectly?
    getting Desert instead of Grasslands
    so, Surveyor_IDK_Print_Terrain_And_Effect() is getting passed a 4 instead of a 1?
        ...
        else if(Square_Is_Desert(l_mx, l_my, _map_plane) == ST_TRUE)
            Surveyor_IDK_Print_Terrain_And_Effect(4, IDK);
        else if(Square_Is_Grasslands(l_mx, l_my, _map_plane) == ST_TRUE)
            Surveyor_IDK_Print_Terrain_And_Effect(1, IDK);




## Surveyor_IDK_Print_Terrain_And_Effect()

    switch(IDK_type)
        case 0:
            Print_Centered(280, 91, aOcean);
        case 1:
            Print_Centered(280, 91, aGrasslands);
        case 2:
            Print_Centered(280, 91, aForest);
        case 3:
            Print_Centered(280, 91, aMountain);
        case 4:
            Print_Centered(280, 91, aDesert);
        case 5:
            Print_Centered(280, 91, aSwamp);
        case 6:
            Print_Centered(280, 91, aTundra);
        case 7:
            Print_Centered(280, 91, aShore);
        case 8:
            Print_Centered(280, 91, aRiverMouth);
        case 9:
            DNE
        case 10:
            Print_Centered(280, 91, aHills);
        case 11:
            Print_Centered(280, 91, aPlains);
        case 12:
            Print_Centered(280, 91, aRiver);
        case 13:
            Print_Centered(280, 91, aVolcano);

IDK_type
type  0     Ocean
type  1     Grasslands
type  2     Forest
type  3     Mountain
type  4     Desert
type  5     Swamp
type  6     Tundra
type  7     Shore
type  8     RiverMouth
type  9     DNE
type 10     Hills
type 11     Plains
type 12     River
type 13     Volcano


## Surveyor_IDK_Set_Font_Stuff__1()
## Surveyor_IDK_Set_Font_Stuff__2()

Surveyor_Window_Display()
    Surveyor_IDK_Set_Font_Stuff__1();
    Print_Centered(280, 143, str_CityResources);
    Surveyor_IDK_Set_Font_Stuff__2();
    Print_ ... Maximum Pop, Prod Bonus, Gold Bonus

Surveyor_IDK_Print_Terrain_And_Effect()
    Surveyor_IDK_Set_Font_Stuff__1();
    Print_Centered(280, 91, aOcean);
    ...
    Surveyor_IDK_Set_Font_Stuff__2();
    Print_Centered(280, 98, GUI_String_1);   empty string or terrain effect #1 or "Corruption
    Print_Centered(280, 105, GUI_String_2);  empty string or terrain effect #2
    ...
    Surveyor_IDK_Set_Font_Stuff__1();
        Print_Centered(280, 114, aIronOre);
        ...
        Print_Centered(280, 114, survey_near_alloc_1_100);
        Surveyor_IDK_Set_Font_Stuff__2();
        Print_Centered(280, 121, survey_near_alloc_2_50);
        Print_Centered(280, 128, survey_near_alloc_3_50);
        Surveyor_IDK_Set_Font_Stuff__1();
        ...
        if(IDK_IDK >= 200)
            Print_Centered(280, 114, survey_near_alloc_1_100);
            Surveyor_IDK_Set_Font_Stuff__2();
            Print_Centered(280, 121, survey_near_alloc_2_50);
            Print_Centered(280, 128, survey_near_alloc_3_50);
        Surveyor_IDK_Set_Font_Stuff__2();
        Print_Paragraph(247, 121, 66, GUI_String_1, 2);
        OR
        city size name and city name






## Surveyor_Screen_Add_Fields()

Add_Game_Button_Fields()
_main_map_grid_field = Add_Grid_Field
_minimap_grid_field = Add_Grid_Field
main_cancel_button_field = Add_Button_Field
...
hotkey_ESC_field = Add_Hot_Key(str_hotkey_ESC__ovr094[0]);





##### Naming Things Is Hard

surveyor button  
status window of the main movement screen  
surveyor information (surveyor window)  
visible map squares  
main movement screen  
information about the map squares revealed in the surveyor window  
surveyor’s information  
statistics for a potential city built on the selected map square  
terrain features  
terrain type  
special terrain features (minerals, crystals, wild game, etc.)  
bonus effects  
food, gold, power and production  
Terrain Types and Terrain Specials  
specific terrain types and features  
surveyor window  
Surveyor summary of effects for indicated square  


HLPENTRY.LBX

survey Help


Page 34  (PDF Page 39)

SURVEYOR

Clicking on the surveyor button replaces the status window of the main movement screen (see Status Window) with surveyor information (surveyor window).  
By running the mouse cursor over visible map squares on the main movement screen, you can see information about the map squares revealed in the surveyor window.  
The surveyor’s information includes statistics for a potential city built on the selected map square: the potential maximum population of a city built at that site and any bonuses such a city would get to its production and gold revenue because of nearby terrain features.  
The information displayed in the surveyor window also includes the terrain type and any special terrain features (minerals, crystals, wild game, etc.), including their bonus effects on food, gold, power and production.  
See Terrain Types and Terrain Specials for more information on specific terrain types and features.  
Click on the cancel button at the bottom of the surveyor window to exit.  

Page 35  (PDF Page 40)

Surveyor
summary of
effects for
indicated
square


Page 56  (PDF Page 61)

MAGIC POWER

· Minerals: Cities built near sources of mithril, adamantium, quork and crysx crystals obtain additional magic power from these resources every turn (see Terrain Specials).  
