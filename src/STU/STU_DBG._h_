#ifndef STU_DBG_H
#define STU_DBG_H

#include <stdio.h>

#include "STU_TYPE.H"

#include "STU.H"
#include "STU_TYPE.H"
#include "STU_DEF.H"
#include "STU_UTIL.H"   /* get_datetime() */
#include "STU_CHK.H"



/*
    ¿ Game-State Debug Variables ?
*/



extern uint32_t DBG__cmbt_cancel_button_seg_size;



extern int16_t STU_DBG_trigger_event;



// WZD dseg:9136
extern struct s_ITEM * DBG_ORIG_TBL_Items;
// WZD dseg:913A
extern SAMB_ptr DBG_ORIG_TBL_Premade_Items;
// WZD dseg:9232
extern SAMB_ptr DBG_ORIG_p0_heroes;
// WZD dseg:9236
extern SAMB_ptr DBG_ORIG_p1_heroes;
// WZD dseg:923A
extern SAMB_ptr DBG_ORIG_p2_heroes;
// WZD dseg:923E
extern SAMB_ptr DBG_ORIG_p3_heroes;
// WZD dseg:9242
extern SAMB_ptr DBG_ORIG_p4_heroes;
// WZD dseg:9246
extern SAMB_ptr DBG_ORIG_p5_heroes;
// WZD dseg:9998
// extern struct s_EVENT_DATA * DBG_ORIG__events_table;
extern int16_t * DBG_ORIG__events_table;
// WZD dseg:9CAC
extern SAMB_ptr DBG_ORIG_TBL_MoveMaps_EMS;
// WZD dseg:9CB0
extern uint8_t * DBG_ORIG_TBL_Scouting;
// WZD dseg:9CB4  
extern uint8_t * DBG_ORIG__map_square_flags;
// WZD dseg:9CB8  
extern SAMB_ptr  DBG_ORIG_TBL_Terr_Specials;
// WZD dseg:9CBC  
extern struct s_CITY * DBG_ORIG__CITIES;
// WZD dseg:9CC0
extern struct s_LAIR * DBG_ORIG__LAIRS;
// WZD dseg:9CC4  
extern struct s_TOWER * DBG_ORIG__TOWERS;
// WZD dseg:9CC8  
extern struct s_FORTRESS * DBG_ORIG__FORTRESSES;
// WZD dseg:9CCC  
extern struct s_NODE * DBG_ORIG__NODES;
// WZD dseg:9CD0
extern uint8_t * DBG_ORIG__landmasses;
// WZD dseg:9CD4
extern SAMB_ptr DBG_ORIG_UU_TBL_2;
// WZD dseg:9CD8
extern SAMB_ptr DBG_ORIG_UU_TBL_1;
// WZD dseg:9CDC
extern uint8_t * DBG_ORIG__world_maps;
// WZD dseg:9EC2
extern struct s_UNIT * DBG_ORIG__UNITS;
// WZD dseg:9EC6
extern struct s_INACTV_HERO * DBG_ORIG_TBL_Hero_Names;
// WZD dseg:9ECA
// extern struct s_WIZARD DBG_ORIG__players[];


extern int DBG_Draw_Map_Cities;
extern int DBG_TST_Made_Map_Move;
extern int DBG_TST_Selected_Stack;
extern int DBG_TST_Validate_Entities;

/* DBG Function Flags */
extern int DBG_TST_Set_Entities_On_Map_Window;
extern int DBG_TST_Select_Unit_Stack;
extern int DBG_TST_Build_Unit_Stack;
extern int DBG_TST_Draw_Map_Units;
extern int DBG_Main_Screen_Draw_Summary_Window;
extern int DBG_Print_Integer_Right;
extern int DBG_Print_Right;
extern int DBG_Get_String_Width;
extern int DBG_Print;
extern int DBG_Print_Display;
extern int DBG_Print_String;
extern int DBG_Print_Character;
extern int DBG_Print_Character_ASM;
extern int DBG_Print_Character_No_Alias;
extern int DBG_Print_Character_No_Alias_ASM;
extern int DBG_Remap_Draw_Picture_ASM;

extern int DBG_Outline_Bitmap_Pixels_No_Glass;
extern int DBG_Draw_Invisibility;
extern int DBG_Create_Remap_Palette;
extern int DBG_Find_Closest_Color;

extern int16_t DBG_TST_unit_stack[];
extern int16_t DBG_TST_entities_on_movement_map[];
/*
    ¿ Game-State Debug Variables ?
*/



#ifdef __cplusplus
extern "C" {
#endif

// #define STU_DEBUG_BREAK() ;() (void)0
// Expands to:
// ;() (void)0

#ifdef STU_DEBUG
#ifdef _WIN32
#define STU_DEBUG_BREAK() __debugbreak()
#endif
#else
#define STU_DEBUG_BREAK() (void)0
#endif


extern FILE * Debug_Log_File;

#ifdef STU_DEBUG
#define DLOG(_str_) { do { fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s\n", __FILE__, __LINE__, (_str_)); fflush(Debug_Log_File); } while(0); }
#else
/* emptiness, when debugging is not enabled */
#define DLOG
#endif



void Debug_Log_Startup(void);
void Debug_Log_Shutdown(void);

void dbg_prn(const char * fmt, ...);


#ifdef __cplusplus
}
#endif

#endif /* STU_DBG_H */
