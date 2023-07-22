#ifndef PNG_DRAW_H
#define PNG_DRAW_H

#include "MoX_TYPE.H"



#define SZ_PNG_HDR                             16

#define PNG_HDR_POS_WIDTH                      0x00
#define PNG_HDR_POS_HEIGHT                     0x02

#define PNG_GET_WIDTH(_ptr_)                    ( GET_2B_OFS( (_ptr_), PNG_HDR_POS_WIDTH)                   )
#define PNG_GET_HEIGHT(_ptr_)                   ( GET_2B_OFS( (_ptr_), PNG_HDR_POS_HEIGHT)                  )

#define PNG_SET_WIDTH(_ptr_,_width_)                ( SET_2B_OFS( (_ptr_), PNG_HDR_POS_WIDTH,         (_width_)       ) )
#define PNG_SET_HEIGHT(_ptr_,_height_)              ( SET_2B_OFS( (_ptr_), PNG_HDR_POS_HEIGHT,        (_height_)      ) )

// sizeof:  10h  16d
struct s_PNG_PICT
{
    /* 0x00 */ uint32_t png_width;
    /* 0x04 */ uint32_t png_height;
    /* 0x08 */ uint8_t * png_pixels;  // assume sizeof 64-bit pointer
    /* 0x10 */
};



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



void PNG_Draw(int start_x, int start_y, struct s_PNG_PICT* png_pict);



#ifdef __cplusplus
}
#endif

#endif  /* PNG_DRAW_H */
