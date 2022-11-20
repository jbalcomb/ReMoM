
#include "win_TYPE.hpp"     /* SAMB_ptr */

#include "win_LBX.hpp"      /* LBX_Load() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


char mainscrn_lbx_file[] = "MAINSCRN";          // MGC  dseg:28A7
char vortex_lbx_file[] = "VORTEX";              // MGC  dseg:28B0

SAMB_ptr mainmenu_top;
SAMB_ptr mainmenu_bot;
SAMB_ptr mainmenu_c;
SAMB_ptr mainmenu_l;
SAMB_ptr mainmenu_n;
SAMB_ptr mainmenu_h;
SAMB_ptr mainmenu_q;


void Main_Menu_Load_Pictures(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Load_Pictures()\n", __FILE__, __LINE__);
#endif

    mainmenu_top = LBX_Load(mainscrn_lbx_file, 0);
    mainmenu_bot = LBX_Load(mainscrn_lbx_file, 5);
    mainmenu_c   = LBX_Load(vortex_lbx_file, 1);
    mainmenu_h   = LBX_Load(vortex_lbx_file, 2);
    mainmenu_q   = LBX_Load(vortex_lbx_file, 3);
    mainmenu_n   = LBX_Load(vortex_lbx_file, 4);
    mainmenu_l   = LBX_Load(vortex_lbx_file, 5);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] mainmenu_top: %p\n", __FILE__, __LINE__, mainmenu_top);
    dbg_prn("DEBUG: [%s, %d] mainmenu_bot: %p\n", __FILE__, __LINE__, mainmenu_bot);
    dbg_prn("DEBUG: [%s, %d] mainmenu_c: %p\n", __FILE__, __LINE__, mainmenu_c);
    dbg_prn("DEBUG: [%s, %d] mainmenu_h: %p\n", __FILE__, __LINE__, mainmenu_h);
    dbg_prn("DEBUG: [%s, %d] mainmenu_q: %p\n", __FILE__, __LINE__, mainmenu_q);
    dbg_prn("DEBUG: [%s, %d] mainmenu_n: %p\n", __FILE__, __LINE__, mainmenu_n);
    dbg_prn("DEBUG: [%s, %d] mainmenu_l: %p\n", __FILE__, __LINE__, mainmenu_l);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Load_Pictures()\n", __FILE__, __LINE__);
#endif

}
