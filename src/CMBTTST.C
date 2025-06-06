
#include "CMBTTST.H"

#include "CMBTTSTU.H"

#include "CMBTDEF.H"

#include "MOX/MOX_DEF.H"
#include "MOX/MOX_T4.H"  /* PageFlipEffect */
#include "MOX/MOX_TYPE.H"



// MOM_Data.C
// dseg:973C
SAMB_ptr combat_background_bottom;
// WZD dseg:C550
SAMB_ptr battle_unit_picts_seg[(2 * MAX_STACK)];



void Combat_Screen_TST_001_Draw(void);
void Combat_Screen_TST_002_Draw(void);
void Combat_Screen_TST_003_Draw(void);
void Combat_Screen_TST_004_Draw(void);



/*
    ¿ No-Op Combat_Screen()  |-> Combat_Screen_Draw() ?
*/
int16_t Combat_Screen_TST_001(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t hotkey_ESC_field = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;

    // Set_Page_Off();
    // Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    // Reset_Window();
    // Clear_Fields();
    // Deactivate_Auto_Function();
    // Deactivate_Help_List();

    /*
        BEGIN: Combat Screen Compose Background
    */
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Reset_Window();
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
    /*
        END: Combat Screen Compose Background
    */

    _page_flip_effect = pfe_NONE;

    Assign_Auto_Function(Combat_Screen_TST_001_Draw, 1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        hotkey_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC_field)
        {
            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Combat_Screen_TST_001_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    // Deactivate_Auto_Function();
    // Deactivate_Help_List();
    // Reset_Window();

    return ST_FALSE;  // attacker did not win

}

/*
    
*/
void Combat_Screen_TST_001_Draw(void)
{
    // Tactical_Combat_Draw()
    Set_Page_Off();

    // CMB_DrawMap__WIP
    Copy_Back_To_Off();
    Set_Page_Off();
    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, 163);
    Reset_Window();

    // Tactical_Combat_Draw()
    Reset_Window();
    
}



/*
    
*/
int16_t Combat_Screen_TST_002(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t hotkey_ESC_field = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;

    // Set_Page_Off();
    // Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    // Reset_Window();
    // Clear_Fields();
    // Deactivate_Auto_Function();
    // Deactivate_Help_List();

    /*
        BEGIN: Combat Screen Compose Background
    */
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Reset_Window();
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
    /*
        END: Combat Screen Compose Background
    */

    _page_flip_effect = pfe_NONE;

    Assign_Auto_Function(Combat_Screen_TST_002_Draw, 1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        hotkey_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC_field)
        {
            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Combat_Screen_TST_002_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    // Deactivate_Auto_Function();
    // Deactivate_Help_List();
    // Reset_Window();

    return ST_FALSE;  // attacker did not win

}

/*
    
*/
void Combat_Screen_TST_002_Draw(void)
{
    // Tactical_Combat_Draw()
    Set_Page_Off();

    // CMB_DrawMap__WIP
    Copy_Back_To_Off();
    Set_Page_Off();
    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, 163);
    Reset_Window();

    // Tactical_Combat_Draw()
    Reset_Window();
    
}



/*
    
*/
int16_t Combat_Screen_TST_003(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t hotkey_ESC_field = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;

    // Set_Page_Off();
    // Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    // Reset_Window();
    // Clear_Fields();
    // Deactivate_Auto_Function();
    // Deactivate_Help_List();

    /*
        BEGIN: Combat Screen Compose Background
    */
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Reset_Window();
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
    /*
        END: Combat Screen Compose Background
    */

    _page_flip_effect = pfe_NONE;

    Assign_Auto_Function(Combat_Screen_TST_003_Draw, 1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        hotkey_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC_field)
        {
            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Combat_Screen_TST_003_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    // Deactivate_Auto_Function();
    // Deactivate_Help_List();
    // Reset_Window();

    return ST_FALSE;  // attacker did not win

}

/*
    
*/
void Combat_Screen_TST_003_Draw(void)
{
    // Tactical_Combat_Draw()
    Set_Page_Off();

    // CMB_DrawMap__WIP
    Copy_Back_To_Off();
    Set_Page_Off();
    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, 163);
    Reset_Window();

    // Tactical_Combat_Draw()
    Reset_Window();
    
}



/*
    ...units...

    
*/
int16_t Combat_Screen_TST_004(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t hotkey_ESC_field = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t defender_unit_count = 0;
    int16_t itr = 0;

    // Set_Page_Off();
    // Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    // Reset_Window();
    // Clear_Fields();
    // Deactivate_Auto_Function();
    // Deactivate_Help_List();

    /*
        BEGIN:  
    */
    Clear_Fields();  // if not here, Toggle_Pages() does Draw_Fields() over the Fill()
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Toggle_Pages();
    _page_flip_effect = pfe_NONE;

    // CMB_BaseAllocs__WIP()
    battlefield = (struct s_BATTLEFIELD *)Allocate_First_Block(_screen_seg, 348);  // 348 PR, 5568 B
    combat_grid_entities = (struct s_COMBAT_ENTITY *)Allocate_Next_Block(_screen_seg, 482);  // 482 PR, 7712 B
    // CMB_LoadResources__WIP()
    battle_units = (struct s_BATTLE_UNIT *)Allocate_Next_Block(_screen_seg, 249);
    Mark_Block(_screen_seg);
    for(itr = 0; itr < MAX_BATTLE_UNIT_COUNT; itr++)
    {
        battle_unit_picts_seg[itr] = Allocate_Next_Block(_screen_seg, 55);  // 55 PR  880 B
    }
    Release_Block(_screen_seg);

    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_attacker_player = combat_attacker_player_idx;
    _combat_defender_player = combat_defender_player_idx;

    // CMB_Terrain_Init__WIP() |-> CMB_ComposeBackgrnd__WIP()
    /*
        BEGIN:  Combat Screen Compose Background
    */
    Set_Page_Off();
    Reset_Window();
    /* ~== draw terrain picts */ Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, COMBAT_MAP_YMAX, ST_GRAY);
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
    /*
        END:  Combat Screen Compose Background
    */
    
    defender_unit_count = Prepare_All_Battle_Units(troop_count, troops);

//     if(_combat_attacker_player == _human_player_idx)
//         Switch_Active_Battle_Unit(0);  /* first attacker battle_unit_idx */
//     else
//         Switch_Active_Battle_Unit((_combat_total_unit_count - defender_unit_count));  /* first defender battle_unit_idx */
    _active_battle_unit = ST_UNDEFINED;

    // Reset_Cycle_Palette_Color();
    // Load_Palette_From_Animation(combat_background_bottom);
    // Apply_Palette();
    // Cycle_Palette_Color__STUB(198, 0, 0, 0, 55, 0, 0, 8);
    // Set_Palette_Changes(0, 243);
    // Calculate_Remap_Colors();

    // Tactical_Combat_Draw();
    Combat_Screen_TST_004_Draw();
    PageFlip_FX();

    Assign_Auto_Function(Combat_Screen_TST_004_Draw, 1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        hotkey_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC_field)
        {
            leave_screen = ST_TRUE;
        }

        if(leave_screen == ST_FALSE)
        {
            Combat_Screen_TST_004_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    // Deactivate_Auto_Function();
    // Deactivate_Help_List();
    // Reset_Window();

    return ST_FALSE;  // attacker did not win

}

/*
    
*/
void Combat_Screen_TST_004_Draw(void)
{
    // Tactical_Combat_Draw()
    Set_Page_Off();

    // CMB_DrawMap__WIP
    Copy_Back_To_Off();
    Set_Page_Off();
    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, 163);
    Reset_Window();

    // BEGIN:  Tactical_Combat_Draw()
    Make_Combat_Grid_Battle_Unit_Entities();
    Draw_Combat_Grid_Battle_Unit_Entities();
    Reset_Window();
    
}
