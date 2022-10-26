// _s22p07a.c RNG_TimerSeed
// ST_DEF.H

#include "ST_DEF.H"


void RNG_TimerSeed(void)
{
    unsigned int Clock_Ticks_Hi;
    unsigned int Clock_Ticks_Lo;
asm    push es
asm    mov ax, 0
asm    mov es, ax
asm    assume es:nothing
asm    mov ax, [es:46Ch]  // System Memory Address  0000:046C
asm    mov [Clock_Ticks_Lo], ax
asm    mov ax, [es:46Eh]  // System Memory Address  0000:046E
asm    mov [Clock_Ticks_Hi], ax
asm    pop es
asm    assume es:nothing
asm    mov ax, [Clock_Ticks_Lo]
asm    mov [g_LFSR_LO_bits], ax
asm    mov ax, [Clock_Ticks_Hi]
asm    mov [g_LFSR_HO_bits], ax
}
