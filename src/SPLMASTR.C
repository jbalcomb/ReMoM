/*
    WIZARDS.EXE
        ovr138

MoO2
    ¿ DNE // N/A ?

*/

#include "MoM.H"



// WZD dseg:6DCA                                                 BEGIN:  ovr138 - Initialized Data

// WZD dseg:6DCA
char spellose_lbx_file__ovr138[] = "spellose";
// WZD dseg:6DD3
char strHasCastThe[] = " has cast the";
// WZD dseg:6DE1
char strSpellOfMastery[] = "Spell of Mastery";
// WZD dseg:6DF2
char music_lbx_file__ovr138[] = "music";
// WZD dseg:6DF8 4D 41 47 49 43 2E 45 58 45 00                   cnst_MAGIC_EXE_File2 db 'MAGIC.EXE',0   ; DATA XREF: IDK_SomScr_Lose+FDo ...
// WZD dseg:6E02 4A 45 4E 4E 59 00                               cnst_MAGICEXE_arg0_2 db 'JENNY',0       ; DATA XREF: IDK_SomScr_Lose+F9o ...
// WZD dseg:6E08
char strHasStartedCastingThe[] = " has started casting the";
// WZD dseg:6E21
char spellscr_lbx_file__ovr138[] = "spellscr";
// WZD dseg:6E2A
char str_hotkey_ESC__ovr138[] = "\x1B";
// WZD dseg:6E2C
char wizlab_lbx_file__ovr138[] = "wizlab";
// WZD dseg:6E33
char splmastr_lbx_file__ovr138[] = "splmastr";
// WZD dseg:6E3C
char cmbtfx_lbx_file__ovr138[] = "cmbtfx";
// WZD dseg:6E43
char monster_lbx_file__ovr138[] = "Monster";

// WZD dseg:6E4B 00                                              align 2

// WZD dseg:6E4B                                                 END:  ovr138 - Initialized Data



// WZD dseg:CA56                                                 BEGIN: ovr138 - Uninitialized Data

// WZD dseg:CA56
// drake178: GAME_SoM_Cast_By
/*
; HoF will skip turn count points if this is not -1 or
; the index of the human player, and SoM if it is not
; the human player
*/
int16_t GAME_SoM_Cast_By;

// WZD dseg:CA58 00 00                                           word_434F8 dw 0                         ; DATA XREF: IDK_SomScr_Lose_Load+B1w ...
// WZD dseg:CA5A 00 00                                           word_434FA dw 0                         ; DATA XREF: IDK_SomScr_Lose_Load+9Dw ...

// WZD dseg:CA5A                                                 END: ovr138 - Uninitialized Data



/*
    WIZARDS.EXE  ovr138
*/

// WZD o138p01
// IDK_SomScr_Lose_Load()

// WZD o138p02
// IDK_SomScr_Lose_Draw()

// WZD o138p03
// IDK_SomScr_Lose()

// WZD o138p04
void SoM_Started_Draw(void)
{
    int16_t itr_frames = 0;  // _SI_
    int16_t frame_count = 0;  // _DI_

    frame_count = (Get_File_Animation_Frame() + 1);

    Set_File_Animation_Frame(0);

    Set_Page_Off();

    for(itr_frames = 0; itr_frames < frame_count; itr_frames++)
    {

    }

    Set_Outline_Color(0);

    Set_Font_Style_Outline(5, 5, 0, 0);

    // TODO  Print_Centered(160, 160, GUI_NearMsgString, 2);
    Print_Centered(160, 160, GUI_NearMsgString);

    Print_Centered(160, 180, strSpellOfMastery);

    OVL_Action_XPos++;

}

// WZD o138p05
void SoM_Started(int16_t player_idx)
{
    int16_t full_screen_ESC_field = 0;

    Deactivate_Auto_Function();

    Stop_All_Sounds__STUB();

    strcpy(GUI_NearMsgString, _players[player_idx].name);

    strcat(GUI_NearMsgString, strHasStartedCastingThe);

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr138, MUSIC_SoM_Started, SND_Music_Segment);

    // SPELLSCR.LBX, 067  "VORTEX3" ""
    Open_File_Animation__HACK(spellscr_lbx_file__ovr138, 67);

    IDK_DiploScrn_scanned_field = 67;

    Set_Mouse_List(1, mouse_list_none);

    Fade_Out();

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

    Copy_On_To_Off_Page();

    Toggle_Pages();

    // TODO  Load_Palette(4, -1);  // ; TEST - spell mastery palette
    Load_Palette(4, 0, 255);

    Set_Page_Off();

    SoM_Started_Draw();

    Toggle_Pages();

    Fade_In();

    Assign_Auto_Function(SoM_Started_Draw, 2);

    if(magic_set.background_music == ST_TRUE)
    {
        Play_Sound__STUB(SND_Spell_Music);
    }

    Clear_Fields();

    full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr138[0], ST_UNDEFINED);

    
    OVL_Action_XPos = 0;
    /* incremented in SoM_Started_Draw() */
    while((OVL_Action_XPos < 200) && (Get_Input() != full_screen_ESC_field))
    {

    }

    Stop_All_Sounds__STUB();

    Play_Background_Music();

    Fade_Out();

    // TODO  Load_Palette(0, -1);  // ; EMPERATO - main game palette
    Load_Palette(0, 0, 255);

    Allocate_Reduced_Map();

    Assign_Auto_Function(Main_Screen_Draw, 2);

    if(player_idx == HUMAN_PLAYER_IDX)
    {
        Set_Mouse_List(1, mouse_list_default);
    }
    else
    {
        Set_Mouse_List(1, mouse_list_hourglass);
    }

    Set_Page_Off();

    Main_Screen_Draw();

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Fade_In();

}

// WZD o138p06
// IDK_SomScr_Load()

// WZD o138p07
// IDK_SomScr_Draw()

// WZD o138p08
// IDK_SomScr()

// WZD o138p09
// CMB_LoadUndeadAnim()

// WZD o138p10
// CMB_DrawUndeadAnim()

// WZD o138p11
// CMB_CreateUndeadAnim()
