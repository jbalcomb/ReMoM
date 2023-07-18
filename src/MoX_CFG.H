#ifndef MOX_CFG_H
#define MOX_CFG_H

#include "MoX_TYPE.H"

struct s_CONFIG_MOM
{
    /*00*/ uint16_t MDI_IO;     // Sound Card Address / Music Card Address
    /*02*/ uint16_t MDI_ID;     // ID - Sound Card / Music Card
    /*04*/ uint16_t MDI_IRQ;    // Music IRQ
    /*06*/ uint16_t DIG_IO;     // Digital Card Address
    /*08*/ uint16_t DIG_ID;     // ID - Digital Card
    /*0A*/ uint16_t DIG_IRQ;    // Digital IRQ
    /*0C*/ uint16_t DIG_DRQ;    // Digital DRQ
    /*0E*/ uint16_t NIU_0E;     // 
    /*10*/ uint16_t NIU_10;     //
    /*12*/ uint16_t NIU_12;     // 
};



#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t _config_mom[20];

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

#endif  /* MOX_CFG_H */
