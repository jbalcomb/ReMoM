/*
    Items

    WIZARDS.EXE
        ovr109
*/

#include "MoX.H"

// WZD o109p01
// drake178: WIZ_AwardItems()
// WIZ_AwardItems()


// WZD o109p02
// drake178: ITEM_DeleteValue()
void ITEM_DeleteValue(int16_t item_idx)
{
    TBL_Items[item_idx].Mana_Value = 0;
}


// WZD o109p03
// drake178: AI_ProcessItems()
// AI_ProcessItems()

// WZD o109p04
// drake178: AI_EquipHeroFirst()
// AI_EquipHeroFirst()

// WZD o109p05
// drake178: AI_EquipRemainder()
// AI_EquipRemainder()
