/*
    Spellbook Screen

    WIZARDS.EXE
        ovr134
    
    SPELLSCR.LBX
*/

#ifndef SBOOKSCR_H
#define SBOOKSCR_H

#include "MOX/MOM_Data.H"
#include "MOX/MOX_BASE.H"
#include "MOX/MOX_BITS.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_SET.H"
#include "MOX/MOX_SND.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/MOX_T4.H"  // ~ MOX & MOX2 //MoO2
#include "MOX/Allocate.H"
#include "MOX/CFG.H"
#include "MOX/DBG.H"
#include "MOX/DIR.H"
#include "MOX/DOS.H"
#include "MOX/EMM.H"
#include "MOX/EXIT.H"
#include "MOX/Fields.H"
#include "MOX/file_ani.H"
#include "MOX/FLIC_Draw.H"
#include "MOX/Fonts.H"
#include "MOX/GENDRAW.H"
#include "MOX/Graphics.H"
#include "MOX/Input.H"
#include "MOX/IO_Buffer.H"
#include "MOX/Keyboard.H"
#include "MOX/LBX_Load.H"
#include "MOX/LOADSAVE.H"
#include "MOX/Mouse.H"
#include "MOX/paragrph.H"
#include "MOX/random.H"
#include "MOX/Timer.H"
#include "MOX/Util.H"
#include "MOX/Video.H"
#include "MOX/Video2.H"

/* ~ MoX ~ MoM v1.31*/
#include "AdvsrScr.H"
#include "AIBUILD.H"
#include "AIDATA.H"
#include "AIDUDES.H"
#include "AIMOVE.H"
#include "AISPELL.H"
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
#include "Help.H"
#include "HIRE.H"
#include "Init.H"
#include "ItemMake.H"
#include "Items.H"
#include "ItemScrn.H"
#include "ItemView.H"
#include "Lair.H"
#include "LOADER.H"
#include "LoadScr.H"
#include "LVLMAKE.H"
#include "MainMenu.H"
#include "MagicScr.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "MOM_DBG.H"
#include "MOM_DEF.H"
#include "MOM_PFL.H"
#include "MOM_SCR.H"
#include "MovePath.H"
#include "Outpost.H"
#include "OverSpel.H"
#include "ProdScr.H"
#include "RACETYPE.H"
#include "REPORT.H"
#include "Roads.H"
#include "SBookScr.H"
#include "SCastScr.H"
#include "SCORE.H"      // Hall Of Fame (HoF)
#include "Settings.H"
#include "SETTLE.H"
#include "special.H"
#include "Surveyor.H"
#include "Terrain.H"
#include "TerrType.H"
#include "UnitList.H"
#include "UnitMove.H"   // WTFMATE
#include "UnitStat.H"
#include "UNITSTK.H"
#include "UNITTYPE.H"   // WTFMATE
#include "UnitView.H"
#include "WIZVIEW.H"
#include "WZD_o059.H"
#include "WZD_o143.H"

#ifdef STU_DEBUG
#include "STU/STU_DBG.H"
#endif
#include "STU/STU_DBG.H"

#include <assert.h>
#include <math.h>       /* sqrt() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */
#include <stdlib.h>     /* abs(); itoa(); malloc(); */
#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
// PRIVATE  void Spell_Screen_Load(void);

// WZD o134p02
void Spellbook_Screen_Draw(void);

// WZD o134p03
void Spellbook_Screen(void);

// WZD o134p04
int16_t Cast_Spell_Overland_Do(int16_t player_idx, int16_t spell_idx, int16_t spellbook_page_spell_index);

// WZD o134p05
// sub_B9837()

// WZD o134p06
void Full_Draw_Main_Screen(void);

// WZD o134p07
void OVL_MosaicFlip__STUB(void);

// WZD o134p08
void Fizzle_Notification(int16_t player_idx, int16_t counter_player_idx, int16_t spell_idx, char * counter_spell_name);

// WZD o134p09
// sub_B9D14()
void Cast_Spell_City_Enchantment_Animation_Load_Music(int16_t spell_idx, int16_t good_bad);

// WZD o134p10
int16_t Spell_Animation_Load_Graphics(int16_t spell_idx);

// WZD o134p11
void Spell_Animation_Load_Sound_Effect__WIP(int16_t spell_idx);

// WZD o134p12
void Spell_Animation_Screen__WIP(int16_t wx, int16_t wy, int16_t wp);



#ifdef __cplusplus
}
#endif


#endif  /* SBOOKSCR_H */
