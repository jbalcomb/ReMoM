/*
    Armies Screen

    WIZARDS.EXE
        ovr066
    
    MoO2:
        Module: ¿ Fleet Screen ?
*/

#include "MoX.H"
#include "ArmyList.H"
#include "MainScr.H"  /* Select_Unit_Stack() */
#include "MainScr_Maps.H"  /* List_Screen_Draw_Reduced_Map() */


/*
Army List Screen Background

ARMYLIST.LBX
    Entries: 9
    Type:    0  ("Pictures")

Name        Description
ARMYLIST    armylist backgrnd
ARMYLBUT    armylist up but
ARMYLBUT    armylist down but
ARMLBUT2    armylist items bu
ARMLBUT2    armylist ok but
HEROBACK    hero background
HEROBAC2    hero outline
HEROBUT4    hero alchemy butt
HEROBUT4    hero ok butt

*/




// ¿ BEGIN: ArmyList Screen - Initialized Data ?

// WZD dseg:3244
char armylist_hotkey_NUL = '\0';
// char armylist_hotkey_NUL[] = "";
// WZD dseg:3245
char cnst_HOTKEY_I_2 = 'I';
// WZD dseg:3247
char cnst_HOTKEY_O_4 = 'O';
// WZD dseg:3249
char cnst_HOTKEY_Esc5 = '\x1B';
// WZD dseg:324B
char cnst_HOTKEY_U_3 = 'U';
// WZD dseg:324D
char cnst_HOTKEY_D_3 = 'D';
// WZD dseg:324F
char aTheArmiesOf[] = "The Armies Of ";
// WZD dseg:325E
char cnst_Upkeep[] = "UPKEEP";
// WZD dseg:3265
char cnst_ARMYLIST_File2[] = "ARMYLIST";
// WZD dseg:326E
char cnst_PORTRAIT_File[] = "PORTRAIT";
// WZD dseg:3277 00                                              db    0
// WZD dseg:3278 05 07 0A 0B 0D 0E                               NoSide_Explores db SCT_BottomLeft or SCT_TopRight
// WZD dseg:3278                                                                                         ; DATA XREF: TILE_ExploreCorners+Do

// ¿ END: ArmyList Screen - Initialized Data ?





// WZD dseg:956E 00 00                                           mirror_screen_background dw 0           ; DATA XREF: Main_Screen_Load_Pictures+35Fw ...

// WZD dseg:9570
// ; 2 frame image (normal - clicked)
SAMB_ptr armylist_ok_button_seg;
// WZD dseg:9572
// ; 2 frame image (normal - clicked)
SAMB_ptr armylist_items_button_seg;
// WZD dseg:9574
// ; 2 frame image (normal - clicked)  ; should use dseg:9510
SAMB_ptr armylist_down_button_seg;
// WZD dseg:9576
// ; 2 frame image (normal - clicked)  ; should use dseg:9512
SAMB_ptr armylist_up_button_seg;
// WZD dseg:9578
SAMB_ptr armylist_background_seg;

// WZD dseg:957A 00 00 00 00 00 00 00 00 00 00 00 00             IMG_ARMY_ItemSlots@ ISLT_ICONS <0>      ; DATA XREF: GFX_Swap_AppendItems+A3w ...
// WZD dseg:957A                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:957A                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:9586 00 00                                           IMG_ItemScrn_Ok@ dw 0                   ; DATA XREF: ITEM_ScreenRedraw+3D6r ...
// WZD dseg:9586                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9588 00 00                                           IMG_ItemScrn_Alch@ dw 0                 ; DATA XREF: ITEM_ScreenRedraw+3AEr ...
// WZD dseg:9588                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:958A 00 00                                           IMG_ItemScrn_HeroBr@ dw 0               ; DATA XREF: ITEM_ScreenRedraw+9Cr ...
// WZD dseg:958C 00 00                                           IMG_ItemScrn_BG@ dw 0                   ; DATA XREF: Items_Screen+2Fr ...
// WZD dseg:958E 00 00                                           profile_gem dw 0                        ; DATA XREF: Magic_Screen_Load_Pictures+BFw ...



// WZD dseg:95F2 00 00                                           goto_booty_icon dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+371w ...

// WZD dseg:95F4
// drake178: IMG_Hero_Portraits
SAMB_ptr hero_portraits_seg[6];

// WZD dseg:9600 00 00 00 00                                     TBL_TERRSTAT_EMS@ dd 0                  ; allocated in the overland djikstra patch





// WZD o66p01
/*
TODO(JimBalcomb,20230922): finish ArmyList_Screen()
    turn back on drawing units, hero portraits, reduced map, unit(+race) name
    buttons moving on any/every click  ...gets to field push down?
    default state is down? ...gets to field push down?
    getting units that aren't owned
    not showing muliple units on a row
    not highlighting the scanned unit
    not drawing hero portraits
    reduced map location?
    reduced map mark?
    *.* unit enchantments
    *.* Unit Statistics Window
    *.* Hero Statistics Window
*/
void ArmyList_Screen(void)
{
    
    int16_t hotkey_ESC;
    int16_t IDK_have_active_stack;
// OBtm= word ptr -1Eh
// ORgt= word ptr -1Ch
// OTop= word ptr -1Ah
// OLft= word ptr -18h
    int16_t screen_changed;
    int16_t button_armylist_items;
    int16_t button_armylist_ok;
    int16_t hotkey_D;
    int16_t hotkey_U;
    int16_t button_armylist_down_left;
    int16_t button_armylist_up_left;
    int16_t button_armylist_down_right;
    int16_t button_armylist_up_right;
    int16_t scanned_field;
    int16_t leave_screen_flag;
    
    int16_t input_field_idx;
    int16_t hotkey_idx_ESC;

    int16_t itr_hero_portraits;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ArmyList_Screen()\n", __FILE__, __LINE__);
#endif

    ArmyList_Screen_Load();

    armylist_start_x = 77;
    armylist_start_y = 24;
    armylist_hero_portrait_start_x = 12;
    armylist_hero_portrait_start_y = 5;
    // TODO  UU_IDK_ArmyList_w42B70 = 5;  // ¿ was to be start_y of second column of hero portraits ?
    // TODO  UU_IDK_ArmyList_w42B6E = 168;  // ¿ was to be start_x of second column of hero portraits ?

    Build_Army_List();

    list_first_item = 0;

    Build_Army_List_Counts();

    // TODO  Deactivate_Auto_Function
    // TODO  Assign_Auto_Function(ArmyList_Screen_Draw(), 1);
    Set_Input_Delay(1);

    IDK_have_active_stack = ST_FALSE;
    if(_unit_stack_count != 0)
    {
        DLOG("(_unit_stack_count != 0)");
        IDK_have_active_stack = ST_TRUE;
    }
    else
    {
        DLOG("(_unit_stack_count == 0)");
    }

    ArmyList_Draw_Reduced_Map();

    armylist_upkeep_gold = Player_Armies_Gold_Upkeep(_human_player_idx);
    // TODO  armylist_upkeep_mana = WIZ_TotalUpkeep_Mana(_human_player_idx);
    armylist_upkeep_food = Player_Armies_Food_Upkeep(_human_player_idx);
    // armylist_upkeep_gold = 70;
    armylist_upkeep_mana =  0;
    armylist_upkeep_food = 64;

    // ; loads the palette for an LBX image, if it contains  ; one (frame 0 only)
    // ¿ using 'armylist_up_button_seg, because it gets loaded as the first sub-allocation in _screen_seg ?
    Load_Palette_From_Animation(armylist_up_button_seg);
    Apply_Palette();
    Clear_Palette_Changes(0, 243);
    Set_Palette_Changes(0, 255);
    Update_Remap_Color_Range(0, 1);

    // TODO  Deactivate_Help_List();

    // WZD ovr104  o104p02
    // TODO  ArmyList_Load_Help();
    /*
        LBX_Load_Data_Static(hlpentry_lbx_file, 2, _help_entries, 0, 16, 10)
        Set_Help_List(_help_entries, 16)
    */

    armylist_item_scanned_field = 0;  /* ¿ Field NONE ?*/

    screen_changed = ST_FALSE;  // DNE in Dasm

    leave_screen_flag = ST_FALSE;

    while(leave_screen_flag == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();

        ArmyList_Add_List_Fields();


        // DEMO
        hotkey_idx_ESC = Add_Hidden_Field(0, 0, 319, 199, 27, -1);


        button_armylist_items = Add_Button_Field(273, 163, "", armylist_items_button_seg, cnst_HOTKEY_I_2, ST_UNDEFINED);

        button_armylist_ok = Add_Button_Field(273, 182, "", armylist_ok_button_seg, cnst_HOTKEY_O_4, ST_UNDEFINED);
        
        hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc5);

        // YayNay Army List Scroll Up
        if(list_first_item != 0)
        {
            hotkey_U = Add_Hot_Key(cnst_HOTKEY_U_3);
            button_armylist_up_right = Add_Button_Field(250, 26, "", armylist_up_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
            button_armylist_up_left = Add_Button_Field(60, 26, "", armylist_up_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
        }
        else
        {
            hotkey_U = INVALID_FIELD;
            button_armylist_up_right = INVALID_FIELD;
            button_armylist_up_left = INVALID_FIELD;
        }
        
        // YayNay Army List Scroll Down
        if( (((list_first_item + 6) - armylist_army_count) != 0) && ((list_item_count - armylist_army_count) != 0) )
        {
            hotkey_D = Add_Hot_Key(cnst_HOTKEY_D_3);
            button_armylist_down_right = Add_Button_Field(250, 139, "", armylist_down_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
            button_armylist_down_left = Add_Button_Field(60, 139, "", armylist_down_button_seg, armylist_hotkey_NUL, ST_UNDEFINED);
        }
        else
        {
            hotkey_D = INVALID_FIELD;
            button_armylist_down_right = INVALID_FIELD;
            button_armylist_down_left = INVALID_FIELD;
        }

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();

    /*
        Demo
    */
        if(input_field_idx == hotkey_idx_ESC)
        {
            leave_screen_flag = ST_TRUE;
        }

    /*
        Leave Screen
    */
    if(input_field_idx == button_armylist_ok || input_field_idx == hotkey_ESC)
    {
        // SND_LeftClickSound();
        leave_screen_flag = ST_TRUE;
        current_screen = scr_Main_Screen;
        // TODO  RP_WIZ_ReturnZero(_human_player_idx);
        if(IDK_have_active_stack != ST_TRUE)
        {
            _unit_stack_count = 0;
        }
        else
        {
            Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _UNITS[_unit].world_x, _UNITS[_unit].world_y);
        }
    }

    /*
        Items
    */
    if(input_field_idx == button_armylist_items)
    {
            // TODO  SND_LeftClickSound();
            leave_screen_flag = ST_TRUE;
            // TODO  ITEM_CurrentAward = ST_UNDEFINED;
            // TODO  current_screen = scr_Items_Screen;
    }

    /*
        Scroll Up
    */
    if( (input_field_idx == button_armylist_up_right) || (input_field_idx == button_armylist_up_left) || (input_field_idx == hotkey_U) )
    {
        // TODO  SND_LeftClickSound();
        if(list_first_item != 0)
        {
            list_first_item--;
            Build_Army_List_Counts();
            screen_changed = ST_TRUE;
        }
    }

    /*
        Scroll Down
    */
    if( (input_field_idx == button_armylist_down_right) || (input_field_idx == button_armylist_down_left) || (input_field_idx == hotkey_D) )
    {
        // TODO  SND_LeftClickSound();
        if(list_first_item + 6 < armylist_army_count)
        {
            list_first_item++;
            Build_Army_List_Counts();
            screen_changed = ST_TRUE;
        }
    }

    /*
        ¿ left-click army list unit ?
    */
    /*
        ¿ right-click army list unit ?
    */

    /*
        scanned army list unit
    */

    /*
        scanned hero portrait
    */
    for(itr_hero_portraits = 0; itr_hero_portraits < armylist_hero_portrait_count; itr_hero_portraits++)
    {
        if(armylist_hero_portrait_fields[itr_hero_portraits] == scanned_field)
        {
            armylist_item_scanned_field = Get_List_Armies_Index(armylist_hero_portrait_unit_indices[itr_hero_portraits]);
        }
    }

    /*
        ¿ left-click hero portrait ?
    */

    /*
        ¿ right-click hero portrait ?
    */



        if(leave_screen_flag == ST_FALSE && screen_changed == ST_FALSE)
        {
            ArmyList_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
        screen_changed = ST_FALSE;
    }

    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    // TODO  Update_Remap_Gray_Palette();
    // TODO  Deactivate_Auto_Function();
    // TODO  Deactivate_Help_List();
    Reset_Window();
    Clear_Fields();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: ArmyList_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o66p02
void ArmyList_Screen_Draw(void)
{
    uint8_t colors1[6];
    uint8_t colors2[6];
    int16_t unit_race;
    int16_t hero_unit_idx;
    int16_t scanned_unit_idx;
    int16_t unit_enchantment_count;
    int16_t unit_type;
    int16_t list_armies_idx;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t IDK_current_armylist_item;

    int16_t itr_colors;
    int16_t itr_hero_portraits;
    int16_t itr_list_item_count;
    int16_t itr_unit_enchantment_count;

    int16_t IDK_Hero_Slot;  // _DI_
    int16_t print_y_offset;  // _DX_

    int16_t unit_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ArmyList_Screen_Draw()\n", __FILE__, __LINE__);
#endif


    ArmyList_Set_List_Item_Count();

    Set_Page_Off();
    Copy_Back_To_Off();

    // BEGIN: Print Title

    strcpy(GUI_String_1, aTheArmiesOf);
    strcat(GUI_String_1, _players[_human_player_idx].Name);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 237;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(162, 9, GUI_String_1);
    Print_Centered(161, 9, GUI_String_1);
    Print_Centered(162, 8, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 238;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 7, GUI_String_1);
    Print_Centered(160, 8, GUI_String_1);
    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 239;
    }
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(161, 8, GUI_String_1);
    colors2[0] = 236;
    colors2[1] = 177;
    colors2[2] = 178;
    colors2[3] = 177;
    colors2[4] = 176;
    Set_Font_Colors_15(4, &colors2[0]);
    Print_Centered(160, 7, GUI_String_1);

    // END: Print Title


    Set_Outline_Color(231);
    colors2[0] = 236;
    colors2[1] = 129;
    colors2[2] = 129;
    Set_Font_Colors_15(1, &colors2[0]);  // ¿ sets font style 1 and font remap colors block ?
    Set_Font_Style1(1, 15, 0, 0);  // shadow - bottom-right, 1 pixel; use special/custom colors font color block

    Print_Integer_Centered(45, 173, armylist_upkeep_gold);
    Print_Integer_Centered(45, 183, armylist_upkeep_mana);
    Print_Integer_Centered(45, 192, armylist_upkeep_food);
    Print_Centered(30, 162, cnst_Upkeep);

    /*
        Up Arrows
    */
    if(list_first_item == 0 && armylist_army_count > 6)
    {
        FLIC_Set_CurrentFrame(armylist_up_button_seg, 1);
        FLIC_Draw(250, 26, armylist_up_button_seg);
        FLIC_Set_CurrentFrame(armylist_up_button_seg, 1);
        FLIC_Draw(60, 26, armylist_up_button_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(armylist_up_button_seg, 0);
        FLIC_Draw(250, 26, armylist_up_button_seg);
        FLIC_Set_CurrentFrame(armylist_up_button_seg, 0);
        FLIC_Draw(60, 26, armylist_up_button_seg);
    }

    /*
        Down Arrows
    */
    if( armylist_army_count > 6 && ( (((list_first_item + 6) - armylist_army_count) == 0)  || ((list_item_count - armylist_army_count) == 0) ) )
    {
        FLIC_Set_CurrentFrame(armylist_down_button_seg, 1);
        FLIC_Draw(250, 139, armylist_down_button_seg);
        FLIC_Set_CurrentFrame(armylist_down_button_seg, 1);
        FLIC_Draw(60, 139, armylist_down_button_seg);
    }
    else
    {
        FLIC_Set_CurrentFrame(armylist_down_button_seg, 0);
        FLIC_Draw(250, 139, armylist_down_button_seg);
        FLIC_Set_CurrentFrame(armylist_down_button_seg, 0);
        FLIC_Draw(60, 139, armylist_down_button_seg);
    }

    FLIC_Draw(273, 163, armylist_items_button_seg);

    FLIC_Draw(273, 182, armylist_ok_button_seg);

    if(armylist_army_count <= 6)
    {
        // TODO  mov     [word ptr _help_entries+78h], 0FFFFh
        // TODO  mov     [word ptr _help_entries+82h], 0FFFFh
        // TODO  mov     [word ptr _help_entries+8Ch], 0FFFFh
        // TODO  mov     [word ptr _help_entries+96h], 0FFFFh
    }

    scanned_unit_idx = list_armies[armylist_item_scanned_field];

    Set_Outline_Color(240);

    /*
        Hero Portraits
    */
    for(itr_hero_portraits = 0; itr_hero_portraits < NUM_HEROES; itr_hero_portraits++)
    {
        hero_unit_idx = _players[_human_player_idx].Heroes->Unit_Index;
        if( (hero_unit_idx <= ST_UNDEFINED) || (_UNITS[hero_unit_idx].owner_idx == ST_UNDEFINED) )
        {
            // ¿ help entry offset ? (itr * 10)
            // TODO  *(_help_entries + 60 + (10 * itr)) = 243;  // "EMPTY HERO SLOT"
        }
        else
        {
            ArmyList_Hero_Portrait_Coords(itr_hero_portraits, &x1, &y1, &x2, &y2);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: hero_portraits_seg[%d]: %p\n", __FILE__, __LINE__, itr_hero_portraits, hero_portraits_seg[itr_hero_portraits]);
#endif
            // DEMO  FLIC_Draw(x1, y1, hero_portraits_seg[itr_hero_portraits]);

            colors2[0] = 236;
            colors2[1] = 129;
            colors2[2] = 129;
            Set_Font_Colors_15(0, &colors2[0]);
            Set_Font_Style1(0, 15, 0, 0);
            Set_Font_LF(0);
            Set_Font_Spacing_Width(1);
            Print_Centered(x1 + 15, y1 + 37, _players[_human_player_idx].Heroes->Name);

            // ¿ help entry offset ? (itr * 10)
            // TODO  *(_help_entries + 60 + (10 * itr)) = ST_UNDEFINED;
        }
    }

    Set_Outline_Color(231);

    /*
        Unit Static Figures
    */
    list_armies_idx = 0;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: list_item_count: %d\n", __FILE__, __LINE__, list_item_count);
#endif
    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        IDK_current_armylist_item = 0;
        if(armylist_list_item_count[itr_list_item_count] > IDK_current_armylist_item)
        {
            DLOG("(armylist_list_item_count[itr_list_item_count] > IDK_current_armylist_item)");
            x1 = armylist_start_x + (18 * IDK_current_armylist_item);
            y1 = armylist_start_y + (22 * itr_list_item_count);
            if(list_armies_idx == armylist_item_scanned_field)
            {
                // DEMO  Gradient_Fill(x1 + 1, y1 + 2, x1 + 19, y1 + 16, 15, 1, ST_NULL, ST_NULL, ST_NULL);
            }
            unit_idx = list_armies[list_armies_idx];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: unit_idx: %d\n", __FILE__, __LINE__, unit_idx);
#endif
            // DEMO  Draw_Unit_StatFig(x1, y1, list_armies[list_armies_idx], 1);
            list_armies_idx++;
            IDK_current_armylist_item++;
        }
        else
        {
            DLOG("(armylist_list_item_count[itr_list_item_count] <= IDK_current_armylist_item)");
        }
    }

    Cycle_Unit_Enchantment_Animation();

    if(armylist_army_count > 0)
    {
        DLOG("armylist_army_count > 0)");

        unit_type = _UNITS[scanned_unit_idx].type;

        IDK_Hero_Slot = _UNITS[scanned_unit_idx].Hero_Slot;
        if(IDK_Hero_Slot != ST_UNDEFINED)
        {
            // offset _players.Heroes.Name
            // + _human_player_idx * 0x4C8
            // + _UNITS[scanned_unit_idx].Hero_Slot * 0x1C
            strcpy(GUI_String_1, _players[_human_player_idx].Heroes[_UNITS[scanned_unit_idx].Hero_Slot].Name);
        }
        else
        {
            // unit_type * 0x24  sizeof(struc s_UNIT_TYPE)
            // DSEG:01B6  _unit_type_table.Attribs_1
            if((_unit_type_table[unit_type].Attribs_1 & 0x8000) == 0)  /* enum ATTRIB_1 Std_Unit */
            {
                /* "Standard Units" */
                strcpy(GUI_String_1, *(_unit_type_table[unit_type].Name));
            }
            else
            {
                /* Race-Specific Units */
                unit_race = _unit_type_table[unit_type].Race;
                if(unit_race < 15)
                {
                    // TODO  strcpy(GUI_String_1, _unit_race_table[unit_race].Name);
                    strcpy(GUI_String_1, "RACIAL");
                }
                else
                {
                    /* Summoned Units */
                    // mov     bx, [bp+unit_race]
                    // shl     bx, 1
                    // push    [(TBL_Races.Growth_Mod+120h)+bx]
                    // DSEG:1EBA
                    // ¿ indexing into an array of 21 DW's (42 bytes) of offsets to (char *) strings of Summoned Unit Race Names ?
                    // starting at 15 * 2 ... 6 offsets ... Arcane, Nature, Sorcery, Chaos, Life, Death
                    strcpy(GUI_String_1, "SUMMONED");
                }
                    strcat(GUI_String_1, " ");  // offset aTheArmiesOf+0Dh AKA aTheArmiesOf_SPACE
                    strcat(GUI_String_1, *(_unit_type_table[unit_type].Name));
            }
        }

        Set_Outline_Color(231);
        colors2[0] = 236;
        colors2[1] = 129;
        colors2[2] = 129;
        Set_Font_Colors_15(1, &colors2[0]);
        Set_Font_Style1(1, 15, 0, 0);
        String_To_Upper(GUI_String_1);
        // DEMO  Print_Centered(188, 161, GUI_String_1);


        // populate echantment name strings and count for specified unit
        // WZD ovr089
        // TODO  IDK_Get_Unit_Enchantment_Names(scanned_unit_idx, armylist_20x20, &unit_enchantment_count);
        unit_enchantment_count = 0;

        if(unit_enchantment_count > 0)
        {
            Set_Font_Style1(1, 15, 0, 0);
            // DONT  IDK_Hero_Slot = unit_enchantment_count;
            // DONT  if(IDK_Hero_Slot > 4)
            // DONT  {
            // DONT      IDK_Hero_Slot = 4;
            // DONT  }
            if(unit_enchantment_count > 4)
            {
                unit_enchantment_count = 4;
            }
            for(itr_unit_enchantment_count = 0; itr_unit_enchantment_count < unit_enchantment_count; itr_unit_enchantment_count++)
            {
                if(unit_enchantment_count < 4)
                {
                    print_y_offset = 7;
                }
                else
                {
                    print_y_offset = 6;
                }
                Print(144, 172 + (print_y_offset * itr_unit_enchantment_count), armylist_20x20[itr_unit_enchantment_count]);
            }
        }

    }  /* if(armylist_army_count > 0) */
    else
    {
        DLOG("armylist_army_count <= 0)");
    }


    Set_Outline_Color(0);

    // DEMO  ArmyList_Draw_Reduced_Map();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: ArmyList_Screen_Draw()\n", __FILE__, __LINE__);
#endif

}


// WZD o66p03
/*
    armylist_fields[] field_idx for each Unit
    armylist_hero_portrait_fields[] field_idx for each Hero Portrait
    ¿ IDK_armylist_row_fields[] field_idx for each Row/Item List ?
*/
void ArmyList_Add_List_Fields(void)
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
    int16_t hero_unit_idx;
    int16_t itr_list_item_count;
    int16_t itr_armylist_list_item_count;
    int16_t itr_num_hero_portrait;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ArmyList_Add_Fields()\n", __FILE__, __LINE__);
#endif

    armylist_item_count = 0;

    ArmyList_Set_List_Item_Count();

    /*
        armylist_unit_fields
    */
    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        for(itr_armylist_list_item_count = 0; itr_armylist_list_item_count < armylist_list_item_count[itr_list_item_count]; itr_armylist_list_item_count++)
        {
            x1 = armylist_start_x + (18 * itr_armylist_list_item_count) + 1;
            y1 = armylist_start_y + (22 * itr_list_item_count) + 1;
            x2 = x1 + 18;
            y2 = y1 + 15;
            armylist_unit_fields[armylist_item_count] = Add_Hidden_Field(x1, y1, x2, y2, armylist_hotkey_NUL, ST_UNDEFINED);
            armylist_item_count++;
        }
    }

    /*
        IDK_armylist_row_fields
    */
    IDK_armylist_row_count = 0;
    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        x1 = armylist_start_x + 1;
        y1 = armylist_start_y + (22 * itr_list_item_count) + 1;
        x2 = x1 + 162;
        y2 = y1 + 15;
        IDK_armylist_row_fields[IDK_armylist_row_count] = Add_Hidden_Field(x1, y1, x2, y2, armylist_hotkey_NUL, ST_UNDEFINED);
        IDK_armylist_row_count++;
    }

    /*
        armylist_hero_portrait_fields
    */
    armylist_hero_portrait_count = NUM_HERO_PORTRAIT;
    for(itr_num_hero_portrait = 0; itr_num_hero_portrait < NUM_HERO_PORTRAIT; itr_num_hero_portrait++)
    {
        hero_unit_idx = _players[_human_player_idx].Heroes->Unit_Index;
        if( (hero_unit_idx <= ST_UNDEFINED) || (_UNITS[hero_unit_idx].owner_idx == ST_UNDEFINED) )
        {
            armylist_hero_portrait_fields[itr_num_hero_portrait] = INVALID_FIELD;
            armylist_hero_portrait_unit_indices[itr_num_hero_portrait] = ST_UNDEFINED;
        }
        else
        {
            ArmyList_Hero_Portrait_Coords(itr_num_hero_portrait, &x1, &y1, &x2, &y2);
            armylist_hero_portrait_fields[itr_num_hero_portrait] = Add_Hidden_Field(x1, y1, x2, y2, armylist_hotkey_NUL, ST_UNDEFINED);
            armylist_hero_portrait_unit_indices[itr_num_hero_portrait] = _players[_human_player_idx].Heroes->Unit_Index;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: ArmyList_Add_Fields()\n", __FILE__, __LINE__);
#endif
}


// WZD o66p04
// IDK_ArmyList_s59569


// WZD o66p05
void ArmyList_Draw_Reduced_Map(void)
{
    int16_t unit_world_x;
    int16_t unit_world_y;
    int16_t unit_world_p;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ArmyList_Draw_Reduced_Map()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: armylist_item_scanned_field: %d\n", __FILE__, __LINE__, armylist_item_scanned_field);
    dbg_prn("DEBUG: [%s, %d]: list_armies[armylist_item_scanned_field]: %d\n", __FILE__, __LINE__, list_armies[armylist_item_scanned_field]);
#endif

    unit_world_x = _UNITS[list_armies[armylist_item_scanned_field]].world_x;
    unit_world_y = _UNITS[list_armies[armylist_item_scanned_field]].world_y;
    unit_world_p = _UNITS[list_armies[armylist_item_scanned_field]].world_plane;

    List_Screen_Draw_Reduced_Map(ARMYLIST_REDUCED_MAP_X, ARMYLIST_REDUCED_MAP_Y, ARMYLIST_REDUCED_MAP_W, ARMYLIST_REDUCED_MAP_H, unit_world_p, unit_world_x, unit_world_y);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: ArmyList_Draw_Reduced_Map()\n", __FILE__, __LINE__);
#endif
}


// WZD o66p06
/*
    loops through armylist_item_count[] to match armylist_hero_portrait_unit_idx
    three branches back to top of loop, but just fall-throughs
    sets list_first_item to 0 on first run through
    ¿ (only) calls Build_Army_List_Counts(), becase it changes list_first_item ?
    ¿ branch around list_first_item is to avoid changing the screen if the hero unit is already displayed ?
    so, runs thorugh current list
    then, restarts from the beginning

    calls Build_Army_List_Counts()
        calls ArmyList_Set_List_Item_Count(), which uses list_first_item to set list_item_count
        uses list_item_count to iterate through armylist_list_item_count
        uses list_first_item to while indexing into armylist_world_?_1000
        updates list_armies[]

*/
/*
    moves army list view to show scanned hero portrait unit
    returns index of list_armies[]
*/
int16_t Get_List_Armies_Index(int16_t armylist_hero_portrait_unit_idx)
{
    int16_t list_armies_itr;
    int16_t found_unit_flag;
    int16_t itr;
    int16_t first_time_done;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_List_Armies_Index(armylist_hero_portrait_unit_idx = %d)\n", __FILE__, __LINE__, armylist_hero_portrait_unit_idx);
#endif

    first_time_done = ST_FALSE;

    for(found_unit_flag = ST_FALSE, itr = 0; found_unit_flag == ST_FALSE && itr < armylist_item_count; itr++)
    {
        if(list_armies[itr] == armylist_hero_portrait_unit_idx)
        {
            list_armies_itr = itr;
            found_unit_flag = ST_TRUE;
        }

        // ¿ not found in current list_armies[], so reset/increment and rebuild list_armies[] ?
        if(found_unit_flag == ST_FALSE)
        {
            if(first_time_done == ST_FALSE)
            {
                list_first_item = 0;
                first_time_done = ST_TRUE;
            }
            else
            {
                list_first_item++;
            }
            Build_Army_List_Counts();
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_List_Armies_Index(armylist_hero_portrait_unit_idx = %d) { list_armies_itr = %d }\n", __FILE__, __LINE__, armylist_hero_portrait_unit_idx, list_armies_itr);
#endif
    return list_armies_itr;
}


// WZD o66p07
void Build_Army_List(void)
{
    int16_t itr_units;
    int16_t itr_armylist_army_count;
    int16_t same_stack;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Build_Army_List()\n", __FILE__, __LINE__);
#endif

    armylist_unit_count = 0;
    armylist_army_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == _human_player_idx)
        {
            armylist_unit_count++;
            same_stack = ST_UNDEFINED;
            for(itr_armylist_army_count = 0; itr_armylist_army_count < armylist_army_count; itr_armylist_army_count++)
            {
                if(
                    (_UNITS[itr_units].world_x == armylist_world_x_1000[itr_armylist_army_count]) &&
                    (_UNITS[itr_units].world_y == armylist_world_y_1000[itr_armylist_army_count]) &&
                    ((_UNITS[itr_units].world_plane == armylist_world_p_1000[itr_armylist_army_count]) || (_UNITS[itr_units].In_Tower == ST_TRUE))
                )
                {
                    same_stack = ST_TRUE;
                }
            }
            if(same_stack == ST_UNDEFINED)
            {
                armylist_world_x_1000[armylist_army_count] = _UNITS[itr_units].world_x;
                armylist_world_y_1000[armylist_army_count] = _UNITS[itr_units].world_y;
                armylist_world_p_1000[armylist_army_count] = _UNITS[itr_units].world_plane;
                armylist_army_count++;
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Build_Army_List()\n", __FILE__, __LINE__);
#endif

}


// WZD o66p08
// ¿ ~== Update_Cities_List() ?
void Build_Army_List_Counts(void)
{
    int16_t armylist_unit_count;
    int16_t itr_list_item_count;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: IDK_Armies_Screen_s597F6()\n", __FILE__, __LINE__);
#endif

    ArmyList_Set_List_Item_Count();

    armylist_unit_count = 0;  /* count of units in the army */

    for(itr_list_item_count = 0; itr_list_item_count < list_item_count; itr_list_item_count++)
    {
        *(armylist_list_item_count + itr_list_item_count) = 0;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {
            if( _UNITS[itr_units].owner_idx == _human_player_idx)
            {
                if(_UNITS[itr_units].world_x == armylist_world_x_1000[(list_first_item + itr_list_item_count)])
                {
                    if(_UNITS[itr_units].world_y == armylist_world_y_1000[(list_first_item + itr_list_item_count)])
                    {
                        if(_UNITS[itr_units].world_plane == armylist_world_p_1000[(list_first_item + itr_list_item_count)])
                        {
                            list_armies[armylist_unit_count] = itr_units;  // ¿ ~== unit_idx ? Yup. SEE: 'left-click army list unit'
                            armylist_unit_count++;
                            armylist_list_item_count[itr_list_item_count] = armylist_list_item_count[itr_list_item_count] + 1;
                        }
                    }
                }
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: IDK_Armies_Screen_s597F6()\n", __FILE__, __LINE__);
#endif
}


// WZD o66p09
// ~== CityList_Set_List_Item_Count()
/*
    sets list item count to min of 6 or army count
    (expects list_first_item and armylist_army_count to already be set)
*/
void ArmyList_Set_List_Item_Count(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ArmyList_Set_List_Item_Count()\n", __FILE__, __LINE__);
#endif

    list_item_count = NUM_ARMY_LIST + list_first_item;

    if(list_item_count > NUM_ARMY_LIST)
    {
        list_item_count = NUM_ARMY_LIST;
    }

    if(list_item_count > armylist_army_count)
    {
        list_item_count = armylist_army_count;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: ArmyList_Set_List_Item_Count()\n", __FILE__, __LINE__);
#endif
}



// WZD o66p10
void ArmyList_Screen_Load(void)
{
    int16_t itr;
    int16_t hero_unit_idx;

    int16_t hero_portrait_lbx_entry_num;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: ArmyList_Screen_Load()\n", __FILE__, __LINE__);
#endif

    armylist_world_x_1000 = Near_Allocate_First(1000);  // 1000 is one byte per unit of 1000 unit count max
    armylist_world_y_1000 = Near_Allocate_Next(1000);  // 1000 is one byte per unit of 1000 unit count max
    armylist_world_p_1000 = Near_Allocate_Next(1000);  // 1000 is one byte per unit of 1000 unit count max

    armylist_list_item_count = Near_Allocate_Next(54);  // ¿ only uses first six bytes ? NOT 54 for bytes for 6 x 9 Units

    list_armies = Near_Allocate_Next(108);

    GUI_String_1 = (char *)Near_Allocate_Next(100);  // why 100 here, but 80 for CityList?

    for(itr = 0; itr < 20; itr++)
    {
        armylist_20x20[itr] = (char *)Near_Allocate_Next(20);
    }

    armylist_unit_fields = Near_Allocate_Next(108);

    armylist_hero_portrait_fields = Near_Allocate_Next(12);      // 12 is 6 2-byte values, 1 per Her Portrait
    armylist_hero_portrait_unit_indices = Near_Allocate_Next(12);  // 12 is 6 2-byte values, 1 per Her Portrait
    IDK_armylist_row_fields = Near_Allocate_Next(12);  // ¿ 12 is 6 2-byte values, 1 per ArmyList List Item Field ?

    Set_Page_Off();
    armylist_background_seg = LBX_Reload("ARMYLIST", 0, _screen_seg);
    // Dump ArmyList Screen Background - 320x200, 64000 palette indices ... non-master palette indices?
    FLIC_Draw(0, 0, armylist_background_seg);
    // Dump Off-Screen VBB
    // ¿ palette indices in the non-standard palette range ?

    Copy_Off_To_Back();
    // Dump Back-Screen VBB

    armylist_up_button_seg    = LBX_Reload("ARMYLIST", 1, _screen_seg);
    armylist_down_button_seg  = LBX_Reload_Next("ARMYLIST", 2, _screen_seg);
    armylist_items_button_seg = LBX_Reload_Next("ARMYLIST", 3, _screen_seg);
    armylist_ok_button_seg    = LBX_Reload_Next("ARMYLIST", 4, _screen_seg);

    for(itr = 0; itr < NUM_HERO_PORTRAIT; itr++)
    {
        hero_unit_idx = _players[_human_player_idx].Heroes->Unit_Index;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: hero_unit_idx: %d\n", __FILE__, __LINE__, hero_unit_idx);
#endif
        if(hero_unit_idx > -1)
        {
            DLOG("(hero_unit_idx > -1)");
            if(_UNITS[hero_unit_idx].owner_idx != ST_UNDEFINED)
            {
                // IMG_Hero_Portraits[itr] = LBX_Reload_Next(cnst_PORTRAIT_File, _unit_type_table[_UNITS[hero_unit_idx].type].Bldng1_or_Portrait, _screen_seg);
                hero_portrait_lbx_entry_num = _unit_type_table[_UNITS[hero_unit_idx].type].Bldng1_or_Portrait;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: hero_portrait_lbx_entry_num: %d\n", __FILE__, __LINE__, hero_portrait_lbx_entry_num);
#endif
                hero_portraits_seg[itr] = LBX_Reload_Next("PORTRAIT", _unit_type_table[_UNITS[hero_unit_idx].type].Bldng1_or_Portrait, _screen_seg);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: hero_portraits_seg[%d]: %p\n", __FILE__, __LINE__, itr, hero_portraits_seg[itr]);
#endif
            }
        }
    }

    _reduced_map_seg = Allocate_Next_Block(_screen_seg, 153);  // 153 PR, 2448 B;  ¿ only needs 49 * 33 = 1617 ? ¿ allocating for 60 * 40 = 2400 + 3 16-byte SAMB headers ?

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Armies_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o66p11
// NX_j_IDK_ArmlyIst_PrintParagraph()
// NX_IDK_ArmlyIst_PrintParagraph()


// WZD o66p12
void ArmyList_Hero_Portrait_Coords(int16_t hero_portrait_num, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
    *x1 = armylist_hero_portrait_start_x + ((hero_portrait_num % 2) * 265);
    *y1 = armylist_hero_portrait_start_y + ((hero_portrait_num / 2) *  51);
    *x2 = *x1 + 32;
    *y2 = *y1 + 32;
}
