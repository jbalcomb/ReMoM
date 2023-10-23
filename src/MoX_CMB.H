/*
    WIZARDS.EXE
        ovr116
*/

#ifndef MOX_CMB_H
#define MOX_CMB_H



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
// drake178: UNIT_BU_ApplyItems()
/*
    drake178:
    converts item powers into attack and enchantment flags,
      applying them to the battle unit along with any stat boosts from items
      with the flags being zeroed instead if no effects apply or the unit is not a hero
    returns the item unit enchantment flags
*/
uint32_t UNIT_BU_ApplyItems(int16_t unit_idx, struct s_BU_REC * BU_Rec);

// WZD o116p02
// drake178: BU_Item_To_UEFlags()
/*
    ¿ "Spell-Specific Artifact Enchantments" ?

*/
void BU_Item_To_UEFlags(int16_t item_idx, struct s_BU_REC * BU_Rec);

// WZD o116p03

// WZD o116p04
int16_t UNIT_GetHitsPerFig(int16_t unit_idx);

// WZD o116p05
// WZD o116p06
// WZD o116p07
// WZD o116p08
// WZD o116p09
// WZD o116p10
// WZD o116p11
// WZD o116p12



#ifdef __cplusplus
}
#endif


#endif  /* MOX_CMB_H */
