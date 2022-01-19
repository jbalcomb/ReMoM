// _s10p10c.c LBX_Load_Entry
// ST_LBX.H

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_LBX.H"
#include "ST_SA.H"

#include "STU_DBG.H"


unsigned int LBX_Load_Entry(char *LbxName, int LbxEntry, sgmt_addr SAMB_head, int LoadType, int LbxHdrFmt)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    sgmt_addr SAMB_data;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    unsigned int EntryTableOffset;  // LBXENTRYOFFSET()
    unsigned long DataOffset_Begin; // LBXENTRYOFFSET()
    unsigned long DataOffset_End;   // LBXENTRYOFFSET()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    sgmt_addr tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_Load_Entry(LbxName = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, FormatType = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt);
#endif

    if ( (strcmp(LbxName, "FONTS.LBX") == 0) && (LbxEntry == 2) )
    {
        dlvfprintf("DEBUG: [%s, %d] BEGIN: LBX_Load_Entry(LbxName=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, LbxHdrFmt=%d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt);
    }
    if ( (strcmp(LbxName, "MAINSCRN") == 0) && (LbxEntry == 0) )
    {
        dlvfprintf("DEBUG: [%s, %d] BEGIN: LBX_Load_Entry(LbxName=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, LbxHdrFmt=%d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt);
    }

    tmp_LbxEntry = LbxEntry;
    tmp_LbxName = LbxName;
    
    LBXTESTNEGATIVEENTRY()

    LBXHEADER()

    ExtractFileBase(LbxName);

    LBXHEADEROFFSET()

    SAMB_data = EMM_LBX_Load_Entry(LbxName, LbxEntry, SAMB_head, LoadType, tmp_LbxHdrFmt);

    if ( SAMB_data == ST_FAILURE )
    {
        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
            HERE("Curr. != Prev.");

            UU_g_LBX_HdrFmt = tmp_LbxHdrFmt;
            
            if ( g_LBX_FileHandle != -1 )
            {
                lbx_close(g_LBX_FileHandle);
            }

            strcpy(g_LBX_Name, LbxName);
            strcpy(tmp_LbxFileName, LbxName);
            strcat(tmp_LbxFileName, g_LBX_FileExtension);
            g_LBX_FileHandle = lbx_open(tmp_LbxFileName);
           
            if ( (g_LBX_FileHandle == ST_FAILURE) && (UU_g_LBX_FilePath == ST_NULL) )
            {
                LBX_Error(LbxName, 0x01, LbxEntry);  /* LBXErr_not_found */
            }
            else
            {
                strcpy(tmp_LbxFilePathName, UU_g_LBX_FilePath );
                strcat(tmp_LbxFilePathName, tmp_LbxFileName);
                g_LBX_FileHandle = lbx_open(tmp_LbxFilePathName);

                if ( g_LBX_FileHandle == ST_FAILURE )
                {
                    LBX_Error(LbxName, 0x01, LbxEntry);  /* LBXErr_not_found */
                }
            }

            /*
                BEGIN: LBX Header Offset
            */
            if ( UU_g_LBX_HdrFmt == 0 )
            {
                tmp_LbxHdrOfst = 0;
            }
            else
            {
                tmp_LbxHdrOfst = 512;
            }
            /*
                END: LBX Header Offset
            */

            if ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )
            {
                LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry);  /* LBXErr_corrupted */
            }

            lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
            {
                LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry);  /* LBXErr_bad_header */
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
            
            /*
                END: Current != Previous
            */
        }
        else
        {
            HERE("Curr. == Prev.");
        }
        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry ); /* LBXErr_entries_exceeded */
        }
        /*
            END: Current vs. Previous
        */

        /*
            BEGIN: Entry - Offset Begin, End, Size
        */
        LBXENTRYOFFSET()
        /*
            END: Entry - Offset Begin, End, Size
        */

        /*
            BEGIN: Load Type
        */
        DataSize_Paras = 1 + (DataSize_Bytes / SZ_PARAGRAPH_B);
        LBXLOADTYPE()
        /*
            END: Load Type
        */
        tmp_SAMB_data = SAMB_data;
        /*
            BEGIN: Read Data
        */
        LBXREADDATA()
        /*
            END: Read Data
        */

    }  /* if ( SAMB_data == 0 ) */

    Update_MemFreeWorst_KB();

#ifdef DEBUG    
    dlvfprintf("DEBUG: [%s, %d]: END: LBX_Load_Entry(LbxName = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, FormatType = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt, SAMB_data);
#endif
    return SAMB_data;
}
