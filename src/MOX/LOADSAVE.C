/*
    WIZARDS.EXE
    seg050
    
    MoO2: Module: LOADSAVE
*/

#include "LOADSAVE.H"

#include "MOX_DAT.H"  /* _players[]; _screen_seg; */
#include "MOX_SET.H"  /* magic_set */

#include "../MOM_Data.H"
#include "../MOM_PFL.H"

#include <assert.h>
#include <math.h>       /* sqrt() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* abs(); itoa(); */
#include <string.h>     /* memset(), strcat(), strcpy(); */



uint8_t _save_gam[123300];


void TST_Validate_GameData(void)
{
    int16_t itr;
    int16_t is_bjorked;
    int8_t * ptr_TBL_Terr_Specials;
    int8_t terrain_special;
    int8_t terrain_special_min;
    int8_t terrain_special_max;

//     int8_t * ptr_square_explored;
//     int8_t unexplored;
//     int8_t unexplored_min;
//     int8_t unexplored_max;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TST_Validate_GameData()\n", __FILE__, __LINE__);
#endif


// terrain_special_min: -128
// terrain_special_max: 64
    is_bjorked = ST_FALSE;
    ptr_TBL_Terr_Specials = (int8_t *)TBL_Terr_Specials;
    terrain_special_min = 0;
    terrain_special_max = 0;
    for(itr = 0; itr < 4800; itr++)
    {
        terrain_special = *(ptr_TBL_Terr_Specials + itr);
#ifdef STU_DEBUG
        dbg_prn("terrain_special: 0x%02X  %d\n", terrain_special, terrain_special);
#endif
        if(terrain_special < terrain_special_min) { terrain_special_min = terrain_special; }
        if(terrain_special > terrain_special_max) { terrain_special_max = terrain_special; }
        // if( !(terrain_special >= 0) || !(terrain_special <= 9) )
        // {
        //     is_bjorked = ST_TRUE;
        // }
    }
#ifdef STU_DEBUG
        dbg_prn("terrain_special_min: %d\n", terrain_special_min);
        dbg_prn("terrain_special_max: %d\n", terrain_special_max);
#endif
//     if(is_bjorked == ST_TRUE)
//     {
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d]: TBL_Terr_Specials is bjorked!!\n", __FILE__, __LINE__);
// #endif
//     }



// // unexplored_min: 0
// // unexplored_max: 15
//     is_bjorked = ST_FALSE;
//     ptr_square_explored = (int8_t *)(_square_explored);
//     unexplored_min = 0;
//     unexplored_max = 0;
//     for(itr = 0; itr < 4800; itr++)
//     {
//         unexplored = *(ptr_square_explored + itr);
// #ifdef STU_DEBUG
//         dbg_prn("unexplored: 0x%02X  %d\n", unexplored, unexplored);
// #endif
//         if(unexplored < unexplored_min) { unexplored_min = unexplored; }
//         if(unexplored > unexplored_max) { unexplored_max = unexplored; }
//         // if( !(unexplored == 0) || !(unexplored == 1) )
//         // {
//         //     is_bjorked = ST_TRUE;
//         // }
//     }
// #ifdef STU_DEBUG
//         dbg_prn("unexplored_min: %d\n", unexplored_min);
//         dbg_prn("unexplored_max: %d\n", unexplored_max);
// #endif
// //     if(is_bjorked == ST_TRUE)
// //     {
// // #ifdef STU_DEBUG
// //         dbg_prn("DEBUG: [%s, %d]: TBL_Scounting is bjorked!!\n", __FILE__, __LINE__);
// // #endif
// //     }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TST_Validate_GameData()\n", __FILE__, __LINE__);
#endif
}

// WZD o50p01
void Save_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[LEN_FILE_NAME];
    char temp_string[LEN_TEMP_STRING];
    FILE * file_pointer;
    long file_pointer_position;

    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
        itoa((save_gam_idx + 1), temp_string, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, temp_string);
        strcat(file_name,".GAM");
    }

    file_pointer = fopen(file_name, "wb");
    assert(file_pointer != NULL);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 0);

    fwrite(_HEROES2[0], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fwrite(_HEROES2[1], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fwrite(_HEROES2[2], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fwrite(_HEROES2[3], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fwrite(_HEROES2[4], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fwrite(_HEROES2[5], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 2520);

    fwrite(&_num_players, 1, 2, file_pointer);
    fwrite(&_landsize , 1, 2, file_pointer);
    fwrite(&_magic, 1, 2, file_pointer);
    fwrite(&_difficulty, 1, 2, file_pointer);
    fwrite(&_cities, 1, 2, file_pointer);
    fwrite(&_units, 1, 2, file_pointer);
    fwrite(&_turn, 1, 2, file_pointer);
    fwrite(&_unit, 1, 2, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 2536);

    fwrite(_players, NUM_PLAYERS, 1224, file_pointer);
    // fwrite(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 9880);

    fwrite(_world_maps, NUM_PLANES, 4800, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 19480);

    fwrite(UU_TBL_1, NUM_PLANES, 96, file_pointer);
    fwrite(UU_TBL_2, NUM_PLANES, 96, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 19864);

    fwrite(_landmasses, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 24664);

    fwrite(_NODES, NUM_NODES, 48, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26104);

    fwrite(_FORTRESSES, NUM_FORTRESSES, 4, file_pointer);
    // fwrite(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26128);

    fwrite(_TOWERS, NUM_TOWERS, 4, file_pointer);
    // fwrite(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26152);

    // fwrite(_LAIRS, NUM_LAIRS, 24, file_pointer);
    /* HACK */  fwrite(_LAIRS, 102, 24, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 28600);

    fwrite(_ITEMS, 138, 50, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 35500);

    fwrite(_CITIES, NUM_CITIES, 114, file_pointer);
    // fwrite(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 46900);

    fwrite(_UNITS, NUM_UNITS, 32, file_pointer);
    // fwrite(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 79188);

    fwrite(TBL_Terr_Specials, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 83988);

    fwrite(_square_explored, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 88788);

    fwrite(movement_mode_cost_maps, NUM_PLANES, 14400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 117588);

    fwrite(events_table, 1, 100, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 117688);

    fwrite(_map_square_flags, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122488);

    fwrite(&grand_vizier, 1, 2, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122490);

    fwrite(TBL_Premade_Items, 250, 1, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122740);

    fwrite(hero_names_table, 16, NUM_HERO_TYPES, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 123300);

    fclose(file_pointer);

    // HACK:
    if(save_gam_idx != ST_UNDEFINED)
    {
        // � don't save settings for continue/auto-save ?
        if (save_gam_idx < 8)
        {
            magic_set.Have_Save[save_gam_idx] = ST_TRUE;
            file_pointer = fopen("MAGIC.SET", "wb");
            fwrite(&magic_set, 466, 1, file_pointer);
            fclose(file_pointer);
        }
    }

}


// WZD o50p02
void Load_SAVE_GAM(int16_t save_gam_idx)
{
    char file_name[LEN_FILE_NAME];
    char temp_string[LEN_TEMP_STRING];
    FILE * file_pointer;
    int32_t file_size;
    int16_t file_size_flag;
    long file_pointer_position;

    if(save_gam_idx == ST_UNDEFINED)
    {
        strcpy(file_name, "SAVETEST.GAM");
    }
    else
    {
        itoa((save_gam_idx + 1), temp_string, 10);
        strcpy(file_name, "SAVE");
        strcat(file_name, temp_string);
        strcat(file_name,".GAM");
    }

    file_size = LOF(file_name);
    assert(file_size != 0);

    if(file_size != 57764)
    {
        file_size_flag = ST_TRUE;
    }

    file_pointer = fopen(file_name, "rb");
    assert(file_pointer != NULL);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 0);  // BoF

    fread(_HEROES2[0], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fread(_HEROES2[1], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fread(_HEROES2[2], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fread(_HEROES2[3], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fread(_HEROES2[4], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);
    fread(_HEROES2[5], sizeof(struct s_HERO), NUM_HERO_TYPES, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 2520);

    fread(&_num_players, 1, 2, file_pointer);
    fread(&_landsize, 1, 2, file_pointer);
    fread(&_magic, 1, 2, file_pointer);
    fread(&_difficulty, 1, 2, file_pointer);
    fread(&_cities, 1, 2, file_pointer);
    fread(&_units, 1, 2, file_pointer);
    fread(&_turn, 1, 2, file_pointer);
    fread(&_unit, 1, 2, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 2536);

    fread(_players, NUM_PLAYERS, 1224, file_pointer);
    // fread(_players, PLAYER_COUNT_MAX, sizeof(struct s_WIZARD), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 9880);

    fread(_world_maps, NUM_PLANES, 4800, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 19480);

    fread(UU_TBL_1, NUM_PLANES, 96, file_pointer);
    fread(UU_TBL_2, NUM_PLANES, 96, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 19864);

    fread(_landmasses, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 24664);

    fread(_NODES, NUM_NODES, 48, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26104);

    fread(_FORTRESSES, NUM_FORTRESSES, 4, file_pointer);
    // fread(_FORTRESSES, FORTRESS_COUNT_MAX, sizeof(struct s_FORTRESS), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26128);

    fread(_TOWERS, NUM_TOWERS, 4, file_pointer);
    // fread(_TOWERS, TOWER_COUNT_MAX, sizeof(struct s_TOWER), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 26152);

    // fread(_LAIRS, NUM_LAIRS, 24, file_pointer);
    /* HACK */  fread(_LAIRS, 102, 24, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 28600);

    fread(_ITEMS, 138, 50, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 35500);

    fread(_CITIES, NUM_CITIES, 114, file_pointer);
    // fread(_CITIES, CITY_COUNT_MAX, sizeof(struct s_CITY), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 46900);

    fread(_UNITS, NUM_UNITS, 32, file_pointer);
    // fread(_UNITS, UNIT_COUNT_MAX, sizeof(struct s_UNIT), file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 79188);

    fread(TBL_Terr_Specials, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 83988);

    fread(_square_explored, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 88788);

    fread(movement_mode_cost_maps, NUM_PLANES, 14400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 117588);

    fread(events_table, 1, 100, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 117688);

    fread(_map_square_flags, NUM_PLANES, 2400, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122488);

    fread(&grand_vizier, 1, 2, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122490);

    fread(TBL_Premade_Items, 250, 1, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 122740);

    // TODO  DEDU  if (file_size_flag == ST_TRUE) { MEM_Clear_Far(hero_names_table, 545); } else { ... }
    fread(hero_names_table, 16, NUM_HERO_TYPES, file_pointer);

    file_pointer_position = ftell(file_pointer);
    assert(file_pointer_position == 123300);

    fclose(file_pointer);

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
