/*
    Lair

    WIZARDS.EXE
        ovr083

BACKGRND.LBX
    LCONFIRM    lair confirm borde
    LCONFIRM    lair bottom w/butt
    LCONFIRM    lair bottom wo/but

NOTE: IDGI, but the FanDom MoM Wiki uses the term "Encounter Zone"
*/

#ifndef LAIR_H
#define LAIR_H

#include "MoM.H"



// WZD dseg:4AB4
extern char * TBL_EZ_Names[];



#ifdef __cplusplus
extern "C" {
#endif



// WZD o83p01
int16_t Square_Has_Lair(int16_t world_x, int16_t world_y, int16_t map_plane);

// WZD o83p02
void Lair_Clear(int16_t lair_idx);

// WZD o83p03
int16_t UU_Player_Maybe_Lair_Combat(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp);

// WZD o83p04
int16_t Lair_Combat__WIP(int16_t lair_idx, int16_t player_idx);

// WZD o83p05
int16_t Do_Lair_Confirm(int16_t lair_idx);

// WZD o83p06
/*

; composes and displays the treasure notification dialog from the passed parameters
; BUG: gold or mana rewards of exactly 1 won't be included


lea     ax, [bp+Reward_Specials]
push    ax                              ; Spec@
push    [bp+Reward_Special_Count]       ; SpCnt
push    [bp+Total_Rewards]              ; Count
push    [bp+Hero_Slot]                  ; HSlot
push    [bp+Reward_Spell]               ; Spell
lea     ax, [bp+item_list]
push    ax                              ; Items@
push    [bp+Dlg_Window_Top]             ; WndTop
push    _SI_lair_idx                    ; Zone_Index
nop
push    cs
Lair_Treasure_Popup()

*/
void Lair_Treasure_Popup(int16_t lair_idx, int16_t window_y, int16_t item_list[], int16_t spell, int16_t hero_slot, int16_t Total_Rewards, int16_t Reward_Special_Count, int16_t Reward_Specials[]);

// WZD o83p07
void Lair_Loot_Gold_And_Mana(int16_t lair_idx, int16_t player_idx);

// WZD o83p08
void Add_Comma_Or_And(int16_t * size, int16_t count, char * list);

// WZD o83p09
char * STR_GetIndefinite(char * word);

// WZD o83p10
void Lair_Generate_Treasure(int16_t player_idx, int16_t lair_idx, int16_t items[], int16_t * spell, int16_t * hero, int16_t * hero_slot, int16_t * total_rewards, int16_t * reward_special_count, int16_t reward_specials[]);

// WZD o83p11
int16_t Lair_Confirm(int16_t UU_lair_confirm_start_x, int16_t UU_lair_confirm_start_y, char * lair_message_box_text, SAMB_ptr lair_confirm_pict, int16_t UU_lair_confirm_pict_x, int16_t UU_lair_confirm_pict_y, int16_t lair_has_guardians, int16_t lair_is_node);

// WZD o83p12
void Lair_Confirm_Draw(void);

// WZD o83p13
void EZ_SpecialTreasure(int16_t player_idx, int16_t lair_idx, int16_t points, int16_t * reward_special_count, int16_t reward_specials[]);

// WZD o83p14
int16_t Lair_Magic_Realm(int16_t lair_idx);



#ifdef __cplusplus
}
#endif

#endif  /* LAIR_H */
