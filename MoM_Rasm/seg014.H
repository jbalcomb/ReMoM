#ifndef SEG014_H
#define SEG014_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg014
*/

// s14p01
// void Hardware_Init(int argInputType, int argSoundChannels, char *argFontFileName, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA);
void Hardware_Init(int argInputType, char *argFontFileName);

// s14p03
void VGA_DAC_Init(char *PaletteLbxFileName);

// s14p04
void VGA_SetDrawWindow(int Min_X, int Min_Y, int Max_X, int Max_Y);

// s14p05
void VGA_ResetDrawWindow(void);


#ifdef __cplusplus
}
#endif


#endif /* SEG014_H */
