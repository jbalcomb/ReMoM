#include "MOX_DAT.H"

#include "MoX_TYPE.H"

#include "../MoM_DEF.H"  /* NUM_PLAYERS */
#include "../MoM_Data.H"  /* struct s_WIZARD*/



// WZD dseg:998E
SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()



// WZD dseg:9ECA
struct s_WIZARD _players[NUM_PLAYERS];
