
/*
https://olegkutkov.me/2019/03/25/simple-logger-with-stdout-files-and-syslog-support-for-c-projects-in-linux/
*/

#include <STDARG.H>     /* va_list; va_arg(), va_end(), va_start() */
#include <STDIO.H>      /* FILE; fclose(), fopen() */
#include <STDLIB.H>     /* exit() */
// C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt\corecrt_wstdio.h
// C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt\stdio.h

#include "STU_TYPE.H"
#include "STU_DBG.H"
#include "STU_UTIL.H"   /* get_datetime() */



/*
    ¿ Game-State Debug Variables ?
*/

// WZD dseg:9136
struct s_ITEM * DBG_ORIG_TBL_Items;
// WZD dseg:913A
SAMB_ptr DBG_ORIG_TBL_Premade_Items;
// WZD dseg:9232
SAMB_ptr DBG_ORIG_p0_heroes;
// WZD dseg:9236
SAMB_ptr DBG_ORIG_p1_heroes;
// WZD dseg:923A
SAMB_ptr DBG_ORIG_p2_heroes;
// WZD dseg:923E
SAMB_ptr DBG_ORIG_p3_heroes;
// WZD dseg:9242
SAMB_ptr DBG_ORIG_p4_heroes;
// WZD dseg:9246
SAMB_ptr DBG_ORIG_p5_heroes;
// WZD dseg:9998
// struct s_EVENT_DATA * DBG_ORIG__events_table;
int16_t * DBG_ORIG__events_table;
// WZD dseg:9CAC
SAMB_ptr DBG_ORIG_TBL_MoveMaps_EMS;
// WZD dseg:9CB0
uint8_t * DBG_ORIG_TBL_Scouting;
// WZD dseg:9CB4  
uint8_t * DBG_ORIG_TBL_Terrain_Flags;
// WZD dseg:9CB8  
SAMB_ptr  DBG_ORIG_TBL_Terr_Specials;
// WZD dseg:9CBC  
struct s_CITY * DBG_ORIG__CITIES;
// WZD dseg:9CC0
struct s_LAIR * DBG_ORIG__LAIRS;
// WZD dseg:9CC4  
struct s_TOWER * DBG_ORIG__TOWERS;
// WZD dseg:9CC8  
struct s_FORTRESS * DBG_ORIG__FORTRESSES;
// WZD dseg:9CCC  
struct s_NODE * DBG_ORIG__NODES;
// WZD dseg:9CD0
uint8_t * DBG_ORIG_TBL_Landmasses;
// WZD dseg:9CD4
SAMB_ptr DBG_ORIG_UU_TBL_2;
// WZD dseg:9CD8
SAMB_ptr DBG_ORIG_UU_TBL_1;
// WZD dseg:9CDC
uint8_t * DBG_ORIG__world_maps;
// WZD dseg:9EC2
struct s_UNIT * DBG_ORIG__UNITS;
// WZD dseg:9EC6
struct s_INACTV_HERO * DBG_ORIG_TBL_Hero_Names;
// WZD dseg:9ECA
// struct s_WIZARD DBG_ORIG__players[6];



int DBG_Draw_Map_Cities = 0;
int DBG_TST_Made_Map_Move = 0;
int DBG_TST_Selected_Stack = 0;
int DBG_TST_Validate_Entities = 0;  // test is _UNITS[55] ? wx,wy,wp 

/* DBG Function Flags */
int DBG_TST_Set_Entities_On_Map_Window = 0;
int DBG_TST_Select_Unit_Stack = 0;
int DBG_TST_Build_Unit_Stack = 0;
int DBG_TST_Draw_Map_Units = 0;
int DBG_Main_Screen_Draw_Summary_Window = 0;
int DBG_Print_Integer_Right = 0;
int DBG_Print_Right = 0;
int DBG_Get_String_Width = 0;
int DBG_Print = 0;
int DBG_Print_Display = 0;
int DBG_Print_String = 0;
int DBG_Print_Character = 0;
int DBG_Print_Character_ASM = 0;
int DBG_Print_Character_No_Alias = 0;
int DBG_Print_Character_No_Alias_ASM = 0;
int DBG_Remap_Draw_Picture_ASM = 0;

int DBG_Outline_Bitmap_Pixels_No_Glass = 0;
int DBG_Draw_Invisibility = 0;
int DBG_Create_Remap_Palette = 0;
int DBG_Find_Closest_Color = 0;

int16_t DBG_TST_unit_stack[18] =
{
    0, 0, 
    0, 0, 
    0, 0, 
    0, 0, 
    0, 0, 
    0, 0, 
    0, 0, 
    0, 0, 
    0, 0
};
int16_t DBG_TST_entities_on_movement_map[120] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
/*
    ¿ Game-State Debug Variables ?
*/



char Debug_Log_FileName[] = "DEBUG.LOG";
FILE * Debug_Log_File = NULL;
char Debug_Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp


void Debug_Log_Startup(void)
{
    Debug_Log_File = fopen(Debug_Log_FileName, "w");
    if(Debug_Log_File == NULL)
    {
        printf("DEBUG [FATAL]: Unable to open log file: %s", Debug_Log_FileName);
        exit(1);
    }
    get_datetime(&Debug_Log_ISO8601_DateTime[0]);
    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Debug_Log_ISO8601_DateTime, "BEGIN: Debug Log");
    fflush(Debug_Log_File);
}

void Debug_Log_Shutdown(void)
{
    get_datetime(&Debug_Log_ISO8601_DateTime[0]);
    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Debug_Log_ISO8601_DateTime, "END: Debug Log");
    fflush(Debug_Log_File);
    fclose(Debug_Log_File);
}

// "dbg" as in "debug"; "prn" as in "print"/"printf";
void dbg_prn(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(Debug_Log_File, fmt, args);
    va_end(args);
    /*
        flush output buffer to disk, in case of crash

        fysnc() flush kernel buffers, as well as userspace buffers, on a POSIX system (i.e., NOT Windows)
    */
   fflush(Debug_Log_File);
}
