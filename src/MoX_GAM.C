
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_GAM.H"

#include "MoX_Data.H"
#include "MoX_DIR.H"
#include "MoX_SET.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */



uint8_t _save_gam[123300];



// WZD o50p01
void Save_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[20];
    char a_idx[20];
    FILE * file_pointer;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Save_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif


    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
        itoa(save_gam_idx, a_idx, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, a_idx);
        strcat(file_name,".GAM");
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: file_name: %s\n", __FILE__, __LINE__, file_name);
#endif

    file_pointer = fopen(file_name, "wb");


    fwrite(p0_heroes, 12, 35, file_pointer);
    fwrite(p1_heroes, 12, 35, file_pointer);
    fwrite(p2_heroes, 12, 35, file_pointer);
    fwrite(p3_heroes, 12, 35, file_pointer);
    fwrite(p4_heroes, 12, 35, file_pointer);
    fwrite(p5_heroes, 12, 35, file_pointer);


    fwrite(&_wizards, 1, 2, file_pointer);
    fwrite(&_landsize , 1, 2, file_pointer);
    fwrite(&_magic, 1, 2, file_pointer);
    fwrite(&_difficulty, 1, 2, file_pointer);
    fwrite(&_cities, 1, 2, file_pointer);
    fwrite(&_units, 1, 2, file_pointer);
    fwrite(&_turn, 1, 2, file_pointer);
    fwrite(&_unit, 1, 2, file_pointer);


    fwrite(TBL_Wizards, 6, 1224, file_pointer);

    fwrite(TBL_Maps, 2, 4800, file_pointer);

    fwrite(UU_TBL_1, 2, 96, file_pointer);
    fwrite(UU_TBL_2, 2, 96, file_pointer);

    fwrite(TBL_Landmasses, 2, 2400, file_pointer);

    fwrite(TBL_Nodes, 30, 48, file_pointer);

    fwrite(TBL_Fortresses, 6, 4, file_pointer);

    fwrite(TBL_Towers, 6, 4, file_pointer);

    fwrite(TBL_Encounters, 102, 24, file_pointer);

    fwrite(TBL_Items, 138, 50, file_pointer);

    fwrite(TBL_Cities, 100, 114, file_pointer);

    fwrite(TBL_Units, 1009, 32, file_pointer);

    fwrite(TBL_Terr_Specials, 2, 2400, file_pointer);

    fwrite(TBL_Scouting, 2, 2400, file_pointer);

    fwrite(TBL_MoveMaps_EMS, 2, 14400, file_pointer);

    fwrite(TBL_Events, 1, 100, file_pointer);

    fwrite(TBL_Terrain_Flags, 2, 2400, file_pointer);

    fwrite(&grand_vizier, 1, 2, file_pointer);

    fwrite(TBL_Premade_Items, 250, 1, file_pointer);

    fwrite(TBL_Hero_Names, 12, 35, file_pointer);


    fclose(file_pointer);

    if( (save_gam_idx > 0) && (save_gam_idx < 9) )
    {
        magic_set.Have_Save[save_gam_idx] = ST_TRUE;
        file_pointer = fopen("MAGIC.SET", "wb");
        fwrite(&magic_set, 466, 1, file_pointer);
        fclose(file_pointer);
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Save_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif

}


// WZD o50p02
void Load_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[20];
    char a_idx[20];
    FILE * file_pointer;
    int32_t file_size;
    int16_t file_size_flag;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif

    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
        itoa(save_gam_idx, a_idx, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, a_idx);
        strcat(file_name,".GAM");
    }

    file_size = LOF(file_name);

    if(file_size != 57764)
    {
        file_size_flag = ST_TRUE;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: file_name: %s\n", __FILE__, __LINE__, file_name);
#endif

    file_pointer = fopen(file_name, "rb");


    fread(p0_heroes, 12, 35, file_pointer);
    fread(p1_heroes, 12, 35, file_pointer);
    fread(p2_heroes, 12, 35, file_pointer);
    fread(p3_heroes, 12, 35, file_pointer);
    fread(p4_heroes, 12, 35, file_pointer);
    fread(p5_heroes, 12, 35, file_pointer);


    fread(&_wizards, 1, 2, file_pointer);
    fread(&_landsize , 1, 2, file_pointer);
    fread(&_magic, 1, 2, file_pointer);
    fread(&_difficulty, 1, 2, file_pointer);
    fread(&_cities, 1, 2, file_pointer);
    fread(&_units, 1, 2, file_pointer);
    fread(&_turn, 1, 2, file_pointer);
    fread(&_unit, 1, 2, file_pointer);


    fread(TBL_Wizards, 6, 1224, file_pointer);

    fread(TBL_Maps, 2, 4800, file_pointer);

    fread(UU_TBL_1, 2, 96, file_pointer);
    fread(UU_TBL_2, 2, 96, file_pointer);

    fread(TBL_Landmasses, 2, 2400, file_pointer);

    fread(TBL_Nodes, 30, 48, file_pointer);

    fread(TBL_Fortresses, 6, 4, file_pointer);

    fread(TBL_Towers, 6, 4, file_pointer);

    fread(TBL_Encounters, 102, 24, file_pointer);

    fread(TBL_Items, 138, 50, file_pointer);

    fread(TBL_Cities, 100, 114, file_pointer);

    fread(TBL_Units, 1009, 32, file_pointer);

    fread(TBL_Terr_Specials, 2, 2400, file_pointer);

    fread(TBL_Scouting, 2, 2400, file_pointer);

    fread(TBL_MoveMaps_EMS, 2, 14400, file_pointer);

    fread(TBL_Events, 1, 100, file_pointer);

    fread(TBL_Terrain_Flags, 2, 2400, file_pointer);

    fread(&grand_vizier, 1, 2, file_pointer);

    fread(TBL_Premade_Items, 250, 1, file_pointer);

    // if (file_size_flag == ST_TRUE) { MEM_Clear_Far(TBL_Hero_Names, 545); } else { ... }
    fread(TBL_Hero_Names, 12, 35, file_pointer);


    fclose(file_pointer);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif
}



void Read_SAVE_GAM(void)
{
    FILE * file_pointer;
    file_pointer = fopen("SAVETEST.GAM", "rb");
    fread(&_save_gam, 123300, 1, file_pointer);
    fclose(file_pointer);
}

void Write_SAVE_GAM(void)
{
    FILE * file_pointer;
    file_pointer = fopen("SAVETEST.GAM", "wb");
    fwrite(&_save_gam, 123300, 1, file_pointer);
    fclose(file_pointer);
}
