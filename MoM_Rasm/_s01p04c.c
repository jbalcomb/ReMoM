// _s01p04c.c GAME_LoadMainImages
// MGC_DEF.H

#include "MGC_DEF.H"
#include "ST_EMM.H"

#include "STU_DBG.H"


void GAME_LoadMainImages(void)
{

    int itr_wizards;

//    printf("DEBUG: [%s, %d]: BEGIN: GAME_LoadMainImages()\n", __FILE__, __LINE__);

    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);

    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
#ifdef DEBUG
    DBG_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
    dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_000: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_000);
#endif
    gsa_VORTEX_1_MenuContinue = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);

    gsa_VORTEX_2_MenuHallOfFame = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);

    gsa_VORTEX_5_MenuLoadGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);

    gsa_VORTEX_4_MenuNewGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);

    gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);

    gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
#ifdef DEBUG
    DBG_MAINSCRN_005 = gsa_MAINSCRN_5_ScreenBottom;
    dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_005: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_005);
#endif
    EMM_Load_LBX_File_1(g_LbxNm_WIZARDS);
    
    for ( itr_wizards = 0; itr_wizards < 14; itr_wizards++ )
    {
        gsa_WIZARDS_0to13[itr_wizards] = LBXE_LoadSingle(g_LbxNm_WIZARDS, itr_wizards);
    }

    EMM_Load_LBX_File_1(g_LbxNm_SPELLDAT);

//    printf("DEBUG: [%s, %d]: END: GAME_LoadMainImages()\n", __FILE__, __LINE__);
}
