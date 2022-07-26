// _s12p12c.c EMM_MapMulti4
// ST_EMM.H

#include <DOS.H>

#include "ST_EMM.H"


void EMM_MapMulti4(int argFirstLogicalPage, int argEmmHandle)
{
    unsigned int bkup_DS;
    unsigned int tmp_log_page;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_MapMulti4(argFirstLogicalPage=%d, argEmmHandle=%d)\n", __FILE__, __LINE__, argFirstLogicalPage, argEmmHandle);
#endif

    bkup_DS = _DS;

    tmp_log_page = argFirstLogicalPage;
    _DX = argEmmHandle;  // DX = EMM handle

    gfp_map_struct[0].log_page = tmp_log_page++;
    gfp_map_struct[1].log_page = tmp_log_page++;
    gfp_map_struct[2].log_page = tmp_log_page++;
    gfp_map_struct[3].log_page = tmp_log_page++;

    _DS = FP_SEG(gfp_map_struct);
    _SI = FP_OFF(gfp_map_struct);  // DS:SI = pointer to mapping array
    _CX = 0x04;  // CX = number of entries in array
    _AL = 0x00;  // 00 Map/unmap pages
    _AH = 0x50;  // INT 67,50 - Map/Unmap Multiple Handle Pages (LIM EMS 4.0+)
    geninterrupt(EMS_INT);

    _DS = bkup_DS;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_MapMulti4(argFirstLogicalPage=%d, argEmmHandle=%d)\n", __FILE__, __LINE__, argFirstLogicalPage, argEmmHandle);
#endif
}
