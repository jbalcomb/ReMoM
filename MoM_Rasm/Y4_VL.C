
#include <DOS.H>

/* VGA ports */
#define SC_INDEX 0x3C4
#define SC_DATA  0x3C5
/* Sequence controller registers */
#define SC_MAPMASK 0x02

extern unsigned int gsa_DSP_Addr;

// #ifndef NOMAIN
// int main(void) { return(0); }
// #endif

void Y4_Set_Pixel(int x, int y, unsigned char color)
{
    outportb( SC_INDEX, SC_MAPMASK );
    outportb( SC_DATA, (1 << (x & 3)) );

    *(unsigned char _FAR *) MK_FP( gsa_DSP_Addr, ((y * 80) + (x / 4)) ) = color;
}
