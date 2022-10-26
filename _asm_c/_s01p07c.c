// _s01p07c.c GAME_LoadTerrstat
// MGC_DEF.H

#include "MGC_DEF.H"
#include "ST_SA.H"


void GAME_Load_TERRSTAT_0(void)
{
    int LbxEntryIndex = 0;
    int RecFirst = 0;
    int RecCount = 770;
    int RecSize = 6;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_TERRSTAT_0()\n", __FILE__, __LINE__);
#endif

    gfp_TBL_Moves_Per_Tile = SA_MK_FP0(LBXR_LoadSingle(g_LbxNm_TERRSTAT, LbxEntryIndex, RecFirst, RecCount, RecSize));

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: GAME_Load_TERRSTAT_0()\n", __FILE__, __LINE__);
#endif
}
