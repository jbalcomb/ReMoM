/**
 * PFL_Audio_Internal.h — Platform-local definitions for sdl2_Audio.c
 *
 * Copies of byte-manipulation macros, sound constants, and forward declarations
 * that sdl3_Audio.c needs. These originate in MoX headers (MOX_BITS.h, MOX_DEF.h,
 * SOUND.h) but are duplicated here to avoid a platform -> engine dependency.
 */

#ifndef PFL_AUDIO_INTERNAL_H
#define PFL_AUDIO_INTERNAL_H

#include <stdint.h>



/* ========================================================================= */
/*  Byte-manipulation macros (from MOX_BITS.h)                               */
/* ========================================================================= */

/** Read a little-endian 16-bit value from a byte pointer at a given offset. */
#define GET_2B_OFS(_ptr_,_ofs_) (                              \
  ( (uint16_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 0) )       )   \
| ( (uint16_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 1) ) <<  8 )   \
)

/** Read a big-endian 16-bit value from a byte pointer. */
#define GET_BE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[1])) | (((uint16_t)(((uint8_t const *)(_p_))[0])) << 8))

/** Read a big-endian 32-bit value from a byte pointer. */
#define GET_BE_32(_p_) (((uint32_t)GET_BE_16((_p_) + 2)) | (((uint32_t)GET_BE_16((_p_))) << 16))

/** Write a big-endian 32-bit value to a byte pointer. */
#define SET_BE_32(_p_,_v_) (((uint8_t *)(_p_))[3] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[0] = (((_v_) >> 24) & 0xffu))



/* ========================================================================= */
/*  Sound constants (from MOX_DEF.h, SOUND.h)                                */
/* ========================================================================= */

/** LBX sound entry magic signature (0xDEAF). */
#define SNDMAGSIG  0xDEAF

/** Audio error code for invalid file format. */
#define SND_INVALID_FILE  14



/* ========================================================================= */
/*  Forward declarations (from SOUND.h)                                      */
/* ========================================================================= */

/** Report an audio subsystem error.  Defined in MoX SOUND module. */
void Audio_Error__STUB(int16_t error);



#endif /* PFL_AUDIO_INTERNAL_H */
