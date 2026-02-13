
#include "MGC_DATA.h"

#include "../../MoX/src/MOX_TYPE.h"



// MGC dseg:5BAC
/*
void Load_TERRSTAT(void)
    terrain_stats_table = LBX_Load_Data(terrstat_lbx_file, 0, 0, 770, 6);
...
{1,0,2,3,4,5}
{Walking,Unused_MoveType,Forester,Mountaineer,Swimming,Sailing}

NOT Unit Type - Movement Type
NOT Movement Modes or MM Movement Modes
NOT whatever is OFS_

*/
SAMB_ptr terrain_stats_table;
// MGC dseg:5BAC                                                                                         ; TERRSTAT,0 - 770 records of 6 bytes each in a single allocation
