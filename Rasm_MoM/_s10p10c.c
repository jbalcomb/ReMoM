
#include "ST_LBX.H"
#include "ST_SA.H"
#include "STU_TYPE.H"

/*
g_LBX_Header
ExtractFileBase
LbxHdrFmt
EMM cache
Current vs. Previous
...Offset Table, Begin, End, Size, Paras...
LOAD TYPE!!
Read Entry data
*/

unsigned int LBX_EntryLoader(char *argLbxName, int argLbxEntry, sgmt_addr argSAMBhead, int argLoadType, int argFormatType)
{
    sgmt_addr varSAMBdata;
    char varLbxFileName[20];
    unsigned long varLbxHeaderOffset;
    unsigned int varEntryTableOffset;
    unsigned long varEntryOffsetBegin;
    unsigned long varEntryOffsetEnd;
    unsigned long varEntrySizeBytes;
    unsigned long varEntrySizeParas;
    unsigned int tmpSAMB_Used;
    unsigned int tmpSAMB_Size;
    sgmt_addr varTmpSAMBdata;
    unsigned int varReadNbytes;

    printf("DEBUG: BEGIN: LBX_EntryLoader()\n");

    LBXHEADER

    ExtractFileBase(argLbxName);

    // LBX Header Offset

    // EMM_LBX_EntryLoader()

    // Current vs. Previous

    strcpy(g_LBX_Name, argLbxName);
    strcpy(varLbxFileName, argLbxName);
    strcat(varLbxFileName, g_LBX_FileExtension);
    g_LBX_FileHandle = lbx_open(varLbxFileName);
    varLbxHeaderOffset = 0;
    lbx_seek(varLbxHeaderOffset, g_LBX_FileHandle);
    lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);
    // Test LBX Magic / Signature               // farpeekw(gsa_LBX_Header, 2); ... if != LBX_MAGSIG
    // Test LBX Entry Count vs. Entry Index     // g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
    varEntryTableOffset = SZ_LBX_HDR_PRE_B + (SZ_LBX_HDR_OFST_B * argLbxEntry);
    varEntryOffsetBegin = farpeekdw(gsa_LBX_Header, varEntryTableOffset);
    varEntryTableOffset = varEntryTableOffset + SZ_LBX_HDR_OFST_B;
    varEntryOffsetEnd = farpeekdw(gsa_LBX_Header, varEntryTableOffset);

    varEntrySizeBytes = varEntryOffsetEnd - varEntryOffsetBegin;

    lbx_seek(varEntryOffsetBegin, g_LBX_FileHandle);

    varEntrySizeParas = 1 + (varEntrySizeBytes / SZ_PARAGRAPH_B);

    if ( argLoadType == 0 )
    {
        varSAMBdata = SA_Allocate_MemBlk(varEntrySizeParas);
    }
    if ( (argLoadType == 1) || (argLoadType == 2) )
    {
        if ( argLoadType == 1 )
        {
            // SA_Alloc_Validate(argSAMBhead);
            // Test SAMB Size                   // if ( varEntrySizeParas > (farpeekw(SAMBhead, SAMB_SIZE) - 1) )
            varSAMBdata = argSAMBhead + 1;
            tmpSAMB_Size = varEntrySizeParas + 1;
        }
        if ( argLoadType == 2 )
        {
            // SA_Alloc_Validate(argSAMBhead);
            // Test SAMB Free                   // if ( varEntrySizeParas > SA_GetFree(argSAMBhead) )
            // tmpSAMB_Used = farpeekw(argSAMBhead, SAMB_USED);
            // varSAMBdata = argSAMBhead + tmpSAMB_Used;
            // tmpSAMB_Used = farpeekw(argSAMBhead, SAMB_USED);
            // tmpSAMB_Size = varEntrySizeParas + tmpSAMB_Used;
            varSAMBdata = argSAMBhead + farpeekw(argSAMBhead, SAMB_USED);
            tmpSAMB_Size = varEntrySizeParas + farpeekw(argSAMBhead, SAMB_USED);
        }
        farpokew(argSAMBhead, SAMB_USED, tmpSAMB_Size);
    }

    varTmpSAMBdata = varSAMBdata;

    varReadNbytes = SZ_32K_B;

    while ( varEntrySizeBytes >= SZ_32K_B )
    {
        varEntrySizeBytes = varEntrySizeBytes - SZ_32K_B;

        lbx_read_sgmt(varTmpSAMBdata, varReadNbytes, g_LBX_FileHandle);

        varTmpSAMBdata = varTmpSAMBdata + SZ_32K_PR;
    }
    if ( varEntrySizeBytes > 0 )
    {
        varReadNbytes = varEntrySizeBytes;
        lbx_read_sgmt(varTmpSAMBdata, varReadNbytes, g_LBX_FileHandle);
    }

    Update_MemFreeWorst_KB();
    
    printf("DEBUG: END: LBX_EntryLoader()\n");
    return varSAMBdata;
}
