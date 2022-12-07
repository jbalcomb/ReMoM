
#include "ST_CFG.H"     /* config_mom */

#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fseek(); */

/*

SEE: MoM-CONFIG_MOM.md


*/



void Load_CONFIG_MOM(void)
{
    FILE * file_pointer;

    file_pointer = fopen("CONFIG.MOM","RB");

    fread(&config_mom, 18, 1, file_pointer);

    fclose(file_pointer);
}
