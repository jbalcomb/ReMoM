

Combat (Tactical)
Combat Information Window
Info Button





CMB_ShowInfoWindow__WIP()
    |-> Combat_Info_Effects()
        |-> Combat_Info_Effects_Base()
...
CMB_DrawInfoWindow__WIP()





CMB_SetCommonXFX()
    handles
        node dispel - Sorcery, Chaos, Nature
        node auta - Sorcery, Nature, Chaos
        Cloud of Shadows, Heavenly Light
        Chaos Surge, Eternal Night

CMB_FillInfoArrays__WIP()
    handles
        Crusade
        Holy Arms
        Charm Of Life
        Zombie Mastery

Why two parts?
    Cloud of Shadow     City Enchantment
    Heavenly Light      City Enchantment
    Chaos Surge         Global Enchantment
    Eternal Night       Global Enchantment

...node effects...
...city enchantments and global enchantments that only apply to combat in cities...
...global enchantments that apply regardless of who has cast them...



CMB_Info_CommonCount  ==> _combat_info_item_count
CMB_FillInfoArrays__WIP() ==> Combat_Info_Effects()
CMB_SetCommonXFX__WIP() ==> Combat_Info_Effects_Base()

IMG_CMB_Info_Bottom ==> _combat_info_wnd_bot_seg
IMG_CMB_Info_Middle ==> _combat_info_wnd_mid_seg
IMG_CMB_Info_Box    ==> _combat_info_wnd_box_seg
IMG_CMB_Info_Top    ==> _combat_info_wnd_top_seg
IMG_CMB_InfoIcons   ==> _combat_info_effect_icon_segs
CMB_External_FX@    ==> _combat_info_effects

CMB_ShowInfoWindow__WIP()   ==> Combat_Information_Window()
CMB_DrawInfoWindow__WIP()   ==> Combat_Information_Window_Draw()

CMB_InfoWindow_Left ==> _combat_info_wnd_start_x
CMB_InfoWindow_Top  ==> _combat_info_wnd_start_y

CMB_ExternalFX_Count ==> _combat_total_battle_effect_count

CMB_CountExternalFX__WIP() ==> Combat_Info_Effects_Count()



## _combat_info_item_count





## _combat_total_battle_effect_count

Tactical_Combat__WIP()
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();
    ...
    ...
    ...
    if(_combat_total_battle_effect_count > 0)
        Info_Button_Index = Add_Button_Field(144, 178, cnst_ZeroString_17__ovr090, _cmbt_info_button_seg, cnst_HOTKEY_U_5[0], ST_UNDEFINED);

CMB_DrawFullScreen__WIP()
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();

_combat_total_battle_effect_count = Combat_Info_Effects_Count()



## CMB_PerSideInfo

set in Combat_Info_Effects()

Combat_Info_Effects()
    Combat_Info_Effects_Base()
    _combat_total_battle_effect_count = 0;
    if(itr_combatants == 0)
    {
        computer_player_battle_effect_count = _combat_total_battle_effect_count;
    }
    if((_combat_total_battle_effect_count - computer_player_battle_effect_count) > computer_player_battle_effect_count)
    {
        CMB_PerSideInfo = (_combat_total_battle_effect_count - computer_player_battle_effect_count);
    }
    else
    {
        CMB_PerSideInfo = computer_player_battle_effect_count;
    }
so, ...
    IDGI.
    something like, CMB_PerSideInfo gets set to whichever is more - the computer player's effect count or the human player's

Combat_Info_Effects_Base() sets _combat_info_item_count
...which gets one added manually when it is odd
Combat_Info_Effects()
sets info_common_count to _combat_info_item_count
for each new effect, _combat_total_battle_effect_count += 1; info_common_count += 2;
but, at the end of the loop, it resets info_common_count to (_combat_info_item_count + 1)
so, info_common_count doesn't do anything


DEBUG
PASS    _combat_info_item_count is 2 in Combat_Info_Effects()
PASS    _combat_info_item_count is 2 in Combat_Information_Window_Draw()

IDK_count = (9 + ((_combat_info_item_count / 2) * 19));
is 90
used for Set_Window()
    for Clipped_Draw(_combat_info_wnd_start_x, _combat_info_wnd_start_y, _combat_info_wnd_top_seg);

¿ CMB_PerSideInfo should not be 0 in Combat_Info_Effects() ?



## Combat_Information_Window_Draw()

Draw Calls:
    Clipped_Draw(_combat_info_wnd_start_x, _combat_info_wnd_start_y, _combat_info_wnd_top_seg);
    Clipped_Draw(_combat_info_wnd_start_x, IDK_count, _combat_info_wnd_mid_seg);
    Clipped_Draw(_combat_info_wnd_start_x, Middle_Y, _combat_info_wnd_bot_seg);
    ...each preceded by a call to Set_Window()
    ...top y2, mid start_y
    ...mid y2, bot y1, bot start_y
    FLIC_Draw((Draw_Left - 1), (Draw_Top - 1), _combat_info_wnd_box_seg);
    FLIC_Draw(Draw_Left, Draw_Top, _combat_info_effects[IDK_itr]->Icon_IMG_Seg);
    Print((Draw_Left + 20), (Draw_Top + 5), _combat_info_effects[IDK_itr]->Name);

if(CMB_PerSideInfo > 0)
    if(_combat_info_item_count > 0)
        PerSide_Draw_Top = (15 + ((_combat_info_item_count / 2) * 19));
    else
        PerSide_Draw_Top = 11;
    if(CMB_AI_Player < NEUTRAL_PLAYER_IDX)
        strcpy(GUI_String_1, _players[CMB_AI_Player].name);
        strcat(GUI_String_1, cnst_SpaceSpells);
        Print_Centered((_combat_info_wnd_start_x + 54), (_combat_info_wnd_start_y + PerSide_Draw_Top), GUI_String_1);
    strcpy(GUI_String_1, _players[combat_human_player].name);
    strcat(GUI_String_1, cnst_SpaceSpells);
    Print_Centered((_combat_info_wnd_start_x + 170), (_combat_info_wnd_start_y + PerSide_Draw_Top), GUI_String_1);

if(_combat_info_item_count > 0)
    PerSide_Draw_Top = 10;
else
    PerSide_Draw_Top = 26;


    _combat_info_wnd_start_x = 50;
    _combat_info_wnd_start_y = 10;
















##### Naming Things is Hard



Page 90  (PDF Page 95)  
Active window  


Page 91  (PDF Page 96)  

Underneath the combat grid (the battlefield) are two enchantment windows:  
one on the left for the defender (this can be a neutral city, node, enemy wizard, etc.) and one on the right for the attacker.  
Combat enchantments that are active for either side (such as darkness and true light) are indicated in this window.  

Between the two enchantment windows are three areas: the spell information window, the action window and the active unit window.  

The action window contains a set of six buttons which can be used to direct actions other than movement and attacking.  

The info button provides important information about anything else that may affect the current battle, including overland enchantments (such as Crusade), city enchantments and nodes.  

