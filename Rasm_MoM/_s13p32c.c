// _s13p32.cpp EMM_GetLowString (_s13p32.asm)
#include <stdlib.h>  /* itoa() */
#include <stdio.h>   /* printf() */
#include <string.h>  /* strcat(), strcpy() */

#include "MOM_DEF.H"
#include "ST_EMM.H"

void EMM_BuildEmmErrStr(char *str_buf)
{
    char tmp_string[20];

    printf("DEBUG: BEGIN: EMM_BuildEmmErrStr()\n");
    //wait_for_any_key();

    printf("DEBUG: str_buf:  [%p]  %s\n", str_buf, str_buf);

    itoa(g_EMM_MinKB, tmp_string, 10);
    strcpy(str_buf, "You must have at least ");
    strcat(str_buf, tmp_string);
    strcat(str_buf, "K of expanded memory.");

    printf("DEBUG: END: EMM_BuildEmmErrStr()\n");
}
