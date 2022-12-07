#ifndef ST_CRSR_H
#define ST_CRSR_H

#include "MoX_TYPE.H"    /* word */


struct s_CONFIG_MOM
{
    /*00*/ word MDI_IO;     // Sound Card Address / Music Card Address
    /*02*/ word MDI_ID;     // ID - Sound Card / Music Card
    /*04*/ word MDI_IRQ;    // Music IRQ
    /*06*/ word DIG_IO;     // Digital Card Address
    /*08*/ word DIG_ID;     // ID - Digital Card
    /*0A*/ word DIG_IRQ;    // Digital IRQ
    /*0C*/ word DIG_DRQ;    // Digital DRQ
    /*0E*/ word NIU_0E;     // 
    /*10*/ word NIU_10;     //
    /*12*/ word NIU_12;     // 
};

struct s_CONFIG_MOM config_mom;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Load_CONFIG_MOM(void);


#ifdef __cplusplus
}
#endif

#endif /* ST_CRSR_H */
