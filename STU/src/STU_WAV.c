/*
should be something like
LBX File Name
VOC LBX Entry
Microsoft WAVE Audio File Name
¿ after LBX_Load() ?
¿ after Convert_VOC_To_WAV() ?


WTFAID
(API) boundaries?
input?
    LBX name and entry num
    LBX entry
    VOC LBX entry
    VOC *file*
    WAV *file

*/

#include "STU_DBG.h"
#include "STU_WAV.h"

#include "../../ext/stu_compat.h"

#include <stdio.h>      /* FILE; fclose(), fread(), frite(), fseek(); */
#include "../../STU/src/STU_LOG.h"

/*
    LBX File Name
    LBX Entry Number

*/
// void Export_WAV_File(char * lbx_filename, int lbx_entry_number, void * buffer)
void Export_WAV_File(void * wav_sound_buffer, int wav_sound_buffer_size)
{
    const char filename[] = "C:\\STU\\devel\\ReMoM\\some.wav";
    const char mode[] = "wb";
    FILE * file_handle = NULL;
    // char output_file_name[(8 + 1 + 3 + 1 + 3 + 1 + 3)] = { 0 };  // SOUNDFX.LBX, 001 ... SOUNDFX_LBX_001.wav

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEGIN: Export_WAV_File()", __FILE__, __LINE__);
#endif

    LOG_DEBUG(LOG_CAT_GENERAL, "BEGIN:  fopen()");
    // // file_handle = stu_fopen(filename, "WB");
    file_handle = stu_fopen(filename, mode);
    if(file_handle == NULL)
    {
        STU_DEBUG_BREAK();
    }
    LOG_DEBUG(LOG_CAT_GENERAL, "END:  fopen()");

    LOG_DEBUG(LOG_CAT_GENERAL, "BEGIN:  fwrite()");
    // stu_fwrite(wav_sound_buffer, wav_sound_buffer_size, 1, file_handle);
    LOG_DEBUG(LOG_CAT_GENERAL, "END:  fwrite()");

    LOG_DEBUG(LOG_CAT_GENERAL, "BEGIN:  fclose()");
    if(file_handle != NULL)
    {
        stu_fclose(file_handle);
    }
    LOG_DEBUG(LOG_CAT_GENERAL, "END:  fclose()");

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: END: Export_WAV_File()", __FILE__, __LINE__);
#endif

}