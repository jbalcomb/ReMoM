#include "MOX_DAT.h"

#include "MOM_Data.h"  /* struct s_WIZARD*/
#include "MOX_TYPE.h"

#include "../../MoX/src/MOM_DEF.h"  /* NUM_PLAYERS */



// WZD dseg:998E
SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()



// WZD dseg:9ECA
struct s_WIZARD _players[NUM_PLAYERS];
