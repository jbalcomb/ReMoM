/*
    Mirror Screen

    WIZARDS.EXE
        ovr074
*/
/*
    WIZARDS.EXE  ovr074
*/

#include "WIZVIEW.H"

#include "MOX/MOX_DAT.H"  /* _screen_seg */

#include "CITYCALC.H"
#include "Help.H"
#include "UNITTYPE.H"   // WTFMATE

#include <string.h>     /* memcpy() memset(), strcat(), strcpy(); */



// MainScr.C
// WZD dseg:956E
extern SAMB_ptr mirror_screen_background_seg;

// MagicScr.C
// WZD dseg:35A4
extern char diplo_state[11][9];
// WZD dseg:35BA
extern char * _treaty_type_names[5];
// WZD dseg:958E
extern SAMB_ptr lilwiz_gem_segs[5];
// WZD dseg:C1EA
extern SAMB_ptr magic_dipl_icon_segs[15];





// WZD dseg:35A4                                                 BEGIN: ovr074 - Initialized Data  (Mirror Screen)

// WZD dseg:35A4 C4 35                                           diplo_state@ dw offset aHate            ; DATA XREF: Magic_Screen_Draw+A38r ...
// WZD dseg:35A4                                                                                         ; "diplomatic relations"
// WZD dseg:35A6 C9 35                                           dw offset aTroubled                     ; "Troubled"
// WZD dseg:35A8 D2 35                                           dw offset aTense                        ; "Tense"
// WZD dseg:35AA D8 35                                           dw offset aRestless                     ; "Restless"
// WZD dseg:35AC E1 35                                           dw offset aUnease                       ; "Unease"
// WZD dseg:35AE E8 35                                           dw offset aNeutral                      ; "Neutral"
// WZD dseg:35B0 F0 35                                           dw offset aRelaxed                      ; "Relaxed"
// WZD dseg:35B2 F8 35                                           dw offset aCalm                         ; "Calm"
// WZD dseg:35B4 FD 35                                           dw offset aPeaceful_0                   ; "Peaceful"
// WZD dseg:35B6 06 36                                           dw offset aFriendly                     ; "Friendly"
// WZD dseg:35B8 0F 36                                           dw offset aHarmony                      ; "Harmony"

// WZD dseg:35BA 17 36                                           treaty_type_names dw offset aNoTreaty   ; DATA XREF: Mirror_Screen_Draw+22Ar
// WZD dseg:35BA                                                                                         ; "No Treaty"
// WZD dseg:35BC 21 36                                           dw offset aWizardPact_1                 ; "Wizard Pact"
// WZD dseg:35BE 2D 36                                           dw offset aAlliance_1                   ; "Alliance"
// WZD dseg:35C0 36 36                                           off_3A060 dw offset aWar                ; "War"
// WZD dseg:35C2 3A 36                                           dw offset aFinalWar                     ; "Final War"
// WZD dseg:35C4 48 61 74 65                                     aHate db 'Hate'                         ; DATA XREF: dseg:diplo_state@o

// WZD dseg:35C8
char str_empty_string__ovr074[] = "";

// WZD dseg:35C9 54 72 6F 75 62 6C 65 64 00                      aTroubled db 'Troubled',0               ; DATA XREF: dseg:35A6o
// WZD dseg:35D2 54 65 6E 73 65 00                               aTense db 'Tense',0                     ; DATA XREF: dseg:35A8o
// WZD dseg:35D8 52 65 73 74 6C 65 73 73 00                      aRestless db 'Restless',0               ; DATA XREF: dseg:35AAo
// WZD dseg:35E1 55 6E 65 61 73 65 00                            aUnease db 'Unease',0                   ; DATA XREF: dseg:35ACo
// WZD dseg:35E8 4E 65 75 74 72 61 6C 00                         aNeutral db 'Neutral',0                 ; DATA XREF: dseg:35AEo
// WZD dseg:35F0 52 65 6C 61 78 65 64 00                         aRelaxed db 'Relaxed',0                 ; DATA XREF: dseg:35B0o
// WZD dseg:35F8 43 61 6C 6D 00                                  aCalm db 'Calm',0                       ; DATA XREF: dseg:35B2o
// WZD dseg:35FD 50 65 61 63 65 66 75 6C 00                      aPeaceful_0 db 'Peaceful',0             ; DATA XREF: dseg:35B4o
// WZD dseg:3606 46 72 69 65 6E 64 6C 79 00                      aFriendly db 'Friendly',0               ; DATA XREF: dseg:35B6o
// WZD dseg:360F 48 61 72 6D 6F 6E 79 00                         aHarmony db 'Harmony',0                 ; DATA XREF: dseg:35B8o

// WZD dseg:3617 4E 6F 20 54 72 65 61 74 79 00                   aNoTreaty db 'No Treaty',0              ; DATA XREF: dseg:treaty_type_nameso
// WZD dseg:3621 57 69 7A 61 72 64 20 50 61 63 74 00             aWizardPact_1 db 'Wizard Pact',0        ; DATA XREF: dseg:35BCo
// WZD dseg:362D 41 6C 6C 69 61 6E 63 65 00                      aAlliance_1 db 'Alliance',0             ; DATA XREF: dseg:35BEo
// WZD dseg:3636 57 61 72 00                                     aWar db 'War',0                         ; DATA XREF: dseg:off_3A060o
// WZD dseg:363A 46 69 6E 61 6C 20 57 61 72 00                   aFinalWar db 'Final War',0              ; DATA XREF: dseg:35C2o

// WZD dseg:3644
char str_hotkey_ESC__ovr074[] = "\x1B";

// WZD dseg:3646
char str_SPACE__ovr074[] = " ";

// WZD dseg:3648
char str_Relations[] = "Relations:";

// WZD dseg:3653
char str_Treaties[] = "Treaties:";

// WZD dseg:365D
char str_Personality[] = "Personality:";

// WZD dseg:366A
char str_Objective[] = "Objective:";

// WZD dseg:3675
char str_GP[] = "GP";

// WZD dseg:3678
char str_MP[] = "MP";

// WZD dseg:367B
char str_Fame__ovr074[] = "Fame";

// WZD dseg:3680 61 6E 64 00                                     
char mirror_screen_abilities_list_and[] = "and";
// WZD dseg:3684 2C 00                                           
char mirror_screen_abilities_list_comma[] = ",";
// WZD dseg:3686 2E 00                                           
char mirror_screen_abilities_list_period[] = ".";

// WZD dseg:3688
char str_Heroes[] = "Heroes";

// WZD dseg:368F
char str_The[] = "The";

// WZD dseg:3693
char lilwiz_lbx_file__ovr074[] = "LILWIZ";

// WZD dseg:369A
char magic_lbx_file__ovr074[] = "MAGIC";

// WZD dseg:369A                                                 END: ovr074 - Initialized Data  (Mirror Screen)





// WZD dseg:C254                                                 BEGIN: ovr074 - Uninitialized Data  (Mirror Screen)

// WZD dseg:C254
int16_t mirror_screen_player_idx;

// WZD dseg:C256
SAMB_ptr * mirror_screen_bookshelf_icon_segs;

// WZD dseg:C258
int16_t mirror_start_y;
// WZD dseg:C25A
int16_t mirror_start_x;

// WZD dseg:C25C
int16_t mirror_screen_bookshelf_icon_count;

// WZD dseg:C25C                                                 END: ovr074 - Uninitialized Data  (Mirror Screen)



// WZD o74p01
void Mirror_Screen(int16_t player_idx, int16_t OL, int16_t OT, int16_t OR, int16_t OB)
{
    int16_t full_screen_ESC_field;
    int16_t input_field_idx;
    int16_t leave_screen;  // _SI_

    mirror_screen_player_idx = player_idx;

    Clear_Fields();

    Set_Page_Off();

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    PageFlip_FX();

    Mirror_Screen_Load();

    Deactivate_Help_List();

    Players_Update_Magic_Power();

    Deactivate_Auto_Function();

    Assign_Auto_Function(Mirror_Screen_Draw, 1);

    Set_Mirror_Screen_Help_List();

    mirror_start_x = 75;
    mirror_start_y = 2;

    if(mirror_screen_player_idx == _human_player_idx) /* ¿ current player ? */
    {
        Set_Page_Off();
        Mirror_Screen_Draw();
        // TODO  HLP_NoResizeExpand__TODO(mirror_start_x, mirror_start_y, (mirror_start_x + 179), (mirror_start_y + 195), OL, OT, OR, OB, ST_TRUE);
    }

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr074[0], ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_ESC_field)
        {
            Play_Left_Click();
            leave_screen = ST_UNDEFINED;
        }

        if (leave_screen == ST_FALSE)
        {
            Copy_Back_To_Off();
            Mirror_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    Deactivate_Help_List();
    Clear_Fields();
    Deactivate_Auto_Function();
    PageFlipEffect = 3;

}


// WZD o74p02
void Mirror_Screen_Draw(void)
{
    int16_t retort_helps[NUM_WIZARD_SPECIAL_ABILITIES];
    int16_t wizard_special_abilities_index[9];  // 1-byte, signed
    char dest[4];
    int16_t x;
    int16_t string_width;
    int16_t y;
    int16_t var_14;
    uint8_t colors[6];
    int16_t val;
    int8_t * wizard_special_abilities;
    int16_t hero_unit_idx;
    int16_t var_6;
    int16_t var_4;
    int16_t itr;
    int16_t wizard_special_abilities_count;  // _SI_
    int16_t wizard_hero_count;  // _SI_
    int16_t IDK_x;  // _DI_

    strcpy(dest, str_SPACE__ovr074);

// TODO  mov     [bp+retort_helps], HLP_ALCHEMY_Ret
// TODO  mov     [bp+retort_helps+2], HLP_WARLORD
// TODO  mov     [bp+retort_helps+4], HLP_CHAOS_MASTERY
// TODO  mov     [bp+retort_helps+6], HLP_NATURE_MASTERY
// TODO  mov     [bp+retort_helps+8], HLP_SORCERY_MASTERY
// TODO  mov     [bp+retort_helps+0Ah], HLP_INFERNAL_POWER
// TODO  mov     [bp+retort_helps+0Ch], HLP_DIVINE_POWER
// TODO  mov     [bp+retort_helps+0Eh], HLP_SAGE_MASTER
// TODO  mov     [bp+retort_helps+10h], HLP_CHANNELER
// TODO  mov     [bp+retort_helps+12h], HLP_MYRRAN
// TODO  mov     [bp+retort_helps+14h], HLP_ARCHMAGE
// TODO  mov     [bp+retort_helps+16h], HLP_MANA_FOCUSING
// TODO  mov     [bp+retort_helps+18h], HLP_NODE_MASTERY
// TODO  mov     [bp+retort_helps+1Ah], HLP_FAMOUS
// TODO  mov     [bp+retort_helps+1Ch], HLP_RUNEMASTER
// TODO  mov     [bp+retort_helps+1Eh], HLP_CONJURER
// TODO  mov     [bp+retort_helps+20h], HLP_CHARISMATIC
// TODO  mov     [bp+retort_helps+22h], HLP_ARTIFICER


    if(mirror_screen_player_idx == _human_player_idx)
    {
        // this happens when you right-click on an opponent wizard's gem on the Magic Screen not when you left-click on "Mirror" on the Info Screen or hit the F9 hot-key
        // TODO  G_USW_DrawEHLines();
    }

    Set_Page_Off();

    FLIC_Draw(mirror_start_x, mirror_start_y, mirror_screen_background_seg);

    FLIC_Draw((mirror_start_x + 11), (mirror_start_y + 11), lilwiz_gem_segs[0]);


    Set_Outline_Color(234);

    colors[0] = 150;

    for(itr = 1; itr < 6; itr++)
    {
        colors[itr] = (177 + itr);
    }

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Print_Centered((mirror_start_x + 114), (mirror_start_y + 11), _players[mirror_screen_player_idx].name);

// TODO  mov     [_help_entries.help_14.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_15.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_16.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_17.entry_idx], 0FFFFh

    if(mirror_screen_player_idx != _human_player_idx)
    {

    var_14 = ((_players[_human_player_idx].Dipl.Visible_Rel[mirror_screen_player_idx] + 100) / 20);

    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    strcpy(GUI_String_1, str_Relations);  // "Relations:"

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, diplo_state[var_14]);

    Print((mirror_start_x + 59), (mirror_start_y + 25), GUI_String_1);

// TODO  mov     [_help_entries.help_14.entry_idx], HLP_RELATIONS
// TODO  mov     ax, [mirror_start_x]
// TODO  add     ax, 59
// TODO  mov     [_help_entries.help_14.x1], ax
// TODO  mov     ax, [mirror_start_y]
// TODO  add     ax, 25
// TODO  mov     [_help_entries.help_14.y1], ax
// TODO  mov     ax, [mirror_start_x]
// TODO  add     ax, 159
// TODO  mov     [_help_entries.help_14.x2], ax
// TODO  mov     ax, [mirror_start_y]
// TODO  add     ax, 32
// TODO  mov     [_help_entries.help_14.y2], ax

    strcpy(GUI_String_1, str_Treaties);  // "Treaties:"

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, _treaty_type_names[_players[_human_player_idx].Dipl.Dipl_Status[mirror_screen_player_idx]]);

    Print((mirror_start_x + 59), (mirror_start_y + 33), GUI_String_1);


// TODO  mov     [_help_entries.help_15.entry_idx], 0FFFFh

    if(_players[_human_player_idx].Dipl.Dipl_Status[mirror_screen_player_idx] == 1)
    {
        // TODO  mov     [_help_entries.help_15.entry_idx], HLP_WIZARD_PACT
    }

    if(_players[_human_player_idx].Dipl.Dipl_Status[mirror_screen_player_idx] == 2)
    {
        // TODO  mov     [_help_entries.help_15.entry_idx], HLP_ALLIANCE
    }

// TODO  mov     ax, [mirror_start_x]
// TODO  add     ax, 59
// TODO  mov     [_help_entries.help_15.x1], ax
// TODO  mov     ax, [mirror_start_y]
// TODO  add     ax, 33
// TODO  mov     [_help_entries.help_15.y1], ax
// TODO  mov     ax, [mirror_start_x]
// TODO  add     ax, 159
// TODO  mov     [_help_entries.help_15.x2], ax
// TODO  mov     ax, [mirror_start_y]
// TODO  add     ax, 40
// TODO  mov     [_help_entries.help_15.y2], ax

    strcpy(GUI_String_1, str_Personality);  // "Personality:"

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, _personality_type_names[_players[mirror_screen_player_idx].Personality]);

    Print((mirror_start_x + 59), (mirror_start_y + 41), GUI_String_1);



// mov     ax, [mirror_screen_player_idx]
// mov     dx, size s_WIZARD
// imul    dx
// mov     bx, ax
// mov     ax, [_players.Personality+bx]
// add     ax, HLP_MANIACAL
// mov     [_help_entries.help_16.entry_idx], ax
// mov     ax, [mirror_start_x]
// add     ax, 59
// mov     [_help_entries.help_16.x1], ax
// mov     ax, [mirror_start_y]
// add     ax, 41
// mov     [_help_entries.help_16.y1], ax
// mov     ax, [mirror_start_x]
// add     ax, 159
// mov     [_help_entries.help_16.x2], ax
// mov     ax, [mirror_start_y]
// add     ax, 48
// mov     [_help_entries.help_16.y2], ax

    strcpy(GUI_String_1, str_Objective);  // "Objective:"

    strcat(GUI_String_1, dest);

    strcat(GUI_String_1, _objective_type_names[_players[mirror_screen_player_idx].Objective]);

    Print((mirror_start_x + 59), (mirror_start_y + 49), GUI_String_1);

// TODO  mov     ax, [mirror_screen_player_idx]
// TODO  mov     dx, size s_WIZARD
// TODO  imul    dx
// TODO  mov     bx, ax
// TODO  mov     ax, [_players.Objective+bx]
// TODO  add     ax, HLP_PRAGMATIST
// TODO  mov     [_help_entries.help_17.entry_idx], ax
// TODO  mov     ax, [mirror_start_x]
// TODO  add     ax, 59
// TODO  mov     [_help_entries.help_17.x1], ax
// TODO  mov     ax, [mirror_start_y]
// TODO  add     ax, 49
// TODO  mov     [_help_entries.help_17.y1], ax
// TODO  mov     ax, [mirror_start_x]
// TODO  add     ax, 159
// TODO  mov     [_help_entries.help_17.x2], ax
// TODO  mov     ax, [mirror_start_y]
// TODO  add     ax, 56
// TODO  mov     [_help_entries.help_17.y2], ax

    }


    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Print_Integer_Right((mirror_start_x + 31), (mirror_start_y + 75), _players[mirror_screen_player_idx].gold_reserve);

    Print_Integer_Right((mirror_start_x + 159), (mirror_start_y + 75), _players[mirror_screen_player_idx].mana_reserve);

    Set_Font_Style_Shadow_Down(0, 15, 0, 0);

    Print((mirror_start_x + 34), (mirror_start_y + 75), str_GP);

    Print((mirror_start_x + 162), (mirror_start_y + 75), str_MP);

    itr = ((mirror_screen_bookshelf_icon_count * 8) / 2);
    var_6 = (mirror_start_x + (90 - itr));
    for(itr = 0; itr < mirror_screen_bookshelf_icon_count; itr++)
    {
        FLIC_Draw((var_6 + (itr * 8)), (mirror_start_y + 66), mirror_screen_bookshelf_icon_segs[itr]);
    }


// TODO  mov     [_help_entries.help_07.entry_idx], e_ST_UNDEFINED

    val = Player_Fame(mirror_screen_player_idx);

    if(val > 0)
    {
// TODO  mov     [_help_entries.help_07.entry_idx], HLP_FAMOUS_Fame
        Set_Font_Style_Shadow_Down(2, 15, 0, 0);
        Print_Integer_Right((mirror_start_x + 79), (mirror_start_y + 96), val);
        Print((mirror_start_x + 85), (mirror_start_y + 96), str_Fame__ovr074);  // "Fame"
    }


// TODO  mov     [_help_entries.help_08.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_09.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_10.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_11.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_12.entry_idx], 0FFFFh
// TODO  mov     [_help_entries.help_13.entry_idx], 0FFFFh

    strcpy(GUI_String_1, str_empty_string__ovr074);

    wizard_special_abilities_count = 0;

    wizard_special_abilities = &_players[mirror_screen_player_idx].alchemy;

    for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
    {
        // WTF  mov  bx, [bp+var_A];  add  bx, [bp+itr];  cmp  [byte ptr bx], 1
        if(wizard_special_abilities[itr] == ST_TRUE)
        {
            // WTF  mov  al, [byte ptr bp+itr];  mov  [byte ptr bp+_SI_IDK_count+], al
            wizard_special_abilities_index[wizard_special_abilities_count] = itr;
            wizard_special_abilities_count++;
        }
    }

    if(wizard_special_abilities_count > 0)
    {
        Set_Outline_Color(234);

        Set_Font_Style_Shadow_Down(1, 15, 0, 0);

        Set_Font_Spacing_Width(1);

        strcpy(GUI_String_1, str_empty_string__ovr074);

        x = (mirror_start_x + 13);
        IDK_x = x;
        y = (mirror_start_y + 111);

        if(wizard_special_abilities_count > 5)
        {
            y -= 2;
        }

        var_4 = 0;

        for(itr = 0; itr < wizard_special_abilities_count; itr++)
        {
            strcpy(GUI_String_1, _wizard_abilities_names[wizard_special_abilities_index[itr]]);

            if((itr + 1) != wizard_special_abilities_count)
            {
                if((itr + 2) == wizard_special_abilities_count)
                {
                    strcat(GUI_String_1, dest);
                    strcat(GUI_String_1, mirror_screen_abilities_list_and);
                }
                else
                {
                    strcat(GUI_String_1, mirror_screen_abilities_list_comma);
                }

                strcat(GUI_String_1, dest);
            }

            string_width = Get_String_Width(GUI_String_1);

            if((IDK_x + string_width) > (x + 155))
            {
                IDK_x = x;
                y += 7;
            }

            Print(IDK_x, y, GUI_String_1);

// TODO  mov     ax, [bp+itr]
// TODO  mov     dx, 10
// TODO  imul    dx
// TODO  mov     bx, ax
// TODO  mov     [_help_entries.help_08.x1+bx], _DI_IDK_x
// TODO  mov     ax, [bp+itr]
// TODO  mov     dx, size s_HELP_ENTRY
// TODO  imul    dx
// TODO  mov     dx, [bp+y]
// TODO  mov     bx, ax
// TODO  mov     [_help_entries.help_08.y1+bx], dx
// TODO  mov     ax, [bp+itr]
// TODO  mov     dx, size s_HELP_ENTRY
// TODO  imul    dx
// TODO  mov     dx, _DI_IDK_x
// TODO  add     dx, [bp+string_width]
// TODO  mov     bx, ax
// TODO  mov     [_help_entries.help_08.x2+bx], dx
// TODO  mov     ax, [bp+itr]
// TODO  mov     dx, size s_HELP_ENTRY
// TODO  imul    dx
// TODO  mov     dx, [bp+y]
// TODO  add     dx, 5
// TODO  mov     bx, ax
// TODO  mov     [_help_entries.help_08.y2+bx], dx
// TODO  lea     ax, [bp+wizard_special_abilities_index]
// TODO  mov     bx, [bp+itr]
// TODO  add     bx, ax
// TODO  mov     al, [bx]
// TODO  cbw
// TODO  shl     ax, 1
// TODO  lea     dx, [bp+retort_helps]
// TODO  add     ax, dx
// TODO  mov     bx, ax
// TODO  mov     ax, [bx]
// TODO  push    ax
// TODO  mov     ax, [bp+itr]
// TODO  mov     dx, size s_HELP_ENTRY
// TODO  imul    dx
// TODO  mov     bx, ax
// TODO  pop     ax
// TODO  mov     [_help_entries.help_08.entry_idx+bx], ax

            IDK_x += (string_width + 1);

            strcpy(GUI_String_1, str_empty_string__ovr074);

        }

        Print(IDK_x, y, mirror_screen_abilities_list_period);

    }


    Set_Font_Style_Shadow_Down(2, 15, 0, 0);

    Print((mirror_start_x + 74), (mirror_start_y + 131), str_Heroes);  // "Heroes"

    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    wizard_hero_count = 0;

    for(itr = 0; itr < NUM_HERO_SLOTS; itr++)
    {
        hero_unit_idx = _players[mirror_screen_player_idx].Heroes[itr].unit_idx;

        if(
            (hero_unit_idx > ST_UNDEFINED)
            &&
            (_UNITS[hero_unit_idx].owner_idx != ST_UNDEFINED)
        )
        {

            strcpy(GUI_String_1, _players[mirror_screen_player_idx].Heroes[itr].name);

            strcat(GUI_String_1, dest);

            strcat(GUI_String_1, str_The);

            strcat(GUI_String_1, dest);

            strcat(GUI_String_1, *_unit_type_table[_UNITS[hero_unit_idx].type].name);

            Print((mirror_start_x + 13), (mirror_start_y + 142 + (wizard_hero_count * 8)), GUI_String_1);

            wizard_hero_count++;

        }

    }

}


// WZD o74p03
void Mirror_Screen_Load(void)
{
    int16_t var_4;
    int16_t var_2;
    int16_t itr1;  // _SI_
    int16_t itr2;  // _DI_
    int16_t lilwiz_lbx_entry_num;  // DNE in Dasm;
    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(20);

    // mirror_screen_bookshelf_icon_segs = Near_Allocate_Next(40);  // 20 * 2-byte 16-bit pointer
    mirror_screen_bookshelf_icon_segs = (SAMB_ptr *)Near_Allocate_Next(20 * sizeof(unsigned char *));  // 20 * 4-byte 64-bit pointer

    // LILWIZ.LBX, {0, ..., 69}
    // {MERLE2, SHAMAN2, VOODOO2, EASTERN2, ARAB2, WHITEH2, REDEYES2, LIZARD2, BARON2, BLOND2, GALEN2, HEDBAND2, AZTEC2, WITCH2}
    // {blue, green, purple, red, yellow}
    // e.g.,
    // LILWIZ.LBX,  00  MERLE2      blue
    // LILWIZ.LBX,  01  MERLE2      green
    // LILWIZ.LBX,  02  MERLE2      purple
    // LILWIZ.LBX,  03  MERLE2      red
    // LILWIZ.LBX,  04  MERLE2      yellow
    // lilwiz_gem_segs[0] = LBX_Reload(lilwiz_lbx_file__ovr074, ((_players[mirror_screen_player_idx].wizard_id * 5) + _players[mirror_screen_player_idx].banner_id), _screen_seg);
    lilwiz_lbx_entry_num = ((_players[mirror_screen_player_idx].wizard_id * 5) + _players[mirror_screen_player_idx].banner_id);
    lilwiz_gem_segs[0] = LBX_Reload(lilwiz_lbx_file__ovr074, lilwiz_lbx_entry_num, _screen_seg);

    // MAGIC.LBX, 033   BOOKS       green book 1
    // MAGIC.LBX, 034   BOOKS       green book 2
    // MAGIC.LBX, 035   BOOKS       green book 3
    // MAGIC.LBX, 036   BOOKS       blue book 1
    // MAGIC.LBX, 037   BOOKS       blue book 2
    // MAGIC.LBX, 038   BOOKS       blue book 3
    // MAGIC.LBX, 039   BOOKS       red book 1
    // MAGIC.LBX, 040   BOOKS       red book 2
    // MAGIC.LBX, 041   BOOKS       red book 3
    // MAGIC.LBX, 042   BOOKS       white book 1
    // MAGIC.LBX, 043   BOOKS       white book 2
    // MAGIC.LBX, 044   BOOKS       white book 3
    // MAGIC.LBX, 045   BOOKS       black book 1
    // MAGIC.LBX, 046   BOOKS       black book 2
    // MAGIC.LBX, 047   BOOKS       black book 3
    // MAGIC.LBX, 048   BOOKS       grey book 1
    // MAGIC.LBX, 049   BOOKS       grey book 2
    // MAGIC.LBX, 050   BOOKS       grey book 3
    for(itr1 = 0; itr1 < NUM_WIZARD_SPECIAL_ABILITIES; itr1++)
    {
        mirrow_screen_books_segs[itr1] = LBX_Reload_Next(magic_lbx_file__ovr074, (33 + itr1), _screen_seg);
    }

    for(itr1 = 0; itr1 < 15; itr1++)
    {
        magic_dipl_icon_segs[itr1] = LBX_Reload_Next(magic_lbx_file__ovr074, (60 + itr1), _screen_seg);
    }

    mirror_screen_bookshelf_icon_count = 0;

    for(itr2 = 0; itr2 < 5; itr2++)
    {
        var_4 = (itr2 * 3);  // {0,3,6,9,12}

        var_2 = 0;

        for(itr1 = 0; itr1 < _players[mirror_screen_player_idx].spellranks[itr2]; itr1++)
        {
            mirror_screen_bookshelf_icon_segs[mirror_screen_bookshelf_icon_count] = mirrow_screen_books_segs[(var_4 + var_2)];
            var_2 = ((var_2 + 1) / 3);
            mirror_screen_bookshelf_icon_count++;
        }
    }

}
