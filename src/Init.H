#ifndef INIT_H
#define INIT_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD s014p01
void Init_Drivers(int input_type, int sound_channels, char * font_file, int midi_driver, int MIDI_IO, int MIDI_IRQ, int MIDI_DMA, int digi_driver, int Digi_IO, int Digi_IRQ, int Digi_DMA);

// WZD s014p02
void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file);



#ifdef __cplusplus
}
#endif

#endif /* INIT_H */
