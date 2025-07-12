#ifndef STU_SND_H
#define STU_SND_H

#include "../MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



void Parse_VOC_LBX_Entry(char * lbx_file_name, int16_t lbx_entry_num);

int STU_Convert_VOC_To_WAV(const uint8_t * voc_buf, uint32_t voc_len, uint8_t ** out_wav_buf, uint32_t * out_wav_len);

int FUU_Convert_VOC_To_WAV(const uint8_t * voc_buf, uint32_t voc_len, int16_t ** out_wav_buf, uint32_t * out_wav_len);



#ifdef __cplusplus
}
#endif

#endif /* STU_SND_H */
