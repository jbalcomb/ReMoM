#include "MOX_DAT.H"

#include "MOX_TYPE.H"

#include "../MOM_DEF.H"  /* NUM_PLAYERS */
#include "../MOM_Data.H"  /* struct s_WIZARD*/



// WZD dseg:998E
SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()



// WZD dseg:9ECA
struct s_WIZARD _players[NUM_PLAYERS];
