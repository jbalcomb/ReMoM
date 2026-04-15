/*
    WIZARDS.EXE
        seg012
        seg013

    Expanded Memory System (EMS)
    Expanded Memory Manager (EMM)
*/

#include "../../../ext/stu_compat.h"

#include "../Allocate.h"
#include "../DOS.h"
#include "../EXIT.h"
#include "../LBX_Load.h"
#include "../MOX_BASE.h"
#include "../MOX_TYPE.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EMS.h"
#include "EMS_EMU.h"



// WZD  dseg:763E                                                 BEGIN:  seg013 - Initialized Data  (EMM)

// WZD  dseg:763E
char str_VGAFILEH__seg013[] = "VGAFILEH";



uint8_t * EMS_PFBA = NULL;
char EMM_DevName[] = "EMMXXXX0";
struct s_EMM_L2P_MAP_RECORD EMM_Log2Phys_Map[4] = { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } };
int16_t EMM_Pages_Reserved = 40;
int16_t EMM_Open_Handles = 0;
char cnst_EMM_Hnd1[] = "YO MOMA";
char cnst_EMM_Hnd12[] = "YO MOMA";

static const char cnst_EMMErr_ResOut1[] = "EMM reserved exceeded by ";
static const char cnst_EMMErr_ResOut2[] = " blocks [";
static const char cnst_EMMErr_ResOut3[] = ".LBX]";
static const char cnst_EMMErr_Reload1[] = " reloaded into EMM, diff size =";
static const char cnst_EMMErr_Space[] = "  ";
static const char str_LBX_File_Too_Long[] = "EMM 2long";
static const char cnst_EMMErr_TooLow1[] = "You must have at least ";
static const char cnst_EMMErr_TooLow2[] = "K of expanded memory.";

static int16_t EMMDATAH_last_offset;
EMM_HANDLE g_EmmHndl_OVERXYZ = EMM_HANDLE_INVALID;
static int16_t EMMDATAH_offset;
EMM_HANDLE _EMMDATAH_seg = EMM_HANDLE_INVALID;
EMM_HANDLE _VGAFILEH_seg = EMM_HANDLE_INVALID;
int16_t EmmHndlNbr_YOMOMA = 0;
int16_t EMM_OK;
struct s_EMM_RECORD EMM_Table[40];
static int16_t m_min_expanded_memory;



int16_t EMM_Detect_Device(void)
{
    if (EMS_EMU_IsAttached() == ST_FALSE)
    {
        return ST_FAILURE;
    }
    EMS_PFBA = EMS_EMU_Get_Page_Frame_Pointer();
    return ST_SUCCESS;
}


int16_t EMM_Number_Of_Handles(void)
{
    return EMS_EMU_GetHandleCount();
}


int16_t EMM_Get_Free_Page_Count(void)
{
    return EMS_EMU_GetFreePageCount(m_min_expanded_memory);
}


int16_t EMM_GetPageCount(EMM_HANDLE emm_handle)
{
    return EMS_EMU_GetPagesOwnedByHandle(emm_handle);
}


EMM_HANDLE EMM_MakeNamedHandle(int16_t pages, const char * name)
{
    return EMS_EMU_AllocHandle(pages, name);
}


int16_t EMM_Get_Handle_Name(char * handle_name, EMM_HANDLE emm_handle)
{
    return EMS_EMU_CopyHandleName(emm_handle, handle_name);
}


void EMM_Release_Handle(EMM_HANDLE handle)
{
    if (handle != EMM_HANDLE_INVALID) {
        EMS_EMU_ReleaseHandleAndMemory(handle);
    }
}


void EMM_MapnRead(void * dst_ptr, uint32_t data_offset, uint16_t nbytes, EMM_HANDLE emm_handle_number)
{
    EMS_EMU_Mapped_Read(dst_ptr, data_offset, nbytes, emm_handle_number);
}


void EMM_MapnWrite(void * src_ptr, uint32_t data_offset, uint16_t nbytes, EMM_HANDLE emm_handle_number)
{
    EMS_EMU_Mapped_Write(src_ptr, data_offset, nbytes, emm_handle_number);
}


void * EMM_Get_Page_Frame(void)
{
    return EMS_EMU_Get_Page_Frame_Pointer();
}


void EMM_MapFourPages(EMM_HANDLE emm_handle, uint16_t emm_page)
{
    EMS_PFBA = EMS_EMU_MapHandleIntoPageFrame(emm_handle, emm_page);
    if(EMS_PFBA == NULL)
    {
        EMS_PFBA = EMS_EMU_Get_Page_Frame_Pointer();
        return;
    }
    EMM_Log2Phys_Map[0].Logical_Page = emm_page;
    EMM_Log2Phys_Map[0].Physical_Page = 0;
    EMM_Log2Phys_Map[1].Logical_Page = emm_page + 1;
    EMM_Log2Phys_Map[1].Physical_Page = 1;
    EMM_Log2Phys_Map[2].Logical_Page = emm_page + 2;
    EMM_Log2Phys_Map[2].Physical_Page = 2;
    EMM_Log2Phys_Map[3].Logical_Page = emm_page + 3;
    EMM_Log2Phys_Map[3].Physical_Page = 3;
}


void EMM_MapMulti4(EMM_HANDLE emm_handle, uint16_t emm_page)
{
    EMM_MapFourPages(emm_handle, emm_page);
}



/*
    WIZARDS.EXE seg013
*/

// WZD s13p01
void EMS_Startup(void)
{
    char buffer[100];
    int16_t open_handles;
    int16_t itr = 0;

    /* 1. Hardware Detection */
    if (EMM_Detect_Device() == ST_FAILURE) {
        EMM_Error_Message(buffer);
        Exit_With_Message(buffer);
        return;
    }
    
    /* 2. Crash Recovery & Handle Reclaiming */
    open_handles = EMM_Number_Of_Handles();
    EmmHndlNbr_YOMOMA = 0;
    for(itr = 1; itr < open_handles; itr++)
    {
        if(EmmHndlNbr_YOMOMA != 0)
        {
            break;
        }
        if(EMM_Get_Handle_Name(buffer, itr) == 0)
        {
            continue;
        }
        if(stu_stricmp(buffer, "YO MOMA") == 0)
        {
            EmmHndlNbr_YOMOMA = itr;
        }
    }

    EMM_Open_Handles = 0;

    if(EmmHndlNbr_YOMOMA == 0)
    {
        EmmHndlNbr_YOMOMA = EMM_Get_Handle(1, cnst_EMM_Hnd12, ST_FALSE);
    }
    else
    {
        stu_strcpy(EMM_Table[EMM_Open_Handles].name, cnst_EMM_Hnd1);
        EMM_Table[EMM_Open_Handles].reserved = ST_FALSE;
        EMM_Table[EMM_Open_Handles].handle = EmmHndlNbr_YOMOMA;
        EMM_Open_Handles = 1;
    }

    if(EmmHndlNbr_YOMOMA == 0)
    {
        EMM_Error_Message(buffer);
        Exit_With_Message(buffer);
        return;
    }

    EMM_OK = ST_TRUE;

    for(itr = (EmmHndlNbr_YOMOMA + 1); itr < 50; itr++)
    {
        if(EMM_Open_Handles >= 40)
        {
            break;
        }
        if(EMM_Get_Handle_Name(buffer, itr) == 0)
        {
            continue;
        }
        if(buffer[0] == '\0')
        {
            continue;
        }
        EMM_Table[EMM_Open_Handles].reserved = ST_FALSE;
        stu_strcpy(EMM_Table[EMM_Open_Handles].name, buffer);
        EMM_Table[EMM_Open_Handles].handle = itr;
        EMM_Open_Handles++;
    }

    /* 3. Minimum Memory Check */
    if(EMM_Get_Free_Page_Count() < EMM_Pages_Reserved)
    {
        EMM_Error_Message(buffer);
        Exit_With_Message(buffer);
        return;
    }
    
    /* 4. Core System Allocations */
    
    /* Allocate 5 logical pages (80KB) for the VGA Screen/Graphics Buffer.
       The '1' flag means it is permanently reserved. */
    _VGAFILEH_seg = EMM_Get_Handle(5, str_VGAFILEH__seg013, ST_TRUE);

    /* Allocate 4 logical pages (64KB) for the generic EMM Data Buffer */
    _EMMDATAH_seg = EMM_Get_Handle(4, "EMMDATAH", ST_TRUE);
    
    /* Initialize the data scratchpad pointer */
    EMMDATAH_offset = 0;

}

void EMM_Release_All(void)
{
    int16_t itr;

    for (itr = 0; itr < EMM_Open_Handles; itr++) {
        EMM_Release_Handle(EMM_Table[itr].handle);
    }
    if (g_EmmHndl_OVERXYZ != EMM_HANDLE_INVALID) {
        EMM_Release_Handle(g_EmmHndl_OVERXYZ);
    }
}

EMM_HANDLE EMM_Get_Handle(int16_t page_count, const char *handle_name, int16_t reserved_flag)
{
    char buffer[LEN_TEMP_STRING];
    int handle_idx;
    int itr;
    EMM_HANDLE emm_handle;

    if (handle_name == NULL || handle_name[0] == '\0') {
        return EMM_HANDLE_INVALID;
    }

    handle_idx = ST_UNDEFINED;

    for (itr = 0; itr < EMM_Open_Handles; itr++) {
        if (stu_stricmp(handle_name, EMM_Table[itr].name) == 0) {
            handle_idx = itr;
        }
    }

    if (handle_idx == ST_UNDEFINED) {
        if (EMM_Open_Handles >= 40) {
            Exit_With_Message("EMM handle table overflow");
            return EMM_HANDLE_INVALID;
        }

        strncpy(EMM_Table[EMM_Open_Handles].name, handle_name, 8);
        EMM_Table[EMM_Open_Handles].name[8] = '\0';

        if (reserved_flag == ST_TRUE && EMM_Table[EMM_Open_Handles].name[0] != '\0') {
            EMM_Table[EMM_Open_Handles].name[0] = (char)(-(signed char)EMM_Table[EMM_Open_Handles].name[0]);
        }

        emm_handle = EMM_MakeNamedHandle(page_count, EMM_Table[EMM_Open_Handles].name);
        if (emm_handle == EMM_HANDLE_INVALID) {
            return EMM_HANDLE_INVALID;
        }

        strncpy(EMM_Table[EMM_Open_Handles].name, handle_name, 8);
        EMM_Table[EMM_Open_Handles].name[8] = '\0';
        EMM_Table[EMM_Open_Handles].reserved = (char)reserved_flag;
        EMM_Table[EMM_Open_Handles].handle = emm_handle;
        EMM_Open_Handles++;

        if (reserved_flag == ST_TRUE) {
            EMM_Pages_Reserved -= page_count;
        }

        if (EMM_Pages_Reserved < 0) {
            stu_itoa(-EMM_Pages_Reserved, buffer, 10);
            stu_strcpy(near_buffer, cnst_EMMErr_ResOut1);
            stu_strcat(near_buffer, buffer);
            stu_strcat(near_buffer, cnst_EMMErr_ResOut2);
            stu_strcat(near_buffer, handle_name);
            stu_strcat(near_buffer, cnst_EMMErr_ResOut3);
            Exit_With_Message(near_buffer);
        }

        return emm_handle;
    }

    emm_handle = EMM_Table[handle_idx].handle;

    if (EMM_GetPageCount(emm_handle) < page_count) {
        stu_itoa(page_count, buffer, 10);
        stu_strcpy(near_buffer, handle_name);
        stu_strcat(near_buffer, cnst_EMMErr_Reload1);
        stu_strcat(near_buffer, cnst_EMMErr_Space);
        stu_strcat(near_buffer, buffer);
        Exit_With_Message(near_buffer);
    }

    return emm_handle;
}

void EMMDATAH_Map(void)
{
    EMM_MapFourPages(_EMMDATAH_seg, 0);
}

uint8_t * EMMDATAH_Allocate_First(uint16_t nparas)
{
    uint16_t free_paras;
    uint16_t total_nparas;
    uint8_t * header;

    total_nparas = (uint16_t)(nparas + 1);
    EMMDATAH_offset = 0;
    EMMDATAH_Map();
    header = EMS_PFBA + ((size_t)EMMDATAH_offset * SZ_PARAGRAPH_B);
    free_paras = (uint16_t)(4090 - EMMDATAH_offset);

    if (free_paras < total_nparas) {
        Allocation_Error(sa_Alloc_EMM_too_small, (uint16_t)(total_nparas - free_paras));
    }

    farpokew(header, SAMB_MEMSIG1, _SA_MEMSIG1);
    farpokew(header, SAMB_MEMSIG2, _SA_MEMSIG2);
    farpokew(header, SAMB_SIZE, (uint16_t)(total_nparas - 1));
    farpokew(header, SAMB_USED, 1);
    farpokew(header, SAMB_MARK, 1);

    EMMDATAH_offset += (int16_t)(total_nparas + 1);
    return header + SZ_PARAGRAPH_B;
}

uint8_t * EMMDATAH_Allocate_Next(uint16_t nparas)
{
    uint16_t free_paras;
    uint16_t total_nparas;
    uint8_t * header;

    EMMDATAH_Map();
    total_nparas = (uint16_t)(nparas + 1);
    header = EMS_PFBA + ((size_t)EMMDATAH_offset * SZ_PARAGRAPH_B);
    free_paras = (uint16_t)(4090 - EMMDATAH_offset);

    if (free_paras < total_nparas) {
        Allocation_Error(sa_Alloc_EMM_too_small, (uint16_t)(total_nparas - free_paras));
    }

    farpokew(header, SAMB_MEMSIG1, _SA_MEMSIG1);
    farpokew(header, SAMB_MEMSIG2, _SA_MEMSIG2);
    farpokew(header, SAMB_SIZE, (uint16_t)(total_nparas - 1));
    farpokew(header, SAMB_USED, 1);
    farpokew(header, SAMB_MARK, 1);

    EMMDATAH_offset += (int16_t)(total_nparas + 1);
    return header + SZ_PARAGRAPH_B;
}

void EMMDATAH_Save_Last_Offset(void)
{
    EMMDATAH_last_offset = EMMDATAH_offset;
}

void EMMDATAH_Restore_Last_Offset(void)
{
    EMMDATAH_offset = EMMDATAH_last_offset;
}

void UU_EMM_LBX_Load2Hnd(char * file_name, EMM_HANDLE emm_handle)
{
    char base_file_name[32] = {0};
    uint16_t handle_page_count;
    uint32_t file_length;
    uint16_t first_log_page;
    uint16_t bytes_to_copy;
    uint8_t * buffer_seg;
    uint16_t needed_pages;
    int16_t i;
    FILE * file_handle = NULL;

    strcpy(base_file_name, file_name);
    for (i = 0; i < 13; i++) {
        if (base_file_name[i] == '.') {
            base_file_name[i] = '\0';
            break;
        }
    }
    strcat(base_file_name, ".LBX");

    file_length = LOF(base_file_name);
    if (file_length == 0) {
        Error_Handler(file_name, le_emm_swap_fail, 0, ST_NULL);
    }

    needed_pages = (uint16_t)(file_length / SZ_EMM_LOGICAL_PAGE);
    if ((file_length % SZ_EMM_LOGICAL_PAGE) != 0) {
        needed_pages++;
    }

    handle_page_count = EMS_EMU_GetPagesOwnedByHandle((EMM_HANDLE)emm_handle);
    if (needed_pages > handle_page_count) {
        Exit_With_Message((char *)str_LBX_File_Too_Long);
    }

    first_log_page = 0;
    EMM_MapFourPages(emm_handle, first_log_page);
    buffer_seg = (uint8_t *)EMM_Get_Page_Frame();

    if (buffer_seg != NULL) {
        file_handle = fopen(file_name, "rb");
        if (file_handle != NULL) {
            bytes_to_copy = SZ_EMM_LOGICAL_PAGE;
            while (file_length >= SZ_EMM_LOGICAL_PAGE) {
                EMM_MapFourPages(emm_handle, first_log_page);
                buffer_seg = (uint8_t *)EMM_Get_Page_Frame();
                file_length -= SZ_EMM_LOGICAL_PAGE;
                lbx_read_sgmt(buffer_seg, bytes_to_copy, file_handle);
                first_log_page++;
            }

            if (file_length > 0) {
                bytes_to_copy = (uint16_t)file_length;
                EMM_MapFourPages(emm_handle, first_log_page);
                buffer_seg = (uint8_t *)EMM_Get_Page_Frame();
                lbx_read_sgmt(buffer_seg, bytes_to_copy, file_handle);
            }

            fclose(file_handle);
        }
    }
}

void EMM_Set_Minimum(int16_t amount)
{
    m_min_expanded_memory = amount;
}

void EMM_Error_Message(char * string)
{
    char temp[LEN_TEMP_STRING] = { 0 };
    stu_itoa(m_min_expanded_memory, temp, 10);
    stu_strcpy(string, cnst_EMMErr_TooLow1);
    stu_strcat(string, temp);
    stu_strcat(string, cnst_EMMErr_TooLow2);
}
