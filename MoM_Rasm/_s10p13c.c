// _s10p13c.c LBX_GetEntryData
// ST_LBX.H
// ~= c&p EMM_LBX_Load_Entry()

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_LBX.H"
#include "ST_SA.H"


unsigned int LBX_GetEntryData(char *LbxName, int LbxEntry, unsigned long *LbxEntryOffset, unsigned long *LbxEntrySize, int LbxHdrFmt)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    SAMB_addr SAMB_data;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    unsigned int EntryTableOffset;
    unsigned long LbxEntryOffset_End;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_GetEntryData(LbxName = %s, LbxEntry = %d, *LbxEntryOffset, *LbxEntrySize, FormatType = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, LbxHdrFmt);
#endif

    tmp_LbxEntry = LbxEntry;
    tmp_LbxName = LbxName;
    
    LBXTESTNEGATIVEENTRY()

    LBXHEADER()

    ExtractFileBase(LbxName);

    LBXHEADEROFFSET()


        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
            dlvfprintf("DEBUG: [%s, %d]: Curr. != Prev.\n", __FILE__, __LINE__);

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
            dlvfprintf("DEBUG: [%s, %d]: Curr. == Prev.\n", __FILE__, __LINE__);
        }
        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry ); /* LBXErr_entries_exceeded */
        }
        /*
            END: Current vs. Previous
        */

    EntryTableOffset = ((4 * LbxEntry) + 8);
    *LbxEntryOffset = farpeekdw(gsa_LBX_Header, EntryTableOffset);
    LbxEntryOffset_End = farpeekdw(gsa_LBX_Header, EntryTableOffset + 4);
    *LbxEntrySize = LbxEntryOffset_End - *LbxEntrySize;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: LBX_GetEntryData(LbxName = %s, LbxEntry = %d, *LbxEntryOffset, *LbxEntrySize, FormatType = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, LbxHdrFmt);
#endif

    return g_LBX_FileHandle;
}
