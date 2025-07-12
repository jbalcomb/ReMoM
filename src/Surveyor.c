/*
    WIZARDS.EXE
        ovr094

*/

#include "MOX/MOX_DAT.h"  /* _screen_seg */

#include "CITYCALC.h"
#include "Help.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "Outpost.h"
#include "Roads.h"
#include "Surveyor.h"
#include "Terrain.h"
#include "UNITTYPE.h"   // WTFMATE
#include "WZD_o059.h"



// WZD dseg:534E                                                 BEGIN:  ovr094 - Initialized Data

// WZD dseg:534E
char str_hotkey_ESC__ovr094[] = "\x1B";

// WZD dseg:534F
char str_empty_string__094[] = "";

// WZD dseg:5350
char str_hotkey_C__ovr094[] = "C";

// WZD dseg:5352
char str_Surveyor[] = "Surveyor";

// WZD dseg:535B
char str_CityResources[] = "City Resources";

// WZD dseg:536A
char str_MaximumPop[] = "Maximum Pop";

// WZD dseg:5376
char str_ProdBonus[] = "Prod Bonus";

// WZD dseg:5381
char str_PlusSign[] = "+";

// WZD dseg:5383
char str_PercentSign[] = "%";

// WZD dseg:5385
char str_GoldBonus[] = "Gold Bonus";

// WZD dseg:5390
char str_OnWater_[] = "on water.";

// WZD dseg:539A
char str_OnLairs_[] = "on lairs.";

// WZD dseg:53A4
char str_OnTowers_[] = "on towers.";

// WZD dseg:53AF
char str_OnMagicNodes_[] = "on magic nodes.";

// WZD dseg:53BF
char str_LessThan3SquaresFromAnyOtherCity[] = "less than 3 squares from any other city.";

// WZD dseg:53E8
char str_CitiesCannotBe[] = "Cities cannot be built ";

// WZD dseg:5400
char aRiver[] = "River";
// WZD dseg:5406
char a2Food[] = "2 food";
// WZD dseg:540D
char a20Gold[] = "+20% gold";
// WZD dseg:5417
char aRiverMouth[] = "River Mouth";
// WZD dseg:5423
char a12Food[] = "1/2 food";
// WZD dseg:542C
char a30Gold[] = "+30% gold";
// WZD dseg:5436
char aGrasslands[] = "Grasslands";
// WZD dseg:5441
char a112Food[] = "1   1/2 food";
// WZD dseg:544E
char aForest[] = "Forest";
// WZD dseg:5455
char a3Production[] = "+3% production";
// WZD dseg:5464
char aMountain[] = "Mountain";
// WZD dseg:546D
char a5Production[] = "+5% production";
// WZD dseg:547C
char aDesert[] = "Desert";
// WZD dseg:5483
char aSwamp[] = "Swamp";
// WZD dseg:5489
char aTundra[] = "Tundra";
// WZD dseg:5490
char aHills[] = "Hills";
// WZD dseg:5496
char aPlains[] = "Plains";
// WZD dseg:549D
char a1Food[] = "1";
// WZD dseg:54A4
char aOcean[] = "Ocean";
// WZD dseg:54AA
char aShore[] = "Shore";
// WZD dseg:54B0
char a10Gold[] = "+10% gold";
// WZD dseg:54BA
char aVolcano[] = "Volcano";

// WZD dseg:54C2
char aCorruption[] = "Corruption";

// WZD dseg:54CD
char aOf_0[] = " of";

// WZD dseg:54D1
char aGoldOre[] = "Gold Ore";
// WZD dseg:54DA
char a3Gold[] = "+3 gold";
// WZD dseg:54E2
char aSilverOre[] = "Silver Ore";
// WZD dseg:54ED
char a2Gold[] = "+2 gold";
// WZD dseg:54F5
char aGems[] = "Gems";
// WZD dseg:54FA
char a5Gold[] = "+5 gold";
// WZD dseg:5502
char aMithrilOre[] = "Mithril Ore";
// WZD dseg:550E
char a1Power[] = "+1 power";
// WZD dseg:5517
char aAdamantiumOre[] = "Adamantium Ore";
// WZD dseg:5526
char a2Power[] = "+2 power";
// WZD dseg:552F
char aQuorkCrystals[] = "Quork Crystals";
// WZD dseg:553E
char a3Power[] = "+3 power";
// WZD dseg:5547
char aCrysxCrystals[] = "Crysx Crystals";
// WZD dseg:5556
char a5Power[] = "+5 power";
// WZD dseg:555F
char aIronOre[] = "Iron Ore";
// WZD dseg:5568
char aReducesNormalU[] = "Reduces normal unit cost by 5%";
// WZD dseg:5587
char aCoal[] = "Coal";
// WZD dseg:558C
char aReducesNorma_0[] = "Reduces normal unit cost by 10%";
// WZD dseg:55AC
char aWildGame[] = "Wild Game";
// WZD dseg:55B6
char a2Food_0[] = "+2 food";
// WZD dseg:55BE
char str_Nightshade__ovr094[] = "Nightshade',0 ; should use dseg:2";
// WZD dseg:55C9
char aProtectsCityFr[] = "Protects city from spells";
// WZD dseg:55E3
char aTower[] = "Tower";
// WZD dseg:55E9
char aCave[] = "Cave";
// WZD dseg:55EE
char aDungeon[] = "Dungeon";
// WZD dseg:55F6
char aTemple[] = "Temple";
// WZD dseg:55FD
char aKeep[] = "Keep";
// WZD dseg:5602
char aLair[] = "Lair";
// WZD dseg:5607
char aRuins[] = "Ruins";
// WZD dseg:560D
char aUnexplored[] = "Unexplored";
// WZD dseg:5618
char aSorceryNode[] = "Sorcery Node";
// WZD dseg:5625
char aNatureNode[] = "Nature Node";
// WZD dseg:5631
char aChaosNode[] = "Chaos Node";
// WZD dseg:563C
char aWarped[] = "Warped";
// WZD dseg:5643
char aGuardianSpir_0[] = "Guardian Spirit";
// WZD dseg:5653
char aMagicSpirit_0[] = "Magic Spirit";

// WZD dseg:5653                                                 END:  ovr094 - Initialized Data



// WZD dseg:C5D6                                                 BEGIN:  ovr094 - Uninitialized Data

// WZD dseg:C5D6
/*
used for second lair guardian name
*/
char * survey_near_alloc_3_50;

// WZD dseg:C5D8
/*
used for first lair guardian name
*/
char * survey_near_alloc_2_50;

// WZD dseg:C5DA
/*
used for node type name
*/
char * survey_near_alloc_1_100;

// WZD dseg:C5DA                                                 END:  ovr094 - Uninitialized Data



/*
    WIZARDS.EXE  ovr094
*/

// WZD o094p01
void Surveyor_Screen(void)
{
    int16_t hotkey_ESC_field = 0;
    int16_t l_map_plane = 0;
    int16_t reduced_map_window_wy = 0;   // ~== target_world_y
    int16_t reduced_map_window_wx = 0;  // ~== target_world_x
    int16_t scanned_field = 0;
    int16_t leave_screen = 0;  // _DI_
    int16_t input_field_idx = 0;  // _SI_

    Set_Draw_Active_Stack_Always();

    Set_Outline_Color(0);

    Deactivate_Auto_Function();

    Assign_Auto_Function(Surveyor_Screen_Draw, 1);

    l_map_plane = _map_plane;

    _reduced_map_seg = Allocate_First_Block(_screen_seg, 153);

    GUI_String_1             = (char *)Near_Allocate_First(100);
    GUI_String_2             = (char *)Near_Allocate_Next( 100);
    survey_near_alloc_1_100  = (char *)Near_Allocate_Next( 100);
    survey_near_alloc_2_50   = (char *)Near_Allocate_Next(  50);
    survey_near_alloc_3_50   = (char *)Near_Allocate_Next(  50);

    Set_Unit_Draw_Priority();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Reset_Map_Draw();

    MainScr_Create_Reduced_Map_Picture();

    Set_Input_Delay(1);

    Deactivate_Help_List();

    Set_Surveyor_Screen_Help_List();

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        Clear_Fields();

        Surveyor_Screen_Add_Fields();

        hotkey_ESC_field = Add_Hot_Key(str_hotkey_ESC__ovr094[0]);

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();

        if(
            (input_field_idx == main_cancel_button_field)
            ||
            (input_field_idx == hotkey_ESC_field)
        )
        {
            Play_Left_Click();
            current_screen = scr_Main_Screen;
            leave_screen = ST_UNDEFINED;
        }

        if(input_field_idx == _plane_button)
        {
            Play_Left_Click();
            Do_Plane_Button__WIP(_human_player_idx, &_map_x, &_map_y, &_map_plane);
            Reset_Map_Draw();
            MainScr_Create_Reduced_Map_Picture();
            Deactivate_Auto_Function();
            Assign_Auto_Function(Surveyor_Screen_Draw, 1);
        }

        if(input_field_idx == _minimap_grid_field)
        {
            Play_Left_Click();
            Reduced_Map_Coords(&reduced_map_window_wx, &reduced_map_window_wy, ((_map_x + (MAP_WIDTH / 2)) % WORLD_WIDTH), (_map_y + (MAP_HEIGHT / 2)), REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);
            _prev_world_x = (reduced_map_window_wx + _minimap_grid_x);  // ...is the 'wx' of the clicked square
            _prev_world_y = (reduced_map_window_wy + _minimap_grid_y);  // ...is the 'wy' of the clicked square
            _map_x = _prev_world_x;
            _map_y = _prev_world_y;
            Center_Map(&_map_x, &_map_y, _prev_world_x, _prev_world_y, _map_plane);
            MainScr_Create_Reduced_Map_Picture();
            Set_Mouse_List_Default();
            Reset_Map_Draw();
        }

        if(-(_main_map_grid_field) == input_field_idx)
        {
            _prev_world_x += (_main_map_grid_x - (MAP_WIDTH  / 2));
            _prev_world_y += (_main_map_grid_y - (MAP_HEIGHT / 2));
            IDK_CheckSet_MapDisplay_XY();
        }

        if(leave_screen == ST_FALSE)
        {
            Surveyor_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Reset_Window();

    _map_plane = l_map_plane;

}


// WZD o094p02
void Surveyor_Screen_Add_Fields(void)
{

    _main_map_grid_field = INVALID_FIELD;
    _minimap_grid_field = INVALID_FIELD;

    Add_Game_Button_Fields();

    if(
        (_map_x == _prev_world_x)
        &&
        (_map_y == _prev_world_y)
    )
    {
        _main_map_grid_field = Add_Grid_Field(MAP_SCREEN_X, MAP_SCREEN_Y, SQUARE_WIDTH, SQUARE_HEIGHT, MAP_WIDTH, MAP_HEIGHT, &_main_map_grid_x, &_main_map_grid_y, ST_UNDEFINED);
    }

    if(
        (_map_x == _prev_world_x)
        &&
        (_map_y == _prev_world_y)
    )
    {
        _minimap_grid_field = Add_Grid_Field(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_SQUARE_WIDTH, REDUCED_MAP_SQUARE_HEIGHT, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT, &_minimap_grid_x, &_minimap_grid_y, ST_UNDEFINED);
    }

    main_cancel_button_field = Add_Button_Field(263, 181, str_empty_string__094, cast_cancel_button, str_hotkey_C__ovr094[0], ST_UNDEFINED);

}


// WZD o094p03
void Surveyor_Screen_Draw(void)
{

    Reset_Window();

    Set_Page_Off();

    Draw_Maps(MAP_SCREEN_X, MAP_SCREEN_Y, MAP_WIDTH, MAP_HEIGHT, &_map_x, &_map_y, _map_plane, _prev_world_x, _prev_world_y, _human_player_idx);

    FLIC_Draw(0, 0, main_background_seg);

    Draw_World_Window(REDUCED_MAP_SCREEN_X, REDUCED_MAP_SCREEN_Y, REDUCED_MAP_WIDTH, REDUCED_MAP_HEIGHT);

    Main_Screen_Draw_Game_Buttons();

    Main_Screen_Draw_Status_Window();

    FLIC_Draw(242, 173, deselect_button_blockout);

    FLIC_Draw(240, 76, survey_background);

    FLIC_Draw(240, 173, cast_button_border);

    Surveyor_Window_Display();

    Reset_Map_Draw();

}

// WZD o094p04
/*
    draws/prints/displays the 'Surveyor Window' information
*/
void Surveyor_Window_Display(void)
{
    int16_t is_unexplored = 0;
    int16_t have_shore = 0;
    int16_t have_adamantium = 0;
    int16_t have_mithril = 0;
    int16_t have_nightshade = 0;
    int16_t magic_units = 0;
    int16_t gold_units = 0;
    int16_t unit_cost_reduction = 0;
    int16_t gold_bonus = 0;
    int16_t production_bonus = 0;
    int16_t val = 0;
    int16_t var_4 = 0;
    int16_t l_my = 0;
    int16_t IDK = 0;  // _SI_
    int16_t l_mx = 0;  // _DI_

    l_mx = Pointer_X();
    l_my = Pointer_Y();

    Set_Font_Style_Shadow_Down(4, 0, 0, 0);

    Set_Outline_Color(148);

    Print_Centered(280, 80, str_Surveyor);

    if(
        (l_mx >= MOVEMENT_MAP_SCREEN_XMAX)
        ||
        (l_my <= MOVEMENT_MAP_SCREEN_YMIN)
    )
    {
        return;
    }

    l_mx = (_map_x + ((l_mx / SQUARE_WIDTH) % WORLD_WIDTH));

    l_my = ((_map_y + (l_my / SQUARE_HEIGHT)) - 1);

    val = 0;

    production_bonus = 0;

    gold_bonus = 0;

    if(SQUARE_EXPLORED(l_mx, l_my, _map_plane))
    {

        Surveyor_IDK_Set_Font_Stuff__1();

        // ; returns 0 if the tile can be settled, or an error
        // ; value if not:
        // ;   1 - tile is in the ocean
        // ;   2 - tile has an intact encounter zone
        // ;   3 - tile has a tower of wizardry
        // ;   4 - tile has a node
        // ;   5 - there's already a city within 3 tiles
        var_4 = Map_Square_Survey(l_mx, l_my, _map_plane);

        if(var_4 == 5)
        {
            
            IDK = Surveyor_Cities(l_mx, l_my, _map_plane);

            if(IDK > 0)
            {
                var_4 = 0;  // HERE: var_4 being 0 means it shows all the summary information
            }

        }

        switch(var_4)
        {
            case 0:
            {

// void Compute_Base_Values_For_Map_Square(int16_t wx, int16_t wy, int16_t wp, int16_t *MaxPop, int16_t *production_bonus, int16_t *gold_bonus, int16_t *unit_cost_reduction, int16_t *gold_units, int16_t *magic_units, int16_t *have_nightshade, int16_t *have_mithril, int16_t *have_adamantium, int16_t *have_shore, int16_t *is_unexplored)
                Compute_Base_Values_For_Map_Square(l_mx, l_my, _map_plane, &val, &production_bonus, &gold_bonus, &unit_cost_reduction, &gold_units, &magic_units, &have_nightshade, &have_mithril, &have_adamantium, &have_shore, &is_unexplored);

                Surveyor_IDK_Set_Font_Stuff__1();

                Print_Centered(280, 143, str_CityResources);

                Surveyor_IDK_Set_Font_Stuff__2();

                Print(245, 151, str_MaximumPop);
                Print_Integer_Right(306, 151, val);

                Print(245, 158, str_ProdBonus);
                itoa(production_bonus, GUI_String_1, 10);
                strcpy(GUI_String_2, str_PlusSign);
                strcat(GUI_String_2, GUI_String_1);
                strcat(GUI_String_2, str_PercentSign);
                Print_Right(312, 158, GUI_String_2);

                Print(245, 165, str_GoldBonus);
                itoa(gold_bonus, GUI_String_1, 10);
                strcpy(GUI_String_2, str_PlusSign);
                strcat(GUI_String_2, GUI_String_1);
                strcat(GUI_String_2, str_PercentSign);
                Print_Right(312, 165, GUI_String_2);

            } break;
            case 1:
            {
                strcpy(GUI_String_1, str_OnWater_);
            } break;
            case 2:
            {
                strcpy(GUI_String_1, str_OnLairs_);
            } break;
            case 3:
            {
                strcpy(GUI_String_1, str_OnTowers_);
            } break;
            case 4:
            {
                strcpy(GUI_String_1, str_OnMagicNodes_);
            } break;
            case 5:
            {
                strcpy(GUI_String_1, str_LessThan3SquaresFromAnyOtherCity);
            } break;
            
        }

        if(var_4 > 0)
        {
            strcpy(GUI_String_2, str_CitiesCannotBe);

            strcat(GUI_String_2, GUI_String_1);

            Print_Paragraph(246, 143, 67, GUI_String_2, 0);
        }

    }

    if(SQUARE_EXPLORED(l_mx, l_my, _map_plane) == ST_TRUE)
    {

        IDK = Surveyor_Cities(l_mx, l_my, _map_plane);

        if(IDK == 0)
        {
            IDK = (GET_MAP_SQUARE_FLAG(l_mx, l_my, _map_plane) & MSF_CORRUPTION);
        }
        else
        {

            // // test [byte ptr es:bx], TF_Corruption;  jz;
            // if(MAP_SQUARE_UNCORRUPT(l_mx, l_my, _map_plane))
            // {
            //     IDK += (1000 + MAP_SQUARE_HAS_CORRUPTION(l_mx, l_my, _map_plane));
            // }

            if((GET_MAP_SQUARE_FLAG(l_mx, l_my, _map_plane) & MSF_CORRUPTION) != 0)
            {
                IDK += (1000 + (GET_MAP_SQUARE_FLAG(l_mx, l_my, _map_plane) & MSF_CORRUPTION));
            }

        }

        // IDGI, but ~ check for any minerals?
        if(IDK == 0)
        {
            IDK = (GET_TERRAIN_SPECIAL(l_mx, l_my, _map_plane) & 0x0F);  // 1st nibble, 1st four bits - {0,...,15} {TS_NONE, TS_IronOre, TS_Coal, TS_SilverOre, TS_GoldOre, TS_Gems, TS_MithrilOre, TS_AdamantiumOre, TS_QuorkCrystals, TS_CrysxCrystals}
        }

        if(IDK == 0)
        {
            IDK = (GET_TERRAIN_SPECIAL(l_mx, l_my, _map_plane) & ts_Wild_Game);
        }

        if(IDK == 0)
        {
            IDK = (GET_TERRAIN_SPECIAL(l_mx, l_my, _map_plane) & ts_Nightshade);
        }

        if(IDK == 0)
        {
            IDK = Surveyor_Lairs(l_mx, l_my, _map_plane);
        }

        if(IDK == 0)
        {
            IDK = Surveyor_Nodes(l_mx, l_my, _map_plane);
        }

        if(Square_Is_Forest(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(2, IDK);
        }
        else if(Square_Is_Volcano(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(13, IDK);
        }
        else if(Square_Is_Mountain(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(3, IDK);
        }
        else if(Square_Is_Hills(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(10, IDK);
        }
        else if(Square_Is_Desert(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(4, IDK);
        }
        else if(Square_Is_Swamp(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(5, IDK);
        }
        else if(Square_Is_Grasslands(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(1, IDK);
        }
        else if(Square_Is_Tundra(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            Surveyor_IDK_Print_Terrain_And_Effect(6, IDK);
        }
        else if(Square_Is_River(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            if(Surveyor_IDK_RiverMouth(l_mx, l_my, _map_plane) == ST_TRUE)
            {
                Surveyor_IDK_Print_Terrain_And_Effect(8, IDK);
            }
            else
            {
                Surveyor_IDK_Print_Terrain_And_Effect(12, IDK);
            }
        }
        else if(Square_Is_Sailable(l_mx, l_my, _map_plane) == ST_TRUE)
        {
            if(Square_Is_Ocean(l_mx, l_my, _map_plane) == ST_TRUE)
            {
                Surveyor_IDK_Print_Terrain_And_Effect(0, IDK);
            }
            else
            {
                Surveyor_IDK_Print_Terrain_And_Effect(7, IDK);
            }
        }
        else
        {
            if(IDK == 201)  /* (200 + node_idx) */
            {
                Surveyor_IDK_Print_Terrain_And_Effect(2, IDK);
            }
        }

    }

}


// WZD o094p05
/*
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

IDK_IDK:
    0:  ¿ DNE ?
    1:  Iron Ore
    2:  Coal
    3:  Silver Ore
    4:  Gold Ore
    5:  Gems
    6:  Mithril Ore
    7:  Adamantium Ore
    8:  Quork Crystals
    9:  Crysx Crystals
    :
    0x40:   Wild Game
    0x80:   Nightshade
    100, 104, 105, 106, 107, 108, 109, 110: Lairs (_LAIRS[].type)

*/
void Surveyor_IDK_Print_Terrain_And_Effect(int16_t IDK_type, int16_t IDK_IDK)
{
    int16_t var_2 = 0;

    Surveyor_IDK_Set_Font_Stuff__1();

    strcpy(GUI_String_1, str_empty_string__094);

    strcpy(GUI_String_2, str_empty_string__094);

    switch(IDK_type)
    {

        case 0:
        {
            Print_Centered(280, 91, aOcean);
            strcpy(GUI_String_1, str_empty_string__094);
            strcpy(GUI_String_2, str_empty_string__094);
        } break;
        case 1:
        {
            Print_Centered(280, 91, aGrasslands);
            strcpy(GUI_String_1, a112Food);
        } break;
        case 2:
        {
            Print_Centered(280, 91, aForest);
            strcpy(GUI_String_1, a12Food);
            strcpy(GUI_String_2, a3Production);
        } break;
        case 3:
        {
            Print_Centered(280, 91, aMountain);
            strcpy(GUI_String_1, a5Production);
        } break;
        case 4:
        {
            Print_Centered(280, 91, aDesert);
            strcpy(GUI_String_1, a3Production);
        } break;
        case 5:
        {
            Print_Centered(280, 91, aSwamp);
            strcpy(GUI_String_1, a12Food);
        } break;
        case 6:
        {
            Print_Centered(280, 91, aTundra);
            strcpy(GUI_String_1, str_empty_string__094);
        } break;
        case 7:
        {
            Print_Centered(280, 91, aShore);
            strcpy(GUI_String_1, a12Food);
            strcpy(GUI_String_2, a10Gold);
        } break;
        case 8:
        {
            Print_Centered(280, 91, aRiverMouth);
            strcpy(GUI_String_1, a12Food);
            strcpy(GUI_String_2, a30Gold);
        } break;
        case 9:
        {

        } break;
        case 10:
        {
            Print_Centered(280, 91, aHills);
            strcpy(GUI_String_1, a12Food);
            strcpy(GUI_String_2, a3Production);
        } break;
        case 11:
        {
            Print_Centered(280, 91, aPlains);
            strcpy(GUI_String_1, a1Food);
        } break;
        case 12:
        {
            Print_Centered(280, 91, aRiver);
            strcpy(GUI_String_1, a2Food);
            strcpy(GUI_String_2, a20Gold);
        } break;
        case 13:
        {
            Print_Centered(280, 91, aVolcano);
            strcpy(GUI_String_1, str_empty_string__094);
        } break;

    }

    Surveyor_IDK_Set_Font_Stuff__2();

    if(
        (IDK_IDK == 32)
        ||
        (IDK_IDK > 1000)
    )
    {

        strcpy(GUI_String_1, aCorruption);

        strcpy(GUI_String_2, str_empty_string__094);

        if(IDK_IDK > 1000)
        {
            IDK_IDK -= 1032;
        }

    }

    Print_Centered(280, 98, GUI_String_1);

    Print_Centered(280, 105, GUI_String_2);

    Surveyor_IDK_Set_Font_Stuff__1();

    if(IDK_IDK < 300)
    {

        strcpy(GUI_String_1, str_empty_string__094);


        var_2 = IDK_IDK;

        // vt_Survey_Display
        //   1,   2,   3,   4,   5,   6,   7,   8,   9,
        //  64, 
        // 100, 104, 105, 106, 107, 108, 109, 110,
        // 128
        switch(var_2)
        {
            case 0:
            {
            } break;
            case 1:
            {
                Print_Centered(280, 114, aIronOre);
                strcpy(GUI_String_1, aReducesNormalU);
            } break;
            case 2:
            {
                Print_Centered(280, 114, aCoal);
                strcpy(GUI_String_1, aReducesNorma_0);
            } break;
            case 3:
            {
                Print_Centered(280, 114, aSilverOre);
                strcpy(GUI_String_1, a2Gold);
            } break;
            case 4:
            {
                Print_Centered(280, 114, aGoldOre);
                strcpy(GUI_String_1, a3Gold);
            } break;
            case 5:
            {
                Print_Centered(280, 114, aGems);
                strcpy(GUI_String_1, a5Gold);
            } break;
            case 6:
            {
                Print_Centered(280, 114, aMithrilOre);
                strcpy(GUI_String_1, a1Power);
            } break;
            case 7:
            {
                Print_Centered(280, 114, aAdamantiumOre);
                strcpy(GUI_String_1, a2Power);
            } break;
            case 8:
            {
                Print_Centered(280, 114, aQuorkCrystals);
                strcpy(GUI_String_1, a3Power);
            } break;
            case 9:
            {
                Print_Centered(280, 114, aCrysxCrystals);
                strcpy(GUI_String_1, a5Power);
            } break;
            case 0x40:
            {
                Print_Centered(280, 114, aWildGame);
                strcpy(GUI_String_1, a2Food_0);
            } break;
            case 0x80:
            {
                Print_Centered(280, 114, str_Nightshade__ovr094);
                strcpy(GUI_String_1, aProtectsCityFr);
            } break;
            /* Lairs */
            case 100:
            case 104:
            case 105:
            case 106:
            case 107:
            case 108:
            case 109:
            case 110:
            {
                Print_Centered(280, 114, survey_near_alloc_1_100);
                Surveyor_IDK_Set_Font_Stuff__2();
                Print_Centered(280, 121, survey_near_alloc_2_50);
                Print_Centered(280, 128, survey_near_alloc_3_50);
                Surveyor_IDK_Set_Font_Stuff__1();
            } break;

        }

        /* Nodes */
        if(IDK_IDK >= 200)
        {

            Print_Centered(280, 114, survey_near_alloc_1_100);

            Surveyor_IDK_Set_Font_Stuff__2();

            Print_Centered(280, 121, survey_near_alloc_2_50);

            Print_Centered(280, 128, survey_near_alloc_3_50);

        }

        Surveyor_IDK_Set_Font_Stuff__2();

        Print_Paragraph(247, 121, 66, GUI_String_1, 2);

    }
    else
    {

        IDK_IDK -= 300;

        strcpy(GUI_String_1, _city_size_names[_CITIES[IDK_IDK].size]);

        strcat(GUI_String_1, aOf_0);

        Print_Centered(280, 114, GUI_String_1);

        _fstrcpy(GUI_String_1, _CITIES[IDK_IDK].name);

        Print_Centered(280, 121, GUI_String_1);

    }

}


// WZD o094p06
// drake178: sub_7B24B()
/*
    IDK/IDGI
        returns (100 + _LAIRS[itr_lairs].type)
*/
int16_t Surveyor_Lairs(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr_lairs = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

    IDK = ST_FALSE;

    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {

        if(
            (_LAIRS[itr_lairs].wp == wp)
            &&
            (_LAIRS[itr_lairs].Intact == ST_TRUE)
            &&
            (
                (_LAIRS[itr_lairs].type == 0)
                ||
                (_LAIRS[itr_lairs].type > 3)
            )
            &&
            (_LAIRS[itr_lairs].wx == wx)
            &&
            (_LAIRS[itr_lairs].wy == wy)
        )
        {

            switch(_LAIRS[itr_lairs].type)
            {
                case 0:
                {
                    strcpy(survey_near_alloc_1_100, aTower);
                } break;
                case 4:
                {
                    strcpy(survey_near_alloc_1_100, aCave);

                } break;
                case 5:
                {
                    strcpy(survey_near_alloc_1_100, aDungeon);
                } break;
                case 6:
                {
                    strcpy(survey_near_alloc_1_100, aTemple);
                } break;
                case 7:
                {
                    strcpy(survey_near_alloc_1_100, aKeep);
                } break;
                case 8:
                {
                    strcpy(survey_near_alloc_1_100, aLair);
                } break;
                case 9:
                {
                    strcpy(survey_near_alloc_1_100, aRuins);
                } break;
                case 10:
                {
                    strcpy(survey_near_alloc_1_100, aTemple);
                } break;
            }

            strcpy(survey_near_alloc_3_50, str_empty_string__094);

            if(Test_Bit_Field(1, &_LAIRS[itr_lairs].Misc_Flags) == ST_FALSE)
            {
                strcpy(survey_near_alloc_2_50, aUnexplored);
            }
            else
            {

                strcpy(survey_near_alloc_2_50, *_unit_type_table[_LAIRS[itr_lairs].guard1_unit_type].name);

                if(
                    (Test_Bit_Field(2, &_LAIRS[itr_lairs].Misc_Flags) != ST_FALSE)
                    &&
                    ((_LAIRS[itr_lairs].guard2_count & 0x0F) > 0)
                )
                {
                    strcpy(survey_near_alloc_3_50, *_unit_type_table[_LAIRS[itr_lairs].guard2_unit_type].name);
                }

            }

            IDK = (100 + _LAIRS[itr_lairs].type);

        }

    }

    return IDK;

}


// WZD o094p07
// drake178: sub_7B451()
int16_t Surveyor_Nodes(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t var_4 = 0;
    int16_t IDK = 0;
    int16_t itr_nodes = 0;  // _DI_
    int16_t itr_lairs = 0;  // _SI_

    IDK = ST_FALSE;

    for(itr_nodes = 0; ((itr_nodes < NUM_NODES) && (IDK = ST_FALSE)); itr_nodes++)
    {

        if(
            (_NODES[itr_nodes].wp == wp)
            &&
            (_NODES[itr_nodes].wx == wx)
            &&
            (_NODES[itr_nodes].wy == wy)
        )
        {

            switch(_NODES[itr_nodes].type)
            {
                case 0:
                {
                    strcpy(survey_near_alloc_1_100, aSorceryNode);
                } break;
                case 1:
                {
                    strcpy(survey_near_alloc_1_100, aNatureNode);
                } break;
                case 2:
                {
                    strcpy(survey_near_alloc_1_100, aChaosNode);
                } break;
                
            }

            strcpy(survey_near_alloc_2_50, str_empty_string__094);

            strcpy(survey_near_alloc_3_50, str_empty_string__094);

            if(_NODES[itr_nodes].owner_idx != ST_UNDEFINED)
            {

                if((_NODES[itr_nodes].flags & NF_WARPED) != 0)
                {
                    strcpy(survey_near_alloc_2_50, aWarped);
                }
                else if((_NODES[itr_nodes].flags & NF_GUARDIAN) != 0)
                {
                    strcpy(survey_near_alloc_2_50, aGuardianSpir_0);
                }
                else
                {
                    strcpy(survey_near_alloc_2_50, aMagicSpirit_0);
                }

            }
            else
            {

                var_4 = ST_FALSE;

                for(itr_lairs = 0; ((itr_lairs < NUM_LAIRS) && (var_4 == ST_FALSE)); itr_lairs++)
                {

                    if(
                        (_LAIRS[itr_lairs].wp == wp)
                        &&
                        (_LAIRS[itr_lairs].Intact == ST_TRUE)
                        &&
                        (_LAIRS[itr_lairs].wx == wx)
                        &&
                        (_LAIRS[itr_lairs].wy == wy)
                    )
                    {

                        var_4 = ST_TRUE;

                        strcpy(survey_near_alloc_3_50, str_empty_string__094);

                        if(Test_Bit_Field(1, &_LAIRS[itr_lairs].Misc_Flags) == ST_FALSE)
                        {
                            strcpy(survey_near_alloc_2_50, aUnexplored);
                        }
                        else
                        {

                            strcpy(survey_near_alloc_2_50, *_unit_type_table[_LAIRS[itr_lairs].guard1_unit_type].name);

                            if(
                                (Test_Bit_Field(2, &_LAIRS[itr_lairs].Misc_Flags) != ST_FALSE)
                                &&
                                ((_LAIRS[itr_lairs].guard2_count & 0x0F) > 0)
                            )
                            {
                                strcpy(survey_near_alloc_3_50, *_unit_type_table[_LAIRS[itr_lairs].guard2_unit_type].name);
                            }

                        }

                    }

                }
                
            }

            IDK = (200 + itr_nodes);

        }

    }

    return IDK;

}


// WZD o094p08
// drake178: sub_7B6EA()
/*
    IDK/IDGI
        returns (300 + itr_cities)
*/
int16_t Surveyor_Cities(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr_cities = 0;  // _CX_
    int16_t return_value = 0;  // _SI_

    return_value = ST_FALSE;

    for(itr_cities = 0; ((itr_cities < _cities) && (return_value == ST_FALSE)); itr_cities++)
    {

        if(
            (_CITIES[itr_cities].wp == wp)
            &&
            (_CITIES[itr_cities].wy == wy)
            &&
            (_CITIES[itr_cities].wx == wx)
        )
        {
            return_value = (300 + itr_cities);
        }

    }

    return return_value;
}


// WZD o094p09
void Surveyor_IDK_Set_Font_Stuff__1(void)
{
    uint8_t colors[4] = { 0, 0, 0, 0 };

    colors[0] = 8;
    colors[1] = 161;
    colors[2] = 161;

    Set_Font_Colors_15(1, &colors[0]);

    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Set_Outline_Color(148);

    Set_Font_Spacing_Width(1);

    Set_Font_LF(1);

}


// WZD o094p10
// drake178: sub_7B7AB()
void Surveyor_IDK_Set_Font_Stuff__2(void)
{

    Set_Font_Style_Shadow_Down(1, 0, 0, 0);

    Set_Outline_Color(148);

    Set_Font_Spacing_Width(1);

    Set_Font_LF(1);

}


// WZD o094p11
// drake178: sub_7B7E3()
/*
    IDK, IDGI, WTF

    off the map?

Surveyor_Window_Display()
    if(Surveyor_IDK_RiverMouth(l_mx, l_my, _map_plane) == ST_TRUE)
    {
        Surveyor_IDK_Print_Terrain_And_Effect(8, IDK)
    }
    else
    {
        Surveyor_IDK_Print_Terrain_And_Effect(12, IDK)
    }
...
st_08:                                  ; case 0x8
    Print_Centered(280, 91, aRiverMouth);
    strcpy(GUI_String_1, a12Food);
    strcpy(GUI_String_2, a30Gold);
st_12:                                  ; case 0x8
    Print_Centered(280, 91, aRiver);
    strcpy(GUI_String_1, a2Food);
    strcpy(GUI_String_2, a20Gold);

*/
int16_t Surveyor_IDK_RiverMouth(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t wx_alt;  // _DI_

    if(
        (
            ((wy - 1) > 0)
            &&
            (Square_Is_OceanLike(wx, wy, wp) == ST_TRUE)
        )
    )
    {
        return ST_TRUE;
    }

    if(
        ((wy + 1) < WORLD_HEIGHT)
        &&
        (Square_Is_OceanLike(wx, wy, wp) == ST_TRUE)
    )
    {
        return ST_TRUE;
    }

    wx_alt = (wx - 1);

    if(wx_alt < 0)
    {
        wx_alt = WORLD_WIDTH;
    }

    if(Square_Is_OceanLike(wx_alt, wy, wp) == ST_TRUE)
    {
        return ST_TRUE;
    }

    return ST_FALSE;
}
