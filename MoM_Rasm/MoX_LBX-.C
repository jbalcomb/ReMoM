
#include "MoX_TYPE.H"   /* SAMB_ptr */
#include "MoX_DEF.H"    /* NULL */

#include "MoX_LBX.H"    /* SZ_LBX_HDR_PR; GET_LBX_HDR_COUNT(), _MAGSIG(), _TYPE(); */

#include "MoX_SA.H"     /* SAMB_SIZE, SAMB_USED; SA_Get_Used() */

#include <stdio.h>      /* printf() */
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

// ? MoO2 ?
int farload_lbx_header_flag = ST_FALSE;
SAMB_ptr farload_lbx_header = NULL;  // lbx_header {entries, magic_number, file_type, dummy, offsets[510]}
FILE * farload_fptr = NULL;
char secondary_drive_path[50];  // MoO2  ORION2.LE  dseg02:0017C544
// DNE MoM  int farload_file_flag;          // MoO2  ORION2.LE  dseg02:0017C576
// MoO2  char farload_file_name[15];
int farload_num_entries;
// ~==> MoM
int farload_fhnd = -1;
// int LBX_fhnd = -1;    // dseg:3E68  DOS Unused File Handle  == -1
char farload_lbx_name[16];


/*
    LBX Globals - Initialized
*/
unsigned int LBX_Header_Allocated = ST_FALSE;             // MGC dseg:3E66
// unsigned int g_LBX_File_Handle = 0xFFFF;          // MGC dseg:3E68  DOS Unused File Handle  == 0xFFFF (! -1)
FILE * g_LBX_File_Pointer = NULL;
// unsigned int UU_LBX_Header_Format_Override = 0;          // MGC dseg:3E6A
// char UU_LBX_File_Path[50] = {0};                // MGC dseg:3E6C  ; This is not in ORION.
char LBX_File_Extension[] = ".LBX";             // MGC dseg:3E9E


char * cnst_Error_Handler_Common1 = ".LBX [entry ";                                            // MGC  dseg:3EA3
char * cnst_Error_Handler_Common2 = "] ";                                                      // MGC  dseg:3EB0
char * cnst_Error_Handler1 = " could not be found.";                                    // MGC  dseg:3EB3
char * cnst_Error_Handler2  = " has been corrupted.";                                   // MGC  dseg:3EC8
char * cnst_Error_Handler31 = "Insufficient memory. You need at least ";                // MGC  dseg:3EDD
char * cnst_Error_Handler32 = "K free. Try removing all TSR',27h,'s.";                  // MGC  dseg:3F05
char * cnst_Error_Handler4  = " was not properly allocated or has been corrupted.";     // MGC  dseg:3F25
char * cnst_Error_Handler51 = " failed to reload. Allocation too small by ";            // MGC  dseg:3F58
char * cnst_Error_Handler52 = " pages";                                                 // MGC  dseg:3F84
char * cnst_Error_Handler7 = " is not an LBX file";                                     // MGC  dseg:3F8B
char * cnst_Error_Handler8 = " exceeds number of LBX entries";                          // MGC  dseg:3F9F
char * cnst_Error_Handler9 = " has an incorrect record size";                           // MGC  dseg:3FBE
char * cnst_Error_HandlerA = " exceeds number of defined records";                      // MGC  dseg:3FDC
char * cnst_Error_HandlerB = " cannot be reloaded into EMS w/o being first released.";  // MGC  dseg:3FFF
char * cnst_Error_HandlerC = " EMM loading error. Insufficient EMM.";                   // MGC  dseg:4036
char * cnst_Error_HandlerD = " Only pictures may be loaded into reserved EMM";          // MGC  dseg:405C
char * cnst_Error_HandlerE = " (Reserved EMM) ";                                        // MGC  dseg:408B
char * cnst_Error_HandlerF1 = " LBX to";                                                // MGC  dseg:409C
char * cnst_Error_HandlerG = " Vga file animation frames cannot exceed 65536 bytes";    // MGC  dseg:40A4

char * error_handler_strings[] =
{
    ".LBX [entry ",
    "] ",
    " could not be found.",
    " has been corrupted.",
    "Insufficient memory. You need at least ",
    "K free. Try removing all TSR',27h,'s.",
    " was not properly allocated or has been corrupted.",
    " failed to reload. Allocation too small by ",
    " pages",
    " is not an LBX file",
    " exceeds number of LBX entries",
    " has an incorrect record size",
    " exceeds number of defined records",
    " cannot be reloaded into EMS w/o being first released.",
    " EMM loading error. Insufficient EMM.",
    " Only pictures may be loaded into reserved EMM",
    " (Reserved EMM) ",
    " LBX to",
    " Vga file animation frames cannot exceed 65536 bytes"
};
/*
char * strings[] =
{
    "\nAhhh.\n\n",
	"\nAnd?\n\n",
	"\nTell me more.\n\n", 
	"\nNo kidding?!\n\n", 
	"\nI see.\n\n", 
	"\nSooooo?\n\n", 
	"\nAnd how does that make you feel?\n\n", 
	"\nWhat do you associate with the word 'guacamole'?\n\n", 
	"\nYou don't say?!\n\n", 
	"\n*snigger*\n\n",
	"\nNothing personal, but you are kidding... aren't you?\n\n",
	"\nYawn.\n\n",
	"\nNo! Really?\n\n",
	"\nAnd why do you think that might be?\n\n",
	"\nTell me about your childhood.\n\n",
	"\nElaborate... I feel there's something  you're not telling me here.\n\n",
	"\nCould you just repeat that a little louder, talking into this inconspicuous daffodil please?\n\n",
	"\nNurse Smith!!!\n We got a live one here, baby!!!\n\n",
	"\nTell me, what size do you take in strait-jackets?\n\n",
	"\nWhat do you associate with the word 'hippo'?\n\n",
	"\nI'd like to recommend you read this book. I feel could help you overcome your perversions... It's called 'Immuno-aspects of the Spleen'.\n\n"
};
    ...
    printf("%s\n", strings[random()%21]);
*/



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
// MGC  s10p01
SAMB_ptr Farload(char * lbx_name, int entry_num);

// MGC  s10p10
SAMB_ptr Farload_Library(char * lbx_name, signed int entry_num, SAMB_ptr base_seg, signed int allocation_type);
// MGC  s10p16
void File_Name_Base(char * file_name);


SAMB_ptr MoX_Explore_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt)
{
    SAMB_ptr SAMB_data;
    unsigned long DataSize_B;
    unsigned long DataSize_PR;
    SAMB_data = NULL;
    MoX_LBX_Header_Allocate();
    MoX_LBX_Header_Load(LBX_Name, LBX_Index);

    DataSize_B = 192000;
    DataSize_PR = (DataSize_B / SZ_PARAGRAPH_B) + 1;
    SAMB_data = Allocate_Space_No_Header(DataSize_PR);

    return SAMB_data;
}

static void MoX_LBX_Header_Allocate(void)
{
    if( LBX_Header_Allocated == ST_FALSE )
    {
        LBX_Header_Allocated = ST_TRUE;
        g_LBX_Header = Allocate_Space_No_Header(SZ_LBX_HDR_PR);
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
    unsigned char * pLbxMagSig;

    strcpy(g_LBX_Name, LBX_Name);
    strcpy(LBX_File_Name, LBX_Name);
    strcat(LBX_File_Name, LBX_File_Extension);
    printf("LBX_File_Name: %s\n", LBX_File_Name);

    g_LBX_File_Pointer = fopen(LBX_File_Name, "rb");

    // lbx_read_sgmt(g_LBX_Header, SZ_LBX_HDR_B, g_LBX_File_Handle);
    fread(g_LBX_Header, 512, 1, g_LBX_File_Pointer);

    // #define GET_LE_16(_ptr_) ( (((unsigned int)(((unsigned char _FAR *)(_ptr_))[1])) << 8) | ((unsigned int)(((unsigned char _FAR *)(_ptr_))[0])) )
    // yay workie LBX_MagSig = ( (unsigned int) ((unsigned char *)(g_LBX_Header))[3] << 8 ) | ( (unsigned int) ((unsigned char *)(g_LBX_Header))[2] );
    // yay workie LBX_MagSig = g_LBX_Header[2] | g_LBX_Header[3] << 8;
    // LBX_MagSig = g_LBX_Header + 2 | g_LBX_Header + 3 << 8;  //  error C2296: '<<': illegal, left operand has type 'SAMB_ptr'
    // precedence issue? << vs. | vs. +
    // trying to shift 3 then add to g_LBX_Header?
    // LBX_MagSig = g_LBX_Header + 2 | (g_LBX_Header + 3) << 8; // error C2296: '<<': illegal, left operand has type 'SAMB_ptr'
    // LBX_MagSig = g_LBX_Header + 2 | *(g_LBX_Header + 3) << 8; // IDE: expression must have integral or unscoped enum typeC/C++(2140)  // compiler:  error C2296: '|': illegal, left operand has type 'SAMB_ptr'
    // yay workie LBX_MagSig = *(g_LBX_Header + 2) | *(g_LBX_Header + 3) << 8;
    pLbxMagSig = g_LBX_Header + 2;
    LBX_MagSig = GET_2B(pLbxMagSig);
    printf("LBX_MagSig: 0x%04X\n", LBX_MagSig);

    if ( LBX_MagSig != LBX_MAGSIG ) { Error_Handler(LBX_Name, 0x07, LBX_Index, NULL); }

    g_LBX_Entry_Count = ( (unsigned int) ((unsigned char *)(g_LBX_Header))[1] << 8 ) | ( (unsigned int) ((unsigned char *)(g_LBX_Header))[0] );
    // g_LBX_Entry_Count = *((unsigned int *)g_LBX_Header[0]);

}


// MGC  s10p01
SAMB_ptr Farload(char * lbx_name, int entry_num)
{
    return Farload_Library(lbx_name, entry_num, NULL, 0);
}
// MGC  s10p02
SAMB_ptr Far_Reload(char * lbx_name, int entry_num, SAMB_ptr base_seg)
{
    return Farload_Library(lbx_name, entry_num, base_seg, 1);
}
// MGC  s10p03
SAMB_ptr Far_Reload_Next(char * lbx_name, int entry_num, SAMB_ptr base_seg)
{
    return Farload_Library(lbx_name, entry_num, base_seg, 2);
}


// MGC  s10p10
/*
    Farload_Library
        Address: 01:00126CEB
        Num params: 4
        Return type: pointer (4 bytes) 
    pointer (4 bytes) file_name
    signed integer (4 bytes) entry_num
    pointer (4 bytes) base_seg
    signed integer (4 bytes) allocation_type
        signed integer (4 bytes) file_index
        signed integer (4 bytes) space_remaining
        signed integer (4 bytes) current_extended_flag
        signed integer (4 bytes) alternate_set
        unsigned integer (4 bytes) num_blocks
        unsigned integer (4 bytes) current_seg
        unsigned integer (4 bytes) read_size
        signed integer (4 bytes) foffset
        signed integer (4 bytes) entry_start
        signed integer (4 bytes) entry_end
        signed integer (4 bytes) entry_length
        array (60 bytes) full_file_path
            Num elements: 60, Type: unsigned integer (4 bytes) 
        pointer (4 bytes) pointer
*/
// SAMB_ptr Farload_Library(char * file_name, signed int entry_num, SAMB_ptr base_seg, signed int allocation_type)
SAMB_ptr Farload_Library(char * lbx_name, signed int entry_num, SAMB_ptr base_seg, signed int allocation_type)
{
    signed int file_index;
    signed int space_remaining;
    signed int current_extended_flag;
    signed int alternate_set;
    unsigned int num_blocks;
    unsigned int current_seg;
    unsigned int read_size;
    signed long int foffset;
    signed long int entry_start;
    signed long int entry_end;
    signed long int entry_length;
    char lbx_file_name[20];
    char full_file_path[60];
    SAMB_ptr pointer;
    SAMB_ptr SAMB_data;  // ? current_seg ?
    SAMB_ptr rvr_SAMB_data;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Farload_Library(lbx_name=%s, entry_num=%d, base_seg=%p, allocation_type=%d)\n", __FILE__, __LINE__, lbx_name, entry_num, base_seg, allocation_type);
#endif

    // entry_num < 0

    // lbx_header alloc
    dbg_prn("DEBUG: [%s, %d] farload_lbx_header_flag: %d\n", __FILE__, __LINE__, farload_lbx_header_flag);
    if( farload_lbx_header_flag == ST_FALSE ) { farload_lbx_header_flag = ST_TRUE; farload_lbx_header = Allocate_Space_No_Header(SZ_LBX_HDR_PR); }
    dbg_prn("DEBUG: [%s, %d] farload_lbx_header_flag: %d\n", __FILE__, __LINE__, farload_lbx_header_flag);
    dbg_prn("DEBUG: [%s, %d] farload_lbx_header: %p\n", __FILE__, __LINE__, farload_lbx_header);

    // file_name base
    dbg_prn("DEBUG: [%s, %d] lbx_name: %s\n", __FILE__, __LINE__, lbx_name);
    File_Name_Base(lbx_name);
    dbg_prn("DEBUG: [%s, %d] lbx_name: %s\n", __FILE__, __LINE__, lbx_name);

    // lbx_header format/offset
    // ? current_extended_flag

    // EMM Farload Library  (? Farload_Buffer_Library ?)
    // SAMB_data = EMM_LBX_Load_Entry(file_name, entry_num, base_seg, allocation_type, ?current_extended_flag?);

    /*
        BEGIN: Current vs. Previous
    */
    if (farload_fptr == NULL) { DLOG("(farload_fptr == NULL)"); } else { DLOG("(farload_fptr != NULL)"); }
    dbg_prn("farload_lbx_name: %s  lbx_name: %s\n", farload_lbx_name, lbx_name);
    if ( (farload_fptr == NULL) || (stricmp(lbx_name, farload_lbx_name) != 0) )
    {
        DLOG("( (farload_fptr == NULL) || (stricmp(lbx_name, farload_lbx_name) != 0)");
            /*
                BEGIN: Current != Previous
            */

            if ( farload_fptr != NULL ) { fclose(farload_fptr); }
            strcpy(farload_lbx_name, lbx_name);
            strcpy(lbx_file_name, lbx_name);
            strcat(lbx_file_name, ".LBX");
            farload_fptr = fopen(lbx_file_name, "rb");

            // LBX Header Offset
            // if ( ?farload_extended_flag? == 0 ) { ?file_index/foffset? = 0; } else { ?file_index/foffset? = 512; }
            // if ( lbx_seek(?file_index/foffset?, farload_fptr) == ST_FAILURE ) { Error_Handler(file_name, 2, entry_num, NULL); }

            fread(farload_lbx_header, 1, SZ_LBX_HDR_B, farload_fptr);

            if ( GET_2B_OFS(farload_lbx_header, 2) != LBX_MAGSIG ) { Error_Handler(lbx_name, 7, entry_num, NULL); }
            dbg_prn("GET_2B_OFS(farload_lbx_header, 2: 0x%04X\n", GET_2B_OFS(farload_lbx_header, 2));

            farload_num_entries = GET_2B_OFS(farload_lbx_header, 0);
            dbg_prn("GET_2B_OFS(farload_lbx_header, 0): 0x%04X\n", GET_2B_OFS(farload_lbx_header, 0));
            dbg_prn("farload_num_entries: %d\n" , farload_num_entries);

            
            /*
                END: Current != Previous
            */
    }
    if ( farload_num_entries < entry_num ) { Error_Handler(lbx_name, 8, entry_num, NULL); }
    /*
        END: Current vs. Previous
    */

    /*
        BEGIN: Entry - Offset Begin, End, Size
    */
    entry_start = GET_4B_OFS(farload_lbx_header, ( 8 + (entry_num * 4) ));
    entry_end = GET_4B_OFS(farload_lbx_header, ( 8 + (entry_num * 4) + 4));
    entry_length = entry_end - entry_start;
    // if ( fseek(farload_fptr, entry_start, 0) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
    fseek(farload_fptr, entry_start, 0);
    /*
        END: Entry - Offset Begin, End, Size
    */

    /*
        BEGIN: Allocation Type
    */
    num_blocks = 1 + (entry_length / SZ_PARAGRAPH_B);
    switch(allocation_type)
    {
        case 0:
            SAMB_data = Allocate_Space_No_Header(num_blocks);
            // if ( SAMB_data == 0 ) { Error_Handler(lbx_name, 3, entry_num, NULL); }
            break;
        case 1:
            if ( Check_Allocation(base_seg) == 0 ) { Error_Handler(lbx_name, 2, entry_num, NULL); };
            if ( num_blocks > (GET_2B_OFS(base_seg, SAMB_SIZE) - 1) ) { Error_Handler(lbx_name, 4, entry_num, (num_blocks - GET_2B_OFS(base_seg, SAMB_SIZE) + 1)); }
            SAMB_data = PTR_ADD_PARAGRAPH(base_seg, 1);
            SET_2B_OFS(base_seg, SAMB_USED, (num_blocks + 1));
            dbg_prn("Used: %0d\n", SA_Get_Used(base_seg));
            break;
        case 2:
            if ( Check_Allocation(base_seg) == 0 ) { Error_Handler(lbx_name, 2, entry_num, NULL); };
            if ( num_blocks > Get_Free_Blocks(base_seg) ) { Error_Handler(lbx_name, 5, entry_num, (num_blocks - Get_Free_Blocks(base_seg))); }
            SAMB_data = PTR_ADD_PARAGRAPH(base_seg, SA_Get_Used(base_seg));
            SET_2B_OFS(base_seg, SAMB_USED, (num_blocks + SA_Get_Used(base_seg)));
            break;
    }
    /*
        END: Allocation Type
    */

    /*
        BEGIN: Read Data
    */
    rvr_SAMB_data = SAMB_data;
    read_size = SZ_32K_B;
    while ( entry_length >= read_size )
    {
        entry_length -= read_size;
        // if ( lbx_read_sgmt(current_seg, read_size, farload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        fread(rvr_SAMB_data, read_size, 1, farload_fptr);
        rvr_SAMB_data -= read_size;
    }
    if ( entry_length > 0 )
    {
        read_size = entry_length;
        // if ( lbx_read_sgmt(current_seg, read_size, farload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        fread(rvr_SAMB_data, read_size, 1, farload_fptr);
    }

    /*
        END: Read Data
    */

    // Check_Free();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Farload_Library(lbx_name=%s, entry_num=%d, base_seg=%p, allocation_type=%d) { SAMB_data = %p }\n", __FILE__, __LINE__, lbx_name, entry_num, base_seg, allocation_type, SAMB_data);
#endif

    return SAMB_data;
}

SAMB_ptr MoX_LBX_Load_Entry(char * file_name, int entry_num, SAMB_ptr SAMB_head, int allocation_type, int LbxHdrFmt)
{
    SAMB_ptr SAMB_data;
    unsigned long DataSize_B;
    unsigned long DataSize_PR;
    SAMB_ptr tmp_SAMB_data;

    SAMB_data = NULL;

    MoX_LBX_Header_Allocate();
    MoX_LBX_Header_Load(file_name, entry_num);
    if ( !(entry_num < farload_num_entries) ) { Error_Handler(file_name, 8, entry_num, NULL); }

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
    switch(allocation_type)
    {
        case 0:
        {
            SAMB_data = Allocate_Space_No_Header(DataSize_PR);
            if ( SAMB_data == 0 ) { Error_Handler(file_name, 3, entry_num, NULL); /* LBXErr_low_RAM */ }
            break;
        }
        case 1:
        {
//            if ( SA_Alloc_Validate(SAMB_head) == 0 ) { Error_Handler(LBX_Name, 0x02, LBX_Index, NULL); }
//            // if ( DataSize_PR > (farpeekw(SAMB_head, SAMB_SIZE) - 1) ) { Error_Handler(LBX_Name, 0x04, LBX_Index, (DataSize_PR - farpeekw(SAMB_head, SAMB_SIZE) + 1)); }
//            SAMB_data = SAMB_head + 1;
//            farpokew(SAMB_head, SAMB_USED, (DataSize_PR + 1));
            break;
        }
        case 2:
        {
//            if ( SA_Alloc_Validate(SAMB_head) == 0 ) { Error_Handler(LBX_Name, 0x02, LBX_Index, NULL); }
//            // if ( DataSize_PR > SA_GetFree(pSAMB_head) ) { Error_Handler(LBX_Name, 0x05, LBX_Index, (DataSize_PR - SA_GetFree(pSAMB_head))); }
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
void Error_Handler(char * file_name, int error_num, int entry_num, int pages)
{
    char buffer[120];
    char buffer2[20];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Error_Handler( file_name = %s, error_num = %d, entry_num = %d, pages = %d)\n", __FILE__, __LINE__, file_name, error_num, entry_num, pages);
#endif

#ifdef STU_DEBUG
    // strcpy(buffer, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");  // 120 characters
    strcpy(buffer, "0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_");  // 110 characters
    dbg_prn("DEBUG: [%s, %d] buffer: %s\n", __FILE__, __LINE__, buffer);
#endif

    strcpy(buffer, file_name);
    itoa(entry_num, buffer2, 10);
    // strcat(buffer, cnst_Error_Handler_Common1);        // char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         // MGC dseg:3EA3
    strcat(buffer, error_handler_strings[0]);
    strcat(buffer, buffer2);
    // strcat(buffer, cnst_Error_Handler_Common2);        // char *cnst_LBXErr_Common2 = "] ";                                                   // MGC dseg:3EB0
    strcat(buffer, error_handler_strings[1]);

    switch (error_num)
    {
        case 1:
            strcat(buffer, cnst_Error_Handler1);    // char *cnst_Error_Handler1 = " could not be found.";                                     // MGC dseg:3EB3
            break;
        case 2:
            strcat(buffer, cnst_Error_Handler2);    // char *cnst_Error_Handler2 = " has been corrupted.";                                     // MGC dseg:3EC8
            break;
        case 3:
            strcpy(buffer, cnst_Error_Handler31);   // char *cnst_Error_Handler31 = "Insufficient memory. You need at least ";                 // MGC dseg:3EDD
            itoa(RAM_Min_KB, buffer2, 10);
            strcat(buffer, buffer2);
            strcat(buffer, cnst_Error_Handler32);   // char *cnst_Error_Handler32 = "K free. Try removing all TSR's.";                         // MGC dseg:3F05
            break;
        case 4:
            strcat(buffer, cnst_Error_Handler4);    // char *cnst_Error_Handler4 = " was not properly allocated or has been corrupted.";       // MGC dseg:3F25
            break;
        case 5:
            strcat(buffer, cnst_Error_Handler51);   // char *cnst_Error_Handler51 = " failed to reload. Allocation too small by ";             // MGC dseg:3F58
            itoa(pages, buffer2, 10);
            strcat(buffer, buffer2);
            strcat(buffer, cnst_Error_Handler52);   // char *cnst_Error_Handler52 = " pages";                                                  // MGC dseg:3F84
            break;
        //case 6:
        //    break;
        case 7:
            strcat(buffer, cnst_Error_Handler7);
            break;
        case 8:
            strcat(buffer, cnst_Error_Handler8);
            break;
        case 9:
            strcat(buffer, cnst_Error_Handler9);
            break;
        case 10:
            strcat(buffer, cnst_Error_HandlerA);
            break;
        case 11:
            strcpy(buffer, file_name);
            strcat(buffer, LBX_File_Extension);
            strcat(buffer, cnst_Error_HandlerB);
            break;
        case 12:
            strcat(buffer, cnst_Error_HandlerC);
            break;
        case 13:
            strcat(buffer, cnst_Error_HandlerD);
            break;
        case 14:
            strcat(buffer, cnst_Error_HandlerC);
            strcat(buffer, cnst_Error_HandlerE);
        case 15:
            strcat(buffer, cnst_Error_HandlerF1);
            strcat(buffer, cnst_Error_HandlerC);
            strcat(buffer, cnst_Error_HandlerE);
            break;
        case 16:
            strcat(buffer, cnst_Error_HandlerG);
            break;
    }

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] buffer: %s\n", __FILE__, __LINE__, buffer);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Error_Handler( file_name = %s, error_num = %d, entry_num = %d, pages = %d)\n", __FILE__, __LINE__, file_name, error_num, entry_num, pages);
#endif

    Exit_With_Message(buffer);
}

// MGC  s10p16
void File_Name_Base(char * file_name)
{
    int itr;
    itr = 0;
/*
    // Continue if the value pointed at by `line` is not NUL
    while (*line) {
        // Dereference `line` and assign a new value '0' then advance the pointer
        *(line++) = '0';
    }
*/
    while (file_name[itr] != '\0')
    {
        if ( file_name[itr] >= 'a' ) { file_name[itr] = file_name[itr] - 32; }
        if ( file_name[itr] == '.' ) { file_name[itr] = '\0'; }
        itr++;
    }
}
