
#include "MoX_TYPE.H"   /* SAMB_ptr, etc. */
#include "MoX_DEF.H"    /* ST_FAILURE, etc. */
#include "MoX_BITS.H"   /* GET_2B_OFS() etc. */

#include "LBX_Load.H"
#include "Allocate.H"   /* Allocate_Space(); SA_GET_USED(); */

#include "MoX_DBG.H"    /* Check_Release_Version() */

#ifdef STU_DEBUG
#include "STU_DBG.H"    /* DLOG() */
#endif

#include <malloc.h>     /* malloc() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */


int16_t lbxload_lbx_header_flag = ST_FALSE;
FILE * lbxload_fptr;
char lbxload_lbx_file_extension[] = ".LBX";
int16_t lbxload_num_entries;
SAMB_ptr lbxload_lbx_header;
char lbxload_lbx_name[16];



char * str_error_handler[] =
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
    return LBX_Load_Library_Data(lbx_name, entry_num, ST_NULL, sa_Single, start_rec, num_recs, record_size);
}
// WZD s10p05
// MoO2 Far_Reload_Data
SAMB_ptr LBX_Reload_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t start_rec, int16_t num_recs, int16_t record_size)
{
    return LBX_Load_Library_Data(lbx_name, entry_num, SAMB_head, sa_First, start_rec, num_recs, record_size);
}
// WZD s10p06
// MoO2 Far_Reload_Next_Data
SAMB_ptr LBX_Reload_Next_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t start_rec, int16_t num_recs, int16_t record_size)
{
    return LBX_Load_Library_Data(lbx_name, entry_num, SAMB_head, sa_Next, start_rec, num_recs, record_size);
}

// MGC s10p10
SAMB_ptr LBX_Load_Entry(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type)
{
    uint16_t num_blocks;
    uint16_t num_blocks_used;
    uint16_t read_size;
    uint32_t entry_start;
    uint32_t entry_end;
    uint32_t entry_length;
    char lbx_file_name[20];
    SAMB_ptr SAMB_data;
    SAMB_ptr rvr_SAMB_data;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LBX_Load_Entry(lbx_name = %s, entry_num = %d, SAMB_head = %p, allocation_type = %d)\n", __FILE__, __LINE__, lbx_name, entry_num, SAMB_head, allocation_type);
#endif

    // if(entry_num < 0) { LBX_Error(lbx_name, 1, entry_num, NULL); }

    if(lbxload_lbx_header_flag == ST_FALSE)
    {
        DLOG("(lbxload_lbx_header_flag == ST_FALSE)");
        lbxload_lbx_header_flag = ST_TRUE;
        lbxload_lbx_header = (SAMB_ptr)Allocate_Space_No_Header(SZ_LBX_HDR_PR);
    }

    File_Name_Base(lbx_name);

    /*
        BEGIN: Current vs. Previous
    */
    if((lbxload_fptr == NULL) || (stricmp(lbx_name, lbxload_lbx_name) != 0))
    {
        DLOG("((lbxload_fptr == NULL) || (stricmp(lbx_name, lbxload_lbx_name) != 0))");
        /*
            BEGIN: Current != Previous
        */

        if(lbxload_fptr != NULL) { fclose(lbxload_fptr); }

        strcpy(lbxload_lbx_name, lbx_name);
        strcpy(lbx_file_name, lbx_name);
        strcat(lbx_file_name, ".LBX");

        lbxload_fptr = fopen(lbx_file_name, "rb");

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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: entry_start: %08X\n", __FILE__, __LINE__, entry_start);
    dbg_prn("DEBUG: [%s, %d]: entry_end: %08X\n", __FILE__, __LINE__, entry_end);
    dbg_prn("DEBUG: [%s, %d]: entry_length: %u\n", __FILE__, __LINE__, entry_length);
#endif

    fseek(lbxload_fptr, entry_start, 0);
    /*
        END: Entry - Offset Start, End, Length
    */


    /*
        BEGIN: Allocation Type
    */
    num_blocks = 1 + (entry_length / SZ_PARAGRAPH_B);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: allocation_type: %d\n", __FILE__, __LINE__, allocation_type);
#endif
    switch(allocation_type)
    {
        case 0:
            DLOG("switch(allocation_type) case 0:");
            SAMB_data = Allocate_Space_No_Header(num_blocks);
            if(SAMB_data == NULL) { Error_Handler(lbx_name, 3, entry_num, ST_NULL); }
            break;
        case 1:
            DLOG("switch(allocation_type) case 1:");
            if(Check_Allocation(SAMB_head) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, ST_NULL); };
            if(num_blocks > (SA_GET_SIZE(SAMB_head) - 1)) { Error_Handler(lbx_name, 4, entry_num, (num_blocks - (SA_GET_SIZE(SAMB_head) + 1))); }
            SAMB_data = SAMB_head + 12;
            SA_SET_USED(SAMB_head, (num_blocks + 1));
            break;
        case 2:
            DLOG("switch(allocation_type) case 2:");
            if(Check_Allocation(SAMB_head) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, ST_NULL); };
            if(num_blocks > Get_Free_Blocks(SAMB_head)) { Error_Handler(lbx_name, 5, entry_num, (num_blocks - Get_Free_Blocks(SAMB_head))); }
            SAMB_data = SAMB_head + 12 + (16 * SA_GET_USED(SAMB_head));
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: num_blocks: %u\n", __FILE__, __LINE__, num_blocks);
//     dbg_prn("DEBUG: [%s, %d]: SA_GET_USED(SAMB_head)): %u\n", __FILE__, __LINE__, SA_GET_USED(SAMB_head));
//     dbg_prn("DEBUG: [%s, %d]: num_blocks + SA_GET_USED(SAMB_head)): %u\n", __FILE__, __LINE__, (num_blocks + SA_GET_USED(SAMB_head)));
// #endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: num_blocks + SA_GET_USED(SAMB_head)): %04X\n", __FILE__, __LINE__, (num_blocks + SA_GET_USED(SAMB_head)));
// 
//     uint16_t new_used_blocks;
//     uint8_t new_used_blocks_hi;
//     uint8_t new_used_blocks_lo;
// 
//     new_used_blocks = num_blocks + SA_GET_USED(SAMB_head);
// 
//     new_used_blocks_hi = new_used_blocks;
//     new_used_blocks_lo = new_used_blocks >> 8;
// 
//     dbg_prn("DEBUG: [%s, %d]: new_used_blocks: %04X\n", __FILE__, __LINE__, new_used_blocks);
//     dbg_prn("DEBUG: [%s, %d]: new_used_blocks_hi: %02X\n", __FILE__, __LINE__, new_used_blocks_hi);
//     dbg_prn("DEBUG: [%s, %d]: new_used_blocks_lo: %02X\n", __FILE__, __LINE__, new_used_blocks_lo);
// 
// 
//             // FAILURE: 
//             // SA_SET_USED(SAMB_head, (num_blocks + SA_GET_USED(SAMB_head)));
//             *( (SAMB_head) + (SAMB_USED) + 0 ) = ( (new_used_blocks)      );
//             *( (SAMB_head) + (SAMB_USED) + 1 ) = ( (new_used_blocks) >> 8 );
// 
// // #define SET_2B_OFS(_ptr_,_ofs_,_val_) (        \
// // *( (_ptr_) + (_ofs_) + 0 ) = ( (_val_)      ), \
// // *( (_ptr_) + (_ofs_) + 1 ) = ( (_val_) >> 8 )  \
// // )
// #endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: num_blocks: %u\n", __FILE__, __LINE__, num_blocks);
    dbg_prn("DEBUG: [%s, %d]: SA_GET_USED(SAMB_head)): %u\n", __FILE__, __LINE__, SA_GET_USED(SAMB_head));
    dbg_prn("DEBUG: [%s, %d]: num_blocks + SA_GET_USED(SAMB_head)): %u\n", __FILE__, __LINE__, (num_blocks + SA_GET_USED(SAMB_head)));
#endif
            num_blocks_used = num_blocks + SA_GET_USED(SAMB_head);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: num_blocks_used: %u\n", __FILE__, __LINE__, num_blocks_used);
#endif
            *( (SAMB_head) + (SAMB_USED) + 0 ) = ( (num_blocks_used)      );
            *( (SAMB_head) + (SAMB_USED) + 1 ) = ( (num_blocks_used) >> 8 );

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: SA_GET_USED(SAMB_head)): %u\n", __FILE__, __LINE__, SA_GET_USED(SAMB_head));
#endif
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
    while(entry_length >= read_size)
    {
        entry_length -= read_size;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
        rvr_SAMB_data += read_size;
    }
    if(entry_length > 0)
    {
        read_size = entry_length;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
    }
    /*
        END: Read Data
    */


    // Update_MemFreeWorst_KB();
    // Check_Free();

#if defined(__DOS16__)
Done:
#endif

#ifdef STU_DEBUG    
    dbg_prn("DEBUG: [%s, %d] END: LBX_Load_Entry(lbx_name = %s, entry_num = %d, SAMB_head = %p, allocation_type = %d) { SAMB_data = %p }\n", __FILE__, __LINE__, lbx_name, entry_num, SAMB_head, allocation_type, SAMB_data);
#endif

    return SAMB_data;
}




// WZD s10p11
SAMB_ptr LBX_Load_Library_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type, uint16_t start_rec, uint16_t num_recs, uint16_t record_size)
{
    SAMB_ptr SAMB_data;

#ifdef STU_DEBUG    
    dbg_prn("DEBUG: [%s, %d] BEGIN: LBX_Load_Library_Data(lbx_name = %s, entry_num = %d, SAMB_head = %p, allocation_type = %d, start_rec = %u, num_recs = %u, record_size = %u)\n", __FILE__, __LINE__, lbx_name, entry_num, SAMB_head, allocation_type, start_rec, num_recs, record_size);
#endif



    SAMB_data = ST_NULL;



#ifdef STU_DEBUG    
    dbg_prn("DEBUG: [%s, %d] END: LBX_Load_Library_Data(lbx_name = %s, entry_num = %d, SAMB_head = %p, allocation_type = %d, start_rec = %u, num_recs = %u, record_size = %u) { SAMB_data = %p }\n", __FILE__, __LINE__, lbx_name, entry_num, SAMB_head, allocation_type, start_rec, num_recs, record_size, SAMB_data);
#endif

    return SAMB_data;
}




// MGC s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages)
{
    char buffer[120];
    char buffer2[20];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Error_Handler(file_name = %s, error_num = %d, entry_num = %d, pages = %d)\n", __FILE__, __LINE__, file_name, error_num, entry_num, pages);
#endif

    strcpy(buffer, file_name);
    itoa(entry_num, buffer2, 10);
    strcat(buffer, str_error_handler[0]);
    strcat(buffer, buffer2);
    strcat(buffer, str_error_handler[1]);

    switch (error_num)
    {
        case 1:
            strcat(buffer, str_error_handler[2]);
            break;
        case 2:
            strcat(buffer, str_error_handler[3]);
            break;
        case 3:
            strcpy(buffer, str_error_handler[4]);
            itoa(640, buffer2, 10);
            strcat(buffer, buffer2);
            strcat(buffer, str_error_handler[5]);
            break;
        case 4:
            strcat(buffer, str_error_handler[6]);
            break;
        case 5:
            strcat(buffer, str_error_handler[7]);
            itoa(pages, buffer2, 10);
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
    dbg_prn("DEBUG: [%s, %d] buffer: %s\n", __FILE__, __LINE__, buffer);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Error_Handler(file_name = %s, error_num = %d, entry_num = %d, pages = %d)\n", __FILE__, __LINE__, file_name, error_num, entry_num, pages);
#endif

    // Exit_With_Message(buffer);
#if defined(__DOS16__)
    Exit(buffer);
#endif

}

// MGC s10p16
void File_Name_Base(char * file_name)
{
    int16_t itr;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: File_Name_Base(file_name = %s)\n", __FILE__, __LINE__, file_name);
#endif
    itr = 0;
    while(file_name[itr] != '\0')
    {
        if(file_name[itr] >= 'a') { file_name[itr] = file_name[itr] - 32; }
        if(file_name[itr] == '.') { file_name[itr] = '\0'; }
        itr++;
    }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: File_Name_Base(file_name = %s)\n", __FILE__, __LINE__, file_name);
#endif
}
