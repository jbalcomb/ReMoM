#ifndef MOM_H
#define MOM_H

#include "MoX.H"
#include "MoM_DEF.H"

#include "MoM_Data.H"

#include "MoM_DBG.H"

/* ~ MoX ~ MoM v1.31*/
#include "AdvsrScr.H"
#include "AIBUILD.H"
#include "AIDATA.H"
#include "AIDUDES.H"
#include "ALLOC.H"
#include "ArmyList.H"
#include "City_ovr55.H"
#include "CityList.H"
#include "CITYCALC.H"
#include "CITYSCAP.H"
#include "CityScr.H"
#include "Combat.H"
#include "DIPLOMAC.H"
#include "EVENTS.H"
#include "Explore.H"
#include "Fields.H"
#include "Fonts.H"
#include "GENDRAW.H"
#include "Graphics.H"
#include "Help.H"
#include "HIRE.H"
#include "Init.H"
#include "Input.H"
#include "ItemMake.H"
#include "Items.H"
#include "ItemScrn.H"
#include "ItemView.H"
#include "Keyboard.H"
#include "Lair.H"
#include "LOADER.H"
#include "LoadScr.H"
#include "LVLMAKE.H"
#include "MainMenu.H"
#include "MagicScr.H"
// TODO  #include "MainScr_Maps.H"
#include "MainScr.H"
#include "MovePath.H"
#include "MoX_CFG.H"
#include "MoX_CMB.H"
#include "MoX_DIR.H"
#include "MoX_DOS.H"
#include "MoX_GAM.H"
#include "MoX_RNG.H"
#include "MoX_UnOrg.H"  // WTFMATE
#include "MoX_Util.H"   // WTFMATE
#include "NEXTTURN.H"
#include "Outpost.H"
#include "paragrph.H"
#include "ProdScr.H"
#include "RACETYPE.H"
#include "REPORT.H"
#include "Roads.H"
#include "SBookScr.H"
#include "SCORE.H"      // Hall Of Fame (HoF)
#include "Settings.H"
#include "SETTLE.H"
#include "special.H"
#include "Spellbook.H"
#include "SOUND.H"
#include "SPLMASTR.H"
#include "Terrain.H"
#include "TerrType.H"
#include "Timer.H"
#include "UnitList.H"
#include "UnitMove.H"   // WTFMATE
#include "UnitStat.H"
#include "UNITSTK.H"
#include "UNITTYPE.H"   // WTFMATE
#include "UnitView.H"
#include "Video.H"
#include "Video2.H"
#include "WIZVIEW.H"
#include "WZD_o059.H"
#include "WZD_o129.H"
#include "WZD_o143.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif
#include "STU\STU_DBG.H"

#include "MoX_PFL.H"

#include <assert.h>
#include <math.h>       /* sqrt() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa(); */
#include <string.h>     /* memset(), strcat(), strcpy(); */

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE seg001
*/

// WZD s01p02
// OON XREF:  main()  WZD s01p02
void Screen_Control(void);



#ifdef __cplusplus
}
#endif

#endif  /* MOM_H */
