/*
    Item Screen

    WIZARDS.EXE
        ovr075
    
*/

#include "MoM.H"



// WZD dseg:36A0                                                 BEGIN:  ovr075 (Item Screen) - Strings

// WZD dseg:36A0 1B                                              
char cnst_HOTKEY_Esc9[] = "\x1B";
// WZD dseg:36A1 00                                              
// char cnst_ZeroString_27[] = "";
char cnst_ZeroString_27 = 0;

// WZD dseg:36A2
char cnst_ItemLoc_Same[] = "Same Location";

// WZD dseg:36B0
char cnst_ItemLoc_Diff[] = "Item Teleport";

// WZD dseg:36BE
char cnst_GP_5[] = "GP";

// WZD dseg:36C1
char cnst_MP_4[] = "MP";

// WZD dseg:36C4
char cnst_ARMYLIST_File3[] = "ARMYLIST";

// WZD dseg:36CD
char cnst_PORTRAIT_File3[] = "PORTRAIT";

// WZD dseg:36D6
char cnst_ItemDestroyMsg1[] = "Do you want to destroy your \x02";  // sets highlight colors

// WZD dseg:36F4
char cnst_ItemDestroyMsg2[] = "\x01 and gain ";  // sets normal colors

// WZD dseg:3700
char cnst_ItemDestroyMsg3[] = " mana crystals?";

// WZD dseg:3710
char cnst_ItemPort_Msg_1[] = "Do you wish to make this transfer at a cost of \x02";  // sets highlight colors

// WZD dseg:3741
char cnst_ItemPort_Msg_2[] = " mana crystals\x01\x3F";  // sets normal colors and ¿ ?

// WZD dseg:3752
char cnst_ItemPort_Msg_3[] = "You do not have enough mana crystals to transfer this item.";

// WZD dseg:378E
char cnst_HOTKEY_A_3 = 'A';

// WZD dseg:3790
char cnst_HOTKEY_O_8 = 'O';

// WZD dseg:3792
char cnst_DrownWarn_Msg[] = "This unit will drown if the item is removed.";

// WZD dseg:37BF 00                                              align 2

// WZD dseg:37BF                                                 END:  ovr075 (Item Screen) - Strings


// WZD dseg:C25E                                                 BEGIN:  ovr075

// WZD dseg:C25E
SAMB_ptr m_item_icon_workarea;

// WZD dseg:C260
int16_t item_pool_list_array[6];

// WZD dseg:C26C
int16_t item_pool_active;

// WZD dseg:C26E
int16_t item_pool_list_index;

// WZD dseg:C270
int16_t item_pool_item_idx;

// WZD dseg:C272
int16_t item_pool_list_count;

// WZD dseg:C274
int16_t * m_item_screen_hero_fields;

// WZD dseg:C276
int16_t * m_item_screen_hero_item_fields[NUM_HERO_SLOTS];

// WZD dseg:C282
int16_t * m_item_screen_vault_item_fields;

// WZD dseg:C284
int16_t m_item_screen_exit_field;

// WZD dseg:C286
int16_t m_item_screen_window_field;

// WZD dseg:C288
int16_t m_item_screen_ok_button_field;

// WZD dseg:C28A
int16_t m_item_screen_alchemy_button_field;

// WZD dseg:C28C
int16_t _item_window_start_y;

// WZD dseg:C28E
int16_t _item_window_start_x;

// WZD dseg:C290
int16_t m_item_screen_anvil_field;

// WZD dseg:C292
int16_t m_item_slot_idx;

// WZD dseg:C294
int16_t m_cursor_item_idx;

// WZD dseg:C294                                                 END:  ovr075




/*
    WIZARDS.EXE  ovr075
*/

// WZD o75p01
/*

; loads, displays, and processes the items screen,
; starting with a forced distribution of any items
; currently on the global award list
; contains some inherited BUGs, most notably about
; removing items from heroes over water

¿ _item_window_start_x,y vs. base_x,y ?

*/
void Item_Screen(void)
{
    int16_t portrait_y2;
    int16_t portrait_x2;
    int16_t portrait_y1;
    int16_t portrait_x1;
    SAMB_ptr Sandbox_Paragraph;
    int16_t hotkey_idx_ESC;
    int16_t item_slot_y;
    int16_t item_slot_x;
    int16_t UU_scanned_field;
    int16_t input_field_idx;
    int16_t leave_screen;
    int16_t itr_hero_slots; // _SI_
    int16_t itr_vault_item_slots;  // _SI_
    int16_t unit_idx;  // _DI_
    int16_t itr_item_slots;  // _DI_

    Item_Screen_Load();

    _item_window_start_x = 17;
    _item_window_start_y = 2;

    m_cursor_item_idx = ST_UNDEFINED;
    m_item_slot_idx = ST_UNDEFINED;

    Clear_Fields();

    Set_Page_Off();

    Load_Palette_From_Animation(hero_background_seg);

    Apply_Palette();

    Copy_Off_To_Back();

    Deactivate_Help_List();

    Assign_Auto_Function(Item_Screen_Draw_Do, 1);

    Set_Items_Screen_Help_List();

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        if(
            (item_pool_active == ST_TRUE)
            &&
            (m_cursor_item_idx == ST_UNDEFINED)
        )
        {
            item_pool_item_idx = item_pool_list_array[item_pool_list_index];
            item_pool_list_index++;
            if(item_pool_list_index == item_pool_list_count)
            {
                item_pool_active = ST_FALSE;
            }
            Item_Screen_Draw_Do();
            Item_Pool_View();
            m_cursor_item_idx = item_pool_item_idx;
            m_item_slot_idx = VAULT_HERO_SLOT_IDX;
            if(item_pool_in_process == ST_TRUE)
            {
                m_item_slot_idx = AWARD_HERO_SLOT_IDX;
            }
            Assign_Auto_Function(Item_Screen_Draw_Do, 1);
            Item_Screen_Load();
        }
        
        Mark_Time();
        Clear_Fields();

        Item_Screen_Add_Fields();

        // TODO  hotkey_idx_ESC = Add_Hot_Key(cnst_HOTKEY_Esc9);
        hotkey_idx_ESC = Add_Hot_Key('\x1B');
        
        input_field_idx = Get_Input();
        UU_scanned_field = Scan_Input();

        if(
            ( (input_field_idx == m_item_screen_ok_button_field)       && (m_cursor_item_idx == ST_UNDEFINED) )
            ||
            ( (input_field_idx == m_item_screen_exit_field)            && (m_cursor_item_idx == ST_UNDEFINED) )
            ||
            ( (input_field_idx == hotkey_idx_ESC)                      && (m_cursor_item_idx == ST_UNDEFINED) )
        )
        {
            // TODO  SND_LeftClickSound();
            leave_screen = ST_TRUE;
        }


        /*
            BEGIN:  Alchemy Button  (Alchemy Popup)
        */
        if(input_field_idx == m_item_screen_alchemy_button_field)
        {
            // TODO  SND_LeftClickSound();
            Alchemy_Popup((_item_window_start_x + 63), (_item_window_start_y + 58));
            Assign_Auto_Function(Item_Screen_Draw_Do, 1);
            Item_Screen_Load();
            Deactivate_Help_List();
            Set_Items_Screen_Help_List();
            PageFlipEffect = 3;  // triggers the *disintegration* effect when the Alchemy Popup exits
        }
        /*
            BEGIN:  Alchemy Button  (Alchemy Popup)
        */


        /*
            Right-Click Hero Portrait - Hero Unit Statistics Window
        */
        for(itr_hero_slots = 0; itr_hero_slots < NUM_HEROES; itr_hero_slots++)
        {
            if(m_cursor_item_idx == ST_UNDEFINED)
            {
                /*
                Right-Click on "Hero Picture"
                brings up the "Hero's Unit Statistics Window"
                USW *grows* out of the the Hero Picture
                    ¿ hence the passing of XY12 ?
                */
                if(-(m_item_screen_hero_fields[itr_hero_slots]) == input_field_idx)
                {
                    // TODO  RP_SND_LeftClickSound2();
                    Deactivate_Help_List();
                    Item_Window_Picture_Coords(itr_hero_slots, &portrait_x1, &portrait_y1, &portrait_x2, &portrait_y2);
                    unit_idx = _players[_human_player_idx].Heroes[itr_hero_slots].unit_idx;

                    USW_FullDisplay(unit_idx, portrait_x1, portrait_y1, portrait_x2, portrait_y2);

                    if(m_cursor_item_idx != ST_UNDEFINED)
                    {
                        Set_Mouse_List(1, mouse_list_none);
                    }
                    Assign_Auto_Function(Item_Screen_Draw_Do, 1);
                    Item_Screen_Load();
                    Deactivate_Help_List();
                    Set_Items_Screen_Help_List();
                }
            }
        }


        /*
            Right-Click Hero Item
        */
        {
            for(itr_hero_slots = 0; itr_hero_slots < NUM_HEROES; itr_hero_slots++)
            {
                for(itr_item_slots = 0; itr_item_slots < NUM_HERO_ITEMS; itr_item_slots++)
                {
                    if(-(m_item_screen_hero_item_fields[itr_hero_slots][itr_item_slots]) == input_field_idx)
                    {
                        if(_players[_human_player_idx].Heroes[itr_hero_slots].Items[itr_item_slots] > -1)
                        {
                            Deactivate_Help_List();
                            // TODO  RP_SND_LeftClickSound2();
                            item_slot_x = (_item_window_start_x + 54 + (((itr_hero_slots % 2) * 136) + (itr_item_slots * 29)));
                            item_slot_y = (_item_window_start_y + ((itr_hero_slots / 2) * 28));
                            if(m_cursor_item_idx != ST_UNDEFINED)
                            {
                                Set_Mouse_List(1, mouse_list_default);
                            }
                            Sandbox_Paragraph = Allocate_First_Block(_screen_seg, 1);
                            Item_View_Popup(_players[HUMAN_PLAYER_IDX].Heroes[itr_hero_slots].Items[itr_item_slots], (item_slot_x + 8), (item_slot_y + 8));
                            if(m_cursor_item_idx != ST_UNDEFINED)
                            {
                                Set_Mouse_List(1, mouse_list_none);
                            }
                            Assign_Auto_Function(Item_Screen_Draw_Do, 1);
                            Item_Screen_Load();
                            Deactivate_Help_List();
                            Set_Items_Screen_Help_List();
                        }
                    }
                }
            }
        }


        /*
            Right-Click Vault Item
        */
        {
            for(itr_vault_item_slots = 0; itr_vault_item_slots < NUM_VAULT_ITEM_SLOTS; itr_vault_item_slots++)
            {
                if(-(m_item_screen_vault_item_fields[itr_vault_item_slots]) == input_field_idx)
                {
                    if(_players[_human_player_idx].Vault_Items[itr_vault_item_slots] > -1)
                    {
                        // TODO  RP_SND_LeftClickSound2();
                        Deactivate_Help_List();
                        if(m_cursor_item_idx != ST_UNDEFINED)
                        {
                            Set_Mouse_List(1, mouse_list_default);
                        }
                        Sandbox_Paragraph = Allocate_First_Block(_screen_seg, 1);
                        Item_View_Popup(_players[HUMAN_PLAYER_IDX].Vault_Items[itr_vault_item_slots], (_item_window_start_x + 59 + (itr_vault_item_slots * 21)), (_item_window_start_y + 179));
                        if(m_cursor_item_idx != ST_UNDEFINED)
                        {
                            Set_Mouse_List(1, mouse_list_none);
                        }
                        Assign_Auto_Function(Item_Screen_Draw_Do, 1);
                        Item_Screen_Load();
                        Deactivate_Help_List();
                        Set_Items_Screen_Help_List();
                    }
                }
            }
        }

        /*
            Left-Click Hero Item
        */
        {
            for(itr_hero_slots = 0; itr_hero_slots < NUM_HEROES; itr_hero_slots++)
            {
                for(itr_item_slots = 0; itr_item_slots < NUM_HERO_ITEMS; itr_item_slots++)
                {
                    if(m_item_screen_hero_item_fields[itr_hero_slots][itr_item_slots] == input_field_idx)
                    {
                        // TODO  SND_LeftClickSound();
                        Deactivate_Help_List();
                        Move_Item(itr_hero_slots, itr_item_slots);
                        Assign_Auto_Function(Item_Screen_Draw_Do, 1);
                        Deactivate_Help_List();
                        Set_Items_Screen_Help_List();
                    }
                }
            }
        }


        /*
            Left-Click Vault Item
        */
        {
            for(itr_vault_item_slots = 0; itr_vault_item_slots < NUM_VAULT_ITEM_SLOTS; itr_vault_item_slots++)
            {
                if(m_item_screen_vault_item_fields[itr_vault_item_slots] == input_field_idx)
                {
                    // TODO  SND_LeftClickSound();
                    Deactivate_Help_List();
                    Move_Item(VAULT_HERO_SLOT_IDX, itr_vault_item_slots);
                    Assign_Auto_Function(Item_Screen_Draw_Do, 1);
                    Deactivate_Help_List();
                    Set_Items_Screen_Help_List();
                }
            }
        }


        /*
            Left-Click Anvil - Destroy Item
        */
        {
            if(input_field_idx == m_item_screen_anvil_field)
            {
                if(m_cursor_item_idx > -1)
                {
                    Deactivate_Auto_Function();
                    // TODO  SND_LeftClickSound();
                    Deactivate_Help_List();
                    Destroy_Item();
                    Deactivate_Help_List();
                    Set_Items_Screen_Help_List();
                    Assign_Auto_Function(Item_Screen_Draw_Do, 1);
                }
            }
        }

        if(leave_screen == ST_FALSE)
        {
            Item_Screen_Draw_Do();
            PageFlip_FX();
            if(m_cursor_item_idx == ST_UNDEFINED)
            {
                Release_Time(1);
            }
        }

    }  /* while(leave_screen == ST_FALSE) */

    Clear_Fields();
    Deactivate_Help_List();
    Deactivate_Auto_Function();

    current_screen = scr_Armies_Screen;

    PageFlipEffect = 3;

}


// WZD o75p02
/*
; a wrapper for ITEM_ScreenRedraw that adds no functionality
; (would be a nosub without the call)
*/
void Item_Screen_Draw_Do(void)
{
    Item_Screen_Draw();
}

// WZD o75p03
/*
; draws the items screen into the current draw segment
; on a background loaded from the third VGA frame
*/
void Item_Screen_Draw(void)
{
    int16_t hero_unit_type;
    uint8_t colors[6];
    struct s_OWNED_HERO * hero_ptr;
    int16_t portrait_y2;
    int16_t portrait_x2;
    int16_t portrait_y1;
    int16_t portrait_x1;
    int16_t item_icon_y;
    int16_t item_icon_x;
    int16_t itr_heroes;  // _SI_
    int16_t itr_vault_items;  // _SI_
    int16_t itr_hero_items;  // _DI_

    if(m_cursor_item_idx > -1)
    {
        Set_Mouse_List(1, mouse_list_none);
    }
    else
    {
        Set_Mouse_List(1, mouse_list_default);
    }

    Set_Page_Off();

    Copy_Back_To_Off();

    FLIC_Draw(_item_window_start_x, _item_window_start_y, hero_background_seg);

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        hero_ptr = &_players[_human_player_idx].Heroes[itr_heroes];

        if(hero_ptr->unit_idx > -1)
        {
            hero_unit_type = _UNITS[hero_ptr->unit_idx].type;

            Item_Window_Picture_Coords(itr_heroes, &portrait_x1, &portrait_y1, &portrait_x2, &portrait_y2);

            FLIC_Draw(portrait_x1, portrait_y1, hero_outline_seg);

            FLIC_Draw((portrait_x1 + 1), (portrait_y1 + 1), hero_portraits_seg[itr_heroes]);

            colors[0] = 225;
            colors[1] = 179;

            Set_Font_Colors_15(1, &colors[0]);

            Set_Font_Style_Shadow_Down(1, 15, 0, 0);

            Set_Outline_Color(0);

            Print((portrait_x1 + 37), portrait_y1, hero_ptr->name);

            for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
            {
                if(hero_ptr->Items[itr_hero_items] > -1)
                {
                    Draw_Item_Icon_With_Enchantment_Outline(hero_ptr->Items[itr_hero_items], m_item_icon_workarea);

                    Draw_Picture((portrait_x1 + 40 + (itr_hero_items * 29)), (portrait_y1 + 15), m_item_icon_workarea);
                }
                else
                {
                    // DEDU  Figure out what the [bx-2] means here  `push    item_slot_icons_seg@[bx-2]       ; picture`
                    FLIC_Draw(
                        (portrait_x1 + 41 + (itr_hero_items * 29)), 
                        (portrait_y1 + 17), 
                        item_slot_icons_seg[(hero_ptr->Item_Slots[itr_hero_items] - 1)]
                    );

                    // TODO  mov     ax, _DI_itr_hero_items
                    // TODO  shl     ax, 1
                    // TODO  mov     bx, [bp+hero_ptr]
                    // TODO  add     bx, ax
                    // TODO  mov     ax, [bx+s_OWNED_HERO.Item_Slots]
                    // TODO  add     ax, HLP_SWORD_SLOT_2-1  ; 582
                    // TODO  push    ax
                    // TODO  mov     ax, _SI_itr_heroes
                    // TODO  mov     dx, 3
                    // TODO  imul    dx
                    // TODO  add     ax, _DI_itr_hero_items
                    // TODO  mov     dx, 10
                    // TODO  imul    dx
                    // TODO  mov     bx, ax
                    // TODO  pop     ax
                    // TODO  mov     [(_help_entries.help_idx+32h)+bx], ax
                }

                if(m_item_slot_idx > -1)
                {
                    if(Check_Same_Location(m_item_slot_idx, itr_heroes) == ST_TRUE)
                    {
                        colors[0] = 225;
                        colors[1] = 157;
                        Set_Font_Colors_15(1, colors);
                        Set_Font_Style_Shadow_Down(1, 15, 0, 0);
                        Set_Outline_Color(0);
                        Print((portrait_x1 + 37), (portrait_y1 + 7), cnst_ItemLoc_Same);  /* "Same Location" */
                    }
                    else
                    {
                        Set_Font_Style_Shadow_Down(1, 2, 0, 0);
                        Set_Alias_Color(225);
                        Print((portrait_x1 + 37), (portrait_y1 + 7), cnst_ItemLoc_Diff);  /* "Item Teleport" */
                    }
                }

            }

        }

    }

    for(itr_vault_items = 0; itr_vault_items < NUM_VAULT_ITEMS; itr_vault_items++)
    {
        if(_players[_human_player_idx].Vault_Items[itr_vault_items] > -1)
        {
            item_icon_x = (_item_window_start_x + 52 + (itr_vault_items * 21));
            item_icon_y = (_item_window_start_y + 170);
            Draw_Item_Icon_With_Enchantment_Outline(_players[_human_player_idx].Vault_Items[itr_vault_items], m_item_icon_workarea);
            Draw_Picture(item_icon_x, item_icon_y, m_item_icon_workarea);
        }
    }


    if(
        (_players[_human_player_idx].gold_reserve > 19999)
        ||
        (_players[_human_player_idx].mana_reserve > 19999)
    )
    {
        Set_Font_Style_Shadow_Down(0, 0, 0, 0);  // narrow
    }
    else
    {
        Set_Font_Style_Shadow_Down(1, 0, 0, 0);  // regular
    }
    Set_Outline_Color(0);
    Set_Alias_Color(225);
    Print_Integer_Right((_item_window_start_x + 166), (_item_window_start_y + 164), _players[HUMAN_PLAYER_IDX].gold_reserve);
    Print_Integer_Right((_item_window_start_x + 200), (_item_window_start_y + 164), _players[HUMAN_PLAYER_IDX].mana_reserve);

    Set_Font_Style_Shadow_Down(0, 0, 0, 0);
    Set_Outline_Color(0);
    Print((_item_window_start_x + 169), (_item_window_start_y + 164), cnst_GP_5);
    Print((_item_window_start_x + 203), (_item_window_start_y + 164), cnst_MP_4);

    FLIC_Set_CurrentFrame(hero_alchemy_button_seg, 0);
    FLIC_Draw((_item_window_start_x + 219), (_item_window_start_y + 155), hero_alchemy_button_seg);
    
    FLIC_Set_CurrentFrame(hero_ok_button_seg, 0);
    FLIC_Draw((_item_window_start_x + 219), (_item_window_start_y + 175), hero_ok_button_seg);

    if(m_cursor_item_idx > -1)
    {
        item_icon_x = (Pointer_X() - 5);
        item_icon_y = (Pointer_Y() - 5);
        Draw_Item_With_Name(item_icon_x, item_icon_y, m_cursor_item_idx);
    }

    Cycle_Item_Enchantment_Animation();
}

// WZD o75p04
void Item_Screen_Load(void)
{
    int16_t itr;  // _SI_
    int16_t hero_unit_idx;  // _DI_

    GUI_String_1 = (char *)Near_Allocate_First(100);

    GUI_String_2 = (char *)Near_Allocate_Next(100);

    m_item_screen_vault_item_fields = (int16_t *)Near_Allocate_Next(8);

    for(itr = 0; itr < NUM_HERO_SLOTS; itr++)
    {
        m_item_screen_hero_item_fields[itr] = (int16_t *)Near_Allocate_Next(6);
    }

    m_item_screen_hero_fields = (int16_t *)Near_Allocate_Next(12);

    m_item_icon_workarea = Allocate_First_Block(_screen_seg, 30);

    // ARMYLIST.LBX, 005  HEROBACK   hero background
    hero_background_seg = LBX_Reload_Next(cnst_ARMYLIST_File3, 5, _screen_seg);

    // ARMYLIST.LBX, 006  HEROBAC2   hero outline
    hero_outline_seg = LBX_Reload_Next(cnst_ARMYLIST_File3, 6, _screen_seg);

    // ARMYLIST.LBX, 007  HEROBUT4   hero alchemy butt
    hero_alchemy_button_seg = LBX_Reload_Next(cnst_ARMYLIST_File3, 7, _screen_seg);

    // ARMYLIST.LBX, 008  HEROBUT4   hero ok butt
    hero_ok_button_seg = LBX_Reload_Next(cnst_ARMYLIST_File3, 8, _screen_seg);

    for(itr = 0; itr < 6; itr++)
    {

        hero_unit_idx = _players[_human_player_idx].Heroes[itr].unit_idx;

        if(
            (hero_unit_idx > -1)
            &&
            (_UNITS[hero_unit_idx].owner_idx != ST_UNDEFINED)
        )
        {
            hero_portraits_seg[itr] = LBX_Reload_Next(cnst_PORTRAIT_File3, _unit_type_table[_UNITS[hero_unit_idx].type].hero_portrait, _screen_seg);
        }
    }

}

// WZD o75p05
/*
    called from Item_Screen()
        Check_Same_Location(m_item_slot_idx, itr_heroes)

6 for the fortress vault and 7 for overland battle spoils
*/
int16_t Check_Same_Location(int16_t src_hero_slot_idx, int16_t dst_hero_slot_idx)
{
    int16_t same_location;

    if(dst_hero_slot_idx == VAULT_HERO_SLOT_IDX)
    {
        return ST_TRUE;
    }

    if(
        (src_hero_slot_idx == VAULT_HERO_SLOT_IDX)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wx == _FORTRESSES[HUMAN_PLAYER_IDX].wx)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wx == _FORTRESSES[HUMAN_PLAYER_IDX].wx)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wx == _FORTRESSES[HUMAN_PLAYER_IDX].wx)
    )
    {
        return ST_TRUE;
    }

    if(
        (src_hero_slot_idx == AWARD_HERO_SLOT_IDX)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wx == m_item_wx)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wy == m_item_wy)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wp == m_item_wp)
    )
    {
        return ST_TRUE;
    }

    if
    (
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[src_hero_slot_idx].unit_idx].wx == _UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wx)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[src_hero_slot_idx].unit_idx].wy == _UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wy)
        &&
        (_UNITS[_players[HUMAN_PLAYER_IDX].Heroes[src_hero_slot_idx].unit_idx].wp == _UNITS[_players[HUMAN_PLAYER_IDX].Heroes[dst_hero_slot_idx].unit_idx].wp)
    )
    {
        same_location = ST_TRUE;
    }
    else
    {
        same_location = ST_FALSE;
    }

    return same_location;
}

// WZD o75p06
void Draw_Item_With_Name(int16_t x, int16_t y, int16_t item_idx)
{
    Draw_Item_Icon_With_Enchantment_Outline(item_idx, m_item_icon_workarea);
    Draw_Picture_Windowed(x, y, m_item_icon_workarea);
    // TODO  String_Copy_Far(GUI_String_1, 0, _ITEMS[item_idx].name);
    strcpy(GUI_String_1, _ITEMS[item_idx].name);
    // Clipped_Print_Centered((x + 12), (y + 19), GUI_String_1);
    Print_Centered((x + 12), (y + 19), GUI_String_1);
}


// WZD o75p07
/*
...break artifact...destroy item...

; shows the confirmation dialog for destroying the item
; on the mouse cursor for mana, and does so if chosen
; INCONSISTENT: unlike Delete_Item, this sets the value to -1 rather than 0
*/
void Destroy_Item(void)
{
    int16_t start_y;
    int16_t start_x;
    int16_t Item_Index;
    int16_t item_cost_mana;  // _SI_

    Clear_Fields();
    Set_Mouse_List(1, mouse_list_default);
    Item_Index = m_cursor_item_idx;
    m_cursor_item_idx = -1;
    Item_Screen_Draw_Do();
    m_cursor_item_idx = Item_Index;
    start_x = (_item_window_start_x + 18);
    start_y = (_item_window_start_y + 160);
    Draw_Item_With_Name(start_x, start_y, m_cursor_item_idx);
    PageFlip_FX();
    Copy_On_To_Off_Page();
    item_cost_mana = (_ITEMS[m_cursor_item_idx].cost / 2);
    strcpy(GUI_String_1, cnst_ItemDestroyMsg1);  // "Do you want to destroy your \x02"
    // TODO  String_Copy_Far(GUI_String_2, _ITEMS[m_cursor_item_idx].name);
    strcpy(GUI_String_2, _ITEMS[m_cursor_item_idx].name);
    strcat(GUI_String_1, GUI_String_2);
    strcat(GUI_String_1, cnst_ItemDestroyMsg2);  // "\x01 and gain "
    itoa(item_cost_mana, GUI_String_2, 10);
    strcat(GUI_String_1, GUI_String_2);
    strcat(GUI_String_1, cnst_ItemDestroyMsg3);  // " mana crystals?"
    Set_Mouse_List(1, mouse_list_default);
    if(Confirmation_Box(GUI_String_1) == ST_TRUE)
    {
        Player_Add_Mana(_human_player_idx, item_cost_mana);
        _ITEMS[m_cursor_item_idx].cost = -1;  // NOTE:  Remove_Item() sets cost to 0
        m_item_slot_idx = -1;
        m_cursor_item_idx = -1;
        Set_Mouse_List(1, mouse_list_default);
    }
    else
    {
        Set_Mouse_List(1, mouse_list_none);
    }

}


// WZD o75p08
/*
    only called from Item_Screen()
        on left-click of hero item or vault item
    maybe item on cursor
        maybe item on cursor is from treasure or dead hero

; picks up, puts down, or swaps an item with the one on the mouse cursor, if possible
; BUG: reads out of bounds memory every time the vault is accessed (luckily no writes or gameplay effect)
; BUGs inherited from the function checking whether an item can be removed from a hero without drowning

Player Hero Slot Index
Hero Item Slot Index

hero_slot_idx: {0, ..., 7}; Hero: {0, ..., 5}, Vault: 6, Temp/Cursor: 7
item_slot_idx: {0,1,2} or {0,1,2,3}  Hero Item Slot Index or Vault Item Slot Index

*/
void Move_Item(int16_t hero_slot_idx, int16_t item_slot_idx)
{
    int16_t Item_Pickup;
    int16_t * item_slots_ptr;  // Item Slots
    int16_t Can_Equip;

    if(hero_slot_idx == VAULT_HERO_SLOT_IDX)
    {
        item_slots_ptr = &_players[_human_player_idx].Vault_Items[0];
    }
    else
    {
        item_slots_ptr = &_players[_human_player_idx].Heroes[hero_slot_idx].Items[0];
    }

    /*
        Is there an item on the mouse cursor?  (item_idx for _ITEMS[])
        YAY:  
        NAY:  
    */
    if(m_cursor_item_idx != ST_UNDEFINED)
    {
        /*
            BEGIN:  Can_Equip?
        */
        Can_Equip = ST_FALSE;

        // is item_idx, right? so, why 0 / why not -1?  ~== `== -1`?
        // DEDU  are these two code paths effectively, exactly the same?
        // mayhap, they were doing that thing they do where a positive or negative value is meaninful - like a 'type'?

        if(item_slots_ptr[item_slot_idx] < 0)
        {
            // ; BUG: [hero_slot_idx] can be 6...

            // Item is "Weapon"

            if(
                (_players[_human_player_idx].Heroes[hero_slot_idx].Item_Slots[item_slot_idx] == 3) /* SwordStaff_Slot */
                &&
                (
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 1) /* Sword_Slot */
                    ||
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 4) /* Staff_Slot */
                )
            )
            {
                Can_Equip = ST_TRUE;
            }

            if(
                (_players[_human_player_idx].Heroes[hero_slot_idx].Item_Slots[item_slot_idx] == 2) /* Bow_Slot */
                &&
                (
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 1) /* Sword_Slot */
                    ||
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 2) /* Bow_Slot */
                )
            )
            {
                Can_Equip = ST_TRUE;
            }

            if(_ITEMS[m_cursor_item_idx].Slot_Type == _players[_human_player_idx].Heroes[hero_slot_idx].Item_Slots[item_slot_idx])
            {
                Can_Equip = ST_TRUE;
            }

            if(hero_slot_idx == VAULT_HERO_SLOT_IDX)
            {
                Can_Equip = ST_TRUE;
            }

        }
        else
        {

            if(hero_slot_idx == VAULT_HERO_SLOT_IDX)
            {
                Can_Equip = ST_TRUE;
            }

            if(_ITEMS[m_cursor_item_idx].Slot_Type == _players[_human_player_idx].Heroes[hero_slot_idx].Item_Slots[item_slot_idx])
            {
                Can_Equip = ST_TRUE;
            }

            if(
                (_players[_human_player_idx].Heroes[hero_slot_idx].Item_Slots[item_slot_idx] == 3) /* SwordStaff_Slot */
                &&
                (
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 1) /* Sword_Slot */
                    ||
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 4) /* Staff_Slot */
                )
            )
            {
                Can_Equip = ST_TRUE;
            }

            if(
                (_players[_human_player_idx].Heroes[hero_slot_idx].Item_Slots[item_slot_idx] == 2) /* Bow_Slot */
                &&
                (
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 1) /* Sword_Slot */
                    ||
                    (_ITEMS[m_cursor_item_idx].Slot_Type == 2) /* Bow_Slot */
                )
            )
            {
                Can_Equip = ST_TRUE;
            }

        }
        /*
            END:  Can_Equip?
        */

        // item type fits slot type
        // HERE: left-clicked, item on cursor, acceptible slotting
        // ¿ NEXT: is there also an item already in that slot that we will need to pick up (and exchange with the cursor) ?
        if(Can_Equip == ST_TRUE)
        {
            Item_Pickup = ST_TRUE;

            if(
                (hero_slot_idx != VAULT_HERO_SLOT_IDX)
                &&
                (item_slots_ptr[item_slot_idx] > ST_UNDEFINED)  /* ¿ BUG: couldn't/shouldn't be here if there is no item in the item slot ? */
            )
            {
                if(Check_Wont_Drown(hero_slot_idx, item_slots_ptr[item_slot_idx]) == ST_FALSE)
                {
                    Item_Pickup = ST_FALSE;
                    Can_Equip = ST_FALSE;
                }
            }

            if(Item_Pickup == ST_TRUE)
            {

                // HERE:  left-clicked, item on cursor, acceptible slotting, wont drown

                // src_hero_slot_idx, dst_hero_slot_idx
                if(Check_Same_Location(m_item_slot_idx, hero_slot_idx) == ST_TRUE)
                {
                    Can_Equip = ST_TRUE;
                }
                else
                {
                    if(_players[_human_player_idx].mana_reserve < 20)
                    {
                        Set_Mouse_List(1, mouse_list_default);
                        Copy_On_To_Off_Page();
                        PageFlip_FX();
                        Warn1(cnst_ItemPort_Msg_3);  /* "You do not have enough mana crystals to transfer this item." */
                        Set_Mouse_List(1, mouse_list_none);
                        Can_Equip = ST_FALSE;
                    }
                    else
                    {
                        Set_Mouse_List(1, mouse_list_default);
                        PageFlip_FX();
                        strcpy(GUI_String_1, cnst_ItemPort_Msg_1);  /* "Do you wish to make this transfer at a cost of " */
                        itoa(20, GUI_String_2, 10);
                        strcat(GUI_String_1, GUI_String_2);
                        strcat(GUI_String_1, cnst_ItemPort_Msg_2);  /* " mana crystals?" */

                        Can_Equip = Confirmation_Box(GUI_String_1);

                        Set_Mouse_List(1, mouse_list_none);

                        if(Can_Equip == ST_TRUE)
                        {
                            _players[_human_player_idx].mana_reserve -= 20;
                        }
                    }
                }
            }  /* if(Item_Pickup == ST_TRUE) */


            /*
                move the item from the cursor to the hero or vault item slot
                OR
                exchange the item in the cursor item slot with the item in the hero or vault item slot
            */
            if(item_slots_ptr[item_slot_idx] < 0)
            {
                if(Can_Equip == ST_TRUE)
                {
                    m_item_slot_idx = ST_UNDEFINED;

                    item_slots_ptr[item_slot_idx] = m_cursor_item_idx;
                    
                    m_cursor_item_idx = ST_UNDEFINED;
                    
                    Set_Mouse_List(1, mouse_list_default);
                }
            }
            else
            {
                if(Can_Equip == ST_TRUE)
                {
                    Set_Mouse_List(1, mouse_list_none);

                    Swap_Short(&item_slots_ptr[item_slot_idx], &m_cursor_item_idx);

                    m_item_slot_idx = hero_slot_idx;
                }
            }

        }

    }
    else  /* if(m_cursor_item_idx != ST_UNDEFINED) */
    {

        if(item_slots_ptr[item_slot_idx] > ST_UNDEFINED)
        {
            Item_Pickup = ST_TRUE;
            
            if (hero_slot_idx != VAULT_HERO_SLOT_IDX)
            {
                if (Check_Wont_Drown(hero_slot_idx, item_slots_ptr[item_slot_idx]) == ST_FALSE)
                {
                    Item_Pickup = ST_FALSE;
                }
            }

            if(Item_Pickup == ST_TRUE)
            {
                m_item_slot_idx = hero_slot_idx;
                m_cursor_item_idx = item_slots_ptr[item_slot_idx];
                item_slots_ptr[item_slot_idx] = ST_UNDEFINED;
                Set_Mouse_List(1, mouse_list_none);
            }

        }

    }

}

// WZD o75p09
/*
; creates the items screen's GUI controls for heroes,
; items, the forge, the vault, and the alchemy and OK
; buttons - storing -1000 instead for inactive ones
*/
void Item_Screen_Add_Fields(void)
{
    int16_t portrait_y2;
    int16_t portrait_x2;
    int16_t portrait_y1;
    int16_t portrait_x1;
    int16_t vault_slots_start_y;
    int16_t vault_slots_start_x;
    int16_t itr_hero_slots;  // _SI_
    int16_t itr_vault_items;  // _SI_
    int16_t itr_hero_items;  // _DI_

    for(itr_hero_slots = 0; itr_hero_slots < NUM_HERO_SLOTS; itr_hero_slots++)
    {

        // TODO  make these not be hard-coded
        m_item_screen_hero_item_fields[itr_hero_slots][0] = INVALID_FIELD;
        m_item_screen_hero_item_fields[itr_hero_slots][1] = INVALID_FIELD;
        m_item_screen_hero_item_fields[itr_hero_slots][2] = INVALID_FIELD;

        m_item_screen_hero_fields[itr_hero_slots] = INVALID_FIELD;

        if(_players[HUMAN_PLAYER_IDX].Heroes[itr_hero_slots].unit_idx > -1)
        {
            Item_Window_Picture_Coords(itr_hero_slots, &portrait_x1, &portrait_y1, &portrait_x2, &portrait_y2);
            m_item_screen_hero_fields[itr_hero_slots] = Add_Hidden_Field(portrait_x1, portrait_y1, portrait_x2, portrait_y2, cnst_ZeroString_27, ST_UNDEFINED);

            for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
            {
                m_item_screen_hero_item_fields[itr_hero_slots][itr_hero_items] = Add_Hidden_Field((portrait_x1 + 40 + (itr_hero_items * 29)), (portrait_y1 + 16), (portrait_x1 + 40 + 21 + (itr_hero_items * 29)), (portrait_y1 + 16 + 17), cnst_ZeroString_27, ST_UNDEFINED);
            }

        }

    }

    for(itr_vault_items = 0; itr_vault_items < NUM_VAULT_ITEM_SLOTS; itr_vault_items++)
    {
        vault_slots_start_x = (_item_window_start_x + 52 + (itr_vault_items * 21));
        vault_slots_start_y = (_item_window_start_y + 172);
        m_item_screen_vault_item_fields[itr_vault_items] = Add_Hidden_Field(vault_slots_start_x, vault_slots_start_y, (vault_slots_start_x + 19), (vault_slots_start_y + 15), cnst_ZeroString_27, ST_UNDEFINED);
    }

    m_item_screen_alchemy_button_field = INVALID_FIELD;
    m_item_screen_ok_button_field = INVALID_FIELD;

    if(m_cursor_item_idx == ST_UNDEFINED)
    {
        m_item_screen_alchemy_button_field = Add_Button_Field((_item_window_start_x + ITEM_SCREEN_ALCHEMY_BUTTON_X), (_item_window_start_y + ITEM_SCREEN_ALCHEMY_BUTTON_Y), "", hero_alchemy_button_seg, cnst_HOTKEY_A_3, ST_UNDEFINED);
        m_item_screen_ok_button_field = Add_Button_Field((_item_window_start_x + ITEM_SCREEN_OK_BUTTON_X), (_item_window_start_y + ITEM_SCREEN_OK_BUTTON_Y), "", hero_ok_button_seg, cnst_HOTKEY_O_8, ST_UNDEFINED);
    }

    m_item_screen_anvil_field = Add_Hidden_Field((_item_window_start_x + ITEM_SCREEN_ANVIL_X), (_item_window_start_y + ITEM_SCREEN_ANVIL_Y), (_item_window_start_x + ITEM_SCREEN_ANVIL_X + ITEM_SCREEN_ANVIL_W), (_item_window_start_y + ITEM_SCREEN_ANVIL_Y + ITEM_SCREEN_ANVIL_H), cnst_ZeroString_27, ST_UNDEFINED);

    // ¿ BUG: never checked for input match ?
    // or, this overlays the fullscreen exit field, so that only clicking outside the window area exits?
    m_item_screen_window_field = Add_Hidden_Field(_item_window_start_x, _item_window_start_y, (_item_window_start_x + ITEM_SCREEN_WIDTH), (_item_window_start_y + ITEM_SCREEN_HEIGHT), cnst_ZeroString_27, ST_UNDEFINED);

    // ¿ ~== _global_exit_field ?
    m_item_screen_exit_field = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_ZeroString_27, ST_UNDEFINED);

}


// WZD o75p10
/*
    alt. path to 'Item View'; no item slot coordinates to grow out from;

e.g.,
    called from G_OVL_Cast() for create item/artifact

*/
void Process_Item_Pool(int16_t item_count, int16_t item_list[])
{
    int16_t itr_item_count;  // _SI_

    SETMAX(item_count, 6);


    item_pool_active = ST_TRUE;

    item_pool_list_count = item_count;

    item_pool_list_index = 0;


    for(itr_item_count = 0; itr_item_count < 6; itr_item_count++)
    {
        item_pool_list_array[itr_item_count] = item_list[itr_item_count];
    }


    Item_Screen();


    PageFlipEffect = 0;

    item_pool_item_idx = ST_UNDEFINED;

    Set_Mouse_List(1, mouse_list_default);

    item_pool_active = ST_FALSE;

}


// WZD o75p11
/*
~== Unit_Window_Picture_Coords()
~== Outpost_Garrison_Picture_Coords()
*/
void Item_Window_Picture_Coords(int16_t hero_slot_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2)
{
    assert(_item_window_start_x == 17);
    assert(_item_window_start_y ==  2);
    // TODO(JimBalcomb,20240505): add module-scoped (manifest) contstants for the magic-numbers used here-in
    // *x1 = (_item_window_start_x + 14 + ((hero_slot_idx % 2) * 135));
    *x1 = (17 + 14 + ((hero_slot_idx % 2) * 135));
    *y1 = (_item_window_start_y + 12 + ((hero_slot_idx / 2) *  46));
    *x2 = (*x1 + 32);
    *y2 = (*y1 + 32);
}


// WZD o75p12
/*
    returns {ST_FALSE,ST_TRUE}
    if this item is unequipped from this hero unit
    ...
    Warn1(cnst_DrownWarn_Msg);  // "This unit will drown if the item is removed."

    OON XREF:  Move_Item()

    sets Item_Pickup flag to ST_FALSE
    used to branch to pick-up/exchange cursor item

So, ...
    specific to drowning
    true means can/do pickup/exchange
    something like "is safe to unequip item"
    Check_Wont_Drown()
    Item_...Hero_... ...if unequipped

; checks whether the specified item can be removed
; from the selected hero without causeing them to drown
; returns 1 if the item can be removed, or returns 0
; after displaying a warning message if not
; contains multiple BUGs in the form of bogus checks
; and ignoring the Non-Corporeal trait in any form

¿ Why doesn't this short-ciruit on Terrain_Is_Sailable() == ST_FALSE ?
...or if !On_Water
Why have On_Water and Cant_Remove?
    if On_Water is false, then Cant_Remove MUST BE false


ITEM_CanRemove(hero_slot_idx, item_slots_ptr[item_slot_idx])

¿ flag for whether drowning is a concern ?
    stack has boat or wind-walker
oddly does not start from the question of whether the item has a power that would preclude this concern?


*/
int16_t Check_Wont_Drown(int16_t hero_slot_idx, int16_t item_idx)
{
    int16_t troop_list[9];
    int16_t Equipped_Item_Index;
    uint32_t item_powers;
    int16_t Local_0_2;
    int16_t Wind_Walker;
    int16_t Wind_Walker_In_Stack;
    int16_t On_Water;
    int16_t troop_count;
    int16_t Cant_Remove;
    int16_t hero_unit_idx;  // _SI_
    int16_t itr_troops;  // _DI_
    int16_t itr_items;  // _DI_

    hero_unit_idx = _players[_human_player_idx].Heroes[hero_slot_idx].unit_idx;

    if(hero_unit_idx == ST_UNDEFINED)
    {
        return ST_TRUE;
    }

    Cant_Remove = ST_FALSE;

    Local_0_2 = 0;

    On_Water = Terrain_Is_Sailable(_UNITS[hero_unit_idx].wx, _UNITS[hero_unit_idx].wy, _UNITS[hero_unit_idx].wp);


    /*
        BEGIN:  clear drown flag if stack has boat
    */
    if(On_Water == ST_TRUE)
    {
        Player_Army_At_Square(_UNITS[hero_unit_idx].wx, _UNITS[hero_unit_idx].wy, _UNITS[hero_unit_idx].wp, _UNITS[hero_unit_idx].owner_idx, &troop_count, &troop_list[0]);
        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            if(troop_list[itr_troops] != hero_unit_idx)
            {
                if(_unit_type_table[_UNITS[itr_troops].type].Transport > 0)
                {
                    On_Water = ST_FALSE;
                }
            }
        }
    }
    /*
        END:  clear drown flag if stack has boat
    */


    /*
        BEGIN:  clear drown flag if equipped item has water walking or flight
    */
    // BUG: ignores Wraith Form
    if(On_Water == ST_TRUE)
    {
        for(itr_items = 0; itr_items < NUM_HERO_ITEMS; itr_items++)
        {
            Equipped_Item_Index = _players[_human_player_idx].Heroes[hero_slot_idx].Items[itr_items];
            if(
                (Equipped_Item_Index > ST_UNDEFINED)
                &&
                (Equipped_Item_Index != item_idx)
            )
            {
                // TODO  item_powers = _ITEMS[Equipped_Item_Index].Powers;
                // item_powers = GET_4B_OFS((uint8_t*)&_ITEMS[Equipped_Item_Index], 0x2E);
                item_powers = GET_ITEM_POWERS(Equipped_Item_Index);
                if(
                    ((item_powers & IP_WATER_WALKING) != 0)
                    ||
                    ((item_powers & IP_FLIGHT) != 0)
                )
                {
                    On_Water = ST_FALSE;
                }
            }
        }
    }
    /*
        END:  clear drown flag if equipped item has water walking or flight
    */


    /*
        BEGIN:  clear drown flag if cursor item has water walking or flight
    */
    if(On_Water == ST_TRUE)
    {
        if(m_cursor_item_idx > ST_UNDEFINED)
        {
            // item_powers = _ITEMS[m_cursor_item_idx].Powers;
            // item_powers = GET_4B_OFS((uint8_t*)&_ITEMS[m_cursor_item_idx], 0x2E);
            item_powers = GET_ITEM_POWERS(m_cursor_item_idx);
            if(
                ((item_powers & IP_WATER_WALKING) != 0)
                ||
                ((item_powers & IP_FLIGHT) != 0)
            )
            {
                On_Water = ST_FALSE;
            }
        }
    }
    /*
        END:  clear drown flag if cursor item has water walking or flight
    */


    if(On_Water == ST_TRUE)
    {
        Player_Army_At_Square(_UNITS[hero_unit_idx].wx, _UNITS[hero_unit_idx].wy, _UNITS[hero_unit_idx].wp, _UNITS[hero_unit_idx].owner_idx, &troop_count, &troop_list[0]);

        Wind_Walker = ST_FALSE;

        if(
            (Unit_Has_WindWalking(hero_unit_idx) == ST_TRUE)
            /* || unit has wind walking item (o71p08_Empty_pFxn(hero_unit_idx)) */
        )
        {
            Wind_Walker = ST_TRUE;
        }

        Wind_Walker_In_Stack = ST_FALSE;

        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            if(troop_list[itr_troops] != hero_unit_idx)
            {
                if(
                    (Unit_Has_WindWalking(itr_troops) == ST_TRUE)
                    /* || unit has wind walking item (o71p08_Empty_pFxn(hero_unit_idx)) */
                )
                {
                    Wind_Walker_In_Stack = ST_TRUE;
                }

            }
        }

        // ¿ BUG:  doesn't set Cant_Remove based on Wind_Walker_In_Stack ?
        if(
            (Wind_Walker_In_Stack == ST_FALSE)
            &&
            (Wind_Walker == ST_FALSE)
        )
        {
            // item_powers = _ITEMS[item_idx].Powers;
            // item_powers = GET_4B_OFS((uint8_t*)&_ITEMS[item_idx], 0x2E);
            item_powers = GET_ITEM_POWERS(item_idx);

            Cant_Remove = ST_TRUE;

            // ¿ BUG:  these should be grouped as water powers and air powers ?
            if(
                ((item_powers & IP_FLIGHT) != 0)
                ||
                (Unit_Has_WaterTravel(hero_unit_idx) == ST_TRUE)
                ||
                (Unit_Has_WaterTravel_Item(hero_unit_idx) == ST_TRUE)
                ||
                (Wind_Walker == ST_TRUE)
            )
            {
                Cant_Remove = ST_FALSE;
            }

            // drake178: ; BUG: does not exclude the item being checked
            if(
                ((item_powers & IP_WATER_WALKING) != 0)
                ||
                (Unit_Has_AirTravel(hero_unit_idx) == ST_TRUE)
                ||
                (Unit_Has_AirTravel_Item(hero_unit_idx) == ST_TRUE)
                ||
                (Wind_Walker == ST_TRUE)
            )
            {
                Cant_Remove = ST_FALSE;
            }

            // BUG:  already checked ¿ overlapping macros ?
            if(
                (Unit_Has_AirTravel(hero_unit_idx) == ST_TRUE)
                ||
                (Unit_Has_WaterTravel(hero_unit_idx) == ST_TRUE)
            )
            {
                Cant_Remove = ST_FALSE;
            }

        }

    }  /* if(On_Water == ST_TRUE) */


    if(Cant_Remove == ST_TRUE)
    {
        Warn1(cnst_DrownWarn_Msg);  // "This unit will drown if the item is removed."
        return ST_FALSE;
    }
    else
    {
        return ST_TRUE;
    }

}
