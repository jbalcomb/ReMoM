


#include "MOX_Lib.H"



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



/*
MoO2
Module: init
    function (0 bytes) Create_IO_Buffer
    Address: 01:00113E08
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) seg_start
*/
void Create_IO_Buffer(void)
{

    io_buffer_seg = Allocate_Dos_Space((64 * 1024) / SZ_PARAGRAPH_B);

    // io_buffer = (io_buffer_seg * 16);
    io_buffer = (io_buffer_seg + 16);

    // ¿ DONT ?  io_buffer_selector = io_buffer_seg;

    // Set_Memory_(io_buffer, 8192, 0);
    Set_Memory((int8_t *)io_buffer, (64 * 1024), 0);

    /*
        MoM
            EMM_Startup()
                _VGAFILEH_seg = EMM_GetHandle(5, cnst_EMM_VGAH_Name, 1);
    */

    _VGAFILEH_seg = (byte_ptr)io_buffer;

}
