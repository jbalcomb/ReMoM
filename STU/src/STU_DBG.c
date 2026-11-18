
/*
https://olegkutkov.me/2019/03/25/simple-logger-with-stdout-files-and-syslog-support-for-c-projects-in-linux/
*/

// #ifdef _WIN32
// #include <STDARG.H>     /* va_list; va_arg(), va_end(), va_start() */
// #include <STDIO.H>      /* FILE; fclose(), fopen() */
// #include <STDLIB.H>     /* exit() */
// // C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt\corecrt_wstdio.h
// // C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt\stdio.h
// #endif

#include "../../ext/stu_compat.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "STU_TYPE.h"
#include "STU_DBG.h"
#include "STU_DEF.h"
#include "STU_UTIL.h"   /* get_datetime() */



/*
    ¿ Game-State Debug Variables ?
*/



uint32_t DBG__cmbt_cancel_button_seg_size;



int16_t DBG_trigger_research = STU_FALSE;



#ifdef STU_DEBUG
/*
    Debug triggers and the Z-hotkey debug flag -- moved from MoM/src/MOM_DBG.c.  Read by EVENTS.c, NEXTTURN.c, MainMenu.c, MainScr.c and Combat.c,
    each use inside #ifdef STU_DEBUG.  The DBG_trigger_event_* variants have no readers.
*/
int16_t DBG_trigger_event = STU_FALSE;
/*
    et_None                 =  0,
    et_Meteor               =  1,
    et_Gift                 =  2,
    et_Disjunction          =  3,
    et_Marriage             =  4,
    et_Earthquake           =  5,
    et_Pirates              =  6,
    et_Plague               =  7,
    et_Rebellion            =  8,
    et_Donation             =  9,
    et_Depletion            = 10,
    et_New_Mine             = 11,
    et_Population_Boom      = 12,
    et_Good_Moon            = 13,
    et_Bad_Moon             = 14,
    et_Conjunction_Chaos    = 15,
    et_Conjunction_Nature   = 16,
    et_Conjunction_Sorcery  = 17,
    et_Mana_Short           = 18
*/
/*
    Plague
    Population Boom
    Good Moon
    Bad Moon
    Conjuction - Chaos
    Conjuction - Nature
    Conjuction - Sorcery
    Mana Short
*/
int16_t DBG_trigger_event_plague = STU_FALSE;
int16_t DBG_trigger_event_population_boom = STU_FALSE;
int16_t DBG_trigger_event_good_moon = STU_TRUE;
int16_t DBG_trigger_event_bad_moon = STU_FALSE;
int16_t DBG_trigger_event_conjunction_chaos = STU_FALSE;
int16_t DBG_trigger_event_conjunction_nature = STU_FALSE;
int16_t DBG_trigger_event_conjunction_sorcery = STU_FALSE;
int16_t DBG_trigger_event_mana_short = STU_FALSE;
/*
    msg_unit_lost_ctr
    MSG_UnitKilled_Count
    MSG_CityGrowth_Count
    MSG_CityDeath_Count
    MSG_BldLost_Count
    MSG_UEsLost_Count
    MSG_CEsLost_Count
    MSG_GEs_Lost
*/
int16_t DBG_trigger_offer_item = STU_FALSE;
int16_t DBG_trigger_offer_merc = STU_FALSE;
int16_t DBG_trigger_offer_hero = STU_FALSE;

int16_t DBG_trigger_complete_research_spell = STU_FALSE;

uint8_t DBG_debug_flag = STU_FALSE;



/*
    Map-square trace filter -- moved from MoM/src/MainScr_Maps.c (Copilot TEMP DEBUG, cd6f198a).  No callers.
*/
/* Copilot TEMP DEBUG: set DBG_TRACE_FIX_WX/WY/WP to filter a single square; leave STU_UNDEFINED for all. */
int16_t DBG_TRACE_FIX_WX = STU_UNDEFINED;
int16_t DBG_TRACE_FIX_WY = STU_UNDEFINED;
int16_t DBG_TRACE_FIX_WP = STU_UNDEFINED;
int16_t DBG_TRACE_FIX_BUDGET = 120;

int16_t DBG_Trace_Fix_Match(int16_t wx, int16_t wy, int16_t wp)
{
    if((DBG_TRACE_FIX_WP != STU_UNDEFINED) && (DBG_TRACE_FIX_WP != wp))
    {
        return STU_FALSE;
    }

    if((DBG_TRACE_FIX_WX != STU_UNDEFINED) && (DBG_TRACE_FIX_WX != wx))
    {
        return STU_FALSE;
    }

    if((DBG_TRACE_FIX_WY != STU_UNDEFINED) && (DBG_TRACE_FIX_WY != wy))
    {
        return STU_FALSE;
    }

    return STU_TRUE;
}
#endif  /* STU_DEBUG */



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
uint8_t * DBG_ORIG__map_square_flags;
// WZD dseg:9CB8  
SAMB_ptr  DBG_ORIG__map_square_terrain_specials;
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
uint8_t * DBG_ORIG__landmasses;
// WZD dseg:9CD4
SAMB_ptr DBG_ORIG_connectivity_grid_sea;
// WZD dseg:9CD8
SAMB_ptr DBG_ORIG_connectivity_grid_land;
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



char Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp

char Debug_Log_FileName[] = "DEBUG.LOG";
FILE * Debug_Log_File = NULL;

char Trace_Log_FileName[] = "TRACE.LOG";
FILE * Trace_Log_File = NULL;


void Debug_Log_Startup(void)
{
// ==33072==ERROR: AddressSanitizer: attempting free on address which was not malloc()-ed: 0x02c500158f90 in thread T0
    Debug_Log_File = stu_fopen(Debug_Log_FileName, "w");
    if(Debug_Log_File == NULL)
    {
        printf("DEBUG [FATAL]: Unable to open log file: %s", Debug_Log_FileName);
#ifdef _WIN32
        exit(1);
#endif
    }
    get_datetime(&Log_ISO8601_DateTime[0]);
    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Log_ISO8601_DateTime, "BEGIN: Debug Log");
    fflush(Debug_Log_File);
}

void Debug_Log_Shutdown(void)
{
    get_datetime(&Log_ISO8601_DateTime[0]);
    fprintf(Debug_Log_File, "[%s] DEBUG: %s\n", Log_ISO8601_DateTime, "END: Debug Log");
    fflush(Debug_Log_File);
    stu_fclose(Debug_Log_File);
}

void Trace_Log_Startup(void)
{
    Trace_Log_File = stu_fopen(Trace_Log_FileName, "w");
    if(Trace_Log_File == NULL)
    {
        printf("TRACE [FATAL]: Unable to open log file: %s", Trace_Log_FileName);
#ifdef _WIN32
        exit(1);
#endif
    }
    get_datetime(&Log_ISO8601_DateTime[0]);
    fprintf(Trace_Log_File, "[%s] TRACE: %s\n", Log_ISO8601_DateTime, "BEGIN: Trace Log");
    fflush(Trace_Log_File);
}

void Trace_Log_Shutdown(void)
{
    get_datetime(&Log_ISO8601_DateTime[0]);
    fprintf(Trace_Log_File, "[%s] TRACE: %s\n", Log_ISO8601_DateTime, "END: Trace Log");
    fflush(Trace_Log_File);
    stu_fclose(Trace_Log_File);
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

// "trc" as in "trace"; "prn" as in "print"/"printf";
void trc_prn(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(Trace_Log_File, fmt, args);
    va_end(args);
    /*
        flush output buffer to disk, in case of crash

        fysnc() flush kernel buffers, as well as userspace buffers, on a POSIX system (i.e., NOT Windows)
    */
   fflush(Trace_Log_File);
}
