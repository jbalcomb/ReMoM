// _s13p04c.c EMM_Load_LBX
// ST_EMM.H

// TODO(JimBalcomb): figure out why signed-long for LbxFielSize gets made negative, like signed-extended cast to int

#include "MOM_DEF.H"
#include "ST_EMM.H"

int EMM_Load_LBX_File(char *LbxFileName, int Reserved)
{
//     char LBX_FileName[20];
//     long LBX_FileSize;
//     strcpy(LBX_FileName, "MAINSCRN.LBX");
//     LBX_FileSize = DOS_GetFileSize(LBX_FileName);
//     printf("DEBUG: [%s, %d]: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
//     printf("DEBUG: [%s, %d]: g_LbxFileSize: %ld (0x%08X)\n", __FILE__, __LINE__, g_LbxFileSize, g_LbxFileSize);
//     if (LBX_FileSize != 196511 )
//     {
//         printf("DEBUG: [%s, %d]: FAILURE: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
//     }
//     else
//     {
//         printf("DEBUG: [%s, %d]: SUCCESS: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
//     }

    //long LbxFileSize;
    //unsigned long LbxFileSize;
    long LbxFileSize;
    long tmp_LbxFileSize;
    char EmmHndlNm[20];
    char EmmHndlFileName[20];
    long UU_varNbytesRead;
    int EmmLogicalPage;
    int ReadNbytes;
    unsigned int tmp_EmmPageFrameSgmtAddr;
    long LbxFileSize16kBlocks;
    int Handle_Exists;
    int EmmHndlNbr;
    int LbxFileHandle;

    int itr_find_zero;
    int itr_pad_zeros;
    int itr_g_EMM_Open_Handles;
    int tmp_EMM_Pages_Available;
    int tmp_EMM_Pages_Required;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Load_LBX_File(LbxFileName=%s, EmmRsvd=%d)\n", __FILE__, __LINE__, LbxFileName, Reserved);
#endif

    ExtractFileBase(LbxFileName);
    strcpy(EmmHndlNm, LbxFileName);

    // for ( ; argc > 0 && **argv == '-'; argc--, argv++)
    // while( *dest++ = *source++ );
    // for ( itr_find_zero = 0; *(EmmHndlNm + itr_find_zero) != '\0'; itr_find_zero++ )
    itr_find_zero = 0;
    while( EmmHndlNm[itr_find_zero] != '\0' )
    {
        itr_find_zero++;
    }

//    printf("DEBUG: [%s, %d]: itr_find_zero = %d)\n", __FILE__, __LINE__, itr_find_zero);

    for ( itr_pad_zeros = itr_find_zero; itr_pad_zeros < 9; itr_pad_zeros++ )
    {
        *(EmmHndlNm + itr_pad_zeros) = '\0';
    }

//    printf("DEBUG: [%s, %d]: itr_pad_zeros = %d)\n", __FILE__, __LINE__, itr_pad_zeros);

//    printf("DEBUG: [%s, %d]: EmmHndlNm = %s)\n", __FILE__, __LINE__, EmmHndlNm);

    Handle_Exists = 0;

    itr_g_EMM_Open_Handles = 0;
    while ( itr_g_EMM_Open_Handles < g_EMM_Open_Handles )
    {
//        printf("DEBUG: [%s, %d]: g_EMM_Table[itr_g_EMM_Open_Handles].eEmmHndlNm = %s)\n", __FILE__, __LINE__, g_EMM_Table[itr_g_EMM_Open_Handles].eEmmHndlNm);

        if ( stricmp(g_EMM_Table[itr_g_EMM_Open_Handles].eEmmHndlNm, EmmHndlNm) == 0 )
        {
            Handle_Exists = 1;
        }

        itr_g_EMM_Open_Handles++;
    }

    if ( Handle_Exists != 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: if ( Handle_Exists != 0 )\n", __FILE__, __LINE__);
        LBX_Error(EmmHndlNm, 0x0B, 0);  // LBXErr_release_first ... ' cannot be reloaded into EMS w/o being first released.'
    }

    if ( g_EMM_OK == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: if ( g_EMM_OK == 0 )\n", __FILE__, __LINE__);
        EmmHndlNbr = 0;
        goto Exit;
    }

    if ( !(g_EMM_Open_Handles < 40) )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: if ( !(g_EMM_Open_Handles < 40) )\n", __FILE__, __LINE__);
        goto Exit;
    }

    strcpy(EmmHndlFileName, EmmHndlNm);
    strcat(EmmHndlFileName, cnst_LBX_Ext2);

    // LbxFileSize = DOS_GetFileSize(EmmHndlFileName);  // TODO(JimBalcomb): maybe spend another 5 hours trying to figure out why this is getting 2-byte sign-extended
    // LbxFileSize = C_GetFileSize(EmmHndlFileName);
    // tmp_LbxFileSize = DOS_GetFileSize(EmmHndlFileName);
    LbxFileSize = DOS_GetFileSize(EmmHndlFileName);
    // if ( tmp_LbxFileSize < 0 )
    // {
    //     printf("DEBUG: [%s, %d]: ERROR: tmp_LbxFileSize is NEGATIVE!! %ld (0x%08X)\n", __FILE__, __LINE__, tmp_LbxFileSize, tmp_LbxFileSize);
    //     //LbxFileSize = ((2^31)*2) + tmp_LbxFileSize
    //     //LbxFileSize = ((2^15)*2) + tmp_LbxFileSize;
    //     LbxFileSize = ((long)(tmp_LbxFileSize & 0xFFFF) << 15);
    // }
    // else
    // {
    //     LbxFileSize = tmp_LbxFileSize;
    // }
//    printf("DEBUG: [%s, %d]: LbxFileSize = %ld (0x%08X)\n", __FILE__, __LINE__, LbxFileSize, LbxFileSize);

    if ( LbxFileSize == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: LbxFileSize = DOS_GetFileSize(EmmHndlFileName)\n", __FILE__, __LINE__);
        goto Exit;
    }

    LbxFileSize16kBlocks = LbxFileSize / 16384; // SZ_16K_B / EMM Page Size
//    printf("DEBUG: [%s, %d]: LbxFileSize16kBlocks = %ld (0x%08X)\n", __FILE__, __LINE__, LbxFileSize16kBlocks, LbxFileSize16kBlocks);

    if ( LbxFileSize % 16384 != 0 )
    {
        LbxFileSize16kBlocks = LbxFileSize16kBlocks + 1;
    }

    if ( Reserved == 0 )
    {
        // TODO(JimBalcomb): double-check and fix this error here ...tmp_EMM_Pages_Available...if ( !(tmp_EMM_Pages_Required < tmp_EMM_Pages_Required) )
        tmp_EMM_Pages_Available = EMM_GetFreePageCount();
        tmp_EMM_Pages_Required = g_EMM_Pages_Reserved + LbxFileSize16kBlocks;
        if ( !(tmp_EMM_Pages_Required < tmp_EMM_Pages_Required) )
        {
//            printf("DEBUG: [%s, %d]: FAILURE: !(tmp_EMM_Pages_Required < tmp_EMM_Pages_Required)\n", __FILE__, __LINE__, tmp_EMM_Pages_Required, tmp_EMM_Pages_Required);
            EmmHndlNbr = 0;
            goto Exit;
        }
    }

   EmmHndlNbr = EMM_GetHandle(LbxFileSize16kBlocks, EmmHndlNm, Reserved);

   if ( EmmHndlNbr == 0 )
   {
//       printf("DEBUG: [%s, %d]: FAILURE: EmmHndlNbr = EMM_GetHandle(LbxFileSize16kBlocks, EmmHndlNm, Reserved)\n", __FILE__, __LINE__);
       goto Exit;
   }

    tmp_EmmPageFrameSgmtAddr = EMM_GetPageFrame();

    EmmLogicalPage = 0;

    if ( tmp_EmmPageFrameSgmtAddr == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: tmp_EmmPageFrameSgmtAddr = EMM_GetPageFrame()\n", __FILE__, __LINE__);
        goto Exit;
    }

    LbxFileHandle = lbx_open(EmmHndlFileName);

    if ( LbxFileHandle == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: LbxFileHandle = lbx_open(EmmHndlFileName)\n", __FILE__, __LINE__);
        goto Exit;
    }

    UU_varNbytesRead = 0;

    ReadNbytes = EMS_PAGE_SIZE; // 16384  SZ_16K_B / EMS/EMM Page Size
    tmp_LbxFileSize = LbxFileSize;

    while ( tmp_LbxFileSize >= EMS_PAGE_SIZE )
    {
        tmp_LbxFileSize -= EMS_PAGE_SIZE; // SZ_16K_B / EMM Page Size

        EMM_Map4(EmmHndlNbr, EmmLogicalPage);

        // printf("DEBUG: [%s, %d]: lbx_read_sgmt(0x%04X, %d, %d)\n", __FILE__, __LINE__, tmp_EmmPageFrameSgmtAddr, ReadNbytes, LbxFileHandle);
        lbx_read_sgmt(tmp_EmmPageFrameSgmtAddr, ReadNbytes, LbxFileHandle);

        EmmLogicalPage++;

        UU_varNbytesRead += EMS_PAGE_SIZE;
    }
    if ( tmp_LbxFileSize > 0 )
    {
        ReadNbytes = tmp_LbxFileSize;
        EMM_Map4(EmmHndlNbr, EmmLogicalPage);
        lbx_read_sgmt(tmp_EmmPageFrameSgmtAddr, ReadNbytes, LbxFileHandle);
    }

    lbx_close(LbxFileHandle);

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
//    dlvfprintf("DEBUG: [%s, %d] g_EMM_PageFrame_Base_Address: 0x%04X\n", __FILE__, __LINE__, g_EMM_PageFrame_Base_Address);
//    dlvfprintf("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(g_EMM_PageFrame_Base_Address, 0));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(g_EMM_PageFrame_Base_Address, 2));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(g_EMM_PageFrame_Base_Address, 4));
//    dlvfprintf("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(g_EMM_PageFrame_Base_Address, 6));
//#endif

Exit:

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Load_LBX_File(LbxFileName=%s, EmmRsvd=%d)\n", __FILE__, __LINE__, LbxFileName, Reserved);
#endif

    return EmmHndlNbr;
}
