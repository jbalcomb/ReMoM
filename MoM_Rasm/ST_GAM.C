
#include "MoX_TYPE.H"    /* word */

#include "ST_GAM.H"     /* magic_set */

#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fseek(); */

/*
    SEE: MoM-SAVE_GAM.md

    MGC ovr048
    WZD ovr050
    Save_SAVE_GAM()
    Load_SAVE_GAM()
*/


// MGC o48p01 WZD o50p01
void Save_SAVE_GAM(short save_gam_idx)
{

}


// MGC o48p02 WZD o50p02
void Load_SAVE_GAM(short save_gam_idx)
{
    char file_name[20];
    char buffer[20];
    FILE * file_pointer;

    itoa(buffer, save_gam_idx, 10);

    strcpy(file_name, "SAVE");
    strcat(file_name,buffer);
    strcat(file_name,".GAM");

    // DISK_GetFileSize(file_name,0);
    file_size = Get_File_Size(file_name);

    if(file_size != 57764)
    {
        flag = ST_TRUE
    }


    // file_handle = DISK_OpenCreate(file_name,"RB");
    file_pointer = fopen(file_name,"RB");

    // DISK_ReadFile(gfp_TBL_HeroStats_P0, gfp_TBL_HeroStats_P0+2, 12, 35, file_handle);
    fread(gfp_TBL_HeroStats_P0, 12, 35, file_pointer);


    
    fclose(file_pointer);
}
