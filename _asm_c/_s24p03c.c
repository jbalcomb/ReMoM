// _s24p03c.c 

#include "ST_DEF.H"


void VGA_GetDACBlock(unsigned int DAC_BLK, unsigned char * Buffer)
{
    unsigned int DAC_Reg;
    DAC_Reg = DAC_BLK * 64;
    _ES = FP_SEG(Buffer);  // ES:DX = pointer to buffer for color registers
    _DX = FP_OFF(Buffer);
    _BX = DAC_Reg;  // BX = first color register to read
    _CX = 64;  // CX = number of color registers to read
    _AL = 0x17;  // AL = 17  read block of DAC color registers
    _AH = 0x10;  // INT 10,10 - Set/Get Palette Registers (EGA/VGA)
    geninterupt(0x10);  // INT 10 - Video BIOS Services
}
