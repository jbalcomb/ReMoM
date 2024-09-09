/*
    Spellbook Screen

    WIZARDS.EXE
        ovr134
    
    SPELLSCR.LBX
*/

#include "MoM.H"
#include "SBookScr.H"
#include "Spellbook.H"
#include "MainScr.H"
#include "MainScr_Maps.H"



// WZD dseg:6994                                                 ¿ BEGIN: SPELLSCR || Spellbook Screen - Initialized Data ?
// WZD dseg:6994                                                 BEGIN:  ovr134

// WZD dseg:6994
// REDEF  char hlpentry_lbx_file [] = "hlpentry";

// WZD dseg:699C
// char empty_string__ovr134[] = "";
// WZD dseg:699D 46 00                                           hotkey_F__ovr134 db 'F',0             ; DATA XREF: Spellbook_Screen+205o ...
// WZD dseg:699F 42 00                                           hotkey_B__ovr134 db 'B',0             ; DATA XREF: Spellbook_Screen+232o ...

// WZD dseg:69A1
char _msg_abort_1[] = "Do you wish to abort ";
// WZD dseg:69B6
// cnst_CounterMsg5_2 db 'your ',0         ; DATA XREF: OVL_CounterMessage+10F
char _msg_abort_2[] = "your ";
// WZD dseg:69BC
char _msg_abort_3[] = " spell?";

// WZD dseg:69C4 1B 00                                           hotkey_ESC__ovr134 db 1Bh,0           ; DATA XREF: Spellbook_Screen+75Eo
// WZD dseg:69C6 6D 65 73 73 61 67 65 00                         message_lbx_file__ovr134 db 'message',0
// WZD dseg:69CE 53 70 65 63 66 78 00                            cnst_SPECFX_File7 db 'Specfx',0         ; DATA XREF: OVL_CounterMessage+51o
// WZD dseg:69D5 4D 55 53 49 43 00                               cnst_MUSIC_FileC db 'MUSIC',0           ; DATA XREF: OVL_CounterMessage+68o ...
// WZD dseg:69DB 59 6F 75 72 20 00                               cnst_CounterMsg1_2 db 'Your ',0         ; DATA XREF: OVL_CounterMessage+8Co
// WZD dseg:69E1 27 20 00                                        cnst_Apostrophe_2 db 27h,' ',0          ; DATA XREF: OVL_CounterMessage+DAo ...
// WZD dseg:69E4 27 73 20 00                                     cnst_Possessive_2 db 27h,'s ',0         ; DATA XREF: OVL_CounterMessage:loc_B9BD4o ...
// WZD dseg:69E8 20 73 70 65 6C 6C 20 68 61 73 20 63 61 75 73 65+cnst_CounterMsg2_2 db ' spell has caused ',0
// WZD dseg:69FB 20 73 70 65 6C 6C 20 74 6F 20 66 69 7A 7A 6C 65+cnst_CounterMsg6_2 db ' spell to fizzle.',0
// WZD dseg:6A0D 63 6D 62 74 66 78 00                            cnst_CMBTFX_File4 db 'cmbtfx',0         ; DATA XREF: GAME_LoadSpellAnim+41o ...
// WZD dseg:6A14 73 70 65 63 66 78 00                            cnst_SPECFX_File8 db 'specfx',0         ; DATA XREF: GAME_LoadSpellAnim+1E4o ...
// WZD dseg:6A1B 53 50 45 43 46 58 00                            cnst_SPECFX_File9 db 'SPECFX',0         ; DATA XREF: GAME_LoadSpellAnim+18Bo
// WZD dseg:6A22 43 4D 42 54 46 58 00                            cnst_CMBTFX_File5 db 'CMBTFX',0         ; DATA XREF: GAME_LoadSpellAnim+1F2o
// WZD dseg:6A29 53 4F 55 4E 44 46 58 00                         cnst_SOUNDFX_FileB db 'SOUNDFX',0       ; DATA XREF: SPL_LoadSoundEffect+39o
// WZD dseg:6A31 4E 45 57 53 4F 55 4E 44 00                      cnst_NEWSOUND_File3 db 'NEWSOUND',0     ; DATA XREF: SPL_LoadSoundEffect+5Co
// WZD dseg:6A3A 73 6F 75 6E 64 66 78 00                         cnst_SOUNDFX_FileC db 'soundfx',0       ; DATA XREF: sub_BA1AF+1Co

// WZD dseg:6A3A                                                 END:  ovr134                            ; should use dseg:29fd




/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
void Spell_Screen_Load(void);

// WZD o134p02
// PUBLIC  void Spellbook_Screen_Draw(void);

// WZD o134p03
// PUBLIC  void Spellbook_Screen(void);

// WZD o134p04
// WIZ_SetOverlandSpell()

// WZD o134p05
// sub_B9837()

// WZD o134p06
// Full_Draw_Main_Screen()

// WZD o134p07
// drake178: OVL_MosaicFlip()
void OVL_MosaicFlip__STUB(void);

// WZD o134p08
// OVL_CounterMessage()

// WZD o134p09
// sub_B9D14()

// WZD o134p10
// GAME_LoadSpellAnim()

// WZD o134p11
// SPL_LoadSoundEffect()

// WZD o134p12
// sub_BA1AF()



/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
void Spell_Screen_Load(void)
{

    Allocate_Reduced_Map();
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    Gradient_Fill(0, 0, 319, 199, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ..., remap_block, Slope, Scale, Seed
    FLIC_Draw(16, 10, _spellbook_small_seg);
    Copy_Off_To_Back();
    Set_Page_On();
    LBX_Load_Data_Static(hlpentry_lbx_file, 0, (SAMB_ptr)_help_entries, 0, 15, 10);  // "Spell Book Help"

}


// WZD o134p02
void Spellbook_Screen_Draw(void)
{

    Copy_Back_To_Off();

    OVL_DrawSpellbook(16, 10);

}


// WZD o134p03
void Spellbook_Screen(void)
{
    char temp_string[20];
    int16_t spellbook_page_left[6];
    int16_t spellbook_page_right[6];
    int16_t hotkey_B;
    int16_t hotkey_F;
    int16_t Ctrl_Index;
    int16_t spell_idx;
    int16_t Abort_Spell__YN;
    int16_t hotkey_ESC;
    int16_t y_start;
    int16_t var_A;
    int16_t player_is_casting;
    int16_t itr_page_spell_count;
    int16_t input_field_idx;
    int16_t leave_screen;
    int16_t itr_spellbook_page_count;  // _SI_
    int16_t itr_spellbook_page_fields;  // _SI_
    int16_t x_start;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Spellbook_Screen()\n", __FILE__, __LINE__);
#endif

    var_A = 0;

    OVL_DisableIncmBlink();

    Load_Palette_From_Animation(_spellbook_small_seg);
    Set_Palette_Changes(0, 223);
    Clear_Palette_Changes(224, 255);
    Update_Remap_Gray_Palette();
    Reset_Cycle_Palette_Color();

    Spell_Screen_Load();

    SBK_BuildSpellbook(0, 6);  // Book_Type, Page_Size

    SBK_Dogears = 1;

    player_is_casting = ST_FALSE;

    for(itr_spellbook_page_count = 0; ((itr_spellbook_page_count < SBK_BookPageCount) && (player_is_casting == ST_FALSE)); itr_spellbook_page_count++)
    {
        for(itr_page_spell_count = 0; SBK_BookPages[itr_spellbook_page_count].Spell_Count <= itr_page_spell_count; itr_page_spell_count++)
        {
            if(player_is_casting == ST_FALSE)
            {
                if(SBK_BookPages[itr_spellbook_page_count].Spells[itr_page_spell_count] == _players[HUMAN_PLAYER_IDX].Spell_Cast)
                {
                    if((itr_spellbook_page_count % 2) != 1)
                    {
                        SBK_OpenPage = itr_spellbook_page_count;
                    }
                    else
                    {
                        SBK_OpenPage = (itr_spellbook_page_count - 1);
                    }

                    player_is_casting = ST_TRUE;
                }
            }
        }
    }

    if(player_is_casting == ST_FALSE)
    {
        SBK_OpenPage = SBK_SomePageSaveVar;
    }

    // TODO  Assign_Auto_Function(Spellbook_Screen_Draw, 2);

    Set_Outline_Color(0);
    Set_Font_Style_Shadow_Down(0, 3, 0, 0);

    Clear_Fields();

    Set_Help_List((char *)&_help_entries[0], 15);

    x_start = 16;
    y_start = 12;

    for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
    {
        spellbook_page_left[itr_spellbook_page_fields] = Add_Hidden_Field((x_start + 16), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 137), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
    }
    for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
    {
        spellbook_page_right[itr_spellbook_page_fields] = Add_Hidden_Field((x_start + 148), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 268), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
    }

    hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
    hotkey_F   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, 'F', ST_UNDEFINED);
    hotkey_B   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, 'B', ST_UNDEFINED);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();

        // TODO  add help fields

        /*
            Hot-Key ESCAPE
        */
        if(input_field_idx == hotkey_ESC)
        {
            // TODO  RP_SND_LeftClickSound2();
            leave_screen = ST_TRUE;
        }

        /*
            Hot-Key Page Forward
        */
        if(input_field_idx == hotkey_F)
        {
            if((SBK_BookPageCount - 2) > SBK_OpenPage)
            {
                // TODO  RP_SND_LeftClickSound2();
                SBK_PageTurn_Small(1, 0, 0);
                SBK_OpenPage += 2;
            }
        }

        /*
            Hot-Key Page Backward
        */
        if(input_field_idx == hotkey_B)
        {
            if(SBK_OpenPage > 1)
            {
                // TODO  RP_SND_LeftClickSound2();
                SBK_PageTurn_Small(0, 0, 0);
                SBK_OpenPage -= 2;
            }
        }

        /*
            BEGIN:  Left-Click Spellbook Page Spell Fields
        */
        for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 12; itr_spellbook_page_fields++)
        {
            if(spellbook_page_left[itr_spellbook_page_fields] == input_field_idx)
            {
                if(itr_spellbook_page_fields < 6)
                {
                    if(SBK_BookPages[SBK_OpenPage].Spell_Count > itr_spellbook_page_fields)
                    {
                        // TODO  SND_LeftClickSound();
                        Ctrl_Index = itr_spellbook_page_fields;
                        spell_idx = SBK_BookPages[SBK_OpenPage].Spells[itr_spellbook_page_fields];
                        
                        if(spell_idx == _players[HUMAN_PLAYER_IDX].Spell_Cast)
                        {
                            spell_idx = ST_UNDEFINED;

                            strcpy(GUI_NearMsgString, _msg_abort_1);
                            // TODO  String_Copy_Far(MK_FP(temp_string, 0), spell_data_table[_players[HUMAN_PLAYER_IDX].Spell_Cast].Name);
                            strcpy(temp_string, spell_data_table[_players[HUMAN_PLAYER_IDX].Spell_Cast].name);
                            strcat(GUI_NearMsgString, temp_string);
                            strcat(GUI_NearMsgString, _msg_abort_3);
                            Abort_Spell__YN = Confirmation_Box(GUI_NearMsgString);

                            if(Abort_Spell__YN == ST_TRUE)
                            {
                                _players[HUMAN_PLAYER_IDX].Cast_Cost_Left = 0;
                                _players[HUMAN_PLAYER_IDX].Cast_Cost_Initial = 0;
                                _players[HUMAN_PLAYER_IDX].Spell_Cast = 0;
                                spell_idx = ST_UNDEFINED;
                                leave_screen = ST_TRUE;
                            }
                        }

                        if(spell_idx > -1)
                        {
                            if(_players[HUMAN_PLAYER_IDX].Spell_Cast > 0)
                            {
                                strcpy(GUI_NearMsgString, _msg_abort_1);
                                // TODO  String_Copy_Far(MK_FP(temp_string, 0), spell_data_table[_players[HUMAN_PLAYER_IDX].Spell_Cast].Name);
                                strcpy(temp_string, spell_data_table[_players[HUMAN_PLAYER_IDX].Spell_Cast].name);
                                strcat(GUI_NearMsgString, temp_string);
                                strcat(GUI_NearMsgString, _msg_abort_3);
                                Abort_Spell__YN = Confirmation_Box(GUI_NearMsgString);

                                if(Abort_Spell__YN != ST_TRUE)
                                {
                                    spell_idx = ST_UNDEFINED;
                                }
                                else
                                {
                                    _players[HUMAN_PLAYER_IDX].Cast_Cost_Left -= _players[HUMAN_PLAYER_IDX].Cast_Cost_Initial;
                                    _players[HUMAN_PLAYER_IDX].Cast_Cost_Initial = 0;
                                    _players[HUMAN_PLAYER_IDX].Spell_Cast = 0;
                                }
                            }
                            else
                            {
                                Abort_Spell__YN = ST_TRUE;
                            }

                            // TODO  IDK_Message(spell_idx, &Abort_Spell__YN);

                            if(Abort_Spell__YN == ST_TRUE)
                            {
                                leave_screen = ST_TRUE;
                                var_A = ST_TRUE;
                                _players[HUMAN_PLAYER_IDX].Spell_Cast = spell_idx;
                            }

                        }
                    }
                    else
                    {
                        spell_idx = ST_UNDEFINED;
                    }
                }
                else
                {

                }
            }
        }

        /*
            END:  Left-Click Spellbook Page Spell Fields
        */

        // TODO  Assign_Auto_Function(Spellbook_Screen_Draw, 2);

        if(leave_screen == ST_FALSE)
        {
            Clear_Fields();

            x_start = 16;
            y_start = 12;

            for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
            {
                spellbook_page_left[itr_spellbook_page_fields] = Add_Hidden_Field((x_start + 16), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 137), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
            }
            for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
            {
                spellbook_page_right[itr_spellbook_page_fields] = Add_Hidden_Field((x_start + 148), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 268), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
            }

            hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, '\x1B', ST_UNDEFINED);
            hotkey_F   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, 'F',   ST_UNDEFINED);
            hotkey_B   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, 'B',   ST_UNDEFINED);

            Set_Page_Off();
            Spellbook_Screen_Draw();
            PageFlip_FX();
        }

    }  /* END:  while(leave_screen == ST_FALSE) */


    // TODO  Deactivate_Auto_Function();

    current_screen = scr_Main_Screen;

    SBK_SomePageSaveVar = SBK_OpenPage;

    Clear_Fields();

    OVL_EnableIncmBlink();

    Deactivate_Help_List();

    if(var_A == ST_TRUE)
    {
        // TODO  WIZ_SetOverlandSpell(0, Spell_Index, Ctrl_Index);
    }
    else
    {
        OVL_MosaicFlip__STUB();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Spellbook_Screen()\n", __FILE__, __LINE__);
#endif
}


// WZD o134p04
// WIZ_SetOverlandSpell()

// WZD o134p05
// sub_B9837()

// WZD o134p06
/*
does a full draw of the main screen on the offscreen video page
and flips the current page to the offscreen video page
    PageFlip_FX() |-> Apply_Palette(); Toggle_Pages();

XREF:
    j_Full_Draw_Main_Screen()
        ...(58)
        (23) G_OVL_Cast()
        Cast_BlackWind(), Cast_CallOfTheVoid(), Cast_ChangeTerain(), Cast_ChaosChannels(), Cast_Corruption(), Cast_FloatingIsland(), Cast_Incarnation(), Cast_Lycantrophy(), Cast_NaturesCures(), Cast_RaiseVolcano(), Cast_Resurrection(), Cast_Stasis(), Cast_SummonHero(), Cast_WarpNode(), Cast_WordOfRecall()
        ...
        Chancellor_Screen()
        City_Built_Building_Message()
        Combat__STUB()
        Show_Event_Message()
        Hero_LevelUp_Popup()
        Hire_Hero_Popup()
        IDK_SummonAnim()
        Player_Process_Item_Pool()
        WIZ_Banishment()
        WIZ_ConquestSpells()

*/
void Full_Draw_Main_Screen(void)
{
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    PageFlip_FX();
}


// WZD o134p07
void OVL_MosaicFlip__STUB(void)
{
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    VGA_MosaicFlip__STUB();
}

// WZD o134p08
// OVL_CounterMessage()

// WZD o134p09
// sub_B9D14()

// WZD o134p10
// GAME_LoadSpellAnim()

// WZD o134p11
// SPL_LoadSoundEffect()

// WZD o134p12
// sub_BA1AF()
