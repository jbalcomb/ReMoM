// _s12p02c.c EMM_GetHandleCount

#include "ST_HEAD.H"
#include "MOM_DEF.H"
#include "ST_EMM.H"


unsigned int EMM_GetActiveHandleCount(void)
{
    unsigned int emm_handle_count;

asm mov ah, 4Bh
asm int 67h
asm mov [emm_handle_count], bx

    return(emm_handle_count);
}
