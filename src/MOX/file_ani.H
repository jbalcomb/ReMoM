/*
    WIZARDS.EXE
        seg030

MoO2
    Module: file_ani

*/

#ifndef FILE_ANI_H
#define FILE_ANI_H

#include "MOX_Lib.H"

#ifdef __cplusplus
extern "C" {
#endif




// WZD dseg:E7DE                                                 BEGIN:  Fonts

// WZD dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0                ; DATA XREF: Load_Font_File+78w ...
// extern struct s_animation_header file_animation_header;
// extern struct s_FLIC_HDR * file_animation_header;
extern SAMB_ptr file_animation_header;



#ifdef __cplusplus
}
#endif

#endif  /* FILE_ANI_H */
