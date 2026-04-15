


#include "Allocate.h"
#include "MOX_BASE.h"
#include "MOX_TYPE.h"
#include "Util.h"
#include "EXIT.h"
#include "EMS/EMS.h"
#include "EMS/EMS_EMU.h"

#include <stdlib.h>



/*
MoO2
Module: init
    unsigned integer (4 bytes) io_buffer_seg
    Address: 02:001A86F8

    pointer (4 bytes) io_buffer
    Address: 02:001A86FC

    signed integer (2 bytes) io_buffer_selector
    Address: 02:001A8700
*/
SAMB_ptr io_buffer_seg;
void * io_buffer;
int16_t io_buffer_selector;
static void * g_ems_arena;
static uint32_t g_ems_arena_bytes;



/*
MoO2
Module: init
    function (0 bytes) Create_IO_Buffer
    Address: 01:00113E08
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) seg_start

io_buffer looks to be the equivalent of _VGAFILEH_seg, as used for the 'File Animation' feature
*/
/*
EMS Emulation:
    16/32 MB of System Memory (SRAM)
    mapped into a 64 KB page frame *window*, as four pages

*/
void Create_IO_Buffer(void)
{
    // io_buffer_seg = Allocate_Dos_Space((64 * 1024) / SZ_PARAGRAPH_B);
    // // io_buffer = (io_buffer_seg * 16);
    // io_buffer = (io_buffer_seg + 16);
    // io_buffer_selector = io_buffer_seg;
    // // Set_Memory_(io_buffer, 8192, 0);
    // Set_Memory((int8_t *)io_buffer, (64 * 1024), 0);

    char buffer[100];

    g_ems_arena_bytes = (uint32_t)EMS_EMU_DEFAULT_PAGES * SZ_EMM_LOGICAL_PAGE;
    g_ems_arena = malloc(g_ems_arena_bytes);
    if (g_ems_arena == NULL)
    {
        EMM_Error_Message(buffer);
        Exit_With_Message(buffer);
        return;
    }

    Set_Memory((int8_t *)g_ems_arena, (int32_t)g_ems_arena_bytes, 0);
    EMS_EMU_AttachArena(g_ems_arena, g_ems_arena_bytes);
}
