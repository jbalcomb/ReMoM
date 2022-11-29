
unsigned int LFSR_LO_bits = 0x3568;       // dseg:431E  3568h  13672d  0011 0101 0110 1000b
unsigned int LFSR_HI_bits = 0x3568;       // dseg:4320


// MGC s22p07
void RND_TimerSeed(void)
{
    unsigned int Clock_Ticks_Hi;
    unsigned int Clock_Ticks_Lo;

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
asm    mov [LFSR_LO_bits], ax
asm    mov ax, [Clock_Ticks_Hi]
asm    mov [LFSR_HI_bits], ax
}
