
#include "MoX_TYPE.H"

#include "MoX_LBX.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"  /* DLOG() */
#endif


/*
    Unitialized Data
*/
uint16_t MoX_RAM_Min_KB;                    // MGC dseg:A5C4  ; set to 583 in _main



// MGC s10p17
    // ();

void MoX_RAM_SetMinKB(int16_t Min_KB)
{
    MoX_RAM_Min_KB = Min_KB;
}
