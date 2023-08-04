#ifndef STU_DBG_H
#define STU_DBG_H

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
    ¿ Game-State Debug Variables ?
*/
extern int DBG_Draw_Map_Cities;
extern int DBG_TST_Made_Map_Move;
extern int DBG_TST_Selected_Stack;
extern int DBG_TST_Validate_Entities;
extern int DBG_TST_Set_Entities_On_Map_Window;
extern int DBG_TST_Select_Unit_Stack;
extern int DBG_TST_Build_Unit_Stack;
extern int DBG_TST_Draw_Map_Units;

extern int16_t DBG_TST_unit_stack[];
extern int16_t DBG_TST_entities_on_movement_map[];
/*
    ¿ Game-State Debug Variables ?
*/



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
