/*
    WIZARDS.EXE
        DNE

    Moo2
        Module: init

*/

#ifndef IO_BUFFER_H
#define IO_BUFFER_H

#include "MoX_Lib.H"

#ifdef __cplusplus
extern "C" {
#endif



extern SAMB_ptr io_buffer_seg;
extern void * io_buffer;
extern int16_t io_buffer_selector;



void Create_IO_Buffer(void);



#ifdef __cplusplus
}
#endif

#endif  /* IO_BUFFER_H */
