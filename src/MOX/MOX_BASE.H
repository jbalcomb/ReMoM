#ifndef MOX_BASE_H
#define MOX_BASE_H



#ifdef _WIN32
#pragma warning (disable : 4101)  // Warning C4101 unreferenced local variable
// #pragma warning (disable : 4701)
#pragma warning (disable : 4703)  // Error C4703 potentially uninitialized local pointer variable used
#pragma warning (disable : 6001)  // Warning C6001 Using uninitialized memory '...'.
#pragma warning(disable : 28183)  // Warning C28183 '...' could be '0', and is a copy of the value found in '...':  this does not adhere to the specification for the function 'fread'.
#endif



/*
    FLIC_Draw / animate
*/
#define FLIC_HDR_POS_WIDTH                      0x00
#define FLIC_HDR_POS_HEIGHT                     0x02
#define FLIC_GET_WIDTH(_ptr_)                   ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_WIDTH)                   )
#define FLIC_GET_HEIGHT(_ptr_)                  ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_HEIGHT)                  )

/*
    MoX_DEF
*/

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200


#define NUM_SAVE_SLOTS         8
#define NUM_SAVES              8
#define NUM_SAVE_GAME_FILES    9
#define NUM_HOF_ENTRIES       10

#define LEN_STRING            30  /* used for found_file in main() */
#define LEN_NAME              20  /* ¿ Wizard Name, Unit Type Name, Building Name, HoF Name, Save-Game Name, ... ? (NOT Item Name, City Name)*/
#define LEN_DESCRIPTION       30  /* ¿ Item Name ? */
#define LEN_TEMP_STRING       20
#define LEN_TEMP_BUFFER       10
#define LEN_FILE_NAME         20

#define LEN_WIZARD_NAME       20
#define LEN_HOF_NAME          20
#define LEN_BUILDING_NAME     20
#define LEN_SPELL_NAME        20
#define LEN_SPELLDAT_NAME        19
#define LEN_SPELL_DESCRIPTION   110
#define LEN_CITY_NAME         14    /* WTF, Mate? */
#define LEN_HERO_NAME         14    /* WTF, Mate? */  /* longest in NAMES is 12 */
#define LEN_SAVE_DESCRIPTION  20
#define LEN_RACE_NAME         20
#define LEN_ITEM_NAME         30    /* array is 30, but last 4 are always 0, longest name is 26 characters */
#define LEN_ITEM_POWER_NAME   18





#define ST_FAILURE       0
#define ST_SUCCESS      -1

#define ST_FALSE         0
#define ST_TRUE          1

#define ST_NULL          0

#define ST_NONE          0
#define ST_UNDEFINED    -1  /* � "Empty" ? (// Army List Screen - "Empty Hero Slot") */
/* Elsewhere, "NOT IN USE" � for Fields ? � Debug for outline of fields on screen */



#define SZ_16K_B                16384
#define SZ_32K_B                32768
#define SZ_PARAGRAPH_B             16
#define SZ_SEGMENT                 16  /* 16-bit memory segment size */
#define SZ_EMM_PHYSICAL_PAGE    16384
#define SZ_EMM_LOGICAL_PAGE     16384



#endif /* MOX_BASE_H */
