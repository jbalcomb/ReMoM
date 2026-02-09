/*
    WIZARDS.EXE

DNE
WZD just fread()'s 18 bytes in _main()

MoO2
    Module: FILEDEF
    Module: INITGAME
*/

#ifndef CFG_H
#define CFG_H

#include "MOX_TYPE.h"



/*
NOTE(JimBalcomb,20250302):  Why do I have two config mom structures?
NOTE(JimBalcomb,20260208):  The file that comes out of INSTALL.EXE is 20 bytes, but MGC and WZD both definitely read 18 bytes into a structure that is sizeof 18 bytes
*/
//  ; (sizeof=0x12)
#pragma pack(push)

#pragma pack(2)

struct s_CONFIG_MOM_18
{
    /* 00 */ uint16_t MIDI_IO;
    /* 02 */ uint16_t midi_driver;
    /* 04 */ uint16_t MIDI_IRQ;
    /* 06 */ uint16_t DIGI_IO;
    /* 08 */ uint16_t DIGI_ID;
    /* 0A */ uint16_t DIGI_IRQ;
    /* 0C */ uint16_t DIGI_DMA;
    /* 0E */ uint16_t NIU_0E;
    /* 10 */ uint16_t NIU_10;
    /* 12 */
};

//  ; (sizeof=0x14)
struct s_CONFIG_MOM
{
    /* 00 */ uint16_t MIDI_IO;     // Sound Card Address / Music Card Address
    /* 02 */ uint16_t MIDI_ID;     // ID - Sound Card / Music Card
    /* 04 */ uint16_t MIDI_IRQ;    // Music IRQ
    /* 06 */ uint16_t DIGI_IO;     // Digital Card Address
    /* 08 */ uint16_t DIGI_ID;     // ID - Digital Card
    /* 0A */ uint16_t DIGI_IRQ;    // Digital IRQ
    /* 0C */ uint16_t DIGI_DMA;    // Digital DRQ
    /* 0E */ uint16_t NIU_0E;     //  ¿ MoO1 ?
    /* 10 */ uint16_t NIU_10;     //  ¿ MoO1 ?
    /* 12 */ uint16_t NIU_12;     //  ¿ MoO1 ?
    /* 14 */
};

#pragma pack(pop)



#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t _config_mom[sizeof(struct s_CONFIG_MOM)];

extern struct s_CONFIG_MOM config_mom;

extern uint16_t CFG_MDI_IO;     // Sound Card Address / Music Card Address
extern uint16_t CFG_MDI_ID;     // ID - Sound Card / Music Card
extern uint16_t CFG_MDI_IRQ;    // Music IRQ
extern uint16_t CFG_DIG_IO;     // Digital Card Address
extern uint16_t CFG_DIG_ID;     // ID - Digital Card
extern uint16_t CFG_DIG_IRQ;    // Digital IRQ
extern uint16_t CFG_DIG_DRQ;    // Digital DRQ
extern uint16_t CFG_NIU_0E;     // 
extern uint16_t CFG_NIU_10;     //
extern uint16_t CFG_NIU_12;     // 



void Load_CONFIG_MOM(void);

void Read_CONFIG_MOM(void);

void Write_CONFIG_MOM(void);



#ifdef __cplusplus
}
#endif

#endif  /* CFG_H */
