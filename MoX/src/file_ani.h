/*
    WIZARDS.EXE
        seg025  ASM
        seg030

MoO2
    Module: file_ani

*/

#ifndef FILE_ANI_H
#define FILE_ANI_H

#include "MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:E7DE                                                 BEGIN:  Fonts

// WZD dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0                ; DATA XREF: Load_Font_File+78w ...
// extern struct s_animation_header file_animation_header;
// extern struct s_FLIC_HDR * file_animation_header;
extern SAMB_ptr file_animation_header;



/*
    WIZARDS.EXE  seg025
*/

// WZD s25p01
void Draw_File_Animation_Frame_ASM(int16_t x, int16_t y, SAMB_ptr file_animation, int16_t frame_num);

// WZD s25p02
// Map_File_Animation_Frame_ASM()

// WZD s25p03
void Remap_Draw_File_Animation_Frame_ASM(int16_t x, int16_t y, SAMB_ptr file_animation, uint16_t frame_data_offset);



/*
    WIZARDS.EXE  seg025
*/



#ifdef __cplusplus
}
#endif

#endif  /* FILE_ANI_H */
