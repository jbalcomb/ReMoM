
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */


char preamble[] = "struct s_MOM_GAME_DATA {";
char postamble[] = "};";


// https://stackoverflow.com/questions/59456859/write-an-xml-file-through-a-c-program
//     fprintf (f,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");


// unsigned char SAVE_GAM[123300] = { 0x10, ..., 0x00 };

void Export_GameData(void)
{
    char file_name[20];  // 8.3
    FILE * file_pointer;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Export_GameData()\n", __FILE__, __LINE__);
#endif

    strcpy(file_name, "TST_GameData.npp");

    file_pointer = fopen(file_name, "wb");

    fwrite(preamble, 1, sizeof(preamble), file_pointer);

    fwrite(postamble, 1, sizeof(postamble), file_pointer);

    fclose(file_pointer);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Export_GameData()\n", __FILE__, __LINE__);
#endif

}

/*

// WZD o50p01
void TST_Save_SAVE_GAM(int16_t save_gam_idx)
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
*/
