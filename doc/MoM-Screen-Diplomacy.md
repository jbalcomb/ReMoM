


MOM_SCR.C
// WZD s01p02
void Screen_Control(void)
    case scr_Diplomacy_Screen:
        Diplomacy_Screen();

MagicScr.C
Magic_Screen()
BEGIN:  Left-Click Gem
    for(itr = 0; itr < gem_count; itr++)
        if((magic_gem_fields[itr] == input_field_idx) && (gem_player_nums[itr] > 0))
            if(_players[_human_player_idx].casting_spell_idx == 214)  /* Spell_Of_Return */
            else
                Play_Left_Click();
                m_diplomac_player_idx = gem_player_nums[itr];
                current_screen = scr_Diplomacy_Screen;
                leave_screen_flag = 2;



IDK_Diplo_Scrn()
GUI_CreateTextDialog()




## IDK_Diplo_Scrn()

XREF:
    Diplomacy_Screen__WIP()
    j_IDK_Diplo_Scrn()
        IDK_Dipl_s72690()
            j_IDK_Dipl_s72690()
                IDK_Dipl_s73FBF()




## GUI_CreateTextDialog()
Fields.C
// WZD s36p60
// GUI_CreateTextDialog();
