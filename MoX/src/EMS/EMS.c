/*
    MAGIC.EXE
        seg012
        seg013
    WIZARDS.EXE
        seg012
        seg013

NOTE(JimBalcomb,20260409): Last night, I started a new effort with Codex+GPT54 to build an EMS emulator. The new EMS folder is so I can give it access.
*/

#include "../../../ext/stu_compat.h"

#include "../Allocate.h"
#include "../EXIT.h"
#include "../MOX_BASE.h"
#include "../MOX_TYPE.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "EMS.h"



#define EMS_EMU_MAX_HANDLES     128
#define EMS_EMU_DEFAULT_PAGES   2048  /* 32MB default */
#define EMS_EMU_DATAH_PAGES     1024  /* 16MB default */

static const char cnst_EMMErr_ResOut1[] = "EMM reserved exceeded by ";
static const char cnst_EMMErr_ResOut2[] = " blocks [";
static const char cnst_EMMErr_ResOut3[] = ".LBX]";
static const char cnst_EMMErr_Reload1[] = " reloaded into EMM, diff size =";
static const char cnst_EMMErr_Space[]   = "  ";


// WZD dseg:760C                                                 BEGIN:  seg012 - Initialized Data

// ; contains the segment address of the EMS page frame
// WZD dseg:760C
SAMB_ptr EMS_PFBA = 0;

// WZD dseg:7617
struct s_EMM_L2P_MAP_RECORD EMM_Log2Phys_Map[4] =
{
               { 0, 0 },
               { 0, 1 },
               { 0, 2 },
               { 0, 3 }
};

// WZD dseg:7628
int16_t EMM_Pages_Reserved = 40;

// WZD dseg:762A
int16_t EMM_Open_Handles = 0;

// WZD dseg:76AC
char cnst_EMMErr_TooLow1[] = "You must have at least ";
// WZD dseg:76C4
char cnst_EMMErr_TooLow2[] = "K of expanded memory.";

// WZD dseg:763D                                                 END:  seg012 - Initialized Data


// WZD dseg:E5EA                                                 BEGIN:  seg013 - Uninitialized Data

static int16_t UU_EMMData_PrevLvl = 0;

int16_t g_EmmHndl_OVERXYZ = 0;
int16_t EMM_Data_Level = 0;
SAMB_ptr _EMMDATAH_seg = 0;
byte_ptr _VGAFILEH_seg = 0;
SAMB_ptr EmmHndlNbr_YOMOMA = 0;
int16_t EMM_OK = 0;

struct s_EMM_RECORD EMM_Table[40];
int16_t EMM_MinKB = 0;

// WZD dseg:E7DC                                                 END:  seg013 - Uninitialized Data


typedef struct s_EMS_HOST_HANDLE
{
    int in_use;
    SAMB_ptr base;
    int16_t pages;
    char name[9];
} s_EMS_HOST_HANDLE;

static s_EMS_HOST_HANDLE g_ems_handles[EMS_EMU_MAX_HANDLES];
static int32_t g_ems_total_pages = 0;

static int EMS_StrICmp(const char *a, const char *b)
{
    unsigned char ca;
    unsigned char cb;

    if (a == NULL && b == NULL) {
        return 0;
    }
    if (a == NULL) {
        return -1;
    }
    if (b == NULL) {
        return 1;
    }

    while (*a != '\0' || *b != '\0') {
        ca = (unsigned char)*a++;
        cb = (unsigned char)*b++;
        ca = (unsigned char)tolower(ca);
        cb = (unsigned char)tolower(cb);
        if (ca != cb) {
            return (int)ca - (int)cb;
        }
    }
    return 0;
}

static s_EMS_HOST_HANDLE *EMS_FindHandle(SAMB_ptr base)
{
    int idx;

    if (base == NULL) {
        return NULL;
    }

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (g_ems_handles[idx].in_use && g_ems_handles[idx].base == base) {
            return &g_ems_handles[idx];
        }
    }

    return NULL;
}

static SAMB_ptr EMS_AllocHandle(int16_t pages, const char *name)
{
    int idx;
    size_t alloc_size;
    SAMB_ptr base;

    if (pages <= 0) {
        return NULL;
    }

    alloc_size = (size_t)pages * SZ_EMM_LOGICAL_PAGE;
    base = (SAMB_ptr)calloc(1u, alloc_size);
    if (base == NULL) {
        return NULL;
    }

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (!g_ems_handles[idx].in_use) {
            g_ems_handles[idx].in_use = 1;
            g_ems_handles[idx].base = base;
            g_ems_handles[idx].pages = pages;
            g_ems_handles[idx].name[0] = '\0';
            if (name != NULL) {
                strncpy(g_ems_handles[idx].name, name, 8);
                g_ems_handles[idx].name[8] = '\0';
            }
            return base;
        }
    }

    free(base);
    return NULL;
}

static void EMS_FreeHandle(SAMB_ptr base)
{
    s_EMS_HOST_HANDLE *handle = EMS_FindHandle(base);

    if (handle == NULL) {
        return;
    }

    if (handle->base != NULL) {
        free(handle->base);
    }

    memset(handle, 0, sizeof(*handle));
}

static int32_t EMS_GetTotalPages(void)
{
    int32_t min_pages;

    if (g_ems_total_pages != 0) {
        return g_ems_total_pages;
    }

    min_pages = 0;
    if (EMM_MinKB > 0) {
        min_pages = (int32_t)EMM_MinKB / 16;
    }

    if (min_pages < EMS_EMU_DEFAULT_PAGES) {
        min_pages = EMS_EMU_DEFAULT_PAGES;
    }

    g_ems_total_pages = min_pages;
    return g_ems_total_pages;
}


/*
    WIZARDS.EXE seg012
*/

// WZD s12p01
int16_t EMM_DetectDevice(void)
{
    return ST_TRUE;
}

// WZD s12p02
int16_t EMM_GetHandleCount(void)
{
    int idx;
    int count = 0;

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (g_ems_handles[idx].in_use) {
            ++count;
        }
    }

    return (int16_t)count;
}

// WZD s12p03
int16_t EMM_GetFreePageCnt(void)
{
    int idx;
    int32_t used = 0;
    int32_t total = EMS_GetTotalPages();

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (g_ems_handles[idx].in_use) {
            used += g_ems_handles[idx].pages;
        }
    }

    if (used >= total) {
        return 0;
    }

    return (int16_t)(total - used);
}

// WZD s12p04
int16_t EMM_GetPageCount(SAMB_ptr emm_handle)
{
    s_EMS_HOST_HANDLE *handle = EMS_FindHandle(emm_handle);

    if (handle == NULL) {
        return 0;
    }

    return handle->pages;
}

// WZD s12p05
SAMB_ptr EMM_MakeNamedHandle(int16_t pages, const char *name)
{
    return EMS_AllocHandle(pages, name);
}

// WZD s12p06
int16_t EMM_GetHandleName(SAMB_ptr emm_handle, char *out_name)
{
    s_EMS_HOST_HANDLE *handle = EMS_FindHandle(emm_handle);

    if (handle == NULL || out_name == NULL) {
        return ST_FAILURE;
    }

    strncpy(out_name, handle->name, 8);
    out_name[8] = '\0';
    return ST_SUCCESS;
}

// WZD s12p07
void EMM_ReleaseHandle__SUTB(SAMB_ptr handle)
{
    if (handle != NULL) {
        EMS_FreeHandle(handle);
    }
}

// WZD s12p08
void EMM_MapnRead(uint16_t target_offset, SAMB_ptr target_seg, uint32_t emm_data_offset, uint16_t byte_count, SAMB_ptr emm_handle)
{
    SAMB_ptr source;
    SAMB_ptr dest;
    s_EMS_HOST_HANDLE *handle = EMS_FindHandle(emm_handle);
    size_t total_bytes;

    if (handle == NULL || handle->base == NULL || target_seg == NULL) {
        return;
    }

    total_bytes = (size_t)handle->pages * SZ_EMM_LOGICAL_PAGE;
    if ((size_t)emm_data_offset + byte_count > total_bytes) {
        return;
    }

    source = handle->base + emm_data_offset;
    dest = target_seg + target_offset;
    memcpy(dest, source, byte_count);
}

// WZD s12p09
void EMM_MapnWrite(uint16_t target_offset, SAMB_ptr target_seg, uint32_t emm_data_offset, uint16_t byte_count, SAMB_ptr emm_handle)
{
    SAMB_ptr source;
    SAMB_ptr dest;
    s_EMS_HOST_HANDLE *handle = EMS_FindHandle(emm_handle);
    size_t total_bytes;

    if (handle == NULL || handle->base == NULL || target_seg == NULL) {
        return;
    }

    total_bytes = (size_t)handle->pages * SZ_EMM_LOGICAL_PAGE;
    if ((size_t)emm_data_offset + byte_count > total_bytes) {
        return;
    }

    source = target_seg + target_offset;
    dest = handle->base + emm_data_offset;
    memcpy(dest, source, byte_count);
}

// WZD s12p10
SAMB_ptr EMM_GetPageFrame(void)
{
    return EMS_PFBA;
}

// WZD s12p11
void EMM_MapFourPages(uint16_t emsFirst, SAMB_ptr emsHandle)
{
    EMS_PFBA = emsHandle + (emsFirst * SZ_EMM_LOGICAL_PAGE);

    EMM_Log2Phys_Map[0].Logical_Page = emsFirst;
    EMM_Log2Phys_Map[0].Physical_Page = 0;
    EMM_Log2Phys_Map[1].Logical_Page = emsFirst + 1;
    EMM_Log2Phys_Map[1].Physical_Page = 1;
    EMM_Log2Phys_Map[2].Logical_Page = emsFirst + 2;
    EMM_Log2Phys_Map[2].Physical_Page = 2;
    EMM_Log2Phys_Map[3].Logical_Page = emsFirst + 3;
    EMM_Log2Phys_Map[3].Physical_Page = 3;
}

// WZD s12p12
void EMM_Map4Pages(int emsFirst, SAMB_ptr emsHandle)
{
    EMM_MapFourPages((uint16_t)emsFirst, emsHandle);
}


/*
    WIZARDS.EXE seg013
*/

// WZD s13p01
void EMM_Startup(void)
{
    int idx;

    EMM_OK = ST_TRUE;
    EMM_Open_Handles = 0;
    EMM_Pages_Reserved = 40;
    EMM_Data_Level = 0;
    EmmHndlNbr_YOMOMA = NULL;

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        g_ems_handles[idx].in_use = 0;
        g_ems_handles[idx].base = NULL;
        g_ems_handles[idx].pages = 0;
        g_ems_handles[idx].name[0] = '\0';
    }

    if (_EMMDATAH_seg == NULL) {
        int16_t pages = EMS_EMU_DATAH_PAGES;
        int32_t min_pages = (EMM_MinKB > 0) ? (int32_t)EMM_MinKB / 16 : 0;
        if (min_pages > pages) {
            pages = (int16_t)min_pages;
        }
        _EMMDATAH_seg = EMM_MakeNamedHandle(pages, "EMMDATAH");
    }
}

// WZD s13p16
void EMM_ReleaseAll__STUB(void)
{
    int16_t itr = 0;  // _SI_
    for (itr = 0; itr < EMM_Open_Handles; itr++)
    {
        EMM_ReleaseHandle__SUTB(EMM_Table[itr].handle);
    }
    if (g_EmmHndl_OVERXYZ != 0)
    {
        EMM_ReleaseHandle__SUTB((SAMB_ptr)g_EmmHndl_OVERXYZ);
    }
}

// WZD s13p20
SAMB_ptr EMM_GetHandle(int16_t page_count, const char *handle_name, int16_t reserved_flag)
{
    char buffer[20];
    int handle_idx;
    int itr;
    SAMB_ptr emm_handle;

    if (handle_name == NULL || handle_name[0] == '\0') {
        return NULL;
    }

    handle_idx = ST_UNDEFINED;

    for (itr = 0; itr < EMM_Open_Handles; itr++) {
        if (EMS_StrICmp(handle_name, EMM_Table[itr].name) == 0) {
            handle_idx = itr;
        }
    }

    if (handle_idx == ST_UNDEFINED) {
        if (EMM_Open_Handles >= 40) {
            Exit_With_Message("EMM handle table overflow");
            return NULL;
        }

        strncpy(EMM_Table[EMM_Open_Handles].name, handle_name, 8);
        EMM_Table[EMM_Open_Handles].name[8] = '\0';

        if (reserved_flag == ST_TRUE && EMM_Table[EMM_Open_Handles].name[0] != '\0') {
            EMM_Table[EMM_Open_Handles].name[0] = (char)(-(signed char)EMM_Table[EMM_Open_Handles].name[0]);
        }

        emm_handle = EMM_MakeNamedHandle(page_count, EMM_Table[EMM_Open_Handles].name);
        if (emm_handle == NULL) {
            return NULL;
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

// WZD s13p25
SAMB_ptr EMM_Map_DataH(void)
{
    if (_EMMDATAH_seg == NULL) {
        return NULL;
    }

    EMM_MapFourPages(0, _EMMDATAH_seg);
    return EMS_PFBA;
}

// WZD s13p26
SAMB_ptr EMM_EMMDATAH_AllocFirst(uint16_t nparas)
{
    uint16_t nparas_total = (uint16_t)(nparas + 1);
    uint16_t free_paras;
    SAMB_ptr header;

    EMM_Data_Level = 0;
    if (EMM_Map_DataH() == NULL) {
        return NULL;
    }

    free_paras = (uint16_t)(4090 - EMM_Data_Level);
    if (free_paras < nparas_total) {
        Allocation_Error(4, (uint16_t)(nparas_total - free_paras));
    }

    header = EMS_PFBA + ((size_t)EMM_Data_Level * SZ_PARAGRAPH_B);
    _SA_SET_MEMSIG1(header);
    _SA_SET_MEMSIG2(header);
    SA_SET_SIZE(header, (uint16_t)(nparas_total - 1));
    SA_SET_USED(header, 1);
    SA_SET_MARK(header, 1);

    EMM_Data_Level = (int16_t)(EMM_Data_Level + nparas_total + 1);
    return header + SZ_PARAGRAPH_B;
}

// WZD s13p27
SAMB_ptr EMM_EMMDATAH_AllocNext(uint16_t nparas)
{
    uint16_t nparas_total = (uint16_t)(nparas + 1);
    uint16_t free_paras;
    SAMB_ptr header;

    if (EMM_Map_DataH() == NULL) {
        return NULL;
    }

    free_paras = (uint16_t)(4090 - EMM_Data_Level);
    if (free_paras < nparas_total) {
        Allocation_Error(4, (uint16_t)(nparas_total - free_paras));
    }

    header = EMS_PFBA + ((size_t)EMM_Data_Level * SZ_PARAGRAPH_B);
    _SA_SET_MEMSIG1(header);
    _SA_SET_MEMSIG2(header);
    SA_SET_SIZE(header, (uint16_t)(nparas_total - 1));
    SA_SET_USED(header, 1);
    SA_SET_MARK(header, 1);

    EMM_Data_Level = (int16_t)(EMM_Data_Level + nparas_total + 1);
    return header + SZ_PARAGRAPH_B;
}

// WZD s13p28
void UU_EMM_Data_Mark(void)
{
    UU_EMMData_PrevLvl = EMM_Data_Level;
}

// WZD s13p29
void UU_EMM_Data_Undo(void)
{
    EMM_Data_Level = UU_EMMData_PrevLvl;
}

// WZD s13p31
void EMM_Set_Minimum(int amount)
{
    EMM_MinKB = amount;
}

// WZD s13p32
void EMM_GetLowString(char *string)
{
    char temp[20];
    stu_itoa(EMM_MinKB, temp, 10);
    stu_strcpy(string, cnst_EMMErr_TooLow1);
    stu_strcat(string, temp);
    stu_strcat(string, cnst_EMMErr_TooLow2);
}

