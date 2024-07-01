#ifndef INIT_H
#define INIT_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD s014p01
// MoO2  Module: init  void Init_Drivers()
void Init_Drivers(int input_type, char * font_file);

// WZD s014p02
// MoO2  DNE
void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file);



#ifdef __cplusplus
}
#endif

#endif /* INIT_H */
