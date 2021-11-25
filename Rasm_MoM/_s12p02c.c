// _s12p02c.c EMM_GetHandleCount

#include "ST_HEAD.H"
#include "MOM_DEF.H"
#include "ST_EMM.H"


unsigned int EMM_GetActiveHandleCount(void)
{
    unsigned int emm_handle_count;
    printf("DEBUG: BEGIN: EMM_GetActiveHandleCount()\n");
    //wait_for_any_key();

asm mov ah, 4Bh
asm int 67h
asm mov [emm_handle_count], bx

    if (_AH)
    {
        printf("DEBUG: _AH: 0x%02X\n", _AH);
        //return(0);
    }

    printf("DEBUG: _BX: 0x%04X\n", _BX);
    printf("DEBUG: emm_handle_count: %u\n", emm_handle_count);
    
    return(emm_handle_count);
// 	_AH = EMM_GETHANDLES;
// 	geninterrupt(EMM_INT);
//     if (_AH)
// 		goto error;

//     printf("DEBUG: _BX: 0x%08X\n", _BX);
//     printf("DEBUG: _BX: 0x%08X\n", (unsigned char)_BX);
//     printf("DEBUG: _BX: 0x%08X\n", (signed char)_BX);
//     printf("DEBUG: _BX: 0x%08X\n", (unsigned short)_BX);
//     printf("DEBUG: _BX: 0x%08X\n", (signed short)_BX);
//     printf("DEBUG: _BX: 0x%08X\n", (unsigned int)_BX);
//     printf("DEBUG: _BX: 0x%08X\n", (signed int)_BX);

//     printf("DEBUG: _BX: %d\n", _BX);
//     printf("DEBUG: _BX: %u\n", _BX);
//     printf("DEBUG: _BX: %hd\n", _BX);
//     printf("DEBUG: _BX: %hu\n", _BX);
//     printf("DEBUG: _BX: %ld\n", _BX);
//     printf("DEBUG: _BX: %lu\n", _BX);

//     emm_handle_count = _BX;

//     printf("DEBUG: emm_handle_count: %d\n", emm_handle_count);
//     printf("DEBUG: emm_handle_count: %u\n", emm_handle_count);
//     printf("DEBUG: emm_handle_count: %hd\n", emm_handle_count);
//     printf("DEBUG: emm_handle_count: %hu\n", emm_handle_count);
//     printf("DEBUG: emm_handle_count: %ld\n", emm_handle_count);
//     printf("DEBUG: emm_handle_count: %lu\n", emm_handle_count);

// error:
//     printf("DEBUG: END: EMM_GetActiveHandleCount()\n");
//     //return(emm_handle_count);
//     return(_BX);

}
