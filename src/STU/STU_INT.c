
#include <stdint.h>

#include "STU_INT.h"



uint16_t STU_INT(uint16_t interrupt, uint16_t _AX_)
{

    uint16_t return_value = 0;

    switch(interrupt)
    {

        case 0x67:
        {

            return_value = STU_INT_67(_AX_);
            
        } break;

    }

    return return_value;

}

/*
Returns 'unallocated raw pages'

XREF:
    WZD_main()
        EMS_Pages_Left = STU_INT(0x67, 5901);

mov  ax, 5901h; int  67h;
;  - LIM EMS 4.0 - GET EXPANDED    MEMORY HARDWARE    INFORMATION
; AL = get unallocated raw page    count
; Return: BX = unallocated raw pages, DX = total raw pages

01  get unallocated raw page count

*/
uint16_t STU_INT_67(uint16_t _AX_)
{



}

/*
on return:
	BX = unallocated raw pages (AL = 01)
	DX = total raw pages (AL = 01)
	AH = status  (see EMS STATUS)
*/
uint16_t STU_INT_67_59(uint8_t _AL_)
{



}
