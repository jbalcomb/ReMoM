
#include "MoX_TYPE.H"   /* MoX_SAMB_ptr, etc. */
#include "MoX_FAR.H"    /* MoX_MK_FP(), etc. */
#include "MoX_DEF.H"    /* MoX_ST_FAILURE, etc. */
#include "MoX_BITS.H"   /* MoX_GET_2B_OFS() etc. */

#include "MoX_LBX.H"

#include "MoX_DBG.H"    /* Check_Release_Version() */
#include "MoX_SA.H"     /* Allocate_Space(); SA_GET_USED(); */

#include "ST_EXIT.H"    /* Exit() */

#ifdef STU_DEBUG
#include "STU_DBG.H"  /* DLOG() */
#endif

#if defined(__DOS16__)
#include <ALLOC.H>      /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <STDLIB.H>     /* itoa() */
#include <STRING.H>     /* strcat(), strcpy() */
#endif
#if defined(__WIN32__)
#include <malloc.h>     /* malloc() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */
#endif



int16_t MoX_lbxload_lbx_header_flag = MoX_ST_FALSE;
FILE * MoX_lbxload_fptr;
char MoX_lbxload_lbx_file_extension[] = ".LBX";
int16_t MoX_lbxload_num_entries;
MoX_SAMB_ptr MoX_lbxload_lbx_header;
char MoX_lbxload_lbx_name[16];


/*
    Unitialized Data
*/
uint16_t MoX_RAM_Min_KB;                    // MGC dseg:A5C4  ; set to 583 in _main


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
MoX_SAMB_ptr LBX_Load(char * lbx_name, int16_t entry_num)
{
    return MoX_LBX_Load_Entry(lbx_name, entry_num, MoX_ST_NULL, sa_Single);
}
// MGC s10p02
MoX_SAMB_ptr LBX_Reload(char * lbx_name, int16_t entry_num, MoX_SAMB_ptr SAMB_head)
{
    return MoX_LBX_Load_Entry(lbx_name, entry_num, SAMB_head, sa_First);
}
// MGC s10p03
MoX_SAMB_ptr LBX_Reload_Next(char * lbx_name, int16_t entry_num, MoX_SAMB_ptr SAMB_head)
{
    return MoX_LBX_Load_Entry(lbx_name, entry_num, SAMB_head, sa_Next);
}



// MGC s10p10
MoX_SAMB_ptr MoX_LBX_Load_Entry(char * lbx_name, int16_t entry_num, MoX_SAMB_ptr SAMB_head, int16_t allocation_type)
{
    uint16_t num_blocks;
    uint16_t read_size;
    uint32_t entry_start;
    uint32_t entry_end;
    uint32_t entry_length;
    char lbx_file_name[20];
    SAMB_ptr SAMB_data;  // MoO2 current_seg
    SAMB_ptr rvr_SAMB_data;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_LBX_Load_Entry(lbx_name = %s, entry_num = %d, SAMB_head = %p, allocation_type = %d)\n", __FILE__, __LINE__, lbx_name, entry_num, SAMB_head, allocation_type);
#endif

    // if(entry_num < 0) { LBX_Error(lbx_name, 1, entry_num, NULL); }

    // ? MoO2 ? lbx_header alloc ?
    if(MoX_lbxload_lbx_header_flag == MoX_ST_FALSE)
    {
        DLOG("(MoX_lbxload_lbx_header_flag == MoX_ST_FALSE)");
        MoX_lbxload_lbx_header_flag = MoX_ST_TRUE;
        MoX_lbxload_lbx_header = (MoX_SAMB_ptr)Allocate_Space_No_Header(MoX_SZ_LBX_HDR_PR);
    }

    File_Name_Base(lbx_name);


#if defined(__DOS16__)
    SAMB_data = (SAMB_ptr)MoX_MK_FP(EMM_LBX_Load_Entry(lbx_name, entry_num, (SAMB_addr)MoX_FP_SEG(SAMB_head), allocation_type, 0),0);
    if(SAMB_data != MoX_ST_FAILURE)
    {
        goto Done;
    }
#endif


    /*
        BEGIN: Current vs. Previous
    */
    if((MoX_lbxload_fptr == NULL) || (stricmp(lbx_name, MoX_lbxload_lbx_name) != 0))
    {
        DLOG("((MoX_lbxload_fptr == NULL) || (stricmp(lbx_name, MoX_lbxload_lbx_name) != 0))");
        /*
            BEGIN: Current != Previous
        */

        if(MoX_lbxload_fptr != NULL) { fclose(MoX_lbxload_fptr); }

        strcpy(MoX_lbxload_lbx_name, lbx_name);
        strcpy(lbx_file_name, lbx_name);
        strcat(lbx_file_name, ".LBX");

        MoX_lbxload_fptr = fopen(lbx_file_name, "rb");

        fread(MoX_lbxload_lbx_header, 1, MoX_SZ_LBX_HDR_B, MoX_lbxload_fptr);

        // if(MoX_LBX_GET_MAGIC_NUMBER(MoX_lbxload_lbx_header) != MoX_LBX_MAGSIG) { MoX_Error_Handler(lbx_name, 7, entry_num, NULL); }

        MoX_lbxload_num_entries = MoX_LBX_GET_NUM_ENTRIES(MoX_lbxload_lbx_header);


        /*
            END: Current != Previous
        */
    }
    if(MoX_lbxload_num_entries < entry_num) { Error_Handler(lbx_name, 8, entry_num, NULL); }
    /*
        END: Current vs. Previous
    */


    /*
        BEGIN: Entry - Offset Start, End, Length
    */
    entry_start = ( MoX_GET_4B_OFS( (MoX_lbxload_lbx_header), ( 8 + ((entry_num) * 4)    ) ) );
    entry_end   = ( MoX_GET_4B_OFS( (MoX_lbxload_lbx_header), ( 8 + ((entry_num) * 4) + 4) ) );
    entry_length = entry_end - entry_start;
    fseek(MoX_lbxload_fptr, entry_start, 0);
    /*
        END: Entry - Offset Start, End, Length
    */


    /*
        BEGIN: Allocation Type
    */
    num_blocks = 1 + (entry_length / MoX_SZ_PARAGRAPH_B);
    switch(allocation_type)
    {
        case 0:
            SAMB_data = Allocate_Space_No_Header(num_blocks);
            if(SAMB_data == NULL) { Error_Handler(lbx_name, 3, entry_num, NULL); }
            break;
        case 1:
            if(Check_Allocation(SAMB_head) == MoX_ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, NULL); };
            if(num_blocks > (MoX_SA_GET_SIZE(SAMB_head) - 1)) { Error_Handler(lbx_name, 4, entry_num, (num_blocks - (MoX_SA_GET_SIZE(SAMB_head) + 1))); }
            // SAMB_data = (SAMB_ptr)PTR_ADD_PARAGRAPH(SAMB_head, 1);
#if defined(__DOS16__)
            SAMB_data = (MoX_SAMB_ptr)MoX_MK_FP((MoX_FP_SEG(SAMB_head) + 1), 0);
#endif
#if defined(__WIN32__)
            SAMB_data = SAMB_head + 12;
#endif
            MoX_SA_SET_USED(SAMB_head, (num_blocks + 1));
            break;
        case 2:
            if(Check_Allocation(SAMB_head) == 0) { Error_Handler(lbx_name, 2, entry_num, NULL); };
            if(num_blocks > Get_Free_Blocks(SAMB_head)) { Error_Handler(lbx_name, 5, entry_num, (num_blocks - Get_Free_Blocks(SAMB_head))); }
            // SAMB_data = (SAMB_ptr)PTR_ADD_PARAGRAPH(SAMB_head, SA_Get_Used(SAMB_head));
#if defined(__DOS16__)
            SAMB_data = (MoX_SAMB_ptr)MoX_MK_FP((MoX_FP_SEG(SAMB_head) + 1), 0);
#endif
#if defined(__WIN32__)
            SAMB_data = SAMB_head + 12;
#endif
            MoX_SA_SET_USED(SAMB_head, (num_blocks + MoX_SA_GET_USED(SAMB_head)));
            break;
    }
    /*
        END: Allocation Type
    */


    /*
        BEGIN: Read Data
    */
    rvr_SAMB_data = SAMB_data;
    read_size = MoX_SZ_32K_B;
    while(entry_length >= read_size)
    {
        entry_length -= read_size;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        fread(rvr_SAMB_data, read_size, 1, MoX_lbxload_fptr);
        rvr_SAMB_data += read_size;
    }
    if(entry_length > 0)
    {
        read_size = entry_length;
        // if ( lbx_read_sgmt(current_seg, read_size, lbxload_fhnd) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); }
        fread(rvr_SAMB_data, read_size, 1, MoX_lbxload_fptr);
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
    dbg_prn("DEBUG: [%s, %d] END: MoX_LBX_Load_Entry(lbx_name = %s, entry_num = %d, SAMB_head = %p, allocation_type = %d) { SAMB_data = %p }\n", __FILE__, __LINE__, lbx_name, entry_num, SAMB_head, allocation_type, SAMB_data);
#endif

    return SAMB_data;
}

// MGC s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages)
{
    char buffer[120];
    char buffer2[20];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_Error_Handler(file_name = %s, error_num = %d, entry_num = %d, pages = %d)\n", __FILE__, __LINE__, file_name, error_num, entry_num, pages);
#endif

#ifdef STU_DEBUG
    // strcpy(buffer, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");  // 120 characters
    strcpy(buffer, "0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_");  // 110 characters
    dbg_prn("DEBUG: [%s, %d] buffer: %s\n", __FILE__, __LINE__, buffer);
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
            itoa(MoX_RAM_Min_KB, buffer2, 10);
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
    dbg_prn("DEBUG: [%s, %d] END: MoX_Error_Handler(file_name = %s, error_num = %d, entry_num = %d, pages = %d)\n", __FILE__, __LINE__, file_name, error_num, entry_num, pages);
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
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_File_Name_Base(file_name = %s)\n", __FILE__, __LINE__, file_name);
#endif
    itr = 0;
    while(file_name[itr] != '\0')
    {
        if(file_name[itr] >= 'a') { file_name[itr] = file_name[itr] - 32; }
        if(file_name[itr] == '.') { file_name[itr] = '\0'; }
        itr++;
    }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: MoX_File_Name_Base(file_name = %s)\n", __FILE__, __LINE__, file_name);
#endif
}

// MGC s10p17
void MoX_RAM_SetMinKB(int16_t Min_KB)
{
    MoX_RAM_Min_KB = Min_KB;
}
