/*
    Simulation Texas Universe
    ~MS-DOS Interrupts
    IBM-PC

*/
#ifndef STU_INT_H
#define STU_INT_H

#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif



uint16_t STU_INT(uint16_t interrupt, uint16_t _AX_);

uint16_t STU_INT_67(uint16_t _AX_);

uint16_t STU_INT_67_59(uint8_t _AL_)



#ifdef __cplusplus
}
#endif

#endif /* STU_INT_H */
