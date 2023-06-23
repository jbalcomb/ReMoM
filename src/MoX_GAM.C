
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
#include <string.h>     /* strcat(), strcpy() */


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
#pragma warning(suppress : 4996)
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


    fwrite(&_num_players, 1, 2, file_pointer);
    fwrite(&_landsize , 1, 2, file_pointer);
    fwrite(&_magic, 1, 2, file_pointer);
    fwrite(&_difficulty, 1, 2, file_pointer);
    fwrite(&_cities, 1, 2, file_pointer);
    fwrite(&_units, 1, 2, file_pointer);
    fwrite(&_turn, 1, 2, file_pointer);
    fwrite(&_unit, 1, 2, file_pointer);


    fwrite(_players, 6, 1224, file_pointer);
    // fwrite(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    fwrite(_world_maps, 2, 4800, file_pointer);

    fwrite(UU_TBL_1, 2, 96, file_pointer);
    fwrite(UU_TBL_2, 2, 96, file_pointer);

    fwrite(TBL_Landmasses, 2, 2400, file_pointer);

    fwrite(TBL_Nodes, 30, 48, file_pointer);

    fwrite(_FORTRESSES, 6, 4, file_pointer);
    // fwrite(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    fwrite(_TOWERS, 6, 4, file_pointer);
    // fwrite(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    fwrite(TBL_Encounters, 102, 24, file_pointer);

    fwrite(TBL_Items, 138, 50, file_pointer);

    fwrite(_CITIES, 100, 114, file_pointer);
    // fwrite(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    fwrite(_UNITS, 1009, 32, file_pointer);
    // fwrite(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    fwrite(TBL_Terr_Specials, 2, 2400, file_pointer);

    fwrite(TBL_Scouting, 2, 2400, file_pointer);

    fwrite(TBL_MoveMaps_EMS, 2, 14400, file_pointer);

    fwrite(_events_table, 1, 100, file_pointer);

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
    long file_pointer_position;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_SAVE_GAM( save_gam_idx = %d )\n", __FILE__, __LINE__, save_gam_idx);
#endif

    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
#pragma warning(suppress : 4996)
        itoa(save_gam_idx, a_idx, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, a_idx);
        strcat(file_name,".GAM");
    }

    file_size = LOF(file_name);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: file_size: %d\n", __FILE__, __LINE__, file_size);
#endif

    if(file_size != 57764)
    {
        file_size_flag = ST_TRUE;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: file_name: %s\n", __FILE__, __LINE__, file_name);
#endif

    // TODO  test balance of sizeof(structs)  DLOG("Y U NO DIE?!?");

    file_pointer = fopen(file_name, "rb");
#ifdef STU_DEBUG
    if(NULL == file_pointer) { DLOG("(NULL == file_pointer)"); }
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 0) { DLOG("(file_pointer_position != 0)"); }
#endif

    fread(p0_heroes, 12, 35, file_pointer);
    fread(p1_heroes, 12, 35, file_pointer);
    fread(p2_heroes, 12, 35, file_pointer);
    fread(p3_heroes, 12, 35, file_pointer);
    fread(p4_heroes, 12, 35, file_pointer);
    fread(p5_heroes, 12, 35, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 2520)
    {
        DLOG("(file_pointer_position != 2520)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(&_num_players, 1, 2, file_pointer);
    fread(&_landsize, 1, 2, file_pointer);
    fread(&_magic, 1, 2, file_pointer);
    fread(&_difficulty, 1, 2, file_pointer);
    fread(&_cities, 1, 2, file_pointer);
    fread(&_units, 1, 2, file_pointer);
    fread(&_turn, 1, 2, file_pointer);
    fread(&_unit, 1, 2, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 2536)
    {
        DLOG("(file_pointer_position != 2536)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(_players, 6, 1224, file_pointer);
    // fread(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 9880)
    {
        DLOG("(file_pointer_position != 9880)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(_world_maps, 2, 4800, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 19480)
    {
        DLOG("(file_pointer_position != 19480)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(UU_TBL_1, 2, 96, file_pointer);
    fread(UU_TBL_2, 2, 96, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 19864)
    {
        DLOG("(file_pointer_position != 19864)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(TBL_Landmasses, 2, 2400, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 24664)
    {
        DLOG("(file_pointer_position != 24664)");
    dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(TBL_Nodes, 30, 48, file_pointer);

    fread(_FORTRESSES, 6, 4, file_pointer);
    // fread(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    fread(_TOWERS, 6, 4, file_pointer);
    // fread(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    fread(TBL_Encounters, 102, 24, file_pointer);

    fread(TBL_Items, 138, 50, file_pointer);

    fread(_CITIES, 100, 114, file_pointer);
    // fread(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    fread(_UNITS, 1009, 32, file_pointer);
    // fread(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 79188)
    {
        DLOG("(file_pointer_position != 79188)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(TBL_Terr_Specials, 2, 2400, file_pointer);

    fread(TBL_Scouting, 2, 2400, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 88788)
    {
        DLOG("(file_pointer_position != 88788)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(TBL_MoveMaps_EMS, 2, 14400, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 117588)
    {
        DLOG("(file_pointer_position != 117588)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    fread(_events_table, 1, 100, file_pointer);

    fread(TBL_Terrain_Flags, 2, 2400, file_pointer);

    fread(&grand_vizier, 1, 2, file_pointer);

    fread(TBL_Premade_Items, 250, 1, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 122740)
    {
        DLOG("(file_pointer_position != 122740)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

    // if (file_size_flag == ST_TRUE) { MEM_Clear_Far(TBL_Hero_Names, 545); } else { ... }
    fread(TBL_Hero_Names, 16, 35, file_pointer);

#ifdef STU_DEBUG
    file_pointer_position = ftell(file_pointer);
    if(file_pointer_position != 123300)
    {
        DLOG("(file_pointer_position != 123300)");
        dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
    }
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: file_pointer_position: %ld\n", __FILE__, __LINE__, file_pointer_position);
#endif

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
