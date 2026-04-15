
/*
Expanded Memory Specification (EMS) Emulator

bank switching
*/
#include "../../../ext/stu_compat.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "EMS_EMU.h"

struct s_EMS_HOST_HANDLE
{
    int in_use;
    EMM_HANDLE handle_number;
    uint16_t start_page;
    int16_t pages;
    char name[9];
};

static struct s_EMS_HOST_HANDLE g_ems_handles[EMS_EMU_MAX_HANDLES];
static uint8_t * g_ems_arena_base;
static uint32_t g_ems_arena_size_bytes;
static uint16_t g_ems_total_pages;
static uint8_t g_ems_page_owner[EMS_EMU_MAX_PAGES];
static uint8_t g_ems_page_frame[EMS_EMU_PAGE_FRAME_BYTES];
static EMM_HANDLE g_mapped_handle = EMM_HANDLE_INVALID;
static uint16_t g_mapped_first_page;
static int g_page_frame_valid;

static struct s_EMS_HOST_HANDLE * EMS_EMU_Find_Handle_By_Number(EMM_HANDLE handle_number)
{
    int idx;

    if (handle_number == EMM_HANDLE_INVALID) {
        return NULL;
    }

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (g_ems_handles[idx].in_use && g_ems_handles[idx].handle_number == handle_number) {
            return &g_ems_handles[idx];
        }
    }

    return NULL;
}

static size_t EMS_EMU_Get_Buffer_Size(const struct s_EMS_HOST_HANDLE * handle)
{
    if (handle == NULL || handle->pages <= 0) {
        return 0;
    }

    return (size_t)handle->pages * SZ_EMM_LOGICAL_PAGE;
}

static uint8_t * EMS_EMU_Get_Handle_Base_Pointer(const struct s_EMS_HOST_HANDLE * handle)
{
    if (handle == NULL || g_ems_arena_base == NULL) {
        return NULL;
    }

    return g_ems_arena_base + ((size_t)handle->start_page * SZ_EMM_LOGICAL_PAGE);
}

void EMS_EMU_Flush_Page_Frame(void)
{
    struct s_EMS_HOST_HANDLE * handle;
    uint8_t * dest;
    size_t bytes_to_copy;
    size_t total_bytes;
    size_t page_offset;

    if (!g_page_frame_valid || g_mapped_handle == EMM_HANDLE_INVALID) {
        return;
    }

    handle = EMS_EMU_Find_Handle_By_Number(g_mapped_handle);
    if (handle == NULL) {
        g_page_frame_valid = 0;
        g_mapped_handle = EMM_HANDLE_INVALID;
        g_mapped_first_page = 0;
        memset(g_ems_page_frame, 0, sizeof(g_ems_page_frame));
        return;
    }

    total_bytes = EMS_EMU_Get_Buffer_Size(handle);
    page_offset = (size_t)g_mapped_first_page * SZ_EMM_LOGICAL_PAGE;
    if (page_offset >= total_bytes) {
        return;
    }

    bytes_to_copy = total_bytes - page_offset;
    if (bytes_to_copy > EMS_EMU_PAGE_FRAME_BYTES) {
        bytes_to_copy = EMS_EMU_PAGE_FRAME_BYTES;
    }

    dest = EMS_EMU_Get_Handle_Base_Pointer(handle);
    if (dest == NULL) {
        return;
    }

    memcpy(dest + page_offset, g_ems_page_frame, bytes_to_copy);
}

void EMS_EMU_AttachArena(void *base, uint32_t size_bytes)
{
    uint32_t total_pages;

    g_ems_arena_base = (uint8_t *)base;
    g_ems_arena_size_bytes = size_bytes;
    total_pages = size_bytes / SZ_EMM_LOGICAL_PAGE;
    if (total_pages > EMS_EMU_MAX_PAGES) {
        total_pages = EMS_EMU_MAX_PAGES;
    }
    g_ems_total_pages = (uint16_t)total_pages;
    memset(g_ems_page_owner, 0, sizeof(g_ems_page_owner));
    memset(g_ems_page_frame, 0, sizeof(g_ems_page_frame));
    g_mapped_handle = EMM_HANDLE_INVALID;
    g_mapped_first_page = 0;
    g_page_frame_valid = 0;
    EMS_EMU_ResetHandleTable();
}

int16_t EMS_EMU_IsAttached(void)
{
    return (g_ems_arena_base != NULL && g_ems_total_pages > 0) ? ST_TRUE : ST_FALSE;
}

void EMS_EMU_ResetHandleTable(void)
{
    int idx;

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        g_ems_handles[idx].in_use = 0;
        g_ems_handles[idx].handle_number = (EMM_HANDLE)(idx + 1);
        g_ems_handles[idx].start_page = 0;
        g_ems_handles[idx].pages = 0;
        g_ems_handles[idx].name[0] = '\0';
    }

    if (EMS_EMU_IsAttached()) {
        memset(g_ems_page_owner, 0, (size_t)g_ems_total_pages);
    }

    memset(g_ems_page_frame, 0, sizeof(g_ems_page_frame));
    g_mapped_handle = EMM_HANDLE_INVALID;
    g_mapped_first_page = 0;
    g_page_frame_valid = 0;
}

int16_t EMS_EMU_GetHandleCount(void)
{
    int idx;
    int count;

    count = 0;
    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (g_ems_handles[idx].in_use) {
            ++count;
        }
    }

    return (int16_t)count;
}

int16_t EMS_EMU_GetFreePageCount(int16_t min_expanded_memory_kb)
{
    int idx;
    int free_pages;

    (void)min_expanded_memory_kb;

    if (!EMS_EMU_IsAttached()) {
        return 0;
    }

    free_pages = 0;
    for (idx = 0; idx < g_ems_total_pages; ++idx) {
        if (g_ems_page_owner[idx] == 0) {
            ++free_pages;
        }
    }

    return (int16_t)free_pages;
}

int16_t EMS_EMU_GetPagesOwnedByHandle(EMM_HANDLE emm_handle)
{
    struct s_EMS_HOST_HANDLE * handle;

    handle = EMS_EMU_Find_Handle_By_Number(emm_handle);
    if (handle == NULL) {
        return 0;
    }

    return handle->pages;
}

EMM_HANDLE EMS_EMU_AllocHandle(int16_t pages, const char *name)
{
    int idx;
    int page_idx;
    int run_length;
    int run_start;

    if (!EMS_EMU_IsAttached() || pages <= 0) {
        return EMM_HANDLE_INVALID;
    }

    run_length = 0;
    run_start = -1;
    for (page_idx = 0; page_idx < g_ems_total_pages; ++page_idx) {
        if (g_ems_page_owner[page_idx] == 0) {
            if (run_length == 0) {
                run_start = page_idx;
            }
            ++run_length;
            if (run_length >= pages) {
                break;
            }
        } else {
            run_length = 0;
            run_start = -1;
        }
    }

    if (run_start < 0 || run_length < pages) {
        return EMM_HANDLE_INVALID;
    }

    for (idx = 0; idx < EMS_EMU_MAX_HANDLES; ++idx) {
        if (!g_ems_handles[idx].in_use) {
            int name_idx;

            g_ems_handles[idx].in_use = 1;
            g_ems_handles[idx].start_page = (uint16_t)run_start;
            g_ems_handles[idx].pages = pages;
            g_ems_handles[idx].name[0] = '\0';
            if (name != NULL) {
                strncpy(g_ems_handles[idx].name, name, 8);
                g_ems_handles[idx].name[8] = '\0';
            }
            for (name_idx = 0; name_idx < pages; ++name_idx) {
                g_ems_page_owner[run_start + name_idx] = (uint8_t)g_ems_handles[idx].handle_number;
            }
            return g_ems_handles[idx].handle_number;
        }
    }

    return EMM_HANDLE_INVALID;
}

int16_t EMS_EMU_CopyHandleName(EMM_HANDLE emm_handle, char *out_name)
{
    struct s_EMS_HOST_HANDLE * handle;

    handle = EMS_EMU_Find_Handle_By_Number(emm_handle);
    if (handle == NULL || out_name == NULL) {
        return ST_FAILURE;
    }

    stu_strncpy(out_name, handle->name, 8);
    out_name[8] = '\0';
    return ST_SUCCESS;
}

void EMS_EMU_ReleaseHandleAndMemory(EMM_HANDLE handle_number)
{
    struct s_EMS_HOST_HANDLE * handle;
    int page_idx;

    EMS_EMU_Flush_Page_Frame();

    handle = EMS_EMU_Find_Handle_By_Number(handle_number);
    if (handle == NULL) {
        return;
    }

    for (page_idx = 0; page_idx < handle->pages; ++page_idx) {
        g_ems_page_owner[handle->start_page + page_idx] = 0;
    }

    handle->in_use = 0;
    handle->start_page = 0;
    handle->pages = 0;
    handle->name[0] = '\0';

    if (g_mapped_handle == handle_number) {
        memset(g_ems_page_frame, 0, sizeof(g_ems_page_frame));
        g_mapped_handle = EMM_HANDLE_INVALID;
        g_mapped_first_page = 0;
        g_page_frame_valid = 0;
    }
}

void EMS_EMU_Mapped_Read(void * dst_ptr, uint32_t data_offset, uint16_t nbytes, EMM_HANDLE emm_handle_name)
{
    uint8_t * src = NULL;
    struct s_EMS_HOST_HANDLE * handle = NULL;
    size_t total_bytes = 0;

    if (g_ems_arena_base == NULL)
    {
        return;
    }

    if (dst_ptr == NULL)
    {
        return;
    }

    EMS_EMU_Flush_Page_Frame();

    handle = EMS_EMU_Find_Handle_By_Number(emm_handle_name);

    if (handle == NULL)
    {
        return;
    }

    total_bytes = EMS_EMU_Get_Buffer_Size(handle);

    if ((size_t)data_offset + (size_t)nbytes > total_bytes)
    {
        return;
    }

    src = EMS_EMU_Get_Handle_Base_Pointer(handle) + data_offset;
    memcpy((uint8_t *)dst_ptr, src, nbytes);
}

void EMS_EMU_Mapped_Write(void * src_ptr, uint32_t data_offset, uint16_t nbytes, EMM_HANDLE emm_handle_name)
{
    uint8_t * dst = NULL;
    struct s_EMS_HOST_HANDLE * handle = NULL;
    size_t total_bytes = 0;

    if (g_ems_arena_base == NULL)
    {
        return;
    }

    if (src_ptr == NULL)
    {
        return;
    }

    EMS_EMU_Flush_Page_Frame();

    handle = EMS_EMU_Find_Handle_By_Number(emm_handle_name);

    if (handle == NULL)
    {
        return;
    }

    total_bytes = EMS_EMU_Get_Buffer_Size(handle);
    if ((size_t)data_offset + (size_t)nbytes > total_bytes) {
        return;
    }

    dst = EMS_EMU_Get_Handle_Base_Pointer(handle) + data_offset;
    memcpy(dst, (uint8_t *)src_ptr, nbytes);
}

uint8_t * EMS_EMU_MapHandleIntoPageFrame(EMM_HANDLE emm_handle, uint16_t emm_page)
{
    struct s_EMS_HOST_HANDLE * handle;
    uint8_t * source;
    size_t total_bytes;
    size_t page_offset;
    size_t bytes_to_copy;

    EMS_EMU_Flush_Page_Frame();

    memset(g_ems_page_frame, 0, sizeof(g_ems_page_frame));
    g_mapped_handle = EMM_HANDLE_INVALID;
    g_mapped_first_page = 0;
    g_page_frame_valid = 0;

    handle = EMS_EMU_Find_Handle_By_Number(emm_handle);
    if (handle == NULL || g_ems_arena_base == NULL) {
        return NULL;
    }

    total_bytes = EMS_EMU_Get_Buffer_Size(handle);
    page_offset = (size_t)emm_page * SZ_EMM_LOGICAL_PAGE;
    if (page_offset >= total_bytes) {
        return g_ems_page_frame;
    }

    bytes_to_copy = total_bytes - page_offset;
    if (bytes_to_copy > EMS_EMU_PAGE_FRAME_BYTES) {
        bytes_to_copy = EMS_EMU_PAGE_FRAME_BYTES;
    }

    source = EMS_EMU_Get_Handle_Base_Pointer(handle);
    if (source == NULL) {
        return NULL;
    }

    memcpy(g_ems_page_frame, source + page_offset, bytes_to_copy);
    g_mapped_handle = emm_handle;
    g_mapped_first_page = emm_page;
    g_page_frame_valid = 1;

    return g_ems_page_frame;
}

uint8_t * EMS_EMU_Get_Page_Frame_Pointer(void)
{
    return g_ems_page_frame;
}
