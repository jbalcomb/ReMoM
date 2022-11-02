#ifndef TST_MGC_H
#define TST_MGC_H

#include "ST_TYPE.H"

/*
    mainmenu_top = Farload(mainscrn_lbx_file, 0);
    mainmenu_bot = Farload(mainscrn_lbx_file, 5);
    mainmenu_c   = Farload(vortex_lbx_file, 1);
    mainmenu_h   = Farload(vortex_lbx_file, 2);
    mainmenu_q   = Farload(vortex_lbx_file, 3);
    mainmenu_n   = Farload(vortex_lbx_file, 4);
    mainmenu_l   = Farload(vortex_lbx_file, 5);
*/
SAMB_addr sad_MAINSCRN_000;
SAMB_ptr  sap_MAINSCRN_000;
SAMB_addr sad_MAINSCRN_005;
SAMB_ptr  sap_MAINSCRN_005;
SAMB_addr sad_VORTEX_001;
SAMB_ptr  sap_VORTEX_001;
SAMB_addr sad_VORTEX_002;
SAMB_ptr  sap_VORTEX_002;
SAMB_addr sad_VORTEX_003;
SAMB_ptr  sap_VORTEX_003;
SAMB_addr sad_VORTEX_004;
SAMB_ptr  sap_VORTEX_004;
SAMB_addr sad_VORTEX_005;
SAMB_ptr  sap_VORTEX_005;
// WIZARDS 0, ..., 13
struct s_TST_LBX_Entry_Loaded
{
    char name[13];
    SAMB_addr sad_VORTEX_005;
    SAMB_ptr  sap_VORTEX_005;
};
struct s_TST_LBX_Entry_Loaded TST_LBX_Entry_Loaded[20];



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


#ifdef __cplusplus
}
#endif

#endif /* TST_MGC_H */
