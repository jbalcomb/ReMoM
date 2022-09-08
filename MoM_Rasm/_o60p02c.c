// _o60p02c.c GAME_PrepareCredits
// MGC_DEF.H

#include "MGC_DEF.H";
#include "MOM_DEF.H";
#include "ST_FLIC.H";
#include "ST_GUI.H";
#include "ST_SA.H"


void GAME_PrepareCredits(void)
{
    GAME_FirstShownCred = 0;
    GAME_CreditTextTop = 95;
    gsa_GUI_Credits_Text_IMG = FP_SEG(SA_Alloc_First(MK_FP(gsa_Sandbox,0), 3125));
    FLIC_Prepare(280, 122, gsa_GUI_Credits_Text_IMG);
}
