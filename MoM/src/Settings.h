/*
    WIZARDS.EXE
        ovr125
    MAGIC.EXE

WZD
MGC

MoO2
    Module: FILEDEF
    Module: INITGAME
    Module: HAROLD
    Module: LOADSAVE
    Module: RUSS

*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/MOX_SET.h"



#ifdef __cplusplus
extern "C" {
#endif



void Read_MAGIC_SET(void);
void Write_MAGIC_SET(void);



/*
    WIZARDS.EXE  ovr125
*/

// WZD o125p01
void Settings_Screen(void);

// WZD o125p02
void Settings_Screen_Draw(void);

// WZD o125p03
void Load_MAGIC_SET(void);

// WZD o125p04
void Set_Default_Game_Settings(void);

// WZD o125p05
void Set_Settings_Screen_Help_List(void);

void DBG_Print_MAGIC_SET(const char * label);
void DBG_Compare_MAGIC_SET(const struct s_MAGIC_SET * before, const struct s_MAGIC_SET * after, const char * label);



#ifdef __cplusplus
}
#endif

#endif  /* SETTINGS_H */
