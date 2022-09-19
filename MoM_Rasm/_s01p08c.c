// _s01p08c.c GAME_LoadSpellData
// MGC_DEF.H

#include "MGC_DEF.H"
#include "ST_SA.H"


void GAME_Load_SPELLDAT_0(void)
{
    int LbxEntryIndex = 0;
    int RecFirst = 0;
    int RecCount = 215;
    int RecSize = 36;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_SPELLDAT_0()\n", __FILE__, __LINE__);
#endif

    gfp_TBL_Moves_Per_Tile = SA_MK_FP0(LBXR_LoadSingle(g_LbxNm_SPELLDAT, LbxEntryIndex, RecFirst, RecCount, RecSize));

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_SPELLDAT_0()\n", __FILE__, __LINE__);
#endif
}
