#ifndef STU_XTRC_H
#define STU_XTRC_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif



void Dump_TERRTYPE_LBX(void);


/*
    FONTS.LBX Extraction
    Each function loads one LBX entry and writes a C source fragment to the given FILE *.
    The caller is responsible for opening/closing the file and writing the #include header.
*/
void Emit_Byte_Array(FILE * out, const char * name, const uint8_t * data, uint32_t size);

void Extract_FONTS_LBX_0(FILE * out);
void Extract_FONTS_LBX_1(FILE * out);
void Extract_FONTS_LBX_2(FILE * out);

void Extract_FONTS_LBX(const char * out_path);



#ifdef __cplusplus
}
#endif

#endif /* STU_XTRC_H */
