/*
    Items

    WIZARDS.EXE
        ovr109
*/

#include "MoX/MoX_DEF.H"
#include "MoX/MoX_TYPE.H"

#include "Items.H"
#include "ItemScrn.H"
#include "MainScr.H"
#include "MOM_SCR.H"
#include "SBookScr.H"



// WZD dseg:5AEA                                                 BEGIN:  ovr109

// WZD dseg:5AEA
char cnst_MESSAGE_File[] = "message";

// WZD dseg:5AEA                                                 END:  ovr109





// WZD o109p01
/*
; processes the passed list of items being awarded to
; the selected wizard - either through the items screen
; if it's the human player, or AI_Process_Item_Pool__STUB otherwise
; has some BUGs in both branches

hard-coded to 'Main Screen'

Combat()
    |-> Player_Process_Item_Pool(combat_attacker_player_idx, Item_Count, &Item_List[0])

*/
void Player_Process_Item_Pool(int16_t player_idx, int16_t item_count, int16_t item_list[])
{
    if(item_count > 0)
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {
            Allocate_Reduced_Map();
            Full_Draw_Main_Screen();
            LBX_Load_Data_Static(cnst_MESSAGE_File, 0, (SAMB_ptr)GUI_NearMsgString, 67, 1, 150);
            Warn0(GUI_NearMsgString);
            /* TODO */  OVL_MosaicFlip__STUB();

            Process_Item_Pool(item_count, item_list);

            current_screen = scr_Main_Screen;
            /* TODO */  OVL_MosaicFlip__STUB();
        }
        else
        {
            // TODO:  AI_Process_Item_Pool__STUB(player_idx, item_count, item_list);
        }
    }
}


// WZD o109p02
/*
    sets 'cost' to 0
    NOTE: Destroy_Item() sets cost to -1

    ¿ 'cost' is (Base) 'Cost' ?
    ¿ what does it mean for this to be 0 ?
*/
void Remove_Item(int16_t item_idx)
{
    _ITEMS[item_idx].cost = 0;
}


// WZD o109p03
// drake178: AI_ProcessItems()
// AI_Process_Item_Pool__STUB()
void AI_Process_Item_Pool__STUB(int16_t player_idx, int16_t item_count, int16_t item_list[])
{

}


// WZD o109p04
// drake178: AI_EquipHeroFirst()
// AI_EquipHeroFirst()

// WZD o109p05
// drake178: AI_EquipRemainder()
// AI_EquipRemainder()
