/*

*/

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"  /* _cities[], _UNITS[] */
#include "../../MoX/src/MOM_Data.h"  /* _cities[], _UNITS[] */
#include "../src/CITYCALC.h"
#include "../src/NEXTTURN.h"
#include "../src/UNITTYPE.h"
#ifdef __cplusplus
}
#endif

void Manufacture_Test_World(void)
{
    _units = 0;
    _UNITS = (struct s_UNIT * )Allocate_Space((sizeof(struct s_UNIT) * 2) / SZ_PARAGRAPH_B);

    Create_Unit__WIP(ut_LizSettlers, 0, 1, 1, 0, 2000);
    Create_Unit__WIP(ut_LizHalberdiers, 1, 2, 2, 1, 2000);
}