/*
    WIZARDS.EXE

DNE
WZD just fread()'s 18 bytes in _main()

MoO2
    Module: FILEDEF
    Module: INITGAME
*/

#include <stdio.h>

#include "CFG.h"



uint8_t _config_mom[sizeof(struct s_CONFIG_MOM)];

struct s_CONFIG_MOM config_mom;



void Load_CONFIG_MOM(void)
{
    FILE * file_pointer;

    file_pointer = fopen("CONFIG.MOM", "rb");

    fread(&config_mom, 18, 1, file_pointer);

    fclose(file_pointer);

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
