
#include "MoX_CFG.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */


uint8_t _config_mom[20];


void Load_CONFIG_MOM(void)
{
    FILE * file_pointer;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_CONFIG_MOM()\n", __FILE__, __LINE__);
#endif

    file_pointer = fopen("CONFIG.MOM", "rb");

    fread(&config_mom, 18, 1, file_pointer);

    fclose(file_pointer);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_CONFIG_MOM()\n", __FILE__, __LINE__);
#endif
}


void Read_CONFIG_MOM(void)
{
    FILE * file_pointer;
    file_pointer = fopen("CONFIG.MOM", "rb");
    fread(&_config_mom, 20, 1, file_pointer);
    fclose(file_pointer);
}

void Write_CONFIG_MOM(void)
{
    FILE * file_pointer;
    file_pointer = fopen("CONFIG.MOM", "wb");
    fwrite(&_config_mom, 20, 1, file_pointer);
    fclose(file_pointer);
}
