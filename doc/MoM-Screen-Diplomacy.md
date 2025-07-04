

IDK_DIPLOMSG_s732D9()  ==>  Get_Diplomacy_Statement()
IDK_Dipl_s72690()  ==>  Npc_Diplomacy_Screen()
¿ ?  ==>  Diplomacy_Screen_Draw_Portrait_Mouth_Animation()

IMG_MOODWIZPortrait  =>  m_diplomac_mood_portrait_seg
IMG_DIPL_TalkAnim  ==>  m_diplomac_talk_portrait_seg



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


void Diplomacy_Screen_Draw__WIP(void)
    if(m_IDK_diplomatic_flag != 1)  /* refuse audience/negative greeting */
        Diplomacy_Screen_Draw_Portrait_Mouth_Animation();

Diplomacy_Screen_Draw_Portrait_Mouth_Animation()




Diplomacy_Screen__WIP()
    Limit_Temporary_Peace_Modifier();
    m_diplomacy_current_music = ST_UNDEFINED;
    word_42E7E = ST_UNDEFINED;
    m_diplomacy_message_IDK_group = 0;
    m_IDK_diplomatic_flag = 6;
    m_IDK_diplomatic_order = _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[m_diplomac_player_idx];
    Diplomacy_Greeting();
    Diplomacy_Screen_Load__WIP();






IDK_Diplo_Scrn()
Get_List_Field()
    |-> Add_String_List_Field()




DIPL_sub_72DB6_Draw()



_sub_6ED5D_Draw() vs. _sub_6F90E_Draw()
~ same, but _sub_6ED5D_Draw() checks Get_Paragraph_Max_Height() and reduces Set_Font_LF(), from 1 to 0

## _sub_6ED5D_Draw()

XREF:
    j__sub_6ED5D_Draw()


## _sub_6F90E_Draw()






Limit_Temporary_Peace_Modifier()

XREF:
    Diplomacy_Screen__WIP()
    IDK_Dipl_s72690()
    IDK_Dipl_s7373B()





## Diplomacy_Screen_Draw_Gargoyle_Eyes()

XREF:
    Diplomacy_Screen_Draw__WIP()
    sub_6ED5D()
    _sub_6F90E_Draw()
    Diplomacy_Screen_Load__WIP()
    Diplomacy_Screen_Load__WIP()
    j_Diplomacy_Screen_Draw_Gargoyle_Eyes()
        DIPL_sub_72DB6_Draw()




Diplomacy_Screen__WIP()

    m_diplomacy_current_music = ST_UNDEFINED;
    word_42E7E = ST_UNDEFINED;
    m_diplomacy_message_IDK_group = 0;
    m_IDK_diplomatic_flag = 6;
    m_IDK_diplomatic_order = _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[m_diplomac_player_idx];
    if((m_IDK_diplomatic_order == 54) || (m_IDK_diplomatic_order == 1))
        return;
    if(m_IDK_diplomatic_order == 0)
        m_IDK_diplomatic_flag = 0;
        IDK_DiplSts_s70570();
            ...sets m_IDK_diplomatic_flag and m_IDK_diplomatic_order
                m_IDK_diplomatic_flag = 0;  m_IDK_diplomatic_order = {42,43};
                m_IDK_diplomatic_flag = 1;  m_IDK_diplomatic_order = 44;



## IDK_Diplo_Scrn()

XREF:
    Diplomacy_Screen__WIP()
    j_IDK_Diplo_Scrn()
        IDK_Dipl_s72690()
            j_IDK_Dipl_s72690()
                IDK_Dipl_s73FBF()
