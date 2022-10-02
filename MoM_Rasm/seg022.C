// ST_DEF.H

#include "seg022.H"

#include "ST_DEF.H"  /* _FAR */
#include "ST_SA.H"  /* SAMB_addr */

#include "STU_DBG.H"


// s22p07
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

// s22p22
int ST_MoveData_FP(unsigned char _FAR * fp_Dst, unsigned char _FAR * fp_Src, unsigned int nbytes)
{
    unsigned int LoopCount;
    unsigned int itrLoopCount;
    unsigned int ret_val;

    if ( nbytes == 0 ) { ret_val = 0; }

    LoopCount = nbytes;
    for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
    {
        fp_Dst[itrLoopCount] = fp_Src[itrLoopCount];
    }

Done:
    return ret_val;
}

int ST_MoveData(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt, unsigned int nbytes)
{
    unsigned int LoopCount;
    unsigned int itrLoopCount;
    unsigned char _FAR *fptrDestByte;
    unsigned char _FAR *fptrSrcByte;
    unsigned int _FAR *fptrDestWord;
    unsigned int _FAR *fptrSrcWord;

    if ( nbytes == 0 ) { return ST_FAILURE; }
    if ( dst_sgmt == 0 ) { dst_sgmt = _DS; }
    if ( src_sgmt == 0 ) { src_sgmt = _DS; }

    if ( (nbytes & 0x01) == 1 )
    {
        fptrDestByte = (unsigned char _FAR *) MK_FP(dst_sgmt, dst_ofst);
        fptrSrcByte = (unsigned char _FAR *) MK_FP(src_sgmt, src_ofst);
        LoopCount = nbytes;
        for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        {
            fptrDestByte[itrLoopCount] = fptrSrcByte[itrLoopCount];
        }
    }
    else
    {
        fptrDestWord = (unsigned int _FAR *) MK_FP(dst_sgmt, dst_ofst);
        fptrSrcWord = (unsigned int _FAR *) MK_FP(src_sgmt, src_ofst);
        LoopCount = (nbytes / 2 );
        for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        {
            fptrDestWord[itrLoopCount] = fptrSrcWord[itrLoopCount];
        }

    }
    return ST_SUCCESS;
}
