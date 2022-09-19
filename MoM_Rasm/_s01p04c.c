// _s01p04c.c GAME_LoadMainImages
// MGC_DEF.H

#include "MGC_DEF.H"

#include "ST_EMM.H"
#include "ST_LBX.H"

//#ifdef DEBUG
#include "STU_DBG.H"
#include "STU_TST.H"
//#endif

/*
    Only MAGIC.EXE?
    Loads MoM/Main Screen background and Buttons as well as New Game - Wizard's Portraits and Spell Data
    
    Load LBX File, into EMM, as 'Reserved'
        MAINSCRN.LBX
        WIZARDS.LBX
        SPELLDAT.LBX
    Load LBX Entry
        MAINSCRN.LBX, 0
        MAINSCRN.LBX, 5
        VORTEX.LBX, 1   "MAINTEXT, "c"
        VORTEX.LBX, 2   "MAINTEXT, "hof"
        VORTEX.LBX, 3   "MAINTEXT, "q"
        VORTEX.LBX, 4   "MAINTEXT, "n"
        VORTEX.LBX, 5   "MAINTEXT, "l"
        WIZARDS.LBX, 0
        WIZARDS.LBX, 1
        WIZARDS.LBX, 2
        WIZARDS.LBX, 3
        WIZARDS.LBX, 4
        WIZARDS.LBX, 5
        WIZARDS.LBX, 6
        WIZARDS.LBX, 7
        WIZARDS.LBX, 8
        WIZARDS.LBX, 9
        WIZARDS.LBX, 10
        WIZARDS.LBX, 11
        WIZARDS.LBX, 12
        WIZARDS.LBX, 13
*/
void GAME_LoadMainImages(void)
{

    int itr_wizards;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif

    HERE("BEFORE: EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);");
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    HERE("AFTER: EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);");

//#ifdef TEST
    HERE("CALL: dl_TST_LBX_MAINSCRN_000();");
    dl_TST_LBX_MAINSCRN_000();

    //g_EMM_Table[TST_LBX_MAINSCRN_000.EMM_Table_Index].eEmmHndlNbr == TST_LBX_MAINSCRN_000.EMM_Handle_Number;

//#endif

    // TODO(JimBalcomb,20220724): add DEBUG - if MAINSCRN LbxFileSize == 196511
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPage: %d\n", __FILE__, __LINE__, EmmLogicalPage);
//    dlvfprintf("DEBUG: [%s, %d] LbxFileSize: %ld\n", __FILE__, __LINE__, LbxFileSize);
//    dlvfprintf("DEBUG: [%s, %d] UU_varNbytesRead: %ld\n", __FILE__, __LINE__, UU_varNbytesRead);
//    dlvfprintf("DEBUG: [%s, %d] tmp_LbxFileSize: %ld\n", __FILE__, __LINE__, tmp_LbxFileSize);
//    dlvfprintf("DEBUG: [%s, %d] UU_varNbytesRead + tmp_LbxFileSize: %ld\n", __FILE__, __LINE__, (UU_varNbytesRead + tmp_LbxFileSize));
//#endif

//#ifdef DEBUG
//    EMM_Map4(EmmHndlNbr, 0);
//    dlvfprintf("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
//    dlvfprintf("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 0));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 2));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 4));
//    dlvfprintf("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 6));
//#endif

    /*
        In-Process
            Load LBX Entry
                LBX (File) Name
                    MAINSCRN.LBX
                LBX Entry Index
                    0
            SAMB Memory Allocation Type / Strategy
                Type 0: standalone block, no header
            Label / Identifier / Variable for Address/Pointer:
                sa_MAINSCRN_000


    */
    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);  // 
#ifdef DEBUG
    DBG_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
    TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_000: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_000);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
#endif
    gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
#ifdef DEBUG
    DBG_MAINSCRN_005 = gsa_MAINSCRN_5_ScreenBottom;
    dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_005: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_005);
#endif

    gsa_VORTEX_1_MenuContinue    = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    gsa_VORTEX_2_MenuHallOfFame  = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    gsa_VORTEX_3_MenuQuitToDOS   = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    gsa_VORTEX_4_MenuNewGame     = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    gsa_VORTEX_5_MenuLoadGame    = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);

    // TODO(JimBalcomb,20220915): figure out where these end up getting used (WZD?) and if that makes it make sense that these are loaded here
    // NOTE(JimBalcomb,20220919): the WIZARDS and SPELLDAT here are used in the 'New Game' code  (SEE: SCRN_Draw_NewScr5_2)
    // gsa_WIZARDS_0to13 is used in SCRN_New_ 1,2,3,4,5_2,6,7
    EMM_Load_LBX_File_1(g_LbxNm_WIZARDS);
    
    for ( itr_wizards = 0; itr_wizards < 14; itr_wizards++ )
    {
        gsa_WIZARDS_0to13[itr_wizards] = LBXE_LoadSingle(g_LbxNm_WIZARDS, itr_wizards);
    }

    EMM_Load_LBX_File_1(g_LbxNm_SPELLDAT);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif

}
