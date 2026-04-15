/*
    WIZARDS.EXE
        seg012
        seg013

*/
#ifndef EMM_H
#define EMM_H

#include "../MOX_TYPE.h"
#include "../MOX_BASE.h"

typedef uint16_t EMM_HANDLE;

#define EMM_HANDLE_INVALID ((EMM_HANDLE)0)

struct s_EMM_RECORD
{
    char name[9];
    char reserved;
    EMM_HANDLE handle;
};

struct s_EMM_L2P_MAP_RECORD
{
    uint16_t Logical_Page;
    uint16_t Physical_Page;
};

extern uint8_t * EMS_PFBA;
extern struct s_EMM_L2P_MAP_RECORD EMM_Log2Phys_Map[4];
extern int16_t EMM_Pages_Reserved;
extern int16_t EMM_Open_Handles;
extern EMM_HANDLE g_EmmHndl_OVERXYZ;
extern EMM_HANDLE _EMMDATAH_seg;
extern EMM_HANDLE _VGAFILEH_seg;
extern int16_t EmmHndlNbr_YOMOMA;
extern int16_t EMM_OK;
extern struct s_EMM_RECORD EMM_Table[40];

#ifdef __cplusplus
extern "C" {
#endif

void * SA_MK_FP0(uintptr_t para_addr);

int16_t EMM_Detect_Device(void);
int16_t EMM_Number_Of_Handles(void);
int16_t EMM_Get_Free_Page_Count(void);
int16_t EMM_GetPageCount(EMM_HANDLE emm_handle);
EMM_HANDLE EMM_MakeNamedHandle(int16_t pages, const char * name);
int16_t EMM_Get_Handle_Name(char * handle_name, EMM_HANDLE emm_handle);
void EMM_Release_Handle(EMM_HANDLE handle);
void EMM_MapnRead(uint16_t target_offset, void * target_seg, uint32_t emm_data_offset, uint16_t byte_count, EMM_HANDLE emm_handle);
void EMM_MapnWrite(uint16_t target_offset, void * target_seg, uint32_t emm_data_offset, uint16_t byte_count, EMM_HANDLE emm_handle);
void * EMM_Get_Page_Frame(void);
void EMM_MapFourPages(EMM_HANDLE emm_handle, uint16_t emm_page);
void EMM_MapMulti4(EMM_HANDLE emm_handle, uint16_t emm_page);

void EMM_Startup(void);
void EMS_Startup(void);
void EMM_Release_All(void);
EMM_HANDLE EMM_Get_Handle(int16_t page_count, const char *handle_name, int16_t reserved_flag);
void EMMDATAH_Map(void);
uint8_t * EMMDATAH_Allocate_First(uint16_t nparas);
uint8_t * EMMDATAH_Allocate_Next(uint16_t nparas);
void EMMDATAH_Save_Last_Offset(void);
void EMMDATAH_Restore_Last_Offset(void);
void UU_EMM_LBX_Load2Hnd(char * file_name, EMM_HANDLE emm_handle);
void EMM_Set_Minimum(int16_t amount);
void EMM_Error_Message(char * string);

#ifdef __cplusplus
}
#endif

#endif
