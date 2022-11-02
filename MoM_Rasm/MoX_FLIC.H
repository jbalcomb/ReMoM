#ifndef MOX_FLIC_H
#define MOX_FLIC_H

#include "MoX_TYPE.H"


struct s_FLIC_HDR //; (sizeof=0x10)
{
    /*00h*/word width;                             // MAX: 320  aafli.h (1990) typedef struct fli_head { ... USHORT width;
    /*02h*/word height;                            // MAX: 200
    /*04h*/word current_frame;                     // Run-Time
    /*06h*/word frame_count;                       //           aafli.h (1990) typedef struct fli_head { ... USHORT frame_count;
    /*08h*/word loop_frame;                        // ? i.e., last frame ? ~/Animator "Ring Frame"?
    /*0Ah*/byte emm_handle_number;                 // Run-Time
    /*0Bh*/byte emm_logical_page_number;           // Run-Time
    /*0Ch*/word emm_logical_page_offset;           // Run-Time
    /*0Eh*/word palette_header_offset;               // if NULL, no palette header/data

    /*10h*/ // Frame-Type: {0:Delta-Frame, 1:Full-Frame}
    /*11h*/ // ?shading?

    /*12h*/ // Frame Index 0 Offset
    /*14h*/ // Frame Index 0 Offset

    /*16h*/ // Frame 0 Offset - End / Frame 1 Offset - Begin
            // ...
            // Frame n Offset - End
            // Palette Header
};



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif



#endif MOX_FLIC_H
