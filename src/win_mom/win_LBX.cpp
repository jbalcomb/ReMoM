
#include "win_TYPE.hpp"
#include "win_DEF.hpp"

#include "win_LBX.hpp"
#include "win_SA.hpp"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <string.h>     /* strcat(), strcpy(), stricmp() */


SAMB_ptr LBX_Load_Entry(char * lbx_name, short entry_num, SAMB_ptr base_seg, short allocation_type);


enum e_LBX_Allocation_Type
{
    sa_single,
    sa_first,
    sa_next,
    sa_invalid
};


int lbxload_lbx_header_flag = ST_FALSE;
FILE * lbxload_fptr = NULL;
char secondary_drive_path[50];  // MoO2  ORION2.LE  dseg02:0017C544
char lbxload_lbx_file_extension[] = ".LBX";
int lbxload_num_entries;
SAMB_ptr lbxload_lbx_header;  // lbx_header {entries, magic_number, file_type, dummy, offsets[510]}
char lbxload_lbx_name[16];




// MGC s10p01
SAMB_ptr LBX_Load(char * lbx_name, short entry_num)
{
    return LBX_Load_Entry(lbx_name, entry_num, NULL, sa_single);
}
// MGC s10p02
SAMB_ptr LBX_Reload(char * lbx_name, short entry_num, SAMB_ptr SAMB_head)
{
    return LBX_Load_Entry(lbx_name, entry_num, SAMB_head, sa_first);
}
// MGC s10p03
SAMB_ptr LBX_Reload_Next(char * lbx_name, short entry_num, SAMB_ptr SAMB_head)
{
    return LBX_Load_Entry(lbx_name, entry_num, SAMB_head, sa_next);
}


// MGC s10p10
SAMB_ptr LBX_Load_Entry(char * lbx_name, short entry_num, SAMB_ptr base_seg, short allocation_type)
{
    unsigned short num_blocks;
    unsigned short read_size;
    signed int entry_start;
    signed int entry_end;
    signed int entry_length;
    char lbx_file_name[20];
    SAMB_ptr SAMB_data;
    SAMB_ptr rvr_SAMB_data;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LBX_Load_Entry(lbx_name = %s, entry_num = %d, base_seg = %p, allocation_type = %d)\n", __FILE__, __LINE__, lbx_name, entry_num, base_seg, allocation_type);
#endif

    if(lbxload_lbx_header_flag == ST_FALSE)
    {
        lbxload_lbx_header_flag = ST_TRUE;
        lbxload_lbx_header = Allocate_Space_No_Header(SZ_LBX_HDR_PR);
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
        if(lbxload_fptr == NULL)
        {
            DLOG("FATAL: (lbxload_fptr == NULL)");
        }

        fread(lbxload_lbx_header, 1, SZ_LBX_HDR_B, lbxload_fptr);

#ifdef STU_DEBUG
        dbg_prn("LBX_GET_MAGICNUMBER(lbxload_lbx_header): 0x%04X\n", LBX_GET_MAGICNUMBER(lbxload_lbx_header));
#endif
        // if(LBX_GET_MAGICNUMBER(lbxload_lbx_header) != LBX_MAGSIG) { Error_Handler(lbx_name, 7, entry_num, NULL); }


#ifdef STU_DEBUG
        dbg_prn("LBX_GET_NUMENTRIES(lbxload_lbx_header): 0x%04X\n", LBX_GET_NUMENTRIES(lbxload_lbx_header));
#endif
        lbxload_num_entries = LBX_GET_NUMENTRIES(lbxload_lbx_header);
#ifdef STU_DEBUG
        dbg_prn("lbxload_num_entries: %d\n" , lbxload_num_entries);
#endif
        /*
            END: Current != Previous
        */
    }
    // if ( lbxload_num_entries < entry_num ) { Error_Handler(lbx_name, 8, entry_num, NULL); }
    /*
        END: Current vs. Previous
    */

    /*
        BEGIN: Entry - Offset Begin, End, Size
    */
    entry_start = LBX_GET_ENTRY_START(lbxload_lbx_header, entry_num);
    entry_end = LBX_GET_ENTRY_END(lbxload_lbx_header, entry_num);
    entry_length = entry_end - entry_start;
    // if(fseek(lbxload_fptr, entry_start, 0) == ST_FAILURE) { Error_Handler(lbx_name, 2, entry_num, NULL); }
    fseek(lbxload_fptr, entry_start, 0);
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
            // if(Check_Allocation(base_seg) == ST_FAILURE ) { Error_Handler(lbx_name, 2, entry_num, NULL); };
            // if(num_blocks > (SA_Get_Size(base_seg) - 1)) { Error_Handler(lbx_name, 4, entry_num, (num_blocks - (SA_Get_Size(base_seg) + 1))); }
            SAMB_data = base_seg + 12;
            SA_SET_USED(base_seg, (num_blocks + 1));
            break;
        case 2:
            // if ( Check_Allocation(base_seg) == 0 ) { Error_Handler(lbx_name, 2, entry_num, NULL); };
            // if ( num_blocks > Get_Free_Blocks(base_seg) ) { Error_Handler(lbx_name, 5, entry_num, (num_blocks - Get_Free_Blocks(base_seg))); }
            SAMB_data = base_seg + ( (12 * 1) + (16 * SA_GET_USED(base_seg)) );
            SA_SET_USED(base_seg, (num_blocks + SA_GET_USED(base_seg)));
            break;
    }

    rvr_SAMB_data = SAMB_data;
    read_size = SZ_32K_B;
    while ( entry_length >= read_size )
    {
        entry_length -= read_size;
        fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
        rvr_SAMB_data += read_size;
    }
    if ( entry_length > 0 )
    {
        read_size = entry_length;
        fread(rvr_SAMB_data, read_size, 1, lbxload_fptr);
    }


#ifdef STU_DEBUG    
    dbg_prn("DEBUG: [%s, %d] END: LBX_Load_Entry(lbx_name = %s, entry_num = %d, base_seg = %p, allocation_type = %d) { SAMB_data = %p }\n", __FILE__, __LINE__, lbx_name, entry_num, base_seg, allocation_type, SAMB_data);
#endif

    return SAMB_data;
}

void File_Name_Base(char * file_name)
{
    int itr;
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
