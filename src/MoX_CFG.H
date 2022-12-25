#ifndef MOX_CFG_H
#define MOX_CFG_H

#include "MoX_TYPE.H"    /* word */

uint16_t CFG_MDI_IO;     // Sound Card Address / Music Card Address
uint16_t CFG_MDI_ID;     // ID - Sound Card / Music Card
uint16_t CFG_MDI_IRQ;    // Music IRQ
uint16_t CFG_DIG_IO;     // Digital Card Address
uint16_t CFG_DIG_ID;     // ID - Digital Card
uint16_t CFG_DIG_IRQ;    // Digital IRQ
uint16_t CFG_DIG_DRQ;    // Digital DRQ
uint16_t CFG_NIU_0E;     // 
uint16_t CFG_NIU_10;     //
uint16_t CFG_NIU_12;     // 

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

struct s_CONFIG_MOM config_mom;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Load_CONFIG_MOM(void);

void Read_CONFIG_MOM(void);
void Write_CONFIG_MOM(void);

#ifdef __cplusplus
}
#endif

#endif  /* MOX_CFG_H */
