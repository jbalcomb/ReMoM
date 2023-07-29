#ifndef LOADER_H
#define LOADER_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:291E                            BEGIN: ovr052

// WZD dseg:291E                            Load/Init Main Game

// WZD dseg:291E rsc01_MAIN_LBX db 'MAIN.LBX',0        
// WZD dseg:2927 rsc02_BACKGRND_LBX db 'BACKGRND.LBX',0
// WZD dseg:2934 rsc03_UNITS1_LBX db 'UNITS1.LBX',0    
// WZD dseg:293F rsc04_UNITS2_LBX db 'UNITS2.LBX',0    
// WZD dseg:294A rsc05_UNITVIEW_LBX db 'UNITVIEW.LBX',0
// WZD dseg:2957 rsc06_SPECIAL_LBX db 'SPECIAL.LBX',0  
// WZD dseg:2963 rsc07_SPECIAL2_LBX db 'SPECIAL2.LBX',0
// WZD dseg:2970 rsc08_ITEMS_LBX db 'ITEMS.LBX',0      
// WZD dseg:297A rsc09_CHRIVER_LBX db 'CHRIVER.LBX',0  
// WZD dseg:2986 rsc0A_ITEMISC_LBX db 'ITEMISC.LBX',0  
// WZD dseg:2992 rsc0B_CITYSCAP_LBX db 'CITYSCAP.LBX',0

// WZD dseg:299F
extern char rsc0C_MAPBACK_LBX[];

// WZD dseg:29AB rsc0D_CMBMAGIC_LBX db 'CMBMAGIC.LBX',0
// WZD dseg:29B8 rsc0E_CMBTCITY_LBX db 'CMBTCITY.LBX',0
// WZD dseg:29C5 rsc0F_CITYWALL_LBX db 'CITYWALL.LBX',0

// WZD dseg:29D2                            Init_Terrain()

// WZD dseg:29D2
extern char terrain_lbx_file[];

// dseg:29DE terrstat_lbx_file db 'TERRSTAT',0

// WZD dseg:29E7
extern char mapback_lbx_file[];

// WZD dseg:29EF                            Menu_Screen_Load_Pictures()

// WZD dseg:29EF main_lbx_file db 'MAIN',0
// WZD dseg:29F4 backgrnd_lbx_file db 'BACKGRND',0
// WZD dseg:29FD cnst_SOUNDFX_File db 'SOUNDFX',0
// WZD dseg:2A05 spelldat_lbx_file db 'SPELLDAT.LBX',0
// WZD dseg:2A12 cnst_BUILDDAT_File db 'BUILDDAT.LBX',0
// WZD dseg:2A1F cnst_Units1_File db 'UNITS1',0
// WZD dseg:2A26 cnst_Units2_File db 'UNITS2',0
// WZD dseg:2A2D cnst_CMBMAGIC_File db 'CMBMAGIC',0
// WZD dseg:2A36 cnst_CMBTCITY_File db 'CMBTCITY',0
// WZD dseg:2A3F cnst_CHRIVER_File db 'CHRIVER',0
// WZD dseg:2A47 cnst_CITYWALL_File db 'CITYWALL',0
// WZD dseg:2A50 cnst_CITYSCAP_File db 'CITYSCAP',0
// WZD dseg:2A59 cnst_UNITVIEW_File db 'UNITVIEW',0
// WZD dseg:2A62 cnst_SPECIAL1_File db 'SPECIAL',0
// WZD dseg:2A6A cnst_SPECIAL2_File db 'SPECIAL2',0
// WZD dseg:2A73 cnst_ITEMS_File db 'ITEMS',0
// WZD dseg:2A79 cnst_ITEMISC_File db 'ITEMISC',0
// WZD dseg:2A81 special2_lbx_file db 'special2',0

// WZD dseg:2A81                            END: ovr052



/*
    MAGIC.EXE   seg001
*/

// MGC s01p04
void Load_MGC_Resources(void);


/*
    WIZARDS.EXE  ovr052
*/

// WZD o52p01
void Load_WZD_Resources(void);

// WZD o52p02
void UNIT_Upkeep_Reset(void);

// WZD o52p03
void Terrain_Init(void);

// WZD o52p20
void GFX_Swap_Reset(void);

// WZD o52p22
// AKA GFX_Swap_AppendUnits()
void Load_Unit_StatFigs(void);

// WZD o52p30
void GFX_Swap_Cities(void);



#ifdef __cplusplus
}
#endif

#endif  /* LOADER_H */
