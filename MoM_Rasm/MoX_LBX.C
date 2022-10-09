
#include "MoX_TYPE.H"   /* SAMB_ptr */
#include "MoX_DEF.H"    /* NULL */

#include "MoX_LBX.H"    /* SZ_LBX_HDR_PR */

#include "MoX_SA.H"     /* SAMB_SIZE, SAMB_USED */

#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fseek(); */
// C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\ucrt\STDIO.H
// #include "C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\ucrt\STDIO.H"
#include <STDLIB.H>     /* itoa() */
// #if defined(__DOS16__)
// #include <MALLOC.H>
// #include <STDLIB.H>     /* itoa() */
// #include <STRING.H>     /* strcat(), strcpy(); */
// #endif
// #if defined(__WIN32__)
// #include <malloc.h>
// #include <stdlib.h>     /* itoa() */
// #endif

#ifdef STU_DEBUG
#include "STU_DBG.H"  /* DLOG() */
#endif


extern int RAM_Min_KB;


/*
    LBX Globals - Initialized
*/
unsigned int LBX_Header_Allocated = ST_FALSE;             // MGC dseg:3E66
// unsigned int g_LBX_File_Handle = 0xFFFF;          // MGC dseg:3E68  DOS Unused File Handle  == 0xFFFF (! -1)
FILE * g_LBX_File_Pointer = NULL;
// unsigned int UU_LBX_Header_Format_Override = 0;          // MGC dseg:3E6A
// char UU_LBX_File_Path[50] = {0};                // MGC dseg:3E6C  ; This is not in ORION.
char LBX_File_Extension[] = ".LBX";             // MGC dseg:3E9E

/*
    LBX Globals - Unitialized
*/
// MoX_MoM  unsigned int RAM_Min_KB;                    // MGC dseg:A5C4  ; set to 583 in main()
unsigned int g_LBX_Entry_Count;                    // MGC dseg:A5C6
// unsigned int UU_LBX_Header_Format;              // MGC dseg:A5C8
SAMB_ptr g_LBX_Header;                            // MGC dseg:A5CA
char g_LBX_Name[16];                              // MGC dseg:A5CC
// unsigned int LBX_EMM_Reserved;                  // MGC dseg:A5EC


static void MoX_LBX_Header_Allocate(void);
static void MoX_LBX_Header_Load(char * LBX_Name, int LBX_Index);


SAMB_ptr MoX_Explore_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt)
{
    SAMB_ptr SAMB_data;
    SAMB_data = NULL;
    MoX_LBX_Header_Allocate();
    MoX_LBX_Header_Load(LBX_Name, LBX_Index);


    return SAMB_data;
}

static void MoX_LBX_Header_Allocate(void)
{
    if( LBX_Header_Allocated == ST_FALSE )
    {
        LBX_Header_Allocated = ST_TRUE;
        g_LBX_Header = SA_Allocate_MemBlk(SZ_LBX_HDR_PR);
    }
}
/*
    sets
        g_LBX_Name
        g_LBX_File_Handle
        g_LBX_Header
        g_LBX_Entry_Count
*/
static void MoX_LBX_Header_Load(char * LBX_Name, int LBX_Index)
{
    char LBX_File_Name[20];
    unsigned int LBX_MagSig;

    strcpy(g_LBX_Name, LBX_Name);

    strcat(LBX_File_Name, LBX_File_Extension);
    g_LBX_File_Pointer = fopen(LBX_File_Name, "rb");

    // lbx_read_sgmt(g_LBX_Header, SZ_LBX_HDR_B, g_LBX_File_Handle);
    fread(&g_LBX_Header, 512, 1, g_LBX_File_Pointer);

    // #define GET_LE_16(_ptr_) ( (((unsigned int)(((unsigned char _FAR *)(_ptr_))[1])) << 8) | ((unsigned int)(((unsigned char _FAR *)(_ptr_))[0])) )
    LBX_MagSig = ( (unsigned int) ((unsigned char *)(g_LBX_Header))[3] << 8 ) | ( (unsigned int) ((unsigned char *)(g_LBX_Header))[2] );

    if ( LBX_MagSig != LBX_MAGSIG ) { LBX_Error(LBX_Name, 0x07, LBX_Index, NULL); }

    g_LBX_Entry_Count = ( (unsigned int) ((unsigned char *)(g_LBX_Header))[1] << 8 ) | ( (unsigned int) ((unsigned char *)(g_LBX_Header))[0] );
    // g_LBX_Entry_Count = *((unsigned int *)g_LBX_Header[0]);

}


// s10p01
SAMB_ptr MoX_LBXE_LoadSingle(char * LBX_Name, int LBX_Index)
{
    return MoX_LBX_Load_Entry(LBX_Name, LBX_Index, NULL, 0, 0);
}


// _s10p10
SAMB_ptr MoX_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt)
{
    SAMB_ptr SAMB_data;
    unsigned long DataSize_B;
    unsigned long DataSize_PR;
    SAMB_ptr tmp_SAMB_data;

    SAMB_data = NULL;

    MoX_LBX_Header_Allocate();
    MoX_LBX_Header_Load(LBX_Name, LBX_Index);
    if ( !(LBX_Index < g_LBX_Entry_Count) ) { LBX_Error(LBX_Name, 0x08, LBX_Index, NULL); }

    DataSize_B = 192000;

    /*
        BEGIN: Load Type
    */
    DataSize_PR = 1 + (DataSize_B / SZ_PARAGRAPH_B);  // 
    // LBXLOADTYPE()  // sets SAMB_data and tmp_SAMB_Size
        /* DataSize_Paras is different (only) in EMM_LBX_Load_Entry */
        /* tmp_SAMB_data is different in EMM_LBXR_DirectLoader and EMM_LBX_HdrOnly */
        // #define LBXLOADTYPE() {
            /*DataSize_Paras = 1 + (DataSize_Bytes / SZ_PARAGRAPH_B);*/
    switch(LoadType)
    {
        case 0:
        {
            SAMB_data = SA_Allocate_MemBlk(DataSize_PR);
            if ( SAMB_data == 0 ) { LBX_Error(LBX_Name, 0x03, LBX_Index, NULL); /* LBXErr_low_RAM */ }
            break;
        }
        case 1:
        {
//            if ( SA_Alloc_Validate(SAMB_head) == 0 ) { LBX_Error(LBX_Name, 0x02, LBX_Index, NULL); }
//            // if ( DataSize_PR > (farpeekw(SAMB_head, SAMB_SIZE) - 1) ) { LBX_Error(LBX_Name, 0x04, LBX_Index, (DataSize_PR - farpeekw(SAMB_head, SAMB_SIZE) + 1)); }
//            SAMB_data = SAMB_head + 1;
//            farpokew(SAMB_head, SAMB_USED, (DataSize_PR + 1));
            break;
        }
        case 2:
        {
//            if ( SA_Alloc_Validate(SAMB_head) == 0 ) { LBX_Error(LBX_Name, 0x02, LBX_Index, NULL); }
//            // if ( DataSize_PR > SA_GetFree(pSAMB_head) ) { LBX_Error(LBX_Name, 0x05, LBX_Index, (DataSize_PR - SA_GetFree(pSAMB_head))); }
//            // SAMB_data = SAMB_head + farpeekw(SAMB_head, SAMB_USED);
//            SAMB_data = SAMB_head + ( ( (unsigned int) ((unsigned char *)(SAMB_head))[SAMB_USED+1] << 8 ) | ( (unsigned int) ((unsigned char *)(SAMB_head))[SAMB_USED] ) );
//#define SET_LE_16(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu))
//
//            b[0] = byte(v)
//// 	b[1] = byte(v >> 8)
//
//            // farpokew(SAMB_head, SAMB_USED, (DataSize_PR + farpeekw(SAMB_head, SAMB_USED)));
//            ((uint8_t *)(SAMB_head))[0] = ((_v_) & 0xffu)
//            ((uint8_t *)(SAMB_head))[1] = (((_v_) >> 8) & 0xffu))

            break;
        }
        /*tmp_SAMB_data = SAMB_data;*/
        /*
            END: Load Type
        */
        tmp_SAMB_data = SAMB_data;

}



    return SAMB_data;
}

// s10p15
void LBX_Error(char * LBX_Name, int LBX_Error_Number, int LBX_Index, int pages)
{
    int LBX_Error_Index;
    char str_itoa[20];
    char str_errmsg[120];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LBX_Error( LBX_Name = %s, LBX_Error_Number = %d, LBX_Index = %d, pages = %d)\n", __FILE__, __LINE__, LBX_Name, LBX_Error_Number, LBX_Index, pages);
#endif

#ifdef STU_DEBUG
    // strcpy(str_errmsg, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");  // 120 characters
    strcpy(str_errmsg, "0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_");  // 110 characters
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

    strcpy(str_errmsg, LBX_Name);
    itoa(LBX_Index, str_itoa, 10);
    strcat(str_errmsg, cnst_LBXErr_Common1);        // char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         // MGC dseg:3EA3
    strcat(str_errmsg, str_itoa);
    strcat(str_errmsg, cnst_LBXErr_Common2);        // char *cnst_LBXErr_Common2 = "] ";                                                   // MGC dseg:3EB0
/*
char *cnst_LBX_Error7 = " is not an LBX file";                                      // MGC dseg:3F8B
char *cnst_LBX_Error8 = " exceeds number of LBX entries";                           // MGC dseg:3F9F
char *cnst_LBX_Error9 = " has an incorrect record size";                            // MGC dseg:3FBE
char *cnst_LBX_ErrorA = " exceeds number of defined records";                       // MGC dseg:3FDC
char *cnst_LBX_ErrorB = " cannot be reloaded into EMS w/o being first released.";   // MGC dseg:3FFF
char *cnst_LBX_ErrorC = " EMM loading error. Insufficient EMM.";                    // MGC dseg:4036
char *cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM";           // MGC dseg:405C
char *cnst_LBX_ErrorE = " (Reserved EMM) ";                                         // MGC dseg:408B
char *cnst_LBX_ErrorF1 = " LBX to";                                                 // MGC dseg:409C
char *cnst_LBX_ErrorG = " Vga file animation frames cannot exceed 65536 bytes";     // MGC dseg:40A4
*/
/*
switch case jump table
seg010:10F5
off_16015           ; jump table for switch statement
offset loc_15EE2    case 0x00
offset loc_15EF4    case 0x01
offset loc_15EF9    case 0x02
offset loc_15F32    case 0x03
offset loc_15F37    case 0x04
offset loc_16006    case default
offset loc_15F70    case 0x06
offset loc_15F76    case 0x07
offset loc_15F7C    case 0x08
offset loc_15F82    case 0x09
offset loc_15F88    case 0x0A
offset loc_15FA9    case 0x0B
offset loc_15FAF    case 0x0C
offset loc_15FB5    case 0x0D
offset loc_15FD3    case 0x0E
offset loc_15FF7    case 0x0F
*/
    LBX_Error_Index = LBX_Error_Number - 1;

    switch (LBX_Error_Index)
    {
        case 0:
            strcat(str_errmsg, cnst_LBX_Error1);    // char *cnst_LBX_Error1 = " could not be found.";                                     // MGC dseg:3EB3
            break;
        case 1:
            strcat(str_errmsg, cnst_LBX_Error2);    // char *cnst_LBX_Error2 = " has been corrupted.";                                     // MGC dseg:3EC8
            break;
        case 2:
            strcpy(str_errmsg, cnst_LBX_Error31);   // char *cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 // MGC dseg:3EDD
            itoa(RAM_Min_KB, str_itoa, 10);
            strcat(str_errmsg, str_itoa);
            strcat(str_errmsg, cnst_LBX_Error32);   // char *cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         // MGC dseg:3F05
            break;
        case 3:
            strcat(str_errmsg, cnst_LBX_Error4);    // char *cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       // MGC dseg:3F25
            break;
        case 4:
            strcat(str_errmsg, cnst_LBX_Error51);   // char *cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             // MGC dseg:3F58
            itoa(pages, str_itoa, 10);
            strcat(str_errmsg, str_itoa);
            strcat(str_errmsg, cnst_LBX_Error52);   // char *cnst_LBX_Error52 = " pages";                                                  // MGC dseg:3F84
            break;
        //case 5:
        //    break;
        case 6:
            strcat(str_errmsg, cnst_LBX_Error7);
            break;
        case 7:
            strcat(str_errmsg, cnst_LBX_Error8);
            break;
        case 8:
            strcat(str_errmsg, cnst_LBX_Error9);
            break;
        case 9:
            strcat(str_errmsg, cnst_LBX_ErrorA);
            break;
        case 10:
            strcpy(str_errmsg, LBX_Name);
            strcat(str_errmsg, LBX_File_Extension);
            strcat(str_errmsg, cnst_LBX_ErrorB);
            break;
        case 11:
            strcat(str_errmsg, cnst_LBX_ErrorC);
            break;
        case 12:
            strcat(str_errmsg, cnst_LBX_ErrorD);
            break;
        case 13:
            strcat(str_errmsg, cnst_LBX_ErrorC);
            strcat(str_errmsg, cnst_LBX_ErrorE);
        case 14:
            strcat(str_errmsg, cnst_LBX_ErrorF1);
            strcat(str_errmsg, cnst_LBX_ErrorC);
            strcat(str_errmsg, cnst_LBX_ErrorE);
            break;
        case 15:
            strcat(str_errmsg, cnst_LBX_ErrorG);
            break;
    }

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: LBX_Error( LBX_Name = %s, LBX_Error_Number = %d, LBX_Index = %d, pages = %d)\n", __FILE__, __LINE__, LBX_Name, LBX_Error_Number, LBX_Index, pages);
#endif

    Exit(str_errmsg);
}
