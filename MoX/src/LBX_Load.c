/**
 * @file LBX_Load.c
 * @brief Resource loading from LBX library files (Master of Magic game archives).
 *
 * Handles loading, reloading, and management of entries from LBX (.LBX) archive files.
 * These archives contain game resources such as graphics, data structures, and sound effects.
 * Supports both full entry loading and partial data loading with record offset/count control.
 *
 * **Core Loading Functions:**
 * - LBX_Load() - Load an entire LBX entry into freshly allocated memory
 * - LBX_Reload() - Reload an entry into existing memory (first allocation)
 * - LBX_Reload_Next() - Reload an entry into existing memory (next allocation)
 *
 * **Data Loading Functions (with record support):**
 * - LBX_Load_Data() - Load specific records from an LBX entry
 * - LBX_Reload_Data() - Reload records into existing memory (first allocation)
 * - LBX_Reload_Next_Data() - Reload records into existing memory (next allocation)
 * - LBX_Load_Data_Static() - Load records into a pre-allocated static buffer
 *
 * **Core Implementation:**
 * - LBX_Load_Entry() - Primary entry loading engine
 * - LBX_Load_Library_Data() - Primary data loading engine with record support
 *
 * **Utilities:**
 * - File_Name_Base() - Extract base filename from path
 * - Set_Alternate_Path() - Set alternate directory for LBX files   
 * - Error_Handler() - Handle and report LBX loading errors
 *
 * **Features:**
 * - Persistent file handle caching to avoid redundant file opens
 * - Supports three allocation strategies (single, first, next)
 * - Record-based partial loading for structured data
 * - Comprehensive error reporting with detailed messages
 * - SDL-compatible buffer handling
 *
 * @note Originally from WIZARDS.EXE segment 010 (s10).
 *       Ported from MoO2 farload module with extensions for Master of Magic.
 *
 * @see Allocate.h for memory allocation interfaces
 * @see MOX_TYPE.h for SAMB_ptr and other type definitions
 */

#include "Allocate.h"
#include "EXIT.h"
#include "MOM_Data.h"
#include "MOX_BASE.h"
#include "MOX_TYPE.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_stdinc.h>

#include "LBX_Load.h"



int16_t lbxload_lbx_header_flag = ST_FALSE;
FILE * lbxload_fptr = NULL;
char lbxload_lbx_file_extension[] = ".LBX";
int16_t lbxload_num_entries;
SAMB_ptr lbxload_lbx_header;
char lbxload_lbx_name[16];
/* HACK */  uint32_t lbxload_entry_length;  /* because. SDL Mixed needs the sound buffer size */


/*
same as Allocation Error for RAM_MinKbytes?
*/
char * str_error_handler[] =
{
    ".LBX [entry ",
    "] ",
    " could not be found.",
    " has been corrupted.",
    "Insufficient memory. You need at least ", /* cnst_LBX_Error31, cnst_Alloc_Error51 */
    "K free. Try removing all TSR',27h,'s.",   /* cnst_LBX_Error32, cnst_Alloc_Error52 */
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


// MGC s10p01
SAMB_ptr LBX_Load(char * lbx_name, int16_t entry_num)
{
    return LBX_Load_Entry(lbx_name, entry_num, ST_NULL, sa_Single);
}
// MGC s10p02
SAMB_ptr LBX_Reload(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head)
{
    return LBX_Load_Entry(lbx_name, entry_num, SAMB_head, sa_First);
}
// MGC s10p03
SAMB_ptr LBX_Reload_Next(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head)
{
    return LBX_Load_Entry(lbx_name, entry_num, SAMB_head, sa_Next);
}

// WZD s10p04
// MoO2 Farload_Data
SAMB_ptr LBX_Load_Data(char * lbx_name, int16_t entry_num, int16_t start_rec, int16_t num_recs, int16_t record_size)
{
    SAMB_ptr SAMB_data;

    SAMB_data = LBX_Load_Library_Data(lbx_name, entry_num, ST_NULL, sa_Single, start_rec, num_recs, record_size);

    return SAMB_data;
}
// WZD s10p05
// MoO2 Far_Reload_Data
SAMB_ptr LBX_Reload_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t start_rec, int16_t num_recs, int16_t record_size)
{
    SAMB_ptr SAMB_data;

    SAMB_data = LBX_Load_Library_Data(lbx_name, entry_num, SAMB_head, sa_First, start_rec, num_recs, record_size);

    return SAMB_data;
}
// WZD s10p06
// MoO2 Far_Reload_Next_Data
SAMB_ptr LBX_Reload_Next_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t start_rec, int16_t num_recs, int16_t record_size)
{
    SAMB_ptr SAMB_data;

    SAMB_data = LBX_Load_Library_Data(lbx_name, entry_num, SAMB_head, sa_Next, start_rec, num_recs, record_size);

    return SAMB_data;
}

// MGC s10p10
// MoO2: Farload_Library()
SAMB_ptr LBX_Load_Entry(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type)
{
    uint16_t num_blocks;
    uint16_t read_size;
    uint32_t entry_start;
    uint32_t entry_end;
    uint32_t entry_length;
    char lbx_file_name[LEN_FILE_NAME];
    SAMB_ptr SAMB_data;
    SAMB_ptr rvr_SAMB_data;

    uint16_t num_blocks_used;  // DNE in Dasm

    // Meh.
    // Error	C4703	potentially uninitialized local pointer variable 'SAMB_data' used	STU - MoM_Rasm	C : \devel\STU - MoM_Rasm\src\LBX_Load.C	255
    SAMB_data = NULL;

    // if(entry_num < 0) { LBX_Error(lbx_name, 1, entry_num, NULL); }

    if(lbxload_lbx_header_flag == ST_FALSE)
    {
        lbxload_lbx_header_flag = ST_TRUE;
        lbxload_lbx_header = (SAMB_ptr)Allocate_Space_No_Header(SZ_LBX_HDR_PR);
    }

    File_Name_Base(lbx_name);

    /*
        BEGIN: Current vs. Previous
    */
// #pragma warning(suppress : 4996)
    // if((lbxload_fptr == NULL) || (stricmp(lbx_name, lbxload_lbx_name) != 0))
    if(
        (lbxload_fptr == NULL)
        ||
        (strcmp(lbx_name, lbxload_lbx_name) != 0)
    )
    {
        /*
            BEGIN: Current != Previous
        */

        if(lbxload_fptr != NULL) { fclose(lbxload_fptr); }

        strcpy(lbxload_lbx_name, lbx_name);
        strcpy(lbx_file_name, lbx_name);
        strcat(lbx_file_name, ".LBX");

        lbxload_fptr = fopen(lbx_file_name, "rb");

        // if(lbxload_fptr == NULL) { if(secondary_drive_path == NULL) { Error_Handler(LBXErr_not_found) } else { ... secondary_drive_path full_file_path lbx_open() ... }

        // if UU_farload_hdr_fmt ... file_hdr_ofst  512 or 0

        // lbx_seek(file_hdr_ofst, farload_fptr)

        fread(lbxload_lbx_header, 1, SZ_LBX_HDR_B, lbxload_fptr);

        // if(LBX_GET_MAGIC_NUMBER(lbxload_lbx_header) != LBX_MAGSIG) { Error_Handler(lbx_name, 7, entry_num, NULL); }

        lbxload_num_entries = LBX_GET_NUM_ENTRIES(lbxload_lbx_header);


        /*
            END: Current != Previous
        */
    }
    if(lbxload_num_entries < entry_num) { Error_Handler(lbx_name, 8, entry_num, ST_NULL); }
    /*
        END: Current vs. Previous
    */


    /*
        BEGIN: Entry - Offset Start, End, Length
    */
    entry_start = ( GET_4B_OFS( (lbxload_lbx_header), ( 8 + ((entry_num) * 4)    ) ) );
    entry_end   = ( GET_4B_OFS( (lbxload_lbx_header), ( 8 + ((entry_num) * 4) + 4) ) );
    entry_length = entry_end - entry_start;

    /* HACK */  lbxload_entry_length = entry_length;  /* because. SDL Mixed needs the sound buffer size */

    fseek(lbxload_fptr, entry_start, 0);
    /*
        END: Entry - Offset Start, End, Length
    */


    /*
        BEGIN: Allocation Type
    */
    num_blocks = 1 + (entry_length / SZ_PARAGRAPH_B);
    switch(allocation_type)
    {
        case sa_Single:
        {
            SAMB_data = Allocate_Space_No_Header(num_blocks);
            if(SAMB_data == NULL) { Error_Handler(lbx_name, 3, entry_num, ST_NULL); }
        } break;
        case sa_First:
        {
            if(Check_Allocation(SAMB_head) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, ST_NULL); };
            if(num_blocks > (SA_GET_SIZE(SAMB_head) - 1)) { Error_Handler(lbx_name, 4, entry_num, (num_blocks - (SA_GET_SIZE(SAMB_head) + 1))); }
            // SAMB_data = SAMB_head + (16 - 4);
            SAMB_data = SAMB_head + 16;
            SA_SET_USED(SAMB_head, (num_blocks + 1));
        } break;
        case sa_Next:
        {
            if(Check_Allocation(SAMB_head) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, ST_NULL); };
            if(num_blocks > Get_Free_Blocks(SAMB_head)) { Error_Handler(lbx_name, 5, entry_num, (num_blocks - Get_Free_Blocks(SAMB_head))); }
            // assert(Check_Allocation(SAMB_head) != ST_FAILURE));
            
            // SAMB_data = SAMB_head + (16 - 4) + (SA_GET_USED(SAMB_head) * SZ_PARAGRAPH_B);
            SAMB_data = SAMB_head + 16 + (SA_GET_USED(SAMB_head) * SZ_PARAGRAPH_B);

            num_blocks_used = num_blocks + SA_GET_USED(SAMB_head);
            *( (SAMB_head) + (SAMB_USED) + 0 ) = (unsigned char)( (num_blocks_used)      );
            *( (SAMB_head) + (SAMB_USED) + 1 ) = (unsigned char)( (num_blocks_used) >> 8 );
            // SET_2B_OFS(SAMB_head, SAMB_USED);
            // SA_SET_USED(SAMB_head, (num_blocks + 1));

        } break;
    }
    /*
        END: Allocation Type
    */


    /*
        BEGIN: Read Data
    */
#pragma warning(suppress : 4703)
    rvr_SAMB_data = SAMB_data;
    read_size = SZ_32K_B;
    while(entry_length >= read_size)
    {
        entry_length -= read_size;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
// #pragma warning(suppress : 28183)
        fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
        rvr_SAMB_data += read_size;
    }
    if(entry_length > 0)
    {
        read_size = entry_length;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
#pragma warning(suppress : 28183 6387)
        fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
    }
    /*
        END: Read Data
    */


    // Update_MemFreeWorst_KB();
    // MoO2: Check_Free();

#if defined(__DOS16__)
Done:
#endif

    return SAMB_data;
}




// WZD s10p11
// MoO2  Module: farload  Farload_Data() |-> Farload_Library_Data()
SAMB_ptr LBX_Load_Library_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type, uint16_t start_rec, uint16_t num_recs, uint16_t record_size)
{
    char full_file_path[LEN_FILE_PATH] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char lbx_file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32_t entry_length = 0;
    uint32_t entry_end = 0;
    uint32_t entry_start = 0;
    // NIU  int16_t file_hdr_fmt = 0;
    int16_t rec_size = 0;
    uint16_t read_size = 0;
    SAMB_ptr rvr_SAMB_data = NULL;
    uint16_t num_blocks = 0;
    int16_t max_records = 0;
// current_extended_flag= word ptr -6
// header_offset= word ptr -4
    SAMB_ptr SAMB_data = NULL;
    uint16_t num_blocks_used = 0;  // DNE in Dasm
    uint32_t record_start = 0;  // DNE in Dasm; entry_start__record_start

    // if(entry_num < 0) { LBX_Error(lbx_name, 1, entry_num, NULL); }  // "<lbx_name>.LBX [entry <entry_num>] could not be found."

    if(lbxload_lbx_header_flag == ST_FALSE)
    {
        lbxload_lbx_header_flag = ST_TRUE;
        lbxload_lbx_header = (SAMB_ptr)Allocate_Space_No_Header(SZ_LBX_HDR_PR);
    }

    File_Name_Base(lbx_name);

    // SAMB_data = EMM_LBX_RecLoader(file_name, entry_num, SAMB_head@, start_rec, num_recs, record_size)
    // current_extended_flag = ST_FALSE;
    // if(SAMB_data == ST_NULL)


    /*
        BEGIN: Current vs. Previous
    */

    // if((lbxload_fptr == NULL) || (stricmp(lbx_name, lbxload_lbx_name) != 0))
    if(
        (lbxload_fptr == NULL)
        ||
        (strcmp(lbx_name, lbxload_lbx_name) != 0)
    )
    {
        /*
            BEGIN: Current != Previous
        */

        if(lbxload_fptr != NULL) { fclose(lbxload_fptr); }

        strcpy(lbxload_lbx_name, lbx_name);
        strcpy(lbx_file_name, lbx_name);
        strcat(lbx_file_name, ".LBX");

        lbxload_fptr = fopen(lbx_file_name, "rb");

        // if(lbxload_fptr == NULL) { if(secondary_drive_path == NULL) { Error_Handler(LBXErr_not_found) } else { ... secondary_drive_path full_file_path lbx_open() ... }

        // DNE  if UU_farload_hdr_fmt ... file_hdr_ofst  512 or 0

        // lbx_seek(file_hdr_ofst, farload_fptr)

        fread(lbxload_lbx_header, 1, SZ_LBX_HDR_B, lbxload_fptr);

        // if(LBX_GET_MAGIC_NUMBER(lbxload_lbx_header) != LBX_MAGSIG) { Error_Handler(lbx_name, 7, entry_num, NULL); }

        lbxload_num_entries = LBX_GET_NUM_ENTRIES(lbxload_lbx_header);


        /*
            END: Current != Previous
        */
    }
    if(lbxload_num_entries < entry_num) { Error_Handler(lbx_name, 8, entry_num, ST_NULL); }  // " exceeds number of LBX entries"
    /*
        END: Current vs. Previous
    */
    
    /*
        BEGIN: Entry - Offset Start, End, Length
    */
    entry_start = ( GET_4B_OFS( (lbxload_lbx_header), ( 8 + ((entry_num) * 4)    ) ) );
    entry_end   = ( GET_4B_OFS( (lbxload_lbx_header), ( 8 + ((entry_num) * 4) + 4) ) );
    entry_length = entry_end - entry_start;

    fseek(lbxload_fptr, entry_start, 0);
    /*
        END: Entry - Offset Start, End, Length
    */


    fread(&max_records, 2, 1, lbxload_fptr);
    fread(&rec_size, 2, 1, lbxload_fptr);

    if(record_size != rec_size)
    {
        // TODO LBX_Error_Handler(LBXErr_recsize_mismatch)
    }

    if(start_rec + num_recs > max_records)
    {
        // TODO LBX_Error_Handler(LBXErr_records_exceeded)
    }

    // ¿ MoO2: foffset ?
    // record_start = entry_start + (start_rec * rec_size);
    record_start = entry_start + (start_rec * rec_size) + 4;
    fseek(lbxload_fptr, record_start, 0);

    /*
        BEGIN: Allocation Type
    */
    num_blocks = 1 + (entry_length / SZ_PARAGRAPH_B);

    switch(allocation_type)
    {
        case 0:  /* sa_Single */
        {
            SAMB_data = Allocate_Space_No_Header(num_blocks);
            if(SAMB_data == NULL) { Error_Handler(lbx_name, 3, entry_num, ST_NULL); }
        } break;
        case 1:  /* sa_First */
        {
            if(Check_Allocation(SAMB_head) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, ST_NULL); };
            if(num_blocks > (SA_GET_SIZE(SAMB_head) - 1)) { Error_Handler(lbx_name, 4, entry_num, (num_blocks - (SA_GET_SIZE(SAMB_head) + 1))); }
            SAMB_data = SAMB_head + 12;
            SA_SET_USED(SAMB_head, (num_blocks + 1));
        } break;
        case 2:  /* sa_Next */
        {
            if(Check_Allocation(SAMB_head) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, ST_NULL); };
            if(num_blocks > Get_Free_Blocks(SAMB_head)) { Error_Handler(lbx_name, 5, entry_num, (num_blocks - Get_Free_Blocks(SAMB_head))); }
            // assert(Check_Allocation(SAMB_head) != ST_FAILURE));
            
            SAMB_data = SAMB_head + 12 + (16 * SA_GET_USED(SAMB_head));

            num_blocks_used = num_blocks + SA_GET_USED(SAMB_head);
            *( (SAMB_head) + (SAMB_USED) + 0 ) = (unsigned char)( (num_blocks_used)      );
            *( (SAMB_head) + (SAMB_USED) + 1 ) = (unsigned char)( (num_blocks_used) >> 8 );

        } break;
    }
    /*
        END: Allocation Type
    */

    /*
        BEGIN: Read Data
    */
    rvr_SAMB_data = SAMB_data;
    read_size = SZ_32K_B;
    while(entry_length >= read_size)
    {
        entry_length -= read_size;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        if(NULL != rvr_SAMB_data)
        {
            fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
        }
        rvr_SAMB_data += read_size;
    }
    if(entry_length > 0)
    {
        read_size = entry_length;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }

        if(NULL != rvr_SAMB_data)
        {
            fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
        }
    }
    /*
        END: Read Data
    */

    
    // Update_MemFreeWorst_KB();
    // MoO2: Check_Free();

#if defined(__DOS16__)
Done:
#endif

    return SAMB_data;
}


// WZD s10p12
    // Main_Screen_Help() LBXR_DirectLoader("helpentry", 1, &_help_entries, 0, 25, 10);
// MoO2: Farload_Data_Static()
/*
function (0 bytes) Farload_Library_Data
Address: 01:00126F3B
Return type: pointer (4 bytes) 
pointer (4 bytes) file_name
signed integer (4 bytes) entry_num
pointer (4 bytes) base_seg
signed integer (4 bytes) allocation_type
unsigned integer (4 bytes) start_rec
unsigned integer (2 bytes) num_recs
unsigned integer (2 bytes) record_size
Locals:
    signed integer (4 bytes) allocation_seg
    signed integer (4 bytes) file_index
    signed integer (4 bytes) space_remaining
    signed integer (4 bytes) current_extended_flag
    unsigned integer (2 bytes) max_records
    unsigned integer (2 bytes) rec_size
    unsigned integer (4 bytes) num_blocks
    unsigned integer (4 bytes) current_seg
    unsigned integer (4 bytes) read_size
    signed integer (4 bytes) foffset
    signed integer (4 bytes) entry_start
    signed integer (4 bytes) entry_end
    signed integer (4 bytes) entry_length
    array (60 bytes) full_file_path
    Num elements:   60, Type:                unsigned integer (4 bytes) 
    pointer (4 bytes) pointer
*/
void LBX_Load_Data_Static(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, uint16_t start_rec, uint16_t num_recs, uint16_t record_size)
{
    char full_file_path[LEN_FILE_PATH] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char lbx_file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // NIU  int16_t file_hdr_fmt = 0;
    uint32_t entry_start = 0;
    uint32_t entry_end = 0;
    uint32_t entry_length = 0;
    int16_t max_records = 0;
    int16_t rec_size = 0;
    uint16_t read_size = 0;
    uint32_t record_start = 0;  // DNE in Dasm; entry_start__record_start

    // if(entry_num < 0) { LBX_Error(lbx_name, 1, entry_num, NULL); }  // "<lbx_name>.LBX [entry <entry_num>] could not be found."

    if(lbxload_lbx_header_flag == ST_FALSE)
    {
        lbxload_lbx_header_flag = ST_TRUE;
        lbxload_lbx_header = (SAMB_ptr)Allocate_Space_No_Header(SZ_LBX_HDR_PR);
    }

    File_Name_Base(lbx_name);

    // UU_SAMB_data = EMM_LBXR_DirectLoad(file_name, entry_num, SAMB_head@, start_rec, num_recs, record_size)
    // current_extended_flag = ST_FALSE;
    // if(UU_SAMB_data == ST_NULL)

    /*
        BEGIN: Current vs. Previous
    */

    if(
        (NULL == lbxload_fptr)
        ||
        (strcmp(lbx_name, lbxload_lbx_name) != 0)
    )
    {
        /*
            BEGIN: Current != Previous
        */

        if(NULL != lbxload_fptr) { fclose(lbxload_fptr); }  // TODO  can't actually happen, right? cause wrapped in if(NULL == lbxload_fptr)

        strcpy(lbxload_lbx_name, lbx_name);
        strcpy(lbx_file_name, lbx_name);
        strcat(lbx_file_name, ".LBX");

        lbxload_fptr = fopen(lbx_file_name, "rb");

        // if(lbxload_fptr == NULL) { if(secondary_drive_path == NULL) { Error_Handler(LBXErr_not_found) } else { ... secondary_drive_path full_file_path lbx_open() ... }
        if(NULL == lbxload_fptr) { Error_Handler(lbx_name, le_not_found, entry_num, ST_NULL); }

        // DNE  if UU_farload_hdr_fmt ... file_hdr_ofst  512 or 0

        // lbx_seek(file_hdr_ofst, farload_fptr)

        if(NULL != lbxload_fptr)
        {
            fread(lbxload_lbx_header, 1, SZ_LBX_HDR_B, lbxload_fptr);
        }

        // if(LBX_GET_MAGIC_NUMBER(lbxload_lbx_header) != LBX_MAGSIG) { Error_Handler(lbx_name, 7, entry_num, NULL); }

        lbxload_num_entries = LBX_GET_NUM_ENTRIES(lbxload_lbx_header);


        /*
            END: Current != Previous
        */
    }
    if(lbxload_num_entries < entry_num) { Error_Handler(lbx_name, le_entries_exceeded, entry_num, ST_NULL); }  // " exceeds number of LBX entries"
    /*
        END: Current vs. Previous
    */





    /*
        BEGIN: Entry - Offset Start, End, Length
    */
    entry_start = ( GET_4B_OFS( (lbxload_lbx_header), ( 8 + ((entry_num) * 4)    ) ) );
    entry_end   = ( GET_4B_OFS( (lbxload_lbx_header), ( 8 + ((entry_num) * 4) + 4) ) );
    entry_length = entry_end - entry_start;

    // Warning	C6387	'lbxload_fptr' could be '0':  this does not adhere to the specification for the function 'fseek'.
    if(NULL != lbxload_fptr)
    {
        fseek(lbxload_fptr, entry_start, 0);
    }
    /*
        END: Entry - Offset Start, End, Length
    */

    if(NULL != lbxload_fptr)
    {
        fread(&max_records, 2, 1, lbxload_fptr);
    }
    
    if(NULL != lbxload_fptr)
    {
        fread(&rec_size, 2, 1, lbxload_fptr);
    }

    if(record_size != rec_size)
    {
        // TODO LBX_Error_Handler(LBXErr_recsize_mismatch)
    }

    if(start_rec + num_recs > max_records)
    {
        // TODO LBX_Error_Handler(LBXErr_records_exceeded)
    }

    // ¿ MoO2: foffset ?
    // record_start = entry_start + (start_rec * rec_size);
    record_start = entry_start + (start_rec * rec_size) + 4;

    if(NULL != lbxload_fptr)
    {
        fseek(lbxload_fptr, record_start, 0);
    }


    // entry_length = num_recs * rec_size;
    // read_size = mod(entry_length, 60000);
    // // num_recs = max_records - start_rec
    read_size = num_recs * rec_size;


    /*
        BEGIN: Read Data
    */

    if(NULL != lbxload_fptr)
    {
        fread(SAMB_head, read_size, 1, lbxload_fptr);
    }

    /*
        END: Read Data
    */







    // DOMSDOS  Update_MemFreeWorst_KB();
    // MoO2: Check_Free();

}

// WZD s10p14
// drake178: UU_LBX_SetPath()
// MoO2  Module: farload  Set_Alternate_Path()
/*
; Unused in MoM
;
; sets a (path) string that LBX disk loader functions
; will append the file name after, to try again if
; they can't open the file with name only
; returns a pointer to the static string variable
*/
/*
MoO2
XREF: 
    Module: MOX2  Set_Mox_Alt_Path_()
        main__0()
            |-> Set_Mox_Alt_Path_()
                |-> Set_Alternate_Path()
OON XREF; gets 60 characters from "orioncd.ini"

*/
/*
NIU
    sets secondary_drive_path to the alternate path

*/
// Message	VCR003	Function 'Set_Alternate_Path' can be made static		C:\STU\devel\ReMoM\MoX\src\LBX_Load.c	674		
static void Set_Alternate_Path(char * alternate)
{
    strcpy(secondary_drive_path, alternate);
}

// WZD s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages)
{
    char buffer[120];
    char buffer2[20];

    strcpy(buffer, file_name);
#pragma warning(suppress : 4996)
    SDL_itoa(entry_num, buffer2, 10);
    strcat(buffer, str_error_handler[0]);
    strcat(buffer, buffer2);
    strcat(buffer, str_error_handler[1]);

    switch (error_num)
    {
        case le_not_found:
            strcat(buffer, str_error_handler[2]);
            break;
        case le_corrupted:
            strcat(buffer, str_error_handler[3]);
            break;
        case le_low_RAM:
            strcpy(buffer, str_error_handler[4]);
            
#pragma warning(suppress : 4996)
            SDL_itoa(640, buffer2, 10);
            strcat(buffer, buffer2);
            strcat(buffer, str_error_handler[5]);
            break;
        case le_alloc_fail:
            strcat(buffer, str_error_handler[6]);
            break;
        case le_reload_fail:
            strcat(buffer, str_error_handler[7]);
#pragma warning(suppress : 4996)
            SDL_itoa(pages, buffer2, 10);
            strcat(buffer, buffer2);
            strcat(buffer, str_error_handler[8]);
            break;
        case 6:
            break;
        case 7:
            strcat(buffer, str_error_handler[9]);
            break;
        case 8:
            strcat(buffer, str_error_handler[10]);
            break;
        case 9:
            strcat(buffer, str_error_handler[11]);
            break;
        case 10:
            strcat(buffer, str_error_handler[13]);
            break;
        case 11:
            strcpy(buffer, file_name);
            strcat(buffer, ".LBX");
            strcat(buffer, str_error_handler[14]);
            break;
        case 12:
            strcat(buffer, str_error_handler[15]);
            break;
        case 13:
            strcat(buffer, str_error_handler[16]);
            break;
        case 14:
            strcat(buffer, str_error_handler[14]);
            strcat(buffer, str_error_handler[16]);
        case 15:
            strcat(buffer, str_error_handler[17]);
            strcat(buffer, str_error_handler[14]);
            strcat(buffer, str_error_handler[16]);
            break;
        case 16:
            strcat(buffer, str_error_handler[18]);
            break;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] Exit_With_Message(buffer): %s\n", __FILE__, __LINE__, buffer);
#endif

    Exit_With_Message(buffer);

#if defined(__DOS16__)
    Exit(buffer);
#endif

}

// WZD s10p16
void File_Name_Base(char * file_name)
{
    int16_t itr;

    itr = 0;
    while(file_name[itr] != '\0')
    {
        if(file_name[itr] >= 'a') { file_name[itr] = (file_name[itr] - 32); }
        if(file_name[itr] == '.') { file_name[itr] = '\0'; }
        itr++;
    }

}

// WZD s10p17
// AKA  RAM_SetMinKB()
void RAM_Set_Minimum(int16_t amount)
{
    RAM_MinKbytes = amount;
}
