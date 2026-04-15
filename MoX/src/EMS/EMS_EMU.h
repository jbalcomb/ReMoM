#ifndef EMS_EMU_H
#define EMS_EMU_H

#include "../MOX_TYPE.h"
#include "../MOX_BASE.h"
#include "EMS.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EMS_EMU_MAX_HANDLES     128
#define EMS_EMU_DEFAULT_PAGES   2048  /* 2048 pages, 16 KB per page, 32 MB per LIM EMS 4.0 */
#define EMS_EMU_DATAH_PAGES     1024
#define EMS_EMU_MAX_PAGES       8192
#define EMS_EMU_PAGE_FRAME_PAGES 4
#define EMS_EMU_PAGE_FRAME_BYTES (EMS_EMU_PAGE_FRAME_PAGES * SZ_EMM_LOGICAL_PAGE)

void EMS_EMU_AttachArena(void *base, uint32_t size_bytes);
int16_t EMS_EMU_IsAttached(void);
void EMS_EMU_ResetHandleTable(void);
int16_t EMS_EMU_GetHandleCount(void);
int16_t EMS_EMU_GetFreePageCount(int16_t min_expanded_memory_kb);
int16_t EMS_EMU_GetPagesOwnedByHandle(EMM_HANDLE emm_handle);
EMM_HANDLE EMS_EMU_AllocHandle(int16_t pages, const char *name);
int16_t EMS_EMU_CopyHandleName(EMM_HANDLE emm_handle, char *out_name);
void EMS_EMU_ReleaseHandleAndMemory(EMM_HANDLE handle_number);
void EMS_EMU_Mapped_Read(void * dst_ptr, uint32_t data_offset, uint16_t nbytes, EMM_HANDLE emm_handle_name);
void EMS_EMU_Mapped_Write(void * src_ptr, uint32_t data_offset, uint16_t nbytes, EMM_HANDLE emm_handle_name);
uint8_t * EMS_EMU_MapHandleIntoPageFrame(EMM_HANDLE emm_handle, uint16_t emm_page);
uint8_t * EMS_EMU_Get_Page_Frame_Pointer(void);
void EMS_EMU_Flush_Page_Frame(void);

#ifdef __cplusplus
}
#endif

#endif
