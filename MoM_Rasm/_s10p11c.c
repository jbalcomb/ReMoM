// _s10p11c.c LBX_Load_Record
// ST_LBX.H
// ~= c&p LBX_Load_Entry()

#include "ST_HEAD.H"
#include "ST_TYPE.H"

#include "ST_EMM.H"
#include "ST_LBX.H"
#include "ST_SA.H"

#include "STU_DBG.H"


SAMB_addr LBX_Load_Record(char *LbxName, int LbxEntry, SAMB_addr SAMB_head, int LoadType, int RecFirst, int RecCount, int RecSize)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    SAMB_addr SAMB_data;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    // TODO(JimBalcomb): fixup the mix of static and unsigned longs
    unsigned int EntryTableOffset;  // LBXENTRYOFFSET()
    unsigned long DataOffset_Begin; // LBXENTRYOFFSET()
    unsigned long DataOffset_End;   // LBXENTRYOFFSET()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    SAMB_addr tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()
    SAMB_ptr pSAMB_head;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_Load_Record(LbxName = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);
#endif

    tmp_LbxName = LbxName;
    tmp_LbxEntry = LbxEntry;

    LBXTESTNEGATIVEENTRY()

    LBXHEADER()

    ExtractFileBase(LbxName);

    SAMB_data = EMM_LBX_Load_Record(tmp_LbxName, tmp_LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

    /*
        BEGIN: LBX Format Type / Header Offset
    */
    tmp_LbxHdrFmt = 0;
    /*
        END: LBX Format Type / Header Offset
    */

    if ( SAMB_data == ST_FAILURE )
    {
        HERE("LBX not in EMM");

        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
//            printf("DEBUG: [%s, %d]: Curr. != Prev.\n", __FILE__, __LINE__);

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
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }
            else
            {
                strcpy(tmp_LbxFilePathName, UU_g_LBX_FilePath );
                strcat(tmp_LbxFilePathName, tmp_LbxFileName);
                g_LBX_FileHandle = lbx_open(tmp_LbxFilePathName);

                if ( g_LBX_FileHandle == ST_FAILURE )
                {
                    LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
                }
            }

            // TODO(JimBalcom): this is wierd and not in LBX_Load_Entry
            if ( g_LBX_FileHandle == 0 )
            {
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }

            /*
                BEGIN: LBX Header Offset
            */
            tmp_LbxHdrOfst = 0;
            /*
                END: LBX Header Offset
            */

            if ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )
            {
                LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry, NULL);  /* LBXErr_corrupted */
            }

            HERE("lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);");
            lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
            {
                LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry, NULL);  /* LBXErr_bad_header */
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
            
            /*
                END: Current != Previous
            */
        }
        else
        {
//            printf("DEBUG: [%s, %d]: Curr. == Prev.\n", __FILE__, __LINE__);
        }
        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry, NULL); /* LBXErr_entries_exceeded */
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
            BEGIN: Records - Offset Begin, Size
        */
        LBXRECORDOFFSET()
        /*
            END: Records - Offset Begin, Size
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
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_Load_Record(LbxName = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize), SAMB_data;
#endif
    return SAMB_data;
}
