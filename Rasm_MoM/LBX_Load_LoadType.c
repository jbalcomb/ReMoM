
#include "ST_SA.H"
#include "STU_TYPE.H"

/*
    Get 'Segment Address', given the 'Load Type'

*/
/*
    Load Type:
        0: Single / Stand-Alone / Head-less
        1: Replace
        2: Append
*/
/*
    varTmpSAMBdata = varSAMBdata;
    ...
    varTmpSAMBdata = LBX_Load_LoadType(argSAMBhead, argLoadType)

    depends on ...
        passed in argSAMBhead and argLoadType
        calculated varEntrySizeParas
*/

sgmt_addr LBX_Load_LoadType(sgmt_addr argSAMBhead, int argLoadType, unsigned long argEntrySizeParas;)
{
    sgmt_addr varSAMBdata;
    unsigned int tmpSAMB_Size;
    
    if ( argLoadType == 0 )
    {
        varSAMBdata = SA_Allocate_MemBlk(argEntrySizeParas);
    }
    if ( (argLoadType == 1) || (argLoadType == 2) )
    {
        if ( argLoadType == 1 )
        {
            // SA_Alloc_Validate(argSAMBhead);
            // Test SAMB Size                   // if ( varEntrySizeParas > (farpeekw(SAMBhead, SAMB_SIZE) - 1) )
            varSAMBdata = argSAMBhead + 1;
            tmpSAMB_Size = argEntrySizeParas + 1;
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
            tmpSAMB_Size = argEntrySizeParas + farpeekw(argSAMBhead, SAMB_USED);
        }
        farpokew(argSAMBhead, SAMB_USED, tmpSAMB_Size);
    }

    return varSAMBdata;
}
