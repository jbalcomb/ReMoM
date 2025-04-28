/*
    Level Make

    WIZARDS.EXE
        ovr102
*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/SOUND.H"

#include "MOM.H"



// WZD dseg:576C                                                 BEGIN:  ovr102 - Initialized Data

// WZD dseg:576C
char reload_lbx_file__ovr102[] = "RELOAD";

// WZD dseg:5772
char str_empty_string__ovr102[] = "";

// WZD dseg:5773
char portrait_lbx_file__ovr102[] = "PORTRAIT";

// WZD dseg:577C
char music_lbx_file__ovr102[] = "MUSIC";

// WZD dseg:5782
char str_hotkey_ESC__ovr102[] = "0x1B";

// WZD dseg:5784
char str_HasMadeALevel__ovr102[] = " has made a level.";

// WZD dseg:5797
char str_Plus1HitPoints__ovr102[] = "+1 Hit Points";

// WZD dseg:57A5
char str_Plus1Resistance__ovr102[] = "+1 Resistance";

// WZD dseg:57B3
char str_Plus1Attack__ovr102[] = "+1 Attack";

// WZD dseg:57BD
char str_Plus__ovr102[] = "+";

// WZD dseg:57BF
char str_SPACE_Defense__ovr102[] = " Defense";

// WZD dseg:57C8
char str_SPACE_ToHit__ovr102[] = " To Hit";

// WZD dseg:57D0
char str_Leadership__ovr102[] = "Leadership";

// WZD dseg:57DB 7D 00                                           
char str_CloseBrace__ovr102[] = "}";

// WZD dseg:57DD
char str_Legendary__ovr102[] = "Legendary";

// WZD dseg:57E7
char str_Blademaster__ovr102[] = "Blademaster";

// WZD dseg:57F3
char str_Armsmaster__ovr102[] = "Armsmaster";

// WZD dseg:57FE
char str_Constitution__ovr102[] = "Constitution";

// WZD dseg:580B
char str_Might__ovr102[] = "Might";

// WZD dseg:5811
char str_ArcanePower__ovr102[] = "Arcane Power";

// WZD dseg:581E
char str_Sage__ovr102[] = "Sage";

// WZD dseg:5823
char str_SPACE__ovr102[] = " ";

// WZD dseg:5825
char str_SPACE_rp__ovr102[] = " rp";

// WZD dseg:5829
char str_Prayermaster__ovr102[] = "Prayermaster";

// WZD dseg:5836
char str_Agility__ovr102[] = "Agility";

// WZD dseg:583E
char str_SPACE_Plus__ovr102[] = " +";

// WZD dseg:5841
char str_Caster__ovr102[] = "Caster";

// WZD dseg:5848
char str_SPACE_OpenParen__ovr102[] = " (";

// WZD dseg:584B
char str_CloseParen__ovr102[] = ")";

// WZD dseg:584D
char str_SPACE_mp__ovr102[] = " mp";

// WZD dseg:5851 00                                              align 2

// WZD dseg:5851                                                 END:  ovr102 - Initialized Data



// WZD dseg:C79E                                                 BEGIN:  ovr102 - Uninitialized Data

// WZD dseg:C79E
// MoM_Data  SAMB_ptr m_hero_portrait_seg;

// WZD dseg:C7A0
SAMB_ptr LVLMAKE_pc_level_bottom;

// WZD dseg:C7A2
SAMB_ptr LVLMAKE_pc_level_screen;

// WZD dseg:C7A4
int16_t lvlmake_base_y;

// WZD dseg:C7A6
int16_t lvlmake_base_x;

// WZD dseg:C7A8
int16_t lvlmake_unit_idx;

// WZD dseg:C7A8                                                 END:  ovr102 - Uninitialized Data



/*
    WIZARDS.EXE  ovr120
*/

// WZD o102p01
void Hero_LevelUp_Popup(int16_t unit_idx)
{
    SAMB_ptr sound_seg;
    int16_t uu_screen_coordinate;
    int16_t ymin;
    int16_t hotkey_ESC;
    int16_t input_field_idx;
    int16_t leave_screen;
    int16_t itr;  // _SI_
    int16_t xmin;  // _DI_

    if(_UNITS[unit_idx].Level > MAX_LEVEL_HERO)
    {
        return;
    }

    // TODO  Stop_All_Sounds__STUB();
    
    Allocate_Reduced_Map();

    Full_Draw_Main_Screen();

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    GFX_Swap_Overland();

    lvlmake_unit_idx = unit_idx;

    lvlmake_base_x = 31;
    lvlmake_base_y = 41;

    Clear_Fields();

    Deactivate_Auto_Function();

    Assign_Auto_Function(Hero_LevelUp_Popup_Draw, 1);

    _UNITS[lvlmake_unit_idx].Level += 1;

    Load_Battle_Unit(unit_idx, global_battle_unit);

    LVLMAKE_pc_level_screen = LBX_Reload_Next(reload_lbx_file__ovr102, 23, _screen_seg);

    LVLMAKE_pc_level_bottom = LBX_Reload_Next(reload_lbx_file__ovr102, 24, _screen_seg);

    m_hero_portrait_seg = LBX_Reload_Next(portrait_lbx_file__ovr102, global_battle_unit->hero_portrait_idx, _screen_seg);

    if(magic_set.event_music == ST_TRUE)
    {
        sound_seg = LBX_Reload(music_lbx_file__ovr102, MUSIC_Fanfare, SND_Music_Segment);
        Play_Sound__WIP(sound_seg);
    }

    GUI_String_1 = (char *)Near_Allocate_First(100);

    uv_specials_list_array = (struct s_UV_List *)Near_Allocate_Next(1520);

    uv_specials_list_count = 0;

    Hero_Build_Specials_List(unit_idx, &uv_specials_list_array[0], &uv_specials_list_count);

    SETMAX(uv_specials_list_count, 8);

    Deactivate_Help_List();

    Set_Level_Screen_Help_List();

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        
        Mark_Time();

        Clear_Fields();

        xmin = (lvlmake_base_x + 13);

        ymin = (lvlmake_base_y + 48);

        for(itr = 0; itr < uv_specials_list_count; itr++)
        {
            
            uv_special_icon_fields[itr] = Add_Picture_Field(xmin, ymin, (SAMB_ptr)uv_specials_list_array->picts[itr], str_empty_string__ovr102[0], uv_specials_list_array->helps[itr]);

            // TODO  _help_entries[itr].help_00.entry_idx = uv_specials_list_array.helps[itr];

            uv_special_desc_fields[itr] = Add_Hidden_Field((xmin + 19), (ymin + 5), (xmin + 93), (ymin + 13), str_empty_string__ovr102[0], uv_specials_list_array->helps[itr]);

            xmin = (lvlmake_base_x + 13);

            if(itr == 0)
            {
                xmin += 115;
            }
            else
            {
                if((itr % 2) == 1)
                {
                    ymin += 17;
                }
                else
                {
                    xmin += 115;
                }
            }

        }

        uu_screen_coordinate = (lvlmake_base_y + 49 + (((uv_specials_list_count + 1) / 2) * 17));

        hotkey_ESC = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr102[0], ST_UNDEFINED);

        input_field_idx = Get_Input();

        for(itr = 0; itr < uv_specials_list_count; itr++)
        {
            if(
                (uv_special_icon_fields[itr] == input_field_idx)
                ||
                (uv_special_desc_fields[itr] == input_field_idx)
            )
            {
                leave_screen = ST_UNDEFINED;
            }
        }

        if(input_field_idx == hotkey_ESC)
        {
            leave_screen = ST_UNDEFINED;
        }

        if(leave_screen == ST_FALSE)
        {
            Copy_Back_To_Off();
            Hero_LevelUp_Popup_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

    }

    GFX_Swap_Cities();
    Deactivate_Help_List();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Allocate_Reduced_Map();
    OVL_MosaicFlip__STUB();
    Play_Background_Music__STUB();

}


// WZD o102p02
void Hero_LevelUp_Popup_Draw(void)
{
    int16_t y;
    int16_t value;
    int16_t unit_level;
    char temp_string[6];
    int16_t lvlmake_icon_y;
    int16_t unit_owner_idx;
    uint8_t colors[5];
    int16_t itr_effects;  // _SI_
    int16_t lvlmake_icon_x;  // _DI_

    unit_owner_idx = _UNITS[lvlmake_unit_idx].owner_idx;

    unit_level = _UNITS[lvlmake_unit_idx].Level;

    Set_Page_Off();

    y = (lvlmake_base_y + 49 + ((uv_specials_list_count + 1) / 2));

    Set_Window(lvlmake_base_x, lvlmake_base_y, (lvlmake_base_x + 250), y);

    Clipped_Draw(lvlmake_base_x, lvlmake_base_y, LVLMAKE_pc_level_screen);

    FLIC_Draw(lvlmake_base_x, y, LVLMAKE_pc_level_bottom);

    FLIC_Draw((lvlmake_base_x + 10), (lvlmake_base_y + 10), m_hero_portrait_seg);

    colors[0] = 240;
    colors[1] = 178;
    colors[2] = 179;
    colors[3] = 180;
    colors[4] = 180;

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Set_Outline_Color(240);

    strcpy(GUI_String_1, _players[unit_owner_idx].Heroes[_UNITS[lvlmake_unit_idx].Hero_Slot].name);

    strcat(GUI_String_1, str_HasMadeALevel__ovr102);  // " has made a level."

    Print((lvlmake_base_x + 48), (lvlmake_base_y + 9), GUI_String_1);

    colors[0] = 240;
    colors[1] = 178;
    colors[2] = 178;
    colors[3] = 178;
    colors[4] = 178;

    Set_Font_Colors_15(2, &colors[0]);

    Set_Font_Style_Shadow_Down(2, 15, 0, 0);

    Set_Outline_Color(240);

    FLIC_Draw((lvlmake_base_x + 47), (lvlmake_base_y + 25), item_view_bullet_seg);

    Print((lvlmake_base_x + 55), (lvlmake_base_y + 24), str_Plus1HitPoints__ovr102);  // "+1 Hit Points"

    FLIC_Draw((lvlmake_base_x + 47), (lvlmake_base_y + 35), item_view_bullet_seg);

    Print((lvlmake_base_x + 55), (lvlmake_base_y + 34), str_Plus1Resistance__ovr102);  // "+1 Resistance"

    FLIC_Draw((lvlmake_base_x + 139), (lvlmake_base_y + 25), item_view_bullet_seg);

    Print((lvlmake_base_x + 147), (lvlmake_base_y + 24), str_Plus1Attack__ovr102);  // "+1 Attack"

    value = ST_UNDEFINED;

    switch((_UNITS[lvlmake_unit_idx].Level - 1))
    {
        case 0:
        case 2:
        case 4:
        case 6:
        {
            value = 1;
        } break;
    }

    if(value != ST_UNDEFINED)
    {
        FLIC_Draw((lvlmake_base_x + 139), (lvlmake_base_y + 35), item_view_bullet_seg);
        itoa(value, temp_string, 10);
        strcpy(GUI_String_1, str_Plus__ovr102);  // "+"
        strcat(GUI_String_1, temp_string);
        strcat(GUI_String_1, str_SPACE_Defense__ovr102);  // " Defense"
        Print((lvlmake_base_x + 147), (lvlmake_base_y + 34), GUI_String_1);
    }

    lvlmake_icon_y = (lvlmake_base_y + 48);

    lvlmake_icon_x = (lvlmake_base_x + 13);

    for(itr_effects = 0; itr_effects < uv_specials_list_count; itr_effects++)
    {
        FLIC_Draw(lvlmake_icon_x, lvlmake_icon_y, (SAMB_ptr)uv_specials_list_array->picts[itr_effects]);
        Print((lvlmake_icon_x + 19), (lvlmake_icon_y + 5), uv_specials_list_array->names[itr_effects]);
        lvlmake_icon_x = (lvlmake_base_x + 13);
        if(itr_effects == 0)
        {
            lvlmake_icon_x += 115;
        }
        else
        {
            if((itr_effects % 2) == 1)
            {
                lvlmake_icon_y += 17;
            }
            else
            {
                lvlmake_icon_x += 115;
            }
        }
    }


}


// WZD o102p03
/*

¿ ~== UnitView.C  USW_Build_Effect_List() ?
¿ ~== UnitView.C  Prod_Build_Specials_List() ?

*/
void Hero_Build_Specials_List(int16_t unit_idx, struct s_UV_List * specials_list, int16_t * specials_count)
{
    char temp_string[LEN_TEMP_STRING];
    uint32_t hero_ability_flag;
    int16_t ability_has_plus;
    int16_t ability_value;
    int16_t unit_owner;
    int16_t itr;
    int16_t unit_type;
    uint32_t hero_abilities_flags;
    int16_t specials_index; // _SI_

    *specials_count = 0;

    specials_index = 0;

    unit_type = _UNITS[unit_idx].type;

    unit_owner = _UNITS[unit_idx].owner_idx;

    strcpy(specials_list->names[specials_index], USW_HeroLevels[_UNITS[unit_idx].Level]);

    specials_list->flags[specials_index] = ST_UNDEFINED;

    specials_list->picts[specials_index] = (SAMB_INT)special_seg[(204 + _UNITS[unit_idx].Level)];

    specials_list->helps[specials_index] = (HLP_HERO_LEVEL_0 + _UNITS[unit_idx].Level);

    specials_index++;

    if(
        (_UNITS[unit_idx].Level == 2)
        ||
        (_UNITS[unit_idx].Level == 5)
        ||
        (_UNITS[unit_idx].Level == 8)
    )
    {
        strcpy(specials_list->names[specials_index], str_Plus__ovr102);
        ability_value = 1;
        itoa(ability_value, temp_string, 10);
        strcat(specials_list->names[specials_index], temp_string);
        strcat(specials_list->names[specials_index], str_SPACE_ToHit__ovr102);  // " To Hit"
        specials_list->flags[specials_index] = ST_UNDEFINED;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[250];
        specials_list->helps[specials_index] = HLP_TOHIT_PLUS;
        specials_index++;
    }

    /*
        BEGIN:  Hero Special Abilities
    */
    for(itr = 0; itr < 23; itr++)
    {
        ability_value = ST_UNDEFINED;

        ability_has_plus = ST_TRUE;

        hero_abilities_flags = (_HEROES2[unit_owner]->heroes[unit_type].abilities & USW_HeroAbilities[itr].flags);

        if(hero_abilities_flags != 0)
        {
            hero_ability_flag = hero_abilities_flags;
            switch(hero_ability_flag)
            {
                    case HSA_AGILITY:
                    {
                        strcpy(specials_list->names[specials_index], str_Agility__ovr102);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_LEADERSHIP:
                    {
                        strcpy(specials_list->names[specials_index], str_Leadership__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 3);
                    } break;
                    case HSA_LEADERSHIP2:
                    {
                        strcpy(specials_list->names[specials_index], str_Leadership__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 2);
                    } break;
                    case HSA_LEGENDARY:
                    {
                        strcpy(specials_list->names[specials_index], str_Legendary__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 3);
                    } break;
                    case HSA_LEGENDARY2:
                    {
                        strcpy(specials_list->names[specials_index], str_Legendary__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = (((_UNITS[unit_idx].Level + 1) * 9) / 2);
                    } break;
                    case HSA_BLADEMASTER:
                    {
                        strcpy(specials_list->names[specials_index], str_Blademaster__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 2);
                    } break;
                    case HSA_BLADEMASTER2:
                    {
                        strcpy(specials_list->names[specials_index], str_Blademaster__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = (((_UNITS[unit_idx].Level + 1) * 3) / 4);
                    } break;
                    case HSA_ARMSMASTER:
                    {
                        strcpy(specials_list->names[specials_index], str_Armsmaster__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 2);
                    } break;
                    case HSA_ARMSMASTER2:
                    {
                        strcpy(specials_list->names[specials_index], str_Armsmaster__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_CONSTITUTION:
                    {
                        strcpy(specials_list->names[specials_index], str_Constitution__ovr102);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_CONSTITUTION2:
                    {
                        strcpy(specials_list->names[specials_index], str_Constitution__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_MIGHT:
                    {
                        strcpy(specials_list->names[specials_index], str_Might__ovr102);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_MIGHT2:
                    {
                        strcpy(specials_list->names[specials_index], str_Might__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_ARCANE_POWER:
                    {
                        strcpy(specials_list->names[specials_index], str_ArcanePower__ovr102);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_ARCANE_POWER2:
                    {
                        strcpy(specials_list->names[specials_index], str_ArcanePower__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_SAGE:
                    {
                        strcpy(specials_list->names[specials_index], str_Sage__ovr102);
                        strcat(specials_list->names[specials_index], str_SPACE__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                        strcat(specials_list->names[specials_index], str_SPACE_rp__ovr102);
                        ability_has_plus = ST_FALSE;
                    } break;
                    case HSA_SAGE2:
                    {
                        strcpy(specials_list->names[specials_index], str_Sage__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        strcat(specials_list->names[specials_index], str_SPACE__ovr102);
                        ability_value = (((_UNITS[unit_idx].Level + 1) * 9) / 2);
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                        strcat(specials_list->names[specials_index], str_SPACE_rp__ovr102);
                        ability_has_plus = ST_FALSE;
                    } break;
                    case HSA_PRAYERMASTER:
                    {
                        strcpy(specials_list->names[specials_index], str_Prayermaster__ovr102);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_PRAYERMASTER2:
                    {
                        strcpy(specials_list->names[specials_index], str_Prayermaster__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_AGILITY2:
                    {
                        strcpy(specials_list->names[specials_index], str_Agility__ovr102);
                        strcat(specials_list->names[specials_index], str_CloseBrace__ovr102);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    // DNE in Dasm  case HSA_LUCKY:
                    // DNE in Dasm  {
                    // DNE in Dasm      strcpy(specials_list->names[specials_index], str_Lucky__ovr102);
                    // DNE in Dasm      ability_value = -1;
                    // DNE in Dasm  } break;
                    // DNE in Dasm  case HSA_CHARMED:
                    // DNE in Dasm  {
                    // DNE in Dasm      strcpy(specials_list->names[specials_index], str_Charmed__ovr102);
                    // DNE in Dasm      ability_value = -1;
                    // DNE in Dasm  } break;
                    // DNE in Dasm  case HSA_NOBLE:
                    // DNE in Dasm  {
                    // DNE in Dasm      strcpy(specials_list->names[specials_index], str_Noble__ovr102);
                    // DNE in Dasm      ability_value = -1;
                    // DNE in Dasm  } break;
            }

            // if((_UNITS[unit_idx].Level >= 0) && (ability_value > 0))
            if(ability_value > 0)
            {
                if(ability_has_plus == ST_TRUE)
                {
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], str_SPACE_Plus__ovr102);
                    strcat(specials_list->names[specials_index], temp_string);
                }
                specials_list->flags[specials_index] = ST_UNDEFINED;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_HeroAbilities[itr].icon_idx];
                specials_list->helps[specials_index] = USW_HeroAbilities[itr].help_idx;
                specials_index++;
            }

        }
    }
    /*
        END:  Hero Special Abilities
    */


    /*
        BEGIN:  Spell Caster
    */
    {
        if(_HEROES2[unit_owner]->heroes[unit_type].Casting_Skill > 0)
        {
            strcpy(specials_list->names[specials_index], str_Caster__ovr102);
            strcat(specials_list->names[specials_index], str_SPACE__ovr102);
                
            if(global_battle_unit->mana_max > global_battle_unit->mana)
            {
                    ability_value = global_battle_unit->mana;
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], temp_string);
                    strcat(specials_list->names[specials_index], str_SPACE_OpenParen__ovr102);
                    ability_value = global_battle_unit->mana_max;
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], temp_string);
                    strcat(specials_list->names[specials_index], str_CloseParen__ovr102);
            }
            else
            {
                ability_value = global_battle_unit->mana_max;
                itoa(ability_value, temp_string, 10);
                strcat(specials_list->names[specials_index], temp_string);
            }
            strcat(specials_list->names[specials_index], str_SPACE_mp__ovr102);
            specials_list->flags[specials_index] = ST_UNDEFINED;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_CASTER];
            specials_list->helps[specials_index] = HLP_CASTER;
            specials_index++;
            }
    }
    /*
        END:  Spell Caster
    */

   *specials_count = specials_index;
}
