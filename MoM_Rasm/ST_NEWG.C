
#include "ST_TYPE.H"    /* SAMB_ptr */
#include "ST_DEF.H"     /* MK_FP() */

#include "MOM_DEF.H"    /* gfp_TBL_Moves_Per_Tile, gfp_TBL_Spell_Data */

#include "ST_LBX.H"     /* LBXR_LoadSingle() */


// MOM_DEF.C  void * gfp_TBL_Moves_Per_Tile;      // dseg:5BAC
// MOM_DEF.C  void * gfp_TBL_Spell_Data;          // dseg:662C
// MOM_DEF.H  extern void * gfp_TBL_Moves_Per_Tile;     // dseg:5BAC
// MOM_DEF.H  extern void * gfp_TBL_Spell_Data;         // dseg:662C


// s01p07
void GAME_Load_TERRSTAT_0(void)
{
    int LbxEntryIndex = 0;
    int RecFirst = 0;
    int RecCount = 770;
    int RecSize = 6;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_TERRSTAT_0()\n", __FILE__, __LINE__);
// #endif

    gfp_TBL_Moves_Per_Tile = (SAMB_ptr) MK_FP(LBXR_LoadSingle(g_LbxNm_TERRSTAT, LbxEntryIndex, RecFirst, RecCount, RecSize),0);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: GAME_Load_TERRSTAT_0()\n", __FILE__, __LINE__);
// #endif
}

// s01p08
void GAME_Load_SPELLDAT_0(void)
{
    int LbxEntryIndex = 0;
    int RecFirst = 0;
    int RecCount = 215;
    int RecSize = 36;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_SPELLDAT_0()\n", __FILE__, __LINE__);
// #endif

    gfp_TBL_Spell_Data = (SAMB_ptr) MK_FP(LBXR_LoadSingle(g_LbxNm_SPELLDAT, LbxEntryIndex, RecFirst, RecCount, RecSize),0);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_SPELLDAT_0()\n", __FILE__, __LINE__);
// #endif
}

