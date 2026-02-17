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

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */

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
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Export_WAV_File()\n", __FILE__, __LINE__);
#endif

    dbg_prn("BEGIN:  fopen()\n");
    // // file_handle = fopen(filename, "WB");
    file_handle = fopen(filename, mode);
    if(file_handle == NULL)
    {
        STU_DEBUG_BREAK();
    }
    dbg_prn("END:  fopen()\n");

    dbg_prn("BEGIN:  fwrite()\n");
    // fwrite(wav_sound_buffer, wav_sound_buffer_size, 1, file_handle);
    dbg_prn("END:  fwrite()\n");

    dbg_prn("BEGIN:  fclose()\n");
    if(file_handle != NULL)
    {
        fclose(file_handle);
    }
    dbg_prn("END:  fclose()\n");

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Export_WAV_File()\n", __FILE__, __LINE__);
#endif

}