// _s13pc.c EMM_Load_LBX_1
// ST_EMM.H

//#include <stdio.h>

#include "MOM_DEF.H"

void EMM_Load_LBX_File_1(char *argLbxFileName)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Load_LBX_File_1(argLbxFileName=%s)\n", __FILE__, __LINE__, argLbxFileName);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] CALL: EMM_Load_LBX_File(argLbxFileName=%s, argEmmRsvd=1);\n", __FILE__, __LINE__, argLbxFileName);
#endif
    EMM_Load_LBX_File(argLbxFileName, 1);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Load_LBX_File_1(argLbxFileName=%s)\n", __FILE__, __LINE__, argLbxFileName);
#endif
}
