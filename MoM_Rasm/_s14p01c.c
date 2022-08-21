// _s14p01c.c Hardware_Init
// MOM_DEF.H

#include "ST_VGA.H"

void Hardware_Init(int argInputType, int argSoundChannels, char *argFontFileName, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
{
    int tmpInputType;

//    dlvfprintf("DEBUG: %s %d BEGIN: Hardware_Init()\n", __FILE__, __LINE__);

    // CRP_Empty_Exit_Fn2()

    EMM_Startup();

    VGA_SetModeY();

    if (argInputType == -1)
    {
        VGA_DAC_Init(DEFAULT_FONTS_FILE);
        //SND_Init(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
        tmpInputType = 1;
    }
    else
    {
        VGA_DAC_Init(argFontFileName);
        //SND_Init(M_Drv, argSoundChannels, M_IO, M_IRQ, M_DMA, D_Drv, D_IO, D_IRQ, D_DMA);
        tmpInputType = argInputType;
    }

    IN_Init(tmpInputType);  // _s34p65

    RNG_TimerSeed();

    VGA_Set_DSP_Addr();

//    dlvfprintf("DEBUG: %s %d END: Hardware_Init()\n", __FILE__, __LINE__);
}
